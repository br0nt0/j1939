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


/******************************************************************************/
canRegisters_t* getCANModuleRegisters( canModule_t module )
{
    static const canRegisters_t* canBase[ CAN_NUMBER_OF_MODULES ] =
    {
        ( canRegisters_t* ) CAN1_BASE_ADDRESS,
        ( canRegisters_t* ) CAN2_BASE_ADDRESS
    };
    
    return ( ( canRegisters_t* ) canBase[ ( uint8_t ) module ] );
}

