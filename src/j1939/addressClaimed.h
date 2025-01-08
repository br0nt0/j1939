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
#include "j1939/j1939Stack.h"

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
	CONTENTION_TIMEOUT_MS = 250u,
	NULL_ADDRESS = 254u,
	ACL_PGN = 60928u
};

typedef struct aclStruct
{
	canDriver_t driver;
	bool_t wasAddressClaimed;
	bool_t wasRequestForACLReceived;
	bool_t wasMessageWithOwnSAReceived;
	bool_t wasContentionReceived;
	uint8_t sourceAddress;
	uint8_t newSourceAddress;
	uint8_t contentionCounterMs;
	uint8_t pseudoDelay;
	uint8_t tickMs;
	int8_t state;
	uint8_t* caName;
	uint8_t contenderName[ 8 ];
}aclStruct_t;
typedef aclStruct_t* acl_t;


void configureAddressClaim( acl_t acl, canDriver_t driver, int8_t state );
void updateAddressClaimed( acl_t acl );
bool_t wasAddressClaimedSuccessfuly( acl_t acl );
void registerRequestForAddressClaim( acl_t acl );
void registerReceivedMessageWithOwnSA( acl_t acl );
void registerReceivedContention( acl_t acl, const uint8_t* caName );

#endif /* ADDRESSCLAIMED_H_ */
