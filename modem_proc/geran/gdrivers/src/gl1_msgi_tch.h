#ifndef GL1_MSGI_TCH_H
#define GL1_MSGI_TCH_H

/*========================================================================
       G L 1  M E S S A G E  L A Y E R -  T R A F F I C   C H A N N E L
                            HEADER FILE
DESCRIPTION
   This file defines the variables shared by the traffic channel modules.
   
Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
========================================================================== */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msgi_tch.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
20/06/13   cs       Major Triton TSTS Syncup
04/08/05   gfr      Combined GSM and GPRS channel types
08/18/04   yhong    Initial version. Variables shared in by tch files
===========================================================================*/

#include "geran_variation.h"

#include "geran_dual_sim.h"

extern gl1_hw_channel_type current_chan[NUM_GERAN_DATA_SPACES];

#define MAX_LOOPBACK_OCTETS 38

extern gl1_defs_loopback_type loopback_type[NUM_GERAN_DATA_SPACES];
extern uint8 gl1_loopback_data[NUM_GERAN_DATA_SPACES][ MAX_LOOPBACK_OCTETS ];

#endif
