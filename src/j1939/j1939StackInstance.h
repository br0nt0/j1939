/*******************************************************************************
 * @file	j1939StackInstance.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef J1939STACKINSTANCE_H_
#define J1939STACKINSTANCE_H_
/******************************************************************************/
#include "j1939/j1939Stack.h"
#include "drivers/CANDriver.h"

/******************************************************************************/
j1939_t createJ1939StackInstance( canDriver_t driver );



#endif /* J1939STACKINSTANCE_H_ */
