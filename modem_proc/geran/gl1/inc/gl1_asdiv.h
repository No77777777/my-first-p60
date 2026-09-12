#ifndef GL1_ASDIV_H
#define GL1_ASDIV_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      GERAN L1 ASDIV HEADER FILE

GENERAL DESCRIPTION
This module handles ASDIV specific functions.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gl1_asdiv.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when      who       what, where, why
--------  --------  ---------------------------------------------------------- 
29/10/17   km        CR2133917 GL1 support for TA.3.0 L+L
06/09/15   br        CR1063442 FR 38050: AsDiv in Acquisition for CSFB in multisimode.
03/11/15   ss        CR916273 FR 29852: AsDiv in Acquisition

===========================================================================*/

/*===========================================================================
           INCLUDE FILES FOR MODULE
===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "ms.h"
#include "gl1_sys_algo.h"
#include "l2_l1.h"
#include "l2_l1_g.h"
#include "msg.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "geran_nv.h"
#include "gl1_msg_g.h"

/** AsDiv Debug Header for GL1 */
#define GL1_ASDIV_HDR "GL1_AsDIV: "

/*===============================================================================*/
/* FR 29852: Acquisition Antenna Switch Diversity (ACQ ASDIV)*/
/*===============================================================================*/
#ifdef FEATURE_GL1_ASDIV_ACQ 

/** 
   GSM850 Band has minimum channels (123),
   intention is to avoid significant increase in Normal PScan
   Time and Also avoid MultiBand Rescans, so 122 is chosen here 
   */ 
#define GL1_ASDIV_ACQ_MAX_FREQ_LIST_LEN 122

/**Power Meas params for BCCH Probes which will be done in 
 * parallel with BCCH Decode */ 
#define GL1_ASDIV_ACQ_NUM_MON_BCCH 4
/** Monitors/Probes per frame for BCCH*/
#define GL1_ASDIV_ACQ_MPF_BCCH     1
/**Number of frames to monitor, are same as Number of Monitors for BCCH*/
#define GL1_ASDIV_ACQ_NUM_FRM_BCCH GL1_ASDIV_ACQ_NUM_MON_BCCH

/**Total Power monitors After PScan will be 6, 3 on Current 
 * Antenna and 3 on Other Antenna */ 
#define GL1_ASDIV_ACQ_NUM_MON_PROBE 6
/**Number of Monitors per frame are same as Number of Monitors after PScan*/
#define GL1_ASDIV_ACQ_MPF_PROBE GL1_ASDIV_ACQ_NUM_MON_PROBE
/** Number of Frames to do Power/Probe meas. After PScan */
#define GL1_ASDIV_ACQ_NUM_FRM_PROBE 1

/** Max. Power Mon to be programmed for init. the AGC ptr
 *  array, one more than max (6 in case of Power Meas. case, So
 *  7 here) which we will be programming */
#define GL1_ASDIV_ACQ_MAX_MEAS 7

/** Various Events in AsDiv Acquisition i.e Events Wherein
 *  Acq AsDiv Decisions are to be made */
typedef enum
{
    GL1_ASDIV_ACQ_EV_NONE,
    GL1_ASDIV_ACQ_EV_PSCAN_END, /*After PScan Ends*/
    GL1_ASDIV_ACQ_EV_PROBE,     /*Probing After PScan*/
    GL1_ASDIV_ACQ_EV_RESCAN,    /*ReScanning for Acq DB Scan*/
    GL1_ASDIV_ACQ_EV_BCCH,      /*BCCH Decode, so have probes during the same*/
    GL1_ASDIV_ACQ_EV_BCCH_END,  /*At the end of BCCH Decode Attempt*/
    GL1_ASDIV_ACQ_EV_INVALID
}gl1_asdiv_acq_event_t;

/** Structure holding a copy of all reqd decision items for
 *  AsDiv Acquisition*/
typedef struct
{
    gl1_asdiv_acq_event_t gl1_acq_asdiv_event;
    uint16                num_freq_valid;
    uint16                freq_list_length;
    ARFCN_T               best_arfcn;
}gl1_asdiv_acq_buff_t;

/**Power Meas Structure for AsDiv Acquisition*/ 
typedef struct
{
    ARFCN_T                      ASDIV_ARFCN[GL1_ASDIV_ACQ_MAX_MEAS];
    sys_algo_agc_T               asdiv_agc[GL1_ASDIV_ACQ_MAX_MEAS];
    sys_algo_agc_T               *asdiv_agc_ptr[GL1_ASDIV_ACQ_MAX_MEAS];
    gl1_msg_pwr_meas_result_type asdiv_acq_pwr_probe_cb_param;
    dBx16_T                      result_buffer[GL1_ASDIV_ACQ_MAX_MEAS];
}gl1_asdiv_acq_pwr_meas_buff_t;

/*==============FUNCTION DECLARATIONS==============*/

boolean gl1_acq_asdiv_band_supported(ARFCN_T arfcn,gas_id_t gas_id);

boolean gl1_asdiv_acq_enabled( gas_id_t gas_id);

boolean gl1_asdiv_acq_check_rescan(gas_id_t gas_id);

boolean gl1_asdiv_acq_is_mon_active(gas_id_t gas_id);

void gl1_asdiv_acq_update_params(gl1_asdiv_acq_event_t event,
                                 ARFCN_T best_arfcn,
                                 uint16 num_freq_valid,
                                 uint16 freq_list_length, 
                                 gas_id_t gas_id);

void gl1_asdiv_acq_state_tick(gas_id_t gas_id);

void gl1_asdiv_acq_probe_callback(gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id);

void gl1_asdiv_acq_probe_abort_callback(gas_id_t gas_id);

void gl1_asdiv_acq_substate_exit(gas_id_t gas_id);

void gl1_asdiv_acq_do_pwr_meas(gl1_asdiv_acq_event_t event, gas_id_t gas_id);

void gl1_asdiv_acq_reset_event(gas_id_t gas_id);

void gl1_asdiv_acq_reset(gas_id_t gas_id);

void gl1_asdiv_acq_bcch_mon_cb(gl1_msg_pwr_meas_result_type *result_data, gas_id_t gas_id);

void gl1_asdiv_acq_bcch_mon_abort_cb(gas_id_t gas_id);

void gl1_asdiv_reset_antenna_to_use(void);

#endif /*FEATURE_GL1_ASDIV_ACQ*/

#endif /* GL1_ASDIV_H */
