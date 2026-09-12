/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           U S E R   I N T E R F A C E   N V   M O D U L E

GENERAL DESCRIPTION

  This source file provides the NV interface functions for RRC.

EXTERNALIZED FUNCTIONS
  rrc_get_nv
    Get an item from NV, handle non-active items

  rrc_put_nv
    Write an item to NV

  rrc_replace_nv
    Replaces an item in NV

  rrc_free_nv
    Free an item from NV

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2001-2009 Qualcomm Technologies, Inc.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcnv.c#8 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/22/20   ha      Enable HI AC even when LO AC absen through NV
12/15/21    ha     FR53313 : Requirement to have configurable pseudo meas defer timer First time entering to DCH through NV
18/5/20   gcs     Added NV support to disable/enable feature FRLTE for LTOW CSFB due to supplementary service
04/20/20   gcs     FR59710: Made changes to delay FRLTE and WtoL Resel for MAV
01/14/20   sp      Mini dump phase-2 changes
06/24/19   gcs     Made changes for FR 57381
11/20/18   ak      Made changes for FR 53313
02/23/17   sas     Changes to have NV gaurd to control the Truncated/Full CA feature.
04/12/18   ak     Made changes to read complete sibs independent of their entry in SIBdb.
04/03/18   sg      Made change to handle back to back SMC
02/21/18   nr      Added code to support LTA during activation time
01/18/18   vs      Added NV for allowing LTA during access priority
02/13/17   nr      Made changes to read the DBDC band supported info from the DBDC single sim NV if only one sim is inserted in a DSDS device
08/29/17   sg      Made changes to remove the GCF NV 947 read and get the value of this NV through an api
10/20/16   sr      FR 38167: Changes to restrict the code to specific PLMN
05/03/16   sp      Made changes to have a maximum wait time for allowing LTAs despite of pending L2 acks
04/19/16   vs      Changes to avoid printing unnecessary f3s on TH.2.0.1
04/15/16   sn      Changes for f3 logging reduction
03/09/16   sn      Changes for FR30881
10/21/15   sp      Made changes to check for RRC release version before setting the EDCH category NV value
10/20/15   gv      Made changes to indicate success to MCFG when NV referesh is triggered for SUB where W is not active
09/25/15   sp      Added NV support for MSIM DBDC band combination
08/07/15   sp      NV changes to control l2 ack wait timer during reselection
08/05/15   vi      Fixed KW errors
07/20/15   sr      Made changes for FR29345 EFS/NV Settings to be Read per Sub
07/28/15   vi      Made changes to remove triple sim related code from function rrc_nv_refresh_map_sub_index_to_as_id
07/01/15   ad      Made changes to allow HSRACH and EDRX concurrency with DSDS
06/17/15   vi      Fixed compilation errors.
06/12/15   vi      Made changes to pass correct sub id to MCFG API for efs read/write operations
06/04/15   bc      Changes to fix compilation errors.
05/22/15   sa      Made changes to Co-existance of DCHSUPA and UL 16QAM
05/21/15   vi      Added NV to half the DRX length.
05/20/15   sr      Changes for NV refresh specific to W+W
02/19/14   sp      Made changes for Mav debug snapshot
03/12/15   bc      Changes to fix W+W compilation errors
03/10/15   sas     NV Changes to control reading of all SIBs for every reselection irrespective of SIBDB.
01/22/15   bc      Changes to provide NV control to allow TA for loop back call and CS Data call
12/29/14   bc      Made changes to control the range of frequencies to be skipped around a WCDMA cell by an nv.
10/05/14   sg      Made changes to disable EDRX and HSRACH DS concurrency capabilities overriding the NV values set.
11/03/14   ad      Made changes to control different DB DC band combinations through NV
10/08/14   sn      FR 22503 PCH to FACH optimization
07/25/14   sr      API return type change for NV refresh FR
07/21/14   ac      NV reorganization fixes in rrc_create_external_nv_item
07/18/14   sp      Made changes to set the default value of DSDS concurrency NV(72581) to enable DCHSDPA
07/17/14   sr      Changes for NV refresh FR
07/15/14   ac      NV reorganization
07/08/14   vi      Made changes to add NV control over meas id extn feature.
06/23/14   vi      Made changes to skip SIB7 reading if SIB7 expiration timer is stil running
06/24/14   sp      Made changes to enable integrigty and ciphering by default if the user has not written to NV
06/20/14   sa      Made changes to update the UL 16QAM
06/17/14   sp      Made changes to mainline DCH-FACH transition optimization changes
04/29/14   sa      Made changes for NV support for default config in CELL FACH
05/16/14   vi      Made changes to support MOB framework
05/14/14   ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
05/14/14   vi      Created new NV to prioritize 850 band
05/12/14   sn      Changes to control SIB sleep before receiving SBs using NV
04/10/14   db      Made changes to enable/disable feature support in DSDS/DSDA/TSTS modes through NV
03/28/14   vi      Added new NV rrc_constant_320ms_drx_nv
03/26/14   sr      Added NV support for uplink compression capability
03/17/14   sn      Changes to control ASF timer with a NV 
03/11/14   vi      Made changes to disable IRAT and IFREQ measurements in different states based on the NV rrc_disable_meas_nv
01/27/14   sr      Added NV control for MFBI support
01/03/14   sa      Made changes to print unconditional wcdma_nv_hsupa_category per L1
12/24/13   db      Introduced new NV to allow CPC,HSFACH,HSRACH,DC-HSDPA in DSDS and DSDA mode
12/13/13   sn      Manual PLMN search optimizations
12/03/13   mn      Moved all declarations to top of the code.
12/03/13   sp      Made changes to add nv support and to save time ticks for 
                   rr to/from rrc mm to/from rrc and to add msg_type for crash debug ota changes
11/15/13   as      Made changes to set default EUL NV category correctly
10/15/13   sr      Made changes to fix compilation errors
09/24/13   vi      Introduced new NV for FE-FACH
08/28/13   vi      Introduced new NV for GANSS feature
08/21/13   gv      Enabling PPAC NV by default
08/13/13   vi      Fixed buffer overflow case while doing strlcpy
07/18/13   vi      Made changes to pass correct data type for rrc_fast_return_to_lte_after_csfb_timer_nv
06/19/13   vi      Made changes to pass correct data type for rrc_nv_freq_lock_item 
05/30/13   gv      Made changes to perform acq db only scan when requested so by higher layers
05/15/13   db      Made changes to enhance NV 67333 to control FDPCH and CPC DTX/DRX features independently
05/10/13   gv      Made changes to modify the default band priority
05/09/13   sn      Reduce fine/raw scan skip range for manual CSG search
05/07/03   vi      Removing feature FEATURE_WCDMA_RRC_SINGLE_CONF_FILE
05/01/13   vi      Fixed compiler warnings
04/30/13   vi      Made changes to use single wcdma_rrc_external.conf file for nv items
04/23/13   vi      Made default value of wcdma_rrc_wtol_tdd_ps_ho_support_nv to TRUE
04/19/13   vi      Made changes to remove IRAT bringup changes
02/21/13   sa      Made the changes to mainline the FEATURE_THIN_UI feature
12/07/12   sn      CSG feature changes
11/29/12   mp      Made chagnes to ignore meas reports for PSC lock under NV
11/02/12   gv      Corrected the changes under FEATURE_WCDMA_DIME_SW
11/02/12   sg      Made changes to remove the nv "rrc_nv_a2_power_opt" and remove
                   remove the code related to HW integrity protection.  
10/16/12   sr      Made changes to disable W2L measurements both in idle and connected mode through NV item
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
03/28/12   zr      Adding feature definitions for Dime
07/03/12   sks     Made changes to support NV item for wtol_TDD ps_ho
09/05/12   md      Mainlined feature flags FEATURE_RRC_INTEGRITY_SW_ENGINE,
                   FEATURE_WCDMA_A2_POWER_COLLAPSE and FEATURE_WCDMA_NIKEL_SW
05/02/12   rd      Made changes to support NV item for wtol_ps_ho
04/02/12   db      Made changes to disable CPC and FDPCH through NV
04/10/12   pm      Added code to support the band priority nv
02/24/12   sn      Changes to periodically save ACQ DB in NV
03/09/12   gv      Provided NV support to enable/disable PPAC functionality
                   and also to use existing conf file to create new NV items
01/02/12   pm      Added rrc_wtol_cm_support_nv to put FEATURE_WCDMA_CM_LTE_SEARCH feature under nv check 
11/17/11   ad      Added featurization for A2 power optimization
11/15/11   sks     Made the changes for A2 power optimization.
10/31/11   sks     Added support for FEATURE_WCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB.
09/16/11   rl      Made changes to remove the AEEstd library functions 
                   to CoreBsp secured function cal
07/05/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.101.00
06/29/11   su      Added code for NV support to enable/disable
                   the FEATURE_RRC_DO_NOT_FORWARD_PAGE.
05/10/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.97.50
05/05/11   vg      added a new NV support for channal locking feature 
05/03/11   su      Added code changes to provide NV support to enable/disable
	           FEATURE_3GPP_CSFB_SKIP_OPT_WCDMA_SIBS.
03/29/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.94.50
03/28/11   su      Made changes to set SRB2 suspend_offset based on 
                   rrc_set_srb2_act_time_nv item.
03/16/11   su      Added code changes to NV item support to enable/disable support
                   for snow3g_security_algo.
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/11/11   rl      Fixed the lint warning
03/09/11   su      Made changes to mainline code under FEATURE_WCDMA HSUPA 2MS TTI.
03/08/11   su      Added code to support enabling and disabling of 
                   FEATURE_UEUT_IGNORE_CELL_BAR_RESERVE_STATUS using NV item.
03/07/11   su      Added code to support device type NV.
05/10/09   kp      Added support for demand paging RRC code using pragma.
05/08/09   ss      Updated Copyright Information
04/29/09   ss      Made changes to compiler warnings on gcc compilers.
03/30/09   ps      Removed changes to enable/disable HE field usingthe NV flag
                   NV_WCDMA_TEST_NV_1_I
03/26/09   ps      Made changes to include hw.h instead of hw_api.h, as hw_api.h
                   will be removed for some of the targets  
02/09/09   ps      Made changes to disbale enable HE field on UL based on
                   NV flag NV_WCDMA_TEST_NV_1_I
10/23/08   ps      Made changes to support FEATURE_WCDMA_TRIM_CAPABILITY
02/17/08   sm      Added function rrc_get_hsupa_category() which is called by L1
                   to get the hsupa category
02/01/08   sm      Modified default value of nv_hsupa_category when feature 
                   FEATURE_WCDMA HSUPA 2MS TTI is defined
10/08/07   da      Added support for reading HSUPA+CM NV item
08/29/06   sm/da   Made changes required for R6 ASN.1 migration.  Also merged
                   FEATURE WCDMA HSUPA, FEATURE_NV WCDMA_OPTIONAL_FEATURE_LIST_SUPPORT,
                   FEATURE_NV_WCDMA HSUPA CATEGORY
04/17/06   vk      Added support for reading NV Item HSDPA_COMPRESSED_MODE_ENABLED
                   under FEATURE HSDPA
03/28/06   vk      Added support for reading hsdpa_cat NV Item
08/31/05   vm      Added support for reading two new NV items: wcdma_dl_freq and
                   wcdma_dl_freq_enabled.
12/23/04   vn      Fixed lint error related to initialization of read_index
08/27/02   kc      Initialized Security Related NV Items to FALSE if user has
                   not written to NV RAM.
08/02/02   kc      Added support to read Integrity, Ciphering, Fake Security
                   Enabled/Disabled flags from NV
05/10/02   bu      Removed rrc_wait_nv(). Now we call the rrc_event_wait().
02/22/02   bu      Added case to assign default value to NV_ACQ_LIST_4_I in
                   rrc_get_nv().
02/20/02   bu      Added cases to give default values to 3 NV_ACQ_LIST items
                   in rrc_get_nv().
01/23/02   bu      Created a new file with NV routines.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "wcdma_variation.h"
#include <stringl/stringl.h>

#include "err.h"
#include "nv.h"
#include "fs_async_put.h"
#include "comdef.h"
#include "rrcsigs.h"
#include "dog.h"
#include "rrccspdb.h"
#include "nv_items.h"
#include "rrcdispatcher.h"
#include "rrcnv.h"
#include "fs_fcntl.h"
#include "fs_public.h"
#include "fs_sys_types.h"
#include "string.h"
#include "fs_lib.h"
#include "rrcsibproc.h"
#include "rrcueci.h"
#include "rrcllc.h"
#include "rrcllcoc.h"
#include "rrcenhstatecfg.h"
#include "mcfg_fs.h"
#include "rrcsmc.h"
#include "rrcrce.h"
#include "rrcdt.h"



/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.


===========================================================================*/
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#ifdef FEATURE_DUAL_WCDMA 
static  nv_cmd_type nvi_sim[MAX_RRC_STACKS];           /* nv command buffer */
static  nv_cmd_ext_type nvi_sim_ext[MAX_RRC_STACKS];
#define nvi nvi_sim[WAS_ID_M]
#else
static  nv_cmd_type nvi;           /* nv command buffer */
#endif

/* -----------------------------------------------------------------------
** Global Definitions
** ----------------------------------------------------------------------- */
#ifdef FEATURE_DUAL_WCDMA 
rrcnv_data_type rrcnv_data_sim[MAX_RRC_STACKS];
#define rrcnv_data rrcnv_data_sim[WAS_ID_M]
#else
rrcnv_data_type rrcnv_data;
#endif
/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */



/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this �. 
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


/*=========================================================================

FUNCTION     : rrc_get_hsupa_category

DESCRIPTION  : RRC returns the HSUPA Category to L1. RRC reads NV to determine 
               UE CAT (valid NV value, invalid NV value, NV not present, 
               NV reading failure)  

DEPENDENCIES : None

RETURN VALUE : HSUPA Category value 1..6

SIDE EFFECTS : None

=========================================================================*/
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
uint16 rrc_get_hsupa_category (sys_modem_as_id_e_type wrrc_as_id)
{
  return(rrcnv_data.wcdma_nv_hsupa_category);
}
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================
FUNCTION RRC_GET_NV

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type rrc_get_nv(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to space for item */
)
{
  uint8 i;

  nvi.tcb_ptr = rex_self();          /* notify this task when done */
  nvi.sigs = RRC_NV_SIG;
  nvi.done_q_ptr = NULL;             /* command goes on no queue when done */

  nvi.item = item;                 /* item to read */
  nvi.cmd = NV_READ_F;

  /* Set up NV so that it will read the data into the correct location */
  nvi.data_ptr = data_ptr;


  /* Clear the return signal, call NV, and wait for a response */
  (void) rex_clr_sigs( rex_self(), RRC_NV_SIG );
#ifdef FEATURE_DUAL_WCDMA
  nvi_sim_ext[wrrc_as_id].nvcmd = &nvi;
  nvi_sim_ext[wrrc_as_id].context = (uint16)rrc_get_as_id( wrrc_as_id);
  nv_cmd_ext(&nvi_sim_ext[wrrc_as_id]);
#else
  nv_cmd( &nvi );
#endif
  rrc_event_wait( RRC_NV_SIG, wrrc_as_id);

/* Handle a NV_NOTACTIVE_S or NV_FAIL_S status internally by replacing
** the random data returned with a default value of our own.  Items that
** share the same structure are lumped together in the switch.
*/

  if( nvi.status == NV_NOTACTIVE_S || nvi.status == NV_FAIL_S)
  {  
    WRRC_MSG2_HIGH("NV Item status was %d for %d ", nvi.status , nvi.item); 
    nvi.status = NV_DONE_S;

    switch( nvi.item )
    {           /* provide our own values */
      case NV_ACQ_DB_I:       /* stored ACQ database */

        /* Initialize Current Read Index to -1 */
        data_ptr->acq_db.curr_rd_index = ~0;

        /* Initialize Current Number of Pointers to 0 */
        data_ptr->acq_db.curr_wr_index = 0;

        data_ptr->acq_db.last_entry_invalid = TRUE;

        /* Initialize all values in ordered acquisition list in acq database to
        */
        for (i=0; i < MAX_ACQ_DB_ENTRIES; i++)
        {
          data_ptr->acq_db.acq_list_indices[i] = i;
        }
        break;

      default:
        nvi.status = NV_NOTACTIVE_S;
        break;
    }
  }
  else
  {
    if( nvi.status != NV_DONE_S )
    {
      WRRC_MSG2_ERROR( "NV Read Failed Item %d Code %d", nvi.item, nvi.status);
    }
  }

  return( nvi.status );
}


/*===========================================================================
FUNCTION RRC_PUT_NV

DESCRIPTION
  Write an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type rrc_put_nv(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
)
{

  nvi.tcb_ptr = rex_self();        /* Notify this task when done */
  nvi.sigs = RRC_NV_SIG;
  nvi.done_q_ptr = NULL;           /* command goes to no queue when done */

  nvi.item = item;                 /* item to write */
  nvi.cmd = NV_WRITE_F;

  nvi.data_ptr =  data_ptr;        /* the data to write */


  /* Clear the signal, call NV, wait for it to finish */
  (void) rex_clr_sigs( rex_self(), RRC_NV_SIG );
#ifdef FEATURE_DUAL_WCDMA
  nvi_sim_ext[wrrc_as_id].nvcmd = &nvi;
  nvi_sim_ext[wrrc_as_id].context = (uint16)rrc_get_as_id( wrrc_as_id);
  nv_cmd_ext(&nvi_sim_ext[wrrc_as_id]);
#else
  nv_cmd( &nvi );
#endif

  rrc_event_wait( RRC_NV_SIG, wrrc_as_id);
  
  if( nvi.status != NV_DONE_S )
  {
    WRRC_MSG2_ERROR( "NV Write Failed Item %d Code %d", nvi.item, nvi.status);
  }

  return( nvi.status );

}

/*===========================================================================
FUNCTION RRC_QUEUE_NV_WRITE

DESCRIPTION
  Posts a NV write request to async_efs_put

RETURN VALUE
  Zero if request is successful otherwise -1

DEPENDENCIES
===========================================================================*/
uint32 rrc_queue_nv_write(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr,          /* pointer to data for item */
  uint32 size
)
{

  char f_name[120];
  uint32 status = -1;

  (void)snprintf(f_name,sizeof(f_name),"/nvm/num/%d",item);
  
  status = efs_async_put(f_name, (void*)data_ptr, size,O_CREAT|O_AUTODIR, 0777);

  return status;
}

/*===========================================================================
FUNCTION RRC_REPLACE_NV

DESCRIPTION
  Replace an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type rrc_replace_nv(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
)
{

  nvi.tcb_ptr = rex_self();        /* Notify this task when done */
  nvi.sigs = RRC_NV_SIG;
  nvi.done_q_ptr = NULL;           /* command goes to no queue when done */

  nvi.item = item;                 /* item to write */
  nvi.cmd = NV_REPLACE_F;

  nvi.data_ptr =  data_ptr;        /* the data to write */


  /* Clear the signal, call NV, wait for it to finish */
  (void) rex_clr_sigs( rex_self(), RRC_NV_SIG );
#ifdef FEATURE_DUAL_WCDMA
  nvi_sim_ext[wrrc_as_id].nvcmd = &nvi;
  nvi_sim_ext[wrrc_as_id].context = (uint16)rrc_get_as_id( wrrc_as_id);
  nv_cmd_ext(&nvi_sim_ext[wrrc_as_id]);
#else
  nv_cmd( &nvi );
#endif
  rrc_event_wait( RRC_NV_SIG, wrrc_as_id);
  
  if( nvi.status != NV_DONE_S )
  {
    WRRC_MSG2_ERROR( "NV Replace Failed Item %d Code %d", nvi.item, nvi.status);
  }

  return( nvi.status );
}

/*===========================================================================
FUNCTION RRC_FREE_NV

DESCRIPTION
  Free an item in NV memory.  Wait till free is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type rrc_free_nv(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
)
{

  nvi.tcb_ptr = rex_self();        /* Notify this task when done */
  nvi.sigs = RRC_NV_SIG;
  nvi.done_q_ptr = NULL;           /* command goes to no queue when done */

  nvi.item = item;                 /* item to free */
  nvi.cmd = NV_FREE_F;

  nvi.data_ptr =  data_ptr;        /* the data to write */

  /* Clear the signal, call NV, wait for it to finish */
  (void) rex_clr_sigs( rex_self(), RRC_NV_SIG );
#ifdef FEATURE_DUAL_WCDMA
  nvi_sim_ext[wrrc_as_id].nvcmd = &nvi;
  nvi_sim_ext[wrrc_as_id].context = (uint16)rrc_get_as_id( wrrc_as_id);
  nv_cmd_ext(&nvi_sim_ext[wrrc_as_id]);
#else
  nv_cmd( &nvi );
#endif

  rrc_event_wait( RRC_NV_SIG, wrrc_as_id);
  
  if( nvi.status != NV_DONE_S )
  {
    WRRC_MSG2_ERROR( "NV Free Failed Item %d Code %d", nvi.item, nvi.status);
  }

  return( nvi.status );

}


/*===========================================================================

FUNCTION rrc_read_nv_wcdma_optional_feature_list

DESCRIPTION
  This  function gets the NV item NV_WCDMA_OPTIONAL_FEATURE_LIST_I and stores it in 
  rrc global variable 'wcdma_nv_optional_feature_list'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_wcdma_optional_feature_list
(
sys_modem_as_id_e_type wrrc_as_id
)
{

#ifdef FEATURE_NV_WCDMA_OPTIONAL_FEATURE_LIST_SUPPORT
  #error code not present
#else
  rrcnv_data.wcdma_nv_optional_feature_list = WNV_GET_DEFAULT(WNV_ID_WCDMA_OPTIONAL_FEATURE_LIST_I);
  WRRC_MSG0_HIGH("FEATURE_NV_WCDMA_OPTIONAL_FEATURE_LIST_SUPPORT not defined. ");
#endif /* FEATURE_NV_WCDMA_OPTIONAL_FEATURE_LIST_SUPPORT */
}  

/*===========================================================================

FUNCTION rrc_read_nv_wcdma_hsupa_category

DESCRIPTION
  This  function gets the NV item NV_WCDMA_HSUPA_CATEGORY_I and stores it in 
  rrc global variable 'wcdma_nv_hsupa_category'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_wcdma_hsupa_category
(
sys_modem_as_id_e_type wrrc_as_id
)
{
  uint8 local_hsupa_category = 0;

  /*read wcdma rrc release indicator from NV*/
  wnv_get_nv_msim( WNV_ID_WCDMA_HSUPA_DEFAULT_CATEGORY_I, 
                  &local_hsupa_category,
                  sizeof(local_hsupa_category),
                  wrrc_as_id);

  /* check for valid hsupa category in NV item - 
     this validity may differ from target to target. Need to take care of this check
     later by adding appropriate check for new target or 2ms support */
  if ((local_hsupa_category < 1) || (local_hsupa_category > 6))
  {
#ifdef FEATURE_WCDMA_DC_HSUPA
    /*
       RRC release version should be atleast rel-9 for DCHSUPA.
       If release version is set to anything below rel-9, use default EDCH category. 
    */
    if ((local_hsupa_category >= MAX_DC_HSUPA_CATEGORY)
        &&
        (rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL9))
    { 
      local_hsupa_category = MAX_DC_HSUPA_CATEGORY;
    }
    else
#endif
    {     
#ifdef FEATURE_WCDMA_16_QAM
    /*
       RRC release version should be greater than or equal to rel-7 for 16 qam.
       If release version is set to anything below rel-7, use default EDCH category. 
    */
      if ((local_hsupa_category >= MAX_UL_16QAM_HSUPA_CATEGORY)
          &&
          (rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL7))
      {
        local_hsupa_category = MAX_UL_16QAM_HSUPA_CATEGORY;       
      }
      else
#endif
      {  
        WRRC_MSG1_ERROR("RRCEUL: HSUPA category NV (nv item: 4210) contains invalid category(%d). set to default",local_hsupa_category);
        local_hsupa_category = RRC_WCMDA_HSUPA_DEFAULT_CATEGORY;
      }
    }
  }
  WRRC_MSG1_HIGH("RRCEUL: NV_WCDMA_HSUPA_CATEGORY_I(nv item: 4210) is set to %d", local_hsupa_category);

  /* use stored nv variable value to set rrc global variable */
  rrcnv_data.wcdma_nv_hsupa_category = local_hsupa_category;
}  


/*===========================================================================

FUNCTION rrc_get_wcdma_hsupa_cm_ctrl_nv

DESCRIPTION
  This  function gets the NV item wcdma_hsupa_cm_ctrl and stores it in 
  rrc global variable 'rrc_wcdma_hsupa_cm_enabled'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void rrc_get_wcdma_hsupa_cm_ctrl_nv
(
sys_modem_as_id_e_type wrrc_as_id
)
{
  /*read wcdma rrc release indicator from NV*/
  wnv_get_nv_msim(WNV_ID_WCDMA_HSUPA_CM_CTRL_I, &rrcnv_data.rrc_wcdma_hsupa_cm_enabled, sizeof(rrcnv_data.rrc_wcdma_hsupa_cm_enabled), wrrc_as_id);
  WRRC_MSG1_HIGH("wcdma_hsupa_cm_ctrl (nv item number 5090) is %d", rrcnv_data.rrc_wcdma_hsupa_cm_enabled);
} /* rrc_get_wcdma_hsupa_cm_ctrl_nv() */




/*===========================================================================

FUNCTION rrc_read_nv_items

DESCRIPTION
  This  function should call all the functions which read NV items to be used in RRC

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_items(sys_modem_as_id_e_type wrrc_as_id)
{
  rrc_read_nv_wcdma_optional_feature_list(wrrc_as_id);

  rrc_read_nv_wcdma_hsupa_category(wrrc_as_id);

  rrc_get_wcdma_hsupa_cm_ctrl_nv(wrrc_as_id);

}

/*===========================================================================

FUNCTION rrc_create_external_nv_item

DESCRIPTION
This function creates the conf file and opens the file for 
writing NV items. If the conf file is already created this 
function does not perform any operation. All new external NV 
items to be created should call rrc_write_to_efs with appropriate
arguments accordingly

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void rrc_create_external_nv_item(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Write conf/nv file and set default NV if it's missing */
  wnv_init_efs_conf_and_nv_files_msim(WNV_CONF_ID_RRC, wrrc_as_id);

  /* NV - 67256 */
  wnv_get_nv_msim(WNV_ID_RRC_WTOL_CM_SUPPORT, &rrcnv_data.rrc_wtol_cm_support_nv, sizeof(rrcnv_data.rrc_wtol_cm_support_nv), wrrc_as_id);
  /* NV - 67305 */
  wnv_get_nv_msim(WNV_ID_RRC_PPAC_SUPPORT, &rrcnv_data.rrc_ppac_support_nv, sizeof(rrcnv_data.rrc_ppac_support_nv), wrrc_as_id);
  /* NV - 67309 */
  wnv_get_nv_msim(WNV_ID_RRC_SAVE_ACQDB, &rrcnv_data.rrc_save_acqdb_nv, sizeof(rrcnv_data.rrc_save_acqdb_nv), wrrc_as_id);
  /* NV - 67293 */
  wnv_get_nv_msim(WNV_ID_RRC_BAND_PRIORITY_CONFIG,&rrcnv_data.rrc_nv_band_priority_config,sizeof(rrc_nv_band_priority_config_type), wrrc_as_id);
  /* NV - 67333 */
  wnv_get_nv_msim(WNV_ID_RRC_DISABLE_CPC_FDPCH, &rrcnv_data.rrc_disable_cpc_fdpch_nv, sizeof(rrcnv_data.rrc_disable_cpc_fdpch_nv), wrrc_as_id);
  /* NV - 72515 (may change)*/
  wnv_get_nv_msim(WNV_ID_RRC_DISABLE_CRASH_DEBUG_INFO, &rrcnv_data.rrc_disable_crash_debug_info_nv, sizeof(rrcnv_data.rrc_disable_crash_debug_info_nv), wrrc_as_id);
  /* NV - 67347 */
  wnv_get_nv_msim(WNV_ID_RRC_WTOL_PS_HO_SUPPORT, &rrcnv_data.wcdma_rrc_wtol_ps_ho_support_nv, sizeof(rrcnv_data.wcdma_rrc_wtol_ps_ho_support_nv), wrrc_as_id);
  /* NV - 69732 */
  wnv_get_nv_msim(WNV_ID_RRC_WTOL_TDD_PS_HO_SUPPORT, &rrcnv_data.wcdma_rrc_wtol_tdd_ps_ho_support_nv, sizeof(rrcnv_data.wcdma_rrc_wtol_tdd_ps_ho_support_nv), wrrc_as_id);

#ifdef FEATURE_WCDMA_DEBUG_ATTEMPT_FULL_SRCH
  /* NV - 70241 */
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_ENABLE_PSC_LOCK,&rrcnv_data.rrc_enable_psc_lock_nv ,sizeof(rrcnv_data.rrc_enable_psc_lock_nv), wrrc_as_id);
#endif

#ifdef FEATURE_FEMTO_CSG
  /* NV - 70256 */
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_CSG_PRUNE_COUNTER,&rrcnv_data.wcdma_rrc_csg_max_prune_count_nv,sizeof(rrcnv_data.wcdma_rrc_csg_max_prune_count_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_CSG_SRCH_CARRIER_SPACE,&rrcnv_data.wcdma_csg_srch_carrier_space_nv,sizeof(rrcnv_data.wcdma_csg_srch_carrier_space_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_ASF_TIMER,&rrcnv_data.wcdma_asf_timer_nv,sizeof(rrcnv_data.wcdma_asf_timer_nv), wrrc_as_id);
    
  /*Protection against EFS corruption*/
  if((rrcnv_data.wcdma_asf_timer_nv == 0)||(rrcnv_data.wcdma_asf_timer_nv > 4))
  {
    rrcnv_data.wcdma_asf_timer_nv = 5;
  }
#endif

  wnv_get_nv_msim(WNV_ID_RRC_IGNORE_CELL_BAR_RESERVE_STATUS, &rrcnv_data.rrc_ignore_cell_bar_nv, sizeof(rrcnv_data.rrc_ignore_cell_bar_nv), wrrc_as_id);

  /*For snow3g_security_algo support */
#ifdef FEATURE_WCDMA_SNOW3G_SECURITY
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_SNOW3G_ENABLED,&rrcnv_data.snow3g_security_algo_supported,sizeof(rrcnv_data.snow3g_security_algo_supported), wrrc_as_id);
#endif

#ifdef FEATURE_SMC_SRB2_SUSPENSION
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_SET_SRB2_ACT_TIME,&rrcnv_data.rrc_set_srb2_act_time_nv,sizeof(rrcnv_data.rrc_set_srb2_act_time_nv), wrrc_as_id);
#endif

  wnv_get_nv_msim(WNV_ID_RRC_CSFB_SKIP_SIBL1_OPT, &rrcnv_data.rrc_csfb_skip_sib11_opt_nv, sizeof(rrcnv_data.rrc_csfb_skip_sib11_opt_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_FREQ_LOCK_ITEM, &rrcnv_data.rrc_nv_channel_lock_item_value, sizeof(rrcnv_data.rrc_nv_channel_lock_item_value), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_DO_NOT_FORWARD_PAGE, &rrcnv_data.rrc_do_not_forward_page_nv, sizeof(rrcnv_data.rrc_do_not_forward_page_nv), wrrc_as_id);

#ifdef FEATURE_WCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB,&rrcnv_data.rrc_fast_return_to_lte_after_csfb_nv,sizeof(rrcnv_data.rrc_fast_return_to_lte_after_csfb_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB_TIMER,&rrcnv_data.rrc_fast_return_to_lte_after_csfb_timer_nv,sizeof(rrcnv_data.rrc_fast_return_to_lte_after_csfb_timer_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_DELAY_RETURN_TO_LTE_NV,&rrcnv_data.rrc_delay_return_to_lte_nv,sizeof(rrcnv_data.rrc_delay_return_to_lte_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB_SS,&rrcnv_data.rrc_fast_return_to_lte_after_csfb_ss_nv,sizeof(rrcnv_data.rrc_fast_return_to_lte_after_csfb_ss_nv),wrrc_as_id);
  WRRC_MSG1_HIGH("CSFB fast feature for SS enabled %d", rrcnv_data.rrc_fast_return_to_lte_after_csfb_ss_nv);
#endif

  /* NV - 70350 */
  wnv_get_nv_msim(WNV_ID_RRC_FREQ_SCAN_TIMER_IN_MS, &rrcnv_data.wcdma_rrc_freq_scan_timer_nv, sizeof(rrcnv_data.wcdma_rrc_freq_scan_timer_nv), wrrc_as_id);
  /* NV - 70351 */
  wnv_get_nv_msim(WNV_ID_RRC_DEEP_SLEEP_NO_SVC_TIMER_IN_MS, &rrcnv_data.wcdma_rrc_deep_sleep_no_svc_timer_nv, sizeof(rrcnv_data.wcdma_rrc_deep_sleep_no_svc_timer_nv), wrrc_as_id);
  /* NV - 70352 */
  wnv_get_nv_msim(WNV_ID_RRC_BPLMN_FREQ_SCAN_TIMER_IN_MS, &rrcnv_data.wcdma_rrc_bplmn_freq_scan_timer_nv, sizeof(rrcnv_data.wcdma_rrc_bplmn_freq_scan_timer_nv), wrrc_as_id);

#ifdef FEATURE_GANSS_SUPPORT
  wnv_get_nv_msim(WNV_ID_RRC_GANSS_SUPPORT_NV,&rrcnv_data.rrc_ganss_support_nv,sizeof(rrcnv_data.rrc_ganss_support_nv), wrrc_as_id);
#endif

  /* NV - 72599 */
  wnv_get_nv_msim(WNV_ID_RRC_DORMANCY_ENABLE,&rrcnv_data.rrc_dormancy_support_nv,sizeof(rrcnv_data.rrc_dormancy_support_nv), wrrc_as_id);
  if((rrcnv_data.rrc_dormancy_support_nv == 0)||(rrcnv_data.rrc_dormancy_support_nv > RRC_NV_DORMANCY_DISABLED))
  {
    rrcnv_data.rrc_dormancy_support_nv = RRC_NV_DORMANCY_ENABLED;
  }

  wnv_get_nv_msim(WNV_ID_RRC_SIB_SLEEP_SB,&rrcnv_data.wcdma_rrc_sib_sleep_sb_nv,sizeof(rrcnv_data.wcdma_rrc_sib_sleep_sb_nv), wrrc_as_id);

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
  wnv_get_nv_msim(WNV_ID_RRC_FE_FACH_SUPPORT,&rrcnv_data.rrc_fe_fach_support_nv,sizeof(rrcnv_data.rrc_fe_fach_support_nv), wrrc_as_id);
#endif

#ifdef FEATURE_DUAL_SIM
  wnv_get_nv_msim(WNV_ID_RRC_DSDS_CONCURRENCY_EXTENDED,&rrcnv_data.rrc_dsds_concurrency,sizeof(rrcnv_data.rrc_dsds_concurrency), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_DSDA_CONCURRENCY_EXTENDED,&rrcnv_data.rrc_dsda_concurrency,sizeof(rrcnv_data.rrc_dsda_concurrency), wrrc_as_id);

  /* Disabling DSDS/DSDA/TSTS concurrency for E-DRX and HS-RACH overriding the values set in NV */

#ifdef FEATURE_WCDMA_EFACH_MSIM
#else
  rrcnv_data.rrc_dsds_concurrency &= ~(NV_DS_EDRX_CONCURRENCY|NV_DS_HSRACH_CONCURRENCY);
#endif
  rrcnv_data.rrc_dsda_concurrency &= ~(NV_DS_EDRX_CONCURRENCY|NV_DS_HSRACH_CONCURRENCY);

  #ifdef FEATURE_TRIPLE_SIM
    wnv_get_nv_msim(WNV_ID_RRC_TSTS_CONCURRENCY_EXTENDED,&rrcnv_data.rrc_tsts_concurrency,sizeof(rrcnv_data.rrc_tsts_concurrency), wrrc_as_id);
    rrcnv_data.rrc_tsts_concurrency &= ~(NV_DS_EDRX_CONCURRENCY|NV_DS_HSRACH_CONCURRENCY);
  #endif
#endif

#ifdef FEATURE_WCDMA_MULTI_FREQ_BAND_INDICATOR
  wnv_get_nv_msim(WNV_ID_RRC_NV_ENABLE_MFBI_SUPPORT,&rrcnv_data.rrc_mfbi_support_nv,sizeof(rrcnv_data.rrc_mfbi_support_nv), wrrc_as_id);
#endif

#ifdef FEATURE_WCDMA_UL_COMPR
  wnv_get_nv_msim(WNV_ID_RRC_UL_COMPR_CAP_SUPPORT,&rrcnv_data.rrc_ul_compr_cap_nv,sizeof(rrcnv_data.rrc_ul_compr_cap_nv), wrrc_as_id);
#endif

  wnv_get_nv_msim(WNV_ID_RRC_PRIORITIZE_850_BAND,&rrcnv_data.rrc_prioritize_850_band_nv,sizeof(rrcnv_data.rrc_prioritize_850_band_nv), wrrc_as_id);

  wnv_get_nv_msim(WNV_ID_RRC_DISABLE_MEAS,&rrcnv_data.rrc_disable_meas_nv,sizeof(rrcnv_data.rrc_disable_meas_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_CONSTANT_320MS_DRX,&rrcnv_data.rrc_constant_320ms_drx_nv,sizeof(rrcnv_data.rrc_constant_320ms_drx_nv), wrrc_as_id);

#ifdef FEATURE_WCDMA_DEFAULT_CFG_CELLFACH
  wnv_get_nv_msim(WNV_ID_RRC_DEFAULT_CONFIG_IN_CELL_FACH,&rrcnv_data.rrc_default_cfg_in_cell_fach_nv,sizeof(rrcnv_data.rrc_default_cfg_in_cell_fach_nv), wrrc_as_id);
#endif
  wnv_get_nv_msim(WNV_ID_RRC_SIB7_EXP_TIME,&rrcnv_data.rrc_sib7_time_nv,sizeof(rrcnv_data.rrc_sib7_time_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_ENABLE_MEAS_ID_EXTN,&rrcnv_data.rrc_meas_id_extn_support,sizeof(rrcnv_data.rrc_meas_id_extn_support), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_IDLE_FEATURES,&rrcnv_data.wcdma_rrc_idle_features,sizeof(rrcnv_data.wcdma_rrc_idle_features), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_DB_DC_BAND_COMB_NV_CONTROL,&rrcnv_data.wcdma_rrc_db_dc_band_comb_nv_control,sizeof(rrcnv_data.wcdma_rrc_db_dc_band_comb_nv_control), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_FREQ_SKIP_RANGE_NV,&rrcnv_data.wcdma_freq_skip_range_nv,sizeof(rrcnv_data.wcdma_freq_skip_range_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_ENABLE_TA_FOR_CS_DATACALL_NV,&rrcnv_data.enable_ta_for_cs_datacall_nv,sizeof(rrcnv_data.enable_ta_for_cs_datacall_nv),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_NV_ENABLE_READ_SIBS_FOR_RESEL,&rrcnv_data.rrc_read_sibs_for_resel_enabled_nv,sizeof(rrcnv_data.rrc_read_sibs_for_resel_enabled_nv),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_HALF_DRX_LEN,&rrcnv_data.rrc_half_drx_len_nv,sizeof(rrcnv_data.rrc_half_drx_len_nv),wrrc_as_id);
#ifdef FEATURE_DUAL_SIM
  wnv_get_nv_msim(WNV_ID_RRC_CHANGE_TRM_PRIORITY_TMR_NV,&rrcnv_data.rrc_change_trm_priority_tmr_nv,sizeof(rrcnv_data.rrc_change_trm_priority_tmr_nv),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_ALLOW_LTA_TIME_NV,&rrcnv_data.wcdma_rrc_time_to_allow_lta_during_access_priority,sizeof(rrcnv_data.wcdma_rrc_time_to_allow_lta_during_access_priority),wrrc_as_id);
#endif
  wnv_get_nv_msim(WNV_ID_RRC_NV_DEFER_RESEL_TIMER_FOR_L2_ACK,&rrcnv_data.wcdma_rrc_defer_cel_resel_waiting_for_l2_ack,sizeof(rrc_nv_defer_cel_resel_waiting_for_l2_ack_type),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_MSIM_DB_DC_BAND_COMB_NV_CONTROL,&rrcnv_data.wcdma_rrc_msim_db_dc_band_comb_nv_control,sizeof(rrcnv_data.wcdma_rrc_msim_db_dc_band_comb_nv_control),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_PSUEDO_SIB19,&rrcnv_data.rrc_nv_psuedo_sib19_nv,sizeof(rrcnv_data.rrc_nv_psuedo_sib19_nv),wrrc_as_id);
#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
  wnv_get_nv_msim(WNV_ID_RRC_SUPPORT_LTA,&rrcnv_data.wcdma_rrc_support_lta,sizeof(wcdma_rrc_support_lta_type),wrrc_as_id);
#endif
  wnv_get_nv_msim(WNV_ID_RRC_NV_ENABLE_READ_SIBS,&rrcnv_data.rrc_read_sibs_enabled_nv,sizeof(rrcnv_data.rrc_read_sibs_enabled_nv),wrrc_as_id);


  /*Validation checks*/
  if(rrcnv_data.rrc_nv_psuedo_sib19_nv.utra_priority > 7)
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.utra_priority = WRRCNV_PSEUDO_SIB19_UTRA_PRI;
  }

  if(rrcnv_data.rrc_nv_psuedo_sib19_nv.s_priority_search1 > 31)
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.s_priority_search1 = WRRCNV_PSEUDO_SIB19_S_PRI_SRCH1;
  }

  if(rrcnv_data.rrc_nv_psuedo_sib19_nv.thresh_serving_low > 31)
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.thresh_serving_low = WRRCNV_PSEUDO_SIB19_THR_SERV_LOW;
  }

  if(rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_priority > 7)
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_priority = WRRCNV_PSEUDO_SIB19_EUTRA_PRI;
  }

  if(rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_meas_bandwidth > 5)
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_meas_bandwidth = WRRCNV_PSEUDO_SIB19_EUTRA_MEAS_BW;
  }

  if(rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_thresh_high > 31)
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_thresh_high = WRRCNV_PSEUDO_SIB19_THR_X_HIGH;
  }

  if(rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_thresh_low > 31)
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_thresh_low = WRRCNV_PSEUDO_SIB19_THR_X_LOW;
  }

  if ((rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_qrxlevmin < -70) || (rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_qrxlevmin > -22))
  {
    rrcnv_data.rrc_nv_psuedo_sib19_nv.eutra_qrxlevmin = WRRCNV_PSEUDO_SIB19_EUTRA_QRXLEVMIN;
  }

  wnv_get_nv_msim(WNV_ID_RRC_MSIM_OPTIMIZATIONS,&rrcnv_data.wcdma_rrc_msim_opt,sizeof(rrcnv_data.wcdma_rrc_msim_opt),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_BAND66_PLMN_SUPPORT,&rrcnv_data.efs_plmn_id,sizeof(rrcnv_data.efs_plmn_id),wrrc_as_id);

  if(rrcnv_data.efs_plmn_id.num_mnc_digits == 2)
  {
    rrcnv_data.efs_plmn_id.mnc[2] = WNV_ID_RRC_PLMN_ID_DEFAULT;
  }

  wnv_get_nv_msim(WNV_ID_RRC_GSM_FALLBACK_INFO,&rrcnv_data.rrc_gsm_fallback_info,sizeof(rrcnv_data.rrc_gsm_fallback_info),wrrc_as_id);
  /*Validation Checks*/
  if(rrcnv_data.rrc_gsm_fallback_info.mcc_count > MCC_SIZE)
  {
    rrcnv_data.rrc_gsm_fallback_info.mcc_count = 0;
  }
  if(rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_priority > 7)
  {
    rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_priority = WRRCNV_SIB19_SERVING_UTRA_PRIORITY;
  }
  if(rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_s_PrioritySearch1 > 31)
  {
    rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_s_PrioritySearch1 = WRRCNV_SIB19_SERVING_UTRA_S_PRIORITYSEARCH1;
  }
  if(rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_threshServingLow > 31)
  {
    rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_threshServingLow = WRRCNV_SIB19_SERVING_UTRA_THRESHSERVINGLOW;
  }
  if((rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_priority > 7) ||
     (rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_priority >= rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_priority))
  {
    rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_priority = WRRCNV_SIB19_GSM_PRIORITY;
    rrcnv_data.rrc_gsm_fallback_info.sib19_Serving_utra_priority = WRRCNV_SIB19_SERVING_UTRA_PRIORITY;
  }
  if(rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_threshx_high > 31)
  {
    rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_threshx_high = WRRCNV_SIB19_GSM_THRESHX_HIGH;
  }
  if(rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_threshx_low > 31)
  {
    rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_threshx_low = WRRCNV_SIB19_GSM_THRESHX_LOW;
  }
  if ((rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_q_rxlevmin < -58) || (rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_q_rxlevmin > -13))
  {
    rrcnv_data.rrc_gsm_fallback_info.sib19_gsm_q_rxlevmin = WRRCNV_SIB19_GSM_Q_RXLEVMIN;
  }
  wnv_get_nv_msim(WNV_ID_RRC_CELL_BAR_UPON_RACH_FAILURE_NV,&rrcnv_data.rrc_cell_bar_upon_rach_failure_nv,sizeof(rrcnv_data.rrc_cell_bar_upon_rach_failure_nv),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_CELL_BAR_DURATION_SECS_NV,&rrcnv_data.rrc_cell_bar_duration_secs_nv,sizeof(rrcnv_data.rrc_cell_bar_duration_secs_nv),wrrc_as_id);

wnv_get_nv_msim(WNV_ID_RRC_BUFFER_SMC_SUPPORT, &rrcnv_data.rrc_buffer_smc_support, sizeof(rrcnv_data.rrc_buffer_smc_support),wrrc_as_id);

  wnv_get_nv_msim(WNV_ID_RRC_NV_REQ_LTE_FOR_TRUNCATED_CA,&rrcnv_data.rrc_req_lte_for_truncated_ca_nv,sizeof(rrcnv_data.rrc_req_lte_for_truncated_ca_nv),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_NV_REQ_LTE_FOR_TRUNCATED_CA_ON_MCC_LIST,&rrcnv_data.rrc_req_lte_for_truncated_ca_on_mcc_list_nv,sizeof(rrc_mcc_list_type),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_REDIRECT_TO_LTE_AFTER_SRVCC_NV,&rrcnv_data.rrc_wtol_redirect_after_srvcc_nv,sizeof(rrcnv_data.rrc_wtol_redirect_after_srvcc_nv),wrrc_as_id);

  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_INFO_NV,&rrcnv_data.rrc_pseudo_meas_info,sizeof(rrcnv_data.rrc_pseudo_meas_info),wrrc_as_id);

    /* Validation checks*/
  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t1 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t1 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T1_DEFAULT;
  }

  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t2 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t2 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T2_DEFAULT;
  }

  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t3 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t3 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T3_DEFAULT;
  }

  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t4 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t4 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T4_DEFAULT;
  }
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_RXAGC_NV,&rrcnv_data.rrc_pseudo_meas_rxagc_info,sizeof(rrcnv_data.rrc_pseudo_meas_rxagc_info),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_TRIGGERS_NV,&rrcnv_data.rrc_pseudo_meas_triggers_info,sizeof(rrcnv_data.rrc_pseudo_meas_triggers_info),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_RSRP_RSRQ_NV,&rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info,sizeof(rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info),wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_FORBIDDEN_EARFCN_TIMER_NV,&rrcnv_data.rrc_pseudo_meas_forbidden_earfcn_timer_nv,sizeof(rrcnv_data.rrc_pseudo_meas_forbidden_earfcn_timer_nv),wrrc_as_id);
  WRRC_MSG2_HIGH("Pseudo Meas Feature enabled %d, Forbidden EARFCN timer %d", rrcnv_data.rrc_pseudo_meas_info.feature_enabled, rrcnv_data.rrc_pseudo_meas_forbidden_earfcn_timer_nv);
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_DEFER_TMR_VALUE_FIRST_TIME_IN_DCH_NV,&rrcnv_data.rrc_pseudo_meas_defer_tmr_value_first_time_in_dch_nv,sizeof(rrcnv_data.rrc_pseudo_meas_defer_tmr_value_first_time_in_dch_nv),wrrc_as_id );
  wnv_get_nv_msim(WNV_ID_RRC_ENABLE_HI_ACCESS_CLASS_NV,&rrcnv_data.rrc_enable_hi_ac,sizeof(rrcnv_data.rrc_enable_hi_ac),wrrc_as_id );
}

/*===========================================================================

FUNCTION rrc_read_nv_item

DESCRIPTION
This function internally calls rrc_create_external_nv_item() 
in order to read or create(is not present) the NV items.This uses the globally defined NV conf file,
NV conf item, address of default value to be written and size of the NV item to call 
the above functions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_item(sys_modem_as_id_e_type wrrc_as_id)
{
  wnv_get_nv_msim(WNV_ID_RRC_DEVICE_TYPE, &rrcnv_data.rrc_device_type_nv, sizeof(rrcnv_data.rrc_device_type_nv), wrrc_as_id);

  rrc_create_external_nv_item(wrrc_as_id);
}


/*===========================================================================

FUNCTION rrc_read_nv_item

DESCRIPTION
This function internally calls rrc_create_external_nv_item() 
in order to read or create(is not present) the NV items.This uses the globally defined NV conf file,
NV conf item, address of default value to be written and size of the NV item to call 
the above functions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_efs_forbid_cells(sys_modem_as_id_e_type wrrc_as_id)
{
  rrc_efs_forbid_cell_type forbid_cells[RRC_NV_NUM_FORBID_CELLS*4];
  uint8 i = 0, j = 0;
  
  /*To get the size of EFS file. As number of entries can vary*/
  struct fs_stat stat_temp;
  
 (void)mcfg_fs_stat("/nv/reg_files/modem/forbidden_cell_list", &stat_temp, MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_NONE);

  memset(rrcnv_data.rrc_nv_forbid_cells,0,sizeof(rrcnv_data.rrc_nv_forbid_cells));

  if((mcfg_fs_read("/nv/reg_files/modem/forbidden_cell_list",forbid_cells,MIN(stat_temp.st_size,sizeof(forbid_cells)),MCFG_FS_TYPE_EFS,MCFG_FS_SUBID_NONE))!= MCFG_FS_STATUS_OK)
  {
     WRRC_MSG0_HIGH("EFS_GET not successful.. Error_code");
  }
  else
  {
     WRRC_MSG0_HIGH("EFS_GET successful ");
  }

  for(i = 0; (i < RRC_NV_NUM_FORBID_CELLS*4 && j < RRC_NV_NUM_FORBID_CELLS); i++)
  {
    if(forbid_cells[i].rat == SYS_SYS_MODE_WCDMA)
    {
      rrcnv_data.rrc_nv_forbid_cells[j].sys_plmn_id = forbid_cells[i].sys_plmn_id;
      rrcnv_data.rrc_nv_forbid_cells[j].cell_id = forbid_cells[i].cell_id;
      j++;
    }
  }
  rrcnv_data.num_forbid_cells = j;
  
}

/*===========================================================================
FUNCTION RRC_PUT_EFS

DESCRIPTION
  Write data to given EFS file.

RETURN VALUE
  Boolean

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
boolean rrc_put_efs(sys_modem_as_id_e_type wrrc_as_id,char *efs_file, void *data,fs_size_t size)
{
  enum mcfg_fs_status_e ret_val;
  mcfg_fs_sub_id_e_type sub_id;
  if(wrrc_as_id == SYS_MODEM_AS_ID_1)
  {
    sub_id = MCFG_FS_SUBID_0;
  }
  else
  {
    sub_id = MCFG_FS_SUBID_1;
  }
  if((ret_val = mcfg_fs_write(efs_file,data,size,MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR,MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,sub_id)) != MCFG_FS_STATUS_OK)
  {
      WRRC_MSG1_HIGH("EFS_PUT not successful.. Error_code: %d",ret_val);
      return FALSE;
  }
  else
  {
     WRRC_MSG0_HIGH("EFS_PUT successful");
     return TRUE;
  }

}

/*===========================================================================
FUNCTION rrc_get_from_efs

DESCRIPTION
  Reads the data from given EFS file

RETURN VALUE
  Boolean

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/

boolean rrc_get_from_efs(sys_modem_as_id_e_type wrrc_as_id,char *efs_file, void *value,fs_size_t size)
{
  enum mcfg_fs_status_e ret_val;
  mcfg_fs_sub_id_e_type sub_id;
  if(wrrc_as_id == SYS_MODEM_AS_ID_1)
  {
    sub_id = MCFG_FS_SUBID_0;
  }
  else
  {
    sub_id = MCFG_FS_SUBID_1;
  }
  if((ret_val = mcfg_fs_read(efs_file,value,size,MCFG_FS_TYPE_EFS,sub_id))!= MCFG_FS_STATUS_OK)
  {
     WRRC_MSG1_HIGH("EFS_GET not successful.. Error_code: %d",ret_val);
     return FALSE;
  }
  else
  {
     return TRUE;
  }
}

/*===========================================================================

FUNCTION rrc_read_all_nv_items

DESCRIPTION
This function internally calls other NV read functions

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_all_nv_items(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Read NV items Fake Security, Integrity Enabled and
     Ciphering Enabled from NV */
  rrcsmc_get_nv_items(wrrc_as_id);
  rrcdt_get_nv_items(wrrc_as_id);
	
  /* read wcdma rrc version from NV */
  rrc_get_wcdma_rrc_version_nv(wrrc_as_id);
	
  rrcueci_get_nv_items(wrrc_as_id);
  rrcllc_get_cm_hsdpa_nv_item(wrrc_as_id);
	
  rrcueci_get_wtog_nacc_nv_item(wrrc_as_id);
	
#ifdef FEATURE_UMTS_PDCP
  rrc_get_wcdma_rrc_pdcp_disabled_nv(wrrc_as_id);
#endif /* FEATURE_UMTS_PDCP */
	
  rrc_read_nv_items(wrrc_as_id);
	
#ifdef FEATURE_WCDMA_HS_FACH
  rrc_create_opt_feature_nv_item(wrrc_as_id);
  rrc_read_opt_feature_nv_item(wrrc_as_id);
#endif

  rrc_read_nv_item(wrrc_as_id);

  rrc_read_efs_forbid_cells(wrrc_as_id);
}

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================
FUNCTION rrc_nv_refresh_map_sub_index_to_as_id

DESCRIPTION
  Maps SUB index from the MCFG callback to W-AS-ID.

RETURN VALUE
  sys_modem_as_id_e_type

DEPENDENCIES
  None
===========================================================================*/
sys_modem_as_id_e_type rrc_nv_refresh_map_sub_index_to_as_id(mcfg_refresh_index_type sub_index)
{
  sys_modem_as_id_e_type wrrc_as_id = SYS_MODEM_AS_ID_1;

  switch (sub_index)
  {
    case MCFG_REFRESH_INDEX_0:
      wrrc_as_id = SYS_MODEM_AS_ID_1;
      break;
      
    case MCFG_REFRESH_INDEX_1:
      wrrc_as_id = SYS_MODEM_AS_ID_2;
      break;
      
    default:
      WRRC_MSG0_ERROR("Invalid SUB Index received");
      break;
  }

  WRRC_MSG1_HIGH("Mapping:: sub_index=%d", sub_index);

  return wrrc_as_id;
}

/*===========================================================================
FUNCTION rrc_nv_refresh_map_as_id_to_sub_index

DESCRIPTION
  Maps W-AS-ID to SUB index .

RETURN VALUE
  mcfg_refresh_index_type

DEPENDENCIES
  None
===========================================================================*/
static mcfg_refresh_index_mask_type rrc_nv_refresh_map_as_id_to_sub_index(sys_modem_as_id_e_type wrrc_as_id)
{
  mcfg_refresh_index_mask_type sub_index = MCFG_REFRESH_INDEX_MASK_NONE;

  switch (wrrc_as_id)
  {
    case SYS_MODEM_AS_ID_1:
      sub_index = MCFG_REFRESH_INDEX_MASK_0;
      break;
      
    case SYS_MODEM_AS_ID_2:
      sub_index = MCFG_REFRESH_INDEX_MASK_1;
      break;
      
    default:
      WRRC_MSG0_ERROR("Invalid W AS ID received");
      break;
  }
  
  WRRC_MSG1_HIGH("Mapping:: sub_index=%d", sub_index);

  return sub_index;
}

/*===========================================================================
FUNCTION rrc_nv_refresh_validate_cb_params

DESCRIPTION
  validates params received from MCFG in call back function.

RETURN VALUE
  TRUE: Params are valid and stored
  FALSE: Invalid params received

DEPENDENCIES
  None
===========================================================================*/
static boolean rrc_nv_refresh_validate_cb_params(mcfg_refresh_info_s_type *p_info)
{
  boolean  param_valid = FALSE;
  
  /* AS ID unavailable. Use the legacy API */
  MSG_4(MSG_SSID_WCDMA_RRC, MSG_LEGACY_HIGH,"MCFG callback params:: type=%d, slot=%d, sub=%d, ref_id=%d", 
    p_info->type, 
    p_info->slot_index, 
    p_info->sub_index, 
    p_info->reference_id);
  
  switch (p_info->type)
  {
    case MCFG_REFRESH_TYPE_SUBS:
    {
      if (p_info->sub_index <= MCFG_REFRESH_INDEX_MAX)
      {
        param_valid = TRUE;
      }
      break;
    }

    /* Refresh based on sub_id is only supported in UTRAN */
    default:
      break;
  }

  return param_valid;
}


/*===========================================================================

FUNCTION rrc_read_nv_item_refresh

DESCRIPTION
This function reads the NV items when SIM hotswap happens

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_item_refresh(sys_modem_as_id_e_type wrrc_as_id)
{
  /* NV - 67256 */
  wnv_get_nv_msim(WNV_ID_RRC_WTOL_CM_SUPPORT, &rrcnv_data.rrc_wtol_cm_support_nv, sizeof(rrcnv_data.rrc_wtol_cm_support_nv), wrrc_as_id);
  /* NV - 67305 */
  wnv_get_nv_msim(WNV_ID_RRC_PPAC_SUPPORT, &rrcnv_data.rrc_ppac_support_nv, sizeof(rrcnv_data.rrc_ppac_support_nv), wrrc_as_id);
  /* NV - 67309 */
  wnv_get_nv_msim(WNV_ID_RRC_SAVE_ACQDB, &rrcnv_data.rrc_save_acqdb_nv, sizeof(rrcnv_data.rrc_save_acqdb_nv), wrrc_as_id);
  /* NV - 67293 */
  wnv_get_nv_msim(WNV_ID_RRC_BAND_PRIORITY_CONFIG,&rrcnv_data.rrc_nv_band_priority_config,sizeof(rrc_nv_band_priority_config_type), wrrc_as_id);  
  /* NV - 67333 */
  wnv_get_nv_msim(WNV_ID_RRC_DISABLE_CPC_FDPCH, &rrcnv_data.rrc_disable_cpc_fdpch_nv, sizeof(rrcnv_data.rrc_disable_cpc_fdpch_nv), wrrc_as_id);
  /* NV - 67347 */
  wnv_get_nv_msim(WNV_ID_RRC_WTOL_PS_HO_SUPPORT, &rrcnv_data.wcdma_rrc_wtol_ps_ho_support_nv, sizeof(rrcnv_data.wcdma_rrc_wtol_ps_ho_support_nv), wrrc_as_id);
  /* NV - 69732 */
  wnv_get_nv_msim(WNV_ID_RRC_WTOL_TDD_PS_HO_SUPPORT, &rrcnv_data.wcdma_rrc_wtol_tdd_ps_ho_support_nv, sizeof(rrcnv_data.wcdma_rrc_wtol_tdd_ps_ho_support_nv), wrrc_as_id);

#ifdef FEATURE_FEMTO_CSG
  /* NV - 70256 */
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_CSG_PRUNE_COUNTER,&rrcnv_data.wcdma_rrc_csg_max_prune_count_nv,sizeof(rrcnv_data.wcdma_rrc_csg_max_prune_count_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_CSG_SRCH_CARRIER_SPACE,&rrcnv_data.wcdma_csg_srch_carrier_space_nv,sizeof(rrcnv_data.wcdma_csg_srch_carrier_space_nv), wrrc_as_id);
#endif

#ifdef FEATURE_SMC_SRB2_SUSPENSION
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_SET_SRB2_ACT_TIME,&rrcnv_data.rrc_set_srb2_act_time_nv,sizeof(rrcnv_data.rrc_set_srb2_act_time_nv), wrrc_as_id);
#endif

  wnv_get_nv_msim(WNV_ID_RRC_CSFB_SKIP_SIBL1_OPT, &rrcnv_data.rrc_csfb_skip_sib11_opt_nv, sizeof(rrcnv_data.rrc_csfb_skip_sib11_opt_nv), wrrc_as_id);

#ifdef FEATURE_WCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB,&rrcnv_data.rrc_fast_return_to_lte_after_csfb_nv,sizeof(rrcnv_data.rrc_fast_return_to_lte_after_csfb_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB_TIMER,&rrcnv_data.rrc_fast_return_to_lte_after_csfb_timer_nv,sizeof(rrcnv_data.rrc_fast_return_to_lte_after_csfb_timer_nv), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_DELAY_RETURN_TO_LTE_NV,&rrcnv_data.rrc_delay_return_to_lte_nv,sizeof(rrcnv_data.rrc_delay_return_to_lte_nv), wrrc_as_id);
  WRRC_MSG2_HIGH("CSFB Fast Feature enabled %d, CSFB Fast timer %d", rrcnv_data.rrc_fast_return_to_lte_after_csfb_nv, rrcnv_data.rrc_fast_return_to_lte_after_csfb_timer_nv);
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB_SS,&rrcnv_data.rrc_fast_return_to_lte_after_csfb_ss_nv,sizeof(rrcnv_data.rrc_fast_return_to_lte_after_csfb_ss_nv), wrrc_as_id);
  WRRC_MSG3_HIGH("CSFB Fast Feature enabled %d, CSFB Fast timer %d CSFB Fast feature for SS enabled %d", rrcnv_data.rrc_fast_return_to_lte_after_csfb_nv, rrcnv_data.rrc_fast_return_to_lte_after_csfb_timer_nv, rrcnv_data.rrc_fast_return_to_lte_after_csfb_ss_nv);
#endif

  /* NV - 70350 */
  wnv_get_nv_msim(WNV_ID_RRC_FREQ_SCAN_TIMER_IN_MS, &rrcnv_data.wcdma_rrc_freq_scan_timer_nv, sizeof(rrcnv_data.wcdma_rrc_freq_scan_timer_nv), wrrc_as_id);
  /* NV - 70351 */
  wnv_get_nv_msim(WNV_ID_RRC_DEEP_SLEEP_NO_SVC_TIMER_IN_MS, &rrcnv_data.wcdma_rrc_deep_sleep_no_svc_timer_nv, sizeof(rrcnv_data.wcdma_rrc_deep_sleep_no_svc_timer_nv), wrrc_as_id);
  /* NV - 70352 */
  wnv_get_nv_msim(WNV_ID_RRC_BPLMN_FREQ_SCAN_TIMER_IN_MS, &rrcnv_data.wcdma_rrc_bplmn_freq_scan_timer_nv, sizeof(rrcnv_data.wcdma_rrc_bplmn_freq_scan_timer_nv), wrrc_as_id);

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
  wnv_get_nv_msim(WNV_ID_RRC_FE_FACH_SUPPORT,&rrcnv_data.rrc_fe_fach_support_nv,sizeof(rrcnv_data.rrc_fe_fach_support_nv), wrrc_as_id);
#endif

#ifdef FEATURE_DUAL_SIM
  wnv_get_nv_msim(WNV_ID_RRC_DSDS_CONCURRENCY_EXTENDED,&rrcnv_data.rrc_dsds_concurrency,sizeof(rrcnv_data.rrc_dsds_concurrency), wrrc_as_id);
  wnv_get_nv_msim(WNV_ID_RRC_DSDA_CONCURRENCY_EXTENDED,&rrcnv_data.rrc_dsda_concurrency,sizeof(rrcnv_data.rrc_dsda_concurrency), wrrc_as_id);
  #ifdef FEATURE_TRIPLE_SIM
    wnv_get_nv_msim(WNV_ID_RRC_TSTS_CONCURRENCY_EXTENDED,&rrcnv_data.rrc_tsts_concurrency,sizeof(rrcnv_data.rrc_tsts_concurrency), wrrc_as_id);
  #endif
#endif

#ifdef FEATURE_WCDMA_MULTI_FREQ_BAND_INDICATOR
  wnv_get_nv_msim(WNV_ID_RRC_NV_ENABLE_MFBI_SUPPORT,&rrcnv_data.rrc_mfbi_support_nv,sizeof(rrcnv_data.rrc_mfbi_support_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("MFBI NV enabled %d", rrcnv_data.rrc_mfbi_support_nv);
#endif

#ifdef FEATURE_WCDMA_UL_COMPR
  wnv_get_nv_msim(WNV_ID_RRC_UL_COMPR_CAP_SUPPORT,&rrcnv_data.rrc_ul_compr_cap_nv,sizeof(rrcnv_data.rrc_ul_compr_cap_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("UL compr cap NV %d", rrcnv_data.rrc_ul_compr_cap_nv);
#endif

  wnv_get_nv_msim(WNV_ID_RRC_NV_ENABLE_READ_SIBS_FOR_RESEL,&rrcnv_data.rrc_read_sibs_for_resel_enabled_nv,sizeof(rrcnv_data.rrc_read_sibs_for_resel_enabled_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("Read SIBs for every reselection NV enabled %d", rrcnv_data.rrc_read_sibs_for_resel_enabled_nv);

  wnv_get_nv_msim(WNV_ID_RRC_CELL_BAR_UPON_RACH_FAILURE_NV,&rrcnv_data.rrc_cell_bar_upon_rach_failure_nv,sizeof(rrcnv_data.rrc_cell_bar_upon_rach_failure_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("rrc_cell_bar_upon_rach_failure_nv set to %d", rrcnv_data.rrc_cell_bar_upon_rach_failure_nv);
  
  wnv_get_nv_msim(WNV_ID_RRC_CELL_BAR_DURATION_SECS_NV,&rrcnv_data.rrc_cell_bar_duration_secs_nv,sizeof(rrcnv_data.rrc_cell_bar_duration_secs_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("rrc_cell_bar_duration_secs_nv set to %d", rrcnv_data.rrc_cell_bar_duration_secs_nv);

  wnv_get_nv_msim(WNV_ID_RRC_NV_ENABLE_READ_SIBS,&rrcnv_data.rrc_read_sibs_enabled_nv,sizeof(rrcnv_data.rrc_read_sibs_enabled_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("Read SIBs to W NV enabled %d", rrcnv_data.rrc_read_sibs_enabled_nv);

  wnv_get_nv_msim(WNV_ID_RRC_NV_REQ_LTE_FOR_TRUNCATED_CA,&rrcnv_data.rrc_req_lte_for_truncated_ca_nv,sizeof(rrcnv_data.rrc_req_lte_for_truncated_ca_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("Request lte for truncated_ca nv set to %d", rrcnv_data.rrc_req_lte_for_truncated_ca_nv);

  wnv_get_nv_msim(WNV_ID_RRC_NV_REQ_LTE_FOR_TRUNCATED_CA_ON_MCC_LIST,&rrcnv_data.rrc_req_lte_for_truncated_ca_on_mcc_list_nv,sizeof(rrc_mcc_list_type), wrrc_as_id);
  WRRC_MSG1_HIGH("Request lte for truncated_ca on MCC list of elements %d", rrcnv_data.rrc_req_lte_for_truncated_ca_on_mcc_list_nv.count);
  wnv_get_nv_msim(WNV_ID_RRC_REDIRECT_TO_LTE_AFTER_SRVCC_NV,&rrcnv_data.rrc_wtol_redirect_after_srvcc_nv,sizeof(rrcnv_data.rrc_wtol_redirect_after_srvcc_nv), wrrc_as_id);
  WRRC_MSG1_HIGH("rrc_wtol_redirect_after_srvcc_nv set to %d", rrcnv_data.rrc_wtol_redirect_after_srvcc_nv);
wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_INFO_NV,&rrcnv_data.rrc_pseudo_meas_info,sizeof(rrcnv_data.rrc_pseudo_meas_info),wrrc_as_id);
  WRRC_MSG1_HIGH("rrc_pseudo_meas_info feature set to %d", rrcnv_data.rrc_pseudo_meas_info.feature_enabled);

    /* Validation checks*/
  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t1 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t1 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T1_DEFAULT;
  }

  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t2 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t2 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T2_DEFAULT;
  }

  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t3 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t3 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T3_DEFAULT;
  }

  if(rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t4 < RRC_PSEUDO_MEAS_MIN_TIMER_VAL)
  {
    rrcnv_data.rrc_pseudo_meas_info.freq_L_srch_t4 = RRC_PSEUDO_MEAS_FREQ_L_SRCH_T4_DEFAULT;
  }
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_RXAGC_NV,&rrcnv_data.rrc_pseudo_meas_rxagc_info,sizeof(rrcnv_data.rrc_pseudo_meas_rxagc_info),wrrc_as_id);
  WRRC_MSG2_HIGH("Rxagc timer set to %d Rxagc threshold set to %d", rrcnv_data.rrc_pseudo_meas_rxagc_info.timer_rxagc,rrcnv_data.rrc_pseudo_meas_rxagc_info.rxagc_threshold);
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_TRIGGERS_NV,&rrcnv_data.rrc_pseudo_meas_triggers_info,sizeof(rrcnv_data.rrc_pseudo_meas_triggers_info),wrrc_as_id);
  WRRC_MSG1_HIGH("triggers_for_pseudo_meas set to %d", rrcnv_data.rrc_pseudo_meas_triggers_info.triggers_for_pseudo_meas);
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_RSRP_RSRQ_NV,&rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info,sizeof(rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info),wrrc_as_id);
  WRRC_MSG2_HIGH("RSRP set to %d RSRQ set to %d", rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info.rsrp,rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info.rsrq);
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_FORBIDDEN_EARFCN_TIMER_NV,&rrcnv_data.rrc_pseudo_meas_forbidden_earfcn_timer_nv,sizeof(rrcnv_data.rrc_pseudo_meas_forbidden_earfcn_timer_nv),wrrc_as_id);
  WRRC_MSG2_HIGH("Pseudo Meas Feature enabled %d, Forbidden EARFCN timer %d", rrcnv_data.rrc_pseudo_meas_info.feature_enabled, rrcnv_data.rrc_pseudo_meas_forbidden_earfcn_timer_nv);
  wnv_get_nv_msim(WNV_ID_RRC_PSEUDO_MEAS_DEFER_TMR_VALUE_FIRST_TIME_IN_DCH_NV,&rrcnv_data.rrc_pseudo_meas_defer_tmr_value_first_time_in_dch_nv,sizeof(rrcnv_data.rrc_pseudo_meas_defer_tmr_value_first_time_in_dch_nv),wrrc_as_id );
#endif
  wnv_get_nv_msim(WNV_ID_RRC_ENABLE_HI_ACCESS_CLASS_NV,&rrcnv_data.rrc_enable_hi_ac,sizeof(rrcnv_data.rrc_enable_hi_ac),wrrc_as_id );
}

/*===========================================================================
FUNCTION rrc_read_nv_refresh

DESCRIPTION
  Reads the NV/EFS values on receiving NV refresh indication

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
void rrc_read_nv_refresh(sys_modem_as_id_e_type wrrc_as_id)
{
  /* function gets the NV item NV_WCDMA_RRC_VERSION_I  */
  rrc_get_wcdma_rrc_version_nv(wrrc_as_id);

  /* function gets NV item NV_HSDPA_CAT_I  */
  rrcueci_get_nv_items(wrrc_as_id);

  /* function gets NV item NV_HSDPA_COMPRESSED_MODE_ENABLED_I */
  rrcllc_get_cm_hsdpa_nv_item(wrrc_as_id);

  /* function gets the NV item wcdma_hsupa_cm_ctrl */
  rrc_get_wcdma_hsupa_cm_ctrl_nv(wrrc_as_id);

  /* function gets the NV item NV_WCDMA_HSUPA_CATEGORY_I */
  rrc_read_nv_wcdma_hsupa_category(wrrc_as_id);

  /* function creates the EFS item RRC_FEATURE_NV_CONF_FILE */
  rrc_create_opt_feature_nv_item(wrrc_as_id);

  /* function reads the EFS item  */
  rrc_read_opt_feature_nv_item(wrrc_as_id);

  /* Creates and writes EFS item */
  rrc_read_nv_item_refresh(wrrc_as_id);
}

/*===========================================================================
FUNCTION rrc_post_nv_refresh_ind

DESCRIPTION
  Callback fucntion invoked by MCFG to post NV refresh indication

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
boolean rrc_post_nv_refresh_ind_cb(mcfg_refresh_info_s_type *p_info)
{
  boolean cb_status;

  sys_modem_as_id_e_type wrrc_as_id = SYS_MODEM_AS_ID_1;

    /* Get the command buffer */
  rrc_cmd_type *cmd_ptr = rrc_get_int_cmd_buf(wrrc_as_id);

  /* Validate the callback before processing them */
  cb_status = rrc_nv_refresh_validate_cb_params(p_info);

  if(cb_status)
  {
#ifdef FEATURE_DUAL_WCDMA 
    wrrc_as_id = rrc_nv_refresh_map_sub_index_to_as_id(p_info->sub_index);
#endif

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = RRC_NV_REFRESH_IND;

  /* Copy the structures to local command */
  WCDMA_MEMCPY(&cmd_ptr->cmd.rrc_nv_refresh, 
               sizeof(rrc_mcfg_refresh_info_s_type),
               p_info,
               sizeof(rrc_mcfg_refresh_info_s_type));

  rrc_put_int_cmd(wrrc_as_id,  cmd_ptr );
  return TRUE;
}
  else
  {
    return FALSE;
  }
}

/*===========================================================================
FUNCTION rrc_nv_refresh_mcfg_regiser_cb

DESCRIPTION
  Function used to register for callback for receiving NV refresh indication

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
void rrc_nv_refresh_mcfg_regiser(sys_modem_as_id_e_type wrrc_as_id)
{
  mcfg_refresh_registration_s_type mcfg_refresh;

  /* Default ORDER for refresh registration 
     * certain task needs to refresh the NV items before the other tasks, similar to the concept of RC init task group. 
     * All the tasks  except policyman now tentatively register with default order 50 for integration. 
     *  The real order # might get bumped up or down depending on the integration result.
     */        
  mcfg_refresh.order = MCFG_REFRESH_ORDER_50;

  /* Client interested in getting refresh callback for all slots, MCFG_REFRESH_INDEX_MASK_ALL(Binary 0x7) should be used */  
  mcfg_refresh.slot_mask = MCFG_REFRESH_INDEX_MASK_NONE;

#ifdef FEATURE_DUAL_WCDMA
  /* Client interested in getting refresh callback for that particular sub */
  mcfg_refresh.sub_mask = rrc_nv_refresh_map_as_id_to_sub_index(wrrc_as_id);
#else
  /* Client interested in getting refresh callback for all subs, MCFG_REFRESH_INDEX_MASK_ALL(Binary 0x7) should be used as WCDMA can be in either SUBs */
  mcfg_refresh.sub_mask = MCFG_REFRESH_INDEX_MASK_ALL;  
#endif

  /* "GROUP" field is reserved for future enhancement, not for this FR. 
     * The NV/EFS items might get divided into couple different groups. 
     * The client will only receive refresh call back for certain groups that it cares about
     */        
  mcfg_refresh.group = 0;

  /* Callback fucntion to receive the NV refresh notification */  
  mcfg_refresh.cb = rrc_post_nv_refresh_ind_cb;

  /* Register with MCFG api */
  if(!mcfg_refresh_register(&mcfg_refresh))
  {
    WRRC_MSG0_ERROR("NV refresh registration failed");
  }
}

#endif

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION wrrc_qsh_dump_nv_vals

DESCRIPTION
  This function dumps nv values

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
 __attribute__((section(".uncompressible.text")))
size_t wrrc_qsh_dump_nv_vals(sys_modem_as_id_e_type wrrc_as_id, uint8* dump_addr, size_t size )
{
  wrrc_qsh_dump_tag_nv_values nv_values;
  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_addr,WRRC_QSH_DUMP_TAG_NV_VALUES,
                                   (sizeof(wrrc_qsh_dump_tag_nv_values) - sizeof(qsh_dump_tag_hdr_s)));
  WCDMA_MEMCPY(&nv_values,sizeof(wrrc_qsh_dump_tag_nv_values),
  (wrrc_qsh_dump_tag_nv_values*)dump_addr, sizeof(wrrc_qsh_dump_tag_nv_values));
  nv_values.wcdma_nv_hsupa_category = rrc_get_hsupa_category(wrrc_as_id);
  nv_values.nv_hsdpa_category_debug = rrcnv_data.nv_hsdpa_category;
  nv_values.rrc_feature_nv_item_value = rrcnv_data.rrc_feature_nv_item_value;
#ifdef FEATURE_DUAL_SIM
  nv_values.rrc_dsds_concurrency = rrcnv_data.rrc_dsds_concurrency;
  nv_values.rrc_dsda_concurrency = rrcnv_data.rrc_dsda_concurrency;
  nv_values.wcdma_look_for_concurrency_nv_debug = rrc_dsds_int_data.wcdma_look_for_concurrency_nv & 0x01;
#endif
  nv_values.rrc_nv_rel_indicator_debug = rrc_nv_rel_indicator;
  //nv_values.rrc_default_cfg_in_cell_fach_nv_debug = rrc_default_cfg_in_cell_fach_nv;
  nv_values.feature_dch_fach_opt_enabled_debug = rrcllc_return_feature_dch_fach_opt_enabled(wrrc_as_id) & 0x01;

  if(sizeof(nv_values) >= size)
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_nv_values*)dump_addr, size,&nv_values, size);
    return ((size_t)size);
  }
  else
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_nv_values*)dump_addr,sizeof(nv_values), &nv_values, sizeof(nv_values));
    return ((size_t)(sizeof(nv_values)));
  }
}
#endif
/*===========================================================================
FUNCTION rrc_get_band_comb_control_nv

DESCRIPTION
  This function dumps nv values

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/

uint16 rrc_get_band_comb_control_nv(sys_modem_as_id_e_type wrrc_as_id)
{

#ifdef FEATURE_DUAL_SIM
  if(rrc_dsds_int_data.wcdma_look_for_concurrency_nv == TRUE)
  {
    if(rrc_dsds_int_data.wcdma_device_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY)
    {
      /* Multi sim band combinations are always be a subset of SS band combinations */
      return (rrcnv_data.wcdma_rrc_msim_db_dc_band_comb_nv_control[WRRC_DSDS_MODE] &
              rrcnv_data.wcdma_rrc_db_dc_band_comb_nv_control);
    }
    else if(rrc_dsds_int_data.wcdma_device_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
    {
      return (rrcnv_data.wcdma_rrc_msim_db_dc_band_comb_nv_control[WRRC_DSDA_MODE] & 
              rrcnv_data.wcdma_rrc_db_dc_band_comb_nv_control);
    }
#ifdef FEATURE_TRIPLE_SIM 
    else if(rrc_dsds_int_data.wcdma_device_mode == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY)
    {
      return (rrcnv_data.wcdma_rrc_msim_db_dc_band_comb_nv_control[WRRC_TSTS_MODE] &
              rrcnv_data.wcdma_rrc_db_dc_band_comb_nv_control);
    }
#endif
  }
#endif
  return (rrcnv_data.wcdma_rrc_db_dc_band_comb_nv_control);
}

/*===========================================================================
FUNCTION rrcnv_get_msim_opt_enabled_nv

DESCRIPTION
  This function returns msim opt nv enabled status

DEPENDENCIES
  None


RETURN VALUE
  uint8


SIDE EFFECTS
  None

===========================================================================*/

uint8 rrcnv_get_msim_opt_enabled_nv(sys_modem_as_id_e_type wrrc_as_id)
{
  return (rrcnv_data.wcdma_rrc_msim_opt.opt_enable_mask);
}

/*===========================================================================
FUNCTION rrcnv_get_msim_opt_block_lta_timer

DESCRIPTION
  This function gets LTA block timer nv value

DEPENDENCIES
  None


RETURN VALUE
  uint16


SIDE EFFECTS
  None

===========================================================================*/

uint16 rrcnv_get_msim_opt_block_lta_timer(sys_modem_as_id_e_type wrrc_as_id)
{
  return (rrcnv_data.wcdma_rrc_msim_opt.block_lta_tmr);
}

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcnv_data

DESCRIPTION
This function dumps the NV globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcnv_data(sys_modem_as_id_e_type wrrc_as_id)
{
  qsh_mdump_collect_high(&rrcnv_data,sizeof(rrcnv_data));
  qsh_mdump_collect_high(&rrc_nv_rel_indicator,sizeof(rrc_nv_rel_indicator));
}
#endif

