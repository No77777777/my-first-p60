 /*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  L A Y E R  1  - M D S P   I N T E R F A C E
                    mDSP Debug Interface Module

GENERAL DESCRIPTION

   This file contains all the debugging and logging functionality for the
   mDSP driver and firmware.


EXTERNALIZED FUNCTIONS


  Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmdsp6/src/mdsp_debug.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ------- ------------------------------------------------------
27/01/20    rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
12/05/15     pjr     CR836628 Move variable declaration out of for loop
20/02/15     ab      CR773948 GL1 Support for provding Dumps and Metrics for QSH
16/03/15     pg      CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
23/01/15     npt     CR697236 Mainline FEATURE_GERAN_VSTMR
20/10/14     df      CR735116 Tidy logging cmds sent to GFW
06/08/14     df      CR536059 Full asynchronous sleep support in idle
02/07/14     sp      CR688572: Memory opts - reduce global data.
30/05/14     pg      CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
28/05/14     cja     CR657368 Fix Klockwork issue of NULL data space pointer
21/05/14     ssh     CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
07/04/14     cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF + HwFN incmd trace as 16-bit
04/04/14     cah     CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
12/03/14     pa      CR630228:Bolt: Changes due to obsolete FW API cleanup
25/02/14     jj      CR622701 fix KW errors 
27/01/14     pa      CR607716:GL1 changes related to legacy GFW interface changes.
23/12/13     cja     CR592613 Add mutex for mdsp cmd trace buffer
22/11/13     sk      CR576246 Ensure Clenup scripts status not cleared while populating startup scripts inside the QTA gap
22/11/13     cja     CR455527 Remove featurisation from files in API directory
22/11/13     jj      CR580519 log  hw fn send over  asynch command 
25/10/13     ws      CR473460 - W/T segment loading, removed WCDMA API calls
21/08/13     npt     CR531633 - Remove FEATURE_DIME_MODEM dependencies 
16/07/13     zc      CR470197 Replace memcpy with memscpy
20/06/13     cs      Major Triton TSTS Syncup
16/05/13     ab      CR487396 - Support EGPRS IR Reduced Window in GFW
09/05/13     cs      TSTS Updates
15/03/13     cja     CR462223 Clear monitor CCS buffers if gap aborted, reduce F3
31/01/13     pg      CR446164: Reset IRAT RF CCS event buffer status to IDLE
28/01/12     pg      CR430354 Ensure all RF CCS event buffers are in IDLE or COMPLETED state
17/01/13     cja     CR439545 If gap aborted set IRAT CCS scripts to idle
12-02-13     pjr     CR451837 modified mdsp_log_qdsp6_async_cmd to log correct
                     async command
08/01/12     gs      CR438601 Tx script split into 2
07/12/12     cja     CR429173 Remove Dime compiler warnings
06/12/12     cja     CR383165 Remove compiler warnings
07/11/12     cja     CR415642 Do not flag mdsp stalled as error
29/10/12     cja     CR403253 Make SBI collision an error.
24/09/12     dv      CR377931 Only check SBi/GRFC buffers when they are used.
20/09/12     pg      Add semaphore check skipping for RF buffer unused by FW
08/09/12     ky      Not resetting the ccs buffer state to IDLE during the sempahore checks
24/07/12     ky      Changes for new TX CCS interface changes
20/06/12     ky      Added support of using new CCS interface and passing new CCS
                     buffer pointers in RF task messages
08/06/12     cja     CR363269 Do not clear IRAT SBI/NonSBI buffers
25/05/12     pg      Add support for RF Task
01/05/12     ws      CR355391 - Deprecate arm.h, armasm.h and hw.h
25/04/12     ws      CR 355838 FR 1604 - GERAN changes to cleanup WCDMA- GL1 API
28/03/12     cja     CR338055 Recovery for Nikel RxFE Stuck
14/03/12     pg      Add support for local HWIO macros (msm.h deprecated)
22/02/12     og      CR338781. Added async interrupt logging to the mdsp_cmd_trace_buf.
22/11/10     ky      mdsp_nv_recovery_restart_type is defined and this structure is pouplated after
                     reading nv item from efs. if the recovery restart is enabled then mdsp drivers
                     ignore the errors other wise it does error fatal and also dump the mdsp memory.
                     mdspsvc provided api to update this nv item read value. so updated this value
                     during mdsp_init
02/02/12     cja     CR329883 For num cmds remove semaphore bit.
19/09/11     jj      CR307435 I/Q logging bug fix
10/06/11     cja     Change mdsp_log_cmd_struct to improve logging
20/05/11     ws      Fixed Compile warnings, removed unused mdsp_efs_write()
08/04/11     ky      Brought SPI feature changes under FEATURE_GSM_RAFT_SPI_CONTROL
07/04/11     ky      Changed sbi_infor pointer to rtr_sbi_infor pointer in SPI feature
14/02/11      npt    CR264682 Add support for asynch dsp completion interrupt
03/11/10      ky     Fix the compiler warnings for 9k
12/10/10      ky     Added qsym count to mdsp cmd trace buffer
18/10/2010    og    FEATURE_LTE_TO_GSM
28/09/10      ky     Added seq num MSG ERR in cases of mdsp commands not processed
                     error and printing SBI/GRFC buffer adderess when the buffer is
                     not processed. This information will help in debugging the crashes
                     in 9k
02/09/10      ky     Crashing 9k SW whenever serious error occured(mdsp halt,
                     commands not processed, buffers are not processed,sbi/grc
                     buffer overflows). Currently dsp errors are ignored but need
                     to modify in the future
16/08/10      ky     IRAT Klockwork fix
10/08/10      ws     Corrected Q6 Featurisation for MOB environment
29/07/10      ky     Synchronous copying of IQ samples from FW is introduced in QDSP6
19/07/10      ky     Klocwork fixes
14/07/10      ws     Fixed compile error in IQ logging code
13/07/10      ky     Fixed 9k warnings
09/06/10      cja    Merge optimised shared memory buffers (to fix compilation error)
09/06/10      og     removal of CM startup MODEM_TX_CTL hack.
02/07/10      ws     Support for IQ logging on QDSP6
20/05/10      og     9k irat updates.
12/05/10      pg     CR 238873: Ensure size of mdsp_cmd_trace_buf is scaled correctly on
                     Q6 targets
17/03/10      og     Further x2g and g2x updates.
17/03/10      ws     Added featurisation to seperate Data mover code from FEATURE_GSM_MDSP_IR
04/02/09      ws     Removed leagcy Q4 MDSP services and image for Q6 targets
19/01/10      ws     CR 223701 - Corrrect typo in featurisation to corectly
                     disable mdsp logging
23/12/09      ws     Added volatile type for accessing pMdspHostIfBaseStore
08/12/09      ws     Added FW frame number to mdsp stall F3
09/11/09      ky     Added new function for q6 logging commands
06/11/09      ws     Removed MDSP SVC app logging as not supported on 9K
05/11/09      ky     jjDebugFTick is replaced with gfwFTickCounter in shared memory for Q6
05/08/09      ws     Removed FEATURE_GSM_MDSP_DTM it's now mainlined
23/07/09      ws     Added FEATURE_GSM_GPRS_QDSP6 to remove QDSP4 references as part
                     of GERAN DSP migration to QDSP6
06/06/09      ab     Feature FEATURE_GSM_EGPRS_IR_OPT for LCU
26/03/09      ws     Added PACKED_POST for QDSP6 targets
26/01/09      cs     Remove chunks logging can be added in later for Mustang
25/01/09      agv    Add back in mDSP chunks logging for Mustang
07/11/08      agv    Filter RSSI=0, so mDSP won't crash if NV1027 is 1 for
                     rx metrics. The initial mask was for power scan only.
19/09/08      agv    Added WPLT and GPLT defines around mDSP Stall check
                     to allow PLT builds to compile.
20/08/08      og     Mustang bring-up changes - disable chunk logging.
15/08/08      agv    Filter RSSI=0, so mDSP won't crash if NV1027 is 1.
11/08/08      agv    CR151870 - WL1 sends an AGC cmd in sleep wakeup that
                     causes an mDSP stall. It's only a problem if NV1027 is 1.
26/07/07      ws     Moved FEATURE_MDSP_ERR_MASK_DUMP_ALWAYS_ON to custgsmdrv.h
23/07/07      cs     Fix compiler warning
19/07/07      og     UOOS changes, reduce number of measured samples.CR119147.
09/07/07      agv    Temp fix for RSSI==0 crash is NV1027 = 1.
02/03/07      agv    Stage 1 of enhanced mDSP error handling.
13/12/06      agv    Prevent crash if mDSP returns zero length chunk buffer.
24/10/06      ip     Added use of FEATURE_GSM_PLT for GPLT builds.
02/20/06      gfr    Mainline command and apps logging features, add
                     support for mDSP services apps logging
01/23/06      gfr    Add error log, increase number of error parameters.
                     Updated mDSP memory dump to contain the above.
12/09/05      gfr    Move loading of NV items into mdsp_intf.c
11/07/05      gfr    Created mdsp_set_check_cm_cleanup_fn function to replace
                     global variable, removed obselete
                     FEATURE_GSM_MDSP_DM_ACTIVE_INT
11/01/05      gfr    Trigger mdsp halt when dumping memory
10/04/05      gfr    Clear correct buffer for fw_setup_buffer
09/22/05      gfr    Removed FEATURE_SBI_MDSP_CONTROLLER
09/20/05      gfr    Suppress mDSP warning about offset 4999
08/19/05      gfr    Improve clarity of mdsp_check_semaphores errors.
                     Enable the GRFC trace buffer.
08/16/05      gfr    Convert tabs to spaces, make all lines less than 90
                     characters long
08/11/05      gfr    Increase length of command trace to support larger
                     gsm_sync_rx command.  Add support to save the grfc
                     write buffer in the mem dump.
07/29/05      gfr    DTM support
07/06/05      gfr    Clear mDSP error/warning immediately after reading to
                     reduce race condition with fw setting between read and write
06/07/05      gfr    Support for logging all GRFC writes, catch mdsp stalled
05/25/05      gfr    Improve cmd trace buffer
05/18/05      gfr    Rename mdsp_pdch_id to mdsp_timeslot_type
05/04/05      yh     Log mdsp apps and mdsp cmds on mdsp warning
04/28/05      gfr    Added check for DM stalled
04/13/05      gfr    Add support for gsmWarning variable
04/08/05      gfr    Use macros for parsing decoded EGPRS header
04/07/05      gfr    Support for third word of gsmError being timestamp
04/07/05      gfr    Disable IR coding scheme check.
03/14/05      yh     Support for mdsp apps log.
03/11/05      gfr    Do not dump memory on fcch/sch buffer overflow errors,
                     add support for preventing mdsp services from reloading
                     if we are trying to dump memory
03/07/05      gfr    Disable validating the IR checksum on AMSS
02/10/05      gfr    Improved support for verifying IR data
01/31/05      gfr    Support for verifying IR data
01/28/05      yh     Changed mdsp_debug_init() API to receive a wait signal.
01/24/05      gfr    Initial version.
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"

#include "sys_type.h"
#include "mcs_hwio.h"

#include "mdsp_debug.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_cmd.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"

#include "gtmrs.h"
#include "gtmrs_g.h"
#include "vtmrs_g.h"
#include "rex.h"
#include "geran_msgs.h"
#include "log.h"
#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"
#include "fs_public.h"
#include "nv.h"
#include "task.h"
#include "time_svc.h"
#include "gmutex.h"
#include "gl1_mutex.h"

#include "stringl.h"

#include "gl1_hw_g.h"
#include "geran_dual_sim_g.h"

#include "gfw_sw_intf.h"
#include "modem_fw_memmap.h"

#ifdef FEATURE_WCDMA
#include "wl1api.h"
#endif
/*==========================================================================

                LOCAL DEBUG DEFINES

==========================================================================*/




/* Define this to validate all collected IR data, Q6 target not supported yet */
#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
#define DEBUG_MDSP_IR_DATA

/* Define this to verify the IR data checksum - very time consuming and can
   report spurious errors when the DM is not complete in time */
#ifdef FEATURE_GSM_PLT
#error code not present
#endif
#endif


/* Extra includes that are conditional on the above definitions */

#ifdef DEBUG_MDSP_IR_DATA
#error code not present
#endif




/*==========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

==========================================================================*/

/* Sequence numbers for the debug buffer */
extern uint16 mdsp_sync_rx_burst_seq_num_fifo[3];

/* The UE is in GSM Mode or not. */
extern boolean l1_check_gsm_state( gas_id_t gas_id );

#ifdef FEATURE_MDSP_ERR_MASK_DUMP_ALWAYS_ON
/* Error Masks as agreed with FW team. */
/* mdsp_error[0] 0000 0000 0000 0001 0010 1100 1100 0111; 1 - Error Enabled */
#define MDSP_ERROR_0_MASK                 0x00012CC7
/* mdsp_error[1] 0000 0000 0000 0011 0000 0101 0011 0000; 1 - Error Enabled */
#define MDSP_ERROR_1_MASK                 0x00030530
#else
/* DSP error mask - Set bits to 0 that you wish to ignore as an error     */
#define MDSP_ERROR_ALL_ERRORS             0xFFFFFFFF
#endif

/* DSP warning mask - Set bits to 0 that you wish to ignore as an error     */
#define MDSP_WARNING_OFFSET_4999          0x00000001
#define MDSP_WARNING_FCCH_BUFF_OVERFLOW   0x00000010
#define MDSP_WARNING_SCH_BUFF_OVERFLOW    0x00000020
#define MDSP_WARNING_ALL_WARNINGS         0xFFFFFFFF

/*--------------------------------------------------------------------------
                mDSP ERROR LOGGING
--------------------------------------------------------------------------*/

/* Keep track of last errors reported */
typedef struct
{
   uint32              FN;
   boolean             compressed_mode;
   mdsp_gsm_error_type cause;
   uint32              params[3];
} mdsp_error_log_type;

#define MAX_LOG_ERRORS 20


/*--------------------------------------------------------------------------
                mDSP APPS LOGGING
--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------
                mDSP COMMAND LOGGING
--------------------------------------------------------------------------*/

typedef PACKED struct PACKED_POST
{
   uint32           FN;
   uint16           cnt;
   uint16           seq;
   uint16           cmd;
   uint16           params[MDSP_CMD_LOG_PARAM_COUNT];
} mdsp_log_cmd_diag_struct;

/*--------------------------------------------------------------------------
                mDSP MEMORY DUMPING
--------------------------------------------------------------------------*/

/* Use a different file name if we are using mDSP services logging since
   the file will not actually contain the mDSP memory */
#ifdef FEATURE_MDSPSVC_LOG
#define MDSP_MEM_DUMP_FILE     "mdsp_gsm_debug"
#else
#define MDSP_MEM_DUMP_FILE     "mdsp_mem"
#endif
#define MDSP_MEM_DUMP_VERSION  0x0006
#define MDSP_MEM_DUMP_MAGIC    0xDEADBEEF


/*--------------------------------------------------------------------------
                mDSP GRFC WRITES LOGGING
--------------------------------------------------------------------------*/

#ifdef DEBUG_MDSP_LOG_GRFC_WRITES
typedef struct
{
   uint32 FN;
   uint16 reg;
   uint32 data;
} mdsp_grfc_write_trace_buf_type;

#define MDSP_LOG_GRFC_WRITES_LEN 40
#endif


/*--------------------------------------------------------------------------
                MISCELLANEOUS
--------------------------------------------------------------------------*/

#ifdef DEBUG_MDSP_IR_DATA_CHECKSUM
#error code not present
#endif

/*===========================================================================
  MULTI-DATASPACED PRIVATE DATA
===========================================================================*/

/*===========================================================================
  PUBLIC AND PRIVATE DATASPACES
===========================================================================*/
 mdsp_debug_private_data_t mdsp_debug_private_data[NUM_GERAN_DATA_SPACES];

/*==========================================================================

                FUNCTION IMPLEMENTATIONS FOR MODULE

==========================================================================*/

/*===========================================================================

FUNCTION get_private_dataspace_ptr

DESCRIPTION
  This function gets the private data space for a specified GAS ID

===========================================================================*/
static mdsp_debug_private_data_t* get_private_dataspace_ptr(const gas_id_t gas_id )
{
  mdsp_debug_private_data_t *data_ptr;

  data_ptr =
      &(mdsp_debug_private_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

  return ( data_ptr );
}

/*===========================================================================

FUNCTION init_private_data_space

DESCRIPTION
  This function initialises a private data space

===========================================================================*/
static void init_private_data_space(gas_id_t gas_id )
{
  mdsp_debug_private_data_t* data = get_private_dataspace_ptr( gas_id );

#ifdef FEATURE_MDSP_ERR_MASK_DUMP_ALWAYS_ON
  /* Set new Error Masks as agreed with FW team. */
  data->mdsp_error_mask[0] = MDSP_ERROR_0_MASK;
  data->mdsp_error_mask[1] = MDSP_ERROR_1_MASK;
#else
  data->mdsp_error_mask[0] = MDSP_ERROR_ALL_ERRORS;
  data->mdsp_error_mask[1] = MDSP_WARNING_ALL_WARNINGS;
#endif

  data->mdsp_warning_mask[0] = MDSP_WARNING_ALL_WARNINGS;
  data->mdsp_warning_mask[1] = MDSP_WARNING_ALL_WARNINGS;


  /* Keep track of last jjDebugFTick value to catch mDSP stalls */
  data->last_jjdebugftick = 0;
  data->mdsp_stalled      = FALSE;

  data->saved_dspHalt = 0;
  data->last_fn = 0;
}

/*===========================================================================

FUNCTION mdsp_debug_init

DESCRIPTION
   This function initializes debug variables.

===========================================================================*/
void mdsp_debug_init (gas_id_t gas_id )

{
   uint32 error_mask[2];
   uint32 warning_mask[2];

   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   init_private_data_space(gas_id);

#ifdef FEATURE_MDSP_ERR_MASK_DUMP_ALWAYS_ON
   /* This now specifies the Critical Errors. */
   error_mask[0] = MDSP_ERROR_0_MASK;
   error_mask[1] = MDSP_ERROR_1_MASK;
#else
   /* Currently, do not ignore any errors */
   error_mask[0] = MDSP_ERROR_ALL_ERRORS;
   error_mask[1] = MDSP_ERROR_ALL_ERRORS;
#endif

   /* Initialise the reset pending flag. */
   private_data_ptr->mdsp_memory_dump.reset_pending = FALSE;

   mdsp_set_dsp_error_mask(error_mask[0], error_mask[1] , gas_id );

   /* Set the default dsp warning mask - we do not care about FCCH or SCH
      buffer overflows, or offset 4999 warnings */
   warning_mask[0] = ~(MDSP_WARNING_FCCH_BUFF_OVERFLOW | MDSP_WARNING_SCH_BUFF_OVERFLOW);
   warning_mask[1] = ~(MDSP_WARNING_OFFSET_4999);
   mdsp_set_dsp_warning_mask(warning_mask[0], warning_mask[1] , gas_id );

#ifdef FEATURE_MDSP_ERR_MASK_DUMP_ALWAYS_ON
   /* Regardless of NV 1027 enable crash/chunks. */
   private_data_ptr->mdsp_memory_dump.enabled = TRUE;
   private_data_ptr->mdsp_memory_dump.suppressed =
        ((uint16)private_data_ptr->mdsp_memory_dump.enabled & 0xFFFE);
#else
   /* Process the NV item */
   if (private_data_ptr->mdsp_nv_mem_dump_enabled.status == NV_DONE_S)
   {
      private_data_ptr->mdsp_memory_dump.enabled =
        (private_data_ptr->mdsp_nv_mem_dump_enabled.item.mdsp_mem_dump_enabled > 0 ? TRUE : FALSE);
      private_data_ptr->mdsp_memory_dump.suppressed =
        (private_data_ptr->mdsp_nv_mem_dump_enabled.item.mdsp_mem_dump_enabled & 0xFFFE);
   }
   else
   {
      private_data_ptr->mdsp_memory_dump.enabled = FALSE;
   }
#endif

   if (private_data_ptr->mdsp_memory_dump.enabled)
   {
      MSG_GERAN_HIGH_1_G("mDSP memory dump enabled (suppressed=0x%04x)",
               private_data_ptr->mdsp_memory_dump.suppressed);
   }

   /* Suppress memory dump if it's a warning */
   private_data_ptr->mdsp_memory_dump.suppressed |= (1 << (uint16)MDSP_GSM_ERROR_WARNING);

   /* Suppress memory dump if it's a RSSI = 0; handle power scan/rx metric. */
#if !defined(MDSP_DEBUG_WARNING_RSSI_0)
   private_data_ptr->mdsp_memory_dump.suppressed |= (1 << (uint16)MDSP_GSM_ERROR_INVALID_POWER);
   private_data_ptr->mdsp_memory_dump.suppressed |= (1 << (uint16)MDSP_GSM_ERROR_INVALID_METRICS);
#endif


   /* Assume we are running to start! */
   private_data_ptr->mdsp_stalled = 0;
}

/*===========================================================================

FUNCTION get_mdsp_nv_mem_dump_enabled

DESCRIPTION
   This function provides a public interface to mdsp_nv_mem_dump_enabled

===========================================================================*/
mdsp_nv_item_type* mdsp_debug_nv_mem_dump_enabled(gas_id_t gas_id )
{
   return &(get_private_dataspace_ptr( gas_id )->mdsp_nv_mem_dump_enabled);
}

/*===========================================================================

FUNCTION get_mdsp_nv_mem_dump_enabled

DESCRIPTION
   This function provides a public interface to mdsp_nv_recovery_restart

===========================================================================*/
mdsp_nv_recovery_restart_type* mdsp_debug_nv_recovery_restart(gas_id_t gas_id )
{
   return &(get_private_dataspace_ptr( gas_id )->mdsp_nv_recovery_restart);
}

/*===========================================================================

FUNCTION mdsp_check_for_errors

DESCRIPTION
   This function should be called at the frame tick and performs a series
   of error checks of the mDSP.

===========================================================================*/
void mdsp_check_for_errors (gas_id_t gas_id )
{
   uint16 num_cmd, jjDebugFTick;
   GfwDspHaltError dspHalt;
   GfwHostCmdBufType      *hostCmd;


   mdsp_debug_private_data_t* private_data_ptr;
   mdsp_intf_public_data_t*   mdsp_intf_data_ptr;

   gas_id = check_gas_id(gas_id);

   private_data_ptr   = get_private_dataspace_ptr( gas_id );
   mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   /* Check for dspHalt - if set we are probably toast.  Only display when
      it changes to avoid endless stream of messages */
   dspHalt = mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.dspHalt;

   if (dspHalt != private_data_ptr->saved_dspHalt)
   {
      if (dspHalt != 0)
      {
         MSG_GERAN_ERROR_3_G("mDSP halted: 0x%04x, SW_FN=%d FW_FN=%d",dspHalt,GSTMR_GET_FN_GERAN(gas_id), mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwFTickCounter);
         if (dspHalt == GFW_ERROR_RXFE_STUCK)
         {
            mdsp_process_error(MDSP_GSM_ERROR_RECOVER, dspHalt, 0, 0 , gas_id );
         }
         else
         {
            mdsp_process_error(MDSP_GSM_ERROR_HALTED, dspHalt, 0, 0 , gas_id );
         }
      }
      private_data_ptr->saved_dspHalt = dspHalt;
   }


   /* Make sure the command buffer is empty */
   hostCmd = (GfwHostCmdBufType *) mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[mdsp_current_buffer( gas_id )];
   num_cmd = hostCmd->semaphoreAndNumCommands & ~MDSP_SEMAPHORE_BIT;
   if (num_cmd != 0)
   {
      MSG_GERAN_ERROR_1_G("mDSP commands not processed: cmd seq num %d ",
                 hostCmd->cmdSeqNum);
      MSG_GERAN_ERROR_3_G("mDSP commands not processed: %d cmds, dspHalt=0x%04x, FN=%d",
                num_cmd, dspHalt, GSTMR_GET_FN_GERAN(gas_id));

      /* Clear it out in case it was a one time error */
      hostCmd->semaphoreAndNumCommands = 0;
      mdsp_process_error(MDSP_GSM_ERROR_CMD_NOT_PROCESSED, num_cmd, dspHalt, 0 , gas_id );
   }

   /* WCDMA WL1 sends a start agc command, while in WCDMA mode, to */
   /* the mDSP which will  cause the mDSP to say it's stalled when */
   /* in fact it's not,and if NV1027 is set to 1 the UE will crash */
   /* for a benign reason.  There is a similar sleep problem which */
   /* extends the GSTMR ftick coming out of sleep,and the same WL1 */
   /* function call is used to tell mdsp drivers in this case not  */
   /* to test for a mDSP stall if it's in W sleep wakeup state.    */
#if !defined (FEATURE_WCDMA_PLT) && !defined (FEATURE_GSM_PLT)
   if ( l1_check_gsm_state( gas_id ) )
#endif /* #if !defined (FEATURE_WCDMA_PLT) && !defined (FEATURE_GSM_PLT) */
   {
     /* Check that mDSP is still ticking (assuming it has not halted) */
     if (private_data_ptr->saved_dspHalt == 0)
     {
        jjDebugFTick =  mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwFTickCounter;
        if (jjDebugFTick == private_data_ptr->last_jjdebugftick)
        {
           MSG_GERAN_HIGH_3_G("mDSP FN not incremented, SW_FN=%d FW_FN=%d count=%d", GSTMR_GET_FN_GERAN(gas_id), mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwFTickCounter, private_data_ptr->mdsp_stalled);
           private_data_ptr->mdsp_stalled += 1;
        }
        private_data_ptr->last_jjdebugftick = jjDebugFTick;
     }
   }

}
/*===========================================================================

FUNCTION mdsp_cm_check_for_error_recovery

DESCRIPTION
   This function checks whether there is an MDSP error that requires the
   CM panic reset.

===========================================================================*/
boolean mdsp_cm_check_for_error_recovery (gas_id_t gas_id )
{
   GfwDspHaltError dspHalt;
   boolean cm_recovery = FALSE;

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   /* Check for dspHalt  */
   dspHalt =  mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.dspHalt;
   if (dspHalt != 0)
   {
      MSG_GERAN_ERROR_3_G("CM GFW halted: 0x%04x, SW_FN=%d FW_FN=%d",dspHalt,GSTMR_GET_FN_GERAN(gas_id), mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwFTickCounter);
      if (dspHalt == GFW_ERROR_RXFE_STUCK)
      {
         cm_recovery = TRUE;
      }
   }
   return cm_recovery;
}

/*===========================================================================

FUNCTION mdsp_get_iq_data

DESCRIPTION
  This function.

===========================================================================*/
boolean mdsp_get_iq_data(int16 *data, uint16 length , gas_id_t gas_id )
{
  boolean success = FALSE;
  uint16  rd = 0;
  mdsp_intf_public_data_t* mdsp_intf_data_ptr;

  GfwIqSamplesBuffer* iqBufferPtr;

  gas_id = check_gas_id(gas_id);

  mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

  iqBufferPtr = (GfwIqSamplesBuffer*) &(mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwIqBuffer[mdsp_current_buffer( gas_id )]);

   /* The read pointer pointer is two frames ago */
   rd = (mdsp_current_buffer_mod3( gas_id ) + 3 - 2) % 3;

   /* Check that the sequence number matches the expected one - do not check
      the semaphore since it may have been cleared already by a subsequent
      rx command - plus this allows fetching multiple debug buffer pieces
      in a single frame */
   if (iqBufferPtr->seqNum != mdsp_intf_data_ptr->mdsp_sync_rx_burst_seq_num_fifo[rd])
   {
      MSG_GERAN_ERROR_3_G("Debug data: bad seq num: got %d exp %d, FN=%d",
                iqBufferPtr->seqNum, mdsp_intf_data_ptr->mdsp_sync_rx_burst_seq_num_fifo[rd], GSTMR_GET_FN_GERAN(gas_id));
   }
   else
   {
      if(iqBufferPtr->numSamples != 0)
     {
       /* need to ignore the seq num so using iqBufferPtr + 2 for memcpy*/
       memscpy(data, (sizeof(GfwIqSamplesBuffer)-sizeof(uint16)), &iqBufferPtr->numSamples,(length* sizeof(int16) ));
       success = TRUE;
     }
     else
     {
       MSG_GERAN_ERROR_1_G("No IQ samples to copy buf %d",mdsp_current_buffer( gas_id ));
     }
   }
  return success;
}

/*===========================================================================

FUNCTION mdsp_get_debug_data

DESCRIPTION
  This function retrieves a debug packet from the mDSP if available.

  data   - pointer to where to put decoded data results
  offset - offset (in words) into the mDSP debug buffer
  length - number of words to retrieve starting at offset

DEPENDENCIES
  The mdsp_sync_rx() function must have been issued 2 frames earlier.

RETURN VALUE
  TRUE if packet available, FALSE otherwise

SIDE EFFECTS
  data will be modified even if results are FALSE.
===========================================================================*/
boolean mdsp_get_debug_data (uint16 *data, uint16 offset, uint16 length , gas_id_t gas_id )
{
   boolean results_avail=FALSE;

   return results_avail;
}



#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
#error code not present
#endif /* FEATURE_GSM_LOG_MDSP_SCALING_FACTORS */



/*===========================================================================

FUNCTION mdsp_log_qdsp6_cmds

DESCRIPTION
  This function logs data into trace buffers along with the current Frame
  number at the time.

  data - pointer to data to log

===========================================================================*/
void mdsp_log_qdsp6_cmds (GfwCmdBufType *cmd, uint16 len, uint16 num_cmds, gas_id_t gas_id)
{
   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Mutex lock while setting msg as buffer could be updated by 2 threads */
   gstmr_log_mtx_lock(gas_id);

   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].FN = GSTMR_GET_FN_GERAN(gas_id);
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].HwFN = get_vstmr_FN_modMaxFN(geran_map_gas_id_to_nas_id(gas_id));
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].num_cmds = num_cmds;
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].seq_num = mdsp_get_current_seq_num( gas_id );
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].qsymbol_count = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));
   memscpy(&private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].cmdType, sizeof(GfwCmdBufType), (uint8 *)cmd, len);
   if (++private_data_ptr->mdsp_cmd_trace_index >= MAX_LOG_CMDS)
   {
     private_data_ptr->mdsp_cmd_trace_index = 0;
   }

   gstmr_log_mtx_unlock(gas_id);

}

void  mdsp_log_qdsp6_async_cmd( msgr_umid_type async_cmd, gas_id_t gas_id, uint16 hwfn )
{
   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Mutex lock while setting msg as buffer could be updated by 2 threads */
   gstmr_log_mtx_lock(gas_id);

   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].FN            = GSTMR_GET_FN_GERAN(gas_id);
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].HwFN          = hwfn;
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].num_cmds      = 0;
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].seq_num       = mdsp_get_current_seq_num( gas_id );
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].qsymbol_count = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));
   /* Clear the data currently at this index since it is no longer
      valid, and copying async_cmd alone won't reuse all of it */
   memset(&private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].cmdType, 0, sizeof(GfwCmdBufType));
   private_data_ptr->mdsp_cmd_trace_buf[private_data_ptr->mdsp_cmd_trace_index].cmdAsync = async_cmd;

   if (++(private_data_ptr->mdsp_cmd_trace_index) >= MAX_LOG_CMDS)
   {
     private_data_ptr->mdsp_cmd_trace_index = 0;
   }

   gstmr_log_mtx_unlock(gas_id);

}

/*===========================================================================

FUNCTION mdsp_trigger_halt

DESCRIPTION
  This function triggers the mDSP firmware to halt.  We do not make it
  static in case someone wants to call it elsewhere, but for now it is
  only used below.

===========================================================================*/
#define MDSP_NMI_INTERRUPT 0x1
void mdsp_trigger_halt (void)
{
}

/*===========================================================================

FUNCTION mdsp_memory_dump_to_file

DESCRIPTION
  This function writes a dump of mdsp memory to flash.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_memory_dump_to_file (gas_id_t gas_id )
{
}



boolean mdsp_memory_dump_requested (gas_id_t gas_id )
{
   return get_private_dataspace_ptr( gas_id )->mdsp_memory_dump.requested;
}

boolean mdsp_memory_dump_in_progress (gas_id_t gas_id )
{
   return get_private_dataspace_ptr( gas_id )->mdsp_memory_dump.in_progress;
}

/*===========================================================================

FUNCTION mdsp_force_memory_dump

DESCRIPTION
  This function forces an mdsp memory dump regardless of whether the
  NV item is set or a previous dump has occured.

===========================================================================*/
void mdsp_force_memory_dump (gas_id_t gas_id )
{
#ifdef FEATURE_MDSP_ERR_LOG_ENABLED
   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Force the memory dump feature to be enabled and reset the flag */
   private_data_ptr->mdsp_memory_dump.enabled = TRUE;

   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].cause = MDSP_GSM_ERROR_FORCED;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].FN = GSTMR_GET_FN_GERAN(gas_id);
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].params[0] = 0;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].params[1] = 0;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].params[2] = 0;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].compressed_mode = mdsp_cm_active(gas_id);

   if (++(private_data_ptr->mdsp_error_log_index) >= MAX_LOG_ERRORS)
   {
     private_data_ptr->mdsp_error_log_index = 0;
   }
#endif
   /* Dump memory */
   //deprecated mdsp_memory_dump_to_file(gas_id );
}





/*===========================================================================

FUNCTION mdsp_process_error

DESCRIPTION
  This function processes errors by issuing various debug information.

===========================================================================*/
void mdsp_process_error
(
   mdsp_gsm_error_type error,
   uint32 param1,
   uint32 param2,
   uint32 param3
   , gas_id_t gas_id
)
{
   mdsp_debug_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id );
   mdsp_intf_public_data_t*   mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
   uint32 this_fn = GSTMR_GET_FN_GERAN(gas_id);

#ifdef FEATURE_MDSP_ERR_LOG_ENABLED
   /* Save the error details */
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].cause = error;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].FN = this_fn;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].params[0] = param1;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].params[1] = param2;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].params[2] = param3;
   private_data_ptr->mdsp_error_log[private_data_ptr->mdsp_error_log_index].compressed_mode = mdsp_cm_active(gas_id);

   if (++(private_data_ptr->mdsp_error_log_index) >= MAX_LOG_ERRORS) private_data_ptr->mdsp_error_log_index = 0;
#endif
   /* Only log once per frame */
   if (this_fn == private_data_ptr->last_fn) return;
   private_data_ptr->last_fn = this_fn;

   switch(error)
   {
     case MDSP_GSM_ERROR_HALTED:
     case MDSP_GSM_ERROR_CMD_NOT_PROCESSED:
    {
       if(private_data_ptr->mdsp_nv_recovery_restart.valid == TRUE)
       {
         if(private_data_ptr->mdsp_nv_recovery_restart.recovery_restart_enabled == FALSE)
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           /* crash for these errors in PRE TA phase*/
           mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwHaltFirmware = TRUE;
           ERR_GERAN_FATAL_1_G("Halting mDSP, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
         }
         else
         {
           /* Recover from these errors for post TA releases*/
           mdsp_handle_panic( gas_id );
         }
       }
       else if(private_data_ptr->mdsp_nv_recovery_restart.valid == FALSE)
       {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
         /* crash for these errors in PRE TA phase*/
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwHaltFirmware = TRUE;
         ERR_GERAN_FATAL_1_G("Halting mDSP, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
       }
       break;
     }


     case MDSP_GSM_ERROR_RECOVER:
     {
       mdsp_handle_panic( gas_id );
       break;
     }
     case MDSP_GSM_ERROR_NONSBI_BUF_OVERFLOW:
     case MDSP_GSM_ERROR_SBI_BUFFER_OVERFLOW:
          {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwHaltFirmware = TRUE;
       ERR_GERAN_FATAL_1_G("Halting mDSP, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
       break;
          }

     case MDSP_GSM_ERROR_FORCED:

     case MDSP_GSM_ERROR_INVALID_POWER:
     case MDSP_GSM_ERROR_INVALID_METRICS:
     case MDSP_GSM_ERROR_INVALID_DATA:
     case MDSP_GSM_ERROR_INVALID_SCH:
     case MDSP_GSM_ERROR_INVALID_FCCH:
     case MDSP_GSM_ERROR_SLEEP:
     case MDSP_GSM_ERROR_WARNING:
     {
       /* ignore these errors for 9k*/
       break;
     }
     default:
     {
       break;
     }
   }
}


/*===========================================================================

FUNCTION mdsp_validate_power_measure

DESCRIPTION
  This function validates the rssi results from a power measure.

DEPENDENCIES
  None

RETURN VALUE
  Whether or not the data is valid.

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_validate_power_measure (uint32 num_rssi, uint32 *rssi , gas_id_t gas_id )
{
   boolean valid = TRUE;

   uint8 i;

   /* Validate the rssi */
   for (i = 0; i < num_rssi; i++)
   {
      if (rssi[i] == 0)
      {
         valid = FALSE;
      }
   }

   if (!valid) mdsp_process_error(MDSP_GSM_ERROR_INVALID_POWER, 0, 0, 0 , gas_id );

   return valid;
}


/*===========================================================================

FUNCTION mdsp_validate_rx_metrics

DESCRIPTION
  This function validates the metrics from a receive.

DEPENDENCIES
 None

RETURN VALUE
  Whether or not the data is valid.

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_validate_rx_metrics (GfwBurstMetricsStruct *metrics , gas_id_t gas_id )
{
   boolean valid = TRUE;

   /* Validate the SNR */
   /* It appears that in low signal areas the dsp actually returns extremely
      low SNR values sometimes, so for now we skip this check
   if (metrics->snr < 8)
   {
      MSG_GERAN_ERROR_2_G("Invalid metrics SNR: %d, FN=%d", metrics->snr, GSTMR_GET_FN_GERAN(gas_id));
      valid = FALSE;
   }
   */

   /* Validate the rssi */
   if (metrics->rssiMsw == 0 && metrics->rssiLsw == 0)
   {
      /* No need for this error as the RF driver will report RSSI=0 */
      /*MSG_ERROR("Invalid metrics rssi: %d, FN=%d",
                  metrics->rssi_l, GSTMR_GET_FN_GERAN(gas_id),0);*/
      valid = FALSE;
   }

   if (!valid) mdsp_process_error(MDSP_GSM_ERROR_INVALID_METRICS, metrics->snr, 0, 0 , gas_id );

   return valid;
}


/*===========================================================================

FUNCTION mdsp_validate_decoded_data

DESCRIPTION
  This function validates the decoded data from a receive.

DEPENDENCIES
 None

RETURN VALUE
  Whether or not the data is valid.

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_validate_decoded_data (GfwDecodedDataStruct *data, boolean validate_rx_qual , gas_id_t gas_id )
{
   boolean valid = TRUE;

   if (validate_rx_qual && data->hardRxQual == 0)
   {
      MSG_GERAN_ERROR_2_G("Invalid rxqual: %d, FN=%d", data->hardRxQual, GSTMR_GET_FN_GERAN(gas_id));
      valid = FALSE;
   }

   if (!valid) mdsp_process_error(MDSP_GSM_ERROR_INVALID_DATA, data->hardRxQual, 0, 0 , gas_id );

   return valid;
}


/*===========================================================================

FUNCTION mdsp_validate_sch_results

DESCRIPTION
  This function validates the results of an SCH receive.

DEPENDENCIES
 None

RETURN VALUE
  Whether or not the data is valid.

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_validate_sch_results (mdsp_sch_results_struct *data , gas_id_t gas_id )
{
   boolean valid = TRUE;

   /* Validate the CRC field */
   if (data->y_crc & ~MDSP_SCH_RESULTS_MASK_CRC)
   {
      MSG_GERAN_ERROR_2_G("Invalid SCH CRC: %d, FN=%d", data->y_crc, GSTMR_GET_FN_GERAN(gas_id));
      valid = FALSE;
   }

   if (!valid) mdsp_process_error(MDSP_GSM_ERROR_INVALID_SCH, data->y_crc, 0, 0 , gas_id );

   return valid;
}


/*===========================================================================

FUNCTION mdsp_validate_fcch_results

DESCRIPTION
  This function validates the results of a FCCH receive.

DEPENDENCIES
 None

RETURN VALUE
  Whether or not the data is valid.

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_validate_fcch_results (GfwFcchResultStruct* data , gas_id_t gas_id )
{
   boolean valid = TRUE;

   /* Validate the SNR */
   if (data->snrEst< 8)
   {
      MSG_GERAN_ERROR_2_G("Invalid FCCH SNR: %d, FN=%d", data->snrEst, GSTMR_GET_FN_GERAN(gas_id));
      valid = FALSE;
   }

   if (!valid) mdsp_process_error(MDSP_GSM_ERROR_INVALID_FCCH, data->snrEst, 0, 0 , gas_id );

   return valid;
}





/*===========================================================================

FUNCTION mdsp_set_dsp_error_mask

DESCRIPTION
  This function allows the application to mask out errors to report as part
  of the mdsp interface detecting mdsp errors.

  A typical use for this might be in the PLT interface where it does not
  care about FCCH overruns.

  mask0 - set any bit to zero to mask out the corresponding bit in MDSP_dspError.
  mask1 - set any bit to zero to mask out the corresponding bit in MDSP_dspError +1.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_set_dsp_error_mask( uint32 mask0, uint32 mask1 , gas_id_t gas_id  )
{
   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   private_data_ptr->mdsp_error_mask[0] = mask0;
   private_data_ptr->mdsp_error_mask[1] = mask1;
}

/*===========================================================================

FUNCTION mdsp_set_dsp_warning_mask

DESCRIPTION
  This function allows the application to mask out warnings to report as part
  of the mdsp interface detecting mdsp errors.

  mask0 - set any bit to zero to mask out the corresponding bit in MDSP_dspError.
  mask1 - set any bit to zero to mask out the corresponding bit in MDSP_dspError +1.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_set_dsp_warning_mask( uint32 mask0, uint32 mask1 , gas_id_t gas_id  )
{
   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   private_data_ptr->mdsp_warning_mask[0] = mask0;
   private_data_ptr->mdsp_warning_mask[1] = mask1;
}

/*===========================================================================

FUNCTION mdsp_log_gprs_tx_trace_buf

DESCRIPTION
  This function logs data into trace buffers.
  It logs the channel type, the puncturing scheme (if EGPRS), the message type
  and the pdch_id

  chan      - channel type
  msg       - ptr to message where we extract the puncturing scheme
  msg_type  - message type: usf or pdch
  pdch_id   - timeslot

===========================================================================*/
void mdsp_log_dtm_tx_trace_buf
(
   mdsp_channel_type         chan,
   mdsp_dtm_tx_msg_type      *msg,
   mdsp_specific_msg_type    msg_type,
   mdsp_timeslot_type        pdch_id
   , gas_id_t gas_id
)
{
   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   #ifdef DEBUG_MDSP_LOG_GPRS_TX_CMDS
   /* Get the frame number and channel*/
   private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].FN = GSTMR_GET_FN_GERAN(gas_id);
   private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].chan = (uint32)chan;

   /* Log the puncturing schemes */
   #ifdef FEATURE_GSM_MDSP_EGPRS
   if (IS_MDSP_EGPRS_CHANNEL(chan))
   {
      private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].puncturing[0] =
        (uint16)msg->egprs.puncturing[0];
      if (chan >= MDSP_PDTCHU_MCS7)
      {
         private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].puncturing[1] =
           (uint16)msg->egprs.puncturing[1];
      }
      /* default */
      else
      {
         private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].puncturing[1] = 0;
      }
   }
   else
   #endif
   {
      /* default values */
      private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].puncturing[0] = 0;
      private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].puncturing[1] = 0;
   }
   private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].msg_type = (uint16) msg_type;
   private_data_ptr->mdsp_cmd_gprs_tx_trace_buf[private_data_ptr->mdsp_cmd_gprs_tx_trace_index].pdch_id = pdch_id;

   /* Circular buffer. Set the index to beginning after reaching end of array */
   if (++(private_data_ptr->mdsp_cmd_gprs_tx_trace_index) >= MAX_LOG_GPRS_TX_CMDS)
   {
       private_data_ptr->mdsp_cmd_gprs_tx_trace_index = 0;
   }
   #else
   /*lint -esym(715,chan,msg,msg_type,pdch_id)*/
   #endif
}



/*===========================================================================

FUNCTION mdsp_log_grfc_write

DESCRIPTION
   Logs the GRFC write information to a trace buffer.

===========================================================================*/
void mdsp_log_grfc_write (uint16 reg, uint32 data , gas_id_t gas_id )
{
   #ifdef DEBUG_MDSP_LOG_GRFC_WRITES
   mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Log the information */
   private_data_ptr->mdsp_grfc_write_trace_buf[private_data_ptr->mdsp_cmd_trace_index].FN = GSTMR_GET_FN_GERAN(gas_id);
   private_data_ptr->mdsp_grfc_write_trace_buf[private_data_ptr->mdsp_cmd_trace_index].reg = reg;
   private_data_ptr->mdsp_grfc_write_trace_buf[private_data_ptr->mdsp_cmd_trace_index].data = data;

   /* Increment the circular buffer */
   if (++(private_data_ptr->mdsp_grfc_write_trace_index) >= MDSP_LOG_GRFC_WRITES_LEN)
   {
      private_data_ptr->mdsp_cmd_trace_index = 0;
   }
   #else
   /*lint -esym(715,reg,data)*/
   #endif
}



#ifdef FEATURE_GSM_MDSP_IR
#ifdef DEBUG_MDSP_IR_DATA
#error code not present
#endif


/*===========================================================================

FUNCTION mdsp_check_IR_data

DESCRIPTION
   This function checks to see if a message CRC failed and if so validates
   that the resulting IR data is good.
   BSN calculations are from 44.060, 10.3a.4 (noting that the EGPRS header
   from the mDSP does not include the USF)

===========================================================================*/
void mdsp_check_IR_data
(
   GfwDecodedDataStruct *hdr
)
{
   #ifdef DEBUG_MDSP_IR_DATA
   #error code not present
#else
   /*lint -esym(715,hdr)*/
   #endif
}
#endif /* FEATURE_GSM_MDSP_IR */

/*===========================================================================

FUNCTION mdsp_get_log_cmd_entry

DESCRIPTION
   This function gets the mdsp command log entry at the specified offset.
   An offset of zero gives the most recent entry written to the buffer.

===========================================================================*/
void mdsp_get_log_cmd_entry( gl1_dump_tag_data_s *pBuf, uint8 len, gas_id_t gas_id)
{
  uint16 mdsp_cmd_trace_index = mdsp_debug_private_data[gas_id].mdsp_cmd_trace_index;
  uint8 i;

  mdsp_log_cmd_struct* entry_p ;

  if (mdsp_cmd_trace_index == 0)
  {
     mdsp_cmd_trace_index =  MAX_LOG_CMDS -1;
  }
  else
  {
    mdsp_cmd_trace_index--;
  }
  
  for (i = 0; i < len; i++)
  {
    entry_p = &mdsp_debug_private_data[gas_id].mdsp_cmd_trace_buf[mdsp_cmd_trace_index];

    pBuf->mdsp_cmd_buf[i].SwFN           = entry_p->FN;
    pBuf->mdsp_cmd_buf[i].num_cmds       = entry_p->num_cmds;
    pBuf->mdsp_cmd_buf[i].seq_num        = entry_p->seq_num;
    pBuf->mdsp_cmd_buf[i].Qs             = entry_p->qsymbol_count;
    pBuf->mdsp_cmd_buf[i].HwFN           = entry_p->HwFN;
    pBuf->mdsp_cmd_buf[i].is_async       = entry_p->cmdAsync;
    pBuf->mdsp_cmd_buf[i].cmdId          = (uint8)entry_p->cmdType.cmdId;

    if (mdsp_cmd_trace_index == 0)
        mdsp_cmd_trace_index =  MAX_LOG_CMDS;
   
    mdsp_cmd_trace_index--;
  }
}

/*===========================================================================

FUNCTION mdsp_set_last_debugftick

DESCRIPTION
   This function sets the last debug ftick to one before it is now, to avoid
   the check for the mDSP stall from always failing as during an asynch int
   the mDSP frame number will be the same for both calls.

===========================================================================*/
void mdsp_set_last_debugftick( gas_id_t gas_id  )
{
  mdsp_debug_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  uint32 local_last_debugftick = private_data_ptr->last_jjdebugftick;

  private_data_ptr->last_jjdebugftick = SUB_FN( local_last_debugftick, 1 );
}

