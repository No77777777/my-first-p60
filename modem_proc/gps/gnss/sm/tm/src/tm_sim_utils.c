/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

   TM UMTS_UP SUPL module interface with PDCOMM.


GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2000 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2018 Qualcomm Technologies, Inc.
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_sim_utils.c#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  03/09/18   py      Remove "FEATURE_MMGSDI_SESSION_LIB" dependency. True always.
  01/04/17   sum     Removed definition of tm_sim_mmgsdi_init() , tm_sim_mmgsdi_client_reg_cb() , tm_sim_mmgsdi_session_reg_cb() and tm_sim_mmgsdi_evt_cb()
  12/23/13   mj	 Remove FEATURE_DUAL_SIM and FEATURE_TRIPLE_SIM
  09/12/13    mj     Send app_type to function tm_mmgsdi_get_sim_info_from_asid
  12/14/09    gk     send comm failure event
  04/06/11    lt     ASN1 tool migration.
  03/03/10    ns     DSDS Support - read cert from efs if new cert 
                     injected via pdapi,also read imsi from mmgsdi module
  12/14/09    gk     send comm failure event
  11/02/09   atien   additional fix for the case where one segmented MMS msg 
                     wipes out previously stored segmented SUPL-INIT msg in 
                     local WAP PUSH buffer
 09/25/09   atien   Fix problem with dropping SUPL-Init msg while processing
                     multi-segmented non-SUPL msg
===========================================================================*/
#include "gps_variation.h"
#include "customer.h"   /* Customer configuration file */

#include "tm_sim_utils.h"
#include "ghdi_exp.h"
#if (defined(FEATURE_CGPS_UMTS_UP_SUPL) || defined(FEATURE_CGPS_XTRA_T)) 

/*==========================================================================

                   Include Files

===========================================================================*/

#include "tm_umts_common_utils.h"
#include "tm_data.h"
#define MNC_LEN_OFFSET                      0x03

#endif /* FEATURE_CGPS_UMTS_UP_SUPL || FEATURE_CGPS_XTRA_T */


/********************************************************************
*
* tm_sim_retrieve_imsi
*
* Function description:
*  Retrieve the IMSI value from the the SIM
*
* Parameters: 
*  pz_IMSI - The retrieved IMSI to return to the caller (output)
*
* Return:
*  TRUE/FALSE
*  
*********************************************************************
*/
boolean tm_sim_retrieve_imsi( cgps_IMSIType * const pz_IMSI )
{
#if (defined(FEATURE_CGPS_UMTS_UP_SUPL) || defined(FEATURE_CGPS_XTRA_T))
  byte pbyAD[MNC_LEN_OFFSET + 1];
  byte   pbyIMSI[MMGSDI_IMSI_LEN];
  boolean mmgsdi_status = FALSE;
  mmgsdi_file_enum_type sim_file_name_imsi = MMGSDI_USIM_IMSI;
  mmgsdi_file_enum_type sim_file_name_ad = MMGSDI_USIM_AD;
  mmgsdi_session_id_type mmgsdi_session_id;
  mmgsdi_app_enum_type mmgsdi_app_type = MMGSDI_APP_UNKNOWN;
  mmgsdi_return_enum_type mmgsdi_return_status;
  mmgsdi_data_type read_data;
  
  sys_modem_as_id_e_type      asubs_id = SYS_MODEM_AS_ID_NONE;  

  uint8 i;
  uint8 u_numMncDigits = 0;
  uint8 u_imsi[MAX_IMSI_LEN_BYTES];

  memset(u_imsi, 0, sizeof(u_imsi));
  memset(pbyAD, 0, sizeof(pbyAD));
  memset(pbyIMSI, 0, sizeof(pbyIMSI));

  /* Get DD subscription ID */
  asubs_id = (sys_modem_as_id_e_type) pz_IMSI->sub;  

  /* Get card mode and session ID for DD subs*/
  if(asubs_id != SYS_MODEM_AS_ID_NONE)
  {
    mmgsdi_status =  tm_mmgsdi_get_sim_info_from_asid(asubs_id,&mmgsdi_session_id, &mmgsdi_app_type, MMGSDI_APP_USIM);
    
	if (mmgsdi_status == FALSE)
	{
	  /* If USIM app_type is not found then try SIM app_type */
	  mmgsdi_status =  tm_mmgsdi_get_sim_info_from_asid(asubs_id,&mmgsdi_session_id,&mmgsdi_app_type, MMGSDI_APP_SIM);
	  
	}
  }

  if(mmgsdi_status == TRUE)
  {
    /* Get the sim file name based on the sim mode */
    if( mmgsdi_app_type == MMGSDI_APP_SIM)
    {
      sim_file_name_imsi = MMGSDI_GSM_IMSI;
      sim_file_name_ad = MMGSDI_GSM_AD;
    }
    else if( mmgsdi_app_type == MMGSDI_APP_USIM)
    {
      sim_file_name_imsi = MMGSDI_USIM_IMSI;
      sim_file_name_ad = MMGSDI_USIM_AD;
    }
    if ( (sim_file_name_imsi == MMGSDI_GSM_IMSI) || (sim_file_name_imsi == MMGSDI_USIM_IMSI) )
    {
      read_data.data_ptr = pbyIMSI;
      read_data.data_len = MMGSDI_IMSI_LEN;
      mmgsdi_return_status = mmgsdi_session_read_cache(mmgsdi_session_id,
                                                     sim_file_name_imsi,
                                                     read_data);

      if (mmgsdi_return_status == MMGSDI_SUCCESS )
      {
        /* pbyIMSI :

           pbyIMSI[3] : Digit 5  |   Digit 4
           pbyIMSI[2] : Digit 3  |   Digit 2
           pbyIMSI[1] : Digit 1  |   Even/Odd num. of digit after Digit1 indicator (9: even; other: odd)
           pbyIMSI[0] : number pbyIMSI elememnts containing IMSI info. -- count as 1 even if only a nibble used.

           eg. IMSI 12345
           pbyIMSI[3] : 0101 0100 
           pbyIMSI[2] : 0011 0010
           pbyIMSI[1] : 0001 1001    
           pbyIMSI[0] : 0000 0011 

           eg. IMSI 123456
           pbyIMSI[4] : xxxx 0110
           pbyIMSI[3] : 0101 0100 
           pbyIMSI[2] : 0011 0010
           pbyIMSI[1] : 0001 yyyy  : yyyy - anything but 1001    
           pbyIMSI[0] : 0000 0100 

        */

        /* eg. IMSI 123456789012345: Left most is Digit 1, right most is digit 15, populate
           the stored IMSI as follows:

           u_imsi[0]= 0x21;
           u_imsi[1]= 0x43;
           u_imsi[2]= 0x65;

           ...
           u_imsi[8]= 0xF5;  */

        if ( ( pbyIMSI[0] <= 8 ) && ( pbyIMSI[0] > 0 ) )
        {
          uint32 i,j;

          memset( u_imsi, 0xFF, MAX_IMSI_LEN_BYTES);

          j=0;
          u_imsi[j] = (pbyIMSI[1] & 0xF0)>>4;

          if ( pbyIMSI[0] == 1 )
          {
            u_imsi[j] |= 0xF0;
          }

          else if ( pbyIMSI[0] == 2 )
          {
            u_imsi[j++] |= (pbyIMSI[2] & 0x0F) << 4;

            if ( (pbyIMSI[1] & 0x0F) == 0x9)
            {
              u_imsi[j] = (pbyIMSI[2] & 0xF0)>>4;
              u_imsi[j] |= 0xF0;
            }

          }

          else if(pbyIMSI[0] <= 8) /*if check added Just to make link happy*/
          {

            for ( i = 2; i < pbyIMSI[0]; i++)
            {
              u_imsi[j++] |= (pbyIMSI[i] & 0x0F) << 4;
              u_imsi[j] = (pbyIMSI[i] & 0xF0)>>4;
            }

            if (( i < MMGSDI_IMSI_LEN ) && (j < MAX_IMSI_LEN_BYTES-1))
            {
              if ( (pbyIMSI[1] & 0x0F) == 0x9)
              {
                u_imsi[j++] |= (pbyIMSI[i] & 0x0F) << 4;
                u_imsi[j] = (pbyIMSI[i] & 0xF0)>>4;
                u_imsi[j] |= 0xF0;
              }
              else
              {
                u_imsi[j++] |= (pbyIMSI[i] & 0x0F) << 4;
              }
            }

          }

          //p_supl_session_status->z_session_id.z_Set_Sess_Id.z_Set_Id.set_id_choice = supl_imsi_chosen;

        } /* IF number of ISMI digits within limit */

      } /* IF SIM read successful */

      read_data.data_ptr = pbyAD;
      read_data.data_len = MNC_LEN_OFFSET+1;
      mmgsdi_return_status = mmgsdi_session_read_cache(mmgsdi_session_id,
                                                     sim_file_name_ad,
                                                     read_data);

      if (mmgsdi_return_status == MMGSDI_SUCCESS )
      { 
        /* pbyAD :

        pbyAD[3] : RFU  |   the MNC length
        pbyAD[2] : Digit 3  |   Digit 2
        */
        u_numMncDigits = (uint8) ( pbyAD[MNC_LEN_OFFSET] & 0xF);
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"The length of the MNC is %d",u_numMncDigits,0,0);

      }

    } /* close if-GSM-or-USIM */

  } /* close if-SIM-initialized-and-valid */
  else
  {
    return FALSE;
  }


  /* If the IMSI value is null return failure */
  if (pbyIMSI[0] == '\0')
  {
      return FALSE;
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW,"Retrieved IMSI for sub:",pz_IMSI->sub,0,0);
    for(i = 0; i < MAX_IMSI_LEN_BYTES; i++)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW,"  IMSI[%d]: %d",i,u_imsi[i],0);
    }
    
    /* Update caller with the updated IMSI */
    memscpy (pz_IMSI->imsi, sizeof(pz_IMSI->imsi), u_imsi, sizeof(u_imsi));
    pz_IMSI->numMNCdigits = u_numMncDigits;
        
    return TRUE;
  }
#else /* FEATURE_CGPS_UMTS_UP_SUPL || FEATURE_CGPS_XTRA_T */
  return FALSE;
#endif /* FEATURE_CGPS_UMTS_UP_SUPL || FEATURE_CGPS_XTRA_T */
}

/********************************************************************
*
* tm_get_imei
*
* Function description:
*  Gets the IMEI value from the GHSDI
*
* Parameters: 
*  p_imei_ptr - The retrieved IMEI to return to the caller (output)
*
* Return:
*  TRUE/FALSE
*  
*********************************************************************
*/  
boolean tm_get_imei(byte *p_imei_ptr)
{
  //nv_item_type itemBuffer;
  ghdi_status_T  status = GHDI_FAILURE;
#ifdef FEATURE_DUAL_SIM
  uint16 w_sub;
  w_sub = (uint16) tm_core_get_current_dd_subs_id();
  status = ghdi_ds_read_imei(p_imei_ptr, w_sub);
#else
  //uint16 w_sub;  /* Removing set-but-not-used variable */
  //w_sub = 0; /* default to subscription 0 as DDS */
  status = ghdi_read_imei(p_imei_ptr);
#endif

  if (status != GHDI_SUCCESS)
  {
    return FALSE;  
  }
  return TRUE;
}