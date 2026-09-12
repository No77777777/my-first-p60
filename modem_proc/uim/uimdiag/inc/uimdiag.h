#ifndef UIMDIAG_H
#define UIMDIAG_H

/*===========================================================================

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2009 - 2015 by QUALCOMM Technologies, Inc (QTI).
All Rights Reserved. QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/uimdiag/inc/uimdiag.h#2 $$ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who   what, where, why
--------   ---   -----------------------------------------------------------
03/11/16   ar    Remove UIMDIAG code using Stubs
11/04/15   stv   Correcting the UIM DIAG command IDs
11/02/15   stv   Rel 12 refresh enforcement policy support
08/28/15   hh    Removed JCDMA related code
07/10/15   stv   Deprecate UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_CMD
05/15/15   yt    Support for mapping sessions to slots
04/25/15   av    Support for hiding CSIM from modem clients
02/27/15   kk    Fix uimdiag cmd enum table
02/25/15   yt    Reduction of UIMDIAG functionality
02/20/15   vv    Remove perso related code
12/26/14   vv    Support for whitelist and blacklist codes in same category
07/10/14   tl    Re-add CREATE_PKCS15_LOOKUP_TABLE_CMD
07/10/14   tl    Remove ALWAYS OFF feature FEATURE_MMGSDI_PKCS15
06/12/14   vv    Remove FEATURE_SIMLOCK
06/10/14   tl    Introduce EF 3GPDUPPExt
03/18/14   tl    Introduce new SIM Lock feature
06/27/13   spo   Added support for files under DF TELECOM
05/24/13   bcho  Support for perso get feature data Cnf
05/06/13   sw    Changes made for secure version of memcpy
09/25/12   tl    Adding nvruim_data_req_enum_type to sessionid_type
07/13/12   vv    Renamed CSGI to CSGL
12/23/11   bcho  files added in uimdiag_file_enum_type enum
10/18/11   bcho  Adding support for mmgsdi_session_open_ext API
10/14/11   bcho  Added support for Get Operator Name API
09/23/11   nb    Support for SAP CONNECT/SAP DISCONNECT
08/05/11   adp   Adding support to trigger refresh in uimdiag
05/27/11   nb    Support for new refresh related APIs
08/22/11   av    Added support for pdown,pup,activate onchip,send apdu commands
04/26/11   yt    Replaced references to TMC heap with Modem heap
12/29/10   ms    Adding support for BT SAP APIs
12/03/10   adp   Adding support for a number of commands
09/07/10   adp   Adding Support for dck_num_retries,lock_down_enable_ltd_access,
                 seek and search commands
08/09/10   adp   Adding support for service enable, disable, get perso key,
                 is service available, perso unblock.
07/02/10   yb    Removing dependency on rex from apps side
04/15/10   adp   Added support for PACKED structs for Q6. PACKED_POST keyword
                 is added for each packed union/structure.
03/25/10   adp   Adding support for deactivate_provisioning,
                 get_app_capabilities,session_get_cphs_info,
                 isim_authenticate,mmgsdi_session_send_card_status,
                 create_pkcreate_lookup_table
03/19/10   vs    Fixed compile error
03/19/10   rk    Added support for JCDMA get card info
02/22/10   adp   Adding support for Perso Commands
02/09/10   yt    Fixed compilation error
12/27/09   ap    Adding support for Refresh Commands
12/03/09   rk    Adding support for get file attributes, read and write commands
10/19/09   adp   Adding support for pin commands
10/18/09   adp   Moving session_open, session_close, act_or_switch, get_info
                 structures to uimdiag_mmgsdi.h
09/02/09   adp   Initial Revision -Added initial mmgsdi session based commands

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "modem_mem.h"
#include "uim.h"
#include <stringl/stringl.h>


/*=============================================================================

                       FUNCTIONS DECLARATIONS

=============================================================================*/

/*===========================================================================
FUNCTION UIMDIAG_INIT

DESCRIPTION
    Registers sub command codes for GSDI Diag sub system.

ARGUMENTS
    None

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None
===========================================================================*/
void uimdiag_init(void);

#endif /* UIMDIAG_H */
