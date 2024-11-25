/*******************************************************************************
 * @file	j1939Stack.h
 * @brief
 * @author  @br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef J1939STACK_H_
#define J1939STACK_H_
/******************************************************************************/

#include "commonTypes.h"

typedef struct j1939Struct* j1939_t;
typedef struct j1939MessageStruct* j1939Message_t;

/******************************************************************************/
void destroyJ1939Stack( j1939_t self );
uint8_t sendJ1939Message( j1939_t self, const j1939Message_t message );
const char* getJ1939StackType( j1939_t self );

#include "j1939StackInterface.h"

#endif /* J1939STACK_H_ */
