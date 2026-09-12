#ifndef WRRC_QSH_INT_H
#define WRRC_QSH_INT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            WRRC_QSH_INT. H                 

GENERAL DESCRIPTION
   Contains declarations for function which are QUALCOMM proprietary 
   and may or may not be shipped as source code.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:$
  $Header: 

when       who     what, where, why
--------   ---     ---------------------------------------------------------
09/22/17   sp      Mini dump phase-2 changes
05/16/16   vs     Added DTF handler to force read all the sibs
05/04/16   nr     Added DTF handlers to buffer and post received OTAs
05/04/16   nr     Added support for WRRC QSH Handler framework
05/04/16   vs     Initial version for wrrc_qsh_int.h

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include <string.h>
#include "trm.h"
#include "qsh.h"
#include "sys.h"
#include "rrcwrm.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION wrrc_collect_mdump
  
DESCRIPTION
  This function collects the mdump for wrrc


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR 
void wrrc_qsh_mdump_collect(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION   RRCMCM_GET_RRC_MODE

DESCRIPTION

  This function retrieves the RRC current mode.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  Returns rrc_mode in rrc_mode_e_type

SIDE EFFECTS

  None.

===========================================================================*/
QSH_MDUMP_FN_ATTR
rrc_mode_e_type rrcmcm_get_rrc_mode(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcdata_globals

DESCRIPTION
This function dumps the Data globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcdata_globals(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcllc_data

DESCRIPTION
This function dumps the LLC globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcllc_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrclcm_data

DESCRIPTION
This function dumps the LCM globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrclcm_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcllcoc_data

DESCRIPTION
This function dumps the LLCOC globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcllcoc_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrccu_data

DESCRIPTION
This function dumps the CU globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrccu_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcrbcommon_data

DESCRIPTION
This function dumps the RBCOMMON globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcrbcommon_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcsmc_data

DESCRIPTION
This function dumps the SMC globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcsmc_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcasn1_data

DESCRIPTION
This function dumps the SMC globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcasn1_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcasu_data

DESCRIPTION
This function dumps the ASU globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcasu_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcrbe_data

DESCRIPTION
This function dumps the RBE globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcrbe_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcrbreconfig_data

DESCRIPTION
This function dumps the RBRECONFIG globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcrbreconfig_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcpcreconfig_data

DESCRIPTION
This function dumps the PCRECONFIG globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcpcreconfig_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrctcreconfig_data

DESCRIPTION
This function dumps the TCRECONFIG globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrctcreconfig_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcrbr_data

DESCRIPTION
This function dumps the RBR globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcrbr_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrctfcc_data

DESCRIPTION
This function dumps the TFCC globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrctfcc_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcumi_data

DESCRIPTION
This function dumps the UMI globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcumi_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcmisc_data

DESCRIPTION
This function dumps the MISC globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcmisc_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcscmgr_data

DESCRIPTION
This function dumps the SCMGR globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcscmgr_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcllcpcie_data

DESCRIPTION
This function dumps the LLCPCIE globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcllcpcie_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcrce_data

DESCRIPTION
This function dumps the RCE globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcrce_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrccho_data

DESCRIPTION
This function dumps the CHO globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrccho_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcdt_data

DESCRIPTION
This function dumps the DT globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcdt_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcrcr_data

DESCRIPTION
This function dumps the RCR globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcrcr_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcccm_data

DESCRIPTION
This function dumps the CCM globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcccm_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcueci_data

DESCRIPTION
This function dumps the UECI globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcueci_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcmeas_data

DESCRIPTION
This function dumps the MEAS globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcmeas_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcdormancy_data

DESCRIPTION
This function dumps the DORMANCY globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcdormancy_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcsibdb_data

DESCRIPTION
This function dumps the SIBDB globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcsibdb_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcueci_data

DESCRIPTION
This function dumps the UECI globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcueci_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcff_data

DESCRIPTION
This function dumps the free floating globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcff_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcmcm_data

DESCRIPTION
This function dumps the MCM globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcmcm_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrciho_data

DESCRIPTION
This function dumps the IHO globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrciho_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcwrm_data

DESCRIPTION
This function dumps the WRM globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcwrm_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrccsp_data

DESCRIPTION
This function dumps the CSP globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrccsp_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcnv_data

DESCRIPTION
This function dumps the NV globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcnv_data(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcdispatcher_data

DESCRIPTION
This function dumps the UECI globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcdispatcher_data(sys_modem_as_id_e_type wrrc_as_id);
#endif /*FEATURE_QSH_MDUMP*/
#endif /*WRRC_QSH_INT_H*/
