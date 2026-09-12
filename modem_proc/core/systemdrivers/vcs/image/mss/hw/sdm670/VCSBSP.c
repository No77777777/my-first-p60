/*
==============================================================================

FILE:         VCSBSP.c

DESCRIPTION:
  This file contains VCS bsp data for DAL based driver.

==============================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/systemdrivers/vcs/image/mss/hw/sdm670/VCSBSP.c#1 $

==============================================================================
            Copyright (c) 2017 Qualcomm Technologies Incorporated.
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
#include "VCSLDORailImpl.h"
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


/*
 * VCSLDOCornerFuse_MSSQ6
 *
 * Set of corner/fuse pairs that determine enablement of the MSS Q6 eLDO.
 */
static VCSLDOCornerFuseType VCSLDOCornerFuse_MSSQ6[] =
{
  { RAIL_VOLTAGE_LEVEL_MIN_SVS, 0, 0 }, // fill in fuse data when available from PTE
  { RAIL_VOLTAGE_LEVEL_LOW_SVS, 0, 0 }, // fill in fuse data when available from PTE
  { RAIL_VOLTAGE_LEVEL_SVS,     0, 0 }, // fill in fuse data when available from PTE
  { RAIL_VOLTAGE_LEVEL_SVS_L1,  0, 0 }, // fill in fuse data when available from PTE
};


/*
 * VCSLDORegisterSettings_MSSQ6_CPU_Standalone
 *
 * Set of register/value pairs that constitute the hardware settings.
 */
static HAL_vcs_RegisterSettingType VCSLDORegisterSettings_MSSQ6_Standalone[] =
{
  { HWIO_ADDR(MSS_QDSP6SS_LDO_USER),     0x0 },
  { HWIO_ADDR(MSS_QDSP6SS_LDO_CFG0),     0x0 },
  { HWIO_ADDR(MSS_QDSP6SS_LDO_CFG1),     0x0 },
  { HWIO_ADDR(MSS_QDSP6SS_LDO_CFG2),     0x0 },
  { HWIO_ADDR(MSS_QDSP6SS_LDO_LD_CTL),   0x0 },
  { HWIO_ADDR(MSS_QDSP6SS_LDO_TRIM_CTL), 0x0 },
};


/*
 * VCSLDOConfigs_MSSQ6
 *
 * Set of configurations for the MSS Q6 eLDO.
 *
 */
static VCSLDOConfigType VCSLDOConfigs_MSSQ6[] =
{
  {
    .eMode      = VCS_LDO_MODE_STANDALONE,
    .bEnable    = FALSE,
    .eCornerMin = RAIL_VOLTAGE_LEVEL_MIN_SVS,
    .eCornerMax = RAIL_VOLTAGE_LEVEL_NOM,
    .HALConfig  = { VCS_DEF_ARR_FIELD(Settings, VCSLDORegisterSettings_MSSQ6_Standalone) },
    .HWVersion  = { {0x00, 0x00}, {0xFF, 0xFF} }
  },
};


/*
 * VCSLDORailImpl_MSSQ6
 *
 * Implementation data for MSS Q6.
 */
static VCSLDORailImplType VCSLDORailImpl_MSSQ6 =
{

  VCS_DEF_ARR_FIELD(Configs, VCSLDOConfigs_MSSQ6),
  VCS_DEF_ARR_FIELD(CornerFuses, VCSLDOCornerFuse_MSSQ6),

  .HALdesc =
  {
    .nAddr = HWIO_ADDR(MSS_QDSP6SS_VERSION),
    .pmControl = &HAL_vcs_QDSP6v65LDOControl
  },
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
    VCS_DEF_RAIL_IMPL(LDO, &VCSLDORailImpl_MSSQ6),
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
