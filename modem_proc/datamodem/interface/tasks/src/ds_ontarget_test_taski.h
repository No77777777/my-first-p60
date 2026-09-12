#ifndef DS_ONTARGET_TESTI_TASKI_H
#define DS_ONTARGET_TESTI_TASKI_H
/*=========================================================================*/
/*!
  @file
  ds_ontarget_test_taski.h

  @brief
  Header file containing internal definitions for DS_ONTARGET_TEST task services.

  @details
  Exports data types, functions and macros internally used by DS_ONTARGET_TEST task.

  @exported_data

  @exported_macros
  DS_ONTARGET_TESTI_SET_SIGNAL()
    Set the signal for DS_ONTARGET_TEST task (internal macro). Clients should use
    ds_ontarget_test_enable_signal()

  DS_ONTARGET_TESTI_CLR_SIGNAL()
    Clear the signal for DS_ONTARGET_TEST task (internal macro). Clients should use
    ds_ontarget_test_disable_signal()

  DS_ONTARGET_TESTI_SET_SIG_MASK()
    Helper routine to set the signal mask where REX sig ext are enabled.

  DS_ONTARGET_TESTI_CLR_SIG_MASK()
    Helper routine to disable signal mask where REX sig ext are enabled.

  DS_ONTARGET_TESTI_IS_SIG_MASK_SET()
    Helper routine to verify if the signal mask is set (for rex sig exts).

  @exported_functions
  ds_ontarget_testi_default_sig_handler()
    Implicit handler for signals processed by DS_ONTARGET_TEST Task.

  ds_ontarget_testi_default_cmd_handler()
    Implicit handler for commands processed by DS_ONTARGET_TEST Task.

  ds_ontarget_testi_powerup()
    Cold start called before DS_ONTARGET_TEST task is started.

  ds_ontarget_testi_init()
    After task start initialization of the DS_ONTARGET_TEST task.

  ds_ontarget_testi_process_cmd()
    Processes command signals in DS_ONTARGET_TEST Task.


  Copyright (c) 2009-2011 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/tasks/src/ds_ontarget_test_taski.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
2012-09-02 aa   Created module
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */

#ifdef __cplusplus
extern "C" {
#endif

#include "rex.h"
#include "task.h"
#include "ds_ontarget_test_task.h"
#include "ds_ontarget_test_svc.h"
#include "queue.h"
#include "ps_crit_sect.h"
#include "ds_Utils_DebugMsg.h"

/*===========================================================================

                            PUBLIC DATA DECLARATIONS

===========================================================================*/
/*--------------------------------------------------------------------------
  Data type for holding DS_ONTARGET_TEST command buffers.
---------------------------------------------------------------------------*/
typedef struct
{
  q_link_type                     link;           /* For linking onto cmd queue   */
  ds_ontarget_test_cmd_enum_type  cmd;            /* Command to be processed      */
  void                          * user_data_ptr;  /* Command specific user data   */
} ds_ontarget_testi_cmd_type;

/*---------------------------------------------------------------------------
  Data type for holding signal handler.
---------------------------------------------------------------------------*/
typedef struct
{
  ds_ontarget_test_signal_handler_type  sig_handler;     /* signal handler cback  */
  void                                * user_data_ptr;   /* signal user data      */
} ds_ontarget_testi_sig_handler_info_type;

/*--------------------------------------------------------------------------
  Data types to hold signal and cmd handlers
---------------------------------------------------------------------------*/
extern ds_ontarget_testi_sig_handler_info_type ds_ontarget_testi_sig_handler[DS_ONTARGET_TEST_SIGNAL_MAX];
extern ds_ontarget_test_cmd_handler_type       ds_ontarget_testi_cmd_handler[DS_ONTARGET_TEST_CMD_MAX];

/*--------------------------------------------------------------------------
  Command Queues.
---------------------------------------------------------------------------*/
extern q_type ds_ontarget_testi_cmd_q;
extern q_type ds_ontarget_testi_cmd_free_q;

/*--------------------------------------------------------------------------
  DS_ONTARGET_TEST's own critical section
---------------------------------------------------------------------------*/
extern ps_crit_sect_type ds_ontarget_test_crit_sect;

/*--------------------------------------------------------------------------
  Pool of signals being currently enabled for DS_ONTARGET_TEST main processing loop
---------------------------------------------------------------------------*/
extern rex_sigs_type ds_ontarget_testi_enabled_sig_mask;

/*===========================================================================

                           PUBLIC MACRO DEFINITIONS

===========================================================================*/
#define DS_ONTARGET_TESTI_SET_SIGNAL( sig )                                           \
  (void)rex_set_sigs(&ds_ontarget_test_tcb, (rex_sigs_type)1 << (rex_sigs_type)(sig))

#define DS_ONTARGET_TESTI_CLR_SIGNAL( sig )                                           \
  (void)rex_clr_sigs(&ds_ontarget_test_tcb, (rex_sigs_type)1 << (rex_sigs_type)(sig))

/*===========================================================================

                          PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*=========================================================================*/
/*!
  @function
  ds_ontarget_testi_default_sig_handler

  @brief
  The default Signal handler for DS_ONTARGET_TEST task.

  @details
  This function is registered as the default signal handler for all the
  signals which do not have a registered handler. It merely prints a warning
  message.

  @dependencies
  None.

  @return
  None.

  @note
  None.
*/
/*=========================================================================*/
boolean ds_ontarget_testi_default_sig_handler
(
  ds_ontarget_test_signal_enum_type sig,             /* Signal to be processed       */
  void *user_data_ptr                      /* Parameter carrying user data */
);


/*=========================================================================*/
/*!
  @function
  ds_ontarget_testi_default_cmd_handler

  @brief
  The default command handler for DS_ONTARGET_TEST task.

  @details
  This function is registered as the default command handler for all the
  signals which do not have a registered handler. It merely prints a warning
  message.

  @dependencies
  None.

  @return
  None.

  @note
  None.
*/
/*=========================================================================*/
void ds_ontarget_testi_default_cmd_handler
(
  ds_ontarget_test_cmd_enum_type cmd,      /* Actual command to be processed         */
  void *user_data_ptr            /* Command specific user parameters       */
);


/*=========================================================================*/
/*!
  @function
  ds_ontarget_testi_powerup

  @brief
  The powerup init function for DS_ONTARGET_TEST task.

  @details
  Powerup (coldstart) initialization function for the DS_ONTARGET_TEST task. This
  function will perform all first time thru functions for the DS_ONTARGET_TEST task.
  After this intiailzation, the DS_ONTARGET_TEST task would signal the TMC that it
  is ready to accept requests from other tasks. Any initializations that
  are NOT dependent upon other tasks should be performed here.

  @dependencies
  None.

  @return
  None.

  @note
  This function must be called only once at system powerup.
*/
/*=========================================================================*/
void ds_ontarget_testi_powerup
(
  void
);


/*=========================================================================*/
/*!
  @function
  ds_ontarget_testi_init

  @brief
  The power-after init function for DS_ONTARGET_TEST task.

  @details
  Initialization function for the DS_ONTARGET_TEST task. This function
  performs the functions needed for the DS_ONTARGET_TEST task to exit disabled state.

  @dependencies
  None.

  @return
  None.

  @note
  DS_ONTARGET_TEST task should have been started.
  This function must be called only once at after signaling task start.
*/
/*=========================================================================*/
void ds_ontarget_testi_init
(
  void
);


/*=========================================================================*/
/*!
  @function
  ds_ontarget_testi_process_cmd

  @brief
  Processes commands posted to the DS_ONTARGET_TEST task.

  @details
  This function is called in response to DS_ONTARGET_TEST_CMD_Q_SIGNAL.  It retrieves
  the next command from the DS_ONTARGET_TEST command queue and processes the command
  by calling the registered cmd handler.

  @dependencies
  An item must be on the DS_ONTARGET_TEST command queue or an LOG_MSG_ERROR will occur.

  @return
  FALSE if cmd queue contains more commands to process and this function
  should be called again, TRUE otherwise.

  @note
  None.
*/
/*=========================================================================*/
boolean ds_ontarget_testi_process_cmd
(
  void
);

#ifdef __cplusplus
}
#endif


#endif /* DS_ONTARGET_TESTI_SVCI_H */

