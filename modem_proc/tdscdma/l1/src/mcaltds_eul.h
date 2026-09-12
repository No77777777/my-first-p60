/* @file  
 * This module has definition and declaration related to MCAL
 * WCDMA EUL (EUL Uplink) module APIs
 */
 
/*======================================================================
               Copyright (c) 2008 Qualcomm Technologies Incorporated.
               All Rights Reserved.
               Qualcomm Confidential and Proprietary
======================================================================*/
/*======================================================================
                        EDIT HISTORY FOR MODULE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_eul.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when         who     what, where, why
----------   ---     ---------------------------------------------------
07/26/2013   yuw          CR499670 Fixed Triton peak TP issue
04/07/2013   yuw           CR472073 Triton HSUPA changes
01/21/2011   weijunz      Initial revision
======================================================================*/
#ifndef MCALTDS_EUL_H
#define MCALTDS_EUL_H

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "customer.h"
#include "comdef.h"
#include "tdseulsg.h"


/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define MCALTDS_EUL_MAX_EPUCH_TIME_SLOT_NUM     5 
/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/
typedef enum 
{
  MCALTDS_EUL_CHAN_DISABLE,
  MCALTDS_EUL_CHAN_ENABLE,
  MCALTDS_EUL_CHAN_RECONFIG,
  MCALTDS_EUL_CHAN_NO_ACTION
} mcaltds_eul_chan_action_enum_type;

typedef enum {
	MCALTDS_EUL_DEFAULT_MIDAMBLE,
    MCALTDS_EULL_UE_SPECIFIC_MIDAMBLE,
	MCALTDS_EUL_INVALID_MIDAMBLE
} mcaltds_eul_midamble_alloc_mode_enum_type;

typedef enum {
	MCALTDS_EUL_MIDAMBLE_CONFIG_2,
	MCALTDS_EUL_MIDAMBLE_CONFIG_4,
	MCALTDS_EUL_MIDAMBLE_CONFIG_6,
	MCALTDS_EUL_MIDAMBLE_CONFIG_8,
	MCALTDS_EUL_MIDAMBLE_CONFIG_10,
	MCALTDS_EUL_MIDAMBLE_CONFIG_12,
	MCALTDS_EUL_MIDAMBLE_CONFIG_14,
	MCALTDS_EUL_MIDAMBLE_CONFIG_16,
	MCALTDS_EUL_NUM_MIDAMBLE_CONFIG
} mcaltds_eul_midamble_config_enum_type;

typedef struct 
{
    uint8 ts_num ;
    mcaltds_eul_midamble_alloc_mode_enum_type alloc_mode ;
    mcaltds_eul_midamble_config_enum_type config ;
    uint8 shift ;

} mcaltds_eul_epuch_midamble_info_struct_type ;

typedef struct
{
    mcaltds_eul_chan_action_enum_type action ;
    uint8 ul_sync_step ;
    uint8 ul_sync_freq ;
    uint16 start_sub_fn ;
    uint8 num_ts ;
    mcaltds_eul_epuch_midamble_info_struct_type ts_cfg_list[MCALTDS_EUL_MAX_EPUCH_TIME_SLOT_NUM] ;

} mcaltds_eul_epuch_db_struct_type ;

#ifdef FEATURE_TDSCDMA_HSUPA_MK_TX
/* WAV_IB struct to be used for EPUCH Channel ENC Configuration */
typedef struct
{
  /*! \brief Array of Number of DATA1 & DATA2 Samples in array for Ch0/Ch1 Encoding*/
  uint16   data1_NumSamp;
  uint16   data2_NumSamp;
}mcaltds_eul_wavib_t;
#endif

typedef struct
{
    /*! \brief Time Slots that are used for E-PUCH.*/
  /** Valid : Bit[0..4] represents Time Slot [1..5]
      \li 1 : Time Slot is used for E-PUCH
      \li 0 : Time Slot is not used
      */
  uint8  timeSlotBitMask ;
  /*! \brief Number of time slots that contains E-PUCH. Valid [1..5] */
  uint8  numTs ;
  
  
  /*! \brief E-PUCH power in dbm. */
  int16  epuch_power;
  
  /*! \brief E-PUCH scheduled type. Valid[0,1] 0: non scheduled, 1: scheduled*/
  uint8  scheduleType;
   
  /** OVSF Code.
   *  For SF=1 Valid 1. For SF =2 Valid 1,2.
   *  For SF=4,valid 1,2,3,4. For SF = 8, valid
   *  [1,2,3,4,5,6,7,8]. For SF=16, valid [1...16] */
  uint8  chanCode ;
  /** Spreading factor for EPUCH. Valid value [1,2,4,8,16] */
  uint8  sf ;
  /*! \brief Absolute Midamble Shift to be used for E-PUCH in each slot[1..5] 
  Valid [0, 128/k, 2*(128/k), .., (k-1)*(128/k) ], where K is midamble 
  configuration [2,4,6,8,10,12,14,16] when the correspoding bit (slot) in 
  timeSlotBitMask above is set */
  uint8   midambleShift[MCALTDS_EUL_MAX_EPUCH_TIME_SLOT_NUM];

  /*! \brief EHICH Channel that is paired for this E-PUCH. Valid [0..3] for
  scheduled. Valid[4] for non-scheduled */
  uint8  ei ;

  uint8  eucchParamK ;
  /*! \brief Parameter used to calc number of instances for E-UCCH. */
  uint8  eucchParamL ;

  /*! \brief HARQ Process ID. Valid [0..3] */
  uint8  harqId ;
  /*! \brief Transport Block Size */
  uint8  tbs ;
  /*! \brief Retransmission Sequence Number. Valid [0..3] */
  uint8  rsn ;

  /*! \brief Modulation type: 0 (QPSK) or 1 (16QAM) */ 
  uint8 modu_type ;
  
  int32 epuch_power_Q9;

  /*! \brief FW write RmRAM Bank index flag, 0: SW write; 1:FW write */
  uint8   fwWriteRmramBankFlag;
  
  /*! \brief RmRAM Bank Index for next sub frame, 0 or 1 */
  uint8  rmramBankIdx;
  
  /*! \brief  5 LSB of sub frame number when this command is sent to FW */
  uint16  curr_sub_fn;
  /*! \brief value of register TX_TDS_EUL_MOD_CTL  */
  uint32 modCtlValue;

#ifdef FEATURE_TDSCDMA_HSUPA_MK_TX
  /*! \brief Array of Channel ENC Ping_Pong Flag for Ch0/Ch1 Encoding */
  uint8    encPingPongFlag;
  /*! \brief WAV_IB Channel Encoder for EPUCH configuration */
  mcaltds_eul_wavib_t   encConfig[TDSL1_UL_MAX_NUM_TS];
 #endif
} mcaltds_eul_epuch_tti_info_struct_type ;
/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/**
@brief Brief description of the function. This can be continued on the next
       line if needed. The first empty line signifies the end of the brief.
  
Put a detailed description of the function here. This should function as a
user's manual for your code, providing a developer everything that needs to
be known in order to use your code.
  
  @param  pFirstParameter: List the function parameters in columnar style.
@param  pNextParameter : Include detailed descriptions of the semantics of
                         each parameter.
  @see    Func2
@return Describe the return value of the function, and what it means to
        the caller.
*/
/*===========================================================================

FUNCTION MCALTDS_EUL_SEND_EPUH_CFG_CMD

DESCRIPTION     This function is the API to be called by the EUL Host layer to
                 configure EUL EPUCH channels.

DEPENDENCIES     R99 DPCH channel must be already setup and the Layer-1 must
                 be aligned to that already.

RETURN VALUE    boolean

SIDE EFFECTS    None

===========================================================================*/
boolean mcaltds_eul_send_epuch_cfg_cmd_msg(mcaltds_eul_epuch_db_struct_type *mcal_eul_info_ptr);


/*===========================================================================

FUNCTION MCALTDS_EUL_SEND_EPUH_CFG_CMD

DESCRIPTION     This function is the API to be called by the EUL Host layer to
                 send E-PUCH and E-UCCH info for next TTI E-DCH transmission.

DEPENDENCIES     R99 DPCH channel must be already setup and the Layer-1 must
                 be aligned to that already.

RETURN VALUE    boolean

SIDE EFFECTS    None

===========================================================================*/
boolean mcaltds_eul_send_epuch_tti_info_msg(mcaltds_eul_epuch_tti_info_struct_type *epuch_tti_info_ptr);

extern void mcaltds_eul_read_ul_pwr_info_from_fw(uint16 curr_sub_frame);

#endif /* #ifndef MCALTDS_EUL_H */

