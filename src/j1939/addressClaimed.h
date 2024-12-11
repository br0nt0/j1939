/*******************************************************************************
 * @file	addressClaimed.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef ADDRESSCLAIMED_H_
#define ADDRESSCLAIMED_H_
/******************************************************************************/
#include "commonTypes.h"
#include "drivers/CANDriver.h"

/******************************************************************************/
enum
{
    UNDEFINED = -1,
    INIT = 0,
    WAIT_FOR_CONTENTION,
    NORMAL_TRAFFIC
};

enum
{
    CONTENTION_TIMEOUT_MS = 250u
};

typedef struct
{
    canDriver_t driver;
    uint8_t* caName;
    uint8_t tickMs;
    uint8_t sourceAddress;
    int8_t initialState;
}aclConfigStruct_t;

void configureAddressClaimed( aclConfigStruct_t* configuration );
void updateAddressClaimed( void );
bool_t wasAddressClaimedSuccessfuly( void );

#endif /* ADDRESSCLAIMED_H_ */
