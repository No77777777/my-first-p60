#ifndef SD_MSGR_H
#define SD_MSGR_H

/**===========================================================================

      S Y S T E M   D E T E R M I N A T I O N    M E S S A G E   R O U T E R

                             H E A D E R   F I L E

DESCRIPTION
  This header contains definitions for SD to notify and recieve notifications
  to other modem modules. This also has information for other modem modules
  to register with message router for the message sent out from SD.
  
Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/* <EJECT> */
/**===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //source/qcom/qct/modem/mmode/api/main/latest/sd_msgr.h

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/20/16   aj      SD message router header added.

===========================================================================*/

#include <msgr.h>
#include <msgr_umid.h>
#include "sd.h"

/*==============================================================================
                           Module definitions
==============================================================================*/


/* SD MSGR definitions */
/* NOTE: SD modules should take 0x31 to 0x50 */

#define MSGR_MODULE_SD          0x31
#define MSGR_MM_SD              MSGR_TECH_MODULE( MSGR_TECH_MM, MSGR_MODULE_SD )



/* SD MSGR definitions */

/* Payload for sending SD LTE addition or removal indication
** through msgr interface 
*/

typedef struct sd_lte_mode_change_ind_s {

  msgr_hdr_struct_type         hdr;
    /* Customary for all payloads to MSGR */

  sd_lte_mode_change_ind_e_type    mode_change;

} sd_lte_mode_change_ind_s_type;


/* In order to define MSGR_ID Which SD sends to other modules
*/
enum
{
  MM_SD_IND_FIRST = MSGR_UMID_BASE(MSGR_MM_SD, MSGR_TYPE_IND),
    
  MSGR_DEFINE_UMID(MM, SD, IND, LTE_MODE_CHANGE, 0x01, sd_lte_mode_change_ind_s_type),
    /* MM_SD_LTE_MODE_CHANGE_IND */

  MM_SD_IND_MAX
};


#endif /* SD_MSGR_H */

