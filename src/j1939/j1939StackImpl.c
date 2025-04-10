/*******************************************************************************
 * @file	j1939StackImpl.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "j1939StackImpl.h"

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
    free( stack );
}

static uint8_t sendMessage( j1939_t base, const j1939Message_t message )
{
    j1939StackImpl_t stack = ( j1939StackImpl_t ) base;

    uint8_t status = sendJ1939MessageToDriver( message, stack->driver );
    return ( status );
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
    setCAName( self->acl, caName );
}

static uint8_t* getName( j1939_t super )
{
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;
    return ( getCAName( self->acl ) );
}

static uint32_t getRequestedPGNFromData( const uint8_t* data )
{
    uint32_t requestedPgn = data[ 0 ];
    requestedPgn |= ( uint32_t ) ( data[ 1 ] << 8UL );
    requestedPgn |= ( uint32_t ) ( data[ 2 ] << 16UL );
    return ( requestedPgn );
}

static void addressClaimHandler( j1939Message_t message, acl_t acl )
{
    if ( getJ1939MessagePGN( message ) == J1939_RQST )
    {
        if ( ( uint32_t ) J1939_AC == getRequestedPGNFromData( getJ1939MessageData( message ) ) )
        {
            registerRequestForACL( acl );
        }
    }
}

static void updateCoreScheduler( j1939_t super )
{
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;

    j1939Message_t message = receiveJ1939MessageFromDriver( self->driver );

    while ( NULL != message )
    {
        addressClaimHandler( message, self->acl );
        destroyJ1939Message( message );
        message = receiveJ1939MessageFromDriver( self->driver );
    }
    updateACLStateMachine( self->acl ); 
}

static bool_t wasAddressClaimed( j1939_t super )
{
    j1939StackImpl_t self = ( j1939StackImpl_t ) super;
    return  ( wasACLSuccessful( self->acl ) );
}

/******************************************************************************/
j1939_t createJ1939StackImpl( acl_t acl, canDriver_t driver )
{
    j1939StackImpl_t self = NULL;

    if ( acl && driver )
    {
        static j1939InterfaceStruct_t interface =
        {
            destroy,
            sendMessage,
            setSA,
            getSA,
            setName,
            getName,
            updateCoreScheduler,
            wasAddressClaimed
        };

        self = ( j1939StackImpl_t ) malloc( sizeof( j1939StackInstanceStruct_t ) );
        self->base.iFace = &interface;
        self->base.type = "J1939Stack";
        self->driver = driver;
        self->acl = acl;
    }

    return ( ( j1939_t ) self );
}

