/*******************************************************************************
 * @file	j1939StackInstance.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "j1939StackInstance.h"
#include "j1939/messageSend.h"
#include "j1939/messageReceive.h"
#include "j1939/addressClaimed.h"

/******************************************************************************/
typedef struct j1939StackInstanceStruct* j1939StackInstance_t;

typedef struct j1939StackInstanceStruct
{
    j1939Struct_t base;
    canDriver_t driver;
    aclStruct_t acl;
    uint8_t caNameStorage[ 8 ];
    uint8_t tickMs;
}j1939StackInstanceStruct_t;

/******************************************************************************/
static void destroy( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
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

static void setSourceAddress( j1939_t base, uint8_t address )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    stack->acl.sourceAddress = address;
}

static uint8_t getSourceAddress( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    return ( stack->acl.sourceAddress );
}

static void setCAName( j1939_t base, const uint8_t* caName )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    if ( caName != NULL )
    {
        for ( uint8_t i = 0u; i < 8u; i++ )
        {
            stack->caNameStorage[ i ] = caName[ i ];
        }
        stack->acl.caName = stack->caNameStorage;
    }
    else
    {
        stack->acl.caName = NULL;
    }
}

static uint8_t* getCAName( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    return ( stack->acl.caName );
}

static uint8_t getConfiguredTickMs( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    return ( stack->tickMs );
}

static void updateCoreScheduler( j1939_t base )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    updateAddressClaimed( &stack->acl );
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
            setSourceAddress,
            getSourceAddress,
            setCAName,
            getCAName,
            getConfiguredTickMs,
            updateCoreScheduler
        };

        self = ( j1939StackInstance_t ) malloc( sizeof( j1939StackInstanceStruct_t ) );
        self->base.iFace = &interface;
        self->base.type = "J1939Stack";
        self->driver = driver;
        self->acl.sourceAddress = 0xffu;
        self->acl.caName = caName;
        self->tickMs = tickMs;
        self->acl.tickMs = tickMs;
        configureAddressClaim( &self->acl, self->driver, INIT );
    }

    return ( ( j1939_t ) self );
}

