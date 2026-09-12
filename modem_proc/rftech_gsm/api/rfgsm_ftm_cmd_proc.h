/*!
  @file rf_gsm_ftm_cmd_proc.h

  @brief
  This file contains the interface to the RF GSM FTM Command Processor

*/

/*============================================================================
                Copyright (c) 2008 - 2010 Qualcomm Technologies Incorporated.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
==============================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_ftm_cmd_proc.h#1 $
$DateTime: 2021/03/09 05:10:17 $ 
$Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/18/15   sc      Deprecate subscription 3 reducing memory footprint
07/30/15   ck      add FTM GSM cmd to override QPOET mode and bias voltage
06/19/15   tsr     Implemented subscription based handling of FTM<->RF Tasks MSGR messages 
06/12/15   tsr     Added MSG Router Framework for dipatching FTM Task->RF Tasks messages based
                   on FTM Subscription 
07/14/14   ec      Function prototypes for SAWless control interface
05/29/14   sb      IP2 cal bug fixes, updates
03/26/14   sb      Add support for IP2 cal for Dime PM
02/15/13   sb      Added FTM hook to override tuner tune code 
10/05/12   av      Expanded rf task architecture to include more tasks. 
08/01/12   ec       Added prototyping for set mode, set pa range, set linear rgi, set 
                    slot override flag, get pa range, dispatch init, dispatch deinit, and
                    internal cmd dispatcher.
07/18/12   ec       Implemented rfgsm_ftm_set_tx_pow_dbm to set tx power via MSGR from 
                    FTM task 
03/22/12   ggs      Removed unnecessary header file
03/12/12   ggs      Initial version based upon rf_lte_ftm_cmd_proc.h
===========================================================================*/

#ifndef RFGSM_FTM_CMD_PROC_H
#define RFGSM_FTM_CMD_PROC_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "ftm.h"
#include "ftmdiag.h"
#include "msgr_types.h"
#include "rfgsm_ftm_msg.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/
/*===========================================================================

  FUNCTION:  rfgsm_ftm_msgr_register

===========================================================================*/
/*!
  @brief
  This function is used by RF Task1 to register for commands sent by the
  RF GSM FTM Dispatcher in the FTM task via the message router.


  @return
  TRUE if all registration were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_ftm_msgr_register( msgr_client_t *client_id, msgr_id_t id );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_msgr_register_sub2

===========================================================================*/
/*!
  @brief
  This function is used by RF Task2 to register for commands sent by the
  RF GSM FTM Dispatcher in the FTM task via the message router.


  @return
  TRUE if all registration were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_ftm_msgr_register_sub2( msgr_client_t *client_id, msgr_id_t id );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_msgr_deregister

===========================================================================*/
/*!
  @brief
  This function is used by RF Task1 to de-register the messages registered earlier 
  for handling the messages sent by the RF GSM FTM processor to the message router


  @return
  TRUE if all de-registration were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_ftm_msgr_deregister( msgr_client_t *client_id );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_msgr_deregister_sub2

===========================================================================*/
/*!
  @brief
  This function is used by RF Task2 to de-register the messages registered earlier 
  for handling the messages sent by the RF GSM FTM processor to the message router


  @return
  TRUE if all de-registration were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_ftm_msgr_deregister_sub2( msgr_client_t *client_id );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_set_mode_cmd

===========================================================================*/
/*! 
  @todo
  This is a dummy placeholder function 
 
  @brief
  This is a dummy placeholder function 
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function and then sends a RSP MSG back and echos the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_mode_cmd( rfa_rf_gsm_ftm_set_mode_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                     msgr_umid_type rsp_umid );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_set_pa_range

===========================================================================*/
/*!
  @brief
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function and then sends a RESP MSG back and echos the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_pa_range( rfa_rf_gsm_ftm_set_pa_range_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                     msgr_umid_type rsp_umid  );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_set_slot_override_flag

===========================================================================*/
/*!
  @brief
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function and then sends a RESP MSG back and echoes the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_slot_override_flag( rfa_rf_gsm_ftm_set_slot_override_flag_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                               msgr_umid_type rsp_umid );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_set_tx_pow_dbm

===========================================================================*/
/*!
  @brief
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function and then sends a RESP MSG back and echoes the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_tx_pow_dbm ( rfa_rf_gsm_ftm_set_tx_pow_dbm_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                        msgr_umid_type rsp_umid );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_set_path_delay

===========================================================================*/
/*!
  @brief
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function and then sends a RESP MSG back and echoes the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_path_delay( rfa_rf_gsm_ftm_set_path_delay_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                       msgr_umid_type rsp_umid );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_set_linear_rgi

===========================================================================*/
/*!
  @brief
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function and then sends a RESP MSG back and echoes the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_linear_rgi( rfa_rf_gsm_ftm_set_linear_rgi_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                       msgr_umid_type rsp_umid );

/*===========================================================================

  FUNCTION:  rfgsm_ftm_set_qpoet_mode_bias

===========================================================================*/
/*!
  @brief
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function and then sends a RESP MSG back and echoes the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_qpoet_mode_bias( rfa_rf_gsm_ftm_set_qpoet_mode_bias_cmd_s* rfgsm_ftm_msg_ptr ,
                                                       msgr_umid_type rsp_umid );

/*===========================================================================*/
/*!
  @brief
  This function is called by rf_gsm_ftm_internal_cmd_dispatch, and calls the respective RF
  function rfgsm_ftm_tuner_override and then sends a RESP MSG back and echoes the arguments of the 
  original command.

  @return
  rfgsm_dispatch_outcome_t.
*/
/*=========================================================================*/
rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_tuner_override ( rfa_rf_gsm_ftm_tuner_override_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                        msgr_umid_type rsp_umid );

/*===========================================================================

  FUNCTION:  rf_gsm_ftm_internal_cmd_dispatcher

===========================================================================*/
/*!
  @brief
  This function is the rf gsm ftm dispatcher for the RFGSM FTM CMDs 
  the RF task receives. The dispatcher calls the function that sends a command 
  to the respective RF function. Upon completion a response is sent to the
  FTM task.

  @return
  rfgsm_dispatch_outcome_t
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_internal_cmd_dispatcher(rfgsm_ftm_msg_u* msg_ptr);

/*===========================================================================

  FUNCTION:  rfgsm_ftm_internal_cmd_task1_dispatcher

===========================================================================*/
/*!
  @brief
  This function is the rf gsm ftm dispatcher for the RFGSM FTM CMDs 
  the RF task 1 receives. The dispatcher calls the function that sends a command 
  to the respective RF function. Upon completion a response is sent to the
  FTM task.

  @return
  rfgsm_dispatch_outcome_t
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_internal_cmd_task1_dispatcher(rfgsm_ftm_msg_u* msg_ptr);

/*===========================================================================

  FUNCTION:  rfgsm_ftm_internal_cmd_task2_dispatcher

===========================================================================*/
/*!
  @brief
  This function is the rf gsm ftm dispatcher for the RFGSM FTM CMDs 
  the RF task 2 receives. The dispatcher calls the function that sends a command 
  to the respective RF function. Upon completion a response is sent to the
  FTM task.

  @return
  rfgsm_dispatch_outcome_t
*/
/*=========================================================================*/

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_internal_cmd_task2_dispatcher(rfgsm_ftm_msg_u* msg_ptr);

/*===========================================================================

  FUNCTION:  rfgsm_ftm_dispatch_init

===========================================================================*/
/*!
  @brief

  @details This funtion has to be called in the rfgsm_mc_init before an RF GSM FTM command is
  sent to RF Driver task. Registration with rf module dispatch table and  rf command dispatch table
  for the RF GSM FTM module.
*/
/*=========================================================================*/
void rfgsm_ftm_dispatch_init(void);
/*===========================================================================*/
/*!
  @brief

  @details This funtion has to be called in the rfgsm_mc_init before an RF GSM FTM command is
  sent to RF Driver task. Registration with rf module dispatch table and  rf command dispatch table
  for the RF GSM FTM module.
*/
/*=========================================================================*/
void rfgsm_ftm_dispatch_task_2_init(void);
/*===========================================================================

  FUNCTION:  rfgsm_ftm_dispatch_deinit 

===========================================================================*/
/*!
  @brief
  This function has to be called in the whenever the RF task is killed. Frees allocated
  memory and registration tables.

  @return
  void
*/
/*=========================================================================*/

void rfgsm_ftm_dispatch_deinit(void);



rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_ip2_cal_params ( rfa_rf_gsm_ftm_ip2_cal_type_cmd_s* rfgsm_ftm_msg_ptr,
                                                            msgr_umid_type rsp_umid );

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_get_ip2_cal_params ( rfa_rf_gsm_ftm_get_ip2_cal_info_cmd_s* rfgsm_ftm_msg_ptr,
                                                            msgr_umid_type rsp_umid );

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_get_ip2_cal_results( rfa_rf_gsm_ftm_get_ip2_cal_results_cmd_s* rfgsm_ftm_msg_ptr,
                                                            msgr_umid_type rsp_umid );

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_set_sawless_lin_mode( rfa_rf_gsm_ftm_set_sawless_lin_mode_cmd_s* rfgsm_ftm_msg_ptr,
                                                             msgr_umid_type rsp_umid );

rfgsm_ftm_dispatch_outcome_t rfgsm_ftm_get_sawless_lin_mode( rfa_rf_gsm_ftm_get_sawless_lin_mode_cmd_s* rfgsm_ftm_msg_ptr,
                                                             msgr_umid_type rsp_umid );

#endif /*  RFGSM_FTM_CMD_PROC_H */

