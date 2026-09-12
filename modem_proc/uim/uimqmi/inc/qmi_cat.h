#ifndef QMI_CAT_H
#define QMI_CAT_H
/*===========================================================================

                         Q M I _ C A T . H

DESCRIPTION

 The Data Services QMI Card Application Toolkit Service header file.

                      COPYRIGHT INFORMATION

Copyright (c) 2009 - 2010, 2012 - 2015 , 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/inc/qmi_cat.h#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/17/17    dt     Enable Data Card Config. if QMI CAT is not supported
11/26/15    sp     Updated copyright
05/04/15    kr     Replaced Hardcoded values with QMI CAT IDL defined values
04/30/15    hh     Include needed header file
04/29/15    av     Do not retry service registration if it fails once
04/03/15    kk     Migrate to mcfg EFS wrappers
02/12/15    hh     Split qmi_cat_init into pre and post startup
10/30/14    gm     Move enable/disable of raw unsupported features to NV
09/24/14    gm     Remove GOBI mode code
09/09/14    bd     Moved an enum to header file
08/05/14    hh     Added support to retrieve supported proactive commands
01/09/14    kb     Add de-init function when gstk task is stopped
04/11/13    tkl    Add NV suport for SETUP Call display alpha value
04/02/13    av     Move UIM tasks to group4 of RCINIT
02/19/13    av     Merge mmgsdi,gstk,qmiuim/cat conf files to mmgsdi.conf
02/14/13    tl     Move QMI CAT to GSTK context from QMI modem context
02/23/12    nmb    QMI QMUX Transition
08/05/10    tkl    QMI UIM & QMI CAT split from data package
03/25/10    dd     Adding decoded tlvs
11/01/09    mj     Initial check-in of QMI_CAT
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "gstk_exp.h"
#include "mmgsdilib_common.h"

/*=============================================================================

                   ENUMERATED DATA

=============================================================================*/

/*---------------------------------------------------------------------------
  QMI CAT configuration type
---------------------------------------------------------------------------*/

/*=============================================================================

                   FUNCTIONS

=============================================================================*/

/*===========================================================================
  FUNCTION QMI_CAT_INIT_PRE_STARTUP()

  DESCRIPTION
    Initializes the QMI CAT global data and message queues

  PARAMETERS
    None

  RETURN VALUE
    Boolean

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern boolean qmi_cat_init_pre_startup
(
  void
);

/*===========================================================================
  FUNCTION QMI_CAT_INIT_POST_STARTUP()

  DESCRIPTION
    Registers the QMI CAT service to the QMI framework to initalize the service
    and registers with MMGSDI, GSTK

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void qmi_cat_init_post_startup
(
  void
);

/*===========================================================================
QMI_CAT_SIG_HANDLER

DESCRIPTION
  QMI CAT signal handler for GSTK signals

PARAMETERS
  None.

  Returns TRUE if signal should be cleared from the set signal mask,
  FALSE if further signal processing is needed and hence signal
  should not be cleared.
===========================================================================*/
extern boolean qmi_cat_sig_handler
(
  void
);

/*===========================================================================
  FUNCTION QMI_CAT_GET_TERMINAL_PROFILE_VALUE_LIST()

  DESCRIPTION
    Get terminal profile value list according to the QMI_CAT config mode.

  PARAMETERS
    none

  RETURN VALUE
    gstk_profile_dl_support_ext_type* : array of terminal profile

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern gstk_profile_dl_support_ext_type* qmi_cat_get_terminal_profile_value_list
(
  uint8                            * tp_count_ptr
);

/*===========================================================================
  FUNCTION QMI_CAT_RECOVERY_COMPLETE()

  DESCRIPTION
    This function is called by the QMI UIM when a recovery indication
    has completed successfully

  PARAMETERS
    slot: Indicates the slot in which the recovery occured

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cat_recovery_complete
(
  mmgsdi_slot_id_enum_type  slot
);

/*===========================================================================
FUNCTION: qmi_cat_deinit

DESCRIPTION:
  This function is used to free the qmicat private resources.
  This is called when GSTK is powering down.

PARAMETERS:
  None

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void qmi_cat_deinit
(
  void
);

#endif /* QMI_CAT_H */
