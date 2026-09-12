/**
  @file policyman.c

  @brief Public interface to Policy Manager Message router functions
*/

/*
    Copyright (c) 2013, 2014,2016 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_msgr.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "policyman_msgr_i.h"
#include "policyman_msg.h"
#include "policyman_state.h"
#include "policyman_task.h"
#include "policyman_dbg.h"
#include "policyman_util.h"
#include "queue.h"
#include <stringl/stringl.h>

#include "msgr_rex.h"
#include "lte_rrc_ext_msg.h"

typedef struct
{
 msgr_umid_type           umid;
 policyman_msgr_cb_fn_t   pfnCallback;
 void                    *pContext;
 sys_modem_as_id_e_type   subsId;
} policyman_msgr_reg_entry_t;


#define POLICYMAN_MSGR_BUF_CNT 5

/*  Registered messages
 */
static  policyman_msgr_reg_entry_t  msgr_set[POLICYMAN_MSGR_BUF_CNT];

/*  The policyman MSGR client
 */
static  msgr_client_t policyman_msgr_client;

/*  The MSGR queue.
 */
static  q_type        policyman_msgr_q;


#define POLICYMAN_OFFSETOF(type,member) (((char*)(&((type*)1)->member))-((char*)1))



typedef struct
{
  q_link_type link;           /* For linking onto msgr queue         */

  msgr_hdr_s  hdr;            /*  Message router header */
} policyman_msgrq_entry_t;




/*-------- policyman_msgr_set_find --------*/
/**
@brief  Find a registration entry in the set of registrations.

@param

@return

*/
STATIC policyman_msgr_reg_entry_t * policyman_msgr_set_find(
  msgr_umid_type           umid,
  sys_modem_as_id_e_type   subsId
)
{
  policyman_msgr_reg_entry_t  *ptr = NULL;
  size_t                       index;

  // search in msgr_set for a matching UMID and subscription Id
  for (index=0; ptr == NULL && index < POLICYMAN_MSGR_BUF_CNT; ++index)
  {
    if (  msgr_set[index].umid == umid
       && msgr_set[index].subsId == subsId )
    {
      ptr = &msgr_set[index];
    }
  }

  return ptr;
}


/*-------- policyman_msgr_set_add --------*/
STATIC void policyman_msgr_set_add(
  msgr_umid_type          umid,
  policyman_msgr_cb_fn_t  pfnCallback,
  sys_modem_as_id_e_type  subsId,
  void                   *data
)
{
  policyman_msgr_reg_entry_t  *pEntry;

  pEntry = policyman_msgr_set_find(0, SYS_MODEM_AS_ID_NONE);

  if (NULL != pEntry)
  {
    pEntry->umid        = umid;
    pEntry->pfnCallback = pfnCallback;
    pEntry->pContext    = data;
    pEntry->subsId      = subsId;
  }  
  else
  {
    POLICYMAN_MSG_ERROR_1( SUBS_PREFIX "item not added to set", subsId);
  }
}

/*-------- policyman_msgr_register_for_subs --------*/
void policyman_msgr_register_for_subs(
  msgr_umid_type           umid,
  policyman_msgr_cb_fn_t   pfnCallback,
  void                    *pContext,
  sys_modem_as_id_e_type   subsId
)
{
  errno_enum_type err = E_FAILURE;

  if (policyman_msgr_set_find(umid, subsId) == NULL)
  {
    err = msgr_register_variant( MSGR_POLICYMAN_CFG,
                                 &policyman_msgr_client,
                                 MSGR_ID_REX, 
                                 umid,
                                 subsId );
  }

  if (err == E_SUCCESS)
  {
    policyman_msgr_set_add(umid, pfnCallback, subsId, pContext);
  }
  else
  {
    POLICYMAN_MSG_ERROR_3( SUBS_PREFIX "Unable to register UMID 0x%x with MSGR, err %d",
                           subsId,
                           umid,
                           err );
  }
}


/*-------- policyman_msgr_set_init --------*/
static void policyman_msgr_set_init(
  void
)
{
  size_t index;

  for (index = 0; index < POLICYMAN_MSGR_BUF_CNT; index++)
  {
    msgr_set[index].umid        = 0;
    msgr_set[index].pfnCallback = NULL;
    msgr_set[index].pContext    = NULL;
    msgr_set[index].subsId      = SYS_MODEM_AS_ID_NONE;
  }
}


/*-------- policyman_msgr_init --------*/
void policyman_msgr_init(
  policyman_signal_t   sig,
  policyman_tcb_t     *pTCB
)
{
  errno_enum_type msgr_err;

  /*  Initialize the message router queue
   */
  (void) q_init( &policyman_msgr_q );

  /*  Create a message router client
   */
  msgr_err = msgr_client_create( &policyman_msgr_client );
  POLICYMAN_MSG_HIGH_1("msgr_client_create returned %d", msgr_err );

  if (msgr_err != E_SUCCESS)
  {
    POLICYMAN_MSG_ERROR_0("Unable to create MSGR client");
    return;
  }

  msgr_err = msgr_client_set_rex_q( &policyman_msgr_client,
                                    (rex_tcb_type *) pTCB,
                                    (rex_sigs_type) sig,
                                    &policyman_msgr_q,
                                    MSGR_NO_QUEUE,
                                    POLICYMAN_OFFSETOF(policyman_msgrq_entry_t, hdr),
                                    MSGR_NO_OFFSET,
                                    0xffff );

  POLICYMAN_MSG_HIGH_1("msgr_client_set_rex_q returned %d", msgr_err);

  policyman_msgr_set_init();

  /* LTE eMBMS registration */
  policyman_msgr_register_for_subs( LTE_RRC_EMBMS_ACTIVE_STS_IND, 
                                    policyman_state_update_embms_status, 
                                    NULL,
                                    SYS_MODEM_AS_ID_1 );

  /* LTE eMBMS registration for SUBS2 */
  policyman_msgr_register_for_subs( LTE_RRC_EMBMS_ACTIVE_STS_IND, 
                                    policyman_state_update_embms_status, 
                                    NULL,
                                    SYS_MODEM_AS_ID_2 );
}


/*-------- policyman_process_msgr --------*/
void policyman_process_msgr(
  void
)
{
  policyman_msgrq_entry_t  *pEntry;

  POLICYMAN_MSG_HIGH_0("Processing MSGR queue");

  while ((pEntry = q_get(&policyman_msgr_q)) != NULL)
  {
    msgr_hdr_s                  *pMsg = &pEntry->hdr;
    policyman_msgr_reg_entry_t  *pRegistration;
    sys_modem_as_id_e_type       subsId;

    subsId = MSGR_GET_VARIANT(pMsg);

    // this is required otherwise UMID has variant appended in it and it won't match with our registration entry
    MSGR_CLEAR_VARIANT(pMsg->id);
    
    pRegistration = policyman_msgr_set_find(pMsg->id, subsId);
    if (pRegistration != NULL)
    {
      POLICYMAN_MSG_HIGH_2( SUBS_PREFIX "msg id %d found in set, calling callback function", 
                            subsId,
                            pMsg->id);

      pRegistration->pfnCallback(pMsg, pRegistration->pContext, subsId);
    }

    msgr_rex_free_msg_buf(&(pEntry->link));
  }
}

#if defined(TEST_FRAMEWORK)
#error code not present
#endif /* +TEST_FRAMEWORK. */
