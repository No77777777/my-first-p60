#ifndef TDSDL_STATES_H
#define TDSDL_STATES_H
/*===========================================================================
                        D L _ S T A T E S . H

GENERAL DESCRIPTION
  This module implements the function that are required for the downlink
  operation. The function included in this module control the operation of
  demodulator front and back end.

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2007 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tdsdlstates.h_v   1.5   29 May 2002 11:31:06   msatyana  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlstates.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
05/03/10    hk      Fixed compiler warnings
05/25/09    hk      Fixed compiler warnings
05/08/09    sup     Changing the includes for the renaming of header files  dec.h, enci.h, l1m.h, srch.h, srchi.h, 
                    srchlog.h to tdsdec.h, tdsenci.h, tdsl1m.h, tdssrch.h, tdssrchi.h, tdssrchlog.h
12/10/08    ks      Mainlining FEATURE_TDSCDMA_DL_ENHANCED
11/11/08    ks      Featurising dl_dem_idle_init, not used for new DL
10/03/08    hk      Bringing in 7k Mailine fixes
01/29/08    scm     Move normalize_sfn() macro from tdsdlstates.h to tdsl1utils.h.
10/24/07    vsr     Mainlining MBMS and Enhanced DL changes 
07/30/07    rgn     Changes to move cm gsm cleanup functions to a separate function
10/10/06    ms      Added prototype for tdsdlstates_dl_dch_cleanup_after_hs_teardown.
08/14/06    ub      Added support for HSUPA.
08/09/06    scm     Add extern to tdsdlstates_dl_aset_sync_info_update_needed.
05/26/06    kps     Change the prototype of tdsdlstates_dl_sib_wake_up_pccpch_n() to return
                    the number of channels woken up.
04/12/06    mc      Added DL DCH maintenance event handler.
                    Added several functions to support this feature.
09/25/05    kps     Preliminary SIB sleep optimization support. 
                    Code changes are inside #ifdef FEATURE_SIB_SLEEP_OPTIMIZATION
08/10/05    kps     add dl_bmc_ctch_sccpch_offset() and
                    dl_bmc_get_msec_to_sleep_for_ctch(uint16)
06/22/05    scm     Extern dl_bmc_extra_ctch_debug for extra run-time debug.
05/25/05    kps     add DL_BMC_INVALIDATE_SFN() for BMC.
05/17/05    kps     added dl_bmc_get_sleep_info() and dl_bmc_next_frame_wanted()
                    to support sleep optimization for BMC.
04/25/05    scm     Add extern to dl_bmc_drop_ctch_sccpch_if_setup().
03/22/05    scm     Support CTCH SCCPCH phychan drop.
03/21/05    scm     dl_bmc_enable_sccpch() now returns boolean.
03/15/05    scm     Externalize some functions for BMC.
03/15/05    kps     Implement non-DRX mode in dl_bmc_frame_is_wanted()
03/03/05    scm     Add CCTrCh index parameter to dl_bmc_cctrch_setup_done().
03/03/05    gs      Externalize variable tdsdlstates_l1_dch_cleanup_hs_cleanup_pending
02/27/05    scm     Adding more function prototypes for BMC support.
02/27/05    scm     Save config info for SCCPCH carrying CTCH for setup later.
02/22/05    scm     Supporting broadcast message reception on CTCH.
12/05/03    scm     Externalize tdsdlstates_dl_end_cm_suspended_state().
05/29/02    m       Added prototypes to support LOD
01/24/2001  mdl     consolidated dldemstates.h and dldecstates.h into this
                    file tdsdlstates.h.  Changed return types of all state
                    cleanup routines to boolean

01/17/2001  mdl     changed all state cleanup routines to take the next state
                    as an input parameter

11/02/2001  mdl     first cut
===========================================================================*/
#include "tdscdma_variation.h"
#include "tdsl1m.h"

/* This flag will enable us to do certain things which we do
   only when we enter DCH for the first time. In other words
   it doesnt consider resumes, doesn't consider reconfigs etc.
   This is useful for doing such things as initializing the
   cell sync info(OFF/Tm) for the active set cells */
extern boolean tdsdlstates_dl_aset_sync_info_update_needed;

/* indicate if HSDPA channel teardown as part of DCH cleanup is going on */
extern boolean tdsdlstates_l1_dch_cleanup_hs_cleanup_pending;


extern tdsl1m_state_enum_type tdsdlstates_dl_dch_cleanup_next_state;

/* This falg is used to control the processing of sfn_cfn_diff_cmd request from searcher.
    will be set to TRUE once if the modulator dpch timing is written to f/w. will be set to FALSE
    when demod initiates seq slm to dpch timing 
*/
extern boolean tdsdlstates_dl_tx_dpch_mod_timing_set_ready;

/*===========================================================================
FUNCTION        DL_ACQ_CLEANUP_NEW

FILE NAME

DESCRIPTION     This is the ACQ state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_acq_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_ACQ_DROP_ALL

FILE NAME

DESCRIPTION     This is the ACQ state drop all routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_acq_drop_all(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        DL_DEC_BCH_INIT

FILE NAME

DESCRIPTION     This is the BCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_dec_bch_init(void);

/*===========================================================================
FUNCTION        DL_DEC_PCH_CLEANUP

FILE NAME

DESCRIPTION     This is the PCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dec_pch_cleanup(tdsl1m_state_enum_type next_state);
/*===========================================================================
FUNCTION        DL_DEC_DCH_INIT

FILE NAME

DESCRIPTION     This is the DCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_dec_dch_init(void);

/*===========================================================================
FUNCTION        DL_DEC_DCH_CLEANUP

FILE NAME

DESCRIPTION     This is the DCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dec_dch_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_DEM_GENERAL_CLEANUP

FILE NAME       dlstates.c

DESCRIPTION     This function performs the cleanup routines common to all
                states.  It begins the operation of tearing down all of
                the active physical channels.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    All of the active physical channels will be started to be
                torn down.
===========================================================================*/
boolean tdsdlstates_dl_dem_general_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_DEM_ACQ_CLEANUP

FILE NAME

DESCRIPTION     This is the ACQ state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    FALSE if channels have to be torn down, TRUE otherwise

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dem_acq_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_DEM_BCH_CLEANUP

FILE NAME

DESCRIPTION     This is the BCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dem_bch_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_DEM_PCH_CLEANUP

FILE NAME

DESCRIPTION     This is the PCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dem_pch_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_DEM_FACH_CLEANUP

FILE NAME

DESCRIPTION     This is the FACH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if no channels have to be torn down, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dem_fach_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_DEM_DCH_INIT

FILE NAME

DESCRIPTION     This is the DCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_dem_dch_init(void);

/*===========================================================================
FUNCTION        DL_DEM_DCH_CLEANUP

FILE NAME

DESCRIPTION     This is the DCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if no channels have to be torn down, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dem_dch_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_INIT

FILE NAME

DESCRIPTION     This is the one time initialization function for the downlink
                controller.  It just calls the associated decoder and
                demodulator initializers.

DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_init(void);

/*===========================================================================
FUNCTION        DL_IDLE_INIT

FILE NAME

DESCRIPTION     This is the IDLE state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_idle_init(void);

/*===========================================================================
FUNCTION        DL_IDLE_CLEANUP

FILE NAME

DESCRIPTION     This is the IDLE state cleanup routine for the downlink
                controller.

DEPENDENCIES    None

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS    L1M may have to wait for a further local command to
                complete the exit from the IDLE state.
===========================================================================*/
boolean tdsdlstates_dl_idle_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_ACQ_INIT

FILE NAME

DESCRIPTION     This is the ACQ state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_acq_init(void);


/*===========================================================================
FUNCTION        TDSDLSTATES_DL_SYNC_INIT

FILE NAME

DESCRIPTION     This is the SYNC state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_sync_init(void);

/*===========================================================================
FUNCTION        TDSDLSTATES_DL_SYNC_CLEANUP

FILE NAME

DESCRIPTION     This is the SYNC state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_sync_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_BCH_INIT

FILE NAME

DESCRIPTION     This is the BCH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_bch_init(void);


/*===========================================================================
FUNCTION        DL_BCH_CLEANUP

FILE NAME

DESCRIPTION     This is the BCH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_bch_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        DL_BCH_DROP_ALL

FILE NAME

DESCRIPTION     This is the BCH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_bch_drop_all(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        DL_PCH_INIT

FILE NAME

DESCRIPTION     This is the PCH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_pch_init(void);


/*===========================================================================
FUNCTION        DL_PCH_CLEANUP

FILE NAME

DESCRIPTION     This is the PCH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_pch_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        DL_PCH_DROP_ALL

FILE NAME

DESCRIPTION     This is the PCH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_pch_drop_all(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        DL_FACH_INIT

FILE NAME

DESCRIPTION     This is the FACH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_fach_init(void);

/*===========================================================================
FUNCTION        DL_FACH_CLEANUP

FILE NAME

DESCRIPTION     This is the FACH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
============================================================================*/

boolean tdsdlstates_dl_fach_cleanup(tdsl1m_state_enum_type next_state);

boolean tdsdlstates_dl_fach_drop_all(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        DL_DCH_INIT

FILE NAME

DESCRIPTION     This is the DCH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsdlstates_dl_dch_init(void);

/*===========================================================================
FUNCTION  dl_maint_event_handler

DESCRIPTION     
      This function is the maintenance handler for the DL DCH channel.

DEPENDENCIES
      None
      
RETURN VALUE
      None
      
SIDE EFFECTS
      None
===========================================================================*/
void tdsdlstates_dl_dch_maint_event_handler(void);


/*===========================================================================
FUNCTION        DL_DCH_CLEANUP_AFTER_HS_TEARDOWN

FILE NAME

DESCRIPTION     This function fo cleanup of DL dec and dem after HS channel
                has been torn down

DEPENDENCIES

RETURN VALUE    

SIDE EFFECTS
===========================================================================*/
extern void tdsdlstates_dl_dch_cleanup_after_hs_teardown(void);




/*===========================================================================
FUNCTION        DL_DCH_CLEANUP

FILE NAME

DESCRIPTION     This is the DCH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dch_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        DL_DCH_DROP_ALL_CHANNEL

FILE NAME

DESCRIPTION     This is the DCH state routine for drop all channels, which is the same as cleanup now
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean tdsdlstates_dl_dch_drop_all(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION tdsdlstates_dl_end_cm_suspended_state

DESCRIPTION
  This function does 3 things:
  - Call tdsl1cmmgr_end_compressed_mode() to initialize CM manager data.
  - Disable the compressed mode update event on the general timeline.
  - Call tdscmmeas_transition_from_cell_dch() to clear CM measurement data.

  Normally these things are done when transitioning out of DCH with the
  tdsl1m_dch_cleanup() call.  However, in the case of W2G HO, we still call
  tdsl1m_dch_cleanup(), but it doesn't fully terminate compressed mode.  This is
  so we can RESUME WCDMA if the handover fails.  But if the handover succeeds,
  this function is called, so compressed mode can clean itself up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsdlstates_dl_end_cm_suspended_state(void);


#endif

