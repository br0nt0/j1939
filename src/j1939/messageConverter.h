/*******************************************************************************
 * @file	messageConverter.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef MESSAGECONVERTER_H_
#define MESSAGECONVERTER_H_
/******************************************************************************/
#include "commonTypes.h"
#include "drivers/CANDriver.h"
#include "j1939Stack.h"

/******************************************************************************/
void encodeCANMessage( canMessage_t canMessage, const j1939Message_t j1939Message );

#endif /* MESSAGECONVERTER_H_ */
