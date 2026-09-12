#ifndef RFGSM_CORE_LOG_H
#define RFGSM_CORE_LOG_H
/*===========================================================================

           GSM  FTM   LOGGING    H E A D E R    F I L E

DESCRIPTION
  This file contains declarations associated with the GSM FTM logging.

REFERENCES
  None

===========================================================================*/

/*===========================================================================
Copyright (c) 2003-2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_core_log.h#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------   
02/02/20   mpa     To support MiniDump Feature
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
08/12/15   sc      Add split Rx/Tx state machines - part 1 (deprecation)
07/16/15   avi     Off-Target Compilation error fix
07/15/15   sw      WTR power saving optimisations (under feature flag)
05/12/15   zgl     Changes for FR 24616: Algorithm to mitigate VBatt droop (down to 2.5V) via TX backoff
05/07/15   sc      Implement command processing log
02/27/15   sc      Ensure NULL NV error checking when processing Rx/Tx band
11/18/14   sw/sc   Deprecate legacy state machines and move cmd buff out of rfa    
02/10/12   sb      Changes to support FTM logging for GSM
06/03/12   sb      Included correct header file to enableGSM FTM logging
============================================================================*/
#include "rfa_variation.h"
#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "queue.h"
#include "msg.h"
#include "rfgsm_core_types.h"
#include "rfcom.h"

#ifdef FTM_HAS_LOGGING
#include "ftm_log.h"
#endif

#ifdef FEATURE_QSH_MDUMP 
#include "qsh.h"
#endif


/*===========================================================================*/
/*                                                                           */
/*                              DEFINTIONS                                   */
/*                                                                           */
/*===========================================================================*/
#define RFGSM_LOG_BUFFERS   10



/* Define levels included in logging */
#define RFGSM_LOG_LEVEL_CMD
#define RFGSM_LOG_LEVEL_MC
//#define RFGSM_LOG_LEVEL_CORE
#define RFGSM_LOG_LEVEL_DEVICE


/*! @enum rfgsm_cmd_proc_input_type                                          */
/*! @brief enum to define the log entry commands                             */
typedef enum
{
  RFGSM_CMD_DEFAULT,

  /* Command Layer */
  RFGSM_CMD_START_GSM_BEGIN,
  RFGSM_CMD_START_GSM_END,
  RFGSM_CMD_STOP_GSM_BEGIN,
  RFGSM_CMD_STOP_GSM_END,
  RFGSM_CMD_RX_WAKEUP_BEGIN,
  RFGSM_CMD_RX_WAKEUP_END,
  RFGSM_CMD_RX_SLEEP_BEGIN,
  RFGSM_CMD_RX_SLEEP_END,
  RFGSM_CMD_TX_WAKEUP_BEGIN,
  RFGSM_CMD_TX_WAKEUP_END,
  RFGSM_CMD_TX_SLEEP_BEGIN,
  RFGSM_CMD_TX_SLEEP_END,
  RFGSM_CMD_RX_BEGIN,
  RFGSM_CMD_RX_END,
  RFGSM_CMD_TX_BAND_BEGIN,
  RFGSM_CMD_TX_BAND_END,
  RFGSM_CMD_TX_BEGIN,
  RFGSM_CMD_TX_END,
  RFGSM_CMD_IDLE_FRAME_PROC_BEGIN,
  RFGSM_CMD_IDLE_FRAME_PROC_END,
  RFGSM_CMD_SET_ANTENNA_BEGIN,
  RFGSM_CMD_SET_ANTENNA_END,
  RFGSM_CMD_CM_ENTER_BEGIN,
  RFGSM_CMD_CM_ENTER_END,
  RFGSM_CMD_CM_EXIT_BEGIN,
  RFGSM_CMD_CM_EXIT_END,
  RFGSM_CMD_BURST_METRICS_BEGIN,
  RFGSM_CMD_BURST_METRICS_END,
  RFGSM_CMD_THERM_READ_RESPONSE_BEGIN,
  RFGSM_CMD_THERM_READ_RESPONSE_END,
  RFGSM_CMD_ASD_READ_RSP_BEGIN,
  RFGSM_CMD_ASD_READ_RSP_END,
  RFGSM_CMD_UPDATE_TEMP_COMP_REQ_BEGIN,
  RFGSM_CMD_UPDATE_TEMP_COMP_REQ_END,
  RFGSM_CMD_BUILD_IP2_CAL_SCRIPT_BEGIN,
  RFGSM_CMD_BUILD_IP2_CAL_SCRIPT_END,
  RFGSM_CMD_INIT_MSM_IP2_CAL_BEGIN,
  RFGSM_CMD_INIT_MSM_IP2_CAL_END,
  RFGSM_CMD_SAR_LIMIT_IND_BEGIN,
  RFGSM_CMD_SAR_LIMIT_IND_END,
  RFGSM_CMD_VBATT_LIMIT_IND_BEGIN,
  RFGSM_CMD_VBATT_LIMIT_IND_END,

  /* Main Control Layer */
  RFGSM_MC_MEAS_IRAT_INFO_GET_BEGIN,
  RFGSM_MC_MEAS_IRAT_INFO_GET_END,
  RFGSM_MC_MEAS_ENTER_BEGIN,
  RFGSM_MC_MEAS_ENTER_END,
  RFGSM_MC_MEAS_EXIT_BEGIN,
  RFGSM_MC_MEAS_EXIT_END,
  RFGSM_MC_MEAS_BUILD_SCRIPTS_BEGIN,
  RFGSM_MC_MEAS_BUILD_SCRIPTS_END,
  RFGSM_MC_MEAS_PRE_CONFIG_BEGIN,
  RFGSM_MC_MEAS_PRE_CONFIG_END,
  RFGSM_MC_MEAS_BUILD_RESET_SCRIPTS_BEGIN,
  RFGSM_MC_MEAS_BUILD_RESET_SCRIPTS_END,

  RFGSM_MC_RX_PREPARE_START,
  RFGSM_MC_RX_PRX_COMPLETE,
  RFGSM_MC_RX_DRX_COMPLETE,
  RFGSM_MC_RX_BUILDING_COMPLETE,
  RFGSM_MC_RX_PREPARE_STOP,

  RFGSM_MC_TX_UPDATE_FREQ_ERROR,
  RFGSM_MC_TX_TUNE,
  RFGSM_MC_TX_SET_POWER,
  RFGSM_MC_TX_SETUP_BURST,

  /* Core Layer */
  RFGSM_CORE_RX_PWR_WAKEUP,
  RFGSM_CORE_RX_PWR_SLEEP,
  RFGSM_CORE_TX_PWR_WAKEUP,
  RFGSM_CORE_TX_PWR_SLEEP,

  RFGSM_CORE_RX_CFG_LIF,
  RFGSM_CORE_RX_SAWLESS,
  RFGSM_CORE_RX_CALC_GAIN,
  RFGSM_CORE_RX_RFC,
  RFGSM_CORE_RX_DEVICE,
  RFGSM_CORE_RX_MSM,
  RFGSM_CORE_RX_DM,
  RFGSM_CORE_RX_CLEAR_BUFF,
  RFGSM_CORE_RX_BUILD_START_SCRIPT,
  RFGSM_CORE_RX_JDET_SCRIPTS,
  RFGSM_CORE_RX_BUILD_STOP_SCRIPT,
  RFGSM_CORE_RX_EVENT_BUILD,

  RFGSM_CMD_MAX
}rfgsm_cmd_proc_input_type;



/*! @struct rfgsm_cmd_proc_log_entry_type                                    */
/*! @brief structure to define the log entry data                            */
/*! @details each call to update an entry in the command processing log will */
/*!          update the data defined by this structure                       */
typedef PACK(struct)
{
  rfgsm_cmd_proc_input_type cmd;
  uint8 sub_id;
  uint32 cmn_arg;
  uint32 rf_sequence_number;
  uint32 current_frame_number;
  uint32 current_ustmr;
  uint32 current_pcycle;
  uint16 start_qs;
  int16 exec_time_us;
}rfgsm_cmd_proc_log_entry_type;



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_CMD_PROC_LOG_NUM_ENTRIES                                   */
/*! @brief definition for max entries in the command processing log          */
#define RFGSM_CMD_PROC_LOG_NUM_ENTRIES 512 //2^9



/*---------------------------------------------------------------------------*/
/*! @var rfgsm_cmd_proc_log                                                  */
/*! @brief variable declaration for command processing log                   */
/*! @details defined in rfgsm_cmd_processing.c                               */
extern rfgsm_cmd_proc_log_entry_type rfgsm_cmd_proc_log[RFGSM_CMD_PROC_LOG_NUM_ENTRIES];



/*---------------------------------------------------------------------------*/
/*! @var rfgsm_cmd_proc_log_entry                                            */
/*! @brief variable used for indexing into the command processing log        */
/*! @details defined in rfgsm_cmd_processing.c                               */
extern uint32 rfgsm_cmd_proc_log_entry;



/*---------------------------------------------------------------------------*/
/*! @fn rfgsm_atomic_inc_mask                                                */
/*! @brief thread-safe increment and mask operation for ensuring unique index*/
/*!        to log                                                            */
static inline uint32
rfgsm_atomic_inc_mask_ret( atomic_word_t *target, unsigned int mask )
{
#ifndef TEST_FRAMEWORK
  uint32 result;
  __asm__ __volatile__(
        "1:     %0 = memw_locked(%2)\n"
        "       %0 = add(%0, #1)\n"
        "       %0 = and(%0, %3)\n"
        "       memw_locked(%2, p0) = %0\n"
        "       if !p0 jump 1b\n"
        : "=&r" (result),"+m" (*target)
        : "r" (target), "r" (mask)
        : "p0");
  return result;
#else
  #error code not present
#endif
}



/*---------------------------------------------------------------------------*/
/*! @define rfgsm_cmd_log_trace                                              */
/*! @brief a function to log command proc calls from GL1 and MCS             */
/*! @details a function is used to enable multiple threads to use the same   */
/*!          log buffer index and to make the log thread safe without use of */
/*!          mutexes. Only other alternative is to use assembly for all log  */
/*!          buffer code which is not preferred.                             */
/*! @param input_cmd - command executed in RF GSM                            */
/*! @param subscription - subscription the command is executed for           */
/*! @param seq_num - GL1 generated sequence number                           */
/*! @param arg - common argument to be used for any variable depending on cmd*/
/*! @param fn - geran software frame number                                  */
/*! @param qs - where in the frame the command was executed                  */
/*! @param ustmr - current ustmr timestamp                                   */
/*! @param pcycle - current pcycle timestamp                                 */
/*! @param exec_time - calculated execution time in microseconds             */
/*! NOTE: cannot pass in structure pointer here because would have to create */
/*!       variable after statements which is not allowed in C99              */
static inline void rfgsm_cmd_log_trace( rfgsm_cmd_proc_input_type input_cmd,
                                        uint8 subscription,
                                        uint32 arg,
                                        uint32 seq_num,
                                        uint32 fn,
                                        uint32 qs,
                                        uint32 usmtr,
                                        uint32 pcycle,
                                        int16 exec_time )
{
  uint32 unique_log_entry = 0;
  unique_log_entry = rfgsm_atomic_inc_mask_ret((atomic_word_t*)&rfgsm_cmd_proc_log_entry, (RFGSM_CMD_PROC_LOG_NUM_ENTRIES-1));

  rfgsm_cmd_proc_log[unique_log_entry].cmd                  = input_cmd;
  rfgsm_cmd_proc_log[unique_log_entry].sub_id               = subscription;
  rfgsm_cmd_proc_log[unique_log_entry].cmn_arg              = arg;
  rfgsm_cmd_proc_log[unique_log_entry].rf_sequence_number   = seq_num;
  rfgsm_cmd_proc_log[unique_log_entry].current_frame_number = fn;
  rfgsm_cmd_proc_log[unique_log_entry].start_qs             = qs;
  rfgsm_cmd_proc_log[unique_log_entry].current_ustmr        = usmtr;
  rfgsm_cmd_proc_log[unique_log_entry].current_pcycle       = pcycle;
  rfgsm_cmd_proc_log[unique_log_entry].exec_time_us         = exec_time;
}



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_PROC_LOG                                                   */
/*! @brief a macro to log processing calls in the rfgsm_cmd_proc_log         */
#define RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) \
          rfgsm_cmd_log_trace( input_cmd,       \
                               subscription,    \
                               arg,             \
                               seq_num,         \
                               fn,              \
                               qs,              \
                               usmtr,           \
                               (uint32)pcycle,  \
                               (int16)exec_time)



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_CMD_PROC_LOG_VARS                                          */
/*! @brief a macro to define the variables needed for logging                */
/*! @details this macro should be called before any statements in the code   */
/*!          and is a dependency if using RFGSM_CMD_PROC_LOG_CMN_START       */
#define RFGSM_CMD_PROC_LOG_VARS() \
                   unsigned long long start_pcycle = qurt_get_core_pcycles(); \
                   unsigned long long stop_pcycle = 0;                        \
                   uint32 start_ustmr = rfgsm_mc_read_ustmr_count();          \
                   uint32 stop_ustmr = 0;                                     \
                   uint32 duration_us = 0;                                    \
                   uint32 start_time_qs = 0;                                  \
                   sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_NONE;       \
                   uint32 frame_number = 0;



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_CMD_PROC_LOG                                               */
/*! @brief a macro to log processing calls at the command level              */
/*! @details if the feature RFGSM_LOG_LEVEL_CMD is not defined they will not */
/*!          be logged                                                       */
#ifdef RFGSM_LOG_LEVEL_CMD
#define RFGSM_CMD_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) \
              RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time )
#else
#define RFGSM_CMD_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_CMD_PROC_LOG_CMN_START                                     */
/*! WARNING: THIS MACRO DEPENDS ON RFGSM_CMD_PROC_LOG_VARS()                 */
/*! @brief a macro to log command proc calls from GL1 and MCS                */
/*! @details macro is used for start where start times, frame number, and    */
/*!          start qs have already been acquired.                            */
/*!          if the feature RFGSM_LOG_LEVEL_CMD is not defined they will not */
/*!          be logged.                                                      */
#ifdef RFGSM_LOG_LEVEL_CMD
#define RFGSM_CMD_PROC_LOG_CMN_START(input_cmd, subscription, arg, seq_num ) \
          RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, frame_number, start_time_qs, start_ustmr, start_pcycle, 0 )
#else
#define RFGSM_CMD_PROC_LOG_CMN_START(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_CMD_PROC_LOG_CMN_STOP                                      */
/*! WARNING: THIS MACRO DEPENDS ON RFGSM_CMD_PROC_LOG_VARS()                 */
/*! @brief a macro to log command proc calls from GL1 and MCS                */
/*! @details macro is used for stop where start times, frame number, and     */
/*!          start qs has already been acquired                              */
/*!          if the feature RFGSM_LOG_LEVEL_CMD is not defined they will not */
/*!          be logged.                                                      */
#ifdef RFGSM_LOG_LEVEL_CMD
#define RFGSM_CMD_PROC_LOG_CMN_STOP(input_cmd, subscription, arg, seq_num ) \
          stop_ustmr = rfgsm_mc_read_ustmr_count();  \
          stop_pcycle = qurt_get_core_pcycles();     \
          duration_us = rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, stop_ustmr ); \
          RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, frame_number, start_time_qs, stop_ustmr, stop_pcycle, duration_us )
#else
#define RFGSM_CMD_PROC_LOG_CMN_STOP(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_MC_PROC_LOG                                                */
/*! @brief a macro to log processing calls at the main control level         */
/*! @details if the feature RFGSM_LOG_LEVEL_MC is not defined they will not  */
/*!          be logged                                                       */
#ifdef RFGSM_LOG_LEVEL_MC
#define RFGSM_MC_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) \
             RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time )
#else
#define RFGSM_MC_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_MC_PROC_LOG_CMN_START                                      */
/*! WARNING: THIS MACRO DEPENDS ON RFGSM_CMD_PROC_LOG_VARS()                 */
/*! @brief a macro to log mc proc calls from meas and within RF GSM          */
/*! @details macro is used for start where start times, frame number, and    */
/*!          start qs have already been acquired.                            */
/*!          if the feature RFGSM_LOG_LEVEL_MC is not defined they will not  */
/*!          be logged.                                                      */
#ifdef RFGSM_LOG_LEVEL_MC
#define RFGSM_MC_PROC_LOG_CMN_START(input_cmd, subscription, arg, seq_num ) \
          RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, frame_number, start_time_qs, start_ustmr, start_pcycle, 0 )
#else
#define RFGSM_MC_PROC_LOG_CMN_START(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_MC_PROC_LOG_CMN_STOP                                       */
/*! WARNING: THIS MACRO DEPENDS ON RFGSM_CMD_PROC_LOG_VARS()                 */
/*! @brief a macro to log mc proc calls from meas and within RF GSM          */
/*! @details macro is used for stop where start times, frame number, and     */
/*!          start qs has already been acquired                              */
/*!          if the feature RFGSM_LOG_LEVEL_MC is not defined they will not  */
/*!          be logged.                                                      */
#ifdef RFGSM_LOG_LEVEL_MC
#define RFGSM_MC_PROC_LOG_CMN_STOP(input_cmd, subscription, arg, seq_num ) \
          stop_ustmr = rfgsm_mc_read_ustmr_count();  \
          stop_pcycle = qurt_get_core_pcycles();     \
          duration_us = rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, stop_ustmr ); \
          RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, frame_number, start_time_qs, stop_ustmr, stop_pcycle, duration_us )
#else
#define RFGSM_MC_PROC_LOG_CMN_STOP(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_CORE_PROC_LOG                                              */
/*! @brief a macro to log processing calls at the main control level         */
/*! @details if the feature RFGSM_LOG_LEVEL_CORE is not defined they will not*/
/*!          be logged                                                       */
#ifdef RFGSM_LOG_LEVEL_CORE
#define RFGSM_CORE_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) \
               RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time )
#else
#define RFGSM_CORE_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif



/*---------------------------------------------------------------------------*/
/*! @define RFGSM_DEVICE_PROC_LOG                                            */
/*! @brief a macro to log processing calls at the main control level         */
/*! @details if the feature RFGSM_LOG_LEVEL_DEVICE is not defined they will  */
/*!          not be logged                                                   */
#ifdef RFGSM_LOG_LEVEL_DEVICE
#define RFGSM_DEVICE_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) \
                 RFGSM_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time )
#else
#define RFGSM_DEVICE_PROC_LOG(input_cmd, subscription, arg, seq_num, fn, qs, usmtr, pcycle, exec_time ) /* Suppressed - Do nothing */
#endif

#ifdef FEATURE_QSH_MDUMP 
  QSH_MDUMP_FN_ATTR void rf_gsm_QSH_mdump_cb ( void );
#endif /* FEATURE_QSH_MDUMP */


#ifdef FTM_HAS_LOGGING
/*---------------------------------------------------------------------------*/
// Rf to FTM_TASK message structure
typedef struct
{
  q_link_type            link;
  rfgsm_core_tx_log_type msg;
} rfgsm_core_log_data;

/*---------------------------------------------------------------------------*/
typedef struct
{
  q_type              rfgsm_log_q;
  q_type              rfgsm_log_free_q;
  rfgsm_core_log_data rfgsm_buffers[ RFGSM_LOG_BUFFERS];
} rfgsm_log_info_type;

/*---------------------------------------------------------------------------*/
typedef PACK(struct)
{
  ftm_log_gen_type       header;
  rfgsm_core_tx_log_type data;
} rfgsm_core_log_type;



/*===========================================================================*/
/*                                                                           */
/*                              PROTOTYPES                                   */
/*                                                                           */
/*===========================================================================*/
/*!
  @details
  This function initializes resources for the FTM task queue
  Rx burst.
  @param void
  @retval void
*/
void rfgsm_core_log_q_init( void);

/*---------------------------------------------------------------------------*/
/*!
  @details
  This function "Allocates" a buffer for RFGSM to send messages to FTM and returns a pointer for the free buffer.
  @param void
  @retval void*
*/
void *rfgsm_core_log_get_buf( void);

/*---------------------------------------------------------------------------*/
/*!
  @details
  This function sends a populated buffer (message structure) to ftm task.
  @param void
  @retval void*
*/
void rfgsm_core_log_send_to_ftm_task( rfm_device_enum_type rfm_dev );

/*---------------------------------------------------------------------------*/
/*!
  @details
  This function dequeues items (head first) from the FTM task buffer and populates log packet before sending it to DiaG.
  @param void
  @retval void*
*/
void rfgsm_core_log_handle_q_from_rf( void );

/*---------------------------------------------------------------------------*/
#endif /* FTM_HAS_LOGGING */
#endif 

