/*******************************************************************************
 * @file	j1939StackImpl.h
 * @brief	J1939 stack implementation
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef J1939STACKIMPL_H_
#define J1939STACKIMPL_H_
/******************************************************************************/
#include "iJ1939Stack.h"
#include "drivers/CANDriver.h"
#include "addressClaimedImpl.h"

/******************************************************************************/
j1939_t createJ1939StackImpl( acl_t acl, canDriver_t driver );

#endif /* J1939STACKIMPL_H_ */
