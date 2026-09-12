/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             GPS Rom Almanac Module

GENERAL DESCRIPTION
  This module contains Default Rom Almanac helper function that is used 
  within the GPS sub-system.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2005-2012 by Qualcomm Technologies INCORPORATED. All Rights Reserved.
  Copyright (c) 2013-2019 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

$Header: //components/rel/gnss8.mpss/11.1/gnss/common/src/gps_rom_almanac.c#1 $ 
  $DateTime: 2019/10/24 01:30:36 $ 
  $Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
2019-10-07          From: xtra3grc_2019ic.txt
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*
 * Include files
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "customer.h"
#include "comdef.h"
#include "gnss_common.h"
#include "err.h"
#include "msg.h"

/*
 * Constant definitions
*/

/*
 * Local function prototypes
*/


/*
 ******************************************************************************
 * gps_GetGpsRomAlmanac
 *
 * Function description:
 *
 * 	gps_GetGpsRomAlmanac is a helper function used to retreive the default GPS
 *	ROM almanac.
 *
 * Parameters: 
 *
 *  pz_GpsRomAlm - Pointer to the GPS ROM almanac structure
 *
 * Return value: 
 *
 *  void
 *
 ******************************************************************************
*/

void gps_GetGpsRomAlmanac( gps_AlmStructType **pz_RomAlm )
{

  if ( (NULL == pz_RomAlm) || (NULL == *pz_RomAlm) )
  {    
    MSG(MSG_SSID_MGPPE,MSG_LEGACY_ERROR, 
        "gps_GetGpsRomAlmanac: NULL input pointer");
    return;
  }
  pz_RomAlm[0]->u_Sv = 1 ;
  pz_RomAlm[0]->u_Health = 0 ;
  pz_RomAlm[0]->u_Toa = 78 ;
  pz_RomAlm[0]->w_E = 19113 ;
  pz_RomAlm[0]->w_DeltaI = 5781 ;
  pz_RomAlm[0]->w_OmegaDot = 64833 ;
  pz_RomAlm[0]->q_SqrtA = 10554557ul ;
  pz_RomAlm[0]->q_Omega0 = 2465043ul ;
  pz_RomAlm[0]->q_Omega = 2047747ul ;
  pz_RomAlm[0]->q_M0 = 11429124ul ;
  pz_RomAlm[0]->w_Af0 = 1884 ;
  pz_RomAlm[0]->w_Af1 = 2044 ;
  pz_RomAlm[0]->w_GpsWeek = 2074 ;

  pz_RomAlm[1]->u_Sv = 2 ;
  pz_RomAlm[1]->u_Health = 0 ;
  pz_RomAlm[1]->u_Toa = 78 ;
  pz_RomAlm[1]->w_E = 40284 ;
  pz_RomAlm[1]->w_DeltaI = 2299 ;
  pz_RomAlm[1]->w_OmegaDot = 64820 ;
  pz_RomAlm[1]->q_SqrtA = 10554357ul ;
  pz_RomAlm[1]->q_Omega0 = 2274054ul ;
  pz_RomAlm[1]->q_Omega = 12211437ul ;
  pz_RomAlm[1]->q_M0 = 12187872ul ;
  pz_RomAlm[1]->w_Af0 = 1710 ;
  pz_RomAlm[1]->w_Af1 = 2046 ;
  pz_RomAlm[1]->w_GpsWeek = 2074 ;

  pz_RomAlm[2]->u_Sv = 3 ;
  pz_RomAlm[2]->u_Health = 0 ;
  pz_RomAlm[2]->u_Toa = 78 ;
  pz_RomAlm[2]->w_E = 5371 ;
  pz_RomAlm[2]->w_DeltaI = 3530 ;
  pz_RomAlm[2]->w_OmegaDot = 64844 ;
  pz_RomAlm[2]->q_SqrtA = 10554669ul ;
  pz_RomAlm[2]->q_Omega0 = 5247864ul ;
  pz_RomAlm[2]->q_Omega = 2059445ul ;
  pz_RomAlm[2]->q_M0 = 8370903ul ;
  pz_RomAlm[2]->w_Af0 = 2027 ;
  pz_RomAlm[2]->w_Af1 = 2047 ;
  pz_RomAlm[2]->w_GpsWeek = 2074 ;

  /* No almanac for PRN 4 */
  pz_RomAlm[3]->u_Sv = C_GPS_SV_ALM_INVALID ;

  pz_RomAlm[4]->u_Sv = 5 ;
  pz_RomAlm[4]->u_Health = 0 ;
  pz_RomAlm[4]->u_Toa = 78 ;
  pz_RomAlm[4]->w_E = 12384 ;
  pz_RomAlm[4]->w_DeltaI = 1338 ;
  pz_RomAlm[4]->w_OmegaDot = 64832 ;
  pz_RomAlm[4]->q_SqrtA = 10554829ul ;
  pz_RomAlm[4]->q_Omega0 = 5171207ul ;
  pz_RomAlm[4]->q_Omega = 2137516ul ;
  pz_RomAlm[4]->q_M0 = 2279168ul ;
  pz_RomAlm[4]->w_Af0 = 2046 ;
  pz_RomAlm[4]->w_Af1 = 0 ;
  pz_RomAlm[4]->w_GpsWeek = 2074 ;

  pz_RomAlm[5]->u_Sv = 6 ;
  pz_RomAlm[5]->u_Health = 0 ;
  pz_RomAlm[5]->u_Toa = 78 ;
  pz_RomAlm[5]->w_E = 2961 ;
  pz_RomAlm[5]->w_DeltaI = 5718 ;
  pz_RomAlm[5]->w_OmegaDot = 64833 ;
  pz_RomAlm[5]->q_SqrtA = 10554613ul ;
  pz_RomAlm[5]->q_Omega0 = 2442959ul ;
  pz_RomAlm[5]->q_Omega = 13622111ul ;
  pz_RomAlm[5]->q_M0 = 12424408ul ;
  pz_RomAlm[5]->w_Af0 = 1959 ;
  pz_RomAlm[5]->w_Af1 = 2044 ;
  pz_RomAlm[5]->w_GpsWeek = 2074 ;

  pz_RomAlm[6]->u_Sv = 7 ;
  pz_RomAlm[6]->u_Health = 0 ;
  pz_RomAlm[6]->u_Toa = 78 ;
  pz_RomAlm[6]->w_E = 27616 ;
  pz_RomAlm[6]->w_DeltaI = 2010 ;
  pz_RomAlm[6]->w_OmegaDot = 64822 ;
  pz_RomAlm[6]->q_SqrtA = 10554711ul ;
  pz_RomAlm[6]->q_Omega0 = 10872973ul ;
  pz_RomAlm[6]->q_Omega = 10298993ul ;
  pz_RomAlm[6]->q_M0 = 9687486ul ;
  pz_RomAlm[6]->w_Af0 = 1918 ;
  pz_RomAlm[6]->w_Af1 = 2046 ;
  pz_RomAlm[6]->w_GpsWeek = 2074 ;

  pz_RomAlm[7]->u_Sv = 8 ;
  pz_RomAlm[7]->u_Health = 0 ;
  pz_RomAlm[7]->u_Toa = 78 ;
  pz_RomAlm[7]->w_E = 9473 ;
  pz_RomAlm[7]->w_DeltaI = 4684 ;
  pz_RomAlm[7]->w_OmegaDot = 64861 ;
  pz_RomAlm[7]->q_SqrtA = 10554417ul ;
  pz_RomAlm[7]->q_Omega0 = 16392224ul ;
  pz_RomAlm[7]->q_Omega = 16154055ul ;
  pz_RomAlm[7]->q_M0 = 1022667ul ;
  pz_RomAlm[7]->w_Af0 = 2038 ;
  pz_RomAlm[7]->w_Af1 = 0 ;
  pz_RomAlm[7]->w_GpsWeek = 2074 ;

  pz_RomAlm[8]->u_Sv = 9 ;
  pz_RomAlm[8]->u_Health = 0 ;
  pz_RomAlm[8]->u_Toa = 78 ;
  pz_RomAlm[8]->w_E = 3131 ;
  pz_RomAlm[8]->w_DeltaI = 1757 ;
  pz_RomAlm[8]->w_OmegaDot = 64856 ;
  pz_RomAlm[8]->q_SqrtA = 10554443ul ;
  pz_RomAlm[8]->q_Omega0 = 8002445ul ;
  pz_RomAlm[8]->q_Omega = 4467882ul ;
  pz_RomAlm[8]->q_M0 = 1759700ul ;
  pz_RomAlm[8]->w_Af0 = 1993 ;
  pz_RomAlm[8]->w_Af1 = 2045 ;
  pz_RomAlm[8]->w_GpsWeek = 2074 ;

  pz_RomAlm[9]->u_Sv = 10 ;
  pz_RomAlm[9]->u_Health = 0 ;
  pz_RomAlm[9]->u_Toa = 78 ;
  pz_RomAlm[9]->w_E = 10313 ;
  pz_RomAlm[9]->w_DeltaI = 3528 ;
  pz_RomAlm[9]->w_OmegaDot = 64842 ;
  pz_RomAlm[9]->q_SqrtA = 10554604ul ;
  pz_RomAlm[9]->q_Omega0 = 5239412ul ;
  pz_RomAlm[9]->q_Omega = 9457524ul ;
  pz_RomAlm[9]->q_M0 = 6151296ul ;
  pz_RomAlm[9]->w_Af0 = 1957 ;
  pz_RomAlm[9]->w_Af1 = 2044 ;
  pz_RomAlm[9]->w_GpsWeek = 2074 ;

  pz_RomAlm[10]->u_Sv = 11 ;
  pz_RomAlm[10]->u_Health = 0 ;
  pz_RomAlm[10]->u_Toa = 78 ;
  pz_RomAlm[10]->w_E = 34812 ;
  pz_RomAlm[10]->w_DeltaI = 60275 ;
  pz_RomAlm[10]->w_OmegaDot = 64795 ;
  pz_RomAlm[10]->q_SqrtA = 10554718ul ;
  pz_RomAlm[10]->q_Omega0 = 1238043ul ;
  pz_RomAlm[10]->q_Omega = 5222721ul ;
  pz_RomAlm[10]->q_M0 = 9358022ul ;
  pz_RomAlm[10]->w_Af0 = 1541 ;
  pz_RomAlm[10]->w_Af1 = 3 ;
  pz_RomAlm[10]->w_GpsWeek = 2074 ;

  pz_RomAlm[11]->u_Sv = 12 ;
  pz_RomAlm[11]->u_Health = 0 ;
  pz_RomAlm[11]->u_Toa = 78 ;
  pz_RomAlm[11]->w_E = 15692 ;
  pz_RomAlm[11]->w_DeltaI = 6424 ;
  pz_RomAlm[11]->w_OmegaDot = 64864 ;
  pz_RomAlm[11]->q_SqrtA = 10554418ul ;
  pz_RomAlm[11]->q_Omega0 = 13818186ul ;
  pz_RomAlm[11]->q_Omega = 2946006ul ;
  pz_RomAlm[11]->q_M0 = 7992386ul ;
  pz_RomAlm[11]->w_Af0 = 205 ;
  pz_RomAlm[11]->w_Af1 = 2047 ;
  pz_RomAlm[11]->w_GpsWeek = 2074 ;

  pz_RomAlm[12]->u_Sv = 13 ;
  pz_RomAlm[12]->u_Health = 0 ;
  pz_RomAlm[12]->u_Toa = 78 ;
  pz_RomAlm[12]->w_E = 8131 ;
  pz_RomAlm[12]->w_DeltaI = 4321 ;
  pz_RomAlm[12]->w_OmegaDot = 64864 ;
  pz_RomAlm[12]->q_SqrtA = 10554623ul ;
  pz_RomAlm[12]->q_Omega0 = 8374230ul ;
  pz_RomAlm[12]->q_Omega = 3203232ul ;
  pz_RomAlm[12]->q_M0 = 14916641ul ;
  pz_RomAlm[12]->w_Af0 = 2006 ;
  pz_RomAlm[12]->w_Af1 = 1 ;
  pz_RomAlm[12]->w_GpsWeek = 2074 ;

  pz_RomAlm[13]->u_Sv = 14 ;
  pz_RomAlm[13]->u_Health = 0 ;
  pz_RomAlm[13]->u_Toa = 78 ;
  pz_RomAlm[13]->w_E = 22561 ;
  pz_RomAlm[13]->w_DeltaI = 3081 ;
  pz_RomAlm[13]->w_OmegaDot = 64859 ;
  pz_RomAlm[13]->q_SqrtA = 10554792ul ;
  pz_RomAlm[13]->q_Omega0 = 8253214ul ;
  pz_RomAlm[13]->q_Omega = 11617457ul ;
  pz_RomAlm[13]->q_M0 = 16524086ul ;
  pz_RomAlm[13]->w_Af0 = 1988 ;
  pz_RomAlm[13]->w_Af1 = 1 ;
  pz_RomAlm[13]->w_GpsWeek = 2074 ;

  pz_RomAlm[14]->u_Sv = 15 ;
  pz_RomAlm[14]->u_Health = 0 ;
  pz_RomAlm[14]->u_Toa = 78 ;
  pz_RomAlm[14]->w_E = 24804 ;
  pz_RomAlm[14]->w_DeltaI = 63204 ;
  pz_RomAlm[14]->w_OmegaDot = 64835 ;
  pz_RomAlm[14]->q_SqrtA = 10554421ul ;
  pz_RomAlm[14]->q_Omega0 = 7779634ul ;
  pz_RomAlm[14]->q_Omega = 2188737ul ;
  pz_RomAlm[14]->q_M0 = 14892349ul ;
  pz_RomAlm[14]->w_Af0 = 1753 ;
  pz_RomAlm[14]->w_Af1 = 1 ;
  pz_RomAlm[14]->w_GpsWeek = 2074 ;

  pz_RomAlm[15]->u_Sv = 16 ;
  pz_RomAlm[15]->u_Health = 0 ;
  pz_RomAlm[15]->u_Toa = 78 ;
  pz_RomAlm[15]->w_E = 22718 ;
  pz_RomAlm[15]->w_DeltaI = 6476 ;
  pz_RomAlm[15]->w_OmegaDot = 64865 ;
  pz_RomAlm[15]->q_SqrtA = 10554580ul ;
  pz_RomAlm[15]->q_Omega0 = 13868669ul ;
  pz_RomAlm[15]->q_Omega = 1545591ul ;
  pz_RomAlm[15]->q_M0 = 3058559ul ;
  pz_RomAlm[15]->w_Af0 = 1962 ;
  pz_RomAlm[15]->w_Af1 = 2047 ;
  pz_RomAlm[15]->w_GpsWeek = 2074 ;

  pz_RomAlm[16]->u_Sv = 17 ;
  pz_RomAlm[16]->u_Health = 0 ;
  pz_RomAlm[16]->u_Toa = 78 ;
  pz_RomAlm[16]->w_E = 27665 ;
  pz_RomAlm[16]->w_DeltaI = 7023 ;
  pz_RomAlm[16]->w_OmegaDot = 64873 ;
  pz_RomAlm[16]->q_SqrtA = 10554453ul ;
  pz_RomAlm[16]->q_Omega0 = 16577448ul ;
  pz_RomAlm[16]->q_Omega = 12236518ul ;
  pz_RomAlm[16]->q_M0 = 288216ul ;
  pz_RomAlm[16]->w_Af0 = 148 ;
  pz_RomAlm[16]->w_Af1 = 2 ;
  pz_RomAlm[16]->w_GpsWeek = 2074 ;

  pz_RomAlm[17]->u_Sv = 18 ;
  pz_RomAlm[17]->u_Health = 0 ;
  pz_RomAlm[17]->u_Toa = 78 ;
  pz_RomAlm[17]->w_E = 32890 ;
  pz_RomAlm[17]->w_DeltaI = 2209 ;
  pz_RomAlm[17]->w_OmegaDot = 64817 ;
  pz_RomAlm[17]->q_SqrtA = 10554602ul ;
  pz_RomAlm[17]->q_Omega0 = 2310062ul ;
  pz_RomAlm[17]->q_Omega = 3845086ul ;
  pz_RomAlm[17]->q_M0 = 10693447ul ;
  pz_RomAlm[17]->w_Af0 = 96 ;
  pz_RomAlm[17]->w_Af1 = 1 ;
  pz_RomAlm[17]->w_GpsWeek = 2074 ;

  pz_RomAlm[18]->u_Sv = 19 ;
  pz_RomAlm[18]->u_Health = 0 ;
  pz_RomAlm[18]->u_Toa = 78 ;
  pz_RomAlm[18]->w_E = 19568 ;
  pz_RomAlm[18]->w_DeltaI = 6683 ;
  pz_RomAlm[18]->w_OmegaDot = 64870 ;
  pz_RomAlm[18]->q_SqrtA = 10554595ul ;
  pz_RomAlm[18]->q_Omega0 = 16700190ul ;
  pz_RomAlm[18]->q_Omega = 4041882ul ;
  pz_RomAlm[18]->q_M0 = 7634808ul ;
  pz_RomAlm[18]->w_Af0 = 1780 ;
  pz_RomAlm[18]->w_Af1 = 1 ;
  pz_RomAlm[18]->w_GpsWeek = 2074 ;

  pz_RomAlm[19]->u_Sv = 20 ;
  pz_RomAlm[19]->u_Health = 0 ;
  pz_RomAlm[19]->u_Toa = 78 ;
  pz_RomAlm[19]->w_E = 10377 ;
  pz_RomAlm[19]->w_DeltaI = 63710 ;
  pz_RomAlm[19]->w_OmegaDot = 64809 ;
  pz_RomAlm[19]->q_SqrtA = 10554499ul ;
  pz_RomAlm[19]->q_Omega0 = 4910658ul ;
  pz_RomAlm[19]->q_Omega = 6542796ul ;
  pz_RomAlm[19]->q_M0 = 10384046ul ;
  pz_RomAlm[19]->w_Af0 = 553 ;
  pz_RomAlm[19]->w_Af1 = 0 ;
  pz_RomAlm[19]->w_GpsWeek = 2074 ;

  pz_RomAlm[20]->u_Sv = 21 ;
  pz_RomAlm[20]->u_Health = 0 ;
  pz_RomAlm[20]->u_Toa = 78 ;
  pz_RomAlm[20]->w_E = 51387 ;
  pz_RomAlm[20]->w_DeltaI = 1267 ;
  pz_RomAlm[20]->w_OmegaDot = 64815 ;
  pz_RomAlm[20]->q_SqrtA = 10554544ul ;
  pz_RomAlm[20]->q_Omega0 = 2283679ul ;
  pz_RomAlm[20]->q_Omega = 13021089ul ;
  pz_RomAlm[20]->q_M0 = 6503548ul ;
  pz_RomAlm[20]->w_Af0 = 1938 ;
  pz_RomAlm[20]->w_Af1 = 2 ;
  pz_RomAlm[20]->w_GpsWeek = 2074 ;

  pz_RomAlm[21]->u_Sv = 22 ;
  pz_RomAlm[21]->u_Health = 0 ;
  pz_RomAlm[21]->u_Toa = 78 ;
  pz_RomAlm[21]->w_E = 14426 ;
  pz_RomAlm[21]->w_DeltaI = 63164 ;
  pz_RomAlm[21]->w_OmegaDot = 64810 ;
  pz_RomAlm[21]->q_SqrtA = 10554625ul ;
  pz_RomAlm[21]->q_Omega0 = 5039510ul ;
  pz_RomAlm[21]->q_Omega = 13360969ul ;
  pz_RomAlm[21]->q_M0 = 14823953ul ;
  pz_RomAlm[21]->w_Af0 = 1248 ;
  pz_RomAlm[21]->w_Af1 = 2047 ;
  pz_RomAlm[21]->w_GpsWeek = 2074 ;

  pz_RomAlm[22]->u_Sv = 23 ;
  pz_RomAlm[22]->u_Health = 0 ;
  pz_RomAlm[22]->u_Toa = 78 ;
  pz_RomAlm[22]->w_E = 27759 ;
  pz_RomAlm[22]->w_DeltaI = 223 ;
  pz_RomAlm[22]->w_OmegaDot = 64848 ;
  pz_RomAlm[22]->q_SqrtA = 10554771ul ;
  pz_RomAlm[22]->q_Omega0 = 7985367ul ;
  pz_RomAlm[22]->q_Omega = 10832927ul ;
  pz_RomAlm[22]->q_M0 = 13523946ul ;
  pz_RomAlm[22]->w_Af0 = 1878 ;
  pz_RomAlm[22]->w_Af1 = 1 ;
  pz_RomAlm[22]->w_GpsWeek = 2074 ;

  pz_RomAlm[23]->u_Sv = 24 ;
  pz_RomAlm[23]->u_Health = 0 ;
  pz_RomAlm[23]->u_Toa = 78 ;
  pz_RomAlm[23]->w_E = 18481 ;
  pz_RomAlm[23]->w_DeltaI = 64754 ;
  pz_RomAlm[23]->w_OmegaDot = 64810 ;
  pz_RomAlm[23]->q_SqrtA = 10554644ul ;
  pz_RomAlm[23]->q_Omega0 = 10690707ul ;
  pz_RomAlm[23]->q_Omega = 1629378ul ;
  pz_RomAlm[23]->q_M0 = 12178759ul ;
  pz_RomAlm[23]->w_Af0 = 2045 ;
  pz_RomAlm[23]->w_Af1 = 0 ;
  pz_RomAlm[23]->w_GpsWeek = 2074 ;

  pz_RomAlm[24]->u_Sv = 25 ;
  pz_RomAlm[24]->u_Health = 0 ;
  pz_RomAlm[24]->u_Toa = 78 ;
  pz_RomAlm[24]->w_E = 17726 ;
  pz_RomAlm[24]->w_DeltaI = 4312 ;
  pz_RomAlm[24]->w_OmegaDot = 64848 ;
  pz_RomAlm[24]->q_SqrtA = 10554572ul ;
  pz_RomAlm[24]->q_Omega0 = 13641867ul ;
  pz_RomAlm[24]->q_Omega = 2380489ul ;
  pz_RomAlm[24]->q_M0 = 6945268ul ;
  pz_RomAlm[24]->w_Af0 = 2032 ;
  pz_RomAlm[24]->w_Af1 = 0 ;
  pz_RomAlm[24]->w_GpsWeek = 2074 ;

  pz_RomAlm[25]->u_Sv = 26 ;
  pz_RomAlm[25]->u_Health = 0 ;
  pz_RomAlm[25]->u_Toa = 78 ;
  pz_RomAlm[25]->w_E = 8380 ;
  pz_RomAlm[25]->w_DeltaI = 1175 ;
  pz_RomAlm[25]->w_OmegaDot = 64831 ;
  pz_RomAlm[25]->q_SqrtA = 10554624ul ;
  pz_RomAlm[25]->q_Omega0 = 13558926ul ;
  pz_RomAlm[25]->q_Omega = 315320ul ;
  pz_RomAlm[25]->q_M0 = 5577271ul ;
  pz_RomAlm[25]->w_Af0 = 61 ;
  pz_RomAlm[25]->w_Af1 = 2 ;
  pz_RomAlm[25]->w_GpsWeek = 2074 ;

  pz_RomAlm[26]->u_Sv = 27 ;
  pz_RomAlm[26]->u_Health = 0 ;
  pz_RomAlm[26]->u_Toa = 78 ;
  pz_RomAlm[26]->w_E = 15183 ;
  pz_RomAlm[26]->w_DeltaI = 6077 ;
  pz_RomAlm[26]->w_OmegaDot = 64868 ;
  pz_RomAlm[26]->q_SqrtA = 10554601ul ;
  pz_RomAlm[26]->q_Omega0 = 16425386ul ;
  pz_RomAlm[26]->q_Omega = 1361708ul ;
  pz_RomAlm[26]->q_M0 = 505596ul ;
  pz_RomAlm[26]->w_Af0 = 1962 ;
  pz_RomAlm[26]->w_Af1 = 2045 ;
  pz_RomAlm[26]->w_GpsWeek = 2074 ;

  pz_RomAlm[27]->u_Sv = 28 ;
  pz_RomAlm[27]->u_Health = 0 ;
  pz_RomAlm[27]->u_Toa = 78 ;
  pz_RomAlm[27]->w_E = 40095 ;
  pz_RomAlm[27]->w_DeltaI = 6305 ;
  pz_RomAlm[27]->w_OmegaDot = 64862 ;
  pz_RomAlm[27]->q_SqrtA = 10554698ul ;
  pz_RomAlm[27]->q_Omega0 = 13879035ul ;
  pz_RomAlm[27]->q_Omega = 12898246ul ;
  pz_RomAlm[27]->q_M0 = 2611134ul ;
  pz_RomAlm[27]->w_Af0 = 797 ;
  pz_RomAlm[27]->w_Af1 = 0 ;
  pz_RomAlm[27]->w_GpsWeek = 2074 ;

  pz_RomAlm[28]->u_Sv = 29 ;
  pz_RomAlm[28]->u_Health = 0 ;
  pz_RomAlm[28]->u_Toa = 78 ;
  pz_RomAlm[28]->w_E = 2422 ;
  pz_RomAlm[28]->w_DeltaI = 7279 ;
  pz_RomAlm[28]->w_OmegaDot = 64875 ;
  pz_RomAlm[28]->q_SqrtA = 10554663ul ;
  pz_RomAlm[28]->q_Omega0 = 16607758ul ;
  pz_RomAlm[28]->q_Omega = 5442030ul ;
  pz_RomAlm[28]->q_M0 = 1008299ul ;
  pz_RomAlm[28]->w_Af0 = 80 ;
  pz_RomAlm[28]->w_Af1 = 2045 ;
  pz_RomAlm[28]->w_GpsWeek = 2074 ;

  pz_RomAlm[29]->u_Sv = 30 ;
  pz_RomAlm[29]->u_Health = 0 ;
  pz_RomAlm[29]->u_Toa = 78 ;
  pz_RomAlm[29]->w_E = 8767 ;
  pz_RomAlm[29]->w_DeltaI = 65108 ;
  pz_RomAlm[29]->w_OmegaDot = 64808 ;
  pz_RomAlm[29]->q_SqrtA = 10554738ul ;
  pz_RomAlm[29]->q_Omega0 = 10942809ul ;
  pz_RomAlm[29]->q_Omega = 9035059ul ;
  pz_RomAlm[29]->q_M0 = 9560801ul ;
  pz_RomAlm[29]->w_Af0 = 1987 ;
  pz_RomAlm[29]->w_Af1 = 2046 ;
  pz_RomAlm[29]->w_GpsWeek = 2074 ;

  pz_RomAlm[30]->u_Sv = 31 ;
  pz_RomAlm[30]->u_Health = 0 ;
  pz_RomAlm[30]->u_Toa = 78 ;
  pz_RomAlm[30]->w_E = 19755 ;
  pz_RomAlm[30]->w_DeltaI = 2760 ;
  pz_RomAlm[30]->w_OmegaDot = 64827 ;
  pz_RomAlm[30]->q_SqrtA = 10554673ul ;
  pz_RomAlm[30]->q_Omega0 = 10912000ul ;
  pz_RomAlm[30]->q_Omega = 102746ul ;
  pz_RomAlm[30]->q_M0 = 8897112ul ;
  pz_RomAlm[30]->w_Af0 = 5 ;
  pz_RomAlm[30]->w_Af1 = 2047 ;
  pz_RomAlm[30]->w_GpsWeek = 2074 ;

  pz_RomAlm[31]->u_Sv = 32 ;
  pz_RomAlm[31]->u_Health = 0 ;
  pz_RomAlm[31]->u_Toa = 78 ;
  pz_RomAlm[31]->w_E = 6800 ;
  pz_RomAlm[31]->w_DeltaI = 2460 ;
  pz_RomAlm[31]->w_OmegaDot = 64858 ;
  pz_RomAlm[31]->q_SqrtA = 10554552ul ;
  pz_RomAlm[31]->q_Omega0 = 8022011ul ;
  pz_RomAlm[31]->q_Omega = 10052716ul ;
  pz_RomAlm[31]->q_M0 = 2495994ul ;
  pz_RomAlm[31]->w_Af0 = 90 ;
  pz_RomAlm[31]->w_Af1 = 4 ;
  pz_RomAlm[31]->w_GpsWeek = 2074 ;

}