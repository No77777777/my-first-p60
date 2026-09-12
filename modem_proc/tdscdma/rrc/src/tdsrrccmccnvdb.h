#ifndef TDSRRCCMCCNVDB_H
#define TDSRRCCMCCNVDB_H
/*===========================================================================
                  R R C   N V   M O D U L E

DESCRIPTION
  This header file contains the predefined TDS RRC NV values that differ between CMCC conformance test and CMCC
  field test/mass product configuration. The predefined value is stored in static array tds_nv_cmcc_test_setting.

  The CMCC predefined TDS RRC NV configuration is controlled by NV 69731, which can take value 0, 1, or 2, with default
  being 0. The rule is:

-	NV "69731" shall be the ONLY NV controlling CMCC specific functionality
-	TDS software will be modified to align as below for the different values of NV 69731 -
	Value "0" - Test Configuration (Engineering Mode)
	Software will by default configure all required NV settings for CMCC Field
	Any TDS specific NV configured by the user (OEM/QMC) will take effect
	OEMs can use this setting for - testing on specific boxes, change capabilities/configurations etc.
	Value "1" - CMCC Lab Configuration
	Software will by default configure all required NV settings for CMCC Lab
	Any TDS specific NV configuration by the user (OEM/QMC) will NOT take effect - ignored
	OEMs should use this setting for "CMCC Lab Testing"
	Value "2" - CMCC Field/Deployment Configuration
	Software will by default configure all required NV settings for CMCC Field
	Any TDS specific NV configuration by the user (OEM/QMC) will NOT take effect - ignored
	OEMs should use this setting for "CMCC Field Testing/Mass Production"
-	Moving forward all TDS Software teams should use the above guideline for introducing any CMCC specific functionality
-	Based on CMCC network evolution/maturity, TDS Software team shall be responsible for updating the above configurations, as needed

Copyright (c) 2010-2013 QUALCOMM Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccmccnvdb.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/09/13  baix    Initial version.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* predefined user pref bits in TDS_OPTIONAL_FEATURE_LIST that are under OEM control for CMCC FT/mass product cfg, i.e., when NV69731 is set to "2" */
#define TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST_USER_PREF_BITS \
	    (TDS_NV_OPTIONAL_FEATURE_HSUPA | TDSCDMA_NV_OPTIONAL_FEATURE_ENABLE_PFR)

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* TDS RRC NVs that differ between CMCC conformance test and CMCC field test/mass product setting
    When a new NV is added to this list, make sure (1) add the new NV before TDS_RRC_NV_CMCC_TEST_SETTING_MAX
    in this list; (2) add predefined value of the new NV in the two sections of static array tds_nv_cmcc_test_setting below;
    (3) change the corresponding entry in function tdsrrcnv_get_default_value in tdsrrcnv.c, to read value from static array 
    tds_nv_cmcc_test_setting, the CMCC field test/mass product cfg section.
*/
typedef enum {
  TDS_RRC_NV_CMCC_TEST_SETTING_RRC_INTEGRITY_ENABLED,
  TDS_RRC_NV_CMCC_TEST_SETTING_RRC_CIPHERING_ENABLED,
  TDS_RRC_NV_CMCC_TEST_SETTING_RRC_FAKE_SECURITY_ENABLED,
  TDS_RRC_NV_CMCC_TEST_SETTING_SPECIAL_FREQ_ENABLED,
  TDS_RRC_NV_CMCC_TEST_SETTING_SPECIAL_FREQ,
  TDS_RRC_NV_CMCC_TEST_SETTING_RRC_PDCP_DISABLED,
  //TDS_RRC_NV_CMCC_TEST_SETTING_RRC_VERSION,
  TDS_RRC_NV_CMCC_TEST_SETTING_HSDPA_CAT,
  TDS_RRC_NV_CMCC_TEST_SETTING_HSUPA_CAT,
  TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST,
  TDS_RRC_NV_CMCC_TEST_SETTING_MAX
} tds_rrc_nv_cmcc_special_test_nv_idx_type;


static uint32 const tds_nv_cmcc_test_setting[][TDS_RRC_NV_CMCC_TEST_SETTING_MAX] = {
  /* predefined CMCC lab test cfg, corresponding to NV69731 set to "1" */
  { 	
	1,         /* CMCC test setting for Conformance    TDS_RRC_NV_RRC_INTEGRITY_ENABLED			  boolean	*/
	0,         /* CMCC test setting for Conformance    TDS_RRC_NV_RRC_CIPHERING_ENABLED			  boolean	*/
	0,         /* CMCC test setting for Conformance    TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED	  boolean	*/
	1,         /* CMCC test setting for Conformance    TDS_RRC_NV_SPECIAL_FREQ_ENABLED	       	  boolean */
	10062,     /* CMCC test setting for Conformance    TDS_RRC_NV_SPECIAL_FREQ       			         uint32 */
	1,         /* CMCC test setting for Conformance    TDS_RRC_NV_RRC_PDCP_DISABLED			  boolean */
	//2,         /* CMCC test setting for Conformance    TDS_RRC_NV_RRC_VERSION 					  uint32    value 2 indicates Rel4/Rel5/REL7 */
	15,        /* CMCC test setting for Conformance    TDS_RRC_NV_HSDPA_CAT						  uint8 */
	6,         /* CMCC test setting for Conformance    TDS_RRC_NV_HSUPA_CAT						  uint8 */
	TDS_NV_OPTIONAL_FEATURE_DEFAULT_VAL_FOR_SPECIAL_TEST
	           /* CMCC test setting for Conformance    TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST		  uint32 */
  },
/* predefined CMCC field test/mass product cfg, corresponding to (a) NV69731 set to "2", or (b) NV69731 set to 0/not set and user does not specify value for below NV */
  {	  
	1,		 /* CMCC test setting for field/mass product       TDS_RRC_NV_RRC_INTEGRITY_ENABLED			         boolean   */
	0,		 /* CMCC test setting for field/mass product	  TDS_RRC_NV_RRC_CIPHERING_ENABLED			  boolean   */
	0,		 /* CMCC test setting for field/mass product	  TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED		  boolean	  */
	0,		 /* CMCC test setting for field/mass product	  TDS_RRC_NV_SPECIAL_FREQ_ENABLED			         boolean */
	10055,	 /* CMCC test setting for field/mass product	  TDS_RRC_NV_SPECIAL_FREQ						  uint32 */
	0,		 /* CMCC test setting for field/mass product	  TDS_RRC_NV_RRC_PDCP_DISABLED				  boolean */

#if 0
#if defined(FEATURE_TDSCDMA_REL8) || defined(FEATURE_TDSCDMA_REL9)
	3, /* value 3 indicates Rel4/Rel5/REL7/Rel9 */
#else
	2, /* value 2 indicates Rel4/Rel5/REL7 */
#endif
	     /*!< For FT, RRC_VERSION is not controled by NV69731 and is read from it's NV66017 directly */
             /* CMCC test setting for field/mass product	  TDS_RRC_NV_RRC_VERSION					         uint32 */
#endif

	TDSRRC_HSDPA_DEFAULT_CATEGORY,
	         /* CMCC test setting for field/mass product	  TDS_RRC_NV_HSDPA_CAT							  uint8 */

#ifdef FEATURE_TDSCDMA_HSUPA
    TDSRRC_TDS_HSUPA_DEFAULT_CATEGORY,
#else
    0, /* not used */
#endif
               /* CMCC test setting for field/mass product	  TDS_RRC_NV_HSUPA_CAT							  uint8 */

	TDS_NV_OPTIONAL_FEATURE_DEFAULT_VAL 
	           /* CMCC test setting for field/mass product	  TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST		         uint32 */
  }
};

#endif /* TDSRRCCMCCNVDB_H */
