#ifndef DCC_TASK_DEFS_H
#define DCC_TASK_DEFS_H
/*===========================================================================

                        D C C _ D E F S . H

DESCRIPTION

  Data Common Control Task header file.
  All definitions and structures needed by DCC task modules.


Copyright (c) 2009-2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/tasks/inc/dcc_task_defs.h#3 $ $DateTime: 2024/05/06 04:25:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/15   sd      FC Task removal changes.
11/17/14   skc     Removing FEATURE_UW_FMC
11/12/14   sk      QMI-DMS Service QCSI migration.
10/10/14   ssh     IP Translation Engine 464XLAT 
11/01/13   rk      Removed feature mobileap.
09/27/13   wc      Support dynamic port configuration
09/14/12   ss      Added TFTP ALG client command.
08/16/12   wc      Reserve QSAP signal
02/10/12   rt      Added support for SoftAPv6
02/01/12   am      Added QCMAP DCC signal and commands.
11/03/11   sm      RTSP ALG Changes.
08/03/11   sa      GSDI cleanup changes.
08/12/11   ss      Added DCC_SIP_ALG_CLIENT_CMD.
08/12/11   cp      Added the support for PPTP ALG.
08/10/11   wc      Data aggregation based on CPU utilization.
03/09/11   kk      Removing DCC command buffer dependency from RmNet.
12/17/10   kk      Removing DCC command buffer dependency from QMI services.
11/08/10   mct     Support for QMI AKAv2.
10/19/10   sy      Removing Iface/Flow/QMG DCC data buffers.
09/24/10   pp      Facilitate user memory instead of DCC data buffers
                   while posting DCC commands.
08/20/10   bd      Fixed CAT_MAX_DATA_LEN value
07/09/10   sk      Added QMI PBM related changes
06/11/10   mib     Add GSTK decoded format
10/23/09   ar      Add supprot for QMUX power save indication filtering
                   and port configuration from NVRAM.
10/08/09   kk      Windows7 compliance - DMS changes.
02/19/09   am      Created module.
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FEATURE_WINCE
#include "rex.h"
#include "task.h"
#endif /* FEATURE_WINCE */

#include "queue.h"
#include "sio.h"
#include "rdevmap.h"

#include "ps_iface_defs.h"
#include "ds_Utils_DebugMsg.h"

#include "ds_qmi_if.h"
#include "ds_qmi_if_ext.h"


#ifdef FEATURE_DATA_QMI_EAP
#include "ps_eap.h"
#include "ps_eap_sim_aka.h"
#include "ps_utils_aka.h"
#endif


#ifdef FEATURE_DATA_QMI_CAT
#include "gstk_exp.h"
#endif /* FEATURE_DATA_QMI_CAT */

#ifdef FEATURE_DATA_QMI_AT
#include "dsatclient.h"
#endif /* FEATURE_DATA_QMI_AT */


#ifdef FEATURE_DUAL_SIM
#include "cm_dualsim.h"
#endif

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
extern rex_tcb_type * rex_dcc_tcb;

#define DCC_SIG_ARR_LEN REX_SIGS_ARRAY_LEN
    
#define DCC_SET_SIGNAL( sig )                                             \
{                                                                         \
    (void)rex_set_sigs( rex_dcc_tcb,                                      \
                       (rex_sigs_type) 1 << (rex_sigs_type)( sig ) );     \
}
    
#define DCC_CLR_SIGNAL( sig )                                             \
{                                                                         \
    (void)rex_clr_sigs( rex_dcc_tcb,                                      \
                        (rex_sigs_type) 1 << (rex_sigs_type)( sig ) );    \
}

/*--------------------------------------------------------------------------
  DCC Task Signal Definitions.  The value of existing signals
  should not be changed while adding or deleting a signal.  Some signals
  (marked Reserved) have fixed value that should stay as it is.
--------------------------------------------------------------------------*/
typedef enum
{
  DCC_CMD_Q_SIGNAL                =  1,   /* DCC command Q signal          */
  DCC_DOG_HB_REPORT_SIGNAL        =  3,   /* Dog heart beat report signal   */
  DCC_NV_CMD_SIGNAL               =  4,   /* NV cmd event signal           */
  DCC_SIG_MASK_CHANGE_SIGNAL      =  5,   /* Indicates sig mask has changed*/

#ifdef TEST_FRAMEWORK
  #error code not present
#endif

/*---------------------------------------------------------------------------
  BEWARE: DO NOT CHANGE ENUMERATION VALUE FOR OFFLINE, STOP AND START SIGS.
---------------------------------------------------------------------------*/
  DCC_TASK_OFFLINE_SIGNAL         = 13,   /* Reserved TASK_OFFLINE_SIG     */
  DCC_TASK_STOP_SIGNAL            = 14,   /* Reserved TASK_STOP_SIG        */
  DCC_TASK_START_SIGNAL           = 15,   /* Reserved TASK_START_SIG       */

#ifdef FEATURE_QSAP_SERVICE
  DCC_QSAP_SIGNAL                 = 16,   /* Reserved for QSAP             */
#endif /* FEATURE_QSAP_SERVICE */

#ifdef FEATURE_8960_SGLTE_FUSION  
  DCC_DS_PROFILE_REMOTE_NOTIFY_CLIENT_SIGNAL  = 17, /* QCCI-notify wds service up ind */
  DCC_DS_PROFILE_REMOTE_SEND_MSG_WAIT_SIGNAL  = 18, /* QCCI Client Init Sig */
  DCC_DS_PROFILE_REMOTE_OS_TIMER_SIGNAL = 19, /* QCCI Timer expire sig */
#endif /* FEATURE_8960_SGLTE_FUSION */
  DCC_DSD_RDR_SIGNAL            = 20,   /* Reader sig for QMI DSD    */
  
  DCC_DPM_RM_WAIT_SIGNAL        = 21,   /* DPM Wait signal */
  DCC_DFS_RDR_SIGNAL            = 22,   /* Reader sig for QMI DFS    */
  DCC_ATP_RDR_SIGNAL            = 23,   /* Reader sig for QMI ATP    */
  DCC_DMS_RDR_SIGNAL            = 24,   /* Reader sig for QMI DMS   */
  DCC_OTT_RDR_SIGNAL            = 25,
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS */
#ifdef FEATURE_DATA_FLOW_MGMT
  DCC_DFC_RDR_SIGNAL            = 27,   /* Reader sig for QMI FC SYNC Service   */
#endif /* FEATURE_DATA_FLOW_MGMT */
#ifdef QWES_FEATURE_ENABLE
  DCC_QTEELS_CLIENT_SIGNAL      = 30,
#endif /* QWES_FEATURE_ENABLE  */  
  DCC_MAX_SIGNALS                         /* use next value for enum       */
} dcc_sig_enum_type;


/*---------------------------------------------------------------------------
  Data Task Command Definitions. These are the commands that can be sent to
  one.
---------------------------------------------------------------------------*/
typedef enum
{
  DCC_MIN_CMD                                  = -1,


  /*-------------------------------------------------------------------------
    Network interface - command to handle WWAN Rm state machine events
  -------------------------------------------------------------------------*/
  DCC_RMNET_SM_EV_CMD = 5,

  /*-------------------------------------------------------------------------
    Network interface - commands for RmNet
  -------------------------------------------------------------------------*/
  DCC_RMNET_SM_CMD = 6,

  /*-------------------------------------------------------------------------
    Network interface - QMI receive command
  -------------------------------------------------------------------------*/
  DCC_QMUX_RX_CMD = 10,

  /*-------------------------------------------------------------------------
    Network interface - command to QMI module
  -------------------------------------------------------------------------*/
  DCC_QMI_CMD = 11,

  /*-------------------------------------------------------------------------
    Network interface - command to QMI Charger module
  -------------------------------------------------------------------------*/
  DCC_QMI_CHARGER_CMD = 12,

  DCC_CM_MSG_IFACE_CMD = 13,

  DCC_QMI_VOICE_CMD = 14,

  DCC_QMI_VOICE_CM_IF_CMD = 15,

  DCC_QMI_PBM_CMD = 16,

  DCC_QMI_PBM_IF_CMD = 17,

  /*-------------------------------------------------------------------------
    MIP CCoA cmds
  -------------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    SLIP IFACE call control commands
  -------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_PS_SLIP
  DCC_SLIP_IFACE_HDLR_CP_DEACTIVATING_EV_CMD     = 80,
  DCC_SLIP_IFACE_HDLR_CP_DOWN_EV_CMD             = 81,
  DCC_SLIP_IFACE_HDLR_CP_UP_EV_CMD               = 82,
  DCC_SLIP_IFACE_HDLR_ADDR_FAILURE_EV_CMD        = 83,
  DCC_SLIP_IFACE_HDLR_ADDR_RELEASED_EV_CMD       = 84,
  DCC_SLIP_IFACE_HDLR_ADDR_CONFIG_SUCCESS_EV_CMD = 85,

#endif /* FEATURE_DATA_PS_SLIP */

  /*-------------------------------------------------------------------------
    Command for processing Logical Iface Callback
  -------------------------------------------------------------------------*/
  DCC_LOGICAL_IFACE_ASSOC_IFACE_EV_CMD  = 86,

  /*-------------------------------------------------------------------------
    Command for processing associated flow events in the logical flow framework
  -------------------------------------------------------------------------*/
  DCC_LOGICAL_FLOW_ASSOC_FLOW_EV_CMD    = 87,

  /*-------------------------------------------------------------------------
    NAT iface control/ALG specific commands
  -------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------
    QMI Service init and Request cmd
  -------------------------------------------------------------------------*/
  DCC_QMI_INIT_LEGACY_SERVICES_CMD     = 91,
  DCC_QMI_RECV_LEGACY_SERVICES_REQ_CMD = 92,
  DCC_RMNET_XPORT_CMD                  = 93,
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS */
#ifdef FEATURE_8960_SGLTE_FUSION
  DCC_DS_PROFILE_REMOTE_PERFORM_OPERATION_CMD = 107,
#endif /* FEATURE_8960_SGLTE_FUSION */

  DCC_DSD_SVC_CMD = 109,
  DCC_REV_IP_TRANSPORT_PERFORM_OPERATION_CMD = 110,
  DCC_DFS_SVC_CMD = 111,
  DCC_ATP_SVC_CMD = 112,

#ifdef FEATURE_DATA_PS_464XLAT
  DCC_CLAT_SM_START_CMD = 112,
  DCC_CLAT_SM_STOP_CMD = 113,
  DCC_CLAT_SM_IPV4_IFACE_UP_CMD = 114,
  DCC_CLAT_SM_IPV4_IFACE_DOWN_CMD = 115,
  DCC_CLAT_SM_IPV6_IFACE_UP_CMD = 116,
  DCC_CLAT_SM_IPV6_IFACE_DOWN_CMD = 117,
  DCC_CLAT_SM_IPV6_IFACE_PREFIX_UPDATE_CMD = 118,
  DCC_CLAT_SM_PLAT_AVAILABLE_CMD = 119,
  DCC_CLAT_SM_PLAT_UNAVAILABLE_CMD = 120,
  DCC_CLAT_SM_DNS_PROXY_ENABLED_CMD = 121,
  DCC_CLAT_SM_DNS_PROXY_DISABLED_CMD = 122,
  DCC_CLAT_SM_DNS64_CALLBACK_CMD = 123,
  DCC_CLAT_SM_DPM_CONTEXT_REGISTERED_CMD = 124,
  DCC_CLAT_SM_DPM_CONTEXT_DEREGISTERED_CMD = 125,
#endif /* FEATURE_DATA_PS_464XLAT */
  DCC_DMS_SVC_CMD = 126,
  DCC_OTT_SVC_CMD = 127,
  DCC_WDS_DELETE_ALL_PROFILES_CMD = 128,
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS */
#ifdef FEATURE_DATA_FLOW_MGMT
  DCC_DFC_SVC_CMD = 130,
#endif/* FEATURE_DATA_FLOW_MGMT */
#ifdef QWES_FEATURE_ENABLE
  DCC_QTEELS_CLIENT_CMD = 136,
  DCC_QTEELS_CLIENT_SERVICE_ERR_CMD = 137,
#endif /* QWES_FEATURE_ENABLE  */
  /* Always must be last */
  DCC_MAX_DEFINED_CMD_TYPES
} dcc_cmd_enum_type;

/*--------------------------------------------------------------------------
  Type definition for the signal handler.
  Returns TRUE if signal should be cleared from the set signal mask,
  FALSE if further signal processing is needed and hence signal
  should not be cleared
---------------------------------------------------------------------------*/

typedef boolean (*dcc_sig_handler_type)
(
  dcc_sig_enum_type sig,                   /* Signal to be processed       */
  void *user_data_ptr                      /* Parameter carrying user data */
);

/*--------------------------------------------------------------------------
  Type definition for the command handler
---------------------------------------------------------------------------*/

typedef void (*dcc_cmd_handler_type)
(
  dcc_cmd_enum_type cmd,        /* Actual command to be processed         */
  void *user_data_ptr           /* Command specific user parameters       */
);

/*--------------------------------------------------------------------------
  Data types to hold signal and cmd handlers
---------------------------------------------------------------------------*/

typedef struct
{
  dcc_sig_handler_type sig_handler;
  void *user_data_ptr;
} dcci_sig_handler_info_type;

/*--------------------------------------------------------------------------
  DCC Task Command queues and Command Data Buffers
---------------------------------------------------------------------------*/

typedef struct
{
  q_link_type       link;           /* For linking onto cmd queue          */
  dcc_cmd_enum_type cmd;            /* Actual command to be processed      */
  void              *user_data_ptr; /* Command specific user parameters    */
} dcci_cmd_type;

/*---------------------------------------------------------------------------
  Struct used for all QMI commands
---------------------------------------------------------------------------*/

typedef struct
{
  uint16  id;
} dcc_qmi_cmd_type;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DCC_TASK_DEFS_H */
