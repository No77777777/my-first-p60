/*
==============================================================================

FILE:         VCSBSP.c

DESCRIPTION:
  This file contains VCS BSP data for DAL based driver.

==============================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/systemdrivers/vcs/image/mss/hw/sm6150/VCSBSP.c#1 $

==============================================================================
            Copyright (c) 2018 Qualcomm Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "comdef.h"
#include "VCSBSP.h"
#include "HALvcsHWIO.h"
#include "HALhwio.h"

// Components used in this image
#include "VCSRPMhRailImpl.h"
#include "VCSStubRailImpl.h"
#include "HALvcsQDSP6v65.h"


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


static const DalChipInfoIdType VCS_ChipInfoId_Default[] =
{
  CHIPINFO_ID_SM6150,
  CHIPINFO_ID_SM6150P,
  DALCHIPINFO_NUM_IDS
};

static const DalChipInfoIdType VCS_ChipInfoId_AU[] =
{
  CHIPINFO_ID_SA6155P,
  CHIPINFO_ID_SA4155P,
  // CHIPINFO_ID_SA6155,
  DALCHIPINFO_NUM_IDS
};


/*
 * aRails
 *
 * Set of rail instances.
 */
static VCSRailNodeType aRails[] =
{
  /*-----------------------------------------------------------------------*/
  /* CX (Modem SW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_cx"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_Default), /* Version */
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_CX_ModemSW),
  },

  /*-----------------------------------------------------------------------*/
  /* CX for AU (Modem SW DRV id)                                           */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_cx"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_AU), /* Version */
    /* no dependency */
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_CX_ModemSW),
  },

  /*-----------------------------------------------------------------------*/
  /* CX (Modem HW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_cx/modem_hw"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_Default), /* Version */
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx/modem_hw"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_CX_ModemHW),
  },

  /*-----------------------------------------------------------------------*/
  /* CX for AU (Modem HW DRV id)                                           */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_cx/modem_hw"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_AU), /* Version */
    /* no dependency */
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_CX_ModemHW),
  },

  /*-----------------------------------------------------------------------*/
  /* MX (Modem SW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mx"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_Default), /* Version */
    /* no dependency */
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MX_ModemSW),
  },

  /*-----------------------------------------------------------------------*/
  /* MX for AU (Modem SW DRV id)                                           */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mx"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_AU), /* Version */
    VCS_DEF_ALIAS("/vcs/vdd_cx"),
  },

  /*-----------------------------------------------------------------------*/
  /* MX (Modem HW DRV id)                                                  */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mx/modem_hw"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_Default), /* Version */
    /* no dependency */
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MX_ModemHW),
  },

  /*-----------------------------------------------------------------------*/
  /* MX for AU (Modem HW DRV id)                                           */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mx/modem_hw"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_AU), /* Version */
    VCS_DEF_ALIAS("/vcs/vdd_cx/modem_hw"),
  },

  /*-----------------------------------------------------------------------*/
  /* MSS (Modem SW DRV id)                                                 */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_Default), /* Version */
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MSS_ModemSW),
  },

  /*-----------------------------------------------------------------------*/
  /* MSS for AU (Modem SW DRV id)                                          */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_AU),
    VCS_DEF_ALIAS("/vcs/vdd_cx"),
  },

  /*-----------------------------------------------------------------------*/
  /* MSS (Modem HW DRV id)                                                 */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss/modem_hw"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_Default), /* Version */
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx/modem_hw"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPMh, &VCSRPMhRailImpl_MSS_ModemHW),
  },

  /*-----------------------------------------------------------------------*/
  /* MSS for AU (Modem HW DRV id)                                          */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss/modem_hw"),
    VCS_DEF_VERSION(0, 0, 0, 0, 0, VCS_ChipInfoId_AU),
    VCS_DEF_ALIAS("/vcs/vdd_cx/modem_hw"),
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
