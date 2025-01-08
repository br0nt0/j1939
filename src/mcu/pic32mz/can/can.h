/*******************************************************************************
 * @file	can.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef CAN_H_
#define CAN_H_
/******************************************************************************/
#include "drivers/CANDriver.h"

typedef struct picCANDriverStruct* picCANDriver_t;

/******************************************************************************/
canDriver_t createPIC32MZCANDriverForModule( uint8_t module );

#endif /* CAN_H_ */
