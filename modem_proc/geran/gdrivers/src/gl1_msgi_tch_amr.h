#ifndef GL1_MSGI_TCH_AMR_H
#define GL1_MSGI_TCH_AMR_H

/*========================================================================
       G L 1  M E S S A G E  L A Y E R -  T C H   A M R
                            HEADER FILE
DESCRIPTION
   This file defines the variables shared by the adaptive multirate 
   traffic channel modules.
   
Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
========================================================================== */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msgi_tch_amr.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
20/06/13   cs       Major Triton TSTS Syncup
11/07/05   gfr      Move prototypes for loopback processing here
07/29/05   gfr      DTM support
04/08/05   gfr      Combined GSM and GPRS channel types
08/18/04   yhong    Initial version. Variables shared in by tch_amr files
===========================================================================*/

#include "geran_variation.h"
#include "customer.h"

#include "gl1_msgi_tch.h"

#include "geran_dual_sim.h"

extern boolean       amr_loopback_sid_update_saved[NUM_GERAN_DATA_SPACES];
extern boolean       gl1_msg_loopback_I_prev_block_was_facch[NUM_GERAN_DATA_SPACES];

void amr_handle_loopback_fr (tch_facch_decode_rpt *rpt,gas_id_t gas_id );
void amr_handle_loopback_hr (tch_facch_decode_rpt *rpt,gas_id_t gas_id );

#endif
