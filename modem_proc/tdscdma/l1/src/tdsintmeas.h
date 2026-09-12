#ifndef L1INTERNALMEAS_H
#define L1INTERNALMEAS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           L1 I N T E R N A L M E A S . H                  

GENERAL DESCRIPTION
  This module handles internal measurement requests from RRC.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2010 by Qualcomm Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsintmeas.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/03/10   rc      Added function prototypes.
01/12/09   rc      Defined TDSINTERNMEA_TX_PWR_VAL_IN_CM to an invalid 
                   Tx pwr value. 
01/30/08   rc      Added a field to tdsinternmea_parm_struct_type to grow 
                   the filter from fc0 to layer3 filter coeff given in MCM.
12/26/07   rc      Added filt_len field to tdsinternmea_parm_struct_type
                   to save the mapped filter length from the filter coefficient
                   specified in the MCM.
09/19/05   cpe     Added a few externs
05/11/05   kps     lint error fixes
04/25/05    mc     Added P4 DateTime and Author header.
04/11/05   cpe     Code addition for resuming to old config upon HHO failure. 
08/24/04   cpe     Added a field to tdsinternmea_parm_struct_type to keep 
                   track of timestamp at which 6C/6D reports have been sent 
05/26/04   cpe     Added an extern
11/17/03   cpe     Some PLT related changes
10/24/03   cpe     Expanded the support for internal measurement data strctures
                   function prototypes etc.
08/18/03   cpe     Created the file

========================================================================== */
#include "tdscdma_variation.h"

#include "tdsl1rrcif.h"
#include "tdssrchset.h"
#include "tdsl1std.h"

#ifndef  FEATURE_TDSCDMA_PLT
#include "tdsrrccmd_v.h" 
#else
/*#include "wpltrrc.h"*/
#endif
              

/* Invalid Tx Pwr val for internal computation */
#define TDSINTERNMEA_INVALID_TX_PWR_VAL (0x7fff)

#define TDSINTERNMEA_INVALID_UE_TADV_VAL (0x7fff)

#define TDSINTERNMEA_INVALID_UE_RSSI_VAL (0xffff)


extern uint16 tdsinternmea_num_internal_meas_in_progress ;
extern boolean tdsinternmea_periodic_rpts_requested;
/*************************************************************************

    D A T A   S T R U C T U R E S
   
 ************************************************************************/ 
typedef struct 
{
  uint16    sfn; 
  boolean   valid;
  uint8     over_max_falg;
  uint8     under_min_flag;
  int16     txpower;
  int16     filt_txpower;
  int16     raw_totalPwr;
}tdsinternmea_txpower_parm_struct_type;

extern tdsinternmea_txpower_parm_struct_type tdsinternmea_txpower_parm_array[TDSL1_UL_MAX_NUM_TS+1];


typedef struct 
{
  uint16    sfn; 
  uint16    bitmask;
  int16     tadv[TDSL1_UL_MAX_NUM_TS+1];
}tdsinternmea_tadv_parm_struct_type;

extern  tdsinternmea_tadv_parm_struct_type tdsinternmea_tadv_parm;

  
typedef enum
{
    eIM_TTT_DISABLED,
    eIM_TTT_ENABLED,
    eIM_TTT_REPORTED
} tdsinternmea_evt_ttt_enum_type;

typedef struct 
{

    uint16 meas_id;

    tdsl1_meas_report_mode_enum_type rpt_mode;

    /* layer 3 filter_coeff  */
    tdsl1_wcdma_filter_coef_enum_type layer3_filter_coeff;
    /* filter_idx- Can be 0,1 or 2 */
    tdsl1_wcdma_filter_coef_enum_type filt_idx;

    /* Filter length to be used in "(n-1)/n" style IIR filter */
    uint16 filt_len;

    /* index used to grow the filter from fc0 to layer3_filter_coeff */
    uint16 curr_filt_len_idx; 

    /* filtered slot 15 tx_agc value of the previous frame */
    int16 previous_frame_filt_tx_val;
    
    /*  indicates if the filter for tx_agc is initialized comment**  */
    boolean tx_val_filter_initialized;

    boolean meas_quan_incl;
    /* Quantity Measured. */
    tdsl1_internal_meas_quan_type   quan_type;

    struct {
      uint16 periodic_rpt_cnt;

      uint16 ts_cnt;

      tdsl1_amount_of_reporting_enum_type num_reports_req;

      uint16 reporting_interval;
    } periodic_s;

    tdsl1_internal_meas_evt_crit_struct_type  evt_trig_s;

    tdsinternmea_evt_ttt_enum_type evt_ttt;

    uint16 ts_cnt;
    uint16 reported_ts_cnt;

    /* common to both evt trig and periodic meas reporting */
    
    boolean rpt_quan_included;

    /* 10.3.7.82 */
    tdsl1_internal_meas_rpt_quan_info_struct_type rpt_quan;

    tdsl1_meas_report_trans_mode_enum_type  rpt_trans_mode;
      /* Measuremnt Report transfer criteria to UTRAN */
                    
} tdsinternmea_parm_struct_type;

extern  tdsinternmea_parm_struct_type tdsinternmea_internal_meas_db[TDSL1_INTERNMEA_MAX_INTERNAL_MEAS];

extern  boolean tdssrchmeas_no_internal_measurements_as_ho_in_progress;

extern uint16 tdsinternmea_get_meas_idx_based_on_id(uint16 meas_id);
/*======================================================================
FUNCTION TDSINTERNMEA_RESET_INTERNAL_MEAS_DBASE

DESCRIPTION
    Resets the existing internal measurement dbase
     
RETURN VALUE
    void

=======================================================================*/
extern void tdsinternmea_reset_internal_meas_dbase(void);
/*===========================================================================

FUNCTION TDSINTERNMEA_EVALUATE_INTERNAL_MEAS_EVENTS

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  .

===========================================================================*/
extern void tdsinternmea_evaluate_internal_meas_events(void); 

/*===========================================================================

FUNCTION TDSINTERNMEA_PARM_UPDATE

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  The internal  measurement data structures may be altered.

===========================================================================*/              
extern boolean tdsinternmea_parm_update(tdsl1_meas_cmd_type *cmd);
/*===========================================================================

FUNCTION TDSINTERNMEA_SEND_PERIODIC_MEAS_REPORT

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  The internal  measurement data structures may be altered.

===========================================================================*/
extern void tdsinternmea_send_periodic_meas_report(uint16 meas_id);

#if 0
extern void tdsinternmea_reset_6f_6g_events(void);
extern void tdsinternmea_reset_6f_6g_events_for_cell (tdssrchset_cell_struct_type *cell_ptr);
#endif

/*======================================================================
FUNCTION TDSINTERNMEA_TX_PWR_FILTERING_NEEDED

DESCRIPTION
  Checks if any of events 6A, 6B, 6C, 6D, 6E, periodical reporting 
  are configured by n/w.

RETURN VALUE
    BOOLEAN

=======================================================================*/
extern boolean tdsinternmea_6a_to_6g_needed(void);


extern void tdsinternmea_init_internal_meas_values(void);



/*======================================================================
FUNCTION TDSINTERNMEA_PRINT_INTERNAL_MEAS_DB_INFO

DESCRIPTION
    Prints a snapshot of the internal meas db info
         
RETURN VALUE
    void

=======================================================================*/
void tdsinternmea_print_internal_meas_db_info(void);

/*======================================================================
FUNCTION TDSINTERNMEA_DELETE_MEAS_FROM_DB

DESCRIPTION
    Deletes the meas_idx entry from the internal measurement dbase
    In particular the following data structures are impacted
    tdsinternmea_internal_meas_db 
     
RETURN VALUE
    void
SIDE EFFECTS:
    The variable tdsinternmea_num_internal_meas_in_progress may be changed
=======================================================================*/
void tdsinternmea_delete_meas_from_db(uint16 meas_idx);

/*===========================================================================

FUNCTION tdsinternmea_validate_internal_meas_ctrl_msg

DESCRIPTION
  This function processes the internal measurement requests from RRC

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  The internal  measurement data structures may be altered.

===========================================================================*/  
boolean tdsinternmea_validate_internal_meas_ctrl_msg(
                                       tdsl1_meas_ctrl_parm_struct_type *ctrl_ptr);

/*======================================================================
FUNCTION TDSINTERNMEA_GET_EVT_TRIG_REPORT_INFO

DESCRIPTION

RETURN VALUE
    void

=======================================================================*/
void tdsinternmea_get_evt_trig_report_info( 
                tdsl1_internal_meas_rpt_crit_struct_type *cmd_ptr,
         uint16 meas_idx);

/*===========================================================================
FUNCTION        TDSINTERNMEA_SEND_MEAS_REPORT
    

DESCRIPTION     This function prepares and sends the event triggered 
                measurement report for UE internal measurements to RRC
                
                                                          
DEPENDENCIES    

RETURN VALUE    
            None
        
SIDE EFFECTS    
===========================================================================*/

void tdsinternmea_send_meas_report(uint16 meas_idx, uint8 tx_num, int16 tadv_val);


#endif
