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
        
    }
    return ( wasSuccessful );
}

void registerRequestForACL( acl_t acl )
{
    if ( acl != NULL )
    {
        
    }
}

void registerRcvMessageWithOwnSourceAddress( acl_t acl )
{
    if ( acl != NULL )
    {
        
    }
}

void registerACLContention( acl_t acl, const uint8_t* name )
{
    ( void ) name;
    if ( acl != NULL )
    {
        
    }
}

void setCAName( acl_t acl, const uint8_t* name )
{
    ( void ) name;
    if ( acl != NULL )
    {
        
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
    ( void ) sourceAddress;
    if ( acl != NULL )
    {
        
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