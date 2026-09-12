/*! 
  @file
  rfcommon_qsh_client.c
 
  @brief
  RFA Common client for QSH
 
  @addtogroup RF_COMMON
  @{
*/

/*==============================================================================

  Copyright (c) 2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/rf/mc/src/rfcommon_qsh_client.c#1 $

when       who     what, where, why
--------      ---     ------------------------------------------------------------- 
01/20/20   moh     QSH MDUMP support 
01/16/20   moh     Initial Release - RF Client for QSH 

==============================================================================*/

#include "qsh.h" 
#include "rfcommon_qsh_client.h"
#include "modem_mem.h"
#include "msg.h"
#include "appmgr.h"
#include "err.h"
#include "stringl.h" /* for memscpy */
#include "ftm.h"
#include "rfm.h"

#ifdef FEATURE_QSH_MDUMP 
/*! @brief Stores function ptr to tech's qsh callback */
static rftech_qsh_mdump_fp_type rf_qsh_mdump_func_tbl[RFCOM_NUM_MODES] = {NULL}; 
/*! @brief API from CCS to dump variable */
extern void rflm_ccs_qsh_mdump(void);
extern void ftm_qsh_mdump(void);
extern boolean ftm_get_mode(void);
extern void rflm_ccs_mmu_qsh_mdump(void);
extern void rfm_qsh_dump_data(void);

/*----------------------------------------------------------------------------
******** Method definitions - QSH MDUMP Callback Register ********
----------------------------------------------------------------------------*/
/*! 
  @brief
  This API registers the mdump qsh callback of all rf tech.
 
  @details
  This API is used for registering the tech callback at the time of their init.
  At the time of crash, registered callback are called to dump global variables. 
  
  @params rftech_qsh_mdump_fp_type
  function ptr of registered callback.
  
  @return boolean
  TRUE / FALSE
*/
boolean rf_qsh_cb_register(rfcom_mode_enum_type tech, rftech_qsh_mdump_fp_type func_ptr)
{
  boolean status = FALSE;

  if (tech >= RFCOM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid tech (%d) during QSH API registeration!", tech);
    return status;
  }

  if (func_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"func_ptr == NULL during QSH API registeration!", 0);
    return status;
  }

  rf_qsh_mdump_func_tbl[tech] = func_ptr;
  status = TRUE;

  return (status);
}


/*----------------------------------------------------------------------------
******** Method definitions - COMMON QSH MDUMP CALLBACK ********
----------------------------------------------------------------------------*/
/*! 
  @brief
  Common QSH MDUMP callback.
 
  @details
  It provides mdump of common RF global variables.
  
  @param void
 
  @return void
*/
QSH_MDUMP_FN_ATTR
void rf_common_QSH_mdump_cb()
{
  /*Dumping CCS global variables*/
  rflm_ccs_qsh_mdump();

  /* MMU variables*/
  rflm_ccs_mmu_qsh_mdump();

  rfm_qsh_dump_data();
	
  if( ftm_get_mode() == FTM_MODE)
  {	
    ftm_qsh_mdump();
  }
	
}

/*----------------------------------------------------------------------------
******** Method definitions - QSH MDUMP DATA COLLECT ********
----------------------------------------------------------------------------*/
/*! 
  @brief
  This is the RFA_COMMON MDUMP collect function
 
  @details
  Collects the global variables using QSH framework
  Used by RF QSH Client callback.
  
  @param void
  
  @return QSH_MDUMP_FN_ATTR
*/
QSH_MDUMP_FN_ATTR
void rf_qsh_collect_mdump_data()
{
  // Dump RF variable only if rfm_init is passed
  if(rfm_init_get_state_mdump() == RFM_INIT_SUCCEEDED)
  {
    rfcom_mode_enum_type tech;

    /*Collect the RF common global variable*/
    rf_common_QSH_mdump_cb();
	
    /*Collect the respective TECH's global variable*/
    for(tech = RFM_PARKED_MODE; tech<RFM_NUM_MODES ; tech++)
    {
      if (rf_qsh_mdump_func_tbl[tech] != NULL)
      {
        (*rf_qsh_mdump_func_tbl[tech])();
      }
    }
  }
}

/*----------------------------------------------------------------------------
******** Method definitions - QSH CLIENT DEREGISTER ********
----------------------------------------------------------------------------*/
/*! 
  @brief
  This is the RFA_COMMON client deregister function
 
  @details
  RF QSH client will call this for deregistering with QSH framework.
  Used by rfm_deinit
  
  @param void
 
  @return void
*/
void rf_qsh_client_deregister()
{
  qsh_client_reg_s qsh_reg_param;
  
  qsh_client_reg_init(&qsh_reg_param);
  qsh_reg_param.client = QSH_CLT_RF;
  qsh_reg_param.major_ver = QSH_CLT_RF_MAJOR_VER;
  qsh_reg_param.minor_ver = QSH_CLT_RF_MINOR_VER;
  qsh_client_reg(&qsh_reg_param);
  
}

/*! 
  @brief
  This is the RFA_COMMON client's QSH callback API
 
  @details
  QSH will call this API for any action (metric/event/dump) pertaining to RFA_COMMON client.
  This API dispatches MSG to RF Apps task to handover the callback processing to RF context.
  
  @param *param
  QSH client callback param pointer
 
  @return void
*/
QSH_MDUMP_FN_ATTR
void rf_qsh_callback_send_msgr(qsh_client_cb_params_s *param)
{
   rfm_rf_cmn_qsh_callback_ind_s qsh_callback_ind_msg;
   errno_enum_type msgr_ret_status;
   qsh_client_cb_params_s *qsh_client_cb_params_ptr = NULL;


   /* based on action as QSH_ACTION_MDUMP, call the qsh collect mdump */   
   // Add here
   if(QSH_ACTION_MDUMP == param->action)
   {
     rf_qsh_collect_mdump_data();
     return;
   }   

   // allocate memory on heap to store callback params.
   qsh_client_cb_params_ptr = (qsh_client_cb_params_s *)modem_mem_alloc(sizeof(qsh_client_cb_params_s), MODEM_MEM_CLIENT_RFA);
   if (qsh_client_cb_params_ptr != NULL)
   {
      // initialize qsh_callback_ind_msg to 0s
      memset((void *)&qsh_callback_ind_msg, 0, sizeof(rfm_rf_cmn_qsh_callback_ind_s));

      // copy callback params to heap
      memscpy(qsh_client_cb_params_ptr,
              sizeof(qsh_client_cb_params_s),
              param,
              sizeof(qsh_client_cb_params_s));

      msgr_init_hdr(&(qsh_callback_ind_msg.hdr),
                    MSGR_RFA_RF_COMMON,
                    RFA_RF_COMMON_MSG_ID_QSH_CALLBACK_IND);

      /*Fill the required params */
      qsh_callback_ind_msg.payload_ptr  = (void *)qsh_client_cb_params_ptr;

      /* Send the MSGR */
      msgr_ret_status = msgr_send(&(qsh_callback_ind_msg.hdr),
                                  sizeof(rfm_rf_cmn_qsh_callback_ind_s));

      if (msgr_ret_status != E_SUCCESS)
      {
         RF_MSG_3(
            RF_ERROR,
            "rf_qsh_callback_send_msgr cannot send "
            "RFA_RF_COMMON_MSG_ID_QSH_CALLBACK_IND sent to RF APPS TASK: "
            "Error:%d, Payload size:%d, Max:%d",
            msgr_ret_status,
            sizeof(rfm_rf_cmn_qsh_callback_ind_s),
            RF_APPS_MAX_CMD_PAYLOAD_SIZE);

         // free memory on heap that stored callback params.
         modem_mem_free(qsh_client_cb_params_ptr, MODEM_MEM_CLIENT_RFA);
      }
   } // if modem_mem_alloc allocated memory
   else
   {
      RF_MSG(
         RF_ERROR,
         "rf_qsh_callback_send_msgr: "
         "Failed to allocate memory to buffer call back params.");
   }
   

}


/*! 
  @brief
  This is the RFA_COMMON QSH clilent's registration API.
 
  @details
  This API will register the RFA_COMMON QSH client for the subscriber ID specified.
 
  @return boolean
  TRUE indicates client requested to register with QSH, else FALSE
*/
void rf_qsh_client_register(void)
{
   qsh_client_reg_s rf_client_reg;

   sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;

   // Initialize client registration srtuct
   qsh_client_reg_init(&rf_client_reg);

   // Initialize client metric config struct
   //qsh_client_metric_cfg_init(&rf_qsh_metric_cfg_arr[0][0], RF_QSH_METRIC_COUNT * RF_MAX_SUBS);


  
      // Populate client_registration.metric_info struct
      //rf_client_reg.metric_info.metric_cfg_arr_ptr = &rf_qsh_metric_cfg_arr[0][0];
      //rf_client_reg.metric_info.metric_cfg_count = RF_QSH_METRIC_COUNT * RF_MAX_SUBS;

      /*!****populate client_registration.dump/event info if needed*****/

      // Populate other client registration params
      rf_client_reg.major_ver     = QSH_CLT_RF_MAJOR_VER;
      rf_client_reg.minor_ver     = QSH_CLT_RF_MINOR_VER;
      rf_client_reg.client        = QSH_CLT_RF;
      rf_client_reg.client_cb_ptr = rf_qsh_callback_send_msgr;
      rf_client_reg.cb_action_support_mask =  ( QSH_ACTION_MDUMP );

      // **** Register client - this should register the client ****
      qsh_client_reg(&rf_client_reg);
	  	  
     /**** Add the thread id with rf qsh client to qsh framework  ****/	  
      qsh_add_thread_id(QSH_CLT_RF);

  
} // rf_qsh_client_register


#endif // FEATURE_QSH_MDUMP

