#ifndef MCALTDS_MCVSCTRL_H
#define MCALTDS_MCVSCTRL_H

/*============================================================================*/
/** @file
 * This module has definition and declaration related to MCAL UMTS downlink
 * channel setup module APIs
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2007 - 2013 QUALCOMM Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_mcvsctrl.h#3 $
$DateTime: 2020/03/05 20:53:31 $
$Author: pwbldsvc $

when        who        what, where, why
--------    ---       --------------------------------------------------------
1/15/03     shouwenl   Initialize MCVS controller module

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/
/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "rex.h"
#include "queue.h"

#ifdef FEATURE_TDSCDMA_MCVS

#include "mcpm_api.h"
#include "tdsl1_nv.h"

#endif
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDS_DEFAULT_CMGR_PRIORITY 0
/* Enum type declaration */
/* --------------------- */
typedef enum
{
  TDS_VOL_SVS2       = 0x01,	            /* 1 */ 
  TDS_VOL_LOW_SVS    = 0x02,                /* 2 */
  TDS_VOL_SVS        = 0x03,	            /* 3 */
  TDS_VOL_SVS_PLUS   = 0x04,	            /* 4 */
  TDS_VOL_NOM        = 0x05,                /* 5 */
  TDS_VOL_TUB        = 0x06,	            /* 6 */
  TDS_VOL_TUB_SUP    = 0x07,                /* 7 */
  TDS_NUM_VOL
}mcaltds_mcvs_voltage_type;

typedef enum
{
  MCALTDS_MCPM_STANDBY = 0x01,
  MCALTDS_MCPM_ACQ,
  MCALTDS_MCPM_IDLE,
  MCALTDS_MCPM_VOICE,
  MCALTDS_MCPM_DATA,
  MCALTDS_MCPM_INIT,
  MCALTDS_NUM_MCPM_STATE
}mcaltds_mcpm_state;

typedef enum
{
 TDS_CHANNEL_NONE = 0,
  TDS_PCCPCH = 0x1,	        /* 1 */
  TDS_SCCPCH,                   /* 2 */
  TDS_DPCH,	                /* 3 */
  TDS_FPACH,                    /* 4 */
  TDS_EAGCH,                    /* 5 */
  TDS_EHICH,                    /* 6 */
  TDS_HSSCCH,                   /* 7 */
  TDS_HSPDSCH,                  /* 8 */
}mcaltds_mcvs_phy_channel_type;

typedef enum
{
  TDS_PHCHAN_DISABLE = 0, 
  TDS_PHCHAN_ENABLE  = 1, 
  TDS_PHCHAN_RECFG   = 2,
  TDS_PHCHAN_NO_OPS = 3,
}mcaltds_mcvs_phy_channel_ops_type;


typedef enum
{
  TDS_STEADY_NONE,                         // 0
  TDS_STEADY_MIN,                          // 1
  TDS_STEADY_RXD_OFF = TDS_STEADY_MIN,     // 1
  TDS_STEADY_RXD_ON,                       // 2
  TDS_STEADY_MAX = TDS_STEADY_RXD_ON,      // 2

  TDS_TRANS_NONE,                          // 3
  TDS_TRANS_MIN,

  TDS_TRANS_RXD_OFF2ON = TDS_TRANS_MIN,    // 4
  TDS_TRANS_CPHY_SETUP,                    // 5
  TDS_TRANS_MEAS_GAP,                      // 6
  TDS_TRANS_QTA_GAP,                       // 7
  TDS_TRANS_MAX = TDS_TRANS_QTA_GAP,       // 7
  TDS_SCN_NUM                              // 8
}tds_cmgr_scenarios_e;

#define TDS_CMGR_VALID_STEADY_SCN(scn) ((TDS_STEADY_NONE == scn) || ((TDS_STEADY_MIN <= scn) && (TDS_STEADY_MAX >= scn)))
#define TDS_CMGR_VALID_TRANS_SCN(scn)  ((TDS_TRANS_NONE == scn) || ((TDS_TRANS_MIN <= scn) && (TDS_TRANS_MAX >= scn)))
 
#define TDS_CMGR_VALID_SCN(scn) (TDS_CMGR_VALID_STEADY_SCN(scn) || TDS_CMGR_VALID_TRANS_SCN(scn))

/* -----------------------------------------------------------------------*/
/*when channel call MCVS update command API*/
typedef struct
{
    mcaltds_mcvs_phy_channel_type channel; 
    uint16	cctrch_id;
    uint32	action_time;				 /* activation time, in subframe*/
    mcaltds_mcvs_phy_channel_ops_type cfg_option; /* disable/enable/recfg		  */
    uint16	 ts_bitmap;
}mcaltds_mcvs_phy_channel_updt_cmd_type;



/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
typedef struct
{
  mcaltds_mcvs_phy_channel_type channel;
  uint8  cctrch_id;
  uint16  ts_bitmap;
  mcaltds_mcvs_phy_channel_ops_type ops;  
} mcaltds_mcvs_beam_type;

typedef struct
{
  uint8 num_jds_cells;
  uint8 num_jdcs_cells;
  uint8 num_meas_cells;  
} mcaltds_mcvs_jds_type;

typedef struct 
{
  mcaltds_mcvs_beam_type beams;
  mcaltds_mcvs_jds_type  ts0_jds;
  mcaltds_mcvs_jds_type  nonts0_jds;
} mcaltds_mcvs_change_reason_type;

typedef struct 
{
  /* Queue Link Type */
  q_link_type             link;

	uint8  req_mask;                            /* 001 – Channel, 010 – JDS, 100 – RxD */
  mcaltds_mcvs_change_reason_type reason;
	uint16 action_time;                         /* in sub-frame number */
} mcaltds_mcvs_pending_change_request_type;

typedef struct 
{
  uint16 sys_time;      /* in sub-frame number */
  uint8 num_ts0_jds_cells;
  uint8 num_ts0_meas_cells;
  uint8 num_nonts0_jds_cells;
  uint8 num_nonts0_meas_cells;
  uint8   ts0_beam;
  uint8   max_num_beams;    
  boolean rxd_enabled;
  mcaltds_mcvs_voltage_type   cur_vol_level;
  mcaltds_mcvs_voltage_type   next_vol_level;
  mcaltds_mcpm_state       cur_mcpm_state;
  uint32 scn_bitmask;
  uint32 scn_to_start_bitmask;
  uint32 scn_to_stop_bitmask;

  uint32 scn_sent_to_mcpm;
} mcaltds_mcvs_status_type;

/* Mutex for access req queue */

#define MCALTDS_MCVS_REQ_INTLOCK()  REX_ISR_LOCK(&mcaltds_mcvs_req_crit_sect)
#define MCALTDS_MCVS_REQ_INTFREE()  REX_ISR_UNLOCK(&mcaltds_mcvs_req_crit_sect)

extern boolean mcaltds_mcvs_idle_bus_clock_bump_done;
extern boolean mcaltds_mcvs_disable_dynamic;

/* Call back function type declaration */
/* ----------------------------------- */

/*===========================================================================
FUNCTION     mcaltds_mcvs_init

DESCRIPTION
  Initialization function of MCVS.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_init(void);
/*===========================================================================
FUNCTION     mcaltds_mcvs_ts0_jds_cells_update

DESCRIPTION
  API function between MCVS controller and JDS/measurement module:
  for TS0 JDS cells

PARAMETERS
  num_jds_cells: number of JDS cells in this update
  action_times: the effective time in FW side



RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_ts0_jds_cells_update(uint8 num_jds_cells,
                                       uint8 num_jdcs_cells,
                                       uint8 num_meas_cells, 
                                       uint32 action_time);

/*===========================================================================
FUNCTION     mcaltds_mcvs_nonts0_jds_cells_update

DESCRIPTION
  API function between MCVS controller and JDS/measurement module:
  for non-TS0 JDS cells

PARAMETERS
  num_jds_cells: number of JDS cells in this update
  action_times: the effective time in FW side



RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_nonts0_jds_cells_update(uint8 num_jds_cells,
                                                 uint8 num_jdcs_cells,
                                                 uint8 num_meas_cells, 
                                                 uint32 action_time);

/*===========================================================================
FUNCTION     mcaltds_mcvs_phychannel_update

DESCRIPTION
  API function between MCVS controller and channel setup.

PARAMETERS
channel type; (PCCPCH, SCCPCH, DPCH, E-AGCH, E-HICH, HS-SCCH, HS-PDSCH)
action_times: the effective time in FW side
timeslot_bitmap: 


RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_phychannel_update(/* physcial channel type */
                                    mcaltds_mcvs_phy_channel_type channel, 
                                    uint16  cctrch_id,
                                    uint32  action_times,                /* activation time, in subframe*/
                                    mcaltds_mcvs_phy_channel_ops_type ops, /* disable/enable/recfg */
                                    uint16   ts_bitmap);                 /* TS bitmap for this channel, */

/*===========================================================================
FUNCTION     mcaltds_mcvs_mcpm_state_update

DESCRIPTION
  API function between MCVS controller and MCPM_DRV

PARAMETERS
  MCPM Request type: mcpm_request_type
 

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_mcpm_state_update(mcpm_request_type  mcpm_req);
/*===========================================================================
FUNCTION     mcaltds_mcvs_latch_chan_update_req

DESCRIPTION
  Latch all requests for updating voltage at begining of TS2

PARAMETERS
  mcvs_state - volatage state setting by MCPM call
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_latch_chan_update_req(void);
/*===========================================================================
FUNCTION     mcaltds_mcvs_event_handler_data_update

DESCRIPTION
  Latch all requests for updating voltage at begining of TS2

PARAMETERS
  mcvs_state - volatage state setting by MCPM call
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_event_handler_data_update(void);

/*===========================================================================
FUNCTION     mcaltds_mcvs_event_handler_clock_adjust

DESCRIPTION
  STMR event to latch all requests for updating voltage at TS1

PARAMETERS
  mcvs_state - volatage state setting by MCPM call
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_event_handler_clock_adjust(void);

/*===========================================================================
FUNCTION     mcaltds_mcvs_is_t2g_enabled

DESCRIPTION
  
PARAMETERS
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcvs_is_t2g_enabled(void);

/*===========================================================================
FUNCTION     mcaltds_mcvs_t2g_enable

DESCRIPTION
  To indicated T2G enabled

PARAMETERS
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_t2g_enable(void);

/*===========================================================================
FUNCTION     mcaltds_mcvs_t2g_disable

DESCRIPTION
  To indicated T2G disabled

PARAMETERS
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_t2g_disable(void);

/*===========================================================================
FUNCTION     mcaltds_mcvs_wakeup_clock_bump_q6

DESCRIPTION
  Increase Q6 / VPE clock during wakuep

PARAMETERS
  number of bit 1
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcvs_wakeup_clock_bump_q6(void);


/*===========================================================================
FUNCTION mcaltds_mcvs_nv_bump_up_clocks

DESCRIPTION
  This function return whether the clock is in the nigh clock mode reading from the NV_item. 
 
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean mcaltds_mcvs_nv_bump_up_clocks(void);

/*===========================================================================
FUNCTION     mcaltds_mcvs_nv_disabled

DESCRIPTION
  Return whether the dynamic clock control of MCVS is disabled. Temporary for BOLT bringup. 

PARAMETERS
  number of bit 2 of NV 71514. 
 
RETURN VALUE
  True - Dynamic control is disabled
  False - Dynamic control is enabled

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcvs_nv_disabled(void);


/*===========================================================================
FUNCTION     mcaltds_mcvs_rxd_is_on

DESCRIPTION
  To check whether RxD is turn on

PARAMETERS

 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcvs_rxd_is_on(void);


/*===========================================================================
FUNCTION     mcaltds_mcvs_config_with_mcpm_req

DESCRIPTION
  send MCVS request together with MCPM request.

PARAMETERS
  mcpm_mcvsrequest_parms_type
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcvs_config_with_mcpm_req(mcpm_request_type  mcpm_req, mcpm_mcvsrequest_parms_type* mcvs_req);


/*===========================================================================
FUNCTION     mcaltds_cmgr_scenario_config

DESCRIPTION
  config scenarios to conc manager.

PARAMETERS
  cmgr_params
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_cmgr_scenario_config(
  tds_cmgr_scenarios_e new_scenario_e, 
  uint32 duration_msec
);
#endif
