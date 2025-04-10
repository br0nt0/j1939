/*******************************************************************************
 * @file	iJ1939Stack.h
 * @brief
 * @author  @br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef IJ1939STACK_H_
#define IJ1939STACK_H_
/******************************************************************************/
#include "commonTypes.h"
#include "j1939Message.h"

enum
{
    J1939_RQST = 59904u,
    J1939_AC = 60928u
};

typedef struct j1939Struct* j1939_t;
typedef struct j1939InterfaceStruct* j1939Interface_t;
typedef struct j1939Struct
{
    j1939Interface_t iFace;
    const char* type;
}j1939Struct_t;

typedef struct j1939InterfaceStruct
{
    void ( *destroy )( j1939_t );
    uint8_t( *sendJ1939Message )( j1939_t, j1939Message_t );
    j1939Message_t( *receiveJ1939Message )( j1939_t );
    void ( *setSourceAddress )( j1939_t, uint8_t );
    uint8_t( *getSourceAddress )( j1939_t );
    void ( *setCAName )( j1939_t, const uint8_t* );
    uint8_t* ( *getCAName )( j1939_t );
    uint8_t( *getTickMs )( j1939_t );
    void ( *updateCoreScheduler )( j1939_t );
    bool_t( *wasAddressClaimed )( j1939_t );
}j1939InterfaceStruct_t;

/******************************************************************************/
void destroyJ1939Stack( j1939_t self );
uint8_t sendJ1939Message( j1939_t self, const j1939Message_t message );
j1939Message_t receiveJ1939Message( j1939_t self );
const char* getJ1939StackType( j1939_t self );
uint8_t getJ1939SourceAddress( j1939_t self );
void setJ1939SourceAddress( j1939_t self, uint8_t address );
uint8_t* getJ1939CAName( j1939_t self );
void setJ1939CAName( j1939_t self, const uint8_t* caName );
uint8_t getJ1939ConfiguredTickMs( j1939_t self );
void updateJ1939CoreScheduler( j1939_t self );
bool_t wasJ1939AddressClaimed( j1939_t self );

#endif /* IJ1939STACK_H_ */
