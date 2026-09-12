/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM LPP Transaction Manager module

GENERAL DESCRIPTION
  This file contains TM's LPP protocol utilities, which encode/decode LPP messages 

  Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2020 Qualcomm Technologies, Inc. Qualcomm Technologies
  Confidential and Proprietary. All Rights Reserved.
 
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* ^L<EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/lpp_common/src/tm_lpp_tx_mgr.c#3 $ $DateTime: 2020/09/04 13:58:08 $ $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  03/06/20   jv      Added function to check if asn1 mem context can be reset.
  12/07/17  ap       Code to test SRVCC handover cross case using DTF
  08/31/17   py      Remove AS ID from definition of tm_lpp_tx_mgr_handle_encode_transaction()
  05/08/17   rn      Updated Early Response 2-Timer code.
  03/22/17   rk      Remove tx that protocols considers to be delete in the end of handling the
                     incoming message.
  01/23/17   kpn     Remove AS ID from Tx packet structures
  12/09/16   rk      Release the memory from LPP data structure in the transaction item when client
                     is cleaned-up.
  10/18/16   kpn     Call lte_rrc_get_camped_cell_info_with_primary_plmn() with as_id as part of L+L changes.
  07/28/16   skm     2Timer re-arc for LPPe includes CR's(985835,1034212,1034213,1034216,1034219) 
  06/30/15   rk      Added a initiator type check condition to remove the transaction from the table.
                     This fixes the premature removal of transaction when the incoming & subsequent outgoing transaction id is same.
  06/10/15   gk      Abort processing robustness
  02/12/15   gk      Ack processing incorrect for seq #0
  03/05/14   sj      Fixed LPP CP Fuzz crash  
  02/27/13   rk      Removed a mem copy since source & destination is same location.
  12/14/11  gk      Lint and other fixes
  04/3/11   gk      Initial version

===========================================================================*/
#include "customer.h"   /* Customer configuration file */
#include "time_svc.h"   /* time_get_uptime_secs() */
#include "tm_osysinit.h"
#include "tm_asn1util.h"
#include "tm_lpp.h"
#include "tm_lpp_asn1util.h"
#include "sm_api.h" /* included only for the data structures defined there */
#include "msg.h"
#include "tm_lpp_tx_mgr.h"

#include "tm_lpp_cp.h" 
#ifdef FEATURE_GNSS_LPPE
#include "tm_lpp_e_asn1util.h"
#include "tm_lpp_e.h"
#endif

#define TM_LPP_TX_MGR_TABLE_INDEX_SUPL  0
#define TM_LPP_TX_MGR_TABLE_INDEX_CP    1

/* Local function protocols */

/* list of LPP registered clients */
#ifdef FEATURE_GNSS_DTF
#include "tm_dtf.h"
#endif
tm_lpp_tx_mgr_client_list_type tm_lpp_tx_mgr_client_list[TM_LPP_TX_MGR_CLIENT_MAX];

/* Dynamic linked list of outstanding LPP transaction for a given LPP Session */  
tm_lpp_transaction_list_summary_s_type   tm_lpp_tx_summary;

/*===========================================================================
  lpp_tx_mgr_release_lpp_data

  Description:
    LPP tx manager function to release the heap memory for LPP data structure.
    This function should be called just before deleting this transaction item
    to release any holding memory not prior release by LPP module.

  Parameters
     p_lpp_tx_item - Pointer to the transaction item

  Returns
    None.
=========================================================================*/
static void lpp_tx_mgr_release_lpp_data( tm_lpp_transaction_info_s_type * p_lpp_tx_item )
{
  if ( p_lpp_tx_item != NULL )
  {
    if ( p_lpp_tx_item->z_lpp_data_type.p_gnss_auxi_info != NULL )
    {
      os_MemFree( (void **) (&p_lpp_tx_item->z_lpp_data_type.p_gnss_auxi_info ));
      p_lpp_tx_item->z_lpp_data_type.p_gnss_auxi_info = NULL;
    }
    if ( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_earlyRsp_fetch_timer != NULL )
    {
      os_TimerStop( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_earlyRsp_fetch_timer );
      os_TimerDestroy( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_earlyRsp_fetch_timer );
      p_lpp_tx_item->z_lpp_data_type.z_rli.pz_earlyRsp_fetch_timer = NULL;
    }
    if (p_lpp_tx_item->z_lpp_data_type.z_rli.pz_gnss_auxi_timer != NULL)
    {
      os_TimerStop( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_gnss_auxi_timer );
      os_TimerDestroy( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_gnss_auxi_timer );
      p_lpp_tx_item->z_lpp_data_type.z_rli.pz_gnss_auxi_timer = NULL;
    }
    if ( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_rli_timer != NULL )
    {
      os_TimerStop( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_rli_timer );
      os_TimerDestroy( p_lpp_tx_item->z_lpp_data_type.z_rli.pz_rli_timer );
      p_lpp_tx_item->z_lpp_data_type.z_rli.pz_rli_timer = NULL;
    }
  }
}

/*===========================================================================
  tm_lpp_tx_mgr_find_seq_number

  Description:
    LPP function to find the transaction with matching sequence number.

  Parameters
     u_directionn - 0 - incoming ; 1 - outgoing
     u_seq_number - sequence number to search for a match
    **p_lpp_tx_type- pointer to the transaction table with a match

  Returns
    TRUE if a match, FALSE otherwise
=========================================================================*/
boolean tm_lpp_tx_mgr_find_seq_number(
  uint8                           u_direction,
  uint8                           u_seq_number,
  tm_lpp_transaction_info_s_type **p_lpp_tx_type
)
{
  tm_lpp_transaction_list_s_type  *p_lpp_tx_list = NULL;
  tm_lpp_transaction_info_s_type  *p_lpp_tx_elem = NULL;
  boolean b_found = FALSE;

  if ((tm_lpp_tx_summary.head == NULL) || (tm_lpp_tx_summary.num_trans == 0) ||
      (p_lpp_tx_type == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Empty list. No entry found",0,0,0);
    return FALSE;
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;
  while (p_lpp_tx_list != NULL)
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;
    if (p_lpp_tx_elem->b_in_use == TRUE)
    {

      /* check for a match of the seq number and direction of the message */
      if ((1 == u_direction) && (p_lpp_tx_elem->b_out_sequence_num_valid))
      {
        if (u_seq_number == p_lpp_tx_elem->u_out_sequence_num)
        {
          b_found = TRUE;
          break;
        }
      }
      else if ((0 == u_direction) && (p_lpp_tx_elem->b_in_sequence_num_valid))
      {
        if (u_seq_number == p_lpp_tx_elem->u_sequence_num)
        {
          b_found = TRUE;
          break;
        }
      }
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *) p_lpp_tx_list->next;
  }
  /* if transaction found return the transaction */
  if (b_found == TRUE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Found matching Seq No:%d Direction(in 0/out 1): %d tx_id = %d",
            u_seq_number, u_direction, p_lpp_tx_elem->z_current_trans_id.u_transaction_id);
    *p_lpp_tx_type = p_lpp_tx_elem;
    return TRUE;
  }
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: No matching seq number",0, 0,0);
  return FALSE;
} /* tm_lpp_tx_mgr_find_seq_number */


/*===========================================================================
  tm_lpp_tx_mgr_client_register

  Description:

     This function is used by clients to register with LPP transaction manager
     with a client handle

  Parameters: 

  *p_client: Pointer to client structure
  *handle: Pointer to client handle which will be filled by this function

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_lpp_tx_mgr_client_register(const tm_lpp_tx_mgr_client_type * p_client, uint8 *handle)
{
  uint8 client_table_index=0;
  int8 client_select_index = -1;


  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Client Register ", 0,0,0);

  if(p_client == NULL || handle == NULL)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:NULL pointers! %x %x", p_client, handle);
    return FALSE;
  }

  /* Each client goes into a unique index in the tx manager's table, fixed at compile time */
  if(p_client->client_id == TM_LPP_TX_MGR_CLIENT_TYPE_SUPL)
  {
    client_select_index = TM_LPP_TX_MGR_TABLE_INDEX_SUPL;
  }
  else if(p_client->client_id == TM_LPP_TX_MGR_CLIENT_TYPE_CP)
  {
    client_select_index = TM_LPP_TX_MGR_TABLE_INDEX_CP;
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Unknown Client! %x", p_client->client_id);
    return FALSE;	  
  }

  /* A client should not repeatedly register, return FALSE */
  if(tm_lpp_tx_mgr_client_list[client_select_index].b_in_use)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Client already registered! %x", p_client->client_id);
    return FALSE;
  }

  /* Add the client */
  tm_lpp_tx_mgr_client_list[client_select_index].b_in_use = TRUE;
  memscpy(&tm_lpp_tx_mgr_client_list[client_select_index].client, 
          sizeof(tm_lpp_tx_mgr_client_list[client_select_index].client),
          p_client, sizeof(*p_client));
  *handle = (uint8)client_select_index;

  MSG_2( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_LPP_TX_MGR: LPP Tx manager client %u added: client handle %d",
         tm_lpp_tx_mgr_client_list[client_select_index].client.client_id,
         client_select_index );

  return TRUE;
  
}


/*===========================================================================
  tm_lpp_tx_mgr_client_deregister

  Description:

     This function is used by clients to deregister with LPP transaction manager
     with a client handle

  Parameters: 

  *p_client: Pointer to client structure
  *handle: Pointer to client handle which will be filled by this function

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_lpp_tx_mgr_client_deregister(uint8 client_handle)
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Client DeRegister ");
  if( (client_handle >= TM_LPP_TX_MGR_CLIENT_MAX)||
      (!tm_lpp_tx_mgr_client_list[client_handle].b_in_use))
  {
    return FALSE;
  }

  MSG_2( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_LPP_TX_MGR: LPP Tx Manager client %u deleted: client handle %d",
         tm_lpp_tx_mgr_client_list[client_handle].client.client_id,
         client_handle );

  /*Clear away the client node*/
  tm_lpp_tx_mgr_client_list[client_handle].b_in_use = FALSE;     
  memset((void *)&(tm_lpp_tx_mgr_client_list[client_handle]), 0, sizeof(tm_lpp_tx_mgr_client_list_type));
  return ( TRUE );
}


/*===========================================================================
  tm_lpp_tx_mgr_client_get_client_ptr

  Description:

    LPP function to get the client structure given a client handle

  Parameters: 

  handle: client handle 

  Return value: 
    pointer to the client type
=============================================================================*/
tm_lpp_tx_mgr_client_type *tm_lpp_tx_mgr_client_get_client_ptr(uint8 client_handle)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Client GetPtr %d", client_handle,0,0);
  if ((client_handle < TM_LPP_TX_MGR_CLIENT_MAX) &&
      (tm_lpp_tx_mgr_client_list[client_handle].b_in_use == TRUE))
  {
    return (&tm_lpp_tx_mgr_client_list[client_handle].client);
  }
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Client GetPtr Failed. in_use %d", 
        tm_lpp_tx_mgr_client_list[client_handle].b_in_use);
  return NULL;
}

/*===========================================================================
  tm_lpp_tx_mgr_get_client_type
 
  Description:
    LPP function to get the client structure given a client handle
  
  Parameters
    u_client_index - Index of the client in the transaction manager client list table.
 
  Returns 
    Returns the pointer to the client type
===========================================================================*/
tm_lpp_tx_mgr_client_type *tm_lpp_tx_mgr_get_client_type(uint8 u_client_index)
{
  return (&tm_lpp_tx_mgr_client_list[u_client_index].client);
}
 
/*===========================================================================
  tm_lpp_tx_mgr_init
 
  Description:
    LPP function to init the transaction manager 
  
  Parameters
    u_client_index - Index of the client in the transaction manager client list table.
 
  Returns 
    none
===========================================================================*/
void tm_lpp_tx_mgr_init(uint8 u_client_index)
{
  tm_lpp_tx_mgr_client_type *p_client;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Tx_mgrinit client type %d  num_trans %d ", u_client_index,
          tm_lpp_tx_summary.num_trans,0);

  p_client = tm_lpp_tx_mgr_get_client_type(u_client_index);
  if (p_client)
  {
    tm_lpp_tx_mgr_remove_entry_by_clientid(p_client->client_id);
  }
}

/*===========================================================================
  tm_lpp_get_next_transaction_id
 
  Description:
    LPP function to generate a transaction ID for an outgoing LPP message
 
  Parameters: none
 
  Returns: 
    Returns the Transaction ID. the Initiator is set to SET
===========================================================================*/
static tm_lpp_transaction_id_s_type tm_lpp_get_next_transaction_id(void)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:GetNextTx_ID ", 0,0,0);
  if( z_lpp_transaction_id.u_transaction_id < 255)
  {
    z_lpp_transaction_id.u_transaction_id++;
    z_lpp_transaction_id.u_initiator = TM_LPP_TRANS_ID_INIT_SET;
  }
  else
  {
    z_lpp_transaction_id.u_transaction_id = 0;
    z_lpp_transaction_id.u_initiator = TM_LPP_TRANS_ID_INIT_SET;
  }
  z_lpp_transaction_id.m.u_trans_id_valid = 1;
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:GetNextTx_ID %d, init %d ", 
    z_lpp_transaction_id.u_transaction_id, z_lpp_transaction_id.u_initiator,0);
  return z_lpp_transaction_id;
}

/*===========================================================================
  tm_lpp_tx_mgr_remove_entry_by_clientid
 
  Description: 
    Remove all transactions associated with the client id.
 
  Parameters:
   u_client_id - client id.
   
  Returns: 
   None
=============================================================================*/
void tm_lpp_tx_mgr_remove_entry_by_clientid(uint8 const u_client_id)
{
  tm_lpp_transaction_list_s_type *p_lpp_tx_curr = NULL,
                                 *p_lpp_tx_prev = NULL,
                                 *p_lpp_tx_next = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Remove Entries. Num of trans:%d", tm_lpp_tx_summary.num_trans, 0, 0);

  p_lpp_tx_prev = p_lpp_tx_curr = tm_lpp_tx_summary.head;

  while ( (p_lpp_tx_curr != NULL) && (tm_lpp_tx_summary.num_trans > 0) )
  {
    /* Save the next element */
    p_lpp_tx_next = p_lpp_tx_curr->next;

    if ( u_client_id == p_lpp_tx_curr->elem.u_client_handle )
    {
      /* If the head element is matched for deleting, update the head pointer */
      if ( p_lpp_tx_curr == tm_lpp_tx_summary.head )
      {
        tm_lpp_tx_summary.head = p_lpp_tx_next;
      }
      else
      {
        /* The current node will be deleted. Link the previous & next node */
        p_lpp_tx_prev->next = p_lpp_tx_next;
      }

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: FREE TX 0x%x Tx ID:%d, Seq Num: %u",
              p_lpp_tx_curr,
              p_lpp_tx_curr->elem.z_current_trans_id.u_transaction_id, 
              p_lpp_tx_curr->elem.u_out_sequence_num);

      lpp_tx_mgr_release_lpp_data( &p_lpp_tx_curr->elem );


      os_MemFree((void **)&p_lpp_tx_curr);
      tm_lpp_tx_summary.num_trans--;

      /* Restore the current node to next node */
      p_lpp_tx_curr = p_lpp_tx_next;

      continue; /* Continue previous node remains the same since current node is deleted */
    }
    /* Point to previous linklist */
    p_lpp_tx_prev = p_lpp_tx_curr;
    /* Increment current pointer to point to next element */
    p_lpp_tx_curr = p_lpp_tx_next;
  } /* while */

}

/*===========================================================================
  tm_lpp_tx_mgr_remove_entry
 
  Description: 
    LPP function to remove one or all transactions associated with a transaction id.
 
  Parameters:
   *p_lpp_tx_type- pointer to the transaction to be removed
   
  Returns: 
   None
=============================================================================*/
void tm_lpp_tx_mgr_remove_entry(tm_lpp_transaction_info_s_type **p_lpp_tx_to_remove)

{
  uint8                           u_transaction_id_to_remove;
  uint8                           u_client_handle_to_remove;
  tm_lpp_transaction_list_s_type  *p_lpp_tx_curr             = NULL,
                                  *p_lpp_tx_prev = NULL,
                                  *p_lpp_tx_next             = NULL;
  uint8                     const u_num_trans_at_start       = tm_lpp_tx_summary.num_trans;
  tm_lpp_trans_id_e_type          u_initiator_to_remove = TM_LPP_TRANS_ID_INIT_SERVER;

  if ( (p_lpp_tx_to_remove == NULL) ||  (0 == tm_lpp_tx_summary.num_trans) )
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Remove entries cancelled due to null ptr(%p) or no transaction(%d)", 
          p_lpp_tx_to_remove, tm_lpp_tx_summary.num_trans);
    return;
  }

  u_transaction_id_to_remove = (*p_lpp_tx_to_remove)->z_current_trans_id.u_transaction_id;
  u_initiator_to_remove = (*p_lpp_tx_to_remove)->z_current_trans_id.u_initiator;
  u_client_handle_to_remove = (*p_lpp_tx_to_remove)->u_client_handle;
  p_lpp_tx_prev = p_lpp_tx_curr = tm_lpp_tx_summary.head;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Remove tx item TID %d, Intr %d. Total tx items:%d",
        u_transaction_id_to_remove, u_initiator_to_remove, tm_lpp_tx_summary.num_trans);

  while ( (p_lpp_tx_curr != NULL) && (tm_lpp_tx_summary.num_trans > 0) )
  {
    /* Save the next element */
    p_lpp_tx_next = p_lpp_tx_curr->next;

    if ( (u_transaction_id_to_remove == p_lpp_tx_curr->elem.z_current_trans_id.u_transaction_id) &&
         (u_initiator_to_remove == p_lpp_tx_curr->elem.z_current_trans_id.u_initiator) &&
         (u_client_handle_to_remove == p_lpp_tx_curr->elem.u_client_handle) )
    {
      /* If the head element is matched for deleting, update the head pointer */
      if ( p_lpp_tx_curr == tm_lpp_tx_summary.head )
      {
        tm_lpp_tx_summary.head = p_lpp_tx_next;
      }
      else
      {
        /* The current node will be deleted. Link the previous & next node */
        p_lpp_tx_prev->next = p_lpp_tx_next;
      }

      MSG_4( MSG_SSID_GPSSM, MSG_LEGACY_MED, "Removed tx item %d, ID %d, SeqNum %u, MsgId %d",
             NULL==p_lpp_tx_curr, u_transaction_id_to_remove, p_lpp_tx_curr->elem.u_out_sequence_num,
             p_lpp_tx_curr->elem.e_messages_id );

      os_MemFree((void **)&p_lpp_tx_curr);
      tm_lpp_tx_summary.num_trans--;

      /* Restore the current node to next node */
      p_lpp_tx_curr = p_lpp_tx_next;

      continue; /* Continue previous node remains the same since current node is deleted */
    }
    /* Point to previous linklist */
    p_lpp_tx_prev = p_lpp_tx_curr;
    /* Increment current pointer to point to next element */
    p_lpp_tx_curr = p_lpp_tx_next;
  } /* while */

  if ( u_num_trans_at_start == tm_lpp_tx_summary.num_trans )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: Cannot find the Tx ID to free", 0, 0, 0);
  }
  else
  {
    /* Found matching transaction & removed */
    *p_lpp_tx_to_remove = NULL;
  }

  return;
} /* tm_lpp_tx_mgr_remove_entry */


/*===========================================================================
  tm_lpp_tx_mgr_insert_entry
 
  Description: 
    LPP function to insert a transaction in transaction 
          manager table
 
  Parameters:
   *p_lpp_tx_id  - pointer to transaction id type
   *p_lpp_tx_type- pointer to the transaction table
    client_handle - the client_handle to which this transaction belongs to

  Returns: 
    Returns the TRUE or FALSE. 
=============================================================================*/
boolean  tm_lpp_tx_mgr_insert_entry(const tm_lpp_transaction_id_s_type *p_lpp_tx_id, 
                                    tm_lpp_transaction_info_s_type **p_lpp_tx_type, 
                                    uint8 u_client_handle)
{
  tm_lpp_transaction_list_s_type  *p_lpp_tx_list = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR:Insert/Update Entry client_handle %d", u_client_handle, 0, 0);

  /* If update the existing transaction */
  if ( *p_lpp_tx_type != NULL )
  {
    memscpy(&((*p_lpp_tx_type)->z_current_trans_id), sizeof((*p_lpp_tx_type)->z_current_trans_id),
            p_lpp_tx_id, sizeof(*p_lpp_tx_id));
    (*p_lpp_tx_type)->b_in_use = TRUE;
    (*p_lpp_tx_type)->b_new_tx = TRUE;
    (*p_lpp_tx_type)->u_client_handle = u_client_handle;
    /* time-stamp the transaction */
    (*p_lpp_tx_type)->q_timestamp = time_get_uptime_secs();

    return ( TRUE );
  }
  else
  {
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *)os_MemAlloc(sizeof(tm_lpp_transaction_list_s_type), OS_MEM_SCOPE_TASK);
    if ( p_lpp_tx_list == NULL )
    {
      return ( FALSE );
    }
    memset(p_lpp_tx_list, 0, sizeof(tm_lpp_transaction_list_s_type));
    memscpy(&p_lpp_tx_list->elem.z_current_trans_id, sizeof(p_lpp_tx_list->elem.z_current_trans_id),
            p_lpp_tx_id, sizeof(*p_lpp_tx_id));
  }

  /*create the list if nothing was there */
  if ( tm_lpp_tx_summary.num_trans == 0 )
  {
    tm_lpp_tx_summary.head = p_lpp_tx_list;
    tm_lpp_tx_summary.num_trans = 1;
  }
  else
  {
    /* add new tx infront of the head */
    p_lpp_tx_list->next = tm_lpp_tx_summary.head;
    tm_lpp_tx_summary.num_trans++;
    tm_lpp_tx_summary.head = p_lpp_tx_list;
  }

  *p_lpp_tx_type = &p_lpp_tx_list->elem;
  (*p_lpp_tx_type)->b_in_use = TRUE;
  (*p_lpp_tx_type)->b_new_tx = TRUE;

  (*p_lpp_tx_type)->u_client_handle = u_client_handle;
  /* time-stamp the transaction */
  (*p_lpp_tx_type)->q_timestamp = time_get_uptime_secs();

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: NEW TX: Id:%d, initiator:%d num_trans:%d",
          (*p_lpp_tx_type)->z_current_trans_id.u_transaction_id,
          (*p_lpp_tx_type)->z_current_trans_id.u_initiator,
          tm_lpp_tx_summary.num_trans);

  return ( TRUE );
}



/*===========================================================================
  tm_lpp_tx_mgr_find_entry
 
  Description: 
    LPP function to find a transaction in transaction manager table

  Parameters:
    p_lpp_tx_type - Pointer to transaction type
    p_lpp_tx_id   - Pointer to transaction id
    client_handle - the client_handle to which this transaction belongs to


  Returns: 
    Returns TRUE or FALSE. 
=============================================================================*/
boolean tm_lpp_tx_mgr_find_entry(
        tm_lpp_transaction_info_s_type     **p_lpp_tx_type,
        const tm_lpp_transaction_id_s_type *p_lpp_tx_id,
        uint8 u_client_handle)
{
  tm_lpp_transaction_list_s_type  *p_lpp_tx_list = NULL;
  tm_lpp_transaction_info_s_type  *p_lpp_tx_elem;
  
  MSG_4( MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_LPP_TX_MGR:Find Entry valid %d trans_id %d, init %d. num tx %u",
         p_lpp_tx_id->m.u_trans_id_valid, p_lpp_tx_id->u_transaction_id, p_lpp_tx_id->u_initiator,
         tm_lpp_tx_summary.num_trans );

  if ((!(p_lpp_tx_id->m.u_trans_id_valid)) || (0 == tm_lpp_tx_summary.num_trans))
  {
    return FALSE;
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;
  while (p_lpp_tx_list != NULL)
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;
    if (p_lpp_tx_elem->b_in_use == TRUE &&
        p_lpp_tx_elem->u_client_handle == u_client_handle)
    {
      if ((p_lpp_tx_id->m.u_trans_id_valid) &&
           (p_lpp_tx_elem->z_current_trans_id.u_initiator == 
           p_lpp_tx_id->u_initiator) &&
          (p_lpp_tx_elem->z_current_trans_id.u_transaction_id == 
           p_lpp_tx_id->u_transaction_id))
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Found transaction entry handle %d txid %d", u_client_handle, p_lpp_tx_id->u_transaction_id, 0);
        if ( p_lpp_tx_type != NULL )
        {
          *p_lpp_tx_type = p_lpp_tx_elem;
        }
        return ( TRUE );
      }
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *) p_lpp_tx_list->next;
  }

  return FALSE;
}


/*===========================================================================
  tm_lpp_tx_mgr_is_osys_ctxt_mem_reset_allowed
 
  Description: 
    LPP function to check if ASN1 context can be resetted 
  Parameters:
    void

  Returns: 
    Returns TRUE or FALSE. 
=============================================================================*/
boolean tm_lpp_tx_mgr_is_osys_ctxt_mem_reset_allowed( void )
{
  if( 0 == tm_lpp_tx_summary.num_trans )
  {
     return TRUE;
  }
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
              "TM_LPP_TX_MGR:osys ctxt mem reset not allowed since no of tx pending is %d",tm_lpp_tx_summary.num_trans,0);
  return FALSE;
}
/*===========================================================================
  tm_lpp_tx_mgr_get_num_tx
 
  Description: 
    LPP function to find a transaction in transaction 
          manager table 
  Parameters:
    u_client_index - Index of the client in the transaction manager client list table.


  Returns: 
    Returns TRUE or FALSE. 
=============================================================================*/
uint8 tm_lpp_tx_mgr_get_num_tx( uint8 u_client_index )
{
  tm_lpp_transaction_list_s_type  *p_lpp_tx_list = NULL;
  tm_lpp_transaction_info_s_type  *p_lpp_tx_elem;
  tm_lpp_tx_mgr_client_type       *p_client      = NULL;  
  uint8 num_tx=0;
  
  if (tm_lpp_tx_summary.num_trans == 0)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Empty list. No entry found",0,0,0);
    return 0;
  }

  p_client = tm_lpp_tx_mgr_get_client_type(u_client_index);
  if (p_client == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: no client for client_handle %d", u_client_index, 0, 0);
    return 0;
  }
  
  p_lpp_tx_list = tm_lpp_tx_summary.head;
  while (p_lpp_tx_list != NULL)
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;
#if 0
    MSG_8(  MSG_SSID_GPSSM, MSG_LEGACY_MED,
           "u_client_handle %d elt client_handle %d b_in_use %d tx_id_valid %d "
           "tx_id %d msg_id %d sess_typ %d pending_ue_tx %d", 
              u_client_handle,
              p_lpp_tx_elem->u_client_handle, 
              p_lpp_tx_elem->b_in_use, 
              p_lpp_tx_elem->z_current_trans_id.u_valid,
              p_lpp_tx_elem->z_current_trans_id.u_transaction_id,
              p_lpp_tx_elem->e_messages_id,
              p_lpp_tx_elem->u_lpp_session_type,
              p_lpp_tx_elem->b_pending_ue_tx);
#endif            
            
    if (p_lpp_tx_elem->b_in_use == TRUE &&
        p_lpp_tx_elem->u_client_handle == p_client->client_id)
    {
      num_tx++;
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *) p_lpp_tx_list->next;
  }

  return num_tx;
}
 



/*===========================================================================
  tm_lpp_tx_mgr_handle_entry
 
  Description: 
    LPP function to handle a transaction. this will either
          create a new entry in transaction manager table or
          update the existing entry
 
  Parameters
    *p_lpp_tx_id  - pointer to the transaction id structure
    *p_lpp_tx_type- pointer to the transaction table
     client_handle - the client_handle to which this transaction belongs to
 
 
  Returns
    TRUE if success, FALSE otherwise
=========================================================================*/
boolean tm_lpp_tx_mgr_handle_entry( 
  uint8 u_client_handle,
  tm_lpp_transaction_id_s_type *p_lpp_tx_id,
  tm_lpp_transaction_info_s_type **p_lpp_tx_type
  )
{
  boolean b_ret = TRUE;
  tm_lpp_transaction_id_s_type z_tx_id;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Handle entry %d", u_client_handle, 0,0);
  if (p_lpp_tx_id == NULL)
  {
    p_lpp_tx_id = &z_tx_id;
    *p_lpp_tx_id = tm_lpp_get_next_transaction_id();
    b_ret = tm_lpp_tx_mgr_insert_entry(p_lpp_tx_id, p_lpp_tx_type, u_client_handle);
    if ( b_ret == FALSE)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: Could not insert transaction", 0,0,0); 
      return FALSE;
    }
  }
  else
  {
    /* Search if the transaction already exists in the table */
    if ( tm_lpp_tx_mgr_find_entry( p_lpp_tx_type, p_lpp_tx_id, u_client_handle ) )
    {
      (*p_lpp_tx_type)->b_new_tx = FALSE;
    }
    else
    {
      /* if no transaction exists in the table, add it */
      b_ret = tm_lpp_tx_mgr_insert_entry(p_lpp_tx_id, p_lpp_tx_type, u_client_handle);
      if ( b_ret  == FALSE)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: could not insert transaction", 0,0,0); 
        return FALSE;
      }
    }
  }
  return TRUE;
}



/*===========================================================================
  tm_lpp_tx_mgr_handle_encode_transaction

  Description:
          LPP function to encode LPP transaction. this function 
          checks the transaction, inserts it in the transaction
          in the table if not already there, handles
          encode of the transaction as passed in

  Parameters: 
      u_client_index - Index of the client in the transaction manager client list table.
      message_type - type of message to be handled
      p_lpp_tx_id  - Look-up transaction id. Used to retrieve the tx item from table if provided.
      p_lpp_tx_type - Pointer to the transaction item. If its provided it should be of valid type.
      So use it directly instead of again calling the handle entry function & copy over the data.
      lpp session type - UP or CP

  Return value: none
=============================================================================*/
boolean tm_lpp_tx_mgr_handle_encode_transaction ( 
      uint8 u_client_index,
      tm_lpp_message_e_type e_message_type,
      tm_lpp_transaction_id_s_type *p_lpp_tx_id,
      tm_lpp_transaction_info_s_type **p_lpp_tx_type,
      uint8                            lpp_session_type
)
{
  boolean                   b_ret = FALSE;
  tm_lpp_tx_mgr_client_type *p_client;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_LPP_TX_MGR: Encode Transaction %d", e_message_type);

  if ( NULL == p_lpp_tx_type )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: p_lpp_tx_type null");
    return ( FALSE );
  }

  p_client = tm_lpp_tx_mgr_get_client_type( u_client_index );
  if ( p_client == NULL )
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Unregisterd client_handle %d", u_client_index);
    return ( FALSE );
  }

  /* If the transaction item is provided it should be of valid type. So use it directly instead of again
  * calling the handle entry function & copy over the data.
  */
  if ( NULL == *p_lpp_tx_type )
  {
    /* validate the transaction and place it in the table */
    b_ret = tm_lpp_tx_mgr_handle_entry( p_client->client_id, p_lpp_tx_id, p_lpp_tx_type );
    if ( b_ret == FALSE )
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Handle entry returned false");
      return ( b_ret );
    }

  }

  (*p_lpp_tx_type)->u_client_handle = p_client->client_id;
  (*p_lpp_tx_type)->e_messages_id = e_message_type;
  (*p_lpp_tx_type)->u_lpp_session_type = lpp_session_type;

  switch ( (*p_lpp_tx_type)->e_messages_id  )
  {
     case  TM_LPP_MESG_PROV_CAP:
       b_ret = tm_lpp_encode_prov_cap( p_client, *p_lpp_tx_type );
       break;

     case TM_LPP_MESG_PROV_LOC_INFO:
       b_ret = tm_lpp_encode_prov_loc_info( p_client, *p_lpp_tx_type );
       break;

     case TM_LPP_MESG_REQ_AD:
       b_ret = tm_lpp_encode_req_ad( p_client, *p_lpp_tx_type );
       break;

     case TM_LPP_MESG_ERROR:
       b_ret = tm_lpp_encode_err( p_client, *p_lpp_tx_type );
       break;

     case TM_LPP_MESG_ABORT:
       b_ret = tm_lpp_encode_abort( p_client, *p_lpp_tx_type );
       break;

     default:
       MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Unhandled message %d", (*p_lpp_tx_type)->e_messages_id, 0, 0 );
       break;
  }


  if ( ((*p_lpp_tx_type)->b_end_sent == TRUE) && ((*p_lpp_tx_type)->b_new_tx == TRUE) )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_LPP_TX_MGR: Got unsolicited trans");
    (*p_lpp_tx_type)->b_unsolicited = TRUE;
  }

  if ( (b_ret == TRUE) && (lpp_session_type == TM_LPP_SESSION_TYPE_CP) )
  {
    /* If transaction is not Early Response, then set wait for ack flag. */
    if (0 == ((*p_lpp_tx_type)->z_lpp_data_type.z_rli.s_lpp_ext & C_LPP_EXT_EARLY_RESP))
    {
      tm_lpp_cp_wait_for_ack( TRUE );
    }
  }

  return ( b_ret );
}


/*===========================================================================
  tm_lpp_tx_mgr_handle_decode_transaction

  Description:
          LPP function to Handle LPP transaction. this function 
          checks the transaction, inserts it in the transaction
          in the table if not already there, handles
          encode/decode of the transaction as passed in

  Parameters: 
      client_handle - handle to the client
      p_paylod         - Pointer to the LPP payload to be decoded
      p_lpp_tx_type    - Pointer to the transaction 
      lpp session type - UP or CP
      tm_lpp_error_type - Pointer to LPP Error found during decode

  Return value: TRUE if success /FALSE otherwise
=============================================================================*/
boolean tm_lpp_tx_mgr_handle_decode_transaction ( 
      uint8 u_client_index,
      const tm_pdu_buf_type *p_payload,
      tm_lpp_transaction_info_s_type **pp_lpp_tx_type,
      uint8                            lpp_session_type,
      tm_lpp_error_type              *p_lpp_error
)
{
  boolean b_ret        = FALSE;
  boolean b_end        = FALSE;

  int decode_status = 0;
  tm_lpp_transaction_id_s_type z_lpp_tx_id;

  /* Decoded LPP Message - allocated in OSYS memory */
  LPP_Message                *p_pdu_decoded_ptr  = NULL; 
#ifdef FEATURE_GNSS_LPPE
  tm_decode_OMA_LPPe_MessageExtension  *p_epdu_decoded_ptr = NULL;
#endif

  tm_lpp_tx_mgr_client_type    *p_client = NULL;
  
  if ((p_lpp_error == NULL) || (p_payload == NULL) || (pp_lpp_tx_type == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: Null ptr passed!! %p %p %p",
              p_lpp_error, p_payload, pp_lpp_tx_type);
    return FALSE;
  }
  p_lpp_error->w_ErrType = LPP_ERROR_NO_ERR;
  
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Decode Transaction", 0, 0,0);
  p_client = tm_lpp_tx_mgr_get_client_type(u_client_index);
  if ( p_client == NULL )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: Unregisterd client_handle %d", u_client_index,0,0);
    p_lpp_error->w_ErrType |= LPP_ERROR_TYPE_LPP_ERROR;
    p_lpp_error->u_ErrorErrCode = LPP_ERROR_UNDEF;
    return FALSE;
  }
    
  /* This function allocates the OSYS memory for the Decoded LPP Message */
  p_pdu_decoded_ptr = (LPP_Message*)tm_lpp_decode_message(p_payload, &decode_status);
    
  if (p_pdu_decoded_ptr == NULL)
  {                            
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: Decode error",0,0,0);
    p_lpp_error->w_ErrType |= LPP_ERROR_TYPE_LPP_ERROR;
    p_lpp_error->u_ErrorErrCode = LPP_ERROR_UNDEF;
    return FALSE;
  }
  if (decode_status != 0 &&
      decode_status != C_ASN1_DECODE_BUFFER_NOT_ALLOCATED) 
  {
    tm_lpp_asn1_osys_IE_free((void**)&p_pdu_decoded_ptr);
    p_lpp_error->w_ErrType |= LPP_ERROR_TYPE_LPP_ERROR;
    p_lpp_error->u_ErrorErrCode = LPP_ERROR_MSGBODY_ERROR;
    return FALSE;
  }  

  /***************************************************************************
   * Only LPP CP handled here                                                *
   ***************************************************************************/  
  if (lpp_session_type == TM_LPP_SESSION_TYPE_CP)
  { 
    boolean b_early_exit = FALSE;
    b_ret = tm_lpp_cp_process_ack(p_pdu_decoded_ptr, pp_lpp_tx_type, p_lpp_error, &b_early_exit);

    if (b_early_exit == TRUE)
    {
      tm_lpp_asn1_osys_IE_free((void**)&p_pdu_decoded_ptr);     
      return b_ret;    
    }
  }
 
  /* Save Transaction Id and create/update the entry */
  if (p_pdu_decoded_ptr->m.transactionIDPresent)
  {
    z_lpp_tx_id.m.u_trans_id_valid = 1;
    z_lpp_tx_id.u_initiator = (tm_lpp_trans_id_e_type)p_pdu_decoded_ptr->transactionID.initiator;
    z_lpp_tx_id.u_transaction_id = p_pdu_decoded_ptr->transactionID.transactionNumber;
  }
  else
  {
    z_lpp_tx_id.m.u_trans_id_valid = 0;
  }

  b_end = p_pdu_decoded_ptr->endTransaction;  
  
  b_ret = tm_lpp_tx_mgr_handle_entry(p_client->client_id, &z_lpp_tx_id, pp_lpp_tx_type);

  if ((FALSE == b_ret) || (NULL == *pp_lpp_tx_type))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: Transaction error",0,0,0);
    tm_lpp_asn1_osys_IE_free((void**)&p_pdu_decoded_ptr);
    p_lpp_error->w_ErrType |= LPP_ERROR_TYPE_LPP_ERROR;
    p_lpp_error->u_ErrorErrCode = LPP_ERROR_INCORRECT_DATA;
    return FALSE;
  }


  if (lpp_session_type == TM_LPP_SESSION_TYPE_CP)
  {
    (*pp_lpp_tx_type)->u_lpp_session_type = TM_LPP_SESSION_TYPE_CP;
    (*pp_lpp_tx_type)->u_sequence_num     = p_pdu_decoded_ptr->sequenceNumber;
    (*pp_lpp_tx_type)->b_in_sequence_num_valid = TRUE; 
  }
  else
  {
    (*pp_lpp_tx_type)->u_lpp_session_type = TM_LPP_SESSION_TYPE_UP;
  }

  if ((p_pdu_decoded_ptr->m.lpp_MessageBodyPresent != 1) ||
      (p_pdu_decoded_ptr->lpp_MessageBody.t != LPP_MESSAGEBODY_C1))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: LPP Unknown message body %d", p_pdu_decoded_ptr->lpp_MessageBody.t, 0, 0);
    b_ret = FALSE;
    tm_lpp_asn1_osys_IE_free((void **)&p_pdu_decoded_ptr); 
    p_lpp_error->w_ErrType |= LPP_ERROR_TYPE_LPP_ERROR;
    p_lpp_error->u_ErrorErrCode = LPP_ERROR_MSGBODY_ERROR;
    return b_ret;
  }
#ifdef FEATURE_GNSS_LPPE
  p_epdu_decoded_ptr = 
    (tm_decode_OMA_LPPe_MessageExtension*) tm_lpp_e_decode_epdu_from_lpp_pdu(
                                   p_pdu_decoded_ptr->lpp_MessageBody.u.c1, 
                                   &decode_status,
                                   &((*pp_lpp_tx_type)->z_lpp_data_type.z_lpp_err));
  /* If epdu is not present or could not be decoded, p_epdu_ptr will be NULL, 
   * Non Null otherwise */
  (*pp_lpp_tx_type)->z_lpp_data_type.p_decode_epdu_ptr = p_epdu_decoded_ptr;
#endif

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,">>>> [Server] Tx id:%d Seq no:%d Msg:%d",
          p_pdu_decoded_ptr->transactionID.transactionNumber,
          p_pdu_decoded_ptr->sequenceNumber,
          p_pdu_decoded_ptr->lpp_MessageBody.u.c1->t); 

  if ( ((*pp_lpp_tx_type)->b_end_sent == TRUE) && (b_end == TRUE))
  {
     /* if we already received end for this transaction and another end is received, it is an abnormal*/
     (*pp_lpp_tx_type)->b_abnomal_transaction = TRUE;
     MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: LPP transaction received after receiving an end. Continue to process ", 0,0,0);
  }

  /* Save a few information in Tx: seq number,  session type , etc. */
  (*pp_lpp_tx_type)->b_end_sent = p_pdu_decoded_ptr->endTransaction;
  (*pp_lpp_tx_type)->z_lpp_data_type.p_pdu_ptr = p_pdu_decoded_ptr;

  switch (p_pdu_decoded_ptr->lpp_MessageBody.u.c1->t) 
  {
    case LPP_MESSAGE_PROV_AD:
      b_ret = tm_lpp_handle_prov_ad(p_client, *pp_lpp_tx_type);
      tm_lpp_asn1_osys_IE_free((void**)&p_pdu_decoded_ptr);
      (*pp_lpp_tx_type)->z_lpp_data_type.p_pdu_ptr = NULL;
	  
	  
#ifdef FEATURE_GNSS_DTF
      if (TRUE == tm_dtf_events_db.pad_received.status)
      {
        b_tm_dtf_srvcc_ho_test_triggered = TRUE;
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: ### DTF###  PAD received Trigger handover %d", 0,0,0);
      }
#endif
	  break;
    case LPP_MESSAGE_REQ_LOCINFO :
      b_ret = tm_lpp_handle_req_loc_info(p_client, *pp_lpp_tx_type);
#ifdef FEATURE_GNSS_LPPE
      if(p_epdu_decoded_ptr != NULL)
      {
        tm_lpp_e_asn1_osys_IE_free((void**)&p_epdu_decoded_ptr);
        (*pp_lpp_tx_type)->z_lpp_data_type.p_decode_epdu_ptr = NULL;
      }      
#endif      
      tm_lpp_asn1_osys_IE_free((void**)&p_pdu_decoded_ptr);
      (*pp_lpp_tx_type)->z_lpp_data_type.p_pdu_ptr = NULL;
      break;
        
    case LPP_MESSAGE_REQ_CAP:
      b_ret = tm_lpp_handle_req_cap(p_client, *pp_lpp_tx_type);
#ifdef FEATURE_GNSS_LPPE
      if(p_epdu_decoded_ptr != NULL)
      {
        tm_lpp_e_asn1_osys_IE_free((void**)&p_epdu_decoded_ptr);
        (*pp_lpp_tx_type)->z_lpp_data_type.p_decode_epdu_ptr = NULL;
      }      
#endif
      tm_lpp_asn1_osys_IE_free((void**)&p_pdu_decoded_ptr);
      (*pp_lpp_tx_type)->z_lpp_data_type.p_pdu_ptr = NULL;
      break;
      
    case LPP_MESSAGE_ERROR:
      b_ret = tm_lpp_handle_error(p_client, *pp_lpp_tx_type);
      break;

    case LPP_MESSAGE_ABORT:
      b_ret = tm_lpp_handle_abort(p_client, *pp_lpp_tx_type);
      break;
        
    default:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LPP_TX_MGR: LPP Unknown message type %d", p_pdu_decoded_ptr->lpp_MessageBody.u.c1->t, 0, 0); 
      b_ret = FALSE;
      p_lpp_error->w_ErrType |= LPP_ERROR_TYPE_LPP_ERROR;
      p_lpp_error->u_ErrorErrCode = LPP_ERROR_UNDEF;
      break;
  }


  if (lpp_session_type == TM_LPP_SESSION_TYPE_CP)
  {
     if (b_ret == FALSE)
     {
       p_lpp_error->w_ErrType |= LPP_ERROR_TYPE_LPP_ERROR;
       p_lpp_error->u_ErrorErrCode = LPP_ERROR_INCORRECT_DATA;
     }

     if ( (p_lpp_error->w_ErrType == LPP_ERROR_NO_ERR) && (*pp_lpp_tx_type != NULL) )
     {
       if ( ((*pp_lpp_tx_type)->b_end_sent == TRUE ) &&
            ((*pp_lpp_tx_type)->u_wait_ack == 0) )
       {
         tm_lpp_tx_mgr_remove_entry(pp_lpp_tx_type);
       }
     }
  }
  
  /* For any reason if protocol indicates this transaction should be delete, this is safe place to
     delete it */
  if ( (*pp_lpp_tx_type != NULL) && (1 == (*pp_lpp_tx_type)->m.s_delete_tx) )
  {
    tm_lpp_tx_mgr_remove_entry(pp_lpp_tx_type);
  }

  return ( b_ret );
} /* tm_lpp_tx_mgr_handle_decode_transaction */

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_rli_timer

  Description:
    Function to find the pending transaction item which has matching RLI timer id.

  Parameters:
   *p_timer_param  - pointer to timer information
   client_handle - the client_handle to which this transaction belongs to

  Returns:
    Return the transaction entry for the matching timer id in the transaction table.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_rli_timer( os_TimerExpiryType const *const p_timer_param,
                                                                   uint8 const u_client_handle )
{
  tm_lpp_transaction_info_s_type *p_lpp_tx_elem = NULL;
  tm_lpp_transaction_list_s_type *p_lpp_tx_list = NULL;

  /* Tainted input? */
  if ( NULL == p_timer_param )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Null timer ptr" );
    return ( NULL );
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;

  while ( (p_lpp_tx_list != NULL) )
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;

    /* Find the pending transaction */
    if ( (TRUE == p_lpp_tx_elem->b_pending_ue_tx) && (u_client_handle == p_lpp_tx_elem->u_client_handle ) )
    {
      /* Find given timer id match with RLI timer */
      if ( ( p_lpp_tx_elem->z_lpp_data_type.z_rli.pz_rli_timer != NULL ) &&
           (p_timer_param->q_TimerId == p_lpp_tx_elem->z_lpp_data_type.z_rli.pz_rli_timer->q_TimerId) )
      {
        return ( p_lpp_tx_elem );
      }
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *)p_lpp_tx_list->next;
  }
  return ( NULL );
}

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_gnss_auxi_timer

  Description:
    Function to find the pending transaction item which has matching GNSS timer id.

  Parameters:
   *p_timer_param  - pointer to timer information
   client_handle - the client_handle to which this transaction belongs to

  Returns:
    Return the transaction entry for the matching timer id in the transaction table.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_gnss_auxi_timer( os_TimerExpiryType const * const p_timer_param,
                                                                  uint8 const u_client_handle )
{
  tm_lpp_transaction_info_s_type *p_lpp_tx_elem = NULL;
  tm_lpp_transaction_list_s_type *p_lpp_tx_list = NULL;

  /* Tainted input? */
  if ( NULL == p_timer_param )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Null timer ptr" );
    return ( NULL );
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;

  while ( (p_lpp_tx_list != NULL) )
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;

    /* Find the pending transaction */
    if ( (TRUE == p_lpp_tx_elem->b_pending_ue_tx) && (u_client_handle == p_lpp_tx_elem->u_client_handle ) )
    {
      /* Find given timer id match with GNSS timer */
      if ( ( p_lpp_tx_elem->z_lpp_data_type.z_rli.pz_gnss_auxi_timer != NULL ) &&
           (p_timer_param->q_TimerId == p_lpp_tx_elem->z_lpp_data_type.z_rli.pz_gnss_auxi_timer->q_TimerId) )
      {
        return ( p_lpp_tx_elem );
      }
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *)p_lpp_tx_list->next;
  }
  return ( NULL );
}

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_tech

  Description:
    Function to find the 'Request Location Info' transactions & return the matching transaction item
    which has request technology in the RLI teck mask. Note the requested technology could be same
    or part of RLI tech mask.

  Parameters:
    tech_mask - Requested technology (like PRM, LR, OTDOA, WLAN-AP, DBH etc)
    u_client_handle - Client handle

  Returns:
    Return the transaction item for the matching technology in the transaction table.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_tech( rli_tx_tech_mask const tech_mask,
                                                              uint8 const u_client_handle )
{
  tm_lpp_transaction_info_s_type  *p_lpp_tx_elem = NULL;
  tm_lpp_transaction_list_s_type  *p_lpp_tx_list = NULL;

  /* Tainted input? */
  if ( 0 == tech_mask )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: No tech mask" );
    return NULL;
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;

  while ( (p_lpp_tx_list != NULL) )
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;

    /* Find the given technology is part of RLI tech mask & its pending transaction */
    if ( ((p_lpp_tx_elem->z_lpp_data_type.z_rli.m_tech & tech_mask) != 0) &&
         (TRUE == p_lpp_tx_elem->b_pending_ue_tx) &&
         (u_client_handle == p_lpp_tx_elem->u_client_handle )
        )
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Get tx by tech found tx item 0x%x", p_lpp_tx_elem);
      return ( p_lpp_tx_elem );
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *)p_lpp_tx_list->next;

  }
  return NULL;
}

/*===========================================================================
  tm_lpp_tx_mgr_get_rli_tx_item

  Description:
    This function would iterate through the transaction table & return the list of pending 'Request
    Location Info' transaction item to the caller.

  Parameters:
    pz_rli_txitem [OUT] - Point to list of RLI transaction item.
    cpu_total_rli_txitem[OUT] - Total number of RLI transaction item found & returned by this
                                function.
    u_client_handle[IN] - Client handle

  Returns:
    Return TRUE if atleast one RLI transaction item is present, otherwise FALSE.
=============================================================================*/
boolean tm_lpp_tx_mgr_get_rli_tx_item( tm_lpp_transaction_info_s_type *pz_rli_txitem[MAX_CONCUR_LPP_RLI_TRANSACTIONS],
                                       uint8 * const cpu_total_rli_txitem,
                                       uint8 const u_client_handle )
{
  tm_lpp_transaction_info_s_type *p_lpp_tx_elem = NULL;
  tm_lpp_transaction_list_s_type *p_lpp_tx_list = NULL;
  uint8 u_RliTxIndex = 0;

  if (( NULL == pz_rli_txitem ) || (NULL == cpu_total_rli_txitem))
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Invalid ptr(s)" );
    return ( FALSE );
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;

  while ( (p_lpp_tx_list != NULL) )
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;

    /* Find the pending RLI transaction */
    if ( (TRUE == p_lpp_tx_elem->b_pending_ue_tx) && (u_client_handle == p_lpp_tx_elem->u_client_handle ) &&
         ( TM_LPP_MESG_REQ_LOC_INFO == p_lpp_tx_elem->e_messages_id) )
    {
      /* If input is valid copy over the RLI transaction item */
      pz_rli_txitem[u_RliTxIndex] = p_lpp_tx_elem;
      ++u_RliTxIndex;
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *)p_lpp_tx_list->next;
  }

  *cpu_total_rli_txitem = u_RliTxIndex;
  return ( u_RliTxIndex > 0 );
}

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_by_early_response

  Description:
    Function to find the 'Request Location Info' transactions & return the matching transaction item
    which has early response in the RLI.

  Parameters:
   *p_timer_param  - pointer to timer information
   client_handle - the client_handle to which this transaction belongs to
 
  Returns:
    Return the transaction item for which early response is needed.
    Otherwise return NULL.
=============================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_get_tx_by_early_timer(os_TimerExpiryType const *const p_timer_param,
                                                                    uint8 const u_client_handle )
{
  tm_lpp_transaction_info_s_type *p_lpp_tx_elem = NULL;
  tm_lpp_transaction_list_s_type *p_lpp_tx_list = NULL;

  if ( NULL == p_timer_param )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: NULL timer ptr" );
    return ( NULL );
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;

  while ( (p_lpp_tx_list != NULL) )
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;

    /* Find the pending transaction */
    if ( (TRUE == p_lpp_tx_elem->b_pending_ue_tx) && (u_client_handle == p_lpp_tx_elem->u_client_handle ) )
    {
      /* Find given timer id match with early timer */
      if (( p_lpp_tx_elem->z_lpp_data_type.z_rli.pz_earlyRsp_fetch_timer != NULL ) &&
           (p_timer_param->q_TimerId == p_lpp_tx_elem->z_lpp_data_type.z_rli.pz_earlyRsp_fetch_timer->q_TimerId))
      {
        MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
              "TM_LPP_TX_MGR: Found Early Response RX item - clienthandle %d timerid %lu",
              u_client_handle,p_timer_param->q_TimerId);
        return ( p_lpp_tx_elem );
      }
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *)p_lpp_tx_list->next;
  }
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
        "TM_LPP_TX_MGR: failed to find Early Response RX item - clienthandle %d timerid %lu",
        u_client_handle,p_timer_param->q_TimerId);
  return ( NULL );
}

/*===========================================================================
  tm_lpp_tx_mgr_clear_early_response_tx

  Description:
    Function to find the 'Request Location Info' transactions  with EarlyResponse
    & return clear the flag

  Parameters:
    pz_lpp_tx_item:  Pointer to RLI Tx item to be removed from TX manager queue
  Returns:

=============================================================================*/
void tm_lpp_tx_mgr_remove_lpp_ext_rli( tm_lpp_transaction_info_s_type const * const pz_lpp_tx_item,
                                             const uint8 lpp_ext)
{
  tm_lpp_transaction_list_s_type *p_lpp_tx_curr = NULL,
                                 *p_lpp_tx_prev = NULL,
                                 *p_lpp_tx_next = NULL;
  uint8 u_transaction_id = 0;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: Remove Entries. Num of trans:%d", tm_lpp_tx_summary.num_trans );

  if ( NULL == pz_lpp_tx_item )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: NULL tx item ptr" );
    return;
  }

  p_lpp_tx_prev = p_lpp_tx_curr = tm_lpp_tx_summary.head;

  /* Need to becareful, since p_lpp_tx_curr and pz_lpp_tx_item might be pointing to the same address.  */
  /* If p_lpp_tx_curr is freed, then pz_lpp_tx_item transaction id will be lost. Decided to save trans */
  /* id into local variable and use it in while loop.                                                  */
  u_transaction_id = pz_lpp_tx_item->z_current_trans_id.u_transaction_id;

  while ( (p_lpp_tx_curr != NULL) && (tm_lpp_tx_summary.num_trans > 0) )
  {
    /* Save the next element */
    p_lpp_tx_next = p_lpp_tx_curr->next;

    /*check for Transaction ID match and also LPP Ext callflow bitmap. 
      This ensures that in OTDOA > 24 AD scenario and other Special transactions specified are removed */
    if (((p_lpp_tx_curr->elem.z_lpp_data_type.z_rli.s_lpp_ext & lpp_ext)!=0)&&
         (u_transaction_id == p_lpp_tx_curr->elem.z_current_trans_id.u_transaction_id) )
    {
      /* If the head element is matched for deleting, update the head pointer */
      if ( p_lpp_tx_curr == tm_lpp_tx_summary.head )
      {
        tm_lpp_tx_summary.head = p_lpp_tx_next;
      }
      else
      {
        /* The current node will be deleted. Link the previous & next node */
        p_lpp_tx_prev->next = p_lpp_tx_next;
      }

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_LPP_TX_MGR: FREE LPP Ext TX item 0x%x Tx ID:%d, Seq Num: %u",
              p_lpp_tx_curr,
              p_lpp_tx_curr->elem.z_current_trans_id.u_transaction_id, 
              p_lpp_tx_curr->elem.u_out_sequence_num);


      os_MemFree((void **)&p_lpp_tx_curr);
      tm_lpp_tx_summary.num_trans--;

      /* Restore the current node to next node */
      p_lpp_tx_curr = p_lpp_tx_next;

      continue; /* Continue previous node remains the same since current node is deleted */
    }
    /* Point to previous linklist */
    p_lpp_tx_prev = p_lpp_tx_curr;
    /* Increment current pointer to point to next element */
    p_lpp_tx_curr = p_lpp_tx_next;
  } /* while */

  return;
}

/*===========================================================================
FUNCTION tm_lpp_tx_mgr_add_early_resp_tx

DESCRIPTION
  This function
  1)Duplicates the original RLI tech carrying EarlyReponse Qos in TX MGR.
  2)Copies required fields into new TX item
  3)Unchecks the EarlyResponse LPP_EXT flag in original TX item.


PARAMETERS:
  pz_lpp_tx_item : Pointer to Original LPP Tx item, which will be used to
                   duplicate the transaction
DEPENDENCIES: none.

RETURN VALUE:   p_lpp_tx_rli   : Pointer to duplicate LPP Tx item which will be used to
                send Early Response.
SIDE EFFECTS: none.

===========================================================================*/
tm_lpp_transaction_info_s_type* tm_lpp_tx_mgr_add_early_resp_tx( tm_lpp_transaction_info_s_type* pz_lpp_tx_item)
{
  tm_lpp_transaction_info_s_type *p_lpp_tx_rli     = NULL;

  if ( NULL == pz_lpp_tx_item )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: NULL tx item ptr" );
    return NULL;
  }

  if (0 == (pz_lpp_tx_item->z_lpp_data_type.z_rli.m_tech &
       (C_LPP_TECH_OTDOA_MSA|C_LPP_TECH_GNSS_MSA|C_LPP_TECH_GNSS_MSB)))
  {
    MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_LPP_TX_MGR: RLI Tech not supported with Early Timer",
           pz_lpp_tx_item->z_lpp_data_type.z_rli.m_tech);
    return NULL;
  }

  /*Add an entry into the transaction table to duplicate the original REQ_LOC
    We will send the POC_LOC with the same transaction ID*/
  tm_lpp_tx_mgr_insert_entry( &(pz_lpp_tx_item->z_current_trans_id), &p_lpp_tx_rli, pz_lpp_tx_item->u_client_handle);
  if ( NULL == p_lpp_tx_rli )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_TX_MGR: Can't add RLI entry as transaction" );
    return NULL;
  }
  /* Set Correct MSG_ID and LPP Extension Call flow type*/
  p_lpp_tx_rli->e_messages_id = TM_LPP_MESG_REQ_LOC_INFO;
  p_lpp_tx_rli->b_pending_ue_tx = TRUE;
  p_lpp_tx_rli->z_lpp_data_type.z_rli.s_lpp_ext = C_LPP_EXT_EARLY_RESP;
  p_lpp_tx_rli->z_lpp_data_type.q_flags = 0;

  /*Copy network provided GNSS method*/
  p_lpp_tx_rli->z_lpp_data_type.z_rli.s_gnss_method = pz_lpp_tx_item->z_lpp_data_type.z_rli.s_gnss_method;

  /*Copy rli_tech_mask, filter only MSA, MSB, OTDOA*/
  p_lpp_tx_rli->z_lpp_data_type.z_rli.m_tech =
     ((pz_lpp_tx_item->z_lpp_data_type.z_rli.m_tech)&
      (C_LPP_TECH_GNSS_MSA|C_LPP_TECH_GNSS_MSB|C_LPP_TECH_OTDOA_MSA));

  /*Remove GNSS and RLI timer IDs for duplicate TX item*/
  p_lpp_tx_rli->z_lpp_data_type.z_rli.pz_gnss_auxi_timer = NULL;
  p_lpp_tx_rli->z_lpp_data_type.z_rli.pz_rli_timer  = NULL;
  p_lpp_tx_rli->z_lpp_data_type.z_rli.pz_earlyRsp_fetch_timer = NULL;

  /*Remove Early Response flag from original RLI*/
  pz_lpp_tx_item->z_lpp_data_type.z_rli.s_lpp_ext = C_LPP_EXT_UNDEF;

  MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_LPP_TX_MGR: Early RLI trans added rli_tec: %d", p_lpp_tx_rli->z_lpp_data_type.z_rli.m_tech);

  return p_lpp_tx_rli;
}

/*===========================================================================
  tm_lpp_tx_mgr_get_tx_item_by_clientid

  Description:
    Function to find the list of transaction item associated to a particular client & return those
    to calling function.

  Parameters:
    pz_txitem [OUT] - Point to list of transaction item.
    cpu_total_txitem[OUT] - Total number of transaction item found & returned by this function.
    u_client_handle[IN] - Client handle

  Returns:
    Return TRUE if atleast one RLI transaction item is present, otherwise FALSE.
=============================================================================*/
boolean tm_lpp_tx_mgr_get_tx_item_by_clientid( tm_lpp_transaction_info_s_type *pz_TxItems[TM_LPP_TX_MGR_TRANSACTIONS_MAX],
                                               uint8 *const cpu_TotalTxItems, uint8 const cu_ClientHandle )
{
  tm_lpp_transaction_info_s_type *p_lpp_tx_elem = NULL;
  tm_lpp_transaction_list_s_type *p_lpp_tx_list = NULL;
  uint8 u_TxIndex = 0;

  if (( NULL == pz_TxItems ) || (NULL == cpu_TotalTxItems))
  {
    return ( FALSE );
  }

  p_lpp_tx_list = tm_lpp_tx_summary.head;

  while ( (p_lpp_tx_list != NULL) )
  {
    p_lpp_tx_elem = &p_lpp_tx_list->elem;

    /* Find & append the transaction items into the pointer array */
    if ( (cu_ClientHandle == p_lpp_tx_elem->u_client_handle) && (u_TxIndex < TM_LPP_TX_MGR_TRANSACTIONS_MAX) )
    {
      pz_TxItems[u_TxIndex] = p_lpp_tx_elem;
      ++u_TxIndex;
    }
    p_lpp_tx_list = (tm_lpp_transaction_list_s_type *)p_lpp_tx_list->next;
  }

  *cpu_TotalTxItems = u_TxIndex;
  return ( u_TxIndex > 0 );
}
