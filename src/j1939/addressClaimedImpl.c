/*******************************************************************************
 * @file	addressClaimedImpl.c
 * @brief
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "addressClaimedImpl.h"
#include "messageSend.h"

/******************************************************************************/
typedef struct aclImplStruct* aclImpl_t;

struct aclImplStruct
{
    aclStruct_t base;
    canDriver_t driver;
	bool_t wasAddressClaimed;
	bool_t wasRequestForACLReceived;
	bool_t wasMessageWithOwnSAReceived;
	bool_t wasContentionReceived;
	uint8_t sourceAddress;
	uint8_t newSourceAddress;
	uint8_t contentionCounterMs;
	uint8_t pseudoDelay;
	uint8_t tickMs;
	int8_t state;
	uint8_t* caName;
	uint8_t contenderName[ 8 ];
};

/******************************************************************************/
inline static uint8_t sendACLMessage( aclImpl_t aclImpl )
{
    uint8_t status;
    j1939Message_t message = ( j1939Message_t ) malloc( sizeof( j1939MessageStruct_t ) );
    message->parameterGroupNumber = ACL_PGN;
    message->sourceAddress = aclImpl->sourceAddress;
    message->destinationAddress = GLOBAL_ADDRESS;
    message->priority = 6u;
    message->data = aclImpl->caName;
    message->dataSize = 8u;
    status = sendJ1939MessageToCANDriver( message, aclImpl->driver );
    free( message );
    return ( status );
}

inline static bool_t doesContenderHaveHigherPriority( aclImpl_t aclImpl )
{
    uint8_t i;
    for ( i = 7u; ( ( aclImpl->caName[ i ] == aclImpl->contenderName[ i ] ) && ( i > 0u ) ); --i ) { }
    return ( aclImpl->contenderName[ i ] <= aclImpl->caName[ i ] );
}

inline static uint8_t getNewSourceAddress( aclImpl_t aclImpl )
{
    aclImpl->newSourceAddress++;
    if ( aclImpl->newSourceAddress > 247u )
    {
        aclImpl->newSourceAddress = NULL_ADDRESS;
    }
    return ( aclImpl->newSourceAddress );
}

inline static void prioritizeContention( aclImpl_t aclImpl )
{
    if ( doesContenderHaveHigherPriority( aclImpl ) )
    {
        aclImpl->sourceAddress = getNewSourceAddress( aclImpl );
        if ( aclImpl->sourceAddress == NULL_ADDRESS )
        {
            aclImpl->state = CANNOT_CLAIM_ADDRESS;
        }
    }
    if ( sendACLMessage( aclImpl ) == CAN_TX_SUCCEEDED )
    {
        aclImpl->wasContentionReceived = false;
    }
}

inline static uint8_t generateDelayBasedOnCAName( const uint8_t* name, uint8_t tickMs )
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

inline static void handleWaitForContention( aclImpl_t aclImpl )
{
    if ( isCANTxBusOffState( aclImpl->driver ) )
    {
        aclImpl->contentionCounterMs = aclImpl->tickMs;
        aclImpl->pseudoDelay = generateDelayBasedOnCAName( aclImpl->caName, aclImpl->tickMs );
        aclImpl->state = DELAY_BEFORE_RECLAIM;
    }
    else if ( aclImpl->wasContentionReceived )
    {
        aclImpl->contentionCounterMs = aclImpl->tickMs;
        prioritizeContention( aclImpl );
    }

    else if ( aclImpl->contentionCounterMs == ( uint8_t ) CONTENTION_TIMEOUT_MS )
    {
        aclImpl->contentionCounterMs = aclImpl->tickMs;
        aclImpl->wasAddressClaimed = true;
        aclImpl->state = NORMAL_TRAFFIC;
    }
    else
    {
        aclImpl->contentionCounterMs += aclImpl->tickMs;
    }
}

inline static void handleDelayAndTransitionTo( aclImpl_t aclImpl, uint8_t transitionState )
{
    if ( aclImpl->pseudoDelay < aclImpl->tickMs )
    {
        if ( sendACLMessage( aclImpl ) == CAN_TX_SUCCEEDED )
        {
            aclImpl->state = transitionState;
        }
    }
    else
    {
        aclImpl->pseudoDelay -= aclImpl->tickMs;
    }
}

inline static void handleCannotClaimAddress( aclImpl_t aclImpl )
{
    if ( aclImpl->wasRequestForACLReceived )
    {
        aclImpl->pseudoDelay = generateDelayBasedOnCAName( aclImpl->caName, aclImpl->tickMs );
        aclImpl->wasRequestForACLReceived = false;
        aclImpl->state = DELAY_BEFORE_CANNOT_CLAIM;
    }
}

inline static void handleNormalTraffic( aclImpl_t aclImpl )
{
    if ( aclImpl->wasRequestForACLReceived
        || aclImpl->wasMessageWithOwnSAReceived )
    {
        if ( sendACLMessage( aclImpl ) == CAN_TX_SUCCEEDED )
        {
            aclImpl->wasRequestForACLReceived = false;
            aclImpl->wasMessageWithOwnSAReceived = false;
        }
    }
    else if ( aclImpl->wasContentionReceived )
    {
        prioritizeContention( aclImpl );
    }
    else
    {
        /* nothing */
    }

}

/******************************************************************************/
static void updateStateMachine( acl_t super )
{
    aclImpl_t self = ( aclImpl_t ) super;

    switch ( self->state )
    {
        case WAIT_FOR_CONTENTION:
        {
            handleWaitForContention( self );
            break;
        }
        case DELAY_BEFORE_RECLAIM:
        {
            handleDelayAndTransitionTo( self, WAIT_FOR_CONTENTION );
            break;
        }
        case CANNOT_CLAIM_ADDRESS:
        {
            handleCannotClaimAddress( self );
            break;
        }
        case DELAY_BEFORE_CANNOT_CLAIM:
        {
            handleDelayAndTransitionTo( self, CANNOT_CLAIM_ADDRESS );
            break;
        }
        case NORMAL_TRAFFIC:
        {
            handleNormalTraffic( self );
            break;
        }
        case INIT:
        default:
        {
            if ( sendACLMessage( self ) == CAN_TX_SUCCEEDED )
            {
                self->state = WAIT_FOR_CONTENTION;
            }
            break;
        }
    }
}

static uint8_t* getName( acl_t super )
{
    aclImpl_t self = ( aclImpl_t ) super;
    return ( self->caName );
}

static uint8_t getSA( acl_t super )
{
    aclImpl_t self = ( aclImpl_t ) super;
    return ( self->sourceAddress );
}


/******************************************************************************/
acl_t createAddressClaimed( canDriver_t driver, uint16_t ticksMS, uint8_t* caName, uint8_t sourceAddress )
{
    aclImpl_t self = NULL;

    static aclInterfaceStruct_t interface = {
        NULL,
        updateStateMachine,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        getName,
        NULL,
        getSA
    };

    if ( driver && ( ticksMS > 0u ) && caName )
    {
        self = ( aclImpl_t ) malloc( sizeof( struct aclImplStruct ) );
        self->base.iFace = &interface;
        self->base.type = "ACLv1";
        self->driver = driver;
        self->tickMs = ticksMS;
        self->caName = caName;
        self->state = INIT;
        self->contentionCounterMs = self->tickMs;
		self->wasAddressClaimed = false;
		self->wasRequestForACLReceived = false;
		self->pseudoDelay = 0u;
		self->wasContentionReceived = false;
		self->newSourceAddress = 127u;
		self->wasMessageWithOwnSAReceived = false;
        self->sourceAddress = sourceAddress;
    }

    return ( ( acl_t ) self );
}

void setACLStateMachineState( acl_t acl, int8_t state )
{
    aclImpl_t self = ( aclImpl_t ) acl;
    self->state = state;
}
