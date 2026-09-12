#ifndef SRCH_HDR_H
#define SRCH_HDR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             S R C H  _ H D R ---  H E A D E R   F I L E 

GENERAL DESCRIPTION
  This module contains header information pertaining to srch_hdr.  srch_hdr 
  contains functions which allow the HDR protocol to interface with the SRCH
  task.

EXTERNALIZED FUNCTIONS
  srch_hdr_in_service
  srch_hdr_get_channel_estimate
  
  
INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2005 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/api/srch_hdr.h#1 $

when       who     what, where, why 
--------   ---     ----------------------------------------------------------
09/30/13   as      Added srch_hdr_get_1x_warmup_ms
09/19/12   vks     Div interlock support no longer required.
12/15/11   vks     Add support to recheck div interlock when HDR/1x retunes
                   to different band.
12/06/11   vks     Avoid 1x and HDR RF tune collision by returning how much
                   time HDR RF tune should back off.
09/24/10   vks     Add support for 'not raising' the srch task priority to
                   srch_max_pri if HDR is in traffic.
04/01/09   adw     Categorized included header files and standardized format.
03/25/09   adw     Removed unnecessary customer.h include.
09/08/05   ejv     Implementation, first cut.
===========================================================================*/ 


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"


/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION       SRCH_HDR_IN_SERVICE
DESCRIPTION    This function returns true if srch is on the system.
               It returns false if search is either inactive or trying
               to get on the system. 

DEPENDENCIES   None

RETURN VALUE   TRUE - srch in on the system; otherwise, FALSE 
SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_hdr_in_service (void);

/*===========================================================================

FUNCTION       SRCH_HDR_IS_IN_TRAFFIC
DESCRIPTION    This function returns true if HDR is in traffic. 

DEPENDENCIES   None

RETURN VALUE   TRUE - HDR in traffic; otherwise, FALSE 
SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_hdr_is_in_traffic (void);

/* Define the following feature so that HDR uses this to make use of the
   following api. This is so that, HDR can check in their stuff without
   worry about dependencies on 1x */
#define FEATURE_1X_TL_BACKOFF_FOR_HDR_TRAFFIC
/*===========================================================================

FUNCTION       SRCH_HDR_SET_HDR_IN_TRAFFIC
DESCRIPTION    This function informs the srch_hdr module that HDR is in/out 
               of traffic. 

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_hdr_set_hdr_in_traffic
(
  boolean hdr_in_traffic
);

/*===========================================================================

FUNCTION       SRCH_HDR_GET_CHANNEL_ESTIMATE
DESCRIPTION    This function returns an estimate of the channel in 1/8 db units.
               This function is sleep safe and can be called in another task
               context 

DEPENDENCIES   None

RETURN VALUE   Channel estimate in 1/8 db units 
SIDE EFFECTS   None

===========================================================================*/
extern uint32 srch_hdr_get_channel_estimate (void);

/*==========================================================================

FUNCTION       SRCH_HDR_TIME_UNTIL_TUNE_COMPLETE
DESCRIPTION    If the 1X tune is happening within HDR tune duration, return
               the time that tune will be completed. Otherwise, return 0ms.

DEPENDENCIES   None

RETURN VALUE   0  : There is no conflict between 1x and HDR RF tune.
               >0 : There is conflict and the time in usec until the 1X tune
                    completes
SIDE EFFECTS   None

==========================================================================*/
extern uint32 srch_hdr_time_until_tune_complete
(
  uint32 tune_duration_hdr_us                 /* HDR tune duration in usec*/
);

/*==========================================================================

FUNCTION       SRCH_HDR_GET_1X_WARMUP_MS
DESCRIPTION    Returns the 1x warmup time. 
               If 1x is awake the return value is 0.

DEPENDENCIES   None

RETURN VALUE   Time in milliseconds 
 
SIDE EFFECTS   None

==========================================================================*/
extern uint32 srch_hdr_get_1x_warmup_ms( void );

#endif /* SRCH_HDR_H */
