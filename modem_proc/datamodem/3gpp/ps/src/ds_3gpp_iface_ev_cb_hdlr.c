/*!
  @file
  ds_3gpp_iface_ev_cb_hdlr.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/dev/data.mpss/3.3/vbhanupr.data.mpss.3.3.pw33_11_24/3gpp/ps/src/ds_3gpp_iface_ev_cb_hdlr.c#1
  $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/25/14   vb     iface ev callback handler

===========================================================================*/

#include "ds_3gpp_iface_ev_cb_hdlr.h"
#include "ps_ifacei.h"
#include "ps_ifacei_event.h"

/*-------------------------------------------------------------------------- 
  Structure definitions
  --------------------------------------------------------------------------*/
ds_3gpp_iface_ev_cb_queue_type ds_3gpp_iface_ev_queue
                                            [DS_3GPP_MAX_IFACE_EV_CNT];

/*------------------------------------------------------------------------ 
  Structures to hold iface reg/dereg events and their count.
  This will always be freed dynamically after registration/deregistration
  ------------------------------------------------------------------------*/
typedef struct
{
  uint8                    num_iface_ev;
  ps_iface_event_enum_type ds_3gpp_iface_ev_list_p[DS_3GPP_MAX_IFACE_EV_CNT];
}ds_3gpp_iface_reg_dereg_list_type;

typedef struct 
{
  ps_iface_type				*this_iface_ptr;
  ps_iface_event_enum_type		 ps_event;
  ps_iface_event_info_u_type		 event_info;
  void					*user_data_ptr;
}ds_3gpp_iface_event_info_type;

/*-------------------------------------------------------------------------- 
  Buffer for iface registrations
  --------------------------------------------------------------------------*/
static void *ds_3gpp_iface_ev_buf_ptr = NULL;



/*===========================================================================
FUNCTION  DS_3GPP_MAP_PS_IFACE_EV_TO_DS

DESCRIPTION
  This function converts PS iface event to the registered PS iface events
  
PARAMETERS 
 
DEPENDENCIES
  None

RETURN VALUE
  ds_3gpp_iface_ev_list 

SIDE EFFECTS
  None

===========================================================================*/
static ds_3gpp_iface_ev_enum_type ds_3gpp_map_ps_iface_ev_to_ds
(
 ps_iface_event_enum_type  ps_event
);

/*===========================================================================
FUNCTION DS_3GPP_IFACE_ALLOC_EV_CB_DATA

DESCRIPTION 
    This function allocates the ev callback for the specified iface event

 
PARAMETERS
     iface_ev  - iface event where memory needs to be allocated
 
DEPENDENCIES
    None.

RETURN VALUE 
    int       -1       if unsuccessful
                       allocated_index if successful
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

static int ds_3gpp_iface_alloc_ev_cb_data
(
  ds_3gpp_iface_ev_enum_type iface_ev,
  sys_modem_as_id_e_type     subs_id   
)
{
  int     ret_val              = -1;
  uint8   cb_cnt               = 0;
  ds_3gpp_iface_ev_cb_data_type   *iface_ev_cb_p = NULL;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(iface_ev >= DS_3GPP_MAX_IFACE_EV_CNT)
  {
    ret_val = -1;
    //"iface_ev:%d passed is INVALID",iface_ev);
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  /*----------------------------------------------------------------------
   Check iface_ev_buf_ptr. If memory is not allocated, Allocate memory from
   the heap, This memory is never freed
  ------------------------------------------------------------------------*/
  if(ds_3gpp_iface_ev_buf_ptr == NULL)
  {
    DS_3GPP_MSG0_HIGH_EX(subs_id, "Allocating iface reg buf");
    ds_3gpp_iface_ev_buf_ptr =  ps_iface_alloc_event_handle(
                                                    NULL,
                                                    ds_3gpp_iface_ev_cback,
                                                    NULL);
    if(ds_3gpp_iface_ev_buf_ptr == NULL)
    {
      DS3GPP_ASSERT(0);
      ret_val = -1;
      //"iface ev buf ptr allocation failed");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }
  }

  for(cb_cnt = 0; cb_cnt < DS_3GPP_IFACE_MAX_CB_CNT;cb_cnt++)
  {
    iface_ev_cb_p = ds_3gpp_iface_ev_queue[iface_ev].iface_ev_cb_data_ptr
                                                                      [cb_cnt];

    if(iface_ev_cb_p != NULL)
    {
      continue;
    }

    iface_ev_cb_p = (ds_3gpp_iface_ev_cb_data_type *)modem_mem_alloc
                    (sizeof(ds_3gpp_iface_ev_cb_data_type),
                    MODEM_MEM_CLIENT_DATA);

    if(iface_ev_cb_p == NULL)
    {
      ret_val = -1;
      //"Heap allocation error");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      goto func_exit;
    }

    ds_3gpp_iface_ev_queue[iface_ev].iface_ev_cb_data_ptr[cb_cnt] = 
                                                                iface_ev_cb_p;

    DS_3GPP_MSG1_MED_EX(subs_id, "Iface Ev cb data allocated at %d",cb_cnt);
    ret_val = cb_cnt;
    break;
  }

  if(cb_cnt >= DS_3GPP_IFACE_MAX_CB_CNT)
  {
    ret_val = -1;
    //"All entries are allocated:%d",cb_cnt);
    local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
    goto func_exit;
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "ds_3gpp_iface_alloc_ev_cb_data: err d",
                           local_err);
  }

  return ret_val;

} /*ds_3gpp_iface_alloc_ev_cb_data*/

/*===========================================================================
FUNCTION DS_3GPP_IFACE_IS_EVENT_REG_DEREG

DESCRIPTION 
    This function will determine if the iface_ev_cb_hdlr will need to
    subscribe events with the FRAMEWORK. If an iface event has already been
    subscribed then there is no need to subscribe again

 
PARAMETERS
    ps_iface_event_enum_type   - iface event on which registration needs to happen
    *cb_fptr;                  - callback function
    ds_3gpp_iface_ev_func      - subscribe or unsubscribe
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/
static boolean ds_3gpp_iface_is_event_reg_dereg
(
  ps_iface_event_enum_type         *iface_ev_p,
  uint8                            iface_ev_cnt,
  ds_3gpp_iface_reg_enum_type      iface_ev_reg,
  sys_modem_as_id_e_type           subs_id  
)
{
  boolean ret_val = FALSE;
  uint8   tmp_iface_ev_cnt = 0;
  ds_3gpp_iface_ev_enum_type iface_ev = DS_3GPP_IFACE_MAX_EV;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for(tmp_iface_ev_cnt = 0;tmp_iface_ev_cnt < iface_ev_cnt; tmp_iface_ev_cnt++)
  {
    if(iface_ev_p == NULL)
    {
      DS_3GPP_MSG0_LOW("IFACE_EV_P is NULL");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      ret_val = FALSE;
      goto func_exit;
    }

    iface_ev = ds_3gpp_map_ps_iface_ev_to_ds(*iface_ev_p);

    if(iface_ev >= DS_3GPP_MAX_IFACE_EV_CNT)
    {
      DS_3GPP_MSG1_LOW("iface_ev:%d passed is INVALID",iface_ev);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      ret_val = FALSE;
      goto func_exit;
    }

    /*--------------------------------------------------------------------- 
      For subscription -- Only if the active cb count is 1 does DS need to
      register with the framework else there is no need
      To unsubscribe - Only if the active cb count is 0 does DS need to
      deregister with the Framework
      ---------------------------------------------------------------------*/

    if(((ds_3gpp_iface_ev_queue[iface_ev].active_cb_cnt == 1) && 
        (iface_ev_reg == DS_3GPP_IFACE_EV_REGISTER)) ||
        ((ds_3gpp_iface_ev_queue[iface_ev].active_cb_cnt == 0) && 
        (iface_ev_reg == DS_3GPP_IFACE_EV_DEREGISTER)))
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id,
                     "Iface Event registration/deregistration needed:%d",
                     iface_ev_reg);

      ret_val = TRUE;
      break;
    }

    iface_ev_p++;
  }

func_exit:
  if(ret_val == FALSE)
  {
    DS_3GPP_ERROR_LOG_2_EX(subs_id, 
                   "iface_is_event_reg_dereg: err d -- iface ev p null/"
                   "iface_ev INVALID", local_err, iface_ev_p);
  }

  return ret_val;
}

/*===========================================================================
FUNCTION DS_3GPP_IFACE_BUILD_EV_REG_DEREG_LIST

DESCRIPTION 
    This function will build the iface events that need to be subscribed
    with PS framework

 
PARAMETERS
    ps_iface_event_enum_type           - iface event on which registration needs to happen
    *cb_fptr;                          - callback function
    ds_3gpp_iface_ev_func              - SUBScribe or UNSUBSCRIBE
    ds_3gpp_iface_reg_dereg_list_type  *iface_reg_dereg_list
 
DEPENDENCIES
    None.

RETURN VALUE 
 void 
   
SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_iface_build_ev_reg_dereg_list
(
  ps_iface_event_enum_type           *iface_ev_p,
  uint8                              iface_ev_cnt,
  ds_3gpp_iface_reg_enum_type        iface_ev_reg,
  ds_3gpp_iface_reg_dereg_list_type *iface_reg_dereg_list
)
{
  int     num_events  = 0;
  uint8   tmp_iface_ev_cnt = 0;
  ds_3gpp_iface_ev_enum_type ds_iface_ev = DS_3GPP_IFACE_MAX_EV;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(iface_ev_cnt > DS_3GPP_IFACE_MAX_EV)
  {
    DS_3GPP_MSG1_LOW("Invalid iface cnt:%d",iface_ev_cnt);
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  if(iface_reg_dereg_list == NULL)
  {
    DS_3GPP_MSG0_LOW("iface_reg_dereg_list is NULL");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto func_exit;
  }

  for(tmp_iface_ev_cnt = 0;tmp_iface_ev_cnt < iface_ev_cnt; tmp_iface_ev_cnt++)
  {
    if(iface_ev_p == NULL)
    {
      DS_3GPP_MSG0_LOW("IFACE_EV_P is NULL");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      goto func_exit;
    }

    ds_iface_ev = ds_3gpp_map_ps_iface_ev_to_ds(*iface_ev_p);

    if(ds_iface_ev >= DS_3GPP_MAX_IFACE_EV_CNT)
    {
      DS_3GPP_MSG1_LOW("iface_ev:%d passed is INVALID",ds_iface_ev);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      goto func_exit;
    }

    if(((ds_3gpp_iface_ev_queue[ds_iface_ev].active_cb_cnt == 1) &&
        (iface_ev_reg == DS_3GPP_IFACE_EV_REGISTER)) ||
        ((ds_3gpp_iface_ev_queue[ds_iface_ev].active_cb_cnt == 0) && 
        (iface_ev_reg == DS_3GPP_IFACE_EV_DEREGISTER)))
    {
      iface_reg_dereg_list->ds_3gpp_iface_ev_list_p[num_events] = *iface_ev_p;
      num_events++;
    }

    iface_ev_p++;
  }

  DS_3GPP_MSG1_HIGH("Number of events filled is %d",num_events);
  iface_reg_dereg_list->num_iface_ev = num_events;

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1("iface_build_ev_reg_dereg_list: err d -- ev cnt/"
                       "list null/evt p null/iface ev", local_err);
  }

  return;
} /*ds_3gpp_iface_build_ev_reg_dereg_list*/

/*===========================================================================
FUNCTION  DS_3GPP_IFACE_DISPATCH_EVENTS

DESCRIPTION
  This function gets called when any iface event is registered
  Check if there is a need to dispatch iface events.
  
PARAMETERS 
  ps_iface_event_enum_type            *iface_ev_p,
  uint8                                iface_ev_cnt,
  ps_iface_type                       *iface_ptr

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void ds_3gpp_iface_dispatch_events
(
  ps_iface_event_enum_type            *iface_ev_p,
  uint8                                iface_ev_cnt,
  ps_iface_type                       *iface_ptr
)
{
  uint8 temp_ev_cnt = 0;
  ps_iface_event_info_u_type event_info;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type           subs_id = SYS_MODEM_AS_ID_NONE;  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*----------------------------------------------------------------------- 
    For each of the iface subscriptions received check if CB's
    have to be dispatched. Since MH registers with PS framework as NULL,
    If the iface is already in that state we will need to dispatch the event
    since Framework will not do this for NULL subscriptions.
    -----------------------------------------------------------------------*/

  if(iface_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("iface ptr is NULL");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  subs_id = ds3gsubsmgr_subs_id_ds_to_cm(PS_IFACE_GET_SUBS_ID(iface_ptr));

  memset(&event_info,0,sizeof(ps_iface_event_info_u_type));

  for(temp_ev_cnt = 0; temp_ev_cnt < iface_ev_cnt; temp_ev_cnt++)
  {
    if(iface_ev_p == NULL)
    {
      DS_3GPP_MSG0_LOW("iface_ev_p is NULL");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }

    switch(*iface_ev_p)
    {
    /*-----------------------------------------------------------------------
                           IFACE_FLOW_ENABLED_EV
      -----------------------------------------------------------------------*/
      case IFACE_FLOW_ENABLED_EV:
        if(iface_ptr->iface_private.tx_flow_mask == 0)
        {
          event_info.flow_mask = 0;
          ds_3gpp_iface_ev_cback(iface_ptr,
                                 *iface_ev_p,
                                 event_info,
                                 NULL);
        }
        break;

      /*-----------------------------------------------------------------------
                            IFACE_FLOW_DISABLED_EV
      -----------------------------------------------------------------------*/
      case IFACE_FLOW_DISABLED_EV:
        if(iface_ptr->iface_private.tx_flow_mask != 0)
        {
          event_info.flow_mask = iface_ptr->iface_private.tx_flow_mask;
          ds_3gpp_iface_ev_cback(iface_ptr,
                                 *iface_ev_p,
                                 event_info,
                                 NULL);
        }
        break;

      /*-----------------------------------------------------------------------
                            IFACE_UP_EV
      -----------------------------------------------------------------------*/
      case IFACE_UP_EV:
        if(iface_ptr->iface_private.state == IFACE_UP)
        {
          event_info.iface_up_info.state = IFACE_UP;
          ds_3gpp_iface_ev_cback(iface_ptr,
                                 *iface_ev_p,
                                 event_info,
                                 NULL);
        }
        break;

      /*-----------------------------------------------------------------------
                            IFACE_DOWN_EV
      -----------------------------------------------------------------------*/
      case IFACE_DOWN_EV:
        if(iface_ptr->iface_private.state == IFACE_DOWN)
        {
          event_info.iface_up_info.state = IFACE_DOWN;
          ds_3gpp_iface_ev_cback(iface_ptr,
                                 *iface_ev_p,
                                 event_info,
                                 NULL);
        }
        break;

      /*-----------------------------------------------------------------------
                            IFACE_OUT_OF_USE_EV
      -----------------------------------------------------------------------*/
      case IFACE_ACTIVE_OUT_OF_USE_EV:
        if((PS_IFACE_GET_REF_CNT(iface_ptr) == 0) &&
           (PS_IFACE_IS_UP(iface_ptr)) &&
           (PS_IFACE_GET_IS_ACTIVE(iface_ptr)))
        {
            ds_3gpp_iface_ev_cback(iface_ptr,
                                   *iface_ev_p,
                                   event_info,
                                   NULL);
        }
        break;

      default:
        DS_3GPP_MSG0_LOW("Invalid iface ev");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
        goto func_exit;
        break;
    }
    iface_ev_p++;
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, 
                           "iface_dispatch_events: err d -- iface ptr/evt ptr/"
                           "invalid ev", local_err);
  }

  return;
} /*ds_3gpp_iface_dispatch_events*/


/*===========================================================================
FUNCTION DS_3GPP_IFACE_REG_EV

DESCRIPTION 
    This function is to be called when MH modules need to subscribe for a
    3gpp iface event

 
PARAMETERS
    ps_iface_event_enum_type - iface event on which registration needs to happen
    *cb_fptr;                - callback function
    *iface_ptr;              - iface_ptr
    *user_data_ptr;          - user_data_ptr
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_iface_reg_ev
(
  ps_iface_event_enum_type        *iface_ev_p,
  uint8                           iface_ev_cnt,
  ps_iface_event_cback_type       cb_fptr,
  ps_iface_type                   *iface_ptr,
  void                            *user_data_ptr
)
{
  boolean                            ret_val = FALSE;
  int8                               cb_cnt  = -1;
  ds_3gpp_iface_ev_enum_type         ds_iface_ev = DS_3GPP_MAX_IFACE_EV_CNT;
  uint8                              iface_tmp_ev_cnt = 0;
  ps_iface_event_enum_type          *iface_tmp_ev_p  = iface_ev_p;
  ds_3gpp_iface_reg_dereg_list_type  iface_reg_dereg_list;
  ds_3gppi_local_err_val_e_type      local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type             subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /*------------------------------------------------------------------------ 
   Check for cb_ptr here, If the cb_ptr is NULL return
  ------------------------------------------------------------------------*/
  if(cb_fptr == NULL)
  {
    DS_3GPP_MSG0_LOW("CB function ptr is NULL");
    ret_val = FALSE;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }
  subs_id = ds3gsubsmgr_subs_id_ds_to_cm(PS_IFACE_GET_SUBS_ID(iface_ptr));

  /*----------------------------------------------------------------------- 
    For every event passed, allocate cb_data and store the corresponding
    iface_ptr, user_data and cb_ptr
  ------------------------------------------------------------------------*/

  for(iface_tmp_ev_cnt = 0;iface_tmp_ev_cnt < iface_ev_cnt; iface_tmp_ev_cnt++)
  {

    if(iface_tmp_ev_p == NULL)
    {
      DS_3GPP_MSG0_LOW("Iface_ev ptr is INVALID");
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }

    ds_iface_ev = ds_3gpp_map_ps_iface_ev_to_ds(*iface_tmp_ev_p);

    if(ds_iface_ev >= DS_3GPP_MAX_IFACE_EV_CNT)
    {
      DS_3GPP_MSG1_LOW("iface_ev:%d passed is INVALID",ds_iface_ev);
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      goto func_exit;
    }

    /*------------------------------------------------------------------------ 
      Check if the ev is already registered with the same cb_fn and
      user_data_ptr. If it is then there is no need to register again
      However if the registration happens on the NULL iface, we will register
      again.
     ------------------------------------------------------------------------*/

    for(cb_cnt = 0;cb_cnt < DS_3GPP_IFACE_MAX_CB_CNT;cb_cnt++)
    {
      if(( ds_3gpp_iface_ev_queue[ds_iface_ev].
           iface_ev_cb_data_ptr[cb_cnt] != NULL)
           && (ds_3gpp_iface_ev_queue[ds_iface_ev].
           iface_ev_cb_data_ptr[cb_cnt]->cb_fptr == cb_fptr) && 
          (ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr
          [cb_cnt]->user_data_ptr == user_data_ptr) &&
          (ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr
          [cb_cnt]->iface_ptr == iface_ptr) &&
          iface_ptr != NULL)
      {
        DS_3GPP_MSG0_HIGH_EX(subs_id, "Reg ev with the same userdata ptr already present:");
        return ret_val;
      }
    } 

    /*------------------------------------------------------------------------- 
      Allocate the cb data for the iface event received
    -------------------------------------------------------------------------*/
    cb_cnt = ds_3gpp_iface_alloc_ev_cb_data(ds_iface_ev, subs_id);

    if(cb_cnt < 0)
    {
      DS_3GPP_MSG0_LOW("Failure to allocate callback memory");
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      goto func_exit;
    }

    /*-------------------------------------------------------------------------- 
      CB data has been allocated at cb_cnt index
      Now save the contents received and increment the cb_data count
    --------------------------------------------------------------------------*/
    ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr[cb_cnt]->cb_fptr
                                                                       = cb_fptr;

    ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr[cb_cnt]->iface_ptr 
                                                                     = iface_ptr;

    ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr[cb_cnt]->user_data_ptr
                                                                  = user_data_ptr;

    ds_3gpp_iface_ev_queue[ds_iface_ev].active_cb_cnt++;

    iface_tmp_ev_p++;

  }

  /*---------------------------------------------------------------------------- 
    Only if iface_tmp_ev_cnt is the same as iface_ev_cnt the for loop has been
    successful, If registration is successful then check if DS needs to subscribe
    to iface events with PS framework
    ----------------------------------------------------------------------------*/

   DS_3GPP_MSG0_HIGH_EX(subs_id, "Registration of all events successful");
   ret_val = TRUE;

   if(ds_3gpp_iface_is_event_reg_dereg(iface_ev_p,
                                       iface_ev_cnt,
                                       DS_3GPP_IFACE_EV_REGISTER,
                                       subs_id))
   {
     
     ds_3gpp_iface_build_ev_reg_dereg_list(iface_ev_p,
                                           iface_ev_cnt,
                                           DS_3GPP_IFACE_EV_REGISTER,
                                          &iface_reg_dereg_list);
                                                              
    if((iface_reg_dereg_list.num_iface_ev > 0) && 
       (iface_reg_dereg_list.num_iface_ev <= DS_3GPP_MAX_IFACE_EV_CNT) &&
       (ds_3gpp_iface_ev_buf_ptr != NULL))
    {
      if(ps_iface_subscribe_event_list(ds_3gpp_iface_ev_buf_ptr,
                                       iface_reg_dereg_list.ds_3gpp_iface_ev_list_p,
                                       iface_reg_dereg_list.num_iface_ev) == -1)
      {
        DS_3GPP_MSG0_LOW("Iface Reg event unsuccessful");
        ret_val = FALSE;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
        goto func_exit;
      }
    }
    else
    {
      ret_val = FALSE;
    }
  }
  /*------------------------------------------------------------------------------- 
    Dispatch events only if registration was successful
    -------------------------------------------------------------------------------*/

  if(ret_val)
  {
    ds_3gpp_iface_dispatch_events(iface_ev_p,
                                  iface_ev_cnt,
                                  iface_ptr);
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id,
                           "iface_reg_ev: err d -- cb ptr/ev ptr/ev/mem allo/"
                           "reg fail", local_err);
  }

  return ret_val;
} /*ds_3gpp_iface_reg_ev*/

/*===========================================================================
FUNCTION DS_3GPP_IFACE_DEREG_EV

DESCRIPTION 
    This function is to be called when MH modules need to unsubscribe for a
    3gpp iface event

 
PARAMETERS
    ds_3gpp_iface_ev_list  - iface event on which registration needs to happen
    uint8                    iface_ev_cnt
    cb_fptr;               - callback function
    ps_iface_type            *iface_ptr,
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_iface_dereg_ev
(
  ps_iface_event_enum_type        *iface_ev_p,
  uint8                            iface_ev_cnt,
  ps_iface_event_cback_type        cb_fptr,
  ps_iface_type                   *iface_ptr
)
{
  boolean                           ret_val = FALSE;
  uint8                             cb_cnt  = 0;
  ps_iface_event_cback_type         cb_tmp_fptr = NULL;
  uint8                             iface_tmp_ev_cnt = 0; 
  ps_iface_event_enum_type         *iface_tmp_ev_p = iface_ev_p;
  ds_3gpp_iface_ev_enum_type        ds_iface_ev = DS_3GPP_IFACE_MAX_EV;
  ds_3gpp_iface_reg_dereg_list_type iface_reg_dereg_list;
  ds_3gppi_local_err_val_e_type     local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type            subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  subs_id = ds3gsubsmgr_subs_id_ds_to_cm(PS_IFACE_GET_SUBS_ID(iface_ptr));

  /*------------------------------------------------------------------------- 
    Loop through to deregister all the required events
   -------------------------------------------------------------------------*/
  for(iface_tmp_ev_cnt = 0;iface_tmp_ev_cnt < iface_ev_cnt;iface_tmp_ev_cnt++)
  {

    if(iface_tmp_ev_p == NULL)
    {
      DS_3GPP_MSG0_LOW("Iface ev ptr is NULL");
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      goto func_exit;
    }

    ds_iface_ev = ds_3gpp_map_ps_iface_ev_to_ds(*iface_tmp_ev_p);

    if(ds_iface_ev >= DS_3GPP_MAX_IFACE_EV_CNT)
    {
      DS_3GPP_MSG1_LOW("iface_ev:%d passed is INVALID",ds_iface_ev);
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }

    for(cb_cnt = 0;cb_cnt < DS_3GPP_IFACE_MAX_CB_CNT; cb_cnt++)
    {
      if(ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr[cb_cnt] == NULL)
      {
        continue;
      }

      cb_tmp_fptr = ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr[cb_cnt]->cb_fptr;

      if((cb_tmp_fptr == NULL) || (cb_tmp_fptr != cb_fptr)) 
      {
        continue;
      }

      /*--------------------------------------------------------------------- 
        Now we have a match by function ptr.
        If iface_ptr is not NULL, then perform iface comparision too to find
        the match
        If iface_ptr is NULL then go ahead and delete the entry.
      --------------------------------------------------------------------*/
      if((iface_ptr != NULL) && 
         (iface_ptr != ds_3gpp_iface_ev_queue[ds_iface_ev].
          iface_ev_cb_data_ptr[cb_cnt]->iface_ptr))
      {
        DS_3GPP_ERROR_LOG_2_EX(subs_id, "Iface mismatch:iface_ptr:0xx,iface_q_ptr:0xx",
                           iface_ptr, ds_3gpp_iface_ev_queue[ds_iface_ev].
                           iface_ev_cb_data_ptr[cb_cnt]->iface_ptr);
        continue;
      }

      /*---------------------------------------------------------------------- 
        Match found, Now deallocate the cb_data and reduce the cb_cnt
       ----------------------------------------------------------------------*/
      modem_mem_free(ds_3gpp_iface_ev_queue[ds_iface_ev].
                                            iface_ev_cb_data_ptr[cb_cnt],
                                            MODEM_MEM_CLIENT_DATA);

      ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr[cb_cnt] = NULL;

      ds_3gpp_iface_ev_queue[ds_iface_ev].active_cb_cnt--;

      DS_3GPP_MSG2_HIGH_EX(subs_id,
                "Active callback count for iface_ev:%d is %d",ds_iface_ev,
                ds_3gpp_iface_ev_queue[ds_iface_ev].active_cb_cnt);

      break;

    }

    if(cb_cnt >= DS_3GPP_IFACE_MAX_CB_CNT)
    {
      DS_3GPP_MSG0_LOW("Deregistration failed");
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      goto func_exit;
    }
    else
    {
      ret_val = TRUE;
    }

    iface_tmp_ev_p++;

  }

  if(ret_val != TRUE)
  {
    DS_3GPP_MSG0_LOW("Deregistration failure");
    ret_val = FALSE;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
    goto func_exit;
  }


  if(ds_3gpp_iface_is_event_reg_dereg(iface_ev_p,
                                      iface_ev_cnt,
                                      DS_3GPP_IFACE_EV_DEREGISTER,
                                      subs_id))
  {
    ds_3gpp_iface_build_ev_reg_dereg_list(iface_ev_p,
                                          iface_ev_cnt,
                                          DS_3GPP_IFACE_EV_DEREGISTER,
                                          &iface_reg_dereg_list);
  /*---------------------------------------------------------------------------- 
    Only if iface_tmp_ev_cnt is the same as iface_ev_cnt the for loop has been
    successful, If deregistration is successful then check if DS needs to unsubscribe
    to iface events with PS framework
    ----------------------------------------------------------------------------*/
    if((iface_reg_dereg_list.num_iface_ev > 0) && 
	   (iface_reg_dereg_list.num_iface_ev <= DS_3GPP_MAX_IFACE_EV_CNT) &&
       (ds_3gpp_iface_ev_buf_ptr != NULL))
    {
      if(ps_iface_unsubscribe_event_list(ds_3gpp_iface_ev_buf_ptr,
                                         iface_reg_dereg_list.ds_3gpp_iface_ev_list_p,
                                         iface_reg_dereg_list.num_iface_ev) == -1)
      {
        DS_3GPP_MSG0_LOW("Iface deReg event unsuccessful");
        ret_val = FALSE;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
        goto func_exit;
      }
      else
      {
        DS_3GPP_MSG0_LOW("Iface deReg event successful");
        ret_val = TRUE;
      }

    }
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, 
                    "iface_dereg_ev: err d -- iface ev ptr/ev/dereg fail...",
                    local_err);
  }

  return ret_val;
} /*ds_3gpp_iface_dereg_ev*/

/*===========================================================================
FUNCTION  DS_3GPP_MAP_PS_IFACE_EV_TO_DS

DESCRIPTION
  This function converts PS iface event to the registered PS iface events
  
PARAMETERS 
 
DEPENDENCIES
  None

RETURN VALUE
  ds_3gpp_iface_ev_list 

SIDE EFFECTS
  None

===========================================================================*/
static ds_3gpp_iface_ev_enum_type ds_3gpp_map_ps_iface_ev_to_ds
(
 ps_iface_event_enum_type  ps_event
)
{
  ds_3gpp_iface_ev_enum_type ds_3gpp_iface_ev = DS_3GPP_IFACE_MAX_EV;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  switch(ps_event)
  {
    case IFACE_UP_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_UP_EV;
      break;

    case IFACE_DOWN_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_DOWN_EV;
      break;

    case IFACE_ROUTEABLE_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_ROUTEABLE_EV;
      break;

    case IFACE_FLOW_ENABLED_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_FLOW_ENABLED_EV;
      break;

    case IFACE_FLOW_DISABLED_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_FLOW_DISABLED_EV;
      break;

    case IFACE_PREFIX_UPDATE_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_PREFIX_UPDATE_EV;
      break;
         
    case IFACE_ACTIVE_OUT_OF_USE_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_ACTIVE_OUT_OF_USE_EV;
      break;

    case IFACE_BEARER_TECH_CHANGED_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_BEARER_TECH_CHANGED_EV;
      break;

    case IFACE_EMBMS_TMGI_ACTIVATED_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_EMBMS_TMGI_ACTIVATED_EV;
      break;

    case IFACE_EMBMS_TMGI_DEACTIVATED_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_EMBMS_TMGI_DEACTIVATED_EV;
      break;

    case IFACE_EMBMS_TMGI_ACTIVATE_DEACTIVATE_RESPONSE_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_EMBMS_TMGI_ACTIVATE_DEACTIVATE_RESPONSE_EV;
      break;

    case IFACE_EMBMS_ACTIVE_TMGI_LIST_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_EMBMS_ACTIVE_TMGI_LIST_EV;
      break;

    case IFACE_EMBMS_AVAILABLE_TMGI_LIST_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_EMBMS_AVAILABLE_TMGI_LIST_EV;
      break;

    case IFACE_EMBMS_AVAILABLE_SAI_LIST_EV:
      ds_3gpp_iface_ev = DS_3GPP_IFACE_EMBMS_AVAILABLE_SAI_LIST_EV;
      break;

    default:
      DS_3GPP_MSG0_HIGH("Invalid iface ev");
  }

  return ds_3gpp_iface_ev;

}

/*===========================================================================
FUNCTION  DS_3GPP_IFACE_EV_CBACK

DESCRIPTION
  This function gets called when any iface event is posted from PS framework
  and posts a command to DS task.
  
PARAMETERS 
  this_iface_ptr:   Iface on which up event was posted
  event:            Event that caused the cback to be invoked
  event_info:       Event info posted by PS
  user_data_ptr:    Data to be passed to the cback function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_iface_ev_cback
(
  ps_iface_type                           *this_iface_ptr,
  ps_iface_event_enum_type                 ps_event,
  ps_iface_event_info_u_type               event_info,
  void                                    *user_data_ptr
)
{
  ds_3gpp_iface_ev_enum_type       ds_iface_ev = DS_3GPP_IFACE_MAX_EV;
  ds_cmd_type                     *cmd_ptr = NULL;
  ds_3gpp_iface_event_info_type   *event_info_ptr = NULL;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type           subs_id = SYS_MODEM_AS_ID_NONE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  do
  {
    if(this_iface_ptr == NULL)
    {
      DS_3GPP_MSG0_LOW("Event received on a NULL Ptr");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
    }

    subs_id = ds3gsubsmgr_subs_id_ds_to_cm(
                    PS_IFACE_GET_SUBS_ID(this_iface_ptr));

    ds_iface_ev = ds_3gpp_map_ps_iface_ev_to_ds(ps_event);

    if(ds_iface_ev == DS_3GPP_IFACE_MAX_EV)
    {
      DS_3GPP_MSG0_LOW("Invalid 3gpp Event received");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
    }
  } while(0);

  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, 
        "iface_ev_cback: err d -- iface ptr/evt", local_err);
    return;
  }
/*-------------------------------------------------------------------------
    Allocate the command buffer 
  -------------------------------------------------------------------------*/
  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_iface_event_info_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id,
        "ds_3gpp_iface_ev_cback():Couldnt allocate the command buffer");
    return ;
  }
  /*-------------------------------------------------------------------------
    Construct the command buffer
  -------------------------------------------------------------------------*/
  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_INVOKE_IFACE_EV_CB_HDLR;
  event_info_ptr = (ds_3gpp_iface_event_info_type*)cmd_ptr->cmd_payload_ptr;

  event_info_ptr->this_iface_ptr = this_iface_ptr;
  event_info_ptr->ps_event = ps_event;
  event_info_ptr->event_info = event_info;
  event_info_ptr->user_data_ptr = user_data_ptr;

  /*-------------------------------------------------------------------------
    Post the command of DS task queue 
  -------------------------------------------------------------------------*/
  ds_put_cmd_ext(cmd_ptr);

  return;

}/* ds_3gpp_iface_ev_cback */

/*===========================================================================
FUNCTION  DS_3GPP_IFACE_EV_CBACK_CMD_HDLR

DESCRIPTION
  This function gets called when any iface event is posted from PS framework
  
PARAMETERS 
 cmd_ptr:     Pointer to the command buffer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_iface_ev_cback_cmd_hdlr
(
   const ds_cmd_type 			   *cmd_ptr
)
{
  ds_3gpp_iface_ev_enum_type       ds_iface_ev = DS_3GPP_IFACE_MAX_EV;
  ds_3gpp_iface_ev_cb_data_type   *iface_ev_cb_data_p = NULL;
  ds_3gpp_iface_event_info_type   *event_info_ptr = NULL;
  uint8                            serviced_cb_cnt = 0;
  uint8                            active_cb_cnt    = 0;
  uint8                            cb_cnt = 0;
  sys_modem_as_id_e_type           subs_id = SYS_MODEM_AS_ID_NONE;
  
  if ( (NULL == cmd_ptr) || (NULL == cmd_ptr->cmd_payload_ptr))
  {
    DS_3GPP_ERROR_LOG_0("NULL cmd_ptr passed, return");
    return;
  }
  
  event_info_ptr = (ds_3gpp_iface_event_info_type*)cmd_ptr->cmd_payload_ptr;

  ds_iface_ev = ds_3gpp_map_ps_iface_ev_to_ds(event_info_ptr->ps_event);

  subs_id = ds3gsubsmgr_subs_id_ds_to_cm(
              PS_IFACE_GET_SUBS_ID(event_info_ptr->this_iface_ptr));

  if(DS_3GPP_IFACE_MAX_EV == ds_iface_ev)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "Invalid 3gpp Event received");
    return;
  }
  active_cb_cnt = ds_3gpp_iface_ev_queue[ds_iface_ev].active_cb_cnt;
  /*------------------------------------------------------------------------- 
    For all the call backs registered on the particular iface_ev, Check if
    the iface match occurs. If the iface on which the iface_ev is received
    is the same as the registered iface 
    The iface on which the event is received is compared with the
    iface cb ev tbl, If a match occurs (Actual match of NULL iface match),
    respective cb function ptrs are dispatched
    ------------------------------------------------------------------------*/
  for(cb_cnt = 0; cb_cnt < DS_3GPP_MAX_IFACE_EV_CNT; cb_cnt++)
  {
    iface_ev_cb_data_p = 
      ds_3gpp_iface_ev_queue[ds_iface_ev].iface_ev_cb_data_ptr[cb_cnt];


    if((iface_ev_cb_data_p != NULL) && 
       ((iface_ev_cb_data_p->iface_ptr == event_info_ptr->this_iface_ptr) || 
       (iface_ev_cb_data_p->iface_ptr == NULL))&&
       (iface_ev_cb_data_p->cb_fptr != NULL))
    {
      DS_3GPP_MSG2_MED_EX(subs_id,
                          "Match occured on iface_p:0x%x for event:%d",
                          event_info_ptr->this_iface_ptr,
                          event_info_ptr->ps_event);

      /*------------------------------------------------------------------ 
        All callback's will have to post a cmd internally. This is because
        this section is under PS task.
        ------------------------------------------------------------------*/
      iface_ev_cb_data_p->cb_fptr(event_info_ptr->this_iface_ptr,
                                  event_info_ptr->ps_event,
                                  event_info_ptr->event_info,
                                  iface_ev_cb_data_p->user_data_ptr);	
								  
      serviced_cb_cnt++;
	  
	  if(serviced_cb_cnt >= active_cb_cnt)
	  {
		DS_3GPP_MSG1_HIGH_EX(subs_id,
                   "Done serviceing all callbacks:%d",serviced_cb_cnt);
		break;
	  }
			  
    }
  }
  return;
}/* ds_3gpp_iface_ev_cback_cmd_hdlr */


