/*******************************************************************************
 * @file	addressClaimed.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "addressClaimed.h"
#include "messageSend.h"
#include "stdlib.h"

/******************************************************************************/
static struct addressClaimed
{
    aclConfigStruct_t* config;
    bool_t wasAddressClaimed;
    bool_t wasRequestForACLReceived;
    bool_t wasMessageWithOwnSAReceived;
    bool_t wasContentionReceived;
    uint8_t contentionCounterMs;
    uint8_t pseudoDelay;
    int8_t state;
    uint8_t contenderName[ 8 ];
}acl;

/******************************************************************************/
static bool_t isConfigValid( aclConfigStruct_t* config )
{
    return ( ( config->caName != NULL )
            && ( config->driver != NULL )
            && ( config->tickMs > 0u ) );
}

static uint8_t sendACLMessage( void )
{
    uint8_t status;
	j1939Message_t message = ( j1939Message_t ) malloc( sizeof( j1939MessageStruct_t ) );
	message->parameterGroupNumber = 60928u;
	message->sourceAddress = acl.config->sourceAddress;
	message->destinationAddress = GLOBAL_ADDRESS;
	message->priority = 6u;
	message->data = acl.config->caName;
	message->dataSize = 8u;
	status = sendJ1939MessageToCANDriver( message, acl.config->driver );
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

static void handleWaitForContention( void )
{
    if ( isCANTxBusOffState( acl.config->driver ) )
    {
        acl.contentionCounterMs = acl.config->tickMs;
        acl.pseudoDelay = generateNewDelay( acl.config->caName, acl.config->tickMs );
        acl.state = DELAY_BEFORE_RECLAIM;
    }
    else if ( acl.wasContentionReceived )
    {
        acl.contentionCounterMs = acl.config->tickMs;
        if ( sendACLMessage( ) == CAN_TX_SUCCEEDED )
        {
            acl.wasContentionReceived = false;
        }
    }
    
    else if ( acl.contentionCounterMs == ( uint8_t ) CONTENTION_TIMEOUT_MS )
	{
		acl.contentionCounterMs = acl.config->tickMs;
		acl.wasAddressClaimed = true;
		acl.state = NORMAL_TRAFFIC;
	}
	else
	{
		acl.contentionCounterMs += acl.config->tickMs;
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
	if ( acl.pseudoDelay < acl.config->tickMs )
	{
		if ( sendACLMessage( ) == CAN_TX_SUCCEEDED )
		{
			acl.state = transitionState;
		}
	}
	else
	{
		acl.pseudoDelay -= acl.config->tickMs;
	}
}

/******************************************************************************/
void configureAddressClaimed( aclConfigStruct_t* configuration )
{
    acl.config = configuration;
    if ( isConfigValid( acl.config ) )
    {
        acl.state = acl.config->initialState;
    }
    else
    {
        acl.state = UNDEFINED;
    }
    acl.contentionCounterMs = acl.config->tickMs;
    acl.wasAddressClaimed = false;
    acl.wasRequestForACLReceived = false;
    acl.wasMessageWithOwnSAReceived = false;
    acl.pseudoDelay = 0u;
    acl.wasContentionReceived = false;
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

