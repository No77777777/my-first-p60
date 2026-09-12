/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               D S C A M 

GENERAL DESCRIPTION
  This module contains the coding for CAM module. CUrrently it runs in DS
  task context. But may be moved to soem other task (eg CM or SD) context
  in future.  Thsi file has functions to handle CAM related commands posted
  from DS module and internal CAM State Machine  and static variables
  associated with this state machine. 


EXTERNALIZED FUNCTIONS
  These functions are defined in DSCAM.H and are external to the  CAM
  module

  ds_cam_process_cmd()
    Processes CAM commadn received by DS task ( currently).


  FEATURE_DS_CAM  - Supports CAM ( Chatty App Manager) module in Hybrid mode

 Copyright (c) 2004-2017 by Qualcomm Technologies Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/



/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/src/dscam.c#6 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/01/16    sc     Add a condition check so that CM calls for listening to 
                   SO33 events are only invoked when FR is enabled
02/08/12    sk     Feature cleanup.
12/23/11    sb     Fixed compiler warnings 
10/24/11    ty     Changed the dscami_process_cam_timer_expiry, dscami_process_cam_call_dormant, 
                   dscami_process_flow_ctrl_timer_expiry, dscam_timer_cb functions to not be static.
                   Done to expose functions to test classes. 
07/27/11    sn     Fix to initialize new CAM timer variable to default CAM timer
                   value to avoid CAM timer getting set to 0 when call ends.
06/13/11    dvk    Merged support to get and set CAM timer value at runtime.
03/15/11    mg     Global variable cleanup
10/13/10    op     Migrated to MSG 2.0 macros
10/07/10    ttv    Added messages which explain the reason for asserting.
03/26/10    gc     Dont allow CAM to set CTA timeout to 1sec in 1x only mode
09/29/09    ms     Post CAM GO NULL CMD only if CAM is not in IDLE STATE.
07/31/09    ls     Cancel DSCAM timer when iface in IFACE_COMING_UP, 
                   IFACE_CONFIGURING, IFACE_ROUTEABLE and IFACE_LINGERING
11/26/08    psng   Fixed off target lint errors.
05/10/07    ac     Lint fix
10/31/06    kvd    Corrected an F3 msg in dscami_process_cam_no_evdo_coloc_cmd
06/23/06    kvd    check for iface state == iFACE_GOING_DOWN before issuing
                   DS_CAM_CALL_GONE_NULL_CMD
06/12/06    vp     SERIALIZER fixes.
10/21/05    as/kvd Changed dscami_707_event_handler_cb to only post the
                   DS_CAM_CALL_GONE_NULL command when IFACE_UP.
04/26/05    vrk    Created diag events to enable automated testing
04/04/05    kvd    donot restore cam CTA timeout on transition to NULL.
10/08/04    kvd    Created Initial Version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "cm.h"

#ifdef FEATURE_HDR
#ifdef FEATURE_DS_CAM

#include "ds3gtimer.h"
#include "ds3gsiolib.h"
#include "err.h"
#include "data_msg.h"
#include "rex.h"
#include "ds707.h"
#include "ds707_pkt_mgr.h"
#include "ds707_pkt_mgr_hdr.h"
#include "ds707_pkt_mgri.h"
#include "ds707_cback.h"
#include "dstaski.h"
#include "ds707_cta_rlp.h"
#include "ds_flow_control.h"
#include "dscam.h"
#include "ds707_event_defs.h"
#include "data_err_suppr.h"
#include "nv.h"        /* Interface to NV services */
#include "ds707_pdn_context.h"
#include "ds707_nv_util.h"
#include "dsutil.h"

#include "policyman.h"
#include "cai.h"
#include "ds3gmgr.h"

#define DSCAM_1X_LTE_HANDUP_EFS_PATH \
       "/nv/item_files/data/3gpp2/1x_to_LTE_handup_enabled"

#define DSCAM_IDLE_PERIOD_EFS_PATH \
        "/nv/item_files/data/3gpp2/dscam_reduced_dormancy_timer"
/*===========================================================================
            REGIONAL DEFINITIONS AND DECLARATIONS FOR MODULE
===========================================================================*/


/*===========================================================================
            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE
===========================================================================*/

/*---------------------------------------------------------------------------
                           Pre-processor Constants
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
  The time period below represents the duration the cam module waits
  since call is connected on 1X, before it decided to swicth to evdo, if
  colocated withe EVDO.
---------------------------------------------------------------------------*/
#define DS_CAM_TIMER                300000 /* 5mnts =  5*60*1000 msec */

/*---------------------------------------------------------------------------
  Maximum number of back off timer values that can be read from EFS file
  "/eHRPD/SRLTE_reselection_timer_config.txt"
---------------------------------------------------------------------------*/
#define DS_MAX_NO_BACKOFF_TIMER     4

/*---------------------------------------------------------------------------
  Default back off timer value in seconds. 5minutes
---------------------------------------------------------------------------*/
#define DS_DEFAULT_BACKOFF_TIMER_VAL 300

/*---------------------------------------------------------------------------
  Default extension timer value in seconds. 10seconds
---------------------------------------------------------------------------*/
#define DS_DEFAULT_EXTENSION_TIMER_VAL 10


#define DS_CAM_CONFIG_MAX_PARAMS_NUM         DS_CAM_CONFIG_PARAM_ID_MAX
#define DS_CAM_CONFIG_MAX_PARAM_NAME_SIZE    100
#define DSCAM_IDLE_PERIOD_FOR_ELEVATOR_EXIT  0

/*---------------------------------------------------------------------------
  Array that stores 1X SRLTE back off timer values that are read from
  EFS file during power up. The default value would be 5 mins
---------------------------------------------------------------------------*/
static uint16 backoff_timer_val[DS_MAX_NO_BACKOFF_TIMER];

static uint8 next_backoff_index = 0;

/*---------------------------------------------------------------------------
                           User configured Block data timer
---------------------------------------------------------------------------*/
uint16 new_flow_control_timer_val = 0; // In milli seconds
uint16 dscam_extension_timer_val  = DS_DEFAULT_EXTENSION_TIMER_VAL * 1000; 
                                      // In milli seconds

/*---------------------------------------------------------------------------
   Reduced dormancy timer for DSCAM
---------------------------------------------------------------------------*/
static uint32 dscam_idle_period_val = DS_CAM_IDLE_PERIOD_DEF_VAL;

/*---------------------------------------------------------------------------
  An array of strings to map the Parameter Name string (mentioned in EFS
  file) to a token ID.
---------------------------------------------------------------------------*/
static char ds_cam_config_param_name_array[DS_CAM_CONFIG_MAX_PARAMS_NUM]
              [DS_CAM_CONFIG_MAX_PARAM_NAME_SIZE] = 
                { "DS_CAM_EXTENSION_TIMER" };

static boolean elevator_exit_ind = FALSE;
/*---------------------------------------------------------------------------
                           Enum Typedefs
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Type that represents the current state of the CAM module.
---------------------------------------------------------------------------*/

typedef struct
{
  dscam_cam_state_enum_type      cam_state;
  dscam_cam_callstate_enum_type  cam_callstate;
  rex_timer_type                 cam_active_timer;   /* CAM activity timer*/
  rex_timer_type                 cam_flow_ctrl_timer;/* CAM flowctrl timer*/
} dscam_type;

/*===========================================================================
                        Variable Definitions
===========================================================================*/

static dscam_type dscam;
static dword      dscam_timer_val = DS_CAM_TIMER;
static dword      dscam_timer_new_val = DS_CAM_TIMER;

/*---------------------------------------------------------------------------
  Function prototypes to allow the desired ordering of functions within this
  module - forward declarations.
---------------------------------------------------------------------------*/
static void dscami_cam_transition_state( dscam_cam_state_enum_type new_state );
static void dscami_process_cam_so33_call_connect( void );
static void dscami_process_cam_call_gone_null( void );
static void dscami_process_cam_evdo_coloc_cmd( void );
static void dscami_process_cam_no_evdo_coloc_cmd( void );
static void dscami_timer_init( void );
static void dscami_start_cam_timer( dword duration );
static void dscami_cancel_cam_timer( void );
static void dscami_start_cam_flow_ctrl_timer( dword duration );
static void dscami_cancel_cam_flow_ctrl_timer( void );
static void dscami_get_coloc_info ( void );
static void dscami_707_event_handler_cb
(
  ds707_cback_event_type               event,
  const ds707_cback_event_info_type   *event_info_ptr
);
static int dscami_reg_phys_link_events ( dscam_type *cam );
static void dscami_elevator_exit_ind_msg_hdlr
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);
static void dscami_process_elevator_exit_ind();

boolean dscam_srlte_device = FALSE;

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================

FUNCTION    DSCAM_READ_POLICYMAN_UEMODE

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS     
  None
===========================================================================*/
sys_ue_mode_e_type dscam_read_policyman_ueMode( void )
{
  const policyman_item_t *pItem = NULL;
  const policyman_item_id_t id = POLICYMAN_ITEM_UE_MODE;
  sys_ue_mode_e_type   ueMode = SYS_UE_MODE_NONE;

  if(POLICYMAN_SUCCEEDED(policyman_get_items_block(&id, 1, &pItem)))
  {
    if(!POLICYMAN_SUCCEEDED(policyman_get_ue_mode(pItem,&ueMode)))
    {
      ueMode = SYS_UE_MODE_NONE;      
    }
  }
  
  policyman_item_release(pItem);

  return ueMode;
}/* dscam_read_policyman_ueMode()*/

/*===========================================================================
FUNCTION      ds_cam_config_get_token_id

DESCRIPTION   This function returns the token id associated with each
              ds_cam_config rule parameter. If multiple token ids are found
              within a particular token set, the last duplicate token id value
              is to be retained as the actual token value assc. with
              the duplicate token id.

PARAMETERS    char *from - start of the token (param name)
              char *to   - end of the token (param name)
              uint8 *token_id_ptr

DEPENDENCIES  None

RETURN VALUE  0 - success
              1 - failure

SIDE EFFECTS  None
===========================================================================*/
int ds_cam_config_get_token_id
(
  char    *from,
  char    *to,
  uint8   *token_id_ptr
)
{
  int   ret_val = -1;     /* return value */
  uint8 i       = 0;      /* counter for number of params */
  uint8 length  = 0;      /* length of the token (param name) */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
    Get the length of the string that is passed
  -------------------------------------------------------------------------*/
  if  ( NULL == from || NULL == to || NULL == token_id_ptr || from > to )
  {
    DATA_MSG3_ERROR( "ds_cam_get_token_id: Invalid Parameters"
                     "from: 0x%x, to: 0x%x, token_id_ptr: 0x%x ",
                      from,
                      to,
                      token_id_ptr );
    return ret_val;
  }

  /*-------------------------------------------------------------------------
   Assumption that the token length will not be greater than 255 
  -------------------------------------------------------------------------*/
  length =(uint8)(to-from);

  for ( i = DS_CAM_CONFIG_PARAM_ID_MIN;
        i < DS_CAM_CONFIG_PARAM_ID_MAX;
        i++ 
      )
  {
    if ( ( 0 < length ) && 
         ( length == strlen(ds_cam_config_param_name_array[i]) ) &&
         (0 == strncasecmp(from,ds_cam_config_param_name_array[i],length) )
       )
    {
      /*---------------------------------------------------------------------
        Set the token id contents to point to the index of matching token
      -----------------------------------------------------------------------*/
      *token_id_ptr = i;
      ret_val = 0;
      break;
    }
  }
  return ret_val;

}/* ds_cam_get_token_id() */

/*===========================================================================
FUNCTION      ds_cam_config_set_param_value

DESCRIPTION   This function sets the values of a parameter in the cache.

PARAMETERS    uint8 token_id
              char *from - start of the token (param name)
              char *to   - end of the token (param name)

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_cam_config_set_param_value
(
  uint8 token_id,
  char *from,
  char *to
)
{
  uint8               length = 0;
  int                 atoi_result = 0;
/*-------------------------------------------------------------------------*/

  switch (token_id)
  {
    case DS_CAM_EXTENSION_TIMER:
    {
      length = to - from;
      if ((length > 0) && (length < DS_EFS_READ_BUFFER_SZ))
      {               
        DSUTIL_ATOI( from,to,atoi_result );
        
        if (atoi_result != 0)
        {
          dscam_extension_timer_val= (uint16)atoi_result * 1000;
        }
        
        DATA_IS707_MSG1(MSG_LEGACY_HIGH,
                        "DS_CAM_EXTENSION_TIMER: %d ms",
                        dscam_extension_timer_val);  
      }
      else
      {
        DATA_MSG0_ERROR("Invalid token length DS_CAM_EXTENSION_TIMER" );
      }
      break;
    }/* DS_CAM_EXTENSION_TIMER */
/*----------------------------------------------------------------------------*/
    default:
    {
      DATA_MSG1_ERROR("Invalid token id %d", token_id );
      break;
    }/*default*/
  }/*switch()*/
}/*ds_cam_config_set_param_value*/

/*===========================================================================

FUNCTION 		DSCAM_GET_FLOW_CONTROL_TIMER_VAL

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void dscam_get_flow_control_timer_val(void)
{
  char efs_file_name[51]="/eHRPD/SRLTE_CHATTY_APP_FLOW_CNTL_timer_config.txt";
  char *from, *to;           /* ptrs to start and end of the token     */
  ds3gcfgmgr_efs_token_type  efs_db; /* structure used for tokenizing the file */
  ds3gcfgmgr_efs_token_parse_status_type  efs_ret_val
                                              = DS3GCFGMGR_EFS_TOKEN_PARSE_SUCCESS;
  uint16 length = 0;
  int   atoi_result=0;
//  int i;
  uint16  temp_flow_control_timer_val 	= DS_CAM_FLOW_CTRL_TIMER; //Initialize with 6 seconds

  if(ds3gcfgmgr_efs_file_init_ex(efs_file_name, 
                                 ds707_curr_subs_id(), &efs_db) == -1)
  {
    DATA_IS707_MSG0(MSG_LEGACY_HIGH, "Error opening file ISN from EFS");
	new_flow_control_timer_val 	=  temp_flow_control_timer_val;
	DATA_IS707_MSG1(MSG_LEGACY_HIGH,"flow control timer val: %d ms",new_flow_control_timer_val); 
    return;
  }

  /* Set the seperator as ; */
  efs_db.seperator = ';';

  while (DS3GCFGMGR_EFS_TOKEN_PARSE_EOF
         != (efs_ret_val = ds3gcfgmgr_efs_tokenizer(&efs_db, &from, &to )))
  {
    if (DS3GCFGMGR_EFS_TOKEN_PARSE_FAILURE == efs_ret_val)
    {
      DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR, "Token Parse Failure");
      break;
    }
    /*------------------------------------------------------------------------
      Token being read. from points to the beginning of the token and
      to point to the end of the token.

      The tokenizer automatically skips blank lines and comments (lines
      beginning with #, so no need to check for them here).
    ------------------------------------------------------------------------*/
    if((from == to) || (DS3GCFGMGR_EFS_TOKEN_PARSE_EOL == efs_ret_val))
    {
      /*----------------------------------------------------------------------
        Skip empty tokens.
      ----------------------------------------------------------------------*/
      continue;
    }
    else if(DS3GCFGMGR_EFS_TOKEN_PARSE_SUCCESS == efs_ret_val)
    {
      length = (uint16)( to - from);
      if ( length <= 0)
      {
        DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR, "Invalid timer value");
      }
      else
      {
        
        DSUTIL_ATOI( from,to,atoi_result );
        
        if (atoi_result == 0)
        {  
          DATA_IS707_MSG1(MSG_LEGACY_HIGH,"flow control timer val: %d seconds",new_flow_control_timer_val);  
        }
        else
        {
          temp_flow_control_timer_val=(uint16)atoi_result * 1000; //convert to ms
          DATA_IS707_MSG1(MSG_LEGACY_HIGH,"over writing flow control timer val: %d second.", temp_flow_control_timer_val); 
        }
      }
    }
  }
  ds3gcfgmgr_efs_file_close(&efs_db);
  new_flow_control_timer_val 	=  temp_flow_control_timer_val;
  DATA_IS707_MSG1(MSG_LEGACY_HIGH,"flow control timer val: %d ms",new_flow_control_timer_val); 
}/*dscam_get_flow_control_timer_val()*/


/*===========================================================================

FUNCTION 		DSCAM_GET_BACKOFF_TIMER_VAL

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void dscam_get_backoff_timer_val(void)
{
  char efs_file_name[42]="/eHRPD/SRLTE_reselection_timer_config.txt";
  char *from, *to;           /* ptrs to start and end of the token     */
  ds3gcfgmgr_efs_token_type  efs_db; /* structure used for tokenizing the file */
  ds3gcfgmgr_efs_token_parse_status_type  efs_ret_val
                                              = DS3GCFGMGR_EFS_TOKEN_PARSE_SUCCESS;
  uint8 backoff_count = 0;
  uint16 length = 0;
  int   atoi_result=0;
  int i;

  /* Initialize the array with default value */
  for (i = 0; i < DS_MAX_NO_BACKOFF_TIMER; i++)
  {
    backoff_timer_val[i] = DS_DEFAULT_BACKOFF_TIMER_VAL;
  }

  if(ds3gcfgmgr_efs_file_init_ex(efs_file_name, 
                                 ds707_curr_subs_id(), &efs_db) == -1)
  {
    DATA_IS707_MSG0(MSG_LEGACY_HIGH, "Error opening file ISN from EFS");
    return;
  }

  /* Set the seperator as ; */
  efs_db.seperator = ';';

  while (DS3GCFGMGR_EFS_TOKEN_PARSE_EOF
         != (efs_ret_val = ds3gcfgmgr_efs_tokenizer(&efs_db, &from, &to )))
  {
    if (DS3GCFGMGR_EFS_TOKEN_PARSE_FAILURE == efs_ret_val)
    {
      DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR, "Token Parse Failure");
      break;
    }
    /*------------------------------------------------------------------------
      Token being read. from points to the beginning of the token and
      to point to the end of the token.

      The tokenizer automatically skips blank lines and comments (lines
      beginning with #, so no need to check for them here).
    ------------------------------------------------------------------------*/
    if((from == to) || (DS3GCFGMGR_EFS_TOKEN_PARSE_EOL == efs_ret_val))
    {
      /*----------------------------------------------------------------------
        Skip empty tokens.
      ----------------------------------------------------------------------*/
      continue;
    }
    else if(DS3GCFGMGR_EFS_TOKEN_PARSE_SUCCESS == efs_ret_val)
    {
      length = (uint16)( to - from);
      if ( length <= 0)
      {
        DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR,
                        "Invalid backoff timer value");
      }
      else
      {
        
        DSUTIL_ATOI( from,to,atoi_result );
        
        if (atoi_result == 0)
        {
          backoff_timer_val[backoff_count] = DS_DEFAULT_BACKOFF_TIMER_VAL;  
          DATA_IS707_MSG2(MSG_LEGACY_HIGH,
                        "Back off timer val at index %d is %d.",
                        backoff_count,backoff_timer_val[backoff_count]);  
        }
        else
        {
          backoff_timer_val[backoff_count]=(uint16)atoi_result;
          DATA_IS707_MSG2(MSG_LEGACY_HIGH,
                        "over writing Back off timer val at index %d "
                        "to %d.",
                        backoff_count,backoff_timer_val[backoff_count]); 
        }
        backoff_count++;
        if (backoff_count >= DS_MAX_NO_BACKOFF_TIMER-1)
        {
          DATA_IS707_MSG0(MSG_LEGACY_HIGH,
                        "Max back off timer values read."); 
          break;
        }
      }
    }

  }
  ds3gcfgmgr_efs_file_close(&efs_db);
  if (backoff_count > 0 && backoff_count <= DS_MAX_NO_BACKOFF_TIMER-1)
  {
    for (i = backoff_count; i < DS_MAX_NO_BACKOFF_TIMER; i++)
    {
      backoff_timer_val[i] = backoff_timer_val[backoff_count-1];
    }
  }  
}/*dscam_get_backoff_timer_val()*/

/*===========================================================================

FUNCTION 		DSCAM_GET_EXTENSION_TIMER_VAL

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void dscam_get_extension_timer_val(void)
{
  char efs_file_name[51]="/eHRPD/extension_timer_config.txt";
  char *from, *to;           /* ptrs to start and end of the token     */
  ds3gcfgmgr_efs_token_type  efs_db; /* structure used for tokenizing the file */
  ds3gcfgmgr_efs_token_parse_status_type  efs_ret_val
                                              = DS3GCFGMGR_EFS_TOKEN_PARSE_SUCCESS;
  boolean                              param_name  = TRUE; 
  uint8                                token_id    = 0;

  /*------------------------------------------------------------------------*/
  
  if(ds3gcfgmgr_efs_file_init_ex(efs_file_name, 
                                 ds707_curr_subs_id(), &efs_db) == -1)
  {
    DATA_IS707_MSG0(MSG_LEGACY_HIGH, "Error opening file ISN from EFS");
    return;
  }

  /* Set the seperator as ; */
  efs_db.seperator = ':';

  while (DS3GCFGMGR_EFS_TOKEN_PARSE_EOF
         != (efs_ret_val = ds3gcfgmgr_efs_tokenizer(&efs_db, &from, &to )))
  {
    /*------------------------------------------------------------------------
      Token being read. from points to the beginning of the token and
      to point to the end of the token.

      The tokenizer automatically skips blank lines and comments (lines
      beginning with #, so no need to check for them here).
    ------------------------------------------------------------------------*/
    if((from == to) || (DS3GCFGMGR_EFS_TOKEN_PARSE_EOL == efs_ret_val))
    {
      /*----------------------------------------------------------------------
        Skip empty tokens.
      ----------------------------------------------------------------------*/
      if (FALSE == param_name)
      {
        param_name = TRUE;
        efs_db.seperator = ':'; 
      }
      continue;
    }
    else if(DS3GCFGMGR_EFS_TOKEN_PARSE_SUCCESS == efs_ret_val)
    {
      /*---------------------------------------------------------------------
           check if we are looking for param name or param value
      ---------------------------------------------------------------------*/
      if ( TRUE == param_name )
      {
        if ( ds_cam_config_get_token_id (from, to, &token_id) < 0 )
        {
          DATA_MSG0_ERROR("Incorrect param name, ignoring");
        }
        else
        {
          DATA_MSG1_MED("Token ID: %d", token_id);
          /*-------------------------------------------------------------
            set param_name as FALSE This means the next token is a
            param value. Set the separator as ;
          -----------------------------------------------------------------*/
          param_name = FALSE;
          efs_db.seperator = ';';
        }
      }
      /*-------------------------------------------------------------------
        This means that the token is a param value
      ---------------------------------------------------------------------*/
      else
      {
        /*------------------------------------------------------------------
          pass in the identifier and param value to extract data
        ------------------------------------------------------------------*/
        ds_cam_config_set_param_value( token_id, from, to);

        /*----------------------------------------------------------------
         set param_name as TRUE This means that next token is a param name
        -------------------------------------------------------------------*/
        param_name = TRUE;

        /* Set the seperator as : */
        efs_db.seperator = ':';
      }      
    }
  }
  ds3gcfgmgr_efs_file_close(&efs_db);  
}/*dscam_get_extension_timer_val()*/


/*===========================================================================

FUNCTION 		DSCAM_GET_SUBS_DEVICE_FEATURE

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean dscam_get_subs_device_feature(void)
{
  uint32   max_subs,subs_index;

  sys_subs_feature_t      subs_device_feature_mode;
  sys_overall_feature_t   device_overall_cap;

  policyman_status_t      status;
  policyman_item_t const  *cm_policyman_device_config = NULL;
  policyman_item_id_t ids[] = { POLICYMAN_ITEM_DEVICE_CONFIGURATION };
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DATA_IS707_MSG0(MSG_LEGACY_HIGH, "dscam_get_subs_device_feature");

  /*------------------------------------------------------------------------ 
	Get the policyman items intially 
    ------------------------------------------------------------------------*/
  status = policyman_get_items_block(
              &ids[0],
              sizeof(ids)/sizeof(policyman_item_id_t),
              &cm_policyman_device_config);
  if (POLICYMAN_FAILED(status))
  {
	DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR, "policyman_get_items_block failed");
    if(cm_policyman_device_config != NULL)
    {
      policyman_item_release(cm_policyman_device_config);
    }
	return FALSE;
  }

	/*------------------------------------------------------------------------ 
    Get the policyman items intially.
     - If MULTISIM, go per subs feature mode
	------------------------------------------------------------------------*/

  status = 
    policyman_device_config_overall_feature(cm_policyman_device_config,
											&device_overall_cap);
  if (POLICYMAN_FAILED(status))
  {
	DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR, "device_config_overall_feature fail");
    policyman_item_release(cm_policyman_device_config);
	return FALSE;
  }

  if ( device_overall_cap != SYS_OVERALL_FEATURE_MODE_MULTISIM )
  {
  	/*------------------------------------------------------------------------ 
  	Single SIM.  .. this needs to be fixed shrawan
  	------------------------------------------------------------------------*/
  	if (SYS_OVERALL_FEATURE_MODE_SRLTE == device_overall_cap) 
  	{
        DATA_IS707_MSG0(MSG_LEGACY_HIGH, "device_overall_cap = SRLTE");
  	  dscam_srlte_device = TRUE;
  	}
  }
  else
  {
	/*------------------------------------------------------------------------ 
	Multi-SIM. 
	------------------------------------------------------------------------*/
    status = 
      policyman_device_config_num_sims(cm_policyman_device_config,(size_t*)&max_subs);
    if (POLICYMAN_FAILED(status) || (max_subs > SYS_MODEM_AS_ID_3))
    {
      DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR, "device_config_overall_feature fail");
      policyman_item_release(cm_policyman_device_config);
      return FALSE;
    }

	/*------------------------------------------------------------------------ 
	We are concerned only with LTE being in this SUB 
	------------------------------------------------------------------------*/
	subs_index = (uint32)ds707_curr_subs_id();
	status = policyman_device_config_get_subs_feature(cm_policyman_device_config,0,
							  subs_index,
							  &subs_device_feature_mode);
	if (POLICYMAN_FAILED(status))
	{  
	  DS_IS707_ERROR_LOG_0(MSG_LEGACY_ERROR, "policyman_device_config_get_subs_feature fail");
          policyman_item_release(cm_policyman_device_config);
	  return FALSE;
	}
	if ( subs_device_feature_mode == SYS_SUBS_FEATURE_MODE_SRLTE 
#ifdef FEATURE_1XSRLTE
      || subs_device_feature_mode == SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE
#endif
    ) 
	{
      DATA_IS707_MSG0(MSG_LEGACY_HIGH, "subs_device_feature_mode = SRLTE");
	  dscam_srlte_device = TRUE;
	}
  }
  if(NULL != cm_policyman_device_config)
  {
    policyman_item_release(cm_policyman_device_config);
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}/*dscam_get_subs_device_feature()*/

/*===========================================================================
FUNCTION DSCAM_CONFIG_INIT

DESCRIPTION
  This function performs config initialization of CAM module.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void dscam_config_init
(
  void
)
{
  nv_stat_enum_type           read_efs_nv_retval;
  boolean                     retval = FALSE; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------
    Read the NV items
  -------------------------------------------------------------------*/
  read_efs_nv_retval = ds3gcfgmgr_read_efs_nv_ex(DSCAM_IDLE_PERIOD_EFS_PATH,
                                      &dscam_idle_period_val,
                                      sizeof(uint32), ds707_curr_subs_id());  
  if (NV_DONE_S == read_efs_nv_retval)
  {
    DATA_IS707_MSG1(MSG_LEGACY_HIGH, "NV item configured. NV value is %d", 
                    dscam_idle_period_val);
  }
  else
  { 
    DATA_IS707_MSG0(MSG_LEGACY_ERROR, "NV item not configured or reading error");
    dscam_idle_period_val = DS_CAM_IDLE_PERIOD_DEF_VAL;
  }

  read_efs_nv_retval = ds3gcfgmgr_read_efs_nv_ex(DSCAM_1X_LTE_HANDUP_EFS_PATH,
                                      &retval,
                                      sizeof(retval), ds707_curr_subs_id());  
  if (NV_DONE_S == read_efs_nv_retval)
  {
    DATA_IS707_MSG1(MSG_LEGACY_HIGH, "NV item configured. NV value is %d", retval);
  }
  else
  { 
    DATA_IS707_MSG0(MSG_LEGACY_ERROR, "NV item not configured or reading error");
    retval = FALSE;
  }

  dscam_get_backoff_timer_val();
  dscam_get_flow_control_timer_val(); //read EFS file for configured flow control
  dscam_get_extension_timer_val();

  next_backoff_index = 0;
  dscam_timer_new_val = backoff_timer_val[next_backoff_index]*1000;

  /*------------------------------------------------------------------
    Initialize SRLTE only if the NV is true.
  -------------------------------------------------------------------*/
  if (retval == TRUE) 
  {
    (void)dscam_get_subs_device_feature();
  }

}

/*===========================================================================

FUNCTION DSCAM_INIT

DESCRIPTION
  This function performs power-up initialization of CAM module.
  This includes initialization  and registration of ds3g timers.

  This function must be called once at data services task startup.
  This could be called from different task context if CAM module
  moves to a different task.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

void  dscam_init
(
  void
)
{

  memset(&dscam,0,sizeof(dscam));

  dscam.cam_state = DS_CAM_IDLE_STATE; 
  dscam.cam_callstate = DS_CAM_CALL_IDLE;

  if(dscami_reg_phys_link_events(&dscam) <0)
  {
    DS_3GPP2_ERROR_FATAL(" CAM module cannot register for phys link events");
  }

  /*-------------------------------------------------------------------------
    Register  ds3g timers for CAM module.
  -------------------------------------------------------------------------*/
  dscami_timer_init();

  dscam_config_init();
  
  ds707_nv_util_read_elevator_exit_support();

  if(ds707_pkt_mgr_get_elevator_exit_support())
  {
    ds3gmsgr_reg_msgr_cb(QMI_NAS_PRIORITIZE_LTE_IND,
                (ds3g_msgr_client_func_ptr)dscami_elevator_exit_ind_msg_hdlr);
  }
  
} /* dscam_init() */


/*= = = = = = = =  = = = = = = = = = =  = = =  = = = = = = = = = = = = = = =
                        INTERNAL FUNCTION DEFINTIONS
= = = = = = = = = = = = = = =  = = = = =  = = = = = = = = = = = = = = = = =*/
/*===========================================================================
FUNCTION       DSCAMI_707_EVENT_HANDLER_CB

DESCRIPTION    Callback when various ds707 events occur.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None.
===========================================================================*/
static void dscami_707_event_handler_cb
(
  ds707_cback_event_type               event,
  const ds707_cback_event_info_type   *event_info_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch(event)
  {
    case DS707_CBACK_PHYS_LINK_UP_EV:
      DATA_IS707_MSG0(MSG_LEGACY_HIGH, 
                        "DBG -- Read dscam_get_extension_timer_val");
      dscam_get_extension_timer_val();
      if( DS_ISDATA_SO33(ds707_pkt_get_prev_call_so()))
      {
        DATA_IS707_MSG0(MSG_LEGACY_HIGH, 
                        "call connected on 1X: posting CAM CALL_CONNECTED");
        dscam_send_cam_cmd( DS_CMD_CAM_SO33_CALL_CONNECTED );
      }
      else
      {
        DATA_IS707_MSG0(MSG_LEGACY_HIGH, "call connected with NON SO33");
        dscam_send_cam_cmd( DS_CMD_CAM_NON_SO33_CALL_CONNECTED );
      }
      break;

    case DS707_CBACK_PHYS_LINK_DOWN_EV:
      if ( event_info_ptr->going_dormant == TRUE )
      {
        /* GO dormant */
        dscam_send_cam_cmd(DS_CMD_CAM_CALL_DORMANT);
        break;
      }
      /*  fall throught for GO NULL state   */
    case DS707_CBACK_ABORT_SESSION_EV: 
      /* If its in CAM_IDLE state, then we dont need to post a gone null command
       * again to CAM module as it simply ignores it.
       */
      if(dscam.cam_state != DS_CAM_IDLE_STATE)
      {
          dscam_send_cam_cmd(DS_CMD_CAM_CALL_GONE_NULL);
      }

      break;

    default:
      DS_IS707_ERROR_LOG_1(MSG_LEGACY_ERROR,"Unknown ds707 event %d for CAM",event);	
      DS3GPP2_ASSERT(0);      
      break;
  }
} /* dscami_707_event_handler_cback() */

/*===========================================================================
FUNCTION DSCAMI_REG_PHYS_LINK_EVENTS()

DESCRIPTION
  The common dscam_init() invokes this function to register
  for the phys link events.
  
DEPENDENCIES
  None.

RETURN VALUE
  0: on success
 -1: on failure
 
SIDE EFFECTS
  None.
===========================================================================*/
static int dscami_reg_phys_link_events
( 
  dscam_type *cam
)
{
  int result = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DATA_MDM_ARG_NOT_CONST(cam);

  /* Register phy link down event */
  if (ds707_cback_register( DS707_CBACK_PHYS_LINK_DOWN_EV, 
                            dscami_707_event_handler_cb) == FALSE)
  {
    DS_3GPP2_ERROR_FATAL("Could not reg PHYS_LINK_DOWN_EV cback");
  }

  /* Register phy link up event */
  if (ds707_cback_register(DS707_CBACK_PHYS_LINK_UP_EV,
                           dscami_707_event_handler_cb) == FALSE)
  {
    DS_3GPP2_ERROR_FATAL("Could not reg PHYS_LINK_UP_EV cback");
  }

  /* Register abort session event which will send phy gone ind */
  if (ds707_cback_register(DS707_CBACK_ABORT_SESSION_EV,
                           dscami_707_event_handler_cb) == FALSE)
  {
    DS_3GPP2_ERROR_FATAL("Could not reg PHYS_LINK_UP_EV cback");
  }
  return result;
} /* dscami_reg_phys_link_events() */

/*===========================================================================
FUNCTION      DSCAMI_TIMER_INIT

DESCRIPTION   Register 3g tiemrs for CAM moudle.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void dscami_timer_init(void)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Register  ds3g timers for CAM module.
  -------------------------------------------------------------------------*/
  ds3g_timer_register(&dscam.cam_active_timer,
                       dscam_timer_cb,
                       DS3G_TIMER_CAM);
  ds3g_timer_register(&dscam.cam_flow_ctrl_timer,
                       dscam_timer_cb,
                       DS3G_TIMER_CAM_FLOW_CTRL);
} /* dscami_timer_init() */

/*===========================================================================
FUNCTION      DSCAM_TIMER_CB

DESCRIPTION   Callback for the  CAM activation & flow control timers. 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  Timer expiation cmds posted to CAM module.
===========================================================================*/
void dscam_timer_cb
(
  unsigned long    timer_id
)
{
  ds_cmd_type           *cmd_ptr;                    /* Pointer to command */
  ds_cmd_enum_type      cmd_id;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ((ds3g_timer_enum_type)timer_id)
  {

    case DS3G_TIMER_CAM:
      cmd_id = DS_CMD_CAM_TIMER_EXPIRED;
      break;

    case DS3G_TIMER_CAM_FLOW_CTRL:
      cmd_id = DS_CMD_CAM_FLOW_CTRL_TIMER_EXPIRED;
      break;

    default:
      DS_IS707_ERROR_LOG_1(MSG_LEGACY_ERROR, "Bad timer id on dscam callback %d",
                      timer_id);
      DS3GPP2_ASSERT(0);
      return;
  }/* end of swicth */

  if((cmd_ptr = ds_allocate_cmd_buf(DS_CMD_PAYLOAD_SIZE_ZERO)) == NULL)
  {
    DS3GPP2_ASSERT(0);
    return;
  }
  else
  {
    DATA_IS707_MSG1(MSG_LEGACY_HIGH, "Sending CAM cmd - %d", cmd_id);
    cmd_ptr->hdr.cmd_id = cmd_id;
    ds_put_cmd( cmd_ptr );
  }
} /* dscam_timer_cb() */

/*===========================================================================
FUNCTION      DSCAMI_GET_COLOC_INFO

DESCRIPTION   Called by DS in repsonse to GET_COLOC_INFO cmd posted by CAM
              module.  This is the callback registered by CAM module while
              posting the cmd.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
static void dscami_get_coloc_info
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Post cmd to CAM module based on colocation info. 
  -------------------------------------------------------------------------*/
  if(dscam_srlte_device == TRUE)
  {
    dscam_send_cam_cmd(DS_CMD_CAM_SRLTE_ENABLED);
  }
  else if( ds707_pkt_evdo_colocated())
  {
    dscam_send_cam_cmd(DS_CMD_CAM_EVDO_COLOCATED);
  }
  else
  {
    dscam_send_cam_cmd(DS_CMD_CAM_NO_EVDO_COLOCATED);
  }
} /* dscami_get_coloc_info() */

/*===========================================================================
FUNCTION DSCAMI_START_CAM_TIMER

DESCRIPTION    This function will set the timer associated with the CAM
               module to the specified duration.  The time units are in msec.
               This calls the ds3gi_start_timer, generic start_timer function
               used in ds707.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Rex timer started for duration, DS_CAM_ACTIVE_TIME.

===========================================================================*/
static void dscami_start_cam_timer
(
  dword         duration                 /* time interval, in msecs        */
)
{
	  ds_cam_timer_struct ds_cam_timer_report;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Start CAM  activity timer. 
  -------------------------------------------------------------------------*/
  DATA_IS707_MSG1(MSG_LEGACY_HIGH, "Starting CAM timer: %d msec", duration);

  ds3g_timer_start(DS3G_TIMER_CAM, duration);

  ds_cam_timer_report.timer_state = DS_CAM_EVENT_TIMER_START;
  ds_cam_timer_report.timer_value = duration;

  event_report_payload (EVENT_DS_CAM_TIMER,
						sizeof(ds_cam_timer_struct), 
						(void*)&ds_cam_timer_report);
  
} /* dscami_start_cam_timer() */

/*===========================================================================
FUNCTION DSCAMI_CANCEL_CAM_TIMER

DESCRIPTION    This function cancels the timer set by CAM module, typicaly
               if we exit the current state before the timer expired due
               to some other event.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   CAM timer cancelled.

===========================================================================*/
static void dscami_cancel_cam_timer
(
  void
)
{
	  ds_cam_timer_struct ds_cam_timer_report;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Cancel CAM timer.
  -------------------------------------------------------------------------*/
  DATA_IS707_MSG0(MSG_LEGACY_HIGH, "Cancelling CAM timer");

  ds3g_timer_stop( DS3G_TIMER_CAM);

  ds_cam_timer_report.timer_state = DS_CAM_EVENT_TIMER_CANCEL;
  ds_cam_timer_report.timer_value = 0;

  event_report_payload (EVENT_DS_CAM_TIMER,
						sizeof(ds_cam_timer_struct), 
						(void*)&ds_cam_timer_report);

} /* dscami_cancel_cam_timer() */

/*===========================================================================
FUNCTION DSCAM_SEND_CAM_CMD

DESCRIPTION
  This function  posts a cmd to the CAM module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void dscam_send_cam_cmd
(
  ds_cmd_enum_type cam_cmd                    /* Cmd sent to CAM module    */
)
{
  ds_cmd_type            *cmd_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(IS_IN_DS_TASK())
  {
    ds_process_cam_cmd(cam_cmd);
  }
  else
  {
    switch ( cam_cmd )
    {
      case DS_CMD_CAM_SO33_CALL_CONNECTED:
      case DS_CMD_CAM_CALL_GONE_NULL:
      case DS_CMD_CAM_CALL_DORMANT:
      case DS_CMD_CAM_EVDO_COLOCATED:
      case DS_CMD_CAM_SRLTE_ENABLED:
      case DS_CMD_CAM_NO_EVDO_COLOCATED:
      case DS_CMD_CAM_NON_SO33_CALL_CONNECTED:
      case DS_CMD_707_ELEVATOR_EXIT:
      {
        break;
      }
      default:
      {
        DS_IS707_ERROR_LOG_1(MSG_LEGACY_ERROR, 
                        "Invalid cmd %d to send to CAM module", cam_cmd);
        return;
      }
    } /* switch cam_cmd */

    if((cmd_ptr = ds_allocate_cmd_buf(DS_CMD_PAYLOAD_SIZE_ZERO)) == NULL)
    {
      DS3GPP2_ASSERT(0);
      return;
    }
    else
    {
      DATA_IS707_MSG1(MSG_LEGACY_HIGH, "Sending CAM cmd - %d", cam_cmd);
      cmd_ptr->hdr.cmd_id = cam_cmd;
      ds_put_cmd( cmd_ptr );
    }
  } /* switch else */
} /* dscam_send_cam_cmd() */

/*===========================================================================
FUNCTION DSCAMI_START_CAM_FLOW_CTRL_TIMER

DESCRIPTION    This function will set the flow ctrl timer associated with CAM
               module to the specified duration. The time units are in msec.
               This cals the ds3gi_start_timer, generic start_timer function
               used in ds707.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Rex timer started for duration, CAM_FLOW_CTRL_TIME.

===========================================================================*/

static void dscami_start_cam_flow_ctrl_timer
(
  dword         duration                 /* time interval, in msecs        */
)
{
	  ds3g_cam_flow_ctrl_timer_struct ds3g_cam_flow_ctrl_timer_report;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Start CAM timer through DS_CAM_TIMER_SIG
  -------------------------------------------------------------------------*/
  DATA_IS707_MSG1(MSG_LEGACY_HIGH, "Starting CAM timer: %d msec",duration);
  ds3g_timer_start(DS3G_TIMER_CAM_FLOW_CTRL, duration);

  ds3g_cam_flow_ctrl_timer_report.timer_state = 
										DS3G_CAM_FLOW_CTRL_EVENT_TIMER_START;
  ds3g_cam_flow_ctrl_timer_report.timer_value = duration;

  event_report_payload (EVENT_DS3G_CAM_FLOW_CTRL_TIMER,
						sizeof(ds3g_cam_flow_ctrl_timer_struct), 
						(void*)&ds3g_cam_flow_ctrl_timer_report);

} /* dscami_start_cam_flow_ctrl_timer() */

/*===========================================================================
FUNCTION DSCAMI_CANCEL_CAM_FLOW_CTRL_TIMER

DESCRIPTION    This function cancels the timer set by CAM module, typicaly if
               we exit the current state before the timer expired due to some
               other event.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   CAM flow ctrl timer cancelled.

===========================================================================*/
static void dscami_cancel_cam_flow_ctrl_timer
(
  void
)
{
  ds3g_cam_flow_ctrl_timer_struct ds3g_cam_flow_ctrl_timer_report;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Cancel CAM timer started through DS_CAM_FLOW_CTRL_TIMER_SIG
  -------------------------------------------------------------------------*/
  DATA_IS707_MSG0(MSG_LEGACY_HIGH, "Cancelling CAM Flow Ctrl timer");
  ds3g_timer_stop( DS3G_TIMER_CAM_FLOW_CTRL);

  ds3g_cam_flow_ctrl_timer_report.timer_state = 
										DS3G_CAM_FLOW_CTRL_EVENT_TIMER_CANCEL;
  ds3g_cam_flow_ctrl_timer_report.timer_value = 0;

  event_report_payload(EVENT_DS3G_CAM_FLOW_CTRL_TIMER,
					   sizeof(ds3g_cam_flow_ctrl_timer_struct),
					   (void*)&ds3g_cam_flow_ctrl_timer_report);

} /* dscami_cancel_cam_flow_ctrl_timer() */

/*===========================================================================
FUNCTION DSCAMI_FLOW_ENABLE()

DESCRIPTION   Enable Flow from UM sockets and laptop. This is called
              on receiving FLOW_ENABLE cmd from CAM module.
              Note that flow coudl still be disabled due to other flow
              control masks.

DEPENDENCIES  None

PARAMETERS    None

RETURN VALUE  None

SIDE EFFECTS  Flow control locks set by CAM modules earlier are released.
              If no other flow control masks are pending data can orig.
===========================================================================*/
static void dscami_flow_enable
(
  void
)
{
  uint8 curr_iface_index;
  ps_iface_type *curr_iface_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Enable sockets & laptop  data flow
  -------------------------------------------------------------------------*/
  DATA_IS707_MSG0(MSG_LEGACY_HIGH, "CAM flow ctrl OFF");
  if(ds3g_siolib_get_modem_port() != DS3G_SIOLIB_PORT_NONE)
  {
    ds3g_siolib_set_inbound_flow(DS_FLOW_CAM_MASK, DS_FLOW_ENABLE);
  }
  for ( curr_iface_index = 0;
        curr_iface_index < DS707_MAX_IFACES;
        curr_iface_index ++ )
  {   
    curr_iface_ptr =  
        ds707_pdncntx_get_iface_ptr_from_iface_tbl_index(curr_iface_index);
    if ( curr_iface_ptr )
    {
       ps_iface_enable_flow( curr_iface_ptr, DS_FLOW_CTRL_CAM_MASK);
    }

  }                     

} /* dscami_flow_enable() */

/*===========================================================================
FUNCTION DSCAMI_FLOW_DISABLE()

DESCRIPTION   Flow control UM sockets and laptop. This is called on
              receiving FLOW_CTRL cmd from CAM module.

DEPENDENCIES  None

PARAMETERS    None

RETURN VALUE  None

SIDE EFFECTS  UM sockets & RM folw controlled until explicitly enabled again
===========================================================================*/
static void dscami_flow_disable
(
  void
)
{
  uint8 curr_iface_index;
  ps_iface_type *curr_iface_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Stop sockets & laptop  data flow
  -------------------------------------------------------------------------*/
  DATA_IS707_MSG0(MSG_LEGACY_HIGH, "CAM flow ctrl ON");
  if(ds3g_siolib_get_modem_port() != DS3G_SIOLIB_PORT_NONE)
  {
    ds3g_siolib_set_inbound_flow(DS_FLOW_CAM_MASK, DS_FLOW_DISABLE);
  }
  for ( curr_iface_index = 0;
        curr_iface_index < DS707_MAX_IFACES;
        curr_iface_index ++ )
  {   
    curr_iface_ptr =  
        ds707_pdncntx_get_iface_ptr_from_iface_tbl_index(curr_iface_index);
    if ( curr_iface_ptr )
    {
       ps_iface_disable_flow( curr_iface_ptr, DS_FLOW_CTRL_CAM_MASK);
    }
  }                        
} /* dscami_flow_disable() */

/*===========================================================================
FUNCTION DS_PROCESS_CAM_CMD

DESCRIPTION
  This module process CAM cmds received by the DS task (possibly by different
  task if CAM module moves to a different task in future); These cmds are for
  CAM module to kick-in and force dormancy on 1X to move to EVDO, if needed.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_process_cam_cmd
(
  ds_cmd_enum_type cam_cmd
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-------------------------------------------------------------------------
    Switch on the cmd name and process the command.
  -------------------------------------------------------------------------*/
  DATA_IS707_MSG2(MSG_LEGACY_MED, "Rcvd cmd - %d in %d state",
                  cam_cmd, dscam.cam_state);

  switch( cam_cmd)
  {
    /*-----------------------------------------------------------------------
      Call connect on 1X with SO 33.
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_SO33_CALL_CONNECTED:
    {
      dscami_process_cam_so33_call_connect();
      break;
    }
    /*-----------------------------------------------------------------------
      CAM timer expired while in dormant or active states.
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_TIMER_EXPIRED:
    {
      dscami_process_cam_timer_expiry();
      break;
    }
    /*-----------------------------------------------------------------------
      CAM Flow ctrl timer expired - call may go on EVDO now.
      Go back to Idle state
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_FLOW_CTRL_TIMER_EXPIRED:
    {
      dscami_process_flow_ctrl_timer_expiry();
      break;
    }
    /*-----------------------------------------------------------------------
      Current 1X call terminated  -  call NULL , not dormant.
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_CALL_GONE_NULL:
    {
      dscami_process_cam_call_gone_null();
      break;
    }
    /*-----------------------------------------------------------------------
      Current 1X call went dormant
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_CALL_DORMANT:
    {
      dscami_process_cam_call_dormant();
      break;
    }

	/*-----------------------------------------------------------------------
      SRLTE is enabled.
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_SRLTE_ENABLED:
    {
	  //colocated DO and LTE
      dscami_process_cam_evdo_coloc_cmd();
      break;
    }

    /*-----------------------------------------------------------------------
      There is Evdo colocated with the current 1X System.
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_EVDO_COLOCATED:
    {
      dscami_process_cam_evdo_coloc_cmd();
      break;
    }

    /*-----------------------------------------------------------------------
      There is NO Evdo colocated with the current 1X System.
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_NO_EVDO_COLOCATED:
    {
      dscami_process_cam_no_evdo_coloc_cmd();
      break;
    }
    /*-----------------------------------------------------------------------
      Current 1X call terminated and some other call connected  - as far as
      the current 1X SO33 call is concerned we haev gone NULL.
    -----------------------------------------------------------------------*/
    case DS_CMD_CAM_NON_SO33_CALL_CONNECTED:
    {
      dscami_process_cam_call_gone_null();
      break;
    }

    case DS_CMD_707_ELEVATOR_EXIT:
    {
      dscami_process_elevator_exit_ind();
      break;
    }
    default:
    {
      return;
    }
  } /* switch cam_cmd */
} /* ds_process_cam_cmd() */

/*===========================================================================
FUNCTION DSCAMI_PROCESS_CAM_SO33_CALL_CONNECT

DESCRIPTION
  This module process call_connect CAM cmd received by the DS task (possibly
  by different task if CAM module moves to a different task in future).
  If receied in correct state, CAMmodule moves to CAM_CALL_ACTIVE state.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void dscami_process_cam_so33_call_connect
( 
  void
)
{
  sys_srv_opt_type so_list[1] = {CAI_SO_PPP_PKT_DATA_3G};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DATA_IS707_MSG1(MSG_LEGACY_MED, "Rcvd SO33 CALL_CONNECT - in CAM state: %d", 
                  dscam.cam_state);

  switch( dscam.cam_state )
  {
    case DS_CAM_IDLE_STATE:
    {
      /*---------------------------------------------------------------------
        start 5 mnt CAM tiemr and transition to ACTIVE state.
      ---------------------------------------------------------------------*/
      dscami_start_cam_timer(dscam_timer_new_val);
      dscam_timer_new_val = backoff_timer_val[next_backoff_index++]*1000;
      if (next_backoff_index == DS_MAX_NO_BACKOFF_TIMER)
      {
        next_backoff_index =  DS_MAX_NO_BACKOFF_TIMER -1;   
      }
      dscami_cam_transition_state(DS_CAM_CALL_ACTIVE_STATE);
      break;
    }
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    {
      /*---------------------------------------------------------------------
        start 5 sec flow ctrl timer to force dormancy long enough for EVDO to
        get locks from 1X. followed by this next call may come up on EVDO.
      ---------------------------------------------------------------------*/
      dscami_cancel_cam_flow_ctrl_timer();
      dscami_flow_enable();
      if (dscam_srlte_device == TRUE)
      {
        DATA_IS707_MSG0(MSG_LEGACY_MED,  "Listening to page SO33");
#ifdef FEATURE_DATA_MPSS_AT20_DEV
        cm_ph_cmd_block_srv_opt(NULL,
                                NULL,
                                ds3g_get_cm_client_id(),
                                0,
                                so_list); 
#else
        cm_ph_cmd_block_srv_opt1(NULL,
                                 NULL,
                                 ds3g_get_cm_client_id(),
                                 0,
                                 so_list,
                                 ds707_curr_subs_id()); 
#endif /* FEATURE_DATA_MPSS_AT20_DEV */
      }
      elevator_exit_ind = FALSE;
      dscami_start_cam_timer(dscam_timer_new_val);
      dscam_timer_new_val = backoff_timer_val[next_backoff_index++]*1000;
      if (next_backoff_index == DS_MAX_NO_BACKOFF_TIMER)
      {
        next_backoff_index =  DS_MAX_NO_BACKOFF_TIMER -1;   
      }
      dscami_cam_transition_state(DS_CAM_CALL_ACTIVE_STATE);
      break;
    }
    case DS_CAM_CALL_ACTIVE_STATE:
    {
      /*---------------------------------------------------------------------
        Nothing to do; Remain in ACTIVE STATE 
      ---------------------------------------------------------------------*/
      break;
    }
    case DS_CAM_CALL_DORMANT_STATE:
    {
      /*---------------------------------------------------------------------
        Transition back to  ACTIVE STATE 
      ---------------------------------------------------------------------*/
      dscami_cam_transition_state(DS_CAM_CALL_ACTIVE_STATE);
      break;
    }
    case DS_CAM_WAITING_COLOC_INFO_STATE:
    {
      /*---------------------------------------------------------------------
        Ideally we should nto get call_connect in this state - but under some
        race conditions this cld happen - Set the ds_cam_callstate to reflect
        the callstate accurately so that correct transition happens based on
        if the call is ACTIVE or DORMANT on the way out of this state.
      ---------------------------------------------------------------------*/
      dscam.cam_callstate = DS_CAM_CALL_ACTIVE;
      break;
    }
    case DS_CAM_WAITING_DORMANCY_STATE:
    {
      DS3GPP2_ASSERT(0);
      return;
    }
    default:
    {
      /* Invalid state */
      return;
    }
  } /* switch cam_state */
} /* dscami_process_cam_so33_call_connect() */

/*===========================================================================
FUNCTION DSCAMI_PROCESS_CAM_TIMER_EXPIRY

DESCRIPTION
  This module process CAM timer expired event received by the DS task
  (possibly different task if CAM module moves to a different task in future)
  If recevied in correct state, CAM module moves to CAM_CALL_WAITING_DORMANY
  or back to CAM_CALL_ACTIVE state if there is no colocated EVDO system to 
  move to.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dscami_process_cam_timer_expiry
(
  void
)
{
 ds_cam_timer_struct ds_cam_timer_report;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DATA_IS707_MSG1(MSG_LEGACY_MED, "Rcvd CAM TIMER EXPIRED cmd - in %d state",
                  dscam.cam_state);

  ds_cam_timer_report.timer_state = DS_CAM_EVENT_TIMER_EXPIRY;
  ds_cam_timer_report.timer_value = 0;

  event_report_payload (EVENT_DS_CAM_TIMER,
						sizeof(ds_cam_timer_struct), 
						(void*)&ds_cam_timer_report);

  switch( dscam.cam_state )
  {
    case DS_CAM_IDLE_STATE:
    {
      /*---------------------------------------------------------------------
        This happends only under soem race conditions where, by the time we
        transition to IDLE state, timers were not yet cleaned completely
        - No harm, ignore the event.
      ---------------------------------------------------------------------*/
      /* Ignore */
      break;
    }
    case DS_CAM_CALL_ACTIVE_STATE:
    case DS_CAM_CALL_DORMANT_STATE:
    {

      if( (dscam_srlte_device == TRUE) && (!elevator_exit_ind) &&
          (cm_get_is_ready_to_scan_more_pref_data(ds707_curr_subs_id(), SYS_SYS_MODE_LTE) != TRUE) )
      {
        DATA_IS707_MSG1(MSG_LEGACY_MED, "Starting Extension timer - %d ms",
                        dscam_extension_timer_val);
        dscami_start_cam_timer(dscam_extension_timer_val);
      }
      else
      {
        /*---------------------------------------------------------------------
         CAM timer expired - time to swicth to Evdo if available. Query CM 
         to see if we are cuurently colocated with Evdo.
         Colocated means => DO and/OR SRLTE are colocated
        ---------------------------------------------------------------------*/
        dscami_cam_transition_state(DS_CAM_WAITING_COLOC_INFO_STATE);

        dscami_get_coloc_info();
      }
      break;
    }
    case DS_CAM_WAITING_DORMANCY_STATE:
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    case DS_CAM_WAITING_COLOC_INFO_STATE:
    {
      DS3GPP2_ASSERT(0);
      return;
    }
    default:
    {
      /* Invalid cam_state */
      return;
    }
  } /* switch cam_state */
}/* dscami_process_cam_timer_expiry() */

/*===========================================================================
FUNCTION DSCAMI_PROCESS_CAM_CALL_GONE_NULL

DESCRIPTION
  This module process CAM timer expired event received by the DS task
  (possibly different task if CAM module moves to a different task in future)
  If recevied in correct state, CAM module moves to CAM_CALL_WAITING_DORMANY
  or back to CAM_CALL_ACTIVE state if there is no colocated EVDO system to
  move to.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL void dscami_process_cam_call_gone_null
(
  void
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DATA_IS707_MSG1(MSG_LEGACY_MED, "Rcvd CALL_NULL cmd - in %d state", 
                  dscam.cam_state);

  switch( dscam.cam_state )
  {
    case DS_CAM_IDLE_STATE:
    {
      /* Ignoring call_gone_null event in CAM IDLE state */
      break;
    }
    /*-----------------------------------------------------------------------
      In all states on this event, transition to IDLE state and cancel all
      timers and restore CTA timeout.
    -----------------------------------------------------------------------*/
    case DS_CAM_WAITING_DORMANCY_STATE:
      ds707_pkt_restore_cam_cta_timeout();
      //lint -fallthrough
    case DS_CAM_CALL_ACTIVE_STATE:
    case DS_CAM_CALL_DORMANT_STATE:
    case DS_CAM_WAITING_COLOC_INFO_STATE:
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    {
      next_backoff_index = 0;
      DATA_IS707_MSG1(MSG_LEGACY_MED, "Reset the 1X backoff index %d", 
                      next_backoff_index);
      dscami_cam_transition_state(DS_CAM_IDLE_STATE);
      break;
    }
    default:
    {
      /* Invalid cam_state */
      return;
    }
  } /* switch cam_state */
}/* dscami_process_cam_call_gone_null()*/

/*===========================================================================
FUNCTION DSCAMI_PROCESS_CAM_CALL_DORMANT

DESCRIPTION
  This module processes call_dormant CAM cmd received by the DS task
  (possibly tby different ask if CAM module moves to a different task later).
  If receied in correct state, CAMmodule moves to CAM_CALL_FLOW_CTRL_ACTIVE
  state.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dscami_process_cam_call_dormant
(
  void
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DATA_IS707_MSG1(MSG_LEGACY_MED, 
                  "Rcvd CALL DORMANT dormant cmd - in %d state", 
                  dscam.cam_state);

  switch( dscam.cam_state )
  {
    case DS_CAM_IDLE_STATE:
    {
      /* Ignoring call_dormant event in CAM IDLE state */
      break;
    }
    case DS_CAM_CALL_ACTIVE_STATE:
    {
      /*---------------------------------------------------------------------
        Transition to DORMANT state. We ping-pong between ACTIVE & DORMANT
        states until the CAM timer expires or call goes NULL.
      ---------------------------------------------------------------------*/
      dscami_cam_transition_state(DS_CAM_CALL_DORMANT_STATE);
      break;
    }
    case DS_CAM_WAITING_DORMANCY_STATE:
    {
      /*---------------------------------------------------------------------
        Call down, check for evdo colocation again, before we flow ctrl for
        additional 5 secs. if no Evdo, continue to be 1X ( dormant ).
      ---------------------------------------------------------------------*/
      ds707_pkt_restore_cam_cta_timeout();
      dscam.cam_callstate = DS_CAM_CALL_DORMANT;
      dscami_cam_transition_state(DS_CAM_WAITING_COLOC_INFO_STATE);
      dscami_get_coloc_info();
      break;
    }
    case DS_CAM_WAITING_COLOC_INFO_STATE:
    {
      /*---------------------------------------------------------------------
        Coloc info from CM says Evdo available, flow ctrl for additional 5sec
        to give enough time for Evdo to acquire the locks.
      ---------------------------------------------------------------------*/
      dscam.cam_callstate = DS_CAM_CALL_DORMANT;
      break;
    }
    case DS_CAM_CALL_DORMANT_STATE:
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    {
      /*---------------------------------------------------------------------
        Already dormant - ignore the event.
      ---------------------------------------------------------------------*/
      break;
    }
    default:
    {
      /* Invalid cam_state */
      return;
    }
  } /* switch cam_state */
}/* dscami_process_cam_call_dormant() */

/*===========================================================================
FUNCTION DSCAMI_PROCESS_CAM_COLOC_CMD

DESCRIPTION
  This module processes coloc cmd from CM and mves to WAITING_FOR_DORMANCY
  state, followed by which the call will try to come up EVDO, since now we
  have a colocated EVDO to move to.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

LOCAL void dscami_process_cam_evdo_coloc_cmd
(
  void
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DATA_IS707_MSG1(MSG_LEGACY_MED, "Rcvd COLOC INFO cmd - in %d state",
                  dscam.cam_state);

  switch( dscam.cam_state )
  {
    /*-----------------------------------------------------------------------
      EVDO colocated - move to WAITING_FOR_DORMANCY  to move call to EVDO/LTE
      followed by dormancy.
    -----------------------------------------------------------------------*/
    case DS_CAM_WAITING_COLOC_INFO_STATE: 
    {
      if(dscam.cam_callstate == DS_CAM_CALL_ACTIVE)
      {
        dscami_cam_transition_state(DS_CAM_WAITING_DORMANCY_STATE);
      }
      else if(dscam.cam_callstate == DS_CAM_CALL_DORMANT)
      {
        dscami_cam_transition_state(DS_CAM_FLOW_CTRL_ACTIVE_STATE);
      }
      else
      {
        DATA_IS707_MSG2(MSG_LEGACY_HIGH, "Wrong callstate %d in %d state",
                        dscam.cam_callstate, dscam.cam_state);
        dscami_cam_transition_state( DS_CAM_IDLE_STATE );
      }
      break;
    }
    /*-----------------------------------------------------------------------
      We could recieve colocation info in any of the following states too 
      under some race conditions. - Ignore the event.
    -----------------------------------------------------------------------*/
    case DS_CAM_IDLE_STATE:
    case DS_CAM_CALL_ACTIVE_STATE:
    case DS_CAM_CALL_DORMANT_STATE:
    case DS_CAM_WAITING_DORMANCY_STATE:
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    {
      /* Ignore */
      break;
    }
    default:
    {
      /* Invalid cam_state */
      return;
    }
  } /* switch cam_state */
} /* dscami_process_cam_evdo_coloc_cmd() */

/*===========================================================================
FUNCTION DSCAMI_PROCESS_CAM_NO_EVDO_COLOC_CMD

DESCRIPTION
  This module processes coloc cmd from CM and mves to WAITING_FOR_DORMANCY
  state, followed by which the call will try to come up EVDO, since now we
  have a colocated EVDO to move to.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void dscami_process_cam_no_evdo_coloc_cmd
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DATA_IS707_MSG1(MSG_LEGACY_MED, 
                  "Rcvd COLOC INFO - No Colocation in %d state", 
                  dscam.cam_state);

  switch( dscam.cam_state )
  {
    /*-----------------------------------------------------------------------
      No EVDO colocated - Go back to call_active state and restart the CAM
      timer  
    -----------------------------------------------------------------------*/
    case DS_CAM_WAITING_COLOC_INFO_STATE:
    {
      if(dscam.cam_callstate == DS_CAM_CALL_ACTIVE)
      {
        dscami_start_cam_timer(dscam_timer_new_val);
        dscam_timer_new_val = backoff_timer_val[next_backoff_index++]*1000;
        if (next_backoff_index == DS_MAX_NO_BACKOFF_TIMER)
        {
          next_backoff_index =  DS_MAX_NO_BACKOFF_TIMER -1;   
        }
        dscami_cam_transition_state(DS_CAM_CALL_ACTIVE_STATE);
      }
      else if(dscam.cam_callstate == DS_CAM_CALL_DORMANT)
      {
        dscami_start_cam_timer(dscam_timer_new_val);
        dscam_timer_new_val = backoff_timer_val[next_backoff_index++]*1000;
        if (next_backoff_index == DS_MAX_NO_BACKOFF_TIMER)
        {
          next_backoff_index =  DS_MAX_NO_BACKOFF_TIMER -1;   
        }
        dscami_cam_transition_state(DS_CAM_CALL_DORMANT_STATE);
      }
      else
      {
        DATA_IS707_MSG2(MSG_LEGACY_HIGH, "Wrong callstate %d in %d state",
                        dscam.cam_callstate, dscam.cam_state);
        dscami_cam_transition_state( DS_CAM_IDLE_STATE );
      }
      break;
    }
    /*-----------------------------------------------------------------------
      We could recieve colocation info in any of the following states too 
      under some race conditions. - Ignore the event.
    -----------------------------------------------------------------------*/
    case DS_CAM_IDLE_STATE:
    case DS_CAM_CALL_ACTIVE_STATE:
    case DS_CAM_CALL_DORMANT_STATE:
    case DS_CAM_WAITING_DORMANCY_STATE:
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    {
      /* Ignoring */
      break;
    }
    default:
    {
      /* Invalid cam_state */
      return;
    }
  } /* switch cam_state */
} /* dscami_process_cam_no_evdo_coloc_cmd() */

/*===========================================================================
FUNCTION DSCAMI_PROCESS_FLOW_CTRL_TIMER_EXPIRY

DESCRIPTION
  This module process Dormant timer expired event.
  We start this timer for 5 sec ( effective followed by call going
  dormant to force additional flow contorl for 5 secs) to give enough
  time for 1X to release the locks.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dscami_process_flow_ctrl_timer_expiry
(
  void
)
{
  ds3g_cam_flow_ctrl_timer_struct ds3g_cam_flow_ctrl_timer_report;
  sys_srv_opt_type so_list[1] = {CAI_SO_PPP_PKT_DATA_3G};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds3g_cam_flow_ctrl_timer_report.timer_state = 
										DS3G_CAM_FLOW_CTRL_EVENT_TIMER_EXPIRY;
  ds3g_cam_flow_ctrl_timer_report.timer_value = 0;

  DATA_IS707_MSG1(MSG_LEGACY_MED, 
                  "Rcvd FLOW CONTROL TIMER EXPIIRY cmd - in %d state",
                  dscam.cam_state);

  event_report_payload(EVENT_DS3G_CAM_FLOW_CTRL_TIMER,
					   sizeof(ds3g_cam_flow_ctrl_timer_struct),
					   (void*)&ds3g_cam_flow_ctrl_timer_report);

  switch( dscam.cam_state )
  {
    /*-----------------------------------------------------------------------
      Under some race conditions the flwo_ctrl timer cld expire in any of the
      following states too. - Ignore the event.
    -----------------------------------------------------------------------*/
    case DS_CAM_IDLE_STATE:
    case DS_CAM_CALL_ACTIVE_STATE:
    case DS_CAM_CALL_DORMANT_STATE:
    case DS_CAM_WAITING_DORMANCY_STATE:
    case DS_CAM_WAITING_COLOC_INFO_STATE:
    {
      /* Ignoring flowctrl timer expiry */
      break;
    }
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    {
      dscami_flow_enable();
      if (dscam_srlte_device == TRUE)
      {
        DATA_IS707_MSG0(MSG_LEGACY_MED,  "Listening to page SO33");
#ifdef FEATURE_DATA_MPSS_AT20_DEV
        cm_ph_cmd_block_srv_opt(NULL,
                                NULL,
                                ds3g_get_cm_client_id(),
                                0,
                                so_list);
#else
        cm_ph_cmd_block_srv_opt1(NULL,
                                 NULL,
                                 ds3g_get_cm_client_id(),
                                 0,
                                 so_list,
                                 ds707_curr_subs_id()); 
#endif /* FEATURE_DATA_MPSS_AT20_DEV */
      }
      dscami_cam_transition_state( DS_CAM_IDLE_STATE );
      elevator_exit_ind = FALSE;
      return;
    }
    default:
    {
      /* Invalid cam_state */
      return;
    }
  } /* switch cam_state */
}/* dscami_process_flow_ctrl_timer_expiry() */

/*===========================================================================
  FUNCTION DSCAMI_CAM_TRANSITION_STATE()

  DESCRIPTION
    This function effects the transition to the CAM state that is passed in.

  PARAMETERS
    new_state: the state to transition to

  RETURN VALUE
    None

  DEPENDENCIES
    Its the caller's responsibility that it initiates only valid state
    transitions. This function does not check if a state transition is
    valid or not.

  SIDE EFFECTS
    None
===========================================================================*/
static void dscami_cam_transition_state
(
  dscam_cam_state_enum_type new_state
)
{
  sys_srv_opt_type so_list[1] = {CAI_SO_PPP_PKT_DATA_3G};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  nv_mode_enum_type curr_pref_mode;
  DATA_IS707_MSG2(MSG_LEGACY_MED,  "CAM goes from %d state to %d state", 
                  dscam.cam_state,new_state);

  dscam.cam_state = new_state;

  /*-------------------------------------------------------------------------
    Perform action associated with entering into new state.
  -------------------------------------------------------------------------*/
  switch( dscam.cam_state )
  {
    case DS_CAM_IDLE_STATE:
    {
      /*---------------------------------------------------------------------
        Set state variables and  wait in this state until SO33_CALL_CONNECTED 
        received.
      ---------------------------------------------------------------------*/
      dscami_cancel_cam_timer();
      dscami_cancel_cam_flow_ctrl_timer();
      dscami_flow_enable();
      if (dscam_srlte_device == TRUE)
      {
        DATA_IS707_MSG0(MSG_LEGACY_MED,  "Listening to page SO33");

#ifdef FEATURE_DATA_MPSS_AT20_DEV
       cm_ph_cmd_block_srv_opt(NULL,
                                NULL,
                                ds3g_get_cm_client_id(),
                                0,
                                so_list); 
#else
        cm_ph_cmd_block_srv_opt1(NULL,
                                 NULL,
                                 ds3g_get_cm_client_id(),
                                 0,
                                 so_list,
                                 ds707_curr_subs_id()); 
#endif /* FEATURE_DATA_MPSS_AT20_DEV */
      }
      elevator_exit_ind = FALSE;
      dscam.cam_callstate = DS_CAM_CALL_IDLE;
      dscam_timer_new_val = backoff_timer_val[next_backoff_index]*1000;
      break;
    }
    case DS_CAM_CALL_ACTIVE_STATE:
    {
      /*---------------------------------------------------------------------
        callstate = ACTIVE . on transition to this state we start CAM timer.
      ---------------------------------------------------------------------*/
      dscam.cam_callstate = DS_CAM_CALL_ACTIVE;
      break;
    }
    case DS_CAM_CALL_DORMANT_STATE:
    {
      dscam.cam_callstate = DS_CAM_CALL_DORMANT;
      break;
    }
    case DS_CAM_WAITING_COLOC_INFO_STATE:
    {
      /*---------------------------------------------------------------------
        Nothing to do : Wait in this state until  we get coloc from CM 
      ---------------------------------------------------------------------*/
      break;
    }
    case DS_CAM_WAITING_DORMANCY_STATE:
    {
      if ((dscam_srlte_device == TRUE) && (dscam_read_policyman_ueMode () !=  SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED)) //shrawan5
      {
        DATA_IS707_MSG1(MSG_LEGACY_HIGH, "DS CAM change CTA time out to %d msec",
                        dscam_idle_period_val);
        if(elevator_exit_ind)
        {
          ds707_pkt_set_cam_cta_timeout( DSCAM_IDLE_PERIOD_FOR_ELEVATOR_EXIT );
        }
        else
        {
          ds707_pkt_set_cam_cta_timeout( dscam_idle_period_val );
        }
      }
      else
      {
      curr_pref_mode = ds707_get_curr_mode_pref();

      if(curr_pref_mode != NV_MODE_CDMA_ONLY && curr_pref_mode !=NV_MODE_HDR_ONLY)
      {
        DATA_IS707_MSG1(MSG_LEGACY_HIGH, "DS CAM change CTA time out to %d sec",
                        dscam_idle_period_val);
        if(elevator_exit_ind)
        {
          ds707_pkt_set_cam_cta_timeout( DSCAM_IDLE_PERIOD_FOR_ELEVATOR_EXIT );
        }
        else
        {
          ds707_pkt_set_cam_cta_timeout( dscam_idle_period_val );
        }
      }
      else
      {
      /*Transition the state to Active */
       /*---------------------------------------------------------------------
        start 5 mnt CAM timer and transition to ACTIVE state.
       ---------------------------------------------------------------------*/
       DATA_IS707_MSG0(MSG_LEGACY_HIGH, " HDR ONLY / CDMA ONLY mode : "
                       "DS CAM go back to Active state");
        dscami_start_cam_timer(dscam_timer_new_val);
        dscam_timer_new_val = backoff_timer_val[next_backoff_index++]*1000;
        if (next_backoff_index == DS_MAX_NO_BACKOFF_TIMER)
        {
          next_backoff_index =  DS_MAX_NO_BACKOFF_TIMER -1;   
        }
       dscami_cam_transition_state(DS_CAM_CALL_ACTIVE_STATE);
      }      
      }
      break;
    }
    case DS_CAM_FLOW_CTRL_ACTIVE_STATE:
    {
      /*---------------------------------------------------------------------
        start the flow ctrl timer and  disable UM & RM  flow 
      ---------------------------------------------------------------------*/
      dscam.cam_callstate = DS_CAM_CALL_DORMANT;
      if (dscam_srlte_device == TRUE)
      {
        DATA_IS707_MSG0(MSG_LEGACY_MED,  "Blocking page SO33");

#ifdef FEATURE_DATA_MPSS_AT20_DEV
        cm_ph_cmd_block_srv_opt(NULL,
                                NULL,
                                ds3g_get_cm_client_id(),
                                1,
                                so_list);
#else
        cm_ph_cmd_block_srv_opt1(NULL,
                                 NULL,
                                 ds3g_get_cm_client_id(),
                                 1,
                                 so_list,
                                 ds707_curr_subs_id()); 
#endif /* FEATURE_DATA_MPSS_AT20_DEV */
      }
      dscami_flow_disable();
      dscami_start_cam_flow_ctrl_timer(new_flow_control_timer_val);
      break;
    }
    default:
    {
      /* Invalid cam_state */
      return;
    }
  } /* switch cam_state */
} /* dscami_cam_transition_state() */

/*===========================================================================
FUNCTION      DSCAM_GET_CAM_TIMER_VALUE

DESCRIPTION   This function returns CAM timer value currently being used in 
              seconds. Caller of this function should convert it to 
              milliseconds or any other qualifier if required.

DEPENDENCIES  None.

RETURN VALUE  
  uint32 - CAM timer value in seconds

SIDE EFFECTS  None.
===========================================================================*/
uint32 dscam_get_cam_timer_value
( 
  void 
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* dscam_timer_val is in milliseconds and so convert this value to seconds
     before returning the timer value to the caller. */

  DATA_IS707_MSG1(MSG_LEGACY_MED, 
                  "CAM timer value in ms is %d", dscam_timer_val);

  return (uint32) (dscam_timer_val/1000);
} /* dscam_get_cam_timer_value() */


/*===========================================================================
FUNCTION      DSCAM_SET_CAM_TIMER_VALUE

DESCRIPTION   This function stores the CAM timer value in an intermediate 
              variable if CAM is not in IDLE state. This intermediate value 
              will be copied to "dscam_timer_val" when CAM transitions to 
              IDLE state. If CAM is not in IDLE state when this function is 
              called, the new timer value will be copied to "dscam_timer_val"
              immediately.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void dscam_set_cam_timer_value
( 
  uint32 new_timer_val
)
{
  int i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( dscam.cam_state == DS_CAM_IDLE_STATE)
  {
    dscam_timer_val = (dword) (new_timer_val * 1000);
    DATA_IS707_MSG1(MSG_LEGACY_MED, 
                    "CAM is in IDLE state. New CAM timer value %d (in ms) set.",
                    dscam_timer_val);
  }
  else
  {
    dscam_timer_new_val = (dword) (new_timer_val * 1000);
    DATA_IS707_MSG2(MSG_LEGACY_MED, 
                    "CAM is in %d state. New CAM timer value %d (in ms) "
                    "will take effect for next call.", 
                    dscam.cam_state, dscam_timer_new_val);
  }
  /*TODO: Not fixed if the timer is configured when call state is active.*/
  for (i = 0; i < DS_MAX_NO_BACKOFF_TIMER; i++)
  {
    backoff_timer_val[i] = new_timer_val;
  }
  return;
} /* dscam_set_cam_timer_value() */

/*===========================================================================
  FUNCTION DSCAMI_ELEVATOR_EXIT_IND_MSG_HDLR()

  DESCRIPTION
    This function executes when elevator exit comes from qmi nas msgr.

  PARAMETERS
   
   msgrtype  - UMID of the message sent
   dsmsg_ptr - Pointer to the payload

  RETURN VALUE
    None

  DEPENDENCIES

  SIDE EFFECTS
    None
===========================================================================*/

void dscami_elevator_exit_ind_msg_hdlr
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
)
{
  if(msgrtype == QMI_NAS_PRIORITIZE_LTE_IND &&
     subs_id == ds707_curr_subs_id() && (dscam.cam_state == DS_CAM_CALL_ACTIVE_STATE))
  {
    dscam_send_cam_cmd(DS_CMD_707_ELEVATOR_EXIT);
  }
  else
  {
    DATA_IS707_MSG2(MSG_LEGACY_HIGH, " Ignore elvator ind for subs %d CDMA sub %d",
	                subs_id, ds707_curr_subs_id());
  }
  
}

/*===========================================================================
  FUNCTION DSCAMI_PROCESS_ELEVATOR_EXIT_IND()

  DESCRIPTION
    This function process elevator exit ind .

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
  None

  SIDE EFFECTS
  None
===========================================================================*/

void dscami_process_elevator_exit_ind()
{
  if(!elevator_exit_ind)
  {
    DATA_IS707_MSG0(MSG_LEGACY_HIGH, "dscami_process_elevator_exit_ind received:");
    elevator_exit_ind = TRUE;
    dscami_cancel_cam_timer();
    dscami_process_cam_timer_expiry();
  }
  return;
}

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */
#endif /* FEATURE_DS_CAM */
#endif /* FEATURE_HDR */

