/*******************************************************************************
 * @file	canRxTxStructures.h
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#ifndef CANRXTXSTRUCTURES_H_
#define CANRXTXSTRUCTURES_H_
/******************************************************************************/
typedef struct
{
		unsigned int SID :11;
		unsigned int FILHIT :5;
		unsigned int CMSGTS :16;
} canRxMsgSid_t;

typedef struct
{
		unsigned int SID :11;
		unsigned int :21;
} canTxMsgSid_t;

typedef struct
{
		unsigned int DLC :4;
		unsigned int RB0 :1;
		unsigned int :3;
		unsigned int RB1 :1;
		unsigned int RTR :1;
		unsigned int EID :18;
		unsigned int IDE :1;
		unsigned int SRR :1;
		unsigned int :2;
} canMsgEid_t;

typedef struct
{
		canRxMsgSid_t sid;
		canMsgEid_t eid;
		uint8_t data[ 8 ];
} canRxMessageBuffer_t;

typedef struct
{
		canTxMsgSid_t sid;
		canMsgEid_t eid;
		uint8_t data[ 8 ];
} canTxMessageBuffer_t;

/******************************************************************************/


#endif /* CANRXTXSTRUCTURES_H_ */
