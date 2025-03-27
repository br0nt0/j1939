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
#include "iJ1939Stack.h"

/******************************************************************************/
void encodeCANMessage( canMessage_t canMessage, const j1939Message_t j1939Message );
void decodeMessage( j1939Message_t j1939Message, const canMessage_t canMessage );

#endif /* MESSAGECONVERTER_H_ */
