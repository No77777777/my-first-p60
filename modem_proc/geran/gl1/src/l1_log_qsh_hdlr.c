/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            L1 Logging Facilities

GENERAL DESCRIPTION
   This module contains functions for logging L1 packets.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2014 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_log_qsh_hdlr.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/02/18   ap       CR2022526 X tech SUB1 page denied by GSM SUB2 when reading PCH in Transfer
16/08/17   km     CR2089992 DTF handler to abort G2L IRAT search in idle mode
15/11/16   sm     CR1089963 Added state transition events and BCCH abort in W2G stare handler, to reproduce issue CR/1082619 
16/03/17   mn     CR2020307 FR40846:Packet transfer Error Recovery mechanism.
21/07/16   sm     CR1044487 Added DTF handler to trigger panic reset.
30/06/16   sm     CR1035750 Added common event to logging all failure events.
13/06/16   nm     CR1024602 PACKET Transfer DTF Events/Handlers Phase2
31/05/16   mn     CR1022253 Idle DTF/Handlers
18/5/16    sn     CR1007565 PACKET Transfer DTF Events/Handlers Phase1
13/05/16   km     CR1012528 DTF sleep scenario "Forced error Recovery"
12/05/16  akm     CR1010520 DTF sleep scenario "PCH and NCELL activity cancellation" 
09/05/16  sm      CR1002861 Added QSH GL1 handler : PCH collision with Ncell activity,TRM updation(reserve/cancel)and continuous Ncell activity failure  .  
08/09/15  sk      QSH handlers for debu
===========================================================================
30/09/15   sk     CR916227 Disable frequent QSH events and add PCH rsv handler
10/09/15   sk     CR899847 GERAN changes to trigger QSH events and handler for debugging
===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "qsh_util.h"
#include "comdef.h"
#include "geran_variation.h"
#include "ms.h"
#include "l1_log.h"         // This module's header file
#include "log.h"            // For log_commit() and log_alloc() and log codes
#include "diag.h"           // Needed for DIAG_DIAGVER prior to calling event.h
#include "event.h"          // For logging RAR event and event codes
#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"  // For GSM log codes
#include "trm.h"

#include "l1i.h"            // For TSKISR struct (for l1_state)
#include "l1_sc.h"          // for l1_sc_globals
#include "gl1_sys_params.h" // for SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16
#include <string.h>         // for memset
#include "gtmrs.h"
#include "gtmrs_g.h"        // For GSTMR_GET_FN_GERAN( gas_id) & SUB_FN()

#include "gpl1_gprs_log.h"  // for convert_txlev_to_pwr_dBm()
#include "l1_utils.h"
#include "gl1_msgi.h"
#include <stringl/stringl.h>

#include "l1_task.h"
#include "l1_drx.h"


#include "l1_isr.h"
#include "gl1_hw_sleep_ctl.h"


#include "geran_multi_sim.h"
#include "geran_dual_sim.h"
#include "l1_log_qsh.h"
#include "gl1_msg_g.h"
#include "geran_msgs.h"
#include "gpl1_gprs_utils.h"
#include "gl1_mutex.h"



#include "gsmdiag.h"
#include "l1_fm.h"
#if defined ( FEATURE_QSH_EVENT_NOTIFY_TO_QSH ) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
boolean qsh_sch_acq_success[NUM_GERAN_DATA_SPACES] = {TRUE};
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif




