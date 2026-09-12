#ifndef RR_PSCAN_DATA_CAPTURE_H
#define RR_PSCAN_DATA_CAPTURE_H

/*! \file rr_pscan_data_capture.h 
 
  This module implements the PSCAN DATA CAPTURE functionality
 
                Copyright (c) 2007-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_pscan_data_capture.h#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "rr_defs.h"

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
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
extern void rr_pscan_data_capture(
  rr_pscan_results_db_T *pscan_db_ptr,
  uint16 pscan_db_offset,
  const gas_id_t gas_id
);

/*!
 * \brief Returns TRUE if power-scan data capture mode is enabled; FALSE otherwise.
 * 
 * \param gas_id 
 * 
 * \return boolean 
 */
extern boolean rr_pscan_data_capture_enabled(const gas_id_t gas_id);

#endif /* ifndef RR_PSCAN_DATA_CAPTURE_H */

/* EOF */

