/*==============================================================================

                M O D E M   C O N F I G   S E L E C T I O N

GENERAL DESCRIPTION
  Contains the MCFG - MRE interface chnages to read the MCFG xml file for the Selection Record DB.

Copyright (c) 2012-2016 by QUALCOMM Technologies Incorporated.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_sel/src/mcfg_sel_db.c#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
12/07/13  sk   Created module

==============================================================================*/


/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"

#include "mcfg_common.h"
#include "msg.h"
#include "mcfg_osal.h"
#include "mcfg_int.h"
#include "mcfg_uim.h"
#include "mcfg_sel.h"
#include "mcfg_setting.h"


#include "mcfg_sel_db.h"
#include "mre_xml.h"
#include "mre_set.h"
#include "mre_namedobj.h"
#include "mre_util.h"
#include "sys_v.h"


#ifdef FEATURE_MCFG_SEL_DB
/*==============================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/

#define MCFG_XML_PATH  "/nv/item_files/mcfg/"
#define MCFG_XML_FILE MCFG_XML_PATH "mcfg_sel_db.xml"

#define MCFG_MEM_FREE_IF(pObj)                                        \
      if (NULL != pObj)                                               \
      {                                                               \
        mcfg_free(pObj);                                              \
      }                                                               

#define MCFG_IS_STR_TRUE(p)                                           \
      (strncmp((p), "true", strlen("true")) == 0)


/*==============================================================================
  Typedefs
==============================================================================*/
typedef struct mcfg_iin_in_condition_s
{
  MRE_CONDITION_BASE;

  boolean      onNotPresent;
  char        *pStoreIn;
  mre_set_t   *pIinSet;
} mcfg_iin_in_condition_s_type;

typedef struct mcfg_gid_in_condition_s
{
  MRE_CONDITION_BASE;

  boolean      onNotPresent;
  char        *pStoreIn;
  mre_set_t   *pGidSet;
} mcfg_gid_in_condition_s_type;

typedef struct mcfg_3gpp_imsi_plmn_in_condition_s
{
  MRE_CONDITION_BASE;

  boolean            onNotPresent;
  char              *pStoreIn;
  mre_named_set_t   *pPlmnSet;
} mcfg_3gpp_imsi_plmn_in_condition_s_type;

typedef struct mcfg_3gpp2_imsi_plmn_in_condition_s
{
  MRE_CONDITION_BASE;

  boolean            onNotPresent;
  char              *pStoreIn;
  mre_named_set_t   *pPlmnSet;
} mcfg_3gpp2_imsi_plmn_in_condition_s_type;

typedef struct mcfg_sel_record_s
{
  // Carrier name and index
  char    *pCarrierName;
  uint32      carrierIndex;

  // Pointers to the various tristates
  mre_tristate_state_t iin;
  mre_tristate_state_t gid1;
  mre_tristate_state_t gid2;
  mre_tristate_state_t m3gpp;
  mre_tristate_state_t m3gpp2;
  mcfg_config_id_s_type config_id;
  uint16      country;
  uint16      ims_feature;
} mcfg_sel_record_s_type;

typedef struct mcfg_sel_record_action_s
{
  MRE_ACTION_BASE;

  // Carrier name and index
  char       *pCarrierName;
  uint32      carrierIndex;

  // Feature tag
  uint16      country;
  uint16      ims_feature;

  // Names for the various tristates
  char *pIin;
  char *p3gpp;
  char *p3gpp2;
  char *pGid1;
  char *pGid2;
} mcfg_sel_record_action_s_type;

typedef enum mcfg_sel_priority_order_e
{
  MCFG_SEL_PRIORITY_ORDER_MIN = 0,
  MCFG_SEL_PRIORITY_ORDER_GID1 =  MCFG_SEL_PRIORITY_ORDER_MIN,
  MCFG_SEL_PRIORITY_ORDER_GID2,
  MCFG_SEL_PRIORITY_ORDER_3GPP2,
  MCFG_SEL_PRIORITY_ORDER_3GPP,
  MCFG_SEL_PRIORITY_ORDER_IIN,
  MCFG_SEL_PRIORITY_ORDER_MAX,
} mcfg_sel_priority_order_t;

/*==============================================================================
  Variables
==============================================================================*/

static mre_policy_t              *pPolicy = NULL;
static mre_namespace_info_t      *pNamespaceInfo;
static mcfg_mre_state_t          *pMcfgState;

static mcfg_sel_record_s_type mcfg_selected_record;
static mcfg_sel_priority_order_t mcfg_sel_priority_order[MCFG_SEL_PRIORITY_ORDER_MAX];

/*============================================================================== 
   
                    PRIVATE FUNCTION DEFINITIONS
   
  ==============================================================================*/

/*===========================================================================

FUNCTION  mcfg_str_dup() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/
char * mcfg_str_dup(
  const char  *pStr
)
{
  size_t   len;
  char    *pDup = NULL;
  MCFG_CHECK_NULL_PTR_RET_NULL(pStr);

  len = strlen(pStr) + 1;
  pDup = mcfg_malloc(len);
  MCFG_CHECK_NULL_PTR_RET_NULL(pDup);

  memscpy(pDup, len, pStr, len);

  return pDup;
}

/*===========================================================================

FUNCTION  mcfg_process_condition_attributes() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static mre_status_t mcfg_process_condition_attributes(
  mre_xml_element_t const  *pElem,
  char                    **ppStoreIn,
  boolean                  *onNotPresent  
)
{
  char const   *pStr;
  mre_status_t  status = MRE_STATUS_ERR_MALFORMED_XML;
  MCFG_CHECK_NULL_PTR_RET_ERROR(pElem, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(ppStoreIn, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(onNotPresent, status);

  /*  Process the attributes and inline for the mcfg condition.
         Both are required, and function returns an error if any are missing.
   */
  pStr = mre_xml_get_attribute(pElem, "not_present");
  if (pStr != NULL)
  {  
    if (strcmp(pStr, "true") == 0)
    {
      *onNotPresent = TRUE;
    }
    else if (strcmp(pStr, "false") == 0)
    {
      *onNotPresent = FALSE;
    }
    else
    {
      // Value other than "true" or "false" was in XML
      goto Done;
    }
  }
  else
  {
    // not_present is required -- this currently fails XML parsing
    goto Done;

    // Can switch it out to below line to set a default value if wanted
    // onNotPresent = FALSE;
  }

  pStr = mre_xml_get_attribute(pElem, "store_in");
  if (pStr != NULL)
  {
    *ppStoreIn = mcfg_str_dup(pStr);
  }
  else
  {
    // store_in is a required XML element.  Can remove this else if want it to be optional (would require changes in processing as well)
    goto Done;
  }

  status = MRE_STATUS_SUCCESS;

Done:
  return status;

}

/*===========================================================================

FUNCTION  mcfg_iin_in_condition_dtor() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static void mcfg_iin_in_condition_dtor(
  void  *pObj
)
{
  MCFG_CHECK_NULL_PTR_RET(pObj);

  mcfg_iin_in_condition_s_type *pMe = (mcfg_iin_in_condition_s_type *) pObj;
  MRE_RELEASE_IF(pMe->pIinSet);
  MCFG_MEM_FREE_IF(pMe->pStoreIn);
  mre_condition_dtor(pMe);
}

/*===========================================================================

FUNCTION  mcfg_iin_in_condition_evaluate() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static boolean mcfg_iin_in_condition_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mcfg_iin_in_condition_s_type *pCond  = (mcfg_iin_in_condition_s_type *) pCondition;
  boolean                  result = FALSE;

  MCFG_CHECK_NULL_PTR_RET_FALSE(pCondition);
  MCFG_CHECK_NULL_PTR_RET_FALSE(pCtx);

  // If IIN is not available, use the onNotPresent value from XML for the return
  if (!pMcfgState->have_iin)
  {
    result = pCond->onNotPresent;
    mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_UNDEF);
    goto Done;
  }
  else
  {
    if (pCond->pIinSet == NULL)
    {
      // No IIN values were in XML
      goto Done;
    }

    if (mre_set_contains(pCond->pIinSet, &pMcfgState->iin))
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_TRUE);
      result = TRUE;
    }
    else
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_FALSE);
    }
  }

Done:

  MCFG_MSG_SPRINTF_3("mcfg_iin_in_condition_evaluate(), have_iin %d, iin %s, result %d", 
                   pMcfgState->have_iin, pMcfgState->iin, result);

  return result;
}

static boolean mcfg_iin_compare(
  void const *e1, 
  void const *e2
)
{
  MCFG_CHECK_NULL_PTR_RET_FALSE(e1);
  MCFG_CHECK_NULL_PTR_RET_FALSE(e2);

  MCFG_MSG_SPRINTF_2("IIN %s v.s ICCID %s", (char *)e1, (char *)e2);

  if (strlen((char *)e1) <= strlen((char *)e2) && 
      strlen((char *)e2) < MCFG_UIM_ICCID_STR_BUF_LEN )
  {
     return (memcmp(e1, e2, strlen((char *)e1)) == 0);
  }
  
  return FALSE;
}

/*===========================================================================

FUNCTION  mcfg_iin_list_read() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static mre_set_t* mcfg_iin_list_read(
  char const               *pStr
)
{
  char       token[MCFG_UIM_ICCID_STR_BUF_LEN];
  mre_set_t *pIinSet;

  MCFG_CHECK_NULL_PTR_RET_NULL(pStr);

  pIinSet = mre_set_new(MCFG_UIM_ICCID_STR_BUF_LEN,
                        5,
                        MRE_SET_NO_MAX_SIZE,
                        mcfg_iin_compare,
                        NULL);

  while (mre_get_token(&pStr, token, MCFG_UIM_ICCID_STR_BUF_LEN-1))
  {
    MCFG_MSG_SPRINTF_1("IIN %s", token);
    mre_set_add(pIinSet, token);
  }

  if (mre_set_num_elems(pIinSet) == 0)
  {
    // No IINs were successfully read, at least one is required - release set to return NULL
    MRE_RELEASE_IF(pIinSet);
  }

Done:
  return pIinSet;
}

/*===========================================================================

FUNCTION  mcfg_iin_in_condition_new() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static mre_status_t mcfg_iin_in_condition_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mcfg_iin_in_condition_s_type *pCond = NULL;
  char const              *pStr;
  mre_status_t             status = MRE_STATUS_ERR_MALFORMED_XML;
  mre_set_t               *pSet = NULL;
  char                    *pStoreIn = NULL;
  boolean                  onNotPresent;

  MCFG_CHECK_NULL_PTR_RET_ERROR(pElem, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(pPolicy, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(ppCondition, status);

  /*  Process the attributes and inline for the iin_in condition.
         All are required, and function returns an error if any are missing.
   */
  if(MRE_FAILED(mcfg_process_condition_attributes(pElem, &pStoreIn, &onNotPresent)))
  {
    goto Done;
  }

  pStr = mre_xml_get_text(pElem);
  if (pStr != NULL)
  {
    pSet = mcfg_iin_list_read(pStr);

    if (pSet == NULL)
    {
      goto Done;
    }
  }

  pCond = (mcfg_iin_in_condition_s_type *) mre_condition_new( sizeof(mcfg_iin_in_condition_s_type),
                                                         pPolicy,
                                                         mcfg_iin_in_condition_evaluate,
                                                         mcfg_iin_in_condition_dtor,
                                                         pElem
                                                       );

  pCond->onNotPresent = onNotPresent;
  pCond->pStoreIn     = pStoreIn;
  pCond->pIinSet      = pSet;

  status = MRE_STATUS_SUCCESS;

  *ppCondition = (mre_condition_t *) pCond;

Done:

  MCFG_MSG_MED_1("mcfg_iin_in_condition_new(), status %d", status);

  return status;
}


/*===========================================================================

FUNCTION  mcfg_gid_in_condition_dtor() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static void mcfg_gid_in_condition_dtor(
  void  *pObj
)
{
  MCFG_CHECK_NULL_PTR_RET(pObj);

  mcfg_gid_in_condition_s_type *pMe = (mcfg_gid_in_condition_s_type *) pObj;
  MRE_RELEASE_IF(pMe->pGidSet);
  MCFG_MEM_FREE_IF(pMe->pStoreIn);
  mre_condition_dtor(pMe);
}

/*===========================================================================

FUNCTION  mcfg_gid_in_condition_evaluate() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static boolean mcfg_gid_in_condition_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mcfg_gid_in_condition_s_type *pCond  = (mcfg_gid_in_condition_s_type *) pCondition;
  boolean                  result = FALSE;
  mcfg_gid_index_e_type    gid_idx = MCFG_GID_INDEX_1;

  MCFG_CHECK_NULL_PTR_RET_FALSE(pCondition);
  MCFG_CHECK_NULL_PTR_RET_FALSE(pCtx);

  if (strncmp(pCond->pStoreIn, "gid2", 4)==0)
  {
    gid_idx = MCFG_GID_INDEX_2;
  }
  else
  {
    gid_idx = MCFG_GID_INDEX_1;
  }

  // If gid is not available, use the onNotPresent value from XML for the return
  if (!pMcfgState->have_gid[gid_idx])
  {
    result = pCond->onNotPresent;
    mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_UNDEF);
    goto Done;
  }
  else
  {
    if (pCond->pGidSet == NULL)
    {
      // No gid values were in XML
      goto Done;
    }

    if (mre_set_contains(pCond->pGidSet, &pMcfgState->gid[gid_idx]))
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_TRUE);
      result = TRUE;
    }
    else
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_FALSE);
    }
  }

Done:

  MCFG_MSG_SPRINTF_4("mcfg_gid_in_condition_evaluate(), gid_idx %d, have_gid %d, result %d, gid %s", 
                   gid_idx, pMcfgState->have_gid[gid_idx], result, pMcfgState->gid[gid_idx]);

  return result;
}

static boolean mcfg_gid_compare(
  void const *e1, 
  void const *e2
)
{
  MCFG_CHECK_NULL_PTR_RET_FALSE(e1);
  MCFG_CHECK_NULL_PTR_RET_FALSE(e2);

  MCFG_MSG_SPRINTF_2("GID from xml %s v.s GID %s", (char *)e1, (char *)e2);

  if (strlen((char *)e1) <= strlen((char *)e2) && 
      strlen((char *)e2) < MCFG_UIM_GID_STR_BUF_LEN )
  {
    return (memcmp(e1, e2, strlen((char *)e1)) == 0);
  }

  return FALSE;
}

/*===========================================================================

FUNCTION  mcfg_gid_list_read() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static mre_set_t* mcfg_gid_list_read(
  char const               *pStr
)
{
  char       token[MCFG_UIM_GID_STR_BUF_LEN];
  mre_set_t *pGidSet;

  MCFG_CHECK_NULL_PTR_RET_NULL(pStr);

  pGidSet = mre_set_new(MCFG_UIM_GID_STR_BUF_LEN,
                        5,
                        MRE_SET_NO_MAX_SIZE,
                        mcfg_gid_compare,
                        NULL);

  while (mre_get_token(&pStr, token, MCFG_UIM_GID_STR_BUF_LEN))
  {
    MCFG_MSG_SPRINTF_1("gid %s", token);
    mre_set_add(pGidSet, token);
  }

  if (mre_set_num_elems(pGidSet) == 0)
  {
    // No GIDs were successfully read, at least one is required - release set to return NULL
    MRE_RELEASE_IF(pGidSet);
  }

Done:
  return pGidSet;
}

/*===========================================================================

FUNCTION  mcfg_gid_in_condition_new() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static mre_status_t mcfg_gid_in_condition_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mcfg_gid_in_condition_s_type *pCond = NULL;
  char const              *pStr;
  mre_status_t             status = MRE_STATUS_ERR_MALFORMED_XML;
  mre_set_t               *pSet = NULL;
  char                    *pStoreIn = NULL;
  boolean                  onNotPresent;

  MCFG_CHECK_NULL_PTR_RET_ERROR(pElem, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(pPolicy, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(ppCondition, status);

  /*  Process the attributes and inline for the gid_in condition.
         All are required, and function returns an error if any are missing.
   */
  if(MRE_FAILED(mcfg_process_condition_attributes(pElem, &pStoreIn, &onNotPresent)))
  {
    goto Done;
  }

  pStr = mre_xml_get_text(pElem);
  if (pStr != NULL)
  {
    pSet = mcfg_gid_list_read(pStr);

    if (pSet == NULL)
    {
      goto Done;
    }
  }

  pCond = (mcfg_gid_in_condition_s_type *) mre_condition_new( sizeof(mcfg_gid_in_condition_s_type),
                                                         pPolicy,
                                                         mcfg_gid_in_condition_evaluate,
                                                         mcfg_gid_in_condition_dtor,
                                                         pElem
                                                       );

  pCond->onNotPresent = onNotPresent;
  pCond->pStoreIn     = pStoreIn;
  pCond->pGidSet      = pSet;

  status = MRE_STATUS_SUCCESS;

  *ppCondition = (mre_condition_t *) pCond;

Done:

  MCFG_MSG_MED_1("mcfg_gid_in_condition_new(), status %d", status);

  return status;
}

/*===========================================================================

FUNCTION  mcfg_3gpp_imsi_plmn_in_condition_dtor() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static void mcfg_3gpp_imsi_plmn_in_condition_dtor(
  void  *pObj
)
{
  mcfg_3gpp_imsi_plmn_in_condition_s_type *pMe = (mcfg_3gpp_imsi_plmn_in_condition_s_type *) pObj;

  MCFG_CHECK_NULL_PTR_RET(pObj);

  MRE_RELEASE_IF(pMe->pPlmnSet);
  MCFG_MEM_FREE_IF(pMe->pStoreIn);
  mre_condition_dtor(pMe);
}

/*===========================================================================

FUNCTION  mcfg_3gpp_imsi_plmn_in_condition_evaluate() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static boolean mcfg_3gpp_imsi_plmn_in_condition_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mcfg_3gpp_imsi_plmn_in_condition_s_type *pCond  = (mcfg_3gpp_imsi_plmn_in_condition_s_type *) pCondition;
  boolean result = FALSE;

  MCFG_CHECK_NULL_PTR_RET_FALSE(pCondition);
  MCFG_CHECK_NULL_PTR_RET_FALSE(pCtx);

  // If PLMN is not available, use the onNotPresent value from XML for the return
  if (!pMcfgState->have_plmn)
  {
    result = pCond->onNotPresent;
    mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_UNDEF);
    goto Done;
  }
  else
  {
    if (pCond->pPlmnSet == NULL)
    {
      // No PLMNs to compare against
      goto Done;
    }

    if (mre_named_set_contains(pCond->pPlmnSet, pMcfgState->pPlmn))
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_TRUE);
      result = TRUE;
    }
    else
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_FALSE);
    }
  }

Done:

    MCFG_MSG_MED_3("mcfg_3gpp_imsi_plmn_in_condition_evaluate(), have_plmn %d, pPlmn %p, result %d", 
                   pMcfgState->have_plmn, pMcfgState->pPlmn, result);
  return result;
}

/*===========================================================================

FUNCTION  mcfg_3gpp_imsi_plmn_in_condition_new() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static mre_status_t mcfg_3gpp_imsi_plmn_in_condition_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mcfg_3gpp_imsi_plmn_in_condition_s_type *pCond = NULL;
  char const                     *pStr;
  mre_status_t                    status = MRE_STATUS_ERR_MALFORMED_XML;
  mre_named_set_t                *pSet;
  char                           *pStoreIn = NULL;
  boolean                         onNotPresent;

  MCFG_CHECK_NULL_PTR_RET_ERROR(pElem, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(pPolicy, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(ppCondition, status);

  if(MRE_FAILED(mcfg_process_condition_attributes(pElem, &pStoreIn, &onNotPresent)))
  {
    goto Done;
  }

  pSet = mre_named_set_new( NULL,
                            MRE_NAMED_PLMN_SET,
                            sizeof(sys_plmn_id_s_type),
                            10,
                            MRE_SET_NO_MAX_SIZE,
                            NULL,
                            NULL );

  status = mre_plmn_list_read(pElem, pSet);

  if (MRE_FAILED(status))
  {
    ref_cnt_obj_release(pSet);
    goto Done;
  }

  pCond = (mcfg_3gpp_imsi_plmn_in_condition_s_type *) mre_condition_new( sizeof(mcfg_3gpp_imsi_plmn_in_condition_s_type),
                                                                    pPolicy,
                                                                    mcfg_3gpp_imsi_plmn_in_condition_evaluate,
                                                                    mcfg_3gpp_imsi_plmn_in_condition_dtor,
                                                                    pElem
                                                                  );

  pCond->onNotPresent = onNotPresent;
  pCond->pStoreIn     = pStoreIn;
  pCond->pPlmnSet     = pSet;

  status = MRE_STATUS_SUCCESS;

  *ppCondition = (mre_condition_t *) pCond;

Done:

  MCFG_MSG_MED_1("mcfg_3gpp_imsi_plmn_in_condition_new(), status %d", status);
  
  return status;
}



/*===========================================================================

FUNCTION  mcfg_3gpp2_imsi_plmn_in_condition_dtor() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static void mcfg_3gpp2_imsi_plmn_in_condition_dtor(
  void  *pObj
)
{
  mcfg_3gpp2_imsi_plmn_in_condition_s_type *pMe = (mcfg_3gpp2_imsi_plmn_in_condition_s_type *) pObj;
  MRE_RELEASE_IF(pMe->pPlmnSet);
  MCFG_MEM_FREE_IF(pMe->pStoreIn);
  mre_condition_dtor(pMe);
}

/*===========================================================================

FUNCTION  mcfg_3gpp2_imsi_plmn_in_condition_evaluate() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static boolean mcfg_3gpp2_imsi_plmn_in_condition_evaluate(
  mre_condition_t const *pCondition,
  void                  *pCtx
)
{
  mcfg_3gpp2_imsi_plmn_in_condition_s_type *pCond  = (mcfg_3gpp2_imsi_plmn_in_condition_s_type *) pCondition;
  boolean                             result = FALSE;

  MCFG_CHECK_NULL_PTR_RET_FALSE(pCond);
  MCFG_CHECK_NULL_PTR_RET_FALSE(pCond->pStoreIn);
  MCFG_CHECK_NULL_PTR_RET_FALSE(pCtx);
  MCFG_CHECK_NULL_PTR_RET_FALSE(pMcfgState);

  // If PLMN is not available, use the onNotPresent value from XML for the return
  if (!pMcfgState->have_plmn_3gpp2)
  {
    result = pCond->onNotPresent;
    mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_UNDEF);
  }
  else
  {
    if (pCond->pPlmnSet == NULL)
    {
      // No PLMN values provided to compare against
      goto Done;
    }

    if (mre_named_set_contains(pCond->pPlmnSet, pMcfgState->pPlmn3gpp2))
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_TRUE);
      result = TRUE;
    }
    else
    {
      mre_tristate_set(pCond->pStoreIn, EXECUTE_CTX_POLICY(pCtx), MRE_TRISTATE_FALSE);
    }
  }

Done:

  MCFG_MSG_MED_3("mcfg_3gpp2_imsi_plmn_in_condition_evaluate(), have_plmn_3gpp2 %d, pPlmn3gpp2 %d, result %d", 
                   pMcfgState->have_plmn_3gpp2, pMcfgState->pPlmn3gpp2, result);
  
  return result;
}

/*===========================================================================

FUNCTION  mcfg_3gpp2_imsi_plmn_in_condition_new() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static mre_status_t mcfg_3gpp2_imsi_plmn_in_condition_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_condition_t         **ppCondition
)
{
  mcfg_3gpp2_imsi_plmn_in_condition_s_type *pCond = NULL;
  char const                     *pStr;
  mre_status_t                    status = MRE_STATUS_ERR_MALFORMED_XML;
  mre_named_set_t                *pSet;
  char                           *pStoreIn = NULL;
  boolean                         onNotPresent;

  MCFG_CHECK_NULL_PTR_RET_ERROR(pElem, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(pPolicy, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(ppCondition, status);

  // reads the not present and store in attributes
  if(MRE_FAILED(mcfg_process_condition_attributes(pElem, &pStoreIn, &onNotPresent)))
  {
    goto Done;
  }

  pSet = mre_named_set_new( NULL,
                            MRE_NAMED_PLMN_SET,
                            sizeof(sys_plmn_id_s_type),
                            10,
                            MRE_SET_NO_MAX_SIZE,
                            NULL,
                            NULL );
  // reads the inline list of plmns -- required for a correct XML
  status = mre_plmn_list_read(pElem, pSet);

  if (MRE_FAILED(status))
  {
    ref_cnt_obj_release(pSet);
    goto Done;
  }

  pCond = (mcfg_3gpp2_imsi_plmn_in_condition_s_type *) mre_condition_new( sizeof(mcfg_3gpp2_imsi_plmn_in_condition_s_type),
                                                                     pPolicy,
                                                                     mcfg_3gpp2_imsi_plmn_in_condition_evaluate,
                                                                     mcfg_3gpp2_imsi_plmn_in_condition_dtor,
                                                                     pElem
                                                                   );

  pCond->onNotPresent = onNotPresent;
  pCond->pStoreIn     = pStoreIn;
  pCond->pPlmnSet     = pSet;

  status = MRE_STATUS_SUCCESS;

  *ppCondition = (mre_condition_t *) pCond;

Done:
  MCFG_MSG_MED_1("mcfg_3gpp2_imsi_plmn_in_condition_new(), status %d", status);

  return status;
}


/*-------- mre_test_condition_map --------*/
static mre_condition_map_element_t const  mcfg_condition_map[] =
{
  {"iin_in",             mcfg_iin_in_condition_new},
  {"imsi_3gpp_plmn_in",  mcfg_3gpp_imsi_plmn_in_condition_new},
  {"imsi_3gpp2_plmn_in", mcfg_3gpp2_imsi_plmn_in_condition_new},
  {"gid_in",			 mcfg_gid_in_condition_new},

  {NULL, NULL}
};

/*===========================================================================

FUNCTION  mcfg_sel_db_set_default_priority_order() 

DESCRIPTION
   This function sets the default priority order.
   
DEPENDENCIES
  None  

RETURN VALUE
  None 

SIDE EFFECTS
  None

===========================================================================*/

void mcfg_sel_db_set_default_priority_order( void )
{
  mcfg_sel_priority_order_t priority_order[MCFG_SEL_PRIORITY_ORDER_MAX] = { 
  	                                                                        MCFG_SEL_PRIORITY_ORDER_GID1,
  	                                                                        MCFG_SEL_PRIORITY_ORDER_GID2,
  	                                                                        MCFG_SEL_PRIORITY_ORDER_3GPP2,
  	                                                                        MCFG_SEL_PRIORITY_ORDER_3GPP,
  	                                                                        MCFG_SEL_PRIORITY_ORDER_IIN
  	                                                                      };
  uint8 sel_cnt =0;
  
  while(sel_cnt < MCFG_SEL_PRIORITY_ORDER_MAX)
  {
    mcfg_sel_priority_order[sel_cnt] = priority_order[sel_cnt];
	sel_cnt++;

  } 
}

/*===========================================================================

FUNCTION  mcfg_sel_db_priority_order_list_read() 

DESCRIPTION
   This Function reads the priority order from mcfg_sel_db.xml file and writes to global array mcfg_sel_priority_order[sel_cnt].

DEPENDENCIES
   None 

RETURN VALUE
   TRUE  :  If sucessfully read.
   FALSE :  If any discreprancy like invalid selection parameter and number of parameters specified is not equal to MCFG_NUMBER_OF_SELECTION_PARAMETERS.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcfg_sel_db_priority_order_list_read(char const *pStr)
{
	char token[MCFG_MAX_SELECTION_PARAMETER_TOKEN_LENGTH+1];
	uint16 sel_cnt = 0;
	boolean is_sel_param_duplicated[MCFG_SEL_PRIORITY_ORDER_MAX]={FALSE};
	mre_status_t  status = MRE_STATUS_ERR_MALFORMED_XML;
	MCFG_CHECK_NULL_PTR_RET_NULL(pStr);
	static MRE_STR_VALUE_TABLE(map) =
    {
      MRE_STR_VALUE_ENTRY("gid1",       MCFG_SEL_PRIORITY_ORDER_GID1),
      MRE_STR_VALUE_ENTRY("gid2",       MCFG_SEL_PRIORITY_ORDER_GID2),
      MRE_STR_VALUE_ENTRY("3gpp2_imsi", MCFG_SEL_PRIORITY_ORDER_3GPP2),
      MRE_STR_VALUE_ENTRY("3gpp_imsi",  MCFG_SEL_PRIORITY_ORDER_3GPP),
      MRE_STR_VALUE_ENTRY("iin",        MCFG_SEL_PRIORITY_ORDER_IIN),
    };
	  
	memset(&token,0,sizeof(token));
    while (mre_get_token(&pStr, token, MCFG_MAX_SELECTION_PARAMETER_TOKEN_LENGTH+1) && 
		   sel_cnt < MCFG_SEL_PRIORITY_ORDER_MAX)
	{  
	  
      MCFG_MSG_SPRINTF_1("mcfg_sel_db_priority_list_read: Priority order is %s",token);
      mcfg_sel_priority_order[sel_cnt] = MCFG_SEL_PRIORITY_ORDER_MAX; 
	  
      MRE_LOOKUP_STR_VALUE(map, token, mcfg_sel_priority_order_t, &mcfg_sel_priority_order[sel_cnt]);
	  if(MRE_FAILED(status))
	  {
	    MCFG_MSG_SPRINTF_1("mcfg_priority_list_read: Invalid selection parameter %s",token);
		return FALSE;
	  }

	  // This check is needed to ensure no repetetion of selection parameters in priority order is configured in seldb file.
	  if(is_sel_param_duplicated[mcfg_sel_priority_order[sel_cnt]])
	  {
	    MCFG_MSG_SPRINTF_1("mcfg_priority_list_read: Duplicate selection parameter %s",token);
		return FALSE;  
	  }
	  
	  is_sel_param_duplicated[mcfg_sel_priority_order[sel_cnt]] = TRUE;
	  sel_cnt++;
   }
		   
   if(sel_cnt != MCFG_SEL_PRIORITY_ORDER_MAX)
   {
     MCFG_MSG_ERROR_1("mcfg_sel_db_priority_list_read: inadequate number of selection parameters provided %d",sel_cnt);
     return FALSE;
   }
   return TRUE;
    
}

/*===========================================================================

FUNCTION  mcfg_sel_db_priority_order_action_dtor() 

DESCRIPTION
   This gets called after completion of <PriorityOrder> execution.It will free the memory for  allocated mre objects.

DEPENDENCIES
   None
RETURN VALUE
   None

SIDE EFFECTS
  None

===========================================================================*/

static void mcfg_sel_db_priority_order_action_dtor(
  void  *pObj
)
{
  mre_action_t  *pMe = (mre_action_t *) pObj;
  MCFG_CHECK_NULL_PTR_RET(pObj);
  mre_action_dtor(pMe);
}

/*===========================================================================

FUNCTION  mcfg_sel_db_priority_order_action_execute() 

DESCRIPTION
   This gets called during mre_execute().
DEPENDENCIES
   None
RETURN VALUE
   TRUE

SIDE EFFECTS
  None

===========================================================================*/

static boolean mcfg_sel_db_priority_order_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  return TRUE;
}

/*===========================================================================

FUNCTION  mcfg_sel_db_priority_order_action_new() 

DESCRIPTION
   This will create a new action item for priority_order tag.

DEPENDENCIES
   None

RETURN VALUE
   mre_status_t

SIDE EFFECTS
  None

===========================================================================*/

mre_status_t mcfg_sel_db_priority_order_action_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t               status = MRE_STATUS_ERR_MALFORMED_XML;
  char const                *pStr;
  boolean valid_prior_order = FALSE;
  mre_action_t  *pAct;
  
  MCFG_CHECK_NULL_PTR_RET_ERROR(pElem, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(pPolicy, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(ppAction, status);


  pStr = mre_xml_get_text(pElem);
  if (pStr != NULL)
  {
    valid_prior_order = mcfg_sel_db_priority_order_list_read(pStr);

    if (!valid_prior_order)
    {
      MCFG_MSG_ERROR("mcfg_priority_order_action_new: Invalid priorityorder in seldb, setting  default priority order");
      mcfg_sel_db_set_default_priority_order();
    }
  }
  
  status = MRE_STATUS_SUCCESS;
  pAct = (mre_action_t *) mre_action_new( sizeof(mre_action_t),
                                                 pPolicy,
                                                 mcfg_sel_db_priority_order_action_execute,
                                                 mcfg_sel_db_priority_order_action_dtor, 
                                                 pElem
                                               );
  *ppAction = (mre_action_t *) pAct;

Done:
  MCFG_MSG_MED_1("mcfg_priority_order_action_new(), Done, status %d",status);

  return status;
}

/*===========================================================================

FUNCTION  mcfg_sel_record_action_dtor() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/


/*-------- mcfg_sel_record_action_dtor --------*/
static void mcfg_sel_record_action_dtor(
  void  *pObj
)
{
  mcfg_sel_record_action_s_type  *pMe = (mcfg_sel_record_action_s_type *) pObj;
  MCFG_CHECK_NULL_PTR_RET(pObj);

  MCFG_MEM_FREE_IF(pMe->pCarrierName);
  MCFG_MEM_FREE_IF(pMe->pIin);
  MCFG_MEM_FREE_IF(pMe->p3gpp);
  MCFG_MEM_FREE_IF(pMe->p3gpp2);
  MCFG_MEM_FREE_IF(pMe->pGid1);
  MCFG_MEM_FREE_IF(pMe->pGid2);
  mre_action_dtor(pMe);
}


/*===========================================================================

FUNCTION  mcfg_sel_record_action_execute() 

DESCRIPTION
  this will get called for the selrec only after match is success

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

static boolean mcfg_sel_record_action_execute(
  mre_action_t const  *pAction,
  void                *pCtx
)
{
  mcfg_sel_record_action_s_type  *pAct = (mcfg_sel_record_action_s_type *) pAction;
  mcfg_sel_record_s_type         *pRecord = NULL;
  mre_status_t               status;
  boolean                         found = FALSE;
  uint16               sel_param_cnt = MCFG_SEL_PRIORITY_ORDER_MIN;
  mre_tristate_state_t pRecord_state = MRE_TRISTATE_UNDEF;
  mre_tristate_state_t sel_record_state = MRE_TRISTATE_UNDEF;

  MCFG_CHECK_NULL_PTR_RET_FALSE(pAction);
  MCFG_CHECK_NULL_PTR_RET_FALSE(pCtx);

  MCFG_MSG_HIGH_1("Executing mcfg_sel_record_action_execute() pAct->carrierIndex %lu", pAct->carrierIndex);

  pRecord = (mcfg_sel_record_s_type *)mcfg_malloc(sizeof(mcfg_sel_record_s_type));
  MCFG_CHECK_NULL_PTR_RET_FALSE(pRecord);

  pRecord->carrierIndex = pAct->carrierIndex;
  pRecord->pCarrierName = pAct->pCarrierName;

  /* Store each of the tristate values that are present in the action.
     * If the action value is NULL, that field was not used in the selection for the record
     * (For example, IIN and 3gpp plmn were used, but 3gpp2 plmn was not applicable, so it would be null)
     */
  if (pAct->pIin != NULL)
  {
    status = mre_tristate_get_value(pAct->pIin,
                                    EXECUTE_CTX_POLICY(pCtx),
                                    &pRecord->iin);
    MCFG_MSG_HIGH_1("iin tristate %d", pRecord->iin);
  }
  else
  {
    pRecord->iin = MRE_TRISTATE_UNDEF;
    MCFG_MSG_HIGH_1("iin tristate %d", pRecord->iin);
  }

  if (pAct->p3gpp != NULL)
  {
    status = mre_tristate_get_value(pAct->p3gpp,
                                    EXECUTE_CTX_POLICY(pCtx),
                                    &pRecord->m3gpp);
    MCFG_MSG_MED_1("3gpp tristate %d", pRecord->m3gpp);
  }
  else
  {
    pRecord->m3gpp = MRE_TRISTATE_UNDEF;
    MCFG_MSG_MED_1("3gpp tristate %d", pRecord->m3gpp);
  }

  if (pAct->p3gpp2 != NULL)
  {
    status = mre_tristate_get_value(pAct->p3gpp2,
                                    EXECUTE_CTX_POLICY(pCtx),
                                    &pRecord->m3gpp2);
    MCFG_MSG_MED_1("3gpp2 tristate %d", pRecord->m3gpp2);
  }
  else
  {
    pRecord->m3gpp2 = MRE_TRISTATE_UNDEF;
    MCFG_MSG_MED_1("3gpp2 tristate %d", pRecord->m3gpp2);
  }

  if (pAct->pGid1 != NULL)
  {
    status = mre_tristate_get_value(pAct->pGid1,
                                    EXECUTE_CTX_POLICY(pCtx),
                                    &pRecord->gid1);
    MCFG_MSG_HIGH_1("gid1 tristate %d", pRecord->gid1);
  }
  else
  {
    pRecord->gid1 = MRE_TRISTATE_UNDEF;
    MCFG_MSG_HIGH_1("gid1 tristate %d", pRecord->gid1);
  }

  if (pAct->pGid2 != NULL)
  {
    status = mre_tristate_get_value(pAct->pGid2,
                                    EXECUTE_CTX_POLICY(pCtx),
                                    &pRecord->gid2);
    MCFG_MSG_HIGH_1("gid2 tristate %d", pRecord->gid2);
  }
  else
  {
    pRecord->gid2 = MRE_TRISTATE_UNDEF;
    MCFG_MSG_HIGH_1("gid2 tristate %d", pRecord->gid2);
  }

  pRecord->country = pAct->country;
  pRecord->ims_feature = pAct->ims_feature;


  MCFG_MSG_MED_1("Sel Record Carrier Index %d", pRecord->carrierIndex);


  if(pRecord->carrierIndex != 0 && mcfg_selected_record.carrierIndex == 0)
  {
    found = TRUE;
  }
  while(found == FALSE && sel_param_cnt < MCFG_SEL_PRIORITY_ORDER_MAX)
  {
    switch(mcfg_sel_priority_order[sel_param_cnt])
    {
      case MCFG_SEL_PRIORITY_ORDER_GID1:
        pRecord_state = pRecord->gid1;
	    sel_record_state = mcfg_selected_record.gid1;
	    break;
      case MCFG_SEL_PRIORITY_ORDER_GID2:
        pRecord_state = pRecord->gid2;
	    sel_record_state = mcfg_selected_record.gid2;
	    break;
      case MCFG_SEL_PRIORITY_ORDER_3GPP2:
        pRecord_state = pRecord->m3gpp2;
	    sel_record_state = mcfg_selected_record.m3gpp2;
	    break;
      case MCFG_SEL_PRIORITY_ORDER_3GPP:
        pRecord_state = pRecord->m3gpp;
	    sel_record_state = mcfg_selected_record.m3gpp;
	    break;
      case MCFG_SEL_PRIORITY_ORDER_IIN:
        pRecord_state = pRecord->iin;
	    sel_record_state = mcfg_selected_record.iin;
	    break;
	  default:
        MCFG_MSG_ERROR_1("mcfg_sel_record_action_execute: Invalid selection parameter %d",mcfg_sel_priority_order[sel_param_cnt]);
	    break;
    }
	
    if(pRecord_state != sel_record_state)
    {
      if(pRecord_state == MRE_TRISTATE_TRUE)
      {
        found = TRUE;
        break;
      }
      else if(sel_record_state == MRE_TRISTATE_TRUE)
      {
        break;
      }
    }
    sel_param_cnt++;
  }

  if (found && pMcfgState)
  {

    MCFG_MSG_HIGH_2("Sel Record Carrier Index %lu is a better match than current %lu", 
                    pRecord->carrierIndex, mcfg_selected_record.carrierIndex);
    if (mcfg_sel_sw_get_config_by_carrier( pMcfgState->sloti,
                                           pRecord->carrierIndex, 
                                          &pRecord->config_id))
    {
      memscpy(&mcfg_selected_record, sizeof(mcfg_sel_record_s_type),
              pRecord, sizeof(mcfg_sel_record_s_type));
    }
    else
    {
      MCFG_MSG_HIGH("however, there's no matching config loaded on device");
    }
  }

  if(pRecord != NULL)
  {
    mcfg_free(pRecord);
    pRecord = NULL;
  }

  return TRUE;
}


/*===========================================================================

FUNCTION  mcfg_sel_record_action_new() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

mre_status_t mcfg_sel_record_action_new(
  mre_xml_element_t const *pElem,
  mre_policy_t            *pPolicy,
  mre_action_t            **ppAction
)
{
  mre_status_t               status = MRE_STATUS_ERR_MALFORMED_XML;
  char const                *pStr;
  char const                *pCarrier = NULL;
  uint32                     carrierIndex = 0;
  char const                *pIin = NULL;
  char const                *pGid1 = NULL;
  char const                *pGid2 = NULL;
  char const                *p3gppPlmn = NULL;
  char const                *p3gpp2Plmn = NULL;
  uint16                     ims_feature = 0;
  uint16                     country = 0;
  mcfg_sel_record_action_s_type  *pAct;

  MCFG_CHECK_NULL_PTR_RET_ERROR(pElem, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(pPolicy, status);
  MCFG_CHECK_NULL_PTR_RET_ERROR(ppAction, status);

  pStr = mre_xml_get_attribute(pElem, "mcfg_carrier_index");
  if (pStr != NULL)
  {
    carrierIndex = atoi(pStr);
  }
  
  pStr = mre_xml_get_attribute(pElem, "carrier_name");
  if (pStr != NULL)
  {
    pCarrier = mcfg_str_dup(pStr);
  }

  pStr = mre_xml_get_attribute(pElem, "country_code");
  if (pStr != NULL)
  {
    country = atoi(pStr);
  }

  pStr = mre_xml_get_attribute(pElem, "volte");
  if (pStr != NULL)
  {
    MCFG_MSG_SPRINTF_1("mcfg_sel_record_action_new(), pStr = '%s'", pStr);
    ims_feature = ims_feat_update(ims_feature, MCFG_IS_STR_TRUE(pStr), MCFG_IMS_FEAT_VOLTE_BIT);
  }

  pStr = mre_xml_get_attribute(pElem, "vowifi");
  if (pStr != NULL)
  {
    ims_feature = ims_feat_update(ims_feature, MCFG_IS_STR_TRUE(pStr), MCFG_IMS_FEAT_VOWIFI_BIT);
  }

  pStr = mre_xml_get_attribute(pElem, "hvolte");
  if (pStr != NULL)
  {
    ims_feature = ims_feat_update(ims_feature, MCFG_IS_STR_TRUE(pStr), MCFG_IMS_FEAT_HVOLTE_BIT);
  }

  // This makes the index and name a mandatory part of a SelRecord in XML -- can be modified/removed
  if (carrierIndex == 0 || pCarrier == NULL)
  {
    goto Done;
  }

  // The various tristate fields are optional, only need to add them in if the record is dependent on them
  pStr = mre_xml_get_attribute(pElem, "iin");
  if (pStr != NULL)
  {
    pIin = mcfg_str_dup(pStr);
  }

  pStr = mre_xml_get_attribute(pElem, "imsi_3gpp");
  if (pStr != NULL)
  {
    p3gppPlmn = mcfg_str_dup(pStr);    
  }

  pStr = mre_xml_get_attribute(pElem, "imsi_3gpp2");
  if (pStr != NULL)
  {
    p3gpp2Plmn = mcfg_str_dup(pStr);    
  }

  pStr = mre_xml_get_attribute(pElem, "gid1");
  if (pStr != NULL)
  {
    pGid1 = mcfg_str_dup(pStr);
  }

  pStr = mre_xml_get_attribute(pElem, "gid2");
  if (pStr != NULL)
  {
    pGid2 = mcfg_str_dup(pStr);
  }

  pAct = (mcfg_sel_record_action_s_type *) mre_action_new( sizeof(mcfg_sel_record_action_s_type),
                                                 pPolicy,
                                                 mcfg_sel_record_action_execute,
                                                 mcfg_sel_record_action_dtor, 
                                                 pElem
                                               );

  MCFG_MSG_MED_3("mcfg_sel_record_action_new(), carrierIndex=%d country=%d ims_feature=0x%02x", 
                 carrierIndex, country, ims_feature);

  pAct->carrierIndex = carrierIndex;
  pAct->pCarrierName = (char *)pCarrier;
  pAct->country = country;
  pAct->ims_feature = ims_feature;
  pAct->pIin         = (char *)pIin;
  pAct->p3gpp        = (char *)p3gppPlmn;
  pAct->p3gpp2       = (char *)p3gpp2Plmn;
  pAct->pGid1        = (char *)pGid1;
  pAct->pGid2        = (char *)pGid2;
  
  status = MRE_STATUS_SUCCESS;

  *ppAction = (mre_action_t *) pAct;

Done:
  MCFG_MSG_MED_2("mcfg_sel_record_action_new(), Done, carrierIndex %d, status %d",
                   carrierIndex, status);

  return status;
}

/*-------- mre_test_action_map --------*/
static mre_action_map_element_t   mcfg_action_map[] =
{
  {"SelRecord", mcfg_sel_record_action_new},
  {"PriorityOrder", mcfg_sel_db_priority_order_action_new},

  {NULL, NULL}
};

/*===========================================================================

FUNCTION  mcfg_create_new_policy() 

DESCRIPTION
  This function is required by mre_policy_create

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

mre_policy_t  *mcfg_create_new_policy(
    char const              *pPolicyName,
    mre_xml_element_t const *pPolicyElem
)
{
  mre_policy_t *pPolicy;
  // Special processing would go in this function if you wanted to expand upon the policy to add your own information into it
  
  pPolicy = (mre_policy_t *) mcfg_malloc(sizeof(*pPolicy));
  MCFG_CHECK_NULL_PTR_RET_NULL(pPolicy);
  ref_cnt_obj_init(pPolicy, mre_policy_dtor);

  return pPolicy;
}


/*===========================================================================

FUNCTION  mcfg_namespace_info_dtor() 

DESCRIPTION
  This function is required to destroy reference counted namespace

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

void mcfg_namespace_info_dtor(
  void *pObj
)
{
  mre_namespace_info_t *pNamespaceInfo = (mre_namespace_info_t *)pObj;
  size_t                subs;

  MCFG_CHECK_NULL_PTR_RET(pObj);

  for (subs = 0; subs < ARR_SIZE(pNamespaceInfo->perSubsNamedObjs); ++subs)
  {
    REF_CNT_OBJ_RELEASE_IF(pNamespaceInfo->perSubsNamedObjs[subs]);
  }

  REF_CNT_OBJ_RELEASE_IF(pNamespaceInfo->pGlobalNamedObjs);

  mre_mem_free(pNamespaceInfo);
}


/*===========================================================================

FUNCTION  mcfg_sel_db_init() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/

void mcfg_sel_db_init(void)
{
  mre_status_t   status;
  mre_policy_execute_ctx_t ctx;

  if (!pPolicy)
  {
    // Namespace initialization, policy creation and policy init need to be done prior to running the policy.  
    // The namespace is where the tristates your XML will be using are stored
    pNamespaceInfo = (mre_namespace_info_t *)mre_mem_alloc(sizeof(mre_namespace_info_t));
    MCFG_CHECK_NULL_PTR_RET(pNamespaceInfo);

    ref_cnt_obj_init(pNamespaceInfo, mcfg_namespace_info_dtor);

    pNamespaceInfo->defaultNS         = MRE_NAMESPACE_SUBS;
    pNamespaceInfo->pfnGetPolicySubs  = NULL;

    status = mre_policy_create( MCFG_XML_FILE,
                                SYS_MODEM_AS_ID_1,
                                mcfg_condition_map,
                                mcfg_action_map,
                                mcfg_create_new_policy,
                                MRE_CLIENT_MCFG,
                                &pPolicy );

    MCFG_CHECK_NULL_PTR_RET(pPolicy);
    if (MRE_STATUS_SUCCESS == status)
    {
      MCFG_MSG_MED("mcfg_sel_db_init() - call mre_policy_init()");
      mre_policy_init(pPolicy, pNamespaceInfo, &ctx);
    }
    else
    {
      MCFG_MSG_ERROR_1("mcfg_sel_db_init() FAILED, status =%d", status);
      pPolicy = NULL;
    }
  }
}

typedef struct
{
  uint8 carrier_index;  // expected carrier index

  char iin[MCFG_UIM_ICCID_STR_BUF_LEN];

  sys_mcc_type mcc;
  sys_mnc_type mnc;

  sys_mcc_type mcc2;
  sys_mnc_type mnc2;
} card_info_t;

#ifdef FEATURE_MCFG_UNIT_TESTING
/*===========================================================================

FUNCTION  mcfg_run_policy() 

DESCRIPTION
  

DEPENDENCIES
   

RETURN VALUE
  

SIDE EFFECTS
  None

===========================================================================*/
void mcfg_run_policy(mcfg_sub_id_type_e_type sub, uint8 test_no)
{
  uint8 i, start, end;
  mcfg_trl_carrier_mcc_mnc_s_type plmn;
  card_info_t card_info_list[] =
  {
    /*Defer*/
/*0*/    {255, "899186123456789"},             

    /*RJIL*/
/*1*/    {27, "8991840123456789", 405, 854},   
/*2*/    {27, "8991840123456789"},            
/*3*/    {27, "899186123456789", 405, 860},   

    /*Vodafone India*/
/*4*/    {4, "899184123456789"},             
/*5*/    {4, "899184123456789", 404,  84},   
/*6*/    {4, "899186123456789", 404,  86},   

    /*Vodafone Netherland */
/*7*/    {4, "893144123456789"},             
/*8*/    {4, "893144123456789", 204,   4},   

    /*CT*/
/*9*/    {19, "898603123456789"},             
/*10*/    {19, "898603123456789", 460,   3},   
/*11*/    {19, "898603123456789", 204,   4, 460,   3},
/*12*/    {19, "898603123456789",   0,   0, 460,   3},

    /*CMCC*/
/*13*/    {32, "898607123456789"},             
/*14*/    {32, "898607123456789", 460,   7},

    /*CU*/
/*15*/    {21, "898601123456789"},             
/*16*/    {21, "898601123456789", 460,   1},

    /*MyTest: CMCC CI-ICCID + CT IMSI*/
/*17*/    {32, "898212123456789", 460,   11},             
    /*MyTest: CT CI-ICCID + CMCC IMSI*/
/*18*/    {19, "8985302123456789", 454,   13},
    /*MyTest: CMCC first values*/
/*19*/    {32, "898607123456789", 460,   07},             
    /*MyTest: CMCC last values*/
/*20*/    {32, "898521123456789", 460,   04},
    /*MyTest: Mrudula Crash 1*/
/*21*/    {3, "89014100000392892254", 310, 410},
/*22*/    {1, "89148000000392892254", 311, 480},

  };

  start = 0;
  end = sizeof(card_info_list)/sizeof(card_info_t);
  if (test_no < end)
  {
    start = test_no;
    end = test_no+1;
  }

  for (i = start; i < end; i++)
  {
    MCFG_MSG_HIGH_1("=====Executing testcase %d=====", i);

    if (card_info_list[i].mcc)
    {
      mcfg_uim_update_iccid((mcfg_slot_index_type_e_type)sub, card_info_list[i].iin);

      plmn.mcc = card_info_list[i].mcc;
      plmn.mnc = card_info_list[i].mnc;

      MCFG_MSG_MED_3("Testcase %d select GW mcc/mnc %d-%02d", i, plmn.mcc, plmn.mnc);
      mcfg_sel_sw_by_plmn(sub, 
                          &plmn,
                          MCFG_UIM_EXT_SESSION_GW);
    }
    else if (card_info_list[i].mcc2)
    {
      mcfg_uim_update_iccid((mcfg_slot_index_type_e_type)sub, card_info_list[i].iin);

      plmn.mcc = card_info_list[i].mcc2;
      plmn.mnc = card_info_list[i].mnc2;

      MCFG_MSG_MED_3("Testcase %d select 1xmcc/mnc %d-%02d", i, plmn.mcc, plmn.mnc);
      mcfg_sel_sw_by_plmn(sub, 
                          &plmn,
                          MCFG_UIM_EXT_SESSION_1X);
    }
    else
    {
      MCFG_MSG_SPRINTF_2("Testcase %d with ICCID %s", i, card_info_list[i].iin);
      mcfg_sel_sw_by_iccid(sub, card_info_list[i].iin);
    }
  }
} /* mcfg_run_policy */
#endif /* FEATURE_MCFG_UNIT_TESTING */

/*===========================================================================
  
    FUNCTION mcfg_sel_db_release
  
    DESCRIPTION.
  
    DEPENDENCIES
      None
  
    PARAMETERS
      None
  
    RETURN VALUE
      None
  
    SIDE EFFECTS
      None
  
===========================================================================*/
void mcfg_sel_db_release(void )
{
  MCFG_MSG_MED("mcfg_sel_db_release(): release all mre objects");
      
  if(pPolicy != NULL)
  {
    ref_cnt_obj_release(pPolicy);
  }
  
  if(pNamespaceInfo != NULL)
  {
    ref_cnt_obj_release(pNamespaceInfo);
  }

  pPolicy = NULL;
} /* mcfg_sel_db_release */


/*===========================================================================
  
    FUNCTION mcfg_sel_db_get_carrier_index
  
    DESCRIPTION.
  
    DEPENDENCIES
      None
  
    PARAMETERS
      None
  
    RETURN VALUE
      None
  
    SIDE EFFECTS
      None
  
===========================================================================*/
boolean mcfg_sel_db_get_carrier_index
(
  mcfg_slot_index_type_e_type     sloti,
  pdc_selection_carrier_enum_v01 *carrier_index,
  mcfg_config_id_s_type          *config_id
)
{
  mcfg_uim_card_info_s_type  *p_card_info = NULL;
  int i;
  boolean result = FALSE;
  sys_plmn_id_s_type plmn, plmn2;
  uint16 mcc, mnc;
  mre_policy_execute_ctx_t ctx;
  mcfg_sub_id_type_e_type sub_id;
  mcfg_config_id_s_type active_config_id;

  MCFG_CHECK_NULL_PTR_RET_FALSE(config_id);
  MCFG_CHECK_NULL_PTR_RET_FALSE(carrier_index);
  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);

  sub_id = mcfg_uim_map_slot_index_to_sub_id(sloti);

  do
  {
    p_card_info = mcfg_uim_get_card_info_p(sloti);
    if (!p_card_info)
    {
      MCFG_MSG_ERROR_1("fail to get card info for sloti %d", sloti);
      break;
    }
    mcfg_sel_db_set_default_priority_order();
    mcfg_sel_db_init();
    if (pPolicy == NULL)
    {
      MCFG_MSG_ERROR("policy init fails");
      break;
    }

    pMcfgState = mcfg_malloc(sizeof(mcfg_mre_state_t));
    if (pMcfgState == NULL)
    {
      MCFG_MSG_ERROR("Couldn't allocate memory for pMcfgState!");
      break;
    }
    memset(pMcfgState, 0, sizeof(mcfg_mre_state_t));
    pMcfgState->sloti = sloti;
    
    pMcfgState->pPlmn = &plmn;
    pMcfgState->pPlmn3gpp2 = &plmn2;

    memset(&mcfg_selected_record, 0, sizeof(mcfg_selected_record));

    pMcfgState->have_iin = FALSE;
    if (strlen(p_card_info->ICCID)>0)
    {
      pMcfgState->have_iin = TRUE;
      memscpy(pMcfgState->iin, MCFG_UIM_ICCID_STR_BUF_LEN,
              p_card_info->ICCID, strlen(p_card_info->ICCID));
      MCFG_MSG_SPRINTF_1("Execute policy with IIN %s", pMcfgState->iin);
    }

    pMcfgState->have_gid[MCFG_GID_INDEX_1] = FALSE;
    if (strlen(p_card_info->group_id1)>0)
    {
      pMcfgState->have_gid[MCFG_GID_INDEX_1] = TRUE;
      memscpy(pMcfgState->gid[MCFG_GID_INDEX_1], MCFG_UIM_GID_STR_BUF_LEN,
              p_card_info->group_id1, strlen(p_card_info->group_id1));
      MCFG_MSG_SPRINTF_1("Execute policy with GID1 %s", pMcfgState->gid[MCFG_GID_INDEX_1]);
    }

    pMcfgState->have_gid[MCFG_GID_INDEX_2] = FALSE;
    if (strlen(p_card_info->group_id2)>0)
    {
      pMcfgState->have_gid[MCFG_GID_INDEX_2] = TRUE;
      memscpy(pMcfgState->gid[MCFG_GID_INDEX_2], MCFG_UIM_GID_STR_BUF_LEN,
              p_card_info->group_id2, strlen(p_card_info->group_id2));
      MCFG_MSG_SPRINTF_1("Execute policy with GID2 %s", pMcfgState->gid[MCFG_GID_INDEX_2]);
    }

    pMcfgState->have_plmn = FALSE;
    mcc = p_card_info->plmn[MCFG_UIM_EXT_SESSION_GW][0].mcc;
    mnc = p_card_info->plmn[MCFG_UIM_EXT_SESSION_GW][0].mnc;
    if (mcc > 0)
    {
      pMcfgState->have_plmn = TRUE;
      sys_plmn_set_mcc_mnc((mnc>=100), mcc, mnc, pMcfgState->pPlmn);
      MCFG_MSG_MED_2("Execute policy with mcc/mnc %d-%02d", mcc, mnc);
    }

    pMcfgState->have_plmn_3gpp2 = FALSE;
    mcc = p_card_info->plmn[MCFG_UIM_EXT_SESSION_1X][0].mcc;
    mnc = p_card_info->plmn[MCFG_UIM_EXT_SESSION_1X][0].mnc;
    if (mcc > 0)
    {
      pMcfgState->have_plmn_3gpp2 = TRUE;
      sys_plmn_set_mcc_mnc((mnc>=100), mcc, mnc, pMcfgState->pPlmn3gpp2);
      MCFG_MSG_MED_2("Execute policy with 1x mcc/mnc %d-%02d", mcc, mnc);
    }

    mre_policy_execute(pPolicy, &ctx);

    if (!mcfg_selected_record.carrierIndex)
    {
      MCFG_MSG_ERROR("Execute policy didn't find any match");
    }
    else if (p_card_info->carrier_index == mcfg_selected_record.carrierIndex &&
             p_card_info->country == mcfg_selected_record.country &&
             p_card_info->ims_feature == mcfg_selected_record.ims_feature &&
             mcfg_utils_get_active_config(MCFG_TYPE_SW, &active_config_id, sub_id) &&
             mcfg_sel_config_ids_match(&mcfg_selected_record.config_id, &active_config_id)
             )
    {
      MCFG_MSG_MED_1("Execute policy found same match for carrier index %d", mcfg_selected_record.carrierIndex);
    }
    else
    {
      MCFG_MSG_HIGH_2("Execute policy found new match for carrier index %lu, was %d", mcfg_selected_record.carrierIndex, p_card_info->carrier_index);

      if(p_card_info->country != mcfg_selected_record.country ||
         p_card_info->ims_feature != mcfg_selected_record.ims_feature)
      {
        p_card_info->country = mcfg_selected_record.country;
        p_card_info->ims_feature = mcfg_selected_record.ims_feature;
        mcfg_setting_update_feature(sloti, mcfg_selected_record.country,
                            (uint16)mcfg_selected_record.ims_feature);
      }

      p_card_info->carrier_index = mcfg_selected_record.carrierIndex;
      *carrier_index = mcfg_selected_record.carrierIndex;

      memscpy(&p_card_info->config_id, sizeof(mcfg_config_id_s_type),
              &mcfg_selected_record.config_id, sizeof(mcfg_config_id_s_type));
      mcfg_uim_update_card_info(sloti, p_card_info);
      memscpy(config_id, sizeof(mcfg_config_id_s_type), 
              &mcfg_selected_record.config_id, sizeof(mcfg_config_id_s_type));
      result = TRUE;
    }

  }while(0);

  if (pMcfgState)
  {
    mcfg_free(pMcfgState);
  }

  if (pPolicy != NULL)
  {
    mcfg_sel_db_release();
  }

  return result;
}
#endif /* FEATURE_MCFG_SEL_DB */
