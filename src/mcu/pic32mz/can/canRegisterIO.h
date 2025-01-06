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
#include "canRxTxStructures.h"

/******************************************************************************/
canRegisters_t* getCANModuleRegisters( canModule_t module );
canRxMessageBuffer_t* getRxCxFIFOUA( canModule_t module, canFifo_t fifo );
canTxMessageBuffer_t* getTxCxFIFOUA( canModule_t module, canFifo_t fifo );

#endif /* CANREGISTERIO_H_ */
