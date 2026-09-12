#ifndef LM_NMEA_H
#define LM_NMEA_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          NMEA Header File

GENERAL DESCRIPTION
  This header file contains all the prototypes and definitions necessary for
  the NMEA data to be properly output.

 
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2005 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc. 
  All Rights Reserved..
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_nmea.h#1 $ 
  $DateTime: 2019/10/24 01:30:36 $ 
  $Author: pwbldsvc $ 
  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/25/18   nath     Added PQDTM change
04/18/17   sum      Added declaration for functions tm_nmea_gngga_put tm_nmea_gnrmc_put and tm_nmea_gnvtg_put
09/14/17   sum      Added support for GPDTM NMEA
09/28/17   py       Functions header updated for $__GSV,$__RMC,$__GNS
09/01/16   mj       Add changes for Leap Second Correction
08/08/14   jv       Galileo support
09/14/07   gk       NMEA CONFIG set parameters support
08/07/06   mrawal   File created
===========================================================================*/

#include "gps_variation.h"
#include "customer.h"  /* Customer configuration file */
#include "sm_api.h"
#include "sio.h"
#ifdef FEATURE_RUNTIME_DEVMAP
#include "rdevmap.h"
#endif /* FEATURE_RUNTIME_DEVMAP */
#include "tm_lm_iface.h"

/*===========================================================================
  EXTERNALS
===========================================================================*/
typedef enum
{
  TM_NMEA_DIAG_OPEN_PORT_UART1,
  TM_NMEA_DIAG_OPEN_PORT_UART2,
  TM_NMEA_DIAG_OPEN_PORT_USB,
  TM_NMEA_DIAG_OPEN_PORT_SMD,
  TM_NMEA_DIAG_OPEN_PORT_NONE, /* Close port */
} tm_nmea_diag_open_port_id_e_type;


typedef enum
{
  TM_NMEA_SYSTEM_ID_DEFAULT = 0x0,
  TM_NMEA_SYSTEM_ID_GP   = 0x01,
  TM_NMEA_SYSTEM_ID_GL   = 0x02,
  TM_NMEA_SYSTEM_ID_GA   = 0x03,
  TM_NMEA_SYSTEM_ID_BDS  = 0x04,
  TM_NMEA_SYSTEM_ID_QZSS = 0x05,
  TM_NMEA_SYSTEM_ID_NAVIC = 0x06,
  TM_NMEA_SYSTEM_ID_LAST = 0x0F
}tm_nmea_system_id_e_type;

typedef enum
{
  TM_NMEA_SIGNAL_ID_ALL = 0x0,

  TM_NMEA_SIGNAL_ID_GPS_L1_CA = 0x1,
  TM_NMEA_SIGNAL_ID_GLONASS_G1_CA = 0x1,
  TM_NMEA_SIGNAL_ID_GALILEO_E5_A = 0x1,
  TM_NMEA_SIGNAL_ID_BDS_B1_I = 0x1,
  TM_NMEA_SIGNAL_ID_QZSS_L1_CA = 0x1,
  TM_NMEA_SIGNAL_ID_NAVIC_L5_SPS = 0x1,

  TM_NMEA_SIGNAL_ID_GPS_L1P_Y = 0x2,
  TM_NMEA_SIGNAL_ID_GLONASS_G1_P = 0x2,
  TM_NMEA_SIGNAL_ID_GALILEO_E5_B = 0x2,
  TM_NMEA_SIGNAL_ID_BDS_B1_Q = 0x2,
  TM_NMEA_SIGNAL_ID_QZSS_L1_C_D = 0x2,
  TM_NMEA_SIGNAL_ID_NAVIC_S_SPS = 0x2,

  TM_NMEA_SIGNAL_ID_GPS_L1_M = 0x3,
  TM_NMEA_SIGNAL_ID_GLONASS_G2_CA = 0x3,
  TM_NMEA_SIGNAL_ID_GALILEO_E5_A_PLUS_B = 0x3,
  TM_NMEA_SIGNAL_ID_BDS_B1_C = 0x3,
  TM_NMEA_SIGNAL_ID_QZSS_L1_C_P = 0x3,
  TM_NMEA_SIGNAL_ID_NAVIC_L5_RS = 0x3,

  TM_NMEA_SIGNAL_ID_GPS_L2P_Y = 0x4,
  TM_NMEA_SIGNAL_ID_GLONASS_M_G2_P = 0x4,
  TM_NMEA_SIGNAL_ID_GALILEO_E6_A = 0x4,
  TM_NMEA_SIGNAL_ID_BDS_B1_A = 0x4,
  TM_NMEA_SIGNAL_ID_QZSS_L1S = 0x4,
  TM_NMEA_SIGNAL_ID_NAVIC_S_RS = 0x4,

  TM_NMEA_SIGNAL_ID_GPS_L2C_M = 0x5,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_5 = 0x5,
  TM_NMEA_SIGNAL_ID_GALILEO_E6_BC = 0x5,
  TM_NMEA_SIGNAL_ID_BDS_B2_A = 0x5,
  TM_NMEA_SIGNAL_ID_QZSS_L2C_M = 0x5,
  TM_NMEA_SIGNAL_ID_NAVIC_L1_SPS = 0x5,

  TM_NMEA_SIGNAL_ID_GPS_L2C_L = 0x6,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_6 = 0x6,
  TM_NMEA_SIGNAL_ID_GALILEO_L1_A = 0x6,
  TM_NMEA_SIGNAL_ID_BDS_B2_B = 0x6,
  TM_NMEA_SIGNAL_ID_QZSS_L2C_L = 0x6,
  TM_NMEA_SIGNAL_ID_NAVIC_RESERVED_6 = 0x6,

  TM_NMEA_SIGNAL_ID_GPS_L5_I = 0x7,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_7 = 0x7,
  TM_NMEA_SIGNAL_ID_GALILEO_L1_BC = 0x7,
  TM_NMEA_SIGNAL_ID_BDS_B2_A_PLUS_B = 0x7,
  TM_NMEA_SIGNAL_ID_QZSS_L5_I = 0x7,
  TM_NMEA_SIGNAL_ID_NAVIC_RESERVED_7 = 0x7,

  TM_NMEA_SIGNAL_ID_GPS_L5_Q = 0x8,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_8 = 0x8,
  TM_NMEA_SIGNAL_ID_GALILEO_RESERVED_8 = 0x8,
  TM_NMEA_SIGNAL_ID_BDS_B3_I = 0x8,
  TM_NMEA_SIGNAL_ID_QZSS_L5_Q = 0x8,
  TM_NMEA_SIGNAL_ID_NAVIC_RESERVED_8 = 0x8,

  TM_NMEA_SIGNAL_ID_GPS_RESERVED_9 = 0x9,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_9 = 0x9,
  TM_NMEA_SIGNAL_ID_GALILEO_RESERVED_9 = 0x9,
  TM_NMEA_SIGNAL_ID_BDS_B3_Q = 0x9,
  TM_NMEA_SIGNAL_ID_QZSS_L6_D = 0x9,
  TM_NMEA_SIGNAL_ID_NAVIC_RESERVED_9 = 0x9,

  TM_NMEA_SIGNAL_ID_GPS_RESERVED_0xA = 0xA,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_0xA = 0xA,
  TM_NMEA_SIGNAL_ID_GALILEO_RESERVED_0xA = 0xA,
  TM_NMEA_SIGNAL_ID_BDS_B3_A = 0xA,
  TM_NMEA_SIGNAL_ID_QZSS_L6_E = 0xA,
  TM_NMEA_SIGNAL_ID_NAVIC_RESERVED_0xA = 0xA,

  TM_NMEA_SIGNAL_ID_GPS_RESERVED_0xB = 0xB,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_0xB = 0xB,
  TM_NMEA_SIGNAL_ID_GALILEO_RESERVED_0xB = 0xB,
  TM_NMEA_SIGNAL_ID_BDS_B2_I = 0xB,
  TM_NMEA_SIGNAL_ID_QZSS_RESERVED_0xB = 0xB,
  TM_NMEA_SIGNAL_ID_NAVIC_RESERVED_0xB = 0xB,

  TM_NMEA_SIGNAL_ID_GPS_RESERVED_0xC = 0xC,
  TM_NMEA_SIGNAL_ID_GLONASS_RESERVED_0xC = 0xC,
  TM_NMEA_SIGNAL_ID_GALILEO_RESERVED_0xC = 0xC,
  TM_NMEA_SIGNAL_ID_BDS_B2_Q = 0xC,
  TM_NMEA_SIGNAL_ID_QZSS_RESERVED_0xC = 0xC,
  TM_NMEA_SIGNAL_ID_NAVIC_RESERVED_0xC = 0xC,
}tm_nmea_signal_id_e_type;

#define TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(LENGTH_WRITTEN, BUF_LENGTH_AVAILABLE, UPDATED_LENGTH)\
{\
  if (LENGTH_WRITTEN < 0 || LENGTH_WRITTEN >= BUF_LENGTH_AVAILABLE)\
  {\
    TM_ERROR_0("NMEA: Error in string formatting");\
    return;\
  }\
  UPDATED_LENGTH += LENGTH_WRITTEN;\
  BUF_LENGTH_AVAILABLE -= LENGTH_WRITTEN;\
}
/*===========================================================================
  FUNCTION PROTOTYPES
===========================================================================*/
/*===========================================================================

FUNCTION tm_nmea_open_cb

DESCRIPTION
  This callback is called when RDM wants NMEA to open a port.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_open_cb(sio_port_id_type port_id);


/*===========================================================================

FUNCTION tm_nmea_close_cb

DESCRIPTION
  This callback is called when RDM wants NMEA to close its port.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_close_cb(void);

/*===========================================================================

FUNCTION tm_nmea_sio_init

DESCRIPTION
  This function contains the initialization for the NMEA sio.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_sio_init(void);

/*===========================================================================

FUNCTION tm_nmea_open_handler

DESCRIPTION
  This function is used to open NMEA message output port.

RETURN VALUE
  None

DEPENDENCIES

===========================================================================*/
extern void tm_nmea_open_handler(void *p_data);


/*===========================================================================

FUNCTION tm_nmea_close_handler

DESCRIPTION
  This function is used to close NMEA message's output port .

RETURN VALUE
  None

DEPENDENCIES

===========================================================================*/
extern void tm_nmea_close_handler(void);

/*===========================================================================
FUNCTION tm_nmea_get_gps_fix_from_refpos

This function takes a reference position of type gps_RefLocStructType and
populates a sm_GpsFixRptStructType with only the fields required to generate 
NMEA strings. This is used in case of MSA where the final fix from the server
doesn't contain all the information required to generate NMEA.
   
DESCRIPTION

DEPENDENCIES

RETURN VALUE sm_GpsFixRptStructType* 

SIDE EFFECTS
 
===========================================================================*/
extern void tm_nmea_get_gps_fix_from_refpos(const gps_RefLocStructType *p_refpos,
                                            sm_FixInfoStructType *p_fix);

/* GGA NMEA Sentences */
void tm_nmea_gpgga_put(const sm_FixInfoStructType *p_Fix,  boolean u_no_fix);
void tm_nmea_gagga_put(const sm_FixInfoStructType *p_fix,  boolean u_no_fix);
void tm_nmea_gbgga_put(const sm_FixInfoStructType *p_Fix,  boolean u_no_fix);
void tm_nmea_gngga_put(const sm_FixInfoStructType *p_fix,  boolean u_no_fix);

/* DTM NMEA Sentences */
void tm_nmea_gpdtm_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix);
void tm_nmea_gndtm_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix);

/* GNS NMEA Sentences */
void tm_nmea_gngns_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix);

#ifdef FEATURE_CGPS_LBS_TASK
/*===========================================================================
FUNCTION tm_nmea_pqxfi_put

DESCRIPTION

tm_nmea_pqxfi_put() is used to build and transmit the NMEA 
Proprietary Qualcomm eXtended Fix Information message. 

Global Positioning System Extended Fix Data.

Extended fix related data for a GPS receiver.

 1     2         3       4 5        6        7 8 9  10 11  12 13 14 15  16   17
 |     |         |       | |        |        | | |  |   |   | |   | |   |    |
$PQXFI,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>

Field Number: 
1) Proprietary Qualcommm eXtended Fix Information.
2) Universal Time Coordinated (UTC) of position.
3) Latitude  (DDmm.mm)    DD = two digits of degrees, mm.mm = minutes.
4) N or S (North or South).
5) Longitude (DDDmm.mm).
6) E or W (East or West).
7) Antenna Altitude above/below mean-sea-level (geoid), meters.
8) HEPE (m, ~63% confidence, 2D horizontal error estimate = root-sum-square of 68% confidence east error estimate and north error estimate)
9) Vertical Uncertainty (m, 68% confidence, 1-sided, 1-dimensional)
10) Velocity Uncertainty (m/s, 20% confidence, 3D = root-sum-square of 68% confidence error estimates on east, north & up velocities) 
11) Checksum

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS

===========================================================================*/
void tm_nmea_pqxfi_put(const sm_FixInfoStructType *p_Fix, boolean u_no_fix);
#endif

/* GSV NMEA Sentences */
void tm_nmea_gagsv_put(const sm_GpsMeasRptStructType *p_meas);
void tm_nmea_gpgsv_put(const sm_GpsMeasRptStructType *p_meas);
void tm_nmea_pqgsv_put(const sm_GpsMeasRptStructType *p_meas);
void tm_nmea_glgsv_put(const sm_GpsMeasRptStructType *p_Meas);
void tm_nmea_gbgsv_put(const sm_GpsMeasRptStructType *p_meas);
void tm_nmea_gqgsv_put(const sm_GpsMeasRptStructType *p_meas);
void tm_nmea_gigsv_put(const sm_GpsMeasRptStructType *p_meas);

/* VTG NMEA Sentences */
void tm_nmea_gpvtg_put(const sm_FixInfoStructType *p_fix);
void tm_nmea_gavtg_put(const sm_FixInfoStructType *p_fix);
void tm_nmea_gbvtg_put(const sm_FixInfoStructType *p_fix);
void tm_nmea_gnvtg_put(const sm_FixInfoStructType *p_fix);

/* GSA NMEA Sentences */
void tm_nmea_gpgsa_put(const sm_FixInfoStructType *p_Fix);
void tm_nmea_gagsa_put(const sm_FixInfoStructType *p_fix);
void tm_nmea_gbgsa_put(const sm_FixInfoStructType *p_Fix);
void tm_nmea_pqgsa_put(const sm_FixInfoStructType *p_fix);
void tm_nmea_gngsa_put(const sm_FixInfoStructType *p_Fix);

/* RMC NMEA Sentences */
void tm_nmea_gprmc_put(const sm_FixInfoStructType *p_Fix);
void tm_nmea_garmc_put(const sm_FixInfoStructType *p_fix);
void tm_nmea_gbrmc_put(const sm_FixInfoStructType *p_fix);
void tm_nmea_gnrmc_put(const sm_FixInfoStructType *p_fix);

/*===========================================================================
FUNCTION tm_nmea_pstis_put

DESCRIPTION
  tm_nmea_pstis_put() is used to transmit PSTIS - GPS session start indication
     
  12      3    
  ||      |      
  $PSTIS,*hh<CR><LF>

  Field Description: 
  1) Talker ID.
  2) PSTIS for Proprietary SnapTrack Inc. session control message.
  3) Checksum


DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS

===========================================================================*/
void tm_nmea_pstis_put(void);

/*===========================================================================

FUNCTION tm_nmea_diag_assign_port

DESCRIPTION
  This function opens NMEA port.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_diag_open_port(tm_nmea_diag_open_port_id_e_type port_to_open);

/*===========================================================================

FUNCTION tm_nmea_diag_close_port

DESCRIPTION
  This function closes NMEA port

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean tm_nmea_diag_close_port(void (*close_func_ptr)(void));

/*===========================================================================

FUNCTION tm_nmea_open_handler_part2

DESCRIPTION
  Handles opening the NMEA port after asynchronous close of 
  already opened port.

DEPENDENCIES
  None.

RETURN VALUE
  None.  

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_open_handler_part2(void);

/*===========================================================================

FUNCTION tm_nmea_diag_open_port_part2

DESCRIPTION
  Handles opening the NMEA diag port after asynchronous close of 
  already opened port.

DEPENDENCIES
  None.

RETURN VALUE
  None.  

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_diag_open_port_part2(void);

/*===========================================================================
FUNCTION tm_nmea_debug

DESCRIPTION
  This function sends the debug NMEA sentences to LBS and also
  on the NMEA COM port if it is open.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_debug(tm_nmea_data_s_type *pDebugNMEA);

/*===========================================================================
FUNCTION tm_nmea_set_gnss_meas_update

DESCRIPTION
  Stores the FCount from measurement report

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS

===========================================================================*/
extern void tm_nmea_set_gnss_meas_update(const sm_GpsMeasRptStructType *p_Meas);

/*===========================================================================
FUNCTION tm_nmea_debug

DESCRIPTION
  This function sends the debug NMEA sentences to LocMW directly via TM and also
  on the NMEA COM port if it is open. This function is not to be confused with
  tm_nmea_debug.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_debug_report(sm_DebugNMEA *pz_DebugNMEA);

/*===========================================================================
FUNCTION tm_nmea_fix_debug_report

DESCRIPTION
  This function generates debug NMEA sentence PQWS1 ans send it to LocMW
  directly via TM and also on the NMEA COM port if it is open.
  This function is not to be confused with tm_nmea_debug.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_fix_debug_report(tm_fix_status_debug_info_s_type *pz_FixDebugInfo);

/*===========================================================================
FUNCTION tm_nmea_var_len_debug_report

DESCRIPTION
  This function sends the debug NMEA sentences to LocMW directly via TM and also
  on the NMEA COM port if it is open. This function is not to be confused with
  tm_nmea_debug.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_var_len_debug_report(sm_DebugNMEAVarLen *pz_DebugNMEA);

#endif /* LM_NMEA_H */
