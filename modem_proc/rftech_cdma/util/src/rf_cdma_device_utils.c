/*! 
  @file
  rf_cdma_utils_freq.c
 
  @brief
  This file defines the common utility functions relating to frequency
  translation used by the CDMA RF driver.
 
  @addtogroup RF_CDMA_COMMON_UTILS
  @{
*/

/*==============================================================================

  Copyright (c) 2005 - 2013 Qualcomm Technologies, Inc.. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc..

==============================================================================*/

/*==============================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_cdma.mpss/3.11/util/src/rf_cdma_device_utils.c#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
07/17/15   avi     Initial version


==============================================================================*/


#include "comdef.h"
#include "rf_cdma_utils_freq.h"
#include "rfcommon_msg.h"
#include "rfcommon_core_error.h"

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Table of mappings from band and channel to Rx carrier freqency.

  @details
  This structure holds the parameters required for calculation
  of Rx Carrier Frequency as described below:
    Find the row where the band is matched and chan lies between
    lo and hi limits then
      Rx Carrier Freq (KHz)= ( base + (spacing * (chan + offset)))

  Note:  The functions utilizing these tables perform the appropriate
  scaling within the calculations to return the Carrier frequencies in KHz.
*/
const rf_carrier_freq_type rf_rx_freq_tbl[] =
{
  /*  band        low   high  spacing  offset ch_step  base (KHz)                   */
  {RFM_CDMA_BC0,     1,   799,  300,        0,      1,  870000}, /* BC0: Ch 1-799    */
  {RFM_CDMA_BC0,   991,  1023,  300,    -1023,      1,  870000}, /* BC0: Ch 991-1023 */
  {RFM_CDMA_BC0,  1024,  1323,  300,    -1024,      1,  860040}, /* BC0: Ch 1024-1323 */
  {RFM_CDMA_BC1,     0,  1199,  500,        0,      1, 1930000}, /* BC1: Ch 0-1199   */
  {RFM_CDMA_BC3,     2,   798,  125,        0,      2,  860000}, /* BC3: Ch 2-798    */
  {RFM_CDMA_BC3,   802,  1038,  125,     -800,      2,  843000}, /* BC3: Ch 802-1038 */
  {RFM_CDMA_BC3,  1042,  1198,  125,    -1040,      2,  832000}, /* BC3: Ch 1042-1198*/
  {RFM_CDMA_BC3,  1202,  1600,  125,    -1200,      2,  838000}, /* BC3: Ch 1202-1600*/
  {RFM_CDMA_BC4,     0,   599,  500,        0,      1, 1840000}, /* BC4: Ch 0-599    */
  {RFM_CDMA_BC5,     1,   400,  250,       -1,      1,  460000}, /* BC5: Ch 1-400    */
  {RFM_CDMA_BC5,   472,   871,  250,     -472,      1,  420000}, /* BC5: Ch 472-871  */
  {RFM_CDMA_BC5,  1039,  1473,  200,    -1024,      1,  461010}, /* BC5: Ch 1039-1473*/
  {RFM_CDMA_BC5,  1536,  1715,  250,    -1536,      1,  489000}, /* BC5: Ch 1536-1715*/
  {RFM_CDMA_BC5,  1792,  2016,  200,    -1792,      1,  489000}, /* BC5: Ch 1792-2016*/
  {RFM_CDMA_BC6,     0,  1199,  500,        0,      1, 2110000}, /* BC6: Ch 0-1199   */
  {RFM_CDMA_BC10,    0,   719,  250,        0,      1,  851000}, /* BC10:Ch 0-919    */
  {RFM_CDMA_BC10,  720,   919,  250,     -720,      1,  935000}, /* BC10:Ch 0-919    */
  {RFM_CDMA_BC11,    1,   400,  250,       -1,      1,  460000}, /* BC11:Ch 1-400    */
  {RFM_CDMA_BC11,  539,   871,  250,     -512,      1,  421000}, /* BC11:Ch 539-871  */
  {RFM_CDMA_BC11, 1039,  1473,  200,    -1024,      1,  461010}, /* BC11:Ch 1039-1473*/
  {RFM_CDMA_BC11, 1536,  1715,  250,    -1536,      1,  489000}, /* BC11:Ch 1536-1715*/
  {RFM_CDMA_BC11, 1792,  2016,  200,    -1792,      1,  489000}, /* BC11:Ch 1792-2016*/
  {RFM_CDMA_BC14,    0,  1299,  500,        0,      1, 1930000}, /* BC14:Ch 0-1299   */
  {RFM_CDMA_BC15,    0,   899,  500,        0,      1, 2110000}, /* BC15:Ch 0-899    */
};

const uint16 rf_rx_freq_tbl_size = ARR_SIZE(rf_rx_freq_tbl);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Table of mappings from band and channel to Rx carrier freqency.

  @details
  This structure holds the parameters required for calculation
  of Tx Carrier Frequency as described below:
    Find the row where the band is matched and chan lies between
    lo and hi limits then
      Tx Carrier Freq (KHz)= ( base + (spacing * (chan + offset)))

  Note:  The functions utilizing these tables perform the appropriate
  scaling within the calculations to return the Carrier frequencies in KHz.
*/
const rf_carrier_freq_type rf_tx_freq_tbl[] =
{
  /*   band        low  high   spacing offset ch_step base (KHz) */
  {RFM_CDMA_BC0,     1,   799,  300,        0,      1,  825000}, /* BC0: Ch 1-799 */
  {RFM_CDMA_BC0,   991,  1023,  300,    -1023,      1,  825000}, /* BC0: Ch 991-1023 */
  {RFM_CDMA_BC0,  1024,  1323,  300,    -1024,      1,  815040}, /* BC0: Ch 1024-1323 */
  {RFM_CDMA_BC1,     0,  1199,  500,        0,      1, 1850000}, /* BC1: Ch 0-1199   */
  {RFM_CDMA_BC3,     2,   798,  125,        0,      2,  915000}, /* BC3: Ch 2-798    */
  {RFM_CDMA_BC3,   802,  1038,  125,     -800,      2,  898000}, /* BC3: Ch 802-1038 */
  {RFM_CDMA_BC3,  1042,  1198,  125,    -1040,      2,  887000}, /* BC3: Ch 1042-1198*/
  {RFM_CDMA_BC3,  1202,  1600,  125,    -1200,      2,  893000}, /* BC3: Ch 1202-1600*/
  {RFM_CDMA_BC4,     0,   599,  500,        0,      1, 1750000}, /* BC4: Ch 0-599    */
  {RFM_CDMA_BC5,     1,   400,  250,       -1,      1,  450000}, /* BC5: Ch 1-400    */
  {RFM_CDMA_BC5,   472,   871,  250,     -472,      1,  410000}, /* BC5: Ch 472-871  */
  {RFM_CDMA_BC5,  1039,  1473,  200,    -1024,      1,  451010}, /* BC5: Ch 1039-1473*/
  {RFM_CDMA_BC5,  1536,  1715,  250,    -1536,      1,  479000}, /* BC5: Ch 1536-1715*/
  {RFM_CDMA_BC5,  1792,  2016,  200,    -1792,      1,  479000}, /* BC5: Ch 1792-2016*/
  {RFM_CDMA_BC6,     0,  1199,  500,        0,      1, 1920000}, /* BC6: Ch 0-1199   */
  {RFM_CDMA_BC10,    0,   719,  250,        0,      1,  806000}, /* BC10:Ch 0-919    */
  {RFM_CDMA_BC10,  720,   919,  250,     -720,      1,  896000}, /* BC10:Ch 0-919    */
  {RFM_CDMA_BC11,    1,   400,  250,       -1,      1,  450000}, /* BC11:Ch 1-400    */
  {RFM_CDMA_BC11,  539,   871,  250,     -512,      1,  411000}, /* BC11:Ch 539-871  */
  {RFM_CDMA_BC11, 1039,  1473,  200,    -1024,      1,  451010}, /* BC11:Ch 1039-1473*/
  {RFM_CDMA_BC11, 1536,  1715,  250,    -1536,      1,  479000}, /* BC11:Ch 1536-1715*/
  {RFM_CDMA_BC11, 1792,  2016,  200,    -1792,      1,  479000}, /* BC11:Ch 1792-2016*/
  {RFM_CDMA_BC14,    0,  1299,  500,        0,      1, 1850000}, /* BC14:Ch 0-1299   */
  {RFM_CDMA_BC15,    0,   899,  500,        0,      1, 1710000}, /* BC15:Ch 0-899   */
};

const uint16 rf_tx_freq_tbl_size = ARR_SIZE(rf_tx_freq_tbl);


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Computes the Tx carrier frequency in kHz of a band and channel.

  @details
  If the band or channel is not supported then 0 is returned.

  @param band
  3GPP2 band class.

  @param chan
  3GPP2 channel that should be valid for the band class in the band
  parameter.

  @return
  Tx frequency in kHz of the channel, or 0 if the RF driver does not
  support the channel.
*/
uint32
rf_cdma_get_tx_carrier_freq
(
  rfm_cdma_band_class_type band,
  int chan
)
{
  int32 freq=0;
  int32  delta = 0;
  int16 i = 0;

  while (i < rf_tx_freq_tbl_size)
  {
    if ((band == rf_tx_freq_tbl[i].band ) &&
        (chan >= rf_tx_freq_tbl[i].lo_chan ) &&
        (chan <= rf_tx_freq_tbl[i].hi_chan ))
      break;
    i++;
  }

  if ( (i < rf_tx_freq_tbl_size)
       && ( (1 == rf_tx_freq_tbl[i].ch_step)
            /* 'lo_chan' is the lowest valid channel#; the following checks if
               'chan' is integral multiples of 'ch_step' above 'lo_chan' */
            || ( ( (chan - rf_tx_freq_tbl[i].lo_chan) % rf_tx_freq_tbl[i].ch_step ) == 0 ) ) )
  {
    /* To avoid doing floating point math:
         1) All calculations are done in Khz.
         2) To avoid losing accuracy, the ch_sp (spacing) parameter in the
            table is scaled to 10 times the value in KHz. At the end of the
            delta calculation, the 10 is divided off.
    */           
    delta = (rf_tx_freq_tbl[i].ch_sp * (chan + rf_tx_freq_tbl[i].ch_offst))/10;
    freq = (rf_tx_freq_tbl[i].base_freq) + delta;
  }
  return(uint32) freq;
} /* rf_cdma_get_tx_carrier_freq() */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Return Channel Number for given frequency

  @details
  This function returns the channel number that is located closest to the
  specified Rx carrier frequency (in KHz) in the indicated band. If the freq
  falls within the band, a successful result is returned.

  @param chan_freq_khz 
  RF freq in KHz on which a reverse channel lookup needs to be done
 
  @param band_class
  Band class in which the lookup needs to be done
 
  @param mid_point_preference
  Specifies if the function should return lower/higher freq RF channel for the 
  case when the specified frequency lies exactly between 2 valid RF channels in 
  the specified band
 
  @param chan_ptr
  Pointer through which the RF channel is returned to the caller
 
  @param converted_chan_freq_khz_ptr
  Pointer through which the frequency of the converted channel is returned to 
  the caller. Might be useful to the caller when the requested RF freq does NOT 
  line up exactly with a valid RF channel. Caller may specify NULL if this info 
  is not needed.

  @return
  Status indicating if channel falls within the given band class
*/

boolean
rf_cdma_convert_rx_freq_khz_to_chan
(
  uint32 const                       chan_freq_khz,
  rfm_cdma_band_class_type const     band_class,
  rx_freq_khz_to_chan_preference_type const mid_point_preference,
  word                               *const chan_ptr,
  uint32                             *const converted_chan_freq_khz_ptr
)
{
  boolean status = FALSE;
  uint32 index = 0;
  uint32 lo_freq_khz = 0;
  uint32 hi_freq_khz = 0;
  typedef struct
  {
     word   chan;
     uint32 chan_freq_khz;
     uint32 freq_offset_khz;
  } candidate_chan_type;
  candidate_chan_type candidate_chan_set[2]
     = {
          {0, 0, 0},
          {0, 0, 0}
       };
  uint32 candidate_chan_set_size = 0;
  uint32 temp_freq_offset_khz = 0;
  int32 inter_chan_khz_x_10 = 0;
  candidate_chan_type *result_ptr = NULL;

  for ( index = 0; index < rf_rx_freq_tbl_size; index++ )
  {
    /* Process each band class matching entry in 'rf_rx_freq_tbl' as follows:
      - if the requested 'chan_freq_khz'
        - falls within the range of 'hi_chan' and 'lo_chan', then extrapolate
          to find out the closest channel. Return TRUE (SUCCESS) and the channel.
        - is below 'lo_chan' or above 'hi_chan'
          - by less than 'ch_sp' spacing, record the potential candidate in
            a candidate array. Continue to process other entries in 'rf_rx_freq_tbl'.
          - by more than or equal to 'ch_sp' spacing, ignore this. Continue to
            process other entries in 'rf_rx_freq_tbl'.
    
     After checking all entries in 'rf_rx_freq_tbl' (case where SUCCESSful result hasn't
     yet been obtained),
      - if there are exactly 2 entries in the candidate array, return TRUE (SUCCESS)
        and the closest channel in the candidate array.
      - else return FALSE (FAILURE).
    */
    if (band_class == rf_rx_freq_tbl[index].band)
    {
      lo_freq_khz = rf_cdma_get_rx_carrier_freq( band_class,
                                                 rf_rx_freq_tbl[index].lo_chan);
      hi_freq_khz = rf_cdma_get_rx_carrier_freq( band_class, 
                                                 rf_rx_freq_tbl[index].hi_chan);

      inter_chan_khz_x_10 = 
                   rf_rx_freq_tbl[index].ch_sp * rf_rx_freq_tbl[index].ch_step;
      if ( (chan_freq_khz >= lo_freq_khz)
           && (chan_freq_khz <= hi_freq_khz) )
      {
         temp_freq_offset_khz = (chan_freq_khz - lo_freq_khz);
         candidate_chan_set[0].chan = ( word )( rf_rx_freq_tbl[index].lo_chan
                                      + ( ( ( 10                     // since ch_sp is scaled by a factor of 10
                                              * temp_freq_offset_khz )
                                            / inter_chan_khz_x_10 )
                                          * rf_rx_freq_tbl[index].ch_step ) );
         candidate_chan_set[0].chan_freq_khz = rf_cdma_get_rx_carrier_freq(
                                                       band_class,
                                                       candidate_chan_set[0].chan);
         candidate_chan_set[0].freq_offset_khz =  chan_freq_khz
                                                  - candidate_chan_set[0].chan_freq_khz;
         if (0 == candidate_chan_set[0].freq_offset_khz)
         {
            /* either of lo_chan or hi_chan is exactly equal to candidate_chan_set[0].chan;
               in this case, make the 2nd candidate an alias of the 1st one in order to make use of
               the common logic for the best candidate detection. */
            candidate_chan_set[1] = candidate_chan_set[0];
         }
         else
         {
            candidate_chan_set[1].chan = candidate_chan_set[0].chan + rf_rx_freq_tbl[index].ch_step;
            candidate_chan_set[1].chan_freq_khz = rf_cdma_get_rx_carrier_freq(
                                                     band_class,
                                                     candidate_chan_set[1].chan);
            candidate_chan_set[1].freq_offset_khz =  candidate_chan_set[1].chan_freq_khz
                                                     - chan_freq_khz;
         }
         candidate_chan_set_size = 2;
      }
      else if (chan_freq_khz < lo_freq_khz)
      {
         temp_freq_offset_khz = (lo_freq_khz - chan_freq_khz);
         if ( (temp_freq_offset_khz*10) < ( uint32 )inter_chan_khz_x_10 )
         {
            candidate_chan_set[candidate_chan_set_size].chan = rf_rx_freq_tbl[index].lo_chan;
            candidate_chan_set[candidate_chan_set_size].chan_freq_khz = lo_freq_khz;
            candidate_chan_set[candidate_chan_set_size].freq_offset_khz = temp_freq_offset_khz;
            candidate_chan_set_size ++;
         }
      }
      else if (chan_freq_khz > hi_freq_khz)
      {
         temp_freq_offset_khz = (chan_freq_khz - hi_freq_khz);
         if ( (temp_freq_offset_khz*10) < ( uint32 )inter_chan_khz_x_10 )
         {
            candidate_chan_set[candidate_chan_set_size].chan = rf_rx_freq_tbl[index].hi_chan;
            candidate_chan_set[candidate_chan_set_size].chan_freq_khz = hi_freq_khz;
            candidate_chan_set[candidate_chan_set_size].freq_offset_khz = temp_freq_offset_khz;
            candidate_chan_set_size ++;
         }
      }

      RF_ASSERT(candidate_chan_set_size <= 2);

      if (2 == candidate_chan_set_size)
      {
         break;
      }
      /* else continue with searching for more candidate channel(s) */

    } /*if (band_class == rf_rx_freq_tbl[index].band) */
  } /* for (index = 0; index < rf_rx_freq_tbl_size; index ++) */

   // At this point, there should be exactly 2 entries in candidate set for the successful case
   if (2 == candidate_chan_set_size)
   {
      if (candidate_chan_set[0].freq_offset_khz < candidate_chan_set[1].freq_offset_khz)
      {
         result_ptr = &candidate_chan_set[0];
      }
      else if (candidate_chan_set[1].freq_offset_khz < candidate_chan_set[0].freq_offset_khz)
      {
         result_ptr = &candidate_chan_set[1];
      }
      else
      {
         result_ptr = &candidate_chan_set[0];
         if ( ( (RX_FREQ_KHZ_TO_CHAN_PREFERENCE__LOWER_FREQ == mid_point_preference)
                && (candidate_chan_set[1].chan_freq_khz < candidate_chan_set[0].chan_freq_khz) )
              || ( (RX_FREQ_KHZ_TO_CHAN_PREFERENCE__HIGHER_FREQ == mid_point_preference)
                   && (candidate_chan_set[1].chan_freq_khz > candidate_chan_set[0].chan_freq_khz) ) )
         {
            result_ptr = &candidate_chan_set[1];
         }
      }

      *chan_ptr = result_ptr->chan;
      if (NULL != converted_chan_freq_khz_ptr)
      {
         *converted_chan_freq_khz_ptr = result_ptr->chan_freq_khz;
      }
      status = TRUE;
   }

   return status;

} /* rf_cdma_convert_rx_freq_khz_to_chan */



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Computes the Rx carrier frequency in kHz of a band and channel.

  @details
  If the band or channel is not supported then 0 is returned.

  @param band
  3GPP2 band class.

  @param chan
  3GPP2 channel that should be valid for the band class in the band
  parameter.

  @return
  Rx frequency in kHz of the channel, or 0 if the RF driver does not
  support the channel.
*/
uint32
rf_cdma_get_rx_carrier_freq
(
  rfm_cdma_band_class_type band,
  int chan
)
{
  int32 freq=0;
  int32  delta = 0;
  int16 i = 0;

  while (i < rf_rx_freq_tbl_size)
  {
    if ((band == rf_rx_freq_tbl[i].band ) &&
        (chan >= rf_rx_freq_tbl[i].lo_chan ) &&
        (chan <= rf_rx_freq_tbl[i].hi_chan ))
      break;
    i++;
  }

  if ( (i < rf_rx_freq_tbl_size)
       && ( (1 == rf_rx_freq_tbl[i].ch_step)
            /* 'lo_chan' is the lowest valid channel#; the following checks if
               'chan' is integral multiples of 'ch_step' above 'lo_chan' */
            || ( ( (chan - rf_rx_freq_tbl[i].lo_chan) % rf_rx_freq_tbl[i].ch_step ) == 0 ) ) )
  {
    /* To avoid doing floating point math:
         1) All calculations are done in Khz.
         2) To avoid losing accuracy, the ch_sp (spacing) parameter in the
            table is scaled to 10 times the value in KHz. At the end of the
            delta calculation, the 10 is divided off.
    */
    delta = (rf_rx_freq_tbl[i].ch_sp * (chan + rf_rx_freq_tbl[i].ch_offst))/10;
    freq = (rf_rx_freq_tbl[i].base_freq) + delta;
  }
  return(uint32) freq;
} /* rf_cdma_get_rx_carrier_freq() */





/*----------------------------------------------------------------------------*/
/*!
  @brief
  Return Rx Channel Separation

  @details
  This function returns the separation between adjacent valid channels in the
  specified band class.
 
  @param band
  Band under consideration
 
  @return
  On success, return the separation between adjacent channels (in KHz), and
  zero on failure
*/
int32 
rf_cdma_get_rx_inter_chan_separation
(
  rfm_cdma_band_class_type const band
)
{
   int16 i = 0;

   for (i=0; i < rf_rx_freq_tbl_size; i++)
   {
      if (band == rf_rx_freq_tbl[i].band)
      {
        /* return in KHz */
        return (rf_rx_freq_tbl[i].ch_sp * rf_rx_freq_tbl[i].ch_step) / 10; 
      }
   }

   return 0;
} /* rf_cdma_get_rx_inter_chan_separation */



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Convert RFM CDMA Band to RF Card Band
 
  @details
  This function will map the given RFM CDMA band to generic RF Card Band.
 
  @param rfm_band
  RFM CDMA to be mapped
 
  @return
  Mapped RF Card Card Band
*/
rf_card_band_type
rf_cdma_convert_rfm_band_to_card_band
(
  rfm_cdma_band_class_type rfm_band
) 
{
  rf_card_band_type rfc_band = RF_MAX_BAND;

  switch( rfm_band )
  {
  case RFM_CDMA_BC0:
    rfc_band = RF_BC0_BAND;
    break;
  case RFM_CDMA_BC1:
    rfc_band = RF_BC1_BAND;
    break;
  case RFM_CDMA_BC2:
    rfc_band = RF_BC2_BAND;
    break;
  case RFM_CDMA_BC3:
    rfc_band = RF_BC3_BAND;
    break;
  case RFM_CDMA_BC4:
    rfc_band = RF_BC4_BAND;
    break;
  case RFM_CDMA_BC5:
    rfc_band = RF_BC5_BAND;
    break;
  case RFM_CDMA_BC6:
    rfc_band = RF_BC6_BAND;
    break;
  case RFM_CDMA_BC7:
    rfc_band = RF_BC7_BAND;
    break;
  case RFM_CDMA_BC8:
    rfc_band = RF_BC8_BAND;
    break;
  case RFM_CDMA_BC9:
    rfc_band = RF_BC9_BAND;
    break;
  case RFM_CDMA_BC10:
    rfc_band = RF_BC10_BAND;
    break;
  case RFM_CDMA_BC11:
    rfc_band = RF_BC11_BAND;
    break;
  case RFM_CDMA_BC12:
    rfc_band = RF_BC12_BAND;
    break;
  case RFM_CDMA_BC13:
    rfc_band = RF_BC13_BAND;
    break;
  case RFM_CDMA_BC14:
    rfc_band = RF_BC14_BAND;
    break;
  case RFM_CDMA_BC15:
    rfc_band = RF_BC15_BAND;
    break;
  case RFM_CDMA_BC16:
    rfc_band = RF_BC16_BAND;
    break;
  case RFM_CDMA_BC18:
    rfc_band = RF_BC18_BAND;
    break;
  case RFM_CDMA_BC19:
    rfc_band = RF_BC19_BAND;
    break;
  default:
    RF_MSG_1( RF_ERROR, "rf_cdma_utils_convert_rfm_band_to_card_band: "
                      "Invalid RFM band :%d" ,rfc_band );
    break;
  }/* switch( rfm_band ) */

  return( rfc_band );

} /* rf_cdma_utils_convert_rfm_band_to_card_band */
