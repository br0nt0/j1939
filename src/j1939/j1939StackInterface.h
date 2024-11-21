/*******************************************************************************
 * @file	j1939StackInterface.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef J1939STACKINTERFACE_H_
#define J1939STACKINTERFACE_H_
/******************************************************************************/

typedef struct j1939MessageStruct* j1939Message_t;
typedef struct j1939MessageStruct
{
    uint32_t parameterGroupNumber;
    uint16_t dataSize;
    uint8_t priority;
    uint8_t sourceAddress;
    uint8_t destinationAddress;
    uint8_t* data;
}j1939MessageStruct_t;

typedef struct j1939InterfaceStruct* j1939Interface_t;
typedef struct j1939Struct
{
    j1939Interface_t iFace;
    const char* type;
}j1939Struct_t;

typedef struct j1939InterfaceStruct
{
    void ( *destroy )( j1939_t );
    uint8_t ( *sendJ1939Message )( j1939_t, j1939Message_t );
}j1939InterfaceStruct_t;


/******************************************************************************/


#endif /* J1939STACKINTERFACE_H_ */
