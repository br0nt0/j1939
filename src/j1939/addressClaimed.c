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
    uint8_t contentionCounterMs;
    int8_t state;
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

static void handleWaitForContention( void )
{
    if ( isCANTxBusOffState( acl.config->driver ) )
    {
        
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
        case NORMAL_TRAFFIC:
        {
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
    return acl.wasAddressClaimed;
}