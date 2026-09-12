#ifndef GNSS_CS_COMMON_H
#define GNSS_CS_COMMON_H
/*=============================================================================

                   GNSS CS(Crowdsourcing) Common Header file

GENERAL DESCRIPTION
  This file contains the structure definitions and function prototypes 
  relevant for GNSS Crowdsourcing.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

Copyright (c) 2017 Qualcomm Technologies, Inc. 
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

=============================================================================*/

/*==============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

Version Control

  $Header: //components/rel/gnss8.mpss/11.1/gnss/common/inc/gnss_cs_common.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

when        who     what, where, why
--------    ---   ----------------------------------------------------------

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*=============================================================================
       Include Files
=============================================================================*/
#include "gnss_cs_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

=============================================================================*/

/*=============================================================================

       Constants

=============================================================================*/


/*=============================================================================

       Macros

=============================================================================*/

#define GNSS_CS_MAJOR_CLIENT_SW_VERSION   (5)
#define GNSS_CS_MINOR_CLIENT_SW_VERSION   (0)
/* gtpProtocolVersion VersionIdentifier ::= {majorRelease 4, minorRelease 1, pointRelease 0} */
#define GNSS_CS_PROTOCOL_MAJOR_RELEASE    (4)
#define GNSS_CS_PROTOCOL_MINOR_RELEASE    (1)
#define GNSS_CS_PROTOCOL_POINT_RELEASE    (0)

/*=============================================================================

       Typedefs

=============================================================================*/


#ifdef __cplusplus
}
#endif

#endif /* GNSS_CS_COMMON_H */
