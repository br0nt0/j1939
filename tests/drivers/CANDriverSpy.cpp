/*******************************************************************************
 * @file	CANDriverSpy.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "CANDriverSpy.h"
/******************************************************************************/

typedef struct canDriverSpyStruct* canDriverSpy_t;

typedef struct canDriverSpyStruct
{
    canDriverStruct_t base;
}canDriverSpyStruct_t;

/******************************************************************************/
static void destroy( canDriver_t base )
{
    canDriverSpy_t spy = reinterpret_cast< canDriverSpy_t > ( base );
    delete spy;
}

static bool_t isOperational( canDriver_t base )
{
    return ( mock( "CANSpy" ).actualCall( "isOperational" ).withPointerParameter( "base", base ).returnBoolValue( ) );
}

static uint8_t sendMessage( canDriver_t base, const CANMessage_t message )
{
    return ( static_cast< uint8_t >( mock( "CANSpy" ).actualCall( "sendMessage" )
                                     .withPointerParameter( "base", base )
                                     .withUnsignedIntParameter( "id", getCANMessageID( message ) )
                                     .withBoolParameter( "isExtended", isCANMessageExtended( message ) )
                                     .withUnsignedIntParameter( "dlc", getCANMessageDLC( message ) )
                                     .withPointerParameter( "data", getCANMessageData( message ) )
                                     .returnUnsignedIntValueOrDefault( 0u ) ) );
}

static CANMessage_t receiveMessage( canDriver_t base )
{
    return ( static_cast< CANMessage_t >( mock( "CANSpy" ).actualCall( "receiveMessage" )
                                                .withPointerParameter( "base", base )
                                                .returnPointerValue( ) ) );
}

static bool_t isTxBusOffState( canDriver_t base )
{
    return ( mock( "CANSpy" ).actualCall( "isTxBusOffState" )
                            .withPointerParameter( "base", base )
                            .returnBoolValue( ) );
}

/******************************************************************************/
canDriver_t createCANDriverSpy( void )
{
    static canDriverInterfaceStruct_t interface =
    {
        destroy,
        isOperational,
        sendMessage,
        receiveMessage,
        isTxBusOffState
    };

    canDriverSpy_t spy = new canDriverSpyStruct_t;
    spy->base.vTable = &interface;
    spy->base.type = "SpyCANDriver";

    return ( reinterpret_cast< canDriver_t > ( spy ) );
}


