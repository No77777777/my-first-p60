/*!
  @file
  ds_3gpp_apn_table.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2010 - 2013 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/src/ds_3gpp_apn_table.c#3 $

when         who     what, where, why
--------     ---     --------------------------------------------------------
06/21/11     dvk     Fixed compilation warnings.
07/27/10     az      Initial version
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#ifdef FEATURE_DATA_3GPP
#include "ds_3gpp_apn_table.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "dsutil.h"
#include "ran.h"
#include "ds3gsubsmgr.h"
#include "ds3gmgr.h"
#include "ds_3gpp_nv_manager.h"

#include <stringl/stringl.h>
/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/

/*---------------------------------------------------------------------------
  APN table which stores the known PDN IP support returned by NW during
  bearer activation.  
  ref TS24.301 v810 6.2.2 
---------------------------------------------------------------------------*/
ds_pdn_context_apn_table_s  ds_apn_table[DS3GSUBSMGR_SUBS_ID_MAX] = {{{NULL}}};

#define DS_PDN_CTXT_INCREMENT_THROT_COUNTER(count) \
  {\
    if (count < 6)\
      count++;\
  }

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/
static boolean ds_apn_table_validate_index
(
  int index,
  ds3gsubsmgr_subs_id_e_type subs_index
);
static boolean ds_apn_table_alloc_dyn_mem
(
  int *alloc_index,
  sys_modem_as_id_e_type subs_id     
);

/*===========================================================================
FUNCTION DSPDNCNTX_GET_APN_IP_SUPPORT_INFO

DESCRIPTION
  This function searchs the APN table and returns the ip support type of a 
  particular APN

PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If APN table has an entry for this apn name
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_get_apn_ip_support_info 
(
  byte                      *apn,
  ds_apn_ip_support_type_e  *ip_support,
  sys_modem_as_id_e_type    subs_id
)
{
  int loop = 0;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();

  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
        (ds_apn_table_validate_index(loop, subs_index)) &&
        (
         (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                      ->apn, (char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)||
         ( 
          ((char)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->
           resolved_apn[0] != '\0') && 
          (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                ->resolved_apn, (char*)apn, DS_UMTS_MAX_APN_STRING_LEN+1) == 0)
         )
        )
       )  
    {
      *ip_support = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->ip_support;
      ds3gpp_leave_global_crit_section();
      return TRUE;
    }
  }

  ds3gpp_leave_global_crit_section();
  return FALSE;
}

/*===========================================================================
FUNCTION DSPDNCNTX_UPDATE_APN_TABLE_PDN_POINTER

DESCRIPTION
  This function updates the PDN context pointer field of the APN table based
  on IP support type

PARAMETERS   :
    
DEPENDENCIES
  Remember to call this function before you update the IP support of the 
  entry in the APN table. Should the order be reversed, APN table behaviour
  will be unpredictable.

RETURN VALUE
  TRUE - Successfully updated APn table
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
void ds_pdn_cntxt_update_apn_table_pdn_pointer
(
  int                       index,
  ds_apn_ip_support_type_e  ip_support,
  ds_pdn_context_s*         pdn_ptr
)
{
  ds_pdn_context_apn_table_dyn_s *apn_table_dyn_p = NULL;
  sys_modem_as_id_e_type          subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  // if the pdn_ptr is null, we do not need to update anything
  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_ptr, &subs_id)== FALSE)
  {
    return;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );

  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return;
  }

  if(ds_apn_table_validate_index(index, subs_index) == FALSE)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "APN table memory not allocated");
    return;
  }

  apn_table_dyn_p = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index];

  switch (ip_support)
  {
    case DS_IPV4_ONLY: //lint fallthrough
    case DS_IPV4_SINGLE_BEARER:
      if(apn_table_dyn_p->ip_support == DS_IPV4V6)
      {
        /*----------------------------------------------------------------------
          This is a transition from Dual IP to single IP or V4 only
          Clean up the other PDN context pointer
        ----------------------------------------------------------------------*/
        apn_table_dyn_p->v6_pdn_ptr = NULL;
      }
      else if((apn_table_dyn_p->ip_support == DS_SINGLE_BEARER) ||
              (apn_table_dyn_p->ip_support == DS_IP_TYPE_INVALID)||
              (apn_table_dyn_p->ip_support == DS_IPV4_ONLY))
      {
        /*----------------------------------------------------------------------
          Here we just want to add another PDN pointer to the APN table entry
        ----------------------------------------------------------------------*/
        apn_table_dyn_p->v4_pdn_ptr = pdn_ptr;

        DS_3GPP_MSG0_HIGH_EX(subs_index,"Debug: Add APN. V4 PDN added");
        DS_3GPP_MSG4_HIGH_EX(subs_index,"ip_support=%d, pdn ptr=0x%x, v4 ptr=0x%x, v6 ptr=0x%x",
                             ip_support,
                             pdn_ptr, 
                             apn_table_dyn_p->v4_pdn_ptr,
                             apn_table_dyn_p->v6_pdn_ptr);
      }
      break;
    case DS_IPV6_ONLY: //lint fallthrough
    case DS_IPV6_SINGLE_BEARER:
      if(apn_table_dyn_p->ip_support == DS_IPV4V6)
      {
        /*----------------------------------------------------------------------
          This is a transition from Dual IP to single IP or V6 only
          Clean up the other PDN context pointer
        ----------------------------------------------------------------------*/
        apn_table_dyn_p->v4_pdn_ptr = NULL;
      }
      else if((apn_table_dyn_p->ip_support == DS_SINGLE_BEARER) ||
              (apn_table_dyn_p->ip_support == DS_IP_TYPE_INVALID)||
              (apn_table_dyn_p->ip_support == DS_IPV6_ONLY))
      {
        /*----------------------------------------------------------------------
          Here we just want to add another PDN pointer to the APN table entry
        ----------------------------------------------------------------------*/

        DS_3GPP_MSG0_HIGH_EX(subs_index,"Debug: Add APN. V6 PDN added");
        apn_table_dyn_p->v6_pdn_ptr = pdn_ptr;

        DS_3GPP_MSG4_HIGH_EX(subs_index,"ip_support=%d, pdn ptr=0x%x, v4 ptr=0x%x, v6 ptr=0x%x",
                             ip_support,
                             pdn_ptr, 
                             apn_table_dyn_p->v4_pdn_ptr, 
                             apn_table_dyn_p->v6_pdn_ptr);

      }
      break;
    case DS_IPV4V6:
      apn_table_dyn_p->v4_pdn_ptr = pdn_ptr;
      apn_table_dyn_p->v6_pdn_ptr = pdn_ptr;
      break;
    default:
      DS_3GPP_ERROR_FATAL("Unknown PDP type.");
  }
} /* ds_pdn_cntxt_update_apn_table_pdn_pointer */

/*===========================================================================
FUNCTION ds_apn_table_fill_info

DESCRIPTION
  This function fills the apn table with the provided info at the provided 
  index

PARAMETERS   :
    
DEPENDENCIES

RETURN VALUE
  TRUE - Successfully updated APN table
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static boolean ds_apn_table_fill_info
(
  int                       index,
  byte*                     apn,
  ds_apn_ip_support_type_e  ip_support,
  ds_pdn_context_s*         pdn_ptr
)
{
  ds_pdn_context_apn_table_dyn_s *apn_table_dyn_p = NULL;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( index < 0 || index >= DS_MAX_APN_NUM )
  {
    DS_3GPP_ERROR_LOG_0("APN index out of bounds!");
    return FALSE;
  }

  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_ptr, &subs_id)== FALSE)
  {              
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );

  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  if(ds_apn_table_validate_index(index, subs_index) == FALSE)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "APN table memory not allocated");
    return FALSE;
  }

  apn_table_dyn_p = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index];

  /*---------------------------------------------------------------------- 
    For fresh entry check if both apn and resolved apn are NULL strings
    If they are null strings the assumption is it is a fresh addition.
    Even if there exists a call with both apn and resolved as NULL, we
    overwrite NULL to apn.
   ---------------------------------------------------------------------*/
  if ((apn_table_dyn_p->apn[0] == '\0') && 
      (apn_table_dyn_p->resolved_apn[0] == '\0'))
  {
    memscpy(apn_table_dyn_p->apn, (DS_UMTS_MAX_APN_STRING_LEN+1),
            apn,(DS_UMTS_MAX_APN_STRING_LEN+1));
  }

  ds_pdn_cntxt_update_apn_table_pdn_pointer(index, ip_support, pdn_ptr);

  switch(ip_support)
  {
    case DS_IPV4_SINGLE_BEARER: // lint fallthrough
    case DS_IPV6_SINGLE_BEARER:
  case DS_SINGLE_BEARER:
      apn_table_dyn_p->ip_support = DS_SINGLE_BEARER;
      break;
    default:
      apn_table_dyn_p->ip_support = ip_support;
      break;
  }

  DS_3GPP_MSG1_HIGH_EX(subs_index,"Debug: Add APN. IP Support added: %d",
                                   apn_table_dyn_p->ip_support);

  return TRUE;
}

/*===========================================================================
FUNCTION DSPDNCNTX_UPDATE_APN_TABLE

DESCRIPTION
  This function updates the APN table with the supplied info

PARAMETERS   :
    
DEPENDENCIES
  Remember to call update_pdn_ptr fn before you update the IP support of the 
  entry in the APN table. Should the order be reversed, APN table behaviour
  will be unpredictable.

RETURN VALUE
  TRUE - Successfully updated APN table
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_update_apn_table
(
  byte                      *apn,
  ds_apn_ip_support_type_e  ip_support,
  ds_pdn_context_s*         pdn_ptr
)
{
  int update_index = -1;
  int loop = 0;
  boolean ret_val = FALSE;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;

  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_ptr, &subs_id)== FALSE)
  {
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();
  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
         (ds_apn_table_validate_index(loop, subs_index)) &&
        (
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
            ->v4_pdn_ptr == pdn_ptr) ||
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
            ->v6_pdn_ptr == pdn_ptr)
         )
       )
    {
      update_index = loop;
    }
  }

  if (update_index >= 0 ) 
  {
     DS_3GPP_MSG2_HIGH_EX(subs_id,"Update APN table : ip_support %d, pdn_ptr 0x%x",
                          ip_support,
                          pdn_ptr);

    ret_val = ds_apn_table_fill_info(update_index, apn, 
                                       ip_support, pdn_ptr);
  }
  else
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "Update Fail. No matching PDN for x in APN table.",
                        pdn_ptr);
  }

  ds3gpp_leave_global_crit_section();
  return ret_val;
} /* ds_pdn_cntxt_update_apn_table */


/*===========================================================================
FUNCTION DSPDNCNTX_REMOVE_ENTRY_IN_APN_TABLE

DESCRIPTION
  This function removes the corresponding entry in the APN table for a
  specific APN

PARAMETERS   :
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Successfully removed this entry
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_remove_entry_in_apn_table
(
  ds_pdn_context_s   *incoming_pdn_cntx_p
)
{
  int                    loop = 0;
  boolean                ret_val = DS3G_FAILURE;
  boolean                clear_table_entry = FALSE;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(incoming_pdn_cntx_p, &subs_id)== FALSE)
  {
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }
  
  ds3gpp_enter_global_crit_section();

  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
         (ds_apn_table_validate_index(loop, subs_index)) &&
	 (
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
            == incoming_pdn_cntx_p) ||
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr 
            == incoming_pdn_cntx_p)
         )
       )
    {
      switch(ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->ip_support)
      {
        case DS_IPV4V6:
          if((ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
              == incoming_pdn_cntx_p) &&
             (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr 
              == incoming_pdn_cntx_p))
          {
            clear_table_entry = TRUE;
          }
          break;

         case DS_IPV4_ONLY:
         case DS_IPV6_ONLY:
         case DS_SINGLE_BEARER:
          /*--------------------------------------------------------------------
            Determine if the APN table has two PDN context pointers associated
            with it. If so, just clean up the incoming PDN context pointer

            If there is only one PDN context pointer present in the APN table
            entry, mark this entry for clearing.
           
                                      In some cases like (IPV6 RS/RA timeout),
            the IP support is moved from DS_SINGLE_BEARER to DS_IPV4_ONLY (or)
            DS_IPV6_ONLY. Hence, it may be possible that V4 and V6 PDN contexts
            may exist and are different
          --------------------------------------------------------------------*/
          if ((ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
               == incoming_pdn_cntx_p) &&
              (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr
                != NULL))
          {
            ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr = NULL;
          }
          else if ((ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr 
                    == incoming_pdn_cntx_p) &&
                   (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
                    != NULL))
          {
            ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr = NULL;
          }
          else
          {
            clear_table_entry = TRUE;
          }
          break;
        default:
          DS_3GPP_ERROR_LOG_3_EX(subs_id, "APN table cannot be in this state: d", 
                ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->ip_support,0,0);
          break;

      } // switch based on the IP support

      if(clear_table_entry == TRUE)
      {
        /* we can only remove this entry if tcm ptr is NULL, otherwise we need
           to keep the throttling info */
        ds_apn_table_free_dyn_mem(loop, subs_id);
      }

      ret_val = DS3G_SUCCESS;

      //Break from the loop
      break;
    }// Found entry in the APN table

  } //For loop to loop over all entries in the APN table.

  ds3gpp_leave_global_crit_section();

  if(ret_val == DS3G_FAILURE)
  {
    /* We reach here if the APN name is not found in the table */
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "APN entry not found in apn table!");
  }

  return ret_val;
} /* ds_pdn_cntxt_remove_entry_in_apn_table */

/*===========================================================================
FUNCTION DSPDNCNTX_IS_PDN_ASSOCIATED_FOR_APN

DESCRIPTION
  This function searchs the APN table and returns whether pdn context exists
  for a given apn

PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If there is a pdn context associated with this apn
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_is_pdn_associated_for_apn
(
  byte                      *apn,
  sys_modem_as_id_e_type     subs_id
)
{
  int loop = 0;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();

  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
        (ds_apn_table_validate_index(loop, subs_index)) &&
        (
         (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                      ->apn, (char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)||
         ( 
          ((char)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn[0]
            != '\0') && 
          (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                      ->resolved_apn, (char*)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)
         )
        )
       )
    {
      if ((ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr != NULL) ||
          (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr != NULL))
      {
        ds3gpp_leave_global_crit_section();
        return TRUE;
      }
    }
  }

  ds3gpp_leave_global_crit_section();
  return FALSE;
}

/*===========================================================================
FUNCTION DS_PDN_CNTX_GET_PS_IFACE_FOR_APN

DESCRIPTION
  This function searchs the APN table and returns the v4 or v6 ps iface 
  for a given apn.

PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If there is a ps iface associated with this apn
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
ps_iface_type* ds_pdn_cntxt_get_ps_iface_for_apn
(
  byte                      *apn,
  sys_modem_as_id_e_type     subs_id
)
{
   int loop = 0;
   ds_pdn_context_s     *pdn_ptr = NULL;
   ps_iface_type        *ps_iface_ptr = NULL;

   ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
   subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );


   if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
   {
     return ps_iface_ptr;
   }

   ds3gpp_enter_global_crit_section();
   for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
   {
     if((ds_apn_table_validate_index(loop, subs_index)) &&
         ((strncasecmp((char*)ds_apn_table[subs_index].
              ds_pdn_context_apn_table_dyn_p[loop]->apn, 
              (char *)apn, DS_UMTS_MAX_APN_STRING_LEN+1) == 0)|| 
          (strncasecmp((char*)ds_apn_table[subs_index].
               ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn, 
              (char*)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0))
       )
       {
         if ((ds_apn_table[subs_index].
                ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr != NULL) || 
             (ds_apn_table[subs_index].
                 ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr != NULL))
         {
           if (ds_apn_table[subs_index].
                 ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr != NULL) 
           {
             pdn_ptr= ds_apn_table[subs_index].
                        ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr;
             ps_iface_ptr = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_ptr);
           }
           else if (ds_apn_table[subs_index].
                      ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr != NULL) 
           {
             pdn_ptr= ds_apn_table[subs_index].
                       ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr;
             ps_iface_ptr = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_ptr);
           }
           break;
       }
     }
   }

   ds3gpp_leave_global_crit_section();
   return ps_iface_ptr;

}
/*===========================================================================
FUNCTION DS_PDN_CNTX_GET_PS_IFACE_FOR_APN

DESCRIPTION
  This function searchs the APN table and returns the v4 or v6 ps iface 
  for a given apn

PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If there is a ps iface associated with this apn
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
void ds_pdn_cntxt_get_resolved_apn_for_apn
(
  byte                       *apn,
  byte                       *resolved_apn,
  sys_modem_as_id_e_type      subs_id
)
{
  int loop = 0;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);

  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return;
  }

  ds3gpp_enter_global_crit_section();

  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if((ds_apn_table_validate_index(loop, subs_index)) &&
        ((strncasecmp((char*)ds_apn_table[subs_index].
              ds_pdn_context_apn_table_dyn_p[loop]->apn, 
              (char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)|| 
         (strncasecmp((char*)ds_apn_table[subs_index].
               ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn, 
              (char*)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)))
    {
      if(strncasecmp((char*)ds_apn_table[subs_index].
                         ds_pdn_context_apn_table_dyn_p[loop]->apn, 
                     (char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0) 
      {
        if((char)ds_apn_table[subs_index].
              ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn[0] != '\0')
        {
             memscpy(resolved_apn, 
                     (DS_UMTS_MAX_APN_STRING_LEN+1),
                      ds_apn_table[subs_index].
                        ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn,
                      (DS_UMTS_MAX_APN_STRING_LEN+1));  
             break;
        }
      }
      else
      {
        if(strncasecmp((char*)ds_apn_table[subs_index].
                         ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn, 
                       (char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0) 
        {
             memscpy(resolved_apn,(DS_UMTS_MAX_APN_STRING_LEN+1),
                      ds_apn_table[subs_index].
                        ds_pdn_context_apn_table_dyn_p[loop]->apn,
                     (DS_UMTS_MAX_APN_STRING_LEN+1));  
             break;
        }
      }
    }
  }

  /*copy the passed apn name to resolved apn; if there does not exist an entry*/
  if (loop == DS_MAX_APN_NUM ) 
  {
    memscpy(resolved_apn, (DS_UMTS_MAX_APN_STRING_LEN+1),
            apn,(DS_UMTS_MAX_APN_STRING_LEN+1));  
  }

  ds3gpp_leave_global_crit_section();
  return;

}

/*===========================================================================
FUNCTION DSPDNCNTX_GET_IP_SUPPORT_INFO_FOR_PDN

DESCRIPTION
  This function searchs the APN table and returns the ip support type of a 
  particular PDN. Note that DS_IPV4V6_SINGLE_BEARER is not a valid support
  type for a particular PDN since there would be 2 PDN connectivity to the 
  same APN and this particular PDN can only be one of them.

PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If APN table has an entry for this pdn
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_get_ip_support_info_for_pdn
(
  ds_pdn_context_s          *pdn_cntx_ptr,
  ds_apn_ip_support_type_e  *ip_support
)
{
  int                    loop = 0;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;

 if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_ptr, &subs_id)== FALSE)
  {
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }
  
  ds3gpp_enter_global_crit_section();
  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if(
        (ds_apn_table_validate_index(loop, subs_index)) &&
        (
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
            == pdn_cntx_ptr) ||
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr 
            == pdn_cntx_ptr)
        )
      )
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id,"Debug: Main loop=%d", loop);
      /* in the single bearer v4v6 case, we need to find out this pdn is
         v4 or v6
      */
      if (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->ip_support 
          == DS_SINGLE_BEARER)
      {
        DS_3GPP_MSG2_HIGH_EX(subs_id,"Debug: v4 ptr=0x%x, v6 ptr=0x%x", 
                            ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr,
                            ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr);

        if (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
            == pdn_cntx_ptr)
        {
          DS_3GPP_MSG0_HIGH_EX(subs_id,"Debug: DS_IPV4_SINGLE_BEARER");
          *ip_support = DS_IPV4_SINGLE_BEARER;
        }
        else if (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr
                  == pdn_cntx_ptr)
        {
          DS_3GPP_MSG0_HIGH_EX(subs_id,"Debug: DS_IPV6_SINGLE_BEARER");
          *ip_support = DS_IPV6_SINGLE_BEARER;
        }
        else
        {
          DS_3GPP_ERROR_LOG_3_EX(subs_id, "PDN pointer does not match either v4 or v6 in APN table",
                    0,0,0);
          ds3gpp_leave_global_crit_section();
          return FALSE;
        }

      }
      else
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id,"Debug: else ip_support=%d", 
                            ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->ip_support);
        *ip_support = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->ip_support;
      }
      ds3gpp_leave_global_crit_section();
      return TRUE;
    }
  }
  ds3gpp_leave_global_crit_section();
  return FALSE;
}


/*===========================================================================
FUNCTION DSPDNCNTX_GET_PDN_CONTEXT_IP_SUPPORT

DESCRIPTION
  This function gets the ip support type of the given pdn context pointer

PARAMETERS   
    
DEPENDENCIES
  None.

RETURN VALUE
  IP support type of the pdn context

SIDE EFFECTS
  None

===========================================================================*/
ds_umts_pdp_type_enum_type ds_pdn_cntxt_get_pdn_context_ip_support
(
  ds_pdn_context_s*           pdn_cntx_p
)
{
  ds_apn_ip_support_type_e   ip_support;
  ds_umts_pdp_type_enum_type pdp_support_type = DS_UMTS_PDP_MAX;  
  sys_modem_as_id_e_type     subs_id = SYS_MODEM_AS_ID_NONE;

  ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id);

#ifdef FEATURE_EMERGENCY_PDN
  if (DS_3GPP_PDN_SUPPORTS_EMERGENCY_SERVICES(pdn_cntx_p))
  {
    if (pdn_cntx_p->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type == DS_UMTS_PDP_IPV4V6)
    {
      //If both the v4 and v6 ifaces are bound to the PDN; ip_support is v4v6.
      if (pdn_cntx_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE] &&
          (pdn_cntx_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]))
      {
        return DS_UMTS_PDP_IPV4V6;
      }
      //One of the IFaces could have been disassociated from the PDN Context
      //if the NW responded with an error during PDN Connectivity Request.
      else if (pdn_cntx_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE])
      {
        return DS_UMTS_PDP_IPV4;
      }
      else if (pdn_cntx_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE])
      {
        return DS_UMTS_PDP_IPV6;
      }
      else 
      {
        DS3GPP_ASSERT(0);
      }
    }
    else 
    {
      return pdn_cntx_p->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;
    }
  }
#endif /* FEATURE_EMERGENCY_PDN */

  if (ds_pdn_cntxt_get_ip_support_info_for_pdn(pdn_cntx_p, &ip_support) != TRUE)
  {
    /*-------------------------------------------------------------------
      If there is no entry for this apn in the apn table, there is an
      error in apn table
    -------------------------------------------------------------------*/
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "APN in pdn context: x is not present in APN table.",
              pdn_cntx_p);
    DS3GPP_ASSERT(0);
  }
  /* */
  switch (ip_support)
  {
    case DS_IPV4_ONLY: //lint fallthrough
    case DS_IPV4_SINGLE_BEARER:
      pdp_support_type =  DS_UMTS_PDP_IPV4;
      break;
    case DS_IPV6_ONLY: //lint fallthrough
    case DS_IPV6_SINGLE_BEARER:
      pdp_support_type = DS_UMTS_PDP_IPV6;
      break;
    case DS_IPV4V6:
      pdp_support_type =  DS_UMTS_PDP_IPV4V6;
      break;
    default:
      DS_3GPP_ERROR_FATAL("Unknown PDP type.");
  }
  return pdp_support_type;
}

/*===========================================================================
FUNCTION ds_pdn_cntxt_get_v4_and_v6_pdn_context

DESCRIPTION
  This function returns the v4 and v6 PDN pointers corresponding to 
  any given PDN entry in APN table. One(or both) of the PDN returned could be 
  same as the input PDN. This API is used to clearly diff bet v4 and v6 PDN.

PARAMETERS   :
  input_pdn_cntx_p*                 - input PDN pointer 
  v4_pdn_cntx_p** (OUT)-               Pointer to the v4 context 
  v6_pdn_cntx_p** (OUT)-               Pointer to the v6 context
      
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If APN table has an entry for this apn name
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_get_v4_and_v6_pdn_context
(
  ds_pdn_context_s          *input_pdn_cntx_p,
  ds_pdn_context_s          **v4_pdn_cntx_p,
  ds_pdn_context_s          **v6_pdn_cntx_p
)
{
  int                    loop = 0;
  boolean                retval = DS3G_FAILURE;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(input_pdn_cntx_p, &subs_id)== FALSE)
  {
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();
  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
        (ds_apn_table_validate_index(loop, subs_index)) &&
        (
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr == 
            input_pdn_cntx_p) ||
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr == 
            input_pdn_cntx_p)
        )
       )
    {
      *v4_pdn_cntx_p = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                                                            ->v4_pdn_ptr;
      *v6_pdn_cntx_p = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                                                            ->v6_pdn_ptr;
      retval = DS3G_SUCCESS;
      break;
    }
  }
  ds3gpp_leave_global_crit_section();
  return retval;
}


/*===========================================================================
FUNCTION DSPDNCNTX_GET_V4_PDN_CONTEXT

DESCRIPTION
  This function returns the pointer to the V4 PDN context

PARAMETERS   :
  byte*              - APN name
  ds_pdn_context_s** - Pointer to the PDN context
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If APN table has an entry for this apn name
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_get_v4_pdn_context
(
  byte                      *apn,
  ds_pdn_context_s          **pdn_cntx_p,
  sys_modem_as_id_e_type    subs_id
)
{
  int loop = 0;
  boolean retval = DS3G_FAILURE;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();
  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
        (ds_apn_table_validate_index(loop, subs_index)) &&
        (
         (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                      ->apn,(char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)||
         ( 
          ((char)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->
           resolved_apn[0] != '\0') && 
          (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
          ->resolved_apn, (char*)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)
         )
        )
       )
    {
      *pdn_cntx_p = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr;
      retval = DS3G_SUCCESS;
      break;
    }
  }
  ds3gpp_leave_global_crit_section();
  return retval;
} /* ds_pdn_cntxt_get_v4_pdn_context */

/*===========================================================================
FUNCTION DSPDNCNTX_GET_V6_PDN_CONTEXT

DESCRIPTION
  This function returns the pointer to the V6 PDN context

PARAMETERS   :
  byte*              - APN name
  ds_pdn_context_s** - Pointer to the PDN context
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If APN table has an entry for this apn name
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_get_v6_pdn_context
(
  byte                      *apn,
  ds_pdn_context_s          **pdn_cntx_p,
  sys_modem_as_id_e_type    subs_id
)
{  
  int loop = 0;
  boolean retval = DS3G_FAILURE;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();
  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
        (ds_apn_table_validate_index(loop, subs_index)) &&
        (
         (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
                      ->apn, (char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)||
         ( 
          ((char)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->
           resolved_apn[0] != '\0') && 
          (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
           ->resolved_apn, (char*)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0)
         )
        )
       )
    {
      *pdn_cntx_p = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr;
      retval = DS3G_SUCCESS;
      break;
    }
  }
  ds3gpp_leave_global_crit_section();
  return retval;
} /* ds_pdn_cntxt_get_v6_pdn_context */


/*===========================================================================
FUNCTION DS_PDN_CONTXT_UPDATE_ALLOWED_IP_TYPE

DESCRIPTION
  This function blocks a specific IP type.
  1. If the IP_type becomes unavailable due to address configuration failure.
     In this case IP support will change from V4V6 to V4_ONLY or V6_ONLY.
  2. If the profile is configured single IP but OTA request is V4V6
     In this case IP support will change from SINGLE_BEARER to V4_ONLY or
     V6_ONLY.
  This function should not be called to change the the ip_support from one
  IP type to another V6 ex of unsupported transition (V4_Single_bearer to
  V6_Single_bearer or V6_ONLY)
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_pdn_cntxt_update_allowed_ip_type
(
  ds_pdn_context_s        *pdn_context_ptr,
  ds_apn_ip_support_type_e allowed_ip_type
)
{
  byte                     *apn = NULL;
  ds_pdn_context_s         *v4_pdn_cntx_p = NULL;
  ds_pdn_context_s         *v6_pdn_cntx_p = NULL;
  ds_apn_ip_support_type_e ip_support = DS_IP_TYPE_INVALID;
  boolean                   update_allowed = FALSE;
  ds_pdn_context_s         *update_pdn_cntx_p = NULL;
  sys_modem_as_id_e_type    subs_id = SYS_MODEM_AS_ID_NONE;  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_context_ptr,&subs_id))
  {
    /*------------------------------------------------------------------------
      Get the APN name from the PDN context whose connection was rejected
    ------------------------------------------------------------------------*/
    apn = pdn_context_ptr->ds_pdn_context_dyn_p->pdp_profile.context.apn;

    if(ds_pdn_cntxt_get_ip_support_info_for_pdn(pdn_context_ptr, &ip_support)
                                                == DS3G_SUCCESS)
    {
      /*----------------------------------------------------------------------
        Get the V6 and the V4 PDN context pointers.
      ----------------------------------------------------------------------*/
      if(ds_pdn_cntxt_get_v4_and_v6_pdn_context(pdn_context_ptr, 
                                                &v4_pdn_cntx_p,
                                                &v6_pdn_cntx_p
                                                ) != DS3G_SUCCESS)
      {
        DS_3GPP_ERROR_LOG_0_EX(subs_id,
                               "APN tbl entry not found. Trying to get V4/V6 PDN ptr");
      }

      if (allowed_ip_type != DS_IPV4_ONLY && allowed_ip_type != DS_IPV6_ONLY)
      {
        DS_3GPP_ERROR_LOG_1_EX(subs_id,
                               "No ip support update for d which is not V4/V6 only",
                               allowed_ip_type);
        return;
      }

      // Update APN IP type with the IP type being passed 
      switch (ip_support)
        {
        case DS_IPV4V6:
          /*-------------- ---------------------------------------------------
            Update allowed ip type - v4 and v6 pdn ptrs should be the same
          -------------------------------------------------------------------*/
          update_allowed = ((v4_pdn_cntx_p != NULL) &&
                            (v4_pdn_cntx_p == v6_pdn_cntx_p));
          update_pdn_cntx_p = v4_pdn_cntx_p;
          break;

        case DS_IPV4_SINGLE_BEARER:
          /*------------------------------------------------------------------
            IPV4_ONLY should be updated only when V6 PDN does not exist, if V6
            PDN ptr exists then there should be no operation and IP type should
            remain as IP_V4_SINGLE_BEARER
          -------------------------------------------------------------------*/
          update_allowed = ((v4_pdn_cntx_p != NULL) &&
                            (allowed_ip_type == DS_IPV4_ONLY) &&
                            (v6_pdn_cntx_p == NULL) );
          update_pdn_cntx_p = v4_pdn_cntx_p;
          break;

        case DS_IPV6_SINGLE_BEARER:
           /*------------------------------------------------------------------
             update per allowed ip type, v6 pdn ptr should not be NULL
           -------------------------------------------------------------------*/
          update_allowed = ((v6_pdn_cntx_p != NULL) &&
                            (allowed_ip_type == DS_IPV6_ONLY)&&
                            (v4_pdn_cntx_p == NULL));
          update_pdn_cntx_p = v6_pdn_cntx_p;
          break;

        case DS_IPV4_ONLY:
        case DS_IPV6_ONLY:
        default:
         //ignore
          break;
        }

      if(update_allowed == TRUE)
        {
        (void)ds_pdn_cntxt_update_apn_table(apn,
                                            allowed_ip_type,
                                            update_pdn_cntx_p);

        DS_3GPP_MSG1_HIGH("IP_Support update to %d",allowed_ip_type);
        }
      else
      {
        DS_3GPP_MSG2_HIGH("No change: IP_Support from %d to %d",
                            ip_support,
                            allowed_ip_type);
        DS_3GPP_MSG2_HIGH("With PDN ptrs 0x%x 0x%x",
                            v4_pdn_cntx_p,
                            v6_pdn_cntx_p);
      }      
      
      DS_3GPP_MSG5_HIGH_EX(subs_id,
                        "update allowed [%d] for IP_Support update from %d "
                        "to %d, with pdn ptrs v4 0x%x v6 0x%x ",
                        update_allowed, ip_support, allowed_ip_type,
                        v4_pdn_cntx_p, v6_pdn_cntx_p);
    } // Found the APN entry in the table
  } // incoming param non NULL
}

/*===========================================================================
FUNCTION DS_PDN_CONTXT_UPDATE_RESOLVED_APN_NAME

DESCRIPTION
  This function updates an entry in the APN table with the resolved APN name.
  This is used only if the APN name in the profile was NULL

PARAMETERS   :
  pdn_cntx_ptr* - Pointer to PDN context
  byte* - APN name as returned by the network

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if the entry corresponding to the profile APN name was found
  FALSE - if otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_pdn_cntxt_update_resolved_apn_name 
(
  ds_pdn_context_s* pdn_cntx_ptr,
  byte* resolved_apn_name
)
{
  int                    loop = 0;
  boolean                ret_val = FALSE;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_ptr, &subs_id)== FALSE)
  {
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();

  for(loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if ( (ds_apn_table_validate_index(loop, subs_index)) &&
           ((ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
             == pdn_cntx_ptr) ||
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr 
            == pdn_cntx_ptr)) )
                                                                          
    {
      DS_3GPP_C_MSG0(DS_3GPP_F3_MASK, "APN tbl entry found. Updating resolved name"); 
      (void)strlcpy((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
       ->resolved_apn,(char*)resolved_apn_name,DS_UMTS_MAX_APN_STRING_LEN+1);
      ret_val = TRUE;
      break;
    }
  }

  ds3gpp_leave_global_crit_section();
  return ret_val;
} /* ds_pdn_cntxt_update_resolved_apn_name */

/*==============================================================================
FUNCTION DS_PDN_CONTXT_GET_RESOLVED_APN_NAME

DESCRIPTION
  This function is used to return the resolved name given 
  the pdn context

PARAMETERS   :

  IN
  pdn_cntx_ptr* - Pointer to PDN context

  OUT
  byte** - Resolved APN name as returned by the network

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if the entry corresponding to the profile APN name was found
  FALSE - if otherwise

SIDE EFFECTS
  None
==============================================================================*/
boolean ds_pdn_cntxt_get_resolved_apn_name
(
  ds_pdn_context_s* pdn_cntx_ptr, 
  byte** res_apn_name
)
{ 
  int                    loop = 0;
  boolean                ret_val = FALSE;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_ptr, &subs_id)== FALSE)
  {
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  ds3gpp_enter_global_crit_section();

  for(loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if ( (ds_apn_table_validate_index(loop, subs_index)) &&
           ((ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr
              == pdn_cntx_ptr) ||
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr 
            == pdn_cntx_ptr)) )
    {
      if((char)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn[0] 
         != '\0')
      {
        DS_3GPP_C_MSG0(DS_3GPP_F3_MASK,"APN tbl entry found. Returning resolved name");
       *res_apn_name = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->resolved_apn;
        ret_val = TRUE;
      }
      
      break;
    }
  }

  ds3gpp_leave_global_crit_section();
  return ret_val;
} /* ds_pdn_cntxt_get_resolved_apn_name*/


/*===========================================================================
FUNCTION ds_apn_table_add_pdn

DESCRIPTION
  This function adds an entry to the APN table with the supplied info

PARAMETERS
  apn        - name of the APN
  ip_support - IP support for the APN 
  pdn_ptr    - Pointer to the PDN context


    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Successfully updated APN table
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_apn_table_add_pdn
(
  byte                      *apn,
  ds_apn_ip_support_type_e  incoming_ip_support,
  ds_pdn_context_s          *pdn_ptr
)
{
  boolean                  ret_val = FALSE;
  ds_pdn_context_s         *v4_pdn_cntxt_ptr = NULL;
  ds_pdn_context_s         *v6_pdn_cntxt_ptr = NULL;
  boolean apn_already_present = FALSE;

  int loop,update_index=0,add_index=DS_MAX_APN_NUM;
  boolean                allow_update = FALSE;
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds3gpp_enter_global_crit_section();

  if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_ptr, &subs_id)== FALSE)
  {
    ds3gpp_leave_global_crit_section();
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }
  
  DS_3GPP_MSG1_HIGH_EX(subs_index,"Debug: Add APN. IP Support:incoming_ip_support: %d",
                       incoming_ip_support);

  
    for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
    {
    if( ds_apn_table_validate_index(loop, subs_index) && 
          ((strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p
            [loop]->apn, (char *)apn,DS_UMTS_MAX_APN_STRING_LEN+1) == 0) ||
             ( 
            ((char)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->
             resolved_apn[0] != '\0') && 
            (strncasecmp((char*)ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]
            ->resolved_apn, (char*)apn,DS_UMTS_MAX_APN_STRING_LEN+1)== 0)
             )) )
      {
        apn_already_present = TRUE;
        //There is already an APN in the APN table. Check for single bearer case
        switch(incoming_ip_support)
        {
          case DS_IPV4_ONLY:
          case DS_IPV4_SINGLE_BEARER:
            if(ds_pdn_cntxt_get_v4_pdn_context(apn, &v4_pdn_cntxt_ptr, subs_id))
            {
            if(v4_pdn_cntxt_ptr == NULL)
	        {
              DS_3GPP_MSG1_HIGH_EX(subs_index,"Debug: Add APN. Already present. Allow update. Index: %d",
                                   loop);
              update_index = loop;
	          allow_update = TRUE;
	        }
            }
            else
            {
              DS3GPP_ASSERT(0);
            }
          break;

          case DS_IPV6_ONLY:
          case DS_IPV6_SINGLE_BEARER:
            if(ds_pdn_cntxt_get_v6_pdn_context(apn, &v6_pdn_cntxt_ptr, subs_id))
            {
            if(v6_pdn_cntxt_ptr == NULL)
	        {
              DS_3GPP_MSG1_HIGH_EX(subs_index,"Debug: Add APN. Already present. Allow update. Index: %d",
                                   loop);
              update_index = loop;
	          allow_update = TRUE;
	        }
            }
            else
            {
              DS3GPP_ASSERT(0);
            }
          break;

          default:
            break; 
        }/* switch */

        if (allow_update == TRUE) 
        {
          break; //break out of for
        }
      }
    }/* for */

    /*-------------------------------------------------------------------
    Decide whether we are going to update or add a new entry
    -------------------------------------------------------------------*/
    if (allow_update == TRUE) 
    {
      //call fill API with update_index
      ret_val = ds_apn_table_fill_info(update_index, apn, 
                                       incoming_ip_support, pdn_ptr);
      ds3gpp_leave_global_crit_section();
      return ret_val;
    }

    if( (apn_already_present == TRUE) &&
       (ds_3gpp_nv_manager_get_multi_pdn_same_apn(subs_id) == FALSE))
      {
      DS_3GPP_MSG0_HIGH_EX(subs_id,"Skipping dup entry addition in APN table");   
      ret_val = FALSE;
      ds3gpp_leave_global_crit_section();
      return ret_val;
    }

    //Add fresh in first available index if it is not already present
    ret_val = ds_apn_table_alloc_dyn_mem(&add_index, subs_id);

    /*----------------------------------------------------------------------- 
      APN not present in the APN table. Allocate memory to record the
      entry in the APN table
      -----------------------------------------------------------------------*/
    if( ret_val == TRUE )    
    {
      //call fill with add_index
      ret_val = ds_apn_table_fill_info(add_index, apn, incoming_ip_support, pdn_ptr);
    }

   ds3gpp_leave_global_crit_section();
   return ret_val;

} /* ds_apn_table_add_entry */

/*===========================================================================
FUNCTION ds_apn_table_validate_index

DESCRIPTION
  This function checks if an index in the APN table has been allocated memory

PARAMETERS
  index        - index to be validated
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static boolean ds_apn_table_validate_index
(
  int                    index,
  ds3gsubsmgr_subs_id_e_type subs_index
)
{
  boolean ret_val = FALSE;
  ds_pdn_context_apn_table_dyn_s *apn_table_dyn_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if((index < 0) || (index >= DS_MAX_APN_NUM))
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_index, "Index:d is out of bounds",index);
    return ret_val;
  }

  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return ret_val;
  }

  apn_table_dyn_p = ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index];

    if((apn_table_dyn_p != NULL) && (apn_table_dyn_p->cookie == APN_VALID_COOKIE))
    {
      ret_val = TRUE;
      DS_3GPP_MSG1_HIGH("APN Index:%d is valid",index);
    }
    
    return ret_val;
  }

/*===========================================================================
FUNCTION ds_apn_table_alloc_dyn_mem

DESCRIPTION
  This function allocates dynamic memory and also determine an index.

PARAMETERS
  *index       - returns the allocated index
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If allocation was successful
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static boolean ds_apn_table_alloc_dyn_mem
(
  int                   *alloc_index,
  sys_modem_as_id_e_type subs_id
)
{
  boolean ret_val = FALSE;
  int index = 0;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );

  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }

  for(index = 0; index < DS_MAX_APN_NUM; index++)
  {
    if(ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index] == NULL)
    {
  
      ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index] =
        (ds_pdn_context_apn_table_dyn_s *) modem_mem_alloc
        (sizeof(ds_pdn_context_apn_table_dyn_s),MODEM_MEM_CLIENT_DATA);

      if (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index] != NULL)
      {
        /*--------------------------------------------------------------------
           Memset the dynamic memory to 0 before using it
         --------------------------------------------------------------------*/
        memset(ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index],0, 
                 sizeof(ds_pdn_context_apn_table_dyn_s));
        ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[index]->cookie = APN_VALID_COOKIE;
       *alloc_index = index;
        ret_val = TRUE;
      }
      DS_3GPP_MSG2_HIGH_EX(subs_index,
                           "APN table dyn memory allocation at index:%d: status %d",
                           index, ret_val);

      break;
    }
  }

  if (index == DS_MAX_APN_NUM)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "Max APN contexts have been allocated");
  }

  return ret_val;

}

/*===========================================================================
FUNCTION ds_apn_table_free_dyn_mem

DESCRIPTION
  This function frees dynamic memory based on the index passed. 

PARAMETERS
   index       - Index to be freed
    
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None

===========================================================================*/
void ds_apn_table_free_dyn_mem
(
  int                    free_index,
  sys_modem_as_id_e_type subs_id
)
{

  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return;
  }
  
  if((free_index < 0) || (free_index >= DS_MAX_APN_NUM))
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "Index passed is invalid:d",free_index);
  }
  else
  {
   /*------------------------------------------------------------------- 
       Free the dynamic memory
    -------------------------------------------------------------------*/
    if (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[free_index] != NULL)
    {
      modem_mem_free(ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[free_index], 
                     MODEM_MEM_CLIENT_DATA);

      ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[free_index] = NULL;

      DS_3GPP_MSG1_HIGH_EX(subs_id,
                           "APN Table dyn memory index:%d freed",
                           free_index);
    }

  }

}

/*===========================================================================
FUNCTION DS_3GPP_APN_TBL_MANAGE_PDN_CNTXT_ENTRY

DESCRIPTION
  This function updates the APN table with the new IP support type and
  removes the old PDN context entry.

PARAMETERS 
  incoming_pdn_cntx_p  -- pointer to pdn context table
  add_ip_type          -- IP type of a particular PDN context that needs to
                          be removed
  remove_ip_type       -- IP type of a particular PDN context that needs to
                          be removed
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Successfully removed this entry
  FALSE- Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_3gpp_apn_tbl_manage_pdn_cntxt_entry
(
  ds_pdn_context_s            *incoming_pdn_cntx_p,
  ds_apn_ip_support_type_e     add_ip_type,
  ds_apn_ip_support_type_e     remove_ip_type
)
{
  int                    loop = 0;
  boolean                ret_val = DS3G_FAILURE;  
  sys_modem_as_id_e_type subs_id;
  ds3gsubsmgr_subs_id_e_type subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 if(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(incoming_pdn_cntx_p, 
                                           &subs_id)== FALSE)
  {
    return FALSE;
  }

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return FALSE;
  }  
  
  ds3gpp_enter_global_crit_section();

  for( loop = 0; loop < DS_MAX_APN_NUM; loop++)
  {
    if (
         (ds_apn_table_validate_index(loop, subs_index)) &&
	 (
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v4_pdn_ptr 
            == incoming_pdn_cntx_p) ||
           (ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->v6_pdn_ptr 
            == incoming_pdn_cntx_p)
         )
       )
    {      
      /*------------------------------------------------------------------- 
        Update IPv4 and remove IPv6 IP support
       -------------------------------------------------------------------*/
      if ((DS_IPV4_ONLY == add_ip_type) && (DS_IPV6_ONLY == remove_ip_type))
      {
        ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->\
           v4_pdn_ptr = incoming_pdn_cntx_p;
        ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->\
           v6_pdn_ptr = NULL;
      }

      /*------------------------------------------------------------------- 
        Update IPv6 and remove IPv4 IP support
       -------------------------------------------------------------------*/
      else if((DS_IPV6_ONLY == add_ip_type) && (DS_IPV4_ONLY == remove_ip_type))
      {
        ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->\
           v6_pdn_ptr = incoming_pdn_cntx_p;
        ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->\
           v4_pdn_ptr = NULL;
      }

      else
      {
        ret_val = DS3G_FAILURE;
        break;
      }

      ds_apn_table[subs_index].ds_pdn_context_apn_table_dyn_p[loop]->\
          ip_support = add_ip_type;

      ret_val = DS3G_SUCCESS;

      //Break from the loop
      break;
    }// Found entry in the APN table

  } //For loop to loop over all entries in the APN table.

  ds3gpp_leave_global_crit_section();

  if(ret_val == DS3G_FAILURE)
  {
    /* We reach here if the APN name is not found in the table */
    DS_3GPP_ERROR_LOG_0_EX(subs_index, "APN entry not found in apn table!");
  }

  return ret_val;
} /* ds_3gpp_apn_tbl_remove_pdn_cntxt_entry */

/*===========================================================================
FUNCTION ds_pdn_cntxt_get_ip_support_info_for_emc_pdn

DESCRIPTION
  This function returns the ip support type of EMC PDN from PDN CONTEXT PTR

PARAMETERS: 
  ds_pdn_context_s          pointer to pdn context table
  ds_apn_ip_support_type_e  IP type of the particular pdn support 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If ip support type is found from PDN CONTEXT PTR
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_pdn_cntxt_get_ip_support_info_for_emc_pdn
(
  ds_pdn_context_s          *pdn_cntx_ptr,
  ds_apn_ip_support_type_e  *ip_support
)
{
  ds_umts_pdp_type_enum_type pdp_support_type = DS_UMTS_PDP_MAX;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pdn_cntx_ptr->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type == DS_UMTS_PDP_IPV4V6)
  {
    if (pdn_cntx_ptr->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE] &&
       (pdn_cntx_ptr->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]))
    {
      pdp_support_type = DS_UMTS_PDP_IPV4V6;
    }
    else if (pdn_cntx_ptr->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE])
    {
      pdp_support_type = DS_UMTS_PDP_IPV4;
    }
    else if (pdn_cntx_ptr->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE])
    {
      pdp_support_type = DS_UMTS_PDP_IPV6;
    }
    else 
    {
      DS_3GPP_ERROR_LOG_0("Unknown PDP type.");
      return FALSE;
    }
  }
  else 
  {
    pdp_support_type = pdn_cntx_ptr->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;
  }

  switch (pdp_support_type)
  {
    case DS_UMTS_PDP_IPV4:
      *ip_support =  DS_IPV4_ONLY;
      break;
    case DS_UMTS_PDP_IPV6:
      *ip_support =  DS_IPV6_ONLY;
      break;
    case DS_UMTS_PDP_IPV4V6:
      *ip_support =  DS_IPV4V6;
      break;
    default:
      DS_3GPP_ERROR_LOG_1("Unknown PDP type %d", pdp_support_type);
      return FALSE;
  }
  return TRUE;
}
#endif /* FEATURE_DATA_3GPP */
