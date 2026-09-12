/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  TM-RRLP-UP-SUPL sub-module

GENERAL DESCRIPTION
  This file presents TM RRLP-UP SUPL sub-module interface that is called by
  the SUPL module (tm_umts_up_supl.c).

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2017 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_up/supl/src/tm_rrlp_up.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  02/10/17   SJ      Initial version

============================================================================*/
#include "comdef.h"
#include "customer.h"   /* Customer configuration file */

#ifndef TM_RRLP_UP_H
#define TM_RRLP_UP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gnss_common.h"
#include "gps_variation.h"
#include "tm_umts_common_utils.h"
#include "tm_umts_up_supl.h"
#include "tm_supl_asn1.h"



/********************************************************************
*
* supl_send_pos_rrlp_to_slp
*
* Function description:
*   Send position in RRLP wrapped by SUPL_POS to SLP
*
* Parameters:
*   p_gad_shape_pos: pointer to the array holding the GAD shape;
*   p_velocity : pointer to a structure of the format for velocity
*                in SUPL POS
*   q_timestamp_ms : gps time of the position in msec.
*   q_pos_sources: GPS and/or GLONASS
*   q_glonass_tod_modulo_1hour_ms: GLONASS time stamp TimeOfDay
*
* Return:
*  None
*
*********************************************************************
*/
void supl_send_pos_rrlp_to_slp
( 
  supl_status_s_type           *p_supl_session_status,
  gad_ellip_alt_unc_ellip_type *p_gad_shape_pos,
  supl_Velocity *p_velocity,
  uint32 q_timestamp_ms,
  uint32 q_pos_sources,
  uint32 q_glonass_tod_modulo_1hour_ms 
);


/*
******************************************************************************
* supl_SuplPos_rrlp_proc
*
* Function description:
*
* Process incoming SUPL POS message containing RRLP payload
*
* Parameters: p_payload: pointer to SUPL POS RRLP payload
*                        payload_length: Length of the payload
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
boolean supl_SuplPos_rrlp_proc
(
  supl_status_s_type   *p_supl_session_status, 
  uint8 *p_payload, 
  const uint16 payload_length
);

/*
 ******************************************************************************
 *
 * tm_umts_up_rrlp_handle_prm_data
 *
 * Function description:
 *   Process the cached measurements from the SUPL session structure & report the qualified
 *   measurement to RRLP server over SUPL.
 *
 * Parameters:
 *  p_supl_session_status - Pointer to SUPL session structure
 *
 * Return value:
 *  True if the measurement are encoded & sent to server. Otherwise False.
 *
 ******************************************************************************
*/
boolean tm_umts_up_rrlp_handle_prm_data( supl_status_s_type *p_supl_session_status );

#ifdef __cplusplus
}
#endif


#endif

