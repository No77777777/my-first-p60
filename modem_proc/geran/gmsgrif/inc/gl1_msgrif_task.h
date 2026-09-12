#ifndef GL1_MSGRIF_TASK_H
#define GL1_MSGRIF_TASK_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  GL1 MSGR Interface Header File

GENERAL DESCRIPTION
   This header file contains declarations relevant to L1 task processing.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmsgrif/inc/gl1_msgrif_task.h#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   -------- ---------------------------------------------
29/10/17   km      CR2133917 GL1 support for L+L
27/04/15   ws      CR815011 Featurise cfcm_cpu_monitor handling uder FEATURE_DUAL_DATA
09/04/15   jj      CR819956 Thor 2.x RF API change, G2W use MSGR
21/01/15   jj      CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK 
17/10/14   jj      CR741205 moving  G2X CNF from l1 task to MSGR 
07/04/14   cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
14/10/13   pa      CR536820: Added GL1 Msgr If Task. Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_msgrif_init.h"
#include "queue.h"
#include "geran_multi_sim.h"
#include "geran_tasks.h"
#include "l1_os.h"
#include "l1_task.h"
#include "l1i.h"

/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/
extern const geran_task_id_t gl1_msgrif_task_id[NUM_GERAN_DATA_SPACES];

extern q_type  gl1_msgrif_rf_task_msgr_q[NUM_GERAN_DATA_SPACES];
extern q_type  gl1_msgrif_rf_task_msgr_free_q[NUM_GERAN_DATA_SPACES];
extern gl1_msgrif_rf_task_msgr_t  gl1_msgrif_rf_task_msgr[NUM_GERAN_DATA_SPACES][GL1_MSGRIF_RF_TASK_MSGR_QUEUE_SIZE];

extern q_type gl1_msgrif_gfw_async_message_router_q[NUM_GERAN_DATA_SPACES];
extern q_type gl1_msgrif_gfw_async_message_router_free_q[NUM_GERAN_DATA_SPACES];
extern gl1_msgrif_gfw_async_msgr_t  gl1_msgrif_gfw_async_msgr[NUM_GERAN_DATA_SPACES][GL1_MSGRIF_GFW_ASYNC_MSGR_QUEUE_SIZE];

#if defined  (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS) || defined (FEATURE_GSM_TO_WCDMA_MSGR)
extern q_type gl1_msgrif_g2x_message_router_q[NUM_GERAN_DATA_SPACES];
extern q_type gl1_msgrif_g2x_message_router_free_q[NUM_GERAN_DATA_SPACES];
extern geran_gl1_msgr_t   gl1_msgrif_g2x_msgr[NUM_GERAN_DATA_SPACES][ GL1_MSGR_QUEUE_SIZE ]; 
void gl1_msgrif_handle_message_router_msgs (gas_id_t gas_id);
#endif  /* (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS)  */

extern q_type gl1_msgrif_vfrmap_message_router_q[NUM_GERAN_DATA_SPACES];
extern q_type gl1_msgrif_vfrmap_message_router_free_q[NUM_GERAN_DATA_SPACES];
extern geran_gl1_vfrmap_msgr_t   gl1_msgrif_vfrmap_msgr[NUM_GERAN_DATA_SPACES][ GL1_MSGR_QUEUE_SIZE ];

void gl1_msgrif_handle_vfrmap_msg (gas_id_t gas_id);

#ifdef FEATURE_DUAL_DATA
extern q_type gl1_msgrif_cfcm_cpu_monitor_router_q[NUM_GERAN_DATA_SPACES];
extern q_type gl1_msgrif_cfcm_cpu_monitor_router_free_q[NUM_GERAN_DATA_SPACES];
extern geran_gl1_msgr_t   gl1_msgrif_cfcm_cpu_monitor_msgr[NUM_GERAN_DATA_SPACES][ GL1_MSGR_QUEUE_SIZE ]; 
void gl1_msgrif_handle_cfcm_cpu_monitor_message_router_msgs (gas_id_t gas_id);
#endif
/*===========================================================================

                         FUNCTION DECLARATIONS

===========================================================================*/

void gsm_gl1_msgrif_task (dword dummy);
void gl1_msgrif_main (dword unused);
rex_tcb_type* gl1_msgrif_task_tcb_read(gas_id_t gas_id);

#endif /* GL1_MSGRIF_TASK_H */
