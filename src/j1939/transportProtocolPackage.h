/*******************************************************************************
 * @file	transportProtocolPackage.h
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#ifndef TRANSPORTPROTOCOLPACKAGE_H_
#define TRANSPORTPROTOCOLPACKAGE_H_
/******************************************************************************/
#include "commonTypes.h"

typedef struct tpPackStruct* tpPack_t;
/******************************************************************************/
tpPack_t createTransportProtocolPackage( uint8_t* data, uint16_t dataSize );
void destroyTransportProtocolPackage( tpPack_t self );
uint8_t getNumberOfPackages( tpPack_t self );
uint8_t* getNextTransportProtocolPackage( tpPack_t self );
void resetPackIndexTo( tpPack_t self, uint8_t index );
void setNextTransportProtocolPackage( tpPack_t self, const uint8_t* dataPack );

#endif /* TRANSPORTPROTOCOLPACKAGE_H_ */
