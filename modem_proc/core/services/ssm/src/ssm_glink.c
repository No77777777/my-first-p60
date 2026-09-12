/**
@file ssm_glink.c
@brief
This module implements the Secured Services Functionality.
*/
/*=============================================================================
NOTE: The @brief description and any detailed descriptions above do not appear 
      in the PDF. 

      The SSM_mainpage.dox file contains all file/group descriptions that
      are in the output PDF generated using Doxygen and Latex. To edit or 
      update any of the file/group text in the PDF, edit the 
      SSM_mainpage.dox file or contact Tech Pubs.

      The above description for this file is part of the SSM group 
      description in the SSM_mainpage.dox file. 
=============================================================================*/

/*=============================================================================
  Copyright (c) 2012 - 2015 by QUALCOMM Technologies Incorporated. 
  All rights reserved.
  Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================

                 S E C U R E   S E R V I C E S  M O D U L E

GENERAL DESCRIPTION
  Implements SSM IPC

EXTERNALIZED FUNCTIONS

  INITIALIZATION AND SEQUENCING REQUIREMENTS

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/ssm/src/ssm_glink.c#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
02/24/15   din     File created.

=============================================================================*/

/*===========================================================================
                          INCLUDE FILES
===========================================================================*/
#include "ssm_task_v.h"
#include "ssm_glink_v.h"
#include "ssm_ipc_v.h"
#include "ssm_v.h"
#include "queue.h"
#include "glink.h"
#ifdef FEATURE_SSM_REX_API_PRESENT
#include "rex.h"
#else /*  FEATURE_SSM_REX_API_PRESENT */
#include "qurt.h"
#endif /* FEATURE_SSM_REX_API_PRESENT */
#include <stdlib.h>
#include <stringl/stringl.h>

/* Port names */
#define SSM_NAME_MODEM_APPS "SSM_RTR_MODEM_APPS"
#define SSM_NAME_MODEM_ADSP "SSM_RTR_MODEM_ADSP"
#define SSM_NAME_MODEM_RIVA "SSM_RTR_MODEM_RIVA"


/*=============================================================================

                           DATA DEFINITIONS

=============================================================================*/

/* SSM GLINK structure */
typedef struct 
{
  /* Handle for glink channel */
  glink_handle_type   handle;
  
  glink_err_type      open_err;
  
  boolean             ssr;

  /* Id */
  uint16             id;
  
  /* Indicates if this glink handle is valid */
  boolean            valid;

  /* Indicates if this glink handle is ready for transfer */
  boolean            transfer_ready;

  /* Originator proc for this glink handle */
  ssm_proc_enum_t    src_proc;

  /* Destionation proc for this glink handle */
  ssm_proc_enum_t    dest_proc;

} ssm_glink_handle_t;

static ssm_glink_handle_t * ssm_glink_handle_struct;

#ifdef FEATURE_SSM_REX_API_PRESENT
/* Look up pointer for ssm tcb */
static rex_tcb_type* ssm_tcb_ptr=NULL;
#else /* FEATURE_SSM_REX_API_PRESENT */
extern qurt_anysignal_t  ssm_sig_qurt;
#endif /* FEATURE_SSM_REX_API_PRESENT */
qurt_anysignal_t  ssm_glink_sig;

typedef struct 
{
  /* Link for the next queue */
  q_link_type                            link;

  /* Id  */
  uint32                                 id;

  /* Glink read data */
  char *                                 read_data;

  /* Size of data */
  uint16                                 size;

} ssm_queue_node_struct_t;

/* Queue to store client id to be processed */
static q_type                            ssm_glink_id_q;



void ssm_glink_open_channel(uint32 id);



/*=============================================================================

                           FUNCTION DEFINITIONS

=============================================================================*/

/** 
Get GLINK Port Connection Status

@param[in] proc                    Proc whose connection status needs to be determined

@return
TRUE or FALSE

@dependencies
None.

*/

boolean ssm_is_glink_ready
( 
  ssm_proc_enum_t proc 
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ssm_glink_handle_struct == NULL )
    return FALSE;
  
  return ssm_glink_handle_struct[proc].transfer_ready;

} /* ssm_is_glink_ready */


/** 
Handle GLINK read information

@param[in]  handle,      handle for the glink channel 
@param[in]  *priv,       priv client data passed in glink_open 
@param[in]  *pkt_priv,   private client data assiciated with the
                         rx intent that client queued earlier
@param[in]  *ptr,        pointer to the received buffer 
@param[in]  size,        size of the packet 
@param[in]  intent_used  size of the intent used for this packet 

@return
void

@dependencies
None.

*/

static void ssm_glink_handle_rx_glink_context
(
  glink_handle_type handle,     /* handle for the glink channel */
  const void        *priv,      /* priv client data passed in glink_open */
  const void        *pkt_priv,  /* private client data assiciated with the
                                   rx intent that client queued earlier */
  const void        *ptr,       /* pointer to the received buffer */
  size_t            size,       /* size of the packet */
  size_t            intent_used /* size of the intent used for this packet */
)
{

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	
  SSM_MSG_MED (" Function ssm_glink_handle_rx " );

  glink_queue_rx_intent( handle, priv, SSM_GLINK_RX_LENGTH );

  if ( ssm_glink_handle_struct == NULL )
  {
    glink_rx_done( handle, ptr, FALSE);
    return ;
  }

  ssm_queue_node_struct_t * temp_node = malloc( sizeof ( ssm_queue_node_struct_t ) );
	
  if ( temp_node == NULL )
  {
    glink_rx_done( handle, ptr, FALSE);
    return;
  }

  temp_node->id = *((uint16 * )priv);

  temp_node->read_data = malloc(size);

  if ( temp_node->read_data == NULL )
  {
    free(temp_node);
    glink_rx_done( handle, ptr, FALSE);
    return;
  }

  temp_node->size = (uint32)size;
  
  memscpy(temp_node->read_data, temp_node->size, ptr, temp_node->size );
          
  /* Link temp_node to ssm_glink_id_q */
  (void) q_link( temp_node, &temp_node->link );
    
  /* Enqueue node in ssm_glink_id_q */
  q_put( &ssm_glink_id_q, &temp_node->link);

  SSM_MSG_LOW (" glink port read event " );
  #ifdef FEATURE_SSM_REX_API_PRESENT
    rex_set_sigs(ssm_tcb_ptr, SSM_GLINK_READ_SIG );
  #else /* FEATURE_SSM_REX_API_PRESENT */
    qurt_anysignal_set( &ssm_sig_qurt, SSM_GLINK_READ_SIG );
  #endif /* FEATURE_SSM_REX_API_PRESENT */

  glink_rx_done( handle, ptr, FALSE);
  
  return ;

} /* ssm_glink_handle_rx_glink_context */

/** 
Handle GLINK read information

@param[in] proc                    Proc from which buffer should be read

@return
ssm_err_t -- E_SSM_SUCCESS if success, E_SSM_FAIL if failure

@dependencies
None.

*/

ssm_err_t ssm_glink_handle_rx(void)
{
  
  ssm_proc_enum_t proc;

  ssm_queue_node_struct_t * temp_node;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	
  SSM_MSG_MED (" Function ssm_glink_handle_rx " );

  if ( ssm_glink_handle_struct == NULL )
    return E_SSM_FAIL;

  /* Get proc information from the queue */
  temp_node = q_get( &ssm_glink_id_q );
  while ( temp_node != NULL )
  {  
    proc = temp_node->id;

    if ( proc >= SSM_PROC_MODEM && proc < SSM_PROC_MAX )
    {
      /* Check if GLINK is initialized */
      if ( ssm_glink_handle_struct[proc].transfer_ready == TRUE )
      {    
        SSM_MSG_LOW_1 (" Function ssm_glink_handle_rx from PROC %d", proc );

        /* Process read buffer */
        ssm_ipc_decode_ipc_message( proc, temp_node->read_data, temp_node->size );
        
      }
    }

    /* Free up space allocted by queue node */
    free ( temp_node->read_data );
    free ( temp_node );

    temp_node = q_get( &ssm_glink_id_q );
  }
  

  return E_SSM_SUCCESS;

} /* ssm_glink_handle_rx */


/** 
Transfer message through glink

@param[in] proc                    Proc to  which buffer should be transferred
@param[in] write_buffer            Buffer to be written
@param[in] length                  length of the buffer

@return
ssm_err_t -- E_SSM_SUCCESS if success, E_SSM_FAIL if failure

@dependencies
None.

*/

ssm_err_t ssm_glink_handle_tx
( 
  ssm_proc_enum_t proc,
  const void * write_buffer, 
  uint32 length
)
{
  SSM_MSG_MED_2 (" Function ssm_glink write to Proc %d of length %d ", proc, length);

  if ( ssm_glink_handle_struct == NULL )
    return E_SSM_FAIL;

  if ( length >= SSM_GLINK_RX_LENGTH )
    return E_SSM_FAIL;
  
  /* Check if GLINK is initialized */
  if ( ssm_glink_handle_struct[proc].transfer_ready == FALSE )
     return E_SSM_FAIL;
  
  /* Write into glink port */
  if ( GLINK_STATUS_SUCCESS != glink_tx( ssm_glink_handle_struct[proc].handle, 
		     &(ssm_glink_handle_struct[proc].id), write_buffer, 
		     length, GLINK_TX_REQ_INTENT) )
  {
    SSM_MSG_ERROR ("Glink write failed " );
    return E_SSM_FAIL;
  }
  
  /* Wait for write to complete */
  qurt_anysignal_wait( &ssm_glink_sig, SSM_GLINK_TX_DONE_SIG );
  
  return E_SSM_SUCCESS;

} /* ssm_glink_handle_tx */


/** 
Callback from glink to notify transfer completion

@param[in] handle              handle for the glink channel
@param[in] *priv,              priv client data passed in glink_open 
@param[in] *pkt_priv,          private client data assiciated with the
                               tx pkt that client queued earlier 
@param[in] *ptr,               pointer to the transmitted buffer
@param[in] size                size of the packet

@return
none

@dependencies
None.

*/

static void ssm_glink_handle_tx_done
( 
  glink_handle_type handle,    /* handle for the glink channel */
  const void        *priv,     /* priv client data passed in glink_open */
  const void        *pkt_priv, /* private client data assiciated with the
                                  tx pkt that client queued earlier */
  const void        *ptr,      /* pointer to the transmitted buffer */
  size_t            size       /* size of the packet */
)
{
  SSM_MSG_MED (" Function ssm_glink tx done ");

  qurt_anysignal_set( &ssm_glink_sig, SSM_GLINK_TX_DONE_SIG );

} /* ssm_glink_handle_tx_done */


/** 
GLINK event handler

@param[in] handle                  Handle type
@param[in] event                   Type of event ot be handled
@param[in] cb_data                 callback data


@return
None

@dependencies
None.

*/

static void ssm_glink_event_notify
(
  glink_handle_type         handle,
  const void                *cb_data,
  glink_channel_event_type  event
)
{
  uint16 id =0;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
   
  id = *((uint16 * )cb_data);

  /* Analyze Event */
  switch(event)
  {
    case GLINK_CONNECTED:
      glink_queue_rx_intent( handle, cb_data, SSM_GLINK_RX_LENGTH );
      ssm_glink_handle_struct[id].transfer_ready = TRUE;
  
      #ifndef FEATURE_SSM_PERIPHERAL_SERVER_PROC
        qurt_anysignal_set( &ssm_sig_qurt, SSM_PARSE_PERM_FILE_SIG );
      #endif /* FEATURE_SSM_PERIPHERAL_SERVER_PROC */
	    
      SSM_MSG_LOW (" glink port open event " );
      
    break;
    
    case GLINK_LOCAL_DISCONNECTED:
      SSM_MSG_ERROR_1 (" received local disconnect event for id = %d ", id );

      ssm_glink_handle_struct[id].transfer_ready = FALSE;
    break;

    case GLINK_REMOTE_DISCONNECTED:

      SSM_MSG_ERROR_1 (" received remote disconnect event for id = %d ", id );

      ssm_glink_handle_struct[id].transfer_ready = FALSE;
      
      //Close the channel on local side if remote closes
      glink_close(handle);
      
      //Open again for next open if its not ssr
      if(ssm_glink_handle_struct[id].ssr != TRUE)
        ssm_glink_open_channel(id);
    break;
    
    default:
      break;
  }
} /* ssm_glink_event_notify */

/** 
Adds intent if needed

@param[in] handle                  Handle type
@param[in] priv                    callback data
@param[n]  req_size                req_size

@return
boolean  -- TRUE, Intent request is updated

@dependencies
None.

*/

static boolean ssm_glink_notify_rx_intent_req_cb
(
  glink_handle_type     handle,  /* handle for the glink channel */
  const void            *priv,   /* priv client data passed in glink_open */
  size_t                req_size /* Requested size */
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  glink_queue_rx_intent( handle, priv, req_size );

  return TRUE;
}

void ssm_glink_open_channel(uint32 id)
{
  glink_open_config_type glink_cfg= {0};
  glink_err_type glink_err;
  
  glink_cfg.transport            = "SMEM";
  glink_cfg.options              = GLINK_OPT_INITIAL_XPORT;  // Option to enable migration to best channel
  glink_cfg.notify_rx            = ssm_glink_handle_rx_glink_context;
  glink_cfg.notify_rxv           = NULL;
  glink_cfg.notify_tx_done       = ssm_glink_handle_tx_done;
  glink_cfg.notify_state         = ssm_glink_event_notify;
  glink_cfg.notify_rx_intent_req = ssm_glink_notify_rx_intent_req_cb;
  glink_cfg.notify_rx_intent     = NULL;
  glink_cfg.notify_rx_sigs       = NULL;
  glink_cfg.name                 = NULL;

  if(id == SSM_PROC_MODEM)
  {
    glink_cfg.remote_ss            = "mpss";
    glink_cfg.priv                 = (void * ) &(ssm_glink_handle_struct[SSM_PROC_MODEM].id);
    #ifdef FEATURE_SSM_PERIPHERAL_SERVER_PROC
      glink_cfg.name                 = NULL;
    #elif FEATURE_SSM_ADSP_PROC
      glink_cfg.name                 = SSM_NAME_MODEM_ADSP;
    #elif FEATURE_SSM_RIVA_PROC
      glink_cfg.name                 = SSM_NAME_MODEM_RIVA;
    #endif
  }
  else if(id == SSM_PROC_APPS)
  {
    glink_cfg.remote_ss            = "apss";
    glink_cfg.priv                 = (void * ) &(ssm_glink_handle_struct[SSM_PROC_APPS].id);
    #ifdef FEATURE_SSM_PERIPHERAL_SERVER_PROC
      glink_cfg.name                 = SSM_NAME_MODEM_APPS;
    #endif
  }
  else if(id == SSM_PROC_ADSP)
  {
    glink_cfg.remote_ss            = "lpass";
    glink_cfg.priv                 = (void * ) &(ssm_glink_handle_struct[SSM_PROC_ADSP].id);
    #ifdef FEATURE_SSM_PERIPHERAL_SERVER_PROC
      glink_cfg.name                 = SSM_NAME_MODEM_ADSP;
    #endif
  }
  else if(id == SSM_PROC_RIVA)
  {
    glink_cfg.remote_ss            = "wcnss";
    glink_cfg.priv                 = (void * ) &(ssm_glink_handle_struct[SSM_PROC_RIVA].id);
    #ifdef FEATURE_SSM_PERIPHERAL_SERVER_PROC
      glink_cfg.name                 = SSM_NAME_MODEM_RIVA;
    #endif
  }
  else
  {
    SSM_MSG_ERROR_1(" invalid proc id %d for glink_open ", id);
    
    return;
  }

  /* Request GLINK to open a port */
  glink_err = glink_open( &glink_cfg, (void * ) &(ssm_glink_handle_struct[id].handle));
  
  ssm_glink_handle_struct[id].open_err = glink_err;
  
  if(glink_err != GLINK_STATUS_SUCCESS)
  {
    SSM_MSG_ERROR_2(" glink_open failed with err %d for remote proc %d", glink_err, id);
  }
}

/** 
Link State callback function

@param[in] link_info               link state info structure
@param[in] priv                    callback data

@return
void

@dependencies
None.

*/
static void ssm_glink_link_state_cb
(
  glink_link_info_type *link_info, /* Ptr to link information strcuture */
  void* priv                       /* Client private data */
)
{
  if(link_info->link_state == GLINK_LINK_STATE_UP)
  {
    #ifdef FEATURE_SSM_PERIPHERAL_SERVER_PROC
    if(0 == strncmp(link_info->remote_ss, "apss", 32))
    {
      ssm_glink_open_channel(SSM_PROC_APPS);
      ssm_glink_handle_struct[SSM_PROC_APPS].ssr = FALSE;
    }

    if(0 == strncmp(link_info->remote_ss, "lpass", 32))
    {
      ssm_glink_open_channel(SSM_PROC_ADSP);
      ssm_glink_handle_struct[SSM_PROC_ADSP].ssr = FALSE;
    }
    
    if(0 == strncmp(link_info->remote_ss, "wcnss", 32))
    {
      ssm_glink_open_channel(SSM_PROC_RIVA);
      ssm_glink_handle_struct[SSM_PROC_RIVA].ssr = FALSE;
    }
    
    #elif FEATURE_SSM_ADSP_PROC
      
    if(0 == strncmp(link_info->remote_ss, "wcnss", 32))
    {
      ssm_glink_open_channel(SSM_PROC_MODEM);
      ssm_glink_handle_struct[SSM_PROC_MODEM].ssr = FALSE;
    }
    
    #elif FEATURE_SSM_RIVA_PROC
    
    if(0 == strncmp(link_info->remote_ss, "modem", 32))
    {
      ssm_glink_open_channel(SSM_PROC_MODEM);
      ssm_glink_handle_struct[SSM_PROC_MODEM].ssr = FALSE;
    }
    
    #endif
  }
  else
  {
    /* GLINK_LINK_STATE_DOWN case */
    
    #ifdef FEATURE_SSM_PERIPHERAL_SERVER_PROC
    if(0 == strncmp(link_info->remote_ss, "apss", 32))
    {
      ssm_glink_handle_struct[SSM_PROC_APPS].ssr = TRUE;
    }

    if(0 == strncmp(link_info->remote_ss, "lpass", 32))
    {
      ssm_glink_handle_struct[SSM_PROC_ADSP].ssr = TRUE;
    }
    
    if(0 == strncmp(link_info->remote_ss, "wcnss", 32))
    {
      ssm_glink_handle_struct[SSM_PROC_RIVA].ssr = TRUE;
    }
    
    #elif FEATURE_SSM_ADSP_PROC
      
    if(0 == strncmp(link_info->remote_ss, "wcnss", 32))
    {
      ssm_glink_handle_struct[SSM_PROC_MODEM].ssr = TRUE;
    }
    
    #elif FEATURE_SSM_RIVA_PROC
    
    if(0 == strncmp(link_info->remote_ss, "modem", 32))
    {
      ssm_glink_handle_struct[SSM_PROC_MODEM].ssr = TRUE;
    }
    #endif
  }
}



/** 
Initialize glink port

@return
ssm_err_t  -- E_SSM_SUCCESS if success, E_SSM_FAIL if failure

@dependencies
None.

*/

ssm_err_t ssm_glink_init(void)
{
  
  ssm_proc_enum_t iter =0;
  glink_link_id_type link_info;
  glink_err_type glink_err;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	
  qurt_anysignal_init( &ssm_glink_sig );	
  
  /* Initialize task information */
  #ifdef FEATURE_SSM_REX_API_PRESENT
  ssm_tcb_ptr = rcinit_lookup_rextask("ssm");
   
  if ( ssm_tcb_ptr == NULL )
  {
    SSM_ERR_FATAL(" SSM task tcb not found", 0 ,0, 0 );
  }
   
  #endif /* FEATURE_SSM_REX_API_PRESENT */

  ssm_glink_handle_struct = malloc ( sizeof(ssm_glink_handle_t) * SSM_PROC_MAX );

  if ( ssm_glink_handle_struct == NULL )
    return E_SSM_FAIL;

  /* Initialize queue */
  q_init( &ssm_glink_id_q );

  /* Initialize SSM GLINK structure */
  for ( iter = SSM_PROC_MODEM; iter < SSM_PROC_MAX; iter++ )
  {
    ssm_glink_handle_struct[iter].id = (uint16)iter;
    ssm_glink_handle_struct[iter].src_proc = SSM_PROC_CURRENT;
    ssm_glink_handle_struct[iter].dest_proc = iter;

    if ( ssm_glink_handle_struct[iter].src_proc == 
	  ssm_glink_handle_struct[iter].dest_proc )
    {
       ssm_glink_handle_struct[iter].valid = FALSE;  
    }
    else
    {
        ssm_glink_handle_struct[iter].valid = TRUE;
    }
    
    ssm_glink_handle_struct[iter].transfer_ready = FALSE;
    ssm_glink_handle_struct[iter].ssr            = FALSE;
    ssm_glink_handle_struct[iter].open_err       = 0;
  }
  
  #ifdef FEATURE_SSM_PERIPHERAL_SERVER_PROC
  
  //Register with glink for apps link up event
  GLINK_LINK_ID_STRUCT_INIT(link_info);
  
  link_info.xport = "SMEM";
  link_info.remote_ss = "apss";
  link_info.link_notifier = ssm_glink_link_state_cb;
  
  glink_err = glink_register_link_state_cb(&link_info, &(ssm_glink_handle_struct[SSM_PROC_APPS].id));
  if(glink_err != GLINK_STATUS_SUCCESS)
    SSM_MSG_ERROR_1(" GLINK link state registration failed with err %d for apps", glink_err);
  
  //Register with glink for adsp link up event
  GLINK_LINK_ID_STRUCT_INIT(link_info);
  
  link_info.xport = "SMEM";
  link_info.remote_ss = "lpass";
  link_info.link_notifier = ssm_glink_link_state_cb;
  
  glink_err = glink_register_link_state_cb(&link_info, &(ssm_glink_handle_struct[SSM_PROC_ADSP].id));
  if(glink_err != GLINK_STATUS_SUCCESS)
    SSM_MSG_ERROR_1(" GLINK link state registration failed with err %d for lpass", glink_err);
  
  //Register with glink for wcnss link up event
  GLINK_LINK_ID_STRUCT_INIT(link_info);
  
  link_info.xport = "SMEM";
  link_info.remote_ss = "wcnss";
  link_info.link_notifier = ssm_glink_link_state_cb;
  
  glink_err = glink_register_link_state_cb(&link_info, &(ssm_glink_handle_struct[SSM_PROC_RIVA].id));
  if(glink_err != GLINK_STATUS_SUCCESS)
    SSM_MSG_ERROR_1(" GLINK link state registration failed with err %d for lpass", glink_err);
  
  #elif FEATURE_SSM_ADSP_PROC
  //Register with glink for modem link up event
  GLINK_LINK_ID_STRUCT_INIT(link_info);
  
  link_info.xport = "SMEM";
  link_info.remote_ss = "mpss";
  link_info.link_notifier = ssm_glink_link_state_cb;
  
  glink_err = glink_register_link_state_cb(&link_info, &(ssm_glink_handle_struct[SSM_PROC_MODEM].id));
  if(glink_err != GLINK_STATUS_SUCCESS)
    SSM_MSG_ERROR_1(" GLINK link state registration failed with err %d for mpss", glink_err);
  
  #elif FEATURE_SSM_RIVA_PROC
  
  //Register with glink for modem link up event
  GLINK_LINK_ID_STRUCT_INIT(link_info);
  
  link_info.xport = "SMEM";
  link_info.remote_ss = "mpss";
  link_info.link_notifier = ssm_glink_link_state_cb;
  
  glink_err = glink_register_link_state_cb(&link_info, &(ssm_glink_handle_struct[SSM_PROC_MODEM].id));
  if(glink_err != GLINK_STATUS_SUCCESS)
    SSM_MSG_ERROR_1(" GLINK link state registration failed with err %d for mpss", glink_err);
  
  #endif 

  
  return E_SSM_SUCCESS;

} /* ssm_glink_init */
