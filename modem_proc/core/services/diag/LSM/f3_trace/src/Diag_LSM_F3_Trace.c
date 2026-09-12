
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              Diag F3 Trace Saving

GENERAL DESCRIPTION

 Contains functionality to support F3 message trace retrieval from process space.


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2013-2019 by QUALCOMM Technologies, Incorporated. All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/core.mpss/10.0/services/diag/LSM/f3_trace/src/Diag_LSM_F3_Trace.c#1 $

when       who    what, where, why
--------   ---    ----------------------------------------------------------
03/01/18   sa     Added diag_f3_trace_enabled_check API.
03/14/17   rs     Fixed the swapped arguments savevars and savetime in diag_f3_save_msg()
09/06/16   as     Expose diag_lsm_f3_trace_init_detail_mask_config()
08/25/16   as     Fixed Klocwork issue
04/11/16   ph     Added support for QShrink 4.0
12/15/15   gn     Qtimer FR28430 changes 
06/18/15   sa     Added support for time sync feature
01/29/15   xy     Fixed compilation warnings
09/19/14   sr     Changed the flag diag_f3_trace_wrap_flag to volatile
03/11/14   sr     Resolved compiler warnings
02/18/13   ph     Resolved concurrent case where index goes out of bounds.
12/19/13   rh     Save message args individually instead of as a group
11/18/13   ph     Remove intermediate buffer in F3 Trace as part of optimization.
01/25/13   sg     Created File
===========================================================================*/

#include "customer.h"
#include "comdef.h"
#include "osal.h"
#include "Diag_LSM_F3_Trace.h" // internal
#include "err.h"
#include "msg.h"
#include "Diag_LSM.h"
#include "Diag_LSMi.h"
#include <stringl.h>
#include "diagdiag_v.h" //for qsr_v4_get_address_table_index
#include "time_svc.h" //for time_get
#include "DDITimetick.h"
#include "dll_global_def.h"

#include "ULog_Diag.h"

#ifndef DIAG_USERPD_TEMP
/* QShrink 4 parameters referenced from msg.c */
extern char* qsr_4_0_msg_aligned_start_address;
extern char* qsr_4_0_msg_unaligned_start_address;
extern char* qsr_4_0_msg_end_address;
extern uint32 msg_v4_ss_num_aligned_entries;
extern msg_v4_ss_aam_table_type msg_v4_ss_aam_table;
extern uint32 msg_v4_ss_num_unaligned_entries;
extern msg_v4_ss_uam_table_type msg_v4_ss_uam_table;
extern uint32 msg_v4_ss_num_table_entries; 
extern msg_v4_ssid_table_type msg_v4_ss_table;
#endif

extern void qsr_v4_incr_drop_count_f3(void);
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

#define DIAG_F3_TRACE_GET_TYPE(x)     ((x) >> 13)
#define DIAG_F3_TRACE_MSG_NUM_ARGS(x) (((x) >> 8) & 0x1F)
#define DIAG_F3_TRACE_MSG_TYPE(x) (((x) >> 4) & 0xF)
#define DIAG_F3_TRACE_BYTE_SIZED_ARGS(x) (((x) & 0xC) >> 2)
#define DIAG_F3_TRACE_GET_DETAILS(x)  ((x) & 0xF)
#define DIAG_F3_TRACE_IS_HEADER_VALID(x) ((x)==diag_F3_TRACE_VALID_HEADER_FLAG)

// MIGRATION NOTE: recover_f3.cmm depends upon err_f3_trace_wrap_flag
volatile uint8                   diag_f3_trace_wrap_flag;
static uint32                  diag_f3_trace_buffer_head;
static diag_f3_trace_id_type    diag_f3_trace_buffer_id;
uint8  *diag_f3_trace_buffer = NULL;

typedef struct
{
   void* const_data_ptr; /* can be ptr to msg_const_type or msg_qsr_const_type */
   diag_f3_trace_tstamp_type tstamp;

} msg_debug_store_type;

static void diag_f3_trace_to_buffer(uint32, uint8*, int); // forward reference

static boolean diag_f3_trace_buffer_init(void); // forward reference

static int diag_f3_trace_buffer_initialized = FALSE;
static int diag_f3_trace_buffer_init_begin = FALSE;
static unsigned int diag_f3_trace_buffer_length = 0;

static boolean diag_f3_trace_time_init(void);
static int diag_f3_trace_time_initialized = FALSE;
static int diag_f3_trace_time_init_begin = FALSE;

static DalDeviceHandle *hTimerHandle = NULL;

/*Default values*/
static uint8 diag_f3_trace_control_mask = DIAG_F3_TRACE_CONTROL_MASK_DEFAULT_VAL;
static uint8 diag_f3_trace_detail_mask = DIAG_F3_TRACE_DETAIL_MASK_DEFAULT_VAL;

extern boolean diag_time_initialized_lsm;

/* Time Get Function Pointer from diag.c */
extern uint8 (*diag_time_get_LSM)(qword time);

extern boolean diag_time_init_LSM(void);

extern uint32 diag_masks;

/*===========================================================================

FUNCTION DIAG_F3_TRACE_INIT_CONTROL_CONFIG

DESCRIPTION
  Initializes the configuration values.

DEPENDENCIES
  This function is not mutex protected.  It is assumed it will only be
  called from serialized init functions (rcinit).

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void diag_lsm_f3_trace_init_control_mask_config(uint8 control)
{
   diag_f3_trace_control_mask = (control & 0xFF);

} /* diag_lsm_f3_trace_init_control_mask_config */


/*===========================================================================

FUNCTION DIAG_F3_TRACE_INIT_DETAIL_CONFIG

DESCRIPTION
  Initializes the configuration values.

DEPENDENCIES
  This function is not mutex protected.  It is assumed it will only be
  called from serialized init functions (rcinit).

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
DLL_API_GLOBAL void diag_lsm_f3_trace_init_detail_mask_config(uint8 detail)
{
   diag_f3_trace_detail_mask = (detail & 0xFF);

} /* diag_lsm_f3_trace_init_detail_mask_config */


/*===========================================================================

FUNCTION DIAG_LSM_F3_TRACE_INIT

DESCRIPTION
  Initializes the buffer provided by client for F3 trace saving


RETURN VALUE
  SUCCESS
  FAILURE if the client has provided invalid buffer

SIDE EFFECTS
  None
===========================================================================*/
DLL_API_GLOBAL boolean diag_lsm_f3_trace_init(uint8 *address, unsigned int length)
{
   diag_f3_trace_buffer = (uint8 *)address;
   diag_f3_trace_buffer_length = length;

   if ((diag_f3_trace_buffer)&&(diag_f3_trace_buffer_length>0))
   {
      diag_f3_trace_buffer_init();
      return TRUE;
   }
   else
   {
      printf("Invalid buffer provided");
      return FALSE;
   }

   /* Initilize the time API to use if not initialized by this time */
   if(!diag_time_initialized_lsm)
   {
      diag_time_init_LSM();
   }

}
/*===========================================================================

FUNCTION DIAG_F3_TRACE_TO_BUFFER

DESCRIPTION
  Saves the buffer to the RAM buffer containing trace information.

DEPENDENCIES
  Interrupts must be locked.  FEATURE_ERR_EXTENDED_STORE must be defined.

RETURN VALUE
  If < 0, failure occurred.  Otherwise, success.

SIDE EFFECTS
  None
===========================================================================*/
static void diag_f3_trace_to_buffer(uint32 index, uint8 *src, int length)
{
   int i;

   if (index >= diag_f3_trace_buffer_length)
   {
      index -= diag_f3_trace_buffer_length;
   }

   /* Save to the circular buffer according to where the head points */
   for (i = 0; i < length; i++)
   {
      diag_f3_trace_buffer[index] = src[i];
      index++;
      if (index >= diag_f3_trace_buffer_length)
      {
         index -= diag_f3_trace_buffer_length;
      }
   }

   return;
}

/*===========================================================================

FUNCTION DIAG_F3_TRACE_BUFFER_INIT

DESCRIPTION
  Initializes the circular buffer indices.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static boolean diag_f3_trace_buffer_init(void)
{
   /* Grab init_begin flag to make sure only one entity initializes the buffer */
   if (!osal_atomic_compare_and_set((osal_atomic_word_t *)&diag_f3_trace_buffer_init_begin, FALSE, TRUE)) return (FALSE);

   /* Set up the buffer indices */
   diag_f3_trace_buffer_head = 0;
   diag_f3_trace_wrap_flag = FALSE;

   /* Initialize build ID associated with this run */
   /* (timestamp set when file is saved) */
   diag_f3_trace_buffer_id.timestamp = 0;
   diag_f3_trace_buffer_id.buffer_ptr = (uint32)(&diag_f3_trace_buffer_id);
   memset(diag_f3_trace_buffer_id.build, 0, DIAG_F3_TRACE_MAX_ID_LENGTH);
   strlcpy((char *)diag_f3_trace_buffer_id.build, "LPASS\0",
           DIAG_F3_TRACE_MAX_ID_LENGTH);

   /* Clear out buffer before we start putting data in it */
   memset(diag_f3_trace_buffer, 0, diag_f3_trace_buffer_length);

   /* Once initialized to true, the trace records will be allowed to save */
   diag_f3_trace_buffer_initialized = TRUE;

   return (TRUE);
}

static boolean diag_f3_trace_time_init(void)
{
   /* Grab init_begin flag to make sure only one entity initializes the buffer */
   if (!osal_atomic_compare_and_set((osal_atomic_word_t*)&diag_f3_trace_time_init_begin, FALSE, TRUE))
      return (FALSE);

   /* Get the pointer to Dal device handle for System Timer */
   DalTimetick_Attach("SystemTimer",&hTimerHandle);
   
   /* Enable system timer */
   if (hTimerHandle)
   {
      DalTimetick_Enable(hTimerHandle,1);
      DalTimetick_InitTimetick64(hTimerHandle);

      diag_f3_trace_time_initialized = TRUE;
   }
   
   return(diag_f3_trace_time_initialized);
}

/*===========================================================================

FUNCTION DIAG_F3_TRACE_BUFFER_INIT

DESCRIPTION
  Initializes the circular buffer indices.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean diag_lsm_f3_trace_deinit(void)
{

   diag_f3_trace_buffer_init_begin = FALSE;

   /* Set up the buffer indices */
   diag_f3_trace_buffer_head = 0;
   diag_f3_trace_wrap_flag = FALSE;

   /* Clear out buffer */
   if (diag_f3_trace_buffer) memset(diag_f3_trace_buffer, 0, diag_f3_trace_buffer_length);

   /* Once initialized to true, the trace records will be allowed to save */
   diag_f3_trace_buffer_initialized = FALSE;
   diag_lsm_f3_trace_init_control_mask_config(0);
   diag_lsm_f3_trace_init_detail_mask_config(0);
   return (TRUE);
}

static uint32 diag_f3_trace_secure_offset(uint32 length)
{
   uint32 old_head;
   uint32 new_head;

   do
   {
      old_head = diag_f3_trace_buffer_head;
      new_head = old_head + length;
      if (new_head >= diag_f3_trace_buffer_length)
      {
         new_head -= diag_f3_trace_buffer_length;
         diag_f3_trace_wrap_flag = TRUE;
      }
   }
   while (!osal_atomic_compare_and_set((osal_atomic_word_t *)&diag_f3_trace_buffer_head, old_head, new_head));

   return (old_head);
}

/*===========================================================================

FUNCTION DIAG_F3_TRACE_SET_UP_MSG_HEADER

DESCRIPTION
  Creates the header for a F3 message record in the trace buffer.  The
  header structure is as follows:
  Total = 16 bits
    * 15:13  3 bits = Record type
    * 12     1 bit  = qtimer flag
    * 11:8   4 bits = number of arguments 
    * 7      1 bit  = msg version (extended)
    * 6:4    3 bits = msg type (qshrink version)
    * 3:2    2 bits = arg size
    * 1      1 bit  = savetime
    * 0      1 bit  = savevars

DEPENDENCIES
  None

RETURN VALUE
  Header for the F3 record

SIDE EFFECTS
  None

===========================================================================*/
static void diag_f3_trace_set_up_msg_header(int nargs, char savevars, char savetime, char byte_sized, uint32 type, diag_f3_trace_hdr_type *hdr)
{
   /* Zero out config data to start */
   hdr->config = 0;

   /* Add config bits to header */
   hdr->config |= (DIAG_F3_TRACE_MSG_BIT << 13);
   hdr->config |= (nargs & 0xF) << 8;
   hdr->config |= (DIAG_F3_TRACE_QTIMER_BIT<< 12);
/*lint -save -e{734} */
   byte_sized = byte_sized << 2;
/*lint -restore */
   hdr->config |= (byte_sized | savevars | savetime);
   hdr->config |= (type & 0xF) << 4;

   /* Write header flag to header */
   hdr->header_flag = DIAG_F3_TRACE_VALID_HEADER_FLAG;
}

/*=========================================================================== 
FUNCTION  DIAG_F3_TRACE_ENABLED_CHECK
 
DESCRIPTION
    This routine checks if the F3 trace functionality is enabled, based on its
    current mask settings.

RETURN VALUE
    TRUE,  F3 trace is enabled.
    FALSE, F3 trace is not enabled.
===========================================================================*/
boolean diag_f3_trace_enabled_check(void)
{
  boolean return_val = FALSE; /*initialize return value to FALSE */

  /* Verify if the trace control mask i.e 0th bit is set*/
  if( (diag_f3_trace_control_mask & DIAG_F3_TRACE_MSG_BIT) )
  {
    /*Check if 3rd bit set which indicates Diag Mask Debug Mode is enabled */
    if (diag_f3_trace_control_mask & DIAG_F3_TRACE_DIAG_MASK_BIT)
    {
      /*Check if masks are enabled */
      if ( diag_masks & DIAG_MASKS_MSG_BIT )
      {
        return_val = TRUE;
      }
    }
    else
    {
      /* Check if any mesage level bits are set i.e(2nd-7th bits) ignoring F3 arguments (0th bit)
             and time stamp (1st bit) in detail mask */
      if ( (diag_f3_trace_detail_mask>>2) & (0xFF) )
      {
        return_val = TRUE;
      }
    }
  }
    return return_val;
}

// THIS IS THE PRIMARY "PUBLIC" API

void diag_f3_save_msg(const void *constant_data_ptr, msg_arg_type *args, uint8 nargs, void *msg, uint32 type, uint8 arg_bytes)
{
   /* Sanity checks */

   /* Basic data checks */
   if ((diag_f3_trace_buffer == NULL) || (diag_f3_trace_buffer_length == 0))
       return;

   if ((constant_data_ptr == NULL) || (type >= DIAG_F3_TRACE_LAST))
       return;

   /* Is F3 trace msg saving enabled? */
   if (!(diag_f3_trace_control_mask & DIAG_F3_TRACE_MSG_BIT))
      return;

   /* Check masks based on DIAG_F3_TRACE_DIAG_MASK_BIT */
   if (diag_f3_trace_control_mask & DIAG_F3_TRACE_DIAG_MASK_BIT)
   {
      /* Use DIAG masks in this case...so just check to see if msg was created */
      if (!msg)
        return;
   }
   else
   {
      /* Not using DIAG masks in this case,
         so compare F3 trace detail mask with ss_mask */
      uint32 mask;
#ifndef DIAG_USERPD_TEMP
      uint32 qsr_hash_v4 = (uint32)constant_data_ptr;
      uint16 xx_msg_index = 0;
      uint16 msg_index;
#endif
      if (type == DIAG_F3_TRACE_STANDARD)
         mask = ((msg_const_type *)constant_data_ptr)->desc.ss_mask;
#ifndef DIAG_USERPD_TEMP
        else if (type==DIAG_F3_TRACE_QSHRINK40)
        {
          if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_aligned_start_address) &&
                (qsr_hash_v4 < (uint32)qsr_4_0_msg_unaligned_start_address))
           {
             /* Aligned Address Mapping */
             msg_index = (~((uint32)qsr_4_0_msg_aligned_start_address) & qsr_hash_v4) >> 13;
             if (msg_index < msg_v4_ss_num_aligned_entries)
             {
               xx_msg_index = (uint16)msg_v4_ss_aam_table[msg_index].index_to_v4_ssid_table;
             }
           }
           else if ((qsr_hash_v4 >= (uint32)qsr_4_0_msg_unaligned_start_address) && (qsr_hash_v4 < (uint32)qsr_4_0_msg_end_address))
           {
             /* Unaligned address mapping */
             xx_msg_index = qsr_v4_get_address_table_index(qsr_hash_v4);
           }
           else 
           {
             qsr_v4_incr_drop_count_f3(); 
             ULogFront_RealTimePrintf(diag_debug_ulog_handle,1,"diag_f3_save_msg: Invalid hash value : qsr_hash_v4 : 0x%x",qsr_hash_v4); 
             return;
           }
           mask = msg_v4_ss_table[xx_msg_index].ss_mask;
        }
#endif
      else
         mask = ((msg_qsr_const_type *)constant_data_ptr)->desc.ss_mask;

      if (!((diag_f3_trace_detail_mask >> 3) & mask))
          return;
   }

   /* Make sure buffer has been initialized (needed prior to intial entry) */
   if (diag_f3_trace_buffer_initialized == FALSE)
   {
      /* Abort if failure/conflict detected during buffer init */
      if (!diag_f3_trace_buffer_init())
         return;
   }
   
   if (diag_f3_trace_time_initialized == FALSE)
   {
      if (!diag_f3_trace_time_init())
         return;
   }

   /* Proceed with saving msg to buffer */
   {
      int i = 0;
      uint32 record_length=0;
      diag_f3_trace_hdr_type record_hdr;
      char savetime = 0, savevars = 0;
      int byte_sized = 3;
      int bytes_to_copy = 0;
      uint8 qtimer_flag = 0;
      uint32 write_offset = 0;
      uint64 timeticks = 0;

      savetime = diag_f3_trace_detail_mask & DIAG_F3_TRACE_SAVETIME;
      savevars = diag_f3_trace_detail_mask & DIAG_F3_TRACE_SAVEVARS;
      qtimer_flag = DIAG_F3_TRACE_QTIMER_BIT;
      if (savevars)
      {
         /* Calculate bytes needed to store the largest argument */
         /* If the message is Qshrink4 then use the arg_bytes to calculate byte_sized */
         byte_sized = 3;
#ifndef DIAG_USERPD_TEMP
         if (type == DIAG_F3_TRACE_QSHRINK40)
         {
            /* use arg_bytes */
            if(arg_bytes != 0)
            {
               byte_sized = sizeof(uint32) - arg_bytes;
            }
         }
         else
#endif
         {
         for (i = 0; i < nargs; i++)
         {
            if (args[i] > MAX_24BIT_VALUE)
            {
               byte_sized = 0;
               break; /* Can't get larger; bail out of loop now */
            }
            else if ((args[i] > MAX_16BIT_VALUE) && (byte_sized > 1))
            {
               byte_sized = 1;
            }
            else if ((args[i] > MAX_8BIT_VALUE) && (byte_sized > 2))
            {
               byte_sized = 2;
            }
         }
      }
      }
      else
      {
         /* Not saving any variables, so set nargs to 0 */
         nargs = 0;
      }

      /*Calculate the trace record_length needed i.e. Add Size of header,
      size of msg_const_type, size of timestamp type and size of arguments if any */

      /*Add the size of header */
      record_length += sizeof(diag_f3_trace_hdr_type);

      /*Size of the address of the msg_const_type*/
      record_length += sizeof(void*);

      /* Add size to save timestamp to the record */
      if(savetime)
      {
        record_length += sizeof(diag_f3_trace_tstamp_type);
      }

      /* Size of arguments to save */
      if(savevars)
      {
        bytes_to_copy = (sizeof(msg_arg_type)-(uint32)byte_sized);
        record_length += bytes_to_copy * nargs;
      }

      /* Secure an address to save the message to */
      write_offset = diag_f3_trace_secure_offset(record_length);

      /* Save to the circular buffer according to where the head points */
      /* Set up msg header */

      diag_f3_trace_set_up_msg_header(nargs, savevars, savetime, (char)byte_sized, type, &record_hdr);
      /* Save record to the main F3 trace buffer*/
      diag_f3_trace_to_buffer(write_offset, (uint8*) &record_hdr, sizeof(diag_f3_trace_hdr_type));
      /*Increment the offset by size of the header */
      write_offset+=sizeof(diag_f3_trace_hdr_type);

      /* Save address of the msg_const_type */
      /* ptr size is same for standard and Qshrink messages */

      /* Save record to the main F3 trace buffer*/
      diag_f3_trace_to_buffer(write_offset, (uint8*) &constant_data_ptr, sizeof(void*));

      /*Increment the offset by size of the address */
      write_offset+=sizeof(void*);

      /* Save timestamp to the record */
      if (savetime)
      {
         qword tstamp;     /* Time-stamp to be used with this message       */
         diag_f3_trace_tstamp_type shortened_ts;
         
         if (!qtimer_flag)
         {
            if (msg)
            {
               /* Get the timestamp from the created F3 message */
               if (type == DIAG_F3_TRACE_STANDARD)
               {
                  qw_equ(tstamp, ((msg_ext_store_type *)msg)->hdr.ts);
               }
               else
               {
                  qw_equ(tstamp, ((msg_qsr_store_type *)msg)->hdr.ts);
               }
            }
            else
            {
              /* Message was dropped and not sent to diag.  Generate own ts */
              time_get(tstamp);                              //lint !e545
            }

            /* Get rid of the last two bytes of the timestamp */
            shortened_ts.hi = qw_hi(tstamp);
            shortened_ts.lo = qw_lo(tstamp) >> HALF_WORD_BITS;
         }
         else
         {
            /* Get rid of the first two bytes of the timestamp */
            DalTimetick_GetTimetick64(hTimerHandle, &timeticks);
            shortened_ts.hi = (uint32)((timeticks & 0xFFFFFFFFFFFF) >> 16);
            shortened_ts.lo = (uint16)(timeticks & 0xFFFF);
         }

         /* Save record to the main F3 trace buffer*/
         diag_f3_trace_to_buffer(write_offset, (uint8*) &shortened_ts, sizeof(diag_f3_trace_tstamp_type));
         /* Increment the offset by size of the timestamp type */
         write_offset+=sizeof(diag_f3_trace_tstamp_type);
      }

      /* Save arguments to the record */
      if (savevars)
      {
         for (i = 0; i < nargs; i++)
         {
         /* Save record to the main F3 trace buffer*/
            diag_f3_trace_to_buffer(write_offset, (uint8*) &(args[i]), bytes_to_copy);

            write_offset+=bytes_to_copy;
         }
      }
  }
} /* diag_f3_save_msg */

