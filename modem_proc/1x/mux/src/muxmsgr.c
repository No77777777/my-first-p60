/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
          M U X  M E S S A G E  R O U T E R   M O D U L E


GENERAL DESCRIPTION
      This file is an interface between the MUX and the MSGR modules. It
      implements the functionality to setup MUX - RXC and TXC tasks with
      the MSGR framework, registers the messages that these tasks are
      interested in receiving from the MSGR and processes them.

EXPORTED FUNCTIONS:
      muxmsgr_init
      muxmsgr_register_msg
      muxmsgr_deregister_msg
      muxmsgr_register_1xdemod_msgs
      muxmsgr_deregister_1xdemod_msgs
      muxmsgr_send_msg
      muxmsgr_process_rcv_msg
      muxmsgr_stop

INITIALIZATION AND SEQUENCING REQUIREMENTS
      Must call muxmsgr_init() before any other functions in this module
      are invoked.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Copyright (c) 2009 - 2015 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/mux/src/muxmsgr.c#2 $ $DateTime: 2020/01/27 00:26:21 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/24/15   eye     Re-arranged macros for RUMI work when RF disabled.
10/02/15   srk     DRX SRCH response deadlock design change and cleanup.
07/27/15   eye     Added get and set API's for DRX global variables.
03/12/15   eye     Added DRX changes to avoid race conditions with SRCH.
02/27/15   eye     Added initial changes for 1x/1xA DRX feature for THOR.
01/06/15   srk     Wait for TX start/stop response before sending ASDIV command
11/07/14   srk     Process Tx Trigger response from FW for both DAC and TxFE.
10/24/14   jh      Remove unused API
10/08/14   srk     FEATURE_MODEM_1X_UTX related cleanup.
09/05/14   srk     Wait for FW response for the DAC stop in UTX targets.
05/29/14   pap     Redistributing messages.
02/13/14   dkb     Add support for optimized Tx trigger.
11/05/13   cjb     Print UMID and failure reason for messages sent out from MUX
10/09/13   dkb     Add additional RL changes.
09/30/13   dkb     Initial RL changes for new FW interface.
08/27/13   pap     Mainlining feature FEATURE_MODEM_1X_USE_ATMR_IN_RC11.
07/03/13   srk     Ensure msgr circular buffer index is within bounds - 1.
10/03/12   srk     Ensure that only one task is updating the muxmsgr debug
                   buffer at any given time
06/05/12   srk     Assert on Success or already done for msgr registration.
08/18/11   adw     Mainline 1x message router support.
04/27/11   jtm     Added receive processing of CDMA_FW_IND_ADV1X_LOG_BUFFER_RDY.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
10/30/10   jtm     Modem 1x SU API changes.
08/05/10   vks     Move TX, RX MSGR module definitions from muxmsgr.h to
                   onex_msgr.h
05/17/10   vks     Replace MAX_NUM_TASKS with NUM_MUX_TASKS to fix lint error
05/07/10   vks     Fill in debug buffer for the MSGR SEND/RCV transactions.
04/07/10   vks     Reduce message level.
01/05/10   vks     Reduce Estatus message to MED from FATAL.
12/07/09   vks     Register/de-register and send messages only after MUX tasks
                   initialization with MSGR is done.
11/16/09   vks     Cleanup.
11/12/09   vks     Add MSGR/Q6 framework support.
09/09/09   vks     Initial implementation.

===========================================================================*/


/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

/* OneX common */
#include "onex_msgr.h"

/* Cp */
#include "rx_v.h"
#include "tx_v.h"

/* FW */
#include "cdmafw_msg.h"

/* Message Router Includes */
#include "msgr_rex.h"
#include "msgr_umid.h"
#include "msgr.h"

/* Mux */
#include "muxmdsp_i.h"
#include "muxmsgr.h"
#include "rxccommon_i.h"
#include "rxctraffic.h"
#include "txccommon_i.h"

/* Other */
#include "IxErrno.h"
#include "queue.h"
#include "task.h"



/*===========================================================================

                   CONSTANTS AND MACROS

===========================================================================*/

/* MSGR RECEIVE QUEUE SIZE */
#define MUXMSGR_RCV_QUEUE_SIZE  10

/* Critical Section */
#define MUXMSGR_ENTER_CRITICAL_SECTION()  rex_enter_crit_sect(&muxmsgr_crit_sect)
#define MUXMSGR_LEAVE_CRITICAL_SECTION()  rex_leave_crit_sect(&muxmsgr_crit_sect)

/*===========================================================================

                   INTERNAL TYPES

===========================================================================*/

/* These are all the messages that MUX (CDMA TX,RX tasks) is interested in */
typedef union
{
  msgr_hdr_struct_type     hdr;

  /* 1X DEMOD RSP MSG Union type - defined in cdmafw_msg.h and contains
  */
  cdmafw_msg_rsp_u         cdmafw_msg;

  cdmafw_msg_ind_u         cdmafw_ind;

} muxmsgr_rcv_msg_msgs_type;

typedef struct
{
  q_link_type             qlink;
} muxmsgr_rcv_msg_hdr_type;

typedef struct
{
  muxmsgr_rcv_msg_hdr_type     msg_hdr;
  muxmsgr_rcv_msg_msgs_type    msg;
} muxmsgr_rcv_msg_type;

typedef struct
{
  muxmsgr_rcv_msg_type     q_items[ MUXMSGR_RCV_QUEUE_SIZE ];
  q_type                   msg_q;
  q_type                   free_q;
} muxmsgr_rcv_buffer_type;



/*===========================================================================

                   LOCAL VARIABLES

===========================================================================*/
/* A buffer each for TX and RX task */
static muxmsgr_rcv_buffer_type      muxmsgr_rcv[(uint32)NUM_MUX_TASKS];

/* Boolean to indicate if MUX TASKS MSGR has been initialized and available */
static boolean                      muxmsgr_available[(uint32)NUM_MUX_TASKS];

/* declare a critical section that is common for both the RX and TX task */
static rex_crit_sect_type           muxmsgr_crit_sect;

static boolean                      muxmsgr_crit_init = FALSE;

/* Boolean to indicate if start response has been processed */
static boolean                      muxmsgr_start_yet_to_be_processed = FALSE;

/* Boolean to indicate if stop response has been processed */
static boolean                      muxmsgr_stop_yet_to_be_processed  = FALSE;
/*===========================================================================

                   DEBUG CAPABILITY

===========================================================================*/

/* MSGR Debug capability */
#define FEATURE_MUXMSGR_DEBUG

#ifdef FEATURE_MUXMSGR_DEBUG
#define MUXMSGR_DEBUG_BUFFER_SIZE    32

typedef enum
{
  SEND,
  RCV,
} muxmsgr_debug_interaction_enum_type;

/* Node type for circular trace buffer */
typedef struct
{
  muxmsgr_tasks_enum_type               task;    /* MUX TASK               */
  msgr_umid_type                        umid;    /* Message UMID           */
  muxmsgr_debug_interaction_enum_type   interaction;
                                                 /* Interaction type with
                                                    MSGR                   */
  timetick_type                         ts;      /* Timestamp              */
} muxmsgr_debug_node_type;

typedef struct
{
  muxmsgr_debug_node_type               debug_buffer
                                                 [MUXMSGR_DEBUG_BUFFER_SIZE];

  uint16 index;                                   /* Circular buffer index */
} muxmsgr_debug_buffer_type;

/* Function declaration */
static void update_mux_debug_buffer
(
  muxmsgr_tasks_enum_type                task,
  msgr_umid_type                         umid,
  muxmsgr_debug_interaction_enum_type    interaction
);

static muxmsgr_debug_buffer_type muxmsgr_debug_buffer;
#endif /* FEATURE_MUXMSGR_DEBUG */



/*===========================================================================

                    INTERNAL FUNCTION DECLARATIONS

===========================================================================*/

static uint32 get_task_msgr_q_signal( muxmsgr_tasks_enum_type task );
static msgr_tech_module_type get_task_tech_module(
                                              muxmsgr_tasks_enum_type task );
static rex_tcb_type* get_task_tcb_ptr( muxmsgr_tasks_enum_type task);
static msgr_client_t* get_msgr_client_id_ptr( muxmsgr_tasks_enum_type task );
static msgr_id_t* get_msgr_queue_id_ptr( muxmsgr_tasks_enum_type task );
static void init_queue( muxmsgr_tasks_enum_type task );
static void setup_client( muxmsgr_tasks_enum_type task );
static void process_rcv_msg( muxmsgr_tasks_enum_type    task,
                             muxmsgr_rcv_msg_msgs_type* rcv_msg_msgs_ptr );



/*===========================================================================

                    INTERNAL FUNCTION DEFINITIONS

===========================================================================*/


/*===========================================================================

FUNCTION       GET_TASK_MSGR_Q_SIGNAL

DESCRIPTION    This function returns the msgr Q signal for the task
               requested.

DEPENDENCIES   None.

RETURN VALUE   Msgr Q signal for the task.

SIDE EFFECTS   None.

===========================================================================*/
static uint32 get_task_msgr_q_signal
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{
  /*-----------------------------------------------------------------------*/

  /* Ensure task is within range */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  if ( task == TX_TASK )
  {
    return ( TXC_MUXMSGR_Q_SIG );
  }
  else /* For RX_TASK. Note the above assert ensures task within range */
  {
    return ( RXC_MUXMSGR_Q_SIG );
  }

} /*  get_task_msgr_q_signal */


/*===========================================================================

FUNCTION       GET_TASK_TECH_MODULE

DESCRIPTION    This function returns the msgr tech module type for the task
               requested.

DEPENDENCIES   None.

RETURN VALUE   msgr_tech_module_type for the task.

SIDE EFFECTS   None.

===========================================================================*/
static msgr_tech_module_type get_task_tech_module
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{
  /*-----------------------------------------------------------------------*/

  /* Ensure task is within range */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  if ( task == TX_TASK )
  {
    return ( MSGR_ONEX_TX );
  }
  else /* For RX_TASK. Note the above assert ensures task within range */
  {
    return ( MSGR_ONEX_RX );
  }

} /*  get_task_tech_module */


/*===========================================================================

FUNCTION       GET_TASK_TCB_PTR

DESCRIPTION    This function returns the pointer to the task TCB.

DEPENDENCIES   None.

RETURN VALUE   rex_tcb_type pointer to the task TCB.

SIDE EFFECTS   None.

===========================================================================*/
static rex_tcb_type*  get_task_tcb_ptr
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{
  /*-----------------------------------------------------------------------*/

  /* Ensure task is within range */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  if ( task == TX_TASK )
  {
    return ( TX_TCB_PTR );
  }
  else /* For RX_TASK. Note the above assert ensures task within range */
  {
    return ( RX_TCB_PTR );
  }

} /*  get_task_msgr_q_signal */


/*===========================================================================

FUNCTION       GET_MSGR_CLIENT_ID_PTR

DESCRIPTION    This function returns the msgr client id ptr for the task
               requested.

DEPENDENCIES   None.

RETURN VALUE   Client id ptr of the task.

SIDE EFFECTS   None.

===========================================================================*/
static msgr_client_t*  get_msgr_client_id_ptr
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{

  static msgr_client_t muxmsgr_client_id[NUM_MUX_TASKS];
                                                 /* MUX client id for MSGR */

  /*-----------------------------------------------------------------------*/

  /* Ensure array limits */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  return &( muxmsgr_client_id[task] );

} /*  get_msgr_client_id_ptr */


/*===========================================================================

FUNCTION       GET_MSGR_QUEUE_ID_PTR

DESCRIPTION    This function returns the pointer to the msgr queue id for the
               task requested.

DEPENDENCIES   None.

RETURN VALUE   Queue id ptr of the task.

SIDE EFFECTS   None.

===========================================================================*/
static msgr_id_t*  get_msgr_queue_id_ptr
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{

  static msgr_id_t muxmsgr_queue_id[(uint32)NUM_MUX_TASKS];
                                                 /* MSGR queue id          */

  /*-----------------------------------------------------------------------*/

  /* Ensure array limits */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  return &( muxmsgr_queue_id[task] );

} /*  get_msgr_queue_id_ptr */


/*===========================================================================

FUNCTION       INIT_QUEUE

DESCRIPTION    This function initializes the receive queue for the task
               requested.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
static void init_queue
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{

  uint8 i;                                       /* loop variable for queue
                                                    size                   */

  /*-----------------------------------------------------------------------*/

  /* Initialize the Msgr Queue, and the Msgr Free Queue */
  (void) q_init( &muxmsgr_rcv[task].msg_q );
  (void) q_init( &muxmsgr_rcv[task].free_q );


  /* Link the Msgr Queue Items onto the Msgr Free Queue */
  for (i=0; i < MUXMSGR_RCV_QUEUE_SIZE; i++)
  {
    q_put( &muxmsgr_rcv[task].free_q,
           q_link( &muxmsgr_rcv[task].q_items[i],
                   &muxmsgr_rcv[task].q_items[i].msg_hdr.qlink )
         );
  }

} /* init_queue */


/*===========================================================================

FUNCTION       SETUP_CLIENT

DESCRIPTION    This function sets up the MUX TX/RX client with the MSGR. It
               first registers a client id ptr with the MSGR framework and
               then uses this client id ptr to add the msgr queues and register
               the TCB and the signals.

DEPENDENCIES   Queues must be initialized.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
static void setup_client
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{
  errno_enum_type  estatus;                      /* Error status           */
  msgr_client_t*   muxmsgr_client_id_ptr = get_msgr_client_id_ptr(task);
                                                 /* 1X client id ptr       */



  /*-----------------------------------------------------------------------*/

  /* Ensure client id ptr is not NULL */
  ONEX_ASSERT(muxmsgr_client_id_ptr != NULL );

  /* Ensure task is within range */
  ONEX_ASSERT( task < NUM_MUX_TASKS );


  /* Register the client id */
  estatus = msgr_client_create( muxmsgr_client_id_ptr );
  ONEX_ASSERT( estatus == E_SUCCESS );

  /* Add REX Q with the MSGR. Use the above client id, tcb ptr and rex sig */
  estatus = msgr_client_add_rex_q ( muxmsgr_client_id_ptr,
                                    get_task_tcb_ptr( task ),
                                    get_task_msgr_q_signal( task ),
                                    &( muxmsgr_rcv[task].msg_q ),
                                    &( muxmsgr_rcv[task].free_q ),
                                    sizeof( muxmsgr_rcv_msg_hdr_type ),
                                    MSGR_NO_OFFSET,
                                    (uint16)sizeof( muxmsgr_rcv_msg_type ),
                                    get_msgr_queue_id_ptr( task ) );
  ONEX_ASSERT( estatus == E_SUCCESS );

} /* setup_client */


/*===========================================================================

FUNCTION       PROCESS_RCV_MSG

DESCRIPTION    This function takes appropriate action based on the type of
               message received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
static void process_rcv_msg
(
  muxmsgr_tasks_enum_type    task,               /* MUX Task               */
  muxmsgr_rcv_msg_msgs_type* rcv_msg_msgs_ptr    /* Pointer to the receive
                                                    message structure      */
)
{

  /*-----------------------------------------------------------------------*/

  /* Ensure rcv_msg_msgs_ptr is not NULL */
  ONEX_ASSERT( rcv_msg_msgs_ptr != NULL );

  /* Ensure task is within range */
  ONEX_ASSERT( task < NUM_MUX_TASKS );


  #ifdef FEATURE_MUXMSGR_DEBUG
  /* Update the debug buffer with the rcv msg umid */
  update_mux_debug_buffer(task, rcv_msg_msgs_ptr->hdr.id, RCV);
  #endif /* FEATURE_MUXMSGR_DEBUG */

  switch ( rcv_msg_msgs_ptr->hdr.src )
  {
    case MSGR_CDMA_FW:
    {
      switch ( rcv_msg_msgs_ptr->hdr.id )
      {
        #ifdef FEATURE_IS2000_1X_ADV
        case CDMA_FW_IND_ADV1X_LOG_BUFFER_RDY:
        {
          cdmafw_msg_ind_u *cdmafw_msg_ind_ptr = NULL;
          cdmafw_msg_ind_ptr = &( rcv_msg_msgs_ptr->cdmafw_ind );

          rxc_mdsp_1xa_logging_isr(cdmafw_msg_ind_ptr->adv1x_log_buffer_rdy.logBufferReadIndex);
          break;
        }
        #endif /* FEATURE_IS2000_1X_ADV */

        #ifndef FEATURE_MODEM_1X_SUPPORTS_RF
        case CDMA_FW_TX_START_RSP:
        #endif /* !FEATURE_MODEM_1X_SUPPORTS_RF */
        #ifdef FEATURE_MODEM_1X_UTX
        case CDMA_FW_TX_DAC_START_RSP:
        #else /* FEATURE_MODEM_1X_UTX */
        case CDMA_FW_RL_TXFE_TRIGGER_RSP:
        #endif /* FEATURE_MODEM_1X_UTX */
          muxmsgr_start_yet_to_be_processed = FALSE;
          M1X_MSG( MUX, LEGACY_HIGH,
            "Received Tx Trigger response from FW" );
          break;

        #ifndef FEATURE_MODEM_1X_SUPPORTS_RF
        case CDMA_FW_TX_STOP_RSP:
        #endif /* !FEATURE_MODEM_1X_SUPPORTS_RF */
        #ifdef FEATURE_MODEM_1X_UTX
        case CDMA_FW_TX_DAC_STOP_RSP:
        #else /* FEATURE_MODEM_1X_UTX */
        case CDMA_FW_RL_TXFE_STOP_RSP:
        #endif /* FEATURE_MODEM_1X_UTX */
          muxmsgr_stop_yet_to_be_processed = FALSE;
          M1X_MSG( MUX, LEGACY_HIGH,
            "Received Tx Stop response from FW" );
          break;

       default:
         M1X_MSG( MUX, LEGACY_ERROR,
           "Invalid CDMA FW message received %d",
           rcv_msg_msgs_ptr->hdr.id);
      } /* end switch */

      break;

    }

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid Message Received, Tech=%d, Module=%d",
        MSGR_TECH_VAL(rcv_msg_msgs_ptr->hdr.id),
        MSGR_MODULE_VAL(rcv_msg_msgs_ptr->hdr.id) );
      break;

  } /* End switch */

  M1X_MSG( MUX, LEGACY_HIGH,
    "Message Received from Tech=%d, Module=%d, UMID=0x%8x",
    MSGR_TECH_VAL(rcv_msg_msgs_ptr->hdr.id),
    MSGR_MODULE_VAL(rcv_msg_msgs_ptr->hdr.id),
    rcv_msg_msgs_ptr->hdr.id );

} /* process_rcv_msg */



/*===========================================================================

                    EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/


/*===========================================================================

FUNCTION       MUXMSGR_INIT

DESCRIPTION    This function initializes the receive queue and registers the
               1x MUX Task client with the message router.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_init
(
  muxmsgr_tasks_enum_type    task              /* MUX Task                 */
)
{
  /*-----------------------------------------------------------------------*/

  /* Ensure task is within range */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  /* Initialize the Queues */
  init_queue( task );

  /* Create and register client */
  setup_client( task );

  if ( !muxmsgr_crit_init )
  {
    /* Initialize critical section */
    rex_init_crit_sect(&muxmsgr_crit_sect);

    /* This needs to be done here to avoid race condition between RX and TX
       tasks trying to initialize critical section one afer the other */
    muxmsgr_crit_init = TRUE;
  }

  /* MUX (task) MSGR is initialized and is now available */
  muxmsgr_available[task] = TRUE;

} /* muxmsgr_init */


/*===========================================================================

FUNCTION       MUXMSGR_REGISTER_MSG

DESCRIPTION    This function registers a message that MUX is interested in
               collecting from the message router.

DEPENDENCIES   1X MUX client setup with the MSGR must have been completed.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_register_msg
(
  muxmsgr_tasks_enum_type    task,               /* MUX Task               */
  msgr_umid_type             umid                /* UMID of the message    */
)
{
  errno_enum_type  estatus;                      /* Error status           */
  msgr_client_t*   muxmsgr_client_id_ptr = NULL;
                                                 /* Task client id ptr     */

  /*-----------------------------------------------------------------------*/

  /* Throw a fatal error for wrong task parameter */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  /* Obtain the client id ptr for this task */
  muxmsgr_client_id_ptr = get_msgr_client_id_ptr( task );
  ONEX_ASSERT( muxmsgr_client_id_ptr != NULL );

  /* Check if MUX (task) MSGR is available and register the message */
  if ( muxmsgr_available[task] )
  {
    estatus = msgr_register( get_task_tech_module( task ),
                             get_msgr_client_id_ptr( task ),
                             MSGR_ID_REX,
                             umid );
    ONEX_ASSERT( estatus == E_SUCCESS || estatus == E_ALREADY_DONE );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_FATAL,
      "MUX TASK(%d) MSGR not available",
      task );
  }

} /* muxmsgr_register_msg */


/*===========================================================================

FUNCTION       MUXMSGR_DEREGISTER_MSG

DESCRIPTION    This function deregisters a message that MUX is no longer
               interested in receving from the message router.

DEPENDENCIES   1X MUX client setup with the MSGR must have been completed.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_deregister_msg
(
  muxmsgr_tasks_enum_type    task,               /* MUX Task               */
  msgr_umid_type             umid                /* UMID of the message    */
)
{
  errno_enum_type  estatus;                      /* Error status           */
  msgr_client_t*   muxmsgr_client_id_ptr = NULL;
                                                 /* Task client id ptr     */

  /*-----------------------------------------------------------------------*/

  /* Throw a fatal error for wrong task parameter */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  /* Obtain the client id ptr for this task */
  muxmsgr_client_id_ptr = get_msgr_client_id_ptr( task );
  ONEX_ASSERT( muxmsgr_client_id_ptr != NULL );

  /* Check if MUX (task) MSGR is available and de-register the message */
  if ( muxmsgr_available[task] )
  {
    estatus = msgr_deregister( get_task_tech_module( task ),
                               get_msgr_client_id_ptr( task ),
                               umid );
    ONEX_ASSERT( estatus == E_SUCCESS );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_FATAL,
      "MUX TASK(%d) MSGR not available",
      task );
  }

} /* muxmsgr_deregister_msg */


/*===========================================================================

FUNCTION       MUXMSGR_REGISTER_1XDEMOD_MSGS

DESCRIPTION    This function registers the demod messages that mux is
               interested in receiving from 1X DEMOD FW (cdmafw_msg.h).

DEPENDENCIES   Must be called after 1X DEMOD APP is successfully enabled.

RETURN VALUE   None.

SIDE EFFECTS   MUX starts receiving the registered messages from 1X DEMOD FW.

===========================================================================*/
void muxmsgr_register_1xdemod_msgs
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{
  /*-----------------------------------------------------------------------*/

  /* Throw a fatal error for wrong task parameter */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  /* cdmafw_tx_high_pwr_det_rsp_msg_t (data structure type for this umid ) */
  muxmsgr_register_msg( task, CDMA_FW_RSP_TX_HIGH_PWR_DET );

} /* muxmsgr_register_1xdemod_msgs*/


/*===========================================================================

FUNCTION       MUXMSGR_DEREGISTER_1XDEMOD_MSGS

DESCRIPTION    This function de-registers the demod messages that mux is no
               longer interested in receiving from 1X DEMOD FW.

DEPENDENCIES   Preferable to call after the 1X DEMOD APP has been disabled.

RETURN VALUE   None.

SIDE EFFECTS   MUX stops receiving the registered messages from 1X DEMOD FW.

===========================================================================*/
void muxmsgr_deregister_1xdemod_msgs
(
  muxmsgr_tasks_enum_type    task                /* MUX Task               */
)
{
  /*-----------------------------------------------------------------------*/

  /* Throw a fatal error for wrong task parameter */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  /* cdmafw_tx_high_pwr_det_rsp_msg_t (data structure type for this umid ) */
  muxmsgr_deregister_msg( task, CDMA_FW_RSP_TX_HIGH_PWR_DET );

} /* muxmsgr_deregister_1xdemod_msgs*/


/*===========================================================================

FUNCTION       MUXMSGR_PROCESS_RCV_MSG

DESCRIPTION    This function processes the received message.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_process_rcv_msg
(
  muxmsgr_tasks_enum_type    task              /* MUX Task                 */
)
{
  muxmsgr_rcv_msg_type*  rcv_msg_ptr;

  #ifdef FEATURE_MUXMSGR_DEBUG
  uint16 count_msgs = 0;
  #endif /* FEATURE_MUXMSGR_DEBUG */

  /*-----------------------------------------------------------------------*/

  /* Throw a fatal error for wrong task parameter */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  /* REVISIT MSGR: Need to enter critical section here?? */

  /* Clear the signal */
  (void) rex_clr_sigs ( get_task_tcb_ptr( task ),
                        get_task_msgr_q_signal( task ) );

  /* There is a chance of getting more than one message on the
     same signal. Hence do a while loop to process all messages */

  /* Get the received message off the queue */
  while ( ( rcv_msg_ptr = (muxmsgr_rcv_msg_type*)
                                 q_get(&muxmsgr_rcv[task].msg_q) ) != NULL )
  {
    #ifdef FEATURE_MUXMSGR_DEBUG
    /* Count number of messages being received in a loop for debug
       purposes */
    count_msgs++;
    #endif /* FEATURE_MUXMSGR_DEBUG */

    /* Now process the received message */
    process_rcv_msg ( task, &(rcv_msg_ptr->msg) );

    /* The processing is over. Now return the buffer to the free queue */
    q_put( &( muxmsgr_rcv[task].free_q ), &( rcv_msg_ptr->msg_hdr.qlink ) );

  } /* End while loop */

  #ifdef FEATURE_MUXMSGR_DEBUG
  M1X_MSG( MUX, LEGACY_LOW,
    "# of messages in one loop = %d",
    count_msgs );
  #endif /* FEATURE_MUXMSGR_DEBUG */

} /* muxmsgr_process_rcv_msg */


/*===========================================================================

FUNCTION       MUXMSGR_SEND_MSG

DESCRIPTION   This function initializes the header and sends the message to
              the message router.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_send_msg
(
  muxmsgr_tasks_enum_type    task,               /* MUX Task               */
  msgr_hdr_struct_type*      msg_hdr_ptr,        /* Pointer to MSG Header  */
  msgr_umid_type             umid,               /* UMID of the message    */
  int32                      size                /* Size of the message    */
)
{
  errno_enum_type  estatus;

  /*-----------------------------------------------------------------------*/

  /* Ensure msg_hdr_ptr is not NULL */
  ONEX_ASSERT( msg_hdr_ptr != NULL );

  /* Ensure task is within range */
  ONEX_ASSERT( task < NUM_MUX_TASKS );


  /* Initialize the message header */
  msgr_init_hdr(msg_hdr_ptr, get_task_tech_module( task ), umid);

  /* Check if MUX (task) MSGR is available and send the message */
  if ( muxmsgr_available[task] )
  {
    #ifdef FEATURE_MUXMSGR_DEBUG
    /* Update the debug buffer with this message umid */
    update_mux_debug_buffer(task,umid,SEND);
    #endif /* FEATURE_MUXMSGR_DEBUG */

    estatus = msgr_send( msg_hdr_ptr, size );
    if( estatus != E_SUCCESS )
    {
      ONEX_ERR_FATAL( "muxmsgr_send_msg( UMID=0x%08X ) failed with ESTATUS = %d",
                 umid, estatus, 0 );
    }
    else
    {
      M1X_MSG( MUX, LEGACY_MED,
        "Sent Message: UMID=0x%08X",
        umid );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_FATAL,
      "MUX TASK(%d) MSGR not available",
      task );
  }

} /* muxmsgr_send_msg */


/*===========================================================================

FUNCTION       MUXMSGR_STOP

DESCRIPTION    This function de-registers the MUX client from the message
               router. We would no longer be able to send/receive messages.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_stop
(
  muxmsgr_tasks_enum_type    task              /* MUX Task                 */
)
{
  uint8            i;
  errno_enum_type  estatus;
  msgr_client_t*   muxmsgr_client_id_ptr = NULL;

  /*-----------------------------------------------------------------------*/

  /* Throw a fatal error for wrong task parameter */
  ONEX_ASSERT( task < NUM_MUX_TASKS );

  /* Obtain the client id ptr for this task */
  muxmsgr_client_id_ptr = get_msgr_client_id_ptr( task );
  ONEX_ASSERT( muxmsgr_client_id_ptr != NULL );

  /* Deregister the client with the MSGR */
  estatus = msgr_client_delete( muxmsgr_client_id_ptr );
  ONEX_ASSERT( estatus == E_SUCCESS );

  /* Clear the Q signal for the task */
  (void) rex_clr_sigs( get_task_tcb_ptr( task),
                       get_task_msgr_q_signal( task ) );


  /* Free up the queue - link the message queue into the free queue */
  for (i=0; i<MUXMSGR_RCV_QUEUE_SIZE; i++)
  {
    q_put( &muxmsgr_rcv[task].free_q,
           q_link( &muxmsgr_rcv[task].q_items[i],
                   &muxmsgr_rcv[task].q_items[i].msg_hdr.qlink)
         );
  }

  /* Indicate MUX (task) MSGR is no longer available */
  muxmsgr_available[task] = FALSE;

} /* muxmsgr_stop */

/*===========================================================================

FUNCTION       MUXMSGR_SET_START_RSP_FLAG

DESCRIPTION    This function sets the flag to indicate that TX
               is waiting for start response from FW.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_set_start_rsp_flag( void )
{
  /*-----------------------------------------------------------------------*/

  muxmsgr_start_yet_to_be_processed = TRUE;

} /* muxmsgr_set_start_rsp_flag */

/*===========================================================================

FUNCTION       MUXMSGR_SET_STOP_RSP_FLAG

DESCRIPTION    This function sets the flag to indicate that TX
               is waiting for stop response from FW.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_set_stop_rsp_flag( void )
{
  /*-----------------------------------------------------------------------*/

  muxmsgr_stop_yet_to_be_processed = TRUE;

} /* muxmsgr_set_stop_rsp_flag */

#ifndef FEATURE_MODEM_1X_SUPPORTS_RF
/*===========================================================================

FUNCTION       MUXMSGR_WAIT_ON_START_RSP

DESCRIPTION    This function waits until the tx start response message is
               processed. While waiting, it processes any other messages
               that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_wait_on_start_rsp( void )
{
  /*-----------------------------------------------------------------------*/

  while ( muxmsgr_start_yet_to_be_processed )
  {
    (void) txc_wait( TXC_MUXMSGR_Q_SIG );

    muxmsgr_process_rcv_msg(TX_TASK);
  }

} /* muxmsgr_wait_on_start_rsp */
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

/*===========================================================================

FUNCTION       MUXMSGR_WAIT_ON_STOP_RSP

DESCRIPTION    This function waits until the txfe stop response message is
               processed. While waiting, it processes any other messages
               that might be received.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void muxmsgr_wait_on_stop_rsp( void )
{
  /*-----------------------------------------------------------------------*/

  while ( muxmsgr_stop_yet_to_be_processed )
  {
    (void) txc_wait( TXC_MUXMSGR_Q_SIG );

    muxmsgr_process_rcv_msg(TX_TASK);
  }

} /* muxmsgr_wait_on_stop_rsp */

/*===========================================================================

FUNCTION       MUXMSGR_WAITING_FOR_TX_RESPONSE_FROM_FW

DESCRIPTION    This function checks if TX is stuck waiting for
               start/stop response from FW.

DEPENDENCIES   None.

RETURN VALUE   TRUE  - If waiting for FW response.
               FALSE - Otherwise

SIDE EFFECTS   None.

===========================================================================*/
boolean muxmsgr_waiting_for_tx_response_from_fw( void )
{
  boolean waiting_for_tx_rsp = FALSE;

  /*-----------------------------------------------------------------------*/

  if ( muxmsgr_start_yet_to_be_processed || muxmsgr_stop_yet_to_be_processed )
  {
    waiting_for_tx_rsp = TRUE;
  }

  return waiting_for_tx_rsp;

} /* muxmsgr_waiting_for_tx_response_from_fw */

/*===========================================================================

FUNCTION       MUXMSGR_IS_TX_TRIGGER_RSP_RECEIVED

DESCRIPTION    This function checks to see if the TxFE/TxDAC
               trigger response has been received.

DEPENDENCIES   None.

RETURN VALUE   TRUE  - received TXFE/TXDAC rsp
               FALSE - waiting for TxFE/TxDAC rsp

SIDE EFFECTS   None.

===========================================================================*/
boolean muxmsgr_is_tx_trigger_rsp_received( void )
{
  boolean tx_trigger_rsp_is_received = TRUE;

  /*-----------------------------------------------------------------------*/

  /* process received messages */
  muxmsgr_process_rcv_msg(TX_TASK);

  if ( muxmsgr_start_yet_to_be_processed )
  {
    tx_trigger_rsp_is_received = FALSE;
  }

  return tx_trigger_rsp_is_received;

} /* muxmsgr_is_tx_trigger_rsp_received */

#ifdef FEATURE_MUXMSGR_DEBUG
/*===========================================================================

FUNCTION       UPDATE_MUX_DEBUG_BUFFER

DESCRIPTION    This function updates the circular debug structure. It stores
               the task that is sending the message, the umid of the message
               and the timestamp of the event.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   muxmsgr debug buffer gets updated.

===========================================================================*/
static void update_mux_debug_buffer
(
  muxmsgr_tasks_enum_type                task,         /* MUX Task         */
  msgr_umid_type                         umid,         /* Message UMID     */
  muxmsgr_debug_interaction_enum_type    interaction   /* Interaction type */
)
{
  timetick_type       ts               = timetick_get();
  uint16              local_index      = 0;

  /*-----------------------------------------------------------------------*/

  /* Ensure that only one task is updating the muxmsgr debug buffer
     at any given time */
  MUXMSGR_ENTER_CRITICAL_SECTION();

  /* Copy the global index value to a local index */
  local_index = muxmsgr_debug_buffer.index;

  /* Use local index value to update the debug buffer structure */
  muxmsgr_debug_buffer.debug_buffer[local_index].task        =  task;
  muxmsgr_debug_buffer.debug_buffer[local_index].umid        =  umid;
  muxmsgr_debug_buffer.debug_buffer[local_index].interaction =  interaction;
  muxmsgr_debug_buffer.debug_buffer[local_index].ts          =  ts;

  /* Update the global index value and make sure it doesn't exceed
     max. buffer size */
  muxmsgr_debug_buffer.index = ( local_index + 1 ) %
                               ( MUXMSGR_DEBUG_BUFFER_SIZE - 1 );

  /* Leave the Critical section after updating the debug buffer */
  MUXMSGR_LEAVE_CRITICAL_SECTION();

} /* update_mux_debug_buffer */
#endif /* FEATURE_MUXMSGR_DEBUG */

