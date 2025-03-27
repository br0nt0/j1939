/*******************************************************************************
 * @file	iAddressClaimed.h
 * @brief   address claimed procedure interface
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#ifndef IADDRESSCLAIMED_H_
#define IADDRESSCLAIMED_H_
 /******************************************************************************/
#include "commonTypes.h"

typedef struct aclStruct* acl_t;
typedef struct aclInterfaceStruct* aclInterface_t;

typedef struct aclStruct
{
    aclInterface_t iFace;
    const char* type;
}aclStruct_t;

typedef struct aclInterfaceStruct
{
    void ( *destroy )( acl_t );
    void ( *updateStateMachine )( acl_t );
    bool_t( *wasAddressClaimed )( acl_t );
    void ( *registerReqForACL )( acl_t );
    void ( *registerMessageWOwnSA )( acl_t );
    void ( *registerContention )( acl_t, const uint8_t* );
    void ( *setName )( acl_t, uint8_t* );
    uint8_t* ( *getName )( acl_t );
    void ( *setSA )( acl_t, uint8_t );
    uint8_t( *getSA )( acl_t );
}aclInterfaceStruct_t;

/******************************************************************************/
void destroyACL( acl_t acl );
void updateACLStateMachine( acl_t acl );
bool_t wasACLSuccessful( acl_t acl );
void registerRequestForACL( acl_t acl );
void registerRcvMessageWithOwnSourceAddress( acl_t acl );
void registerACLContention( acl_t acl, const uint8_t* name );
void setCAName( acl_t acl, uint8_t* name );
uint8_t* getCAName( acl_t acl );
void setACLSourceAddress( acl_t acl, uint8_t sourceAddress );
uint8_t getACLSourceAddress( acl_t acl );
const char* getACLType( acl_t acl );

#endif /* IADDRESSCLAIMED_H_ */

