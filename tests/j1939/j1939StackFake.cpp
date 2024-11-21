/*******************************************************************************
 * @file	j1939StackFake.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "j1939StackFake.h"
/******************************************************************************/

typedef struct j1939FakeStruct* j1939Fake_t;

typedef struct j1939FakeStruct
{
    j1939Struct_t base;
    canDriver_t driver;
}j1939FakeStruct_t;

/******************************************************************************/

static void destroy( j1939_t base )
{
    j1939Fake_t fake = reinterpret_cast< j1939Fake_t > ( base );
    destroyCANDriver( fake->driver );
    delete fake;
}

static uint8_t sendMessage( j1939_t base, const j1939Message_t message )
{
    ( void ) base;
    ( void ) message;
    return 0;
}

/******************************************************************************/
j1939_t createJ1939FakeStack( canDriver_t driver )
{
    static j1939InterfaceStruct_t interface =
    {
        destroy,
        sendMessage
    };

    j1939Fake_t self = new j1939FakeStruct_t;
    self->base.type = "FakeJ1939Stack";
    self->base.iFace = &interface;
    self->driver = driver;

    return { reinterpret_cast< j1939_t >( self ) };
}
