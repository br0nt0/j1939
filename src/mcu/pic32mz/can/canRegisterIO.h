/*******************************************************************************
 * @file	canRegisterIO.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef CANREGISTERIO_H_
#define CANREGISTERIO_H_
/******************************************************************************/
#include "canTypes.h"
#include "canEnumTypes.h"

/******************************************************************************/
canRegisters_t* getCANModuleRegisters( canModule_t module );

#endif /* CANREGISTERIO_H_ */
