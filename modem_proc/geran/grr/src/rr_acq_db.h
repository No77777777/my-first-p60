#ifndef RR_ACQ_DB_H
#define RR_ACQ_DB_H

/*==============================================================================

                R R   A C Q   D B   ---   H E A D E R   F I L E

GENERAL DESCRIPTION
  This module implements the RR acquisition database.  This provides an API
  for querying whether an ARFCN is known to be in a particular PLMN.

EXTERNALIZED FUNCTIONS
  rr_acq_db_init
  rr_acq_db_save
  rr_acq_db_is_arfcn_in_plmn
  rr_acq_db_add_arfcn
  rr_acq_db_populate_pscan_db
  rr_acq_db_prioritise_pscan_results

INITIALIZATION AND SEQUENCING REQUIREMENTS
  rr_acq_db_init should be called at task initialisation to load the saved
  acquisition database from NV.

Copyright (c) 2007-2015 Qualcomm Technologies, Inc.

$Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_acq_db.h#1 $

==============================================================================*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "sys_type.h"
#include "sys.h"
#include "sys_v.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "geran_multi_sim.h"
#include "rr_pscan.h"
#include "geran_grr_msg.h"
#include "rr_defs.h"
/*==============================================================================

                PUBLIC DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/

/*==============================================================================
  Constants and Macros
==============================================================================*/

/*==============================================================================
  Typedefs
==============================================================================*/
/* Maximum number of GERAN frequencies LTE can pass to GERAN*/
#define RR_IRAT_ACQ_DB_MAX_FREQ 33 

/*==============================================================================
  Variables
==============================================================================*/

typedef struct
{
  uint8 num_arfcns;
  ARFCN_T arfcn[RR_IRAT_ACQ_DB_MAX_FREQ];
} rr_irat_acq_db_freq_list_t;

typedef struct
{
  rr_rat_e                    rat;          /*Originator of IRAT acq db info if set to RR_RAT_UNDEFINED the irat acq db is invalid*/
  sys_plmn_id_s_type          plmn_id;      /*PLMN ID of X RAT*/
  boolean                     is_hst_cell;  /*High Speed cell indicator*/
  rr_irat_acq_db_freq_list_t  freq_list;    /*GERAN neighbours from X RAT*/
} rr_irat_acq_db_t;

/*==============================================================================
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
==============================================================================*/

/**
 * rr_acq_db_init
 *
 * Initialises the RR acquisition database.  This includes reading and saved
 * entries from NV.
 *
 * @params none
 * @returns void
 */
extern void rr_acq_db_init(
  const gas_id_t gas_id
);

/**
 * rr_acq_db_init
 *
 * Initialises the RR acquisition database.  This includes reading and saved
 * entries from NV.
 *
 * @params none
 * @returns void
 */
extern void rr_irat_acq_db_init(void);

/**
 * rr_acq_db_save
 *
 * Saves the RR acquisition database in non-volatile storage.
 */
extern void rr_acq_db_save(
  const gas_id_t gas_id
);

/**
 * rr_acq_db_is_arfcn_in_plmn
 *
 * Verifies whether the specified ARFCN has been previously identified as
 * belonging to the specified PLMN.
 *
 * @param arfcn the ARFCN to search for
 * @param plmn_id the PLMN ID to search for
 * @return TRUE if the ARFCN is known to belong to the given PLMN, FALSE otherwise.
 */
extern boolean rr_acq_db_is_arfcn_in_plmn(
  ARFCN_T arfcn,
  sys_plmn_id_s_type plmn_id,
  const gas_id_t gas_id
);

/**
 * Stores an ARFCN and associated PLMN in the RR acquisition database.
 *
 * Note that if the ARFCN already has a PLMN associated with it and it is
 * different to the new PLMN specified then before adding this entry to the
 * acquisition database all other entries for the specified PLMN will be
 * removed as it is assumed that the UE has moved to a new region where
 * the operator has a different ARFCN allocation.
 *
 * @param arfcn The ARFCN to store.
 * @param plmn_id The PLMN ID to associate with the specified ARFCN.
 */
typedef enum
{
  RR_ACQ_DB_HASH_FAILED,
  RR_ACQ_DB_ARFCN_ADDED,
  RR_ACQ_DB_PLMN_CHANGE
} rr_acq_db_result_t;

extern rr_acq_db_result_t rr_acq_db_add_arfcn(
  ARFCN_T arfcn,
  sys_plmn_id_s_type plmn_id,
  boolean to_log,
  const gas_id_t gas_id
);

/**
 * Populates the power scan request/results structure with entries for the
 * requested PLMN.
 *
 * @param pscan_request The list in to which ARFCNs will be copied.
 * @param band_pref Bitmap of allowed bands.
 * @param plmn_id The PLMN Id to search for.
 * @param requested_plmn_only TRUE if only ARFCNs for the requested PLMN should
          be included in the power scan, FALSE if ARFCNs from other known PLMNs
          may be included.
 */
extern void rr_acq_db_populate_pscan_db(
  rr_l1_pscan_results_T * pscan_request,
  sys_band_mask_type      band_pref,
  sys_plmn_id_s_type      plmn_id,
  boolean                 requested_plmn_only,
  rr_pscan_request_type_e req_type,
  const gas_id_t          gas_id
);

/**
 * Prioritises the power scan results according to the acquisition database.
 *
 * This function is used to ensure that ARFCNs belonging to the requested PLMN
 * appear towards the front of the list of ARFCNs sent to L1 for BCCH decode.
 *
 * @param pscan_results The power scan results from L1, sorted by signal
 *                      strength.
 * @param plmn_id The requested PLMN.
 */
extern void rr_acq_db_prioritise_pscan_results(
  rr_l1_pscan_results_T *pscan_results,
  sys_plmn_id_s_type plmn_id,
  const gas_id_t gas_id
);
/**
 * check how many arfcn in acq db
 * @param band_pref band_pref information
*/
extern uint16 rr_num_of_arfcn_in_acq_db(sys_band_mask_type band_pref, const gas_id_t gas_id);

#ifdef PERLUTF
#error code not present
#endif /* PERLUTF */


/**
 * check if the IRAT acq DB is valid to use
 * @param gas_id
 * @return irat db valid
*/
extern boolean rr_irat_acq_db_valid(const gas_id_t gas_id);

#ifdef FEATURE_GERAN_HST_OPT
/**
 * Returns the high speed indicator for the current irat acq db db
 * @param void
 * @return boolean - High speed flag
*/
extern boolean rr_high_speed_ind_set(void);
#endif /*FEATURE_GERAN_HST_OPT*/

/*!
 * \brief Return a pointer to the module data.
 *
 * \return rr_irat_acq_db_t *
 */
extern rr_irat_acq_db_t *rr_irat_acq_db_get_data_ptr(void);


/**
 * Copy GERAN freq info update to internal storage
 * @param freq_info_ptr 
 *        gas_id 
*/
extern void rr_store_lte_geran_freq_info_ind(geran_grr_lte_geran_freq_info_ind_t *freq_info_ptr,
                                             const gas_id_t gas_id);
/**
 * Copy all the ARFCNs in the Acq Db in to the arfcn_list of log packet and update the count of log packets
 * @param arfcn_list 
 *        count 
*/
extern void rr_acq_db_populate_log_packet(rr_log_arfcn_list_t *arfcn_list, int *count);

#endif /* #ifndef RR_ACQ_DB_H */

/* EOF */

