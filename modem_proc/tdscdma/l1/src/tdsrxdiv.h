#ifndef TDSRXDIV_H
#define TDSRXDIV_H
/*============================================================================
                                  TDSL1RXDIV.H
DESCRIPTION

Copyright (c) 2012 Qualcomm Technologies, Incorporated.  All Rights Reserved.

  Qualcomm Proprietary

QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

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

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsrxdiv.h#3 $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/28/12   kguo       updated function prototype for RF/Fw Intf
07/24/2012 weijunz    RXD RDDS coding 
07/21/2012 weijunz    RxD RDDS coding 
07/19/12   kguo       updated SW/FW interface related code 
06/08/2012 Weijunz    TDS_RXD Controller coding
05/21/12   kguo       Initial Revision

==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/


/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/




/*==============================================================================

                             MACROS

==============================================================================*/





#include "comdef.h"
#include "tdsl1const.h"
#include "tdsl1m.h"
#include "tds_l1_api.h"
 
 
 
/*==============================================================================
                       STRUCTURES AND UNIONS

==============================================================================*/
/*-------------------------------------------*/
/*   RxD global structure             */
/*-------------------------------------------*/
typedef enum
{
  TDSRXD_CHAIN_0_ON = 0,
  TDSRXD_CHAIN_1_ON,
  TDSRXD_BOTH_CHAIN_ON,
  TDSRXD_CHAIN_STATUS_INVALID
}tdsrxd_rx_chain_status_type;

typedef enum 
{
    RESET,
    OBTAIN,
    START_RELEASE_NOW,
    START_RELEASE_LATER,
    RX_CONFIG_UPDATE_RSP,
    REQUEST
}div_chain_event_type;

typedef enum 
{
    AVAILABLE,
    REGISTERED,
    RELEASING,
    NONE,
    REQUESTING
}div_chain_state_type;

typedef struct
{
    div_chain_state_type    current_state;
    div_chain_state_type    next_state;
    boolean is_registered;
    //valid only when is_registered is TRUE
    uint16  start_release_time_ms;
}div_chain_info_type;

typedef union
{
    uint16 start_release_time_ms;
    tfw_rx_config_update_rsp_t* rx_config_update_rsp_pt;
}div_chain_event_data_type;

typedef enum
{
    TDSRXD_STATE_RXD_INIT ,
    TDSRXD_STATE_RXD_OFF ,
    TDSRXD_STATE_RXD_ON ,
    TDSRXD_STATE_RXD_TRANS ,
    TDSRXD_STATE_RXD_CLK_PREP1 ,
    TDSRXD_STATE_INVALID

} tdsrxd_state_type;

typedef struct
{
    boolean                   lta_started;
    boolean                   rxd_resume_complete;
    tdsrxd_state_type         rxd_state_saved;
    uint16                    rdds_cond_saved;
    uint8                     watchdog_counter;
}tdsrxdiv_lta_type;

typedef enum
{
    TDSRXD_RX_CHAIN_CONFIG_OFF = 0,
    TDSRXD_RX_CHAIN_CONFIG_CHAIN_0_ON = 1,
    TDSRXD_RX_CHAIN_CONFIG_CHAIN_1_ON = 2,
    TDSRXD_RX_CHAIN_CONFIG_CHAIN_BOTH_ON = 3
}tdsrxdiv_rx_chain_config_type;

typedef struct
{
  uint32 rflm_buf_idx;
  uint32 rflm_buf_idx_div;
  boolean is_rxd_supported;
  uint8   prev_state;
  uint8   current_state;
  uint8 fwlmStaticBufIndex;
  uint8 fwlmStaticBufIndexDiv;
  tdsrxd_rx_chain_status_type   rxdiv_enable_disable_flag;
  boolean is_rf_both_on;
  boolean is_div_only;
  boolean is_rdds_supported;
  boolean is_diag_happen;
  /*For RxD New feature*/
  boolean is_fach_rxd_supported;
  boolean is_idle_rxd_supported;
  boolean is_rxdpm_supported;
  boolean is_bch_rxd_supported;
  boolean is_acq_rxd_supported;
  boolean is_ard_rxd_supported;
  boolean is_hsdpa_call;

  boolean freeze_rxd_state_in_qta;
  boolean freeze_rxd_state_in_t2l;
  boolean freeze_rxd_state_in_t2g;  

  boolean               first_reacq_for_idle_rxd ;
  boolean               have_both_chain;
  boolean               need_reply_to_tdrm;
  boolean               is_voluntary_release;
  div_chain_info_type   div_chain_info;
  rex_timer_type        release_wait_fw_confirm_timer;       
  rex_timer_type        clk_prep1_state_timer;       
  //This is a special case configured through CMAPI.
  //--FW treats it as RxD on; L1-->RFSW-->FED to override and set both chains on.
  boolean is_tdsrxd_chain_0_both_on;
  tdsrxdiv_lta_type lta;
  tdsrxdiv_rx_chain_config_type rx_chain_config;
}tdsrxdiv_global_type;

extern boolean tdsrxdiv_rdds_cond_update ;

extern boolean tdsrxdiv_idle_turn_on_rxd;

typedef struct
{
    uint8 trch_id ;
    uint8 total_crc_num;
    uint8 crc_err_num ;
    boolean sb_reported;
    uint8 cfn ;

}tdsrxdiv_dl_trch_crc_status_type;


typedef enum
{
    
    TDSRXD_ARD_STATE_RX0 ,
    TDSRXD_ARD_STATE_RX1 ,
    TDSRXD_ARD_STATE_RX_BOTH ,
    TDSRXD_ARD_STATE_DISBALED ,
    TDSRXD_ARD_STATE_ENABLED ,
    TDSRXD_ARD_STATE_INVALID

} tdsrxd_rdds_ard_state_type ;

extern tdsrxd_rdds_ard_state_type rdds_ard_state ;

extern tdsrxdiv_global_type tdsrxdiv_global;

typedef enum
{

    TDSRXD_COND_DCH = 0 ,
    TDSRXD_COND_FON  ,
    TDSRXD_COND_BLER  ,
    TDSRXD_COND_RSCP ,
    TDSRXD_COND_SBQUAL ,
    TDSRXD_COND_SIR ,
    TDSRXD_COND_HSSCCH  ,
    TDSRXD_COND_HO ,
    TDSRXD_COND_OOS,
    TDSRXD_COND_DIAG_ENABLE_BOTH,
    TDSRXD_COND_DIAG_ENABLE_DIV,
#ifdef FEATURE_TDSCDMA_ANTENNA_SWITCH_DIVERSITY
    TDSRXD_COND_ASD,
#endif
    TDSRXD_COND_INVALID = 16
} tdsrxd_cond_type ;
/* Type definition for the callback function that will be called in response to a forced OFF
    request, when the RxD controller is ready with releasing the RF resources */
typedef void (*tdsrxd_foff_cb_type)(void);

typedef enum 
{
    TDSRXD_HO_E1G_EVENT,
    TDSRXD_HO_E2A_EVENT,
    TDSRXD_HO_E2D_EVENT,
    TDSRXD_HO_E3A_EVENT,
    TDSRXD_HO_IVALID_EVENT
}tdsrxd_ho_event_type;
 
typedef struct
{
    boolean scch_crc_pass ;
    uint8 scch_hcsn ;
    int16 scch_sir_target ;

}tdsrxdiv_scch_update ;

extern  tdsrxdiv_scch_update scch_update_to_rxdiv ;

void tdsrxdiv_set_condition(tdsrxd_cond_type cond) ;

boolean tdsrxdiv_check_condition(tdsrxd_cond_type cond) ;

void tdsrxdiv_clear_condition(tdsrxd_cond_type cond) ;

void tdsrxdiv_clear_all_conditions(void) ;
                    

void tdsrxdiv_set_ho_event(tdsrxd_ho_event_type ho_event);
void tdsrxdiv_clear_ho_event(tdsrxd_ho_event_type ho_event);

void tdsrxdiv_update_scch_crc_pass(tdsrxdiv_scch_update* scch_update);
/*===========================================================================
FUNCTION tdsrxdiv_init 

DESCRIPTION
  Initialize global and state for RxD
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_init( void  );

extern void tdsrxdiv_set_enable_disable_mask(tdsrxd_rx_chain_status_type rxdiv_on_off_flag);
//extern boolean tdsrxd_set_rx_chain_status(boolean chain0, boolean chain1);
/*===========================================================================
FUNCTION tdsrxdiv_set_state 

DESCRIPTION
  Update RxD state in RxD global
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_set_state( tdsrxd_state_type rxd_state );
void tdsrxdiv_state_update(void) ;

boolean  tdsrxdiv_is_active(void) ;

void tdsrxd_state_update_event_handler(void) ;

void tdsrxdiv_set_forced_off(boolean foff, tdsrxd_foff_cb_type cb_func);

void tdsrxdiv_configure(uint8 call_type_mask) ;
/*===========================================================================
FUNCTION tdsrxdiv_rf_turn_on_rxd

DESCRIPTION
  Turn on RF for RxD
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_rf_turn_on_rxd( uint16 freq );

/*===========================================================================
FUNCTION tdsrxdiv_fw_turn_on_rxd

DESCRIPTION
  turn on FW for RxD
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_fw_turn_on_rxd(void);

/*===========================================================================
FUNCTION tdsrxdiv_rf_turn_off_rxd

DESCRIPTION
  Turn on RF for RxD
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_rf_turn_off_rxd( void );

/*===========================================================================
FUNCTION tdsrxdiv_fw_turn_off_rxd

DESCRIPTION
  turn off FW for RxD
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_fw_turn_off_rxd(void);

/*===========================================================================
FUNCTION tdsrxdiv_dl_goto_oos

DESCRIPTION
  DL inform RxD controller that DL  goes to OOS
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_dl_goto_oos(void) ;

/*===========================================================================
FUNCTION tdsrxdiv_dl_backto_insync

DESCRIPTION
  DL inform RxD controller that DL  goes back  to in sync.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_dl_backto_insync(void) ;

/*===========================================================================
FUNCTION tdsrxdiv_dl_update_sir

DESCRIPTION
  DL inform RxD controller updated SIR value
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_dl_update_sir(int16 sir_val) ;

 /*===========================================================================
FUNCTION tdsrxdiv_dl_update_crc

DESCRIPTION
  DL inform RxD controller current CRC status
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_dl_update_crc(tdsrxdiv_dl_trch_crc_status_type dl_trch_crc_status) ;

/*===========================================================================
FUNCTION tdsrxdiv_dl_update_special_burst_qual

DESCRIPTION
  DL inform RxD controller that DL  goes to OOS
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrxdiv_dl_update_special_burst_qual(int16 sb_qual) ;

void tdsrxdiv_idle_process_after_wakeup(void);

void tdsrxdiv_idle_process_before_sleep(void);

void tdsrxdiv_idle_update_sir_rscp(void);

void tdsrxdiv_idle_update_sir(uint16 sir) ;

void tdsrxdiv_idle_update_rscp(int16 rscp);

void tdsrxdiv_idle_reset_rscp_sir_filtering(void) ;

void tdsrxdiv_idle_pch_crc_err_update(void) ;

void tdsrxdiv_send_ard_cmd(boolean ard_op,boolean reset);

void tdsrxdiv_proc_rdds_ard_ind(tfw_ant_sel_e ard_ant_sel);

void tdsrxdiv_idle_pch_turn_off_rxd(void);

extern div_chain_event_data_type dummy_div_chain_event_data;

void tdsrxdiv_set_div_chain_releasing(uint16);
void tdsrxdiv_clear_div_chain_releasing();
boolean tdsrxdiv_chain_state_machine(div_chain_event_type, div_chain_event_data_type);

/*===========================================================================
FUNCTION 
    void tdsrxdiv_release_div_chain(uint64 action_time);

DESCRIPTION
    TDRM-->RxD.
    handle TDRM's request of releasing diversity chain.

DEPENDENCIES

PARAMETERS
action_time:    ms. required time point by which diversity chain has to be released.

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsrxdiv_release_div_chain(uint64 action_time);


/*===========================================================================
FUNCTION 
    tdsrxdiv_obtained_div_chain_proc()

DESCRIPTION
    Function used by TDRM to inform RxD that TDRM has obtained diversity chain. 
    Obtaining diversity chain within the context of DR (dual-receive) implicitly means also 
    having primary chain. 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsrxdiv_obtain_div_chain(void);

/*===========================================================================
FUNCTION 
    void tdsrxdiv_request_div_chain(void)

DESCRIPTION
    TDS RxD asks TDRM to request diveristy chain.  
    (This is just the final action to TDRM)

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsrxdiv_request_div_chain(void);

/*===========================================================================
FUNCTION 
    void tdsrxdiv_complete_release(void)

DESCRIPTION
    TDS RxD info TDRM it is OK to release diversity chain.  

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsrxdiv_complete_release(void);

/*===========================================================================
FUNCTION 
    void tdsrxdiv_start_voluntary_release(void)

DESCRIPTION
    RxD volunatarily start releasing diversity chain

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsrxdiv_start_voluntary_release(void);


/*===========================================================================
FUNCTION 
    tdsrxdiv_l1mstate_need_div_chain(tdsl1m_state_enum_type l1m_state)

DESCRIPTION
    check if a l1m state needs to request diversity chain for RxD.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
boolean tdsrxdiv_l1mstate_need_div_chain(tdsl1m_state_enum_type l1m_state);

/*===========================================================================
FUNCTION 
    tdsrxdiv_release_chain_wait_fw_rsp_timeout()
DESCRIPTION
    reset div chain status to 'None', when timer waiting for FW confirmaiton has 
    expired.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tdsrxdiv_release_chain_wait_fw_rsp_timeout(uint32 unused);

/*===========================================================================
FUNCTION 
    tdsrxdiv_manage_div_chain_at_l1m_state_trans(tdsl1m_state_enum_type new_state)
DESCRIPTION
    decide eithe if need to request or release chain when entering 'new_state' 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsrxdiv_manage_div_chain_at_l1m_state_trans(tdsl1m_state_enum_type new_state);

/*===========================================================================
FUNCTION 
    tdsrxdiv_clk_prep1_state_timeout(uint32 unused)
DESCRIPTION
    turn on RxD and RxD state transits from CLK_PREP1 to ON

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tdsrxdiv_clk_prep1_state_timeout(uint32 unused);


void tdsrxdiv_lta_start(tdsl1_ext_cmd_type* cmd_ptr);
void tdsrxdiv_lta_rxd_resume_complete(void);

/*===========================================================================
FUNCTION 
    void tdsrxdiv_chain_state_reset_no_chain_request(void)
DESCRIPTION
    1. reset div chain state machine;
    2. no further request for div chain.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tdsrxdiv_chain_state_reset_no_chain_request(void);

/*===========================================================================
FUNCTION 
    void tdsrxdiv_place_rxd_cmd_to_l1m_local_cmd_queue(tdsl1def_l1_local_cmd_enum_type rxd_cmd_name)
DESCRIPTION
    place cmd from RxD module to l1m local cmd queue

INPUTS: 
    a. TDSRXD_TURN_OFF_RXD_CMD:
        1. 'turn off' RxD
                --'turn off' here specifically means reset RxD div chain state to 'None' and
                do not request div chain after reset. 
        2. convert 'turn off' action into l1m context(thread)
        3. ***NOTE***: this is special case handling. This function is NOT intended to be used as a
            conventional function call to turn off RxD. 
    b. TDSRXD_RELEASE_DIV_CHAIN_NOW_CMD
        release div chain 'now'
 
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsrxdiv_place_rxd_cmd_to_l1m_local_cmd_queue(tdsl1def_l1_local_cmd_enum_type rxd_cmd_name);

#endif /* TDSRXDIV__H */
