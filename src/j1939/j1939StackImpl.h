/*******************************************************************************
 * @file	j1939StackImpl.h
 * @brief	J1939 stack implementation
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef J1939STACKIMPL_H_
#define J1939STACKIMPL_H_
/******************************************************************************/
#include "j1939/iJ1939Stack.h"
#include "drivers/CANDriver.h"

/******************************************************************************/
j1939_t createJ1939StackImpl( canDriver_t driver, uint8_t tickMs, uint8_t* caName );

#endif /* J1939STACKIMPL_H_ */
