
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

  $Header: //components/dev/data.mpss/3.3/vbhanupr.data.mpss.3.3.pw33_11_24/3gpp/ps/src/ds_3gpp_phys_link_ev_cb_hdlr.c#1
  $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/25/14   vb     phys_link ev callback handler

===========================================================================*/

#include "ds_3gpp_phys_link_ev_cb_hdlr.h"

/*-------------------------------------------------------------------------- 
  Structure definitions
  --------------------------------------------------------------------------*/
ds_3gpp_phys_link_ev_cb_queue_type ds_3gpp_phys_link_ev_queue
                                            [DS_3GPP_MAX_PHYS_LINK_EV_CNT];

/*-------------------------------------------------------------------------- 
  Buffer for phys_link registrations
  --------------------------------------------------------------------------*/
static void *phys_link_ev_buf_ptr = NULL;

/*------------------------------------------------------------------------ 
  Structures to hold phys_link reg/dereg events and their count.
  This will always be freed dynamically after registration/deregistration
  ------------------------------------------------------------------------*/
typedef struct
{
  uint8                    num_phys_link_ev;
  ps_iface_event_enum_type ds_3gpp_phys_link_ev_list_p
                            [DS_3GPP_MAX_PHYS_LINK_EV_CNT];
}ds_3gpp_phys_link_reg_dereg_list_type;

/*===========================================================================
FUNCTION  DS_3GPP_MAP_PHYS_LINK_EV_TO_DS

DESCRIPTION
  This function converts PS iface event to the registered 3gpp phys_link events
  
PARAMETERS 
 
DEPENDENCIES
  None

RETURN VALUE
  ds_3gpp_phys_link_ev_list 

SIDE EFFECTS
  None

===========================================================================*/
static ds_3gpp_phys_link_ev_enum_type ds_3gpp_map_phys_link_ev_to_ds
(
 ps_iface_event_enum_type  ps_event
);
/*===========================================================================
FUNCTION  DS_3GPP_PHYS_LINK_EV_CBACK

DESCRIPTION
  This function gets called when any phys_link event is posted from PS framework
  
PARAMETERS 
  this_phys_link_ptr:   phys_link on which up event was posted
  event:                Event that caused the cback to be invoked
  event_info:           Event info posted by PS
  user_data_ptr:        Data to be passed to the cback function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_phys_link_ev_cback
(
  ps_phys_link_type                       *this_phys_link_ptr,
  ps_iface_event_enum_type                 ps_event,
  ps_iface_event_info_u_type               event_info,
  void                                    *user_data_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_PHYS_LINK_ALLOC_EV_CB_DATA

DESCRIPTION 
    This function allocates the ev callback for the specified phys_link event

 
PARAMETERS
    ds_3gpp_phys_link_ev_list   - phys_link event where memory needs to be allocated
 
DEPENDENCIES
    None.

RETURN VALUE 
    int       -1       if unsuccessful
                       allocated_index if successful
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

static int ds_3gpp_phys_link_alloc_ev_cb_data
(
   ds_3gpp_phys_link_ev_enum_type phys_link_ev
)
{
  int                                 ret_val = -1;
  uint8                               cb_cnt  = 0;
  ds_3gpp_phys_link_ev_cb_data_type   *phys_link_ev_cb_p = NULL;
  ds_3gppi_local_err_val_e_type       local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(phys_link_ev >= DS_3GPP_MAX_PHYS_LINK_EV_CNT)
  {
    ret_val = -1;
    //"phys_link_ev:%d passed is INVALID",
    //phys_link_ev);
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  if(phys_link_ev_buf_ptr == NULL)
  {
    DS_3GPP_MSG0_HIGH("Allocating flow reg buf");
    phys_link_ev_buf_ptr =  ps_phys_link_alloc_event_handle(NULL,
                                                  ds_3gpp_phys_link_ev_cback,
                                                  NULL);
    if(phys_link_ev_buf_ptr == NULL)
    {
      DS3GPP_ASSERT(0);
      ret_val = -1;
      //"phys_link ev buf ptr allocation failed");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }
  }


  for(cb_cnt = 0; cb_cnt < DS_3GPP_PHYS_LINK_MAX_CB_CNT;cb_cnt++)
  {
    phys_link_ev_cb_p = ds_3gpp_phys_link_ev_queue[phys_link_ev].
                                          phys_link_ev_cb_data_ptr[cb_cnt];

    if(phys_link_ev_cb_p != NULL)
    {
      continue;
    }

    phys_link_ev_cb_p = (ds_3gpp_phys_link_ev_cb_data_type *)modem_mem_alloc
                        (sizeof(ds_3gpp_phys_link_ev_cb_data_type),
                        MODEM_MEM_CLIENT_DATA);

    if(phys_link_ev_cb_p == NULL)
    {
      DS_3GPP_MSG0_LOW("Heap allocation error");
      ret_val = -1;
      //"Heap allocation error");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      goto func_exit;
    }

    ds_3gpp_phys_link_ev_queue[phys_link_ev].phys_link_ev_cb_data_ptr[cb_cnt] = 
                                                                phys_link_ev_cb_p;

    DS_3GPP_MSG1_MED("Iface Ev cb data allocated at %d",cb_cnt);
    ret_val = cb_cnt;
    break;
  }

  if(cb_cnt >= DS_3GPP_PHYS_LINK_MAX_CB_CNT)
  {
    ret_val = -1;
    //"All entries are allocated:%d",
    //cb_cnt);
    local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
    goto func_exit;
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_3("ds_3gpp_phys_link_alloc_ev_cb_data: err d ev d cnt d",
                       local_err, phys_link_ev, cb_cnt);
  }

  return ret_val;
} /*ds_3gpp_phys_link_alloc_ev_cb_data*/

/*===========================================================================
FUNCTION DS_3GPP_PHYS_LINK_IS_EVENT_REG_DEREG

DESCRIPTION 
    This function will determine if the phys_link_ev_cb_hdlr will need to
    subscribe events with the FRAMEWORK. If an phys_link event has already been
    subscribed then there is no need to subscribe again

 
PARAMETERS
    ps_phys_link_event_enum_type   - phys_link event on which registration
                                     needs to happen
    *cb_fptr;                      - callback function
    ds_3gpp_phys_link_ev_func      - subscribe or unsubscribe
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/
static boolean ds_3gpp_phys_link_is_event_reg_dereg
(
  ps_iface_event_enum_type         *phys_link_ev_p,
  uint8                            phys_link_ev_cnt,
  ds_3gpp_phys_link_reg_enum_type        phys_link_ev_func
)
{
  boolean ret_val = FALSE;
  uint8   phys_link_tmp_ev_cnt = 0;
  ds_3gpp_phys_link_ev_enum_type ds_phys_link_ev = DS_3GPP_PHYS_LINK_MAX_EV;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for(phys_link_tmp_ev_cnt = 0;phys_link_tmp_ev_cnt < phys_link_ev_cnt; 
       phys_link_tmp_ev_cnt++)
  {
    if(phys_link_ev_p == NULL)
    {
      DS_3GPP_MSG0_LOW("PHYS_LINK_EV_P is NULL");
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
    }

    ds_phys_link_ev = ds_3gpp_map_phys_link_ev_to_ds(*phys_link_ev_p);

    if(ds_phys_link_ev >= DS_3GPP_MAX_PHYS_LINK_EV_CNT)
    {
      DS_3GPP_MSG1_LOW("phys_link_ev:%d passed is INVALID",ds_phys_link_ev);
      ret_val = FALSE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
    }

    /*--------------------------------------------------------------------- 
      For subscription -- Only if the active cb count is 1 does DS need to
      register with the framework else there is no need
      To unsubscribe - Only if the active cb count is 0 does DS need to
      deregister with the Framework
      ---------------------------------------------------------------------*/

    if(((ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].active_cb_cnt == 1) && 
        (phys_link_ev_func == DS_3GPP_PHYS_LINK_EV_SUBSCRIBE)) ||
        ((ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].active_cb_cnt == 0) && 
        (phys_link_ev_func == DS_3GPP_PHYS_LINK_EV_UNSUBSCRIBE)))
    {
      DS_3GPP_MSG1_HIGH("phys_link Event subscription/unsubscription needed:%d",
                        phys_link_ev_func);

      ret_val = TRUE;
      break;
    }

    phys_link_ev_p++;
  }

  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1("ds_3gpp_phys_link_is_event_reg_dereg: err d -- phys "
                       "link ev pnull/phys link ev invalid",
                       local_err);
  }

  return ret_val;
}

/*===========================================================================
FUNCTION DS_3GPP_PHYS_LINK_BUILD_EV_SUBS_UNSUBS_LIST

DESCRIPTION 
    This function will build the phys_link events that need to be subscribed
    with PS framework

 
PARAMETERS
    ps_phys_link_event_enum_type       - phys_link event on which registration needs to happen
    *cb_fptr;                          - callback function
    ds_3gpp_phys_link_ev_func          - SUBScribe or UNSUBSCRIBE
    ds_3gpp_phys_link_reg_dereg_list_type *phys_link_reg_dereg_list
 
DEPENDENCIES
    None.

RETURN VALUE 
   void 
   
SIDE EFFECTS 
  None.
===========================================================================*/
static void ds_3gpp_phys_link_build_ev_reg_dereg_list
(
  ps_iface_event_enum_type              *phys_link_ev_p,
  uint8                                  phys_link_ev_cnt,
  ds_3gpp_phys_link_reg_enum_type        phys_link_ev_func,
  ds_3gpp_phys_link_reg_dereg_list_type *phys_link_reg_dereg_list
)
{
  uint8   num_events  = 0;
  uint8   phys_link_tmp_ev_cnt = 0;
  ds_3gpp_phys_link_ev_enum_type ds_phys_link_ev = DS_3GPP_PHYS_LINK_MAX_EV;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(phys_link_ev_cnt > DS_3GPP_MAX_PHYS_LINK_EV_CNT)
  {
    //"Invalid phys_link_ev_cnt:%d",
    //phys_link_ev_cnt);
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  for(phys_link_tmp_ev_cnt = 0;phys_link_tmp_ev_cnt < phys_link_ev_cnt;
        phys_link_tmp_ev_cnt++)
  {
    if(phys_link_ev_p == NULL)
    {
      //"PHYS_LINK_EV_P is NULL");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }

    ds_phys_link_ev = ds_3gpp_map_phys_link_ev_to_ds(*phys_link_ev_p);

    if(ds_phys_link_ev >= DS_3GPP_MAX_PHYS_LINK_EV_CNT)
    {
      //"phys_link_ev:%d passed is INVALID",
      //ds_phys_link_ev);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      goto func_exit;
    }

    if(((ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].active_cb_cnt == 1) &&
        (phys_link_ev_func == DS_3GPP_PHYS_LINK_EV_SUBSCRIBE)) ||
        ((ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].active_cb_cnt == 0) && 
        (phys_link_ev_func == DS_3GPP_PHYS_LINK_EV_UNSUBSCRIBE)))
    {
        phys_link_reg_dereg_list->ds_3gpp_phys_link_ev_list_p[num_events] = 
                                                              *phys_link_ev_p;
	    num_events++;
	  }

	phys_link_ev_p++;
  }

  DS_3GPP_MSG1_HIGH("Number of events returned is %d",num_events);
  phys_link_reg_dereg_list->num_phys_link_ev = num_events;

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_3("ds_3gpp_phys_link_build_ev_reg_dereg_list: err d"
                       " phys linkt ev d ev cnt d",
                       local_err, ds_phys_link_ev, phys_link_ev_cnt);
  }

  return;

} /*ds_3gpp_phys_link_build_ev_reg_dereg_list*/

/*===========================================================================
FUNCTION  DS_3GPP_PHYS_LINK_DISPATCH_EVENTS

DESCRIPTION
  This function gets called when any phys_link event is registered
  Check if there is a need to dispatch phys_link events.
  
PARAMETERS 
  ps_iface_event_enum_type            *phys_link_ev_p,
  uint8                                phys_link_ev_cnt,
  ps_phys_link_type                   *phys_link_ptr

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_phys_link_dispatch_events
(
  ps_iface_event_enum_type            *phys_link_ev_p,
  uint8                                phys_link_ev_cnt,
  ps_phys_link_type                   *phys_link_ptr
)
{
  uint8 temp_ev_cnt = 0;
  ps_iface_event_info_u_type event_info;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*----------------------------------------------------------------------- 
    For each of the phys_link subscriptions received check if CB's
    have to be dispatched. Since MH registers with PS framework as NULL,
    If a phys_linkis already in that state we will need to dispatch the event
    since Framework will not do this for NULL subscriptions.
    -----------------------------------------------------------------------*/

  if(phys_link_ptr == NULL)
  {
    //"Phys_link ptr is NULL");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  memset(&event_info,0,sizeof(ps_iface_event_info_u_type));

  for(temp_ev_cnt = 0; temp_ev_cnt < phys_link_ev_cnt; temp_ev_cnt++)
  {
    if(phys_link_ev_p == NULL)
    {
      //"Phys_link_ev_p is NULL");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }

    switch(*phys_link_ev_p)
    {
    /*-----------------------------------------------------------------------
                           PHYS_LINK_GONE_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_GONE_EV:
        if (PS_PHYS_LINKI_GET_STATE(phys_link_ptr) == PHYS_LINK_NULL)
        {
          event_info.phys_link_event_info.state     = PHYS_LINK_NULL;
          event_info.phys_link_event_info.info_code = PS_EIC_NOT_SPECIFIED;          
          ds_3gpp_phys_link_ev_cback(phys_link_ptr,*phys_link_ev_p,
                                     event_info,NULL);
        }
        break;


      /*-----------------------------------------------------------------------
                            PHYS_LINK_FLOW_ENABLED_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_FLOW_ENABLED_EV:
        if (PS_PHYS_LINKI_FLOW_ENABLED(phys_link_ptr))
        {
          event_info.flow_mask = ALL_FLOWS_ENABLED;
          ds_3gpp_phys_link_ev_cback(phys_link_ptr,*phys_link_ev_p,
                                     event_info,NULL);
        }
        break;

      /*-----------------------------------------------------------------------
                           PHYS_LINK_FLOW_DISABLED_EV
      -----------------------------------------------------------------------*/
      case PHYS_LINK_FLOW_DISABLED_EV:
        if (!PS_PHYS_LINKI_FLOW_ENABLED(phys_link_ptr))
        {
          event_info.flow_mask = ps_phys_link_get_flow_mask(phys_link_ptr);
          ds_3gpp_phys_link_ev_cback(phys_link_ptr,*phys_link_ev_p,
                                     event_info,NULL);
        }
        break;

      default:
        DS_3GPP_ERROR_LOG_0("Invalid phys_link");
        break;
    }
    phys_link_ev_p++;
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1("ds_3gpp_phys_link_dispatch_events: err d",
                       local_err);
  }
} /*ds_3gpp_phys_link_dispatch_events*/

/*===========================================================================
FUNCTION DS_3GPP_PHYS_LINK_REG_EV

DESCRIPTION 
    This function is to be called when MH modules need to subscribe for a
    3gpp phys_link event

 
PARAMETERS
    ps_iface_event_enum_type - iface event on which registration needs to happen
    *cb_fptr;                - callback function
    *phys_link_ptr;          - phys_link_ptr
    *user_data_ptr;          - user_data_ptr
 
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_phys_link_reg_ev
(
  ps_iface_event_enum_type            *phys_link_ev_p,
  uint8                                phys_link_ev_cnt,
  ps_phys_link_event_cback_type        cb_fptr,
  ps_phys_link_type                   *phys_link_ptr,
  void                                *user_data_ptr
)
{
  boolean                                ret_val = FALSE;
  int8                                   cb_cnt  = -1;
  ds_3gpp_phys_link_ev_enum_type         ds_phys_link_ev = DS_3GPP_MAX_PHYS_LINK_EV_CNT;
  uint8                                  phys_link_tmp_ev_cnt = 0;
  ps_iface_event_enum_type              *tmp_phys_link_ev_p  = phys_link_ev_p;
  ds_3gpp_phys_link_reg_dereg_list_type  phys_link_reg_dereg_list;
  ds_3gppi_local_err_val_e_type          local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /*------------------------------------------------------------------------ 
   Check for cb_ptr here, If the cb_ptr is NULL return
  ------------------------------------------------------------------------*/
  if(cb_fptr == NULL)
  {
    ret_val = FALSE;
    //"CB function ptr is NULL");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  /*----------------------------------------------------------------------- 
    For every event passed, allocate cb_data and store the corresponding
    phys_link_ptr, user_data and cb_ptr
  ------------------------------------------------------------------------*/

  for(phys_link_tmp_ev_cnt = 0;phys_link_tmp_ev_cnt < phys_link_ev_cnt; 
       phys_link_tmp_ev_cnt++)
  {

    if(tmp_phys_link_ev_p == NULL)
    {
      ret_val = FALSE;
      //"phys_link ptr is INVALID");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }

    ds_phys_link_ev = ds_3gpp_map_phys_link_ev_to_ds(*tmp_phys_link_ev_p);

    if(ds_phys_link_ev >= DS_3GPP_MAX_PHYS_LINK_EV_CNT)
    {
      ret_val = FALSE;
      //"phys_link_ev:%d passed is INVALID",
      //ds_phys_link_ev);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      goto func_exit;
    }

    /*------------------------------------------------------------------------ 
      Check if the ev is already registered with the same cb_fn and
      user_data_ptr. If it is then there is no need to register again
    ------------------------------------------------------------------------*/
    for(cb_cnt = 0;cb_cnt < DS_3GPP_PHYS_LINK_MAX_CB_CNT;cb_cnt++)
    {
      if(( ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].
           phys_link_ev_cb_data_ptr[cb_cnt] != NULL)
           && (ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].
           phys_link_ev_cb_data_ptr[cb_cnt]->cb_fptr == cb_fptr) && 
          (ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].phys_link_ev_cb_data_ptr
          [cb_cnt]->user_data_ptr == user_data_ptr) &&
          (ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].phys_link_ev_cb_data_ptr
          [cb_cnt]->phys_link_ptr == phys_link_ptr) &&
           phys_link_ptr != NULL)
      {
        ret_val = FALSE;
        //"Reg ev with the same userdata ptr already present:");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
        goto func_exit;
      }
    } 

    /*------------------------------------------------------------------------- 
      Allocate the cb data for the phys_link event received
    -------------------------------------------------------------------------*/
    cb_cnt = ds_3gpp_phys_link_alloc_ev_cb_data(ds_phys_link_ev);

    if(cb_cnt < 0)
    {
      ret_val = FALSE;
      //"Failure to allocate callback memory");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
      goto func_exit;
    }

    /*-------------------------------------------------------------------------- 
      CB data has been allocated at cb_cnt index
      Now save the contents received and increment the cb_data count
    --------------------------------------------------------------------------*/
    ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].phys_link_ev_cb_data_ptr[cb_cnt]
                                                          ->cb_fptr = cb_fptr;

    ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].phys_link_ev_cb_data_ptr[cb_cnt]
                                               ->phys_link_ptr  = phys_link_ptr;

    ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].phys_link_ev_cb_data_ptr[cb_cnt]->user_data_ptr
                                                                  = user_data_ptr;

    ds_3gpp_phys_link_ev_queue[ds_phys_link_ev].active_cb_cnt++;

    tmp_phys_link_ev_p++;

  }

  /*---------------------------------------------------------------------------- 
    Only if phys_link_tmp_ev_cnt is the same as phys_link_ev_cnt the for loop has
    been successful, If registration is successful then check if DS needs to subscribe
    to phys_link events with PS framework
    ----------------------------------------------------------------------------*/

   DS_3GPP_MSG0_HIGH("Registration of all events successful");
   ret_val = TRUE;

   if(ds_3gpp_phys_link_is_event_reg_dereg(phys_link_ev_p,phys_link_ev_cnt,
                                                DS_3GPP_PHYS_LINK_EV_SUBSCRIBE))
   {
     ds_3gpp_phys_link_build_ev_reg_dereg_list(phys_link_ev_p,
                                               phys_link_ev_cnt,
                                               DS_3GPP_PHYS_LINK_EV_SUBSCRIBE,
                                               &phys_link_reg_dereg_list);
     /*---------------------------------------------------------------------------- 
       Deallocate the phys_link_ev_list always after registration/deregistration with the
       PS
       -----------------------------------------------------------------------------*/
     if((phys_link_reg_dereg_list.num_phys_link_ev > 0) &&
        (phys_link_reg_dereg_list.num_phys_link_ev <= DS_3GPP_MAX_PHYS_LINK_EV_CNT) && 
        (phys_link_ev_buf_ptr != NULL))
     {
       if(ps_phys_link_subscribe_event_list(phys_link_ev_buf_ptr,
                                            phys_link_reg_dereg_list.
                                            ds_3gpp_phys_link_ev_list_p,
                                            phys_link_reg_dereg_list.num_phys_link_ev) == -1)
       {
         ret_val = FALSE;
         //"phys_link Reg event unsuccessful");
         local_err = DS_3GPPI_LOCAL_ERR_VAL_5;
       }
     }
	 else
	 {
		ret_val = FALSE;
	 }
   }

   if(ret_val)
   {
     ds_3gpp_phys_link_dispatch_events(phys_link_ev_p,
                                       phys_link_ev_cnt,
                                       phys_link_ptr);
   }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1("ds_3gpp_phys_link_reg_ev: err d",
                       local_err);
  }

  return ret_val;
} /* ds_3gpp_phys_link_reg_ev */

/*===========================================================================
FUNCTION DS_3GPP_PHYS_LINK_DEREG_EV

DESCRIPTION 
    This function is to be called when MH modules need to unsubscribe for a
    3gpp phys_link event

 
PARAMETERS
    ps_iface_event_enum_type  - iface event on which registration needs to happen
    uint8                     -  phys_link_ev_cnt
    cb_fptr;                  - callback function
    ps_phys_link_type         - *phys_link_ptr
   
DEPENDENCIES
    None.

RETURN VALUE 
    boolean        FALSE       if unsuccessful
                               otherwise
  
   
SIDE EFFECTS 
  None.
===========================================================================*/

boolean ds_3gpp_phys_link_dereg_ev
(
  ps_iface_event_enum_type        *phys_link_ev_p,
  uint8                            phys_link_ev_cnt,
  ps_phys_link_event_cback_type    cb_fptr,
  ps_phys_link_type               *phys_link_ptr

)
{
  boolean                               ret_val = FALSE;
  uint8                                 cb_cnt  = 0;
  ps_phys_link_event_cback_type         cb_tmp_fptr = NULL;
  uint8                                 phys_link_tmp_ev_cnt = 0; 
  ps_iface_event_enum_type             *phys_link_tmp_ev_p = phys_link_ev_p;
  ds_3gpp_phys_link_ev_enum_type        phys_link_ev = DS_3GPP_PHYS_LINK_MAX_EV;
  ds_3gpp_phys_link_reg_dereg_list_type phys_link_reg_dereg_list;
  ds_3gppi_local_err_val_e_type         local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------- 
    Loop through to deregister all the required events
   -------------------------------------------------------------------------*/
  memset(&phys_link_reg_dereg_list, 0, sizeof(ds_3gpp_phys_link_reg_dereg_list_type));
  for(phys_link_tmp_ev_cnt = 0;phys_link_tmp_ev_cnt < phys_link_ev_cnt;
       phys_link_tmp_ev_cnt++)
  {

    if(phys_link_tmp_ev_p == NULL)
    {
      ret_val = FALSE;
      //"phys_link ev ptr is NULL");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      goto func_exit;
    }

    phys_link_ev = ds_3gpp_map_phys_link_ev_to_ds(*phys_link_tmp_ev_p);

    if(phys_link_ev >= DS_3GPP_MAX_PHYS_LINK_EV_CNT)
    {
      ret_val = FALSE;
      //"phys_link:%d passed is INVALID",
      //phys_link_ev);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }

    for(cb_cnt = 0;cb_cnt < DS_3GPP_PHYS_LINK_MAX_CB_CNT; cb_cnt++)
    {
      if(ds_3gpp_phys_link_ev_queue[phys_link_ev].phys_link_ev_cb_data_ptr
                    [cb_cnt] == NULL)
      {
        continue;
      }

      cb_tmp_fptr = ds_3gpp_phys_link_ev_queue[phys_link_ev].phys_link_ev_cb_data_ptr
                    [cb_cnt]->cb_fptr;

      if((cb_tmp_fptr == NULL) || (cb_tmp_fptr != cb_fptr)) 
      {
        continue;
      }

      /*--------------------------------------------------------------------- 
        Now we have a match by function ptr.
        If phys_link_ptr is not NULL, then perform phys_link comparision too
        to find the match
        If phys_link_ptr is NULL then go ahead and delete the entry.
      --------------------------------------------------------------------*/
      if((phys_link_ptr != NULL) && 
         (phys_link_ptr != ds_3gpp_phys_link_ev_queue[phys_link_ev].
         phys_link_ev_cb_data_ptr[cb_cnt]->phys_link_ptr))
      {
        continue;
      }

      modem_mem_free((void *)ds_3gpp_phys_link_ev_queue[phys_link_ev].phys_link_ev_cb_data_ptr
                    [cb_cnt],MODEM_MEM_CLIENT_DATA);

      ds_3gpp_phys_link_ev_queue[phys_link_ev].phys_link_ev_cb_data_ptr
                                                       [cb_cnt] = NULL;

      ds_3gpp_phys_link_ev_queue[phys_link_ev].active_cb_cnt--;

      DS_3GPP_MSG2_HIGH("Active callback count for phys_link:%d is %d",phys_link_ev,
                        ds_3gpp_phys_link_ev_queue[phys_link_ev].active_cb_cnt);

      break;

    }

    if(cb_cnt >= DS_3GPP_PHYS_LINK_MAX_CB_CNT)
    {
      DS_3GPP_MSG0_LOW("Deregistration failed");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
    }
    else
    {
      ret_val = TRUE;
    }

    phys_link_tmp_ev_p++;

  }

  if(ret_val != TRUE)
  {
    //"Deregistration failure");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
    goto func_exit;
  }

  if(ds_3gpp_phys_link_is_event_reg_dereg(phys_link_ev_p,
                                          phys_link_ev_cnt,
                                          DS_3GPP_PHYS_LINK_EV_UNSUBSCRIBE))
  {
    ds_3gpp_phys_link_build_ev_reg_dereg_list(phys_link_ev_p,
                                              phys_link_ev_cnt,
                                              DS_3GPP_PHYS_LINK_EV_UNSUBSCRIBE,
                                              &phys_link_reg_dereg_list);

  /*---------------------------------------------------------------------------- 
    Only if phys_link_tmp_ev_cnt is the same as phys_link_ev_cnt the for loop has been
    successful, If deregistration is successful then check if DS needs to unsubscribe
    to iface events with PS framework
    ----------------------------------------------------------------------------*/
    if((phys_link_reg_dereg_list.num_phys_link_ev > 0) && 
	     (phys_link_reg_dereg_list.num_phys_link_ev <= DS_3GPP_MAX_PHYS_LINK_EV_CNT) && 
       (phys_link_ev_buf_ptr != NULL))
    {
      if(ps_phys_link_unsubscribe_event_list(phys_link_ev_buf_ptr,
                                             phys_link_reg_dereg_list.
                                             ds_3gpp_phys_link_ev_list_p,
                                             phys_link_reg_dereg_list.
                                             num_phys_link_ev) == -1)
      {
        ret_val = FALSE;
        //"Iface deReg event unsuccessful");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
      }
      else
      {
        ret_val = TRUE;
        DS_3GPP_MSG0_HIGH("Iface deReg event successful");
      }

    }
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1("ds_3gpp_phys_link_dereg_ev: err d",
                       local_err);
  }

  return ret_val;
} /*ds_3gpp_phys_link_dereg_ev*/

/*===========================================================================
FUNCTION  DS_3GPP_MAP_PHYS_LINK_EV_TO_DS

DESCRIPTION
  This function converts PS iface event to the registered 3gpp phys_link events
  
PARAMETERS 
 
DEPENDENCIES
  None

RETURN VALUE
  ds_3gpp_phys_link_ev_list 

SIDE EFFECTS
  None

===========================================================================*/
static ds_3gpp_phys_link_ev_enum_type ds_3gpp_map_phys_link_ev_to_ds
(
 ps_iface_event_enum_type  ps_event
)
{
  ds_3gpp_phys_link_ev_enum_type ds_3gpp_phys_link_ev = DS_3GPP_PHYS_LINK_MAX_EV;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  switch(ps_event)
  {
    case PHYS_LINK_FLOW_ENABLED_EV:
      ds_3gpp_phys_link_ev = DS_3GPP_PHYS_LINK_FLOW_ENABLED_EV;
      break;

    case PHYS_LINK_FLOW_DISABLED_EV:
      ds_3gpp_phys_link_ev = DS_3GPP_PHYS_LINK_FLOW_DISABLED_EV;
      break;

    case PHYS_LINK_GONE_EV:
      ds_3gpp_phys_link_ev = DS_3GPP_PHYS_LINK_GONE_EV;
      break;
    
    default:
      DS_3GPP_MSG0_HIGH( "Invalid iface ev");
  }

  return ds_3gpp_phys_link_ev;

}

/*===========================================================================
FUNCTION  DS_3GPP_PHYS_LINK_EV_CBACK

DESCRIPTION
  This function gets called when any phys_link event is posted from PS framework
  
PARAMETERS 
  this_phys_link_ptr:   phys_link on which up event was posted
  event:                Event that caused the cback to be invoked
  event_info:           Event info posted by PS
  user_data_ptr:        Data to be passed to the cback function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_phys_link_ev_cback
(
  ps_phys_link_type                       *this_phys_link_ptr,
  ps_iface_event_enum_type                 ps_event,
  ps_iface_event_info_u_type               event_info,
  void                                    *user_data_ptr
)
{
  ds_cmd_type                       *cmd_ptr = NULL;
  ds_3gpp_phys_link_ev_cb_cmd_type  *phys_link_ev_info_cmd_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_phys_link_ev_cb_cmd_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }
  else
  {
    phys_link_ev_info_cmd_ptr = (ds_3gpp_phys_link_ev_cb_cmd_type*)
                                 (cmd_ptr->cmd_payload_ptr);

    
    cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PHYS_LINK_EV_CB;
    phys_link_ev_info_cmd_ptr->phys_link_ptr = this_phys_link_ptr;
    phys_link_ev_info_cmd_ptr->ps_event = ps_event;
    memscpy(&(phys_link_ev_info_cmd_ptr->event_info),
            sizeof(phys_link_ev_info_cmd_ptr->event_info),
            &event_info, sizeof(ps_iface_event_info_u_type));

    ds_put_cmd( cmd_ptr );
  }
}


void ds_3gpp_phys_link_ev_cback_hdlr
(
  const ds_cmd_type      *cmd_ptr
)
{
  uint8                              cb_cnt = 0;
  ds_3gpp_phys_link_ev_enum_type     ds_3gpp_phys_link_ev = DS_3GPP_PHYS_LINK_MAX_EV;
  uint8                              active_cb_cnt    = 0;
  ds_3gpp_phys_link_ev_cb_data_type *phys_link_ev_cb_data_p = NULL;
  uint8                              serviced_cb_cnt = 0;
  ds_3gpp_phys_link_ev_cb_cmd_type  *phys_link_ev_info_cmd_ptr = NULL;
  ps_phys_link_type                 *cmd_phys_link_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (cmd_ptr == NULL || cmd_ptr->cmd_payload_ptr == NULL)
  {
    DS_3GPP_ERROR_LOG_0("Command ptr or payload is NULL");
    return;
  }

  phys_link_ev_info_cmd_ptr = (ds_3gpp_phys_link_ev_cb_cmd_type*)cmd_ptr->cmd_payload_ptr;
  cmd_phys_link_ptr = phys_link_ev_info_cmd_ptr->phys_link_ptr;

  ds_3gpp_phys_link_ev = ds_3gpp_map_phys_link_ev_to_ds(phys_link_ev_info_cmd_ptr->ps_event);

  if(cmd_phys_link_ptr == NULL ||
     ds_3gpp_phys_link_ev == DS_3GPP_PHYS_LINK_MAX_EV)
  {
    DS_3GPP_ERROR_LOG_1("phys_link_ptr NULL (x) or Invalid 3gpp Event rcvd",
                        cmd_phys_link_ptr);
    return;
  }

  active_cb_cnt = ds_3gpp_phys_link_ev_queue[ds_3gpp_phys_link_ev].active_cb_cnt;
  /*------------------------------------------------------------------------- 
    For all the call backs registered on the particular iface_ev, Check if
    the iface match occurs. If the iface on which the iface_ev is received
    is the same as the registered iface 
    The iface on which the event is received is copared with the
    iface cb ev tbl, If a match occurs (Actual match of NULL iface match),
    respective cb function ptrs are dispatched
    ------------------------------------------------------------------------*/
  for(cb_cnt = 0; cb_cnt < DS_3GPP_MAX_PHYS_LINK_EV_CNT; cb_cnt++)
  {
    phys_link_ev_cb_data_p = 
      ds_3gpp_phys_link_ev_queue[ds_3gpp_phys_link_ev].
      phys_link_ev_cb_data_ptr[cb_cnt];

    if((phys_link_ev_cb_data_p != NULL) && 
       ((phys_link_ev_cb_data_p->phys_link_ptr == cmd_phys_link_ptr) || 
       (phys_link_ev_cb_data_p->phys_link_ptr == NULL))&&
       (phys_link_ev_cb_data_p->cb_fptr != NULL))
    {
      DS_3GPP_MSG2_MED("Match occured on phys_link_p:0x%x for event:%d",
                       cmd_phys_link_ptr,
                       phys_link_ev_info_cmd_ptr->ps_event);

      phys_link_ev_cb_data_p->cb_fptr(cmd_phys_link_ptr,
                                      phys_link_ev_info_cmd_ptr->ps_event,
                                      phys_link_ev_info_cmd_ptr->event_info,
                                      phys_link_ev_cb_data_p->user_data_ptr);
      serviced_cb_cnt++;
	  
	  if(serviced_cb_cnt >= active_cb_cnt)
	  {
		DS_3GPP_MSG1_HIGH("Done serviceing all callbacks:%d",serviced_cb_cnt);
		break;
	  }
			  
    }
  }
  return;
}

