/*******************************************************************************
 * @file	addressClaimed.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "addressClaimed.h"
#include "messageSend.h"

/******************************************************************************/
enum
{
    NULL_ADDRESS = 254u,
    ACL_PGN = 60928u
};
static struct addressClaimed
{
    j1939_t stack;
    canDriver_t driver;
    bool_t wasAddressClaimed;
    bool_t wasRequestForACLReceived;
    bool_t wasMessageWithOwnSAReceived;
    bool_t wasContentionReceived;
    uint8_t* caName;
    uint8_t tickMs;
    uint8_t newSourceAddress;
    uint8_t contentionCounterMs;
    uint8_t pseudoDelay;
    int8_t state;
    uint8_t contenderName[ 8 ];
}acl;

/******************************************************************************/
static uint8_t sendACLMessage( void )
{
    uint8_t status;
	j1939Message_t message = ( j1939Message_t ) malloc( sizeof( j1939MessageStruct_t ) );
	message->parameterGroupNumber = ACL_PGN;
    message->sourceAddress = getJ1939SourceAddress( acl.stack );
    message->destinationAddress = GLOBAL_ADDRESS;
	message->priority = 6u;
	message->data = acl.caName;
	message->dataSize = 8u;
	status = sendJ1939MessageToCANDriver( message, acl.driver );
	free( message );
	return ( status );
}

static uint8_t generateNewDelay( const uint8_t* name, uint8_t tickMs )
{
    uint16_t result = 0u;
    for ( uint8_t i = 0u; i < 8u; i++ )
    {
        result += name[ i ];
    }
    result %= 255u;
    result *= 6u;
    result = ( uint16_t ) ( result / tickMs );

    return ( ( uint8_t ) result );
}

static bool_t doesContenderHaveHigherPriority( void )
{
    uint8_t i;
    for ( i = 7u; ( ( acl.caName[ i ] == acl.contenderName[ i ] ) && ( i > 0u ) ); --i ) { }
    return ( acl.contenderName[ i ] <= acl.caName[ i ] );
}

static uint8_t getNewSourceAddress( void )
{
	acl.newSourceAddress++;
	if ( acl.newSourceAddress > 247u )
	{
		acl.newSourceAddress = NULL_ADDRESS;
	}
	return ( acl.newSourceAddress );
}

static void handleWaitForContention( void )
{
    if ( isCANTxBusOffState( acl.driver ) )
    {
        acl.contentionCounterMs = acl.tickMs;
        acl.pseudoDelay = generateNewDelay( acl.caName, acl.tickMs );
        acl.state = DELAY_BEFORE_RECLAIM;
    }
    else if ( acl.wasContentionReceived )
    {
        acl.contentionCounterMs = acl.tickMs;
        if ( doesContenderHaveHigherPriority( ) )
        {
            uint8_t sourceAddress = getNewSourceAddress( );
		    setJ1939SourceAddress( acl.stack, sourceAddress );
        }
        if ( sendACLMessage( ) == CAN_TX_SUCCEEDED )
        {
            acl.wasContentionReceived = false;
        }
    }
    
    else if ( acl.contentionCounterMs == ( uint8_t ) CONTENTION_TIMEOUT_MS )
	{
		acl.contentionCounterMs = acl.tickMs;
		acl.wasAddressClaimed = true;
		acl.state = NORMAL_TRAFFIC;
	}
	else
	{
		acl.contentionCounterMs += acl.tickMs;
	}
}

static void handleNormalTraffic( void )
{
    if ( acl.wasRequestForACLReceived
        || acl.wasMessageWithOwnSAReceived )
    {
        if ( sendACLMessage( ) == CAN_TX_SUCCEEDED )
        {
            acl.wasRequestForACLReceived = false;
            acl.wasMessageWithOwnSAReceived = false;
        }
    }
}

static void handleDelayAndTransitionTo( uint8_t transitionState )
{
	if ( acl.pseudoDelay < acl.tickMs )
	{
		if ( sendACLMessage( ) == CAN_TX_SUCCEEDED )
		{
			acl.state = transitionState;
		}
	}
	else
	{
		acl.pseudoDelay -= acl.tickMs;
	}
}

/******************************************************************************/
void configureAddressClaimed( j1939_t stack, int8_t state )
{    
    if ( NULL != stack )
    {
        acl.stack = stack;
        acl.driver = getJ1939ConfiguredCANDriver( stack );
        acl.caName = getJ1939CAName( stack );
        acl.tickMs = getJ1939ConfiguredTickMs( stack );
        acl.state = state;
    }
    else
    {
        acl.state = UNDEFINED;
    }
    acl.contentionCounterMs = acl.tickMs;
    acl.wasAddressClaimed = false;
    acl.wasRequestForACLReceived = false;
    acl.wasMessageWithOwnSAReceived = false;
    acl.pseudoDelay = 0u;
    acl.wasContentionReceived = false;
    acl.newSourceAddress = 127u;
}


void updateAddressClaimed( void )
{
    switch ( acl.state )
    {
        case WAIT_FOR_CONTENTION:
        {
            handleWaitForContention( );
            break;
        }
        case DELAY_BEFORE_RECLAIM:
        {
            handleDelayAndTransitionTo( WAIT_FOR_CONTENTION );
            break;
        }
        case NORMAL_TRAFFIC:
        {
            handleNormalTraffic( );
            break;
        }
        case INIT:
        {
            if ( sendACLMessage( ) == CAN_TX_SUCCEEDED )
            {
                acl.state = WAIT_FOR_CONTENTION;
            }
            break;
        }
        case UNDEFINED:
        default:
        {
            break;
        }
    }
}

bool_t wasAddressClaimedSuccessfuly( void )
{
    return ( acl.wasAddressClaimed );
}

void registerRequestForAddressClaim( void )
{
    acl.wasRequestForACLReceived = true;
}

void registerReceivedMessageWithOwnSA( void )
{
    acl.wasMessageWithOwnSAReceived = true;
}

void registerReceivedContention( const uint8_t* caName )
{
    for ( uint8_t i = 0u; i < 8u; i++ )
    {
        acl.contenderName[ i ] = caName[ i ];
    }
    acl.wasContentionReceived = true;
}

