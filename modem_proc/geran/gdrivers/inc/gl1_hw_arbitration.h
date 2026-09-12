#ifndef GL1_HW_ARBITRATION_H
#define GL1_HW_ARBITRATION_H
/*========================================================================
                      GSM GPRS NPL1 HW LAYER ARBITRATION
DESCRIPTION
   This module defines the arbitration interface exposed by the the GSM/GPRS
   frame layer. The interface allows an external arbitration component
   to be installed in the GSM/GPRS frame layer and perform Rx and Tx
   arbitration against external access stratums.

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
========================================================================== */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/inc/gl1_hw_arbitration.h#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
19/08/15   mc        CR874662 : CXM Freq ID support for X2G startup and X2G preload
17/06/15   mc        CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
13/03/15   mc        CR773788 : COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
03/07/14   og        CR652720. L2G DSDA bring-up updates.
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS
26/03/14   ab        CR642231 : COEX : GL1 Arbitrator to allow upto 10 Monitor Freq required for X2G to prevent Invalid Freq Id
                     and possible memory corruption
21/03/14   mc        CR.588853 : COEX : GL1 changes for Enhanced FCCH algorithm
05/03/14   mc        CR.583282 : Limit the number of PSCAN monitors per frame to that which can be COEX protected in X+G DSDA
04/11/13   mc        CR.556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
05/11/13   js        CR.555528 : COEX : CXM Priority and Freq ID for Async Rx
01/10/13   sk        CR552259 CR sync from 1.0/1.1
23/09/13   sk        2.0 syncup from 1.1
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
========================================================================== */

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "geran_dual_sim.h"

typedef enum {ARBITRATE_RX, ARBITRATE_TX, ARBITRATE_MONITORS, ARBITRATE_ACQ, ARBITRATE_SCH, ARBITRATE_ASYNCRX, ARBITRATE_X2G} gl1_hw_arbitrated_event_t;

typedef void ( *gl1_hw_arbitration_reg_t )(boolean, gas_id_t);
typedef void ( *gl1_hw_arbitration_notify_t )(gl1_hw_arbitrated_event_t, gas_id_t);

#define MAX_MONITOR_NOZONES 2

#define MAX_MEAS_PER_FRAME_INC_IRAT  MAX_CM_RSSI_MEASUREMENTS

typedef struct
{
  boolean                       configured;
  gl1_hw_channel_type           channel_type;
  uint32*                       priority;
  uint32*                       desense_id;
  uint8                         burst_num;
} gl1_hw_arbitrated_rx_slot_t;

typedef struct
{
  boolean                       configured;
  gl1_hw_channel_type           channel_type;
  uint32*                       priority;
  uint32*                       desense_id;
  int16*                        backoff_desense;
  int16*                        backoff_vbatt;
  uint8                         burst_num;
  geran_pdu_priority_t          pdu_prio;
} gl1_hw_arbitrated_tx_slot_t;

typedef struct
{
  ARFCN_T                       arfcn;
  uint32                        search_width;
  gl1_hw_arbitrated_rx_slot_t   slots[GL1_DEFS_SLOTS_IN_FRAME];
} gl1_hw_arbitrated_rx_t;

typedef struct
{
  ARFCN_T                       arfcn;
  uint16                        timing_advance;
  int16                         power;
  gl1_hw_arbitrated_tx_slot_t   slots[GL1_DEFS_SLOTS_IN_FRAME];
} gl1_hw_arbitrated_tx_t;

typedef struct
{
  ARFCN_T                       arfcn;
  int32                         qs_position;
  uint32                        priority;
  uint32                        desense_id;
  uint32                        skip;
} gl1_hw_arbitrated_monitor_t;

typedef struct
{
  ARFCN_T*                      arfcns;
  gl1_hw_arbitrated_monitor_t   monitor[MAX_MEAS_PER_FRAME_INC_IRAT];
  int32                         count;
  int32                         rf_setup_time;
  int32                         guard_period;
  int32                         monitor_duration;
  boolean                       l2g_high_priority;
  int32                         max_per_frame;
  int32                         nozone_start[MAX_MONITOR_NOZONES];
  int32                         nozone_end[MAX_MONITOR_NOZONES];
  int32                         nozone_count;
} gl1_hw_arbitrated_monitors_t;

typedef struct
{
  ARFCN_T                       arfcn;
  boolean                       enhanced_fcch_detection;
  uint32                        priority;
  uint32                        desense_id;
} gl1_hw_arbitrated_acq_t;

typedef struct
{
  ARFCN_T                       arfcn;
  uint16                        timing_offset;
  uint32                        priority;
  uint32                        desense_id;
} gl1_hw_arbitrated_sch_t;

typedef struct
{
  ARFCN_T                       arfcn;
  uint16                        timing_offset;
  uint32                        priority;
  uint32                        desense_id;
} gl1_hw_arbitrated_asyncrx_t;

typedef struct
{
  ARFCN_T                       arfcn;
  boolean                       interband_monitors;
  uint32                        activity_priority;
  uint32                        no_activity_priority;
  uint32                        desense_id;
} gl1_hw_arbitrated_x2g_t;


typedef struct
{
  gl1_hw_arbitrated_rx_t        rx;
  gl1_hw_arbitrated_tx_t        tx;
  gl1_hw_arbitrated_monitors_t  monitors;
  gl1_hw_arbitrated_acq_t       acq;
  gl1_hw_arbitrated_sch_t       sch;
  gl1_hw_arbitrated_asyncrx_t   async_rx;
  gl1_hw_arbitrated_x2g_t       x2g;

  gl1_hw_arbitration_notify_t   notify_event;

  gl1_hw_arbitration_reg_t      registration;
  gl1_hw_arbitration_reg_t      deregistration;


} gl1_hw_arbitration_t;

gl1_hw_arbitration_t* gl1_hw_get_arbitration(gas_id_t gas_id);

void gl1_hw_arbitration_init(gas_id_t gas_id);

void gl1_hw_cfg_arbitration(gl1_hw_arbitration_reg_t registration, gl1_hw_arbitration_reg_t deregistration, gas_id_t gas_id);

#endif /* GL1_HW_ARBITRATION_H */
