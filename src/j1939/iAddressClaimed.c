/*******************************************************************************
 * @file	iAddressClaimed.c
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "iAddressClaimed.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
void destroyACL( acl_t acl )
{
    if ( acl != NULL )
    {
        acl->iFace->destroy( acl );
    }
}

void updateACLStateMachine( acl_t acl )
{
    if ( acl != NULL )
    {
        acl->iFace->updateStateMachine( acl );
    }
}

bool_t wasACLSuccessful( acl_t acl )
{
    bool_t wasSuccessful = false;
    if ( acl != NULL )
    {
        wasSuccessful = acl->iFace->wasAddressClaimed( acl );
    }
    return ( wasSuccessful );
}

void registerRequestForACL( acl_t acl )
{
    if ( acl != NULL )
    {
        acl->iFace->registerReqForACL( acl );
    }
}

void registerRcvMessageWithOwnSourceAddress( acl_t acl )
{
    if ( acl != NULL )
    {
        acl->iFace->registerMessageWOwnSA( acl );
    }
}

void registerACLContention( acl_t acl, const uint8_t* name )
{
    if ( acl != NULL )
    {
        acl->iFace->registerContention( acl, name );
    }
}

void setCAName( acl_t acl, uint8_t* name )
{
    if ( acl != NULL )
    {
        acl->iFace->setName( acl, name );
    }
}

uint8_t* getCAName( acl_t acl )
{
    uint8_t* caName = NULL;
    if ( acl != NULL )
    {
        caName = acl->iFace->getName( acl );
    }
    return ( caName );
}

void setSourceAddress( acl_t acl, uint8_t sourceAddress )
{
    if ( acl != NULL )
    {
        acl->iFace->setSA( acl, sourceAddress );
    }
}

uint8_t getSourceAddress( acl_t acl )
{
    uint8_t sourceAddress = 0xfeu;
    if ( acl != NULL )
    {
        sourceAddress = acl->iFace->getSA( acl );
    }
    return ( sourceAddress );
}

const char* getACLType( acl_t acl )
{
    return ( acl->type );
}