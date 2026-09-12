/*
==============================================================================

FILE:         VCSBSP.c

DESCRIPTION:
  This file contains VCS BSP data for DAL based driver.

==============================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/systemdrivers/vcs/image/mss/hw/nicobar/VCSBSP.c#1 $

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
#include "VCSRPMRailImpl.h"
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
 * VCSRPMhRailImpl_CX
 */
static VCSRPMRailImplType VCSRPMRailImpl_CX =
{
  .szLocalName = "/vcs/vdd_cx/vlvl",
  .szRemoteName = "rwcx\x00\x00\x00\x00",
  .szUnits = "vlvl",
};

static VCSRPMRailImplType VCSRPMRailImpl_MX =
{
  .szLocalName = "/vcs/vdd_mx/vlvl",
  .szRemoteName = "rwmx\x00\x00\x00\x00",
  .szUnits = "vlvl",
};

/*
 * aRails
 *
 * Set of rail instances.
 */
static VCSRailNodeType aRails[] =
{
  /*-----------------------------------------------------------------------*/
  /* CX                                                                    */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_cx"),
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_cx/vlvl"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPM, &VCSRPMRailImpl_CX),
  },
  
  /*-----------------------------------------------------------------------*/
  /* MX                                                                    */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mx"),
    VCS_DEF_RAIL_DEP_NAME("/vcs/vdd_mx/vlvl"),
    VCS_DEF_RAIL_ATTRIBUTES(NPA_RESOURCE_DEFAULT),
    VCS_DEF_RAIL_IMPL(RPM, &VCSRPMRailImpl_MX),
  },
  
   /*-----------------------------------------------------------------------*/
  /* MSS                                                                    */
  /*-----------------------------------------------------------------------*/
  {
    VCS_DEF_RAIL_NAME("/vcs/vdd_mss_q6"),
    VCS_DEF_ALIAS("/vcs/vdd_cx"),
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
