/*******************************************************************************
 * @file	canRegisterIO.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "canRegisterIO.h"
/******************************************************************************/
#define CAN1_BASE_ADDRESS 0xBF880000u
#define CAN2_BASE_ADDRESS 0xBF881000u

/******************************************************************************/
static const canRegisters_t* canBase[ CAN_NUMBER_OF_MODULES ] =
{
    ( canRegisters_t* ) CAN1_BASE_ADDRESS,
    ( canRegisters_t* ) CAN2_BASE_ADDRESS
};


static canRxMessageBuffer_t* address2RxBuffer( uintptr_t address )
{
	return ( ( canRxMessageBuffer_t* ) address );
}

static canTxMessageBuffer_t* address2TxBuffer( uintptr_t address )
{
	return ( ( canTxMessageBuffer_t* ) address );
}

static uint32_t pa2kva1( uint32_t address )
{
	return ( address | 0xa0000000u );
}

/******************************************************************************/
canRegisters_t* getCANModuleRegisters( canModule_t module )
{    
    return ( ( canRegisters_t* ) canBase[ ( uint8_t ) module ] );
}

canRxMessageBuffer_t* getRxCxFIFOUA( canModule_t module, canFifo_t fifo )
{
	return ( address2RxBuffer( pa2kva1( canBase[ module ]->canFifoRegisters[ fifo ].CxFIFOUA ) ) );
}

canTxMessageBuffer_t* getTxCxFIFOUA( canModule_t module, canFifo_t fifo )
{
	return ( address2TxBuffer( pa2kva1( canBase[ module ]->canFifoRegisters[ fifo ].CxFIFOUA ) ) );
}
