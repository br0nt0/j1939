/*******************************************************************************
 * @file	CANDriverInterface.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef CANDRIVERINTERFACE_H_
#define CANDRIVERINTERFACE_H_
/******************************************************************************/
#include "commonTypes.h"

typedef struct canMessageStruct
{
    uint32_t id;
    bool_t isExtended;
    uint8_t dlc;
    uint8_t* data;
} canMessageStruct_t;

typedef struct canDriverInterfaceStruct* canDriverInterface_t;
typedef struct canDriverStruct
{
    canDriverInterface_t vTable;
    const char* type;
}canDriverStruct_t;

typedef struct canDriverInterfaceStruct
{
    void ( *destroy )( canDriver_t );
    bool_t( *isOperational )( canDriver_t );
    uint8_t( *sendMessage )( canDriver_t, const canMessage_t );
    canMessage_t( *receiveMessage )( canDriver_t );
}canDriverInterfaceStruct_t;

/******************************************************************************/


#endif /* CANDRIVERINTERFACE_H_ */
