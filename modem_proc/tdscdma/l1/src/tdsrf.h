#ifndef TDSCDMA_RF__H
#define TDSCDMA_RF__H
/*!
  @file tdsrf.h

  @brief
  The interface definitions between the TDSCDMA L1 Software and RF Drivers and 
  Link Manager modules.

  This files defines APIs that can be called from TDSCDMA L1 that invoke services 
  provided by RF drivers and Link Manager

  @ingroup 
*/

/*==============================================================================

  Copyright (c) 2009 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsrf.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/17/14   sk      Fixed compile warnings
11/22/13   sk      RxLM optimization phase 2 - CR56584  
04/04/13   kguo    Upmerge from Nikel
09/17/12   kguo    Updated data structure for diversity
08/28/12   kguo    Updated function prototype for RxD
07/19/12   kguo    Updated function prototype to support RxD
06/19/12   asm     Ensure RF messages are handled in serialized order
06/06/12   kguo    Added RxD support under feature " FEATURE_TDSCDMA_RX_DIVERSITY " 
03/21/12   asm     Fixed Compiler Warnings
03/09/12   kguo    Added support for sleep/wakeup interface with RF
01/30/12   asm     Added interface for setting Network Tx Power Limit to RF
11/15/11   asm     Cleaned up code and removed all LM workarounds
11/09/11   asm     Added interface to query Rx LM buffer index for Interfreq HHO
10/19/11   asm     Modified interface for Disable_tx and added support for exit_mode
09/19/11   asm     Added TDS RF Callback type
09/19/11   asm     Added ifreq API support for off-target
09/19/11   asm     Added APIs for updating ifreq neighbors and working freq
09/01/11   asm     Correction to API definition
08/30/11   asm     Added Missing include file
08/30/11   asm     Added more API definitions
08/30/11   asm     Changes to enable Tx
08/23/11   asm     Cleaned up and consolidated RF-related procedures in RF module
08/18/11   asm     Initial revision - Created RF Interface Definitions and Link Manager interfaces

==============================================================================*/


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include "tdscdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "sys.h"
#include "tdsrf_lm.h"
#include "tfw_sw_intf_msg.h"
#include "tdsl1m.h"

#ifdef FEATURE_TDSCDMA_RF_SUPPORT
#include "rf_tdscdma_msg.h"
#include "rfcom.h"
#endif

#include "tdsrxdiv.h"

#include "cxm.h"

#include "tdsl1m_rcinit.h"
/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

// RF call-back function type
typedef void (*tdsrf_cb_type) ( void );
 
// Interfreq neighbor type
// Fields: uint16 channel, uint8 nbr_buf_idx
#ifdef FEATURE_TDSCDMA_RF_SUPPORT
typedef rfa_tdscdma_nbr_entry_t tdsrf_ifreq_nbr_type;
#else
// defining the struct from above for off-target RF support
typedef struct {
  uint16 channel;
  uint8 nbr_buf_idx;
} tdsrf_ifreq_nbr_type;
#endif

// RF-related data required for programming FW
typedef struct
{
  uint32 rxlm_static_buf_idx;
  uint32 rxlm_static_buf_idx_div;
  uint8  rf_rx_buf_idx;
  uint8  rf_rx_buf_idx_div;
  uint32 txlm_static_buf_idx;
  uint8  rf_tx_buf_idx;
  uint32 carrier_freq;

} tdsrf_rf_freq_related_info_type;

// Enum to indicate to tdsrf module the state of HSDPA
typedef enum
{
  TDSRF_HSDPA_DISABLE,
  TDSRF_HSDPA_ENABLE
} tdsrf_hsdpa_state_enum_type;

typedef enum {
  TDSRF_TX_DISABLED=0, 
  TDSRF_TX_IS_ENALBING=1, 
  TDSRF_TX_ENABLED=2, 
  TDSRF_TX_IS_DISALBING=3,
  TDSRF_TX_INVALID
}tdsrf_tx_status;


// Enum to indicate the state of tdsrf in the DRX mode
typedef enum
{
   TDSRF_ASLEEP,
   TDSRF_WAITING_FOR_WAKEUP_CNF,
   TDSRF_AWAKE,
   TDSRF_WAITING_FOR_SLEEP_CNF,
   TDSRF_INVALID
}tdsrf_drx_state_enum_type;

#ifdef FEATURE_TDSCDMA_RF_SUPPORT

#define MAX_RF_WARM_UP_TIME_USEC 5000
#define MIN_RF_WARM_UP_TIME_USEC 1
#define TDSRF_MSGR_RF_NUM_BUFS   20

// Max number of messages stored in history
#define TDSRF_MAX_MSG_LOG_SIZE    10

#endif /* FEATURE_TDSCDMA_RF_SUPPORT */

#define TDSRF_IS_TX_DISABLE()  (tdsrf_globals.tx_is_enabled == TDSRF_TX_DISABLED)

extern rfa_tdscdma_rx_path_t  tdsrf_last_rx_path_config;   

/*==============================================================================

                             MACROS

==============================================================================*/
#define  TDSRF_MAX_NBR_FREQ             8


#define TDSRF_DRX_RF_STATE_INTLOCK()  REX_ISR_LOCK(&tdsrf_drx_rf_state_mutex)
#define TDSRF_DRX_RF_STATE_INTFREE()  REX_ISR_UNLOCK(&tdsrf_drx_rf_state_mutex)

/*==============================================================================

                       STRUCTURES AND UNIONS

==============================================================================*/
#ifdef FEATURE_TDSCDMA_RF_SUPPORT
// RF MSGR TYPES
typedef union
{
  /* Message Router Header field */
  msgr_hdr_s             msgr_hdr;

  rfa_tdscdma_enter_mode_cnf_s           enter_mode_cnf;
  rfa_tdscdma_exit_mode_cnf_s            exit_mode_cnf;
  rfa_tdscdma_enable_rx_cnf_s            enable_rx_cnf;
  rfa_tdscdma_enable_tx_cnf_s            enable_tx_cnf;
  rfa_tdscdma_disable_tx_cnf_s           disable_tx_cnf;
  rfa_tdscdma_sleep_cnf_s                sleep_cnf;
  rfa_tdscdma_wakeup_cnf_s               wakeup_cnf;
  rfa_tdscdma_set_tx_pwr_limit_cnf_s     set_netwrk_tx_pwr_limit_cnf;
  rfa_tdscdma_get_max_tx_pwr_cnf_s       get_device_tx_pwr_limit_cnf;
  rfa_tdscdma_get_rf_warmup_time_cnf_s   get_rf_warmup_time_cnf;
  rfa_tdscdma_update_bho_cnf_s           update_bho_cnf;
  rfa_tdscdma_switch_buf_cnf_s           switch_buf_cnf;
  rfa_tdscdma_update_tds_nbr_cnf_s       update_tds_nbr_cnf;
  rfa_tdscdma_update_gsm_nbr_cnf_s       update_gsm_nbr_cnf;
  rfa_tdscdma_get_rxagc_val_cnf_s        get_rxagc_val_cnf;
  rfa_tdscdma_get_agc_log_cnf_s          get_agc_log_cnf;
  rfa_tdscdma_therm_backoff_cnf_s       therm_backoff_cnf;
  #ifdef FEATURE_BOLT_MODEM
  rfa_tdscdma_get_rf_txchain_no_cnf_s    get_rf_txchain_no_cnf;
  #endif
#ifdef FEATURE_TDSCDMA_ANTENNA_SWITCH_DIVERSITY
  rfa_tdscdma_ant_sw_cnf_s               ant_sw_cnf;
#endif
#ifdef FEATURE_TDSCDMA_QTUNER
  rfa_tdscdma_update_tuner_cnf_s         update_tuner_cnf_s;
#endif
  #ifndef FEATURE_TDSCDMA_JOLOKIA
  rfa_tdscdma_dynamic_script_cnf_s       dynamic_tuner_cnf;
  #endif

} tdsrf_msgr_cmd_code_type;

// RF CMD type
typedef struct
{
  q_link_type link;

  tdsrf_msgr_cmd_code_type cmd_code;

} tdsrf_msgr_cmd_type;

// RF Internal status globals
typedef struct 
{
  // TDSCDMA Mode Enabled
  boolean tds_mode_enabled;

  uint16 prev_dl_channel_num; /* prev dl_channel_num when BHO */
  // Current DL Channel number
  uint16 dl_channel_num;

  // Current DL Carrier Frequency
  uint32 curr_carrier_freq_Hz;

  // Current RxLM mode
  rfa_tdscdma_rxlm_mode_t rxlm_mode;

  // Current DL Link Type
  tdsrf_rx_link_enum_type dl_link_type;

  // State of Tx
  //boolean tx_is_enabled;
  tdsrf_tx_status tx_is_enabled;

  //FW TXLM is enabled
  boolean txlm_disable_bypass;
  
  boolean rx_is_enabled;

  // RF Sleep State
  boolean rf_asleep; 
  
  uint16  rf_warmup_time;
  
  uint16  rf_txchain_no; //10_14

  uint32  rf_wake_up_late_cnt;

  // Device Tx power limit
  int16 device_tx_pwr_limit;

  // Network-signaled Tx power limit
  int16 netwrk_tx_pwr_limit;

  // Effective UE Tx power limit
  int16 effective_ue_tx_pwr_limit;

  // RF Rx buffer index [0,1]
  uint8 rf_rx_buf_idx;

  // RF Tx buffer index [0,1]
  uint8 rf_tx_buf_idx;

  // RF freqency list  
  uint32 carrier_freq_list[TDSRF_MAX_NBR_FREQ];

  // ifreq callback func
  tdsrf_cb_type ifreq_cb_func;

  #ifdef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
  // LNA state
  tfw_rx_lna_t lna_state;

  // Default LNA state
  tfw_rx_lna_t default_lna_state;

  // Default LNA state
  tfw_rx_lna_t default_acq_lna_state;
  
  #endif

  // HSDPA configured status
  tdsrf_hsdpa_state_enum_type hsdpa_state;

  // Sent message Log
  msgr_umid_type sent_msg_log[ TDSRF_MAX_MSG_LOG_SIZE ];
  uint8          sent_msg_log_idx;

  // Rcvd message Log
  msgr_umid_type rcvd_msg_log[ TDSRF_MAX_MSG_LOG_SIZE ];
  uint8          rcvd_msg_log_idx;

  // Last rcvd mesg
  msgr_umid_type last_recvd_rf_msg_id;

  // Last sent mesg
  msgr_umid_type last_sent_msg_id;

  // 
  boolean rxd_config_during_bho; 

  tdsrf_rx_link_enum_type save_link_type_for_bho;

  tdsrxd_rx_chain_status_type is_rxd_on;
  
  // DRX RF state
  tdsrf_drx_state_enum_type tdsrf_drx_state;

  // last time RF wake up indicator
  rfa_tdscdma_wakeup_indicator_t last_rf_wakeup_ind;

  rfm_device_enum_type pri_dev_id;
  rfm_device_enum_type sec_dev_id;

  boolean tx_chain_configured;
} tdsrf_globals_type;

extern tdsrf_globals_type tdsrf_globals;

#endif /* FEATURE_TDSCDMA_RF_SUPPORT */
/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/
extern boolean tdsrf_tds_mode_enabled (void);

/*===========================================================================
FUNCTION tdsrf_enable_rf_and_enter_mode

DESCRIPTION
  Initialize the TDSCDMA L1 RF Rx Interface and enter TD-SCDMA mode
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_enable_rf_and_enter_mode(void);

/*===========================================================================
FUNCTION tdsrf_disable_rf_and_exit_mode

DESCRIPTION
  De-initialize the TDSCDMA L1 RF Interface and Exit TD-SCDMA mode
  
DEPENDENCIES
  Tx Should be Disabled

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_disable_rf_and_exit_mode(void);

/*===========================================================================
FUNCTION tdsrf_enable_tx

DESCRIPTION
  Initialize the TDSCDMA L1 RF Tx Interface and enable Tx
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_enable_tx( tdsrf_tx_link_enum_type link_type );

/*===========================================================================
FUNCTION tdsrf_disable_tx

DESCRIPTION
  De-Initialize the TDSCDMA L1 RF Tx Interface and disable Tx
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_disable_tx( tdsrf_tx_link_enum_type link_type );

/*===========================================================================
FUNCTION tdsrf_is_tx_enabled

DESCRIPTION
  Check whether RF TX is enabled"
  
DEPENDENCIES
  Rx should be enabled

RETURN VALUE
  Boolean
  TRUE - RF TX is enabled, FALSE - otherwise

SIDE EFFECTS
  None
  
===========================================================================*/
boolean tdsrf_is_tx_enabled(void);


/*===========================================================================
FUNCTION tdsrf_is_rx_enabled

DESCRIPTION
  Check whether RF RX is enabled"
  
DEPENDENCIES

RETURN VALUE
  Boolean
  TRUE - RF RX is enabled, FALSE - otherwise

SIDE EFFECTS
  None
  
===========================================================================*/
boolean tdsrf_is_rx_enabled(void);

/*===========================================================================
FUNCTION tdsrf_tune_to_chan

DESCRIPTION
  Tune RF to required Channel, change RXLM mode based on FW state
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_tune_to_chan( uint16 freq_param, rfa_tdscdma_rx_path_t rx_path, tdsrf_rx_link_enum_type link_type);

#ifdef FEATURE_TDSCDMA_RF_SUPPORT
/*===========================================================================
FUNCTION tdsrf_tune_to_chan_with_mode

DESCRIPTION
  Tune RF to required Channel
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_tune_to_chan_with_mode( uint16 freq_param, tdsrf_rx_link_enum_type link_type, rfa_tdscdma_rxlm_mode_t mode );
#endif

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
/*===========================================================================
FUNCTION tdsrf_get_irat_lte_nbr_ant0_buffer_idx

DESCRIPTION
  Return the RF Inter-RAT LTE Neighbor Link Manager buffer index
  
DEPENDENCIES
  

RETURN VALUE
  uint8

SIDE EFFECTS
  None
  
===========================================================================*/
uint8 tdsrf_get_irat_lte_nbr_ant0_buffer_idx( void );

/*===========================================================================
FUNCTION tdsrf_get_irat_lte_nbr_ant1_buffer_idx

DESCRIPTION
  Return the RF Inter-RAT LTE Neighbor Link Manager buffer index
  
DEPENDENCIES
  

RETURN VALUE
  uint8

SIDE EFFECTS
  None
  
===========================================================================*/
uint8 tdsrf_get_irat_lte_nbr_ant1_buffer_idx( void );

#endif

/*===========================================================================
FUNCTION tdsrf_get_home_buffer_idx

DESCRIPTION
  Return the RF Link Manager Home buffer index
  
DEPENDENCIES
  

RETURN VALUE
  uint8

SIDE EFFECTS
  None
  
===========================================================================*/
uint8 tdsrf_get_home_buffer_idx( void );


/*===========================================================================
FUNCTION tdsrf_set_tx_pwr_limit

DESCRIPTION
  Set the Network-indicated Tx Power Limit in RF
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_set_netwrk_tx_pwr_limit( int16 tx_pwr_limit );

/*===========================================================================
FUNCTION  tdsrf_update_working_freq

DESCRIPTION
  Update the working frequency with the passed value
  
DEPENDENCIES
  None
  
RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_update_working_freq( uint32 new_freq_param );

/*===========================================================================
FUNCTION  tdsrf_update_ifreq_nbr_list

DESCRIPTION
  Update interfrequency neighbor list to RF
  
DEPENDENCIES
  None
  
RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_update_ifreq_nbr_list( 
  tdsrf_ifreq_nbr_type *nbr_list, 
  uint8 nbr_cnt, 
  tdsrf_cb_type ifreq_cb_func,
  rfa_tdscdma_rx_path_t rx_path,
  uint8 div_buf_idx);

/*===========================================================================
FUNCTION tdsrf_get_rf_warmup_time_usec

DESCRIPTION
  Get the RF warmup time in microseconds
  
DEPENDENCIES
  None

RETURN VALUE
  RF warmup time in microseconds

SIDE EFFECTS
  None
  
===========================================================================*/
uint16 tdsrf_get_rf_warmup_time_usec( void );

/*===========================================================================
FUNCTION tdsrf_get_rxagc_val

DESCRIPTION
  Get the Current Rx-AGC value
  
DEPENDENCIES
  None

RETURN VALUE
  RxAGC value in AGC units 

SIDE EFFECTS
  None
  
===========================================================================*/
int16 tdsrf_get_rxagc_val( void );


/*===========================================================================
FUNCTION tdsrf_get_rxagc_val_for_rscp

DESCRIPTION
  Get the Current Rx-AGC value
  
DEPENDENCIES
  None

RETURN VALUE
  RxAGC value in AGC units 

SIDE EFFECTS
  None
  
===========================================================================*/
int16 tdsrf_get_rxagc_val_for_rscp( void );


/*===========================================================================
FUNCTION tdsrf_set_txfreezeaccum

DESCRIPTION
  Freeze Tx Accum
  
DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
  
===========================================================================*/
// is this needed?
void tdsrf_set_txfreezeaccum( boolean freeze );

/*===========================================================================
FUNCTION tdsrf_get_curr_band

DESCRIPTION
  Returns the current band
  
DEPENDENCIES
  None

RETURN VALUE
  sys_band_class_e_type

SIDE EFFECTS
  None
  
===========================================================================*/
#ifdef FEATURE_TDSCDMA_RF_SUPPORT
extern sys_band_class_e_type tdsrf_get_curr_band_rf_type(uint16 freq);
#endif

extern sys_band_class_e_type tdsrf_get_curr_band( void );


/*===========================================================================

FUNCTION TDSRF_PROCESS_MSGR_RF_CMD

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
msgr_umid_type tdsrf_process_msgr_rf_cmd( void );

/*===========================================================================
FUNCTION     tdsrf_init

DESCRIPTION  This function initializes TDSCDMA RF interface
  
DEPENDENCIES
 

PARAMETERS
 

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrf_init(void);

/*===========================================================================
FUNCTION     tdsrf_msgr_delete

DESCRIPTION  This function delete MSGR for TDSCDMA RF client
  
DEPENDENCIES
 

PARAMETERS
 

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrf_msgr_delete(void);



/*===========================================================================
FUNCTION tdsrf_get_tx_pwr_limit

DESCRIPTION
  Return the Stored Max Tx Power Limit from RF
  
DEPENDENCIES
  None

RETURN VALUE
  int16  

SIDE EFFECTS
  None
  
===========================================================================*/
extern int16 tdsrf_get_tx_pwr_limit( void );

/*===========================================================================
FUNCTION  tdsrf_get_tx_agc_val

DESCRIPTION
  Get Tx AGV value in dB units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int16
  
SIDE EFFECTS
  None
===========================================================================*/
//void tdsrf_update_gsm_nbr_freq(
//  tdsrf_irat_nbr_freq_type *x,
//  uint8 nbr_cnt,
//  tdsrf_cb_type *irat_cb_func);



/*===========================================================================
FUNCTION tdsrf_cfg_acq_mode

DESCRIPTION
  Configures Link Manager for ACQ mode.
  
DEPENDENCIES
  Should be called only after ACQ success

RETURN VALUE
  Void 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_cfg_acq_mode( void );

/*===========================================================================
FUNCTION tdsrf_cfg_trk_mode

DESCRIPTION
  Configures Link Manager for TRACKING mode.
  
DEPENDENCIES
  Should be called only after ACQ success

RETURN VALUE
  Void 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_cfg_trk_mode( void );

/*===========================================================================
FUNCTION tdsrf_get_ifreq_nbr_buffer_idx

DESCRIPTION
  Return the RF Interfrequency Link Manager buffer index
  
DEPENDENCIES
  

RETURN VALUE
  uint8

SIDE EFFECTS
  None
  
===========================================================================*/
uint8 tdsrf_get_ifreq_nbr_buffer_idx( void );

/*===========================================================================
FUNCTION  tdsrf_send_update_bho_req

DESCRIPTION
  send UPDATE_BHO_REQ to RF
  
DEPENDENCIES
  None
  
RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_send_update_bho_req(uint16 primary_freq, uint16 working_freq, rfa_tdscdma_rx_path_t rx_path);

/*===========================================================================
FUNCTION  tdsrf_switch_buf

DESCRIPTION
  send SWITCH_BUF_REQ to RF
  
DEPENDENCIES
  None
  
RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_switch_buf( tdsrf_buf_swap_t buf_to_swap);

/*===========================================================================
FUNCTION  tdsrf_swap_rxlm_txlm_indices

DESCRIPTION
  Swap RXLM TXLM indices
  
DEPENDENCIES
  None
  
RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_swap_rxlm_txlm_indices(void);

/*===========================================================================
FUNCTION tdsrf_send_fw_rx_config

DESCRIPTION
  Send RX Config to FW.
  link_type: Link-type: Home, IFreq, etc
  freq_error_offset: freq error in 1/256 Hz
DEPENDENCIES
  None

RETURN VALUE
  Void 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_send_fw_rx_config( 
  tdsrf_rx_link_enum_type link_type,
  tdsrxd_rx_chain_status_type is_rxd_on );
void tdsrf_send_fw_rx_config_update( 
  tdsrf_rx_link_enum_type link_type,
  tdsrxd_rx_chain_status_type is_rxd_on );

/*===========================================================================
FUNCTION tdsrf_get_rxlm_buf

DESCRIPTION
  Function gets the RxLM chain for the client specified.
  
DEPENDENCIES
  None

RETURN VALUE
  RxLM buf allocated.

SIDE EFFECTS
  None
  
===========================================================================*/
uint32 tdsrf_get_rxlm_buf(
  /* Client to be translated */
  tdsrf_rx_link_enum_type client);

/*===========================================================================
FUNCTION tdsrf_get_txlm_buf

DESCRIPTION
  Function gets the RxLM chain for the client specified.
  
DEPENDENCIES
  None

RETURN VALUE
  RxLM buf allocated.

SIDE EFFECTS
  None
  
===========================================================================*/
uint32 tdsrf_get_txlm_buf(
  /* Client to be translated */
  tdsrf_tx_link_enum_type client);

/*===========================================================================
FUNCTION  tdsrf_get_freq_related_info

DESCRIPTION
  Supplies caller with a data structure containing frequency-specific information
  
DEPENDENCIES
  None
  
RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_get_freq_related_info( tdsrf_rf_freq_related_info_type *freq_data, tdsrf_rx_link_enum_type link_type );


#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF
/*===========================================================================
FUNCTION tdsrf_therm_backoff
         
DESCRIPTION
  This function calls RF API to reduce MTPL value in dB

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrf_therm_backoff(uint8 backoff_db);

#endif /* FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF */

/*===========================================================================
FUNCTION  tdsrf_asleep
         
DESCRIPTION
  return whether RF is in sleep state

DEPENDENCIES
  None

RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsrf_asleep(void);

/*===========================================================================
FUNCTION  tdsrf_awake
         
DESCRIPTION
  return whether RF is in sleep state

DEPENDENCIES
  None

RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsrf_awake(void);

/*===========================================================================
FUNCTION  tdsrf_sleep
         
DESCRIPTION
  send SLEEP REQ to RF

DEPENDENCIES
  None

RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_sleep( void );

/*===========================================================================
FUNCTION  tdsrf_wakeup

DESCRIPTION
  send WAKEUP REQ to RF
  
DEPENDENCIES
  None

RETURN VALUE
  void
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_wakeup(uint16 freq_param, rfa_tdscdma_wakeup_indicator_t flag) ; 

#ifdef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
/*===========================================================================
FUNCTION tdsrf_get_lna_state
         
DESCRIPTION
  This function returns the lna state data structure
  as required by FW

DEPENDENCIES
  None

RETURN VALUE
  tfw_rx_lna_t returned in the reference passed

SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_get_lna_state( tfw_rx_lna_t *lna_ptr );

/*===========================================================================
FUNCTION tdsrf_set_lna_state
         
DESCRIPTION
  This function sets the lna state data structure
  as returned by FW

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_set_lna_state( tfw_rx_lna_t *fw_lna_state );

/*===========================================================================
FUNCTION tdsrf_get_default_lna_state
         
DESCRIPTION
  This function returns the default lna state data structure
  as required by FW

DEPENDENCIES
  None

RETURN VALUE
  tfw_rx_lna_t returned in the reference passed

SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_get_default_lna_state( tfw_rx_lna_t* lna_ptr );

/*===========================================================================
FUNCTION tdsrf_update_hs_cfg
         
DESCRIPTION
  This function updates the HSDPA state to the tdsrf module

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_update_hs_cfg( tdsrf_hsdpa_state_enum_type hsdpa_state );

#endif

void tdsrf_cfg_rxd_mode (
  tdsrf_rx_link_enum_type link_type, // Link-type: Home, IFreq, etc
  tdsrxd_rx_chain_status_type is_rxd_on );

void tdsrf_update_curr_carrier_freq_Hz(void);

void tdsrf_recover_dl_channel_num_when_cancel_bho(void);

void tdsrf_update_dl_channel_num(uint16 freq);

void tdsrf_check_if_rxd_rx_config_is_pending(void);

/*===========================================================================

FUNCTION tdsrf_send_cxm_wwan_tech_state_ind

DESCRIPTION
  Send an update to MCS module 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void tdsrf_send_cxm_wwan_tech_state_ind(boolean stop_tds);

void tdsrf_send_cxm_wwan_tech_state_at_wakeup(uint16 freq);
#ifdef FEATURE_TDSCDMA_DSDA
/*===========================================================================

FUNCTION TDSRF_SEND_CXM_COEX_POWER_IND

DESCRIPTION
  Send an update to MCS module 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
#define TDSCDMA_MIN_TX_PWR_IN_DB_x10 (-64*10)
void tdsrf_send_cxm_coex_power_ind(int32 rx_power, uint32 rx_freqid, int32 tx_power, uint32 tx_freqid);
#ifdef FEATURE_TDSCDMA_BAND_AVOID
/*===========================================================================

FUNCTION TDSRF_SEND_CXM_COEX_BA_POWER_IND

DESCRIPTION
  Send band avoidance filtered rx/tx power report to MCS module 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void tdsrf_send_cxm_coex_ba_pwr_ind(int32 filt_rx_pwr, int32 filt_tx_pwr);
#endif
#else
/*===========================================================================

FUNCTION TDSRF_SEND_CXM_COEX_POWER_IND

DESCRIPTION
  Send an update to MCS module 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void tdsrf_send_cxm_coex_power_ind(int32 rx_power);
#endif

#ifdef FEATURE_TDSCDMA_ANTENNA_SWITCH_DIVERSITY
void  tdsrf_switch_ant(uint8 ant_no);
void  tdsrf_switch_ant_acq(uint8 ant_no);
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
/*===========================================================================
FUNCTION  WL1_RFM_MEAS_COMMON_EXIT_W2L

DESCRIPTION
  The function sends a meas_common_exit after a W2L gap search
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_rfm_meas_common_exit_t2l(uint16 freq, boolean aborted,
	 uint32 		cleanup_buffer_idx,
	 uint32 		startup_buffer_idx);


/*===========================================================================
FUNCTION  WL1_RFM_MEAS_COMMON_ENTER_W2L

DESCRIPTION
  The function sends a meas_common_enter before a W2L gap search
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrf_rfm_meas_common_enter_t2l(
uint16 freq, 
lm_handle_type target_rxlm_buf_idx_ant0, 
lm_handle_type target_rxlm_buf_idx_ant1,
uint32         cleanup_buffer_idx,
uint32         pre_gap_bsp_buffer_idx, 
uint32         build_bsp_script);
#endif
/*===========================================================================
FUNCTION tdsrf_send_update_qtuner_req

DESCRIPTION
  trigger RFA_RF_TDSCDMA_UPDATE_TUNER_REQ send
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
#ifdef FEATURE_TDSCDMA_QTUNER
void tdsrf_send_update_qtuner_req(tdsl1m_state_enum_type l1m_curr_state, uint16 curr_sub_frame);
#endif

/*===========================================================================
FUNCTION tdsrf_send_ifreq_qtuner_update_req

DESCRIPTION
  send RFA_RF_TDSCDMA_DYNAMIC_SCRIPT_REQ message to RF
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_send_ifreq_qtuner_update_req(uint8 buf_idx, uint32 freq_num);

#ifdef FEATURE_TDSCDMA_RF_SUPPORT
/*===========================================================================
FUNCTION tdsrf_get_curr_rf_card_band

DESCRIPTION
  Returns the current band
  
DEPENDENCIES
  None

RETURN VALUE
  rf_card_band_type

SIDE EFFECTS
  None

===========================================================================*/
rf_card_band_type tdsrf_get_curr_rf_card_band( uint16 freq );
/*===========================================================================
FUNCTION tdsrf_get_current_freq

DESCRIPTION
  Returns the current freqence
  
DEPENDENCIES
  None

RETURN VALUE
  uint16

SIDE EFFECTS
  None

===========================================================================*/
uint16 tdsrf_get_current_freq(void);


#endif /* FEATURE_TDSCDMA_RF_SUPPORT */



/*===========================================================================

FUNCTION tdsrf_get_drx_rf_state

DESCRIPTION
  Get the DRX RF state now. 

DEPENDENCIES
  None

RETURN VALUE
  tdsrf_drx_state_enum_type

SIDE EFFECTS

===========================================================================*/
tdsrf_drx_state_enum_type tdsrf_get_drx_rf_state(void);


#ifdef FEATURE_BOLT_MODEM
/*===========================================================================
FUNCTION tdsrf_get_tx_chain_no

DESCRIPTION
  Get tx chain no from RFA
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_get_tx_chain_no(void);
/*===========================================================================
FUNCTION tdsrf_cfg_wav_mk

DESCRIPTION
  Configures FW to download WAV mK
  
DEPENDENCIES
  None

RETURN VALUE
  Void 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_cfg_wav_mk(void);

#endif /*FEATURE_BOLT_MODEM*/

/*===========================================================================
FUNCTION tdsrf_allocate_rxlm_buffers_basic

DESCRIPTION
  Allocates required RxLM buffers for TD-SCDMA except LTE IRAT buffers.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_allocate_rxlm_buffers_basic(tdsl1_start_cause_enum_type  start_cause);

/*===========================================================================
FUNCTION tdsrf_allocate_rxlm_buffers_lte

DESCRIPTION
  Allocates IRAT RxLM buffers for TD-SCDMA.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_allocate_rxlm_buffers_lte( void );

/*===========================================================================
FUNCTION tdsrf_deallocate_rxlm_buffers_lte

DESCRIPTION
  De-allocates LTE IRAT RxLM buffers for TD-SCDMA.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_deallocate_rxlm_buffers_lte( void );


/*===========================================================================
FUNCTION tdsrf_allocate_txlm_buffers_basic

DESCRIPTION
  Allocates TxLM buffers.
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_allocate_txlm_buffers_basic( void );

/*===========================================================================
FUNCTION tdsrf_deallocate_rxlm_buffers

DESCRIPTION
  Deallocates all RxLM buffers.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_deallocate_rxlm_buffers( void );

/*===========================================================================
FUNCTION tdsrf_deallocate_txlm_buffers

DESCRIPTION
  deallocates all TxLM buffers.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_deallocate_txlm_buffers( void );

/*===========================================================================
FUNCTION tdsrf_send_rxd_chain_override_req

DESCRIPTION
  Ask RF to enable both chains for RxD even when TFW turns only Prx.
  This is mainly used for the 'Prx-only-both-chain-on' configured with CMAPI   

DEPENDENCIES
  RF support of the override 

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_send_rxd_chain_override_req(boolean is_rxd_override);

/*===========================================================================
FUNCTION tdsrf_save_pri_dev_id

DESCRIPTION
  save the primary device id info.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_save_pri_dev_id( rfm_device_enum_type device_id );


/*===========================================================================
FUNCTION tdsrf_clr_pri_dev_id

DESCRIPTION
  clear the primary device id info.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_clr_pri_dev_id( void );


/*===========================================================================
FUNCTION tdsrf_save_sec_dev_id

DESCRIPTION
  save the secondary device id info.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_save_sec_dev_id( rfm_device_enum_type device_id );


/*===========================================================================
FUNCTION tdsrf_clr_sec_dev_id

DESCRIPTION
  clear the secondary device id info.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_clr_sec_dev_id( void );

/*===========================================================================
FUNCTION tdsrf_get_pri_dev_id

DESCRIPTION
  get the primary device id info.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
rfm_device_enum_type tdsrf_get_pri_dev_id( void );


/*===========================================================================
FUNCTION tdsrf_get_sec_dev_id

DESCRIPTION
  get the primary device id info.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
rfm_device_enum_type tdsrf_get_sec_dev_id( void );

#ifdef FEATURE_TDSCDMA_RF_SUPPORT
extern void tdsrf_cfg_txlm_mode(  boolean enable_tx, tdsrf_tx_link_enum_type link_type );
#endif

#endif /* TDSCDMA_RF__H */

