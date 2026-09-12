/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              H D R   P H Y S I C A L  L A Y E R  P R O T O C O L       

GENERAL DESCRIPTION
  This module deals with AT-initiated configuration of Physical layer
  protocol. 
  
EXTERNALIZED FUNCTIONS
  hdrphy_configure
   
INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2007 - 2012 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================

                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrphy.c#1 $ $DateTime: 2019/06/19 22:12:26 $Author: $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/16/12   arm     Resolve TCB errors for Dime. 
10/02/09   lyl     Updated for rumi bringup
01/12/09   etv     Disable DTXMode after connection close or fail.
11/21/08   etv     Propagate DTXMode value to RMAC4 after SESSION_CHANGED ind.
09/03/08   etv     Added support to process AN GAUP.
06/18/08   etv     Added support for indication processing.
                   Added support for DTXMode attribute.
08/15/07   etv     Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                          INCLUDE FILES FOR MODULE
                     
===========================================================================*/
#include "hdr_variation.h"
#include "task.h"
#include "hdrphy.h"

#include "hdrtx.h"
#include "hdrscmphy.h"
#include "hdrscp.h"
#include "hdrlog.h"
#include "hdrind.h"

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/
#define HDRPHY_TASK_PTR          HDRTX_TASK_PTR


/* GAUPable configuration attributes of AMAC */
LOCAL const hdrscmphy_attrib_id_enum_type 
  hdrphy_gaup_attrib_ids[HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS] = 
{
  HDRSCMPHY_MAX_RL_TX_POWER_DIFF,             /* Not Supported */
  HDRSCMPHY_DTX_MODE,                         /* Only AN GAUP supported */
  HDRSCMPHY_DTX_RRI_MODE,                     /* Not Supported */
  HDRSCMPHY_ADDITIONAL_CARRIERS_INIT_TX_POWER /* Not Supported */
};

/* PHY cares only about the version numbers of the GAUPable attributes to find
   out the attributes changed due to recent GAUP. This table maintains the
   offset of each GAUPable attribute's version number (in the same order as the
   table "hdrphy_gaup_attrib_ids") in the master version number array */
LOCAL const uint16
  hdrphy_gaup_attrib_verno_offsets[HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS] = 
{
  HDRSCMPHY_MAX_RL_TX_POWER_DIFF_VERNO_OFFSET, 
    /* HDRSCMPHY_MAX_RL_TX_POWER_DIFF - Not Supported */
  HDRSCMPHY_DTX_MODE_VERNO_OFFSET, 
    /* HDRSCMPHY_DTX_MODE - Only AN GAUP Supported */
  HDRSCMPHY_DTX_RRI_MODE_VERNO_OFFSET, 
    /* HDRSCMPHY_DTX_RRI_MODE - Not Supported */
  HDRSCMPHY_ADDITIONAL_CARRIERS_INIT_TX_POWER_VERNO_OFFSET 
    /* HDRSCMPHY_ADDITIONAL_CARRIERS_INIT_TX_POWER - Not Supported */
};

static struct
{
  hdrscmphy_config_struct_type config;
    /* Negotiated Config */

  hdrlog_dtx_mode_enum_type dtx_mode_pref;
    /* DTXMode user's preference */

  boolean override;
    /* Override DTXMode with user preference? */

  uint16 current_subtype;
    /* Current subtype of Physical layer protocol */

  hdrphy_dtx_mode_monitor_type dtx_mode_monitor;
    /* DTXMode Monitor. 
       There is only one DTXMode monitor (RMAC). Once we have a need 
       for more than one monitor, this can be changed to a table. 
       Let's keep it simple for now. */

  uint8 gaup_attrib_verno[HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS];
    /* GAUP attribute version numbers */

  void *gaup_attribs[HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS];
    /* Pointer to each of the GAUPable attrib in the config struct instance. */

} hdrphy;


/* EJECT */
/*===========================================================================

FUNCTION HDRPHY_RECORD_VERSION_NUMS

DESCRIPTION
  The Configuration attribute version numbers are needed only for GAUPable
  attributes in order to query the SCM DB and find out the updated attribs. 
  This function is used to store the version number of GAUPable attributes
  into static memory.

DEPENDENCIES
  None.

PARAMETERS
  verno - Array of version numbers one for each PHY config attrib.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void hdrphy_record_version_nums
(
  hdrscmphy_verno_type verno
)
{

  uint8 g_attr_ind;
    /* GAUP attribute index in the version number array */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  /* Store the version numbers of GAUPable attributes */
  for (g_attr_ind=0; g_attr_ind<HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS; ++g_attr_ind)
  {
    hdrphy.gaup_attrib_verno[g_attr_ind] = 
      verno[hdrphy_gaup_attrib_verno_offsets[g_attr_ind]];
  }

} /* hdrphy_record_version_nums */

/*===========================================================================
FUNCTION HDRPHY_INIT                                   EXTERNAL

DESCRIPTION
  This function does the initialization for Physical layer protocol

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrphy_init (void)
{

  hdrphy.override = FALSE;

  hdrphy.current_subtype = HDRSCP_DEFAULT_PHYS_LAYER;

  hdrphy.gaup_attribs[0] = &hdrphy.config.max_rl_tx_pwr_diff;
  hdrphy.gaup_attribs[1] = &hdrphy.config.dtx_mode;
  hdrphy.gaup_attribs[2] = &hdrphy.config.dtx_rri_mode;
  hdrphy.gaup_attribs[3] = &hdrphy.config.init_tx_pwr;

} /* hdrphy_init */

/*===========================================================================
FUNCTION HDRPHY_IND_CB                                              EXTERNAL

DESCRIPTION
  This function processes incoming indications for this protocol.

DEPENDENCIES
  None.

PARAMETERS
  ind_name     - Name of the indication from hdrhai.h
  ind_data_ptr - Pointer to any indication-specific data

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrphy_ind_cb
(
  hdrind_ind_name_enum_type ind_name,
  void *ind_data
)
{

  HDR_MSG_PROT_1( MSG_LEGACY_LOW, "hdrphy_ind_cb %d", ind_name );

  if (rex_self () == HDRPHY_TASK_PTR)
  {
    /* Indication is from another protocol in the same task, so consider it
     * an internal indication
     */
    hdrphy_process_ind (ind_name, (hdrind_ind_data_union_type *)ind_data);
  }
  else
  {
    hdrtx_queue_ind( HDRHAI_PHYSICAL_LAYER_PROTOCOL, ind_name, ind_data );
  }

} /* hdrphy_ind_cb */

/* EJECT */
/*===========================================================================

FUNCTION HDRPHY_PROCESS_ATTRIB_UPDATE

DESCRIPTION
  This function processes the GAUP Update indication by querying SCM to find
  the updated attributes and gets the values of updated attributes and takes
  appropriate action.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrphy_process_attrib_update(void)
{
  boolean is_changed[HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS];
    /* Has the GAUPable attribute changed since last read? */

  uint16 g_attr_ind;
    /* GAUPable attribute index. */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */  
#ifndef FEATURE_RUMI3_BRINGUP
  /* Query SCM DB to find the changed attributes */
  hdrscmdb_find_changed_attribs( HDRHAI_PHYSICAL_LAYER_PROTOCOL,
                                 HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS,
                                 hdrphy.gaup_attrib_verno,
                                 hdrphy_gaup_attrib_verno_offsets,
                                 is_changed  );
#endif /* FEATURE_RUMI3_BRINGUP */

  for ( g_attr_ind = 0; 
        g_attr_ind < HDRSCMPHY_MAX_NUM_GAUPABLE_ATTRIBS; ++g_attr_ind )
  {
    if (is_changed[g_attr_ind])
    {
      /* Get the Updated attribute value from the SCM Database */
      hdrphy.gaup_attrib_verno[g_attr_ind] = 
#ifndef FEATURE_RUMI3_BRINGUP
      hdrscmdb_get_inuse_attrib( HDRHAI_PHYSICAL_LAYER_PROTOCOL,
                                 hdrphy_gaup_attrib_ids[g_attr_ind], 
                                 hdrphy.gaup_attribs[g_attr_ind],
                                 HDRSCP_SUB3_PHYS_LAYER );
#else
      0;
#endif /* FEATURE_RUMI3_BRINGUP */

      switch (hdrphy_gaup_attrib_ids[g_attr_ind])
      {
#ifdef FEATURE_HDR_REVB_DTX_MODE
        case HDRSCMPHY_DTX_MODE:
          /* If there is a valid registration */
          if (hdrphy.dtx_mode_monitor != NULL)
          {
            /* Notify the change to DTXMode to the registered monitor */
            (*hdrphy.dtx_mode_monitor) ( (hdrphy.config.dtx_mode == 0x01) );
          }
          break;
#endif /* FEATURE_HDR_REVB_DTX_MODE */

        default:
          HDR_MSG_PROT_1(MSG_LEGACY_ERROR, 
                         "GAUP of attrib 0x%x not supported", 
                         hdrphy_gaup_attrib_ids[g_attr_ind]);
      }
    }
  }


} /* hdrphy_process_attrib_update */

/* EJECT */
/*===========================================================================

FUNCTION HDRPHY_PROCESS_IND 

DESCRIPTION
  This function processes indications given to Physical layer protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  ind_name - Name of the indication for PHY to process
  ind_data_ptr - pointer to data associated with the indication
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrphy_process_ind
(
  hdrind_ind_name_enum_type ind_name,
  hdrind_ind_data_union_type * ind_data_ptr
)
{

  hdrscmphy_verno_type ver_no;
    /* Array to hold the version numbers of PHY config attribs */

  hdrerrno_enum_type status = E_SUCCESS;
    /* Error code */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrphy.current_subtype = 
    hdrscp_get_current_subtype(HDRHAI_PHYSICAL_LAYER_PROTOCOL);

  if (hdrphy.current_subtype == HDRSCP_SUB3_PHYS_LAYER)
  {
    switch(ind_name)
    {
      case HDRIND_SCM_SESSION_CHANGED:
        /* Get the current value and the version number for each attribute */
#ifndef FEATURE_RUMI3_BRINGUP
        hdrscmdb_get_inuse_config_and_verno( 
          HDRHAI_PHYSICAL_LAYER_PROTOCOL, 
          &hdrphy.config, 
          ver_no, 
          HDRSCMPHY_MAX_NUM_CONFIG_ATTRIBS );
#endif /* FEATURE_RUMI3_BRINGUP */

        /* Record the version numbers that we need */
        hdrphy_record_version_nums(ver_no);

#ifdef FEATURE_HDR_REVB_DTX_MODE
        /* If there is a valid registration */
        if (hdrphy.dtx_mode_monitor != NULL)
        {
          /* Notify the DTXMode configuration to the registered monitor */
          (*hdrphy.dtx_mode_monitor) ( (hdrphy.config.dtx_mode == 0x01) );
        }

        break;

      case HDRIND_ALMP_CONNECTION_CLOSED:
      case HDRIND_OVHD_SUPERVISION_FAILED:
      case HDRIND_CCMAC_SUPERVISION_FAILED:
      case HDRIND_FTCMAC_SUPERVISION_FAILED:
      case HDRIND_RUP_ASSIGNMENT_REJECTED:
        /* Disable DTX Mode on connection close or fail */
        hdrphy.config.dtx_mode = 0;

        /* If there is a valid registration */
        if (hdrphy.dtx_mode_monitor != NULL)
        {
          /* Notify the DTXMode configuration to the registered monitor */
          (*hdrphy.dtx_mode_monitor) ( (hdrphy.config.dtx_mode == 0x01) );
        }

        /* Reset DTXMode in SCM database */
        hdrscmdb_reset_dtx_mode();

#endif /* FEATURE_HDR_REVB_DTX_MODE */

        /* Break not featurized, so that we don't fall through from 
           SCM_SESSION_CHANGED indication when DTX feature is not defined. */
        break;

      case HDRIND_SCM_PHY_AN_GAUP:
        hdrphy_process_attrib_update();
        break;

      default:
        status = E_DATA_INVALID;

    } /* Switch */

  }
  else
  {
    status = E_DATA_INVALID;
  }

  if (status != E_SUCCESS)
  {
    ERR("Invalid indication: %d for subtype %d", 
        ind_name, hdrphy.current_subtype, 0);
  }

} /* hdrphy_process_ind */


/*===========================================================================
FUNCTION HDRPHY_CONFIGURE                                   EXTERNAL

DESCRIPTION
  This function does the AT-init configuration of MaxTransmitPower for 
  Subtype-3 physical layer protocol

DEPENDENCIES
  None.

PARAMETERS
  sender - The calling protocol.

RETURN VALUE
  TRUE - If the configuration was needed.
  FALSE - Otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrphy_configure
(
  hdrhai_protocol_name_enum_type sender
)
{
  uint16 subtype = hdrscp_get_config_subtype(HDRHAI_PHYSICAL_LAYER_PROTOCOL);
    /* PHY In-Config Subtype */

  boolean config_status = FALSE;
    /* AT-init Config needed? */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* mark the unused variable to avoid the lint error */
  SYS_ARG_NOT_USED( sender );

  /* Get InConfig subtype */
  subtype = hdrscp_get_config_subtype(HDRHAI_PHYSICAL_LAYER_PROTOCOL);

  /* If subtype is PHY3 and configuration is needed, */
  if ( (subtype == HDRSCP_SUB3_PHYS_LAYER) && hdrscmphy_config_is_needed() )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "PHY Config is required" );

    config_status = hdrscmphy_configure();
  }

  return config_status;

} /* hdrphy_configure */


#ifdef FEATURE_HDR_REVB_DTX_MODE
/*===========================================================================
FUNCTION HDRPHY_SET_DIAG_DTX_MODE                          EXTERNAL

DESCRIPTION
  Sets the diag user DTXMode preference and action.
  Triggers callback registered to monitor changes to DTXMode.

  If the diag user overrides the DTXMode using the diag command, the overridden
  mode will be used otherwise the negotiated value will be used.

DEPENDENCIES
  None.

PARAMETERS
  action - (HDRLOG_RESET_DTX_MODE/HDRLOG_OVERRIDE_DTX_MODE) DTX modes
  mode   - DTXMode to override (valid only when action is Override)

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void hdrphy_set_diag_dtx_mode
(
  hdrlog_dtx_mode_action_enum_type action,
  hdrlog_dtx_mode_enum_type mode
)
{
  uint8 dtx_mode = hdrphy.config.dtx_mode;
    /* Temp variable - DTXMode */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the action is to override, set override to true */
  hdrphy.override = (action == HDRLOG_OVERRIDE_DTX_MODE);

  if (hdrphy.override)
  {
    /* Read user preference DTXMode if the action is to override */
    hdrphy.dtx_mode_pref = mode;
    dtx_mode = hdrphy.dtx_mode_pref;
  }

  /* If there is a valid registration */
  if (hdrphy.dtx_mode_monitor != NULL)
  {
    /* Notify the change to DTXMode to the registered monitor */
    (*hdrphy.dtx_mode_monitor)( (dtx_mode == 0x01) );

  }

} /* hdrphy_set_diag_dtx_mode */

/*===========================================================================
FUNCTION HDRPHY_REGISTER_DTX_MODE_MONITOR                          EXTERNAL

DESCRIPTION
  This function is used to register callbacks to monitor changes to DTXMode.
  At present this module supports only one Monitor.

DEPENDENCIES
  None.

PARAMETERS
  cb - DTXMode monitor

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrphy_register_dtx_mode_monitor (hdrphy_dtx_mode_monitor_type cb)
{

  hdrphy.dtx_mode_monitor = cb;

} /* hdrphy_register_dtx_mode_monitor */

#endif /* FEATURE_HDR_REVB_DTX_MODE */
