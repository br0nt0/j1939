/*******************************************************************************
 * @file	j1939StackImpl.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "j1939StackImpl.h"
#include "j1939/addressClaimedImpl.h"

/******************************************************************************/
typedef struct j1939StackImplStruct* j1939StackImpl_t;

typedef struct j1939StackImplStruct
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
    j1939StackImpl_t stack = ( j1939StackImpl_t ) base;
    destroyACL( stack->acl );
    free( stack );
}

static uint8_t sendMessage( j1939_t base, const j1939Message_t message )
{
    j1939StackImpl_t stack = ( j1939StackImpl_t ) base;

    uint8_t status = sendJ1939MessageToDriver( message, stack->driver );
    return ( status );
}

static j1939Message_t receiveMessage( j1939_t base )
{
    j1939StackImpl_t stack = ( j1939StackImpl_t ) base;

    j1939Message_t message = receiveJ1939MessageFromDriver( stack->driver );

    return ( message );
}

static void setSA( j1939_t super, uint8_t address )
{
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;
    setACLSourceAddress( self->acl, address );
}

static uint8_t getSA( j1939_t base )
{
    j1939StackImpl_t stack = ( j1939StackImpl_t ) base;
    return ( getACLSourceAddress( stack->acl ) );
}

static void setName( j1939_t super, const uint8_t* caName )
{
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;
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
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;
    return ( getCAName( self->acl ) );
}

static uint8_t getConfiguredTickMs( j1939_t base )
{
    j1939StackImpl_t stack = ( j1939StackImpl_t ) base;
    return ( stack->tickMs );
}

static void updateCoreScheduler( j1939_t super )
{
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;

    j1939Message_t message = receiveJ1939MessageFromDriver( self->driver );
    destroyJ1939Message( message );

    while ( NULL != message )
    {
        message = receiveJ1939MessageFromDriver( self->driver );
        destroyJ1939Message( message );
    }
    updateACLStateMachine( self->acl ); 
}

static bool_t wasAddressClaimed( j1939_t super )
{
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;
    return  ( wasACLSuccessful( self->acl ) );
}

/******************************************************************************/
j1939_t createJ1939StackImpl( canDriver_t driver, uint8_t tickMs, uint8_t* caName )
{
    j1939StackImpl_t self = NULL;

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

        self = ( j1939StackImpl_t ) malloc( sizeof( j1939StackInstanceStruct_t ) );
        self->base.iFace = &interface;
        self->base.type = "J1939Stack";
        self->driver = driver;
        self->tickMs = tickMs;
        self->acl = createAddressClaimed( driver, tickMs, caName, 0xffu );
    }

    return ( ( j1939_t ) self );
}

