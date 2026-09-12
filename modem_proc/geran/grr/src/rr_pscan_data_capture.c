/*! \file rr_pscan_data_capture.c

  This module implements the PSCAN DATA CAPTURE functionality.

                Copyright (c) 2007-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_pscan_data_capture.c#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "rr_pscan_data_capture.h"
#include "rr_defs.h"
#include "rr_general.h"
#include "rr_multi_sim.h"
#include "cmapi.h"

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

// Note: PSCAN data capture is only supported for Sub1
struct
{
  boolean  enabled;
  uint32  *buffer_ptr;
  uint16   max_num_of_entries;
  uint16   num_of_entries;
  gas_id_t enabled_gas_id;
} rr_pscan_data_capture_data;

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/

/*!
 * \brief called when PSCAN data capture is enabled by CM. The buffer supplied by CM is initialised.
 *
 * \param memptr - pointer to the buffer where the PSCAN data should be stored
 * \param max_num_of_entries max_num_of_entries - max num of entries that can be stored in the buffer
 */
static void rr_pscan_enable_pscan_data_capture(
  uint32 *memptr,
  uint16 max_num_of_entries,
  gas_id_t gas_id
)
{
  int i;

  MSG_GERAN_HIGH_0_G("PSCAN data capture enabled");

  rr_pscan_data_capture_data.enabled            = TRUE;
  rr_pscan_data_capture_data.buffer_ptr         = memptr;
  rr_pscan_data_capture_data.max_num_of_entries = max_num_of_entries;
  rr_pscan_data_capture_data.num_of_entries     = 0;
  rr_pscan_data_capture_data.enabled_gas_id     = gas_id;

  /*
  1st 32-bit word is a header
    0-15 : CMAPI_SYS_MODE_GSM
   16-31 : num of freqs
  */
  (*memptr) = (uint16) CMAPI_SYS_MODE_GSM;  // Intialise with num_of_freqs=0
  ++memptr;

  /*
  Remaining buffer is array of 32-bit words containing scan result
    0-15 : arfcn
   16-23 : band
   24-31 : rssi
  */
  for (i = 0; i < max_num_of_entries; i++)
  {
    memptr[i] = 0x0000ffff;  // Initialise entry to invalid ARFCN
  }

  return;
}

/*!
 * \brief Called when PSCAN data capture is disabled by CM. Local data related to this feature are reset.
 */
static void rr_pscan_disable_pscan_data_capture(void)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  gas_id_t gas_id;

  gas_id = rr_pscan_data_capture_data.enabled_gas_id;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  MSG_GERAN_HIGH_0_G("PSCAN data capture disabled");

  rr_pscan_data_capture_data.enabled            = FALSE;
  rr_pscan_data_capture_data.buffer_ptr         = NULL;
  rr_pscan_data_capture_data.max_num_of_entries = 0;
  rr_pscan_data_capture_data.num_of_entries     = 0;
  rr_pscan_data_capture_data.enabled_gas_id     = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  return;
}

/*----------------------------------------------------------------------------
* Externalized Function Definitions
* -------------------------------------------------------------------------*/

/*!
 * \brief If PSCAN data capture is enabled, the data is copied to the buffer provided by CM.
 *
 *  Called when PSCAN data is available. If PSCAN data capture is enabled, the data is copied to the buffer
 *  provided by CM.
 *
 * \param pscan_db_ptr - PSCAN database
 * \param pscan_db_offset - PSCAN database offset
 */
void rr_pscan_data_capture(
  rr_pscan_results_db_T *pscan_db_ptr,
  uint16 pscan_db_offset,
  const gas_id_t gas_id
)
{
  RR_NULL_CHECK_RETURN_VOID(pscan_db_ptr);

  if (rr_pscan_data_capture_enabled(gas_id) &&
      (rr_pscan_data_capture_data.buffer_ptr != NULL))
  {
    uint16 arfcn;
    uint8 band;
    int8 rssi;
    uint16 num_of_freqs;
    uint16 pscan_db_index;
    uint32 *capture_buffer_ptr;
    uint16 capture_buffer_index;
    boolean arfcn_found;

    /*
    rr_pscan_data_capture_data.buffer_ptr is the buffer provided by CM to place the scan results.

    Format is as follows:

    First 32 bits of scan data memory (common to all RATs)
      1st 16 bits: cmapi_sys_mode_e RAT type typecast to uint16 (e.g. set to CMAPI_SYS_MODE_GSM)
      2nd 16 bits: number of following 32 bit words filled by the RAT
    */
    struct capture_buffer_header_s
    {
      uint16 sys_mode;
      uint16 num_of_freqs;
    } *header_ptr;

    /*
    Remaining 32 bit words contain the scan data
    */
    struct capture_buffer_entry_s
    {
      uint16 arfcn;
      uint8 band;
      int8 rssi;
    } *capture_buffer_entry_ptr;

    // Pointer to the 32-bit word 'header'
    header_ptr = (struct capture_buffer_header_s *)rr_pscan_data_capture_data.buffer_ptr;

    // Pointer to initial position to store scan data
    capture_buffer_ptr = rr_pscan_data_capture_data.buffer_ptr + 1;

    // Loop thru the frequencies in the pscan database, starting at the offset indicated
    pscan_db_index = pscan_db_offset;
    num_of_freqs = pscan_db_ptr->pscan_results.num_frequencies;

    while (pscan_db_index < num_of_freqs)
    {
      arfcn = (uint16) pscan_db_ptr->pscan_results.meas[pscan_db_index].arfcn.num;
      band = (uint8) pscan_db_ptr->pscan_results.meas[pscan_db_index].arfcn.band;
      rssi = (int8) rxlev_to_dbm(pscan_db_ptr->pscan_results.meas[pscan_db_index].RXLEV_average);

      MSG_GERAN_MED_3("PSCAN data capture entry: arfcn=%d band=%d rssi=%d",(int)arfcn,(int)band,(int)rssi);

      arfcn_found = FALSE;

      // Scan through the data capture buffer to check if there is an entry for this ARFCN already
      for (capture_buffer_index = 0;
           capture_buffer_index < rr_pscan_data_capture_data.num_of_entries;
           capture_buffer_index++)
      {
        capture_buffer_entry_ptr =
          (struct capture_buffer_entry_s *)capture_buffer_ptr + capture_buffer_index;

        if ((capture_buffer_entry_ptr->arfcn == arfcn) &&
            (capture_buffer_entry_ptr->band == band))
        {
          MSG_GERAN_MED_2("arfcn=%d band=%d already captured, updating rssi",(int)arfcn,(int)band);

          // Update the RSSI for this ARFCN
          capture_buffer_entry_ptr->rssi = rssi;

          // Mark ARFCN as found and exit loop
          arfcn_found = TRUE;
          break;
        }
      }

      if (!arfcn_found)
      {
        // This is a new ARFCN, not present in the capture buffer

        // Check if there is any room left in the capture buffer
        if (rr_pscan_data_capture_data.num_of_entries < rr_pscan_data_capture_data.max_num_of_entries)
        {
          capture_buffer_entry_ptr =
            (struct capture_buffer_entry_s *)capture_buffer_ptr + rr_pscan_data_capture_data.num_of_entries;

          // Store an entry in the capture buffer
          capture_buffer_entry_ptr->arfcn = arfcn;
          capture_buffer_entry_ptr->band = band;
          capture_buffer_entry_ptr->rssi = rssi;

          // Increment the number of entries
          ++rr_pscan_data_capture_data.num_of_entries;
        }
        else
        {
          MSG_GERAN_HIGH_0("PSCAN data capture buffer full");
          break;
        }
      }

      ++pscan_db_index;
    }

    // Update the number of entries into the header
    header_ptr->num_of_freqs = rr_pscan_data_capture_data.num_of_entries;
  }
}

/*!
 * \brief Returns TRUE if power-scan data capture is enabled for this GAS ID; FALSE otherwise.
 *
 * \param gas_id
 *
 * \return boolean
 */
boolean rr_pscan_data_capture_enabled(const gas_id_t gas_id)
{
  return(rr_pscan_data_capture_data.enabled && (gas_id == rr_pscan_data_capture_data.enabled_gas_id));
}

/*!
 * \brief Starts capture of power-scan data into the provided buffer. Note: Called by CM.
 *
 * \param memptr - Pointer to a buffer to place the results
 * \param memsize - Size (bytes) of the buffer
 *
 * \return boolean - TRUE if successful; FALSE otherwise
 */
boolean geran_start_capture_scan_data(uint32 *memptr, uint16 memsize)
{
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_1;
  int16  max_num_of_entries;

  /*
  memsize is size of the buffer, in bytes
  Each scan entry is 32-bits, so maximum number of entries is (memsize / 4)
  But the first 32-bit entry in the buffer is a header, so subtract 1.
  */
  max_num_of_entries = (memsize / 4) - 1;

  if ((max_num_of_entries > 0) && (memptr != NULL))
  {
    rr_pscan_enable_pscan_data_capture(memptr, max_num_of_entries, geran_map_nas_id_to_gas_id(as_id));

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*!
 * \brief Stops capture of power-scan data. Note: Called by CM.
 *
 * \return boolean - TRUE if successful; FALSE otherwise
 */
boolean geran_stop_capture_scan_data(void)
{
  rr_pscan_disable_pscan_data_capture();

  return TRUE;
}


#ifdef PERLUTF
#error code not present
#endif // PERLUTF

/* EOF */

