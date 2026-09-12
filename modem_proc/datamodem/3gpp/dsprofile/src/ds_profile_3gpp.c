/******************************************************************************
  @file    ds_profile_3gpp.c
  @brief   

  DESCRIPTION
  Tech specific implementation of 3GPP Profile Management  

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2010-2014 QUALCOMM technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsprofile/src/ds_profile_3gpp.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/27/12   vs      Removed ds_profile ENTRY and EXIT log messages
01/27/12   nd      Cleaning redundant param validation in set functions.
09/13/11   ttv     Changes to add src_stat_desc as profile parameter.
05/17/11   ss      Added profile parameter validation for pdp_type.
02/28/11   hs      Added changes to support APN class in profile
02/24/11   ttv     Fixed compilation warnings.
01/26/11   ars     Added support for apn_disabled_flag and inactivity_timer_val
01/17/11   ss      Added support for Iface linger.
09/28/10   ss      NULL terminating variable length strings in Get APIs.
09/03/10   ss      Fixed issue in length field of variable length parameters 
                   updated wrongly in ds_profile_3gpp get APIs.
08/24/10   sa      Added support for zero length param.
02/07/10   ss      Reset string type PDP profile parameters before updating new 
                   value.
06/28/10   ss      Fixed compiler warnings.
05/17/10   hs      Added APN disable and PDN inactivity feature support.
04/30/10   ars     Made modem_mem_alloc/free and str fns platform independent
10/15/09   vk      Added support for LTE params
09/30/09   mg      Created the module. First version of the file.
===========================================================================*/
#include "datamodem_variation.h"
#include "ds_profile_3gppi.h"
#include "ds_profile_db_handler.h"
#include "ds_profile_os.h"
#include "dsumtspdpreg.h"
#include "dsumtspsqos.h"

#include "ds_3gpp_qos.h"
#include "ds_3gpp_pdn_context.h"
#include "dstask_v.h"

/*---------------------------------------------------------------------------
                       UTILITY MACROS
---------------------------------------------------------------------------*/

/*lint -save -e641*/

/* Macro to check info (for get_param function)
  (info->buf not NULL and validate info->len) */
#define GET_INFO_IS_VALID( info, max_len ) \
  ( (info != NULL) && (info->buf != NULL) && (info->len >= max_len) )

/* Macro to check info (for set_param function)
  (info->buf not NULL and validate info->len) */
#define SET_INFO_IS_VALID( info, max_len ) \
  ( (info->buf != NULL) && ( (info->len > 0) && (info->len <= max_len) ) )

/* 3GPP QoS Parameters */
#define DS_PROFILE_3GPP_QOS_PARAM_MAX_UL_BITRATE   QOS_MAX_UL_KBIT_RATE
#define DS_PROFILE_3GPP_QOS_PARAM_MAX_DL_BITRATE   QOS_MAX_DL_KBIT_RATE

#define DS_PROFILE_3GPP_QOS_PARAM_MAX_GTD_UL_BITRATE  QOS_MAX_UL_KBIT_RATE
#define DS_PROFILE_3GPP_QOS_PARAM_MAX_GTD_DL_BITRATE  QOS_MAX_DL_KBIT_RATE

#define DS_PROFILE_3GPP_QOS_PARAM_MAX_SDU_SIZE     QOS_PARAM_MAX_SDU_SIZE

#define DS_PROFILE_3GPP_QOS_PARAM_MAX_TRANS_DELAY  QOS_PARAM_MAX_TRANS_DELAY

#define DS_PROFILE_3GPP_QOS_PARAM_MAX_THANDLE_PRIORITY  \
                                               QOS_PARAM_MAX_THANDLE_PRIORITY

#define DS_PROFILE_3GPP_GPRS_QOS_PARAM_MAX_RELIABILITY_CLASS  \
                                               QOS_PARAM_MAX_RELIABILITY_CLASS

#define DS_PROFILE_3GPP_GPRS_QOS_PARAM_MAX_DELAY_CLASS        \
                                               QOS_PARAM_MAX_DELAY_CLASS

#define DS_PROFILE_3GPP_GPRS_QOS_PARAM_MAX_PRECEDENCE_CLASS   \
                                               QOS_PARAM_MAX_PRECEDENCE_CLASS

#define DS_PROFILE_3GPP_GPRS_QOS_PARAM_MAX_PEAK_THROUGHPUT_CLASS   \
                                            QOS_PARAM_MAX_PEAK_THROUGHPUT_CLASS

#define DS_PROFILE_3GPP_GPRS_QOS_PARAM_MAX_MEAN_THROUGHPUT_CLASS   \
                                            QOS_PARAM_MAX_MEAN_THROUGHPUT_CLASS

/* LTE QoS parameters */
#define DS_PROFILE_3GPP_LTE_QOS_PARAM_MAX_UL_BITRATE   QOS_LTE_MAX_UL_KBIT_RATE
#define DS_PROFILE_3GPP_LTE_QOS_PARAM_MAX_DL_BITRATE   QOS_LTE_MAX_DL_KBIT_RATE

#define DS_PROFILE_3GPP_LTE_QOS_PARAM_MAX_GTD_UL_BITRATE  QOS_LTE_MAX_UL_KBIT_RATE
#define DS_PROFILE_3GPP_LTE_QOS_PARAM_MAX_GTD_DL_BITRATE  QOS_LTE_MAX_DL_KBIT_RATE

/*-----------------------------------------------------------------------------
         LOCAL definitions and declarations for module
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
          3GPP PARAMS Table internal to DSI_PROFILE_3GPP module
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
  List of callback function tables
---------------------------------------------------------------------------*/
list_type ds_profile_3gpp_callback_table_list = {0};

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_UPDATE_LTE_ATTACH_PDN_PROFILES
 
DESCRIPTION
  This function is triggered when any profile resides in the attach profile
  list changed. Recalculation for attach profile will be called.

PARAMETERS 


DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT
  DS_PROFILE_REG_RESULT_ERR_LEN_INVALID
  DS_PROFILE_REG_RESULT_SUCCESS
 
SIDE EFFECTS 
  
===========================================================================*/
#ifdef FEATURE_DATA_LTE
static ds_profile_status_etype dsi_profile_3gpp_update_lte_attach_pdn_profiles(ds_profile_subs_etype subs)
{
  ds_cmd_type             *cmd_ptr = NULL;
  sys_modem_as_id_e_type  *data_ptr = NULL;

  /*------------------------------------------------------------------
  send msg to DS task to refresh attach pdn list in cache
  -------------------------------------------------------------------*/
  DS_PROFILE_LOGD("dsi_profile_3gpp_update_lte_attach_pdn_profiles",0);
  cmd_ptr = ds_allocate_cmd_buf(sizeof(sys_modem_as_id_e_type));
  if( (NULL == cmd_ptr) || (NULL == cmd_ptr->cmd_payload_ptr) )
  {
  	return DS_PROFILE_REG_RESULT_FAIL;
  }

  data_ptr = (sys_modem_as_id_e_type*)cmd_ptr->cmd_payload_ptr;
  *data_ptr = (sys_modem_as_id_e_type)(subs - 1);
  cmd_ptr->hdr.cmd_id = DS_CMD_DSD_APM_REFRESH_ATTACH_PDN_LIST;
  ds_put_cmd(cmd_ptr);
  return DS_PROFILE_REG_RESULT_SUCCESS;
}
#endif /*FEATURE_DATA_LTE*/

#if 0
/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_GET_PARAM
 
DESCRIPTION
  This function is used to get 3GPP parameter value from the local copy
  of the profile

PARAMETERS 
  blob : pointer to local copy of profile
  ident : identifier to get value
  info : pointer to store value of identifier fetched

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT
  DS_PROFILE_REG_RESULT_ERR_LEN_INVALID
  DS_PROFILE_REG_RESULT_SUCCESS
 
SIDE EFFECTS 
  
===========================================================================*/
static ds_profile_status_etype dsi_profile_3gpp_get_param(
  void                        *blob,
  ds_profile_identifier_type   ident,
  ds_profile_info_type        *info
)
{
  uint8 index = 0;
  /* Validate identifier */
  if ( !DS_PROFILE_3GPP_IDENT_IS_VALID( ident ) )
  {
    ident = DS_PROFILE_3GPP_PROFILE_PARAM_INVALID; 
    return DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT;
  }
  index = dsi_profile_3gpp_get_index_from_ident( ident );
  /* Validate info->buf and info->len */
  if ( !GET_INFO_IS_VALID( info, ds_profile_3gpp_profile_params_desc_tbl[index].len ) ) 
  {
    return DS_PROFILE_REG_RESULT_ERR_LEN_INVALID;
  }
  info->len = ds_profile_3gpp_profile_params_desc_tbl[index].len;
  DS_PROFILE_LOGD("get 3gpp_set_param:calling ds_profile_3gpp_acc_mut_fn_tbl set_fn",0);
  return ds_profile_3gpp_acc_mut_fn_tbl[index].get_fn(blob, info);
}

#endif

/*===========================================================================
FUNCTION DS_PROFILE_3GPP_GET_CALLBACK_TABLE_LIST
 
DESCRIPTION
  This function is used to get callback table list.

PARAMETERS
  void

DEPENDENCIES 
  
RETURN VALUE 
  Pointer to Callback Table List
 
SIDE EFFECTS 
 
===========================================================================*/
list_type* ds_profile_3gpp_get_callback_table_list
(  
  void
)
{
  return (&ds_profile_3gpp_callback_table_list);
}/* ds_profile_3gpp_get_callback_table_list */

/*===========================================================================

FUNCTION DS_PROFILE_3GPP_REGISTER_CALLBACK_TABLE

DESCRIPTION
  This function registers a callback function table of a client
  with DS profile.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  
===========================================================================*/
void ds_profile_3gpp_register_callback_table
(
  ds_profile_3gpp_cb_func_tbl_type  *cb_tbl_ptr /* Callback function table */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( cb_tbl_ptr != NULL )
  {
    /*----------------------------------------------------------------------
      Push the list item into the list
    ----------------------------------------------------------------------*/
    list_push_front(&(ds_profile_3gpp_callback_table_list), &(cb_tbl_ptr->link));
  }
  return;
} /* ds_profile_3gpp_register_callback_table() */

 /*===========================================================================

FUNCTION DS_PROFILE_3GPP_DEREGISTER_CALLBACK_TABLE

DESCRIPTION
  This function deregisters a callback function table of a client
  with DS profile.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  
===========================================================================*/
void ds_profile_3gpp_deregister_callback_table
(
  ds_profile_3gpp_cb_func_tbl_type  *cb_tbl_ptr /* Callback function table */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( cb_tbl_ptr != NULL )
  {
    /*----------------------------------------------------------------------
      Pop the item from the list
    ----------------------------------------------------------------------*/
    list_pop_item(&(ds_profile_3gpp_callback_table_list),
                   &(cb_tbl_ptr->link));
  }
  return;
} /* ds_profile_3gpp_deregister_callback_table() */


/*===========================================================================
FUNCTION DS_PROFILE_3GPP_INIT

DESCRIPTION
  This function is called on the library init. It initializes the function
  pointers to valid functions for 3gpp

PARAMETERS 
  fntbl : pointer to table of function pointers

DEPENDENCIES 
  
RETURN VALUE 
  returns the mask for 3gpp. (Used later as valid mask which is ORed value
  returned from all techs.
SIDE EFFECTS 
  
===========================================================================*/

uint8 ds_profile_3gpp_init ( tech_fntbl_type *fntbl )
{
  DS_PROFILE_LOGD("ds_profile_3gpp_init:initializing function pointers",0);
  /* Init function pointers */
  fntbl->dealloc   = ds_profile_db_handler_dealloc;
  fntbl->set_param = ds_profile_db_handler_set_param;
  fntbl->get_param = ds_profile_db_handler_get_param;
#ifdef FEATURE_DATA_LTE
  fntbl->update_lte_attach_pdn_list_profiles = 
	                        dsi_profile_3gpp_update_lte_attach_pdn_profiles;
#endif
  /*----------------------------------------------------------------------------
    Initialize the list to hold handler tables of clients of DS Profile
  ----------------------------------------------------------------------------*/
  list_init(&(ds_profile_3gpp_callback_table_list));
  return (0x01 << DS_PROFILE_TECH_3GPP);
}

/*lint -restore Restore lint error 641*/

