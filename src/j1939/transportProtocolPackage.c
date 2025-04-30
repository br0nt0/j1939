/*******************************************************************************
 * @file	transportProtocolPackage.c
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "transportProtocolPackage.h"
/******************************************************************************/
enum
{
	NUMBER_OF_BYTES_IN_PACK = 7u,
	MAXIMUM_NUMBER_OF_PACKS = 255u
};

struct tpPackStruct
{
	uint16_t dataSize;
	uint8_t* data;
	uint8_t numberOfPacks;
	uint8_t packageIndex;
	uint8_t package[ 8 ];
};
/******************************************************************************/
static uint8_t getNumberOfPacksFromDataSize( uint16_t dataSize )
{
	uint8_t numberOfPacks = 0u;

	if ( dataSize > ( MAXIMUM_NUMBER_OF_PACKS * NUMBER_OF_BYTES_IN_PACK ) )
	{
		numberOfPacks = MAXIMUM_NUMBER_OF_PACKS;
	}
	else
	{
		numberOfPacks = ( uint8_t ) ( dataSize / NUMBER_OF_BYTES_IN_PACK );
		if ( ( dataSize % NUMBER_OF_BYTES_IN_PACK ) > 0U )
		{
			++numberOfPacks;
		}
	}

	return ( numberOfPacks );
}

static void setPackDataToFF( uint8_t pack[8] )
{
	for ( uint8_t i = 0u; i < 8u; i++ )
	{
		pack[ i ] = 255u;
	}
}

static uint8_t getNumberOfBytes2Write( uint8_t index, uint8_t maxPacks, uint16_t dataSize )
{
	return ( ( index + 1u ) < maxPacks ) ? NUMBER_OF_BYTES_IN_PACK : ( uint8_t ) ( dataSize % NUMBER_OF_BYTES_IN_PACK );
}

/******************************************************************************/
tpPack_t createTransportProtocolPackage( uint8_t* data, uint16_t dataSize )
{
	tpPack_t self = NULL;
	if ( data && ( dataSize > 8u ) )
	{
		self = ( tpPack_t ) malloc( sizeof( struct tpPackStruct ) );
		self->numberOfPacks = getNumberOfPacksFromDataSize( dataSize );
		self->packageIndex = 0u;
		self->data = data;
		self->dataSize = dataSize;
	}
	return ( self );
}

void destroyTransportProtocolPackage( tpPack_t self )
{
	if ( self != NULL )
	{
		free( self );
	}
}

uint8_t getNumberOfPackages( tpPack_t self )
{
	uint8_t numberOfPacks = 0u;
	if ( self != NULL )
	{
		numberOfPacks = self->numberOfPacks;
	}
	return ( numberOfPacks );
}

uint8_t* getNextTransportProtocolPackage( tpPack_t self )
{
	uint8_t* pack = NULL;
	if ( ( NULL != self ) && ( self->packageIndex < self->numberOfPacks ) )
	{
		setPackDataToFF( self->package );
		self->package[ 0 ] = self->packageIndex + 1u;
		uint8_t numberOfBytesToWrite = getNumberOfBytes2Write( self->packageIndex, self->numberOfPacks, self->dataSize );
		for ( uint8_t i = 0u; i < numberOfBytesToWrite; i++ )
		{
			self->package[ i + 1u ] = self->data[ ( self->packageIndex * NUMBER_OF_BYTES_IN_PACK ) + i ];
		}
		self->packageIndex++;
		pack = self->package;
	}
	return ( pack );
}

void resetPackIndexTo( tpPack_t self, uint8_t index )
{
	if ( ( self != NULL ) && ( index < self->numberOfPacks ) )
	{
		self->packageIndex = index;
	}
}

void setNextTransportProtocolPackage( tpPack_t self, const uint8_t* dataPack )
{
	if ( self && dataPack )
	{
		self->packageIndex = dataPack[ 0 ] - 1u;
		if ( self->packageIndex < self->numberOfPacks )
		{
			uint8_t numberOfBytesToWrite = getNumberOfBytes2Write( self->packageIndex, self->numberOfPacks, self->dataSize );
			for (uint8_t i = 0u; i < numberOfBytesToWrite; i++)
			{
				self->data[ ( self->packageIndex * NUMBER_OF_BYTES_IN_PACK ) + i ] = dataPack[ i + 1u ];
			}
		}
	}
}

