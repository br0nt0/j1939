/*******************************************************************************
 * @file	j1939StackInstance.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "j1939StackInstance.h"
#include "j1939/messageSend.h"
#include "j1939/messageReceive.h"
#include "j1939/addressClaimedImpl.h"

/******************************************************************************/
typedef struct j1939StackInstanceStruct* j1939StackInstance_t;

typedef struct j1939StackInstanceStruct
{
    j1939Struct_t base;
    canDriver_t driver;
    acl_t acl;
    uint8_t caNameStorage[ 8 ];
    uint8_t tickMs;
}j1939StackInstanceStruct_t;

/******************************************************************************/
static void destroy( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    destroyACL( stack->acl );
    free( stack );
}

static uint8_t sendMessage( j1939_t base, const j1939Message_t message )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;

    uint8_t status = sendJ1939MessageToCANDriver( message, stack->driver );
    return ( status );
}

static j1939Message_t receiveMessage( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;

    j1939Message_t message = receiveJ1939MessageFromCANDriver( stack->driver );

    return ( message );
}

static void setSA( j1939_t super, uint8_t address )
{
    j1939StackInstance_t self = ( j1939StackInstance_t ) super;
    setSourceAddress( self->acl, address );
}

static uint8_t getSA( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    return ( getSourceAddress( stack->acl ) );
}

static void setName( j1939_t super, const uint8_t* caName )
{
    j1939StackInstance_t self = ( j1939StackInstance_t ) super;
    if ( caName != NULL )
    {
        for ( uint8_t i = 0u; i < 8u; i++ )
        {
            self->caNameStorage[ i ] = caName[ i ];
        }
        setCAName( self->acl, self->caNameStorage );
    }
    else
    {
        setCAName( self->acl, NULL );
    }
}

static uint8_t* getName( j1939_t super )
{
    j1939StackInstance_t self = ( j1939StackInstance_t ) super;
    return ( getCAName( self->acl ) );
}

static uint8_t getConfiguredTickMs( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    return ( stack->tickMs );
}

static void updateCoreScheduler( j1939_t super )
{
    j1939StackInstance_t self = ( j1939StackInstance_t ) super;

    j1939Message_t message = receiveJ1939MessageFromCANDriver( self->driver );

    while ( NULL != message )
    {
        message = receiveJ1939MessageFromCANDriver( self->driver );
    }
    updateACLStateMachine( self->acl );
}

static bool_t wasAddressClaimed( j1939_t super )
{
    j1939StackInstance_t self = ( j1939StackInstance_t ) super;
    return  ( wasACLSuccessful( self->acl ) );
}

/******************************************************************************/
j1939_t createJ1939StackInstance( canDriver_t driver, uint8_t tickMs, uint8_t* caName )
{
    j1939StackInstance_t self = NULL;

    if ( ( NULL != driver ) && ( tickMs > 0u ) && ( NULL != caName ) )
    {
        static j1939InterfaceStruct_t interface =
        {
            destroy,
            sendMessage,
            receiveMessage,
            setSA,
            getSA,
            setName,
            getName,
            getConfiguredTickMs,
            updateCoreScheduler,
            wasAddressClaimed
        };

        self = ( j1939StackInstance_t ) malloc( sizeof( j1939StackInstanceStruct_t ) );
        self->base.iFace = &interface;
        self->base.type = "J1939Stack";
        self->driver = driver;
        self->tickMs = tickMs;
        self->acl = createAddressClaimed( driver, tickMs, caName, 0xffu );
    }

    return ( ( j1939_t ) self );
}

