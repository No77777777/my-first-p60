#ifndef L1INTERNALMEAS_H
#define L1INTERNALMEAS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           L1 I N T E R N A L M E A S . H                  

GENERAL DESCRIPTION
  This module handles internal measurement requests from RRC.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2010 by Qualcomm Technologies, Inc. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/l1internalmeas.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
12/11/19   ac      Searcher Mini Dump Changes
01/08/18   hs      Doubling of internal measurement database and and allocationg a portion of its memory through heap 
06/09/15   jk      Adding code to change 6D entering and exiting threshold via QXDM commands.
02/25/15   vr      Support for 0x4035 Slot level TXAGC log packet.
01/30/15   jk      Fixed compiler warnings.
01/20/15   jd      Global re-org for W + W
10/20/14   jk      FR 22545: SW enhancement to skip event 6D reporting - L1 Searcher Changes.
12/06/13   jk      Fixed issue where excessive triggering was seen on poor RF conditions.
05/03/10   rc      Added function prototypes.
01/12/09   rc      Defined L1_INTERNALMEAS_TX_PWR_VAL_IN_CM to an invalid 
                   Tx pwr value. 
01/30/08   rc      Added a field to l1_internal_meas_parm_struct_type to grow 
                   the filter from fc0 to layer3 filter coeff given in MCM.
12/26/07   rc      Added filt_len field to l1_internal_meas_parm_struct_type
                   to save the mapped filter length from the filter coefficient
                   specified in the MCM.
09/19/05   cpe     Added a few externs
05/11/05   kps     lint error fixes
04/25/05    mc     Added P4 DateTime and Author header.
04/11/05   cpe     Code addition for resuming to old config upon HHO failure. 
08/24/04   cpe     Added a field to l1_internal_meas_parm_struct_type to keep 
                   track of timestamp at which 6C/6D reports have been sent 
05/26/04   cpe     Added an extern
11/17/03   cpe     Some PLT related changes
10/24/03   cpe     Expanded the support for internal measurement data strctures
                   function prototypes etc.
08/18/03   cpe     Created the file

========================================================================== */
#include "wcdma_variation.h"
#include "l1rrcif.h"
#include "srchset.h"

#ifndef FEATURE_WCDMA_PLT
#include "rrccmd_v.h" 
#else
#error code not present
#endif


              
#define L1_MAX_INTERNAL_MEAS 8

/* TxAGC value for a CM slot should be an invalid value since there
   is no Tx during that slot. Setting to 50dBm, since this is an
   invalid TxAGC value. This value is used only to distinguis from 
   a valid Tx Pwr value.*/
#define L1_INTERNALMEAS_TX_PWR_VAL_IN_CM 766

#define IS_L1_INTERNALMEAS_IN_CM_SLOT(tx_pwr) ((tx_pwr == L1_INTERNALMEAS_TX_PWR_VAL_IN_CM) \
											  || (tx_pwr == (2 * L1_INTERNALMEAS_TX_PWR_VAL_IN_CM))) \
												? TRUE:FALSE



/*************************************************************************

    D A T A   S T R U C T U R E S
   
 ************************************************************************/ 
  
typedef enum
{
    eIM_TTT_DISABLED,
    eIM_TTT_ENABLED,
    eIM_TTT_REPORTED
} l1_internal_meas_evt_ttt_enum_type;

typedef struct 
{

    uint16 meas_id;

    l1_meas_report_mode_enum_type rpt_mode;

    /* layer 3 filter_coeff  */
    l1_wcdma_filter_coef_enum_type layer3_filter_coeff;
    /* filter_idx- Can be 0,1 or 2 */
    l1_wcdma_filter_coef_enum_type filt_idx;

    /* Filter length to be used in "(n-1)/n" style IIR filter */
    uint16 filt_len;

    /* index used to grow the filter from fc0 to layer3_filter_coeff */
    uint16 curr_filt_len_idx; 

    /* filtered slot 15 tx_agc value of the previous frame */
    int32 previous_frame_filt_tx_val;

    /*  indicates if the filter for tx_agc is initialized comment**  */
    boolean tx_val_filter_initialized;

    boolean meas_quan_incl;
    /* Quantity Measured. */
    l1_internal_meas_quan_type   quan_type;

    struct {
      uint16 periodic_rpt_cnt;

      uint16 ts_cnt;

      l1_amount_of_reporting_enum_type num_reports_req;

      uint16 reporting_interval;
    } periodic_s;

    l1_internal_meas_evt_crit_struct_type  evt_trig_s;

    l1_internal_meas_evt_ttt_enum_type evt_ttt;

    uint16 ts_cnt;
    uint16 reported_ts_cnt;

    /* common to both evt trig and periodic meas reporting */
    
    boolean rpt_quan_included;

    /* 10.3.7.82 */
    l1_internal_meas_rpt_quan_info_struct_type rpt_quan;

    l1_meas_report_trans_mode_enum_type  rpt_trans_mode;
      /* Measuremnt Report transfer criteria to UTRAN */
                    
} l1_internal_meas_parm_struct_type;

typedef struct 
{
  boolean event_6d_triggered;
  boolean event_6d_elapsed_half_ttt;
  boolean event_6d_expired;
} l1_mac_6d_iface_struct_type;

typedef struct
{
  boolean l1_internal_periodic_rpts_requested;
  uint16 l1_num_internal_meas_in_progress;
   boolean l1_internal_heap_allocated;

}l1internalmeas_ext_cntrl_params_mdump_type;


typedef struct
{
  l1_internal_meas_parm_struct_type *l1_internal_meas_db[L1_MAX_INTERNAL_MEAS];
  uint8 debug_ul_6d_enter_thresh;
  uint8 debug_ul_6d_exit_thresh;

    /*Mini Dump: Variables that need to be dumped moved to this structure*/
  l1internalmeas_ext_cntrl_params_mdump_type l1internalmeas_ext_cntrl_params_mdump;
}l1internalmeas_ext_cntrl_params_type;

extern l1internalmeas_ext_cntrl_params_type l1internalmeas_ext_cntrl_params[WCDMA_NUM_SUBS];


typedef struct
{
  /* global array to store the filtered values per slot in a frame */
  int32 filt_tx_val_array[SLOTS_PER_FRAME];
}l1internalmeas_int_cntrl_params_mdump_type;

typedef struct
{
  uint8 event6x_configured_bmsk;
  int16 l1_internalmeas_tx_pwr_val;
  l1_mac_6d_iface_struct_type l1_mac_iface_vars;

  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  l1internalmeas_int_cntrl_params_mdump_type l1internalmeas_int_cntrl_params_mdump;
}l1internalmeas_int_cntrl_params_type;



/*======================================================================
FUNCTION l1_internalmeas_get_meas_idx_based_on_id

DESCRIPTION
    Returns the meas index into the measurements DB table based on
    the measurement id
RETURN VALUE
    uint16

=======================================================================*/
extern uint16 l1_internalmeas_get_meas_idx_based_on_id(uint16 meas_id,wsub_id_e_type wsub_id);
/*======================================================================
FUNCTION l1_internalmeas_get_meas_idx_based_on_event_id

DESCRIPTION
    Returns the meas index into the measurements DB table based on
    the event id
RETURN VALUE
    uint16

=======================================================================*/
extern uint16 l1_internalmeas_get_meas_idx_based_on_event_id(l1_internal_meas_event_enum_type evt_id,wsub_id_e_type wsub_id);
/*======================================================================
FUNCTION L1_INTERNALMEAS_RESET_INTERNAL_MEAS_DBASE

DESCRIPTION
    Resets the existing internal measurement dbase
     
RETURN VALUE
    void

=======================================================================*/
extern void l1_internalmeas_reset_internal_meas_dbase( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION L1_INTERNALMEAS_EVALUATE_INTERNAL_MEAS_EVENTS

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  .

===========================================================================*/
extern void l1_internalmeas_evaluate_internal_meas_events( wsub_id_e_type wsub_id); 

/*===========================================================================

FUNCTION L1_INTERNAL_MEAS_PARM_UPDATE

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  The internal  measurement data structures may be altered.

===========================================================================*/              
extern boolean l1_internal_meas_parm_update(l1_meas_cmd_type *cmd, wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION L1_INTERNALMEAS_SEND_PERIODIC_MEAS_REPORT

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  The internal  measurement data structures may be altered.

===========================================================================*/
extern void l1_internalmeas_send_periodic_meas_report(uint16 meas_id, wsub_id_e_type wsub_id);


extern void l1_internalmeas_evaluate_internal_meas_events_6f_6g( wsub_id_e_type wsub_id);
extern void l1internalmeas_reset_6f_6g_events( wsub_id_e_type wsub_id);
extern void l1internalmeas_reset_6f_6g_events_for_cell (srch_cell_struct_type *cell_ptr);


/*======================================================================
FUNCTION L1_INTERNALMEAS_TX_PWR_FILTERING_NEEDED

DESCRIPTION
  Checks if any of events 6A, 6B, 6C, 6D, 6E, periodical reporting 
  are configured by n/w.

RETURN VALUE
    BOOLEAN

=======================================================================*/
extern boolean l1_internalmeas_tx_pwr_filtering_needed( wsub_id_e_type wsub_id);


/*======================================================================
FUNCTION L1_MEAS_PRINT_INTERNAL_MEAS_DB_INFO

DESCRIPTION
    Prints a snapshot of the internal meas db info
         
RETURN VALUE
    void

=======================================================================*/
void l1_meas_print_internal_meas_db_info( wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION L1_INTERNALMEAS_DELETE_MEAS_FROM_DB

DESCRIPTION
    Deletes the meas_idx entry from the internal measurement dbase
    In particular the following data structures are impacted
    l1internalmeas_ext_cntrl_params.l1_internal_meas_db 
     
RETURN VALUE
    void
SIDE EFFECTS:
    The variable l1internalmeas_ext_cntrl_params.l1_num_internal_meas_in_progress may be changed
=======================================================================*/
void l1_internalmeas_delete_meas_from_db(uint16 meas_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION L1_INTERNAL_MEAS_PARM_UPDATE

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  The internal  measurement data structures may be altered.

===========================================================================*/  
boolean l1meas_validate_internal_meas_ctrl_msg(
                                       l1_meas_ctrl_parm_struct_type *ctrl_ptr, wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION L1_INTERNALMEAS_GET_EVT_TRIG_REPORT_INFO

DESCRIPTION

RETURN VALUE
    void

=======================================================================*/
void l1_internalmeas_get_evt_trig_report_info( 
                l1_internal_meas_rpt_crit_struct_type *cmd_ptr,
         uint16 meas_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1_INTERNALMEAS_SEND_MEAS_REPORT
    

DESCRIPTION     This function prepares and sends the event triggered 
                measurement report for UE internal measurements to RRC
                
                                                          
DEPENDENCIES    

RETURN VALUE    
            None
        
SIDE EFFECTS    
===========================================================================*/
void l1_internalmeas_send_meas_report(uint16 meas_idx, int16 meas_val, 
                                              srch_cell_struct_type *cell_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        l1internal_meas_rxtx_diff_required
    

DESCRIPTION     
                                                          
DEPENDENCIES    

RETURN VALUE    
            None
        
SIDE EFFECTS    
===========================================================================*/
boolean l1internal_meas_rxtx_diff_required(void);

/*===========================================================================

FUNCTION L1_INTERNALMEAS_CALCULATE_TX_TIME

DESCRIPTION
  This function computes the tx sys time 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  

===========================================================================*/
void l1internalmeas_calculate_rx_tx_diff( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1_INTERNALMEAS_COMPUTE_FILTERED_TX_PWR
    

DESCRIPTION     This function filers the 15 tx pwr values measured per slot.
                
DEPENDENCIES    

RETURN VALUE    
            int16 filtered tx_pwr value
        
SIDE EFFECTS    
===========================================================================*/

LOCAL void l1_internalmeas_compute_filtered_tx_pwr(uint16 meas_idx, 
                                                   int16 * l1_tx_pwr_val_per_slot_array, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     L1INTERNALMEAS_UPDATE_6D_ELAPSED_HALF_TTT_FLAG

DESCRIPTION
  This function updates the flags that will be used by MAC layer to prevent MTPL cycles.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static inline void l1internalmeas_update_6d_elapsed_half_ttt_flag(uint16 ts_cnt, 
                                   uint16 ttt, l1_internal_meas_evt_ttt_enum_type evt_ttt, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     L1INTERNALMEAS_RESET_6D_ELAPSED_FLAGS

DESCRIPTION
  This function resets the flags that will be used by MAC layer to prevent MTPL cycles.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static inline void l1internalmeas_reset_6d_elapsed_flags();

/*===========================================================================
FUNCTION l1internalmeas_int_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize exclusive variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void l1internalmeas_int_cntrl_params_init( void );

/*===========================================================================
FUNCTION        is_6x_logpkt_bmsk_set
    

DESCRIPTION     This function filers the 15 tx pwr values measured per slot.
                
DEPENDENCIES    

RETURN VALUE    int16 filtered tx_pwr value

SIDE EFFECTS    
===========================================================================*/
extern boolean is_6x_logpkt_bmsk_set( uint8 i);

/*===========================================================================
FUNCTION l1internalmeas_ext_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize exclusive variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
void l1internalmeas_ext_cntrl_params_init( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION l1internalmeas_alloc_heapmem

DESCRIPTION
    This function is used to allocate heap memory ext_l1internalmeas_cntrl->l1_internal_meas_db

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
void l1internalmeas_alloc_heapmem( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION l1internalmeas_dealloc_heapmem

DESCRIPTION
    This function is used to deallocate heap memory ext_l1internalmeas_cntrl->l1_internal_meas_db

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/

void l1internalmeas_dealloc_heapmem( wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION     l1internalmeas_qsh_mdump_collect

  DESCRIPTION
  This function dumps all the required parameters from l1intenalmeas internal and 
  external control parameter structures.

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/

extern QSH_MDUMP_FN_ATTR void l1internalmeas_qsh_mdump_collect(wsub_id_e_type wsub_id);
#endif

#endif
