#ifndef RR_G2W_HO_H
#define RR_G2W_HO_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   RR GSM To WCDMA Handover Module

GENERAL DESCRIPTION
   This module performs the handshaking between RR and the other layers when
   performing a handover to WCDMA

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_g2w_handover.h#2 $ $DateTime: 2020/01/27 04:29:28 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
05/05/09    sjw     Reworked interfaces between G2G and G2W handover code
01/22/03    JAC     Started 

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_WCDMA

#include "comdef.h"
#include "rr_defs.h"
#include "ms.h"
#include "rr_general.h"
#include "rr_l1_send.h"
#include "rr_mm_send.h"
#include "rr_l2_send.h"
#include "rr_mode.h"
#include "rr_rrc_send.h"
#include "geran_multi_sim.h"

extern rr_event_T rr_g2w_handover_control(
  rr_event_T g2w_handover_event,
  rr_cmd_bdy_type *message_ptr,
  const gas_id_t gas_id
);

extern void rr_prepare_rr_rrc_interrat_measurement_data(
       rr_rrc_interrat_measurement_data_type *rr_rrc_interrat_meas_data, 
       rr_ded_irat_measurements_t  *wcdma_ded_meas,
       wcdma_neighbor_cell_list_T *wcdma_neighbor_cell_list
       );

extern byte rr_g2w_map_handover_failure_cause(
  const gas_id_t gas_id
);

extern void rr_g2w_handover_discard_data(
  const gas_id_t gas_id
);

extern void rr_g2w_handover_task_start_init(void);

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
extern void grr_mdump_rr_g2w_handover(const gas_id_t gas_id);
#endif /* FEATURE_QSH_MDUMP */
#endif /* FEATURE_WCDMA */
#endif /* #ifndefRR_G2W_HO_H */

/* EOF */

