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
    uint32_t placeHolder;
} canMessageStruct_t;

typedef struct canDriverStruct
{
    const char* type;
}canDriverStruct_t;

/******************************************************************************/


#endif /* CANDRIVERINTERFACE_H_ */
