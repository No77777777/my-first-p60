#ifndef LOGOD_H
#define LOGOD_H
/*===========================================================================

                    L O G O D . H
DESCRIPTION
  This file contains data types and prototypes for log on demand.
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  

  Copyright (c) 2000, 2001, 2002 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/logod.h_v   1.3   26 Jun 2002 16:07:48   msatyana  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/logod.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/28/14    sr      Initial checkin for 0x187B WCDMA power log pkt
05/29/2002  m       Initial release to support LOD

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "l1def.h"
#include "log.h"
#include "log_codes_wcdma_int.h"
#include "dlcctrchlog.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#ifdef FEATURE_L1_LOG_ON_DEMAND
typedef struct
{
  /* generic command header */
  l1_local_cmd_hdr_type hdr;
  log_code_type         log_on_demand_code;
} l1_log_on_demand_type;
#endif

extern boolean wl1_pwr_logpkt_due;

typedef l1_log_hdr_struct_type wl1_pwr_logpkt_log_hdr_struct_type;

typedef struct
{
  uint8 num_scch_decode_attempted[WL1_MAX_NUM_CARRIER_IDX];
  uint8 num_scch_valid[WL1_MAX_NUM_CARRIER_IDX];
}wl1_power_logpkt_sched_rate_struct_type;

typedef struct
{
  boolean cm_enabled;
  boolean deocder_type;
  uint8 fet_success_slots;
  wl1_power_logpkt_sched_rate_struct_type sched_rate;
}wl1_power_logpkt_internal_db_struct_type;

typedef struct
{
  boolean schic[WL1_MAX_NUM_CARRIER_IDX];
  boolean qice_enabled[WL1_MAX_NUM_CARRIER_IDX];
  uint8 qice_num_cells[WL1_MAX_NUM_CARRIER_IDX];
  uint8 qice_num_iter[WL1_MAX_NUM_CARRIER_IDX];
}wl1_power_logpkt_qice_info_struct_type;

LOG_RECORD_DEFINE(WCDMA_PHY_LAYER_POWER_LOG_PACKET)
  /* Version number */
  uint8 version_num;

  /* USTMR Ref time */
  uint64 ustmr_ref_time;

  /* Number of assigned fingers */
  uint8 num_fings;

  /*
  num_aset_cells_num_DL_carr
  BIT [0..4]:  Num ASET cells
  BIT [5..7]:  Num carriers in DL
  */
  uint8 num_aset_cells_num_DL_carr;


  /*
  numBands_dec_TTI_EDRX_FDPCH_rxd_schic
  BIT 0:        0 for Single, 1 for dual band
  BIT 1:        1 for TURBO, 0 for CONV decoder
  BIT [2..3]:  0/2/10, 0 means no HSUPA. 
  BIT 4:        1 for EDRX enabled, Otherwise 0
  BIT 5:        1 for FDPCH enabled, Otherwise 0
  BIT 6:        1 for RxD enabled, Otherwise 0
  BIT 7:        1 for SCHIC enabled, Otherwise 0
  */
  uint8 numBands_dec_TTI_EDRX_FDPCH_rxd_schic;

  /*
  rxd_txd_CM_HS_FETsuccessSlots_bmask
  BIT 0:        1 if RxD is enabled, Otherwise 0
  BIT 1:        1 if TxD is enabled, Otherwise 0
  BIT 2:        1 if CM is enabled, Otherwise 0
  BIT 3:        1 if HS is enabled, Otherwise 0
  */
  uint8 txd_CM_HS_bmask;

  /* FET RF off success slots */
  uint8 fet_rf_off_slots;

  /*
  Qice enable info per carrier
  BIT 0:        1 if QICE is enabled on C0, Otherwise 0
  BIT 1:        1 if QICE is enabled on C1, Otherwise 0
  BIT 2:        1 if QICE is enabled on C2, Otherwise 0
  */
  uint8 qice_enable_info;

  /*
  Num QICE iterations per carrier.
  */
  uint8 qice_num_iter[3];

  /*
  Num QICE cells per carrier.
  C0 - bit[0-1]; C1- bit[2-3]; C2 - bit[4-5]
  */
  uint8 qice_num_cells_per_carr_bmask;

  /* UPLINK power */
  int16 tx_agc_raw;

  /* RF envelope tracking mode */
  uint8 env_tracking;

  /*Scheduling rate*/
  uint8 num_scch_attempt[3];
  uint8 num_scch_valid[3];
LOG_RECORD_END


/* =======================================================================
**                        Function Declarations
** ======================================================================= */

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION  dl_cctrch_log_on_demand

DESCRIPTION
  This function receives log on demand request for CCTrCH information
  from QXDM and triggers log generation to QXDM.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
log_on_demand_status_enum_type handle_log_on_demand(log_code_type log_code);
#endif

/*===========================================================================
FUNCTION        wl1_pwr_logpkt_upd_cm_status

FILE NAME       logod.c

DESCRIPTION  This funtion updates internal db for CM status

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS 
===========================================================================*/
extern void wl1_pwr_logpkt_upd_cm_status(l1_cm_active_deactive_enum_type cm_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        wl1_pwr_logpkt_upd_fet_slots

FILE NAME       logod.c

DESCRIPTION  This funtion updates FET RF off slots

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS 
===========================================================================*/
extern void wl1_pwr_logpkt_upd_fet_slots(uint8 fet_enable, uint8 fet_slots, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        wl1_pwr_logpkt_upd_decoder_type

FILE NAME       logod.c

DESCRIPTION  This funtion updates decoder type

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS 
===========================================================================*/
extern void wl1_pwr_logpkt_upd_decoder_type(boolean dec_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        wl1_pwr_logpkt_register_client_cbs

FILE NAME       logod.c

DESCRIPTION  This funtion registers cbs with different modules

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS 
===========================================================================*/
extern void wl1_pwr_logpkt_register_client_cbs( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_pwr_logpkt_record_scch_stat
         
DESCRIPTION
  Function called by decoder to record SCCH statistics for each subframe

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_pwr_logpkt_record_scch_stat(boolean* scch_valid, boolean* scch_demod_attempted);

/*===========================================================================
FUNCTION        wl1_pwr_logpkt_fill_and_send

FILE NAME       logod.c

DESCRIPTION  This funtion fills and submits W power log pkt

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS 
===========================================================================*/
extern void wl1_pwr_logpkt_fill_and_send( wsub_id_e_type wsub_id);
#endif

