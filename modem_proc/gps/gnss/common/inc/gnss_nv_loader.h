#ifndef __GNSS_NV_LOADER_H__
#define __GNSS_NV_LOADER_H__
/*============================================================================
  @file gnss_nv_loader.h

                  GNSS NV Loader Definitions 

GENERAL DESCRIPTION

  This file containts various NV definitions for GNSS NV Loader module

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

            Copyright (c) 2013-2014 QUALCOMM ATHEROS, INCORPORATED.
            All Rights Reserved
            Qualcomm Confidential and Proprietary
			
			Copyright (c) 2021-2022 Qualcomm Technologies, Inc.
            All Rights Reserved.
            Confidential and Proprietary - Qualcomm Technologies, Inc.


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

=============================================================================*/

/*============================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/common/inc/gnss_nv_loader.h#8 $
  $DateTime: 2022/02/24 00:26:22 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/12/21  anksha   Add Url's for Qtang2 XTRA downloads
01/28/20  anksha   Add the Url's to be used for Navic Assist download
06/30/14   tf      Change default URLS for XTRA2 to cloud-hosted servers
11/06/13   rh      Changed default URLs to XTRA3 if FEATURE_GNSS_XTRA3 defined 
10/07/13   rh      Initial version

=============================================================================*/

/* ---------------------------------------------------------------------------
 * Include files
 * ---------------------------------------------------------------------------*/
#include "customer.h"
#include "comdef.h"

/* Default XTRA file URL */
#ifdef FEATURE_GNSS_XTRA3
#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
#ifndef FEATURE_QTANG2
#define NV_AAGPS_DEFAULT_XTRA_PRIMARY_SERVER_URL        "https://path1.xtracloud.net/xtra3Mgrbeji.bin"
#define NV_AAGPS_DEFAULT_XTRA_SECONDARY_SERVER_URL      "https://path2.xtracloud.net/xtra3Mgrbeji.bin"
#define NV_AAGPS_DEFAULT_XTRA_TERTIARY_SERVER_URL       "https://path3.xtracloud.net/xtra3Mgrbeji.bin"
#else
#define NV_AAGPS_DEFAULT_XTRA_PRIMARY_SERVER_URL        "https://path4.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define NV_AAGPS_DEFAULT_XTRA_SECONDARY_SERVER_URL      "https://path5.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define NV_AAGPS_DEFAULT_XTRA_TERTIARY_SERVER_URL       "https://path6.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#endif
#else /*FEATURE_XTRA3_DYN_URL_SELECTION*/
#ifndef FEATURE_QTANG2
#define NV_AAGPS_DEFAULT_XTRA_PRIMARY_SERVER_URL        "https://path1.xtracloud.net/xtra3Mgrcej.bin"
#define NV_AAGPS_DEFAULT_XTRA_SECONDARY_SERVER_URL      "https://path2.xtracloud.net/xtra3Mgrcej.bin"
#define NV_AAGPS_DEFAULT_XTRA_TERTIARY_SERVER_URL       "https://path3.xtracloud.net/xtra3Mgrcej.bin"
#else
#define NV_AAGPS_DEFAULT_XTRA_PRIMARY_SERVER_URL        "https://path4.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define NV_AAGPS_DEFAULT_XTRA_SECONDARY_SERVER_URL      "https://path5.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define NV_AAGPS_DEFAULT_XTRA_TERTIARY_SERVER_URL       "https://path6.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#endif
#endif /*FEATURE_XTRA3_DYN_URL_SELECTION*/
#else /* FEATURE_GNSS_XTRA3 */
#define NV_AAGPS_DEFAULT_XTRA_PRIMARY_SERVER_URL        "http://xtrapath1.izatcloud.net/xtra2.bin"
#define NV_AAGPS_DEFAULT_XTRA_SECONDARY_SERVER_URL      "http://xtrapath2.izatcloud.net/xtra2.bin"
#define NV_AAGPS_DEFAULT_XTRA_TERTIARY_SERVER_URL       "http://xtrapath3.izatcloud.net/xtra2.bin"
#endif /* FEATURE_GNSS_XTRA3 */

/* Default XTRA SNTP time server URL */
#ifdef FEATURE_GNSS_XTRA3
#define  NV_AAGPS_DEFAULT_XTRA_PRIMARY_SNTP_SERVER_URL          "time.xtracloud.net"
#define  NV_AAGPS_DEFAULT_XTRA_SECONDARY_SNTP_SERVER_URL        "time.xtracloud.net"
#define  NV_AAGPS_DEFAULT_XTRA_TERTIARY_SNTP_SERVER_URL         "time.xtracloud.net"
#else /* FEATURE_GNSS_XTRA3 */
#define  NV_AAGPS_DEFAULT_XTRA_PRIMARY_SNTP_SERVER_URL          "time.xtracloud.net"
#define  NV_AAGPS_DEFAULT_XTRA_SECONDARY_SNTP_SERVER_URL        "time.xtracloud.net"
#define  NV_AAGPS_DEFAULT_XTRA_TERTIARY_SNTP_SERVER_URL         "time.xtracloud.net"
#endif /* FEATURE_GNSS_XTRA3 */


#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
/*The following Url's to be used for NON-IOT targets as default fallback Url's*/
#ifndef FEATURE_QTANG2
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_PRIMARY_SERVER_URL         "https://path1.xtracloud.net/xtra3Mgrbeji.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_SECONDARY_SERVER_URL       "https://path2.xtracloud.net/xtra3Mgrbeji.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_TERTIARY_SERVER_URL        "https://path3.xtracloud.net/xtra3Mgrbeji.bin"

#define FALLBACK_DEFAULT_XTRA_MULTIBAND_B2a_PRIMARY_SERVER_URL     "https://path1.xtracloud.net/xtra3Mgrbeji.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_B2a_SECONDARY_SERVER_URL   "https://path2.xtracloud.net/xtra3Mgrbeji.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_B2a_TERTIARY_SERVER_URL    "https://path3.xtracloud.net/xtra3Mgrbeji.bin"

#define FALLBACK_DEFAULT_XTRA_PRIMARY_SERVER_URL                   "https://path1.xtracloud.net/xtra3grcej.bin"
#define FALLBACK_DEFAULT_XTRA_SECONDARY_SERVER_URL                 "https://path2.xtracloud.net/xtra3grcej.bin"
#define FALLBACK_DEFAULT_XTRA_TERTIARY_SERVER_URL                  "https://path3.xtracloud.net/xtra3grcej.bin"
#else
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_NAVIC_ENABLED_PRIMARY_SERVER_URL         "https://path4.xtracloud.net/qtang2/xtra3Mgrbeji.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_NAVIC_ENABLED_SECONDARY_SERVER_URL       "https://path5.xtracloud.net/qtang2/xtra3Mgrbeji.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_NAVIC_ENABLED_TERTIARY_SERVER_URL        "https://path6.xtracloud.net/qtang2/xtra3Mgrbeji.bin"

#define FALLBACK_DEFAULT_XTRA_MULTIBAND_PRIMARY_SERVER_URL         "https://path4.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_SECONDARY_SERVER_URL       "https://path5.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_TERTIARY_SERVER_URL        "https://path6.xtracloud.net/qtang2/xtra3Mgrbej.bin"

#define FALLBACK_DEFAULT_XTRA_MULTIBAND_B2a_PRIMARY_SERVER_URL     "https://path4.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_B2a_SECONDARY_SERVER_URL   "https://path5.xtracloud.net/qtang2/xtra3Mgrbej.bin"
#define FALLBACK_DEFAULT_XTRA_MULTIBAND_B2a_TERTIARY_SERVER_URL    "https://path6.xtracloud.net/qtang2/xtra3Mgrbej.bin"

#define FALLBACK_DEFAULT_XTRA_PRIMARY_SERVER_URL                   "https://path4.xtracloud.net/qtang2/xtra3grbej.bin"
#define FALLBACK_DEFAULT_XTRA_SECONDARY_SERVER_URL                 "https://path5.xtracloud.net/qtang2/xtra3grbej.bin"
#define FALLBACK_DEFAULT_XTRA_TERTIARY_SERVER_URL                  "https://path6.xtracloud.net/qtang2/xtra3grbej.bin"
#endif
#endif

#ifndef FEATURE_QTANG2
#define NAVIC_ASSIST_DEFAULT_XTRA_PRIMARY_SERVER_URL         "http://path1.xtracloud.net/xtra3Mi_eph.bin"
#define NAVIC_ASSIST_DEFAULT_XTRA_SECONDARY_SERVER_URL       "http://path2.xtracloud.net/xtra3Mi_eph.bin"
#define NAVIC_ASSIST_DEFAULT_XTRA_TERTIARY_SERVER_URL        "http://path3.xtracloud.net/xtra3Mi_eph.bin"
#else
#define NAVIC_ASSIST_DEFAULT_XTRA_PRIMARY_SERVER_URL         "http://path4.xtracloud.net/qtang2/xtra3Mi_eph_qt.bin"
#define NAVIC_ASSIST_DEFAULT_XTRA_SECONDARY_SERVER_URL       "http://path5.xtracloud.net/qtang2/xtra3Mi_eph_qt.bin"
#define NAVIC_ASSIST_DEFAULT_XTRA_TERTIARY_SERVER_URL        "http://path6.xtracloud.net/qtang2/xtra3Mi_eph_qt.bin"
#endif
#endif  /* __GNSS_NV_LOADER_H__ */
