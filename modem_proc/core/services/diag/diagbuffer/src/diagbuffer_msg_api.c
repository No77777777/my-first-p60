/*==========================================================================

                      Diagnostic Packet Definitions

  Description: diagbuffer message API implementations


Copyright (c) 2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

  $Header: //components/rel/core.mpss/10.0/services/diag/diagbuffer/src/diagbuffer_msg_api.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/17/16   ph      Diag and ULOG integration.
===========================================================================*/

#include "msg_qsr.h"
#include "diagbuffer_int.h"

#define DIAGBUFFER_MAX_VAR_ARGS 9

#ifndef MSG_FMT_STR_ARG_SIZE
#define MSG_FMT_STR_ARG_SIZE 100 /* 100 is guess */
#endif

/**
 * <!-- diagbuffer_debug_vmsg_ext -->
 *
 * @brief Writes a Diag F3 msg packet to the specified diagbuffer handle.
 * Example: msg_v4_diagbuffer_debug_msg(handle, 3, "test %d:
 * %s", 1, "complete");
 *
 * @param handle : Handle to the micro_diagbuffer
 * @param msg_const_ptr: constant pointer to the v4 message.
 * @param num_args : Number of word-sized parameters in the given va_list
 * @param version : Version number to interpret data type stored in 'param'
 * @param parameter : If version == 0, param is ignored.
 *                If version == 1, param is a pointer to a uint32 ID.
 * @param ap : A variable list of arguments to the printf
 *
 * @return DAL_SUCCESS on success, DIAGBUFFER_ERRORS enum for errors.
 */
diagbuffer_result diagbuffer_debug_vmsg_ext( diagbuffer_handle h,
                                             const msg_const_type* const_type,
                                             uint8 num_args,
                                             uint8 version,
                                             void* param,
                                             va_list ap )
{

  uint32 total_size;
  uint32 bufsize_mask;
  uint8 param_size = 0;
  uint32* wordPtr = NULL;
  uint8 *bytePtr;
  diagbuffer_result ret = DAL_ERROR;
  uint32 pad =0;
  uint32 pkt_length =0;
  boolean mask_enabled = FALSE;
  DIAGBUFFER_TYPE *log = (DIAGBUFFER_TYPE *)h;
  uint8 oneMsgBuffer[512];
  char* str = NULL;
  unsigned int fname_length = 0;      /* Stores the file name along with '\0'     */
  unsigned int fmt_length = 0;        /* Stores the fmt length,'\0' and arg size  */

  if ( !log || !(log->logStatus & DIAGBUFFER_STATUS_DEFINED) || num_args > DIAGBUFFER_MAX_VAR_ARGS )
  {
    return DAL_ERROR;
  }

  if (version == 0)
  {
    /* No parameters */
    param_size = 0;
  }
  else if (version == 1)
  {
    if(param == NULL)
    {
      return DAL_ERROR;
    }
    /* Param field is pointer to a uint32 ID */
    param_size = sizeof(uint32);
  }
  else
  {
    /* Unsupported version */
    return DAL_ERROR;
  }

  /*Add mask check logic here of custom mask or none mask */
  mask_enabled = TRUE;

  fname_length = strlen (const_type->fname) + 1;

  /* Copy the length of the fmt string along with MSG_LENGTH_OF_ARGS */
  fmt_length = strlen (const_type->fmt) + 1 + MSG_FMT_STR_ARG_SIZE;

  if ((fname_length + fmt_length) >= sizeof(oneMsgBuffer))
  {
    /* Format string is too long to fit into*/
    return ret;
  }

  str = (char *) oneMsgBuffer;
  (void) vsnprintf (str, fmt_length, const_type->fmt, ap);
  str += strlen((const char *)str) + 1;

  /* Copy the filename and strlcpy adds the ending NULL terminal */
  /*Since the str dest size is computed before, there would not be any buffer overflow */
  (void)strlcpy(str, (const char *)const_type->fname, fname_length );
  str += strlen((const char *)str) + 1;

  pkt_length = (uint32)(str - (char *)oneMsgBuffer);


  if(mask_enabled)
  {

    bufsize_mask = DIAGBUFFER_BUFSIZE_MASK( log );

    /* format + length + TS + msg_const_type* + version + 3 (1 +2 ) bytes padding + param + full format string(pkt_length)  */
    total_size = 2 + 2 + DIAGBUFFER_TIMESTAMP_SIZE + sizeof(uint32) + sizeof(uint8) + sizeof(uint8) + sizeof(uint16) + param_size + pkt_length;

    pad = DIAGBUFFER_PADCOUNT( total_size ) - total_size;

    if (pad >=sizeof(uint32))
    {
        return DAL_ERROR;
    }

    if (diagbufferInternal_UseFastRam(log))
    {
        uint32 start_idx;
        if ( DIAGBUFFER_FAST_TO_RAM_SUCCESS == diagbufferInternal_FastFmtSizeTS(h, total_size, DIAGBUFFER_SUBTYPE_DIAG_MSG_SPRINTF, &start_idx))
        {
          /* Address of const msg_const_type */
          wordPtr = (uint32 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *wordPtr = (uint32)const_type;
          log->writerCore->write +=4;

          /* Version */
          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *bytePtr = (uint8)version;
          log->writerCore->write +=1;

          /*reserved 3 bytes*/
          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *bytePtr = 0;
          log->writerCore->write +=1;

          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *bytePtr = 0;
          log->writerCore->write +=1;

          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *bytePtr = 0;
          log->writerCore->write +=1;

           /* Param */
          if (version == DIAGBUF_VERSION_1)
          {
            wordPtr = (uint32 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *wordPtr = *((uint32 *)param);
            log->writerCore->write += 4;
          }

          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);

          if ((((log->writerCore->write) & (bufsize_mask)) + pkt_length) <= (log->bufSize))
          {
            /*if the packet can fit in the buffer*/
            memscpy( bytePtr, log->bufSize-(log->writerCore->write & bufsize_mask), oneMsgBuffer, pkt_length );
            log->writerCore->write  += pkt_length;
          }
          else
          {
            /*wrap around case*/
            uint32 first_part_length = log->bufSize - (log->writerCore->write & bufsize_mask);
            uint32 second_part_length = pkt_length - first_part_length;
            memscpy( bytePtr, (log->bufSize - (log->writerCore->write & bufsize_mask)), oneMsgBuffer, first_part_length );
            log->writerCore->write  += first_part_length;
            bytePtr =  (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            memscpy( bytePtr, (log->writerCore->readWriter & bufsize_mask), ((byte *)oneMsgBuffer + first_part_length), second_part_length );
            log->writerCore->write  += second_part_length;
          }

          /*add padding at the end of the msg to round it to the next word boundary */
          log->writerCore->write += pad;

          /* Update the message length */
          wordPtr = (uint32*) &(log->buffer[start_idx & bufsize_mask]);
          *wordPtr |= ((total_size << DIAGBUFFER_LENGTH_SHIFT) & DIAGBUFFER_LENGTH_MASK );

          /* Release the lock, acquired within FastFmtSizeTS */
          diagbufferInternal_LockRelease(log);
          ret = DAL_SUCCESS;
        }
        else
        {
            ret =  DAL_ERROR;
        }
    }
    else
    {
        /*TBD: Handle alternate transport such as STM*/
    }
  }

  return ret;
}

/**
 * <!-- diagbuffer_v4_debug_vmsg_ext -->
 *
 * @brief Writes a Diag F3 msg packet to the specified diagbuffer handle.
 * Example: msg_v4_diagbuffer_debug_msg(handle, 3, "test %d:
 * %s", 1, "complete");
 *
 * @param handle : Handle to the micro_diagbuffer
 * @param msg_const_ptr: constant pointer to the v4 message.
 * @param num_args : Number of word-sized parameters in the given va_list
 * @param version : Version number to interpret data type stored in 'param'
 * @param parameter : If version == 0, param is ignored.
 *                If version == 1, param is a pointer to a uint32 ID.
 * @param ap : A variable list of arguments to the printf
 *
 * @return DAL_SUCCESS on success, DIAGBUFFER_ERRORS enum for errors.
 */
diagbuffer_result diagbuffer_v4_debug_vmsg_ext( diagbuffer_handle h,
                                             const msg_v4_const_type* const_type,
                                             uint8 num_args,
                                             uint8 version,
                                             void* param,
                                             va_list ap )
{

  uint32 total_size;
  uint32 bufsize_mask;
  uint8 param_size = 0;
  uint32* wordPtr = NULL;
  uint8 *bytePtr;
  uint8 i;
  diagbuffer_result ret = DAL_ERROR;
  uint32 pad =0;
  boolean mask_enabled = FALSE;
  uint32 qsr_hash = (uint32)const_type;
  DIAGBUFFER_TYPE *log = (DIAGBUFFER_TYPE *)h;
  uint32 arg[num_args];
  uint8 arg_bytes = 0;


  if ( !log || !(log->logStatus & DIAGBUFFER_STATUS_DEFINED) || num_args > DIAGBUFFER_MAX_VAR_ARGS )
  {
    return DAL_ERROR;
  }

  if (version == 0)
  {
    /* No parameters */
    param_size = 0;
  }
  else if (version == 1)
  {
    if(param == NULL)
    {
      return DAL_ERROR;
    }
    /* Param field is pointer to a uint32 ID */
    param_size = sizeof(uint32);
  }
  else
  {
    /* Unsupported version */
    return DAL_ERROR;
  }

  /*Add mask check logic here of custom mask or none mask */
  mask_enabled = TRUE;

  for (i = 0; i < num_args; i++)
  {
    arg[i] = va_arg(ap, uint32);
    arg_bytes =  qsr_v4_check_bytes(arg_bytes, arg[i]);
  }

  if(mask_enabled)
  {

    bufsize_mask = DIAGBUFFER_BUFSIZE_MASK( log );

    /* format + length + TS + msg_const_type*+ num_args + version + 2bytes padding + param + args */
    total_size = 2 + 2 + DIAGBUFFER_TIMESTAMP_SIZE + sizeof(uint32) + sizeof(uint8) + sizeof(uint8) + sizeof(uint16) + (num_args * arg_bytes) + param_size;

    pad = DIAGBUFFER_PADCOUNT( total_size ) - total_size;

    if (pad >=sizeof(uint32))
    {
        return DAL_ERROR;
    }

    if (diagbufferInternal_UseFastRam(log))
    {
        uint32 start_idx;
        if ( DIAGBUFFER_FAST_TO_RAM_SUCCESS == diagbufferInternal_FastFmtSizeTS(h, total_size, DIAGBUFFER_SUBTYPE_DIAG_MSG_QSR4, &start_idx))
        {
            /* Address of const msg_v4_const_type */
            wordPtr = (uint32 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *wordPtr = qsr_hash;
            log->writerCore->write+= 4;

             /* Num args */
            bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *bytePtr = (uint8)((arg_bytes<<4)|num_args);
            log->writerCore->write +=1;

            /* Version */
            bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *bytePtr = (uint8)version;
            log->writerCore->write +=1;

            /*reserved 2 bytes*/
            bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *bytePtr = 0;
            log->writerCore->write +=1;

            bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *bytePtr = 0;
            log->writerCore->write +=1;

             /* Param */
            if (version == DIAGBUF_VERSION_1)
            {
              wordPtr = (uint32 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
              *wordPtr = *((uint32 *)param);
              log->writerCore->write += 4;
            }

            for (i = 0; i < num_args; i++)
            {
              bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
              qsr_v4_copy_args(bytePtr,(uint8 *)&arg[i],arg_bytes);
              log->writerCore->write += arg_bytes;
            }

            /*add padding at the end of the msg to round it to the next word boundary */
            log->writerCore->write += pad;

            /* Update the message length */
            wordPtr = (uint32*) &(log->buffer[start_idx & bufsize_mask]);
            *wordPtr |= ((total_size << DIAGBUFFER_LENGTH_SHIFT) & DIAGBUFFER_LENGTH_MASK );

            /* Release the lock, acquired within FastFmtSizeTS */
            diagbufferInternal_LockRelease(log);
            ret = DAL_SUCCESS;
        }
        else
        {
            ret =  DAL_ERROR;
        }
    }
    else
    {
        /*TBD: Handle alternate transport such as STM*/
    }
  }

  return ret;
}
/**
 * <!-- msg_v4_diagbuffer_debug_msg -->
 *
 * @brief Writes a Diag F3 msg packet to the specified diagbuffer handle.
 * Example: msg_v4_diagbuffer_debug_msg(handle, 3, "test %d:
 * %s", 1, "complete");
 *
 * @param handle : Handle to the micro_diagbuffer
 * @param msg_const_ptr: constant pointer to the v4 message.
 * @param num_args : Number of word-sized parameters in the given va_list
 * @param version : Version number to interpret data type stored in 'param'
 * @param parameter : If version == 0, param is ignored.
 *                If version == 1, param is a pointer to a uint32 ID.
 *
 * @return DAL_SUCCESS on success, DIAGBUFFER_ERRORS enum for errors.
 */

diagbuffer_result msg_v4_diagbuffer_debug_msg(diagbuffer_handle handle,
                                const msg_v4_const_type * msg_const_ptr,
                                uint8 num_args, uint32 version, void* parameter, ...)
{
  va_list ap;
  diagbuffer_result ret;

  va_start( ap, parameter );
  ret = diagbuffer_v4_debug_vmsg_ext( handle, msg_const_ptr, num_args, version, parameter, ap );
  va_end( ap );

  return ret;
}

/*!
@ingroup diag_message_service
   @brief
   This will build a new style optimized diagnostic message with 1 parameter.

   @warning
   Do not call directly; use macro DIAGBUFFER_MSG_EXT()

   @param[in] *xx_msg_v4_const  static constant constructed using
   the different XX_MSG_V4_CONST macros
   @param[in] xx_handle                Specifies the diagbuffer handle.
   @param[in] version                  Specifies the version - currently, there are two versions
                                       0 - same as msg_v3_send() (for backwards compatibility)
                                       1 - The outgoing msg header will have the support to
                                           include ID field to differentiate messages
                                           coming from different instances of the processor.

   @param[in] *parameter               Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                       0             NULL
                                       1             uint32 * . The uint32 value this pointer points to,
                                                     is the ID of the instance generating the msg.

   @dependencies
   msg_init() must have been called previously.  A free buffer must
   be available or the message will be ignored (never buffered).

   @sideeffects
   None

   @return
   None

   @sa
   msg_v4_send_ext, msg_v4_send_2_ext, msg_v4_send_3_ext, msg_v4_send_var_ext
*/
void _msg_v4_diagbuffer_send_ext (diagbuffer_handle handle,
                                   const msg_v4_const_type * xx_msg_const_ptr,
                                   uint32 version, void* parameter)
{
    /*If handle is SYSDIAG_HANDLE, fallback to DIAG API*/
    if(handle == SYSDIAG_HANDLE)
    {
        msg_v4_send_ext (xx_msg_const_ptr, version, parameter);
    }
    else
    {
        msg_v4_diagbuffer_debug_msg(handle, xx_msg_const_ptr, 0, version, parameter);
    }
}

/*!
@ingroup diag_message_service
   @brief
   This will build a new style optimized diagnostic message with 1 parameter.

   @warning
   Do not call directly; use macro DIAGBUFFER_MSG_1_EXT()

   @param[in] *xx_msg_v4_const  static constant constructed using
   the different XX_MSG_V4_CONST macros
   @param[in] xx_handle                Specifies the diagbuffer handle.
   @param[in] version                  Specifies the version - currently, there are two versions
                                       0 - same as msg_v3_send_1() (for backwards compatibility)
                                       1 - The outgoing msg header will have the support to
                                           include ID field to differentiate messages
                                           coming from different instances of the processor.

   @param[in] *parameter               Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                       0             NULL
                                       1             uint32 * . The uint32 value this pointer points to,
                                                     is the ID of the instance generating the msg.

   @param[in]  xx_arg1           integer parameter 1 for the optimized message

   @dependencies
   msg_init() must have been called previously.  A free buffer must
   be available or the message will be ignored (never buffered).

   @sideeffects
   None

   @return
   None

   @sa
   msg_v4_send_ext, msg_v4_send_2_ext, msg_v4_send_3_ext, msg_v4_send_var_ext
*/
void _msg_v4_diagbuffer_send_1_ext (diagbuffer_handle handle,
                                   const msg_v4_const_type * xx_msg_const_ptr,
                                   uint32 version, void* parameter, uint32 xx_arg1)
{
    /*If handle is SYSDIAG_HANDLE, fallback to DIAG API*/
    if(handle == SYSDIAG_HANDLE)
    {
        msg_v4_send_1_ext (xx_msg_const_ptr, version, parameter, (uint32) (xx_arg1));
    }
    else
    {
        msg_v4_diagbuffer_debug_msg(handle, xx_msg_const_ptr, 1, version, parameter, xx_arg1);
    }
}
  /*!
@ingroup diag_message_service
   @brief
   This will build a new style optimized diagnostic message with 2 parameters.

   @warning
   Do not call directly; use macro DIAGBUFFER_MSG_2_EXT()

   @param[in] *xx_msg_v4_const  static constant constructed using
   the different XX_MSG_V4_CONST macros
   @param[in] xx_handle                Specifies the diagbuffer handle.
   @param[in] version                  Specifies the version - currently, there are two versions
                                       0 - same as msg_v4_send_2() (for backwards compatibility)
                                       1 - The outgoing msg header will have the support to
                                           include ID field to differentiate messages
                                           coming from different instances of the processor.

   @param[in] *parameter                Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                       0             NULL
                                       1             uint32 * . The uint32 value this pointer points to,
                                                     is the ID of the instance generating the msg.

   @param[in]  xx_arg1          integer parameter 1 for the optimized message
   @param[in]  xx_arg2          integer parameter 2 for the optimized message

   @dependencies
   msg_init() must have been called previously.  A free buffer must
   be available or the message will be ignored (never buffered).

   @sideeffects
   None

   @return
   None

   @sa
   msg_v4_send_ext, msg_v4_send_1_ext, msg_v4_send_3_ext, msg_v4_send_var_ext
*/
void _msg_v4_diagbuffer_send_2_ext (diagbuffer_handle handle,
                                   const msg_v4_const_type * xx_msg_const_ptr,
                                   uint32 version, void* parameter, uint32 xx_arg1, uint32 xx_arg2)
{
    /*If handle is SYSDIAG_HANDLE, fallback to DIAG API*/
    if(handle == SYSDIAG_HANDLE)
    {
        msg_v4_send_2_ext (xx_msg_const_ptr, version, parameter, (uint32) (xx_arg1), (uint32) xx_arg2);
    }
    else
    {
        msg_v4_diagbuffer_debug_msg(handle, xx_msg_const_ptr, 2, version, parameter, xx_arg1, xx_arg2);
    }
}
/*!
@ingroup diag_message_service
   @brief
   This will build a new style optimized diagnostic message with 3 parameters.

   @warning
   Do not call directly; use macro DIAGBUFFER_MSG_3_EXT()

   @param[in] *xx_msg_v4_const  static constant constructed using
   the different XX_MSG_V4_CONST macros
   @param[in] xx_handle                Specifies the diagbuffer handle.
   @param[in] version                  Specifies the version - currently, there are two versions
                                       0 - same as msg_v4_send_3() (for backwards compatibility)
                                       1 - The outgoing msg header will have the support to
                                           include ID field to differentiate messages
                                           coming from different instances of the processor.

   @param[in] *parameter                Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                       0             NULL
                                       1             uint32 * . The uint32 value this pointer points to,
                                                     is the ID of the instance generating the msg.

   @param[in]  xx_arg1                integer parameter 1 for the optimized message
   @param[in]  xx_arg2                integer parameter 2 for the optimized message
   @param[in]  xx_arg3                integer parameter 3 for the optimized message

   @dependencies
   msg_init() must have been called previously.  A free buffer must
   be available or the message will be ignored (never buffered).

   @sideeffects
   None

   @return
   None

   @sa
   msg_v4_send, msg_v4_send_1, msg_v4_send_2, msg_v4_send_var
*/
void _msg_v4_diagbuffer_send_3_ext (diagbuffer_handle handle,
                                   const msg_v4_const_type * xx_msg_const_ptr,
                                   uint32 version, void* parameter, uint32 xx_arg1, uint32 xx_arg2, uint32 xx_arg3)
{
    /*If handle is SYSDIAG_HANDLE, fallback to DIAG API*/
    if(handle == SYSDIAG_HANDLE)
    {
        msg_v4_send_3_ext (xx_msg_const_ptr, version, parameter, (uint32) (xx_arg1), (uint32) xx_arg2, (uint32)xx_arg3);
    }
    else
    {
        msg_v4_diagbuffer_debug_msg(handle, xx_msg_const_ptr, 3, version, parameter, xx_arg1, xx_arg2, xx_arg3);
    }
}
/*!
@ingroup diag_message_service
   @brief
   This will build a new style optimized diagnostic message with variable number
   of (4 to 9) parameters.

   @warning
   Do not call directly; use macros DIAGBUFFER_MSG_N_EXT()

   @param[in] *xx_msg_v4_const            static constant constructed using
   the different XX_MSG_V4_CONST macros
   @param[in] xx_handle                Specifies the diagbuffer handle.
   @param[in] version                  Specifies the version - currently, there are two versions
                                       0 - same as msg_v4_send_var() (for backwards compatibility)
                                       1 - The outgoing msg header will have the support to
                                           include ID field to differentiate messages
                                           coming from different instances of the processor.

   @param[in] *parameter                Based on the version passed in, this pointer is interpreted
                                       as follows-
                                       version       pointer
                                       0             NULL
                                       1             uint32 * . The uint32 value this pointer points to,
                                                     is the ID of the instance generating the msg.

   @param[in] num_args                number of parameters

   @dependencies
   msg_init() must have been called previously.  A free buffer must
   be available or the message will be ignored (never buffered).

   @sideeffects
   None

   @return
   None

   @sa
   msg_v4_send, msg_v4_send_1, msg_v4_send_2, msg_v4_send_3
*/
void _msg_v4_diagbuffer_send_var_ext (diagbuffer_handle handle,
                                   const msg_v4_const_type * xx_msg_const_ptr,
                                   uint32 version, void* parameter,uint32 num_args, ...)
{
    va_list ap;
    va_list arg_list;
    diagbuffer_result ret;
    uint32 i =0;
    uint32 arg[num_args];
    uint8 args_count = MIN(num_args,DIAGBUFFER_MAX_VAR_ARGS);

    va_start( arg_list, num_args );

    for (i = 0; i < args_count; i++)
    {
        arg[i] = va_arg(arg_list, uint32);
    }
    va_end(arg_list);


    /*If handle is SYSDIAG_HANDLE, fallback to DIAG API*/
    if( handle == SYSDIAG_HANDLE)
    {
      switch ( args_count )
      {
          case 4:
            msg_v4_send_var_ext (xx_msg_const_ptr, version, parameter, args_count, arg[0], arg[1], arg[2], arg[3] );
            break;
          case 5:
            msg_v4_send_var_ext (xx_msg_const_ptr, version, parameter, args_count, arg[0], arg[1], arg[2], arg[3], arg[4] );
            break;
          case 6:
            msg_v4_send_var_ext (xx_msg_const_ptr, version, parameter, args_count, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5] );
            break;
          case 7:
            msg_v4_send_var_ext (xx_msg_const_ptr, version, parameter, args_count, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5],arg[6] );
            break;
          case 8:
            msg_v4_send_var_ext (xx_msg_const_ptr, version, parameter, args_count, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7] );
            break;
          case 9:
            msg_v4_send_var_ext (xx_msg_const_ptr, version, parameter, args_count, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8] );
            break;
          default:
            break;
      }
    }
    else
    {
        va_start( ap, num_args );
        ret = diagbuffer_v4_debug_vmsg_ext( handle, xx_msg_const_ptr, num_args, version, parameter, ap );
        va_end( ap );
    }
}

void _msg_diagbuffer_sprintf_ext(diagbuffer_handle handle, uint32 version, void *parameter, uint32 num_args, const msg_const_type *const_blk, ...)
{
  va_list ap;
  va_list arg_list;
  diagbuffer_result ret;
  uint32 i =0;
  uint32 arg[num_args];
  uint8 args_count = MIN(num_args,DIAGBUFFER_MAX_VAR_ARGS);

  va_start( arg_list, const_blk );
  for(i = 0; i < args_count; i++)
  {
    arg[i] = va_arg(arg_list, uint32);
  }
  va_end(arg_list);

  /*If handle is SYSDIAG_HANDLE, fallback to DIAG API*/
  if( handle == SYSDIAG_HANDLE)
  {
    switch ( args_count )
    {
      case 1:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0] );
        break;
      case 2:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1] );
        break;
      case 3:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1], arg[2] );
        break;
      case 4:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1], arg[2], arg[3] );
        break;
      case 5:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1], arg[2], arg[3], arg[4] );
        break;
      case 6:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5] );
        break;
      case 7:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6] );
      case 8:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7] );
        break;
      case 9:
        msg_sprintf_ext (version, parameter, args_count, const_blk, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8] );
      default:
        break;
    }
  }
  else
  {
    va_start( ap, const_blk );
    /*write the msg into diag_buffer */
    ret = diagbuffer_debug_vmsg_ext( handle, const_blk, num_args, version, parameter, ap );
    va_end( ap );
  }
}
