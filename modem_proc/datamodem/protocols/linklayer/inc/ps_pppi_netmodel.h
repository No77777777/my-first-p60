#ifndef PS_PPPI_NETMODEL_H
#define PS_PPPI_NETMODEL_H
/*===========================================================================

                      P S _ P P P I _ N E T M O D E L . H

DESCRIPTION

 The PPP Network model internal header file.

Copyright (c) 2009 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ps_ppp_netmodel.h_v   1.3   15 Nov 2002 23:37:30   jeffd  $
  $Header: //components/rel/data.mpss/3.5..2.1/protocols/linklayer/inc/ps_pppi_netmodel.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/09/09   mga     Merged Compiler warnings fix
05/07/09    pp     Initial release. Created as part of CMI Phase-4: SU Level 
                   API Effort. [Split from ps_ppp_netmodel.h]
===========================================================================*/


/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "ps_ppp_defs.h"
#include "dsm.h"
#include "ps_ppp_snoop.h"


/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
#ifdef FEATURE_DS_MOBILE_IP
/*===========================================================================
FUNCTION NETMDL_RESET_PPP()

DESCRIPTION
  This function will spoof Term Requests over both the Um adn Rm interfaces.

PARAMETERS
  kill_ppp: if this is TRUE tear down the call, otherwise spoof a C-Req to
  reset the connection.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void netmdl_reset_ppp
(
  boolean kill_ppp
);

/*===========================================================================
  FUNCTION NETMDL_COMPARE_LCP_OPTS()

  DESCRIPTION
    Compares the LCP options on the Rm and Um.

  PARAMETERS
    None

  RETURN VALUE
    TRUE - Rm and Um LCP options match
    FALSE - Rm and Um LCP options differ

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean netmdl_compare_lcp_opts
(
  void
);

#endif /* FEATURE_DS_MOBILE_IP */

/*===========================================================================
  FUNCTION NETMDL_COMPARE_ICPC_OPTS()

  DESCRIPTION
    Compares the ICPC options on the Rm and Um.

  PARAMETERS
    TRUE - Rm and Um ICPC options match
    FALSE - Rm and Um ICPC options differ

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean netmdl_compare_ipcp_opts
(
  void
);


#endif /* PS_PPPI_NETMODEL_H */
