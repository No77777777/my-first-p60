#ifndef MMODE_INT_H
#define MMODE_INT_H

/**==========================================================================
@FILE_DOC

              M U L T I M O D E   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary for Call Manager,
  Multimode Controller AND System Determination modules. Only the definitions
  needed for ALL 3 MMODE modules.

  Note: The header file should not be used outside of mmode modules.

Copyright (c) 2002 - 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/api/mmode_internal.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/09/15   AK      Initial version
===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MASK_GPP          BM(0)
#define MASK_GPP2         BM(1)

#if defined FEATURE_MMODE_TRIPLE_SIM
#define MAX_SIMS                                       3 /* No.of sims*/
#elif defined(FEATURE_MMODE_SXLTE_G) || defined(FEATURE_MMODE_DUAL_SIM)
#define MAX_SIMS                                       2 /* No.of sims*/
#else
#define MAX_SIMS                                       1 /* No.of sims*/
#endif

/* Max stacks supported for any sub */
#define MAX_STACKS        3  

/*
** Enumeration of stack id
*/
typedef enum multimode_stack_e {

  MM_STACK_NONE = -1,

  MM_STACK_0,

  MM_STACK_1,

  MM_STACK_2,

  MM_STACK_ALL

} multimode_stack_e_type;

#define  MM_STACK_NO_ACTIVE_MASK    0
#define  MM_STACK_0_MASK            BM(MM_STACK_0)
#define  MM_STACK_1_MASK            BM(MM_STACK_1)
#define  MM_STACK_2_MASK            BM(MM_STACK_2)

#define SD_SS_MODE_PREF_CS_ONLY (SD_SS_MODE_PREF_CDMA |\
                                SD_SS_MODE_PREF_GSM | SD_SS_MODE_PREF_WCDMA | \
                                SD_SS_MODE_PREF_TDS )

typedef struct mm_unique_stk_id_s{

  sys_modem_as_id_e_type    asubs_id;

  multimode_stack_e_type    stk_id;

}mm_sub_stk_id_s_type;


#define SUB_STK_TYPE\
  sys_modem_as_id_e_type    asubs_id,\
  multimode_stack_e_type    stk_id\

/*===========================================================================

FUNCTION cmsds_is_gwt_scan_allowed

DESCRIPTION
  This function returns whether GWT scan allowed during RLF based
  on VOLTE call and NV enabled

DEPENDENCIES
  none

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsds_is_gwt_scan_allowed_new(
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

FUNCTION cmsds_is_gwt_scan_allowed

DESCRIPTION
  This function returns whether GWT scan allowed during RLF based
  on VOLTE call and NV enabled

DEPENDENCIES
  none

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsds_is_gwt_scan_allowed(
  sys_modem_as_id_e_type asubs_id,
  multimode_stack_e_type *stk_id
);

/*===========================================================================

FUNCTION cmcall_check_spl_emergency_on_hplmn

DESCRIPTION
  This function checks if the call is special emergency on the home plmn.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  TRUE if the call is special emergency on the home plmn
  False Otherwise

SIDE EFFECTS
  none

===========================================================================*/
boolean cmcall_check_spl_emergency_on_hplmn( void );

/*===========================================================================

FUNCTION cmregprx_check_lte_priority_scan

DESCRIPTION
  Check if CS to PS redial is enabled
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmregprx_check_lte_priority_scan(
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

FUNCTION cmregprx_check_lte_priority_scan_voice

DESCRIPTION
 This function checks if LTE priority scan is
 required when the CS to PS special VOLTE redial is enabled.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern boolean  cmregprx_check_lte_priority_scan_voice
(
  mm_sub_stk_id_s_type  mm_id
);


/*===========================================================================

FUNCTION cmutil_is_mm_id_valid

DESCRIPTION
  Check mm id is valid or not

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean cmutil_is_mm_id_valid(
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

FUNCTION mm_map_sub_stk_to_id

DESCRIPTION
  Map sub id and stack id to id in structure.

DEPENDENCIES
  None.

RETURN VALUE
  mm_sub_stk_id_s_type

SIDE EFFECTS
  None

===========================================================================*/
mm_sub_stk_id_s_type  mm_map_sub_stk_to_id(
  sys_modem_as_id_e_type  as_id,
  multimode_stack_e_type  stk_id
);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
