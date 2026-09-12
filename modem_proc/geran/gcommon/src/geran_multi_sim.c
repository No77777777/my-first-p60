/*! \file geran_multi_sim.c

  Provides common functions for use by GERAN entities supporting Multi SIM.

                Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/gcommon/src/geran_multi_sim.c#3 $ */
/* $DateTime: 2020/01/27 06:21:59 $ */
/* $Author: pwbldsvc $ */

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"
#include "geran_multi_sim_api.h"
#include "gpl1_grm_intf.h"
#include <stringl.h>
#include "trm.h"
#include "geran_nv.h"
#include "cm.h"
#include "rex.h"
#include "gs.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif
/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/
#define GERAN_NUM_GTA_VETO_CLIENTS 10/* number of different clients allowed to vote against GPRS Tuneaway */

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

typedef struct
{
  /* array of votes, one per client */
  boolean client_vote[GERAN_NUM_GTA_VETO_CLIENTS];
} gta_veto_t;

typedef struct
{
  geran_exchange_direction_t direction;
  grm_client_enum_t          src_rx_client;
  grm_client_enum_t          dst_rx_client;
  grm_client_enum_t          src_tx_client;
  grm_client_enum_t          dst_tx_client;
} geran_exchange_info_t;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 *--------------------------------------------------------------------------*/
static rex_crit_sect_type gta_lock_crit_sec;
static rex_crit_sect_type gcmn_xchange_dir_crit_sec;
static gta_veto_t         gta_veto_status[NUM_GERAN_DATA_SPACES];

#define GTA_VOTE_LOCK()   rex_enter_crit_sect(&gta_lock_crit_sec)
#define GTA_VOTE_UNLOCK() rex_leave_crit_sect(&gta_lock_crit_sec)

#define GCMN_EXCHANGE_LOCK()       rex_enter_crit_sect(&gcmn_xchange_dir_crit_sec)
#define GCMN_EXCHANGE_UNLOCK()     rex_leave_crit_sect(&gcmn_xchange_dir_crit_sec)

static geran_traffic_reason_t geran_traffic_reason[NUM_GERAN_DATA_SPACES] = {0};

#ifdef FEATURE_TRIPLE_SIM
/* TSTS - three Layer 1 task queues and CxM IDs exist */
static const gs_queue_id_T gl1_queue_ids[NUM_GERAN_DATA_SPACES] = {GS_QUEUE_LAYER_1,
                                                                   GS_QUEUE_LAYER_1_2,
                                                                   GS_QUEUE_LAYER_1_3};
static const cxm_tech_type geran_cxm_ids[NUM_GERAN_DATA_SPACES] = {CXM_TECH_GSM1, CXM_TECH_GSM2, CXM_TECH_GSM3};
#else
#ifdef FEATURE_DUAL_SIM
/* DSDS - two Layer 1 task queues and CxM IDs exist */
static const gs_queue_id_T gl1_queue_ids[NUM_GERAN_DATA_SPACES] = {GS_QUEUE_LAYER_1,
                                                                   GS_QUEUE_LAYER_1_2};
static const cxm_tech_type geran_cxm_ids[NUM_GERAN_DATA_SPACES] = {CXM_TECH_GSM1, CXM_TECH_GSM2};
#else
/* SSSS - one Layer 1 task queue and CxM ID exists */
static const gs_queue_id_T gl1_queue_ids[NUM_GERAN_DATA_SPACES] = {GS_QUEUE_LAYER_1};
static const cxm_tech_type geran_cxm_ids[NUM_GERAN_DATA_SPACES] = {CXM_TECH_GSM1};
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_TRIPLE_SIM */

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
static sys_modem_dual_standby_pref_e_type geran_sys_standby_pref;
static sys_modem_device_mode_e_type       geran_sys_device_mode;
static sys_modem_device_mode_e_type       geran_multi_sim_mode;
static sys_subs_feature_t                 geran_sys_subs_feature;

/* Storage for superset of subscription capabilities */
/* N.B. not the same as "RAT Priority List" contents */
static uint32 geran_subscription_capabilities[MAX_AS_IDS];

static trm_grant_return_enum_type         geran_chain_exchanged[NUM_GERAN_DATA_SPACES][2];
static geran_exchange_direction_t         geran_exchange_on_release[NUM_GERAN_DATA_SPACES];

#define IS_SINGLE_STANDBY_MODE(dm) (SYS_MODEM_DEVICE_MODE_SINGLE_SIM == (dm))
#define IS_DUAL_STANDBY_MODE(dm) (SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY == (dm))
#define IS_DUAL_ACTIVE_MODE(dm) (SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE == (dm))
#define IS_DSDX_DEVICE_MODE(dm) (IS_DUAL_STANDBY_MODE(dm) || IS_DUAL_ACTIVE_MODE(dm))

#ifdef FEATURE_TRIPLE_SIM
#define IS_TRIPLE_STANDBY_MODE(dm) (SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY == (dm))
#define IS_MULTI_STANDBY_MODE(dm)  (IS_DUAL_STANDBY_MODE(dm) || IS_TRIPLE_STANDBY_MODE(dm))
/* mapping between GAS ID and ASID is now statically defined at compile time */
static const gas_id_t                     as_id_to_gas_id[NUM_GERAN_DATA_SPACES] = {
  GERAN_ACCESS_STRATUM_ID_1, GERAN_ACCESS_STRATUM_ID_2, GERAN_ACCESS_STRATUM_ID_3}; /* ASID to GAS ID mapping */
static const sys_modem_as_id_e_type       gas_id_to_as_id[NUM_GERAN_DATA_SPACES] = {
  SYS_MODEM_AS_ID_1,         SYS_MODEM_AS_ID_2,         SYS_MODEM_AS_ID_3};         /* GAS ID to ASID mapping */
#else
#define IS_TRIPLE_STANDBY_MODE(dm) (FALSE)
#define IS_MULTI_STANDBY_MODE(dm)  IS_DUAL_STANDBY_MODE(dm)
/* mapping between GAS ID and ASID is now statically defined at compile time */
static const gas_id_t                     as_id_to_gas_id[NUM_GERAN_DATA_SPACES] = {
  GERAN_ACCESS_STRATUM_ID_1, GERAN_ACCESS_STRATUM_ID_2}; /* ASID to GAS ID mapping */
static const sys_modem_as_id_e_type       gas_id_to_as_id[NUM_GERAN_DATA_SPACES] = {
  SYS_MODEM_AS_ID_1,         SYS_MODEM_AS_ID_2};         /* GAS ID to ASID mapping */
#endif /* FEATURE_TRIPLE_SIM */

#ifdef FEATURE_LTE
static uint32 geran_lte_activity_status[NUM_GERAN_DATA_SPACES];
#endif /* FEATURE_LTE */

static sys_modem_as_id_e_type geran_dds_as_id;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *--------------------------------------------------------------------------*/
static uint32 geran_get_default_subscription_capability(void)
{
  uint32 subs_cap_mask;

  /* initialise the mask using a special bit that */
  /* can be used to infer the defaults are in use */
  subs_cap_mask = 1UL << (uint32)SYS_SYS_MODE_GWL;

  subs_cap_mask |= (uint32)SYS_SYS_MODE_MASK_GSM;
#ifdef FEATURE_WCDMA
  subs_cap_mask |= (uint32)SYS_SYS_MODE_MASK_WCDMA;
#endif /* FEATURE_WCDMA */
#ifdef FEATURE_GSM_TDS
  subs_cap_mask |= (uint32)SYS_SYS_MODE_MASK_TDS;
#endif /* FEATURE_GSM_TDS */
#ifdef FEATURE_LTE
  subs_cap_mask |= (uint32)SYS_SYS_MODE_MASK_LTE;
#endif /* FEATURE_LTE */
#ifdef FEATURE_CDMA
  subs_cap_mask |= (uint32)SYS_SYS_MODE_MASK_CDMA;
#endif /* FEATURE_CDMA */
#ifdef FEATURE_HDR
  subs_cap_mask |= (uint32)SYS_SYS_MODE_MASK_HDR;
#endif /* FEATURE_HDR */

  return(subs_cap_mask);
}

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)

/*!
 * \brief Returns the equivalent AS-ID
 *
 * \param gas_id
 *
 * \return sys_modem_as_id_e_type
 */
sys_modem_as_id_e_type ftm_sub(gas_id_t gas_id)
{
  return(geran_map_gas_id_to_nas_id(gas_id));
}

/*!
 * \brief Returns the equivalent GAS-ID
 *
 * \param as_id
 *
 * \return gas_id_t
 */
gas_id_t ftm_gas_id(sys_modem_as_id_e_type as_id)
{
  return(geran_map_nas_id_to_gas_id(as_id));
}

/*!
 * \brief Returns the multi-SIM mode.
 *
 * \param device_mode
 * \param standby_pref
 *
 * \return sys_modem_device_mode_e_type
 */
static sys_modem_device_mode_e_type determine_multi_sim_mode(
  sys_modem_device_mode_e_type device_mode,
  sys_modem_dual_standby_pref_e_type standby_pref
)
{
  sys_modem_device_mode_e_type multi_sim_mode = device_mode;

  // If SS, then return SS
  // If DSDS, then return SS/DSDS according to upper layer SS/DS indication
  // If DSDA, then return DSDA
  if (IS_MULTI_STANDBY_MODE(device_mode))
  {
    if (SYS_MODEM_DS_PREF_SINGLE_STANDBY == standby_pref)
    {
      multi_sim_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
    }
  }
  else if (IS_DUAL_ACTIVE_MODE(device_mode))
  {
    if ((SYS_MODEM_DS_PREF_SINGLE_STANDBY == standby_pref) &&
         ((geran_sys_subs_feature == SYS_SUBS_FEATURE_MODE_SGLTE) ||
          (geran_sys_subs_feature == SYS_SUBS_FEATURE_MODE_SVLTE) ||
          (geran_sys_subs_feature == SYS_SUBS_FEATURE_MODE_SRLTE)))
    {
      multi_sim_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
    }
  }

  MSG_GERAN_HIGH_3("device mode=%d, standby_pref=%d => multi_sim_mode=%d)",
                   (int)device_mode,
                   (int)standby_pref,
                   (int)multi_sim_mode);

  return(multi_sim_mode);
}

/*!
 * \brief  Reset all inter-RAT lock exchange clients
 *
 * \param  Multi-SIM mode and GERAN Access Stratum Identifier
 *
 * \return None
 */
static void geran_reset_lock_exchange_clients(const sys_modem_device_mode_e_type multi_sim_mode, const gas_id_t gas_id)
{
  if (IS_SINGLE_STANDBY_MODE(multi_sim_mode))
  {
    gas_id_t lxir_id;

    /* for SSSS, abandon X2G lock exchange across all subscriptions now */
    for (lxir_id = GERAN_ACCESS_STRATUM_ID_1; lxir_id <= GERAN_ACCESS_STRATUM_ID_MAX; lxir_id++)
    {
      geran_abandon_lock_exchange(GERAN_EXCHANGE_XTOG, lxir_id);
    }
  }
  else
  {
    /* abandon X2G client usage for the specified subscription only */
    geran_abandon_lock_exchange(GERAN_EXCHANGE_XTOG, gas_id);
  }

  return;
}
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*!
 * \brief Retrieves the system mode capabilities for the given GAS ID
 *
 * \param GERAN Access Stratum Identifier
 *
 * \return uint32 bitmask of sys_sys_mode_mask_e_type values
 */
uint32 geran_retrieve_subscription_capabilities(const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
    int as_index = (int)geran_map_gas_id_to_nas_id(gas_id);

    if ((as_index >= 0) && (as_index < MAX_AS_IDS))
    {
      return(geran_subscription_capabilities[as_index]);
    }
#else
    /* Single SIM build - just return the feature-based default */
    return(geran_get_default_subscription_capability());
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  }

  return((uint32)SYS_SYS_MODE_MASK_NONE);
}

/*!
 * \brief Stores the system mode capabilities for each ASID
 *
 * \param capability_ptr - array of (uint32) bitmasks
 *
 * \return void
 */
void geran_store_subscription_capabilities(uint32 *capability_ptr)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (capability_ptr != NULL)
  {
    memscpy((void *)geran_subscription_capabilities,
            sizeof(geran_subscription_capabilities),
            (void *)capability_ptr,
            MAX_AS_IDS * sizeof(uint32));
  }
  else
  {
    int i;

    for (i = 0; i < MAX_AS_IDS; i++)
    {
      /* restore each subscription entry with the feature-based default */
      geran_subscription_capabilities[i] = geran_get_default_subscription_capability();
    }
  }
#else
  NOTUSED(capability_ptr);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return;
}

/*!
 * \brief Checks subscription capabilities for support for a particular tech
 *
 * \param sys_mask - sys_sys_mode_mask_e_type e.g. SYS_SYS_MODE_MASK_CDMA
 * \param gas_id   - GERAN Access Stratum Identifier
 *
 * \return TRUE if the requested tech is supported by the specified subscription
 */
boolean geran_subscription_capability_includes_sys_mask(const sys_sys_mode_mask_e_type sys_mask, const gas_id_t gas_id)
{
  if (geran_retrieve_subscription_capabilities(gas_id) & (uint32)sys_mask)
  {
    return(TRUE);
  }

  return(FALSE);
}

/*!
 * \brief Returns GL1 task queue id based on GAS ID
 *
 * \param gas_id
 *
 * \return gs_queue_id_T
 */

gs_queue_id_T geran_get_gl1_queue_id(const gas_id_t gas_id)
{
#ifndef PERLUTF
  return(gl1_queue_ids[geran_get_data_space_index(gas_id)]);
#else
  #error code not present
#endif /* !PERLUTF */
}

/*!
 * \brief Returns GL1 task queue id based on ASID
 *
 * \param as_id
 *
 * \return gs_queue_id_T
 */

gs_queue_id_T geran_api_get_l1_queue_id(const sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  return(geran_get_gl1_queue_id(gas_id));
}

/*-----------------------------------------------------------------------------
 * Externalised Function Definitions
 *---------------------------------------------------------------------------*/

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
/*!
 * \brief Sets the the multimode subs feature
 *
 * \Input geran_sys_subs_feature
 * returns Void
 */
void geran_set_multimode_subs_feature(sys_subs_feature_t geran_subs_feature)
{
  geran_sys_subs_feature = geran_subs_feature;
  MSG_GERAN_HIGH_1("Sys subs feature set to %d", (int)geran_sys_subs_feature);
  return;
}
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*!
 * \brief Indicates if the given GAS-ID represents a multimode subscription.
 *
 * \param gas_id
 *
 * \return boolean - TRUE if this is a multimode sub, FALSE otherwise.
 */
boolean geran_is_multi_mode_gas_id(gas_id_t gas_id)
{
  /* all valid GAS IDs are considered to be multimode capable */
  return(gas_id <= GERAN_ACCESS_STRATUM_ID_MAX);
}

/*!
 * \brief Converts a GERAN access stratum ID to a NAS ID.
 *
 * \param gas_id (in)
 *
 * \return sys_modem_as_id_e_type
 */
sys_modem_as_id_e_type geran_map_gas_id_to_nas_id(gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    return gas_id_to_as_id[gas_id];
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(SYS_MODEM_AS_ID_1);
}

/*!
 * \brief Converts a NAS ID to a GERAN access stratum ID.
 *
 * \param as_id (in)
 *
 * \return gas_id_t
 */
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */
gas_id_t geran_map_nas_id_to_gas_id(sys_modem_as_id_e_type as_id)
{
  gas_id_t result = GERAN_ACCESS_STRATUM_ID_1;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (as_id != SYS_MODEM_AS_ID_NONE)
  {
    if (as_id < MAX_AS_IDS)
    {
      result = as_id_to_gas_id[as_id];
    }
  }
#else
  NOTUSED(as_id);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(check_gas_id(result));
}

/*===========================================================================

FUNCTION  geran_get_diag_sub_id

DESCRIPTION
  Convert GAS ID to diag subscription ID

RETURN VALUE
  Diag Subscription Id

SIDE EFFECTS
  None

===========================================================================*/
byte geran_get_diag_sub_id(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type nas_id;

  nas_id = geran_map_gas_id_to_nas_id(gas_id);
#ifdef FEATURE_TRIPLE_SIM
  if (SYS_MODEM_AS_ID_3 == nas_id)
  {
    return(3);
  }
#endif /* FEATURE_TRIPLE_SIM */

  if (SYS_MODEM_AS_ID_2 == nas_id)
  {
    return(2);
  }

  return(1);
}

/*===========================================================================

FUNCTION       geran_get_data_space_index

DESCRIPTION    Maps GAS-ID to data-space index
               Range check as_index to ensure no buffer overflow when it is used

DEPENDENCIES   None

RETURN VALUE   int - data-space index

SIDE EFFECTS   None

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */
int geran_get_data_space_index(const gas_id_t gas_id)
{
  int as_idx = GERAN_DATA_SPACE_INDEX_1;

  switch (check_gas_id(gas_id))
  {
    case GERAN_ACCESS_STRATUM_ID_1:
    {
      as_idx = GERAN_DATA_SPACE_INDEX_1;
      break;
    }
#ifdef FEATURE_DUAL_SIM
    case GERAN_ACCESS_STRATUM_ID_2:
    {
      as_idx = GERAN_DATA_SPACE_INDEX_2;
      break;
    }
#endif
#ifdef FEATURE_TRIPLE_SIM
    case GERAN_ACCESS_STRATUM_ID_3:
    {
      as_idx = GERAN_DATA_SPACE_INDEX_3;
      break;
    }
#endif
    default:
    {
      break;
    }
  }
  return as_idx;
}

/*!
 * \brief Sets the multi-SIM configuration.
 *
 * \param new_device_mode
 * \param new_standby_pref
 *
 * \return boolean - TRUE if the multi-SIM mode has changed, FALSE otherwise
 */
boolean geran_set_sys_multi_sim_config(
  const sys_modem_device_mode_e_type new_device_mode,
  const sys_modem_dual_standby_pref_e_type new_standby_pref
)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)

  sys_modem_device_mode_e_type new_multi_sim_mode;

  boolean multi_sim_mode_changed = FALSE;

  MSG_GERAN_HIGH_2("Multi-SIM device mode %d (was %d)",
                   (int)new_device_mode,
                   (int)geran_sys_device_mode);

  geran_sys_device_mode = new_device_mode;

  // Determine the GERAN multi-SIM mode from the new combination of device_mode + standby_pref
  new_multi_sim_mode = determine_multi_sim_mode(new_device_mode, new_standby_pref);

  if (new_multi_sim_mode != geran_multi_sim_mode)
  {
    geran_multi_sim_mode = new_multi_sim_mode;
    multi_sim_mode_changed = TRUE;
  }

  MSG_GERAN_HIGH_2("Multi-SIM standby pref %d (was %d)",
                   (int)new_standby_pref,
                   (int)geran_sys_standby_pref);

  geran_sys_standby_pref = new_standby_pref;

  return multi_sim_mode_changed;

#else

  NOTUSED(new_device_mode);
  NOTUSED(new_standby_pref);

  return FALSE;

#endif /* FEATURE_DUAL_SIM) || FEATURE_TRIPLE_SIM */
}

/*!
 * \brief Gets the multi-SIM preference.
 *
 * \return standby_pref
 */
sys_modem_dual_standby_pref_e_type geran_get_sys_multi_sim_pref(void)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  /* retrieve the stored preference */
  return(geran_sys_standby_pref);
#else
  return(SYS_MODEM_DS_PREF_SINGLE_STANDBY);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
}

/*!
 * \brief Returns the multi-SIM mode.
 *
 * \return sys_modem_device_mode_e_type
 */
sys_modem_device_mode_e_type geran_get_multi_sim_mode(void)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  return(geran_multi_sim_mode);
#else
  return(SYS_MODEM_DEVICE_MODE_SINGLE_SIM);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
}

/*!
 * \brief Converts sys_modem_as_id_e_type to integer
 *
 * \param as_id (in) - Subscription identifier
 *
 * \return Integer in range [0..NUM_GERAN_DATA_SPACES]
 */
static int geran_map_asid_to_index(const sys_modem_as_id_e_type as_id)
{
  int as_index;

  switch(as_id)
  {
    case SYS_MODEM_AS_ID_1:
      as_index = 0;
      break;

    case SYS_MODEM_AS_ID_2:
      as_index = 1;
      break;

    case SYS_MODEM_AS_ID_3:
      as_index = 2;
      break;

    default:
      as_index = NUM_GERAN_DATA_SPACES;
      break;
  }

  if (as_index >= NUM_GERAN_DATA_SPACES)
  {
    MSG_GERAN_ERROR_1("ASID %d is out of range", (int)as_id);
    as_index = NUM_GERAN_DATA_SPACES;
  }

  /* caller has responsibility to range-check the result */
  return(as_index);
}

/*!
 * \brief Indicates whether GPRS Tuneaway should be blocked on the specified AS ID
 *
 * \param as_id (in) - Subscription identifier
 *
 * \return TRUE if GPRS Tuneaway is vetod by one or more clients
 */
boolean geran_api_get_tuneaway_blocked(const sys_modem_as_id_e_type as_id)
{
  boolean now_blocked = FALSE;
  int     as_index;

  /* convert inbound ASID to data space index */
  as_index = geran_map_asid_to_index(as_id);

  /* sanity check the result */
  if (as_index < NUM_GERAN_DATA_SPACES)
  {
    int vote_index;

    /* acquire exclusive access to the voting table while it is queried */
    GTA_VOTE_LOCK();
    for (vote_index = 0; vote_index < GERAN_NUM_GTA_VETO_CLIENTS; vote_index++)
    {
      if (TRUE == gta_veto_status[as_index].client_vote[vote_index])
      {
        /* this client has voted against GPRS Tuneaway for this ASID */
        now_blocked = TRUE;
        break;
      }
    }
    GTA_VOTE_UNLOCK();
  }

  return(now_blocked);
}

/*!
 * \brief Maps a client_id from the enum into an integer array index
 *
 * \param client_id (in) - client from enumerated list
 *
 * \return int - position of client's vote in veto list
 */
static int geran_get_client_vote_index(const geran_api_client_id_t client_id)
{
  switch(client_id)
  {
  case GERAN_API_CLIENT_EDTM:
    return(0);
  case GERAN_API_CLIENT_MAC:
    return(1);
  case GERAN_API_CLIENT_GMM:
    return(2);
  case GERAN_API_CLIENT_PDP:
    return(3);
  case GERAN_API_CLIENT_RLC:
    return(4);
  case GERAN_API_CLIENT_GL1:
    return(5);
  case GERAN_API_CLIENT_G2X:
    return(6);
  case GERAN_API_CLIENT_CMUI:
    return(7);
  case GERAN_API_CLIENT_VSIM:
    return(8);
  case GERAN_API_CLIENT_PAGE:
    return(9);
  case GERAN_API_CLIENT_RESET:
    return(GERAN_NUM_GTA_VETO_CLIENTS);
  case GERAN_API_CLIENT_UNKNOWN:
  /* fallthrough*/
  default:
     break;
  }

  /* caller's responsibility to range check this return value */
  MSG_GERAN_ERROR_1("Unexpected GTA client ID %d", (int)client_id);
  return(GERAN_NUM_GTA_VETO_CLIENTS);
}

/*!
 * \brief Applies a vote for blocking GPRS Tuneaway on a specified ASID
 *
 * \param client_id (in)   - client changing their vote
 * \param gta_blocked (in) - the new vote
 * \param as_id (in)       - subscription identifier
 *
 * \return void
 */
void geran_api_set_tuneaway_blocked(const geran_api_client_id_t client_id, const boolean gta_blocked, const sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id;
  int      as_index;

  /* determine a GAS ID for logging purposes */
  gas_id = geran_map_nas_id_to_gas_id(as_id);

  /* convert inbound ASID to data space index */
  as_index = geran_map_asid_to_index(as_id);

  /* sanity check the result */
  if (as_index < NUM_GERAN_DATA_SPACES)
  {
    int vote_index;

    /* check for special reset case */
    if (GERAN_API_CLIENT_RESET == client_id)
    {
      /* acquire exclusive access to the voting table while it is updated */
      GTA_VOTE_LOCK();
      for (vote_index = 0; vote_index < GERAN_NUM_GTA_VETO_CLIENTS; vote_index++)
      {
        /* reset every vote to the value set by the caller */
        gta_veto_status[as_index].client_vote[vote_index] = gta_blocked;
      }
      GTA_VOTE_UNLOCK();

      MSG_GERAN_HIGH_1_G("GTA veto status reset to %d (1=GTA blocked)", (int)gta_blocked);
    }
    else
    {
      /* map the client ID into an index into the voting table */
      vote_index = geran_get_client_vote_index(client_id);

      if (vote_index < GERAN_NUM_GTA_VETO_CLIENTS)
      {
        /* acquire exclusive access to the voting table while it is updated */
        GTA_VOTE_LOCK();
        gta_veto_status[as_index].client_vote[vote_index] = gta_blocked;
        GTA_VOTE_UNLOCK();

        MSG_GERAN_HIGH_2_G("GTA veto client %d vote set to %d (1=GTA blocked)", (int)client_id, (int)gta_blocked);
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_2_G("Unable to apply GTA veto for client %d, ASID 0x%02X", (int)client_id, (int)as_id);
  }

  return;
}

/*===========================================================================

FUNCTION     geran_set_chain_exchanged

DESCRIPTION  Records whether an RF chain exchange has taken place

PARAMS       trm_grant: grant status for RF chain

DEPENDENCIES None

RETURN VALUE TRUE if a valid chain exchange was recorded

===========================================================================*/
static boolean geran_set_chain_exchanged(const geran_exchange_direction_t direction,
                                         const trm_grant_return_enum_type trm_grant,
                                         const gas_id_t                   gas_id)
{
  if (GERAN_EXCHANGE_NULL == direction)
  {
    MSG_GERAN_ERROR_0_G("LXIR: invalid parameter to geran_set_chain_exchanged");
  }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  else if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    int dir_idx;

    dir_idx = (GERAN_EXCHANGE_XTOG == direction) ? 0 : 1;
    geran_chain_exchanged[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)][dir_idx] = trm_grant;
    return(TRM_DENIAL != trm_grant);
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(FALSE);
}

/*===========================================================================

FUNCTION     geran_was_lock_exchanged

DESCRIPTION  Indicates whether TRM lock exchange has taken place

PARAMS       gas_id

DEPENDENCIES None

RETURN VALUE TRUE if any RF chain was exchanged

===========================================================================*/
boolean geran_was_lock_exchanged(const geran_exchange_direction_t direction,
                                 const gas_id_t                   gas_id)
{
  if (GERAN_EXCHANGE_NULL == direction)
  {
    MSG_GERAN_ERROR_0_G("LXIR: invalid parameter to geran_was_lock_exchanged");
  }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  else if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    int dir_idx;

    dir_idx = (GERAN_EXCHANGE_XTOG == direction) ? 0 : 1;
    return(TRM_DENIAL != geran_chain_exchanged[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)][dir_idx]);
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(FALSE);
}

/*===========================================================================

FUNCTION     geran_clear_lock_exchanged

DESCRIPTION  Resets the specified lock exchange data to TRM_DENIAL

PARAMS       direction: GERAN_EXCHANGE_GTOX or GERAN_EXCHANGE_XTOG

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/
void geran_clear_lock_exchanged(const geran_exchange_direction_t direction, const gas_id_t gas_id)
{
  /* just call the local utility function to reset the chain */
  (void)geran_set_chain_exchanged(direction, TRM_DENIAL, gas_id);

  return;
}

/*===========================================================================

FUNCTION     geran_lock_exchange_allowed

DESCRIPTION  Indicates whether specified GAS ID is allowed to exchange locks

PARAMS       GAS ID

DEPENDENCIES Result is based on hardcoded GAS ID check as well as device mode

RETURN VALUE TRUE if lock exchange is allowed for the specified GAS ID

===========================================================================*/
static boolean geran_lock_exchange_allowed(const gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (geran_is_multi_mode_gas_id(gas_id))
  {
    return(IS_MULTI_STANDBY_MODE(geran_get_multi_sim_mode()));
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(FALSE);
}

/*===========================================================================

FUNCTION      geran_get_irat_client_id

DESCRIPTION   Determines the IRAT exchange client ID associated with a GAS ID

DEPENDENCIES  None

RETURN VALUE  GRM_IRAT or GRM_IRAT2

SIDE EFFECTS  None

===========================================================================*/
static grm_client_enum_t geran_get_irat_client_id(const gas_id_t gas_id)
{
  return((GERAN_ACCESS_STRATUM_ID_1 == gas_id) ? GRM_IRAT : GRM_IRAT2);
}

/*===========================================================================

FUNCTION      geran_api_get_irat_client

DESCRIPTION   External API to determine the TRM IRAT client ID associated with a Modem AS ID.

DEPENDENCIES  None

RETURN VALUE  TRM IRAT Client Corresponding to a Subscription.

SIDE EFFECTS  None

===========================================================================*/

trm_client_enum_t geran_api_get_irat_client(const sys_modem_as_id_e_type as_id)
{
  grm_client_enum_t grm_irat_client_id = geran_get_irat_client_id(geran_map_nas_id_to_gas_id(as_id));
  return(grm_map_grm_client_id_to_trm_client_id(grm_irat_client_id));
}

/*===========================================================================

FUNCTION      geran_get_grm_client_id

DESCRIPTION   Determines the GRM client ID associated with a GAS ID

DEPENDENCIES  None

RETURN VALUE  GRM_GSM1, GRM_GSM2, or GRM_GSM3

SIDE EFFECTS  None

===========================================================================*/
grm_client_enum_t geran_get_grm_client_id(const gas_id_t gas_id)
{
  if (GERAN_ACCESS_STRATUM_ID_1 == gas_id)
  {
    return(GRM_GSM1);
  }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  else if (GERAN_ACCESS_STRATUM_ID_2 == gas_id)
  {
    return(GRM_GSM2);
  }
#ifdef FEATURE_TRIPLE_SIM
  else if (GERAN_ACCESS_STRATUM_ID_3 == gas_id)
  {
    return(GRM_GSM3);
  }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  MSG_GERAN_ERROR_0_G("Unable to resolve TRM client ID for this subscription");
  return(GRM_NO_CLIENT);
}

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#ifndef PERLUTF
/*===========================================================================

FUNCTION     geran_release_irat_client

DESCRIPTION  Instruct TRM to release for the specified inter-RAT client ID

PARAMS       GRM client ID (GRM_NO_CLIENT will be ignored safely)

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/
static void geran_release_irat_client(const grm_client_enum_t grm_client, const gas_id_t gas_id)
{
  if ((GRM_IRAT     == grm_client) ||
      (GRM_IRAT2    == grm_client) ||
      (GRM_IRAT_TX  == grm_client) ||
      (GRM_IRAT2_TX == grm_client))
  {
    trm_release_chain_input_info release_data = {0};

    /* map the GRM client ID to a TRM client ID */
    release_data.client = grm_map_grm_client_id_to_trm_client_id(grm_client);

    /* then invoke the TRM API to perform the release */
    trm_release_chain(&release_data);
  }
  else if (GRM_NO_CLIENT != grm_client)
  {
    MSG_GERAN_ERROR_1_G("LXIR: invalid inter-RAT client %d", (int)grm_client);
  }

  return;
}
#endif /* !PERLUTF */

/*===========================================================================

FUNCTION     geran_release_lock_exchange_clients

DESCRIPTION  Instruct TRM to release for the appropriate exchange clients

PARAMS       Pointer to client info structure and GAS ID

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/
static void geran_release_lock_exchange_clients(geran_exchange_info_t *info_ptr, const gas_id_t gas_id)
{
#ifndef PERLUTF
  if (info_ptr != NULL)
  {
    switch(info_ptr->direction)
    {
    case GERAN_EXCHANGE_XTOG:
      /* release the src clients only (X tech would have parked TRM_IRATx */
      geran_release_irat_client(info_ptr->src_rx_client, gas_id);
      geran_release_irat_client(info_ptr->src_tx_client, gas_id);
      break;

    case GERAN_EXCHANGE_GTOX:
      /* release the dst clients only (G tech would have parked TRM_IRATx */
      geran_release_irat_client(info_ptr->dst_rx_client, gas_id);
      geran_release_irat_client(info_ptr->dst_tx_client, gas_id);
      break;

    default:
      MSG_GERAN_ERROR_1_G("LXIR: invalid exchange direction %d", (int)(info_ptr->direction));
      break;
    }
  }
#else
  #error code not present
#endif /* !PERLUTF */

  return;
}

/*===========================================================================

FUNCTION     geran_populate_exchange_info

DESCRIPTION  Loads a structure with details of all the clients involved in
             a G2X or X2G lock exchange procedure

PARAMS       info_ptr - input/output structure
             GAS ID

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/
static void geran_populate_exchange_info(const geran_exchange_direction_t direction, geran_exchange_info_t *info_ptr, const gas_id_t gas_id)
{
  if (info_ptr != NULL)
  {
    /* always initialise the direction */
    info_ptr->direction = direction;

#ifndef PERLUTF
    if (GERAN_EXCHANGE_XTOG == direction)
    {
      /* X2G inter-RAT procedure */
      info_ptr->src_rx_client = geran_get_irat_client_id(gas_id); /* X-RAT (LTE, WCDMA, TDS etc.) */
      info_ptr->dst_rx_client = geran_get_grm_client_id(gas_id);  /* G-RAT */
    }
    else
    {
      /* G2X inter-RAT procedure */
      info_ptr->src_rx_client = geran_get_grm_client_id(gas_id);  /* G-RAT */
      info_ptr->dst_rx_client = geran_get_irat_client_id(gas_id); /* X-RAT (LTE, WCDMA, TDS etc.) */
    }

#ifdef FEATURE_GSM_RX_TX_SPLIT
    /* look up the correct TX client for each RX client */
    info_ptr->src_tx_client = grm_map_rx_to_tx_client_id(info_ptr->src_rx_client, gas_id);
    info_ptr->dst_tx_client = grm_map_rx_to_tx_client_id(info_ptr->dst_rx_client, gas_id);
#else
    info_ptr->src_tx_client = GRM_NO_CLIENT;
    info_ptr->dst_tx_client = GRM_NO_CLIENT;
#endif /* FEATURE_GSM_RX_TX_SPLIT */
#endif /* !PERLUTF */
  }

  return;
}
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION     geran_exchange_lock

DESCRIPTION  Exchanges the TRM lock between multimode GERAN and inter-RAT client

PARAMS       direction: GERAN_EXCHANGE_GTOX or GERAN_EXCHANGE_XTOG

DEPENDENCIES None

RETURN VALUE TRUE if the procedure succeeded, FALSE otherwise

===========================================================================*/
boolean geran_exchange_lock(const geran_exchange_direction_t direction, const gas_id_t gas_id)
{
  if (GERAN_EXCHANGE_NULL == direction)
  {
    MSG_GERAN_ERROR_0_G("LXIR: invalid parameter to geran_exchange_lock");
    return(FALSE);
  }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#ifndef PERLUTF
  else
  {
    trm_grant_return_enum_type trm_granted;
    trm_get_info_return_type   output_data;
    geran_exchange_info_t      client_info;
    GCMN_EXCHANGE_LOCK();
	/* Re-query if exchange is still allowed or not*/
	if (GERAN_EXCHANGE_NULL == geran_get_exchange_on_release(gas_id))
	{
      MSG_GERAN_ERROR_0_G("LXIR: not required");
	  GCMN_EXCHANGE_UNLOCK();
      return(FALSE);	
	}
    /* only exchange for multimode subscription in multi-standby mode */
    /* Note that this deliberately excludes Dual SIM Dual Active mode */
    if (FALSE == geran_lock_exchange_allowed(gas_id))
    {
      /* no exchange will be done, but need to ensure that the */
      /* inter-RAT clients are not holding chains indefinitely */
      if (GERAN_EXCHANGE_XTOG == direction)
      {
        geran_reset_lock_exchange_clients(geran_get_multi_sim_mode(), gas_id);
      }
      GCMN_EXCHANGE_UNLOCK();
      /* reset the exchange state variable and return it */
      return(geran_set_chain_exchanged(direction, TRM_DENIAL, gas_id));
    }

    /* populate the client info structure */
    geran_populate_exchange_info(direction, &client_info, gas_id);

    /* make sure the primary source client already owns an RF chain */
    trm_granted = grm_get_granted(client_info.src_rx_client, &output_data, gas_id);
    if (TRM_DENIAL != trm_granted)
    {
      trm_client_exchange_input_info input;

      /* nullify any existing unlock callback to avoid pre-emption */
      grm_retain_lock(client_info.src_rx_client, NULL, gas_id);
      if (GRM_NO_CLIENT != client_info.src_tx_client)
      {
        grm_retain_lock(client_info.src_tx_client, NULL, gas_id);
      }

      MSG_GERAN_HIGH_3_G("LXIR: client %d granted chain %d, RF device %d",
                         (int)(client_info.src_rx_client), (int)trm_granted, (int)(output_data.info.client_info.dev));

      /* clear down the state of the receiving clients */
      grm_release(client_info.dst_rx_client, gas_id);

      /* and call the TRM interface to complete the exchange for the RX clients */
      input.client_1 = grm_map_grm_client_id_to_trm_client_id(client_info.dst_rx_client);
      input.client_2 = grm_map_grm_client_id_to_trm_client_id(client_info.src_rx_client);
      trm_client_exchange(&input);

      /* if TX clients have also been specified, exchange them as well */
      if ((GRM_NO_CLIENT != client_info.src_tx_client) &&
          (GRM_NO_CLIENT != client_info.dst_tx_client))
      {
        input.client_1 = grm_map_grm_client_id_to_trm_client_id(client_info.dst_tx_client);
        input.client_2 = grm_map_grm_client_id_to_trm_client_id(client_info.src_tx_client);
        trm_client_exchange(&input);
      }

      /* clear down the state of the source clients */
      grm_release(client_info.src_rx_client, gas_id);

      MSG_GERAN_HIGH_2_G("LXIR: exchange completed for client %d to %d",
                         (int)(client_info.src_rx_client), (int)(client_info.dst_rx_client));
      GCMN_EXCHANGE_UNLOCK();
      /* update the exchange state variable and return it */
      return(geran_set_chain_exchanged(direction, trm_granted, gas_id));
    }
    else
    {
      /* explicit release here just to clean up anything in the TX client as well */
      MSG_GERAN_ERROR_1_G("LXIR: client %d does not hold any RF chain", (int)(client_info.src_rx_client));
	  GCMN_EXCHANGE_UNLOCK();
      grm_release(client_info.src_rx_client, gas_id);
    }
  }
#endif /* !PERLUTF */
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  /* reset the exchange state variable and return it */
  return(geran_set_chain_exchanged(direction, TRM_DENIAL, gas_id));
}

/*===========================================================================

FUNCTION     geran_set_exchange_on_release

DESCRIPTION  Sets the flag to specify trm_exchange() instead of trm_release

PARAMS       direction: GERAN_EXCHANGE_NULL, GERAN_EXCHANGE_GTOX, or GERAN_EXCHANGE_XTOG

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/
void geran_set_exchange_on_release(const geran_exchange_direction_t direction, const gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    geran_exchange_on_release[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = direction;
  }
  else
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  {
    MSG_GERAN_HIGH_2("LXIR: direction %d not supported for GAS ID 0x%02X", (int)direction, (int)gas_id);
  }

  return;
}

/*===========================================================================

FUNCTION     geran_get_exchange_on_release

DESCRIPTION  Gets the flag to determine trm_exchange() instead of trm_release

PARAMS       GAS ID

DEPENDENCIES None

RETURN VALUE GERAN_EXCHANGE_NULL, GERAN_EXCHANGE_GTOX or GERAN_EXCHANGE_XTOG

===========================================================================*/
geran_exchange_direction_t geran_get_exchange_on_release(const gas_id_t gas_id)
{
  if (geran_lock_exchange_allowed(gas_id))
  {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
    if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
    {
      return(geran_exchange_on_release[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)]);
    }
    else
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
    {
      MSG_GERAN_HIGH_0("LXIR: no exchange info available");
    }
  }

  return(GERAN_EXCHANGE_NULL);
}

/*===========================================================================

FUNCTION     geran_in_multi_standby_mode

DESCRIPTION  Indicates whether multi-standby (DSDS or TSTS) mode is active

PARAMS       None

DEPENDENCIES Device mode supplied by MMCP

RETURN VALUE TRUE if device mode is DSDS or TSTS, FALSE for SSSS and DSDA

===========================================================================*/
boolean geran_in_multi_standby_mode(void)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  sys_modem_device_mode_e_type dev_mode = geran_get_multi_sim_mode();

  if (IS_MULTI_STANDBY_MODE(dev_mode))
  {
    return(TRUE);
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(FALSE);
}

/*!
 * \brief Sets the reason for the pending GERAN traffic operation.
 *
 * \param traffic_reason (in) - new reason to be set
 * \param gas_id (in)         - access stratum ID
 */
void geran_set_traffic_reason(const geran_traffic_reason_t traffic_reason, const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    geran_traffic_reason[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = traffic_reason;
  }

  return;
}

/*!
 * \brief Gets the reason for the current GERAN traffic operation
 *
 * \param ps_ptr (out)        - set to TRUE if reason is for PS
 * \param gas_id (in)         - access stratum ID
 *
 * \return geran_traffic_reason_t - Most recent value of global traffic reason
 */
geran_traffic_reason_t geran_get_traffic_reason(boolean *ps_ptr, const gas_id_t gas_id)
{
  geran_traffic_reason_t local_reason;

  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    local_reason = geran_traffic_reason[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)];
  }
  else
  {
    local_reason = GERAN_TRAFFIC_DEFAULT;
  }

  /* caller may optionally indicate that it needs to know whether */
  /* the value returned here corresponds to (E)GPRS packet access */
  if (ps_ptr != NULL)
  {
    if ((GERAN_TRAFFIC_PS_SIGNALLING == local_reason) ||
        (GERAN_TRAFFIC_PS_DATA       == local_reason) ||
        (GERAN_TRAFFIC_PS_OTHER      == local_reason))
    {
      *ps_ptr = TRUE;
    }
    else
    {
      *ps_ptr = FALSE;
    }
  }

  return(local_reason);
}

/*!
 * Stub function for GPLT support
 */
void geran_as_id_gas_id_init(void)
{
  MSG_GERAN_HIGH_0("ASID mapping is now statically defined");
  return;
}

/*===========================================================================

FUNCTION     geran_abandon_lock_exchange

DESCRIPTION  Release the lock held by the inter-RAT exchange client

PARAMS       direction, GAS ID

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/

void geran_abandon_lock_exchange(const geran_exchange_direction_t direction, const gas_id_t gas_id)
{
  GCMN_EXCHANGE_LOCK();
  /* clear exchange-on-release flag to prevent GL1 from taking further action */
  geran_set_exchange_on_release(GERAN_EXCHANGE_NULL, gas_id);

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (geran_is_multi_mode_gas_id(gas_id))
  {
    geran_exchange_info_t client_info;

    /* populate info structure for the specified direction */
    geran_populate_exchange_info(direction, &client_info, gas_id);

    /* explicitly release the inter-RAT clients */
    geran_release_lock_exchange_clients(&client_info, gas_id);
    geran_clear_lock_exchanged(direction, gas_id);
    MSG_GERAN_HIGH_1_G("LXIR: abandoned exchange in direction=%d", (int)direction);
  }
#else
  NOTUSED(direction);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  GCMN_EXCHANGE_UNLOCK();
  return;
}

/*!
 * \brief Module initialisation function. Called during RR task initialisation.
 */
void geran_multi_sim_init(void)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  gas_id_t gas_id;

  /* assume a default config of Single SIM, Single Standby */
  geran_sys_device_mode  = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
  geran_multi_sim_mode   = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
  geran_sys_standby_pref = SYS_MODEM_DS_PREF_SINGLE_STANDBY;
  geran_sys_subs_feature = SYS_SUBS_FEATURE_MODE_NORMAL;

  /* Initialize DDS sub id to invalid value. NAS will send the right value at power up. */
  geran_dds_as_id = SYS_MODEM_AS_ID_NONE;

  for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
  {
    /* initialise the lock exchange flags for this access stratum */
    geran_set_exchange_on_release(GERAN_EXCHANGE_NULL, gas_id);
    (void)geran_set_chain_exchanged(GERAN_EXCHANGE_XTOG, TRM_DENIAL, gas_id);
    (void)geran_set_chain_exchanged(GERAN_EXCHANGE_GTOX, TRM_DENIAL, gas_id);
#ifdef FEATURE_LTE
    geran_lte_activity_status[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = LTE_ACTIVITY_NONE;
#endif /* FEATURE_LTE */
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  /* initialise GTA voting mechanism */
  rex_init_crit_sect(&gta_lock_crit_sec);
  /* initialise G lock exchange mechanism */
  rex_init_crit_sect(&gcmn_xchange_dir_crit_sec);
  memset(gta_veto_status, 0, sizeof(gta_veto_status));

  /* initialise subscription capabilities */
  geran_store_subscription_capabilities(NULL);

  return;
}

/*===========================================================================

FUNCTION     geran_device_in_svlte_dsda_mode

DESCRIPTION  Indicates whether device is configued in SVLTE+G DSDA mode

PARAMS       None

DEPENDENCIES Device mode supplied by MMCP

RETURN VALUE TRUE if device is configued in SVLTE+G DSDA mode

===========================================================================*/
boolean geran_device_in_svlte_dsda_mode(void)
{
  boolean svlte_dsda_config = FALSE;

#ifdef FEATURE_SVLTE_DUAL_SIM
  sys_modem_device_mode_e_type dev_mode = geran_get_multi_sim_mode();
  uint8                        simult_cap = 0;

  if (IS_DUAL_ACTIVE_MODE(dev_mode))
  {
    trm_get_info_input_type input;
    trm_get_info_return_type output;

    input.client_id = TRM_GSM1;
    input.info_type = TRM_SIMUL_CAPABILITY;

    trm_get_info(&input, &output);

    simult_cap = output.info.simul_cap_info;

    if ((simult_cap & TRM_SVLTE_IS_ENABLED) &&
        (simult_cap & TRM_DSDA_IS_ENABLED))
    {
      svlte_dsda_config = TRUE;
    }
  }

  MSG_GERAN_HIGH_3("MULTISIM=%d, simult_cap=%#02X, svlte_dsda_config=%d", dev_mode, simult_cap, svlte_dsda_config);
#endif /* FEATURE_SVLTE_DUAL_SIM */

  return(svlte_dsda_config);
}

/*===========================================================================

FUNCTION     geran_api_get_client_id_from_sub_id

DESCRIPTION  Converts ASID into the GRM client ID for GERAN

PARAMS       ASID

DEPENDENCIES None

RETURN VALUE GRM client ID (GRM_GSM1, GRM_GSM2 or GRM_GSM3)

===========================================================================*/
trm_client_enum_t geran_api_get_client_id_from_sub_id(const sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id;
  grm_client_enum_t grm_client_id;

  /* convert the supplied ASID into a GAS ID */
  gas_id = geran_map_nas_id_to_gas_id(as_id);

  /* look up the client ID for the GAS ID */
  grm_client_id = geran_get_grm_client_id(gas_id);

  return(grm_map_grm_client_id_to_trm_client_id(grm_client_id));
}

/*===========================================================================

FUNCTION     geran_api_get_sub_id_from_client_id

DESCRIPTION  Converts TRM client ID into an ASID

PARAMS       TRM client ID

DEPENDENCIES Subs Priority API within MCS

RETURN VALUE ASID

===========================================================================*/
sys_modem_as_id_e_type geran_api_get_sub_id_from_client_id(const trm_client_enum_t client_id)
{
#ifndef PERLUTF
  sys_modem_as_id_e_type as_id;
  uint32                 sp_id;

  /* Subs Priority API uses an integer format for the client ID */
  sp_id = (uint32)client_id;
  if (sp_get_asid_for_trm_client(sp_id, &as_id))
  {
    return(as_id);
  }
#endif /* !PERLUTF */

  return(SYS_MODEM_AS_ID_NONE);
}

/*!
 * \brief Returns a CxM tech ID based on GAS ID
 *
 * \param gas_id
 *
 * \return CXM_TECH_GSM1,_GSM2,_GSM3 or CXM_TECH_DFLT_INVLD
 */
cxm_tech_type geran_map_gas_id_to_cxm_id(const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    return(geran_cxm_ids[(int)gas_id]);
  }
  else
  {
    MSG_GERAN_ERROR_1("Invalid GAS ID 0x%02X passed for CxM mapping", (int)gas_id);
  }

  return(CXM_TECH_DFLT_INVLD);
}

/*!
 * \brief Returns a CxM tech ID based on AS ID
 *
 * \param as_id
 *
 * \return CXM_TECH_GSM1, CXM_TECH_GSM2 or CXM_TECH_GSM3
 */
cxm_tech_type geran_api_get_cxm_id_from_sub_id(const sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  return(geran_map_gas_id_to_cxm_id(gas_id));
}

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#ifdef FEATURE_LTE
/*===========================================================================

FUNCTION     geran_set_lte_activity_status

DESCRIPTION  Updates tracking flags for LTE stack activity

PARAMS       uint32 bitmask of activities (1 implies enabled, 0 otherwise)
             GAS ID

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/
void geran_set_lte_activity_status(const uint32 activity_mask, const gas_id_t gas_id)
{
  geran_lte_activity_status[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = activity_mask;
  return;
}

/*===========================================================================

FUNCTION     geran_get_lte_activity_status

DESCRIPTION  Returns tracking flags for LTE stack activity

PARAMS       GAS ID

DEPENDENCIES None

RETURN VALUE uint32 bitmask of activities

===========================================================================*/
uint32 geran_get_lte_activity_status(const gas_id_t gas_id)
{
  return(geran_lte_activity_status[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)]);
}
#endif /* FEATURE_LTE */

/*===========================================================================

FUNCTION     geran_set_current_dds_sub_id

DESCRIPTION  Updates the current DDS sub id

PARAMS       AS ID

DEPENDENCIES None

RETURN VALUE VOID

===========================================================================*/
void geran_set_current_dds_sub_id(sys_modem_as_id_e_type as_id)
{
  geran_dds_as_id = as_id;
  return;
}

/*===========================================================================

FUNCTION     geran_get_current_dds_sub_id

DESCRIPTION  Returns the current DDS sub id 

PARAMS       void

DEPENDENCIES None

RETURN VALUE sys_modem_as_id_e_type

===========================================================================*/
sys_modem_as_id_e_type geran_get_current_dds_sub_id(void)
{
  return(geran_dds_as_id);
}

/*===========================================================================

FUNCTION     geran_api_is_alt_sub_in_data

DESCRIPTION  Check if alternative SUB is in DATA

PARAMS       GAS ID

DEPENDENCIES NONE

RETURN VALUE TRUE/FALSE

===========================================================================*/
boolean geran_api_is_alt_sub_in_data(const gas_id_t gas_id)
{
  return grm_is_alt_sub_in_data(gas_id);
}

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION     geran_stack_status

DESCRIPTION  This function indicate the geran stack status to TRM.

PARAMS       stack_status, gas_id

DEPENDENCIES None

RETURN VALUE None
===========================================================================*/

void geran_stack_status(trm_protocol_stack_status_e_type stack_status, gas_id_t gas_id)
{
#ifndef PERLUTF
  trm_protocol_stack_ind_input_type  input_info;
  trm_protocol_stack_ind_output_type output_info;
 
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    input_info.client_id = TRM_GSM1;
    output_info.client_id = TRM_GSM1;
  }
  else
  {
    input_info.client_id = TRM_GSM2;
    output_info.client_id = TRM_GSM2;
  }
  
  input_info.as_id = geran_map_gas_id_to_nas_id(gas_id); 
  input_info.status = stack_status;
    
  output_info.result = FALSE;
 
  trm_protocol_stack_ind(&input_info, &output_info);
#endif
}
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR void geran_qsh_mdump_set_vars(qsh_mdump_crash_cat_e category, qsh_clt_e rep_client, qsh_clt_e client)
{
  qsh_mdump_set_crash_client(rep_client, client);
  qsh_mdump_set_crash_rat(rep_client, SYS_SYS_MODE_GSM);
  qsh_mdump_set_crash_category(rep_client, category);
}
#endif /* FEATURE_QSH_MDUMP */
/* EOF */

