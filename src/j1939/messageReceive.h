/*******************************************************************************
 * @file	messageReceive.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef MESSAGERECEIVE_H_
#define MESSAGERECEIVE_H_
/******************************************************************************/
#include "j1939Stack.h"
#include "drivers/CANDriver.h"

/******************************************************************************/
j1939Message_t receiveJ1939MessageFromCANDriver( canDriver_t driver );

#endif /* MESSAGERECEIVE_H_ */
