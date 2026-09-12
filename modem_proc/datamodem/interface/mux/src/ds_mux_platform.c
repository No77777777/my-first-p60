/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     D S _  M U X _ P L A T F O R M . C

DESCRIPTION

This file contains all the functions, definitions and data types needed 
for  DS MUX  to interface with Platform/OS specific defination 

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/dev/data.mpss/3.4/adubey.data.mpss.3.4.ds_mux_changes/interface/tasks/src/ds_mux_platform.c#1 
  $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
10/07/10    am     created module.
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "ds_mux_defs.h"
#include "ds_mux_platform.h"
#include "ps_utils.h"
#include "ps_utils_defs.h"
#include "rcevt.h"
#include "rcecb.h"
#include "sys_m_messages.h"
#include "ds_mux.h"
/*===========================================================================

                      LOCAL DECLARATIONS FOR MODULE

===========================================================================*/

/*--------------------------------------------------------------------------
  Definitions and data for the bit-field conversion functions.
  first_set_bits_array_const[i] == X, 0<=X<=7
  where X is the first non-zero bit in binary representation of i.

  Examples:
  first_set_bits_array_const[8] == 3 since binary of 8 is 0001000
  so bit 3 is first non-zero bit.

  Note: first_set_bits_array_const[0] == 8 since no bit is set for 0
  and 8 isn't valid bit number.
---------------------------------------------------------------------------*/
#define SET_BIT_NUM          (8)
#define SET_BIT_NUM_LENGTH   ((1<<SET_BIT_NUM))

 ds_mux_nv_config_info_type   ds_mux_nv_config;
#ifdef TEST_FRAMEWORK
#error code not present
#endif/*TEST_FRAMEWORK*/

static const uint8 first_set_bits_array_const[SET_BIT_NUM_LENGTH] = 
{ 
  8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0,
  2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0,
  3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0,
  2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
  4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0,
  2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0,
  3, 0, 1, 0, 2, 0, 1, 0, 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0,
  2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
  5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0,
  2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0,
  3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0,
  2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
  4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/*===========================================================================

                  EXTERNAL FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/

void ds_mux_init_crit_section
(
  ds_mux_crit_sect_type* crit_sect_ptr
)
{
  PS_INIT_CRIT_SECTION(crit_sect_ptr);
  return;
}/*ds_mux_init_crit_section*/


void ds_mux_enter_crit_section
(
  ds_mux_crit_sect_type*  crit_sect_ptr
)
{
  PS_ENTER_CRIT_SECTION(crit_sect_ptr);
  return;
}/*ds_mux_enter_crit_section*/

void ds_mux_leave_crit_section
(
  ds_mux_crit_sect_type*  crit_sect_ptr
)
{
  PS_LEAVE_CRIT_SECTION(crit_sect_ptr);
  return;
}/*ds_mux_leave_crit_section*/

void ds_mux_destroy_crit_section
(
  ds_mux_crit_sect_type*  crit_sect_ptr
)
{
  PS_DESTROY_CRIT_SECTION(crit_sect_ptr);
  return;

}/*ds_mux_destroy_crit_section*/

uint32 ds_mux_find_set_bits(rex_sigs_type* sig_bit_field, uint32 length, uint32* buffer)
{
  uint32 pos;
  uint32 set_bits;
  uint32 i;
  uint32 size;
  uint8 val;
  const uint8* pointer;

  /*-------------------------------------------------------------------------
    init set bits to no bits set
  -------------------------------------------------------------------------*/
  set_bits = 0;

  /*-------------------------------------------------------------------------
    convert the dword into a byte array
  -------------------------------------------------------------------------*/
  pointer = (const uint8*)sig_bit_field;

  /*-------------------------------------------------------------------------
    set number of bytes
  -------------------------------------------------------------------------*/
  size = length * sizeof(rex_sigs_type);

  /*-------------------------------------------------------------------------
    for each byte do
  -------------------------------------------------------------------------*/
  for ( i = 0; i < size; i++)
  {
    /*-------------------------------------------------------------------------
      get the byte value
    -------------------------------------------------------------------------*/
    val = pointer[i];

    /*-------------------------------------------------------------------------
     initialize the current bit position
    -------------------------------------------------------------------------*/
    pos = (i * SET_BIT_NUM);
    
    /*-------------------------------------------------------------------------
     while there are set bits remaining
    -------------------------------------------------------------------------*/
    while(val != 0)
    {
      /*-------------------------------------------------------------------------
        add the bit to the list of set bits
      -------------------------------------------------------------------------*/
      buffer[set_bits] = (first_set_bits_array_const[val] + pos);
      set_bits++;

      /*-------------------------------------------------------------------------
        update pos to reflect the current bit position
      -------------------------------------------------------------------------*/
      pos = pos + (first_set_bits_array_const[val] + 1);

      /*-------------------------------------------------------------------------
        shift the value to remove the current set bit
      -------------------------------------------------------------------------*/
      val = val >> (first_set_bits_array_const[val] + 1);
    }
  }

  /*-------------------------------------------------------------------------
    return the number of set bits
  -------------------------------------------------------------------------*/
  return set_bits;
} /* ds_mux_find_set_bits() */


/*===========================================================================
FUNCTION  DSATUTIL_MEMSCPY

DESCRIPTION
  
 This function will validate and copy data from src pointer to dest pointer.
  
DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/

size_t ds_mux_memscpy
(
  void        *dst,
  size_t      dst_size,
  const void  *src,
  size_t      src_size

)
{
  if(dst_size == 0 || src_size == 0 || dst == NULL || src == NULL)
  {
    return 0;
  }
  else
  {
    return memscpy( dst,dst_size, src,src_size);
  }
} /* dsatutil_free_memory() */

 ds_mux_timer_handle_type ds_mux_timer_alloc
(
  void (* callback) (void *),
  void *cb_param
)
{
  return ps_timer_alloc( callback, cb_param);
}/* ds_mux_timer_alloc*/

 ds_mux_timer_handle_type ds_mux_timer_alloc_ext
(
  void (* callback) (void *),
  void *cb_param
)
{
  return ps_timer_alloc_ext( callback, cb_param);
}/* ds_mux_timer_alloc*/


ds_mux_timer_error_type ds_mux_timer_start
(
  ps_timer_handle_type handle,
  uint64 time
)
{
  return ps_timer_start( handle, time);
}/* ds_mux_timer_start */

 ds_mux_timer_error_type ds_mux_timer_start_ext
 (
   ps_timer_handle_type handle,
   uint64 time
 )
 {
   return ps_timer_start_ext( handle, time);
 }/* ds_mux_timer_start */

 ds_mux_timer_error_type ds_mux_timer_cancel
 (
   ps_timer_handle_type timer_handle
 )
 {
   return ps_timer_cancel( timer_handle );
 }

 ds_mux_timer_error_type ds_mux_timer_cancel_ext
 (
   ps_timer_handle_type timer_handle
 )
 {
   return ps_timer_cancel_ext( timer_handle );
 }

 ds_mux_timer_error_type ds_mux_timer_free
(
  ps_timer_handle_type handle
)
{
  return ps_timer_free( handle );
}/* ds_mux_timer_free */

 ds_mux_timer_error_type ds_mux_timer_free_ext
(
  ps_timer_handle_type handle
)
{
  return ps_timer_free_ext( handle );
}/* ds_mux_timer_free */


void ds_mux_read_nv_value
(
  boolean           *mux_initiator_mode_enable,
  boolean           *cmux_initialization_enable
)
{
    const char      * NVConfigFilepath = 
        PS_NV_CONF_ITEM_INFO(PS_NV_CONF_DS_MUX_INIT_CONFIG);

    boolean    is_read_success = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  //SSR offtarget testing
#ifdef TEST_FRAMEWORK
  #error code not present
#endif/*TEST_FRAMEWORK*/

  memset(&ds_mux_nv_config, 0x0, sizeof(ds_mux_nv_config));
  
  if (0 != ps_read_efs_nv( NVConfigFilepath, 
                           &ds_mux_nv_config, 
                           sizeof(ds_mux_nv_config) ) )
  {

    is_read_success = FALSE;
    /*-------------------------------------------------------------------------
      Set defaults if NV not configured.
        AT+CMUX NOT supported and mux will be Initiator mode (HE 1.5 Specific config)
    -------------------------------------------------------------------------*/
    ds_mux_nv_config.mux_initiator_mode_enable  = TRUE;
    ds_mux_nv_config.cmux_initialization_enable = FALSE;
    
    if(ps_write_efs_nv(NVConfigFilepath,
                       &ds_mux_nv_config,
                       sizeof(ds_mux_nv_config)))
    {
      DS_MUX_LOG_MSG_ERROR_0("failed to write port config to NV item");
    }
  }

  *mux_initiator_mode_enable  = ds_mux_nv_config.mux_initiator_mode_enable;
  *cmux_initialization_enable = ds_mux_nv_config.cmux_initialization_enable;


  DS_MUX_LOG_MSG_INFO1_3("DS_MUX_CFG: operation mode %d +CMUX init mode %d NV read status %d",
    *mux_initiator_mode_enable, *cmux_initialization_enable, is_read_success);

  return;
}/* ds_mux_read_nv_value */


void ds_mux_shut_down_cb (void)
{
  ds_muxi_cmd_info_type  * ds_mux_cmd_info_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_MUX_LOG_MSG_INFO2_0( "ds_mux_shut_down_cb ");
  
  /*-------------------------------------------------------------------------
 Allocate DS MUX cmd buf, populate it and post cmd to DS MUX task
------------------------------------------------------------------------*/
  DS_MUX_MEM_ALLOC(ds_mux_cmd_info_ptr, sizeof(ds_muxi_cmd_info_type),
                            ds_muxi_cmd_info_type*);
  
  if (NULL == ds_mux_cmd_info_ptr)
  {
    return;
  }
  
  ds_mux_cmd_info_ptr->cmd_id  = DS_MUXI_SSR_SHUT_DOWN;  
  ds_mux_send_cmd(DS_MUX_CMD, ds_mux_cmd_info_ptr);
}

void ds_mux_power_up_cb (void)
{
  ds_muxi_cmd_info_type  * ds_mux_cmd_info_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_MUX_LOG_MSG_INFO2_0( "ds_mux_power_up_cb ");
  
  /*-------------------------------------------------------------------------
 Allocate DS MUX cmd buf, populate it and post cmd to DS MUX task
------------------------------------------------------------------------*/
  DS_MUX_MEM_ALLOC(ds_mux_cmd_info_ptr, sizeof(ds_muxi_cmd_info_type),
                            ds_muxi_cmd_info_type*);
  
  if (NULL == ds_mux_cmd_info_ptr)
  {
    return;
  }
  
  ds_mux_cmd_info_ptr->cmd_id  = DS_MUXI_SSR_POWER_UP;  
  ds_mux_send_cmd(DS_MUX_CMD, ds_mux_cmd_info_ptr);
}

void ds_mux_reg_ssr_events
(
  boolean   is_initiator
)
{

  /*-------------------------------------------------------------------------
    Register for before SHUT DOWN event.
    Initiator will register for AFTER POWER UP event
    Responder will register for BEFORE POWER UP event
  -------------------------------------------------------------------------*/
  (void)rcecb_register_name(SYS_M_SSR_EXT_MODEM_BEFORE_SHUTDOWN, (void *)ds_mux_shut_down_cb);

  if ( TRUE == is_initiator )
  {
    (void)rcecb_register_name(SYS_M_SSR_EXT_MODEM_AFTER_POWERUP, (void *)ds_mux_power_up_cb);
  }
  else
  {
    (void)rcecb_register_name(SYS_M_SSR_EXT_MODEM_BEFORE_POWERUP, (void *)ds_mux_power_up_cb);
  }
}/* ds_mux_reg_ssr_events */


#ifdef TEST_FRAMEWORK
#error code not present
#endif/*TEST_FRAMEWORK*/


