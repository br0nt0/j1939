/*******************************************************************************
 * @file	canRegisterIOMockTests.cpp
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "mcu/pic32mz/can/canRegisterIO.h"
}

TEST_GROUP( canRegisterIOMock )
{
    canRegisters_t canRegisters[ CAN_NUMBER_OF_MODULES ];
    void setup( void )
    {
    }
    void teardown( void )
    {
        mock( "PIC32MZ_CAN" ).checkExpectations( );
        mock( ).clear( );
    }
    void expectOneCallToGetCANRegistersOfModule( canModule_t module, canRegisters_t * regs )
    {
        mock( "PIC32MZ_CAN" )
            .expectOneCall( "getPIC32MZCANModuleRegs" )
            .withParameter( "module", module )
            .andReturnValue( regs );
    }
    void expectOneCallToGetRxFifoUserAddressRegisterOfModule( canModule_t module, canFifo_t fifo, canRxMessageBuffer_t * buffer )
    {
        mock( "PIC32MZ_CAN" )
            .expectOneCall( "getPIC32MZRxCxFIFOUA" )
            .withParameter( "module", module )
            .withParameter( "fifo", fifo )
            .andReturnValue( buffer );
    }
    void expectOneCallToGetTxFifoUserAddressRegisterOfModule( canModule_t module, canFifo_t fifo, canTxMessageBuffer_t * buffer )
    {
        mock( "PIC32MZ_CAN" )
            .expectOneCall( "getPIC32MZTxCxFIFOUA" )
            .withParameter( "module", module )
            .withParameter( "fifo", fifo )
            .andReturnValue( buffer );
    }
};

TEST( canRegisterIOMock, given_the_can_module_1_when_getting_address_pointer_from_HW_then_full_register_structure_returned )
{
    // given
    canRegisters_t* expectedCANModule1Registers = &canRegisters[ 0 ];
    expectOneCallToGetCANRegistersOfModule( CAN1, expectedCANModule1Registers );

    // when
    canRegisters_t* canModule1Regs = getCANModuleRegisters( CAN1 );

    // then
    POINTERS_EQUAL( expectedCANModule1Registers, canModule1Regs );
}

TEST( canRegisterIOMock, given_a_can_controller_when_getting_address_pointer_from_HW_then_all_modules_can_be_returned )
{
    // given
    for ( size_t i = 0; i < CAN_NUMBER_OF_MODULES; i++ )
    {
        expectOneCallToGetCANRegistersOfModule( ( canModule_t ) i, &canRegisters[ i ] );

    // when
        canRegisters_t* canModuleRegs = getCANModuleRegisters( ( canModule_t ) i );

    // then
        POINTERS_EQUAL( &canRegisters[ i ], canModuleRegs );
    }
}

TEST( canRegisterIOMock, given_a_configured_receive_HW_fifo_when_getting_message_then_receive_message_user_addresss_register_buffer_received )
{
    // given
    canRxMessageBuffer_t receivedBuffer;
    expectOneCallToGetRxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO0, &receivedBuffer );

    // when
    canRxMessageBuffer_t* message = getRxCxFIFOUA( CAN1, CAN_FIFO0 );

    // then
    POINTERS_EQUAL( &receivedBuffer, message );
}

TEST( canRegisterIOMock, given_a_configured_transmit_HW_fifo_when_setting_the_message_then_transmit_user_address_register_buffer_received )
{
    // given
    canTxMessageBuffer_t sendBuffer;
    expectOneCallToGetTxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO1, &sendBuffer );

    // when
    canTxMessageBuffer_t* message = getTxCxFIFOUA( CAN1, CAN_FIFO1 );

    // then
    POINTERS_EQUAL( &sendBuffer, message );
}


