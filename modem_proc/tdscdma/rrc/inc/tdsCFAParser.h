#ifndef TDSCFAPARSER_H
#define TDSCFAPARSER_H
/*===========================================================================
                      TDS CFA PARSER

DESCRIPTION

   This module contains external definitions for the interface to CFA logging


Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------

06/30/11   ps      created file

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "customer.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* This enum will inidcate which Module Queue the msg is being put inside */
typedef enum
{
  TDSCFA_UNKNOWN_Q,
  TDSCFA_L2UL_Q,
  TDSCFA_L2DL_Q,  
  TDSCFA_L1_Q,
  TDSCFA_RRC_Q, 
  TDSCFA_MM_Q,
  TDSCFA_RABM_Q,
  TDSCFA_COMP_Q,
  TDSCFA_CM_Q,
  TDSCFA_TC_Q,
  TDSCFA_RR_Q,
  TDSCFA_LTE_Q,
  TDSCFA_QMI_Q,
  TDSCFA_WRRC_Q,
  
  TDSCFA_MAX_Q
}tds_cfa_queue_type;
typedef enum
{ 
  TDSRRC_DSDS_EVENT_NONE,
  TDSRRC_DSDS_EXTERNAL_COMMAND,
  TDSRRC_DSDS_INTERNAL_COMMAND,
  TDSRRC_DSDS_UNLOCK_REQUEST,
  TDSRRC_DSDS_GET_LOCK,
  TDSRRC_DSDS_RELEASE_LOCK,
  TDSRRC_DSDS_WRM_SIG,
  TDSRRC_DSDS_EXCHANGE_LOCK
}tdsrrc_dsds_event_type;
 /*===========================================================================
 
 FUNCTION tds_parse_cfa
 
 DESCRIPTION
 
   Parses  CFA for TDS messages. Message will be parsed regardless.
   q_type indicates the queue in which the message is being put to
   cmd_ptr is the pointer to be logged.
 
 
 RETURN VALUE
 
   None.
 
 SIDE EFFECTS
 
   None.
 
 ===========================================================================*/

 void tds_parse_cfa
(
  tds_cfa_queue_type q_type,
  void   * cmd_ptr

);
/*===========================================================================
 
 FUNCTION tdscfa_dsds_add_event
 
 DESCRIPTION
 
   Parses  CFA for TDS messages and event. 
 
 
 RETURN VALUE
 
   None.
 
 SIDE EFFECTS
 
   None.
 
 ===========================================================================*/
void tdscfa_dsds_add_event(tdsrrc_dsds_event_type dsds_event);
#endif /* TDSCFAPARSER_H */

