/*===========================================================================

  Copyright (c) 2010 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/fw/components/fw_wcdma.mpss/rel/17.0/api/wfw_qsh_ext.h#1 $
===========================================================================*/
#ifndef WFW_QSH_EXT_H_
#define WFW_QSH_EXT_H_

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "wfw_sw_cmd_proc_intf.h"
/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/

#define WFW_QSH_MAJOR_VER 1
#define WFW_QSH_MINOR_VER 1

/*===========================================================================

                   STRUCTURE DEFINITIONS

===========================================================================*/
typedef struct
{
  /* Resource enable bitmasks */
  uint32 cellEnableBmsk;                                 
  uint32 fingEnableBmsk;                                 
  uint32 chanEnableBmsk;                                 
  uint32 combEnableBmsk;                                 
  uint32 qiceEnableBmsk;                                 

  /* Bit 0: DPCCH, Bit 1: RACH, Bit 2: HS-DPCCH, Bit 3: EUL */
  uint32 txEnableBmsk;                                   

  /* RxAGC enable bitmask */
  uint32 rxAgcEnBmsk;                                    

  /* Bit 0: CPC-DRX, Bit 1: CPC-DTX, Bit 2: E-DRX, Bit 3: FET */
  uint32 featureEnableBmsk;                              

  /* Frequency common */
  uint32 freqCommon;
} wfw_qsh_dump_common_s;

typedef struct
{
  /* Reference finger */
  uint32 refFingId   : 5;            /* Bit 4:0   */

  /* TxD mode */
  uint32 tdMode      : 1;            /* Bit 5     */

  uint32             : 23;           /* Bit 28:6  */
  uint32 reserved_ID : 3;            /* Bit 31:29 */
} wfw_qsh_dump_cell_s;

typedef struct
{
  /* Finger pn position in cx8 */
  uint32 pnPosCx8    : 19;           /* Bit 18:0  */

  /* Cell index to which this finger belongs */
  uint32 cellId      : 4;            /* Bit 22:19 */

  /* Sample server index  */
  uint32 ssId        : 1;            /* Bit 23 */

  uint32             : 5;            /* Bit 28:24  */
  uint32 reserved_ID : 3;            /* Bit 31:29 */
} wfw_qsh_dump_fing_s;

typedef struct
{
  /* Channel type */
  uint32 type        : 4;            /* Bit 3:0  */

  /* TxD mode */
  uint32 tdMode      : 2;            /* Bit 5:4 */

  /* Offset of channel from cell timing (in bpgs) */
  uint32 tto         : 8;            /* Bit 13:6 */

  uint32             : 15;           /* Bit 28:14  */
  uint32 reserved_ID : 3;            /* Bit 31:29 */
} wfw_qsh_dump_chan_s;

typedef struct
{
  /* EQ Enabled */
  uint32 enabled : 1;                /* Bit 0 */

  /* Master indicator */
  uint32 master : 1;                 /* Bit 1 */

  /* Rx diversity enable */
  uint32 rxdEnable : 1;              /* Bit 2 */

  /* QICE enabled */
  uint32 qiceEn : 1;                 /* Bit 3 */

  /* Cell index */
  uint32 cellIdx : 4;                /* Bit 7:4 */

  /* Starting PN pos, Cx1 units. */
  uint32 pnPosCx1 : 16;              /* Bit 23:8 */
  
  /* Iteration number for this cell */
  uint32 numIter : 2;                /* Bit 25:24 */

  uint32             : 3;            /* Bit 28:26 */
  uint32 reserved_ID : 3;            /* Bit 31:29 */
} wfw_qsh_dump_qice_s;

typedef struct
{

  /* Chain is configured */
  uint32 chainCfged : 1;
  /* Chain is enabled */
  uint32 chainEnabled : 1;

  /* ADC rate */
  uint32 adcRate;

  /* FCW */
  uint32 FCW;

  /* WideBand id */
  uint32 wbIdx : 8;

  /* NarrowBand id */
  uint32 nbIdx : 8;

  /* LNA state */
  uint32 lnaState: 3;

  /* DVGA gain */
  uint32 dvgaGain;

  /* DC loop gain */
  uint32 dcGain;

  /* AGC accum */
  uint32 rxagcValue;

  /* I2Q2 NB accum */
  uint32 i2q2NbAccum;

  /* I2Q2 WB accum */
  uint32 i2q2WbAccum;

  /* RxLM handle */
  UWord8 rxlmHandle;

} wfw_qsh_dump_rxfe_s;

typedef struct
{
  /* TX state function pointer */
  void (*txProcStateFunc) (void);
  /* Tx Stmr count read for TC context*/
  uint32 txCnt;
} wfw_qsh_dump_tx_s;

typedef struct
{
  /* Number of search tasks pending */
  uint32 numTasks;

  /* Type of last search submitted to HW */
  uint32 lastSrchType;
} wfw_qsh_dump_srch_s;

typedef struct
{
  /* Current QTA state */
  uint32 currentQtaState : 4;            /* Bit 3:0 */

  /* RX desense state */
  uint32 rxDesenseState : 4;             /* Bit 7:4 */

  /* TX desense state */
  uint32 txDesenseState : 4;            /* Bit 11:8 */

  uint32 : 20;
} wfw_qsh_dump_dsds_s; 

/*! @brief Define the header locally. This should match QSH def "qsh_dump_tag_hdr_s" */
typedef struct
{
  /*! set using dump_tag enum exported by client */
  uint16  dump_tag;
  
  /*! size of dump struct in bytes */
  uint16  dump_struct_size_bytes;
} wfw_qsh_dump_tag_hdr_s;

/*! @brief WCDMA FW cmd processing trace buffer */
typedef struct
{
  /*! Current ref count */
  UWord16 refCntrFrame;
  UWord16 refCntrCx1;
  UWord16 refCntrRspFrame;
  UWord16 refCntrRspCx1;
  /*! Command ID */
  WfwSwCmdEnum   cmdId;
  /*! Sync Cmd */
  WfwSwCmdStruct cmd;
  /*! Cmd processing result status */
  UWord8 status;
} wfw_qsh_dump_sw_cmd_trace_s;

/***************************************************************************/
/*                   Top level WFW QSH dump structure                      */
/***************************************************************************/

typedef struct
{
  wfw_qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */

  /* WFW main state */
  UWord32 state;

  /* Cmd proc trace buffer */
  wfw_qsh_dump_sw_cmd_trace_s swCmdProcTraceBuf[2];                /* 184 bytes */

  /* Common info struct */
  wfw_qsh_dump_common_s commonInfo;                                /* 36 bytes */

  /* Resource information */
  wfw_qsh_dump_cell_s cellInfo[WFW_DEMOD_NUM_MAX_CELLS];         /* 56 bytes */
  wfw_qsh_dump_fing_s fingInfo[WFW_DEMOD_NUM_MAX_FINGERS];       /* 96 bytes */
  wfw_qsh_dump_chan_s chanInfo[WFW_DEMOD_NUM_MAX_PHCHS];         /* 124 bytes */

  /* QICE info database */
  wfw_qsh_dump_qice_s eqInfo[WFW_DEMOD_NUM_MAX_EQS];              /* 36 bytes */

  /* Common info database */
  WfwDemodCommonInfoStruct demodCommonInfo[WFW_DEMOD_NUM_MAX_CARRIERS]; /* 12 bytes */

  /* RXFE info database */
  wfw_qsh_dump_rxfe_s rxfeInfo[WFW_RXAGC_MAX_ANTENNAS][WFW_DEMOD_NUM_MAX_CARRIERS];/* 160 bytes */

  /* TX info database */
  wfw_qsh_dump_tx_s txInfo;                                      /* 16 bytes */

  /* Search info database */
  wfw_qsh_dump_srch_s srchInfo;                                  /* 8 bytes */

  /* DSDS info database */
  wfw_qsh_dump_dsds_s dsdsInfo;                                  /* 4 bytes */

} wfw_qsh_dump_s;

/***************************************************************************/
/*                   TLV structure                                         */
/***************************************************************************/

/* WFW QSH dump enum */
typedef enum
{
  WFW_QSH_DUMP_TAG_MINI = 0

} wfw_qsh_dump_tag_e;

/* WFW QSH dump TLV structure */
typedef struct
{
  /* Tag value */
  uint16           tag_value;

  /* Tag data size */
  uint16           tag_data_size;

  /* QSH dump */
  wfw_qsh_dump_s   wfw_qsh_dump;

} wfw_qsh_dump_tag_mini_s;

#endif /* WFW_QSH_EXT_H_ */



