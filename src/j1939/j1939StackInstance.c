/*******************************************************************************
 * @file	j1939StackInstance.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "j1939StackInstance.h"
#include "j1939/messageSend.h"

/******************************************************************************/
typedef struct j1939StackInstanceStruct* j1939StackInstance_t;

typedef struct j1939StackInstanceStruct
{
    j1939Struct_t base;
    canDriver_t driver;
}j1939StackInstanceStruct_t;

/******************************************************************************/
static void destroy( j1939_t base )
{
    ( void ) base;
}

static uint8_t sendMessage( j1939_t base, const j1939Message_t message )
{
    j1939StackInstance_t stack = ( j1939StackInstance_t ) base;
    
    uint8_t status = sendJ1939MessageToCANDriver( message, stack->driver );
    return ( status );
}

/******************************************************************************/
j1939_t createJ1939StackInstance( canDriver_t driver )
{
    static j1939InterfaceStruct_t interface =
    {
        destroy,
        sendMessage
    };
    static j1939StackInstanceStruct_t stackMemory;

    j1939StackInstance_t self = &stackMemory;
    self->base.iFace = &interface;
    self->base.type = "J1939Stack";
    self->driver = driver;
    return ( ( j1939_t ) self );
}

