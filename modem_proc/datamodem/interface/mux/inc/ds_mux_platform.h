#ifndef DS_MUX_PLATFORM_H
#define DS_MUX_PLATFORM_H
/*===========================================================================

                        D S _  M U X_ P L A T F O R M . H

DESCRIPTION

  This file contains all the functions, definitions and data types needed 
  for  DS MUX  to interface with Platform/OS specific defination 

Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/dev/data.mpss/3.4/adubey.data.mpss.3.4.ds_mux_changes/interface/tasks/inc/ds_mux_platform.h#1 
  $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "ps_crit_sect.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_system_heap.h"
#include <stringl/stringl.h>
#include "ps_utils.h"
#include "pstimer.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef  ps_crit_sect_type ds_mux_crit_sect_type;

typedef PACKED struct PACKED_POST
{
 uint8      cmux_initialization_enable;
 uint8      mux_initiator_mode_enable;
} ds_mux_nv_config_info_type;


/*===========================================================================

                          PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

                          CRITICAL SECTION FUNCTION DECLARATIONS

===========================================================================*/

/*!
  @brief 
  Initialize a critical section

  @params[in] crit_sect_ptr
  Critical section pointer to be initialized, client just need to pass a 
  structure of type ds_mux_crit_sect_type, memory for any implementation 
  specific data is allocated by platforms. 

  @return
  NONE.

  @notes
  This function should have been called before any calls to
  PS_ENTER_CRIT_SECT() or PS_LEAVE_CRIT_SECT().
  After the critical section usage is complete, client should call
  PS_DESTROY_CRIT_SECTION to release the critical section and its 
  associated memory.
*/
#define DS_MUX_INIT_CRIT_SECTION  ds_mux_init_crit_section
void ds_mux_init_crit_section
(
  ds_mux_crit_sect_type*  crit_sect_ptr
);


/*!
  @brief 
  Enter a critical section

  @params[in] crit_sect_ptr
  Critical section to be entered. 

  @return
  NONE.

  @notes
  Critical section should be initialized prior to calling this function. 
*/


#define DS_MUX_ENTER_CRIT_SECTION ds_mux_enter_crit_section
void ds_mux_enter_crit_section
(
  ds_mux_crit_sect_type*  crit_sect_ptr
);


/*!
  @brief 
  Leave a critical section

  @params[in] crit_sect_ptr
  Critical section to be left.

  @return
  NONE.

  @notes
  Critical section should be initialized prior to calling this function. 
  If not already in critical section, the behavior is undefined.
*/
#define DS_MUX_LEAVE_CRIT_SECTION ds_mux_leave_crit_section
void ds_mux_leave_crit_section
(
  ds_mux_crit_sect_type*  crit_sect_ptr
);

/*!
  @brief 
  Destroy a critical section

  @params[in] crit_sect_ptr
  Critical section to be destroyed.

  @return
  NONE.

  @notes
  Should be only called on initialized critical sections. Otherwise
  behavior is undefined.
*/
#define DS_MUX_DESTROY_CRIT_SECTION ds_mux_destroy_crit_section
void ds_mux_destroy_crit_section
(
  ds_mux_crit_sect_type*  crit_sect_ptr
);


/*===========================================================================

                          LOGGING FUNCTION DECLARATIONS

===========================================================================*/

/**
  This macro is used to log recoverable errors with in Common Data code.
*/
#define   DS_MUX_LOG_MSG_ERROR     LOG_MSG_ERROR
#define   DS_MUX_LOG_MSG_ERROR_0   LOG_MSG_ERROR_0
#define   DS_MUX_LOG_MSG_ERROR_1   LOG_MSG_ERROR_1
#define   DS_MUX_LOG_MSG_ERROR_2   LOG_MSG_ERROR_2
#define   DS_MUX_LOG_MSG_ERROR_3   LOG_MSG_ERROR_3
#define   DS_MUX_LOG_MSG_ERROR_4   LOG_MSG_ERROR_4
#define   DS_MUX_LOG_MSG_ERROR_5   LOG_MSG_ERROR_5
#define   DS_MUX_LOG_MSG_ERROR_6   LOG_MSG_ERROR_6

#define   DS_MUX_LOG_MSG_INFO1    LOG_MSG_INFO1
#define   DS_MUX_LOG_MSG_INFO1_1  LOG_MSG_INFO1_1
#define   DS_MUX_LOG_MSG_INFO1_2  LOG_MSG_INFO1_2
#define   DS_MUX_LOG_MSG_INFO1_3  LOG_MSG_INFO1_3
#define   DS_MUX_LOG_MSG_INFO1_4  LOG_MSG_INFO1_4
#define   DS_MUX_LOG_MSG_INFO1_5  LOG_MSG_INFO1_5
#define   DS_MUX_LOG_MSG_INFO1_6  LOG_MSG_INFO1_6
#define   DS_MUX_LOG_MSG_INFO1_9  LOG_MSG_INFO1_9

#define   DS_MUX_LOG_MSG_INFO2    LOG_MSG_INFO2
#define   DS_MUX_LOG_MSG_INFO2_0  LOG_MSG_INFO2_0
#define   DS_MUX_LOG_MSG_INFO2_1  LOG_MSG_INFO2_1
#define   DS_MUX_LOG_MSG_INFO2_2  LOG_MSG_INFO2_2
#define   DS_MUX_LOG_MSG_INFO2_3  LOG_MSG_INFO2_3
#define   DS_MUX_LOG_MSG_INFO2_4  LOG_MSG_INFO2_4
#define   DS_MUX_LOG_MSG_INFO2_5  LOG_MSG_INFO2_5
#define   DS_MUX_LOG_MSG_INFO2_6  LOG_MSG_INFO2_6

/*===========================================================================

                          TASK UTIL FUNCTION DECLARATIONS

===========================================================================*/


/**
  @brief Find the set bits in the dword array and return them in a list.
  Note this function relies on the (little) endianness of the sig_bit_field.
  The buffer variable is assumed to be long enough to handle all the
  set bits..
*/
uint32 ds_mux_find_set_bits(rex_sigs_type* sig_bit_field, uint32 length, uint32* buffer);


/**
  @brief This function will validate and copy data from src pointer to dest pointer.
*/

size_t ds_mux_memscpy( void   *dst, size_t dst_size, const void  *src, size_t  src_size );

/*===========================================================================

                          MEM ALLOC/DE-ALLOC FUNCTION DECLARATIONS

===========================================================================*/

#define DS_MUX_MEM_ALLOC(buf_ptr,size,typecast_type) \
          PS_SYSTEM_HEAP_MEM_ALLOC(buf_ptr,size,typecast_type);     \
          memset( (void *)buf_ptr, 0x0, size);

#define DS_MUX_MEM_FREE(mem_ptr)                     \
          PS_SYSTEM_HEAP_MEM_FREE(mem_ptr)

/*===========================================================================

                          TIMER APIS FUNCTION DECLARATIONS

===========================================================================*/

#define  DS_MUX_TIMER_INVALID_HANDLE   PS_TIMER_INVALID_HANDLE 
typedef  ps_timer_handle_type          ds_mux_timer_handle_type;
typedef  ps_timer_error_type           ds_mux_timer_error_type;

 /**
   @brief wrapper function for ps timer alloc.
 */
 ds_mux_timer_handle_type ds_mux_timer_alloc
(
  void (* callback) (void *),
  void *cb_param
);

/**
  @brief wrapper function for ps timer alloc ext.
*/
ds_mux_timer_handle_type ds_mux_timer_alloc_ext
(
  void (* callback) (void *),
  void *cb_param
);

/**
  @brief wrapper function for ps timer start.
*/
ds_mux_timer_error_type ds_mux_timer_start
(
  ps_timer_handle_type handle,
  uint64 time
);

/**
  @brief wrapper function for ps timer start ext.
*/
ds_mux_timer_error_type ds_mux_timer_start_ext
 (
   ps_timer_handle_type handle,
   uint64 time
 );

/**
  @brief wrapper function for ps timer free.
*/
ds_mux_timer_error_type ds_mux_timer_free
(
  ps_timer_handle_type handle
);

/**
  @brief wrapper function for ps timer free ext.
*/
ds_mux_timer_error_type ds_mux_timer_free_ext
(
  ps_timer_handle_type handle
);

/**
  @brief wrapper function for ps timer cancel.
*/
ds_mux_timer_error_type ds_mux_timer_cancel
(
  ps_timer_handle_type timer_handle
);

/**
  @brief wrapper function for ps timer cancel ext.
*/
ds_mux_timer_error_type ds_mux_timer_cancel_ext
(
  ps_timer_handle_type timer_handle
);

/**
  @brief Read DS MUX configuration 
*/
void ds_mux_read_nv_value
(
  boolean           *mux_initiator_mode_enable,
  boolean           *cmux_initialization_enable
);

/**
  @brief SSR shut down call back function,  Function will be registered for SSR shutdown event
*/
void ds_mux_shut_down_cb (void);

/**
  @brief SSR Power up call back function,  Function will be registered for SSR power up event
*/
void ds_mux_power_up_cb (void);


/**
  @brief Register for SSR events 
*/
void ds_mux_reg_ssr_events
(
  boolean   is_initiator
);


#ifdef TEST_FRAMEWORK
#error code not present
#endif/*TEST_FRAMEWORK*/



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DS_MUX_PLATFORM_H */
