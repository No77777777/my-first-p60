#ifndef QMI_UIM_H
#define QMI_UIM_H
/*===========================================================================

                         Q M I _ U I M . H

DESCRIPTION

 The Data Services QMI User Identity Module Service header file.

                        COPYRIGHT INFORMATION

Copyright (c) 2010, 2012 - 2016, 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/inc/qmi_uim.h#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
08/17/18    rps    Add support for mini dump phase 2
05/26/16    vdc    Move internal enums, structs to new internal header file
04/15/16    vdc    Split QMI UIM files for supporting IOE
11/26/15    sp     Updated copyright
08/10/15    av     Introduce MMGSDI event for indicating recovery completion
04/30/15    hh     Include needed header file
04/29/15    av     Do not retry service registration if it fails once
04/25/15    av     Support for hiding CSIM from modem clients
04/03/15    kk     Migrate to mcfg EFS wrappers
03/04/15    vdc    Added support for QMI UIM GBA
09/30/14    kk     Enhancements to NV handling
08/29/14    tl     Add support for supply voltage command and indication
05/22/14    tl     Added support for recovery complete indications
12/24/13    am     Add NV protection to Recovery API
02/19/13    av     Merge mmgsi,gstk,qmiuim/cat conf files to mmgsdi.conf
02/14/13    tl     Move QMI UIM to MMGSDI context from QMI modem context
02/23/12    nmb    QMI QMUX Changes
08/05/10    tkl    QMI UIM & QMI CAT split from data package
01/14/10    mib    Initial check-in of QMI_UIM
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_p.h"

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif /* FEATURE_QSH_MDUMP */

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/
#define QMI_UIM_CONFIG_GBA_FQDN_WHITE_LIST  "/nv/item_files/modem/qmi/uim/gba_fqdn_white_list"
#define QMI_UIM_CONFIG_GBA_FQDN_BLACK_LIST  "/nv/item_files/modem/qmi/uim/gba_fqdn_black_list"

/* Name of QMI UIM client (to be notified to MMGSDI when doing a client id reg*/
#define QMI_UIM_CLIENT_NAME                 "qmi_uim"

/*===========================================================================
  FUNCTION QMI_UIM_INIT_PRE_STARTUP()

  DESCRIPTION
    Initializes the QMI UIM global variables and register with MMGSDI

  PARAMETERS
    void

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_init_pre_startup
(
  void
);

/*===========================================================================
  FUNCTION QMI_UIM_INIT_POST_STARTUP()

  DESCRIPTION
    Registers the QMI UIM service to the QMI framework to initalize
    the service.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_init_post_startup
(
  void
);

/*===========================================================================
QMI_UIM_SIG_HANDLER

DESCRIPTION
  QMI UIM signal handler for MMGSDI task signals

PARAMETERS
  None.

  Returns TRUE if signal should be cleared from the set signal mask,
  FALSE if further signal processing is needed and hence signal
  should not be cleared.
===========================================================================*/
extern boolean qmi_uim_sig_handler
(
  void
);

/*===========================================================================
  FUNCTION QMI_UIM_SUPPLY_VOLTAGE_IND()

  DESCRIPTION
    This function is invoked by the UIM drivers when the Vcc needs to
    be deactivated or when Vcc has been activated.

  PARAMETERS
    slot                                      : UIM slot id
    uimdrv_qmi_indications_type               : LDO state
    uimdrv_qmi_power_management_callback_type : callback to ack drivers

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_supply_voltage_ind
(
  uim_slot_type                                     slot,
  uimdrv_qmi_indications_type                       ldo_state,
  uimdrv_qmi_power_management_callback_type         uim_callback_ptr
);

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
  FUNCTION  UIM_QSH_MDUMP_UIMQMI

  DESCRIPTION
    This function is used to collect qmi module globals.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
QSH_MDUMP_FN_ATTR void uim_qsh_mdump_uimqmi(void);
#endif /* FEATURE_QSH_MDUMP */
#endif /* QMI_UIM_H */
