#ifndef MCALTDS_MCPMDRV_H
#define MCALTDS_MCPMDRV_H

/*============================================================================*/
/** @file
 * This module has definition and declaration related to MCAL UMTS downlink
 * channel setup module APIs
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2007 - 2011 QUALCOMM Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_mcpmdrv.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/03/10   weiz   Create new MCPM API driver in tdscdma

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/
/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#ifdef FEATURE_MCPM
#include "mcpm_api.h"
#endif
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Call back function type declaration */
/* ----------------------------------- */

/* Enum type declaration */
/* --------------------- */

/** This enumerates the various FW physical channel types */
/* Enumeration detailing the different TDSCDMA PWR driver clients */
#define MCALTDS_MCPMDRV_STACK_ACTIVE             0x0001
#define MCALTDS_MCPMDRV_ACQ_ACTIVE               0x0002
#define MCALTDS_MCPMDRV_IDLE_OR_DL_ACTIVE        0x0004
#define MCALTDS_MCPMDRV_WAKE_UP                  0x0008
#define MCALTDS_MCPMDRV_UL_ACTIVE                0x0010
#define MCALTDS_MCPMDRV_HSDPA                    0x0020
#define MCALTDS_MCPMDRV_HSUPA                    0x0040
#define MCALTDS_MCPMDRV_TURBO                    0x0080
#define MCALTDS_MCPMDRV_GO_TO_SLEEP              0x0100
#define MCALTDS_MCPMDRV_WAKEUP_UPDATE            0x0200
#define MCALTDS_MCPMDRV_INIT                     0x0400
#define MCALTDS_MCPMDRV_RXD                      0x1000
#define MCALTDS_MCPMDRV_T2G                      0x2000
#define MCALTDS_MCPMDRV_T2L                      0x4000
#define MCALTDS_MCPM_CLIENT_NO_ACTION            0x0000

typedef uint16 mcaltds_mcpmdrv_client_type;


/* Structure type declaration */
/* -------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
extern boolean mcaltds_mcpmdrv_decoder_status;

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*============================================================================*/
/**
  * This validates the rl set physical channel with specified parameters.
  * Returns result of validation
  */
/*============================================================================*/

/*===========================================================================
FUNCTION     MCALTDS_MCPMDRV_REQUEST_POWER_CONFIG

DESCRIPTION
  Wrapper function for TDSCDMA MCPM clients to request for power resourcess.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcpmdrv_request_power_config(mcaltds_mcpmdrv_client_type req_client);

/*===========================================================================
FUNCTION     MCALTDS_MCPMDRV_RELINQUISH_POWER_CONFIG

DESCRIPTION
  Wrapper function for W MCPM clients to relinquish power resources.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void mcaltds_mcpmdrv_relinquish_power_config(mcaltds_mcpmdrv_client_type rel_client);

/*===========================================================================
FUNCTION     mcaltds_mcpmdrv_get_t2g_client_status

DESCRIPTION
  Returns the status of T2G client from the internal client bit mask

PARAMETERS
  None.

RETURN VALUE
  boolean (TRUE - if T2G client is active / FALSE - otherwise)

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcpmdrv_get_t2g_client_status(void);

/*===========================================================================
FUNCTION     mcaltds_mcpmdrv_is_IDLE_OR_DL_ACTIVE

DESCRIPTION
  Returns whether IDLE_OR_DL_ACTIVE

PARAMETERS
  None.

RETURN VALUE
  boolean (TRUE - if IDLE_OR_DL_ACTIVE / FALSE - otherwise)

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcpmdrv_is_IDLE_OR_DL_ACTIVE(void);

/*===========================================================================
FUNCTION     mcaltds_mcpmdrv_is_INIT

DESCRIPTION
  Returns whether in MCALTDS_MCPMDRV_INIT state

PARAMETERS
  None.

RETURN VALUE
  boolean (TRUE - if MCALTDS_MCPMDRV_INIT / FALSE - otherwise)

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcpmdrv_is_INIT(void);

/*===========================================================================
FUNCTION     mcaltds_mcpmdrv_is_upa_dpa_active

DESCRIPTION
  Returns whether both HSDPA and HSUPA are active

PARAMETERS
  None.

RETURN VALUE
  boolean (TRUE - if both HSDPA and HSUPA are active / FALSE - otherwise)

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcpmdrv_is_upa_dpa_active(void);

/*===========================================================================
FUNCTION     mcaltds_mcpmdrv_is_data_active

DESCRIPTION
  Returns whether HSDPA or HSUPA or Turbo are active

PARAMETERS
  None.

RETURN VALUE
  boolean (TRUE - if HSDPA or HSUPA or Turbo are active / FALSE - otherwise)

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcpmdrv_is_data_active(void);


/*===========================================================================
FUNCTION     mcaltds_mcpmdrv_is_UL_active

DESCRIPTION
  Returns whether UL is active

PARAMETERS
  None.

RETURN VALUE
  boolean (TRUE - if UL is active / FALSE - otherwise)

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean mcaltds_mcpmdrv_is_UL_active(void);

/*===========================================================================
FUNCTION     MCALTDS_MCPMDRV_GPIO_PROFILE_TOGGLE

DESCRIPTION
  Wrapper function for TDSCDMA MCPM clients for gpio profile strobe toggle.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
#ifdef FEATURE_TDSCDMA_POWER_GPIO_TOGGLE

extern void mcaltds_mcpmdrv_gpio_profile_pwr_strobe_toggle(void);

#else
#define mcaltds_mcpmdrv_gpio_profile_pwr_strobe_toggle() /* do nothing */
#endif


#ifdef FEATURE_MCPM
#define MCALTDS_MCPM_ENABLE_DOB_TDS_MODE()           HWIO_OUT(DEM_SYM_BUFF_TDS_MODE, 0x01); \
                                                     HWIO_OUT(DECODER_TDS_MODE, 0x01);\
                                                     mcaltds_mcpmdrv_decoder_status = TRUE

#define MCALTDS_MCPM_DISABLE_DOB_TDS_MODE()          HWIO_OUT(DEM_SYM_BUFF_TDS_MODE, 0x00); \
                                                     HWIO_OUT(DECODER_TDS_MODE, 0x00);\
                                                     mcaltds_mcpmdrv_decoder_status = FALSE
#else
#define MCALTDS_MCPM_ENABLE_DOB_TDS_MODE()           mcaltds_mcpmdrv_decoder_status = TRUE

#define MCALTDS_MCPM_DISABLE_DOB_TDS_MODE()          mcaltds_mcpmdrv_decoder_status = FALSE
#endif

#endif
