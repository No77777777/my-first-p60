/*===========================================================================

Diag Legacy Service Mapping Layer Implementation for Debug Message
(F3 Message) Service

GENERAL DESCRIPTION
   API definitons for Debug Message Service Mapping Layer.

EXTERNALIZED FUNCTIONS
   msg_send
   msg_send_1
   msg_send_2
   msg_send_3
   msg_send_var
   msg_sprintf
   Note: These functions should not be used directly, use the MSG_* macros instead.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2012-2018, 2020 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/diag/LSM/qurt/src/Diag_LSM_Msg.c#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/18   sa      Removed Unused APIs.
09/29/17   sa      Send IOCTL for mask update after diagID is set.
07/12/17   sp      Fix issue with extracting userPD F3 diag
05/25/17   sp      Support for DIAG Extended Log and Event APIs on UserPD
05/02/17   gn      Fixed bug in Hash value calculation for F3s falling under unaligned mapping (User PD)
09/13/16   rs      Removed unused variables/functions to avoid compilation warnings
09/06/16   as      Created msg_mask_is_set()
07/11/16   sa      Fixed MSG_SPRINTF for userpd clients
03/10/16   tbg     Added DLL_API_GLOBAL flag to public APIs
02/24/16   gn      Resolved Klockwork errors
06/18/15   sa      Added support for time sync feature.
04/11/16   ph      Added support for QShrink 4.0
01/29/15   xy      Fixed compilation warnings
12/04/14   vk      Multi-PD Optimizations
10/31/14   rh      Added msg_status() based on DCM implementation
07/15/14   is      Stub out msg_v3 functions
05/30/14   is      Fix multi-pd compilation issues
12/23/13   ph      Included header files required to account for removal of ASSERT macro
09/27/13   xy      Removed code under FEATURE_WINCE
07/18/13   sr      Fixed the issue with F3s not coming out on Sensor PD
03/20/13   sg      Added Qshrink 2.0 support
01/25/13   sg      Added support for f3 save trace in multi-pd
12/3/12    sg      Added support for MSG_ERR_LOG macros in sensors pd on adsp
07/05/12   sg      Changes to bringup Diag MultiPD
04/12/10   sg      Merging both DCM rtos and WM files
                   Moved msg_mask_type,msg_large_args structures
                   to msgi.h
01/20/10   sg      Ported Qshrink to WM7
09/29/09   mad     Included msg_arrays_i.h, removed MSG_TBL_GEN feature
09/23/09   sg      Mainlined code when MSM5000_IRAM_FWD is not defined
07/21/09   mad     Changed MSG_FMT_STR_ARG_SIZE (max. size of string argument
                   for MSG_SPRINTF macro) to 280.
07/07/09   mad     Using secure versions of strcpy and _vsnprintf, in place
                   of deprecated versions.
07/07/09   mad     Removed inclusion of customer.h and diagtune.h
04/02/09   mad     Added msg_send_ts(), to let clients pass in their timestamps.
                   Modified msg_send_prep() to take timestamps.
03/19/09   mad     Included customer.h directly
09/30/08   mad     Changes to take out Component services, and use
                   filesystem calls to access windiag driver.
05/01/08   JV      Added support to update the copy of run-time masks in the
                   msg_mask_tbl in this process during initialization and also
                   on mask change
11/29/07   mad     Created File
===========================================================================*/

#include "comdef.h"
#include "Diag_LSM_Msg_i.h"
#include "msg_pkt_defs.h" /* For msg_ext_type etc */
#include "msg.h"
#include "msgcfg.h"

#include "err.h"

#include "diagcmd.h" /* For DIAG_EXT_MSG_F */
#include "diag_shared_i.h" /* for definition of diag_data struct, and diag datatypes. */
#include "msgi.h"
#include "msg_arrays_i.h" /* for definition of different mask arrays and msg_mask_tbl */
#include "msg_qsr.h"
#include "qurt_qdi.h"
#include "Diag_LSMi.h" /* for ghWinDiag etc */
#include "Diag_LSM_Qdi_wrapper.h"
#include "Diag_LSM_F3_Trace.h"

#include "diagdiag_v.h"
#include "diagbuf_mpd.h"

#include <stringl/stringl.h>  /* For memscpy */
#include <stdlib.h>           /* For malloc */
#include "time_svc.h" //for time_get

#include "dll_global_def.h" /* definitions for shared object API export macros */
#include "ULog_Diag.h"

extern ULogHandle diag_debug_ulog_handle;

/*---------------------------------------------------------------------------
  This is the structure that is used to represent the final structure that
  is sent to the external device.  'msg_ext_store_type' is expanded to this
  structure in DIAG task context at the time it is sent to the comm layer.
---------------------------------------------------------------------------*/
typedef struct
{
  msg_hdr_type hdr;
  msg_desc_type desc;
  uint32 msg_hash;
  uint32 args[1];
}
msg_qsr_type;

typedef struct
{
  msg_hdr_type hdr;
  msg_desc_type desc;
  uint32 msg_hash;
  uint32 args[1];
}
msg_qsr_v2_type;

#define MSG_V2_CHECK_IS_HASH(X) ((((uint32)(X)) & 0x03) != 0)

/* Define the symbol that tells this module which time format to use. */
#define MSG_WCDMA_TIME_FORMAT 2
#define MSG_CDMA_TIME_FORMAT  3

/* Number of bits in the frame number that is inserted into the timestamp,
   and the mask to extract these bits. */
#define MSG_FN_NUM_BITS 10
#define MSG_FN_MASK 0x3ff

/* ToDo: Need to implement registration for this wcdma callback in user space for mpd. */
diag_wcdma_wl1_frame_cb_type diag_wcdma_wl1_frame_cb;

/* TODO: These statistics needs to be shared across processes */
/* Statistics */
static uint32 msg_dropped;  /* number of dropped messages */
static uint32 msg_drop_delta;   /* number of dropped messages */

uint32 qsr_v4_drop_count_f3 = 0; /* number of dropped messages due to corrupted Hash */
#ifndef DIAG_USERPD_TEMP
/* Filled by QShrink 4.0 scripts  START */
/*QShrink 4.0 Values */
extern char __QSR_4_0_MSG_ALIGNED_START__,__QSR_4_0_MSG_UNALIGNED_START__,__QSR_4_0_MSG_END__;

const char* qsr_4_0_msg_aligned_start_address QSR_MSG_V3_ATTR= &__QSR_4_0_MSG_ALIGNED_START__ ;

const char* qsr_4_0_msg_unaligned_start_address QSR_MSG_V3_ATTR= &__QSR_4_0_MSG_UNALIGNED_START__ ;

const char* qsr_4_0_msg_end_address QSR_MSG_V3_ATTR= &__QSR_4_0_MSG_END__;

uint32 msg_v4_ss_num_aligned_entries QSR_MSG_V3_ATTR= 0;
msg_v4_ss_aam_table_type msg_v4_ss_aam_table QSR_MSG_V3_ATTR= {{0}};

uint32 msg_v4_ss_num_unaligned_entries QSR_MSG_V3_ATTR= 0;
msg_v4_ss_uam_table_type msg_v4_ss_uam_table QSR_MSG_V3_ATTR= {{0}};

uint32 msg_v4_ss_num_table_entries QSR_MSG_V3_ATTR= 0; /* =msg_v4_ss_num_aligned_entries+msg_v4_ss_num_unaligned_entries*/
msg_v4_ssid_table_type msg_v4_ss_table QSR_MSG_V4_ATTR_DATA= {{0,0,1}};

uint8 msg_v4_dbase_guid[QSHRINK_4_DATABASE_GUID_LENGTH] QSR_MSG_V4_ATTR_DATA= {0};
msg_v4_dbase_name_type msg_v4_dbase_name QSR_MSG_V4_ATTR_DATA;
/* Filled by QShrink 4.0 scripts  END */
#endif



#ifndef MSG_FMT_STR_ARG_SIZE
#define MSG_FMT_STR_ARG_SIZE 280 /* Max. size of string argument for MSG_SPRINTF macros */
#endif
#define MAX_VAR_ARGS 9


/* Internal function declarations */
static void msg_save_trace( const void *,
                            msg_arg_type *,
                            uint8,
                            void *,
                            uint32 );


static uint8 msg_get_time( qword *ts );
static byte* msg_sprintf_prep( const msg_const_type *const_blk, unsigned int num_args, unsigned int *pLength );
static byte* qsr_msg_send_prep( const msg_qsr_const_type *const_blk,
                                unsigned int num_args, unsigned int *pLength, uint32 isHash );

static boolean msg_get_ssid_rt_mask( uint16 ssid, uint32 *mask );
static const char* msg_format_filename2( const char *filename );

#ifndef DIAG_USERPD_TEMP
static byte msg_v4_stream_check(uint16 msg_v4_index);

static msg_v4_store_type * qsr_v4_msg_send_prep (uint32 qsr_hash, unsigned int num_args, 
                                                 uint8 arg_bytes, uint8 stream_id);

static inline void qsr_v4_copy_args(uint8 dst[], uint8 src[], uint8 length);
#endif

/* Function pointer to update the time API to be used */
extern uint8 (*diag_time_get_LSM)(qword);
/* use this global buffer to read message mask in, from the kernel-mode
driver.Avoids a malloc everytime the mask needs updation. */
byte *gMsg_Mask_Read_Buf = NULL;
uint32 gMsg_Mask_Size = 0; /* size of gMsg_Mask_Read_Buf, this is calculated */

/*---------------------------------------------------------------------------------------------------
                                    Stubbed functions
---------------------------------------------------------------------------------------------------*/
void msg_v3_send( const msg_v3_const_type *const_blk )
{
  return;
}

void msg_v3_send_1( const msg_v3_const_type *const_blk, uint32 xx_arg1 )
{
  return;
}

void msg_v3_send_2( const msg_v3_const_type *const_blk, uint32 xx_arg1, uint32 xx_arg2 )
{
  return;
}

void msg_v3_send_3( const msg_v3_const_type *const_blk, uint32 xx_arg1, uint32 xx_arg2, uint32 xx_arg3 )
{
  return;
}

void msg_v3_send_var( const msg_v3_const_type *const_blk, uint32 num_args, ... )
{
  return;
}

/*---------------------------------------------------------------------------------------------------
                                    External functions
(Do not call any of these functions directly, use the Macros defined in msg.h instead.)
---------------------------------------------------------------------------------------------------*/

/*===========================================================================

FUNCTION qsr_v4_incr_drop_count_f3

DESCRIPTION
  Increments qsr_v4_drop_count_f3 when a Hash value is not falling in QSR region
===========================================================================*/
void qsr_v4_incr_drop_count_f3(void)
{
	if(qsr_v4_drop_count_f3 >= MAX_VALUE_UINT32)
	{
		qsr_v4_drop_count_f3 = 0;
		ULogFront_RealTimePrintf(diag_debug_ulog_handle,0,"Variable qsr_v4_drop_count_f3 has reached maximum value");
	}
	qsr_v4_drop_count_f3++;	
}

/*===========================================================================

FUNCTION MSG_STATUS

DESCRIPTION
  Returns whether the specific SSID and subsystem mask is enabled or not.

DEPENDENCIES
   None

===========================================================================*/
DLL_API_GLOBAL boolean msg_status( uint16 ss_id, uint32 ss_mask )
{
  uint32 rt_mask = 0;
  boolean valid_ssid = FALSE;

  valid_ssid = msg_get_ssid_rt_mask(ss_id, &rt_mask);
  if ( (valid_ssid && (ss_mask & rt_mask)) )
  {
    return (TRUE);
  }
  else
  {
    return (FALSE);
  }
}


/*===========================================================================

FUNCTION MSG_SAVE_TRACE

DESCRIPTION
  Extracts arguments based on the constant_data_ptr type and saves it to the
  circular buffer.  If timestamp-saving option is checked, it will be saved
  to the circular buffer as well.

DEPENDENCIES
  None
 
PARAMETERS 
  constant_data_pt - const pointer to the F3 msg
  args -  arguments of the msg
  nargs - number of arguments
  msg -  pointer to the msg
  type - type of the msg

SIDE EFFECTS
  None

===========================================================================*/
static void
msg_save_trace
(
   const void *constant_data_ptr,
   msg_arg_type *args,
   uint8 nargs,
   void *msg,
   uint32 type
   )
{

  diag_f3_save_msg(constant_data_ptr, args, nargs, msg, type, 0);

  return;
}

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  /*===========================================================================
  
  FUNCTION MSG_SAVE_TRACE_V4
  
  DESCRIPTION
	Same as msg_save_trace function expect that argument bytes are passed in to
	prevent recalculation if the message is Qshink 4 type
  
  DEPENDENCIES
	None
   
  PARAMETERS 
	constant_data_pt - const pointer to the F3 msg
	args -	arguments of the msg
	nargs - number of arguments
	msg -  pointer to the msg
	type - type of the msg
	arg_bytes - maximun bytes needed to store each argument in the msg
   
  RETURN VALUE
	None
  
  SIDE EFFECTS
	None
  
  ===========================================================================*/
   void
  msg_save_trace_v4
  (
	  const void *constant_data_ptr,
	  msg_arg_type *args,
	  uint8 nargs,
	  void *msg,
	  uint32 type,
	  uint8 arg_bytes
	  )
  {
  
	diag_f3_save_msg(constant_data_ptr, args, nargs, msg, type, arg_bytes);
  
	return;
  }

#endif /* FEATURE_SAVE_DEBUG_TRACE */

#ifndef DIAG_USERPD_TEMP
/*===========================================================================

FUNCTION qsr_v4_get_address_table_index

DESCRIPTION
  Binary search on address table to get the index pointed to by the function
  
DEPENDENCIES
  The table is expected to have limited entries which are unmapped in QShrink 4.0
  message sections.
 
RETURN 
  uint16 - the index of the msg in msg_v4_ss_table 
===========================================================================*/
uint16
qsr_v4_get_address_table_index(const uint32 search_address)
{
  uint16 index = 0;
  uint16 msg_index = 0;
  /* sheethal new changes start */
  index = (search_address-((uint32)qsr_4_0_msg_unaligned_start_address)) >> 3;
  /* ASSERT(index < MAX_MSG_V4_SSID_UAM_TABLE_ENTRY); */
  /* sheethal new changes end */

if(index < msg_v4_ss_num_unaligned_entries)
  {
	  msg_index = (uint16)msg_v4_ss_uam_table[index].index_to_v4_ssid_table;
  }
  return msg_index;
}

/*===========================================================================

FUNCTION MSG_V4_STREAM_CHECK

DESCRIPTION
  This helper function performs a check to see which streams a given QSR3 message
  is enabled on. In cases where STM is enabled, a LISTENER stream bit is also set
  if listeners are enabled.

PARAMETERS
  msg_v4_index -  Index to the qshrink_4 table
 
RETURN 
  byte - Stream ID of the qshrink_4 F3 msg
===========================================================================*/
static byte msg_v4_stream_check(uint16 msg_v4_index)
{
  byte stream_id = 0;


  /* Check the runtime mask */
  if (msg_v4_index < msg_v4_ss_num_table_entries)
  {
    if ((msg_v4_ss_table[msg_v4_index].rt_mask) &
            DIAG_STREAM_1)
    {
      stream_id |= DIAG_STREAM_1;
    }
#if defined(DIAG_STREAM_2_MASK_CHECK)
    if ((msg_v4_ss_table[msg_v4_index].rt_mask) &
            DIAG_STREAM_2)
    {
      stream_id |= DIAG_STREAM_2;
    }
#endif
  }

  return stream_id;
}

/*===========================================================================

FUNCTION qsr_v4_check_bytes

DESCRIPTION
  This function is used to calculate the bytes needed to store the argument.

PARAMETERS 
  arg_bytes - Number of bytes needed to store the current largest argument
  arg -  The argument that need to be checked how many bytes should use
 
RETURN 
  uint8 - Number of bytes needed to store the current largest argument

===========================================================================*/
static uint8 qsr_v4_check_bytes(uint8 arg_bytes, uint32 arg)
{
  int byte_sized = 0;

  if (arg > MAX_24BIT_VALUE)
  {
    byte_sized = 4;
  }
  else if ((arg > MAX_16BIT_VALUE))
  {
    byte_sized = 3;
  }
  else if ((arg > MAX_8BIT_VALUE))
  {
    byte_sized = 2;
  }
  else
  {
    byte_sized = 1;
  }
/*compare the current largest bytes needed and the bytes needed by this arg, return the larger one,
  which means the return largest bytes needed for current arguments in the msg including this arg*/
  return (MAX(arg_bytes, byte_sized));
  //return 4;

}

/*===========================================================================

FUNCTION qsr_v4_copy_args

DESCRIPTION
  Function used to copy message arguments for Qshrink4 F3s.
  
DEPENDENCIES
  None
 
PARAMETERS 
  dst - destination pointer
  src - source pointer
  length - number of bytes to copy
   
===========================================================================*/
static inline void qsr_v4_copy_args(uint8 dst[], uint8 src[], uint8 length)
{
  uint8 i = 0;
  if (length > 4)
    return;

  for (i = 0; i < length; i++)
  {
    dst[i] = src[i];
  }
  return;
}
/*===========================================================================

FUNCTION qsr_v4_msg_send_prep

DESCRIPTION
  This message performs the common operations used by all messages.
  It allocates, fills in all data except arguments, and returns a pointer
  to the allocated message buffer.  It also handles message statisitics.

PARAMETERS 
 qsr_hash - qshrink_4 hash value
 num_args - number of arguments
 arg_bytes - maximum number if bytes needed to store arguments in the msg
 stream_id - stream ID of the msg

RETURN 
 msg_v4_store_type * - The allocated qshrink_4 f3 msg
===========================================================================*/
static msg_v4_store_type*
qsr_v4_msg_send_prep(uint32 qsr_hash, unsigned int num_args, uint8 arg_bytes, uint8 stream_id)
{
  msg_v4_store_type *msg = NULL;

  void *msg_ptr = NULL;

  if (arg_bytes > 4)
    return NULL;

  unsigned int rsp_len = FPOS(msg_v4_store_type, args) + num_args * arg_bytes;

  msg_ptr = (void *)diagbuf_mpd_alloc(DIAG_QSR4_EXT_MSG_TERSE_F, rsp_len, 0, DIAGBUF_ALLOC_REQUEST_MSG, 0, DIAG_EXT_API_VERSION_0);

  if (msg_ptr)
  {
    /*write stream ID*/
    diagbuf_mpd_write_stream_id(msg_ptr, stream_id); /* write default stream ID for all packets allocated in diagbuf  */
    diagbuf_mpd_set_bit_outbound_pkt_handler(msg_ptr); /*Set MSB in stream ID to skip calling fp in diagbuf_drain() */
    msg = (msg_v4_store_type *)msg_ptr;
  } else
  {
    msg_drop_delta++;
    msg_dropped++;        /* count up one more ignored message */
    return NULL;
  }


  /*-----------------------------------------------------------------------
    Get a pointer to a buffer.  If it's a NULL pointer, there aren't any
    free buffers.  No effort is made to save higher priority messages in
    favor of lower priority messages, as message flooding hides enough
    details to make the usable margin on such a feature too small for
    implementation.
  -----------------------------------------------------------------------*/
  if (msg)
  {
/*lint -save -e{545} Suspicious use of & */
    msg_get_time(&(msg->hdr.ts));
/*lint -restore */

#ifdef FEATURE_DIAG_WCDMA_CFN_SFN_TIMESTAMP
    msg->hdr.ts_type = MSG_TS_TYPE_GW;
#else
    msg->hdr.ts_type = MSG_TS_TYPE_CDMA_FULL;
#endif

    msg->hdr.num_args = (uint8)((arg_bytes << 4) | num_args);

    msg->hdr.drop_cnt = (unsigned char)((msg_drop_delta > 255) ? 255 : msg_drop_delta);

    msg_drop_delta = 0;   /* Reset delta drop count */

    /*-----------------------------------------------------------------------
      Set the pointer to the constant blk, to be expanded in DIAG context.
    -----------------------------------------------------------------------*/
    // msg->qsr_index_flag = QSR_BITFLAG_IS_V4 | msg_v4_index;
    msg->qsr_hash = qsr_hash;
    /* Send last 2 bytes of QSHrink Database Guid as part of hash  */
    msg->dbase_guid[0] = msg_v4_dbase_guid[15];
    msg->dbase_guid[1] = msg_v4_dbase_guid[14];
  }

  return (void *)msg;
}               /* qsr_v4_msg_send_prep() */
#endif


#ifndef DIAG_USERPD_TEMP
/*===========================================================================

FUNCTION MSG_V4_SEND

DESCRIPTION
  This function is similar to QSR_MSG_SEND function, except the msg value. The msg value in const_blk may be 
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h
  
DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v4_send(const msg_v4_const_type *xx_msg_const_ptr)
{
  if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
    return;
  uint32 qsr_hash_v4 = (uint32)xx_msg_const_ptr;
  msg_v4_store_type *msg = NULL;
  const unsigned int num_args = 0;  /* # of message arguments */
  uint8 stream_id = 0;

  uint16 xx_msg_index = 0;
  uint16 msg_index;


  if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
    return;

  if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_aligned_start_address) &&
       (qsr_hash_v4 < (uint32)qsr_4_0_msg_unaligned_start_address))
  {
    /* Aligned Address Mapping */
    msg_index = (~((uint32)qsr_4_0_msg_aligned_start_address) & qsr_hash_v4) >> 13;
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;

    if (msg_index < msg_v4_ss_num_aligned_entries)
    {
      xx_msg_index = (uint16)msg_v4_ss_aam_table[msg_index].index_to_v4_ssid_table;
    }
  }
  else if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_unaligned_start_address) && (qsr_hash_v4 < (uint32)qsr_4_0_msg_end_address))
  {
    /* Unaligned address mapping */
    xx_msg_index = qsr_v4_get_address_table_index(qsr_hash_v4);
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;
  }
  else
  {
    /* Message not part of QShrink  - Error to be investigated */
	qsr_v4_incr_drop_count_f3();
	ULogFront_RealTimePrintf(diag_debug_ulog_handle,1,"msg_v4_send: Invalid hash value : qsr_hash_v4 : 0x%x",qsr_hash_v4);
    return;
  }

  stream_id = msg_v4_stream_check(xx_msg_index);

  if (stream_id)
  {
    msg = qsr_v4_msg_send_prep(qsr_hash_v4, num_args, 0, stream_id);
  }

  if (msg)
  {
    diagbuf_mpd_commit(msg);
  }

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  msg_save_trace_v4((const void *)xx_msg_const_ptr, NULL, 0, (void *)msg, DIAG_F3_TRACE_QSHRINK40, 0);
#endif /* FEATURE_SAVE_DEBUG_TRACE */
} /* msg_v4_send */


#ifdef __HEXMSGABI_2_SUPPORTED__
#ifdef __FLUSH_HARDWARE_RAS__
DLL_API_GLOBAL void msg_v4_send_level1 ( const msg_v4_const_type * xx_msg_const_ptr)
{
 asm("{r31 = r4; jump msg_v4_send}");
}

DLL_API_GLOBAL void msg_v4_send_level2 ( const msg_v4_const_type * xx_msg_const_ptr)
{
 asm("{call msg_v4_send_level1}");
}

DLL_API_GLOBAL void msg_v4_send_level3 ( const msg_v4_const_type * xx_msg_const_ptr)
{
 asm("{call msg_v4_send_level2}");
}

DLL_API_GLOBAL void msg_v4_send_level4 ( const msg_v4_const_type * xx_msg_const_ptr)
{
 asm("{call msg_v4_send_level3}");
}

DLL_API_GLOBAL void msg_v4_send_pcrelR0 ( const msg_v4_const_type * xx_msg_const_ptr)
{
  asm("{r0 = memw(r31++#4)}");
  asm("{r4 = r31; call msg_v4_send_level4}");
}
#else
DLL_API_GLOBAL void msg_v4_send_pcrelR0 ( const msg_v4_const_type * xx_msg_const_ptr)
{
  asm("{r0 = memw(r31++#4); jump msg_v4_send}");
}
#endif
#endif
#endif

#ifndef DIAG_USERPD_TEMP
/*===========================================================================

FUNCTION MSG_V4_SEND_1

DESCRIPTION
  This function is similar to QSR_MSG_SEND_1 function, except the msg value. The msg value in const_blk may be 
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v4_send_1 (const msg_v4_const_type *xx_msg_const_ptr, uint32 xx_arg1)
{
  if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
    return;

  uint32 qsr_hash_v4 = (uint32)xx_msg_const_ptr;
  msg_v4_store_type *msg = NULL;
  uint8 *args = NULL;
  const unsigned int num_args = 1;  /* # of message arguments */
  uint8 stream_id = 0;

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  msg_arg_type    f3_args[1];
#endif /* FEATURE_SAVE_DEBUG_TRACE */

  uint16 xx_msg_index = 0;
  uint16 msg_index;

  uint8 arg_bytes = 0;

  if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_aligned_start_address) &&
       (qsr_hash_v4 < (uint32)qsr_4_0_msg_unaligned_start_address))
  {
    /* Aligned Address Mapping */
    msg_index = (~((uint32)qsr_4_0_msg_aligned_start_address) & qsr_hash_v4) >> 13;
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;

    if (msg_index < msg_v4_ss_num_aligned_entries)
    {
      xx_msg_index = (uint16)msg_v4_ss_aam_table[msg_index].index_to_v4_ssid_table;
    }
  }
  else if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_unaligned_start_address) && (qsr_hash_v4 < (uint32)qsr_4_0_msg_end_address))
  {
    /* Unaligned address mapping */
    xx_msg_index = qsr_v4_get_address_table_index(qsr_hash_v4);
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;
  }
  else
  {
    /* Message not part of QShrink  - Error to be investigated */
	qsr_v4_incr_drop_count_f3();
	ULogFront_RealTimePrintf(diag_debug_ulog_handle,1,"msg_v4_send_1: Invalid hash value : qsr_hash_v4 : 0x%x",qsr_hash_v4);
    return;
  }
  /*check the actual byte the xx_arg1 needed*/
  arg_bytes =  qsr_v4_check_bytes(arg_bytes, xx_arg1);

  stream_id = msg_v4_stream_check(xx_msg_index);
  
  if(stream_id)
  {
    msg = qsr_v4_msg_send_prep(qsr_hash_v4, num_args, arg_bytes, stream_id);
  }

  if (msg)
  {
    /*-----------------------------------------------------------------------
    Store the arguments in the buffer.
    -----------------------------------------------------------------------*/
    args = (uint8 *)msg->args;
    qsr_v4_copy_args(args, (uint8 *)&xx_arg1, arg_bytes);
    diagbuf_mpd_commit(msg);
  }


#if defined (FEATURE_SAVE_DEBUG_TRACE)

  f3_args[0] = xx_arg1;

  msg_save_trace_v4((const void *)xx_msg_const_ptr, f3_args, 1, (void *)msg, DIAG_F3_TRACE_QSHRINK40, arg_bytes);

#endif /* FEATURE_SAVE_DEBUG_TRACE */
} /* msg_v4_send_1 */

#ifdef __HEXMSGABI_2_SUPPORTED__
#ifdef __FLUSH_HARDWARE_RAS__
DLL_API_GLOBAL void msg_v4_send_1_level1 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1)
{
 asm("{r31 = r4; jump msg_v4_send_1}");
}

DLL_API_GLOBAL void msg_v4_send_1_level2 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1)
{
 asm("{call msg_v4_send_1_level1}");
}

DLL_API_GLOBAL void msg_v4_send_1_level3 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1)
{
 asm("{call msg_v4_send_1_level2}");
}

DLL_API_GLOBAL void msg_v4_send_1_level4 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1)
{
 asm("{call msg_v4_send_1_level3}");
}

DLL_API_GLOBAL void msg_v4_send_1_pcrelR0 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1)
{
 asm("{r0 = memw(r31++#4)}");
 asm("{r4 = r31; call msg_v4_send_1_level4}");
}
#else
DLL_API_GLOBAL void msg_v4_send_1_pcrelR0 (const msg_v4_const_type * xx_msg_const_ptr,uint32 xx_arg1)
{
  asm("{r0 = memw(r31++#4); jump msg_v4_send_1}");
}
#endif
#endif
#endif


#ifndef DIAG_USERPD_TEMP
/*===========================================================================

FUNCTION MSG_V4_SEND_2

DESCRIPTION
  This function is similar to QSR_MSG_SEND_2 function, except the msg value. The msg value in const_blk may be 
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v4_send_2 (const msg_v4_const_type *xx_msg_const_ptr, uint32 xx_arg1, uint32 xx_arg2)
{
  if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
    return;

  uint32 qsr_hash_v4 = (uint32)xx_msg_const_ptr;
  msg_v4_store_type *msg = NULL;
  uint8 *args = NULL;
  const unsigned int num_args = 2;  /* # of message arguments */
  uint8 stream_id = 0;

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  msg_arg_type    f3_args[2];
#endif /* FEATURE_SAVE_DEBUG_TRACE */


  uint16 xx_msg_index = 0;
  uint16 msg_index;

  uint8 arg_bytes = 0;

  if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_aligned_start_address) &&
       (qsr_hash_v4 < (uint32)qsr_4_0_msg_unaligned_start_address))
  {
    /* Aligned Address Mapping */
    msg_index = (~((uint32)qsr_4_0_msg_aligned_start_address) & qsr_hash_v4) >> 13;
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;

    if (msg_index < msg_v4_ss_num_aligned_entries)
    {
      xx_msg_index = (uint16)msg_v4_ss_aam_table[msg_index].index_to_v4_ssid_table;
    }
  }
  else if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_unaligned_start_address) && (qsr_hash_v4 < (uint32)qsr_4_0_msg_end_address))
  {
    xx_msg_index = qsr_v4_get_address_table_index(qsr_hash_v4);
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;
  }
  else
  {
    /* Message not part of QShrink  - Error to be investigated */
	qsr_v4_incr_drop_count_f3();
	ULogFront_RealTimePrintf(diag_debug_ulog_handle,1,"msg_v4_send_2: Invalid hash value : qsr_hash_v4 : 0x%x",qsr_hash_v4);
    return;
  }

  /*check the actual bytes needed for each argument*/
  arg_bytes =  qsr_v4_check_bytes(arg_bytes, xx_arg1);
  arg_bytes =  qsr_v4_check_bytes(arg_bytes, xx_arg2);

  stream_id = msg_v4_stream_check(xx_msg_index);

  if(stream_id)
  {
    msg = qsr_v4_msg_send_prep(qsr_hash_v4, num_args, arg_bytes, stream_id);
  }

  if (msg)
  {
    /*-----------------------------------------------------------------------
          Store the arguments in the buffer.
         -----------------------------------------------------------------------*/
    args = (uint8 *)&(msg->args);
    qsr_v4_copy_args(args, (uint8 *)&xx_arg1, arg_bytes);
    args += arg_bytes;
    qsr_v4_copy_args(args, (uint8 *)&xx_arg2, arg_bytes);

    diagbuf_mpd_commit(msg);
  }

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  f3_args[0] = xx_arg1;
  f3_args[1] = xx_arg2;

  msg_save_trace_v4((const void *)xx_msg_const_ptr, f3_args, 2, (void *)msg, DIAG_F3_TRACE_QSHRINK40, arg_bytes);
#endif /* FEATURE_SAVE_DEBUG_TRACE */
} /* msg_v4_send_2 */

#ifdef __HEXMSGABI_2_SUPPORTED__
#ifdef __FLUSH_HARDWARE_RAS__
DLL_API_GLOBAL void msg_v4_send_2_level1 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1, uint32 xx_arg2)
{
 asm("{r31 = r4; jump msg_v4_send_2}");
}

DLL_API_GLOBAL void msg_v4_send_2_level2 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1, uint32 xx_arg2)
{
 asm("{call msg_v4_send_2_level1}");
}

DLL_API_GLOBAL void msg_v4_send_2_level3 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1, uint32 xx_arg2)
{
 asm("{call msg_v4_send_2_level2}");
}

DLL_API_GLOBAL void msg_v4_send_2_level4 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1, uint32 xx_arg2)
{
 asm("{call msg_v4_send_2_level3}");
}

DLL_API_GLOBAL void msg_v4_send_2_pcrelR0 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1, uint32 xx_arg2)
{
 asm("{r0 = memw(r31++#4)}");
 asm("{r4 = r31; call msg_v4_send_2_level4}");
}
#else
DLL_API_GLOBAL void msg_v4_send_2_pcrelR0 (const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1, uint32 xx_arg2)
{
  asm("{r0 = memw(r31++#4); jump msg_v4_send_2}");
}
#endif
#endif
#endif


#ifndef DIAG_USERPD_TEMP
/*===========================================================================

FUNCTION MSG_V4_SEND_3

DESCRIPTION
  This function is similar to QSR_MSG_SEND_3 function, except the msg value. The msg value in const_blk may be 
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v4_send_3 (const msg_v4_const_type *xx_msg_const_ptr, uint32 xx_arg1,
     uint32 xx_arg2, uint32 xx_arg3)
{
  if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
    return;

  uint32 qsr_hash_v4 = (uint32)xx_msg_const_ptr;
  msg_v4_store_type *msg = NULL;
  uint8 *args = NULL;
  const unsigned int num_args = 3;  /* # of message arguments */
  uint8 stream_id = 0;

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  msg_arg_type    f3_args[3];
#endif /* FEATURE_SAVE_DEBUG_TRACE */


  uint16 xx_msg_index = 0;
  uint16 msg_index;

  uint8 arg_bytes = 0;

  if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_aligned_start_address) &&
       (qsr_hash_v4 < (uint32)qsr_4_0_msg_unaligned_start_address))
  {
    /* Aligned Address Mapping */
    msg_index = (~((uint32)qsr_4_0_msg_aligned_start_address) & qsr_hash_v4) >> 13;
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;

    if (msg_index < msg_v4_ss_num_aligned_entries)
    {
      xx_msg_index = (uint16)msg_v4_ss_aam_table[msg_index].index_to_v4_ssid_table;
    }
  }
  else if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_unaligned_start_address) && (qsr_hash_v4 < (uint32)qsr_4_0_msg_end_address))
  {
    xx_msg_index = qsr_v4_get_address_table_index(qsr_hash_v4);
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;
  }
  else
  {
    /* Message not part of QShrink  - Error to be investigated */
	qsr_v4_incr_drop_count_f3();
	ULogFront_RealTimePrintf(diag_debug_ulog_handle,1,"msg_v4_send_3: Invalid hash value : qsr_hash_v4 : 0x%x",qsr_hash_v4);
    return;
  }

  /*check the actual bytes needed for each argument*/
  arg_bytes =  qsr_v4_check_bytes(arg_bytes, xx_arg1);
  arg_bytes =  qsr_v4_check_bytes(arg_bytes, xx_arg2);
  arg_bytes =  qsr_v4_check_bytes(arg_bytes, xx_arg3);

  stream_id = msg_v4_stream_check(xx_msg_index);

  if(stream_id)
  {
    msg = qsr_v4_msg_send_prep(qsr_hash_v4, num_args, arg_bytes, stream_id);
  }
  if (msg)
  {
    /*-----------------------------------------------------------------------
      Store the arguments in the buffer.
      -----------------------------------------------------------------------*/
    args = (uint8 *)&msg->args;
    qsr_v4_copy_args(args, (uint8 *)&xx_arg1, arg_bytes);
    args += arg_bytes;
    qsr_v4_copy_args(args, (uint8 *)&xx_arg2, arg_bytes);
    args += arg_bytes;
    qsr_v4_copy_args(args, (uint8 *)&xx_arg3, arg_bytes);


    diagbuf_mpd_commit(msg);
  }

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  f3_args[0] = xx_arg1;
  f3_args[1] = xx_arg2;
  f3_args[2] = xx_arg3;

  msg_save_trace_v4((const void *)xx_msg_const_ptr, f3_args, 3, (void *)msg, DIAG_F3_TRACE_QSHRINK40, arg_bytes);
#endif /* FEATURE_SAVE_DEBUG_TRACE */
} /* msg_v4_send_3 */

#ifdef __HEXMSGABI_2_SUPPORTED__
#ifdef __FLUSH_HARDWARE_RAS__
DLL_API_GLOBAL void msg_v4_send_3_level1 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1,
        uint32 xx_arg2, uint32 xx_arg3)
{
 asm("{r31 = r4; jump msg_v4_send_3}");
}

DLL_API_GLOBAL void msg_v4_send_3_level2 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1,
        uint32 xx_arg2, uint32 xx_arg3)
{
 asm("{call msg_v4_send_3_level1}");
}

DLL_API_GLOBAL void msg_v4_send_3_level3 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1,
        uint32 xx_arg2, uint32 xx_arg3)
{
 asm("{call msg_v4_send_3_level2}");
}

DLL_API_GLOBAL void msg_v4_send_3_level4 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1,
        uint32 xx_arg2, uint32 xx_arg3)
{
 asm("{call msg_v4_send_3_level3}");
}

DLL_API_GLOBAL void msg_v4_send_3_pcrelR0 ( const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1,
        uint32 xx_arg2, uint32 xx_arg3)
{
 asm("{r0 = memw(r31++#4)}");
 asm("{r4 = r31; call msg_v4_send_3_level4}");
}

#else
DLL_API_GLOBAL void msg_v4_send_3_pcrelR0 (const msg_v4_const_type * xx_msg_const_ptr, uint32 xx_arg1,
                            uint32 xx_arg2, uint32 xx_arg3)
{
  asm("{r0 = memw(r31++#4); jump msg_v4_send_3}");
}
#endif
#endif
#endif

#ifndef DIAG_USERPD_TEMP
/*===========================================================================

FUNCTION MSG_V4_SEND_VAR

DESCRIPTION
  This function is similar to QSR_MSG_SEND_VAR function, except the msg value. The msg value in const_blk may be 
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void
msg_v4_send_var(const msg_v4_const_type *xx_msg_const_ptr, uint32 num_args, ...)
{
  if (diag_qdi_handle < DIAG_FD_MIN_VALUE)
    return;

  uint32 qsr_hash_v4 = (uint32)xx_msg_const_ptr;
  msg_v4_store_type *msg = NULL;
  uint8 *args = NULL;
  va_list arg_list;     /* ptr to the variable argument list */
  unsigned int i;
  uint8 stream_id = 0;

#if defined (FEATURE_SAVE_DEBUG_TRACE)
  msg_arg_type    f3_args[MAX_VAR_ARGS];  // Assuming at most MAX_VAR_ARGS arguments
#endif

  uint16 xx_msg_index = 0;
  uint16 msg_index;
  uint8 arg_bytes = 0;
  uint32 arg[MAX_VAR_ARGS];
  /* num_args should never be greater than MAX_VAR_ARGS */
  if(num_args > MAX_VAR_ARGS)
  {
    ERR_FATAL("msg_v4_send_var:  num_args(%lu) > 9", num_args, 0, 0);
  }
  num_args = MIN(num_args, MAX_VAR_ARGS);

  if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_aligned_start_address) &&
       (qsr_hash_v4 < (uint32)qsr_4_0_msg_unaligned_start_address))
  {
    /* Aligned Address Mapping */
    msg_index = (~((uint32)qsr_4_0_msg_aligned_start_address) & qsr_hash_v4) >> 13;
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;

    if (msg_index < msg_v4_ss_num_aligned_entries)
    {
      xx_msg_index = (uint16)msg_v4_ss_aam_table[msg_index].index_to_v4_ssid_table;
    }
  }
  else if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_unaligned_start_address) && (qsr_hash_v4 < (uint32)qsr_4_0_msg_end_address))
  {
    xx_msg_index = qsr_v4_get_address_table_index(qsr_hash_v4);
    qsr_hash_v4 = (qsr_hash_v4 - (uint32)qsr_4_0_msg_aligned_start_address) >> 3;
  }
  else
  {
    /* Message not part of QShrink  - Error to be investigated */
	qsr_v4_incr_drop_count_f3();
	ULogFront_RealTimePrintf(diag_debug_ulog_handle,1,"msg_v4_send_var: Invalid hash value : qsr_hash_v4 : 0x%x",qsr_hash_v4);
    return;
  }
  /*lint -save -e{64,10,78,718,746,628,732,530} */
  /* Initialize variable arguments */
  va_start(arg_list, num_args);
  /*lint -restore */

  /* Store arguments from variable list. */
  /*lint -save -e{64,10,78,718,746,628,732,530} */
  for (i = 0; i < num_args; i++)
  {
    arg[i] = va_arg(arg_list, uint32);
    arg_bytes =  qsr_v4_check_bytes(arg_bytes, arg[i]);
  }               /* for */
  /*lint -restore */
  /* Reset variable arguments */
  va_end(arg_list);

  stream_id = msg_v4_stream_check(xx_msg_index);

  if(stream_id)
  {
    msg = qsr_v4_msg_send_prep(qsr_hash_v4, num_args, arg_bytes, stream_id);
  }

  if (msg)
  {
    /*-----------------------------------------------------------------------
      Store the arguments in the buffer.
    -----------------------------------------------------------------------*/
    args = (uint8 *)&msg->args;
    for (i = 0; i < num_args; i++)
    {
      qsr_v4_copy_args(args, (uint8 *)&arg[i], arg_bytes);
      args += arg_bytes;
    }

    diagbuf_mpd_commit(msg);
  }


#if defined (FEATURE_SAVE_DEBUG_TRACE)
  /*lint -save -e{64,10,718,746,628} */
  /* Initialize variable arguments */
  va_start(arg_list, num_args);

  /* Store arguments from variable list. */
  for (i = 0; i < num_args; i++)
  {
    f3_args[i] = va_arg(arg_list, uint32);
  }               /* for */

  /* Reset variable arguments */
  va_end(arg_list);
/*lint -restore */
  //XXX TODO: don't cast like this --rh
  msg_save_trace_v4((const void *)xx_msg_const_ptr, f3_args, num_args, (void *)msg, DIAG_F3_TRACE_QSHRINK40, arg_bytes);
#endif

} /* msg_v4_send_var */
#endif

/*===========================================================================
FUNCTION MSG_SPRINTF

DESCRIPTION

   This function sends out a debug message with variable number of arguments
   across DiagPkt CS interface.
   This will build a message sprintf diagnostic Message with var #
   of parameters.
   Do not call directly; use macro MSG_SPRINTF_* defined in msg.h

DEPENDENCIES
   windiag driver handle should be initialised.
===========================================================================*/
DLL_API_GLOBAL void
msg_sprintf( const msg_const_type *const_blk, ... )
{

  uint32 pkt_length = 0;
  if ( const_blk && (diag_qdi_handle >= 0) )
  {
    /* Declared union for future use */
    typedef union
    {
      msg_ext_type ext;
    } msg_sprintf_desc_type;
    byte *pMsg = NULL;
    unsigned int int_cnt = 0;           /* Calculate the # args, to allocate buffer */
    unsigned int fname_length = 0;      /* Stores the file name along with '\0'     */
    unsigned int fmt_length = 0;        /* Stores the fmt length,'\0' and arg size  */
    unsigned int total_allocated = 0;   /* Total buffer allocated                   */
    const char *abb_filename = NULL;

    /* Point to note: In AMSS, two files cannot have the same name,
    so the whole file path is not displayed in QXDM. In WM, that need not be the case?
     For now, get the file name only, as done in legacy diag. */
    abb_filename = msg_format_filename2(const_blk->fname);
    fname_length = strlen(abb_filename) + 1;

    fmt_length = strlen(const_blk->fmt) + 1 + MSG_FMT_STR_ARG_SIZE;
    /* Calculate # of arguments to ensure enough space is allocated. */
    int_cnt = sizeof(msg_desc_type) - FSIZ(msg_ext_store_type, const_data_ptr) +
       fmt_length + fname_length;
    /* Calculates number of uint32s required */
    int_cnt = (int_cnt + sizeof(uint32) - 1) / sizeof(uint32);
    /*  Allocates the buffer required, fills in the header  */
    pMsg = msg_sprintf_prep(const_blk, int_cnt, &total_allocated);
    if ( pMsg )
    {
      msg_sprintf_desc_type *pTemp = (msg_sprintf_desc_type *)(pMsg);
      char *str = NULL; /* Used to copy the file name and fmt string to the msg */
      va_list arg_list;   /* ptr to the variable argument list */
      unsigned int fmt_len_available = 0; /* Remaining buffer for format string       */

      /* Queue a debug message in Extended Message Format. */
      pTemp->ext.hdr.cmd_code = DIAG_EXT_MSG_F;
      /* This function embedds the argument in the string itself.
       ** Hence the num_args is assigned 0 */
      pTemp->ext.hdr.num_args = 0;
      /* Copy 'desc'. */
      pTemp->ext.desc = const_blk->desc;
      /* Copy the format string where the argument list would start.
          Since there are no arguments, the format string starts in the 'args'
          field. */
      str = (char *)pTemp->ext.args;
      /* Calculate the buffer left to copy the format string */
      fmt_len_available = total_allocated - (FPOS(msg_ext_type, args) + fname_length);
      if ( fmt_len_available < fmt_length )
      {
        fmt_length = fmt_len_available;
      }
      /* Initialize variable argument list */
      va_start(arg_list, const_blk);
      /* Copy the format string with arguments */
      vsnprintf(str, fmt_length, const_blk->fmt, arg_list);

      str[fmt_length - 1] = '\0';
      /* Reset variable arguments */
      va_end(arg_list);
      /* Move the str pass the format string, strlen excludes the terminal
      ** NULL hence 1 is added to include NULL. */
      str += strlen((const char *)str) + 1;
      /* Copy the filename */
      strlcpy(str, abb_filename, fname_length);
      /* Move the str pass the filename, strlen excludes the terminal NULL
      ** hence 1 is added to include NULL. */
      str += strlen((const char *)str) + 1;
      /*shorten the packet for tool to parse it properly else the filename is not printed correctly*/
      pkt_length = (uint32)(str - (char*)pMsg);
      diagbuf_mpd_shorten(pMsg, pkt_length);
      diagbuf_mpd_commit(pMsg);
    } /* if (pMsg) */
  } /* if(const_blk && ghWinDiag ) */
  return;
}  /* msg_sprintf */

/*===========================================================================
FUNCTION qsr_int_msg_send

DESCRIPTION
   This function sends out a debug message with no arguments.
   This function, or the macro QSR_MSG should not be called directly,
   MSG macros are converted to QSR_MSG macro by text-replacement.

DEPENDENCIES
   Diag_LSM_Init() should be called before calling this function.
===========================================================================*/
void qsr_int_msg_send( const msg_qsr_const_type *const_blk, uint32 isHash )
{
  if ( diag_qdi_handle >= DIAG_FD_MIN_VALUE )
  {
    byte *pMsg = NULL;
    const unsigned int num_args = 0;  /* # of message arguments */
    unsigned int nLength = 0;
    uint8 qshrink_type = DIAG_F3_TRACE_STANDARD;

    if ( isHash )
    {
      qshrink_type = DIAG_F3_TRACE_QSHRINK20;
    }

    pMsg = qsr_msg_send_prep(const_blk, num_args, &nLength, isHash);
    if ( pMsg )
    {
      diagbuf_mpd_commit(pMsg);
    }
    msg_save_trace((const void *)const_blk, NULL, 0, (void *)(pMsg), qshrink_type);
  }
} /* qsr_int_msg_send */

/*===========================================================================
FUNCTION qsr_int_msg_send_1

DESCRIPTION
   This function sends out a debug message with one argument.
   This function, or the macro QSR_MSG_1 should not be called directly,
   MSG* macros are converted to QSR_MSG* macros by text-replacement.

DEPENDENCIES
   Diag_LSM_Init() should be called before calling this function.
===========================================================================*/
void qsr_int_msg_send_1( const msg_qsr_const_type *const_blk, uint32 isHash, uint32 xx_arg1 )
{
  if ( diag_qdi_handle >= DIAG_FD_MIN_VALUE )
  {
    byte *pMsg = NULL;
    const unsigned int num_args = 1;  /* # of message arguments */
    unsigned int nLength = 0;
    msg_arg_type f3_args[1];
    uint8 qshrink_type = DIAG_F3_TRACE_STANDARD;

    f3_args[0] = xx_arg1;

    if ( isHash )
    {
      qshrink_type = DIAG_F3_TRACE_QSHRINK20;
    }

    pMsg = qsr_msg_send_prep(const_blk, num_args, &nLength, isHash);
    if ( pMsg )
    {
      if ( QSR_BITFLAG_IS_HASH & isHash )
      {
        msg_qsr_type *pTemp = (msg_qsr_type *)(pMsg);
        /* Store the arguments in the buffer. */
        pTemp->args[0] = xx_arg1;
      }
      else
      {
        msg_ext_type *pTemp = (msg_ext_type *)(pMsg);
        /* Store the arguments in the buffer. */
        pTemp->args[0] = xx_arg1;
      }
      diagbuf_mpd_commit(pMsg);
    }
    msg_save_trace((const void *)const_blk, f3_args, 1, (void *)pMsg, qshrink_type);
  }
} /* qsr_int_msg_send_1 */

/*===========================================================================
FUNCTION qsr_int_msg_send_2

DESCRIPTION
   This function sends out a debug message with two arguments.
   This function, or the macro QSR_MSG_2 should not be called directly,
   MSG* macros are converted to QSR_MSG* macros by text-replacement.

DEPENDENCIES
   Diag_LSM_Init() should be called before calling this function.
===========================================================================*/
void qsr_int_msg_send_2( const msg_qsr_const_type *const_blk, uint32 isHash, uint32 xx_arg1, uint32 xx_arg2 )
{
  if ( diag_qdi_handle >= DIAG_FD_MIN_VALUE )
  {
    byte *pMsg = NULL;
    const unsigned int num_args = 2;  /* # of message arguments */
    unsigned int nLength = 0;
    uint8 qshrink_type = DIAG_F3_TRACE_STANDARD;
    msg_arg_type f3_args[2];

    f3_args[0] = xx_arg1;
    f3_args[1] = xx_arg2;

    if ( isHash )
    {
      qshrink_type = DIAG_F3_TRACE_QSHRINK20;
    }

    pMsg = qsr_msg_send_prep(const_blk, num_args, &nLength, isHash);
    if ( pMsg )
    {
      if ( QSR_BITFLAG_IS_HASH & isHash )
      {
        msg_qsr_type *pTemp = (msg_qsr_type *)(pMsg);
        /* Store the arguments in the buffer. */
        pTemp->args[0] = xx_arg1;
        pTemp->args[1] = xx_arg2;
      }
      else
      {
        msg_ext_type *pTemp = (msg_ext_type *)(pMsg);
        /* Store the arguments in the buffer. */
        pTemp->args[0] = xx_arg1;
        pTemp->args[1] = xx_arg2;
      }

      diagbuf_mpd_commit(pMsg);
    }

    msg_save_trace((const void *)const_blk, f3_args, 2, (void *)pMsg, qshrink_type);
  }
} /* qsr_int_msg_send_2 */

/*===========================================================================
FUNCTION qsr_int_msg_send_3

DESCRIPTION
   This function sends out a debug message with three arguments.
   This function, or the macro QSR_MSG_3 should not be called directly,
   MSG* macros are converted to QSR_MSG* macros by text-replacement.

DEPENDENCIES
   Diag_LSM_Init() should be called before calling this function.
===========================================================================*/
void qsr_int_msg_send_3( const msg_qsr_const_type *const_blk, uint32 isHash, uint32 xx_arg1, uint32 xx_arg2, uint32 xx_arg3 )
{
  if ( diag_qdi_handle >= DIAG_FD_MIN_VALUE )
  {
    byte *pMsg = NULL;
    const unsigned int num_args = 3;  /* # of message arguments */
    uint8 qshrink_type = DIAG_F3_TRACE_STANDARD;
    msg_arg_type f3_args[3];
    unsigned int nLength = 0;

    f3_args[0] = xx_arg1;
    f3_args[1] = xx_arg2;
    f3_args[2] = xx_arg3;

    if ( isHash )
    {
      qshrink_type = DIAG_F3_TRACE_QSHRINK20;
    }

    pMsg = qsr_msg_send_prep(const_blk, num_args, &nLength, isHash);
    if ( pMsg )
    {
      if ( QSR_BITFLAG_IS_HASH & isHash )
      {
        msg_qsr_type *pTemp = (msg_qsr_type *)(pMsg);
        /* Store the arguments in the buffer. */
        pTemp->args[0] = xx_arg1;
        pTemp->args[1] = xx_arg2;
        pTemp->args[2] = xx_arg3;
      }
      else
      {
        msg_ext_type *pTemp = (msg_ext_type *)(pMsg);
        /* Store the arguments in the buffer. */
        pTemp->args[0] = xx_arg1;
        pTemp->args[1] = xx_arg2;
        pTemp->args[2] = xx_arg3;
      }

      diagbuf_mpd_commit(pMsg);
    }

    msg_save_trace((const void *)const_blk, f3_args, 3, (void *)pMsg, qshrink_type);
  }
} /* qsr_int_msg_send_3 */



/*===========================================================================

FUNCTION MSG_V2_SEND

DESCRIPTION
  This function is similar to QSR_MSG_SEND function, except the msg value. The msg value in const_blk may be
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v2_send( const msg_v2_const_type *const_blk )
{
  qsr_int_msg_send((msg_qsr_const_type *)const_blk, MSG_V2_CHECK_IS_HASH(const_blk->msg));

} /* msg_v2_send */

/*===========================================================================

FUNCTION MSG_V2_SEND_1

DESCRIPTION
  This function is similar to QSR_MSG_SEND_1 function, except the msg value. The msg value in const_blk may be
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v2_send_1( const msg_v2_const_type *const_blk, uint32 xx_arg1 )
{
  qsr_int_msg_send_1((msg_qsr_const_type *)const_blk,
                     MSG_V2_CHECK_IS_HASH(const_blk->msg), xx_arg1);

} /* msg_v2_send_1 */


/*===========================================================================

FUNCTION MSG_V2_SEND_2

DESCRIPTION
  This function is similar to QSR_MSG_SEND_2 function, except the msg value. The msg value in const_blk may be
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v2_send_2( const msg_v2_const_type *const_blk, uint32 xx_arg1, uint32 xx_arg2 )
{
  qsr_int_msg_send_2((msg_qsr_const_type *)const_blk,
                     MSG_V2_CHECK_IS_HASH(const_blk->msg), xx_arg1, xx_arg2);

} /* msg_v2_send_2 */

/*===========================================================================

FUNCTION MSG_V2_SEND_3

DESCRIPTION
  This function is similar to QSR_MSG_SEND_3 function, except the msg value. The msg value in const_blk may be
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void msg_v2_send_3( const msg_v2_const_type *const_blk, uint32 xx_arg1,
                    uint32 xx_arg2, uint32 xx_arg3 )
{
  qsr_int_msg_send_3((msg_qsr_const_type *)const_blk,
                     MSG_V2_CHECK_IS_HASH(const_blk->msg), xx_arg1, xx_arg2, xx_arg3);

} /* msg_v2_send_3 */


/*===========================================================================

FUNCTION MSG_V2_SEND_VAR

DESCRIPTION
  This function is similar to QSR_MSG_SEND_VAR function, except the msg value. The msg value in const_blk may be
  converted to a hash value, if the QSHRINK20 tool is executed in the linker output.

  The msg value is a pointer to a string, if its value is divisible by 4, else it is a hash value.

  Do not call directly; use macro MSG_* defined in msg.h

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

===========================================================================*/
DLL_API_GLOBAL void
msg_v2_send_var( const msg_v2_const_type *const_blk, uint32 num_args, ... )
{
  if ( diag_qdi_handle >= DIAG_FD_MIN_VALUE )
  {
    byte *pMsg = NULL;
    unsigned int nLength = 0;
    unsigned int i;
    uint32 *args = NULL;
    msg_arg_type f3_args[MAX_VAR_ARGS];  // Assuming at most MAX_VAR_ARGS arguments
    va_list arg_list;     /* ptr to the variable argument list */
    uint8 qshrink_type = DIAG_F3_TRACE_STANDARD;

    va_start(arg_list, num_args);

    /* Store arguments from variable list. */
    for ( i = 0; i < MIN(num_args, MAX_VAR_ARGS); i++ )
    {
      f3_args[i] = va_arg(arg_list, uint32);
    }               /* for */

    /* Reset variable arguments */
    va_end(arg_list);

    if ( QSR_BITFLAG_IS_HASH & MSG_V2_CHECK_IS_HASH(const_blk->msg) )
    {
      qshrink_type = DIAG_F3_TRACE_QSHRINK20;
    }

    pMsg = qsr_msg_send_prep((msg_qsr_const_type *)const_blk, num_args, &nLength, MSG_V2_CHECK_IS_HASH(const_blk->msg));
    if ( pMsg )
    {
      if ( QSR_BITFLAG_IS_HASH & MSG_V2_CHECK_IS_HASH(const_blk->msg) )
      {
        msg_qsr_type *pTemp = (msg_qsr_type *)(pMsg);
        args = MSG_LARGE_ARGS(pTemp->args);
      }
      else
      {
        msg_ext_type *pTemp = (msg_ext_type *)(pMsg);
        args = MSG_LARGE_ARGS(pTemp->args);
      }

      /* Initialize variable arguments */
      va_start(arg_list, num_args);
      /* Store arguments from variable list. */
      for ( i = 0; i < num_args; i++ )
      {
        args[i] = va_arg(arg_list, uint32);
      }
      /* Reset variable arguments */
      va_end(arg_list);

      diagbuf_mpd_commit(pMsg);
    }

    msg_save_trace((const void *)const_blk, f3_args, MIN(num_args, MAX_VAR_ARGS), (void *)(pMsg), qshrink_type);
  }
} /* msg_v2_send_var */

/*----------------------------------------------------------------------------
                                          Internal functions
-----------------------------------------------------------------------------*/
/*
Computes message mask size.
*/
static uint32 Diag_LSM_Msg_ComputeMaskSize( void )
{
  uint32 msg_mask_size = 0;
  int i = 0;
  for ( i = 0; i < MSG_MASK_TBL_CNT; i++ )
  {
    msg_mask_size += sizeof(uint16) + sizeof(uint16);
    msg_mask_size += (msg_mask_tbl[i].ssid_last - msg_mask_tbl[i].ssid_first + 1) * sizeof(uint32);
  }
  return (msg_mask_size);
}

/*===========================================================================
FUNCTION Diag_LSM_Msg_Init

DESCRIPTION
Initializes the Diag Message service mapping layer.
Masks will be updated later after diagID has been set.

DEPENDENCIES
None

===========================================================================*/
boolean Diag_LSM_Msg_Init( void )
{
  boolean status = TRUE;
  gMsg_Mask_Size = Diag_LSM_Msg_ComputeMaskSize();
  if ( !gMsg_Mask_Read_Buf )
  {
    gMsg_Mask_Read_Buf = malloc(gMsg_Mask_Size);
    if ( !gMsg_Mask_Read_Buf )
    {
      status = FALSE;
    }
    else
    {
      status = TRUE;
    }
  }
  return (status);
} /* Diag_LSM_Msg_Init() */

/*===========================================================================
FUNCTION Diag_LSM_Msg_DeInit

DESCRIPTION
Prepares mapping layer exit for Diag message service.
Frees the global buffer allocated to read in message mask.

DEPENDENCIES
None.


===========================================================================*/
boolean Diag_LSM_Msg_DeInit( void )
{
  boolean success = TRUE;
  if ( gMsg_Mask_Read_Buf )
  {
    free(gMsg_Mask_Read_Buf);
    gMsg_Mask_Read_Buf = NULL;
  }
  return (success);
} /* boolean Diag_LSM_Msg_DeInit() */

#ifndef DIAG_USERPD_TEMP
/*===========================================================================

FUNCTION msg_update_v4_ss_table

DESCRIPTION
  Same function as msg_update_v3_ss_table, but using v4 table and v4_entries.

DEPENDENCIES 
 
PARAMETERS 
  curr_ssid - SSID to be updated in the table
  ss_mask - Mask to update for ssid specified
  stream_id - SSID for this stream ID will be updated. Valid stream IDs are
              DIAG_MIN_STREAM_ID to DIAG_MAX_STREAM_ID.
  preset_id - SSID for this preset ID will be updated. Valid preset IDs are
              DIAG_MIN_PRESET_ID to DIAG_MAX_PRESET_ID.
RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
static void msg_update_v4_ss_table (uint16 curr_ssid, uint32 ss_mask, uint8 stream_id, uint8 preset_id)
{
  uint32 mid_index;
  uint32 start_index = 0; 
  uint32 end_index = msg_v4_ss_num_table_entries - 1;
  uint32 table_index = msg_v4_ss_num_table_entries;
  int ss_index; /* needs to be signed; not enormously concerned about range */
  uint16 rt_mask = 0;

  if (msg_v4_ss_num_table_entries == 0)
  {
   return;
  }

  if (stream_id > DIAG_MAX_STREAM_ID )
  {
    return;
  }
  rt_mask = 0x0001;
  /* the binary search modified*/
  while( start_index <= end_index )
  {
    mid_index = ( start_index + end_index ) >> 1;

    if( msg_v4_ss_table[mid_index].ss_id == curr_ssid )
    {
      table_index = mid_index;
      break;
    }
    else if(curr_ssid > msg_v4_ss_table[mid_index].ss_id)
    {
      start_index = mid_index + 1;
    }
    else
    {
      end_index = mid_index - 1;
    }
  }

  if (table_index >= msg_v4_ss_num_table_entries)
  { 
    return;
  }
  /* came to an area where ssid match occurred */ 

  for (ss_index = table_index;
        ss_index >= 0 && msg_v4_ss_table[ss_index].ss_id == curr_ssid;
        ss_index--)
  {
    if (msg_v4_ss_table[ss_index].ss_mask & ss_mask)
    {
      msg_v4_ss_table[ss_index].rt_mask |= rt_mask;
    }
    else {
      msg_v4_ss_table[ss_index].rt_mask &= ~rt_mask;
    }
  }

  for (ss_index = table_index+1;
        ss_index < msg_v4_ss_num_table_entries && msg_v4_ss_table[ss_index].ss_id == curr_ssid;
        ss_index++)
  {
    if (msg_v4_ss_table[ss_index].ss_mask & ss_mask){
      msg_v4_ss_table[ss_index].rt_mask |= rt_mask;
    }
    else {
      msg_v4_ss_table[ss_index].rt_mask &= ~rt_mask;
    }
  }

} /* msg_update_v4_ss_table () */
#endif

/*===========================================================================

FUNCTION    msg_mask_is_set

DESCRIPTION
  Checks if the msg mask is set for this user process

DEPENDENCIES
  None.

RETURN VALUE
  FALSE if not set, else TRUE

SIDE EFFECTS
  None

===========================================================================*/
boolean msg_mask_is_set(void)
{
  for (int i = 0; i < MSG_MASK_TBL_CNT; i++)
  {
     for (int j = 0; j < DIAG_MAX_STREAM_ID; j++)
     {
        if (msg_mask_tbl[i].rt_mask_array[j] > 0)
        {
            return TRUE;
        }
     }
  }
  return FALSE;
} /* msg_mask_is_set */

/*===========================================================================
FUNCTION msg_update_mask

DESCRIPTION
   Updates the msg mask. This function is called whenever there is a
   mask change event.

DEPENDENCIES
None.


===========================================================================*/
boolean msg_update_mask( void )
{

  boolean status = TRUE;
#ifndef DIAG_USERPD_TEMP
  uint32 rt_mask = 0;
#endif
  if ( gMsg_Mask_Read_Buf && (diag_qdi_handle >= DIAG_FD_MIN_VALUE) )
  {
    dword maskLen_Recvd = 0;
    if ( diag_lsm_ioctl(DIAG_IOCTL_GETMSGMASK, NULL, 0, gMsg_Mask_Read_Buf, gMsg_Mask_Size, &maskLen_Recvd) && maskLen_Recvd <= gMsg_Mask_Size )
    {
      /* Copy the mask received into this process' msg_mask_tbl */
      int i = 0;
      uint32 length = 0;
#ifndef DIAG_USERPD_TEMP
      int j = 0;
      boolean valid_ssid = FALSE;
#endif
      msg_mask_read_buffer_type *ptemp_buf = (msg_mask_read_buffer_type *)gMsg_Mask_Read_Buf;
      byte *ptemp_byte = gMsg_Mask_Read_Buf;
      for ( i = 0; i < MSG_MASK_TBL_CNT; i++ )
      {
        ptemp_buf = (msg_mask_read_buffer_type *)ptemp_byte;
        if ( ptemp_buf->ssid_first == msg_mask_tbl[i].ssid_first &&
             ptemp_buf->ssid_last == msg_mask_tbl[i].ssid_last )
        {
          ptemp_byte += sizeof(ptemp_buf->ssid_first) + sizeof(ptemp_buf->ssid_last);
          length = (msg_mask_tbl[i].ssid_last - msg_mask_tbl[i].ssid_first + 1) * sizeof(uint32);
          memcpy((byte *)(msg_mask_tbl[i].rt_mask_array[DIAG_STREAM_1 - 1]), ptemp_byte, length);
          ptemp_byte += length;
#ifndef DIAG_USERPD_TEMP
	      /*update msg_v4_ss_table*/
	      for (j = msg_mask_tbl[i].ssid_first;j<=msg_mask_tbl[i].ssid_last;j++) 
	     {
	       /*update thte runtime mask*/
           valid_ssid = msg_get_ssid_rt_mask(j, &rt_mask);
	       if(valid_ssid)
	       {
	         msg_update_v4_ss_table (j, rt_mask, DIAG_STREAM_1, DIAG_PRESET_MASK_ALL);/*hardcode stream_id and preset_id here*/
	       }
	     }
#endif
        }
        else
        {
          /* Something is wrong if the ssid ranges don't match, just bail out */
          status = FALSE;
          break;
        }
      }
    }
    else
    {
      status = FALSE;
    }
  }
  else
  {
    status = FALSE;
  }
  return (status);
} /* msg_update_mask() */


/*===========================================================================
FUNCTION msg_format_filename2

DESCRIPTION
retrieves the position of filename from full file path.

DEPENDENCIES
None.
===========================================================================*/
static const char*
msg_format_filename2( const char *filename )
{
  const char *p_front = filename;
  const char *p_end = filename + strlen(filename);

  while ( p_end != p_front )
  {
    if ( (*p_end == '\\') || (*p_end == ':') )
    {
      p_end++;
      break;
    }
    p_end--;
  }
  return (p_end);
} /*const char *msg_format_filename2() */

/*===========================================================================
FUNCTION msg_get_time

DESCRIPTION
  Gets Timestamp and overwrite the 10 least significant bits if the WCDMA 
  frame callback is registered with Diag. 
  In other words, not overwrite the 10 least significant bits of the timestamp 
  in cases where the WCDMA frame callback is not registered with Diag.

DEPENDENCIES
  None.
===========================================================================*/
static uint8
msg_get_time( qword *ts )
{
  uint32 frame_number=0;

  uint8 time_type;
  /* Get timestamp for this message ASAP */
   time_type = diag_time_get_LSM(*ts);

  /* Get the frame number (either SFN or CFN, depending on the
  ** state of the mobile), extract its least significant 10 bits
  ** and add these bits to the timestamp.
  */

  if ( diag_wcdma_wl1_frame_cb != NULL )
  {
    frame_number = diag_wcdma_wl1_frame_cb() & MSG_FN_MASK;
	(*ts)[0] >>= MSG_FN_NUM_BITS;
    (*ts)[0] <<= MSG_FN_NUM_BITS;
    (*ts)[0] |= frame_number;
  }
  
  return time_type;
} /* msg_get_time() */


/*
In LSM, we're getting the mask only for one SSID.
Getting masks for a range of SSIDs is needed only in the DCM
counterpart of this function.
So simplifying the search logic in LSM.
Little optimizations.
*/
static boolean
msg_get_ssid_rt_mask( uint16 ssid, uint32 *mask )
{
  boolean success = FALSE;
  if ( mask )
  {
    const msg_mask_tbl_type *tbl = msg_mask_tbl;
    const msg_mask_tbl_type *tbl_last = &msg_mask_tbl[MSG_MASK_TBL_CNT - 1];
    const uint32 *mask_array = NULL;  /* First pass forces a search */
    /* Initialize as successful.  If invalid entry found, set to FALSE. */
    success = TRUE;
    /* Table search: if mask_array is not set */
    while ( !mask_array && tbl <= tbl_last )
    {
      if ( ssid >= tbl->ssid_first && ssid <= tbl->ssid_last )
      {
        mask_array = tbl->rt_mask_array[DIAG_STREAM_1 - 1];
      }
      else
      {
        tbl++;        /* Look at next table entry */
      }
    }

    /* At the end of the loop, we should have the mask_array,
    o/w the ssid was invalid. */

    if ( mask_array )
    {
      /* Valid SSID found.  Write mask to caller's mask. */
      *mask = mask_array[ssid - tbl->ssid_first];
    }
    else
    {
      /* Invalid SSID.  Indicate failure */
      *mask = 0;
      success = FALSE;
    }
  }

  return (success);
}               /* msg_get_ssid_rt_mask() */

/*===========================================================================
FUNCTION MSG_SPRINTF_PREP

DESCRIPTION
   Prepares the buffer needed by msg_sprintf().
   Allocates, fills in all data except arguments, and returns a pointer
   to the allocated message buffer.  It also handles message statisitics.

RETURN VALUE
   Returns the allocated buffer, and the length of the buffer

DEPENDENCIES
   None
===========================================================================*/
static byte*
msg_sprintf_prep( const msg_const_type *pconst_blk, unsigned int num_args, unsigned int *pLength )
{
  uint32 rt_mask;
  boolean valid_ssid = FALSE;
  byte *pMsg = NULL;
  unsigned int alloc_len = FPOS(msg_ext_store_type, args) +
                           num_args * FSIZ(msg_ext_store_type, args[0]);

  /* Check the runtime mask */
  valid_ssid =
     msg_get_ssid_rt_mask(pconst_blk->desc.ss_id, &rt_mask);

  if ( valid_ssid && (pconst_blk->desc.ss_mask & rt_mask) )
  {
    /* Get a pointer to a buffer. If it's a NULL pointer, there is
       no memory in the UserPD diag buffer.*/
    pMsg = (byte *)diagbuf_mpd_alloc(0, alloc_len, 0, DIAGBUF_ALLOC_REQUEST_MSG, 0, DIAG_EXT_API_VERSION_0);
    if ( pMsg )
    {
      msg_ext_store_type *pTemp = (msg_ext_store_type *)((byte *)pMsg);

      if ( pLength )
      {
        *pLength = alloc_len; /* return value, of how much memory is allocated */
      }

      if ( diag_wcdma_wl1_frame_cb != NULL )
      {
        pTemp->hdr.ts_type = MSG_TS_TYPE_GW;
      }
      else
      {
        pTemp->hdr.ts_type = MSG_TS_TYPE_CDMA_FULL;
      }

      pTemp->hdr.num_args = (uint8)num_args;
      pTemp->hdr.drop_cnt = (unsigned char)((msg_drop_delta > 255) ? 255 : msg_drop_delta);
      msg_drop_delta = 0;   /* Reset delta drop count */
      msg_get_time(&(pTemp->hdr.ts));

      /* Set the pointer to the constant blk, to be expanded by msg_sprintf */
      pTemp->const_data_ptr = pconst_blk;
    }
    else
    {
      msg_drop_delta++;
      msg_dropped++;        /* count up one more ignored message */
    }
  }

  return (pMsg);
}               /* msg_sprintf_prep() */

/*===========================================================================
FUNCTION qsr_msg_send_prep

DESCRIPTION
   Internal function.
   Prepares the buffer that is sent to diag driver by the qsr_msg_send* functions.
   The const block is expanded in the context of the caller.
   If pLength is passed in as NULL, the msg will still be created but the
   calling function wont know how much data has been allocated.
===========================================================================*/
static byte* qsr_msg_send_prep( const msg_qsr_const_type *const_blk,
                                unsigned int num_args, unsigned int *pLength, uint32 isHash )
{
  uint32 rt_mask;
  boolean valid_ssid = FALSE;
  byte *pMsg = NULL;
  int filename_len = 0;
  char *str_buf;
  char null_terminator = '\0';

  if ( pLength )
  {
    *pLength = 0;
  }
  /* Check the runtime mask */
  valid_ssid = msg_get_ssid_rt_mask(const_blk->desc.ss_id, &rt_mask);
  if ( valid_ssid && (const_blk->desc.ss_mask & rt_mask) )
  {
    unsigned int alloc_len = 0;

    /* total number of bytes to be allocated, including space for the hash value */
    if ( QSR_BITFLAG_IS_HASH & isHash )
    {
      alloc_len = FPOS(msg_qsr_type, args) + /* includes header,desc and hash size */
                  num_args * FSIZ(msg_qsr_type, args[0]);
    }
    else
    {
      for ( str_buf = (char *)const_blk->msg_hash; *str_buf != ':'; str_buf++ ) ;
      filename_len = str_buf - (char *)const_blk->msg_hash;
      /* total number of bytes to be allocated, including dereferenced FileName and Format strings */
      alloc_len = FPOS(msg_ext_type, args) +
                  num_args * FSIZ(msg_ext_type, args[0]) +
                  MIN(MSG_MAX_STRLEN, (int)strlen((char *)const_blk->msg_hash + filename_len + 1)) + 1 +
                  filename_len + 1;

    }
    /* Get a pointer to a buffer.  If it's a NULL pointer, there is no memory in the client heap. */
    pMsg = (byte *)diagbuf_mpd_alloc(0, alloc_len, 0, DIAGBUF_ALLOC_REQUEST_MSG, 0, DIAG_EXT_API_VERSION_0);
    if ( pMsg )
    {
      if ( pLength )
      {
        *pLength = alloc_len; /* return the number of bytes allocated. */
      }

      if ( QSR_BITFLAG_IS_HASH & isHash )
      {
        /* Find the position to copy in the header, const expanded values etc */
        msg_qsr_type *pTemp = (msg_qsr_type *)((byte *)pMsg);

        pTemp->hdr.cmd_code = DIAG_QSR_EXT_MSG_TERSE_F; /* cmd_code = 146 for QSR messages */

        if ( diag_wcdma_wl1_frame_cb != NULL )
          pTemp->hdr.ts_type = MSG_TS_TYPE_GW;
        else
          pTemp->hdr.ts_type = MSG_TS_TYPE_CDMA_FULL;


        pTemp->hdr.num_args = (uint8)num_args;
        pTemp->hdr.drop_cnt = (unsigned char)((msg_drop_delta > 255) ? 255 : msg_drop_delta);
        msg_drop_delta = 0;   /* Reset delta drop count */
        msg_get_time(&(pTemp->hdr.ts));

        /* expand it now.
         The order is: hdr (already done),desc,hash,args. args are copied in the qsr_msg_send* functions */
        pTemp->desc.line = const_blk->desc.line;
        pTemp->desc.ss_id = const_blk->desc.ss_id;
        pTemp->desc.ss_mask = const_blk->desc.ss_mask;
        pTemp->msg_hash = const_blk->msg_hash;
      }
      else
      {
        msg_qsr_v2_type *pTemp = (msg_qsr_v2_type *)((byte *)pMsg);
        uint32 fmt_pos = 0;
        uint16 tmp_length = 0;

        pTemp->hdr.cmd_code = DIAG_EXT_MSG_F; /* cmd_code = 146 for QSR messages */

        if ( diag_wcdma_wl1_frame_cb != NULL )
          pTemp->hdr.ts_type = MSG_TS_TYPE_GW;
        else
          pTemp->hdr.ts_type = MSG_TS_TYPE_CDMA_FULL;


        pTemp->hdr.num_args = (uint8)num_args;
        pTemp->hdr.drop_cnt = (unsigned char)((msg_drop_delta > 255) ? 255 : msg_drop_delta);
        msg_drop_delta = 0;   /* Reset delta drop count */
        msg_get_time(&(pTemp->hdr.ts));

        /* expand it now.
         The order is: hdr (already done),desc,args,format string, filename. args are copied in the qsr_msg_send* functions */
        pTemp->desc.line = const_blk->desc.line;
        pTemp->desc.ss_id = const_blk->desc.ss_id;
        pTemp->desc.ss_mask = const_blk->desc.ss_mask;

        /*The arguments are copied in qsr_msg_send* functions*/
        /*Copy the fmt string*/
        fmt_pos = sizeof(msg_hdr_type) + sizeof(msg_desc_type) + num_args * sizeof(pTemp->args[0]);
        tmp_length = MIN(MSG_MAX_STRLEN, (int)strlen((char *)const_blk->msg_hash + filename_len + 1));
        memscpy((void *)((char *)(pMsg) + fmt_pos), (alloc_len - fmt_pos),
		        (void *)(const_blk->msg_hash + filename_len + 1), tmp_length);

        /*Copy the null terminator*/
        fmt_pos += tmp_length;

        *((char *)(pMsg) + fmt_pos) = null_terminator;
        fmt_pos++;

        /*Copy the file name*/
        memscpy((void *)((char *)(pMsg) + fmt_pos), (alloc_len - fmt_pos),
		       (void *)(const_blk->msg_hash), filename_len);
        fmt_pos += filename_len;

        /*Copy the null terminator*/
        *((char *)(pMsg) + fmt_pos) = null_terminator;
      }
    }
    else
    {
      msg_drop_delta++;
      msg_dropped++;        /* count up one more ignored message */
    } /* if (pMsg) */
  } /* if (valid_ssid && (const_blk->desc.ss_mask & rt_mask)) */

  return (pMsg);

} /* qsr_msg_send_prep */

