
#ifndef RFLTE_MC_FTM_TXAGC_H
#define RFLTE_MC_FTM_TXAGC_H

/*!
  @file rflte_mdsp.c 

  @brief
  This file contains the MDSP HAL for the LTE technology.



*/

/*===========================================================================

  Copyright (c) 2009 - 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================


								EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_mc_ftm_txagc.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/22/15   sg      CA_NS based MPR/AMPR computation
02/04/15   jf      64QAM initial support
01/19/15   yzw     Added carrier idx as parameter into FTM TxAGC APIS
09/02/13   nrk     Add FTM single Tx UL CA support
08/12/13   bsh     FTM: LTE Fixed Vector Tx
04/05/13   php     Enable FW HDET and Temp Comp loops and remove ISR 
06/28/11   can     Initial version. 


===========================================================================*/

/*===========================================================================

									INCLUDE FILES

===========================================================================*/
#include "msg.h"
#include "rfcom.h"
#include "rffw_lte_intf.h"


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*!
   @brief
   The structure maintains state variables for FTM LTE CORE TX AGC for each Tx Cell.

   @details
   
*/
typedef struct
{

  /*! @brief Boolean Flag, to specify the Initialization is complete*/
  boolean init_done;

  /*! @brief Critical section lock for all LTE FTM Tx Activities */
  rf_lock_data_type lock;

  /*! @brief Maintains the state of the Tx Modulator if it is ON/OFF */
  boolean modulator_on;
  
  /*! @brief Maintains the type of Modulation for the LTE FTM Tx Waveform */
  rflm_lte_mod_type_s mod_type;

  /*! @brief Maintains the RB Block size */
  uint8 rb_block;

  /*! @brief Maintains the start number of the RB Block */
  uint8 rb_start;

  /*! @brief Maintains the NS Back off NS_x */
  uint8 ns_x;

  /*! @brief Maintains if NS_type */
  uint8 ns_type;

  /*! @brief Maintains the state of TxAGC if TXAGC is enabled or disabled */
  boolean txagc_enabled;

  /*! @brief Maintains the TXAGC Value */
  int16 txagc_val;

  /*! @brief Maintains the TXAGC MTPL Value  */
  int16 mtpl_val;
  
}rflte_ftm_core_txagc_carrier_info_type;

/*!
   @brief
   The structure maintains state variables for FTM LTE CORE TX AGCl.

   @details
   
*/
typedef struct
{

  /*! @brief TXAGC State Variables per Tx Cell */
  rflte_ftm_core_txagc_carrier_info_type rf_txagc_path_state[RFA_RF_LTE_MAX_CELL_SUPPORTED];

}rflte_ftm_core_txagc_sm_type;

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/* ----------------------------------------------------------------------- */
void rflte_mc_ftm_txagc_enable (uint32 carrier_idx, boolean enable);
/* ----------------------------------------------------------------------- */
void rflte_mc_ftm_txagc_update_rb_allocation (uint32 carrier_idx, 
                                                        uint8 rb_block, 
                                                        uint8 rb_start);
/* ----------------------------------------------------------------------- */
void rflte_mc_ftm_txagc_update_mod_type (uint32 carrier_idx, rflm_lte_mod_type_s mod_type);
/* ----------------------------------------------------------------------- */
void rflte_mc_ftm_txagc_update_agc_val (uint32 carrier_idx, int16 txagc_val);
/* ----------------------------------------------------------------------- */
void rflte_mc_ftm_txagc_update_ns_x (uint32 carrier_idx, uint8 ns_x);
/* ----------------------------------------------------------------------- */
boolean rflte_mc_ftm_txagc_set_enc_to_fixed_vector(uint8 harq_idx, 
                                                   void *lte_req_pkt_ptr);
/* ----------------------------------------------------------------------- */
void rflte_mc_ftm_txagc_reset(void);
/* -----------------------------------------------------------------------*/
void rflte_mc_ftm_txagc_set_modulator_on (uint32 carrier_idx, boolean modulator_on);
/* -----------------------------------------------------------------------*/
#endif /*RFLTE_MC_FTM_TXAGC_H*/

