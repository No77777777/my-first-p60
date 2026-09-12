#ifndef MCCSO_H
#define MCCSO_H

/*===========================================================================

         S E R V I C E   O P T I O N S   D E F I N I T I O N S

GENERAL DESCRIPTION
  This module contains definitions of service options other than data.

  Copyright (c) 2000 - 2017 Qualcomm Technologies, Inc. 
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

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/mccso.h_v   1.0.2.0   30 Nov 2001 17:30:26   fchan  $
$Header: //components/rel/1x.mpss/8.0/cp/src/mccso.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/31/17   ppr     Sub specific 1x EFS Item support chgs
09/29/15   agh     Remove all references to MVS and cleanup WON feature
03/19/15   agh     Changes to support Voice Adapter based Audio architecture
03/01/11   ag      Fix to release MVS before MUX is commanded to release SO.
01/04/11   ag      Mainlined FEATURE_MVS_MIGRATE.
09/14/10   ag      Featurized voice support under FEATURE_MODEM_1X_VOICE_SUPPORT.
05/15/08   bb      Code changes for "MVS API Update" feature
11/02/00   lcc     Initial release for FEATURE_COMPLETE_SNM.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "modem_1x_defs.h"

#include "1x_variation.h"

/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
/*===========================================================================

FUNCTION MCC_SET_AVG_ENCODER_RATE

DESCRIPTION
  This function provides the average encoder to MVS or the CDMA Voice Adapter.
  Depending on the audio architecture being used, this function will invoke MVS
  API or provide required EVENT to CDMA Voice Adapter.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mccso_set_avg_encoder_rate ( byte rate ) ;

#endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

/*===========================================================================

FUNCTION MCCSO_RELEASE_MVS

DESCRIPTION
  This function releases MVS if it was acquired for CDMA voice services.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mccso_release_mvs ( void );

/*===========================================================================

FUNCTION mccso_initialize

DESCRIPTION
  This function initializes all the service options defined in mccso.c.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccso_initialize( void );

#ifdef FEATURE_1X_CUST_VOICE_SO
/*===========================================================================

FUNCTION MCCSO_UPDATE_CUST_VOICE_SO 

DESCRIPTION
  This API will add customer voice SO to SO3 altenate SO list if required.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccso_update_cust_voice_so( void );
#endif /* FEATURE_1X_CUST_VOICE_SO */

#endif /* MCCSO_H */