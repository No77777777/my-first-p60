/*!
   @file rfgsm_core_log.c

   @brief

   @details

*/


/*===========================================================================
Copyright (c) 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/rf/core/src/rfgsm_core_log.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/02/20   mpa     To support MiniDump Feature
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
05/07/15   sc      Implement command processing log
08/14/14   zbz     Add SMPS/ICQ/temp_rem to FTM log packet 
07/22/13   ec	   Use new F3s/Null pointer checks
05/28/13   ak      Featurize/Fix G+W only build compiler warnings
03/18/13   svi     Replaced memcpy() by safer memscpy(). 
11/26/12   aka     Modified rfgsm_core_log_send_to_ftm_task to use rfgsm handle
02/22/12   shb     Fixed offtarget compiler error due to stray ";"
02/10/12   sb      Changes to support FTM logging for GSM
============================================================================*/

#include "queue.h"
#include "target.h"
#include "msg.h"
#include "ftm.h"
#include "rf_test_task_v.h"
#include "ftm_log.h"
#include "rfgsm_core_log.h"
#include "ftm_task.h"
#include "rfgsm_core.h"
#include "log_codes.h"
#include "stringl.h"



/*===========================================================================

                                DEFINITIONS

=============================================================================*/
// Declare a queue
static rfgsm_log_info_type  rfgsm_core_log_status;



/*!==========================================================================*/
/*! @var rfgsm_cmd_proc_log                                                  */
/*! @brief variable definition for command processing log                    */
rfgsm_cmd_proc_log_entry_type rfgsm_cmd_proc_log[RFGSM_CMD_PROC_LOG_NUM_ENTRIES];



/*!==========================================================================*/
/*! @var rfgsm_cmd_proc_log_entry                                            */
/*! @brief variable definition for indexing into the command processing log  */
uint32 rfgsm_cmd_proc_log_entry = 0;



/*===========================================================================

                           FUNCTION PROTOTYPES

=============================================================================*/
extern uint32 gstmr_rd_qsymbol_count( void );


static void rfgsm_core_log_free_buffer( void *msg);

#ifdef FEATURE_QSH_MDUMP 
#include "qsh.h"
#include "ftm_gsm_rftest.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_gsm_device.h"
#include "rflm_gsm_dm.h"
#include "rfc_card_gsm.h"
#include "ftm_common_control.h"
#include "ftm_gsm_rftest.h"
#include "rfm_device_types.h"
#include "rflm_api_gsm.h"
#endif


/*===========================================================================

                                  FUNCTIONS

=============================================================================*/
#ifdef FTM_HAS_LOGGING
// This function is called from rf_test_task.c  (FTM task) to initialize the resources for queue

void rfgsm_core_log_q_init( void)
{
   uint8    buf_count;
   //The specified queue is initialized
  (void)q_init( &rfgsm_core_log_status.rfgsm_log_q);
  (void)q_init( &rfgsm_core_log_status.rfgsm_log_free_q);

  for ( buf_count = 0; buf_count < RFGSM_LOG_BUFFERS; buf_count++)
  {

    (void)q_link( (void *) &rfgsm_core_log_status.rfgsm_buffers[buf_count],
                  &(rfgsm_core_log_status.rfgsm_buffers[buf_count].link) );

    q_put( &rfgsm_core_log_status.rfgsm_log_free_q,
           &(rfgsm_core_log_status.rfgsm_buffers[buf_count].link)) ;

  }
  
}

// This function "Allocates" a buffer for RFGSM to send messages to FTM.

void *rfgsm_core_log_get_buf( void)
{

  /* Get a buffer from the Free Q */
  void *msg = NULL;

  msg = q_get( &rfgsm_core_log_status.rfgsm_log_free_q);

  return( msg);

}


void rfgsm_core_log_send_to_ftm_task(rfm_device_enum_type rfm_dev)
{
  //uint32 start_time = gstmr_rd_qsymbol_count();
  //uint32 end_time = 0;
  rfgsm_core_log_data *message = NULL;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;

  message = ( rfgsm_core_log_data *) rfgsm_core_log_get_buf();	

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  RF_NULL_CHECK_RTN_V(rfgsm_core_handle_ptr,"rfgsm_core_handle_ptr is NULL!");

  /* Specify FTM log packet version number */
  rfgsm_core_handle_ptr->tx_log.ver_num = 2;

  if( message != NULL )
  {
    //copy the populated log message into the buffer item of queue
    memscpy( &(message->msg), sizeof(message->msg), &(rfgsm_core_handle_ptr->tx_log), sizeof(rfgsm_core_tx_log_type));
    (void)q_link( ( void *) message, &message->link);
    q_put( &rfgsm_core_log_status.rfgsm_log_q, &message->link );
    (void)rex_set_sigs( &ftm_tcb, FTM_GSM_Q_SIG);
  }

  //end_time = gstmr_rd_qsymbol_count();
  //MSG_1(MSG_SSID_FTM, MSG_LEGACY_HIGH,"ftm_rfgsm_send_data_to_ftm() took %d qs", (end_time - start_time));
}




// This function dequeues an data block containing RFGSM - FTM task info, populates the RFGSM Log packet and sends the log packet to diag
void rfgsm_core_log_handle_q_from_rf()
{

    rfgsm_core_log_type *gsm_log = NULL;

    rfgsm_core_log_data  *message = NULL;

    //uint32 start_time = gstmr_rd_qsymbol_count();
    //uint32 end_time = 0;    
    while ( (message = q_get( &rfgsm_core_log_status.rfgsm_log_q)) != NULL)
    {
        gsm_log = (rfgsm_core_log_type *)ftm_log_malloc( (uint16)FTM_LOG_GSM_ENH, sizeof (rfgsm_core_log_type));

        if ( gsm_log != NULL )
        { 
            //------------------------------dequeue item from the the queue, latest q item retrieved---------//
            memscpy ( &(gsm_log->data), sizeof(gsm_log->data), &(message->msg), sizeof(rfgsm_core_tx_log_type));

            log_commit( ( ftm_log_type*) gsm_log );

        }
       
        rfgsm_core_log_free_buffer(( void *)message);
        
    }

    //end_time = gstmr_rd_qsymbol_count();
    //MSG_1(MSG_SSID_FTM, MSG_LEGACY_HIGH,"rfgsm_handle_logs_from_rf() took %d qs", (end_time - start_time));

   
}

static void rfgsm_core_log_free_buffer( void *msg)
{

  /* Return a buffer to the free Q for future use */

  if( msg != NULL )
  {
      rfgsm_core_log_data  *message = ( rfgsm_core_log_data *) msg;
    
      (void)q_link( ( void *) message, &message->link);
    
      q_put( &rfgsm_core_log_status.rfgsm_log_free_q, &message->link);
  }

}
#endif /* FTM_HAS_LOGGING */

#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR void rf_gsm_QSH_mdump_cb (  void )
{
  rfm_device_enum_type device = 0;
  ftm_gsm_chains_enum_type ftm_gsm_chain_idx= FTM_GSM_C0;
  ftm_rfstate_enum_type ftm_rfstate = FTM_STATE_UNKNOWN;
  boolean is_gsm_ftm_mode = FALSE;

  rflm_gsm_data_type* rfgsm_dm_ptr = NULL;
  rflm_gsm_subscription_id  sub_id = 0;
  boolean is_rfgsm_active = FALSE;

  for ( sub_id = RFLM_GSM_SUBSCRIPTION_1; sub_id < RFLM_GSM_MAX_SUBSCRIPTIONS; sub_id ++)
  {
    if (rflm_gsm_dm_get_table_ptr( sub_id) != NULL)
    {
      is_rfgsm_active = TRUE;
      break;
    }
  }

  if ( is_rfgsm_active == TRUE)
  {
    qsh_mdump_collect_high((void*)rfgsm_core_handle_get_address(), 4 * RFM_MAX_WAN_DEVICES );
    qsh_mdump_collect_high((void*)rflm_gsm_dm_get_table_ptr_address(), 4 * RFLM_GSM_MAX_SUBSCRIPTIONS);

    for(device = 0; device < RFM_MAX_WAN_DEVICES; device++)
    {
      if(rfgsm_core_handle_get(device) != NULL)
      {
        qsh_mdump_collect_high((void*)rfgsm_core_handle_get(device), sizeof(rfgsm_core_handle_type) );
        qsh_mdump_collect_high((void*)rfgsm_core_handle_get(device)->rfc_gsm_core_ptr, sizeof(rfc_gsm_core_config_type) );
        qsh_mdump_collect_high((void*)rfgsm_core_handle_get(device)->rfgsm_tx_nv_tbl, sizeof(rfgsm_nv_band_tx_data_type) );
      }

    }

    for ( sub_id = RFLM_GSM_SUBSCRIPTION_1; sub_id < RFLM_GSM_MAX_SUBSCRIPTIONS; sub_id ++)
    {
      if (rflm_gsm_dm_get_table_ptr( sub_id) != NULL)
      {
        qsh_mdump_collect_high((void*)rflm_gsm_dm_get_table_ptr( sub_id) ,sizeof(rflm_gsm_data_type));
        qsh_mdump_collect_high((void*)rflm_gsm_dm_get_table_ptr( sub_id)->rflm_tuner_intf,sizeof(rflm_cmn_tuner_event_data_type));
        qsh_mdump_collect_high((void*)rflm_gsm_dm_get_table_ptr( sub_id)->tuner_aol_rx_status_ptr,sizeof(rflm_ccs_rf_event_status_type));
        qsh_mdump_collect_high((void*)rflm_gsm_dm_get_table_ptr( sub_id)->tuner_aol_tx_status_ptr,sizeof(rflm_ccs_rf_event_status_type));
        qsh_mdump_collect_high((void*)rflm_gsm_dm_get_table_ptr( sub_id)->tuner_cl_status_ptr,sizeof(rflm_cmn_tuner_event_status_type));
      }
    }

    qsh_mdump_collect_high(rfc_gsm_get_address(), 4);
    qsh_mdump_collect_high(rfc_gsm_ptr_get_address(),rfc_gsm_ptr_get_size() );

    qsh_mdump_collect_high((void*)&rfgsm_cmd_proc_log, sizeof (rfgsm_cmd_proc_log));
    qsh_mdump_collect_high((void*)&rfgsm_cmd_proc_log_entry, sizeof(uint32));
    qsh_mdump_collect_high((void*)rfgsm_core_get_intf_data_ptr_address(),4);
    qsh_mdump_collect_high((void*)rfgsm_core_get_intf_data_ptr_address(),sizeof(rflm_gsm_intf));
  }

  for (device = 0; device < RFM_MAX_WAN_DEVICES; device ++)
  {
    ftm_rfstate = ftm_get_current_state(device);

    if ( (ftm_rfstate == FTM_STATE_GSM) || (ftm_rfstate == FTM_STATE_GSM_NS))
    {
      is_gsm_ftm_mode = TRUE;
      break;
    }
  }

  if ( is_gsm_ftm_mode == TRUE)
  {
    /* If in FTM mode */
    qsh_mdump_collect_high((void*)ftm_gsm_get_chain_vars_ptr_address(), 4 * FTM_GSM_MAX_CHAINS);
    for(ftm_gsm_chain_idx = FTM_GSM_C0; ftm_gsm_chain_idx < FTM_GSM_MAX_CHAINS;ftm_gsm_chain_idx++)
    {
      if(ftm_gsm_get_chain_vars_ptr(ftm_gsm_chain_idx) != NULL)
      {
        qsh_mdump_collect_high((void*)ftm_gsm_get_chain_vars_ptr(ftm_gsm_chain_idx), sizeof(ftm_gsm_chain_vars_s));
        qsh_mdump_collect_high((void*)ftm_gsm_get_chain_vars_ptr(ftm_gsm_chain_idx)->cmn_vars_ptr, sizeof(ftm_gsm_cmn_vars_s));
        qsh_mdump_collect_high((void*)ftm_gsm_get_chain_vars_ptr(ftm_gsm_chain_idx)->rx_vars_ptr, sizeof(ftm_gsm_rfctl_vars_s));
        qsh_mdump_collect_high((void*)ftm_gsm_get_chain_vars_ptr(ftm_gsm_chain_idx)->tx_vars_ptr, sizeof(ftm_gsm_rfctl_vars_s));
  
      }
    }
  
 

    qsh_mdump_collect_high((void*)ftm_gsm_get_rfctrl_vars_address(), 4 * RFM_MAX_WAN_DEVICES);
    for(device = 0; device < RFM_MAX_WAN_DEVICES; device++)
    {
      if(ftm_gsm_get_rfctrl_vars(device) != NULL)
      {
        qsh_mdump_collect_high((void*)ftm_gsm_get_rfctrl_vars(device), sizeof(ftm_gsm_rfctl_vars_s));
      }
    }
  
  #ifdef FEATURE_FACTORY_TESTMODE   
  #ifdef FTM_HAS_UMTS
  #ifdef FEATURE_GSM
    if ( ftm_gsm_rftest_get_rftest_enabled())
    {
      qsh_mdump_collect_high((void*)ftm_gsm_get_rftest_globals_address(), sizeof(ftm_gsm_rftest_global_t));
    }
  #endif
  #endif
  #endif
  }

} /* rf_gsm_QSH_mdump_cb */
#endif
