/*==========================================================================

                         QMI_CAT_WITHOUT_UI. C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Card Application Toolkit Service source
 file.

Copyright (c) 2016 - 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_cat_without_ui.c#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/17/17    dt     Enable Data Card Config. if QMI CAT is not supported
01/20/16    vr     Initial check-in of QMI_CAT without UI
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "comdef.h"
#include "customer.h"

#include "qmi_cat.h"


/*===========================================================================

                                DATA TYPES

===========================================================================*/

/* Empty TERMINAL PROFILE support from QMI-CAT */
static gstk_profile_dl_support_ext_type qmi_cati_empty_profile_value_list[1] = {{0}};


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_CAT_INIT_PRE_STARTUP()

  DESCRIPTION
    Initializes the QMI CAT global data and message queues

  PARAMETERS
    None

  RETURN VALUE
    boolean

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_cat_init_pre_startup
(
  void
)
{
  return FALSE;
} /* qmi_cat_init_pre_startup */


/*===========================================================================
  FUNCTION QMI_CAT_INIT_POST_STARTUP()

  DESCRIPTION
    Registers the QMI CAT service to the QMI framework to initalize the service
    and registers with MMGSDI, GSTK.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cat_init_post_startup
(
  void
)
{
  /* Dummy Function */
} /* qmi_cat_init_post_startup */


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
gstk_profile_dl_support_ext_type* qmi_cat_get_terminal_profile_value_list
(
  uint8                            * tp_count_ptr
)
{
  /* Dummy Function */
  if (tp_count_ptr == NULL)
  {
    return NULL;
  }

  *tp_count_ptr = 0;

  return qmi_cati_empty_profile_value_list;
} /* qmi_cat_get_terminal_profile_value_list */


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
boolean qmi_cat_sig_handler
(
  void
)
{
  /* Dummy Function */
  return TRUE;
} /* qmi_cat_sig_handler */


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
)
{
  /* Dummy function */
  (void) slot;
} /* qmi_cat_recovery_complete*/


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
void qmi_cat_deinit
(
  void
)
{
  /* Dummy function */
}/* qmi_cat_deinit */
