/**
  @file mre_policy.c

  @brief Public interface to Policy Manager
*/

/*
    Copyright (c) 2013-2017,2020 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/mre_engine.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_dbg.h"
#include "mre_efs.h"
#include "mre_engine.h"
#include "mre_lang.h"
#include "mre_rules.h"
#include "mre_set.h"
#include "mre_util_i.h"
#include "mre_xml.h"

#include "sys_v.h"
#include <timetick.h>
#include <stringl/stringl.h>


/*-------- mre_policy_dtor --------*/
void mre_policy_dtor(
  void  *pObj
)
{
  mre_policy_t  *pPolicy = (mre_policy_t *) pObj;

  MRE_RELEASE_IF(pPolicy->pEhPlmnSet);
  MRE_RELEASE_IF(MRE_POLICY_PNAMEDOBJS(pPolicy));
  MRE_RELEASE_IF(MRE_POLICY_INITSET(pPolicy));
  MRE_RELEASE_IF(MRE_POLICY_ACTIONSET(pPolicy));

  mre_mem_free(pPolicy->pInfo);
  mre_mem_free(pPolicy);
}


/*-------- mre_policy_enable --------*/
void mre_policy_enable(
  mre_policy_t  *pPolicy,
  boolean        enable
)
{
  MRE_POLICY_ENABLED(pPolicy) = enable;
}


/*-------- mre_policy_is_enabled --------*/
boolean mre_policy_is_enabled(
  mre_policy_t  *pPolicy
)
{
  return MRE_POLICY_ENABLED(pPolicy);
}


/*-------- mre_policy_get_info --------*/
/**
@brief  Read the policy_info file for this policy, parse it, and return
        pointers to the parsed document tree.

@param[in]  pPath        Pointer to path to the policy XML file.
@param[in]  subsId       The context to read the policy file from (AS_ID)
@param[out] ppRoot       Pointer to storage for a pointer to the document root.

@return
  MRE_STATUS_SUCCESS if policy XML file was read and successfully parsed
  MRE_STATUS_ERR_NOT_PRESENT if policy XML was not found
  MRE_STATUS_ERR_MALFORMED_XML if policy XML was invalid
*/
static mre_status_t mre_policy_get_info(
  char const             *pPath,
  sys_modem_as_id_e_type  subsId,
  mre_xml_root_t const  **ppPolicyDoc
)
{
  size_t                   size;
  mre_efs_status_t         fs_status;
  mre_status_t             status;
  char const              *pPolicyInfo = NULL;
  mre_xml_root_t const    *pPolicyDoc  = NULL;
  mre_xml_element_t const *pElement;

  /*  Read the policy_info file for this policy.
   */
  fs_status = mre_efs_get_file( pPath,
                               (void **)&pPolicyInfo,
                               &size,
                               subsId );

  if (MRE_EFS_STATUS_SUCCESS != fs_status)
  {
    status = MRE_STATUS_ERR_NOT_PRESENT;
    goto Done;
  }

  /*  Parse the policy file into a document tree.
   */
  status = mre_xml_parse(pPolicyInfo, &pPolicyDoc);
  if (MRE_STATUS_SUCCESS != status)
  {
    MRE_MSG_ERROR_1("Failed to parse policy: %08x", status);
    goto Done;
  }

  /*  Check that the document has a root element.
   */
  pElement = mre_xml_get_root_element(pPolicyDoc);
  if (pElement == NULL)
  {
    status = MRE_STATUS_ERR_MALFORMED_XML;
    MRE_MSG_ERROR_0("Policy document does not have a root element");
    MRE_RELEASE_IF(pPolicyDoc);
  }

Done:
  MRE_MEM_FREE_IF(pPolicyInfo);

  *ppPolicyDoc = pPolicyDoc;

  return status;
}


/*-------- mre_policy_get_version --------*/
static void mre_policy_get_version(
  mre_policy_t            *pPolicy,
  mre_xml_element_t const *pElement
)
{
  char const  *pStr;
  char         token[8];

  /*  Find the version attribute; return if not present.
   */
  pStr = mre_xml_get_attribute(pElement, "version");
  if (NULL == pStr)
  {
      pStr = mre_xml_get_attribute(pElement, "policy_ver");
      if (pStr == NULL)
      {
        pStr = mre_xml_get_attribute(pElement, "mcfg_db_ver");
      }
  }
  if (pStr == NULL)
  {
    MRE_UT_MSG_0("no policy version specified");
    return;
  }

  /*  Get anything up to a '.' that separates major and minor versions.
   */
  if (!mre_get_token_ex(&pStr, token, sizeof(token), '.'))
  {
    return;
  }

  /*  Get the policy number
   */
  MRE_POLICY_NUMBER(pPolicy) = atoi(token);

  /*  Get the policy type
   */
  if (*pStr++ == '.' && mre_get_token_ex(&pStr, token, sizeof(token), '.'))
  {
    MRE_POLICY_TYPE(pPolicy) = atoi(token);
  }

  /*  Get the policy version
   */
  if (*pStr++ == '.' && mre_get_token(&pStr, token, sizeof(token)))
  {
    MRE_POLICY_VERSION(pPolicy) = atoi(token);
  }
}


/*-------- mre_policy_get_changelist --------*/
static void mre_policy_get_changelist(
  mre_policy_t            *pPolicy,
  mre_xml_element_t const *pElement
  )
{
  char const  *pStr;
  char         token[32];

  /*  Find the changelist attribute; return if not present.
   */
  pStr = mre_xml_get_attribute(pElement, "changelist");
  if (pStr == NULL)
  {
    MRE_UT_MSG_0("no changelist specified");
    return;
  }

  /*  Get anything up to a ':' that separates RCS keyword from actual CL number.
   */
  if (!mre_get_token_ex(&pStr, token, sizeof(token), ':'))
  {
    return;
  }

  /* Ensure returned token is $Change:
   */
  if (strcasecmp(token, "$Change") != 0)
  {
    MRE_MSG_ERROR_0("token returned is not a changelist");
    return;
  }

  /*  Get the changelist type
   */
  if (*pStr++ == ':')
  {
    MRE_POLICY_CHANGELIST(pPolicy) = atoi(pStr);
  }
}


/*-------- mre_is_node_initial --------*/
static boolean mre_is_node_initial(
  char const  *pStr
)
{
  return strcasecmp(pStr, "initial") == 0;
}


/*-------- policy_add_action --------*/
static mre_status_t policy_add_action(
  mre_xml_element_t const *pElement,
  void                    *pCtx
)
{
  mre_policy_t *pPolicy = (mre_policy_t *) pCtx;
  char const   *pStr;
  boolean       succeeded;

  pStr = mre_xml_get_tag(pElement);
  MRE_XML_MSG("processing %s", pStr, 0, 0);

  if (mre_is_node_initial(pStr))
  {
    succeeded = mre_initset_add(pPolicy, pElement);
  }
  else
  {
    mre_set_t *pActionSet;

    pActionSet = mre_ensure_actionset(pPolicy);
    succeeded = mre_rule_add_action_to_set(pElement, pPolicy, pActionSet);
  }

  return succeeded? MRE_STATUS_SUCCESS : MRE_STATUS_ERR_MALFORMED_XML;
}


/*-------- mre_policy_add_actions --------*/
static mre_status_t mre_policy_add_actions(
  mre_policy_t            *pPolicy,
  mre_xml_element_t const *pElement
)
{
  mre_status_t  status;

  MRE_XML_MSG("###################################################", 0, 0, 0);
  MRE_XML_MSG("adding actions to policy", 0, 0, 0);

  status = mre_xml_process_children(pElement, policy_add_action, pPolicy);

  if (MRE_SUCCEEDED(status))
  {
    MRE_XML_MSG("action setup succeeded", 0, 0, 0);
  }
  else
  {
    MRE_XML_MSG("action setup failed", 0, 0, 0);
  }
  MRE_XML_MSG("###################################################", 0, 0, 0);

  mre_policy_enable(pPolicy, MRE_SUCCEEDED(status));

  return status;
}


/*-------- mre_policy_set_execute_subs --------*/
static void mre_policy_set_execute_subs(
  mre_policy_t            *pPolicy,
  sys_modem_as_id_e_type   subsId
)
{
  POLICY_SUBS(pPolicy) = (subsId == SYS_MODEM_AS_ID_NONE) ? SYS_MODEM_AS_ID_1 : subsId;
}

/*-------- get_continue_by_default_override --------*/
STATIC void get_continue_by_default_override(
  mre_xml_element_t const *pPolicyElem,
  mre_policy_t            *pPolicy
)
{
  char const  *pContinueByDefault;
  boolean      continueByDefault;

  pContinueByDefault = mre_xml_get_attribute(pPolicyElem, "continueByDefault");
  if (NULL != pContinueByDefault)
  {
    if (MRE_SUCCEEDED(mre_str_to_boolean(pContinueByDefault, &continueByDefault)))
    {
      MRE_POLICY_CONTINUEBYDEFAULT(pPolicy) = continueByDefault;
    }
    else
    {
      MRE_UT_MSG_S1("Invalid value for continueByDefault attribute: %s", pContinueByDefault);
    }
  }
}

/*=============================================================================
  Public MRE Engine APIs
=============================================================================*/


/*-------- mre_policy_create_internal --------*/
STATIC mre_status_t mre_policy_create_internal(
  char                          const  *pPolicyPath,
  sys_modem_as_id_e_type                subsId,
  mre_condition_map_element_t   const  *pCondTable,
  mre_action_map_element_t      const  *pActionTable,
  boolean                               continueByDefault,
  pfnNewPolicy_t                        pfnNewPolicy,
  mre_client_t                          mreClient,
  mre_policy_t                        **ppPolicy
)
{
  mre_status_t             status;
  mre_xml_root_t const    *pPolicyDoc  = NULL;
  mre_xml_element_t const *pPolicyElem;
  mre_policy_t            *pPolicy     = NULL;
  char const              *pPolicyName;

  /*---------------------------------------------------------------------------
    Try loading and parsing the XML file
  ---------------------------------------------------------------------------*/
  /*  Try to load the given policy.
   */
  status = mre_policy_get_info( pPolicyPath,
                                subsId,
                                &pPolicyDoc );

  /*  If unable to load policy file return an error.
   */
  if (MRE_FAILED(status) || NULL == pPolicyDoc)
  {
    switch(status)
    {
      case MRE_STATUS_ERR_NOT_PRESENT:
        MRE_UT_MSG_S2( "subs %d: Policy file '%s' was not present in EFS",
                       subsId,
                       pPolicyPath );
        break;

      case MRE_STATUS_ERR_MALFORMED_XML:
        MRE_MSG_ERROR_1(CLIENT "Malformed XML in policy file", mreClient);
        MRE_UT_MSG_S2( "subs %d: Malformed policy file '%s'",
                       subsId,
                       pPolicyPath );

        break;

      default:
        status = MRE_STATUS_FAILED;
        MRE_MSG_ERROR_2(CLIENT "Unknown error %d in mre__parse_policy", mreClient, status);
        break;
    }
    goto Done;
  }

  /*---------------------------------------------------------------------------
    Call the client callback to create a policy.
  ---------------------------------------------------------------------------*/

  /*  Get a pointer to the actual policy node.
   */
  pPolicyElem = mre_xml_get_root_element(pPolicyDoc);

  /*  Create the policy.
   */
  pPolicyName = mre_xml_get_attribute(pPolicyElem, "name");
  pPolicy = pfnNewPolicy(pPolicyName, pPolicyElem);
  if (pPolicy == NULL)
  {
    MRE_MSG_ERROR_1(CLIENT "Unable to create policy from XML: pfnNewPolicy=0x%08x", pfnNewPolicy);
    status = MRE_STATUS_FAILED;
    goto Done;
  }

  /*---------------------------------------------------------------------------
    With policy in hand, set it up.
  ---------------------------------------------------------------------------*/
  mre_policy_set_execute_subs(pPolicy, subsId);

  POLICY_CLIENT(pPolicy) = mreClient;
  POLICY_PFN_NEW_POLICY(pPolicy) = pfnNewPolicy;

  MRE_POLICY_INFO(pPolicy) = mre_mem_alloc(sizeof(mre_policy_info_t));
  MRE_POLICY_CONDTBL(pPolicy) = pCondTable;
  MRE_POLICY_ACTTBL(pPolicy) = pActionTable;
  MRE_POLICY_CONTINUEBYDEFAULT(pPolicy) = continueByDefault;

  /*  Override the continueByDefault setting if it is set in XML
   */
  get_continue_by_default_override(pPolicyElem, pPolicy);

  /*  Get the version of the policy.
   */
  mre_policy_get_version(pPolicy, pPolicyElem);

  /*  Get the changelist of the policy.
   */
  mre_policy_get_changelist(pPolicy, pPolicyElem);

  /*  Add the rules to the policy.
   */
  status = mre_policy_add_actions(pPolicy, pPolicyElem);

Done:
  MRE_RELEASE_IF(pPolicyDoc);

  if (MRE_SUCCEEDED(status))
  {
    *ppPolicy = pPolicy;
  }
  else
  {
    MRE_RELEASE_IF(pPolicy);
  }

  return status;
}


/*-------- mre_policy_create_legacy --------*/
mre_status_t mre_policy_create_legacy(
  char                          const  *pPolicyPath,
  sys_modem_as_id_e_type                subsId,
  mre_condition_map_element_t   const  *pCondTable,
  mre_action_map_element_t      const  *pActionTable,
  pfnNewPolicy_t                        pfnNewPolicy,
  mre_policy_t                        **ppPolicy
)
{
  return mre_policy_create_internal(pPolicyPath,
                                    subsId,
                                    pCondTable,
                                    pActionTable,
                                    FALSE,
                                    pfnNewPolicy,
                                    MRE_CLIENT_POLICYMAN,
                                    ppPolicy);
}


/*-------- mre_policy_create --------*/
mre_status_t mre_policy_create(
  char                          const  *pPolicyPath,
  sys_modem_as_id_e_type                subsId,
  mre_condition_map_element_t   const  *pCondTable,
  mre_action_map_element_t      const  *pActionTable,
  pfnNewPolicy_t                        pfnNewPolicy,
  mre_client_t                          mreClient,
  mre_policy_t                        **ppPolicy
)
{
  return mre_policy_create_internal(pPolicyPath,
                                    subsId,
                                    pCondTable,
                                    pActionTable,
                                    TRUE,
                                    pfnNewPolicy,
                                    mreClient,
                                    ppPolicy);
}


/*-------- mre_policy_release_initset --------*/
void mre_policy_release_initset(
  mre_policy_t              *pPolicy
)
{
  MRE_RELEASE_IF(MRE_POLICY_INITSET(pPolicy));
}


/*-------- mre_policy_init_and_keep_initset --------*/
void mre_policy_init_and_keep_initset(
  mre_policy_t              *pPolicy,
  mre_namespace_info_t      *pNamespaceInfo,
  mre_policy_execute_ctx_t  *pCtx
)
{
  MRE_POLICY_NAMESPACE_INFO(pPolicy) = pNamespaceInfo;

  pPolicy->pEhPlmnSet = NULL;
  sys_plmn_undefine_plmn_id(&pPolicy->hplmn);

  if (NON_NULL(MRE_POLICY_INITSET(pPolicy)))
  {
    EXECUTE_CTX_POLICY(pCtx) = pPolicy;
    mre_actionset_execute(MRE_POLICY_INITSET(pPolicy), pCtx);
  }
}


/*-------- mre_policy_init --------*/
void mre_policy_init(
  mre_policy_t              *pPolicy,
  mre_namespace_info_t      *pNamespaceInfo,
  mre_policy_execute_ctx_t  *pCtx
)
{
  mre_policy_init_and_keep_initset(pPolicy,
                                   pNamespaceInfo,
                                   pCtx);

  mre_policy_release_initset(pPolicy);
}


/*-------- mre_policy_execute --------*/
void mre_policy_execute(
  mre_policy_t  *pPolicy,
  mre_policy_execute_ctx_t  *pCtx
)
{
  uint32   end_timetick;
  uint32   start_timetick;

  pPolicy->last_exec_time = mre_util_get_time();
  start_timetick = timetick_get();

  if (!MRE_POLICY_ENABLED(pPolicy))
  {
    MRE_MSG_ERROR_1(CLIENT "Policy not enabled due to parse/initialization failure", POLICY_CLIENT(pPolicy));
    goto Done;
  }

  if (NULL == MRE_POLICY_ACTIONSET(pPolicy))
  {
    MRE_MSG_ERROR_1(CLIENT "No actions for policy", POLICY_CLIENT(pPolicy));
    goto Done;
  }

  MRE_POLICY_SET_TOP_LEVEL(pPolicy);
  MRE_POLICY_CONTEVAL(pPolicy) = MRE_POLICY_CONTINUEBYDEFAULT(pPolicy);
  MRE_POLICY_STOPEVAL(pPolicy) = FALSE;

  EXECUTE_CTX_POLICY(pCtx) = pPolicy;

  mre_actionset_execute(MRE_POLICY_ACTIONSET(pPolicy), pCtx);

Done:
  end_timetick = timetick_get();
  pPolicy->elapsed_timeticks = timetick_diff(start_timetick, end_timetick, T_TICK);
}


/*-------- mre_policy_clone --------*/
mre_policy_t * mre_policy_clone(
  mre_policy_t          *pPolicy,
  mre_namespace_info_t  *pNamespaceInfoClone
)
{ 
  mre_policy_t      *pPolicyClone;
  mre_policy_info_t *pPolicyInfoClone;

   pPolicyClone     = pPolicy->pfnNewPolicy(POLICY_PNAME(pPolicy), NULL);
  *pPolicyClone     = *pPolicy;
   pPolicyInfoClone = (mre_policy_info_t *)mre_mem_alloc(sizeof(mre_policy_info_t));
  *pPolicyInfoClone = *MRE_POLICY_INFO(pPolicy);

  if (NON_NULL(MRE_POLICY_INITSET(pPolicy)))
  {
    ref_cnt_obj_add_ref((void *)MRE_POLICY_INITSET(pPolicy));
  }

  if (NON_NULL(MRE_POLICY_ACTIONSET(pPolicy)))
  {
    ref_cnt_obj_add_ref((void *)MRE_POLICY_ACTIONSET(pPolicy));
  }

  pPolicyInfoClone->pNamedObjs     = mre_named_objects_clone(MRE_POLICY_PNAMEDOBJS(pPolicy));
  pPolicyInfoClone->pNamespaceInfo = pNamespaceInfoClone;
  MRE_POLICY_INFO(pPolicyClone) = pPolicyInfoClone;

  if (NON_NULL(pPolicyClone->pEhPlmnSet))
  {
    ref_cnt_obj_add_ref(pPolicyClone->pEhPlmnSet);
  }

  return pPolicyClone;
}

/*-------- mre_policy_set_hplmn --------*/
mre_status_t mre_policy_set_hplmn(
  mre_policy_t       *pPolicy,
  mmgsdi_data_type   *pImsi,
  mmgsdi_data_type   *pAdData
)
{
  mre_status_t         status = MRE_STATUS_FAILED;
  sys_plmn_id_s_type   hplmn;
  uint8                num_mnc_digits;

  if ( NON_NULL(pPolicy)
      && sys_get_num_of_mnc_digits(pImsi, pAdData, &num_mnc_digits) )
  {
    mre_extract_plmn_from_imsi(*pImsi, num_mnc_digits, &hplmn);

    if ( !sys_plmn_id_is_undefined(hplmn)
       && sys_plmn_id_is_valid(hplmn) )
    {
      pPolicy->hplmn = hplmn;
      status = MRE_STATUS_SUCCESS;
    }

    MRE_MSG_HIGH_1("HPLMN updated in policy 0x%x", pPolicy);
  }

  return status;
}


/*-------- mre_policy_add_ehplmn_set --------*/
mre_status_t mre_policy_add_ehplmn_set(
  mre_policy_t  *pPolicy,
  mre_set_t     *pEhPlmnSet
)
{
  mre_status_t status = MRE_STATUS_SUCCESS;

  MRE_RELEASE_IF(pPolicy->pEhPlmnSet);
  if (NON_NULL(pEhPlmnSet))
  {
    ref_cnt_obj_add_ref(pEhPlmnSet);
    pPolicy->pEhPlmnSet = pEhPlmnSet;
  }

  MRE_MSG_HIGH_2( "EHPLMN set 0x%x added to the policy 0x%x", 
                  pEhPlmnSet,
                  pPolicy );

  return status;
}

/*-------- mre_policy_create_ehplmn_set --------*/
mre_set_t *mre_policy_create_ehplmn_set(
  mmgsdi_data_type   *pEhPlmnList
)
{
  mre_set_t    *pEhPlmnSet;

  size_t              index;
  size_t              plmnCount;
  sys_plmn_id_s_type *pPlmn;

  plmnCount = pEhPlmnList->data_len / sizeof(sys_plmn_id_s_type);
  pPlmn = (sys_plmn_id_s_type *)pEhPlmnList->data_ptr;

  pEhPlmnSet = mre_set_new( sizeof(sys_plmn_id_s_type),
                            5,
                            MRE_SET_NO_MAX_SIZE,
                            NULL,
                            NULL );

  for (index = 0; index < plmnCount; index++)
  {
    sys_mcc_type        mcc                    = 0xFF;
    sys_mnc_type        mnc                    = 0xFF;
    boolean             plmn_is_undefined      = FALSE;
    boolean             mnc_includes_pcs_digit = FALSE;

    sys_plmn_get_mcc_mnc(pPlmn[index], &plmn_is_undefined, &mnc_includes_pcs_digit, &mcc, &mnc);
    MRE_MSG_HIGH_5( "create_ehplmn_set : index %d, (MCC %d, MNC %d, pcs_incl %d, plmn_is_undefined %d)", 
                    index,
                    mcc,
                    mnc,
                    mnc_includes_pcs_digit,
                    plmn_is_undefined );

    if ( !sys_plmn_id_is_undefined(pPlmn[index]) 
       && sys_plmn_id_is_valid(pPlmn[index]) 
       )
    {
      mre_set_add(pEhPlmnSet, &pPlmn[index]);
    }
  }

  return pEhPlmnSet;
}

/*-------- mre_policy_delete_hplmn --------*/
void mre_policy_delete_hplmn(
  mre_policy_t *pPolicy
)
{
  sys_plmn_undefine_plmn_id(&pPolicy->hplmn);
}
