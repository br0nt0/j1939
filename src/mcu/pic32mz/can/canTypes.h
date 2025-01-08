/*******************************************************************************
 * @file	canTypes.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef CANTYPES_H_
#define CANTYPES_H_
/******************************************************************************/
#include "stdint.h"

/******************************************************************************/
#define CAN_NUM_OF_FILTERS 				32u
#define CAN_NUM_OF_FILTER_MASKS 		4u
#define CAN_NUM_OF_FILTER_CONTROL_REGS 	8u
#define CAN_NUM_OF_FIFOS				32u

#define CXFIFOCON_UINC_MASK             0x00002000u
#define CXFIFOCON_TXREQ_MASK            0x00000008u

typedef struct
{
		unsigned int DNCNT :5;
		unsigned int :6;
		unsigned int CANBUSY :1;
		unsigned int :1;
		unsigned int SIDL :1;
		unsigned int :1;
		unsigned int ON :1;
		unsigned int :4;
		unsigned int CANCAP :1;
		unsigned int OPMOD :3;
		unsigned int REQOP :3;
		unsigned int ABAT :1;
} cxcon_t;

typedef struct
{
		unsigned int BRP :6;
		unsigned int SJW :2;
		unsigned int PRSEG :3;
		unsigned int SEG1PH :3;
		unsigned int SAM :1;
		unsigned int SEG2PHTS :1;
		unsigned int SEG2PH :3;
		unsigned int :3;
		unsigned int WAKFIL :1;
} cxcfg_t;

typedef struct
{
		unsigned int ICODE :7;
		unsigned int :1;
		unsigned int FILHIT :5;
} cxvec_t;

typedef struct
{
		unsigned int CANTSPRE :16;
		unsigned int CANTS :16;
} cxtmr_t;

typedef struct
{
		unsigned int EID :18;
		unsigned int :1;
		unsigned int MIDE :1;
		unsigned int :1;
		unsigned int SID :11;
} cxrxm_t;

typedef struct
{
		union
		{
				volatile uint32_t x;
				volatile cxrxm_t bits;
		} CxRXM;
		volatile uint32_t CxRXMCLR;
		volatile uint32_t CxRXMSET;
		volatile uint32_t CxRXMINV;

} canFilterMaskRegs_t;

typedef struct
{
		unsigned int EID :18;
		unsigned int :1;
		unsigned int EXID :1;
		unsigned int :1;
		unsigned int SID :11;
} cxrxf_t;

typedef struct
{
		volatile uint32_t CxFLTCON;
		volatile uint32_t CxFLTCONCLR;
		volatile uint32_t CxFLTCONSET;
		volatile uint32_t CxFLTCONINV;

} canFilterControlRegs_t;

typedef struct
{
		union
		{
				volatile uint32_t x;
				volatile cxrxf_t bits;
		} CxRXF;
		volatile uint32_t CxRXFCLR;
		volatile uint32_t CxRXFSET;
		volatile uint32_t CxRXFINV;

} canFilterRegs_t;

typedef struct
{
		unsigned int TXPRI :2;
		unsigned int RTREN :1;
		unsigned int TXREQ :1;
		unsigned int TXERR :1;
		unsigned int TXLARB :1;
		unsigned int TXABAT :1;
		unsigned int TXEN :1;
		unsigned int :4;
		unsigned int DONLY :1;
		unsigned int UINC :1;
		unsigned int FRESET :1;
		unsigned int :1;
		unsigned int FSIZE :5;
} cxfifocon_t;

typedef struct
{
		unsigned int RXNEMPTYIF :1;
		unsigned int RXHALFIF :1;
		unsigned int RXFULLIF :1;
		unsigned int RXOVFLIF :1;
		unsigned int :4;
		unsigned int TXEMPTYIF :1;
		unsigned int TXHALFIF :1;
		unsigned int TXNFULLIF :1;
		unsigned int :5;
		unsigned int RXNEMPTYIE :1;
		unsigned int RXHALFIE :1;
		unsigned int RXFULLIE :1;
		unsigned int RXOVFLIE :1;
		unsigned int :4;
		unsigned int TXEMPTYIE :1;
		unsigned int TXHALFIE :1;
		unsigned int TXNFULLIE :1;
} cxfifoint_t;

typedef struct
{
		union
		{
				volatile uint32_t x;
				volatile cxfifocon_t bits;
		} CxFIFOCON;
		volatile uint32_t CxFIFOCONCLR;
		volatile uint32_t CxFIFOCONSET;
		volatile uint32_t CxFIFOCONINV;

		union
		{
				volatile uint32_t x;
				volatile cxfifoint_t bits;
		} CxFIFOINT;
		volatile uint32_t CxFIFOINTCLR;
		volatile uint32_t CxFIFOINTSET;
		volatile uint32_t CxFIFOINTINV;

		volatile uint32_t CxFIFOUA;
		volatile uint32_t CxFIFOUACLR;
		volatile uint32_t CxFIFOUASET;
		volatile uint32_t CxFIFOUAINV;

		volatile uint32_t CxFIFOCI;
		volatile uint32_t CxFIFOCICLR;
		volatile uint32_t CxFIFOCISET;
		volatile uint32_t CxFIFOCIINV;
} canFifoRegs_t;

typedef struct
{
		union
		{
				volatile uint32_t x;
				volatile cxcon_t bits;
		} CxCON;
		volatile uint32_t CxCONCLR;
		volatile uint32_t CxCONSET;
		volatile uint32_t CxCONINV;

		union
		{
				volatile uint32_t x;
				volatile cxcfg_t bits;
		} CxCFG;
		volatile uint32_t CxCFGCLR;
		volatile uint32_t CxCFGSET;
		volatile uint32_t CxCFGINV;

		volatile uint32_t CxINT;
		volatile uint32_t CxINTCLR;
		volatile uint32_t CxINTSET;
		volatile uint32_t CxINTINV;

		union
		{
				volatile uint32_t x;
				volatile cxvec_t bits;
		} CxVEC;
		volatile uint32_t CxVECCLR;
		volatile uint32_t CxVECSET;
		volatile uint32_t CxVECINV;

		volatile uint32_t CxTREC;
		volatile uint32_t CxTRECCLR;
		volatile uint32_t CxTRECSET;
		volatile uint32_t CxTRECINV;

		volatile uint32_t CxFSTAT;
		volatile uint32_t CxFSTATCLR;
		volatile uint32_t CxFSTATSET;
		volatile uint32_t CxFSTATINV;

		volatile uint32_t CxRXOVF;
		volatile uint32_t CxRXOVFCLR;
		volatile uint32_t CxRXOVFSET;
		volatile uint32_t CxRXOVFINV;

		union
		{
				volatile uint32_t x;
				volatile cxtmr_t bits;
		} CxTMR;
		volatile uint32_t CxTMRCLR;
		volatile uint32_t CxTMRSET;
		volatile uint32_t CxTMRINV;

		volatile canFilterMaskRegs_t canFilterMaskRegs[ CAN_NUM_OF_FILTER_MASKS ];

		volatile canFilterControlRegs_t canFilterControlRegs[ CAN_NUM_OF_FILTER_CONTROL_REGS ];
		volatile canFilterRegs_t canFilterRegs[ CAN_NUM_OF_FILTERS ];

		volatile uint32_t CxFIFOBA;
		volatile uint32_t CxFIFOBACLR;
		volatile uint32_t CxFIFOBASET;
		volatile uint32_t CxFIFOBAINV;

		volatile canFifoRegs_t canFifoRegisters[ CAN_NUM_OF_FIFOS ];

} canRegisters_t;



#endif /* CANTYPES_H_ */
