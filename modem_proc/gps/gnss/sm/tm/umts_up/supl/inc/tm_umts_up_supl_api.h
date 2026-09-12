#ifndef TM_SUPL_API_H
#define TM_SUPL_API_H

/*===========================================================================
  
                            TM_UMTS_UP_SUPL_API.H  
                   
DESCRIPTION

  
  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_up/supl/inc/tm_umts_up_supl_api.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
05/03/07    lt      Added this file header.
04/25/07    lt      Initial check in.
===========================================================================*/


#include "gps_variation.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "tm_prtl_iface.h"  
#include "tm_supl_asn1.h"
#include "tm_umts_up_supl.h"

void tm_umts_up_supl_init (void);


/************** API functions TmCore uses to access SUPL ****************/
 
prtl_start_sess_req_result_e_type tm_umts_up_supl_start_sess_req_handler
(
  tm_sess_handle_type               sess_handle,
  prtl_start_sess_req_param_s_type  *start_sess_req_param,
  tm_sess_req_start_param_s_type    *actions

);


boolean tm_umts_up_supl_stop_sess_req_handler
(
  tm_sess_handle_type           sess_handle,
  prtl_sess_stop_type           stop_type,  
  tm_sess_stop_reason_e_type    stop_reason
);


boolean tm_umts_up_supl_sess_req_data_handler
(
  tm_sess_handle_type  session_handle,
  prtl_data_req_type   req_data
);


boolean tm_umts_up_supl_sess_info_handler
(
  tm_sess_handle_type          session_handle,
  prtl_sess_info_e_type        sess_info_type,
  prtl_sess_info_param_u_type  *sess_info_param
);

#ifdef FEATURE_CGPS_PDCOMM

boolean tm_umts_up_supl_data_pipe_handler
(
  pd_comms_client_ipc_message_type  msg_type,
  void                              *msg_data_ptr
);

#endif

void tm_umts_up_supl_timer_cb_handler
(
  void *timerData
);


boolean tm_umts_up_supl_event_cb_handler
(
  prtl_event_e_type  event_type,
  prtl_event_u_type  *p_event_payload

);

boolean tm_umts_up_supl_general_query_handler
( prtl_query_e_type query_type, 
  prtl_ans_u_type *answer_ptr 
);

#ifdef FEATURE_CGPS_WCDMA
void tm_umts_supl_api_rrc_cell_info_proc
( rrcmeas_plmn_cell_info_struct_type cell_id_info,
  rrc_cell_id_status_type            status
);
#endif /* FEATURE_CGPS_WCDMA */

boolean tm_umts_up_supl_api_close_session
(tm_sess_stop_reason_e_type  stop_reason);

boolean tm_umts_up_supl_gm_event_handler
(
  uint32  msg_type,
  void    *msg_data_ptr
);

/*****************************************************************************
  BEGIN - Proprietary External Functions
*****************************************************************************/
#ifdef FEATURE_CGPS_UMTS_UP_SUPL
extern wl1_lsm_cell_meas_update_type       z_DBCellMeas;

/********************************************************************
*
* tm_umts_up_supl_cell_db_update
*
* Function description:
*   Processing cell db update from MGP: WCDMA, LTE or OOS.
*
* Parameters:
*
* Return:
*  None
*
*********************************************************************
*/
extern void tm_umts_up_supl_update_mlid_db (const cgps_CellInfoCachedStruct *cell_db_data_ptr );

/*
******************************************************************************
* tm_umts_up_supl_update_mlid_db
*
* Function description:
*
*   Update multi-location ID database
*
* Parameters: cell_db_data_ptr point to the cell database info.
*
* Return value: None.
*
******************************************************************************
*/
extern void tm_umts_up_supl_cell_db_update( const cgps_CellInfoCachedStruct *cell_db_data_ptr );

#ifdef FEATURE_CGPS_LTE_CELLDB
/*===========================================================================

FUNCTION
  tm_umts_up_supl_lte_mrl_update

DESCRIPTION
  This function copies the cell-measurements sent from the LTE L1, into the
  database (if they are valid).

PARAMETERS

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None.

===========================================================================*/

extern void tm_umts_up_supl_lte_mrl_update(lte_ml1_gps_mrl_cell_meas_update_t *cell_meas_update_ptr, lte_ml1_gps_mrl_cell_meas_update_t *z_lte_mrl);
#endif  /* FEATURE_CGPS_LTE_CELLDB */

/********************************************************************
 *
 * tm_umts_up_supl_cell_info_construct
 *
 * Function description:
 *   Construct cell information used by several SUPL messages.
 *
 * Parameters:
 *   p_asn1_cell_info      : pointer to a structure that contains 
 *                           memory allocated for the ASN1 tool
 *
 *   p_cell_info_allocated : pointer to a structure to hold the
 *                           to-be-constructed cell information
 *
 * Return: TRUE/FALSE ( FALSE: no valid cell ID )
 *********************************************************************
 */
extern boolean tm_umts_up_supl_cell_info_construct(supl_CellInfo *p_asn1_cell_info,
                                                   supl_cell_info_struct_type *p_cell_info_allocated);

/********************************************************************
 *
 * tm_umts_up_supl_chk_and_build_all_mlid
 *
 * Function description:
 *  utility function to construct multiple location ID used by several
 *  SUPL messages.
 *
 * Parameters:
 *   p_supl_LocationIdData - pointer to a dynamically allocated array
 *      of structure of supl_LocationIdData.
 *
 *  p_supl_LocationId -- pointer to a dynamically allocated array
 *      of structure of supl_LocationData.
 *
 *  p_cell_info_allocated - pointer to a dynamically allocated array
 *     of structure supl_cell_info_struct_type.
 *
 *  u_num_allocated_array_elements - number of array elements allocated
 *     for each of the above three arrays.
 *
 * Return: number of Multiple Location ID entries
 *
 *********************************************************************
 */
extern uint8 tm_umts_up_supl_chk_and_build_all_mlid(supl_LocationIdData *p_supl_LocationIdData,
                                                    supl_LocationId *p_supl_LocationID,
                                                    supl_cell_info_struct_type *p_cell_info_allocated,
                                                    uint8 u_num_allocated_array_elements);

/********************************************************************
 *
 * tm_umts_up_supl_mlid_construct
 *
 * Function description:
 *  utility function to populate the structure of one supl_CellInfo in MLID.
 *
 * Parameters:
 *   p_asn1_cell_info - pointer to a structure to hold the to-be-constructed
 *       cell information.
 *
 *   p_cell_info_allocated -- pointer to a structure that contains memory
 *      allocated for the ASN1 tool
 * Return: TRUE/FALSE ( FALSE: no valid cell ID )
 *********************************************************************
 */
extern boolean tm_umts_up_supl_mlid_construct(supl_CellInfo *p_asn1_cell_info,
                                              supl_cell_info_struct_type *p_cell_info_allocated,
                                              uint8 u_mlid_buffer_index);

/*===========================================================================

FUNCTION
  tm_umts_up_supl_cell_meas_update

DESCRIPTION
  This function copies the cell-measurements sent from the WCDMA L1, into the
  database (if they are valid).

PARAMETERS

DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None.

===========================================================================*/
extern void tm_umts_up_supl_cell_meas_update(wl1_lsm_cell_meas_update_type   *cell_meas_update_ptr);
#endif /* FEATURE_CGPS_UMTS_UP_SUPL */
/*****************************************************************************
  END - Proprietary External Functions
*****************************************************************************/

/********************************************************************
*
* supl_cell_info_sanity_chk_ok
*
* Function description:
*   Sanity checking cell db update from MGP: WCDMA or LTE.
*
* Parameters:
*
* Return:
*  Boolean: TRUE - sanity check passed; FALSE - sanity check failed.
*
*********************************************************************
*/
boolean supl_cell_info_sanity_chk_ok(const cgps_CellInfoCachedStruct *cell_db_data_ptr);

/*
******************************************************************************
* static boolean supl_SuplPos_rrlp_tx
*
* Function description:
*
* Encode and transmit SUPL POS with RRLP payload.
* measurements.
*
* Parameters: p_rrlp_data - pointer to rrlp payload;
*             w_byte_cnt - num. of bytes in rrlp payload;
*             p_velocity - pointer the struture holding velocity (null if not available )
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
boolean supl_SuplPos_rrlp_tx 
(
  uint8         *p_rrlp_data,
  uint16        w_byte_cnt,
  supl_Velocity *p_velocity
);

#ifdef __cplusplus
}
#endif

#endif /* TM_SUPL_API_H */

