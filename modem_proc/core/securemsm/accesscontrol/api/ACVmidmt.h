#ifndef TZBSP_VMIDMT_H
#define TZBSP_VMIDMT_H

/*===========================================================================
Copyright (c) 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/securemsm/accesscontrol/api/ACVmidmt.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/18/12   sg      First version.
============================================================================*/
#include <stdbool.h>


#define TZBSP_VMID_NOACCESS     0
#define TZBSP_VMID_VMID_0       0
#define TZBSP_VMID_RPM          1
#define TZBSP_VMID_TZ           2
#define TZBSP_VMID_AP           3
#define TZBSP_VMID_MSS          4
#define TZBSP_VMID_LPASS        5
#define TZBSP_VMID_CP           6
#define TZBSP_VMID_VIDEO        7
#define TZBSP_VMID_ZAP_SHADER   8
#define TZBSP_VMID_MDSS         9
/* Space for VMID 10 */
#define TZBSP_VMID_SSC          11

/* These values were obtained by the IPA team, and are documented in the
** 'IPAv2 Pipes - Istari SMMU VMID configuration' spreadsheet
** these shouldn't be changed as these are used by IPA to generate SID */
#define TZBSP_VMID_IPA_AP       (0b1100) /* 12 */
#define TZBSP_VMID_IPA_UC_PIPE  (0b1101) /* 13 */
#define TZBSP_VMID_IPA_UC       (0b1110) /* 14*/

#define TZBSP_VMID_WLAN         15     
#define TZBSP_VMID_IPA_WIFI     16

#define TZBSP_VMID_NOACCESS_BIT     (1<<TZBSP_VMID_NOACCESS)
#define TZBSP_VMID_VMID_0_BIT       (1<<TZBSP_VMID_VMID_0)
#define TZBSP_VMID_TZ_BIT           (1<<TZBSP_VMID_TZ)
#define TZBSP_VMID_RPM_BIT          (1<<TZBSP_VMID_RPM)
#define TZBSP_VMID_LPASS_BIT        (1<<TZBSP_VMID_LPASS)
#define TZBSP_VMID_MSS_BIT          (1<<TZBSP_VMID_MSS)
#define TZBSP_VMID_AP_BIT           (1<<TZBSP_VMID_AP)
#define TZBSP_VMID_CP_BIT           (1<<TZBSP_VMID_CP)
#define TZBSP_VMID_VIDEO_BIT        (1<<TZBSP_VMID_VIDEO)
#define TZBSP_VMID_MDSS_BIT         (1<<TZBSP_VMID_MDSS)
#define TZBSP_VMID_SSC_BIT          (1<<TZBSP_VMID_SSC)

#define TZBSP_VMID_IPA_AP_BIT       (1 << TZBSP_VMID_IPA_AP)
#define TZBSP_VMID_IPA_UC_PIPE_BIT  (1 << TZBSP_VMID_IPA_UC_PIPE)
#define TZBSP_VMID_IPA_UC_BIT       (1 << TZBSP_VMID_IPA_UC)

#define TZBSP_VMID_WLAN_BIT         (1<<TZBSP_VMID_WLAN)
#define TZBSP_VMID_ZAP_SHADER_BIT   (1<<TZBSP_VMID_ZAP_SHADER)
#define TZBSP_VMID_IPA_WIFI_BIT     (1<<TZBSP_VMID_IPA_WIFI)

#define TZBSP_ALL_VMID              ((~TZBSP_VMID_NOACCESS) & (~TZBSP_DOMAIN_MASK))


/* Domain definitions. Start downwards from 31 so it doesn't conflict with VMIDs */
#define TZBSP_DOMAIN_MASK 0xFF000000 /* Reserve 8 bits for domains */
#define TZBSP_DOMAIN_MSA   31
#define TZBSP_DOMAIN_HYP   30
#define TZBSP_DOMAIN_SP    29
#define TZBSP_DOMAIN_TZ    28


/* Domain definitions */
#define TZBSP_DOMAIN_MSA_BIT   1 << TZBSP_DOMAIN_MSA
#define TZBSP_DOMAIN_HYP_BIT   1 << TZBSP_DOMAIN_HYP
#define TZBSP_DOMAIN_SP_BIT    1 << TZBSP_DOMAIN_SP
#define TZBSP_DOMAIN_TZ_BIT    1 << TZBSP_DOMAIN_TZ


#endif /* TZBSP_VMIDMT_H */
