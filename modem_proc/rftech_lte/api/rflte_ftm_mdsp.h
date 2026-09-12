
#ifndef RFLTE_FTM_MDSP
#define RFLTE_FTM_MDSP

/*!
  @file rflte_ftm_mdsp.c 

  @brief
  This file contains the MDSP HAL for the LTE technology in FTM mode



*/

/*===========================================================================

  Copyright (c) 2011-2015 QUALCOMM Technologies Incorporated. All Rights Reserved
  
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

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_ftm_mdsp.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/16/15   JJ      Use TXLM CA CONFIG msg in intra_band ULCA mode
02/12/15   JJ      Added an api to pass phychan deconfig msg to FW for disable SCC 
01/19/15   yzw     Added carrier idx as parameter into FTM TxAGC related APIS
01/16/15   JJ      Fixed a compilation warning
01/14/15   JJ      Added rflte_ftm_tx_waveform_info_type type
12/30/14   JJ      Added all changes for intra_band ULCA in Cal mode
07/31/14   ndb     Added support for extended EARFCN for LTE
05/30/14   gvn     Fix LTE clean up after calibration 
05/28/14   kab     Callback APIs implemented for RxAGC based FW commands.
03/07/13   gvn     Fixes for CA-LTE FTM IRAT
12/30/13   pl      added L2L FTM IRAT support 
12/20/13   pl      added timing information for RXAGC start/stop
12/20/13   pl      remove redundant interface
12/05/13   pl      Added RXAGC modes for IRAT suspend and resume
08/29/13   svi     Changes to support UL CA
04/18/13   vss     Added support for Dynamic SCELL switch
04/29/13   cri     Band change without reset after DPD IQ capture
03/01/13   bsh     FTM IRAT: Copy rx_gain_ctrl from rxagc_ncell_ca_enb to rxagc: new function
10/19/12   pl      Update RXAGC fucntion to make re-entrant
08/27/12   pl/sbm  Added carrier index for CA support
03/24/12   yzw     Added FTM command FTM_LTE_SET_FREQ_ADJUST, FTM_LTE_ENABLE_FREQ_CORRECTION and FTM_LTE_DISABLE_FREQ_CORRECTION 
02/07/12   can     Added FTM set modulation command.
02/07/12   aca     LTE Dac calibration support
10/28/11   whc     Added driver API to support 'FTM GET RX LEVEL DBM' command
05/11/11   pl      Added support for FW RXLM/TXLM command  
04/19/11   pl      Initial version. 


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES

===========================================================================*/
#include "msg.h"
#include "rfcom.h"
#include "lm_types.h"
#include "rf_lte_ftm_msg.h"
#include "rflte_msg.h"

/*===========================================================================

                     EXTERNAL INTERFACES

===========================================================================*/
typedef enum
{
   RXAGC_ENABLE,
   RXAGC_DISABLE,
   SAMPLE_REC_DISABLE,
   RXAGC_IRAT_SUSPEND,
   RXAGC_IRAT_RESUME,
   RXAGC_IRAT_START,
   RXAGC_IRAT_SUSPEND_SCELL
}rxagc_action_type;

typedef struct
{
   uint32 carrier_idx;
   rfcom_lte_band_type band;
   uint16 rx_chan;
   rfcom_lte_bw_type rx_bw;
}rflte_ftm_carrier_info_irat_type;

typedef struct
{
   byte waveform;
   byte num_rbs_pusch;
   byte num_rbs_pucch;
   byte start_pusch_rb_index;
   uint32 transp_block_size_A;
   rfcom_lte_band_type lte_band;
}rflte_ftm_tx_waveform_info_type;

/* ----------------------------------------------------------------------- */
void rflte_ftm_mdsp_enter(void);

/* ----------------------------------------------------------------------- */
void rflte_ftm_mdsp_exit(void);

/* ----------------------------------------------------------------------- */
/* Semi Stat cnf */
void rflte_ftm_mdsp_semi_stat_cnf_received(void);

/* ----------------------------------------------------------------------- */
/* SCELL cnf */
void rflte_ftm_mdsp_scell_cnf_received(void);

/* ----------------------------------------------------------------------- */
/* sys config cnf */
void rflte_ftm_mdsp_sys_config_cnf_received(void);

/* ----------------------------------------------------------------------- */
/* RxAGC cfg Param cnf */
void rflte_ftm_mdsp_rxagc_cfg_param_cnf_received(void);

/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_time_slam(void);
boolean rflte_ftm_mdsp_time_slam_new(uint32 carrierIdx, rfcom_lte_bw_type bandwidth);

/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_rxagc_start_stop
(
   uint32 carrier_idx,
   rxagc_action_type action,
   rfcom_lte_band_type band,
   rfcom_lte_earfcn_type rx_chan,
   rfcom_lte_earfcn_type tx_chan,
   rfcom_lte_bw_type ftm_current_lte_rx_bw,
   uint32 action_time
   );
boolean rflte_ftm_mdsp_rxagc_start_stop_new
(
   uint32 carrier_idx,
   rxagc_action_type action,
   rfcom_lte_band_type band,
   rfcom_lte_earfcn_type rx_chan,
   rfcom_lte_earfcn_type tx_chan,
   rfcom_lte_bw_type ftm_current_lte_rx_bw,
   uint32 action_time,
   boolean is_ho_rxd_enabled
   );
/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_dl_ul_start_config
(
   uint32 carrier_idx,
   rfcom_lte_band_type band,
   rfcom_lte_earfcn_type rx_chan,
   rfcom_lte_earfcn_type tx_chan,
   rfcom_lte_bw_type rx_bw,
   rfcom_lte_bw_type tx_bw
   );

boolean rflte_ftm_mdsp_dl_ul_start_config_new
(
   uint32 carrier_idx,
   rfcom_lte_band_type band,
   rfcom_lte_earfcn_type rx_chan,
   rfcom_lte_earfcn_type tx_chan,
   rfcom_lte_bw_type rx_bw,
   rfcom_lte_bw_type tx_bw,
   boolean is_ho_rxd_enabled
   );
/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_config_ul_phychan(uint8 carrier_mask, rflte_ftm_tx_waveform_info_type *tx_waveform_info);
/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_start_ul_transmission(uint32 carrier_idx,
                                             byte waveform,
                                             byte num_rbs_pusch,
                                             byte num_rbs_pucch,
                                             byte start_pusch_rb_index,
                                             uint32 transport_block_size_A);

/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_stop_ul_transmission(uint32 carrier_idx);

/* ----------------------------------------------------------------------- */
void rflte_ftm_mdsp_config_rxfe(uint32 carrier_idx,
                                lm_handle_type rxlm_prx_buf_idx,
                                lm_handle_type rxlm_drx_buf_idx);

/* ----------------------------------------------------------------------- */
void rflte_ftm_mdsp_config_tx(uint8 carrier_mask, lm_handle_type *txlm_buf_idx);
/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_send_tx_dac_cal_cmd(boolean ant0, boolean ant1);
/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_chan_modulation(rfa_rf_lte_ftm_ul_modulation_type requested_modulation);
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_slam_rot_start_freq_corr(int32 freq_err);

/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_copy_rx_gain(rfm_device_enum_type device, uint32 carrier_idx);

/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_rxagc_start_stop_ca_lte
(
   rflte_ftm_carrier_info_irat_type *irat_carriers,
   uint8 num_carriers,
   rxagc_action_type action,
   uint32 action_time
   );
/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_rxagc_sys_config_app_ca_lte
(
   rflte_ftm_carrier_info_irat_type *irat_carriers,
   uint8 num_carriers,
   rxagc_action_type action,
   uint32 action_time
   );
boolean rflte_ftm_mdsp_exit_lte_sys_config_app(void);
/* ----------------------------------------------------------------------- */
boolean rflte_ftm_mdsp_deconfig_ul_phychan(uint32 carrier_idx);
/* ----------------------------------------------------------------------- */
#endif /*RFLTE_MDSP*/
