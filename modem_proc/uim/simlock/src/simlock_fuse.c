/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M L O C K   F U S E   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains fuse related utility functions that are to
  read or blow the SimLock fuse.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_fuse.c#7 $$ $DateTime: 2021/04/06 14:23:00 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/05/21   tq      Simlock fuse support for Divar chipset
12/09/20   shp     Simlock fuse support for Atherton chipset
09/09/20   vgd     Simlock fuse support for qtang2 chipset
02/14/20   vdc     Simlock fuse support for Agatti chipset
10/04/19   sch     Simlock fuse support for 8953 chipset
10/03/19   sch     Simlock fuse support for kamorta chipset  
05/02/19   bcho    Simlock fuse support for Rennell
11/13/18   bcho    Simlock fuse support for Nicobar
05/11/18   bcho    Simlock fuse support for SM6150/SM7150
08/16/17   bcho    Simlock fuse support for SDM670
02/24/17   av      Simlock fuse support for sdx50m, sdx20m and sdx24
09/28/16   bcho    Simlock fuse support for SDM660 and SDM630
06/16/16   av      Postpone creation of simlock config files
06/13/16   av      Bring back simlock fuse support for 8998
04/20/16   rb      Fix 8998 compilation error until core fix is available
03/21/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "simlock_platform.h"
#include "simlock_common.h"
#include "simlock_file.h"
#include "IxErrno.h"
#include "secboot_util.h"
#include "qfprom.h"
#include "HALhwio.h"
#include "DALPropDef.h" /* For DAL_CONFIG_PROC_MPSS definition */

/* Note regarding SIMLOCK_FUSE_BITMASK defined below:
   Bit reserved for Simlock in the MSA AntiRollback region...
   On 8998/8997, AR4 region is reserved for MSA AntiRollback whereas
   on 9x55/9x65, AR3 region is reserved for MSA AntiRollback.
   On 8998,8997,9x55,9x65 the bit reserved for SimLock is the most
   significant bit of the most significant byte of respective AR regions */
#ifdef FEATURE_UIM_MSM8998_HWIO_MACROS     
  #include "simlock_hwio_macros_8998.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_MSM8997_HWIO_MACROS
  #include "simlock_hwio_macros_8997.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_MDM9x55_HWIO_MACROS
  #include "simlock_hwio_macros_9x55.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_3_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_3_MSB, SIMLOCK)
#elif defined FEATURE_UIM_MDM9x65_HWIO_MACROS
  #include "simlock_hwio_macros_9x65.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_3_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_3_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SDM660_HWIO_MACROS
  #include "simlock_hwio_macros_sdm660.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SDM630_HWIO_MACROS
  #include "simlock_hwio_macros_sdm630.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SDM845_HWIO_MACROS
  #include "simlock_hwio_macros_sdm845.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SDX50M_HWIO_MACROS
  #include "simlock_hwio_macros_sdx50m.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_3_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_3_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SDX20M_HWIO_MACROS
  #include "simlock_hwio_macros_sdx20m.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_3_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_3_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SDX24_HWIO_MACROS
  #include "simlock_hwio_macros_sdx24.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SDM670_HWIO_MACROS
  #include "simlock_hwio_macros_sdm670.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SM6150_HWIO_MACROS
  #include "simlock_hwio_macros_sm6150.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_SM7150_HWIO_MACROS
  #include "simlock_hwio_macros_sm7150.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_NICOBAR_HWIO_MACROS
  #include "simlock_hwio_macros_nicobar.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_RENNELL_HWIO_MACROS
  #include "simlock_hwio_macros_rennell.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_KAMORTA_HWIO_MACROS
  #include "simlock_hwio_macros_kamorta.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_MSM8953_HWIO_MACROS
  #include "simlock_hwio_macros_8953.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_3_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_3_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_3_MSB, SIMLOCK)
#elif defined FEATURE_UIM_AGATTI_HWIO_MACROS
  #include "simlock_hwio_macros_agatti.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_QTANG2_HWIO_MACROS
  #include "simlock_hwio_macros_qtang2.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_ATHERTON_HWIO_MACROS
  #include "simlock_hwio_macros_atherton.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#elif defined FEATURE_UIM_DIVAR_HWIO_MACROS
  #include "simlock_hwio_macros_divar.h"
  #define SIMLOCK_FUSE_BITMASK                    HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_MSB_SIMLOCK_BMSK
  #define SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS       HWIO_QFPROM_RAW_ANTI_ROLLBACK_4_LSB_PHYS
  #define SIMLOCK_IS_FUSE_BLOWN                   HWIO_INF(ANTI_ROLLBACK_4_1, SIMLOCK)
  #define SIMLOCK_IS_CORR_FUSE_BLOWN              HWIO_INF(QFPROM_CORR_ANTI_ROLLBACK_4_MSB, SIMLOCK)
#else
  #error "SimLock: hwio macros file for the target not found!"
#endif

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* Set reserved HW Mutex ID for Security use. This is an agreement between
   apps/tz and modem... Currently this HW mutex is being used to blow
   antirollback fuses by modem_sec and tz. '8' is a value agreed upon for
   that purpose across modem and tz. There is no public header for this
   hence hardcoding it here (just like it is hardcoded in modem_sec and
   tz code)*/
#define SIMLOCK_MODEM_HW_MUTEX_ID               8

/* Set Processor ID to 2 to indicate reqest from Modem */
#define SIMLOCK_MODEM_HW_MUTEX_PID              DAL_CONFIG_PROC_MPSS

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_FUSE_GRAB_HW_MUTEX

DESCRIPTION
  Function called to grab a hardware mutex protecting the AntiRollback region
  registers.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None
===========================================================================*/
static void simlock_fuse_grab_hw_mutex
(
  void
)
{
  do
  {
    /* Attempt to write Processor ID */
    HWIO_OUTI( TCSR_MUTEX_REG_n, SIMLOCK_MODEM_HW_MUTEX_ID, SIMLOCK_MODEM_HW_MUTEX_PID );

    /* Check if PID was written (i.e. the hardware mutex is locked) */
  } while( HWIO_INI( TCSR_MUTEX_REG_n, SIMLOCK_MODEM_HW_MUTEX_ID ) != SIMLOCK_MODEM_HW_MUTEX_PID );
} /* simlock_fuse_grab_hw_mutex */


/*===========================================================================
FUNCTION SIMLOCK_FUSE_RELEASE_HW_MUTEX

DESCRIPTION
  Function called to release a hardware mutex protecting the AntiRollback
  region registers.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None
===========================================================================*/
static void simlock_fuse_release_hw_mutex
(
  void
)
{
  /* Unlock the hardware mutex by writing 0 */
  HWIO_OUTI( TCSR_MUTEX_REG_n, SIMLOCK_MODEM_HW_MUTEX_ID, 0 );
} /* simlock_fuse_release_hw_mutex */


/*===========================================================================
FUNCTION SIMLOCK_FUSE_IS_FUSE_BLOWN

DESCRIPTION
  Function called to find out if the SIMLOCK fuse is blown or not.

DEPENDENCIES
  This routine should be called at boot up only. The underlying register
  contains fuses currently blown in actual QFPROM, at power up. If after
  a device is powered up, and a fuse bit is blown in QFPROM, this underlying
  register would be unaware of that until next boot up.

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean simlock_fuse_is_fuse_blown
(
  void
)
{
  uint32 auth_enabled                  = 0;


  if (E_SUCCESS != secboot_util_hw_is_auth_enabled(SECBOOT_UTIL_HW_MSS_CODE_SEGMENT, &auth_enabled) ||
      !auth_enabled)
  {
    /* We return SUCCESS here because we only guarantee SimLock's robustness
       if secure boot is enabled. If it is not enabled, we don't worry about
       blowing the simlock fuse */
    SIMLOCK_MSG_HIGH_0("SecureBoot for modem is not enabled! So, ignoring the SimLock fuse status.");
    return FALSE;
  }

  if(SIMLOCK_IS_FUSE_BLOWN)
  {
    SIMLOCK_MSG_HIGH_0("Simlock fuse is blown!");
    return TRUE;
  }

  SIMLOCK_MSG_HIGH_0("Simlock fuse not blown!");
  return FALSE;
} /* simlock_fuse_is_fuse_blown */


/*===========================================================================
FUNCTION SIMLOCK_FUSE_BLOW_FUSE

DESCRIPTION
  Function called to blow the SIMLOCK fuse that is in the MSA accessible
  AntiRollback region.

DEPENDENCIES
  None

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
simlock_result_enum_type simlock_fuse_blow_fuse
(
  void
)
{
  uint32 auth_enabled                  = 0;
  uint32 ret_code                      = QFPROM_NO_ERR;
  /* In row_data[1], we only set our SimLock fuse bit... rest of the bits of
     row_data can be left as zero, without worrying about the existing values
     of those bits in QFPROM... the reason is that if those bits are 1
     (blown already) in QFPROM, they will never get set to 0 (unblown) even
     if try to write 0 as their values via the qfprom write row API */
  uint32 row_data[2]                   = {0, SIMLOCK_FUSE_BITMASK};
  uint32 simlock_fuse_raw_row_address  = (uint32) SIMLOCK_FUSE_RAW_ROW_ADDRESS_PHYS;

  /* Find out if AUTH_EN fuse for MSS in SecureBoot region is blown or not,
     that is, if MSS image authentication is enabled or not. Unless that is
     enabled, we cannot guarantee a robust SimLock solution and hence no
     need to blow the fuse */
  if (E_SUCCESS != secboot_util_hw_is_auth_enabled(SECBOOT_UTIL_HW_MSS_CODE_SEGMENT, &auth_enabled) ||
      !auth_enabled)
  {
    /* We return SUCCESS here because we only guarantee SimLock's robustness
       if secure boot is enabled. If it is not enabled, we don't worry about
       blowing the simlock fuse */
    SIMLOCK_MSG_HIGH_0("SecureBoot for modem is not enabled! Not blowing SimLock fuse.");
    return SIMLOCK_SUCCESS;
  }

  /* GRAB MUTEX */
  simlock_fuse_grab_hw_mutex();

  do
  {
    /* Read CORR fuse value */
    if(SIMLOCK_IS_CORR_FUSE_BLOWN)
    {
      /* Fuse is already blown */
      SIMLOCK_MSG_HIGH_0("Simlock fuse already blown");
      break;
    }

    /* RELEASE MUTEX */
    simlock_fuse_release_hw_mutex();

    /* We need to blow the fuse... we are here because we have successfully
       updated the simlock config in the EFS, after which we must ensure to
       perform a efs_sync() before we consider the simlock config update as
       complete and we proceed with blowing the fuse */
    simlock_file_commit_to_fs_if_needed();

    /* Now acquire the HW crit sect and blow the fuse.
       GRAB MUTEX */
    simlock_fuse_grab_hw_mutex();

    /* Blow the fuse */
    ret_code = qfprom_write_row(simlock_fuse_raw_row_address,
                                row_data,
                                0);
    SIMLOCK_MSG_HIGH_1("SimLock fuse blowing status = 0x%x", ret_code);
  } while(0);

  /* RELEASE MUTEX */
  simlock_fuse_release_hw_mutex();

  return (ret_code == QFPROM_NO_ERR) ? SIMLOCK_SUCCESS : SIMLOCK_GENERIC_ERROR;
} /* simlock_fuse_blow_fuse */

#endif /* FEATURE_SIMLOCK */
