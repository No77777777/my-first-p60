#ifndef DS3GMMGSDIIF_EX_H
#define DS3GMMGSDIIF_EX_H
/*===========================================================================

               3 G   D A T A   S E R V I C E S   M A N A G E R

           M M G S D I   I N T E R F A C E   F U N C T I O N S

                            H E A D E R    F I L E

DESCRIPTION
  This file contains functions for interfacing to MMGSDI, including
  Data Services client initialization and processing of MMGSDI events. This
  header file is intended to be used internal to the 3G Dsmgr module only.


  Copyright (c) 2018-2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3gmmgsdiif_ex.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/24/13   sd      TSTS changes for DS3G.
01/02/12   msh     Coan: Feature cleanup
09/09/11   vpk     Fix for empty username during AN authentication
09/05/11   ss      Changes to support RPM.
08/20/11   ttv     GSDI to MMGSDI_SESSION api migration changes.
04/11/11   ack     Removing FEATURE_UIM_SUPPORT_HOTSWAP wrap
04/11/11   ttv     Added support for Hot Swap feature.
01/31/11   op      Cleaning up usage of UIM API
01/04/11   op      Added support for ACL enable/disable flag
11/22/10   ttv     Added support for DSDS.
01/16/10   ls      All mmgsdi API shall be called under DS context
11/25/09   ls      Add support for retrieving slot info
11/09/09   ls      Add support for retrieving IMSI info from USIM card
10/21/09   ls      Support 1x pri/sec and GW pri/sec
07/17/09   ls      Removed CDMA service table info since UIM is taking care
                   of reading CDMA service table and update table. Change
                   FEATURE_CSIM to FEATURE_MMGSDI_SESSION_LIB
04/27/09   ls      Centralized MMGSDI interface
11/26/08   sn      Removed header files that are not used.
10/23/08   yz      Fixed featurization issues
10/10/08   yz      Initial version of the file.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "dstask_v.h"
#include "dstaski.h"
#include "dsati.h"

#include "ds_3gpp_config_mgr.h"

#ifdef FEATURE_MMGSDI
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION DS3G_MMGSDI_GET_INDEX_FROM_SESSION_TYPE

DESCRIPTION
  This function returns the array index based on session_type.

PARAMETERS
  session_type : mmgsdi session type.
  *index       : array index to return

RETURN VALUE
  TRUE if successful/valid
  FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_mmgsdi_get_arr_index_from_session_type
(
  mmgsdi_session_type_enum_type  session_type,
  uint8                         *index_ptr
);

/*===========================================================================
FUNCTION DS3G_MMGSDIF_READ_PSDATAOFF
 
DESCRIPTION
  This function reads the PS Data Off File from USIM 
 
PARAMETERS 
  Session Type
 
DEPENDENCIES 
  None. 
  
RETURN VALUE 
  TRUE if PS Data Off Exempt Services list is read successfully
  FALSE otherwise.
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
boolean ds3g_mmgsdi_read_efpsdataoff
(
  mmgsdi_session_type_enum_type  session_type
);

/*===========================================================================
FUNCTION DS3G_MMGSDI_EFPSDATAOFF_UPDATE_CACHE

DESCRIPTION
  This function updates the PS Data Off Exempt services USIM cache with the
  values read from SIM for the corresponding session type.
 
PARAMETERS
  const ds_cmd_type   *cmd_ptr

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds3g_mmgsdi_efpsdataoff_update_cache
(
  const ds_cmd_type   *cmd_ptr
);

/*===========================================================================
FUNCTION DS3G_MMGSDI_RESET_EFPSDATAOFF

DESCRIPTION
  This function is called when DS3G_MMGSDI_CARD_ERROR_EVENT is posted.
  It resets the EF PS Data Off values to zero (all non exempt)

PARAMETERS
  Subscription ID

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds3g_mmgsdi_reset_efpsdataoff
(
  sys_modem_as_id_e_type     subs_id
);

/*===========================================================================
FUNCTION DS3G_MMGSDI_GET_EFPSDATAOFF_EXEMPT_SERVICES

DESCRIPTION
  Returns the EF PS DATA OFF exempt services list as provisioned in USIM

PARAMETERS
  [in] Subs ID: Subscription ID to retrieve the USIM PS Data off
                Exempt services list from.
  [out] psdataoff_usim_info_ptr: Pointer to the cache storing the PS data off
                Exempt services list as provisioned in the USIM.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if successfully read
  FALSE otherwise

SIDE EFFECTS
  None.
============================================================================*/
boolean ds3g_mmgsdi_get_efpsdataoff_exempt_services
(
  sys_modem_as_id_e_type                        subs_id,
  const ds_3gpp_config_mgr_psdataoff_cfg_type **psdataoff_usim_info_ptr
);

#endif /* FEATURE_MMGSDI */
#endif /* DS3GMMGSDIIF_EX_H */

