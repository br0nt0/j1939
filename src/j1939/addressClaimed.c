/*******************************************************************************
 * @file	addressClaimed.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "addressClaimed.h"
#include "messageSend.h"

/******************************************************************************/

/******************************************************************************/
inline static uint8_t sendACLMessage( acl_t acl )
{
	uint8_t status;
	j1939Message_t message = ( j1939Message_t ) malloc( sizeof( j1939MessageStruct_t ) );
	message->parameterGroupNumber = ACL_PGN;
	message->sourceAddress = acl->sourceAddress;
	message->destinationAddress = GLOBAL_ADDRESS;
	message->priority = 6u;
	message->data = acl->caName;
	message->dataSize = 8u;
	status = sendJ1939MessageToCANDriver( message, acl->driver );
	free( message );
	return ( status );
}

inline static bool_t doesContenderHaveHigherPriority( acl_t acl )
{
	uint8_t i;
	for ( i = 7u; ( ( acl->caName[ i ] == acl->contenderName[ i ] ) && ( i > 0u ) ); --i ) { }
	return ( acl->contenderName[ i ] <= acl->caName[ i ] );
}

inline static uint8_t getNewSourceAddress( acl_t acl )
{
	acl->newSourceAddress++;
	if ( acl->newSourceAddress > 247u )
	{
		acl->newSourceAddress = NULL_ADDRESS;
	}
	return ( acl->newSourceAddress );
}

inline static void prioritizeContention( acl_t acl )
{
	if ( doesContenderHaveHigherPriority( acl ) )
	{
		acl->sourceAddress = getNewSourceAddress( acl );
		if ( acl->sourceAddress == NULL_ADDRESS )
		{
			acl->state = CANNOT_CLAIM_ADDRESS;
		}
	}
	if ( sendACLMessage( acl ) == CAN_TX_SUCCEEDED )
	{
		acl->wasContentionReceived = false;
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

inline static void handleWaitForContention( acl_t acl )
{
    if ( isCANTxBusOffState( acl->driver ) )
    {
        acl->contentionCounterMs = acl->tickMs;
        acl->pseudoDelay = generateDelayBasedOnCAName( acl->caName, acl->tickMs );
        acl->state = DELAY_BEFORE_RECLAIM;
    }
    else if ( acl->wasContentionReceived )
    {
        acl->contentionCounterMs = acl->tickMs;
        prioritizeContention( acl );
    }

    else if ( acl->contentionCounterMs == ( uint8_t ) CONTENTION_TIMEOUT_MS )
    {
        acl->contentionCounterMs = acl->tickMs;
        acl->wasAddressClaimed = true;
        acl->state = NORMAL_TRAFFIC;
    }
    else
    {
        acl->contentionCounterMs += acl->tickMs;
    }
}

inline static void handleDelayAndTransitionTo( acl_t acl, uint8_t transitionState )
{
	if ( acl->pseudoDelay < acl->tickMs )
	{
		if ( sendACLMessage( acl ) == CAN_TX_SUCCEEDED )
		{
			acl->state = transitionState;
		}
	}
	else
	{
		acl->pseudoDelay -= acl->tickMs;
	}
}

inline static void handleCannotClaimAddress( acl_t acl )
{
	if ( acl->wasRequestForACLReceived )
	{
		acl->pseudoDelay = generateDelayBasedOnCAName( acl->caName, acl->tickMs );
		acl->wasRequestForACLReceived = false;
		acl->state = DELAY_BEFORE_CANNOT_CLAIM;
	}
}

inline static void handleNormalTraffic( acl_t acl )
{
	if ( acl->wasRequestForACLReceived
		|| acl->wasMessageWithOwnSAReceived )
	{
		if ( sendACLMessage( acl ) == CAN_TX_SUCCEEDED )
		{
			acl->wasRequestForACLReceived = false;
			acl->wasMessageWithOwnSAReceived = false;
		}
	}
	else if ( acl->wasContentionReceived )
	{
		prioritizeContention( acl );
	}
	
}

static bool_t areNameAndTicksValid( uint8_t* caName, uint8_t tickMs )
{
	return ( ( caName != NULL ) && ( tickMs > 0u ) );
}

/******************************************************************************/
void setAddressClaimInitialState( acl_t acl, int8_t state )
{
	if ( acl && acl->driver )
	{
		if ( areNameAndTicksValid( acl->caName, acl->tickMs ) )
		{
			acl->state = state;
		}
		else
		{
			acl->state = UNDEFINED;
		}
		acl->contentionCounterMs = acl->tickMs;
		acl->wasAddressClaimed = false;
		acl->wasRequestForACLReceived = false;
		acl->pseudoDelay = 0u;
		acl->wasContentionReceived = false;
		acl->newSourceAddress = 127u;
		acl->wasMessageWithOwnSAReceived = false;
	}
}

void updateAddressClaimed( acl_t acl )
{
	if ( NULL != acl )	
{
		switch ( acl->state )
		{
			case WAIT_FOR_CONTENTION:
			{
				handleWaitForContention( acl );
				break;
			}
			case DELAY_BEFORE_RECLAIM:
			{
				handleDelayAndTransitionTo( acl, WAIT_FOR_CONTENTION );
				break;
			}
			case CANNOT_CLAIM_ADDRESS:
			{
				handleCannotClaimAddress( acl );
				break;
			}
			case DELAY_BEFORE_CANNOT_CLAIM:
			{
				handleDelayAndTransitionTo( acl, CANNOT_CLAIM_ADDRESS );
				break;
			}
			case NORMAL_TRAFFIC:
			{
				handleNormalTraffic( acl );
				break;
			}
			case INIT:
			{
				if ( sendACLMessage( acl ) == CAN_TX_SUCCEEDED )
				{
					acl->state = WAIT_FOR_CONTENTION;
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
}

bool_t wasAddressClaimedSuccessfuly( acl_t acl )
{
	return ( acl->wasAddressClaimed );
}

void registerRequestForAddressClaim( acl_t acl )
{
	acl->wasRequestForACLReceived = true;
}

void registerReceivedMessageWithOwnSA( acl_t acl )
{
	acl->wasMessageWithOwnSAReceived = true;
}

void registerReceivedContention( acl_t acl, const uint8_t* caName )
{
	for ( uint8_t i = 0u; i < 8u; i++ )
	{
		acl->contenderName[ i ] = caName[ i ];
	}
	acl->wasContentionReceived = true;
}

