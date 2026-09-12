#ifndef MICRO_LOG_H
#define MICRO_LOG_H
/*!
@ingroup log_service
@file micro_log.h
@brief
This file describes Logging Service APIs for micro image mode

@details
  The logging service allows clients to send information in the form of a
  log record to the external device that is collecting logs (i.e., QXDM).


@note
  Important usage note:
  The logging service uses a memory management system for logging outbound
  information.  Due to limited resources, this memory management system is a
  circular buffer.
*/

/*
Copyright (c) 2015-16 by QUALCOMM Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/api/services/micro_log.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/02/16   ph      Added micro_log_set_timestamp API.
05/12/15   xy      Created file
===========================================================================*/
#include "comdef.h"          /* Definitions for byte, word, etc. */
#include "log_codes.h"
#include "log.h"                       /*for log_code_type*/

/*!
   @ingroup log_service
   @brief
   Allocate a buffer of size 'length' for logging data. And copy the data from the ptr passed into
   the buffer. Supports version field  This API will be called by clients when they want to generate
   a new log. It will check to see if uImage mode is active or not when it is called.
   If active, the API will copy the log packet into the uImage diag_buffer using the diag_buffer
   interface. A dedicated diag_buffer will be set aside for uImage logging. This buffer will act
   as a circular buffer and will not be drained while in uImage mode. Upon exiting uImage mode,
   Diag task will drain out the uImage diag_buffer as part of it’s regular draining process.

   @param[in] code                 ID of the log to be reported
   @param[in] length               The specified length is the length of the entire log,
                                   including the log header.
   @param[in] ptr                  Pointer to the log packet.
   @param[in] version              Specifies the version - currently, there are two versions
                                   0 - same as log_submit() if not running in uImage mode. In uImage mode,
                                       the buffering/queueing behavior is slightly different
                                   1 - The outgoing log packet header will have support to
                                       include ID field to differentiate the log pkts
                                       coming from different instances of the processor.
                                       same as log_submit_ext() if not running in uImage mode. In uImage mode,
                                       the buffering/queueing behavior is slightly different
   @param[in] *parameter           Based on the version passed in, this pointer is interpreted
                                   as follows-
                                   version       pointer
                                   0             NULL
                                   1             uint32 * . The uint32 value this pointer points to,
                                                 is the ID of the instance generating the
                                                 log packet

   @dependencies
     Log services must be initialized. log_init() should be called to initialize the log services


   @return
   <ul>
   <li>Return TRUE on success.
   <li>Return FLASE if the log code is disabled, .
   </ul>
   @sa

*/
boolean
micro_log_submit_ext (log_code_type code, unsigned int length, void * ptr, unsigned int version, void * parameter);

/*!
   @ingroup log_service
   @brief
   This function sets the length field in the given log record.

   @param[in] *ptr          ptr points to the log whose length field is being set
   @param[in] length       length to be set in the log header


   @dependencies
   ''ptr' must point to the first byte of the log pkt.

   @sideeffects
   Use with caution.  It is possible to corrupt a log record using this
   command.  It is intended for use only with accumulated log records

   @return
   None

   @sa
   None
*/
  void micro_log_set_length (void * ptr, unsigned int length);

/*!
   @ingroup log_service
   @brief
   This function sets the log code

   @param[in] *ptr         ptr points to the log whose logcode field is being set
   @param[in] code        length to be set in the log header


   @dependencies


   @sideeffects

   @return
   None

   @sa
   None
*/
  void micro_log_set_code(void * ptr, log_code_type code);

/*!
   @ingroup log_service
   @brief
      This function sets the timestamp for the given log pointer passed when diag is
      not in island mode. In island mode, this is just a stub and doesn't set
      timestamp. Timestamp update is taken care in micro_log_submit_ext() API and
      client need not set the timestamp field explicitly.

   @param[in] *ptr         ptr points to the log whose timestamp field is being set


   @dependencies


   @sideeffects

   @return
   None

   @sa
   None
*/
  void micro_log_set_timestamp (
    void * ptr
    );

/*!
   @ingroup log_service
   @brief
   This function returns whether a particular code is enabled for logging.

   @param[in] code     Specifies the code.


   @dependencies


   @sideeffects

   @return
   <ul>
   <li> TRUE if log mask is enabled
   <li> FALSE if log mask is disabled
   </ul>
   @sa
   None
*/
  boolean micro_log_status (log_code_type code);

/*!
   @ingroup log_service
   @brief
   This function returns the length field in the given log record.

   @param[in] *ptr         ptr points to the log whose length field is being queried.


   @dependencies


   @sideeffects

   @return
   <ul>
   <li> returns the length field in the given log record.
   </ul>
   @sa
   None
*/
  unsigned int micro_log_get_length(void * ptr);

/*!
   @ingroup log_service
   @brief
   This function returns the log code of the given log record.

   @param[in] *ptr         ptr points to the log whose code field is being queried.


   @dependencies


   @sideeffects

   @return
   <ul>
   <li> returns the log_code
   </ul>

   @sa
   None
*/
  log_code_type micro_log_get_code (void * ptr);

#endif        /* LOG_H */
