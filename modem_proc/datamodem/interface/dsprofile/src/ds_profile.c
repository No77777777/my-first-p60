/******************************************************************************
  @file    ds_profile.c
  @brief   DS PROFILE API implementation

  DESCRIPTION
  This file contains implementation of DS PROFILE API.

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2009-2019 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsprofile/src/ds_profile.c#5 $ $DateTime: 2023/01/31 00:56:07 $ $Author: pwbldsvc $

when	   who     what, where, why
--------   ---     ----------------------------------------------------------
11/27/12   vs      Removed ds_profile ENTRY and EXIT log messages
05/31/11   bvd     Added changes for supporting non-persistent profile and other profile attributes
02/16/11   ttv     Added DSDS changes for consolidated profile family.
07/14/10   asn     Address initialization of DS Profile and UMTS Profile Reg
04/30/10   ars     Made modem_mem_alloc/free and str fns platform independent
09/30/09   mg      Created the module. First version of the file.
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "datamodem_variation.h"
#include "ds_profile_plm.h"
#include "ds_profile_os.h"
#include "ds_profilei.h"
#include "ds_profile_server.h"
#include "ds_profile_remote_client.h"
#include "ps_crit_sect.h"
#include "ds_profile_db.h"
#include "ds_profile.h"
#include "ds_profile_3gpp.h"
#include "ds_profile_3gpp2.h"
#include "ds_profile_db_handler.h"
#include "ps_logging_helper.h"
#include "ps_utils.h"

#define DS_PROFILE_LIB_STATE_INVAL   0x00
#define DS_PROFILE_LIB_STATE_INITED  0x01
#define MAX_LIB_INST_NAME   		 10
static char 		  lib_state;

/* Platform specific lock */
static plm_lock_type  lib_lock, callback_lock;
static uint32   	  instance;
static boolean ds_profile_modified = FALSE;

static ds_util_list_hndl_type callbacks_list;

static char logging_prefix[MAX_LIB_INST_NAME] = "PRFREG_";


/* node to store callback info, used internally */
typedef struct {
  ds_profile_cb_handle_type cb_handle; /* should be first member */
  ds_profile_db_callback_id_type db_cb_handle;
  union
  {
    ds_profile_cb_type cback;
    ds_profile_cb_type_per_sub cback_per_sub;
  }ds_profile_call_back;
  ds_profile_tech_etype tech;
  void *user_data;
  ds_profile_db_subs_type subs_id;
  boolean cb_per_sub;
} ds_profile_cback_func_info_node;

/*---------------------------------------------------------------------------
                               UTILITY MACROS
---------------------------------------------------------------------------*/
/*lint -save -e655*/
/* Macro to check if profile handle is valid   */
#define HNDL_IS_VALID( hndl ) \
		  ( ( hndl ) != NULL )

/* Macro to check if lib is inited  		   */
#define LIB_STATE_IS_VALID( lib_state ) \
  ( lib_state == DS_PROFILE_LIB_STATE_INITED )

/* Macro to check if transation type is valid  */
#define TRN_IS_VALID( trn ) \
  ( trn & DS_PROFILE_TRN_VALID )

/* Macro to check if list_type is valid 	   */
#define LIST_TYPE_IS_VALID( lst ) \
  ( lst != NULL )

/* Macro to check if itr is valid   		   */
#define ITR_IS_VALID( itr ) \
  ( itr != NULL )

/* TEST_FRAMEWORK for QTF   				   */
#if 0
/* Macro to acquire lock					   */
#define ACQ_LOCK( msg ) \
  if ( ds_profile_lock_acq( lib_lock ) != 0 ) \
  { \
	DS_PROFILE_LOGE( msg , 0 ); \
	DS_PROFILE_LOGE( "FAIL: unable to acquire lock", 0 ); \
	return DS_PROFILE_REG_RESULT_FAIL; \
  }

/* Macro to acquire lock					   */
#define ACQ_LOCK_CALLBACK( msg ) \
  if ( ds_profile_lock_acq( callback_lock ) != 0 ) \
{ \
  DS_PROFILE_LOGE( msg , 0 ); \
  DS_PROFILE_LOGE( "FAIL: unable to acquire callback lock", 0 ); \
  return DS_PROFILE_REG_RESULT_FAIL; \
}

/* Macro to release lock					   */
#define REL_LOCK( msg ) \
  if ( ds_profile_lock_rel( lib_lock ) != 0 ) \
  { \
	DS_PROFILE_LOGE( msg , 0 ); \
	DS_PROFILE_LOGE( "FAIL: unable to release lock", 0 ); \
	return DS_PROFILE_REG_RESULT_FAIL; \
  }

/* Macro to release lock					   */
#define REL_LOCK_CALLBACK( msg ) \
  if ( ds_profile_lock_rel( callback_lock ) != 0 ) \
  { \
	DS_PROFILE_LOGE( msg , 0 ); \
	DS_PROFILE_LOGE( "FAIL: unable to release callback lock", 0 ); \
	return DS_PROFILE_REG_RESULT_FAIL; \
  }

#else
/* Macro to acquire lock					   */
#define ACQ_LOCK( msg ) \
  if ( ds_profile_lock_acq( &lib_lock ) != 0 ) \
  { \
	DS_PROFILE_LOGE( msg , 0 ); \
	DS_PROFILE_LOGE( "FAIL: unable to acquire lock", 0 ); \
	return DS_PROFILE_REG_RESULT_FAIL; \
  }

#define ACQ_LOCK_CALLBACK( msg ) \
  if ( ds_profile_lock_acq( &callback_lock ) != 0 ) \
  { \
	DS_PROFILE_LOGE( msg , 0 ); \
	DS_PROFILE_LOGE( "FAIL: unable to acquire callback lock", 0 ); \
	return DS_PROFILE_REG_RESULT_FAIL; \
  }

/* Macro to release lock					   */
#define REL_LOCK( msg ) \
  if ( ds_profile_lock_rel( &lib_lock ) != 0 ) \
  { \
	DS_PROFILE_LOGE( msg , 0 ); \
	DS_PROFILE_LOGE( "FAIL: unable to release lock", 0 ); \
	return DS_PROFILE_REG_RESULT_FAIL; \
  }

/* Macro to release lock					   */
#define REL_LOCK_CALLBACK( msg ) \
  if ( ds_profile_lock_rel( &callback_lock ) != 0 ) \
  { \
	DS_PROFILE_LOGE( msg , 0 ); \
	DS_PROFILE_LOGE( "FAIL: unable to release callback lock", 0 ); \
	return DS_PROFILE_REG_RESULT_FAIL; \
  }
#endif

#define FAMILY_IS_VALID(family) \
  ( family == DS_PROFILE_EMBEDDED_PROFILE_FAMILY  || \
	family == DS_PROFILE_TETHERED_PROFILE_FAMILY )


/*===========================================================================      
                  Internal helper functions
===========================================================================*/
static ds_profile_db_supported_tech_mask legacy_tech_to_techmask(
  ds_profile_tech_etype tech
  )
{
  switch (tech)
  {
  case(DS_PROFILE_TECH_EPC):
    return (DB_TECH_TO_TECHMASK(DB_TECH_3GPP) |
              DB_TECH_TO_TECHMASK(DB_TECH_3GPP2) |
              DB_TECH_TO_TECHMASK(DB_TECH_COMMON));
  default:
    return (DB_TECH_TO_TECHMASK(tech));
  }
}

static ds_profile_db_profile_num_type legacy_num_to_db_num(
  ds_profile_num_type num
  )
{
  return (DS_PROFILE_UNSPECIFIED_NUM == num) ? (DB_UNSPECIFIED_PROFILE_NUM) : num;
}

static ds_profile_identifier_type db_ident_to_legacy_ident(
  ds_profile_db_ident_type ident,
  ds_profile_tech_etype legacy_tech
  )
{
  switch (ident)
  {
  case (GENERAL_PROFILE_PARAM_PROFILE_NAME):
    return (DS_PROFILE_TECH_3GPP2 == legacy_tech ?
            DS_PROFILE_3GPP2_PROFILE_PARAM_INVALID :
            DS_PROFILE_3GPP_PROFILE_PARAM_PROFILE_NAME);

  case (GENERAL_PROFILE_PARAM_APN_NAME):
    return (DS_PROFILE_TECH_3GPP2 == legacy_tech ?
            DS_PROFILE_3GPP2_PROFILE_PARAM_APN_STRING :
            DS_PROFILE_3GPP_PROFILE_PARAM_PDP_CONTEXT_APN);

  case (GENERAL_PROFILE_PARAM_SUBS_ID):
    return (DS_PROFILE_TECH_3GPP2 == legacy_tech ?
            DS_PROFILE_3GPP2_PROFILE_PARAM_INVALID :
            DS_PROFILE_3GPP_PROFILE_PARAM_SUBS_ID);

  case (GENERAL_PROFILE_PARAM_IPV6_DELEGATION):
    return DS_PROFILE_GENERAL_PROFILE_PARAM_IPV6_PREFIX_DELEGATION_FLAG;

    case (GENERAL_PROFILE_PARAM_CLAT_ENABLE_FLAG):
      return DS_PROFILE_GENERAL_PROFILE_PARAM_CLAT_ENABLE_FLAG;

  default:
    return ident;
  }
}

static ds_profile_tech_etype ds_techmask_to_legacy_tech(
  ds_profile_db_supported_tech_mask tech_mask)
{
  switch (tech_mask)
  {
  case(DB_TECH_TO_TECHMASK(DB_TECH_3GPP)):
    return DS_PROFILE_TECH_3GPP;
  case(DB_TECH_TO_TECHMASK(DB_TECH_3GPP2)):
    return DS_PROFILE_TECH_3GPP2;
  case(DB_TECH_TO_TECHMASK(DB_TECH_3GPP) |
         DB_TECH_TO_TECHMASK(DB_TECH_3GPP2) |
         DB_TECH_TO_TECHMASK(DB_TECH_COMMON)):
    return DS_PROFILE_TECH_EPC;
  default:
    return DS_PROFILE_TECH_INVALID;
  }
}

static ds_profile_status_etype ds_profile_validate_subs_id(
  ds_profile_subs_etype subs_id)
{
  if ((subs_id <= DS_PROFILE_ACTIVE_SUBSCRIPTION_NONE) || (subs_id > DS_PROFILE_ACTIVE_SUBSCRIPTION_MAX))
  {
    DS_PROFILE_LOGE("Invalid Subs id passed", 0);
    return DS_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID;
  }
  return DS_PROFILE_REG_RESULT_SUCCESS;
}

/*===========================================================================
FUNCTION DS_PROFILE_POWER_UP_INIT

DESCRIPTION
  This function performs the DS Profile power up init.

PARAMETERS

DEPENDENCIES 
  
RETURN VALUE 

SIDE EFFECTS 
 
===========================================================================*/
void ds_profile_power_up_init(void)
{
  (void)ds_profile_lock_init(&lib_lock);
  (void)ds_profile_lock_init(&callback_lock);
  return;
}

/*===========================================================================
FUNCTION DS_PROFILE_INIT_LIB

DESCRIPTION
  This function initializes the DS profile library. On modem, this function is 
  called only once at initialization. This will initialize the library for
  that process domain.

PARAMETERS

DEPENDENCIES 
  ds_profile_power_up_init should have been called already
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS  : On successful operation
  DS_PROFILE_REG_RESULT_FAIL	 : On general errors. This return code 
                                   provides blanket coverage
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_init_lib(
  void
  )
{
  uint8 tech_mask = 0;
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  char lib[MAX_LIB_INST_NAME + 5];

  /* The flag below is used to ensure that atexit() function is called only once */
  static boolean called_atexit = FALSE;

  /*---------------------------------------------------------------------------
    multiple calls to init lib allowed from different thread contexts
  ---------------------------------------------------------------------------*/
  instance++;
  instance %= DS_PROFILE_MAX_NUM_HNDL;
  (void)DS_PROFILE_STR_PRINTF(lib,
                              sizeof(logging_prefix) + sizeof(instance),
                              "%s[%ld]", logging_prefix, instance);

  do
  {
    /*--------------------------------------------------------------------------- 
      Initialize logging
    ---------------------------------------------------------------------------*/
    if (ds_profile_log_init(lib) != 0)
    {
      return_status = DS_PROFILE_REG_RESULT_FAIL;
      break;
    }

    /* Acquire lock */
    ACQ_LOCK("_init_lib");

    /*--------------------------------------------------------------------------- 
      Allow initialization only if not inited, ignore otherwise
    ---------------------------------------------------------------------------*/
    if (LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_SUCCESS;
    }

    /*--------------------------------------------------------------------------- 
      Initialize server thread, takes care of SubSystem Restart events
    ---------------------------------------------------------------------------*/
    return_status = ds_profile_server_start();
    if (DS_PROFILE_REG_RESULT_SUCCESS != return_status)
    {
      break;
    }

    /*--------------------------------------------------------------------------- 
      Call internal function for initialization. Returns the ORed value of all
      tech types, if 0 then error 
    ---------------------------------------------------------------------------*/
    if ((tech_mask = dsi_profile_init()) ==  0)
    {      
      return_status = DS_PROFILE_REG_RESULT_FAIL;
      break;
    }

    /* Register process exit cleanup handler */
    if (FALSE == called_atexit)
    {
      (void)atexit(dsi_profile_close_lib);
      called_atexit = TRUE;
    }

    lib_state = DS_PROFILE_LIB_STATE_INITED;

    /* To register handler with dcc task to handle remote operations */
    ds_profile_remote_init();


    ACQ_LOCK_CALLBACK("_init_lib");
    if (ds_util_list_get_hndl(&callbacks_list) != DS_SUCCESS)
    {
      REL_LOCK_CALLBACK("_init_lib");
      return_status = DS_PROFILE_REG_RESULT_FAIL;
      break;
    }

    REL_LOCK_CALLBACK("_init_lib");
    REL_LOCK("_init_lib");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  }while (0);

  if (DS_PROFILE_REG_RESULT_SUCCESS != return_status)
  {
    REL_LOCK("_init_lib");
  }

  DS_PROFILE_LOGE("_init_lib: EXIT with ERR", return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_CLOSE_LIB 
 
DESCRIPTION
  This functions cleans up any open handles, closes the library

PARAMETERS

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS  : On successful operation
  DS_PROFILE_REG_RESULT_FAIL	 : On general errors. This return code 
                                   provides blanket coverage
SIDE EFFECTS  
===========================================================================*/
ds_profile_status_etype ds_profile_close_lib(
  void
  )
{
  /* Check lib was initialized */
  if (!LIB_STATE_IS_VALID(lib_state))
  {
    DS_PROFILE_LOGE("_close_lib: FAIL lib was not initialized", 0);
    return DS_PROFILE_REG_RESULT_FAIL;
  }

  ACQ_LOCK("_close_lib");
  lib_state = DS_PROFILE_LIB_STATE_INVAL;
  /*--------------------------------------------------------------------------- 
    Check handles have been released and iterators are destroyed
  ---------------------------------------------------------------------------*/
  dsi_profile_close_lib();

  ACQ_LOCK_CALLBACK("_close_lib");
  ds_util_list_rel_hndl(callbacks_list);
  REL_LOCK_CALLBACK("_close_lib");

  REL_LOCK("_close_lib");
  return DS_PROFILE_REG_RESULT_SUCCESS;
}

/*===========================================================================
FUNCTION DS_PROFILE_BEGIN_TRANSACTION

DESCRIPTION
  This returns a Handle that the clients of this software library can use for 
  subsequent Profile operations. The Handle returned is of requested 
  transaction type. All Profile operations using this Handle require that 
  DS_PROFILE_END_TRANSACTION be called at the end. 

PARAMETERS
  trn  : requested transaction type
  tech : technology type
  num  : profile number
  hndl : pointer to return requested handle

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 		   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  :  Invalid profile number 
  DS_PROFILE_REG_RESULT_FAIL			   : On general errors. This return 
                                             code provides blanket coverage
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_begin_transaction(
  ds_profile_trn_etype   trn,
  ds_profile_tech_etype  tech,
  ds_profile_num_type    num,
  ds_profile_hndl_type  *hndl
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate transaction */
    if (!TRN_IS_VALID(trn))
    {
      return_status = DS_PROFILE_REG_RESULT_FAIL;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    ACQ_LOCK("_begin_transaction");
    /*-------------------------------------------------------------------------- 
      Call internal function to allocate handle for transaction
    --------------------------------------------------------------------------*/
    if ((return_status = dsi_profile_alloc_hndl(trn, tech, num, hndl, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_begin_transaction");
      break;
    }
    REL_LOCK("_begin_transaction");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);


  DS_PROFILE_LOGD_4("ds_profile_begin_transaction: ended with trn %d, "
                    "tech %d, num %d and err %d",
                    trn, tech, num, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_BEGIN_TRANSACTION_PER_SUB

DESCRIPTION
  This returns a Handle that the clients of this software library can use for 
  subsequent Profile operations. The Handle returned is of requested 
  transaction type. All Profile operations using this Handle require that 
  DS_PROFILE_END_TRANSACTION be called at the end. 

PARAMETERS
  trn      : requested transaction type
  tech     : technology type
  num      : profile number
  subs_id  : Profile subscription to identify on which subs_id to reset
  hndl     : pointer to return requested handle
  

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 		   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  :  Invalid profile number 
  DS_PROFILE_REG_RESULT_FAIL			   : On general errors. This return 
                                             code provides blanket coverage
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_begin_transaction_per_sub(
  ds_profile_trn_etype trn,
  ds_profile_tech_etype tech,
  ds_profile_num_type num,
  ds_profile_subs_etype subs_id,
  ds_profile_hndl_type *hndl
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate transaction */
    if (!TRN_IS_VALID(trn))
    {
      return_status = DS_PROFILE_REG_RESULT_FAIL;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, 
                                                          num, 
                                                          subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    ACQ_LOCK("_begin_transaction");
    /*-------------------------------------------------------------------------- 
      Call internal function to allocate handle for transaction
    --------------------------------------------------------------------------*/
    if ((return_status = dsi_profile_alloc_hndl(trn, 
                                                tech, 
                                                num, 
                                                hndl, 
                                                subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_begin_transaction");
      break;
    }

    REL_LOCK("_begin_transaction");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  }while (0);

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_DSPROFILE, ps_utils_subs_id_dsprofile_to_ps(subs_id), 
  	                 "ds_profile_begin_transaction_per_sub: "
                         "ended with trn %d, tech %d, num %d, subs_id %d, error %d",
                         trn, tech, num, subs_id, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_BEGIN_TRANSACTION_EX

DESCRIPTION
  This returns a Handle that the clients of this software library can use for 
  subsequent Profile operations. The Handle returned is of requested 
  transaction type. All Profile operations using this Handle require that 
  DS_PROFILE_END_TRANSACTION be called at the end. 

PARAMETERS
  trn     : requested transaction type
  tech    : technology type
  num     : profile number
  hndl    : pointer to return requested handle
  subs_id : subs_id id of the profile

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 		   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  :  Invalid profile number 
  DS_PROFILE_REG_RESULT_FAIL			   : On general errors. This return 
                                             code provides blanket coverage
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_begin_transaction_ex(
  ds_profile_trn_etype  trn,
  ds_profile_tech_etype tech,
  ds_profile_num_type   num,
  ds_profile_hndl_type  *hndl
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate transaction */
    if (!TRN_IS_VALID(trn))
    {
      return_status = DS_PROFILE_REG_RESULT_FAIL;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    ACQ_LOCK("_begin_transaction");
    /*-------------------------------------------------------------------------- 
      Call internal function to allocate handle for transaction
    --------------------------------------------------------------------------*/
    if ((return_status = dsi_profile_alloc_hndl(trn, tech, num, hndl, subs_id))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_begin_transaction");
      break;
    }

    REL_LOCK("_begin_transaction");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_begin_transaction_ex: ended with trn %d, "
                    "tech %d, num %d, error %d", 
                    trn, tech, num, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_END_TRANSACTION

DESCRIPTION
  This function commits the prefetched modified profile to the persistent 
  storage on the modem. It also invokes cleanup routines for the profile
  handle specified. On return the handle becomes unusable

PARAMETERS
 hndl  : profile handle
 act   : action (commit / cancel)

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 	   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL : Invalid handle
  DS_PROFILE_REG_RESULT_FAIL		   : On general errors.
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_end_transaction(
  ds_profile_hndl_type    profile_hndl,
  ds_profile_action_etype act
  )
{
  ds_profile_num_type     profile_num;
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate handle */
    if (!HNDL_IS_VALID(profile_hndl))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
      break;
    }

    /* Lock here */
    ACQ_LOCK("_end_transaction");
    if (act == DS_PROFILE_ACTION_COMMIT)
    {
      /*----------------------------------------------------------------------- 
        Call internal function to end transaction and write back the changes
        made on the profile
      ------------------------------------------------------------------------*/
      if ((return_status = dsi_profile_end_transaction(profile_hndl))
          !=  DS_PROFILE_REG_RESULT_SUCCESS)
      {
        /*-------------------------------------------------------------------- 
        If end transaction fails, call internal function to dealloc handle,
        release memory
        ---------------------------------------------------------------------*/
        (void)dsi_profile_dealloc_hndl(&profile_hndl);
        REL_LOCK("_end_transaction");
        break;
      }
    }

    profile_num = dsi_profile_get_profile_num_from_handle(profile_hndl);

    /*-------------------------------------------------------------------- 
    Call internal function to dealloc handle, release memory
    ---------------------------------------------------------------------*/
    if ((return_status = dsi_profile_dealloc_hndl(&profile_hndl))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_end_transaction");
      break;
    }

    /*-------------------------------------------------------------------- 
    Call operation to modify profile on remote modem
    ---------------------------------------------------------------------*/
    if (ds_profile_modified == TRUE)
    {
      ds_profile_modified = FALSE;
      if (act == DS_PROFILE_ACTION_COMMIT)
      {
        ds_profile_remote_modify_profile(profile_num);
      }
    }

    REL_LOCK("_end_transaction");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  if (ds_profile_modified == TRUE)
  {
    ds_profile_modified = FALSE;
  }

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_DSPROFILE, PS_SYS_DEFAULT_SUBS,
                         "ds_profile_end_transaction: ended with hndl %d, "
                         "act %d, error %d", 
                         profile_hndl, act, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_SET_PARAM

DESCRIPTION
  This function is used to set profile data element identified by the 
  identifier. The identifiers are specified in the corresponding tech 
  header file. The prefetched copy is modified. end_transaction will
  modify the profile on modem. 

PARAMETERS
  profile_hndl  : handle to profile to set profile data elements
  identifier	: to identify profile data elements
  info  		: pointer to value to which data element is to be set
                  (size of buffer passed can atmost be the max size of 
                  the parameter which needs to be set)
DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 		   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL     : Invalid handle 
  DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT    : Invalid identifier
  DS_PROFILE_REG_RESULT_ERR_LEN_INVALID    : Buffer size more than expected
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type 
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_FAIL			   : On general errors 
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_set_param(
  ds_profile_hndl_type  	  hndl,
  ds_profile_identifier_type  identifier,
  const ds_profile_info_type *info
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {

    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state)) 
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate handle */
    if ((!HNDL_IS_VALID(hndl))) 
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
      break;
    }

    ACQ_LOCK("_set_param");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech and identifier
      calls tech-specific set function
    ---------------------------------------------------------------------*/
    if ((return_status = dsi_profile_set_param(hndl, identifier, info))
        !=  DS_PROFILE_REG_RESULT_SUCCESS) 
    {
      REL_LOCK("_set_param");
      break;
    }

    /*-------------------------------------------------------------------- 
      We want to make sure that everytime profile is modified
      we want to invoke ds_profile remote client to send the modify operation
      to QSC
    ---------------------------------------------------------------------*/
    ds_profile_modified = TRUE;
    ds_profile_remote_profile_identifier_set(identifier, TRUE);

    REL_LOCK("_set_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

 DS_INTERFACE_LOG_MSG_INFO1_3(PS_LOGGING_CAT_DS_PROFILE, "ds_profile_set_param: ended with "
                    "hndl %d, ident %d, error %d",
                              (int)hndl, (int)identifier, (int)return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_PARAM

DESCRIPTION
  This function is used to get Profile data element identified by the 
  identifier. The identifiers are specified in the corresponding tech 
  header file. The data elements are read from the prefetched Profile and
  info is returned with that value and length.

PARAMETERS
  profile_hndl : handle to profile to get profile data element
  identifier   : to identify profile data element
  info  	   : pointer to store value and length of data element
                 (size of buffer allocated should atleast be the max size of 
                  the parameter which needs to be fetched)

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 		   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL     : Invalid handle 
  DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT    : Invalid identifier
  DS_PROFILE_REG_RESULT_ERR_LEN_INVALID    : Buffer size less than required
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type 
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_FAIL			   : On general errors 
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_get_param(
  ds_profile_hndl_type  	  hndl,
  ds_profile_identifier_type  identifier,
  ds_profile_info_type  	 *info
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }
    /* Validate handle */
    if (!HNDL_IS_VALID(hndl))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
      break;
    }

    ACQ_LOCK("_get_param");
    /*-------------------------------------------------------------------- 
      Call internal function, depending on tech and identifier
      calls tech-specific get function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_get_param(hndl, identifier, info))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_get_param");
      break;
    }

    REL_LOCK("_get_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  /*DS_PROFILE_LOGD_3("ds_profile_get_param: exited with hndl %d, "
                    "ident %d, error %d",hndl, identifier, 
                    return_status);*/
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_DELETE

DESCRIPTION
  This is used to reset a profile to undefined and return to free pool. Not
  all technology types support this operation.
  Not allowed for default profiles

PARAMETERS
  tech : technology type
  num  : profile number

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS  : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	 : On general errors. This return code 
                                   provides blanket coverage
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_delete(
  ds_profile_tech_etype  tech,
  ds_profile_num_type    num
  )
{
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();
  return ds_profile_delete_per_sub(tech, num, subs_id);
}

/*===========================================================================
FUNCTION DS_PROFILE_DELETE_EX
============================================================================*/
/** @ingroup ds_profile_delete_ex
  Resets a profile to undefined and return to free pool. 
  Default profiles can be deleted only if the flag is explicitly set to true

  @note1hang Not all technology types support this operation.
 
  @datatypes
  #ds_profile_tech_etype\n
  #ds_profile_num_type\n
  #boolean\n


  @vertspace
  @param[in] tech   						   Technology.
  @param[in] num							   Profile number to be deleted.
  @param[in] enable_deleting_default_profile   Enable deleting default profile
 
  @return
  DS_PROFILE_REG_RESULT_SUCCESS 			   -- Operation succeeded.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE -- Invalid technology type.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  -- Invalid profile number.\n
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED     -- Library is not initialized.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP		   -- Operation is not supported  
                                                  for the technology type.\n
  DS_PROFILE_REG_RESULT_FAIL				   -- General failure. 

  @dependencies 
  The software library must be initialized.
  @newpage
*/
ds_profile_status_etype ds_profile_delete_ex(
  ds_profile_tech_etype  tech,
  ds_profile_num_type    num,
  boolean   			 enable_deleting_default_profile
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id)) !=
          DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }
    /*-------------------------------------------------------------------
        Delete the profile under PS critical section in order to avoid 
        a scenario wherein route lookup is happening during call bring-up 
        in PS task for a specific profile and client/application deletes 
        the same profile in DCC task preempting PS task
     ------------------------------------------------------------------*/
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

    ACQ_LOCK("_delete");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific delete function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_delete(tech, num, enable_deleting_default_profile, subs_id)) !=
          DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_delete");
      PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      break;
    }

    /*-------------------------------------------------------------------- 
      Delete the profile on remote modem
    --------------------------------------------------------------------*/
    if (tech == DS_PROFILE_TECH_3GPP)
    {
      ds_profile_remote_delete_profile(tech, (uint8)num);
    }

    REL_LOCK("_delete");
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_delete_ex: started with tech %d, num %d, "
                    "delete_default flag %d, error %d",tech, num, 
                    enable_deleting_default_profile, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_DELETE_PER_SUB
============================================================================*/
/** @ingroup DS_PROFILE_DELETE_PER_SUB
  Resets a profile to undefined and return to free pool. 
  Default profiles can be deleted only if the flag is explicitly set to true

  @note1hang Not all technology types support this operation.
 
  @datatypes
  #ds_profile_tech_etype\n
  #ds_profile_num_type\n
  #boolean\n


  @vertspace
  @param[in] tech   						   Technology.
  @param[in] num							   Profile number to be deleted.
  @param[in] enable_deleting_default_profile   Enable deleting default profile
  #param[in] subs_id						   Profile subscription to identify
                                               on which subs_id to reset						   
 
  @return
  DS_PROFILE_REG_RESULT_SUCCESS 			   -- Operation succeeded.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE -- Invalid technology type.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  -- Invalid profile number.\n
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED     -- Library is not initialized.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP		   -- Operation is not supported  
                                                  for the technology type.\n
  DS_PROFILE_REG_RESULT_FAIL				   -- General failure. 

  @dependencies 
  The software library must be initialized.
  @newpage
*/
ds_profile_status_etype ds_profile_delete_per_sub(
  ds_profile_tech_etype tech,
  ds_profile_num_type num,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id)) !=
          DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }
    /*-------------------------------------------------------------------
        Delete the profile under PS critical section in order to avoid 
        a scenario wherein route lookup is happening during call bring-up 
        in PS task for a specific profile and client/application deletes 
        the same profile in DCC task preempting PS task
     ------------------------------------------------------------------*/
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

    ACQ_LOCK("_delete");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific delete function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_delete(tech, num, FALSE, subs_id)) !=
          DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_delete");
      PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      break;
    }

    /*-------------------------------------------------------------------- 
      Delete the profile on remote modem
    --------------------------------------------------------------------*/
    if (tech == DS_PROFILE_TECH_3GPP)
    {
      ds_profile_remote_delete_profile(tech, (uint8)num);
    }

    REL_LOCK("_delete");
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_delete_ex: exited with tech %d, num %d, "
                    "delete_default flag %d, error %d",tech, num, 
                    FALSE, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_DELETE_ATTACH_PER_SUB
============================================================================*/
/** @ingroup DS_PROFILE_DELETE_PER_SUB
  Resets a profile to undefined and return to free pool. 
  Default profiles can be deleted only if the flag is explicitly set to true

  @note1hang Not all technology types support this operation.
 
  @datatypes
  #ds_profile_tech_etype\n
  #ds_profile_num_type\n
  #boolean\n


  @vertspace
  @param[in] tech                              Technology.
  @param[in] num                               Profile number to be deleted.
  #param[in] subs_id                           Profile subscription to identify
                                               on which subs_id to reset                           
 
  @return
  DS_PROFILE_REG_RESULT_SUCCESS                -- Operation succeeded.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE -- Invalid technology type.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  -- Invalid profile number.\n
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED     -- Library is not initialized.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP           -- Operation is not supported  
                                                  for the technology type.\n
  DS_PROFILE_REG_RESULT_FAIL                   -- General failure. 

  @dependencies 
  The software library must be initialized.
  @newpage
*/
ds_profile_status_etype ds_profile_delete_attach_per_sub(
  ds_profile_tech_etype tech,
  ds_profile_num_type num,
  ds_profile_subs_etype subs_id
)
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  DS_PROFILE_LOGD_2("ds_profile_delete_attach_per_sub: started with tech %d, num %d",
                    tech, num);

  /* Validate lib state */
  if (!LIB_STATE_IS_VALID(lib_state))
  {
    DS_PROFILE_LOGE("_delete: FAIL lib not inited ", 0);
    return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
    goto ret_err;
  }

  /* Validate tech type */
  if (!TECH_IS_VALID(tech))
  {
    DS_PROFILE_LOGE("_delete: FAIL invalid tech type ", 0);
    return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
    goto ret_err;
  }

  if (DS_PROFILE_REG_RESULT_SUCCESS != 
      (return_status = ds_profile_validate_subs_id(subs_id)))
  {
    goto ret_err;
  }

  /* Validate profile number */
  if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id)) !=
        DS_PROFILE_REG_RESULT_SUCCESS)
  {
    DS_PROFILE_LOGE("_delete: FAIL invalid profile number ", 0);
    goto ret_err;
  }
  /*-------------------------------------------------------------------
      Delete the profile under PS critical section in order to avoid 
      a scenario wherein route lookup is happening during call bring-up 
      in PS task for a specific profile and client/application deletes 
      the same profile in DCC task preempting PS task
   ------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  ACQ_LOCK("_delete");
  /*-------------------------------------------------------------------- 
    Call internal function which depending on tech calls the
    tech specific delete function
  --------------------------------------------------------------------*/
  if ((return_status = dsi_profile_delete_attach(tech, num, subs_id)) !=
       DS_PROFILE_REG_RESULT_SUCCESS )
  {
    DS_PROFILE_LOGE("_delete: FAIL internal delete function", 0 );
    REL_LOCK( "_delete" );
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    goto ret_err;
  }

  REL_LOCK( "_delete" );
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return DS_PROFILE_REG_RESULT_SUCCESS;

ret_err:
  DS_PROFILE_LOGD( "_delete: EXIT with ERR", 0 );
  return return_status;
} 

/*===========================================================================
FUNCTION DS_PROFILE_CREATE

DESCRIPTION
  This function is used to return a profile number from a pool of free
  profiles. Not all technology types support this operation.

PARAMETERS
  tech  : technology type
  num   : pointer to return profile number of profile created

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS  : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	 : On general errors. This return code 
                                   provides blanket coverage
SIDE EFFECTS 
===========================================================================*/
ds_profile_status_etype ds_profile_create
(
  ds_profile_tech_etype  tech,
  ds_profile_num_type   *num
  )
{
  return ds_profile_create_ex(tech, NULL, num);
}

/*===========================================================================
FUNCTION DS_PROFILE_CREATE_EX

DESCRIPTION
  This function is used to return a profile number from a pool of free
  profiles. Not all technology types support this operation.

PARAMETERS
  tech  : technology type
  config_ptr : config type for persistent property and other profile attributes
  num   : pointer to return profile number of profile created

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS  : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	 : On general errors. This return code 
                                   provides blanket coverage
SIDE EFFECTS 
===========================================================================*/
ds_profile_status_etype ds_profile_create_ex
(
  ds_profile_tech_etype  tech,
  ds_profile_config_type *config_ptr,
  ds_profile_num_type    *num
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  //If the below log message not printed assume NULL config
  if (NULL != config_ptr)
  {
    DS_PROFILE_LOGD_2("ds_profile_create_ex: started with tech %d, config_mask 0x%x",
                      tech, config_ptr->config_mask);
  }

  do
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = 
        DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = 
        DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (num == NULL)
    {
      return_status = 
        DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_create");

    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific create function
    --------------------------------------------------------------------*/
    return_status = dsi_profile_create(tech, 
                                       config_ptr,
                                       num);
    if (return_status !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_create");
      break;
    }

    /*-------------------------------------------------------------------- 
      Create profile on remote modem
    --------------------------------------------------------------------*/
    if (tech == DS_PROFILE_TECH_3GPP)
    {
      ds_profile_remote_create_profile(tech);
    }

    REL_LOCK("_create");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);


  DS_PROFILE_LOGD_2("ds_profile_create_ex: exited with tech %d, error %d",
                      tech, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RESET_PARAM_TO_DEFAULT

DESCRIPTION
  This function resets the value of parameter in a profile to default. It
  directly changes the value on modem, so begin/end transaction need not
  be called before/after this function.

PARAMETERS
  tech  : technology type
  num   : profile number
  ident : to identify the profile parameter to be set to default

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_reset_param_to_default(
  ds_profile_tech_etype 	  tech,
  ds_profile_num_type   	  num,
  ds_profile_identifier_type  ident
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = 
        DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, 
                                                          num, 
                                                          subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    ACQ_LOCK("_reset_param");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_reset_param(tech, 
                                                 num, 
                                                 ident, 
                                                 subs_id, 
                                                 FALSE))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_param");
      break;
    }

    REL_LOCK("_reset_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_reset_param_to_default: "
                    "exited with tech %d, num %d, "
                    "ident %d, error %d",tech, num, 
                    ident, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RESET_PARAM_TO_DEFAULT_PER_SUB

DESCRIPTION
  This function resets the value of parameter in a profile to default. It
  directly changes the value on modem, so begin/end transaction need not
  be called before/after this function.

PARAMETERS
  tech     : technology type
  num      : profile number
  ident    : to identify the profile parameter to be set to default
  subs_id  : Profile subscription to identify on which subs_id to reset

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_reset_param_to_default_per_sub(
  ds_profile_tech_etype tech,
  ds_profile_num_type num,
  ds_profile_identifier_type ident,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      DS_PROFILE_LOGE("_reset_param: FAIL invalid profile number ", 0);
      break;
    }

    ACQ_LOCK("_reset_param");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_reset_param(tech, num, ident, subs_id, FALSE))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_param");
      break;;
    }

    REL_LOCK("_reset_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_reset_param_to_default: exited with tech %d, num %d, "
                    "ident %d, error %d",tech, num, ident, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RESET_PARAM_TO_INVALID

DESCRIPTION
  This function resets the value of parameter in a profile to default. It
  directly changes the value on modem, so begin/end transaction need not
  be called before/after this function.

PARAMETERS
  tech  : technology type
  num   : profile number
  ident : to identify the profile parameter to be set to default

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_reset_param_to_invalid(
  ds_profile_tech_etype 	  tech,
  ds_profile_num_type   	  num,
  ds_profile_identifier_type  ident
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    ACQ_LOCK("_reset_param");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_reset_param(tech, num, ident, subs_id, TRUE))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_param");
      break;
    }

    REL_LOCK("_reset_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_reset_param_to_invalid: "
                    "exited with tech %d, num %d, "
                    "ident %d, error %d",tech, num, 
                    ident, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RESET_PARAM_TO_INVALID_PER_SUB

DESCRIPTION
  This function resets the value of parameter in a profile to default. It
  directly changes the value on modem, so begin/end transaction need not
  be called before/after this function.

PARAMETERS
  tech    : technology type
  num     : profile number
  ident   : to identify the profile parameter to be set to default
  subs_id : subscription id of the profile

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_reset_param_to_invalid_per_sub(
  ds_profile_tech_etype tech,
  ds_profile_num_type num,
  ds_profile_identifier_type ident,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = 
        DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, 
                                                          num, 
                                                          subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    ACQ_LOCK("_reset_param");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_reset_param(tech, 
                                                 num, 
                                                 ident, 
                                                 subs_id, 
                                                 TRUE))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_param");
      break;
    }

    REL_LOCK("_reset_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_5("ds_profile_reset_param_to_invalid_per_sub: "
                    "exited with tech %d, num %d, "
                    "ident %d, subs_id %d, error %d",
                    tech, num, ident, 
                    subs_id, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION ds_profile_reset_pdp_context

DESCRIPTION
  This function is used to send a request to reset a pdp context 
  value to default.

PARAMETERS 
  tech - technology
  num  - profile number


DEPENDENCIES 
  
RETURN VALUE 
  
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_reset_pdp_context(
  ds_profile_tech_etype 	  tech,
  ds_profile_num_type   	  num
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = 
        DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, 
                                                          num, 
                                                          subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    ACQ_LOCK("_reset_pdp_context");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset function
    --------------------------------------------------------------------*/
    if ((return_status = ds_profile_db_handler_reset_pdp_context(tech, 
                                                                 num, 
                                                                 subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_param");
      break;
    }

    REL_LOCK("_reset_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_3("ds_profile_reset_pdp_context: exited "
                    "with tech %d, num %d, error %d",
                    tech, num, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RESET_PDP_CONTEXT_PER_SUB

DESCRIPTION
  This function is used to send a request to reset a pdp context 
  value to default.

PARAMETERS 
  tech     - technology
  num      - profile number
  subs_id  - subscription id


DEPENDENCIES 
  
RETURN VALUE 
  
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_reset_pdp_context_per_sub(
  ds_profile_tech_etype tech,
  ds_profile_num_type num,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      break;
    }

    ACQ_LOCK("_reset_pdp_context");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset function
    --------------------------------------------------------------------*/
    if ((return_status = ds_profile_db_handler_reset_pdp_context(tech, num, subs_id))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_param");
      break;
    }

    REL_LOCK("_reset_param");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_reset_pdp_context_per_sub: exited with tech %d, "
                    "num %d, subs_id %d, error %d",
                    tech, num, subs_id, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RESET_PROFILE_TO_DEFAULT

DESCRIPTION
  This function resets all the parameters of the profile to default. It
  directly changes the value on modem, so begin/end transaction need not
  be called before/after this function.

PARAMETERS
  tech  : technology type
  num   : profile number

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_reset_profile_to_default(
  ds_profile_tech_etype  tech,
  ds_profile_num_type    num
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM;
      break;
    }

    ACQ_LOCK("_reset_profile_to_default");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset_profile function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_reset_profile_to_default(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_profile_to_default");
      break;
    }

    /*-------------------------------------------------------------------- 
      Reset profile on remote modem
    --------------------------------------------------------------------*/
    ds_profile_remote_reset_profile_to_default((uint8)num);

    REL_LOCK("_reset_profile_to_default");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_3("ds_profile_reset_profile_to_default: exited with tech %d, num %d, error %d",
                    tech, num, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RESET_PROFILE_TO_DEFAULT_PER_SUB

DESCRIPTION
  This function resets all the parameters of the profile to default. It
  directly changes the value on modem, so begin/end transaction need not
  be called before/after this function.

PARAMETERS
  tech     : technology type
  num      : profile number
  subs_id  : Subscription Id of the profile

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_reset_profile_to_default_per_sub(
  ds_profile_tech_etype tech,
  ds_profile_num_type num,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM;
      break;
    }

    ACQ_LOCK("_reset_profile_to_default");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific reset_profile function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_reset_profile_to_default(tech, num, subs_id))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_reset_profile_to_default");
      break;
    }

    /*-------------------------------------------------------------------- 
      Reset profile on remote modem
    --------------------------------------------------------------------*/
    ds_profile_remote_reset_profile_to_default((uint8)num);

    REL_LOCK("_reset_profile_to_default");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_reset_profile_to_default_per_sub: exited with tech %d, "
                    "num %d, subs_id %d, error %d",tech, num, subs_id, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_SET_DEFAULT_PROFILE_NUM

DESCRIPTION
  This function sets the given profile number as default profile for the
  family of the specified tech.

PARAMETERS
  tech   : technology type
  family : profile family
  num    : profile number

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_set_default_profile_num(
  ds_profile_tech_etype  tech,
  uint32				 family,
  ds_profile_num_type    num
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (!FAMILY_IS_VALID(family))
    {
      return_status = DS_PROFILE_REG_INVAL_PROFILE_FAMILY;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM;
      break;
    }

    ACQ_LOCK("_set_default_profile_num");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_set_default_profile(tech, family, num))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_set_default_profile_num");
      break;
    }

    REL_LOCK("_set_default_profile_num");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);
  
  DS_PROFILE_LOGD_4("ds_profile_set_default_profile_num: exited with tech %d, "
                    "family %d, num %d, error %d",tech, family, num, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_SET_DEFAULT_PROFILE_NUM_PER_SUBS

DESCRIPTION
  This function calls the internal function to set the given profile number as 
  default profile for the specified tech, family and subscription.

PARAMETERS
  tech    : technology type (3gpp or 3gpp2)
  family  : profile family  (embedded or tethered)
  subs_id : subscription id
  num     : profile number

DEPENDENCIES 
  None

RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
  DS_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID : Invalid subscription id passed

SIDE EFFECTS
  None
===========================================================================*/
ds_profile_status_etype ds_profile_set_default_profile_num_per_subs(
  ds_profile_tech_etype  tech,
  uint32				 family,
  ds_profile_subs_etype subs_id,
  ds_profile_num_type    num
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate family */
    if (!FAMILY_IS_VALID(family))
    {
      return_status = DS_PROFILE_REG_INVAL_PROFILE_FAMILY;
      break;
    }

    /* Validate subscription id */
    if ((subs_id <= DS_PROFILE_ACTIVE_SUBSCRIPTION_NONE) ||
        (subs_id > DS_PROFILE_ACTIVE_SUBSCRIPTION_MAX))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID;
      break;
    }

    /* Validate profile number */
    if ((return_status = dsi_profile_validate_profile_num(tech, num, subs_id))
        != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM;
      break;
    }

    ACQ_LOCK("_set_default_profile_num_per_subs");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific function
    --------------------------------------------------------------------*/
    if ((return_status =
         dsi_profile_set_default_profile_per_subs(tech, family, subs_id, num))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_set_default_profile_num_per_subs");
      break;
    }

    REL_LOCK("_set_default_profile_num_per_subs");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;
  } while (0);

  DS_PROFILE_LOGD_5("ds_profile_set_default_profile_num_per_subs: exited with tech %d, family %d, "
                    "ubs_id %d, num %d, error %d", tech, family, subs_id, num, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_DEFAULT_PROFILE_NUM

DESCRIPTION
  This function gets the default profile number for the family of the
  specified tech.

PARAMETERS
  tech   : technology type
  family : profile family
  num    : pointer to store default profile number

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_get_default_profile_num(
  ds_profile_tech_etype   tech,
  uint32				  family,
  ds_profile_num_type    *num
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (!FAMILY_IS_VALID(family))
    {
      return_status = DS_PROFILE_REG_INVAL_PROFILE_FAMILY;
      break;
    }

    if (num == NULL)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_get_default_profile_num");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_get_default_profile(tech, family, num))
        !=  DS_PROFILE_REG_RESULT_SUCCESS)
    {
      REL_LOCK("_get_default_profile_num");
      break;
    }

    REL_LOCK("_get_default_profile_num");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_3("ds_profile_get_default_profile: exited with tech %d, family %d, error %d",
                    tech, family, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_DEFAULT_PROFILE_NUM_PER_SUBS

DESCRIPTION
  This function calls the internal function to get the default profile number 
  form the specified tech, family and subscription.

PARAMETERS
  tech    : technology type (3gpp or 3gpp2)
  family  : profile family  (embedded or tethered)
  subs_id : subscription id
  num     : pointer to store default profile number

DEPENDENCIES
  None

RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
  DS_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID : Invalid subscription id passed

SIDE EFFECTS
  None
===========================================================================*/
ds_profile_status_etype ds_profile_get_default_profile_num_per_subs(
  ds_profile_tech_etype   tech,
  uint32				  family,
  ds_profile_subs_etype  subs_id,
  ds_profile_num_type     *num
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do 
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech)) 
    {
      return_status =
        DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (!FAMILY_IS_VALID(family)) 
    {
      return_status =
        DS_PROFILE_REG_INVAL_PROFILE_FAMILY;
      break;
    }

    /* Validate subscription id */
    if ((subs_id <= DS_PROFILE_ACTIVE_SUBSCRIPTION_NONE) ||
        (subs_id > DS_PROFILE_ACTIVE_SUBSCRIPTION_MAX)) 
    {
      return_status =
        DS_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID;
      break;
    }

    if (num == NULL) 
    {
      return_status =
        DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_get_default_profile_num_per_subs");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific function
    --------------------------------------------------------------------*/
    if ((return_status = dsi_profile_get_default_profile_per_subs(
          tech, family, subs_id, num))
        !=  DS_PROFILE_REG_RESULT_SUCCESS) 
    {
      REL_LOCK("_get_default_profile_num_per_subs");
      break;
    }

    REL_LOCK("_get_default_profile_num_per_subs");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_get_default_profile_num_per_subs: "
                    "exited with tech %d, "
                    "family %d, subs_id %d, return_status %d",
                    tech, family, subs_id, return_status);
  return return_status;
}

/*=========================================================================
FUNCTION DS_PROFILE_GET_MAX_NUM

DESCRIPTION
  This function returns the maximum number of Profiles possible for a 
  given technology type  

PARAMETERS
  tech    : technology type
  max_num : pointer to store maximum number of profiles possible

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVALID_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_get_max_num(
  ds_profile_tech_etype  tech,
  uint32				*max_num
  )
{
  uint16 min_num = 0;
  uint16 mx = 0;
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do 
  {
    if (max_num == NULL)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    /* Validate tech */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    ACQ_LOCK("_get_max_num");
    /* Call internal function */
    dsi_profile_get_profile_num_range(tech, &min_num, &mx);
    if (mx == 0 && min_num == 0)
    {
      REL_LOCK("_get_max_num");
      break;
    }
    *max_num = (uint32)mx;

    REL_LOCK("_get_max_num");
    return_status = DS_PROFILE_REG_RESULT_SUCCESS;

  } while (0);

  DS_PROFILE_LOGD_2("ds_profile_get_max_num: exited with "
                    "tech %d, return_status %d",
                    tech, return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_SUPPORTED_TYPE 
 
---NOT EXPOSED AS AN API 
===========================================================================*/
ds_profile_status_etype ds_profile_get_supported_type(
  uint32				 *num,
  ds_profile_tech_etype  *tech
  )
{
  if (num == NULL || tech == NULL)
  {
    return DS_PROFILE_REG_RESULT_ERR_INVAL;
  }
  if (dsi_profile_get_supported_type(num, tech) !=
        DS_PROFILE_REG_RESULT_SUCCESS)
  {
    DS_PROFILE_LOGD("_get_supported_type: EXIT with ERR ", 0);
    return DS_PROFILE_REG_RESULT_FAIL;
  }
  return DS_PROFILE_REG_RESULT_SUCCESS;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_LIST_ITR

DESCRIPTION
  This is used to get the list of Profiles of a particular tech type. This 
  function returns an Iterator. The Iterator is traversed using 
  DS_PROFILE_ITR_NEXT. After traversal is complete, the caller is 
  expected to call DS_PROFILE_ITR_DESTROY.

PARAMETERS
  tech  : technology type
  lst   : type of list, (list with all profiles / depending on some search)
  itr   : iterator to traverse through search result

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS  : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_FAIL	 : On general errors. This return code 
                                   provides blanket coverage
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_get_list_itr(
  ds_profile_tech_etype  tech,
  ds_profile_list_type  *lst,
  ds_profile_itr_type   *itr
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  if (NULL == lst)
  {
    DS_PROFILE_LOGE("_get_list_itr: FAIL NULL lst pointer ", 0);
    return DS_PROFILE_REG_RESULT_ERR_INVAL;
  }

  do 
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    /* Validate list_type and itr_type */
    if (!LIST_TYPE_IS_VALID(lst))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_LST;
      break;
    }

    /* Validate itr_type */
    if (!ITR_IS_VALID(itr))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_get_list_itr");
    return_status = 
      dsi_profile_get_list_itr(tech, lst, 
                               itr, subs_id);
        

    REL_LOCK("_get_list_itr");

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_get_list_itr: exiting with tech %d, "
                    "dfn %d, ident %d, return_status %d",
                    tech, lst->dfn, lst->ident, 
                    return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_LIST_ITR_PER_SUB

DESCRIPTION
  This is used to get the list of Profiles of a particular tech type. This 
  function returns an Iterator. The Iterator is traversed using 
  DS_PROFILE_ITR_NEXT. After traversal is complete, the caller is 
  expected to call DS_PROFILE_ITR_DESTROY.

PARAMETERS
  tech     : technology type
  lst      : type of list, (list with all profiles / depending on some search)
  itr      : iterator to traverse through search result
  subs_id  : Subscription ID for which the list is required

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS  : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type
  DS_PROFILE_REG_RESULT_FAIL	 : On general errors. This return code 
                                   provides blanket coverage
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_get_list_itr_per_sub(
  ds_profile_tech_etype tech,
  ds_profile_list_type *lst,
  ds_profile_itr_type *itr,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  if (NULL == lst)
  {
    DS_PROFILE_LOGE("_get_list_itr: FAIL NULL lst pointer ", 0);
    return DS_PROFILE_REG_RESULT_ERR_INVAL;
  }

  do 
  {
    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    /* Validate list_type and itr_type */
    if (!LIST_TYPE_IS_VALID(lst))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_LST;
      break;
    }

    /* Validate itr_type */
    if (!ITR_IS_VALID(itr))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_get_list_itr");
    return_status = 
      dsi_profile_get_list_itr(tech, lst,
                               itr, subs_id);
   
    REL_LOCK("_get_list_itr");

  } while (0);
  
  DS_PROFILE_LOGD_5("ds_profile_get_list_itr_per_sub: exited with tech %d, "
                    "dfn %d, ident %d, subs %d, return_status %d",
                    tech, lst->dfn, lst->ident, subs_id, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_ITR_NEXT

DESCRIPTION
  This routine advances the Iterator to the next element.  

PARAMETERS
  itr : iterator

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return code 
                                    provides blanket coverage
  DS_PROFILE_REG_RESULT_ERR_INVAL : Invalid argument (iterator)
  DS_PROFILE_REG_RESULT_LIST_END  : End of list
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_itr_next(
  ds_profile_itr_type   itr
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do 
  {
    /* Validate itr_type */
    if (!ITR_IS_VALID(itr))
    {
      return_status = 
        DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_itr_next");

    return_status = dsi_profile_itr_next(itr);

    REL_LOCK("_itr_next");

  } while (0);

  DS_PROFILE_LOGD("ds_profile_itr_next exit with "
                  "return_status %d", return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_ITR_FIRST

DESCRIPTION
  This routine resets the Iterator to the beginning of the list.  

PARAMETERS
  itr : iterator

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return code 
                                    provides blanket coverage
  DS_PROFILE_REG_RESULT_ERR_INVAL : Invalid argument (iterator)
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_itr_first(
  ds_profile_itr_type   itr
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do {
    /* Validate itr_type */
    if (!ITR_IS_VALID(itr))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_itr_first");

    return_status = dsi_profile_itr_first(itr);

    REL_LOCK("_itr_first");

  } while (0);

  DS_PROFILE_LOGD("ds_profile_itr_first return_status %d", return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_INFO_BY_ITR

DESCRIPTION
  This routine gets info stored in that Iterator node.  

PARAMETERS
  itr   	: iterator
  list_info : pointer to structure to return profile info

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL : Invalid argument (iterator)
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return code 
                                    provides blanket coverage
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_get_info_by_itr(
  ds_profile_itr_type   	  itr,
  ds_profile_list_info_type  *list_info
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do 
  {
    /* Validate itr_type */
    if (!ITR_IS_VALID(itr))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    if (list_info == NULL)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_LST;
      break;
    }

    ACQ_LOCK("_get_info_by_itr");

    return_status = 
      dsi_profile_get_info_by_itr(itr, list_info);
   
    REL_LOCK("_get_info_by_itr");

  } while (0);

  DS_PROFILE_LOGD("_get_info_by_itr: EXIT with "
                  "return_status %d", return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_ITR_DESTROY

DESCRIPTION
  This routine destroys the Iterator  

PARAMETERS
  itr : iterator

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return code 
                                    provides blanket coverage
  DS_PROFILE_REG_RESULT_ERR_INVAL : Invalid argument (iterator)
SIDE EFFECTS 
  none
===========================================================================*/
ds_profile_status_etype ds_profile_itr_destroy(
  ds_profile_itr_type   itr
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do 
  {
    /* Validate itr_type */
    if (!ITR_IS_VALID(itr))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    ACQ_LOCK("_itr_destroy");
    return_status = dsi_profile_itr_destroy(itr);
    REL_LOCK("_itr_destroy");

  } while (0);

  DS_PROFILE_LOGD("_itr_destroy: EXIT with "
                  "return_Status %d", return_status);

  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_INTERNAL_CALLBACK
============================================================================*/
/** 
  Internal
  Performs the translation between DB callback and DS PROFILE callback
*/
static void ds_profile_callback_internal(
  ds_profile_db_event_type event,
  ds_profile_db_profile_managed_type **profiles,
  uint16 profile_count,
  void *user_data,
  ds_profile_db_subs_type subs_id
  )
{
  ds_profile_cback_func_info_node *cb_info_node;
  ds_profile_cb_handle_type cb_handle = (ds_profile_cb_handle_type)user_data;
  ds_profile_changes_info *profiles_changes = NULL;
  uint16 i, j;
  ds_profile_db_ident_type *idents = NULL, *gen_idents = NULL;
  uint32 count;
  uint16 idents_count, gen_idents_count;
  ds_profile_db_result_type res;

  DS_PROFILE_LOGD("ds_profile_callback_internal: started with event %d", profile_count);

  if (ds_profile_lock_acq(&callback_lock) != 0)
  {
    ds_profile_db_free_profiles(profiles, profile_count); 
    return;
  }
  cb_info_node = (ds_profile_cback_func_info_node *)
    ds_util_list_find_node(callbacks_list, &cb_handle, sizeof(cb_handle));
  if (NULL == cb_info_node || (0 == cb_info_node->db_cb_handle))
  {
    DS_PROFILE_LOGE("_callback_register: FAIL to find db client id from the list of valid clients", 0);
    ds_profile_db_free_profiles(profiles, profile_count); 
    if (ds_profile_lock_rel(&callback_lock) != 0)
    {
      DS_PROFILE_LOGE("FAIL: unable to release callback lock", 0);
    }
    return;
  }

  if (event == DB_REFRESH_PROFILE_EVENT || 
      event == DB_PROFILES_READY_ON_SUBS_EVENT)
  {
    goto bail1;
  }

  if (0 == profile_count)
  {
    DS_PROFILE_LOGE("_callback_internal: error occured callback with 0 profiles", 0);
    if (ds_profile_lock_rel(&callback_lock) != 0)
    {
      DS_PROFILE_LOGE("FAIL: unable to release callback lock", 0);
    }
    return;
  }

  /* convert DB info to ds profile info */
  profiles_changes = (ds_profile_changes_info *)
    DS_PROFILE_MEM_ALLOC(sizeof(ds_profile_changes_info) * profile_count,
                         MODEM_MEM_CLIENT_DATA);
  if (profiles_changes == NULL)
  {
    DS_PROFILE_LOGE("_callback_internal: FAIL DS_PROFILE_MEM_ALLOC", 0);
    if (ds_profile_lock_rel(&callback_lock) != 0)
    {
      ds_profile_db_free_profiles(profiles, profile_count);
      DS_PROFILE_LOGE("FAIL: unable to release callback lock", 0);
    }
    return;
  }
  memset(profiles_changes, 0, sizeof(ds_profile_changes_info) * profile_count);

  for (i = 0; i < profile_count; i++)
  {

    profiles_changes[i].profile_num = ds_profile_db_get_profile_num(profiles[i]);
    profiles_changes[i].profile_tech = ds_techmask_to_legacy_tech(
      ds_profile_db_get_supported_tech_mask(profiles[i]));
    if (DS_PROFILE_TECH_INVALID == profiles_changes[i].profile_tech)
    {
      DS_PROFILE_LOGE("_callback_internal: unsupported techmask received from db, %d, ignoring event",
                      ds_profile_db_get_supported_tech_mask(profiles[i]));
      goto bail;
    }

    idents = NULL;
    gen_idents = NULL;
    res = ds_profile_db_get_changed_idents(profiles[i],
                                           legacy_tech_to_techmask(cb_info_node->tech),
                                           &idents,
                                           &idents_count,
                                           &gen_idents,
                                           &gen_idents_count);
    if (DB_RESULT_SUCCESS != res)
    {
      goto bail;
    }

    count = gen_idents_count + idents_count;
    profiles_changes[i].idents_num = count;

    if (0 != count)
    {
      profiles_changes[i].changed_idents_array =
        (ds_profile_identifier_type *)
        DS_PROFILE_MEM_ALLOC(sizeof(ds_profile_identifier_type) * count,
                             MODEM_MEM_CLIENT_DATA);

      if (NULL == profiles_changes[i].changed_idents_array)
      {
        DS_PROFILE_LOGE("_callback_internal: FAIL DS_PROFILE_MEM_ALLOC", 0);
        goto bail;
      }

      memset(profiles_changes[i].changed_idents_array, 0,
             sizeof(ds_profile_identifier_type) * count);
    }

    for (j = 0; j < idents_count; j++)
    {
      if (idents)
      {
        profiles_changes[i].changed_idents_array[j] =
          db_ident_to_legacy_ident(idents[j], cb_info_node->tech);
      }
    }

    for (j = idents_count; j < count; j++)
    {
      if (gen_idents)
      {
        profiles_changes[i].changed_idents_array[j] =
          db_ident_to_legacy_ident(gen_idents[j - idents_count], cb_info_node->tech);
      }
    }

    ds_profile_db_free_idents(idents);
    idents = NULL;
    ds_profile_db_free_idents(gen_idents);
    gen_idents = NULL;
  }

bail1:

  if (cb_info_node->cb_per_sub)
  {
    (*cb_info_node->ds_profile_call_back.cback_per_sub)((ds_profile_event_etype)event,
                                                        profile_count,
                                                        profiles_changes,
                                                        cb_info_node->user_data,
                                                        (ds_profile_subs_etype)subs_id);
    DS_PROFILE_LOGD_2("ds_profile_callback_internal_per_sub: invoking user callback 0x%x, "
                      "user_data 0x%x",cb_info_node->ds_profile_call_back.cback_per_sub, 
                      user_data);
  } 
  else
  {
    (*cb_info_node->ds_profile_call_back.cback)((ds_profile_event_etype)event,
                                                profile_count,
                                                profiles_changes,
                                                cb_info_node->user_data);
    DS_PROFILE_LOGD_2("ds_profile_callback_internal: invoking user callback 0x%x, user_data 0x%x",
                      cb_info_node->ds_profile_call_back.cback, user_data);
  }

  ds_profile_db_free_profiles(profiles, profile_count); 

  /* Release callback lock here so that no race condition happens */
  if (ds_profile_lock_rel(&callback_lock) != 0)
  {
    DS_PROFILE_LOGE("FAIL: unable to release callback lock", 0);
  }

  if (profiles_changes)
  {
    for (i = 0; i < profile_count; i++)
    {
      if (profiles_changes[i].changed_idents_array)
      {
        DS_PROFILE_MEM_FREE(profiles_changes[i].changed_idents_array, MODEM_MEM_CLIENT_DATA);
        profiles_changes[i].changed_idents_array =  NULL;
      }
    }
  }
  DS_PROFILE_MEM_FREE(profiles_changes, MODEM_MEM_CLIENT_DATA);
  profiles_changes = NULL;
  return;


bail:

  /* Release callback lock here otherwise it may result in deadlock */
  if (ds_profile_lock_rel(&callback_lock) != 0)
  {
    DS_PROFILE_LOGE("FAIL: unable to release callback lock", 0);
  }

  if (profiles_changes)
  {
    for (i = 0; i < profile_count; i++)
    {
      if (profiles_changes[i].changed_idents_array)
      {
        DS_PROFILE_MEM_FREE(profiles_changes[i].changed_idents_array, MODEM_MEM_CLIENT_DATA);
      }
    }
  }

  DS_PROFILE_MEM_FREE(profiles_changes, MODEM_MEM_CLIENT_DATA);

  if (idents)
  {
    ds_profile_db_free_idents(idents);
    idents = NULL;
  }

  if (gen_idents)
  {
    ds_profile_db_free_idents(gen_idents);
    gen_idents = NULL;
  }

  ds_profile_db_free_profiles(profiles, profile_count);

}

/*===========================================================================
FUNCTION DS_PROFILE_REGISTER_CALLBACK
============================================================================*/
/** @ingroup ds_profile_register_callback
  Registers for notifications triggered by changes in profiles.
  User can watch either specific profile, or all profiles of a particular technology.
 
  @datatypes
  #ds_profile_num_type\n
  #ds_profile_tech_etype\n
  #ds_profile_cb_handle_type\n
  #ds_profile_cb_type\n

  @vertspace
  @param[in] num		   Profile number to observe. If you do not wish to observe a particular
                           profile, use DS_PROFILE_UNSPECIFIED_NUM. 
                           Registration per particular number and technology is only allowed 
                           for existing profile with matching technology
  @param[in] tech   	   Technology to observe (if a particular profile is observed, technology should
                           match profile's technology)
  @param[in] cback  	   Function to be called upon event
  @param[in] user_data     User can provide data to be passed to callback function upon event
 
  @return
  Valid cb handle in case of success, 0 in case of failure

  @dependencies
  The software library must be initialized.
  @newpage
*/
ds_profile_cb_handle_type ds_profile_register_callback(
  ds_profile_num_type num,
  ds_profile_tech_etype tech,
  ds_profile_cb_type cback,
  void *user_data
  )
{
  ds_profile_db_callback_id_type db_callback_id;
  ds_profile_cback_func_info_node cb_info_node;
  static uint32 callback_id = 1;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();
  typedef enum 
  {
    DS_PROFILE_LOCAL_ERR_NONE            = 0,
    DS_PROFILE_LOCAL_ERR_INVAL_LIB_STATE = 1,
    DS_PROFILE_LOCAL_ERR_INVAL_TECH      = 2,
    DS_PROFILE_LOCAL_ERR_INVAL_DB_CB_ID  = 3,
    DS_PROFILE_LOCAL_ERR_LIST_ADD        = 4
  }ds_profile_local_error_type;

  ds_profile_local_error_type local_error = 
    DS_PROFILE_LOCAL_ERR_NONE;

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      local_error = DS_PROFILE_LOCAL_ERR_INVAL_LIB_STATE;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      local_error = DS_PROFILE_LOCAL_ERR_INVAL_TECH;
      break;
    }

    ACQ_LOCK("_callback_register");


    /* allocate a new number in a cyclic manner */
    if (0 == ++callback_id)
    {
      callback_id = 1;
    }

    /*-------------------------------------------------------------------- 
      Call directly ds_profile_db interface
    --------------------------------------------------------------------*/
    db_callback_id = ds_profile_db_register_for_notifications(legacy_num_to_db_num(num),
                                                              legacy_tech_to_techmask(tech),
                                                              ds_profile_callback_internal,
                                                              (ds_profile_db_subs_type)subs_id,
                                                              (void *)callback_id);
    if (0 == db_callback_id)
    {
      local_error = DS_PROFILE_LOCAL_ERR_INVAL_DB_CB_ID;
      REL_LOCK("_callback_register");
      break;
    }

    cb_info_node.cb_handle = callback_id;
    cb_info_node.db_cb_handle = db_callback_id;
    cb_info_node.ds_profile_call_back.cback = cback;
    cb_info_node.user_data = user_data;
    cb_info_node.tech = tech;
    cb_info_node.cb_per_sub = FALSE;

    ACQ_LOCK_CALLBACK("_callback_register");

    if (DS_SUCCESS != ds_util_list_add(callbacks_list, &cb_info_node, sizeof(cb_info_node)))
    {
      local_error = DS_PROFILE_LOCAL_ERR_LIST_ADD;
      REL_LOCK_CALLBACK("_callback_register");
      ds_profile_db_deregister_for_notifications(db_callback_id,(ds_profile_db_subs_type)subs_id);

      REL_LOCK("_callback_register");
      break;
    }

    REL_LOCK_CALLBACK("_callback_register");
    REL_LOCK("_callback_register");

  } while (0);
 
  DS_PROFILE_LOGD_5("ds_profile_register_callback: with "
                    "num %d, tech %d, cback 0x%x, "
                    "callback_id %d, local_error %d",
                    num, tech, cback, 
                    callback_id,
                    local_error);

  if (local_error != DS_PROFILE_LOCAL_ERR_NONE) 
  {
    return 0;
  }
  else
  {
    return callback_id;
  }
}

/*===========================================================================
FUNCTION DS_PROFILE_REGISTER_CALLBACK_PER_SUB
============================================================================*/
/** @ingroup ds_profile_register_callback
  Registers for notifications triggered by changes in profiles.
  User can watch either specific profile, or all profiles of a particular technology.
 
  @datatypes
  #ds_profile_num_type\n
  #ds_profile_tech_etype\n
  #ds_profile_cb_handle_type\n
  #ds_profile_cb_type\n

  @vertspace
  @param[in] num		   Profile number to observe. If you do not wish to observe a particular
                           profile, use DS_PROFILE_UNSPECIFIED_NUM. 
                           Registration per particular number and technology is only allowed 
                           for existing profile with matching technology
  @param[in] tech   	   Technology to observe (if a particular profile is observed, technology should
                           match profile's technology)
  @param[in] cback  	   Function to be called upon event
  @param[in] user_data     User can provide data to be passed to callback function upon event
  @param[in] subs_id	   Subscription Id for which the client needs to register
 
  @return
  Valid cb handle in case of success, 0 in case of failure

  @dependencies
  The software library must be initialized.
  @newpage
*/
ds_profile_cb_handle_type ds_profile_register_callback_per_sub(
  ds_profile_num_type num,
  ds_profile_tech_etype tech,
  ds_profile_cb_type_per_sub cback,
  void *user_data,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_db_callback_id_type db_callback_id;
  ds_profile_cback_func_info_node cb_info_node;
  static uint32 callback_id = 1;
  typedef enum 
  {
    DS_PROFILE_LOCAL_ERR_NONE            = 0,
    DS_PROFILE_LOCAL_ERR_INVAL_LIB_STATE = 1,
    DS_PROFILE_LOCAL_ERR_INVAL_TECH      = 2,
    DS_PROFILE_LOCAL_ERR_INVAL_DB_CB_ID  = 3,
    DS_PROFILE_LOCAL_ERR_LIST_ADD        = 4,
    DS_PROFILE_LOCAL_ERR_INVAL_SUBS_ID   = 5
  }ds_profile_local_error_type;

  ds_profile_local_error_type local_error = 0;

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      local_error = 
        DS_PROFILE_LOCAL_ERR_INVAL_LIB_STATE;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      local_error = 
        DS_PROFILE_LOCAL_ERR_INVAL_TECH;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (ds_profile_validate_subs_id(subs_id)))
    {
      local_error = 
        DS_PROFILE_LOCAL_ERR_INVAL_SUBS_ID;
      break;
    }

    ACQ_LOCK("_callback_register");


    /* allocate a new number in a cyclic manner */
    if (0 == ++callback_id)
    {
      callback_id = 1;
    }

    /*-------------------------------------------------------------------- 
      Call directly ds_profile_db interface
    --------------------------------------------------------------------*/
    db_callback_id = 
      ds_profile_db_register_for_notifications(legacy_num_to_db_num(num),
                                               legacy_tech_to_techmask(tech),
                                               ds_profile_callback_internal,
                                               (ds_profile_db_subs_type)subs_id,
                                               (void *)callback_id);
    if (0 == db_callback_id)
    {
      REL_LOCK("_callback_register");
      local_error = 
        DS_PROFILE_LOCAL_ERR_INVAL_DB_CB_ID;
      break;
    }

    cb_info_node.cb_handle = callback_id;
    cb_info_node.db_cb_handle = db_callback_id;
    cb_info_node.ds_profile_call_back.cback_per_sub = cback;
    cb_info_node.user_data = user_data;
    cb_info_node.tech = tech;
    cb_info_node.cb_per_sub = TRUE;

    ACQ_LOCK_CALLBACK("_callback_register");
    if (DS_SUCCESS != ds_util_list_add(callbacks_list, 
                                       &cb_info_node, 
                                       sizeof(cb_info_node)))
    {
      REL_LOCK_CALLBACK("_callback_register");
      ds_profile_db_deregister_for_notifications(db_callback_id,(ds_profile_db_subs_type)subs_id);
      REL_LOCK("_callback_register");
      local_error = DS_PROFILE_LOCAL_ERR_LIST_ADD;
      break;
    }

    REL_LOCK_CALLBACK("_callback_register");
    REL_LOCK("_callback_register");

  } while (0);

  DS_PROFILE_LOGD_5("ds_profile_register_callback: "
                    "with num %d, tech %d, "
                    " local_error %d, "
                    "subs_id %d, callback_id %d",
                    num, tech, local_error, 
                    subs_id, callback_id);

  if (DS_PROFILE_LOCAL_ERR_NONE != local_error) 
  {
    return 0;
  }
  else
  {
    return callback_id;
  }
}

/*===========================================================================
FUNCTION DS_PROFILE_ARE_PROFILES_READY_ON_SUB
============================================================================*/
/** @ingroup ds_profile_profiles_ready_on_sub
  Returns whether profiles are ready on the specified subscription. 
 
  @datatypes
  #ds_profile_subs_etype\n

  @vertspace
  @param[in] subs_id     Profile subscription
   
  @return
  TRUE                -- Profiles Ready on specified sub.\n
  FALSE               -- Profiles Not Ready on specified sub. \n
  
  @dependencies 
  The software library must be initialized.
  @newpage
*/
boolean ds_profile_profiles_ready_on_sub(
  ds_profile_subs_etype subs_id
)
{
  boolean profiles_ready_on_sub = FALSE;
  /* Validate lib state */
  if (!LIB_STATE_IS_VALID(lib_state))
  {
    return FALSE;
  }

  if (DS_PROFILE_REG_RESULT_SUCCESS != (ds_profile_validate_subs_id(subs_id)))
  {
    return FALSE;
  }

  ACQ_LOCK("_profiles_ready_on_sub");
  /*-------------------------------------------------------------------- 
    Call directly ds_profile_db interface
   --------------------------------------------------------------------*/
  profiles_ready_on_sub = 
    ds_profile_db_profiles_ready_on_subs((ds_profile_db_subs_type)subs_id);
  REL_LOCK("_profiles_ready_on_sub");
  return profiles_ready_on_sub;
}


/*===========================================================================
FUNCTION DS_PROFILE_UNREGISTER_CALLBACK
============================================================================*/
/** @ingroup ds_profile_unregister_callback
  Unregisters from notifications  
 
  @datatypes
  #ds_profile_cb_handle_type\n

  @vertspace
  @param[in] cb_handle     Cb handle obtained by ds_profile_register_callback()
  
 
  @return
  DS_PROFILE_REG_RESULT_SUCCESS 			   -- Operation succeeded.\n
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED     -- Library is not initialized.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP		   -- Invoked from within callback
  DS_PROFILE_REG_RESULT_ERR_INVAL   		   -- Invalid callback id
  DS_PROFILE_REG_RESULT_FAIL				   -- General failure. 

  @dependencies 
  The software library must be initialized.
  @newpage
*/
ds_profile_status_etype ds_profile_unregister_callback(
  ds_profile_cb_handle_type cb_handle
  )
{
  ds_profile_db_result_type return_status = DB_RESULT_FAIL;
  ds_profile_cback_func_info_node *cb_info_node = NULL;
  ds_profile_db_callback_id_type db_callback_id;
  ds_profile_subs_etype subs_id = 
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();


  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      break;
    }

    ACQ_LOCK("_callback_unregister");
    ACQ_LOCK_CALLBACK("_callback_unregister");

    cb_info_node = (ds_profile_cback_func_info_node *)
      ds_util_list_find_node(callbacks_list, &cb_handle, sizeof(cb_handle));

    if (NULL == cb_info_node || (0 == cb_info_node->db_cb_handle))
    {
      REL_LOCK_CALLBACK("_callback_unregister");
      REL_LOCK("_callback_unregister");
      break;
    }

    db_callback_id = cb_info_node->db_cb_handle;

    if (DS_SUCCESS != (return_status = ds_util_list_remove(callbacks_list,
                                                           &cb_handle, 
                                                           sizeof(cb_handle))))
    {
      REL_LOCK_CALLBACK("_callback_unregister");
      REL_LOCK("_callback_unregister");
      break;
    }
    REL_LOCK_CALLBACK("_callback_unregister");


    return_status = 
         ds_profile_db_deregister_for_notifications(db_callback_id,(ds_profile_db_subs_type)subs_id);


    REL_LOCK("_callback_unregister");
  } while (0);

    DS_PROFILE_LOGD_2("ds_profile_unregister_callback: with cb_handle 0x%x,"
                    "return_status %d",cb_handle, return_status);

  return (ds_profile_status_etype)return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_UNREGISTER_CALLBACK_PER_SUB
============================================================================*/
/** @ingroup ds_profile_unregister_callback_per_sub
  Unregisters from notifications  
 
  @datatypes
  #ds_profile_cb_handle_type\n

  @vertspace
  @param[in] cb_handle     Cb handle obtained by ds_profile_register_callback()
  @param[in] subs_id	   Subscription Id for callback
  
 
  @return
  DS_PROFILE_REG_RESULT_SUCCESS 			   -- Operation succeeded.\n
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED     -- Library is not initialized.\n
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP		   -- Invoked from within callback
  DS_PROFILE_REG_RESULT_ERR_INVAL   		   -- Invalid callback id
  DS_PROFILE_REG_RESULT_FAIL				   -- General failure. 

  @dependencies 
  The software library must be initialized.
  @newpage
*/
ds_profile_status_etype ds_profile_unregister_callback_per_sub(
  ds_profile_cb_handle_type cb_handle,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_db_result_type return_status = DB_RESULT_FAIL;
  ds_profile_cback_func_info_node *cb_info_node = NULL;
  ds_profile_db_callback_id_type db_callback_id;

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      break;
    }

    if (DB_RESULT_SUCCESS !=
          (return_status = (ds_profile_db_result_type)ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    ACQ_LOCK("_callback_unregister");
    ACQ_LOCK_CALLBACK("_callback_unregister");

    cb_info_node = (ds_profile_cback_func_info_node *)
      ds_util_list_find_node(callbacks_list, &cb_handle, sizeof(cb_handle));

    if (NULL == cb_info_node || (0 == cb_info_node->db_cb_handle))
    {
      REL_LOCK_CALLBACK("_callback_unregister");
      REL_LOCK("_callback_unregister");
      break;
    }

    db_callback_id = cb_info_node->db_cb_handle;
    if (DS_SUCCESS != ds_util_list_remove(callbacks_list, 
                                          &cb_handle, 
                                          sizeof(cb_handle)))
    {
      REL_LOCK_CALLBACK("_callback_unregister");
      REL_LOCK("_callback_unregister");
      break;
    }
    REL_LOCK_CALLBACK("_callback_unregister");

    return_status = 
      ds_profile_db_deregister_for_notifications((db_callback_id),(ds_profile_db_subs_type)subs_id);
    REL_LOCK("_callback_unregister");

  } while (0);

  DS_PROFILE_LOGD_3("ds_profile_unregister_callback_per_sub: with "
                    "cb_handle 0x%x, subs_id %d, return_status %d",
                    cb_handle, subs_id, return_status);

  return (ds_profile_status_etype)return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_RELEASE_PROFILES_CHANGES_ARRAY
============================================================================*/
/** @ingroup ds_profile_release_profiles_changes_array
  Release profiles_changes array received as part in callback function.
  Also releases internal arrays per every node
 
  @datatypes
  #ds_profile_changes_info\n  

  @vertspace
  @param[in] profiles_changes - array to be released
  @param[in] profile_count -   number of profiles in array
 
  @dependencies 
  The software library must be initialized.
  @newpage
*/
void ds_profile_release_profiles_changes_array(
  ds_profile_changes_info *profiles_changes,
  unsigned int profile_count
  )
{
  unsigned int i;

  if (profiles_changes)
  {
    for (i = 0; i < profile_count; i++)
    {
      if (profiles_changes[i].changed_idents_array)
      {
        DS_PROFILE_MEM_FREE(profiles_changes[i].changed_idents_array,
                            MODEM_MEM_CLIENT_DATA);
      }
    }
  }

  DS_PROFILE_MEM_FREE(profiles_changes, MODEM_MEM_CLIENT_DATA);
}

#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION DS_PROFILE_UPDATE_LTE_ATTACH_PDN_LIST_PROFILES


DESCRIPTION
  This function updates the profile parameters in the lte attach pdn
  list of the specified technology

PARAMETERS
  tech  : technology type

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_update_lte_attach_pdn_list_profiles(
  ds_profile_tech_etype 	  tech
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    ACQ_LOCK("_update_lte_profiles");
    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific create function
    --------------------------------------------------------------------*/
    return_status = 
      dsi_profile_update_lte_attach_pdn_list_profiles(tech, 
                                                      subs_id);
    REL_LOCK("_update_lte_profiles");

  } while (0);

  DS_PROFILE_LOGD_2("_update_lte_profiles: EXIT with DDS %d, return_Status %d", 
                    subs_id, return_status);
  return return_status;
}
/*lint -restore Restore lint error 655*/


/*===========================================================================
FUNCTION DS_PROFILE_UPDATE_LTE_ATTACH_PDN_LIST_PROFILES_PER_SUBS

DESCRIPTION
  This function updates the profile parameters in the lte attach pdn
  list of the specified technology

PARAMETERS
  tech     : technology type
  subs_id  : subscription id

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL	  : On general errors. This return 
                                    code provides blanket coverage
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_update_lte_attach_pdn_list_profiles_per_subs(
  ds_profile_tech_etype tech,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    /* Validate tech type */
    if (!TECH_IS_VALID(tech))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    ACQ_LOCK("_update_lte_profiles");

    /*-------------------------------------------------------------------- 
      Call internal function which depending on tech calls the
      tech specific create function
    --------------------------------------------------------------------*/
    return_status = dsi_profile_update_lte_attach_pdn_list_profiles(tech, subs_id);
    REL_LOCK("_update_lte_profiles");

  } while (0);

  DS_PROFILE_LOGD_2("_update_lte_profiles per subs: EXIT with "
                    "subs_id %d, return_status %d", 
                    subs_id, return_status);
  return return_status;
}
#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION DS_PROFILE_GET_TECH_TYPE_FROM_PROFILE_NUM

DESCRIPTION
  This function gets the tech type for the given profile num

PARAMETERS
  profile_num : profile number

DEPENDENCIES 
  
RETURN VALUE 
  ds_profile_tech_etype
 
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_tech_etype ds_profile_get_tech_type_from_profile_num(
  ds_profile_num_type profile_num
  )
{
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();
  ds_profile_tech_etype tech_type = DS_PROFILE_TECH_INVALID;

  do 
  {
    if (ds_profile_lock_acq(&lib_lock) != 0)
    {
      tech_type = DS_PROFILE_TECH_INVALID;
      break;
    }

    tech_type = ds_profile_db_handler_get_tech_type(profile_num, subs_id);

    if (ds_profile_lock_rel(&lib_lock) != 0)
    {
      tech_type = DS_PROFILE_TECH_INVALID;
      break;
    }

  } while (0);

  DS_PROFILE_LOGD("Get_Tech_type: exited with tech_type %d", tech_type);
  return tech_type;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_TECH_TYPE_FROM_PROFILE_NUM_PER_SUB

DESCRIPTION
  This function gets the tech type for the given profile num

PARAMETERS
  profile_num : profile number
  subs_id     : Subscription Id.

DEPENDENCIES 
  
RETURN VALUE 
  ds_profile_tech_etype
 
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_tech_etype ds_profile_get_tech_type_from_profile_num_per_sub(
  ds_profile_num_type profile_num,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_tech_etype tech_type = DS_PROFILE_TECH_INVALID;
  if (ds_profile_lock_acq(&lib_lock) != 0)
  {
    DS_PROFILE_LOGE("FAIL: unable to acquire lock", 0);
    return tech_type;
  }

  if (DS_PROFILE_REG_RESULT_SUCCESS !=
        ds_profile_validate_subs_id(subs_id))
  {
    if (ds_profile_lock_rel(&lib_lock) != 0)
    {
      DS_PROFILE_LOGE("FAIL: unable to release lock", 0);
    }
    return DS_PROFILE_TECH_INVALID;
  }

  tech_type = ds_profile_db_handler_get_tech_type(profile_num, subs_id);

  if (ds_profile_lock_rel(&lib_lock) != 0)
  {
    DS_PROFILE_LOGE("FAIL: unable to release lock", 0);
    return DS_PROFILE_TECH_INVALID;
  }
  return tech_type;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_PERSISTENCE_FROM_PROFILE_NUM

DESCRIPTION
  This function gets whether the given profile number is persistent or not.

PARAMETERS
  profile_num : profile number
  is_persistence: boolean

DEPENDENCIES 
  
RETURN VALUE 
  ds_profile_status_etype
 
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_get_persistence_from_profile_num(
  ds_profile_num_type profile_num,
  boolean *is_persistent
  )
{
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();
  ds_profile_status_etype status = DS_PROFILE_REG_RESULT_FAIL;
  if (is_persistent == NULL)
  {
    return DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
  }
  ACQ_LOCK("_get_persistence");
  status = ds_profile_db_handler_get_persistence(profile_num, 
                                                 is_persistent, 
                                                 subs_id);
  REL_LOCK("_get_persistence");
  return status;
}


/*===========================================================================
FUNCTION DS_PROFILE_GET_PERSISTENCE_FROM_PROFILE_NUM_PER_SUB

DESCRIPTION
  This function gets whether the given profile number is persistent or not.

PARAMETERS
  profile_num    : profile number
  is_persistence : boolean
  subs_id   	 : Subscription ID of the profile

DEPENDENCIES 
  
RETURN VALUE 
  ds_profile_status_etype
 
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_get_persistence_from_profile_num_per_sub(
  ds_profile_num_type profile_num,
  boolean *is_persistent,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype status = DS_PROFILE_REG_RESULT_FAIL;
  if (is_persistent == NULL)
  {
    return DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
  }

  if (DS_PROFILE_REG_RESULT_SUCCESS !=
        (status = ds_profile_validate_subs_id(subs_id)))
  {
    return status;
  }

  ACQ_LOCK("_get_persistence");
  status = ds_profile_db_handler_get_persistence(profile_num, 
                                                 is_persistent, 
                                                 subs_id);
  REL_LOCK("_get_persistence");
  return status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_PROFILE_CLIENT_FROM_PROFILE_NUM_PER_SUB

DESCRIPTION
  This function gets whether the given profile number client.

PARAMETERS
  profile_num    : profile number
  profile_client : profile client
  subs_id        : Subscription ID of the profile

DEPENDENCIES 
  
RETURN VALUE 
  ds_profile_status_etype
 
SIDE EFFECTS 
  
===========================================================================*/
ds_profile_status_etype ds_profile_get_profile_client_from_profile_num_per_sub(
  ds_profile_num_type profile_num,
  uint8 *profile_client,
  ds_profile_subs_etype subs_id
)
{
  ds_profile_status_etype status = DS_PROFILE_REG_RESULT_FAIL;
  if (profile_client == NULL)
  {
    return DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
  }

  if (DS_PROFILE_REG_RESULT_SUCCESS != 
      (status = ds_profile_validate_subs_id(subs_id)))
  {
    return status;
  }

  ACQ_LOCK("_get_profile_client");
  status = ds_profile_db_handler_get_profile_client(profile_num, profile_client, subs_id);
  REL_LOCK("_get_profile_client");
  return status;
}


/*===========================================================================
FUNCTION DS_PROFILE_GET_PARAM_IN_USE

DESCRIPTION
  This function is used to get Profile data element identified by the 
  identifier. The identifiers are specified in the corresponding tech 
  header file. The data elements are read from the prefetched Profile and
  info is returned with that value and length. The profile type should be
  EPC type and only common parameters can be queried for a particular 
  tech_type.

PARAMETERS
  profile_hndl : handle to profile to get profile data element
  identifier   : to identify profile data element
  info  	   : pointer to store value and length of data element
                 (size of buffer allocated should atleast be the max size of 
                  the parameter which needs to be fetched)
  tech_type    : Tech Type needs to be either 3GPP/3GPP2 

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 		   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL     : Invalid handle 
  DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT    : Invalid identifier
  DS_PROFILE_REG_RESULT_ERR_LEN_INVALID    : Buffer size less than required
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type 
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_FAIL			   : On general errors 
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_get_param_in_use(
  ds_profile_hndl_type  	  hndl,
  ds_profile_identifier_type  identifier,
  ds_profile_info_type  	  *info,
  ds_profile_tech_etype 	  tech_type
  )
{

  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_subs_etype subs_id =
    (ds_profile_subs_etype)ps_sys_get_default_data_subscription();

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }
    /* Validate handle */
    if (!HNDL_IS_VALID(hndl))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
      break;
    }

    ACQ_LOCK("_get_param_in_use");
    /*-------------------------------------------------------------------- 
      Call internal function, depending on tech and identifier
      calls tech-specific get function
    --------------------------------------------------------------------*/
    return_status = dsi_profile_get_param_in_use(hndl, identifier,
                                                 info, tech_type,
                                                 subs_id);
    REL_LOCK("_get_param_in_use");

  } while (0);

  DS_PROFILE_LOGD_3("ds_profile_get_param_in_use: exited "
                    "with hndl %d, ident %d, return_status %d",
                    hndl, identifier, return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_GET_PARAM_IN_USE_PER_SUB

DESCRIPTION
  This function is used to get Profile data element identified by the 
  identifier. The identifiers are specified in the corresponding tech 
  header file. The data elements are read from the prefetched Profile and
  info is returned with that value and length. The profile type should be
  EPC type and only common parameters can be queried for a particular 
  tech_type.

PARAMETERS
  profile_hndl : handle to profile to get profile data element
  identifier   : to identify profile data element
  info  	   : pointer to store value and length of data element
                 (size of buffer allocated should atleast be the max size of 
                  the parameter which needs to be fetched)
  tech_type    : Tech Type needs to be either 3GPP/3GPP2
  subs_id      : Subscription on which the profile operation has to be carried
                 out.

DEPENDENCIES 
  
RETURN VALUE 
  DS_PROFILE_REG_RESULT_SUCCESS 			   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED     : Library is not initialized
  DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL  	   : Invalid handle 
  DS_PROFILE_REG_RESULT_ERR_INVAL_IDENT 	   : Invalid identifier
  DS_PROFILE_REG_RESULT_ERR_LEN_INVALID 	   : Buffer size less than required
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Invalid tech type 
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_FAIL				   : On general errors 
SIDE EFFECTS 
 
===========================================================================*/
ds_profile_status_etype ds_profile_get_param_in_use_per_sub(
  ds_profile_hndl_type hndl,
  ds_profile_identifier_type identifier,
  ds_profile_info_type *info,
  ds_profile_tech_etype tech_type,
  ds_profile_subs_etype subs_id
  )
{
  ds_profile_status_etype return_status = DS_PROFILE_REG_RESULT_FAIL;

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }
    /* Validate handle */
    if (!HNDL_IS_VALID(hndl))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL_HNDL;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    ACQ_LOCK("_get_param_in_use");
    /*-------------------------------------------------------------------- 
      Call internal function, depending on tech and identifier
      calls tech-specific get function
    --------------------------------------------------------------------*/
    return_status = dsi_profile_get_param_in_use(hndl, identifier,
                                                 info, tech_type,
                                                 subs_id);

    REL_LOCK("_get_param_in_use");

  } while (0);

  DS_PROFILE_LOGD_4("ds_profile_get_param_in_use_per_sub: "
                    "exited with hndl %d, ident %d, subs_id %d, "
                    "retrun_status %d",
                    hndl, identifier, subs_id, return_status);

  return return_status;
}


/*===========================================================================
FUNCTION DS_PROFILE_IS_VALID_PROFILE_PER_SUB

DESCRIPTION
  This function returns if the profile is valid for a given
  Profile number, subscription and tech_type.

PARAMETERS
  profile_num : profile number
  subs_id     : Subscription Id.

DEPENDENCIES 
  
RETURN VALUE 
  ds_profile_tech_etype
 
SIDE EFFECTS 
  
===========================================================================*/

ds_profile_status_etype ds_profile_is_valid_profile_per_sub(
  ds_profile_num_type profile_num,
  ds_profile_subs_etype subs_id,
  ds_profile_tech_etype tech_type,
  boolean *is_valid
  )
{
  ds_profile_status_etype return_status = 
    DS_PROFILE_REG_RESULT_SUCCESS;

  do 
  {
    /* Validate lib state */
    if (!LIB_STATE_IS_VALID(lib_state))
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED;
      break;
    }

    if (is_valid == NULL)
    {
      return_status = DS_PROFILE_REG_RESULT_ERR_INVAL;
      break;
    }

    if (DS_PROFILE_REG_RESULT_SUCCESS !=
          (return_status = ds_profile_validate_subs_id(subs_id)))
    {
      break;
    }

    ACQ_LOCK("_is_valid_profile_per_sub");

    tech_type = ds_profile_db_handler_is_valid_profile(profile_num, 
                                                       subs_id, 
                                                       tech_type, is_valid);

    REL_LOCK("_is_valid_profile_per_sub");
    
  } while (0);

  DS_PROFILE_LOGD("_is_valid_profile_per_sub: EXIT with ERR %d", return_status);
  return return_status;
}

/*===========================================================================
FUNCTION DS_PROFILE_IS_TECH_VALID

DESCRIPTION
  This function check if the tech type passed is valid or not.

PARAMETERS 
  ds_profile_tech_etype    tech (ENUM type)

DEPENDENCIES 
NONE 
  
RETURN VALUE 
TRUE: if tech is valid 
FALSE otherwise 
  
SIDE EFFECTS 
NONE 
 
===========================================================================*/
boolean ds_profile_is_tech_valid
(
  ds_profile_tech_etype    tech
  )
{
  return dsi_profile_is_tech_valid(tech);
}
