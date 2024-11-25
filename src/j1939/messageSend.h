/*******************************************************************************
 * @file	messageSend.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef MESSAGESEND_H_
#define MESSAGESEND_H_
/******************************************************************************/
#include "j1939Stack.h"
#include "drivers/CANDriver.h"

/******************************************************************************/
uint8_t sendJ1939MessageToCANDriver( j1939Message_t message, canDriver_t driver );

#endif /* MESSAGESEND_H_ */
