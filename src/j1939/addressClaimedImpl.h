/*******************************************************************************
 * @file	addressClaimedImpl.h
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#ifndef ADDRESSCLAIMEDIMPL_H_
#define ADDRESSCLAIMEDIMPL_H_
/******************************************************************************/
#include "iAddressClaimed.h"
#include "drivers/CANDriver.h"


enum
{
    INIT = 0u,
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

/******************************************************************************/
acl_t createAddressClaimed( canDriver_t driver, uint16_t ticksMS, uint8_t* caName, uint8_t sourceAddress );
void setACLStateMachineState( acl_t acl, uint8_t state );
uint8_t getACLStateMachineState( acl_t acl );

#endif /* ADDRESSCLAIMEDIMPL_H_ */
