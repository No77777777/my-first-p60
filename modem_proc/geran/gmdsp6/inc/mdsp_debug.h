#ifndef MDSP_DEBUG_H
#define MDSP_DEBUG_H
/*===========================================================================
                          M D S P _ D E B U G . H


GENERAL DESCRIPTION
  This header file contains debug definitions.  It should only be included
  by internal clients.


Copyright (c) 2012-2015 Qualcomm Technologies, Inc.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/geran/dev/dime/dsda/gmdsp6/inc/mdsp_dtm.h#4

when         who     what, where, why
----------  -------- ------------------------------------------------------
27/01/20     rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
16/03/15     pg      CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
11/04/14     cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF (and others already mainlined)
11/04/14     pg      CR645875 Fix Bolt multisim compialtion errors
27/01/14     pa      CR607716:GL1 changes related to legacy GFW interface changes.
22/11/13     sk      CR576246 Ensure Clenup scripts status not cleared while populating startup scripts inside the QTA gap
22/03/13     cja     CR462223 Clear monitor CCS buffers if gap aborted, reduce F3
12-02-13     pjr     CR451837 modified mdsp_log_qdsp6_async_cmd to log correct
                     async command
12-11-12     tjw     Initial version
===========================================================================*/

#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "sys_type.h" /* dBx16_T */

#include "mdsp_intf.h"
#include "mdsp_cmd.h"

/* Structure used to hold the results of loading gsm recovery restart NV item
   - this nv item is read during mdsp_init*/
typedef struct
{
  boolean  valid;
  boolean  recovery_restart_enabled;
} mdsp_nv_recovery_restart_type;


/* Logging of gprs tx commands to a trace buffer */
#define DEBUG_MDSP_LOG_GPRS_TX_CMDS


#ifdef  DEBUG_MDSP_LOG_GPRS_TX_CMDS
#define MAX_LOG_GPRS_TX_CMDS  25
#define MDSP_LOG_PUNC_SCH_LEN 2
typedef  struct
{
  uint32              FN;
  uint32              chan;
  /* This is ONLY used if FEATURE_GSM_MDSP_EGPRS is defined. However,
   * we define it even though the feature is not defined because during
   * mdsp_mem_dump, we always decode these values. Therefore, we always
   * allocate these variables */
  uint16              puncturing[MDSP_LOG_PUNC_SCH_LEN];
  uint16              msg_type;
  mdsp_timeslot_type  pdch_id;
} mdsp_log_cmd_gprs_tx_struct;

#endif

/* Structure to define the cmd trace buffer */
typedef struct
{
  uint32                    FN;
  uint16                    num_cmds;
  uint16                    seq_num;
  uint16                    qsymbol_count;
  uint16                    HwFN;
  union
  {
     GfwCmdBufType             cmdType;
     msgr_umid_type            cmdAsync;
  };
} mdsp_log_cmd_struct;

#define MAX_LOG_CMDS   50

#define MDSP_CMD_LOG_COUNT  16
#define MDSP_CMD_LOG_PARAM_COUNT   5

/* Header structure stored in dump file */
typedef PACKED struct PACKED_POST
{
   uint32    magic;           /* Magic number for sanity */
   uint16    version;         /* Dump version */
   uint16    fw_version;      /* MDSP FW version */
   uint16    gsm_version[2];  /* MDSP GSM version */
   uint32    FN;              /* Frame number of the error */
   uint16    cause;           /* Error cause */
   uint32    params[3];       /* Error parameters */
   time_type timestamp;       /* Timestamp */
} mdsp_mem_dump_header_type;


/* Memory dumping variables */
typedef struct
{
   /* Set if memory dumping is enabled */
   boolean enabled;

   /* Bitmask indicates which of the possible errors to suppress dumping for */
   uint16 suppressed;

   /* Used to trigger a dump manually by poking in QXDM */
   boolean requested;

   /* Indicates if a dump is in progress to avoid issuing more commands */
   boolean in_progress;

   /* Flag if we need to let mdsp services reset us after the dump */
   boolean reset_pending;

   /* Header of memory dump */
   mdsp_mem_dump_header_type header;
} mdsp_memory_dump_type;


typedef struct
{
  /* Set new Error Masks as agreed with FW team. */
  uint32                      mdsp_error_mask[2];
  uint32                      mdsp_warning_mask[2];

#ifdef FEATURE_MDSP_ERR_LOG_ENABLED 
  uint16                      mdsp_error_log_index;
  mdsp_error_log_type         mdsp_error_log[MAX_LOG_ERRORS];
#endif
  uint16                      mdsp_cmd_gprs_tx_trace_index;
  mdsp_log_cmd_gprs_tx_struct mdsp_cmd_gprs_tx_trace_buf[MAX_LOG_GPRS_TX_CMDS];


  uint16              mdsp_cmd_trace_index;
  mdsp_log_cmd_struct mdsp_cmd_trace_buf[MAX_LOG_CMDS];

  mdsp_memory_dump_type mdsp_memory_dump;

  /* Storage for the NV item loaded in mdsp_load_nv */
  mdsp_nv_item_type mdsp_nv_mem_dump_enabled;

  /* stores the information regarding recovery restart is enabled or not */
  mdsp_nv_recovery_restart_type mdsp_nv_recovery_restart;

#ifdef DEBUG_MDSP_LOG_GRFC_WRITES
  uint16 mdsp_grfc_write_trace_index;
  mdsp_grfc_write_trace_buf_type mdsp_grfc_write_trace_buf[MDSP_LOG_GRFC_WRITES_LEN];
#endif

/* Keep track of last jjDebugFTick value to catch mDSP stalls */
  uint16 last_jjdebugftick;
  uint16 mdsp_stalled;

  GfwDspHaltError saved_dspHalt;

  uint32 last_fn;

} mdsp_debug_private_data_t;





/*===========================================================================

FUNCTION get_mdsp_nv_mem_dump_enabled

DESCRIPTION
   This function provides a public interface to mdsp_nv_mem_dump_enabled

===========================================================================*/
mdsp_nv_item_type* mdsp_debug_nv_mem_dump_enabled(gas_id_t gas_id );

/*===========================================================================

FUNCTION get_mdsp_nv_mem_dump_enabled

DESCRIPTION
   This function provides a public interface to mdsp_nv_recovery_restart

===========================================================================*/
mdsp_nv_recovery_restart_type* mdsp_debug_nv_recovery_restart(gas_id_t gas_id );

/*===========================================================================

FUNCTION mdsp_check_for_errors

DESCRIPTION
   This function should be called at the frame tick and performs a series
   of error checks of the mDSP.

===========================================================================*/
void mdsp_check_for_errors (gas_id_t gas_id );

/*===========================================================================

FUNCTION mdsp_cm_check_for_error_recovery

DESCRIPTION
   This function checks whether there is an MDSP error that requires the
   CM panic reset.

===========================================================================*/
boolean mdsp_cm_check_for_error_recovery (gas_id_t gas_id );

/*===========================================================================

FUNCTION mdsp_get_iq_data

DESCRIPTION
  This function.

===========================================================================*/
boolean mdsp_get_iq_data(int16 *data, uint16 length , gas_id_t gas_id );

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
boolean mdsp_get_debug_data (uint16 *data, uint16 offset, uint16 length , gas_id_t gas_id );

#ifdef FEATURE_GSM_LOG_MDSP_SCALING_FACTORS
#error code not present
#endif



/*===========================================================================

FUNCTION mdsp_trigger_halt

DESCRIPTION
  This function triggers the mDSP firmware to halt.  We do not make it
  static in case someone wants to call it elsewhere, but for now it is
  only used below.

===========================================================================*/
void mdsp_trigger_halt (void) GMDSP6_DEPRECATED_ATTRIBUTE;

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
void mdsp_memory_dump_to_file (gas_id_t gas_id ) GMDSP6_DEPRECATED_ATTRIBUTE;

boolean mdsp_memory_dump_requested (gas_id_t gas_id );
boolean mdsp_memory_dump_in_progress (gas_id_t gas_id );

/*===========================================================================

FUNCTION mdsp_force_memory_dump

DESCRIPTION
  This function forces an mdsp memory dump regardless of whether the
  NV item is set or a previous dump has occured.

===========================================================================*/
void mdsp_force_memory_dump (gas_id_t gas_id );

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
);




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
void mdsp_set_dsp_error_mask( uint32 mask0, uint32 mask1 , gas_id_t gas_id  );

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
void mdsp_set_dsp_warning_mask( uint32 mask0, uint32 mask1 , gas_id_t gas_id  );

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
);


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
);

/*===========================================================================

FUNCTION mdsp_set_last_debugftick

DESCRIPTION
   This function sets the last debug ftick to one before it is now, to avoid
   the check for the mDSP stall from always failing as during an asynch int
   the mDSP frame number will be the same for both calls.

===========================================================================*/
void mdsp_set_last_debugftick( gas_id_t gas_id  );

/*===========================================================================

FUNCTION mdsp_log_grfc_write

DESCRIPTION
   ogs the GRFC write information to a trace buffer.
===========================================================================*/
void mdsp_log_grfc_write (uint16 reg, uint32 data , gas_id_t gas_id );

/*===========================================================================

FUNCTION mdsp_log_qdsp6_cmds

DESCRIPTION
   This function logs data into trace buffers along with the current Frame
   number at the time.
===========================================================================*/
void mdsp_log_qdsp6_cmds (GfwCmdBufType *cmd, uint16 len, uint16 num_cmds, gas_id_t gas_id);

/*===========================================================================

FUNCTION mdsp_debug_init

DESCRIPTION
   This function initializes debug variables.
===========================================================================*/
void mdsp_debug_init (gas_id_t gas_id );
#endif /* MDSP_DEBUG_H */
