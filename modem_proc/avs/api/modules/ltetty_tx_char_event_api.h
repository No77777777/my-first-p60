/*==============================================================================
  @file ltetty_tx_char_event_api.h
  @brief This file contains Event api to send the ltetty Tx characters.
==============================================================================*/

/*===========================================================================
Copyright (c) 2019 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
======================================================================== */

/*========================================================================
 Edit History

 when       who     what, where, why
 --------   ---     -------------------------------------------------------
 09/06/19  harsh     Created
 ========================================================================== */


#ifndef LTETTY_TX_CHAR_EVENT_API_H
#define LTETTY_TX_CHAR_EVENT_API_H

/*==============================================================================
   Event ID
==============================================================================*/

#define EVENT_ID_LTETTY_CHAR 	0x080011A9

/*==============================================================================
   Type definitions
==============================================================================*/

/** @h2xmlp_parameter   {"EVENT_ID_LTETTY_CHAR", EVENT_ID_LTETTY_CHAR}
    @h2xmlp_description {Event for sending the detected TTY character to Voice Service. Voice Service must register this event with LTETTY-Tx Module.}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */


typedef struct event_id_ltetty_char_t event_id_ltetty_char_t;

#include "gk_begin_pack.h"
struct event_id_ltetty_char_t
{
    uint32_t tty_char;
    /**< @h2xmle_description {TTY character. In UTF-8 format.}
         @h2xmle_default     {0}
         @h2xmle_range       {0x0..0xFF}
         @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

#define PARAM_ID_LTETTY_CHAR  0x080011A8
/** @h2xmlp_parameter   {"PARAM_ID_LTETTY_CHAR", PARAM_ID_LTETTY_CHAR}
    @h2xmlp_description {Parameter for sending the TTY character to Module. Voice Service should send this parameter.}
    @h2xmlp_toolPolicy  {Calibration} */


typedef struct param_id_ltetty_char_t param_id_ltetty_char_t;

#include "gk_begin_pack.h"
struct param_id_ltetty_char_t
{
    uint32_t tty_char;
    /**< @h2xmle_description {TTY character. In UTF-8 format.}
         @h2xmle_default     {0}
         @h2xmle_range       {0x0..0xFF}
         @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;


#endif // LTETTY_TX_CHAR_EVENT_API_H
