#ifndef MCFG_SEL_DB_H
#define MCFG_SEL_DB_H
/*==============================================================================

                M O D E M   C O N F I G   S E L E C T I O N

GENERAL DESCRIPTION
  Contains the MCFG - MRE interface chnages to read the MCFG xml file for the Selection Record DB.

Copyright (c) 2012 -2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_sel/inc/mcfg_sel_db.h#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
12/07/16  sk   Created module

==============================================================================*/
#include "sys.h"
#include "mre.h"
#include "mcfg_uim.h"

/*==============================================================================

                PUBLIC DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/

#define MCFG_MAX_SELECTION_PARAMETER_TOKEN_LENGTH 32

/*==============================================================================
  Typedefs
==============================================================================*/

  typedef struct
  {
    mcfg_slot_index_type_e_type sloti;
      
    boolean have_iin;
    char  iin[MCFG_UIM_ICCID_STR_BUF_LEN];
  
    boolean have_plmn;
    sys_plmn_id_s_type *pPlmn;
  
    boolean have_plmn_3gpp2;
    sys_plmn_id_s_type *pPlmn3gpp2;

    boolean have_gid[MCFG_GID_INDEX_MAX];
    char    gid[MCFG_GID_INDEX_MAX][MCFG_UIM_GID_STR_BUF_LEN];

  } mcfg_mre_state_t;


/*==============================================================================
  Variables
==============================================================================*/


/*==============================================================================

                    PUBLIC FUNCTION DECLARATIONS FOR MODULE

==============================================================================*/


/*===========================================================================
  
  FUNCTION mcfg_sel_db_init

  DESCRIPTION.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_sel_db_init(void);

/*===========================================================================

  FUNCTION mcfg_run_policy

  DESCRIPTION.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_run_policy(mcfg_sub_id_type_e_type sub, uint8 test_no);

/*===========================================================================

  FUNCTION mcfg_create_new_policy

  DESCRIPTION
    This function is required by mre_policy_create

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
mre_policy_t  *mcfg_create_new_policy(
    char const              *pPolicyName,
    mre_xml_element_t const *pPolicyElem
);

/*===========================================================================
  
    FUNCTION mcfg_sel_db_release
  
    DESCRIPTION.
  
    DEPENDENCIES
      None
  
    PARAMETERS
      None
  
    RETURN VALUE
      None
  
    SIDE EFFECTS
      None
  
===========================================================================*/
void mcfg_sel_db_release(void );

/*===========================================================================
  
    FUNCTION mcfg_sel_db_get_carrier_index
  
    DESCRIPTION.
  
    DEPENDENCIES
      None
  
    PARAMETERS
      None
  
    RETURN VALUE
      None
  
    SIDE EFFECTS
      None
  
===========================================================================*/
boolean mcfg_sel_db_get_carrier_index
(
  mcfg_slot_index_type_e_type     sloti,
  pdc_selection_carrier_enum_v01 *carrier_index,
  mcfg_config_id_s_type          *config_id
);

#endif /* MCFG_SEL_DB_H */
