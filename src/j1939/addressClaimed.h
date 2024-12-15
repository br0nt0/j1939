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
#include "j1939Stack.h"

/******************************************************************************/
enum
{
    UNDEFINED = -1,
    INIT = 0,
    WAIT_FOR_CONTENTION,
    NORMAL_TRAFFIC,
    DELAY_BEFORE_RECLAIM,
    CANNOT_CLAIM_ADDRESS,
    DELAY_BEFORE_CANNOT_CLAIM
};

enum
{
    CONTENTION_TIMEOUT_MS = 250u
};

typedef struct aclConfigStruct* aclConfig_t;


void configureAddressClaimed( j1939_t stack, int8_t state );
void updateAddressClaimed( void );
bool_t wasAddressClaimedSuccessfuly( void );
void registerRequestForAddressClaim( void );
void registerReceivedMessageWithOwnSA( void );
void registerReceivedContention( const uint8_t* caName );

#endif /* ADDRESSCLAIMED_H_ */
