/*
==============================================================================

FILE:         VCSBSP.c

DESCRIPTION:
  This file contains VCS bsp data for DAL based driver.

==============================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/systemdrivers/vcs/image/mss/hw/sdm855/VCSBSP.c#1 $

==============================================================================
            Copyright (c) 2014 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "comdef.h"
#include "VCSBSP.h"

// Components used in this image
#include "VCSRPMhRailImpl.h"
#include "VCSStubRailImpl.h"


/*=========================================================================
      Externs
==========================================================================*/


/*=========================================================================
      Macros
==========================================================================*/


/*=========================================================================
      Data Declarations
==========================================================================*/


/*
 * VCSRPMhRailImpl_MX_ModemSW
 */
static VCSRPMhRailImplType VCSRPMhRailImpl_MX_ModemSW =
{
  .szName        = "mx.lvl",
  .eDRVId        = RSC_DRV_MODEM_SW,
  .bBatchCommand = TRUE,
};


/*
 * VCSRPMhRailImpl_MX_ModemHW
 */
static VCSRPMhRailImplType VCSRPMhRailImpl_MX_ModemHW =
{
  .szName        = "mx.lvl",
  .eDRVId        = RSC_DRV_MODEM_HW,
  .bBatchCommand = TRUE,
};


/*
 * VCSRPMhRailImpl_CX_ModemSW
 */
static VCSRPMhRailImplType VCSRPMhRailImpl_CX_ModemSW =
{
  .szName        = "cx.lvl",
  .eDRVId        = RSC_DRV_MODEM_SW,
  .bBatchCommand = TRUE,
};


/*
 * VCSRPMhRailImpl_CX_ModemHW
 */
static VCSRPMhRailImplType VCSRPMhRailImpl_CX_ModemHW =
{
  .szName        = "cx.lvl",
  .eDRVId        = RSC_DRV_MODEM_HW,
  .bBatchCommand = TRUE,
};


/*
 * VCSRPMhRailImpl_MSS_ModemSW
 */
static VCSRPMhRailImplType VCSRPMhRailImpl_MSS_ModemSW =
{
  .szName        = "mss.lvl",
  .eDRVId        = RSC_DRV_MODEM_SW,
  .bBatchCommand = TRUE,
};


/*
 * VCSRPMhRailImpl_MSS_ModemHW
 */
static VCSRPMhRailImplType VCSRPMhRailImpl_MSS_ModemHW =
{
  .szName        = "mss.lvl",
  .eDRVId        = RSC_DRV_MODEM_HW,
  .bBatchCommand = TRUE,
};


/*
 * aRails
 *
 * Set of rail instances.
 */
static VCSRailNodeType aRails[] =
{
  /*-----------------------------------------------------------------------*/
  /* MX (Modem SW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mx"),
    /* no dependency */
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MX_ModemSW),
  },

  /*-----------------------------------------------------------------------*/
  /* MX (Modem HW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mx/modem_hw"),
    /* no dependency */
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MX_ModemHW),
  },

  /*-----------------------------------------------------------------------*/
  /* CX (Modem SW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_cx"),
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_CX_ModemSW),
  },

  /*-----------------------------------------------------------------------*/
  /* CX (Modem HW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_cx/modem_hw"),
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx/modem_hw"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_CX_ModemHW),
  },

  /*-----------------------------------------------------------------------*/
  /* MSS (Modem SW DRV id)                                                 */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss"),
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MSS_ModemSW),
  },

  /*-----------------------------------------------------------------------*/
  /* MSS (Modem HW DRV id)                                                 */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss/modem_hw"),
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx/modem_hw"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MSS_ModemHW),
  },

  /*-----------------------------------------------------------------------*/
  /* MSS_Q6                                                                */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss_q6"),
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mss"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_SINGLE_CLIENT),
    VCS_DEF_RAIL_IMPL(Stub, NULL),
  },

};


/*
 * VCS Log Default Configuration.
 */
const VCSLogType VCS_LogDefaultConfig[] =
{
  {
    /* .nLogSize = */ 4096,
  }
};


static VCSBaseType aBases[] =
{
  /* empty */
};


/*
 *  VCSBSP
 *
 *  List and length of Rail and LDO configurations.
 */
const VCSBSPType VCSBSP =
{
  VCS_DEF_ARR_FIELD(Bases, aBases),
  VCS_DEF_ARR_FIELD(Rails, aRails),
};
