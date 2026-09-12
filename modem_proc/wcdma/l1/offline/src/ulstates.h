#ifndef UL_STATES_H
#define UL_STATES_H


/*===========================================================================
                           U L _ S T A T E S . H

DESCRIPTION
This file contains the prototypes for all of the state processing routines
for the uplink controller.


Copyright (c) 2000-2004 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/ulstates.h_v   1.8   29 May 2002 11:40:34   msatyana  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/ulstates.h#5 $ $DateTime: 2020/08/19 04:13:18 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
09/15/15    ar      Crash UE if ul_state is changed to UL_ERROR
08/28/15    rsr     Added changes for Sleep->Traffic Transition.
08/18/15    ar      Doubling uplink state variables to maintain per subscription state
08/04/15    ar      Ensure that HSRACH cleanup routine allows connected state cleanup to 
                    continue in case HSRACH is not configured.
07/19/15    vr      Code cleanup to move all UL/EUL/EDL/HSRACH states to common struct.
06/26/15    vr      Code cleanup to use MACROs for UL/EUL state globals.
03/11/15    rsr     Phase 4 W+W Checkin.
03/11/15    vr      FTM support for EUL & DCHSUPA.
01/16/15    vr      Global reorg for W + W.
07/08/13    vr      Enhanced API to check if UL is active, to accomodate for HSRACH state machine.
06/25/13    yw      Code cleanup for IDLE REQ, EUL/EDL MODULE_CLEANUP
08/09/12    hk      Streamlined FACH and DCH module cleanup procedures
02/01/12   gnk      Nikel feature cleanup
05/08/09   sup      Changing the includes for the renaming of header files  dec.h, enci.h, l1m.h, srch.h, srchi.h, 
                    srchlog.h to wdec.h, wenci.h, wl1m_v.h, wsrch.h, wsrchi.h, wsrchlog.h
03/02/07    am      Added support for log on demand for 4303 logpkt
08/14/06    yiz     Mainline HSUPA code
10/22/04    yiz     Add ul_dch_sync_a_pending flag.
05/29/02    m       Added prototypes for LOD functions
01/14/01    sh      Added flag to indicate a module cleanup is pending.
01/18/2001  mdl     get next L1M state as input parameter to cleanup routines
09/22/00    sk      file created.
===========================================================================*/

#include "wcdma_variation.h"
#include "wl1m.h"
#include "eulcfg.h"
#include "mcalwcdma_edl.h"

/* UL states */
typedef enum {
  /* initialized */
  UL_INIT,
  /* PRACH active */
  UL_PRACH_TX,
  /* UL DPCH active */
  UL_DPCH_TX,
  /* un-recoverable error */
  UL_ERROR
} ul_state_enum_type;

typedef enum {
  UL_DPCH_STATE_SETUP,
  UL_DPCH_STATE_PCP_WAIT,
  UL_DPCH_STATE_TX,
  UL_DPCH_STATE_DISABLED
} ul_dpch_state_enum_type;

#ifdef FEATURE_WCDMA_HS_RACH
/* HS RACH STATE -- This indicates the possible states the HS RACH
** state machine can be in at any given point in time*/
typedef enum {
  /* Initialized state .. HS RACH is not configured*/
  WL1_HS_RACH_INIT,
  /* HS RACH config in progress ... during the CPHY SETUP is handled*/
  WL1_HS_RACH_CONFIG,
  /* Once the SETUP is done ... WCDMA L1 is waiting for MAC Tx IND to
  ** trigger RACH procedure*/
  WL1_HS_RACH_READY,
  /* Common EDCH Resource is allocated, after E-AI/AI ACK*/
  WL1_HS_RACH_IN_PROGRESS,
  /* This is a state when the HS RACH SW currently is handling a SEAMLESS
   ** F2D transition*/
  WL1_HS_RACH_SL_F2D_TRANSITION,
  /* INVALID ENUM STATE*/
  WL1_NUM_HS_RACH_STATE
} wl1_hs_rach_state_enum_type;

/* HS RACH SUB STATE -- This indicates the possible HS RACH sub states
** and each of these substates are uniquely mapped to one of the HS RACH
** states*/
typedef enum {
  /* The sub state when HS RACH is not configured..
  ** MAPS to WL1_HS_RACH_INIT State*/
  WL1_HS_RACH_SUBSTATE_INIT,
  /* WHen the HS RACH START is being handled...
  ** MAPS to WL1_HS_RACH_CONFIG*/
  WL1_HS_RACH_CFG_START,
  /* When WCDMA L1 is waiting for TX IND from MAC ..
  ** MAPS to WL1_HS_RACH_READY */
  WL1_HS_RACH_WAITING_FOR_TX_IND,
  /* When WCDMA L1 is doing RACH procedure to acquire
  ** common EDCH index. MAPS to WL1_HS_RACH_READY*/
  WL1_HS_RACH_ACQ_EDCH_RSRC_IDX,
  /* This substate indicates that the UE is doing DPCCH only Tx
  ** for Additional EDCH Transmission Back Off + 1/2 TTIs*/
  WL1_HS_RACH_DPCCH_ONLY_TX,
  /* When EDCH resource is allocated and Collision resolution
  ** is ongoing for DxCH. Maps to WL1_HS_RACH_IN_PROGRESS */
  WL1_HS_RACH_COLLISION_RESOLUTION,
  /* EDCH Tx has started. After Collision resolution for DxCH and
  ** after E-AI//AI ACK for CCCH. Maps to
  ** WL1_HS_RACH_IN_PROGRESS*/
  WL1_HS_RACH_EDCH_TX,
  /* When the EDCH resource is in the process of being released
  ** due to one of the various reasons. Maps to
  ** WL1_HS_RACH_IN_PROGRESS*/
  WL1_HS_RACH_EDCH_REL_PROC,
  /* INVALID SUBSTATE ENUM*/
  WL1_NUM_HS_RACH_SUBSTATE,
} wl1_hs_rach_sub_state_enum_type;

#endif /* FEATURE_WCDMA_HS_RACH */

/* ENUM to identify current RACH STATE */
typedef enum {
  WENC_RACH_INVALID_STATE,        /* Indicates Prach channel is not up */
  WENC_RACH_READY_STATE,          /* Indicates Prach channel is up and 
                                   * Rach proc can be started */
  WENC_RACH_NOT_READY_STATE,      /* Indicates Rach procedure cannot be started*/
  WENC_RACH_IN_PROGRESS_STATE,           /* Indicates Rach procedure is in progress */
  WENC_RACH_EDCH_TX_IN_PROGRESS_STATE,   /* Indicates HS RACH EDCH Tx in progress */
  WENC_RACH_ABORT_STATE,          /* Indicates current Rach Procedure is aborted */
  WENC_RACH_NUM_STATES
} wenc_rach_state_enum_type;

/* EUL uplink  states maintained in uplink module*/
typedef enum {
  /* initialized state. EUL uplink enters into this state whenever the 
  ** WCDMA is started and l1m init is called */
  E_UL_INIT,
  /* EUL EDPCH active state. EUL uplink enters into this state when  EDPCH 
  ** is requested and leaves this state when EDPCH is dropped */
  E_UL_EDPCH_TX,
  /* un-recoverable error state. EUL uplink enters into this state when eul
  ** uplink module encounter some error condition */
  E_UL_ERROR
} e_ul_state_enum_type;

/* EUL EDPCH states. These states are used only when EUL uplink module is
** in E_UL_EDPCH_TX */
typedef enum {
  /* edpch default state */
  E_UL_EDPCH_INIT,
  /* EDPCH is in set up or DCH is in PCP */
  E_UL_EDPCH_STATE_SETUP,
  /* EDPCH is in normal tx state*/
  E_UL_EDPCH_STATE_TX,
  /* EDPCH is disabled due to out of sync */
  E_UL_EDPCH_STATE_DISABLED,
  /* ERORR state to recover */
  E_UL_EDPCH_STATE_ERROR
} e_ul_edpch_state_enum_type;


typedef struct {
  /* Flag to indicate that UL sync procedure A is required
  ** when UE enters DCH state from other states
  ** when UE goes from DCH to DCH via HHO
  ** when UE resumes DCH fater inter-RAT HHO or HHO failure */
  boolean ul_dch_sync_a_pending;
  /* Flag to indicate a module cleanup is pending */
  boolean ul_module_cleanup_flg;
} ulstates_ext_cntrl_params_type;

extern ulstates_ext_cntrl_params_type ulstates_ext_cntrl_params;

typedef struct {
  /* Initial UL state is Un-Initialized */
  ul_state_enum_type             ul_state;
  /* This is the UL DPCH state */
  ul_dpch_state_enum_type        ul_dpch_state;
  /* Initialize rach state variable  */
  wenc_rach_state_enum_type      wenc_rach_curr_state;
  /* Initial EUL uplink module state is Un-Initialized  This variable is used to
  ** know the state of EUL uplink module */
  e_ul_state_enum_type           e_ul_state;
  /* EDPCH state maintained during EUL uplink module is EDPCH TX state. */
  e_ul_edpch_state_enum_type     e_ul_edpch_state;
  mcalwcdma_e_dl_state_enum_type e_dl_state;
  #ifdef FEATURE_WCDMA_HS_RACH
  /*Global variable that maintains the HS RACH state*/
  wl1_hs_rach_state_enum_type     wl1_hs_rach_state;
  /*Global variable that maintains the HS RACH sub state*/
  wl1_hs_rach_sub_state_enum_type wl1_hs_rach_sub_state;
  #endif /* FEATURE_WCDMA_HS_RACH */
} wl1_ul_state_db_struct_type;

typedef struct {
  wl1_ul_state_db_struct_type wl1_ul_state_db;
} ulstates_ext_persub_cntrl_params_type;

extern ulstates_ext_persub_cntrl_params_type ulstates_ext_persub_cntrl_params[WCDMA_NUM_SUBS];

#define WL1_GET_CURR_UL_STATE()     (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.ul_state)
#define WL1_SET_CURR_UL_STATE(ulSt) \
do{ \
  ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.ul_state =  (ulSt); \
  if(ulSt == UL_ERROR ) \
  { \
    mdsp_sync_send_fw_panic_cmd( NULL, NULL, wsub_id); \
    WL1_ERR_FATAL("UL_ERR : UL state moved to UL_ERROR!",0,0,0); \
  } \
} while(0)

#define WL1_CHK_CURR_UL_STATE(ulSt) (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.ul_state == (ulSt))

#define WL1_CHK_CURR_CONN_UL_STATE(ulSt) (WL1_CHK_CURR_UL_STATE(ulSt))

/*===========================================================================
FUNCTION        UL_INIT

FILE NAME       

DESCRIPTION     This is the one time initialization function for the uplink
                controller.  It just calls the associated uplink control and 
                encoder driver initializers.

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
void ul_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        E_UL_INIT

FILE NAME       

DESCRIPTION     
  This is the one time initialization function for the EUL uplink controller. 
  It initializes the EUL state and calls the eul encoder driver initializer.
  This function is called from ul_init .

DEPENDENCIES    
  None

RETURN VALUE    
  None

SIDE EFFECTS    
  None
===========================================================================*/
extern void e_ul_init(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_CLEANUP

FILE NAME       

DESCRIPTION     This is the one time cleanup routine for the uplink 
                controller. It calls the associated uplink control and
                encoder driver cleanup routines.
                
DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
boolean ul_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        UL_IDLE_INIT

FILE NAME       

DESCRIPTION     This is the IDLE state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void ul_idle_init( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        UL_IDLE_CLEANUP

FILE NAME       

DESCRIPTION     This is the IDLE state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean ul_idle_cleanup(l1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_ACQ_INIT

FILE NAME       

DESCRIPTION     This is the ACQ state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void ul_acq_init(void);


/*===========================================================================
FUNCTION        UL_ACQ_CLEANUP

FILE NAME       

DESCRIPTION     This is the ACQ state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean ul_acq_cleanup(l1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_BCH_INIT

FILE NAME       

DESCRIPTION     This is the BCH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void ul_bch_init(void);

/*===========================================================================
FUNCTION        UL_BCH_CLEANUP

FILE NAME       

DESCRIPTION     This is the BCH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean ul_bch_cleanup(l1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_PCH_INIT

FILE NAME       

DESCRIPTION     This is the PCH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void ul_pch_init(void);

/*===========================================================================
FUNCTION        UL_PCH_CLEANUP

FILE NAME       

DESCRIPTION     This is the PCH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean ul_pch_cleanup(l1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        UL_FACH_INIT

FILE NAME       

DESCRIPTION     This is the FACH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void ul_fach_init(void);

/*===========================================================================
FUNCTION        UL_FACH_CLEANUP

FILE NAME       

DESCRIPTION     This is the FACH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean ul_fach_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_UL_HSRACH_CLEANUP

FILE NAME       

DESCRIPTION     This is the HSRAC cleanup routing called during 
                connected state cleanup

DEPENDENCIES    

RETURN VALUE    TRUE : HS RACH is not configured, continue with connected state cleanup
                FALSE : HS RACH is configured, process the cleanup action

SIDE EFFECTS    Will cause any ongoing HSRACH procedure to be aborted
===========================================================================*/
boolean wl1_ul_hsrach_cleanup(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_DCH_INIT

FILE NAME       

DESCRIPTION     This is the DCH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void ul_dch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_DCH_CLEANUP

FILE NAME       

DESCRIPTION     This is the DCH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean ul_dch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_CONN_STATE_CLEANUP

FILE NAME

DESCRIPTION     This is the connected (FACH/DCH) state cleanup routine for Uplink
                module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern boolean ul_conn_state_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

 /*===========================================================================
FUNCTION        E_UL_DCH_CLEANUP

FILE NAME       

DESCRIPTION     
  This is the DCH state cleanup routine for the EUL uplink module. if eul state
  is E_UL_EDPCH_TX ,enable the module clean up flags and the edpch cfg event.
      
DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern boolean e_ul_dch_cleanup( wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION UL_PHYCHAN_IS_UL_ACTIVE_IN_FACH

DESCRIPTION
  This API returns the state of UL PHY channel, whether it is active or not. 
  - In legacy RACH case UL_STATE will be set to PRACH_TX. 
  - In HSRACH case UL_STATE will be set to PRACH_TX when channels are added 
    (and when resuming HSPRACH). It remains in this state until FDPCH 
    interrupt is received. After FDPCH interrupt is received UL_STATE is 
    moved to DPCH_TX.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  == DPCH active when HSRACH is configured or PRACH active in FACH.
  FALSE == PRACH or DPCH not active

SIDE EFFECTS
  None.

=========================================================================*/
extern boolean ul_phychan_is_ul_active_in_fach(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_INIT_IN_WTASK

FILE NAME

DESCRIPTION     One time init of UL globals.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern void ul_init_in_wtask (void);

/*===========================================================================
FUNCTION        UL_MODULE_L1M_INIT

FILE NAME

DESCRIPTION     Init of UL module globals called from L1M.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern void ul_module_l1m_init(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_MODULE_CONN_INIT

FILE NAME

DESCRIPTION     Init of UL module globals called when entering connected state.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern void ul_module_conn_init (wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_MODULE_L1M_DEINIT

FILE NAME

DESCRIPTION     De-init of UL module globals called exiting connected state.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern void ul_module_l1m_deinit(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_MODULE_CONN_DEINIT

FILE NAME

DESCRIPTION     De-init of UL module globals called when exiting connected state.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern void ul_module_conn_deinit(wsub_id_e_type wsub_id);

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        UL_LOG_INIT

FILE NAME

DESCRIPTION
  This function registers all the UL log on demand handlers to
  corresponding log code given out by QXDM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void ul_log_init (void);

/*===========================================================================
FUNCTION     EUL_LOG_INIT

DESCRIPTION  This function registers all the EUL log on demand handlers to
             corresponding log code given out by QXDM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void eul_log_init (void);
#endif

#endif
