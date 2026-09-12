#ifndef TDSULLOGPKT_H
#define TDSULLOGPKT_H

/*============================================================================
              T D S U L L O G P K T    H E A D E R   F I L E

DESCRIPTION
This module contains the function prototypes of Uplink logging packetk.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000 - 2011 Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsullogpkt.h#1 $ 
  $DateTime: 
  $Author: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/31/11   sk     File created.
                    
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                        Function Prototypes                              */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/*===========================================================================
FUNCTION        tdsullogpkt_log_dch_cfg_pkt

DESCRIPTION     This function logs DCH transport channel info.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_dch_cfg_pkt(void);

/*===========================================================================
FUNCTION        tdsullogpkt_log_dch_gain_factor_cfg_pkt

DESCRIPTION     This function logs DCH gain factor info.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_dch_gain_factor_cfg_pkt(void);

/*===========================================================================
FUNCTION        tdsullogpkt_log_rach_cfg_pkt

DESCRIPTION     This function logs RACH transport channel info.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_rach_cfg_pkt(void);

/*===========================================================================
FUNCTION        tdsullogpkt_log_prach_fpach_cfg_pkt

DESCRIPTION     This function logs PRACH/FPACH physical channels info.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_prach_fpach_cfg_pkt(void);

/*===========================================================================
FUNCTION        tdsullogpkt_log_random_access_cfg_pkt

DESCRIPTION     This function logs UPPCH physical channels info.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_random_access_cfg_pkt(void);

/*===========================================================================
FUNCTION        tdsullogpkt_submit_tfc_cfg_pkt

DESCRIPTION     This function logs the Uplink DCH transport format combinations.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_submit_tfc_cfg_pkt (void);

/*===========================================================================
FUNCTION        tdsullogpkt_submit_dpch_cfg_pkt

DESCRIPTION     This function logs the uplink dedicated physical channels 
                configuration information

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_submit_dpch_cfg_pkt (void);

/*===========================================================================
FUNCTION        tdsullogpkt_log_fw_uppch_fpach_cfg_pkt

DESCRIPTION     This function logs the UPPCH/FPACH configuration information
                that sent to FW for one random access attempt.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_fw_uppch_fpach_cfg_pkt
(
  tfw_uppch_fpach_config_t *cfg_ptr
);

/*===========================================================================
FUNCTION        tdsullogpkt_log_fw_prach_cfg_pkt

DESCRIPTION     This function logs the PRACH configuration information
                that sent to FW for RACH transmission.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_fw_prach_cfg_pkt
(
  tfw_prach_config_t *cfg_ptr
);

/*===========================================================================
FUNCTION        tdsullogpkt_log_fw_dpch_cfg_pkt

DESCRIPTION     This function logs the DPCH configuration information
                that sent to FW for Tx, SB or DTX.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_fw_dpch_cfg_pkt
(
  tfw_ul_dpch_config_t *cfg_ptr
);

/*===========================================================================
FUNCTION        tdsullogpkt_log_fpach_decode_result_pkt

DESCRIPTION     This function logs the FPACH decode result that sent by
                FW via FPACH_DECODE_DONE_ISR.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_fpach_decode_result_pkt
(
  tfw_fpach_result_buf_t *fpach_result_ptr
);

/*===========================================================================
FUNCTION        tdsullogpkt_log_uppch_pwr_timing_cfg_pkt

DESCRIPTION     This function logs the UpPCH power timing contorl message.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_uppch_pwr_timing_cfg_pkt
(
  tfw_uppch_tx_pwr_timing_config_cmd_t *pwr_timing_cfg_ptr
);

/*===========================================================================
FUNCTION        tdsullogpkt_log_dpch_pwr_timing_cfg_pkt

DESCRIPTION     This function logs the DPCH power timing contorl message.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_dpch_pwr_timing_cfg_pkt
(
  tfw_dpch_tx_pwr_timing_config_cmd_t *pwr_timing_ptr
);

/*===========================================================================
FUNCTION        tdsullogpkt_log_erucch_si_data_pkt

DESCRIPTION     This function logs the ERUCCH payload.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_erucch_si_data_pkt
(
  uint8 *erucch_data_ptr
);

/*===========================================================================
FUNCTION        tdsullogpkt_log_fw_erucch_cfg_pkt

DESCRIPTION     This function logs the erucch configuration information
                that sent to FW for ERUCCH transmission.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsullogpkt_log_fw_erucch_cfg_pkt
(
  tfw_prach_config_t *cfg_ptr
);

#endif

#endif /* TDSULLOGPKT_H */
