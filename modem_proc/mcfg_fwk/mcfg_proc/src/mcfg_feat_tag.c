/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    MCFG FeatureTag related functions

GENERAL DESCRIPTION

  This file encapsulates the config feature functionality 
  used to process subsections of a configuration based on enabled features
  
EXTERNALIZED FUNCTIONS
  
INITIALIZATION AND SEQUENCING REQUIREMENTS
 
Copyright (c) 2016-2017 by QUALCOMM Technologies Incorporated.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

    EDIT HISTORY FOR MODULE/

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_proc/src/mcfg_feat_tag.c#1 $ $DateTime: 2021/04/08 00:04:19 $ $Author: pwbldsvc $ 
 
when       who     what, where, why
--------   ---    ----------------------------------------------------------
10/5/2016   as     Initial Creation.

===========================================================================*/


/*===========================================================================

     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "list.h"                           /* List processing header file */

#include "mcfg_feature_config.h"
#include "mcfg_feat_tag.h"
#include "mcfg_tlv.h"
#include "mcfg_proc.h" /* validate_mcfg_header */
#include "mcfg_load.h" /* mcfg_load_seg */
#include "mcfg_common.h" /* mcfg err codes used */
#include "mcfg_osal.h" /* mcfg_malloc used */
#include "mcfg_int.h" /* mcfg error messages */
#include "mcfg_setting.h"/* keep */
#include "mcfg_refresh.h"
#include "mcfg_uim.h"
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_MCFG_FEATURETAG

/*===========================================================================

    GLOBAL DEFINITIONS AND DECLARATIONS

This section contains global definitions for constants, macros, types,
variables

===========================================================================*/

/*==============================================================================
  Constants and Macros
==============================================================================*/

/*------------------------------------------------------------------------------
  TLV IDs
------------------------------------------------------------------------------*/

#define MCFG_FEAT_TLV_ID_FEATURE_MASK      (0x01)
#define MCFG_FEAT_TLV_ID_FEATURE_CONDITION (0x02)
#define MCFG_FEAT_TLV_ID_COND_EXPRESSION   (0x03)

/*===========================================================================

    LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

// TODO: impelement stack based on list type if time allows
// typedef list_link_type mcfg_feat_list_node_list_type;

typedef struct mcfg_feat_list_node
{
  list_link_type        link;
  boolean               was_off; // Tracks whether the previous if scope was inactive
  boolean               is_off; // Tracks whether the current clause of the current scope is inactive
  boolean               true_found; // Tracks when a true condition has been found while evaluating a conditional
  mcfg_cond_type_e      conditional_type;
  // struct mcfg_feat_list_node  *prev_node;
} mcfg_feat_list_node;

typedef list_type mcfg_feat_list_type;

typedef PACKED struct
{
  mcfg_supported_features_e  feature;
  mcfg_cond_type_e           conditional_type;
  mcfg_expr_type_s           expression;  // leave as an integer for now until tokenizer is put in place for strings
} mcfg_feat_s_type;

/*===========================================================================
                    GLOBAL VARIABLES 
============================================================================*/
static boolean mcfg_processing_status = PROCESSING_ON;
static mcfg_feat_list_type mcfg_feat_list;

static boolean mcfg_feat_list_purge(void);

/*===========================================================================

                                  FUNCTIONS 

===========================================================================*/
/*===========================================================================
                    PRIVATE FUNCTION DECLARATIONS FOR MODULE
============================================================================*/
/*===========================================================================

FUNCTION  mcfg_print_node() 

DESCRIPTION
  Prints contents of a stack node

DEPENDENCIES

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
void mcfg_print_node
(
  mcfg_feat_list_node *node
)
{
  MCFG_CHECK_NULL_PTR_RET(node);

  MCFG_MSG_LOW("== Node settings ==");
  //MCFG_MSG_LOW_1("node.link %p", node->link);
  MCFG_MSG_LOW_1("node.was_off %d", node->was_off);
  MCFG_MSG_LOW_1("node.is_off %d", node->is_off);
  MCFG_MSG_LOW_1("node.true_found %d", node->true_found);
  MCFG_MSG_LOW_1("node.conditional_type %d", node->conditional_type);
  
  return;
}
/*===========================================================================

FUNCTION  mcfg_cmp_setting_to_expr() 

DESCRIPTION
  Compares a setting in mcfg_setting file to the expression given for a featureTag
  TODO: change to a macro if comparison remains simple

DEPENDENCIES

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
    FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_cmp_setting_to_expr
(
   uint16 setting, 
   mcfg_expr_type_s expression
)
{
  boolean result = FALSE; 

/* -------------------------------------------------------------------- */
  MCFG_MSG_LOW_3("compare setting to expr: comparator - %d, target_value - %d, comparison_value - %d", expression.comparison, setting, expression.value);

  switch (expression.comparison)
  {
    case MCFG_EQUAL:
      result = ((setting) == (expression.value));
      break;

    case MCFG_NOT_EQUAL:
      result = ((setting) != (expression.value));
      break;

    default: 
      MCFG_MSG_ERROR_1("Feature: expression comparison not supported: %d", expression.comparison);
  }
  
  return result;
}

/*===========================================================================

FUNCTION  mcfg_is_expr_true() 

DESCRIPTION
  Evaluates the condition associated with a feature tag to be either
  TRUE or False

DEPENDENCIES

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
    FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_is_expr_true
(
   mcfg_slot_index_type_e_type sloti,
   mcfg_supported_features_e featureTag,
   mcfg_expr_type_s expression,
   mcfg_feat_context_e_type proc_context 
)
{
  boolean condition = FALSE; 
  pdc_get_feature_ind_msg_v01 feature;
  mcfg_setting_store_e_type feat_store = MCFG_SETTING_CURR; 

/* -------------------------------------------------------------------- */

  MCFG_MSG_MED_1("eval expr for featureTag %d", featureTag);
  if (MCFG_FEAT_CONTEXT_DEACTIVATION == proc_context)
  {
    MCFG_MSG_MED_1("cmp with original mcfg_settings for slot %d", sloti);
    feat_store = MCFG_SETTING_BKUP; 
  }

  // features are dependent on the slot and processing context
  if (MCFG_ERR_NONE != mcfg_get_feature(sloti, feat_store, &feature))
  {
    MCFG_MSG_ERROR_1("mcfg_is_expr_true sloti %d fail to get feature", 0);
    return FALSE;
  }

  // TODO: remove print statement
  MCFG_MSG_LOW("Finished comparing feature");

  // expression will point to a character buffer in future
  // MCFG_CHECK_NULL_PTR_RET_FALSE(expression);

  // process expression depending on the feature 
  // TODO: Add cases for other feature settings
  switch (featureTag)
  {
    case MCFG_FEAT_MULTISIM:
      condition = mcfg_cmp_setting_to_expr(feature.multisim, expression);
      break;

    case MCFG_FEAT_OS:
      condition = mcfg_cmp_setting_to_expr(feature.os, expression);
      break;

    case MCFG_FEAT_MARKET:
      condition = mcfg_cmp_setting_to_expr(feature.market, expression);
      break;

    case MCFG_FEAT_DEPLOYMENT:
      condition = mcfg_cmp_setting_to_expr(feature.deployment, expression);
      break;

    case MCFG_FEAT_IMS_FEAT:
      // Mask expression with current ims_features before comparison
      condition = mcfg_cmp_setting_to_expr((feature.ims_feature & expression.value), expression);
      break;

    case MCFG_FEAT_COUNTRY:
      condition = mcfg_cmp_setting_to_expr(feature.country, expression);
      break;

    case MCFG_FEAT_VARIANT:
      condition = mcfg_cmp_setting_to_expr(feature.variant, expression);
      break;

    case MCFG_FEAT_CARRIER:
      condition = mcfg_cmp_setting_to_expr(feature.carrier, expression);
      break;

    default: 
      // return false if feature is not current supported
      MCFG_MSG_HIGH_1("featureTag not supported: %d", featureTag);
  }

  MCFG_MSG_LOW_1("mcfg_is_expr_true ret status %d", condition);
  // return the value of the conditional expression
  return condition;
}


/*===========================================================================

FUNCTION  mcfg_get_processing_status() 

DESCRIPTION
  Determines whether the current config processing status is active

DEPENDENCIES
  Requires core service tasks like NV and EFS be available 

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
    FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_processing_status_set(boolean new_status) {
  uint8 success = TRUE; // in case additional logic is needed in future

/* -------------------------------------------------------------------- */

#ifdef MCFG_DEBUG 
  MCFG_MSG_MED_1("set processing flag: %d", new_status);
#endif /* MCFG_DEBUG */
  mcfg_processing_status = new_status;
  return success;
}


/*===========================================================================

FUNCTION  mcfg_feat_if() 

DESCRIPTION
  Processes an "if" tag

DEPENDENCIES

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
            FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_feat_if
(
   mcfg_slot_index_type_e_type sloti, 
   mcfg_feat_s_type *feat_data,
   mcfg_feat_context_e_type proc_context 
)
{
  boolean status = TRUE;
  boolean is_off = TRUE;
  mcfg_feat_list_node *new_node;
  mcfg_feat_list_node *top_node;

/* -------------------------------------------------------------------- */

  MCFG_CHECK_NULL_PTR_RET_FALSE(feat_data);

  // create a new node to be pushed onto feature list
  new_node = mcfg_malloc(sizeof(mcfg_feat_list_node));
  if (!new_node)
  {
    return FALSE;
  }
  MCFG_MSG_LOW_1("mcfg_feat_if new_node addr: %p", (void *) new_node);

  top_node = list_peek_front(&(mcfg_feat_list));
  if ((top_node == NULL) || !top_node->was_off) {
    is_off = !(mcfg_is_expr_true(sloti, feat_data->feature, feat_data->expression, proc_context));
  }

  MCFG_MSG_LOW_1("feat_tag parameter - is_off: %d", is_off);

  // set properties of new node
  if (top_node == NULL)
  {
    MCFG_MSG_LOW("feat stack empty, new->was_off = FALSE");
    new_node->was_off = FALSE;
  }
  else 
  {
    MCFG_MSG_LOW("feat stack not empty, new->was_off = top->is_off");
    new_node->was_off = top_node->is_off;
  }
  new_node->is_off = is_off;
  /* true_found tracks when a clause evaluates to true. This ensures
     "elif" clauses are skipped if a previous clause had evaluated to true */
  new_node->true_found = !is_off;
  new_node->conditional_type = feat_data->conditional_type;

  mcfg_print_node(new_node);

  // add node to stack
  list_push_front(&(mcfg_feat_list), &(new_node->link));
  //MCFG_MSG_LOW_1("mcfg_feat_if mcfg_feat_list top addr (original): %p", (void *) top_node);

  MCFG_MSG_LOW_1("mcfg_feat_if status: %d", status);
  return status; 
}


/*===========================================================================

FUNCTION  mcfg_feat_elif() 

DESCRIPTION
  Processes an "elif" tag

DEPENDENCIES

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
            FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_feat_elif
(
   mcfg_slot_index_type_e_type sloti, 
   mcfg_feat_s_type* feat_data,
   mcfg_feat_context_e_type proc_context 
)
{
  boolean status = TRUE;
  boolean is_off = TRUE;
  mcfg_feat_list_node *top_node;

/* -------------------------------------------------------------------- */

  MCFG_CHECK_NULL_PTR_RET_FALSE(feat_data);

  top_node = list_peek_front(&(mcfg_feat_list));
  do
  {
    if (top_node == NULL)
    {
      MCFG_MSG_LOW("mcfg_feat_elif: skip eval expr - missing if clause before elif");
      status = FALSE; 
      break;
    }
    if (MCFG_COND_TYPE_ELSE == top_node->conditional_type)
    {
      MCFG_MSG_LOW("mcfg_feat_elif: skip eval expr - elif or else clause after else");
      status = FALSE; 
      break;
    }
  }
  while (0); 

  if (status == TRUE)
  {
    // "elif" only matters if it appears within a previous scope that was not being skipped, i.e. that was "on"
    if (!top_node->was_off) 
    {
      /* true_found tracks whether a previous clause had evaluated to true */
      if (top_node->true_found)
      {
        MCFG_MSG_LOW("mcfg_feat_elif: skip eval expr - already entered prev clause in curr scope");
      }
      else 
      {
        if (MCFG_COND_TYPE_ELSE == feat_data->conditional_type)
        {
          is_off = FALSE;
          top_node->true_found = TRUE;
        }
        else 
        {
          // set is_off variable when no clause from current scope has been "on", i.e. true hasn't been found
          is_off = !(mcfg_is_expr_true(sloti, feat_data->feature, feat_data->expression, proc_context));
          top_node->true_found = !is_off;
        }
      }
    }
    top_node->is_off = is_off;
    top_node->conditional_type = feat_data->conditional_type;
  }

  mcfg_print_node(top_node);

  MCFG_MSG_LOW_1("mcfg_feat_elif status: %d", status);
  return status; 
}


/*===========================================================================

FUNCTION  mcfg_feat_endif() 

DESCRIPTION
  Processes an "endif" tag

DEPENDENCIES

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
            FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_feat_endif()
{
  boolean status = TRUE;
  boolean was_off;
  mcfg_feat_list_node *tmp; 
  mcfg_feat_list_node *top_node;

/* -------------------------------------------------------------------- */

  top_node = list_pop_front(&(mcfg_feat_list));
  if (top_node == NULL)
  {
#ifdef MCFG_DEBUG 
    MCFG_MSG_MED("mcfg_feat_endif feat_stack already empty");
#endif /* MCFG_DEBUG */
    // error: endif clause cannot appear unless there is a preceding if clause
    status = FALSE; 
  }
  else 
  {
    // save was_off state
    was_off = top_node->was_off;

#ifdef MCFG_DEBUG 
    MCFG_MSG_LOW_1("mcfg_feat_endif mcfg_feat_list top addr (original): %p", (void *) top_node);
    //MCFG_MSG_LOW_1("mcfg_feat_endif feat_stack addr: %p", (void *) feat_stack);
    //MCFG_MSG_LOW_1("mcfg_feat_endif tmp addr: %p", (void *) tmp);
#endif /* MCFG_DEBUG */
    mcfg_free(top_node);
    MCFG_MSG_LOW("mcfg_feat_endif: just freed previous scope");

    top_node = list_peek_front(&(mcfg_feat_list));
    if (top_node != NULL)
    {
      // update current status to be same as "was off"
      top_node->is_off = was_off; 
      mcfg_print_node(top_node);
    }
  }
  if (top_node == NULL)
  {
    MCFG_MSG_LOW("mcfg_feat_endif: top_node == NULL");
  }


  MCFG_MSG_LOW_1("mcfg_feat_endif status: %d", status);
  return status; 
}

/*===========================================================================

FUNCTION  mcfg_feature_status_cleanup() 

DESCRIPTION
  Restores feature stack and processing status back to default values

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static void mcfg_feature_status_cleanup()
{
  // delete the feature stack
  mcfg_feat_list_purge();
  // set default processing status back to ACTIVE
  mcfg_processing_status_set(PROCESSING_ON);
}


/*===========================================================================

FUNCTION  mcfg_feat_list_update() 

DESCRIPTION
  Deletes the feature stack 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_feat_list_purge()
{
  boolean status = TRUE;
  mcfg_feat_list_node *top_node; 

/* -------------------------------------------------------------------- */

  MCFG_MSG_LOW("purging FeatureTag stack");

  top_node = list_pop_front(&(mcfg_feat_list));
  while (top_node != NULL)
  {
    mcfg_free(top_node);
    top_node = list_pop_front(&(mcfg_feat_list));
  }

  MCFG_MSG_LOW_1("FeatureTag stack purge status: %d", status);
  return status;
}


/*===========================================================================

FUNCTION  mcfg_feat_list_update() 

DESCRIPTION
  Updates processing status based on current state of feature stack

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_feat_list_update
(
   mcfg_slot_index_type_e_type sloti, 
   mcfg_feat_s_type *feat_data,
   mcfg_feat_context_e_type proc_context 
) 
{
  boolean status = TRUE;

/* -------------------------------------------------------------------- */

  MCFG_CHECK_NULL_PTR_RET_FALSE(feat_data);

  MCFG_MSG_LOW_1("feat_stack_update: begin processing conditional (type = 0x%02x)", feat_data->conditional_type);
  switch (feat_data->conditional_type) 
  {
    case MCFG_COND_TYPE_IF:
      status = mcfg_feat_if(sloti, feat_data, proc_context);
      break;
    
    case MCFG_COND_TYPE_ELIF:
    case MCFG_COND_TYPE_ELSE: 
      status =  mcfg_feat_elif(sloti, feat_data, proc_context);
      break;
    
    case MCFG_COND_TYPE_ENDIF:
      status =  mcfg_feat_endif();
      break;
    
    default: 
      status = FALSE;
  }

  MCFG_MSG_LOW_2("MCFG FeatureTag: %d returned status %d", 
                 feat_data->conditional_type, status); 

  return status;
}

/*===========================================================================

FUNCTION  mcfg_process_status_update() 

DESCRIPTION
  Updates processing status based on current state of feature stack.
  When stack is currently skipping items the processing status is inactive.
  When stack is not currently skipping items the processing status is active.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_process_status_update() {
  boolean status = TRUE;
  mcfg_feat_list_node *top_node;

/* -------------------------------------------------------------------- */
  top_node = list_peek_front(&(mcfg_feat_list));
  if (top_node == NULL) {
    mcfg_processing_status_set(PROCESSING_ON);
  } else {
    mcfg_processing_status_set(!(top_node->was_off || top_node->is_off));
  }

  return status;
}

/*===========================================================================

FUNCTION  mcfg_feat_info_unpack() 

DESCRIPTION
    Reads TLV data into a buffer representing a featureTag. 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static boolean mcfg_feat_info_unpack
(
    uint8* ram_cust_data_ptr,
    mcfg_feat_s_type* feat_node,
    uint32 feat_tlv_size
) 
{
  uint32 tlv_data_read = 0;
  boolean status = TRUE;

/* -------------------------------------------------------------------- */

  MCFG_CHECK_NULL_PTR_RET_FALSE(feat_node);

  // find condition tlv (mandatory), and read into buffer
  if(!mcfg_feat_decode_tlv(ram_cust_data_ptr, MCFG_FEAT_TLV_ID_CONDITION,
                           &feat_node->conditional_type, sizeof(mcfg_cond_type_e), feat_tlv_size))
  {
    // error if not found
    MCFG_MSG_ERROR("FeatDecode: Return fail for MCFG_FEAT_TLV_ID_CONDITION");
    return FALSE; 
  }

  MCFG_MSG_MED_1("feat_info_unpack: conditional = 0x%02x", feat_node->conditional_type);

  if (feat_node->conditional_type != MCFG_COND_TYPE_ENDIF && 
      feat_node->conditional_type != MCFG_COND_TYPE_ELSE)
  {
    // find featurename tlv (mandatory), and read into buffer
    if(!mcfg_feat_decode_tlv(ram_cust_data_ptr, MCFG_FEAT_TLV_ID_FEATURE_NAME,
                             &feat_node->feature, sizeof(mcfg_supported_features_e), feat_tlv_size))
    {
      // error if not found
      MCFG_MSG_ERROR("FeatDecode: Return fail for MCFG_FEAT_TLV_ID_FEATURE_NAME");
      return FALSE; 
    }

    // TODO: remove
    // MCFG_MSG_HIGH_1("feat_info_unpack (just before expr): conditional = 0x%02x", feat_node->conditional_type);
    if(!mcfg_feat_decode_tlv(ram_cust_data_ptr, MCFG_FEAT_TLV_ID_CONDITIONAL_EXPRESSION,
                             &feat_node->expression, sizeof(mcfg_expr_type_s), feat_tlv_size))
    {
      // Enable feature expression by default if expr. not provided
      feat_node->expression.value = TRUE;
    }
  }

  return status;
}

/*===========================================================================
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
============================================================================*/
/*===========================================================================

FUNCTION  mcfg_feat_init() 

DESCRIPTION
  Initializes the feature list

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mcfg_feat_init() {

  memset(&mcfg_feat_list, 0, sizeof(mcfg_feat_list_type));
}

/*===========================================================================

FUNCTION  mcfg_processing_status_get() 

DESCRIPTION
  Determines whether the current config processing status is active

DEPENDENCIES

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
    FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
boolean mcfg_processing_status_get() {

#ifdef MCFG_DEBUG 
  MCFG_MSG_MED_1("get processing flag: %d", mcfg_processing_status);
#endif /* MCFG_DEBUG */
  return mcfg_processing_status;
}

/*===========================================================================

FUNCTION  mcfg_feature_tag_walk_through() 

DESCRIPTION
  Performs a preliminary scan through MCFG data segment to verify sequence of
  feature tags listed create logical scopes that can be activated/deactivated
  within the configuration.

DEPENDENCIES

RETURN VALUE
  boolean - 0: SUCCESS - FeatureTags in config match intended usage
    1: FAILURE - FeatureTag unsuccessfully processed

SIDE EFFECTS
  None

===========================================================================*/
mcfg_error_e_type mcfg_feature_tag_walk_through(mcfg_config_type_e_type type, mcfg_config_s_type* config) 
{
  mcfg_error_e_type status = MODEM_CFG_INVALID_FEATURE_TAGS;

  uint8* mcfg_seg_ptr; 
  uint8* ram_cust_data_ptr; 
  uint32 mcfg_add_info = 0;
  uint32  num_mcfg_items = 0;
  mcfg_config_s_type*    p_mcfg_buf=NULL;

/* -------------------------------------------------------------------- */
  MCFG_CHECK_NULL_PTR_RET_FALSE(config);
  MCFG_MSG_LOW("====  starting mcfg_feature_tag_walk_through ==== ");

  do
  {
    /* Get the MCFG SEG from the Modem Config File buffer */
    mcfg_seg_ptr = mcfg_load_seg(config, &mcfg_add_info );
    if (mcfg_seg_ptr == NULL )
    {
      MCFG_MSG_ERROR("Cannot find MCFG_SEG for cfg");
      //modem_cfg_process_status = MODEM_CFG_PROC_LOAD_FAIL; 
      break;
    }

    /* Initialize our RAM ptr to the start of MCFG segment */
    ram_cust_data_ptr = mcfg_seg_ptr;

    /* Validate the MCFG Header Information and get the number of NV items from this data
       Defaulting parameters to be HW related to skip MUX'd processing within header validation */
    num_mcfg_items = validate_mcfg_header(type, ram_cust_data_ptr, MCFG_SUB_ID_FIRST, MCFG_REFRESH_TYPE_SUBS);
    if (!num_mcfg_items ) 
    {
      MCFG_MSG_ERROR("Validate MCFG Header Fail");
      break;
    }

    /* Move pointer to beginning of NV items */
    ram_cust_data_ptr = mcfg_seg_ptr + MCFG_NV_DATA_OFFSET;

    /* Subscription processing not relavent to feature walk through
       Defaulting related parameters to 0 */
    if (!mcfg_update_nvefs(type, num_mcfg_items, ram_cust_data_ptr, 
                           MCFG_SUB_ID_FIRST, MCFG_REFRESH_TYPE_SUBS, TRUE))
    {
      MCFG_MSG_ERROR("FeatureTag Err: Failure during walk through");
      //modem_cfg_process_status = MODEM_CFG_UPDATE_NVEFS_FAIL;
      break;
    }

    // feat stack should be empty after walk through
    if (list_size(&(mcfg_feat_list)) > 0)
    {
      MCFG_MSG_ERROR("Invalid FeatureTag sequence - Mismatched feature stack nodes");
      status = MODEM_CFG_INVALID_FEATURE_TAGS;
      break;
    }

    MCFG_MSG_LOW("FeatureTag walk through success");
    status = MCFG_ERR_NONE;
  }
  while (0); 

  if (status != MCFG_ERR_NONE)
  {
    mcfg_feature_status_cleanup();
  }

  return status;
}

/*===========================================================================

FUNCTION  mcfg_process_feature() 

DESCRIPTION
  Processes an MCFG FeatureTag

DEPENDENCIES

RETURN VALUE
  boolean - 0: SUCCESS - FeatureTag successfully processed
    1: FAILURE - FeatureTag unsuccessfully processed

SIDE EFFECTS
  None

===========================================================================*/
boolean mcfg_process_feature
(
    uint8  subs_id,
    uint8* ram_cust_data_ptr,
    uint32 feat_data_size,
    mcfg_feat_context_e_type proc_context 
) 
{
  mcfg_slot_index_type_e_type sloti;
  boolean status = TRUE;
  mcfg_feat_s_type feature;

/* -------------------------------------------------------------------- */

  MCFG_CHECK_NULL_PTR_RET_FALSE(ram_cust_data_ptr);

  sloti = mcfg_uim_map_sub_id_to_slot_index(subs_id);
  do {
    // Extract the feature information 
    if (!mcfg_feat_info_unpack(ram_cust_data_ptr, &feature, feat_data_size)) {
      status = FALSE;
      break;
    }
    MCFG_MSG_LOW("mcfg_process_feature: data unpacked");
    // update processing active/inactive status based on new feature stack
    if (!mcfg_feat_list_update(sloti, &feature, proc_context)) {
      status = FALSE;
      break;
    }
    MCFG_MSG_LOW("mcfg_process_feature: stack updated");
    // update processing active/inactive status based on updated feature stack
    if (!mcfg_process_status_update()) {
      status = FALSE;
      break;
    }
    MCFG_MSG_LOW("mcfg_process_feature: process status updated");
  }while (0);

  if (status != TRUE) {
    // purge_feat_stack whenever any featureTag extraction fails
    mcfg_feature_status_cleanup();
  }

  return status;
}
#endif  /* FEATURE_MCFG_FEATURETAG */
