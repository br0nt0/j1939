/*******************************************************************************
 * @file	CANMessage.h
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#ifndef CANMESSAGE_H_
#define CANMESSAGE_H_
/******************************************************************************/
#include "commonTypes.h"

typedef struct CANMessageStruct* CANMessage_t;

/******************************************************************************/
void destroyCANMessage( CANMessage_t self );
bool_t isCANMessageExtended( CANMessage_t self );
uint8_t* getCANMessageData( CANMessage_t self );
uint8_t getCANMessageDLC( CANMessage_t self );
uint32_t getCANMessageID( CANMessage_t self );
CANMessage_t createCANMessage( uint32_t id, bool_t isExtended, uint8_t* data, uint8_t dlc );


#endif /* CANMESSAGE_H_ */
