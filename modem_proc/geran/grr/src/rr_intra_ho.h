#ifndef RR_INTRA_HO_H
#define RR_INTRA_HO_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               rr_intra_ho.h

GENERAL DESCRIPTION
   This module contains functions for dealing with intracell handoffs.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_intra_ho.h#2 $

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "sys_cnst.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "mm_rr.h"
#include "timers.h"
#include "rr_defs.h"
#include "geran_multi_sim.h"

/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

rr_event_T rr_intra_cell_handover_control(
  rr_event_T,
  void *,
  const gas_id_t gas_id
);

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
extern void grr_mdump_rr_intra_ho(const gas_id_t gas_id);
#endif /* FEATURE_QSH_MDUMP */

#endif
