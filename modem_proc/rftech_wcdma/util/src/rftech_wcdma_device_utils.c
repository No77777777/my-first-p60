/*!
  @file
  rfwcdma_core.c

  @brief
  RF WCDMA Core (Generic) Functions.

  @details

*/

/*===========================================================================

Copyright (c) 2008, 2009 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:08:47 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_wcdma.mpss/3.11/util/src/rftech_wcdma_device_utils.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/17/15   avi     Initial version


============================================================================*/

#include "rfwcdma_core.h"
#include "bsp.h"
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns Tx UL Chan for given frequency

  @param uint32 freq - Uplink Frequency in kHz

  @return uint16 chan - Uplink UARFCN
*/
uint16 rfwcdma_core_get_uarfcn_from_freq
(
  rfcom_wcdma_band_type band,
  uint32 freq,                            /* input freq */
  rfwcdma_core_uarfcn_ul_dl_type ul_dl
)
{
  uint16 chan = BSP_RF_TX_CHAN_WCDMA_MIN;

  if ( freq == 0 )
  {
    return (0);
  }

  /* All frequency numbers are in KHz and hence, the appropriate conversion factors are applied here */
  switch ( band )
  {
    case RFCOM_BAND_IMT:
      if( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_MAX )
      {
        chan =  (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
      }
      else if( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_MAX )
      {
        chan =  (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
      }
      break;

    case RFCOM_BAND_BC11:

      if( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_BC11_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC11_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_RX_BC11_FREQUENCY_OFFSET * 1000) ))/1000);
      }
      else if( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_BC11_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC11_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_TX_BC11_FREQUENCY_OFFSET * 1000) ))/1000);
      }
      break;

    case RFCOM_BAND_1900:

      if( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_BC2_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC2_MAX )
      {
        chan = (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
      }
      else if( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_BC2_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC2_MAX )
      {
        chan = (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
      }
      else if( ( freq >= BSP_RF_RX_FREQ_WCDMA_BC2_ADDITIONAL_MIN ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_DL)
      {
        chan = (uint16)(( ( ( freq - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC2_REG_TO_ADDITIONAL_CH_OFFSET;
      }
      else if( ( freq >= BSP_RF_TX_FREQ_WCDMA_BC2_ADDITIONAL_MIN ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL)
      {
        chan = ((uint16)( ( ( freq - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC2_REG_TO_ADDITIONAL_CH_OFFSET;
      }
      break;

    case RFCOM_BAND_BC3:

        if( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_BC3_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC3_MAX )
        {
          chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_RX_BC3_FREQUENCY_OFFSET * 1000) ))/1000);
        }
        else if( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_BC3_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC3_MAX )
        {
          chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_TX_BC3_FREQUENCY_OFFSET * 1000) ))/1000);
        }
        break; 

    case RFCOM_BAND_BC9:
        
        if( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_BC9_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC9_MAX )
        {
          chan = (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
        }
        else if( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_BC9_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC9_MAX )
        {
          chan = (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
        }
        break;

    case RFCOM_BAND_BC4:

      if ( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_BC4_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC4_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_RX_BC4_FREQUENCY_OFFSET * 1000) ))/1000);
      }
      else if ( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_BC4_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC4_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_TX_BC4_FREQUENCY_OFFSET * 1000) ))/1000);
      }
      else if ( ( freq >= BSP_RF_RX_FREQ_WCDMA_BC4_ADDITIONAL_MIN ) &&
                ( freq <= BSP_RF_RX_FREQ_WCDMA_BC4_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        chan =((uint16) ( ( ( freq - (BSP_RF_RX_BC4_FREQUENCY_OFFSET * 1000) - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC4_REG_TO_ADDITIONAL_CH_OFFSET;
      }
      else if ( ( freq >= BSP_RF_TX_FREQ_WCDMA_BC4_ADDITIONAL_MIN ) &&
                ( freq <= BSP_RF_TX_FREQ_WCDMA_BC4_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        chan = ((uint16)( ( ( freq - (BSP_RF_TX_BC4_FREQUENCY_OFFSET * 1000) - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC4_REG_TO_ADDITIONAL_CH_OFFSET;
      }
      break;

       /* RFCN to Frequency calculation for WCDMA Band XIX */
    case RFCOM_BAND_BC19:
      if ( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_BC19_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC19_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_RX_BC19_FREQUENCY_OFFSET * 1000) ))/1000);
      }
      else if ( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_BC19_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC19_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_TX_BC19_FREQUENCY_OFFSET * 1000) ))/1000);
      }
      else if ( ( freq >= BSP_RF_RX_FREQ_WCDMA_BC19_ADDITIONAL_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC19_ADDITIONAL_MAX ) &&
                  ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        chan = ((uint16)( ( ( freq - (BSP_RF_RX_BC19_FREQUENCY_OFFSET * 1000) - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC19_REG_TO_ADDITIONAL_CH_OFFSET;
      }
      else if ( ( freq >= BSP_RF_TX_FREQ_WCDMA_BC19_ADDITIONAL_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC19_ADDITIONAL_MAX ) &&
                  ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        chan =((uint16) ( ( ( freq - (BSP_RF_TX_BC19_FREQUENCY_OFFSET * 1000) - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC19_REG_TO_ADDITIONAL_CH_OFFSET;
      }
      break;

    case RFCOM_BAND_800:

      if ( ( freq >= BSP_RF_RX_FREQ_WCDMA_BC5_MIN ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        chan =  (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
      }
      else if ( ( freq >= BSP_RF_TX_FREQ_WCDMA_BC5_MIN ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        chan =  (uint16)((freq * BSP_RF_FREQ_WCDMA_MULT_FACTOR)/1000);
        MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"Converted Chan : %d", chan);
      }
      else if ( ( freq >= BSP_RF_RX_FREQ_WCDMA_BC5_ADDITIONAL_MIN ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        chan = ((uint16)( ( ( freq - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC5_REG_TO_ADDITIONAL_CH_OFFSET;
      }
      else if ( ( freq >= BSP_RF_TX_FREQ_WCDMA_BC5_ADDITIONAL_MIN ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        chan = ((uint16)( ( ( freq - 100 ) * BSP_RF_FREQ_WCDMA_MULT_FACTOR )/1000 )) - BSP_BC5_REG_TO_ADDITIONAL_CH_OFFSET;
        MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"Converted Chan (additional) : %d", chan);
      }
      break;

    case RFCOM_BAND_BC8:

      if ( ul_dl == RFWCDMA_CORE_UARFCN_DL && freq >= BSP_RF_RX_FREQ_WCDMA_BC8_MIN && freq <= BSP_RF_RX_FREQ_WCDMA_BC8_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_RX_BC8_FREQUENCY_OFFSET * 1000) ))/1000);
      }
      else if ( ul_dl == RFWCDMA_CORE_UARFCN_UL && freq >= BSP_RF_TX_FREQ_WCDMA_BC8_MIN && freq <= BSP_RF_TX_FREQ_WCDMA_BC8_MAX )
      {
        chan = (uint16)((BSP_RF_FREQ_WCDMA_MULT_FACTOR * ( freq - (BSP_RF_TX_BC8_FREQUENCY_OFFSET * 1000) ))/1000);
      }

      break;

    default :
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid RF Band = %d", band);
  }

  return (chan);

} /* rfwcdma_core_get_uarfcn_from_freq */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function handles channel numbering schemes for different bands.

  @param band WCDMA operational band
  @param chan UARFCN number
  @param ul_dl Uplink or Downlink

  @return uint32 Tx channel frequency in Khz.
*/
uint32 rfwcdma_core_get_freq_from_uarfcn
(
  rfcom_wcdma_band_type band,
  word chan,                            /* input channel number */
  rfwcdma_core_uarfcn_ul_dl_type ul_dl
)
{
  uint32 freq = BSP_RF_TX_CHAN_WCDMA_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;

  if ( chan == 0 )
  {
    return (0);
  }

  switch ( band )
  {
    case RFCOM_BAND_IMT:

      freq = BSP_RF_TX_CHAN_WCDMA_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;

      if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_MIN &&
             chan <= BSP_RF_RX_CHAN_WCDMA_MAX ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq =  chan * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_MIN  &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq =  chan * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else
      {
        switch (ul_dl)
        {
          case RFWCDMA_CORE_UARFCN_DL:
            freq = BSP_RF_RX_CHAN_WCDMA_BC1_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC1 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
          case RFWCDMA_CORE_UARFCN_UL:
            freq = BSP_RF_TX_CHAN_WCDMA_BC1_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC1 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
        }
      }

      break;

    case RFCOM_BAND_1900:

      if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC2_MIN &&
             chan <= BSP_RF_RX_CHAN_WCDMA_BC2_MAX ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq =  chan * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC2_MIN ) &&
                ( chan <= BSP_RF_TX_CHAN_WCDMA_BC2_MAX ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq =  chan * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC2_ADDITIONAL_MIN &&
                  chan <= BSP_RF_RX_CHAN_WCDMA_BC2_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_DL)
      {
        freq = ( ( chan + BSP_BC2_REG_TO_ADDITIONAL_CH_OFFSET ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC2_ADDITIONAL_MIN &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_BC2_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL)
      {
        freq = ( ( chan + BSP_BC2_REG_TO_ADDITIONAL_CH_OFFSET ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100;
      }
      else
      {
        switch (ul_dl)
        {
          case RFWCDMA_CORE_UARFCN_DL:
            freq = BSP_RF_RX_CHAN_WCDMA_BC2_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC2 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
          case RFWCDMA_CORE_UARFCN_UL:
            freq = BSP_RF_TX_CHAN_WCDMA_BC2_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC2 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
        }
      }

      break;

    case RFCOM_BAND_BC3:
    case RFCOM_BAND_BC9:
      /* rfnv_1800.c needs to call this function for the tx/rx cal channel
       * lists, and does not know in advance whether the phone supports BC3
       * or BC9. So we map them to the same case and use the rfwcdma_core_uses_bc3 flag
       */
      
        if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC3_MIN &&
               chan <= BSP_RF_RX_CHAN_WCDMA_BC3_MAX ) &&
             ul_dl == RFWCDMA_CORE_UARFCN_DL )
        {
          freq = ( chan + ( 5 * BSP_RF_RX_BC3_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
        }
        else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC3_MIN &&
                    chan <= BSP_RF_TX_CHAN_WCDMA_BC3_MAX ) &&
                  ul_dl == RFWCDMA_CORE_UARFCN_UL )
        {
          freq = ( chan + ( 5 * BSP_RF_TX_BC3_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
        }

		else if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC9_MIN &&
               chan <= BSP_RF_RX_CHAN_WCDMA_BC9_MAX ) &&
             ul_dl == RFWCDMA_CORE_UARFCN_DL )
        {
          freq = chan * BSP_RF_FREQ_WCDMA_CH_STEP;
        }

        else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC9_MIN &&
                    chan <= BSP_RF_TX_CHAN_WCDMA_BC9_MAX ) &&
                  ul_dl == RFWCDMA_CORE_UARFCN_UL )
        {
          freq = chan * BSP_RF_FREQ_WCDMA_CH_STEP;
        }
		
        else
        {
          switch (ul_dl)
          {
            case RFWCDMA_CORE_UARFCN_DL:

				if(band==RFCOM_BAND_BC3)
					{
              freq = ( BSP_RF_RX_CHAN_WCDMA_BC3_MIN + ( 5 * BSP_RF_RX_BC3_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
              MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC3 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
              break;
					}

				else
					{
              freq = BSP_RF_RX_CHAN_WCDMA_BC9_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
              MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC9 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
              break;
					}
					
            case RFWCDMA_CORE_UARFCN_UL:
				
				if(band==RFCOM_BAND_BC3)
					{
              freq = ( BSP_RF_TX_CHAN_WCDMA_BC3_MIN + ( 5 * BSP_RF_TX_BC3_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
              MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC3 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
              break;
					}
				else
					{
              freq = BSP_RF_TX_CHAN_WCDMA_BC9_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
              MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC9 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
              break;}
					
          }
        }
      break;

      
       
        


    case RFCOM_BAND_BC4:

      if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC4_MIN &&
             chan <= BSP_RF_RX_CHAN_WCDMA_BC4_MAX ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq = ( chan + ( 5 * BSP_RF_RX_BC4_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC4_MIN &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_BC4_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq = ( chan + ( 5 * BSP_RF_TX_BC4_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC4_ADDITIONAL_MIN &&
                  chan <= BSP_RF_RX_CHAN_WCDMA_BC4_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq = ( ( ( chan + BSP_BC4_REG_TO_ADDITIONAL_CH_OFFSET ) + (5 * BSP_RF_RX_BC4_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100; /*lint !e732*/
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC4_ADDITIONAL_MIN &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_BC4_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq = ( ( ( chan + BSP_BC4_REG_TO_ADDITIONAL_CH_OFFSET ) + (5 * BSP_RF_TX_BC4_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100; /*lint !e732*/
      }
      else
      {
        switch (ul_dl)
        {
          case RFWCDMA_CORE_UARFCN_DL:
            freq = ( BSP_RF_RX_CHAN_WCDMA_BC4_MIN + ( 5 * BSP_RF_RX_BC4_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC4 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
          case RFWCDMA_CORE_UARFCN_UL:
            freq = ( BSP_RF_TX_CHAN_WCDMA_BC4_MIN + ( 5 * BSP_RF_TX_BC4_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC4 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
        }
      }

      break;

    /* RFCN to Frequency calculation for WCDMA Band_800(Band5) and Band 19 */
    case RFCOM_BAND_800:
    case RFCOM_BAND_BC19:
      
      freq = BSP_RF_TX_CHAN_WCDMA_BC5_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;

      if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC5_MIN &&
             chan <= BSP_RF_RX_CHAN_WCDMA_BC5_MAX ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq =  chan * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC5_MIN &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_BC5_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq =  chan * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC5_ADDITIONAL_MIN &&
                  chan <= BSP_RF_RX_CHAN_WCDMA_BC5_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq = ( ( chan + BSP_BC5_REG_TO_ADDITIONAL_CH_OFFSET ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100; /*lint !e732*/
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC5_ADDITIONAL_MIN &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_BC5_ADDITIONAL_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq = ( ( chan + BSP_BC5_REG_TO_ADDITIONAL_CH_OFFSET ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100; /*lint !e732*/
      }

      else if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC19_MIN && chan <= BSP_RF_RX_CHAN_WCDMA_BC19_MAX ) &&
             ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq = ( chan + ( 5 * BSP_RF_RX_BC19_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC19_MIN && chan <= BSP_RF_TX_CHAN_WCDMA_BC19_MAX ) &&
                  ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq = ( chan + ( 5 * BSP_RF_TX_BC19_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC19_ADDITIONAL_MIN && chan <= BSP_RF_RX_CHAN_WCDMA_BC19_ADDITIONAL_MAX ) &&
                  ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq = ( ( ( chan - BSP_BC19_REG_TO_ADDITIONAL_CH_OFFSET ) + (5 * BSP_RF_RX_BC19_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100; /*lint !e732*/
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC19_ADDITIONAL_MIN && chan <= BSP_RF_TX_CHAN_WCDMA_BC19_ADDITIONAL_MAX ) &&
                  ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq = ( ( ( chan - BSP_BC19_REG_TO_ADDITIONAL_CH_OFFSET ) + (5 * BSP_RF_TX_BC19_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP ) + 100; /*lint !e732*/
      }
      else
      {
        switch (ul_dl)
        {
          case RFWCDMA_CORE_UARFCN_DL:

                   if(band==RFCOM_BAND_800)
                   {
            freq = BSP_RF_RX_CHAN_WCDMA_BC5_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC5 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
                   }

                 else
                  {
            freq = ( BSP_RF_RX_CHAN_WCDMA_BC19_MIN + ( 5 * BSP_RF_RX_BC19_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC19 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
                  }


        
          case RFWCDMA_CORE_UARFCN_UL:
                if(band==RFCOM_BAND_800)
                   {
            freq = BSP_RF_TX_CHAN_WCDMA_BC5_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC5 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
                   }   

                else  
                   {
            freq = ( BSP_RF_TX_CHAN_WCDMA_BC19_MIN + ( 5 * BSP_RF_TX_BC19_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC19 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
        }
            
        }
      }
      break;

  
    case RFCOM_BAND_BC8:
      freq = BSP_RF_TX_CHAN_WCDMA_BC8_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;

      if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC8_MIN &&
             chan <= BSP_RF_RX_CHAN_WCDMA_BC8_MAX ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq = ( chan + ( 5 * BSP_RF_RX_BC8_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC8_MIN &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_BC8_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq = ( chan + ( 5 * BSP_RF_TX_BC8_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else
      {
        switch (ul_dl)
        {
          case RFWCDMA_CORE_UARFCN_DL:
            freq = ( BSP_RF_RX_CHAN_WCDMA_BC8_MIN + ( 5 * BSP_RF_RX_BC8_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC8 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
          case RFWCDMA_CORE_UARFCN_UL:
            freq = ( BSP_RF_TX_CHAN_WCDMA_BC8_MIN + ( 5 * BSP_RF_TX_BC8_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC8 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
        }
      }

      break;

case RFCOM_BAND_BC11:
      freq = BSP_RF_TX_CHAN_WCDMA_BC11_MIN * BSP_RF_FREQ_WCDMA_CH_STEP;

      if ( ( chan >= BSP_RF_RX_CHAN_WCDMA_BC11_MIN &&
             chan <= BSP_RF_RX_CHAN_WCDMA_BC11_MAX ) &&
           ul_dl == RFWCDMA_CORE_UARFCN_DL )
      {
        freq = ( chan + ( 5 * BSP_RF_RX_BC11_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else if ( ( chan >= BSP_RF_TX_CHAN_WCDMA_BC11_MIN &&
                  chan <= BSP_RF_TX_CHAN_WCDMA_BC11_MAX ) &&
                ul_dl == RFWCDMA_CORE_UARFCN_UL )
      {
        freq = ( chan + ( 5 * BSP_RF_TX_BC11_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
      }
      else
      {
        switch (ul_dl)
        {
          case RFWCDMA_CORE_UARFCN_DL:
            freq = ( BSP_RF_RX_CHAN_WCDMA_BC11_MIN + ( 5 * BSP_RF_RX_BC11_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC11 RX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
          case RFWCDMA_CORE_UARFCN_UL:
            freq = ( BSP_RF_TX_CHAN_WCDMA_BC11_MIN + ( 5 * BSP_RF_TX_BC11_FREQUENCY_OFFSET ) ) * BSP_RF_FREQ_WCDMA_CH_STEP;
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"BC11 TX invalid channel %d, freq %d.%d MHz", chan, freq/1000, freq%1000);
            break;
        }
      }

      break;

    default :
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid RF Band = %d", band);
  }

  return (freq);

} /* rfwcdma_core_get_freq_from_uarfcn */