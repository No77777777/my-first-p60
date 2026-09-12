#ifndef HDRSRCHRFI_H
#define HDRSRCHRFI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=

                        HDR Search RF Manager Internal Header

                   HDR Search RF Manager Internal Header File

GENERAL DESCRIPTION

  This file provides structures for the HDR SRCH RF files.

Copyright (c) 2009 - 2021 by Qualcomm Technologies, Inc.  All Rights Reserved.

=*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=



==============================================================================

                           EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchrfi.h#2 $ $DateTime: 2021/05/18 05:54:37 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     --------------------------------------------------------- 
05/18/2021   vaa     Fix for compilation errors in MSIM disable build flavor
08/24/2017   rmv     Mainline FEATURE_HDR_QTA_THRU_TRM 
10/27/2015   rmv     Prevent reporting QTA END twice to TRM 
10/08/2015   vke     Use TA information from TRM callback for deciding QTA
09/30/2015   vko     Update subreason type to sys_proc_type_e_type
07/24/2015   wsh     Fixed TX chain event being treated as PRI chain 
06/30/2015   wsh     Added WTR hopping support
06/29/2015   vko     Pass subreason also, while calling the TRM API
06/22/2015   wsh     Fixed not releasing TX chain 
05/07/2015   wsh     Added RxTx split support 
08/26/2014   bb      Changes to handle diversity release when unlock event is 
                     ignored by primary client during Access state
08/07/2014   mbs     Added support for trm lock exchange during IRAT procedures
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
05/15/2014   arm     TRM API changes for DR-DSDS. 
05/14/2014   vke     Fix to prevent blocking of TAs for 1x and G due to 
                     bug in the HDR TRM-interface
05/08/2014   dsp     Disabled Access TA for GSM.
03/21/2013   kss     Remove unused legacy code. 
09/25/2012   rmg     Added trm_freq_info field to hdrsrchrf struct definition.
03/08/2012   wsh     Removed deprecated sample server interface
02/19/2012   wsh     Moved Req&Notify pending logic to hdrsrchrf.c 
01/05/2012   wsh     Added support for SVLTE+SHDR
01/05/2011   wsh     Fixed crash in HHO to same band 
04/17/2011   kss     Added support for rfm_hdr_exec_wakeup_rx().
04/01/2011   kss     Integrated RxLM APIs. 
06/21/2010   cnx     Fixed compiler warnings.
04/08/2010   smd     Mainlined hdrsrchrf_rfm_cb() and hdrsrchrf_rfm_wait().
02/04/2010   ljl     Supported MSGR interface.
05/31/2005   grl     Initial AMSS version

============================================================================*/



/*============================================================================

                           INCLUDE FILES FOR MODULE

============================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "comdef.h"
#include "hdrmdsp.h"
#include "lm_types.h"
#include "hdrsrchrf.h"

/*============================================================================

                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

============================================================================*/

/* Converts a HDRSRCHRF chain to a TRM client. */
#define HDRSRCHRF_CHAIN_TO_TRM( hdrsrchrf_chain )                      \
  ( ( hdrsrchrf_chain == HDRSRCHRF_PRI_CHAIN ) ? TRM_HDR :             \
    ( hdrsrchrf_chain == HDRSRCHRF_DIV_CHAIN ) ? TRM_HDR_SECONDARY :   \
                                                 TRM_HDR_TX )

/* Converts to a TRM client from a HDRSRCHRF chain. */
#define HDRSRCHRF_CHAIN_FROM_TRM( trm_client )                         \
  ( ( trm_client == TRM_HDR ) ?           HDRSRCHRF_PRI_CHAIN :        \
    ( trm_client == TRM_HDR_SECONDARY ) ? HDRSRCHRF_DIV_CHAIN :        \
                                          HDRSRCHRF_TX_CHAIN )

/* Retrieve the antenna selection from the diversity NV item. */
#define HDRSRCHRF_GET_ANT_SEL( comb ) \
  ( ( hdrsrchrf_comb_ant_enum_type ) ( ( comb >> 3 ) & 0x3 ) )

/* Convert an AGC value to Power in DB */
#define HDRSRCHRF_CONVERT_AGC_TO_DB( agc ) \
  agc += ( agc < 0 ? -128 : 128 );         \
    /* Round of to nearest 0.5dB */        \
  agc /= 256;                              \
    /* Convert to full dbm unit */

#define HDRSRCHRF_DIV_TRM_CLIENT TRM_HDR_SECONDARY
  /* The TRM diversity client */

/* The tuning state of a receiver. */
typedef enum
{
  HDRSRCHRF_NO_TUNE_IN_PROGRESS,
    /* Not tuned yet. */

  HDRSRCHRF_WAITING_BAND_GRANT,
    /* Requested band change to TRM */

  HDRSRCHRF_TUNING_IN_PROGRESS,
    /* Started tune but it isn't done. */

  HDRSRCHRF_LOOPS_SETTLING,
    /* Tune is done but AGC loops are still settling. */

  HDRSRCHRF_RF_TUNED
    /* Completed the tune and AGCs are settled. */
}
hdrsrchrf_tune_state_type;


#ifdef FEATURE_HDR_REVB
typedef struct
{
  boolean          enabled;
    /* if the CSR is enabled */

  sys_channel_type channel;
    /* the channel index to the list of channel[] */
}
hdrsrchrf_carrier_type;
#endif /* FEATURE_HDR_REVB */

/* Contains all information related to a pending tune. */
typedef struct
{
  /* temp. channel and demod_idx, needed in case band tune is canceled */
#ifdef FEATURE_HDR_REVB
  sys_channel_type                    channel[HDRSRCH_MAX_NUM_SUBASETS];
    /* Channel list to tune */

  hdrsrch_demod_idx_type              demod_idx[HDRSRCH_MAX_NUM_SUBASETS];
    /* Demod index assignment matching to the channel assignment*/

  uint8                               channel_cnt;
    /* the number of channel configured */
#else
  sys_channel_type                    channel;
    /* Channel to tune to */
#endif /* FEATURE_HDR_REVB */

  boolean                             band_granted;
    /* keep track if band is granted */

  boolean                             band_changing;
    /* keep track if band has changed, if so, call band_tune_complete */

  hdrsrchrf_tune_state_type           prev_state;
    /* prev state prior to band tune */
}
hdrsrchrf_tune_band_change_struct_type;

/* Contains all information related to tuning a given receiver. */
typedef struct
{
#ifdef FEATURE_HDR_REVB
  sys_channel_type                    channel[HDRSRCH_MAX_NUM_SUBASETS];
    /* Channel list to tune */

  hdrsrch_demod_idx_type              demod_idx[HDRSRCH_MAX_NUM_SUBASETS];
    /* Demod index assignment matching to the channel assignment*/

  hdrsrchrf_carrier_type              csr[HDRSRCH_MAX_NUM_CARRIERS];
    /* Carrier list which indicates in any carrier is enabled */

  uint8                               channel_cnt;
    /* the number of channel configured */
#else
  sys_channel_type                    channel;
    /* Channel to tune to */
#endif /* FEATURE_HDR_REVB */

  hdrsrchrf_tune_done_cb_t            done_cb;
    /* Function to call when tuning is complete */

  hdrsrchrf_tune_state_type           state;
    /* Not tuning, tuning, or settling */

  rex_sigs_type                       tune_sig;
    /* Signals to add to the wait mask */

  timer_type                          tune_timer;
    /* Timer used for RF tuning */

  hdrsrchrf_tune_band_change_struct_type
                                      band_change;
}
hdrsrchrf_tune_struct_type;


/*============================================================================

CALLBACK HDRSRCHRF_UNLOCK_CALLBACK_T

DESCRIPTION
  The prototype for unlock event callback functions, used to inform the clients 
  of when it should unlock a resource it holds.
  
DEPENDENCIES
  It may be called from a task context of another client, or from interrupt 
  context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

typedef void (*hdrsrch_rf_unlock_cb_t)
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_unlock_event_enum_t         event,
    /* The event being sent to the client */

  uint32                                unlock_by_sclk
    /* Sclk timestamp for HDRSRCHRF_UNLOCK_BY */
);

/* The enum defines the current TRM lock grantor */
typedef enum
{
  HDRSRCHRF_NONE = 0,
  HDRSRCHRF_IRAT,
  HDRSRCHRF_TRM
} hdrsrchrf_lock_grant_enum_type;

/* The enum to keep track of RxTx TRM async cb */
typedef enum
{
  HDRSRCHRF_NO_EVENT,
  HDRSRCHRF_GRANTED,
  HDRSRCHRF_UNLOCK_REQUESTED,
  HDRSRCHRF_DENIAL
} hdrsrchrf_async_event_type;

/* Structure which contains all tracked RF chain information. */
typedef struct
{
  boolean                       owns_chain;
    /* Whether this client owns rf_chain */

  boolean                       is_enabled;
    /* Whether the chain has been enabled already. */

  boolean                       is_tuned;
    /* Whether the chain has been tuned already. */

  rfm_device_enum_type          rf_chain;
  /* The RF physical chain mapped into the logical chain. */

  /*  The following members are not relevant to diversity chains. */

  hdrsrchrf_grant_callback_t    grant_cb;
    /* The granting callback. */

  hdrsrch_rf_unlock_cb_t        unlock_cb;
    /* The unlock callback for this given client. */

  hdrsrchrf_tune_struct_type    tune;
    /* Information related to tuning. */

  hdrsrchrf_retain_lock_state_t rl_state;
    /* Information related to retain lock. */

  hdrsrchrf_reason_enum_t       reason;
    /* Reason the lock is held */

  hdrsrchrf_sub_reason_enum_t   sub_reason;
    /* Sub reason for the lock */

  lm_handle_type                rxlm_handle;
    /* State of each logical chain */

  boolean                       rxlm_handle_valid;
    /* Validity of the stored handle locations. */

  boolean                       req_notify_pending;

  trm_grant_return_enum_type    modify_chain_granted;
    /* Was modify request granted by TRM? */

  trm_duration_t                modify_duration;
    /* If granted what was the new duration */

  trm_modify_return_enum_type   modify_band_granted;

  hdrsrchrf_lock_grant_enum_type    lock_grantor;
    /* Keeps track of TRM lock grantor */

  hdrsrchrf_resource_enum_t     resource;
    /* The RF resource which is being requested */

  hdrsrchrf_duration_t          duration;
    /* How long the resource will be needed for (in sclks) */

  hdrsrchrf_async_event_type    rxtx_event;
    /* TRM async event (grant/unlock) for RxTx R&N handling */
     
  rfm_device_enum_type          new_dev;
    /* Newly granted dev after band tune */

} hdrsrchrf_logical_chain_t;

/* Convert half slots to microseconds. */
#define HDRSRCHRF_HS_TO_US( num_hs ) ( num_hs * 833 )

#define HDRSRCHRF_OVERSHOOT_LEN         100
  /* Number of tracking periods in which AGC variance is tracked. The algorithm
     is based on this being 100 so changes must be made elsewhere if this value
     is changed. */

/* For DAST (Dynamic AGC Settling Time) - The channel condition information 
   tracked to decide whether to go into the resume AGC acq algorithm or the 
   fast AGC acq algorithm. */
typedef struct
{
  sys_channel_type               channel;
    /* Which channel this structure is estimating. */

  boolean                        is_stable_chan;
    /* Whether the channel has been declared stable enough to use the resume
       AGC acquisition algorithm. */

  boolean                        force_resume_agc_off;
    /* Whether an external module has indicated the channel is unstable
       by an external criteria and wants to force the resume AGC 
       algorithm off. */

  uint8                          overshoot_inst[ HDRSRCHRF_OVERSHOOT_LEN ];
    /* An array of instances in which the AGC varied beyond the design of
       the AGC resume acquisition algorithm. Each instance contains either
       a 0 or a 1. This is a circular buffer whose next entry goes into
       overshoot_idx. */

  uint8                          overshoot_idx;
    /* Where we are in the overshoot_inst circular buffer. */

  int16                          prev_agc;
    /* The AGC that was captured the end of the last tracking interval. */

  hdrsrchrf_time_t               prev_time;
    /* The last time in an SCLK timestamp (wraps every 18 hours) which 
       AGC information was captured. */

} hdrsrchrf_agc_est_struct_type;

/* The structure which contains filtered receive power */
typedef struct
{
  int                            rx0;
   /* filtered rx0 power */

  int                            rx1;
   /* filtered rx1 power */

  int16                          length;
   /* The filter length */

}  hdrsrchrf_filtered_rx_power_struct_type;

/* Debug structure to store sclk times */
typedef struct
{
  uint32                            start_time_sclk;
    /* Start of rf tune time in sclk */

  uint32                            enable_time_sclk;
    /* Time rf chain is enabled in sclk */

  uint32                            tune_time_sclk;
    /* Time rf tune is complete in sclk */

}
hdrsrchrf_time_struct_type;

/* Structure of the firmware messages */
typedef struct
{
  hdrfw_diversity_cfg_msg_t          div_cfg_msg;
    /* Diveristy config message */

  hdrfw_force_comb_mode_msg_t        force_comb_mode_msg;
    /* Diversity combining mode message */
}
hdrsrchrf_fw_msg_struct_type;

/* The structure which contains all module information. */
typedef struct
{
  int                               tx_open_loop;
    /* saved open loop value */

  int                               rx0_dbm;
  int                               rx1_dbm;
    /* stored AGC parameters in dbm units */

  hdrsrchrf_logical_chain_t         chain[HDRSRCHRF_NUM_CHAINS];
    /* State of each logical chain */

  boolean                           rf_exec_wakeup_rx_pending;
    /* Tracks whether there is a rfm_hdr_wakeup_exec call pending */

  hdrsrchrf_agc_acq_mode_enum_type  agc_acq_mode;
    /* Mode for AGC acquisition */

  uint32                            agc_rtc_offset;
    /* RTC offset to use for fast AGC mode */

  int16                             agc_value;
    /* Starting AGC value to be used for starting the resume AGC acq 
       algorithm. */

  uint8                             loop_settling_time;
    /* Time taken for AGC loop settling */

  hdrsrchrf_agc_est_struct_type     agc_est;
    /* The AGC stability estimator used for the DAST algorithm  */

  boolean                           tune_timer_defined;
    /* Flag to indicate tune timer defined or not */

  hdrsrchrf_filtered_rx_power_struct_type rx_power;
    /* Filtered receive power */

  hdrsrchrf_fw_msg_struct_type      fw_msg;
    /* Firmware messages */

  hdrsrchrf_trm_freq_info_type      trm_freq_info;
    /* Frequency info to be provided to TRM for resource reservation. */

  boolean                           pri_chain_unlock_ignored;
   /* flag to indicate that primary chain is ignored UNLOCK call back during Access state */ 

  rex_timer_type                    rxtx_pending_timer;
    /* timer to enforce upper bound waiting for RxTx chain */

  boolean                           rxtx_pending;
    /* Whether RxTx request is pending */

#ifdef FEATURE_HDR_QTA
  hdrsrchrf_client_state_enum_type  qta_client_state;
    /* Recent QTA client state sent to TRM */
#endif /* FEATURE_HDR_QTA */

}
hdrsrchrf_struct_type;

extern hdrsrchrf_struct_type         hdrsrchrf;
 /* HDR Searcher RF parameters consolidated in one structure */


/*===========================================================================

FUNCTION HDRSRCHRF_RFM_CB

DESCRIPTION
  This is the generic callback function passed to rfm function calls.
  Depending on the rf status, this function sends a signal to let the hdrsrch
  task know that rf processing is complete and hdr srch processing can
  continue.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void hdrsrchrf_rfm_cb
(
  rfm_cb_event_enum_type      rf_status,
    /* Status of rf enable transmitter */

  void *                      data
    /* Callback data */
); 

/*===========================================================================

FUNCTION HDRSRCHRF_RFM_WAIT

DESCRIPTION
  This function waits for and clears the generic HDRSRCH_RFM_SIG signal.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void hdrsrchrf_rfm_wait( void );


/*===========================================================================

FUNCTION HDRSRCHRF_ASSIGN_SAMPLE_SERVERS

DESCRIPTION
  The function assign the sample based on the current channel configuration.
  The sample server configuration should match to the CSR configuration in 
  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr.

DEPENDENCIES
  The channel list should be updated before calling this function. For the
  Diversity chain, the tunning info and CSR info should be copied from
  the primary chain to the secondary chain.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hdrsrchrf_assign_sample_servers
(
  hdrsrchrf_logical_chain_enum_t  chain_id
    /* Logic chain ID */
);

/*============================================================================

CALLBACK HDRSRCHRF_UNLOCK_CALLBACK

DESCRIPTION
  The prototype for unlock event callback functions, used by the Transceiver
  Resource Manager to inform the clients of when it should unlock a resource
  it holds.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
extern void hdrsrchrf_unlock_callback
(

  trm_client_enum_t               client,
    /* The client which is being informed of an event */

  trm_unlock_event_enum_t         event,
    /* The event being sent to the client */

  uint32                          unlock_by_sclk,
    /* Sclk timestamp for HDRSRCHRF_UNLOCK_BY */

  trm_client_info_t               winning_client,
    /* Winning Client */

  trm_tuneaway_enum_type         ta_info
    /* TA status (QTA or LTA) */
);

#endif /* HDRSRCHRFI_H */

