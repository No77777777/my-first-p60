/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


        M M G S D I   C A C H E - R E L A T E D   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains all the utility functions for accessing MMGSDI
  cache.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2011 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdicache.c#6 $ $DateTime: 2020/09/10 02:38:24 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/10/20   vgd     Don't leave file_cache_crit_sec to read ARR in main task
02/28/20   tq      Add main task check before going to card
02/26/20   cj      caching couple of 1X files before subready
02/26/20   cj      Adding 1X files in post pin caching
09/16/19   sg      Fallback to file system/cache for missing files
04/15/19   vm      Update item cache with NOT_FOUND when select failed
02/25/19   vm      Clear common telecom cache during FCN Refresh 
02/22/19   vm      Clear common telecom cache during FULL FCN Refresh
11/28/18   tq      Optimize F3 by removing record cache len print
10/30/18   bcho    Cache EF-TMSI and EF-SSCI during initialization
05/07/18   rps     Retrieving attribute cache from caller context
02/16/18   dd      Optimization of F3 prints
10/31/17   rps     Add NULL check while reading data from cache 
09/28/17   me      Ignore cache attr read failure if persistance cache is active 
06/20/17   me      Added cache UIM enum support for usim msisdn
06/08/17   nr      Do not apply FPLMN optimization when onchip is enabled
04/24/17   nr      Support to store more FPLMNs in cache for MMGSDI_USIM_FPLMN
04/17/17   nc      Heap overflow correction  
03/14/17   nc      Better checks for return values 
03/20/17   bcho    Allow first write on EF-LOCI with same data
02/10/17   nr      Registering all files for FCN refresh
01/10/17   dt      Removing F3s from cache APIs in DIAG context 
12/29/16   ar      Send write request to card for EF-NAMLOCK
08/30/16   me      Added cache support for MMSS files   
08/30/16   bcho    Optimization of MMGSDI attribute cache access functions
08/08/16   me      Added cache UIM enum support for msisdn, call term efs  
06/29/16   sp      Added stub functions for EFs under PKCS directory
06/20/16   ar      Featurizing GSM related tables
06/19/16   bcho    F3 frequency reduction by conditional logic
05/20/16   tkl     Review of macros used by MMGSDI
05/20/16   vdc     Remove F3 messages for memory allocation failure
05/03/16   tkl     F3 log prints cleanup
04/15/16   bcho    Make cache synch when record is updated in client context
03/08/16   sp      Add EF SMSCAP to RUIM and CSIM cache list
02/29/16   bcho    Delete attr cache while deleting cache of EPSLOCI and EPSNSC
02/12/16   tkl     Skipped write operation if card data remains unchanged
02/10/16   bcho    Cache EF-ARR records
02/10/16   bcho    Set length as 0x3C if feature EXTENDED_FPLMN_ICC is enabled
01/22/16   kv      Send Card and session events based on SFS data in PSM mode
01/20/16   ar      Don't RESET card if EF-DIR req is not processed by UIMDRV
12/28/15   ks      Retry read request for non spec compliant cards
10/05/15   sp      Reduce signed/unsigned conversions in UIM
10/01/15   tkl     Fix update of persistent cache
09/10/15   kv      Features_status_list is split into device and slot items
09/09/15   hh      Add support for potential silent file change notification
09/02/15   lxu     Fake the fourth byte of EF AD for buggy card
08/31/15   yt      Remove length information from requests and confirmations
08/19/15   gm      Remove duplicate file in cache list
08/13/15   sp      merging mmgsdiutil_mem_free macros into one
06/30/15   vdc     Cache complete data for req's with partial read/write data
06/12/15   bcho    Allow to update MMGSDI cache in client context also
05/20/15   bcho    Access file and attr cache from app info ptr on deactivation
04/29/15   bcho    Support for App specific cache
04/10/15   vv      Added support for the ProSe EFs
03/25/15   kk      Subs prov EFS error handling
03/25/15   ar      Support for cross-mapping in multisim_auto_provisioning NV
12/04/14   stv     Fix for subscription switch from SIM to USIM
12/02/14   tkl     Added check to persistent cache write
09/29/14   tkl     Added support for persistent cache
09/09/14   kk      Enhancements in NV handling
09/30/14   ar      Support to store more FPLMNs in cache for MMGSDI_GSM_FPLMN
09/11/14   bcho    Dont cache attributes for files under DF 7F50
09/04/14   ar      Remove NVRUIM cache
08/06/14   ar      Replace MMGSDIUTIL_TMC_MEM_MALLOC_AND_VALIDATE with new macro
07/30/14   ar      Revert FR 20469 changes to align with NAS check-in dates
07/25/14   ar      Support to store more FPLMNs in cache for MMGSDI_GSM_FPLMN
07/14/14   tl      Remove always ON feature FEATURE_MODEM_RCINIT
06/19/14   av      Fix compiler warning
06/16/14   av      Delete common attributes cache at card error
06/12/14   av      Cache mandatory EFs used by NVRUIM for critical 1x NV reads
06/03/14   tl      Remove additional parameter variables
05/11/14   ar      Added NULL check before dereferencing the pointer
04/17/14   av      Enhance MMGSDI attributes cache
04/06/14   am      Added mmgsdi heap alloc clean up logic
04/03/14   am      Fix compile error on SBM due to assert_on_crit_sect_owner
03/04/14   df      Add NULL check to mmgsdi_cache_alloc_and_populate_file_attr_cache
02/24/14   ar      Removed wrapper function mmgsdi_util_alloc()
02/20/14   ak      Fix compile errors due to strict compiler on 9x35
12/16/13   tkl     decouple caching with uim item type
01/15/14   ar      Cached CDMA_RUIM_ID in mmgsdi cache
01/10/14   df      Use v2.0 diag macros
01/09/14   ar      Replace old Macros with MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE
12/18/13   tl      Prevent initalizing the cache with partial files
12/16/13   vdc     Cache the ECC file contents while initialization
12/11/13   df      Remove unused memory paging feature
12/09/13   vdc     Initialize the total length of ECC entry in the cache table
10/28/13   kb      Add offtarget support to check mutex is owned or not
10/16/13   df      Removing unused functions
10/04/13   vdc     F3 message reduction
10/01/13   vv      Do not perform READ, if EF size or rec length is zero
09/24/13   yt      Remove function mmgsdi_util_is_gsdi_task_context()
09/17/13   vdc     Convert GW / 1X prov cache pointers into array of pointers
08/06/13   vv      Fixed the check that determines if a record exists in cache
06/25/13   av      Add SF EUIMID to 1x cache list
06/24/13   vdc     Added support for triple SIM
06/20/13   at      Update ECC cache init status to MMGSDI_CACHE_INIT only
                   after succesfully reading at least 1 record.
05/28/13   spo     Replaced instances of memcpy with memscpy
05/21/13   av      Assume security attributes per spec if ARR read is skipped
05/20/13   vv      Fixed assignment of incorrect cache table pointer
05/18/13   tl      Fixed dest size param of memscpy in mmgsdi_cache_write_item()
05/17/13   bcho    Don't free ECC cache pointer when any read record fails
05/16/13   vv      Add EF-NASCONFIG to cache
05/16/13   vdc     Replace memcpy with safer version memscpy
05/06/13   spo     Allocate memory for CPHS variables during runtime
04/24/13   vv      Reset CSG eons info in the app. info
04/17/13   tl      Replace Featurization with NV
04/16/13   tl      Clean up of radio access features from MMGSDI code
04/04/13   spo     Porting back TMSI_PLMN check in mmgsdi_cache_write()
02/19/13   av      Caching support for GID1,GID2 and few 1x files
02/04/13   vv      Added mmgsdi_session_read_prl support for RUIM card
10/21/12   av      Added support for new ECC event
10/19/12   spo     Do not update ECC cache when data buffer is not allocated
10/15/12   bcho    CPHS files cache cleared on session activation/deactivation
10/13/12   abg     Add utility to delete the contents of the common cache
10/01/12   vs      Added utility for context check including logic for SGLTE
09/28/12   abg     Fixed for reading EF-ECC record after geting read error status
09/28/12   abg     Updated ERR to MSG_ERROR
09/25/12   av      Skip reading EF-ARR for internal selects
09/05/12   av      Fixed null pointer reference
09/04/12   av      Don't send READ req if get_file_attr returns file_not_found
09/03/12   bcho    OPL-PNN file cache cleared on session activation
03/16/12   yt      Modify references to gsdi_task_tcb
03/05/12   bcho    File cache critical section protection added in
                   mmgsdi_cache_init_usim_ecc()
02/23/12   av      Clean up remaining always ON features
02/23/12   bcho    Critical Section Enter/Leave function calls replaced by
                   respective macro to add logging
01/17/12   kk      Do not allow external cache updates for specific EFs when
                   cache is empty
01/05/12   shr     Removed F3 logging from within critical sections
12/21/11   kk      Legacy GSDI removal updates
12/21/11   kk      Added USIM ECC caching support
10/28/11   yt      Initial revision

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "comdef.h"
#include "intconv.h"
#include "mmgsdicache.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "mmgsdi.h"
#include "uim.h"
#include "mmgsdi_uim_uicc.h"
#include "mmgsdi_uim_icc.h"
#include "mmgsdi_eons.h"
#include "mmgsdi_evt.h"
#include "mmgsdi_csg.h"
#include "mmgsdi_file.h"
#include "mmgsdi_nv.h"
#include "mmgsdi_eons_opl_pnn.h"
#include "mmgsdi_card_init.h"
#include "mmgsdi_uicc_rsp.h"
#include "mmgsdi_icc_rsp.h"

#ifdef FEATURE_UIM_PERSISTENT_CACHE
#include "mmgsdi_persistent_cache.h"
#endif /* FEATURE_UIM_PERSISTENT_CACHE */

#include "bit.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Critical section to protect accesses to MMGSDI file/attributes caches */
static rex_crit_sect_type  mmgsdi_cache_crit_sect;

/* ===========================================================================
 MACRO:       MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT
 DESCRIPTION: To enter rex critical section for file cache
 ===========================================================================*/
#define MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT                                 \
  MMGSDIUTIL_ENTER_CRIT_SECT(&mmgsdi_cache_crit_sect,                          \
                             MMGSDIUTIL_CRIT_SECT_FILE_CACHE);                 \

/* ===========================================================================
 MACRO:       MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT
 DESCRIPTION: To leave rex critical section for file cache
 ===========================================================================*/
#define MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT                                 \
  MMGSDIUTIL_LEAVE_CRIT_SECT(&mmgsdi_cache_crit_sect,                          \
                             MMGSDIUTIL_CRIT_SECT_FILE_CACHE);                 \

/* ===========================================================================
   MACRO:       MMGSDIUTIL_CHECK_FILE_CACHE_CRIT_SECT_ACCESS
   DESCRIPTION: To call ERR_FATAL if file cache critical section is accessed
                without acquiring it.
   ===========================================================================*/
#define MMGSDIUTIL_CHECK_FILE_CACHE_CRIT_SECT_ACCESS                      \
   MMGSDIUTIL_CHECK_CRIT_SECT_ACCESS(&mmgsdi_cache_crit_sect,             \
     MMGSDIUTIL_CRIT_SECT_FILE_CACHE);

/* slot1 Common cache */
static mmgsdi_cache_element_type mmgsdi_common_cache_slot1[] =
{
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* ICCID */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* ELP */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* DIR */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* MLPL */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* MSPL */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}  /* MMSSMODE */
};

/* Slot2 Common cache */
static mmgsdi_cache_element_type mmgsdi_common_cache_slot2[] =
{
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* ICCID */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* ELP */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* DIR */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* MLPL */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* MSPL */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}  /* MMSSMODE */
};

/* Slot3 Common cache */
static mmgsdi_cache_element_type mmgsdi_common_cache_slot3[] =
{
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* ICCID */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* ELP */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* DIR */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* MLPL */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}, /* MSPL */
  {MMGSDI_CACHE_NOT_INIT, FALSE, MMGSDI_MAX_FILE_STRUCTURE_ENUM}  /* MMSSMODE */
};

static mmgsdi_attr_cache_type mmgsdi_slot_common_cache_tbl[MMGSDI_MAX_NUM_SLOTS] = {{NULL}};

/*===========================================================================
                          IMPORTANT NOTE
=============================================================================
When adding a new file to the MMGSDI cache lists, the following
needs to be done:
a. Add the file to the MMGSDI REFRESH list.
b. Verify if the file needs to be cached during post-pin init.
c. Add an entry into mmgsdi_util_default_file_sec_attr_table.
d. If the file is record based, the file needs to be re-cached
   during REFRESH FCN handling using mmgsdi_init_cache_record()
   (in mmgsdi_refresh_process_evt()).
e. If the file is record based, in mmgsdi_session_write_cache(),
   the WRITE request should be issued using mmgsdi_session_write_record().
===========================================================================*/
#ifdef FEATURE_MMGSDI_GSM_CARD
/* SIM-CACHE INITIAL CONTENTS */
static mmgsdi_file_enum_type mmgsdi_sim_cache_uim_enums[] =
{
  MMGSDI_GSM_IMSI,     MMGSDI_GSM_KC,       MMGSDI_GSM_HPLMN, MMGSDI_GSM_ACM_MAX, MMGSDI_GSM_SST,
  MMGSDI_GSM_ACM,      MMGSDI_GSM_SPN,      MMGSDI_GSM_PUCT,  MMGSDI_GSM_BCCH,    MMGSDI_GSM_ACC,
  MMGSDI_GSM_FPLMN,    MMGSDI_GSM_LOCI,     MMGSDI_GSM_EMLPP, MMGSDI_GSM_AAEM,    MMGSDI_GSM_KCGPRS,
  MMGSDI_GSM_LOCIGPRS, MMGSDI_GSM_PHASE,    MMGSDI_GSM_AD,    MMGSDI_GSM_PLMN,    MMGSDI_GSM_PLMNWACT,
  MMGSDI_GSM_OPLMNWACT,MMGSDI_GSM_HPLMNACT, MMGSDI_GSM_CBMID, MMGSDI_GSM_LP,      MMGSDI_GSM_ECC,
  MMGSDI_GSM_SPDI,     MMGSDI_GSM_GID1,     MMGSDI_GSM_GID2,  MMGSDI_SIM_7F66_PROP1_SPT_TABLE,
  MMGSDI_SIM_7F66_PROP1_ACT_HPLMN,          MMGSDI_GSM_OPL,   MMGSDI_GSM_PNN
};
#endif /* FEATURE_MMGSDI_GSM_CARD */

#ifdef FEATURE_MMGSDI_3GPP
/* USIM CACHE ELEMENTS */
static mmgsdi_file_enum_type mmgsdi_usim_cache_uim_enums[] =
{
  MMGSDI_USIM_IMSI,      MMGSDI_USIM_KEYS,      MMGSDI_USIM_KEYSPS, MMGSDI_USIM_HPLMN,    MMGSDI_USIM_ACM_MAX,
  MMGSDI_USIM_UST,       MMGSDI_USIM_EST,       MMGSDI_USIM_ACM,    MMGSDI_USIM_SPN,      MMGSDI_USIM_PUCT,
  MMGSDI_USIM_ACC,       MMGSDI_USIM_LOCI,      MMGSDI_USIM_PSLOCI, MMGSDI_USIM_EMLPP,    MMGSDI_USIM_AAEM,
  MMGSDI_USIM_START_HFN, MMGSDI_USIM_THRESHOLD, MMGSDI_USIM_KC,     MMGSDI_USIM_PLMNWACT, MMGSDI_USIM_OPLMNWACT,
  MMGSDI_USIM_HPLMNWACT, MMGSDI_USIM_RPLMNACT,  MMGSDI_USIM_FPLMN,  MMGSDI_USIM_LI,       MMGSDI_USIM_KCGPRS,
  MMGSDI_USIM_AD,        MMGSDI_USIM_CBMID,     MMGSDI_USIM_EHPLMN,    MMGSDI_USIM_EHPLMNPI, MMGSDI_USIM_LRPLMNSI,
  MMGSDI_USIM_EPSLOCI,   MMGSDI_USIM_EPSNSC,    MMGSDI_USIM_HIDDENKEY, MMGSDI_USIM_SPDI,     MMGSDI_USIM_ECC,
  MMGSDI_USIM_GID1,      MMGSDI_USIM_GID2,      MMGSDI_USIM_NASCONFIG, MMGSDI_USIM_7F66_PROP1_RAT,
  MMGSDI_USIM_7F66_PROP1_SPT_TABLE,             MMGSDI_USIM_7F66_PROP1_ACT_HPLMN,            MMGSDI_USIM_PROSE_PST,
  MMGSDI_USIM_OPL,       MMGSDI_USIM_PNN,       MMGSDI_USIM_ACSGL,     MMGSDI_USIM_OCSGL,    MMGSDI_USIM_CSGT,
  MMGSDI_USIM_OCSGT,     MMGSDI_USIM_HNBN,      MMGSDI_USIM_OHNBN,     MMGSDI_USIM_ARR,      MMGSDI_USIM_MSISDN
};
#endif /* FEATURE_MMGSDI_3GPP */

#ifdef FEATURE_MMGSDI_3GPP2
/* RUIM CACHE ELEMENTS */
static mmgsdi_file_enum_type mmgsdi_ruim_cache_uim_enums[] =
{
  MMGSDI_CDMA_IMSI_M,              MMGSDI_CDMA_IMSI_T,        MMGSDI_CDMA_SVC_TABLE,        MMGSDI_CDMA_PREF_LANG,
  MMGSDI_CDMA_HOME_SVC_PVDR_NAME,  MMGSDI_CDMA_ECC,           MMGSDI_CDMA_PRL,              MMGSDI_CDMA_EPRL,
  MMGSDI_CDMA_3GPD_MIPUPP,         MMGSDI_CDMA_3GPD_SIPUPP,   MMGSDI_CDMA_HRPDUPP,          MMGSDI_CDMA_SF_EUIM_ID,
  MMGSDI_CDMA_SERVICE_PREF,        MMGSDI_CDMA_NAM_LOCK,      MMGSDI_CDMA_HOME_SID_NID,     MMGSDI_CDMA_ZONE_BASED_REGN_IND,
  MMGSDI_CDMA_DIST_BASED_REGN_IND, MMGSDI_CDMA_SYS_REGN_IND,  MMGSDI_CDMA_OTAPA_SPC_ENABLE, MMGSDI_CDMA_RUIM_ID,
  MMGSDI_CDMA_ACCOLC,              MMGSDI_CDMA_3GPD_MIPSP,    MMGSDI_CDMA_3GPD_MIPFLAGS,    MMGSDI_CDMA_CALL_TERM_MODE_PREF,
  MMGSDI_CDMA_3GPD_3GPDOPM,        MMGSDI_CDMA_3GPD_SIPPAPSS, MMGSDI_CDMA_SMS_CAP,          MMGSDI_CDMA_MSISDN,
  MMGSDI_CDMA_TMSI,                MMGSDI_CDMA_SCI,           MMGSDI_CDMA_3GPD_DGC,         MMGSDI_CDMA_3GPD_UPPEXT
};

/* CSIM Cache Elements */
static mmgsdi_file_enum_type mmgsdi_csim_cache_uim_enums[] =
{
  MMGSDI_CSIM_IMSI_M,            MMGSDI_CSIM_IMSI_T,                   MMGSDI_CSIM_CST,                      MMGSDI_CSIM_LANG_IND,
  MMGSDI_CSIM_EST,               MMGSDI_CSIM_AD,                       MMGSDI_CSIM_RUIM_ID,                  MMGSDI_CSIM_SERVICE_PREF,
  MMGSDI_CSIM_ACCOLC,            MMGSDI_CSIM_PRL,                      MMGSDI_CSIM_EPRL,                     MMGSDI_CSIM_MAXIMUM_PRL,
  MMGSDI_CSIM_HIDDEN_KEY,        MMGSDI_CSIM_SPN,                      MMGSDI_CSIM_ECC,                      MMGSDI_CSIM_3GPD_MIPUPP,
  MMGSDI_CSIM_3GPD_SIPUPP,       MMGSDI_CSIM_HRPDUPP,                  MMGSDI_CSIM_SF_EUIM_ID,               MMGSDI_CSIM_NAM_LOCK,
  MMGSDI_CSIM_CDMA_HOME_SID_NID, MMGSDI_CSIM_CDMA_ZONE_BASED_REGN_IND, MMGSDI_CSIM_CDMA_DIST_BASED_REGN_IND, MMGSDI_CSIM_CDMA_SYS_REGN_IND,
  MMGSDI_CSIM_OTAPA_SPC_ENABLE,  MMGSDI_CSIM_3GPD_MIPSP,               MMGSDI_CSIM_3GPD_MIPFLAGS,            MMGSDI_CSIM_CALL_TERM_MODE_PREF,
  MMGSDI_CSIM_3GPD_3GPDOPM,      MMGSDI_CSIM_3GPD_SIPPAPSS,            MMGSDI_CSIM_ARR,                      MMGSDI_CSIM_SMS_CAP,
  MMGSDI_CSIM_MSISDN,            MMGSDI_CSIM_TMSI,                     MMGSDI_CSIM_SSCI,                     MMGSDI_CSIM_3GPD_DGC,
  MMGSDI_CSIM_3GPD_UPPEXT
};
#endif /* FEATURE_MMGSDI_3GPP2 */

static mmgsdi_file_enum_type mmgsdi_common_cache_uim_enums[] =
{
  MMGSDI_ICCID,                  MMGSDI_ELP,              MMGSDI_DIR,           MMGSDI_TELECOM_MLPL,    
  MMGSDI_TELECOM_MSPL,           MMGSDI_TELECOM_MMSSMODE
};


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE_CACHE_ELEMENT

   DESCRIPTION:
     This function deletes cached file element

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None

CRITICAL SECTIONS:
     The calling function must enter mmgsdi_cache_crit_sect crit
     sect before invoking this util function. Protection for cache pointers.
==========================================================================*/
static void mmgsdi_cache_delete_cache_element (
  mmgsdi_cache_element_type * cache_element_ptr)
{
  mmgsdi_record_cache_node_type  *temp_node_p     = NULL;
  mmgsdi_record_cache_node_type  *current_node_p  = NULL;

  if(cache_element_ptr == NULL)
  {
    return;
  }

  if (cache_element_ptr->init == MMGSDI_CACHE_NOT_INIT)
  {
    return;
  }

  if(cache_element_ptr->init == MMGSDI_CACHE_NOT_FOUND)
  {
    cache_element_ptr->init = MMGSDI_CACHE_NOT_INIT;
    cache_element_ptr->in_synch = FALSE;
    return;
  }

  cache_element_ptr->init = MMGSDI_CACHE_NOT_INIT;
  cache_element_ptr->in_synch = FALSE;

  if(cache_element_ptr->file_type == MMGSDI_TRANSPARENT_FILE)
  {
    cache_element_ptr->file_cache.cache_data.data_len = 0;
    MMGSDIUTIL_TMC_MEM_FREE(cache_element_ptr->file_cache.cache_data.data_ptr);
  }
  else
  {
    current_node_p = cache_element_ptr->file_cache.cache_node.head_p;

    while(current_node_p != NULL)
    {
      temp_node_p = current_node_p->next_p;
      MMGSDIUTIL_TMC_MEM_FREE(current_node_p->record_data.record_data.data_ptr);
      MMGSDIUTIL_TMC_MEM_FREE(current_node_p);
      current_node_p = temp_node_p;
    }
    cache_element_ptr->file_cache.cache_node.head_p = NULL;
    cache_element_ptr->file_cache.cache_node.tail_p = NULL;
  }
}/* mmgsdi_cache_delete_cache_element */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_GET_PROV_CACHE_PTRS

   DESCRIPTION:
     This function return provisioning app cache pointer based on the session
     type and the app type.
     It also returns the cache size and the corresponding cache item pointer

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

CRITICAL SECTIONS:
     The calling function must enter mmgsdi_cache_crit_sect crit
     sect before invoking this util function. Protection for cache pointers.
==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_get_prov_cache_ptrs(
  mmgsdi_session_type_enum_type  session_type,
  mmgsdi_cache_element_type    **cache_table_pptr,
  int32                         *cache_size_ptr,
  mmgsdi_file_enum_type        **cache_item_pptr)
{
  mmgsdi_session_id_type     session_id       = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_int_app_info_type * int_app_info_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL_3(cache_table_pptr, cache_item_pptr, cache_size_ptr);

  mmgsdi_status = mmgsdi_util_get_mmgsdi_session_id_from_provisioning_type(session_type,
                                                                           &session_id);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_get_prov_session_info(session_id, NULL, NULL,
                                                    &int_app_info_ptr);

  if(mmgsdi_status != MMGSDI_SUCCESS || int_app_info_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  *cache_table_pptr = int_app_info_ptr->app_cache.cache_table_ptr;
  *cache_size_ptr   = int_app_info_ptr->app_cache.cache_table_size;

  if(*cache_table_pptr == NULL || *cache_size_ptr  == 0 )
  {
    UIM_MSG_ERR_2("Cache table not assigned yet for session_type 0x%x cache size 0x%x",
                  session_type,*cache_size_ptr );
    return MMGSDI_ERROR;
  }

  switch (int_app_info_ptr->app_data.app_type)
  {
#ifdef FEATURE_MMGSDI_GSM_CARD
    case MMGSDI_APP_SIM:
      *cache_item_pptr = mmgsdi_sim_cache_uim_enums;
      break;
#endif /* FEATURE_MMGSDI_GSM_CARD */

#ifdef FEATURE_MMGSDI_3GPP
    case MMGSDI_APP_USIM:
      *cache_item_pptr = mmgsdi_usim_cache_uim_enums;
      break;
#endif /* FEATURE_MMGSDI_3GPP */

#ifdef FEATURE_MMGSDI_3GPP2
    case MMGSDI_APP_RUIM:
      *cache_item_pptr = mmgsdi_ruim_cache_uim_enums;
      break;
    case MMGSDI_APP_CSIM:
      *cache_item_pptr = mmgsdi_csim_cache_uim_enums;
      break;
#endif /* FEATURE_MMGSDI_3GPP2 */

    default:
      UIM_MSG_ERR_1("Invalid app_type for cache access 0x%x",
                    int_app_info_ptr->app_data.app_type);
      return MMGSDI_ERROR;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_cache_get_prov_cache_ptrs */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_GET_COMMON_CACHE_PTR

   DESCRIPTION:
     This function, based on the session_type, determines what slot is it
     associated with and thereby determines the common cache pointer

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Sucessful handling
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None
==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_get_common_cache_ptr(
  mmgsdi_session_type_enum_type  session_type,
  mmgsdi_cache_element_type    **cache_table_pptr)
{
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_slot_id_enum_type   slot_id          = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_session_info_type  *session_info_ptr = NULL;

  switch(session_type)
  {
    case MMGSDI_NON_PROV_SESSION_SLOT_1:
    case MMGSDI_CARD_SESSION_SLOT_1:
      *cache_table_pptr = mmgsdi_common_cache_slot1;
      return MMGSDI_SUCCESS;

    case MMGSDI_NON_PROV_SESSION_SLOT_2:
    case MMGSDI_CARD_SESSION_SLOT_2:
      *cache_table_pptr = mmgsdi_common_cache_slot2;
      return MMGSDI_SUCCESS;

    case MMGSDI_NON_PROV_SESSION_SLOT_3:
    case MMGSDI_CARD_SESSION_SLOT_3:
      *cache_table_pptr = mmgsdi_common_cache_slot3;
      return MMGSDI_SUCCESS;

    default:
      break;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                               mmgsdi_generic_data_ptr->pri_gw_session_id,
                               &slot_id,NULL,NULL,NULL,&session_info_ptr,NULL);
  if(mmgsdi_status != MMGSDI_SUCCESS ||
     session_info_ptr == NULL ||
     session_info_ptr->session_type != session_type)
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                                 mmgsdi_generic_data_ptr->sec_gw_session_id,
                                 &slot_id,NULL,NULL,NULL,&session_info_ptr,NULL);
    if(mmgsdi_status != MMGSDI_SUCCESS ||
       session_info_ptr == NULL ||
       session_info_ptr->session_type != session_type)
    {
      mmgsdi_status = mmgsdi_util_get_session_app_info(
                                   mmgsdi_generic_data_ptr->ter_gw_session_id,
                                   &slot_id,NULL,NULL,NULL,&session_info_ptr,NULL);
      if(mmgsdi_status != MMGSDI_SUCCESS ||
         session_info_ptr == NULL ||
         session_info_ptr->session_type != session_type)
      {
        mmgsdi_status = mmgsdi_util_get_session_app_info(
                                     mmgsdi_generic_data_ptr->pri_1x_session_id,
                                     &slot_id,NULL,NULL,NULL,&session_info_ptr,NULL);
        if(mmgsdi_status != MMGSDI_SUCCESS ||
           session_info_ptr == NULL ||
           session_info_ptr->session_type != session_type)
        {
          mmgsdi_status = mmgsdi_util_get_session_app_info(
                                       mmgsdi_generic_data_ptr->sec_1x_session_id,
                                       &slot_id,NULL,NULL,NULL,&session_info_ptr,NULL);
          if(mmgsdi_status != MMGSDI_SUCCESS ||
             session_info_ptr == NULL ||
             session_info_ptr->session_type != session_type)
          {
            mmgsdi_status = mmgsdi_util_get_session_app_info(
                                         mmgsdi_generic_data_ptr->ter_1x_session_id,
                                         &slot_id,NULL,NULL,NULL,&session_info_ptr,NULL);
            if(mmgsdi_status != MMGSDI_SUCCESS ||
               session_info_ptr == NULL ||
               session_info_ptr->session_type != session_type)
            {
              UIM_MSG_ERR_1("Invalid session type - 0x%x, passed to look for common cache ptr",
                            session_type);
              return MMGSDI_ERROR;
            }
          }
        }
      }
    }
  }

  switch(slot_id)
  {
    case MMGSDI_SLOT_1:
      *cache_table_pptr = mmgsdi_common_cache_slot1;
      mmgsdi_status =  MMGSDI_SUCCESS;
      break;

    case MMGSDI_SLOT_2:
      *cache_table_pptr = mmgsdi_common_cache_slot2;
      mmgsdi_status =  MMGSDI_SUCCESS;
      break;

    case MMGSDI_SLOT_3:
      *cache_table_pptr = mmgsdi_common_cache_slot3;
      mmgsdi_status =  MMGSDI_SUCCESS;
      break;

    default:
      UIM_MSG_ERR_1("Invalid session type - 0x%x, passed to look for common cache ptr",
                    session_type);
      mmgsdi_status = MMGSDI_ERROR;
      break;
  }

  return mmgsdi_status;
}/*mmgsdi_cache_get_common_cache_ptr*/


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_GET_CACHE_PTRS_AND_INDEX

   DESCRIPTION:
     This function maps the passed in file enum to either the common cache or
     it calls the mmgsdi_cache_get_prov_cache_ptrs to get the
     provisioning cache pointer if the file passed in is not part of the
     common cache.
     It returns the cache pointer, cache size, the corresponding cache item
     pointer as well as the index location of the file in that caches

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

CRITICAL SECTIONS:
     The calling function must enter mmgsdi_cache_crit_sect crit
     sect before invoking this util function. Protection for cache pointers.
==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_get_cache_ptrs_and_index(
  mmgsdi_file_enum_type          mmgsdi_file,
  mmgsdi_session_type_enum_type  session_type,
  mmgsdi_cache_element_type    **cache_table_pptr,
  int32                         *cache_size_ptr,
  int32                         *file_index_ptr)
{
  mmgsdi_return_enum_type mmgsdi_status   = MMGSDI_SUCCESS;
  int32                   file_cnt        = 0;
  mmgsdi_file_enum_type  *cache_item_ptr  = NULL;
  int32                   cache_size      = 0;

  MMGSDIUTIL_RETURN_IF_NULL_3(cache_table_pptr, cache_size_ptr, file_index_ptr);

  cache_size = sizeof(mmgsdi_common_cache_uim_enums) /
                 sizeof(mmgsdi_common_cache_uim_enums[0]);

  /* Determine if the file is in common cache or not */
  for(file_cnt = 0; file_cnt < cache_size; file_cnt++)
  {
    if (mmgsdi_common_cache_uim_enums[file_cnt] == mmgsdi_file)
    {
      mmgsdi_status = mmgsdi_cache_get_common_cache_ptr(session_type,cache_table_pptr);
      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        return MMGSDI_ERROR;
      }
      *cache_size_ptr  = cache_size;
      *file_index_ptr  = file_cnt;
      return MMGSDI_SUCCESS;
    }
  }

  mmgsdi_status = mmgsdi_cache_get_prov_cache_ptrs(session_type,
                                                   cache_table_pptr,
                                                   cache_size_ptr,
                                                   &cache_item_ptr);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if(*cache_table_pptr == NULL)
  {
    UIM_MSG_ERR_1("Cache table not assigned yet for session_type 0x%x",
                  session_type);
    return MMGSDI_ERROR;
  }

  for(file_cnt = 0; file_cnt < *cache_size_ptr; file_cnt++)
  {
    if (cache_item_ptr[file_cnt] == mmgsdi_file)
    {
      *file_index_ptr  = file_cnt;
      return MMGSDI_SUCCESS;
    }
  }

  UIM_MSG_ERR_1("Cache not found for file 0x%x", mmgsdi_file);
  return MMGSDI_NOT_FOUND;
} /* mmgsdi_cache_get_cache_ptrs_and_index */


#ifdef FEATURE_UIM_PERSISTENT_CACHE
/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_UPDATE_PERSISTENT_CACHE

   DESCRIPTION:
     This function update the persistent cache with cache data

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

   SIDE EFFECTS:
     None
==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_update_persistent_cache(
  mmgsdi_session_id_type        session_id,
  const mmgsdi_access_type     *file_access_ptr,
  mmgsdi_data_from_enum_type    data_fr_card,
  mmgsdi_rec_num_type           rec_num
)
{
  mmgsdi_slot_id_enum_type    slot_id       = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_return_enum_type     mmgsdi_status = MMGSDI_SUCCESS;
  mmgsdi_data_type            cache_data;

  if(file_access_ptr == NULL ||
     file_access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS ||
     data_fr_card == MMGSDI_DATA_FROM_CLIENT)
  {
    UIM_MSG_HIGH_0("Invalid file_access_ptr or data from client");
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_util_get_prov_session_info(session_id,
                                                    NULL,
                                                    &slot_id,
                                                    NULL);

  if ((mmgsdi_status != MMGSDI_SUCCESS) || (slot_id == MMGSDI_MAX_SLOT_ID_ENUM))
  {
    UIM_MSG_HIGH_1("Failed to get slot_id: mmgsdi_status = 0x%x", mmgsdi_status);
    return mmgsdi_status;
  }

  /* for MMGSDI_DATA_FROM_CARD_INCOMPLETE, if persistent cache for the
     EF is not initialized (ie. not found), then skip the update. */
  memset(&cache_data, 0x00, sizeof(mmgsdi_data_type));
  if(data_fr_card == MMGSDI_DATA_FROM_CARD_INCOMPLETE)
  {
    mmgsdi_status = mmgsdi_persistent_cache_read(slot_id,
                                                 file_access_ptr->file.file_enum,
                                                 rec_num,
                                                 &cache_data.data_len,
                                                 &cache_data.data_ptr);

    MMGSDIUTIL_TMC_MEM_FREE(cache_data.data_ptr);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      UIM_MSG_HIGH_1("data from card incomplete for file 0x%x",
                     file_access_ptr->file.file_enum);
      return mmgsdi_status;
    }
  }

  /* update persistent cache */
  /* retrieve data from cache to write into persistent cache */
  memset(&cache_data, 0x00, sizeof(mmgsdi_data_type));
  mmgsdi_status = mmgsdi_cache_read_len(session_id,
                                        file_access_ptr,
                                        rec_num,
                                        &cache_data.data_len,
                                        NULL);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_1("failed to read cache len for enum 0x%x",
                   file_access_ptr->file.file_enum);
    return mmgsdi_status;
  }

  if(cache_data.data_len <= 0)
  {
    return MMGSDI_ERROR;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cache_data.data_ptr,  cache_data.data_len);
  if (cache_data.data_ptr == NULL)
  {
    /* Unable to allocate memory */
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_cache_read(session_id,
                                    file_access_ptr,
                                    &cache_data,
                                    0,
                                    1,
                                    NULL);

  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    /* write to persistent cache */
    mmgsdi_status = mmgsdi_persistent_cache_write(slot_id,
                                                  file_access_ptr->file.file_enum,
                                                  rec_num,
                                                  cache_data.data_len,
                                                  (const uint8*)cache_data.data_ptr);
  }
  MMGSDIUTIL_TMC_MEM_FREE(cache_data.data_ptr);
  return mmgsdi_status;
} /* mmgsdi_cache_update_persistent_cache */
#endif /* FEATURE_UIM_PERSISTENT_CACHE */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_READ_ITEM_LEN

   DESCRIPTION:
     This function return the cache item len

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read_item_len(
  mmgsdi_file_enum_type         mmgsdi_file,
  mmgsdi_session_type_enum_type session_type,
  mmgsdi_rec_num_type           rec_num,
  mmgsdi_len_type              *total_len_ptr,
  mmgsdi_cache_init_enum_type  *cache_state_ptr
)
{
  /*-----------------------------------------------------------------------------
    Initialize variables
  ------------------------------------------------------------------------------*/
  int32                      file_index      = 0;
  int32                      cache_size      = 0;
  mmgsdi_cache_element_type *cache_table_ptr = NULL;
  mmgsdi_return_enum_type    mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_task_enum_type      curr_task       = MMGSDI_TASK_MAX;

  curr_task = mmgsdi_util_get_current_task();

  /* There are multiple requests to read UST cache before sending READ/WRITE 
     cmd for USIM files; which leads to burst of F3 prints. 
     Skip F3 traces when executed in DIAG/MMGSDI_1/MMGSDI_2/GSDI task
     to reduce logs caused by SIM data collection by QXDM.
  */
  if((mmgsdi_file != MMGSDI_USIM_UST) && 
     !MMGSDIUTIL_IS_DIAG_TASK() &&
     !MMGSDIUTIL_IS_MMGSDI_TASK(curr_task))
  {
    UIM_MSG_HIGH_2("mmgsdi_cache_read_item_len file 0x%x, session_type 0x%x",
                      mmgsdi_file, session_type);
  }

  if(cache_state_ptr != NULL)
  {
    *cache_state_ptr = MMGSDI_CACHE_MAX_ENUM;
  }

  MMGSDIUTIL_RETURN_IF_NULL(total_len_ptr);

  *total_len_ptr = 0;

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_cache_get_cache_ptrs_and_index(mmgsdi_file,
                                                          session_type,
                                                          &cache_table_ptr,
                                                          &cache_size,
                                                          &file_index);
    if((mmgsdi_status != MMGSDI_SUCCESS) || (cache_table_ptr == NULL) ||
       (file_index >= cache_size))
    {
      if(mmgsdi_status == MMGSDI_SUCCESS)
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;
    }

    if(cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_INIT)
    {
      UIM_MSG_ERR_1("Cache not initialized 0x%x", mmgsdi_file);
      mmgsdi_status = MMGSDI_NOT_INIT;
      break;
    }
    else if( cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_FOUND )
    {
      UIM_MSG_HIGH_1("Cache status indicates that file not found on card 0x%x",
                     mmgsdi_file);

      if(cache_state_ptr != NULL)
      {
        *cache_state_ptr = MMGSDI_CACHE_NOT_FOUND;
      }
      mmgsdi_status = MMGSDI_NOT_FOUND;
      break;
    }

    if(cache_table_ptr[file_index].file_type == MMGSDI_TRANSPARENT_FILE)
    {
      *total_len_ptr =
        cache_table_ptr[file_index].file_cache.cache_data.data_len;
    }
    else
    {
      mmgsdi_record_cache_node_type      * current_node_p   = NULL;

      if(rec_num <= 0)
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }

      /* --------------------------------------------------------------------------
         check to see if mmgsdi_record_cache_list_node has been set with a
         valid Record entry. Also initialize mmgsdi_status for records.
         ------------------------------------------------------------------------*/
      mmgsdi_status = MMGSDI_NOT_FOUND;
      current_node_p = cache_table_ptr[file_index].file_cache.cache_node.head_p;

      while(current_node_p != NULL)
      {
        if(current_node_p->record_data.rec_num == rec_num)
        {
          *total_len_ptr = current_node_p->record_data.record_data.data_len;
          mmgsdi_status = MMGSDI_SUCCESS;
          break;
        }
        current_node_p = current_node_p->next_p;
      }
    }
  }while(0);

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_read_item_len */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_READ_LEN

   DESCRIPTION:
     This function return the total len - offset of the cache item

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          Copy Success
     MMGSDI_ERROR:            Unable to Copy
     MMGSDI_INCORRECT_PARAMS: Null input pointer

   SIDE EFFECTS:
     If the same EF is presented in more than 1 cache, it will return the data
     from the first cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read_len(
  mmgsdi_session_id_type         session_id,
  const mmgsdi_access_type      *file_access_ptr,
  mmgsdi_rec_num_type            rec_num,
  mmgsdi_len_type               *data_len_ptr,
  mmgsdi_cache_init_enum_type   *cache_state_ptr
)
{
  mmgsdi_return_enum_type       mmgsdi_status  = MMGSDI_SUCCESS;
  mmgsdi_session_type_enum_type session_type   = MMGSDI_MAX_SESSION_TYPE_ENUM;

  if(cache_state_ptr != NULL)
  {
    *cache_state_ptr = MMGSDI_CACHE_MAX_ENUM;
  }

  MMGSDIUTIL_RETURN_IF_NULL_2(file_access_ptr, data_len_ptr);

  if(file_access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS)
  {

    UIM_MSG_ERR_1("Skip Cache: not by EF ENUM ACCESS: 0x%x",
                  file_access_ptr->access_method);
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  return mmgsdi_cache_read_item_len(file_access_ptr->file.file_enum,
                                    session_type,
                                    rec_num,
                                    data_len_ptr,
                                    cache_state_ptr);
} /* mmgsdi_cache_read_len */


/*===========================================================================
FUNCTION MMGSDI_CACHE_POPULATE_ATTR_CACHE_FROM_STATIC_FILE_TABLE

DESCRIPTION
  Checks for file attributes in static sfi table, if found attr ptr is returned.
  If not return error.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  If the calling function expects the attributes data, then *cache_attr_pptr
  will hold the attributes and the calling function should take care of freeing
  this ptr along with its member ptrs.
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_populate_attr_cache_from_static_file_table(
  mmgsdi_decoded_attr_cache_type    **cache_attr_pptr,
  const mmgsdi_access_type           *access_ptr,
  mmgsdi_slot_id_enum_type            slot,
  mmgsdi_file_security_access_type   *security_access_out_ptr
)
{
  mmgsdi_return_enum_type            mmgsdi_status          = MMGSDI_ERROR;
  mmgsdi_file_size_type              file_size              = 0;
  mmgsdi_len_type                    rec_len                = 0;
  mmgsdi_rec_num_type                num_of_rec             = 0;
  boolean                           *increase_ptr           = NULL;
  mmgsdi_file_security_access_type  *security_access_in_ptr = NULL;
  mmgsdi_file_attributes_type        file_attr;
  mmgsdi_file_sfi_table_type         file_info;

  MMGSDIUTIL_RETURN_IF_NULL_2(cache_attr_pptr, access_ptr);

  memset(&file_info, 0x00, sizeof(mmgsdi_file_sfi_table_type));
  memset(&file_attr, 0x00, sizeof(file_attr));

  if (!mmgsdi_file_is_sfi_supported(slot))
  {
    return MMGSDI_ERROR;
  }

  /* File attributes are not cached yet. Lets try to fetch the attributes
    1. First try searching the static tables for the attributes IF:
       a). access type is EF_ENUM AND
       b). caller did not mandate an explicit SELECT
    2. If not found in static table or if the caller mandated an explicit
       SELECT, get attributes by performing an explicit SELECT to the card */

  if(access_ptr->access_method == MMGSDI_EF_ENUM_ACCESS)
  {
    /* If SFI is enabled, see if the hardcoded tables have file/rec size and
       security attributes related info */
    mmgsdi_status = mmgsdi_file_get_file_info_from_table(access_ptr->file.file_enum, &file_info);
  }

  if (MMGSDI_SUCCESS != mmgsdi_status ||
      file_info.size == MMGSDI_INVALID_FILE_SIZE)
  {
    return MMGSDI_ERROR;
  }

  /* We got the file/rec size info from the static SFI table. Now lets
     get the security attributes from the static table as well and
     update the attributes cache with all this info. */

  file_attr.file_type = file_info.file_type;

  mmgsdi_status = mmgsdi_file_set_default_sec_attr_per_spec(&file_attr.file_info,
                                                            file_attr.file_type,
                                                            access_ptr->file.file_enum);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  switch(file_info.file_type)
  {
    case MMGSDI_LINEAR_FIXED_FILE:
      security_access_in_ptr = &file_attr.file_info.linear_fixed_file.file_security;
      rec_len = file_info.size;
      /* There is no way for our static SFI tables to have num_of_recs.
         For that reason num_of_rec is zero here for the files for which
         found a match in the SFI table otherwise */
      num_of_rec = 0;
      file_size = num_of_rec * rec_len;
      break;
    case MMGSDI_TRANSPARENT_FILE:
      security_access_in_ptr = &file_attr.file_info.transparent_file.file_security;
      file_size = file_info.size;
      break;
    case MMGSDI_CYCLIC_FILE:
      security_access_in_ptr = &file_attr.file_info.cyclic_file.file_security;
      increase_ptr = &file_attr.file_info.cyclic_file.increase_allowed;
      rec_len = file_info.size;
      /* There is no way for our static SFI tables to have num_of_recs.
         For that reason num_of_rec is zero here for the files for which
         found a match in the SFI table otherwise */
      num_of_rec = 0;
      file_size = num_of_rec * rec_len;
    default:
      UIM_MSG_ERR_1("Invalid file_type 0x%x",
                    file_attr.file_type);
      return MMGSDI_ERROR;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE((*cache_attr_pptr),
                                     sizeof(mmgsdi_decoded_attr_cache_type));
  if(*cache_attr_pptr == NULL)
  {
    mmgsdi_file_free_security_info(security_access_in_ptr);
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Write the attributes fetched from the static tables to the file attributes cache */
  (void) mmgsdi_util_copy_access_type(&((*cache_attr_pptr)->access),
                                      access_ptr);
  if(increase_ptr)
  {
    (*cache_attr_pptr)->cyclic_increase_allowed = *increase_ptr;
  }

  (*cache_attr_pptr)->file_size           = file_size;
  (*cache_attr_pptr)->file_type           = file_info.file_type;
  (*cache_attr_pptr)->num_of_rec          = num_of_rec;
  (*cache_attr_pptr)->rec_len             = rec_len;
  (*cache_attr_pptr)->sfi                 = file_info.sfi;

  /* Fill security parameters when it is requested */
  if(security_access_out_ptr != NULL)
  {
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_in_ptr->read,
                                                    &(security_access_out_ptr->read));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_in_ptr->write,
                                                    &(security_access_out_ptr->write));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_in_ptr->increase,
                                                    &(security_access_out_ptr->increase));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_in_ptr->invalidate_deactivate,
                                                    &(security_access_out_ptr->invalidate_deactivate));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_in_ptr->rehabilitate_activate,
                                                    &(security_access_out_ptr->rehabilitate_activate));
  }

  mmgsdi_file_free_security_info(security_access_in_ptr);
  return mmgsdi_status;
}/* mmgsdi_cache_populate_attr_cache_from_static_file_table */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_WRITE_ITEM

   DESCRIPTION:
     This function writes to the cache

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_write_item(
  mmgsdi_file_enum_type         mmgsdi_file,
  mmgsdi_session_type_enum_type session_type,
  mmgsdi_len_type               data_len,
  mmgsdi_len_type               data_offset,
  const uint8 *                 data_ptr,
  mmgsdi_data_from_enum_type    data_fr_card,
  mmgsdi_rec_num_type           rec_num
)
{
  /*-----------------------------------------------------------------------------
    Initialize variables
  ------------------------------------------------------------------------------*/
  int32                            file_index                 = 0;
  mmgsdi_return_enum_type          mmgsdi_status              = MMGSDI_SUCCESS;
  int32                            cache_size                 = 0;
  mmgsdi_cache_element_type       *cache_table_ptr            = NULL;
  mmgsdi_access_type               access_type;
  mmgsdi_session_id_type           session_id                 = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_slot_id_enum_type         slot_id                    = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_decoded_attr_cache_type  *cache_attr_ptr             = NULL;
  boolean                          support_extended_fplmn     = FALSE;
  mmgsdi_file_structure_enum_type  file_type                  = MMGSDI_TRANSPARENT_FILE;
  mmgsdi_slot_data_type           *slot_data_ptr              = NULL;

  UIM_MSG_HIGH_2("mmgsdi_cache_write_item file 0x%x, session_type 0x%x",
                 mmgsdi_file, session_type);

  memset(&access_type, 0x00, sizeof(mmgsdi_access_type));

  access_type.access_method  = MMGSDI_EF_ENUM_ACCESS;
  access_type.file.file_enum = mmgsdi_file;

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_cache_get_cache_ptrs_and_index(mmgsdi_file,
                                                          session_type,
                                                          &cache_table_ptr,
                                                          &cache_size,
                                                          &file_index);
    if((mmgsdi_status != MMGSDI_SUCCESS) || (cache_table_ptr == NULL) ||
       (file_index >= cache_size))
    {
      if(mmgsdi_status == MMGSDI_SUCCESS)
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;
    }

    mmgsdi_status = mmgsdi_util_get_mmgsdi_session_id_from_type(session_type,
                                                                &session_id);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Mark the cache as not found if file is not found on card */
    if(data_fr_card == MMGSDI_DATA_NOT_FOUND &&
       rec_num == 0)
    {
      cache_table_ptr[file_index].init = MMGSDI_CACHE_NOT_FOUND;
      mmgsdi_status = MMGSDI_SUCCESS;
      break;
    }

    mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                     &slot_id,
                                                     NULL,
                                                     NULL,
                                                     NULL,
                                                     NULL,
                                                     NULL);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);
    if(slot_data_ptr == NULL)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* Check if the attributes are already cached */
    mmgsdi_status = mmgsdi_cache_attr_read(session_id,
                                           &access_type,
                                           &cache_attr_ptr,
                                           MMGSDI_SKIP_SECURITY_ATTRIBUTES,
                                           NULL,
                                           NULL,
                                           NULL);

    if((mmgsdi_status == MMGSDI_SUCCESS) && cache_attr_ptr != NULL)
    {
      file_type = cache_attr_ptr->file_type;
    }

#ifdef FEATURE_UIM_PERSISTENT_CACHE
    /* We don't have file attribute cache during profile switch when persistance profile is active.
       So we need to bypass attribute cache read failure here. We will use TRANSPARENT_FILE as
       default file type for all files that we allow caching in persistent cache other than
       MMGSDI_CSIM_CDMA_ZONE_BASED_REGN_IND, MMGSDI_USIM_EPSNSC as they are linear fixed files */
    else if(mmgsdi_persistent_cache_is_profile_active(slot_id, session_id))
    {
      if(mmgsdi_file == MMGSDI_CSIM_CDMA_ZONE_BASED_REGN_IND ||
         mmgsdi_file == MMGSDI_USIM_EPSNSC)
      {
        file_type = MMGSDI_LINEAR_FIXED_FILE;
      }
    }
#endif /* FEATURE_UIM_PERSISTENT_CACHE */

    /* MMGSDI caches EF-ICCID without reading its attributes based on data passed
       in from the Link Established message. Also while caching file data
       during onchip activation we don't have file attribute cache. So we need to bypass attribute
       cache read failure. We will use TRANSPARENT_FILE as default file type */
    else if(mmgsdi_file != MMGSDI_ICCID &&
            slot_data_ptr->onchip_sim_data.state != MMGSDI_ONCHIP_SIM_INIT)
    {
      /* File attribtues are not cached yet and if we are here. It means MMGSDI uses static
         table to fetch the attributes (for EF_ENUM type access only) while reading the files */
      mmgsdi_status = mmgsdi_cache_populate_attr_cache_from_static_file_table(&cache_attr_ptr,
                                                                              &access_type,
                                                                              slot_id,
                                                                              NULL);
      if(MMGSDI_SUCCESS != mmgsdi_status)
      {
        break;
      }
      file_type = cache_attr_ptr->file_type;
    }
    mmgsdi_status = MMGSDI_SUCCESS;
    MMGSDIUTIL_TMC_MEM_FREE(cache_attr_ptr);
    cache_table_ptr[file_index].file_type = file_type;

    /* Check Extended FPLMN NV status when file is FPLMN and onchip is not enabled.
       If ENABLED, write complete length in cache. Return error if the req lenth is
       not equal to FPLMN cache size and greater than card size */
    if(slot_data_ptr->onchip_sim_data.state != MMGSDI_ONCHIP_SIM_INIT &&
       (mmgsdi_file == MMGSDI_GSM_FPLMN ||
        mmgsdi_file == MMGSDI_USIM_FPLMN) &&
       mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SUPPORT_EXTENDED_FPLMN)
          == MMGSDI_FEATURE_ENABLED)
    {
      mmgsdi_int_app_info_type        *app_info_ptr               = NULL;

      mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                       NULL,
                                                       NULL,
                                                       &app_info_ptr,
                                                       NULL,
                                                       NULL,
                                                       NULL);
      if(app_info_ptr == NULL)
      {
        break;
      }

      UIM_MSG_MED_1("Support Extended FPLMN with data_len 0x%x", data_len);

      if(data_len > app_info_ptr->fplmn_actual_size_on_card &&
         data_len != MMGSDI_EXTENDED_FPLMN_CACHE_LEN)
      {
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
        break;
      }
      support_extended_fplmn = TRUE;

      if(mmgsdi_file == MMGSDI_USIM_FPLMN &&
         app_info_ptr->fplmn_actual_size_on_card >= MMGSDI_EXTENDED_FPLMN_CACHE_LEN)
      {
        support_extended_fplmn = FALSE;
      }
    }

    /* Below logic is specific to transparent files. It can not be applied to
       record based caches. If any record is not found we should not mark file
       status as NOT FOUND */
    if(cache_table_ptr[file_index].file_type  == MMGSDI_TRANSPARENT_FILE)
    {
      if (cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_FOUND)
      {
        UIM_MSG_HIGH_1("Cache status indicates file not found on card 0x%x",
                       mmgsdi_file);
        mmgsdi_status = MMGSDI_NOT_FOUND;
        break;
      }
    }

    if (data_ptr == NULL)
    {
      MMGSDIUTIL_DEBUG_MSG_ERROR_0("Attempt to use NULL POINTER");
      mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      break;
    }

    /*-----------------------------------------------------------------------------
      Handle the special case of updating the data len in cache during init when
      GSDI is caching information.  During init, GSDI caches whole records
      partial records or offsets of records are not cached during init
    -----------------------------------------------------------------------------*/
    if(cache_table_ptr[file_index].file_type == MMGSDI_TRANSPARENT_FILE)
    {
      /* Need to first update the Length of the file */
      /* Is this the first time reading the file ?     */
      if (cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_INIT)
      {
        /* Do not allow external Write Cache request for an item
           which is in our post pin cache list when cache is empty. */
        /* Also ensure the cache is initalized with the full size
           of the file by check if the card is init complete. */
        if((data_fr_card == MMGSDI_DATA_FROM_CLIENT &&
            mmgsdi_util_find_ef_in_power_up_cache_list(mmgsdi_file)) ||
            data_fr_card == MMGSDI_DATA_FROM_CARD_INCOMPLETE)
        {
          UIM_MSG_HIGH_2("Unable to initalize cache for file: 0x%x, data from card: 0x%x",
                         mmgsdi_file, data_fr_card);
          mmgsdi_status = MMGSDI_ERROR;
          break;
        }

        /* OK to update to length returned */
        /* Need to avoid updating to length 0 */
        if(data_len > 0)
        {
          /* If file is FPLMN with extended support, allocate complete chunk
             of 60 Bytes and memset it with 0xFF to make the content invalid */
          if (support_extended_fplmn)
          {
            MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
              cache_table_ptr[file_index].file_cache.cache_data.data_ptr,
              MMGSDI_EXTENDED_FPLMN_CACHE_LEN);

            if(cache_table_ptr[file_index].file_cache.cache_data.data_ptr == NULL)
            {
              mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }
            cache_table_ptr[file_index].file_cache.cache_data.data_len =
              MMGSDI_EXTENDED_FPLMN_CACHE_LEN;
            memset(cache_table_ptr[file_index].file_cache.cache_data.data_ptr, 0xFF,
                   MMGSDI_EXTENDED_FPLMN_CACHE_LEN);
          }
          else
          {
            MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
              cache_table_ptr[file_index].file_cache.cache_data.data_ptr,
              data_len);

            if(cache_table_ptr[file_index].file_cache.cache_data.data_ptr == NULL)
            {
              mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }
            cache_table_ptr[file_index].file_cache.cache_data.data_len = data_len;
          }
        } /* endif - data len is not 0 */
      } /* cache file ! init */

      /*-----------------------------------------------------------------------------
        Handle case where requested length is longer than the length of the cached data
      -----------------------------------------------------------------------------*/
      if (data_offset + data_len > cache_table_ptr[file_index].file_cache.cache_data.data_len)
      {
        UIM_MSG_ERR_3("Data offset 0x%x + data len 0x%x > file size 0x%x",
                      data_offset,
                      data_len,
                      cache_table_ptr[file_index].file_cache.cache_data.data_len);
        if (cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_INIT)
        {
          MMGSDIUTIL_TMC_MEM_FREE(cache_table_ptr[file_index].file_cache.cache_data.data_ptr);
          cache_table_ptr[file_index].file_cache.cache_data.data_len = 0;
        }
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
        break;
      } /* end if - requested data is too long */

      /*-----------------------------------------------------------------------------
        Proceed with cache write, Data length of 0 indicates the max number of bytes
        to be written
      -----------------------------------------------------------------------------*/
      if (data_len == 0)
      {
        if (cache_table_ptr[file_index].file_cache.cache_data.data_len <= data_offset)
        {
          UIM_MSG_ERR_2("Data offset 0x%x same or greater than total len 0x%x",
                        data_offset, cache_table_ptr[file_index].file_cache.cache_data.data_len);
          if (cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_INIT)
          {
            MMGSDIUTIL_TMC_MEM_FREE(cache_table_ptr[file_index].file_cache.cache_data.data_ptr);
            cache_table_ptr[file_index].file_cache.cache_data.data_len = 0;
          }
          mmgsdi_status = MMGSDI_ERROR;
          break;
        }
        data_len = cache_table_ptr[file_index].file_cache.cache_data.data_len - data_offset;
      } /* Write max data length */

      /* Copy contents to be written into cache */
      mmgsdi_memscpy((cache_table_ptr[file_index].file_cache.cache_data.data_ptr) + data_offset,
                     cache_table_ptr[file_index].file_cache.cache_data.data_len - data_offset,
                     data_ptr,
                     data_len);
      cache_table_ptr[file_index].init = MMGSDI_CACHE_INIT;

     /* Update whether the cache data is in synch with card or not,
        In the New arch, util write is called only after a process to the card
        It does not have the write to cache and then queue the command */
      switch (data_fr_card)
      {
        case MMGSDI_DATA_FROM_CARD_INCOMPLETE:
        case MMGSDI_DATA_FROM_CARD_COMPLETE:
          cache_table_ptr[file_index].in_synch = TRUE;
          break;
        case MMGSDI_DATA_FROM_CLIENT:
        default:
          cache_table_ptr[file_index].in_synch = FALSE;
          break;
      }
    }
    else
    {
      mmgsdi_record_cache_node_type      * new_node_p       = NULL;

      /* Return error if record number is invalid or
         length is invalid or
         record in not found or
         client is trying to update the file cache before MMGSDI initialize it
         as part of card power up/app activation */
      if(rec_num <= 0 || data_len <= 0 ||
         data_fr_card == MMGSDI_DATA_NOT_FOUND ||
         (data_fr_card == MMGSDI_DATA_FROM_CLIENT &&
          mmgsdi_util_find_ef_in_power_up_cache_list(mmgsdi_file) &&
          cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_INIT))
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }

      /* --------------------------------------------------------------------------
         Allocate memory for a node to be linked in to the list
         ------------------------------------------------------------------------*/
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(new_node_p, sizeof(mmgsdi_record_cache_node_type))

      if (new_node_p == NULL )
      {
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
        break;
      }

      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
        new_node_p->record_data.record_data.data_ptr,
        data_len)

      if (new_node_p->record_data.record_data.data_ptr == NULL )
      {
        MMGSDIUTIL_TMC_MEM_FREE(new_node_p);
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
        break;
      }

      (void)mmgsdi_cache_delete_record(session_id, access_type, rec_num);

      cache_table_ptr[file_index].init = MMGSDI_CACHE_INIT;

      /* For any record updated in client context we set in_synch as FALSE.
         It will be set to TRUE when we receive write response from card*/
      switch (data_fr_card)
      {
        case MMGSDI_DATA_FROM_CARD_INCOMPLETE:
        case MMGSDI_DATA_FROM_CARD_COMPLETE:
          cache_table_ptr[file_index].in_synch = TRUE;
          break;
        case MMGSDI_DATA_FROM_CLIENT:
        default:
          cache_table_ptr[file_index].in_synch = FALSE;
          break;
      }

      /* --------------------------------------------------------------------------
         Set the Node Pointer information
         ------------------------------------------------------------------------*/
      new_node_p->next_p  = NULL;

      new_node_p->record_data.rec_num = rec_num;
      new_node_p->record_data.record_data.data_len = data_len;
      mmgsdi_memscpy(new_node_p->record_data.record_data.data_ptr,
                     data_len,
                     data_ptr,
                     data_len);

      /* --------------------------------------------------------------------------
         First element
         ----------------------------------------------------------------------- */
      if(cache_table_ptr[file_index].file_cache.cache_node.head_p == NULL)
      {
        cache_table_ptr[file_index].file_cache.cache_node.head_p = new_node_p;
        cache_table_ptr[file_index].file_cache.cache_node.tail_p = new_node_p;
        break;
      }
      else
      {
        /* --------------------------------------------------------------------------
           From this point on...running because this is not the first time
           an entry is being made into this list.  start by locating the tail
           of the list.  The new node will be linked to the tail to become the new
           tail.
           ----------------------------------------------------------------------- */
        /* --------------------------------------------------------------------------
           Now link the new_node_p to the tail of the list.
           ------------------------------------------------------------------------*/
        cache_table_ptr[file_index].file_cache.cache_node.tail_p->next_p = new_node_p;
        cache_table_ptr[file_index].file_cache.cache_node.tail_p = new_node_p;
      }
    }
  }while(0);

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_write_item */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_CREATE_PROV_FILE_CACHE_CLEAR_CACHES

   DESCRIPTION:
     If the function is invoked due to a Session activation, this function
     creates the provisioning file cache by allocating the
     exact number of memory space required based on the corresponding
     session type and app type.
     If the provisioning file cache already exists, the file cache is
     cleared and allocated memory freed.
     This function also clears the File attributes cache and the record cache.

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_create_prov_file_cache_clear_caches(
  mmgsdi_session_id_type        session_id,
  boolean                       session_activate
)
{
  mmgsdi_return_enum_type         mmgsdi_status            = MMGSDI_SUCCESS;
  mmgsdi_session_type_enum_type   session_type             = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_aid_type                 aid_type;
  mmgsdi_int_app_info_type       *int_app_info_ptr         = NULL;

  memset(&aid_type, 0x00, sizeof(mmgsdi_aid_type));

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               &aid_type);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  UIM_MSG_HIGH_1("mmgsdi_cache_create_prov_file_cache_clear_caches for session_type 0x%x",
                 session_type);

  /* Delete all the cached data for app specific files */
  if(session_activate == FALSE &&
     (mmgsdi_nv_get_device_feature_status(
        MMGSDI_FEATURE_KEEP_CACHE_AFTER_SESSION_DEACTIVATION)
        == MMGSDI_FEATURE_DISABLED))
  {
    mmgsdi_status = mmgsdi_util_get_session_app_info(
                      session_id,
                      NULL,
                      NULL,
                      &int_app_info_ptr,
                      NULL,
                      NULL,
                      NULL);

    if(mmgsdi_status == MMGSDI_SUCCESS &&
       int_app_info_ptr != NULL)
    {
      /* Clear the File attribute cache */
      (void)mmgsdi_cache_delete_attr_selective_items(
              &int_app_info_ptr->attr_cache_tbl, NULL, 0, NULL, 0);

      mmgsdi_eons_opl_pnn_delete(&int_app_info_ptr->opl_cache);
      mmgsdi_eons_opl_pnn_delete(&int_app_info_ptr->pnn_cache);

      /* Reset the CSG EONS info. The related EFs are read after
         subscription is published */
      mmgsdi_csg_reset_eons_flags_in_app_info(int_app_info_ptr);

      /* Clear the file cache */
      (void)mmgsdi_cache_delete(int_app_info_ptr, 0, NULL);
    }
  }
  return mmgsdi_status;
} /* mmgsdi_cache_create_prov_file_cache_clear_caches */


/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_FILE_ATTR_CACHE_ITEM

DESCRIPTION:
  This function clears specific file attributes cache item

DEPENDENCIES:
  None

LIMITATIONS:
   None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void mmgsdi_cache_delete_file_attr_cache_item(
  mmgsdi_attr_cache_element_type ** attr_cache_element_pptr)
{
  if((*attr_cache_element_pptr) != NULL)
  {
    mmgsdi_file_free_security_info(&((*attr_cache_element_pptr)->security_access));
    MMGSDIUTIL_TMC_MEM_FREE((*attr_cache_element_pptr)->raw_file_attr_data.data_ptr);
  }

  MMGSDIUTIL_TMC_MEM_FREE((*attr_cache_element_pptr));
} /* mmgsdi_cache_delete_file_attr_cache_item */


/*===========================================================================
FUNCTION MMGSDI_CACHE_RESET_FILE_ATTR_CACHE_ITEM

DESCRIPTION:
  This function resets specific file attributes cache item (without deleting
  the item from the corresponding cache list)

DEPENDENCIES:
  None

LIMITATIONS:
   None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static void mmgsdi_cache_reset_file_attr_cache_item(
  mmgsdi_attr_cache_element_type ** attr_cache_element_pptr)
{
  if((*attr_cache_element_pptr) == NULL)
  {
    return;
  }

  (*attr_cache_element_pptr)->security_access.read.protection_method  = MMGSDI_ALWAYS_ALLOWED;
  (*attr_cache_element_pptr)->security_access.read.num_protection_pin = 0;
  (*attr_cache_element_pptr)->security_access.write.protection_method  = MMGSDI_ALWAYS_ALLOWED;
  (*attr_cache_element_pptr)->security_access.write.num_protection_pin = 0;
  (*attr_cache_element_pptr)->security_access.increase.protection_method  = MMGSDI_ALWAYS_ALLOWED;
  (*attr_cache_element_pptr)->security_access.increase.num_protection_pin = 0;
  (*attr_cache_element_pptr)->security_access.invalidate_deactivate.protection_method  = MMGSDI_ALWAYS_ALLOWED;
  (*attr_cache_element_pptr)->security_access.invalidate_deactivate.num_protection_pin = 0;
  (*attr_cache_element_pptr)->security_access.rehabilitate_activate.protection_method  = MMGSDI_ALWAYS_ALLOWED;
  (*attr_cache_element_pptr)->security_access.rehabilitate_activate.num_protection_pin = 0;

  mmgsdi_file_free_security_info(&((*attr_cache_element_pptr)->security_access));

  (*attr_cache_element_pptr)->raw_file_attr_data.data_len = 0;
  MMGSDIUTIL_TMC_MEM_FREE((*attr_cache_element_pptr)->raw_file_attr_data.data_ptr);
} /* mmgsdi_cache_reset_file_attr_cache_item */


/*===========================================================================
FUNCTION:    MMGSDI_CACHE_DELETE_ATTR_CACHE_BY_PATH

DESCRIPTION:
  This function clears file attributes cache for particular file by path.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  This function acquires task state critical section while accessing cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_attr_cache_by_path
(
  mmgsdi_attr_cache_type       *attr_cache_table_ptr,
  mmgsdi_path_type              file_path
)
{
  mmgsdi_attr_cache_element_type *curr_attr_cache_element_ptr = NULL;
  mmgsdi_attr_cache_element_type *next_attr_element_ptr       = NULL;
  mmgsdi_attr_cache_element_type *prev_attr_element_ptr       = NULL;
  int32                           index                       = 0;
  boolean                         match                       = FALSE;

  MMGSDIUTIL_RETURN_IF_NULL(attr_cache_table_ptr);

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  curr_attr_cache_element_ptr = attr_cache_table_ptr->item_ptr;

  /* Delete file attribute cache elements of the file */
  while(curr_attr_cache_element_ptr != NULL)
  {
    next_attr_element_ptr = curr_attr_cache_element_ptr->next_ptr;

    match = FALSE;
    if((curr_attr_cache_element_ptr->access.access_method == MMGSDI_BY_PATH_ACCESS) &&
       (FALSE != mmgsdi_file_compare_path_type(
                         curr_attr_cache_element_ptr->access.file.path_type,
                         file_path)))
    {
      match = TRUE;
    }

    if(match)
    {
      /* Relink the list of cache elements when deleting a file's attribute
         from cache */
      if(prev_attr_element_ptr != NULL)
      {
        prev_attr_element_ptr->next_ptr = next_attr_element_ptr;
      }
      else
      {
        attr_cache_table_ptr->item_ptr = next_attr_element_ptr;
      }
      mmgsdi_cache_delete_file_attr_cache_item(&curr_attr_cache_element_ptr);
      break;
    }
    else
    {
      prev_attr_element_ptr = curr_attr_cache_element_ptr;
    }
    curr_attr_cache_element_ptr = next_attr_element_ptr;
  }
  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return MMGSDI_SUCCESS;
} /* mmgsdi_cache_delete_attr_cache_by_path */


/*===========================================================================
FUNCTION:    MMGSDI_CACHE_DELETE_ATTR_SELECTIVE_ITEMS

DESCRIPTION:
  This function clears file attributes cache.
  It has optional param to provide specific file list to be deleted from attr
  cache and file list not to be deleted from attr cache. If both lists are
  present, only "to be deleted" list will be used. If these optional lists are
  not present, MMGSDI will delete all cached attr from cache.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  This function acquires task state critical section while accessing cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_attr_selective_items
(
  mmgsdi_attr_cache_type       *attr_cache_table_ptr,
  const mmgsdi_file_enum_type  *files_not_to_delete_ptr,
  uint16                        num_files_not_to_delete,
  const mmgsdi_file_enum_type  *files_to_delete_ptr,
  uint16                        num_files_to_delete
)
{
  mmgsdi_attr_cache_element_type *curr_attr_cache_element_ptr = NULL;
  mmgsdi_attr_cache_element_type *next_attr_element_ptr       = NULL;
  mmgsdi_attr_cache_element_type *prev_attr_element_ptr       = NULL;
  int32                           index                       = 0;
  boolean                         match                       = FALSE;

  MMGSDIUTIL_RETURN_IF_NULL(attr_cache_table_ptr);

  if(num_files_to_delete > 0)
  {
    MMGSDIUTIL_RETURN_IF_NULL(files_to_delete_ptr);
  }

  if(num_files_not_to_delete > 0)
  {
    MMGSDIUTIL_RETURN_IF_NULL(files_not_to_delete_ptr);
  }

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  curr_attr_cache_element_ptr = attr_cache_table_ptr->item_ptr;

  /* Delete file attribute cache elements of the files which are not part of
     not_to_delete list */
  while(curr_attr_cache_element_ptr != NULL)
  {
    next_attr_element_ptr = curr_attr_cache_element_ptr->next_ptr;

    match = TRUE;
    if(files_to_delete_ptr != NULL)
    {
      match = FALSE;
      for (index = 0; index < num_files_to_delete; index++)
      {
        if((curr_attr_cache_element_ptr->access.access_method == MMGSDI_EF_ENUM_ACCESS) &&
           (curr_attr_cache_element_ptr->access.file.file_enum == files_to_delete_ptr[index]))
        {
          /* item matches the file that should not be deleted,
             skip to the next file */
          match = TRUE;
          break;
        }
      }
    }
    else if (files_not_to_delete_ptr != NULL)
    {
      for (index = 0; index < num_files_not_to_delete; index++)
      {
        if((curr_attr_cache_element_ptr->access.access_method == MMGSDI_EF_ENUM_ACCESS) &&
           (curr_attr_cache_element_ptr->access.file.file_enum == files_not_to_delete_ptr[index]))
        {
          /* item matches the file that should not be deleted,
             skip to the next file */
          match = FALSE;
          break;
        }
      }
    }

    if(match)
    {
      /* Relink the list of cache elements when deleting a file's attribute
         from cache */
      if(prev_attr_element_ptr != NULL)
      {
        prev_attr_element_ptr->next_ptr = next_attr_element_ptr;
      }
      else
      {
        attr_cache_table_ptr->item_ptr = next_attr_element_ptr;
      }
      mmgsdi_cache_delete_file_attr_cache_item(&curr_attr_cache_element_ptr);
    }
    else
    {
      prev_attr_element_ptr = curr_attr_cache_element_ptr;
    }
    curr_attr_cache_element_ptr = next_attr_element_ptr;
  }
  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return MMGSDI_SUCCESS;
} /* mmgsdi_cache_delete_attr_selective_items */


/*===========================================================================
FUNCTION:    MMGSDI_CACHE_DELETE_TELECOM_ATTR_ITEMS

DESCRIPTION:
  This function clears telecom files attributes cache.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  This function acquires task state critical section while accessing cache
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_delete_telecom_attr_items
(
  mmgsdi_attr_cache_type       *attr_cache_table_ptr
)
{
  mmgsdi_attr_cache_element_type *curr_attr_cache_element_ptr = NULL;
  mmgsdi_attr_cache_element_type *next_attr_element_ptr       = NULL;
  mmgsdi_attr_cache_element_type *prev_attr_element_ptr       = NULL;
  boolean                         match                       = FALSE;
  mmgsdi_return_enum_type         mmgsdi_status               = MMGSDI_SUCCESS;
  mmgsdi_path_type                mmgsdi_path;

  MMGSDIUTIL_RETURN_IF_NULL(attr_cache_table_ptr);
  memset(&mmgsdi_path, 0x00, sizeof(mmgsdi_path));
  
  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  curr_attr_cache_element_ptr = attr_cache_table_ptr->item_ptr;

  /* Delete file attribute cache elements of the files which come under TELECOM DF */
  while(curr_attr_cache_element_ptr != NULL)
  {
    next_attr_element_ptr = curr_attr_cache_element_ptr->next_ptr;

    if(curr_attr_cache_element_ptr->access.access_method == MMGSDI_EF_ENUM_ACCESS)
    {       
      mmgsdi_status = mmgsdi_file_get_path_from_file(curr_attr_cache_element_ptr->access.file.file_enum,
                                                     &mmgsdi_path);
    }
    else
    {
      mmgsdi_memscpy(mmgsdi_path.path_buf, 
                     sizeof(mmgsdi_path.path_buf),
                     curr_attr_cache_element_ptr->access.file.path_type.path_buf,
                     curr_attr_cache_element_ptr->access.file.path_type.path_len * sizeof(uint16));
      mmgsdi_path.path_len = curr_attr_cache_element_ptr->access.file.path_type.path_len;
    }

    if(mmgsdi_status == MMGSDI_SUCCESS                                              &&
       (curr_attr_cache_element_ptr->access.access_method == MMGSDI_BY_PATH_ACCESS  ||
        curr_attr_cache_element_ptr->access.access_method == MMGSDI_EF_ENUM_ACCESS) &&
       mmgsdi_file_is_ef_under_global_telecom_df(&mmgsdi_path))
    {
      match = TRUE;
    }

    if(match)
    {
      /* Relink the list of cache elements when deleting a file's attribute
         from cache */
      if(prev_attr_element_ptr != NULL)
      {
        prev_attr_element_ptr->next_ptr = next_attr_element_ptr;
      }
      else
      {
        attr_cache_table_ptr->item_ptr = next_attr_element_ptr;
      }
      mmgsdi_cache_delete_file_attr_cache_item(&curr_attr_cache_element_ptr);
    }
    else
    {
      prev_attr_element_ptr = curr_attr_cache_element_ptr;
    }
    curr_attr_cache_element_ptr = next_attr_element_ptr;
  }
  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return MMGSDI_SUCCESS;
} /* mmgsdi_cache_delete_telecom_attr_items */


/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_COMMON_FILE_ATTR_CACHE

DESCRIPTION:
  This function clears the common files attributes cache. 
  Depends upon the telecom_only flag. if this flag is set then
  it clears only telecom files attr cache otherwise it clear 
  full common cache.

DEPENDENCIES:
  None

LIMITATIONS:
   None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_common_file_attr_cache(
  mmgsdi_slot_id_enum_type slot_id,
  boolean                  telecom_only)
{
  mmgsdi_return_enum_type   mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_attr_cache_type  * attr_cache_tbl_ptr = NULL;
  uint8                     slot_index         = MMGSDI_SLOT_1_INDEX;

  UIM_MSG_HIGH_0("mmgsdi_cache_delete_common_file_attr_cache");

  mmgsdi_status = mmgsdi_util_get_slot_index(slot_id, &slot_index);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  attr_cache_tbl_ptr = &mmgsdi_slot_common_cache_tbl[slot_index];
  if(telecom_only)
  {
    mmgsdi_status = mmgsdi_cache_delete_telecom_attr_items(attr_cache_tbl_ptr);
  }
  else
  {
    mmgsdi_status = mmgsdi_cache_delete_attr_selective_items(
                      attr_cache_tbl_ptr, NULL, 0, NULL, 0);
  }
  return mmgsdi_status;
} /* mmgsdi_cache_delete_common_file_attr_cache */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE_COMMON_FILE_CACHE

   DESCRIPTION:
     This function deletes all the files in the common file cache and common
     attributes cache for the slot

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_common_file_cache(
  mmgsdi_slot_id_enum_type slot_id
)
{
  mmgsdi_cache_element_type     *cache_table_ptr = NULL;
  uint32                         i               = 0;
  uint32                         cache_size      = 0;

  UIM_MSG_HIGH_1("mmgsdi_cache_delete_common_file_cache slot_id 0x%x", slot_id);

  switch(slot_id)
  {
    case MMGSDI_SLOT_1:
      cache_table_ptr = mmgsdi_common_cache_slot1;
      break;

    case MMGSDI_SLOT_2:
      cache_table_ptr = mmgsdi_common_cache_slot2;
      break;

    case MMGSDI_SLOT_3:
      cache_table_ptr = mmgsdi_common_cache_slot3;
      break;

    default:
      return MMGSDI_ERROR;
  }

  cache_size = sizeof(mmgsdi_common_cache_uim_enums) /
                 sizeof(mmgsdi_common_cache_uim_enums[0]);

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  for(i = 0; i < cache_size; i++)
  {
    mmgsdi_cache_delete_cache_element(&cache_table_ptr[i]);
  }

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  /* Common file cache has been deleted. Now proceed with deleting common
     attributes cache */
  return mmgsdi_cache_delete_common_file_attr_cache(slot_id, FALSE);
} /* mmgsdi_cache_delete_common_file_cache */


/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_PROV_FILE_ATTR_CACHE

DESCRIPTION:
  This function clears the provisioning session specific file attributes cache.
  It has optional param to provide specific file list to be deleted from attr
  cache and file list not to be deleted from attr cache. If both lists are
  present, only "to be deleted" list will be used. If these optional lists are
  not present, MMGSDI will delete all cached attr from cache.

DEPENDENCIES:
  None

LIMITATIONS:
   None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_prov_file_attr_cache(
  mmgsdi_session_type_enum_type session_type,
  const mmgsdi_file_enum_type  *files_not_to_delete_ptr,
  uint16                        num_files_not_to_delete,
  const mmgsdi_file_enum_type  *files_to_delete_ptr,
  uint16                        num_files_to_delete
)
{
  mmgsdi_attr_cache_type        * attr_cache_table_ptr = NULL;
  mmgsdi_session_id_type          session_id           = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_int_app_info_type      * app_info_ptr         = NULL;
  mmgsdi_return_enum_type         mmgsdi_status        = MMGSDI_ERROR;

  UIM_MSG_HIGH_1("mmgsdi_cache_delete_prov_file_attr_cache: session_type=0x%x",
                 session_type);

  mmgsdi_status = mmgsdi_util_get_mmgsdi_session_id_from_type(session_type,
                                                              &session_id);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* We need to determine the app_type and session_type from session_id */
  mmgsdi_status = mmgsdi_util_get_prov_session_info(session_id,
                                                    NULL,
                                                    NULL,
                                                    &app_info_ptr);
  if((mmgsdi_status != MMGSDI_SUCCESS) || !app_info_ptr)
  {
    return MMGSDI_ERROR;
  }

  attr_cache_table_ptr = &app_info_ptr->attr_cache_tbl;
  mmgsdi_status = mmgsdi_cache_delete_attr_selective_items(
                     attr_cache_table_ptr,
                     files_not_to_delete_ptr,
                     num_files_not_to_delete,
                     files_to_delete_ptr,
                     num_files_to_delete);

  return mmgsdi_status;
} /* mmgsdi_cache_delete_prov_file_attr_cache */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE_ITEM

   DESCRIPTION:
     This function delete specific file from cache. It has optional param to
     provide specific file list to be deleted from app cache and file list not
     to be deleted from app cache. If both lists are present, only
     "to be deleted" list will be used. If these optional lists are not present,
     MMGSDI will delete all cached files from app cache.

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_item(
  mmgsdi_session_id_type        session_id,
  uint16                        num_files_to_delete,
  const mmgsdi_file_enum_type  *files_to_delete_ptr,
  uint16                        num_files_not_to_delete,
  const mmgsdi_file_enum_type  *files_not_to_delete_ptr
)
{
  mmgsdi_return_enum_type         mmgsdi_status       = MMGSDI_SUCCESS;
  mmgsdi_session_type_enum_type   session_type        = MMGSDI_MAX_SESSION_TYPE_ENUM;
  int32                           i                   = 0;
  int32                           j                   = 0;
  int32                           cache_size          = 0;
  mmgsdi_cache_element_type      *cache_table_ptr     = NULL;
  mmgsdi_file_enum_type          *cache_item_ptr      = NULL;
  boolean                         match               = FALSE;

  UIM_MSG_HIGH_0("mmgsdi_cache_delete_item");

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_cache_get_prov_cache_ptrs(session_type,
                                                   &cache_table_ptr,
                                                   &cache_size,
                                                   &cache_item_ptr);
  if((mmgsdi_status != MMGSDI_SUCCESS) ||
     (cache_table_ptr == NULL) ||
     (cache_item_ptr  == NULL))
  {
    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      mmgsdi_status = MMGSDI_ERROR;
    }
    return mmgsdi_status;
  }

  if(num_files_to_delete > 0)
  {
    MMGSDIUTIL_RETURN_IF_NULL(files_to_delete_ptr);
  }

  if(num_files_not_to_delete > 0)
  {
    MMGSDIUTIL_RETURN_IF_NULL(files_not_to_delete_ptr);
  }

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  for(i = 0; i < cache_size; i++)
  {
    match = TRUE;
    if(files_to_delete_ptr != NULL)
    {
      match = FALSE;
      for (j = 0; j < num_files_to_delete; j++)
      {
        if(cache_item_ptr[i] == files_to_delete_ptr[j])
        {
          /* item matches the file that should not be deleted,
             skip to the next file */
          match = TRUE;
          break;
        }
      }
    }
    else if(files_not_to_delete_ptr != NULL)
    {
      for (j = 0; j < num_files_not_to_delete; j++)
      {
        if(cache_item_ptr[i] == files_not_to_delete_ptr[j])
        {
          /* item matches the file that should not be deleted,
             skip to the next file */
          match = FALSE;
          break;
        }
      }
    }

    if(match)
    {
      mmgsdi_cache_delete_cache_element(&cache_table_ptr[i]);
    }
  }
  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_delete_item */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE_RECORD

   DESCRIPTION:
     This function delete the record cache requested by the callere

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete_record(
  mmgsdi_session_id_type session_id,
  mmgsdi_access_type     access,
  mmgsdi_len_type        rec_num
)
{
  mmgsdi_return_enum_type         mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_session_type_enum_type   session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  int32                           file_index      = 0;
  int32                           cache_size      = 0;
  mmgsdi_cache_element_type      *cache_table_ptr = NULL;
  mmgsdi_record_cache_node_type  *previous_node_p = NULL;
  mmgsdi_record_cache_node_type  *current_node_p  = NULL;

  UIM_MSG_HIGH_0("mmgsdi_cache_delete_record");

  if (rec_num <= 0)
  {
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_cache_get_cache_ptrs_and_index(access.file.file_enum,
                                                          session_type,
                                                          &cache_table_ptr,
                                                          &cache_size,
                                                          &file_index);
    if((mmgsdi_status != MMGSDI_SUCCESS) || (cache_table_ptr == NULL) ||
       (file_index >= cache_size))
    {
      /* cannot find the file, return error */
      if(mmgsdi_status == MMGSDI_SUCCESS)
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;
    }

    if (cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_INIT)
    {
      mmgsdi_status = MMGSDI_NOT_INIT;
      break;
    }
    else if( cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_FOUND )
    {
      mmgsdi_status = MMGSDI_NOT_FOUND;
      break;
    }

    if(cache_table_ptr[file_index].file_type != MMGSDI_LINEAR_FIXED_FILE)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    current_node_p = cache_table_ptr[file_index].file_cache.cache_node.head_p;
    if(current_node_p == NULL)
    {
      /* this case is to cover the first node when nothing has been added yet */
      UIM_MSG_HIGH_2("0x%x, 0x%x not in record cache for deletion",
                    access.file.file_enum, rec_num);
      mmgsdi_status = MMGSDI_NOT_FOUND;
      break;
    }

    mmgsdi_status = MMGSDI_NOT_FOUND;

    while(current_node_p != NULL)
    {
      if(current_node_p->record_data.rec_num == rec_num)
      {
        if(current_node_p == cache_table_ptr[file_index].file_cache.cache_node.head_p)
        {
          cache_table_ptr[file_index].file_cache.cache_node.head_p = current_node_p->next_p;
          if(current_node_p == cache_table_ptr[file_index].file_cache.cache_node.tail_p)
          {
            cache_table_ptr[file_index].file_cache.cache_node.tail_p = NULL;
          }
        }
        else
        {
          if(current_node_p == cache_table_ptr[file_index].file_cache.cache_node.tail_p)
          {
            cache_table_ptr[file_index].file_cache.cache_node.tail_p = previous_node_p;
          }

          if(previous_node_p != NULL)
          {
            previous_node_p->next_p = current_node_p->next_p;
          }
        }
        MMGSDIUTIL_TMC_MEM_FREE(current_node_p->record_data.record_data.data_ptr);
        current_node_p->record_data.record_data.data_len = 0;
        MMGSDIUTIL_TMC_MEM_FREE(current_node_p);
        mmgsdi_status = MMGSDI_SUCCESS;
        break;
      }
      previous_node_p = current_node_p;
      current_node_p = current_node_p->next_p;
    }
  }while(0);

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_delete_record */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_DELETE

   DESCRIPTION:
     This function deletes all the files in app specific file cache. It has
     optional param to provide specific file list to be deleted from app cache

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_delete(
  mmgsdi_int_app_info_type    * int_app_info_ptr_ptr,
  int32                         num_files_to_delete,
  const mmgsdi_file_enum_type * files_to_delete_ptr
)
{
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_cache_element_type     *cache_table_ptr = NULL;
  int32                          cache_data_len  = 0;
  mmgsdi_file_enum_type         *cache_item_ptr  = NULL;
  int32                          i               = 0;
  int32                          j               = 0;
  boolean                        match           = FALSE;

  MMGSDIUTIL_RETURN_IF_NULL(int_app_info_ptr_ptr);

  /* If optional file count is present, check for file list ptr validity */
  if(num_files_to_delete > 0)
  {
    MMGSDIUTIL_RETURN_IF_NULL(files_to_delete_ptr);
  }

  UIM_MSG_HIGH_1("mmgsdi_cache_delete app_type 0x%x",
                 int_app_info_ptr_ptr->app_data.app_type);

  cache_data_len  = int_app_info_ptr_ptr->app_cache.cache_table_size;
  cache_table_ptr = int_app_info_ptr_ptr->app_cache.cache_table_ptr;

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  do
  {
    switch (int_app_info_ptr_ptr->app_data.app_type)
    {
#ifdef FEATURE_MMGSDI_GSM_CARD
      case MMGSDI_APP_SIM:
        cache_item_ptr = mmgsdi_sim_cache_uim_enums;
        break;
#endif /* FEATURE_MMGSDI_GSM_CARD */

#ifdef FEATURE_MMGSDI_3GPP
      case MMGSDI_APP_USIM:
        cache_item_ptr = mmgsdi_usim_cache_uim_enums;
        break;
#endif /* FEATURE_MMGSDI_3GPP */

#ifdef FEATURE_MMGSDI_3GPP2
      case MMGSDI_APP_RUIM:
        cache_item_ptr = mmgsdi_ruim_cache_uim_enums;
        break;

      case MMGSDI_APP_CSIM:
        cache_item_ptr = mmgsdi_csim_cache_uim_enums;
        break;
#endif /* FEATURE_MMGSDI_3GPP2 */

      default:
        UIM_MSG_ERR_1("Invalid app_type for cache access 0x%x",
                      int_app_info_ptr_ptr->app_data.app_type);
        mmgsdi_status = MMGSDI_ERROR;
        break;
    }

    if((mmgsdi_status != MMGSDI_SUCCESS) || (cache_table_ptr == NULL))
    {
      if(mmgsdi_status == MMGSDI_SUCCESS)
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;
    }

    for(i = 0; i < cache_data_len; i++)
    {
      match = TRUE;
      if(files_to_delete_ptr != NULL)
      {
        match = FALSE;
        for (j = 0; j < num_files_to_delete; j++)
        {
          if(cache_item_ptr[i] == files_to_delete_ptr[j])
          {
            /* item matches the file that should not be deleted,
               skip to the next file */
            match = TRUE;
            break;
          }
        }
      }

      if(match)
      {
        mmgsdi_cache_delete_cache_element(&cache_table_ptr[i]);
      }
    }

    /* If optional file list is present, don't free cache table pointer */
    if(files_to_delete_ptr == NULL)
    {
      MMGSDIUTIL_TMC_MEM_FREE(int_app_info_ptr_ptr->app_cache.cache_table_ptr);
      int_app_info_ptr_ptr->app_cache.cache_table_size = 0;
    }
  }while(0);

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_delete */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_READ_ITEM

   DESCRIPTION:
     This function reads to the cache

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type
     MMGSDI_SUCCESS          - Command dequeued, or no commands pending
     MMGSDI_INCORRECT_PARAMS - Wrong input parameters provided to function
     MMGSDI_ERROR            - Other unknown failure

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read_item (
  mmgsdi_file_enum_type          mmgsdi_file,
  mmgsdi_session_type_enum_type  session_type,
  mmgsdi_len_type                requested_data_len,
  mmgsdi_len_type                data_offset,
  mmgsdi_rec_num_type            rec_num,
  mmgsdi_len_type *              actual_data_len_ptr,
  uint8  *                       data_ptr,
  mmgsdi_cache_init_enum_type   *cache_state_ptr
)
{
  /*-----------------------------------------------------------------------------
    Initialize variables
  ------------------------------------------------------------------------------*/
  int32                      file_index      = 0;
  int32                      cache_size      = 0;
  mmgsdi_cache_element_type *cache_table_ptr = NULL;
  mmgsdi_return_enum_type    mmgsdi_status   = MMGSDI_SUCCESS;

  /* There are multiple requests to read UST cache before sending READ/WRITE 
     cmd for USIM files; which leads to burst of F3 prints. 
     Skip F3 traces when executed in DIAG task to reduce logs caused by SIM data 
     collection by QXDM.*/
  if((mmgsdi_file != MMGSDI_USIM_UST) && (!MMGSDIUTIL_IS_DIAG_TASK()))
  {
    UIM_MSG_HIGH_2("mmgsdi_cache_read_item file 0x%x, session_type 0x%x",
                   mmgsdi_file, session_type);
  }

  if(cache_state_ptr != NULL)
  {
    *cache_state_ptr = MMGSDI_CACHE_MAX_ENUM;
  }

  MMGSDIUTIL_RETURN_IF_NULL_2(actual_data_len_ptr, data_ptr);

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_cache_get_cache_ptrs_and_index(mmgsdi_file,
                                                          session_type,
                                                          &cache_table_ptr,
                                                          &cache_size,
                                                          &file_index);
    if((mmgsdi_status != MMGSDI_SUCCESS) || (cache_table_ptr == NULL) ||
       (file_index >= cache_size))
    {
      if(mmgsdi_status == MMGSDI_SUCCESS)
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;
    }

    /*-----------------------------------------------------------------------------
      Handle Cache Not Init
    ------------------------------------------------------------------------------*/
    if (cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_INIT)
    {
      UIM_MSG_ERR_1("Cache not initialized 0x%x", mmgsdi_file);
      mmgsdi_status = MMGSDI_NOT_INIT;
      break;
    } /* Cache Not Init */
    /* File not found on card */
    else if( cache_table_ptr[file_index].init == MMGSDI_CACHE_NOT_FOUND )
    {
      UIM_MSG_HIGH_1("Cache status indicates file not found on card 0x%x",
                     mmgsdi_file);

      if(cache_state_ptr != NULL)
      {
        *cache_state_ptr = MMGSDI_CACHE_NOT_FOUND;
      }
      mmgsdi_status = MMGSDI_NOT_FOUND;
      break;
    }

    /* Check if record number is zero (or less) or requested length is zero
       (or less) for record based files */
    if((rec_num <= 0 || requested_data_len == 0) &&
       cache_table_ptr[file_index].file_type != MMGSDI_TRANSPARENT_FILE)
    {
      mmgsdi_status =  MMGSDI_ERROR;
      break;
    }

    if(cache_table_ptr[file_index].file_type == MMGSDI_TRANSPARENT_FILE)
    {
      /*-----------------------------------------------------------------------------
        Handle Incorrect Paramters
      -----------------------------------------------------------------------------*/
      if (data_offset + requested_data_len > cache_table_ptr[file_index].file_cache.cache_data.data_len)
      {
        UIM_MSG_ERR_3("Requested data len 0x%x + Offset 0x%x > cache file size 0x%x",
                      requested_data_len,
                      data_offset,
                      cache_table_ptr[file_index].file_cache.cache_data.data_len);
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
        break;
      }  /* Incorrect Paramters */

      /*-----------------------------------------------------------------------------
        Proceed with Read
      ------------------------------------------------------------------------------*/
      if (requested_data_len == 0)
      {
        if (cache_table_ptr[file_index].file_cache.cache_data.data_len <= data_offset)
        {
          UIM_MSG_ERR_2("Data offset 0x%x same or greater than total len 0x%x",
                        data_offset, cache_table_ptr[file_index].file_cache.cache_data.data_len);
          mmgsdi_status = MMGSDI_ERROR;
          break;
        }
        /* When requested data_len is 0, it means that whole file should
        be returned */
        *actual_data_len_ptr = cache_table_ptr[file_index].file_cache.cache_data.data_len - data_offset;
      } /* end if - requested_data_len is 0 */
      else
      {
        *actual_data_len_ptr = requested_data_len;
      } /* end else - requested data len is not 0 */

      /* Copy from cache */
      mmgsdi_memscpy(data_ptr,
                     *actual_data_len_ptr,
                     (cache_table_ptr[file_index].file_cache.cache_data.data_ptr) + data_offset,
                     *actual_data_len_ptr);
    }
    else
    {
      mmgsdi_record_cache_node_type *current_node_p =
        cache_table_ptr[file_index].file_cache.cache_node.head_p;

      mmgsdi_status = MMGSDI_ERROR;

      if(current_node_p == NULL)
      {
        UIM_MSG_ERR_0("MMGSDI_NOT_FOUND: Record Cache not populated");
        mmgsdi_status = MMGSDI_NOT_FOUND;
        break;
      }

      do
      {
        if( current_node_p->record_data.rec_num == rec_num )
        {
          /* Get the data, if the data len passed in is greater than the
            cache data, return the cached data and update the data len to
             reflect the actual data being copied, else copy the requested data len*/
          if (requested_data_len > current_node_p->record_data.record_data.data_len)
          {
            *actual_data_len_ptr = current_node_p->record_data.record_data.data_len;
          }
          else
          {
            *actual_data_len_ptr = requested_data_len;
          }

          if(current_node_p->record_data.record_data.data_ptr != NULL)
          {
            mmgsdi_memscpy(data_ptr,
                    requested_data_len,
                    current_node_p->record_data.record_data.data_ptr,
                    *actual_data_len_ptr);
            mmgsdi_status = MMGSDI_SUCCESS;
            break;
          }
        }
        current_node_p = current_node_p->next_p;
      } while(current_node_p != NULL);
    }
  }while(0);

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_read_item */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_WRITE

   DESCRIPTION:
     This function converts the mmgsdi file enum type to uim enum type.
     And write data to different caches based on the applications available
     on the card by calling the corresding util write cache for each
     technology

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          Copy Success
     MMGSDI_ERROR:            Unable to Copy
     MMGSDI_INCORRECT_PARAMS: Null input pointer

   SIDE EFFECTS:
     If the same EF is presented in more than 1 cache, all the caches will be
     updated

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_write(
  mmgsdi_session_id_type        session_id,
  const mmgsdi_access_type     *file_access_ptr,
  mmgsdi_data_type              data,
  mmgsdi_len_type               offset,
  mmgsdi_data_from_enum_type    data_fr_card,
  mmgsdi_rec_num_type           rec_num
)
{
  mmgsdi_return_enum_type       mmgsdi_status  = MMGSDI_SUCCESS;
  mmgsdi_session_type_enum_type session_type   = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_aid_type               aid_type;

  memset(&aid_type, 0x00, sizeof(mmgsdi_aid_type));

  MMGSDIUTIL_RETURN_IF_NULL(file_access_ptr);

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               &aid_type);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if(file_access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS)
  {

    UIM_MSG_ERR_1("Skip Cache: not by EF ENUM ACCESS: 0x%x",
                  file_access_ptr->access_method);
    return MMGSDI_ERROR;
  }

  /* Do TMSI PLMN check for EF-LOCI before writing to cache */
  if(file_access_ptr->file.file_enum == MMGSDI_USIM_LOCI ||
     file_access_ptr->file.file_enum == MMGSDI_GSM_LOCI)
  {
    (void)mmgsdi_util_tmsi_plmn_check(session_id,
                                      &data);
  }

  mmgsdi_status = mmgsdi_cache_write_item(file_access_ptr->file.file_enum,
                                          session_type,
                                          data.data_len,
                                          offset,
                                          data.data_ptr,
                                          data_fr_card,
                                          rec_num);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

#ifdef FEATURE_UIM_PERSISTENT_CACHE
  /* update presistent cache when data from card is completed. To handle the case
     for offset > 0, entire data will need to be read from cache and then write to
     persistent cache.

     for MMGSDI_DATA_FROM_CLIENT doesn't trigger a persistent cache update as cache will
     be updated first, then queue a request to card, thus persistent cache will be written
     here after writing to the card; for MMGSDI_DATA_FROM_CARD_INCOMPLETE, if persistent
     cache is not found for the file, then skip the update.
  */

  (void)mmgsdi_cache_update_persistent_cache(session_id,
                                             file_access_ptr,
                                             data_fr_card,
                                             rec_num);
#endif /* FEATURE_UIM_PERSISTENT_CACHE*/

  return MMGSDI_SUCCESS;
} /* mmgsdi_cache_write */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_READ

   DESCRIPTION:
     This function converts the mmgsdi file enum type to uim enum type.
     And read data to different caches based on the applications available
     on the card by calling the corresding util read cache for each
     technology

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          Copy Success
     MMGSDI_ERROR:            Unable to Copy
     MMGSDI_INCORRECT_PARAMS: Null input pointer

   SIDE EFFECTS:
     If the same EF is presented in more than 1 cache, it will return the data
     from the first cache

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_read(
  mmgsdi_session_id_type        session_id,
  const mmgsdi_access_type     *file_access_ptr,
  mmgsdi_data_type             *data_ptr,
  mmgsdi_len_type               offset,
  mmgsdi_rec_num_type           rec_num,
  mmgsdi_cache_init_enum_type  *cache_state_ptr
)
{
  mmgsdi_return_enum_type       mmgsdi_status  = MMGSDI_SUCCESS;
  mmgsdi_session_type_enum_type session_type   = MMGSDI_MAX_SESSION_TYPE_ENUM;

  if(cache_state_ptr != NULL)
  {
    *cache_state_ptr = MMGSDI_CACHE_MAX_ENUM;
  }

  MMGSDIUTIL_RETURN_IF_NULL_2(file_access_ptr, data_ptr);

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if(file_access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS)
  {

    UIM_MSG_ERR_1("Skip Cache: not by EF ENUM ACCESS: 0x%x",
                  file_access_ptr->access_method);
    return MMGSDI_ERROR;
  }

  return mmgsdi_cache_read_item(file_access_ptr->file.file_enum,
                                session_type,
                                data_ptr->data_len,
                                offset,
                                rec_num,
                                &data_ptr->data_len,
                                data_ptr->data_ptr,
                                cache_state_ptr);
} /* mmgsdi_cache_read */


/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_GET_ATTR_CACHE

DESCRIPTION:
  This function gets the appropriate attributes cache and locates the file
  (enum or path) in the cache.

DEPENDENCIES
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type:
    MMGSDI_SUCCESS:  if cache was found for the file enum/path, return valid
                     attr_element_cache_pptr and item_index_ptr and
                     attr_cache_tbl_pptr are valid
    MMGSDI_ERROR:    no matching cache at all
    MMGSDI_NOT_INIT: cache was found but the file enum/path is not in the cache
                     yet, only attr_cache_tbl_pptr is valid

SIDE EFFECTS:
  None

CRITICAL SECTIONS:
   The calling function must enter mmgsdi_cache_crit_sect crit
   sect before invoking this util function. Protection for attributes cache.
==========================================================================*/
static mmgsdi_return_enum_type  mmgsdi_cache_get_attr_cache(
  mmgsdi_session_id_type             session_id,
  const mmgsdi_access_type          *access_ptr,
  mmgsdi_attr_cache_element_type   **attr_element_cache_pptr,
  mmgsdi_attr_cache_type           **attr_cache_tbl_pptr)
{
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_SUCCESS;
  mmgsdi_int_app_info_type          *app_info_ptr        = NULL;
  uint8                              slot_index          = MMGSDI_SLOT_1_INDEX;
  mmgsdi_slot_id_enum_type           slot_id             = MMGSDI_MAX_SLOT_ID_ENUM;

  MMGSDIUTIL_RETURN_IF_NULL_3(attr_element_cache_pptr, attr_cache_tbl_pptr, access_ptr);

  if((access_ptr->access_method != MMGSDI_BY_PATH_ACCESS &&
      access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS) ||
     (access_ptr->access_method == MMGSDI_BY_PATH_ACCESS &&
      access_ptr->file.path_type.path_len == 0))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Check if file is common file, if so go to common cache,
     i.e., MF, TELECOM, ORANGESIM, CINGULARSIM, DCS1800, HZI */
  if (mmgsdi_file_is_ef_under_mf(access_ptr) ||
      mmgsdi_file_is_ef_under_df_in_mf(access_ptr))
  {

    mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                     &slot_id,
                                                     NULL, NULL, NULL, NULL, NULL);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }

    /* check if we should be comparing with the MF attr cache instead */
    mmgsdi_status = mmgsdi_util_get_slot_index(slot_id, &slot_index);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }

    *attr_cache_tbl_pptr = &mmgsdi_slot_common_cache_tbl[slot_index];
  }
  else
  {
    /* We need to determine the app_type and session_type from session_id */
    mmgsdi_status = mmgsdi_util_get_prov_session_info(session_id,
                                                      NULL,
                                                      NULL,
                                                      &app_info_ptr);
    if((mmgsdi_status != MMGSDI_SUCCESS) || !app_info_ptr)
    {
      return MMGSDI_ERROR;
    }
    *attr_cache_tbl_pptr = &app_info_ptr->attr_cache_tbl;
  }

  if(*attr_cache_tbl_pptr == NULL)
  {
    UIM_MSG_ERR_0("Null *attr_cache_tbl_pptr");
    return MMGSDI_ERROR;
  }

  *attr_element_cache_pptr = (*attr_cache_tbl_pptr)->item_ptr;
  while(*attr_element_cache_pptr != NULL)
  {
    if(FALSE != mmgsdi_file_compare_access_type(
                  &(*attr_element_cache_pptr)->access,
                  access_ptr))
    {
      return MMGSDI_SUCCESS;
    }
    *attr_element_cache_pptr = (*attr_element_cache_pptr)->next_ptr;
  }

  return MMGSDI_NOT_INIT;
} /* mmgsdi_cache_get_attr_cache */


/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_CREATE_FILE_ATTR_CACHE_ELEMENT

DESCRIPTION:
  This function allocates memory for a file attribute cache element and
  assigns the item with the passed in parameter

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type:
    MMGSDI_SUCCESS:  If the element is created successfully
    MMGSDI_ERROR:    If the memory allocation failed

SIDE EFFECTS:

==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_create_file_attr_cache_element(
  const mmgsdi_access_type        *access_ptr,
  mmgsdi_attr_cache_type          *cache_attr_tbl_ptr,
  mmgsdi_attr_cache_element_type **cache_attr_element_pptr)
{
  mmgsdi_attr_cache_element_type *new_attr_element_ptr = NULL;
  mmgsdi_attr_cache_element_type *tmp_attr_element_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL_2(cache_attr_tbl_ptr, access_ptr);

  if((access_ptr->access_method != MMGSDI_BY_PATH_ACCESS &&
      access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS) ||
     (access_ptr->access_method == MMGSDI_BY_PATH_ACCESS &&
      access_ptr->file.path_type.path_len == 0))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(new_attr_element_ptr,
    sizeof(mmgsdi_attr_cache_element_type));
  if (new_attr_element_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  if (cache_attr_element_pptr)
  {
    *cache_attr_element_pptr = new_attr_element_ptr;
  }

  if (mmgsdi_util_copy_access_type(&new_attr_element_ptr->access,
                                   access_ptr) != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(new_attr_element_ptr);
    return MMGSDI_ERROR;
  }

  if(cache_attr_tbl_ptr->item_ptr == NULL)
  {
    /* first element in the table */
    cache_attr_tbl_ptr->item_ptr = new_attr_element_ptr;
  }
  else
  {
    tmp_attr_element_ptr = cache_attr_tbl_ptr->item_ptr;
    while(tmp_attr_element_ptr->next_ptr != NULL)
    {
      tmp_attr_element_ptr = tmp_attr_element_ptr->next_ptr;
    }
    tmp_attr_element_ptr->next_ptr = new_attr_element_ptr;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_cache_create_file_attr_cache_element */


/*===========================================================================
FUNCTION MMGSDI_CACHE_ALLOC_AND_POPULATE_FILE_ATTR_CACHE

INPUT
  mmgsdi_file_security_type  security_status_input
  mmgsdi_file_security_type *security_status_output_ptr

OUTPUT
  void

DESCRIPTION

RETURN VALUE
  void

CRITICAL SECTIONS:
   The calling function must enter mmgsdi_cache_crit_sect crit
   sect when security_status_output_ptr is from attributes cache
   before invoking this util function. Protection for attributes cache.
===========================================================================*/
void mmgsdi_cache_alloc_and_populate_file_attr_cache(
  mmgsdi_file_security_type  security_status_input,
  mmgsdi_file_security_type *security_status_output_ptr)
{
  uint32                  pin_data_size = 0;

  if(security_status_output_ptr == NULL)
  {
    UIM_MSG_ERR_0("Null security_status_output_ptr");
    return;
  }

  security_status_output_ptr->protection_method  = MMGSDI_ALWAYS_ALLOWED;
  security_status_output_ptr->num_protection_pin = 0;
  security_status_output_ptr->protection_pin_ptr = NULL;

  if(security_status_input.num_protection_pin > 0)
  {
    if(security_status_input.protection_pin_ptr == NULL)
    {
      /* If num_protection_pin indicates there is at least
         one pin then protection_pin_ptr cannot be NULL */
      UIM_MSG_ERR_0("Null security_status_input.protection_pin_ptr with non-zero num_protection_pin");
      return;
    }
    pin_data_size = int32touint32(security_status_input.num_protection_pin)*
                    sizeof(mmgsdi_pin_enum_type);
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
      security_status_output_ptr->protection_pin_ptr,
      pin_data_size);
    if(security_status_output_ptr->protection_pin_ptr == NULL)
    {
      return;
    }

    mmgsdi_memscpy((void*)security_status_output_ptr->protection_pin_ptr,
                   pin_data_size,
                   (void*)security_status_input.protection_pin_ptr,
                   pin_data_size);
  }
  security_status_output_ptr->num_protection_pin =
    security_status_input.num_protection_pin;
  security_status_output_ptr->protection_method =
    security_status_input.protection_method;
} /* mmgsdi_cache_alloc_and_populate_file_attr_cache */


/*===========================================================================
FUNCTION MMGSDI_CACHE_POPULATE_ATTR_CACHE_IF_NEEDED

DESCRIPTION
  Checks for file attributes cache, if found cache ptr is returned. If not
  found and if it is a file in the cache list. Get File attributes is perfomed
  on that EF. Get File attributes is performed only if it is a internal call

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  If the calling function expects the attributes data, then *cache_attr_pptr
  will hold the attributes and the calling function should take care of freeing
  this ptr along with its member ptrs.

CRITICAL SECTIONS:
  The function uses mmgsdi_cache_crit_sect to protect accesses to
  the File attributes cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_populate_attr_cache_if_needed(
  mmgsdi_session_id_type                     session_id,
  mmgsdi_cache_sec_attr_operation_enum_type  skip_uicc_arr,
  boolean                                    is_select_mandatory,
  const mmgsdi_access_type                 * access_ptr,
  mmgsdi_decoded_attr_cache_type          ** cache_attr_pptr,
  mmgsdi_sw_status_enum_type               * sw_present_ptr,
  mmgsdi_file_security_access_type         * security_access_out_ptr
)
{
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_SUCCESS;
  mmgsdi_get_file_attr_req_type     *select_ptr          = NULL;
  mmgsdi_get_file_attr_cnf_type     *cnf_ptr             = NULL;
  boolean                            is_gsdi_task        = TRUE;
  mmgsdi_protocol_enum_type          protocol            = MMGSDI_NO_PROTOCOL;
  mmgsdi_slot_id_enum_type           slot                = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_task_enum_type              mmgsdi_task         = MMGSDI_TASK_MAX;
  mmgsdi_client_id_type              client_id           = MMGSDI_CLIENT_ID_ZERO;

  MMGSDIUTIL_RETURN_IF_NULL_2(access_ptr, cache_attr_pptr);

  if (sw_present_ptr)
  {
    *sw_present_ptr = MMGSDI_STATUS_WORD_INVALID;
  }

  if(access_ptr->access_method != MMGSDI_BY_PATH_ACCESS &&
     access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS)
  {
    UIM_MSG_ERR_0("Cannot access cache");
    /* Return SUCCESS since the check for file attr did not fail at
      this point*/
    return MMGSDI_SUCCESS;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                   &slot,
                                                   NULL, NULL, NULL, NULL, NULL);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  protocol = mmgsdi_util_determine_protocol(slot);

  /* Commands can be sent to UIM in sync mode only in MMGSDI slot tasks
      context */
  mmgsdi_task = mmgsdi_util_get_current_task();
  if(!MMGSDIUTIL_IS_MMGSDI_SLOT_TASK(mmgsdi_task))
  {
    is_gsdi_task = FALSE;
  }

  if(is_select_mandatory && !is_gsdi_task)
  {
    /* The explicit request to perform a SELECT can only come from gsdi
       task context */
    return MMGSDI_ERROR;
  }

  if(!is_select_mandatory)
  {
      /* Check if the attributes are already cached */
      mmgsdi_status = mmgsdi_cache_attr_read(session_id,
                                             access_ptr,
                                             cache_attr_pptr,
                                             skip_uicc_arr,
                                             security_access_out_ptr,
                                             NULL,
                                             NULL);
  
    if(mmgsdi_status != MMGSDI_NOT_INIT)
    {
      /* File may have been found in the attributes cache or there may be some
         error other than MMGSDI_NOT_INIT */
      return mmgsdi_status;
    }
    
    /* File attributes are not cached yet. Lets try to fetch the attributes
       1. First from the static tables (for EF_ENUM type access only)
       2. If not found, from an explicit SELECT to the card */
    mmgsdi_status = mmgsdi_cache_populate_attr_cache_from_static_file_table(cache_attr_pptr,
                                                                            access_ptr,
                                                                            slot,
                                                                            security_access_out_ptr);
    
    if(MMGSDI_SUCCESS == mmgsdi_status)
    {
      return MMGSDI_SUCCESS;
    }
  }

  /* We are here only when file attributes are not cached yet and also could not be populated
     from static tables or when is_select_mandatory is set to TRUE */
  if(!is_gsdi_task)
  {
    return MMGSDI_NOT_INIT;
  }

  mmgsdi_status = mmgsdi_util_get_client_id(session_id,
                                            &client_id);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  
  /* Will have to send the SELECT to the card to get the attributes */
  UIM_MSG_HIGH_0("Unknown Security Attr, Get File Attribute will be performed");
  
  /* Build the file attribute command to populate the attribute info */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(select_ptr,
                                     sizeof(mmgsdi_get_file_attr_req_type));

  if (select_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  select_ptr->request_header.client_id         = client_id;
  select_ptr->request_header.session_id        = session_id;
  select_ptr->request_header.request_type      = MMGSDI_GET_FILE_ATTR_REQ;
  select_ptr->request_header.orig_request_type = MMGSDI_GET_FILE_ATTR_REQ;
  select_ptr->request_header.slot_id           = slot;
  select_ptr->request_header.client_data       = 0;
  select_ptr->request_header.response_cb       = NULL;
  select_ptr->activate_aid                     = TRUE;
  /* Skip reading EF-ARR for internal calls */
  select_ptr->skip_uicc_arr                    = skip_uicc_arr == MMGSDI_DECODE_ACTUAL_ATTRIBUTES ? FALSE : TRUE;
  
  mmgsdi_memscpy(&select_ptr->access,
          sizeof(mmgsdi_access_type),
          access_ptr,
          sizeof(mmgsdi_access_type));
  
  switch(protocol)
  {
    case MMGSDI_ICC:
      mmgsdi_status = mmgsdi_uim_icc_select (select_ptr,
                                             TRUE,
                                             &cnf_ptr);
      break;
    case MMGSDI_UICC:
      mmgsdi_status = mmgsdi_uim_uicc_select (select_ptr,
                                              TRUE,
                                              &cnf_ptr,
                                              TRUE);
      break;
    default:
      UIM_MSG_HIGH_1("Invalid protocol 0x%x for file attr cache but allow continue",
                     protocol);
      MMGSDIUTIL_TMC_MEM_FREE(select_ptr);
      return MMGSDI_SUCCESS;
  }
  
  if (cnf_ptr != NULL)
  {
    /* If mmgsdi_status is SUCCESS, it means the response_header is
       correctly populated with UIMDRV response. In that case, we can
       rely on SW present in cnf */
    if (sw_present_ptr != NULL &&
        mmgsdi_status == MMGSDI_SUCCESS)
    {
      if (cnf_ptr->response_header.status_word.present)
      {
        *sw_present_ptr = MMGSDI_STATUS_WORD_PRESENT;
      }
      else
      {
        *sw_present_ptr = MMGSDI_STATUS_WORD_ABSENT;
      }
    }
    mmgsdi_status = cnf_ptr->response_header.mmgsdi_status;
  }
  
  /* The file attributes would already have been cached while handling the
     CNF for this SELECT request... so no need to cache them here again */
  MMGSDIUTIL_TMC_MEM_FREE(select_ptr);
  
  mmgsdi_util_free_response_ptr((mmgsdi_cnf_type*)cnf_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(cnf_ptr);

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    /* Read out the attributes into the out-param */
    mmgsdi_status = mmgsdi_cache_attr_read(session_id,
                                           access_ptr,
                                           cache_attr_pptr,
                                           skip_uicc_arr,
                                           security_access_out_ptr,
                                           NULL,
                                           NULL);
  }

  return mmgsdi_status;
} /* mmgsdi_cache_populate_attr_cache_if_needed */


/*===========================================================================
FUNCTION MMGSDI_CACHE_DECODE_RAW_FCP_PARAMETERS

DESCRIPTION
  This function decode the raw attributes of file and returns in decoded
  structure.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_decode_raw_fcp_parameters(
  mmgsdi_session_id_type             session_id,
  mmgsdi_get_file_attr_data_type     raw_file_attr_data,
  mmgsdi_file_attributes_type      * file_attrib_ptr,
  const mmgsdi_access_type         * access_ptr
)
{
  mmgsdi_protocol_enum_type    protocol        = MMGSDI_NO_PROTOCOL;
  mmgsdi_slot_id_enum_type     slot_id         = MMGSDI_SLOT_NONE;
  mmgsdi_return_enum_type      mmgsdi_status   = MMGSDI_SUCCESS;

  MMGSDIUTIL_RETURN_IF_NULL(file_attrib_ptr);

  mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                   &slot_id,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   NULL);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Check if UICC or ICC */
  protocol = mmgsdi_util_determine_protocol(slot_id);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  if(protocol == MMGSDI_UICC)
  {
    /* Extract the important file attributes from the file header */
    mmgsdi_status = mmgsdi_uicc_rsp_decode_EF_header_data(session_id,
                                                          access_ptr,
                                                          FALSE,
                                                          raw_file_attr_data.data_ptr,
                                                          raw_file_attr_data.data_len,
                                                          file_attrib_ptr,
                                                          TRUE);
  }
  else
  {
    /* Extract the important file attributes from the file header */
    mmgsdi_status = mmgsdi_icc_rsp_extract_file_attr(slot_id,
                                                     raw_file_attr_data.data_ptr,
                                                     raw_file_attr_data.data_len,
                                                     file_attrib_ptr,
                                                     TRUE);
  }

  return mmgsdi_status;
}/* mmgsdi_cache_decode_raw_fcp_parameters */


/*===========================================================================
FUNCTION MMGSDI_CACHE_BUILD_SECURITY_ATTRIBUTES

DESCRIPTION
  This function is used to get security attributes from cache.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  File cache critical section should be aquired in the caller
  before calling this routine.

===========================================================================*/
static mmgsdi_return_enum_type mmgsdi_cache_build_security_attributes(
  mmgsdi_session_id_type                     session_id,
  mmgsdi_attr_cache_element_type            *cache_attr_ptr,
  mmgsdi_file_structure_enum_type            mmgsdi_file_type,
  const mmgsdi_access_type                  *access_ptr,
  mmgsdi_cache_sec_attr_operation_enum_type  skip_uicc_arr,
  mmgsdi_file_security_attributes_type      *security_access_out_ptr
)
{
  mmgsdi_return_enum_type            mmgsdi_status     = MMGSDI_ERROR;
  mmgsdi_protocol_enum_type          protocol          = MMGSDI_NO_PROTOCOL;
  mmgsdi_slot_id_enum_type           slot_id           = MMGSDI_SLOT_NONE;

  MMGSDIUTIL_RETURN_IF_NULL_3(access_ptr, cache_attr_ptr, security_access_out_ptr);

  /* Check if file cache critical section is already acquired */
  MMGSDIUTIL_CHECK_FILE_CACHE_CRIT_SECT_ACCESS;

  mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                   &slot_id,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   NULL);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  /* Check if UICC or ICC */
  protocol = mmgsdi_util_determine_protocol(slot_id);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  /* Copy attributes from cache in below cases:
     1) When cached security attributes is default attributes and requested is also default attributes
     2) When cached security attributes is actual attributes and requested is also actual attributes 
     3) If protocol is ICC */
  if(((cache_attr_ptr->is_arr_skipped && (skip_uicc_arr == MMGSDI_DECODE_DEFAULT_ATTRIBUTES)) ||
      (!cache_attr_ptr->is_arr_skipped && (skip_uicc_arr == MMGSDI_DECODE_ACTUAL_ATTRIBUTES))) || 
     protocol == MMGSDI_ICC)
  {
    mmgsdi_file_security_access_type *mmgsdi_file_security_access_ptr = NULL;

    if(mmgsdi_file_get_security_attributes(mmgsdi_file_type,
                                           security_access_out_ptr,
                                           &mmgsdi_file_security_access_ptr) != MMGSDI_SUCCESS)
    {
      return MMGSDI_ERROR;
    }

    /* Copy attributes from cache to output structure pointer */
    mmgsdi_cache_alloc_and_populate_file_attr_cache(cache_attr_ptr->security_access.read,
                                                    &mmgsdi_file_security_access_ptr->read);
    mmgsdi_cache_alloc_and_populate_file_attr_cache(cache_attr_ptr->security_access.write,
                                                    &mmgsdi_file_security_access_ptr->write);
    mmgsdi_cache_alloc_and_populate_file_attr_cache(cache_attr_ptr->security_access.increase,
                                                    &mmgsdi_file_security_access_ptr->increase);
    mmgsdi_cache_alloc_and_populate_file_attr_cache(cache_attr_ptr->security_access.invalidate_deactivate,
                                                    &mmgsdi_file_security_access_ptr->invalidate_deactivate);
    mmgsdi_cache_alloc_and_populate_file_attr_cache(cache_attr_ptr->security_access.rehabilitate_activate,
                                                    &mmgsdi_file_security_access_ptr->rehabilitate_activate);
  }
  else if(cache_attr_ptr->is_arr_skipped &&
          skip_uicc_arr == MMGSDI_DECODE_ACTUAL_ATTRIBUTES)
  {
    uint16 add_file_id   = cache_attr_ptr->arr_file_id;
    uint8  arr_record_no = cache_attr_ptr->arr_record_no;

    /* Do not leave file cache critical section to read ARR in MAIN task 
       as there is a possibility of slot task acquiring the critical section
       and delete the cache attributes resulting in crash. Return NOT_INIT
       so that caller can proceed with sending access command to card or
       queue the request to MMGSDI(if this is called in caller context). */
    if(MMGSDIUTIL_IS_MAIN_TASK())
    {
      return MMGSDI_NOT_INIT;
    }

    /* NOTE:
       As file cache critical section is alredy aquired in caller. Leaving of critical section is important here,
       because as part of reading ARR record, it is possible that we may goto card to read the ARR record in
       sync, which will result in holding of critical section for long. In the mean time if other slot task aquire
       task sync critical sections and also aquire file cache critical section, then this will cause the current 
       task to be in deadlock, as while reporting response in sync we again try to get task sync critical section */
    MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

    /* Try to decode security attributes from arr file */
    mmgsdi_status = mmgsdi_uicc_rsp_read_arr(session_id,
                                             access_ptr,
                                             add_file_id,
                                             arr_record_no,
                                             mmgsdi_file_type,
                                             security_access_out_ptr);

    MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      return MMGSDI_NOT_INIT;
    }
  }
  else if(skip_uicc_arr == MMGSDI_DECODE_DEFAULT_ATTRIBUTES)
  {
    (void)mmgsdi_file_set_default_sec_attr_per_spec(security_access_out_ptr,
                                                    mmgsdi_file_type,
                                                    access_ptr->file.file_enum);
  }

  return MMGSDI_SUCCESS;
}/* mmgsdi_cache_build_security_attributes */


/*===========================================================================
FUNCTION MMGSDI_CACHE_ATTR_READ

DESCRIPTION
  Checks for file attributes cache, if found cache ptr is returned in the
  function argument. In the SUCCESS case, *cache_attr_pptr will hold the
  attributes and the calling function should take care of freeing this ptr
  along with its member ptrs.
  Based on the value of sec_attr_oper, either default attributes OR actual
  security attributes will be decoded. If sec_attr_oper indicates that no
  security attributes is needed then no operation is done.

  Raw data or security parameters or both will be populated only if the
  respective pointers are valid.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  None

CRITICAL SECTIONS:
  The function uses mmgsdi_cache_crit_sect to protect accesses to
  the File attributes cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_attr_read(
  mmgsdi_session_id_type                       session_id,
  const mmgsdi_access_type                    *access_ptr,
  mmgsdi_decoded_attr_cache_type             **cache_attr_pptr,
  mmgsdi_cache_sec_attr_operation_enum_type    sec_attr_oper,
  mmgsdi_file_security_access_type            *security_access_out_ptr,
  mmgsdi_len_type                             *raw_data_len_ptr,
  uint8                                      **raw_data_pptr
)
{
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_SUCCESS;
  mmgsdi_attr_cache_type            *cache_attr_tbl_ptr  = NULL;
  mmgsdi_attr_cache_element_type    *new_cache_attr_ptr  = NULL;
  mmgsdi_file_attributes_type        decoded_attr;

  MMGSDIUTIL_RETURN_IF_NULL_2(access_ptr, cache_attr_pptr);

  if(access_ptr->access_method != MMGSDI_BY_PATH_ACCESS &&
     access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS)
  {
    return MMGSDI_ERROR;
  }

  memset(&decoded_attr, 0x00, sizeof(mmgsdi_file_attributes_type));

  /* Enter critical Section */
  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  do
  {
    mmgsdi_status = mmgsdi_cache_get_attr_cache(session_id,
                                                access_ptr,
                                                &new_cache_attr_ptr,
                                                &cache_attr_tbl_ptr);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    mmgsdi_status = mmgsdi_cache_decode_raw_fcp_parameters (session_id,
                                                            new_cache_attr_ptr->raw_file_attr_data,
                                                            &decoded_attr,
                                                            access_ptr);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* got the cache already */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE((*cache_attr_pptr),
                                       sizeof(mmgsdi_decoded_attr_cache_type));
    if(*cache_attr_pptr == NULL)
    {
      mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    (void) mmgsdi_util_copy_access_type(&((*cache_attr_pptr)->access),
                                        &new_cache_attr_ptr->access);

    /* Fill security access attributes from cache or card */
    if(security_access_out_ptr != NULL &&
       sec_attr_oper != MMGSDI_SKIP_SECURITY_ATTRIBUTES)
    {
      mmgsdi_file_security_access_type   *security_access_ptr = NULL;

      mmgsdi_status = mmgsdi_cache_build_security_attributes(session_id,
                                                             new_cache_attr_ptr,
                                                             decoded_attr.file_type,
                                                             access_ptr,
                                                             sec_attr_oper,
                                                             &decoded_attr.file_info);
      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        MMGSDIUTIL_TMC_MEM_FREE(*cache_attr_pptr);
        break;
      }

      mmgsdi_status = mmgsdi_file_get_security_attributes(decoded_attr.file_type,
                                                          &decoded_attr.file_info,
                                                          &security_access_ptr);
      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        MMGSDIUTIL_TMC_MEM_FREE(*cache_attr_pptr);
        break;
      }

      /* Copy security attributes to output structure pointer */
      mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->read,
                                                      &(security_access_out_ptr->read));
      mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->write,
                                                      &(security_access_out_ptr->write));
      mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->increase,
                                                      &(security_access_out_ptr->increase));
      mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->invalidate_deactivate,
                                                      &(security_access_out_ptr->invalidate_deactivate));
      mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->rehabilitate_activate,
                                                      &(security_access_out_ptr->rehabilitate_activate));
      mmgsdi_file_free_security_info(security_access_ptr);
    }

    /* Copy the retrieved attributes to the file attributes cache, if not already
       present in file attributes cache */
    (*cache_attr_pptr)->file_type    = decoded_attr.file_type;
    (*cache_attr_pptr)->sfi          = decoded_attr.sfi;
    (*cache_attr_pptr)->file_id[0]   = decoded_attr.file_id[0];
    (*cache_attr_pptr)->file_id[1]   = decoded_attr.file_id[1];

    switch ((*cache_attr_pptr)->file_type)
    {
      case MMGSDI_LINEAR_FIXED_FILE:
        (*cache_attr_pptr)->num_of_rec  = decoded_attr.file_info.linear_fixed_file.num_of_rec;

        /* Resetting num_of_rec to 254 if any non spec complaint cards are configured to have more than 254.
           This is to align with spec 102.221 8.2.2.2 max_records should not be more than 254 */
        if((*cache_attr_pptr)->num_of_rec > MMGSDI_MAX_RECS_IN_LINEAR_FIXED_EF)
        {
          (*cache_attr_pptr)->num_of_rec = MMGSDI_MAX_RECS_IN_LINEAR_FIXED_EF;
        }
        (*cache_attr_pptr)->rec_len     = decoded_attr.file_info.linear_fixed_file.rec_len;
        (*cache_attr_pptr)->file_size   = ((*cache_attr_pptr)->num_of_rec) * ((*cache_attr_pptr)->rec_len);
        (*cache_attr_pptr)->file_status = decoded_attr.file_info.linear_fixed_file.file_status;
        break;
      case MMGSDI_TRANSPARENT_FILE:
        (*cache_attr_pptr)->file_size   = decoded_attr.file_size;
        (*cache_attr_pptr)->file_status = decoded_attr.file_info.transparent_file.file_status;
        break;
      case MMGSDI_CYCLIC_FILE:
        (*cache_attr_pptr)->num_of_rec  = decoded_attr.file_info.cyclic_file.num_of_rec;
        (*cache_attr_pptr)->rec_len     = decoded_attr.file_info.cyclic_file.rec_len;
        (*cache_attr_pptr)->file_size   = ((*cache_attr_pptr)->num_of_rec) * ((*cache_attr_pptr)->rec_len);
        (*cache_attr_pptr)->file_status = decoded_attr.file_info.cyclic_file.file_status;

        if(decoded_attr.file_info.cyclic_file.file_security.increase.protection_method != MMGSDI_NEVER_ALLOWED)
        {
          (*cache_attr_pptr)->cyclic_increase_allowed = TRUE;
        }
		break;

      default:
        mmgsdi_status = MMGSDI_ERROR;
        break;
    }

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      MMGSDIUTIL_TMC_MEM_FREE(*cache_attr_pptr);
      mmgsdi_file_free_security_info(security_access_out_ptr);
      break;
    }

    if(raw_data_pptr != NULL && raw_data_len_ptr != NULL)
    {
      *raw_data_len_ptr = new_cache_attr_ptr->raw_file_attr_data.data_len;

      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(*raw_data_pptr, *raw_data_len_ptr);
      if(*raw_data_pptr == NULL)
      {
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
        MMGSDIUTIL_TMC_MEM_FREE(*cache_attr_pptr);
        mmgsdi_file_free_security_info(security_access_out_ptr);
        break;
      }
      mmgsdi_memscpy(*raw_data_pptr,
                     *raw_data_len_ptr,
                     new_cache_attr_ptr->raw_file_attr_data.data_ptr,
                     new_cache_attr_ptr->raw_file_attr_data.data_len);
    }
  } while (0);

  /* Leave critical Section */
  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_attr_read */


/*===========================================================================
FUNCTION MMGSDI_CACHE_ATTR_WRITE

DESCRIPTION
  Checks for file attributes cache for a file. If the attributes are not
  cached already, it writes the attribtues to the attribtues cache

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS
    MMGSDI_ERROR

SIDE EFFECTS
  None

CRITICAL SECTIONS:
  The function uses mmgsdi_cache_crit_sect to protect accesses to
  the File attributes cache
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_attr_write(
  mmgsdi_session_id_type                   session_id,
  const mmgsdi_access_type               * access_ptr,
  const mmgsdi_file_security_access_type * security_access_ptr,
  boolean                                  is_arr_skipped,
  uint16                                   arr_file_id,
  uint8                                    arr_record_no,
  mmgsdi_get_file_attr_data_type           raw_file_attr_data
)
{
  mmgsdi_return_enum_type            mmgsdi_status = MMGSDI_SUCCESS;
  mmgsdi_attr_cache_type            *cache_attr_tbl_ptr  = NULL;
  mmgsdi_attr_cache_element_type    *new_cache_attr_ptr  = NULL;

  MMGSDIUTIL_RETURN_IF_NULL_3(access_ptr,
                              security_access_ptr,
                              raw_file_attr_data.data_ptr);

  if(access_ptr->access_method != MMGSDI_BY_PATH_ACCESS &&
     access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS)
  {
    return MMGSDI_ERROR;
  }

  /* Enter critical Section */
  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  do
  {
    /* Retrieve the appropriate cache ptr */
    mmgsdi_status = mmgsdi_cache_get_attr_cache(session_id,
                                                access_ptr,
                                                &new_cache_attr_ptr,
                                                &cache_attr_tbl_ptr);

    if(mmgsdi_status == MMGSDI_SUCCESS && new_cache_attr_ptr != NULL)
    {
      /* Looks like attributes are already cached for this file.
         lets reset the existing attributes */
      mmgsdi_cache_reset_file_attr_cache_item(&new_cache_attr_ptr);
    }
    else if(mmgsdi_status == MMGSDI_NOT_INIT)
    {
      /* This is the first time we are caching attributes for this file */
      mmgsdi_status = mmgsdi_cache_create_file_attr_cache_element(access_ptr,
                                                                  cache_attr_tbl_ptr,
                                                                  &new_cache_attr_ptr);
      if((mmgsdi_status != MMGSDI_SUCCESS) || (new_cache_attr_ptr == NULL))
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }
    }
    else
    {
      break;
    }

    new_cache_attr_ptr->is_arr_skipped = is_arr_skipped;
    new_cache_attr_ptr->arr_file_id    = arr_file_id;
    new_cache_attr_ptr->arr_record_no  = arr_record_no;

    /* Start populating the cache with new attributes */
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->read,
                                                   &(new_cache_attr_ptr->security_access.read));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->write,
                                                   &(new_cache_attr_ptr->security_access.write));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->increase,
                                                   &(new_cache_attr_ptr->security_access.increase));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->invalidate_deactivate,
                                                   &(new_cache_attr_ptr->security_access.invalidate_deactivate));
    mmgsdi_cache_alloc_and_populate_file_attr_cache(security_access_ptr->rehabilitate_activate,
                                                   &(new_cache_attr_ptr->security_access.rehabilitate_activate));

    new_cache_attr_ptr->raw_file_attr_data.data_len = raw_file_attr_data.data_len;
    if(new_cache_attr_ptr->raw_file_attr_data.data_len > 0)
    {
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(new_cache_attr_ptr->raw_file_attr_data.data_ptr,
                                         new_cache_attr_ptr->raw_file_attr_data.data_len);
      if (new_cache_attr_ptr->raw_file_attr_data.data_ptr == NULL)
      {
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
        break;
      }

      mmgsdi_memscpy(new_cache_attr_ptr->raw_file_attr_data.data_ptr,
                     new_cache_attr_ptr->raw_file_attr_data.data_len,
                     raw_file_attr_data.data_ptr,
                     raw_file_attr_data.data_len);
    }
  } while (0);

  /* Leave critical Section */
  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_cache_attr_write */


/*===========================================================================
FUNCTION MMGSDI_CACHE_INIT_USIM_ECC

DESCRIPTION
  This function determines the number of ECC records and caches all the records
  upto a maximum of MMGSDI_MAX_ECC_RECORDS (255) records.
  An ECC event is also sent out.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_init_usim_ecc(
  mmgsdi_session_id_type       session_id
)
{
#ifdef FEATURE_MMGSDI_3GPP
  mmgsdi_return_enum_type             mmgsdi_status        = MMGSDI_SUCCESS;
  mmgsdi_session_type_enum_type       session_type         = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_cache_element_type          *cache_table_ptr      = NULL;
  mmgsdi_rec_num_type                 num_of_recs          = 0;
  byte                                rec_num              = 0;
  mmgsdi_evt_session_notify_type      notify_type;
  uint16                              prov_cache_size      = 0;
  mmgsdi_int_app_info_type          * int_app_info_ptr     = NULL;
  mmgsdi_decoded_attr_cache_type    * attr_cache_ptr       = NULL;
  mmgsdi_access_type                  access_data;
  mmgsdi_slot_id_enum_type            slot_id              = MMGSDI_MAX_SLOT_ID_ENUM;

  memset(&notify_type, 0x00, sizeof(mmgsdi_evt_session_notify_type));
  memset(&access_data, 0x00, sizeof(mmgsdi_access_type));

  mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                   &slot_id,
                                                   NULL, NULL, NULL, NULL, NULL);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Based on the session type, determine the cache ptr for caching ECC */
  do
  {
    mmgsdi_status = mmgsdi_util_get_session_type(session_id,&session_type,NULL);
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    if(session_type != MMGSDI_GW_PROV_PRI_SESSION &&
       session_type != MMGSDI_GW_PROV_SEC_SESSION &&
       session_type != MMGSDI_GW_PROV_TER_SESSION)
    {
      UIM_MSG_ERR_1("Invalid session_type for cache access 0x%x", session_type);
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    mmgsdi_status = mmgsdi_util_get_prov_session_info(session_id, NULL, NULL,
                                                      &int_app_info_ptr);

    if(mmgsdi_status != MMGSDI_SUCCESS || int_app_info_ptr == NULL)
    {
      break;
    }

    cache_table_ptr    = int_app_info_ptr->app_cache.cache_table_ptr;
    prov_cache_size    = int_app_info_ptr->app_cache.cache_table_size;

    if( cache_table_ptr == NULL || prov_cache_size == 0 )
    {
      UIM_MSG_ERR_1("Prov app cache is not allocated for this session 0x%x",session_type);
      break;
    }

    access_data.access_method = MMGSDI_EF_ENUM_ACCESS;
    access_data.file.file_enum = MMGSDI_USIM_ECC;

    mmgsdi_status = mmgsdi_cache_populate_attr_cache_if_needed(
      session_id,
      MMGSDI_SKIP_SECURITY_ATTRIBUTES,
      FALSE,
      &access_data,
      &attr_cache_ptr,
      NULL,
      NULL);

    if(mmgsdi_status != MMGSDI_SUCCESS ||
       attr_cache_ptr == NULL ||
       attr_cache_ptr->file_type != MMGSDI_LINEAR_FIXED_FILE ||
       attr_cache_ptr->num_of_rec == 0)
    {
      UIM_MSG_ERR_0("Select EF ECC failed or no Record Found");

      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    num_of_recs = attr_cache_ptr->num_of_rec;

    if(num_of_recs > MMGSDI_MAX_ECC_RECORDS)
    {
      /* Set to Max Number of Records Supported */
      UIM_MSG_HIGH_1("ECC # or Record 0x%x exceed MMGSDI cache", num_of_recs);
      num_of_recs = MMGSDI_MAX_ECC_RECORDS;
    }

    for(rec_num = 1; rec_num <= num_of_recs; rec_num++)
    {
      mmgsdi_status = mmgsdi_card_init_cache_record(session_id,
                                                    MMGSDI_APP_USIM,
                                                    slot_id,
                                                    MMGSDI_USIM_ECC,
                                                    rec_num,
                                                    NULL);
      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        UIM_MSG_ERR_1("Read EF ECC failed for record 0x%x", rec_num);

        break;
      }
    }
  } while(0);

  UIM_MSG_HIGH_2("Read MMGSDI_USIM_ECC total rec 0x%x, num of rec read 0x%x", num_of_recs, rec_num);
  MMGSDIUTIL_TMC_MEM_FREE(attr_cache_ptr);

  /* Send out the ECC event(even if the ECC read failed) */
  notify_type.notify_type = MMGSDI_EVT_NOTIFY_ALL_SESSIONS;
  notify_type.slot_id     = slot_id;
  notify_type.session_id  = session_id;
  mmgsdi_evt_build_and_send_ecc(notify_type);

  return MMGSDI_SUCCESS;
#else
  (void)session_id;
   return MMGSDI_ERROR;
#endif /* FEATURE_MMGSDI_3GPP */
}/*mmgsdi_cache_init_usim_ecc*/


/*===========================================================================
FUNCTION MMGSDI_CACHE_GET_ECC_FILE_DATA

DESCRIPTION
 This function copies whole of the ECC data from cache if available.
 The function allocates memory for the ecc data. It is caller's
 responsibility to free the data ptr.
 For RUIM/CSIM/GSM ECC files, the record length returned in ecc_rec_len_ptr
 is zero.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_get_ecc_file_data(
  mmgsdi_session_id_type         session_id,
  mmgsdi_file_enum_type          file_enum,
  mmgsdi_data_type               *ecc_data_ptr,
  mmgsdi_len_type                *ecc_rec_len_ptr
)
{
  mmgsdi_return_enum_type       mmgsdi_status   = MMGSDI_ERROR;
  mmgsdi_access_type            access;

  if(ecc_data_ptr == NULL ||
     ecc_rec_len_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  *ecc_rec_len_ptr = 0;

  memset(&access, 0x00, sizeof(mmgsdi_access_type));

  access.file.file_enum = file_enum;
  access.access_method  = MMGSDI_EF_ENUM_ACCESS;

  /* Get length of the first ECC record that was populated during
     mmgsdi_cache_init_usim_ecc */
  if(file_enum == MMGSDI_USIM_ECC)
  {
    do
    {
      mmgsdi_decoded_attr_cache_type *cache_attr_ptr  = NULL;
      mmgsdi_rec_num_type             num_of_rec      = 0;
      mmgsdi_len_type                 len_of_rec     = 0;
      uint8                           index          = 0;
      mmgsdi_data_type                data_type;

      memset(&data_type, 0x00, sizeof(mmgsdi_data_type));

      /* Check if the attributes are already cached */
      mmgsdi_status = mmgsdi_cache_attr_read(session_id,
                                             &access,
                                             &cache_attr_ptr,
                                             MMGSDI_SKIP_SECURITY_ATTRIBUTES,
                                             NULL,
                                             NULL,
                                             NULL);

      if((mmgsdi_status == MMGSDI_SUCCESS) && cache_attr_ptr != NULL &&
          cache_attr_ptr->num_of_rec <= 0xFF)
      {
        num_of_rec = (uint8)cache_attr_ptr->num_of_rec;
        len_of_rec = cache_attr_ptr->rec_len;
        MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(ecc_data_ptr->data_ptr,
                                           len_of_rec * num_of_rec);
        if (ecc_data_ptr->data_ptr != NULL)
        {
          *ecc_rec_len_ptr = len_of_rec;
          ecc_data_ptr->data_len = len_of_rec * num_of_rec;
          memset(ecc_data_ptr->data_ptr, 0xFF, len_of_rec * num_of_rec);

          data_type.data_len = len_of_rec;

          for(index = 1; index <= num_of_rec; index++)
          {
            data_type.data_ptr = &ecc_data_ptr->data_ptr[len_of_rec*(index - 1)];

            (void)mmgsdi_cache_read(session_id,
                                    &access,
                                    &data_type,
                                    0,
                                    index,
                                    NULL);
          }
        }
        else
        {
          mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
        }
      }
      MMGSDIUTIL_TMC_MEM_FREE(cache_attr_ptr);
    }while(0);
  }
  else
  {
    /* Get length of the whole ecc file data */
    mmgsdi_status = mmgsdi_cache_read_len(session_id,
                                          &access,
                                          0,
                                          &ecc_data_ptr->data_len,
                                          NULL);

    /* Allocate the data ptr and read data from ecc cache */
    if(mmgsdi_status == MMGSDI_SUCCESS &&
       ecc_data_ptr->data_len > 0)
    {
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(ecc_data_ptr->data_ptr,
                                         ecc_data_ptr->data_len);
      if (ecc_data_ptr->data_ptr != NULL)
      {
        mmgsdi_status = mmgsdi_cache_read(session_id,
                                          &access,
                                          ecc_data_ptr,
                                          0,
                                          0,
                                          NULL);
      }
      else
      {
        mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
    }

  }
  return mmgsdi_status;
} /* mmgsdi_cache_get_ecc_file_data */


/*===========================================================================
FUNCTION MMGSDI_CACHE_INIT

DESCRIPTION
  Function called to initialize critical section for MMGSDI cache.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called once during task initialization.
===========================================================================*/
void mmgsdi_cache_init(
  void
)
{
  rex_init_crit_sect(&mmgsdi_cache_crit_sect);
} /* mmgsdi_cache_init */


/*===========================================================================
FUNCTION MMGSDI_CACHE_DELETE_PKCS15_ATTR_CACHE

DESCRIPTION
 This function deletes all file attributes cached under the PKCS15 DF 7F50.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_cache_delete_pkcs15_attr_cache (
  mmgsdi_slot_id_enum_type  slot_id
)
{
  mmgsdi_attr_cache_type         * attr_cache_tbl_ptr     = NULL;
  uint8                            slot_index             = MMGSDI_SLOT_1_INDEX;
  mmgsdi_attr_cache_element_type * attr_cache_element_ptr = NULL;
  mmgsdi_attr_cache_element_type * next_attr_element_ptr  = NULL;
  mmgsdi_attr_cache_element_type * prev_attr_element_ptr  = NULL;

  UIM_MSG_LOW_0("mmgsdi_cache_delete_pkcs15_cache");

  if(mmgsdi_util_get_slot_index(slot_id, &slot_index) != MMGSDI_SUCCESS)
  {
    return;
  }

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  attr_cache_tbl_ptr = &mmgsdi_slot_common_cache_tbl[slot_index];
  if (attr_cache_tbl_ptr != NULL)
  {
    attr_cache_element_ptr = attr_cache_tbl_ptr->item_ptr;

    /* Delete all the File attribute cache elements */
    while(attr_cache_element_ptr != NULL)
    {
      next_attr_element_ptr = attr_cache_element_ptr->next_ptr;

      if(attr_cache_element_ptr->access.access_method == MMGSDI_BY_PATH_ACCESS &&
         attr_cache_element_ptr->access.file.path_type.path_len > 1 &&
         attr_cache_element_ptr->access.file.path_type.path_buf[1] == 0x7F50)
      {
        /* relink the list of cache elements when deleting a PKCS15 file from cache */
        if(prev_attr_element_ptr != NULL)
        {
          prev_attr_element_ptr->next_ptr = next_attr_element_ptr;
        }
        else
        {
          attr_cache_tbl_ptr->item_ptr = next_attr_element_ptr;
        }

        mmgsdi_cache_delete_file_attr_cache_item(&attr_cache_element_ptr);
      }
      else
      {
        prev_attr_element_ptr = attr_cache_element_ptr;
      }

      attr_cache_element_ptr = next_attr_element_ptr;
    }
  }

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;
} /* mmgsdi_cache_delete_pkcs15_attr_cache */


/*================================================================
FUNCTION  MMGSDI_CACHE_CREATE_APP_CACHE_TABLE

DESCRIPTION:
  Populate app cache table pointer along with CPHS cache pointer

INPUT PARAMETERS:
  None

DEPENDENCIES:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None
===============================================================*/
mmgsdi_return_enum_type mmgsdi_cache_create_app_cache_table(
  mmgsdi_int_app_info_type *app_info_ptr)
{
  uint16                  cache_size    = 0;
  mmgsdi_return_enum_type mmgsdi_status = MMGSDI_SUCCESS;

  MMGSDIUTIL_RETURN_IF_NULL(app_info_ptr);

  if(app_info_ptr->cphs_info_ptr == NULL &&
     (app_info_ptr->app_data.app_type == MMGSDI_APP_SIM ||
      app_info_ptr->app_data.app_type == MMGSDI_APP_USIM))
  {
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(app_info_ptr->cphs_info_ptr,
                                       sizeof(mmgsdi_cphs_int_info_type));
  }

  if(app_info_ptr->app_cache.cache_table_ptr != NULL)
  {
    /* Cache is already allocated */
    return mmgsdi_status;
  }

  switch(app_info_ptr->app_data.app_type)
  {
#ifdef FEATURE_MMGSDI_GSM_CARD
    case MMGSDI_APP_SIM:
      cache_size = sizeof(mmgsdi_sim_cache_uim_enums) /
                   sizeof(mmgsdi_sim_cache_uim_enums[0]);
      break;
#endif /* FEATURE_MMGSDI_GSM_CARD */

#ifdef FEATURE_MMGSDI_3GPP
    case MMGSDI_APP_USIM:
      cache_size = sizeof(mmgsdi_usim_cache_uim_enums) /
                   sizeof(mmgsdi_usim_cache_uim_enums[0]);
      break;
#endif /* FEATURE_MMGSDI_3GPP */

#ifdef FEATURE_MMGSDI_3GPP2
    case MMGSDI_APP_RUIM:
      cache_size = sizeof(mmgsdi_ruim_cache_uim_enums) /
                   sizeof(mmgsdi_ruim_cache_uim_enums[0]);
       break;

    case MMGSDI_APP_CSIM:
      cache_size = sizeof(mmgsdi_csim_cache_uim_enums) /
                   sizeof(mmgsdi_csim_cache_uim_enums[0]);
      break;
#endif /* FEATURE_MMGSDI_3GPP2 */

     default:
       mmgsdi_status = MMGSDI_ERROR;
       break;
  }

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    /*App is selected; allocate memory for file cache table*/
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
      app_info_ptr->app_cache.cache_table_ptr,
      (sizeof(mmgsdi_cache_element_type) * (uint32)cache_size));

    if(app_info_ptr->app_cache.cache_table_ptr != NULL)
    {
      /* Update the allocated cache size in the session cache size global */
      app_info_ptr->app_cache.cache_table_size = cache_size;
    }
    else
    {
      mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
  }

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT

  return mmgsdi_status;
}/* mmgsdi_cache_create_app_cache_table */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_READ_IN_SYNCH_STATUS

   DESCRIPTION:
     This function return the cache item in_synch status

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     boolean - indication if cache data is in_synch with card

   SIDE EFFECTS:
     None

   CRITICAL SECTIONS:
     The function uses mmgsdi_cache_crit_sect to protect accesses to
     the File cache
==========================================================================*/
static boolean mmgsdi_cache_read_in_synch_status(
  mmgsdi_file_enum_type         mmgsdi_file,
  mmgsdi_session_id_type        session_id
)
{
  mmgsdi_session_type_enum_type session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  int32                         file_index      = 0;
  int32                         cache_size      = 0;
  mmgsdi_cache_element_type    *cache_table_ptr = NULL;
  mmgsdi_return_enum_type       mmgsdi_status   = MMGSDI_SUCCESS;
  boolean                       in_synch        = FALSE;

  UIM_MSG_HIGH_2("mmgsdi_cache_read_in_synch_status file 0x%x, session_type 0x%x",
                 mmgsdi_file, session_type);

  mmgsdi_status = mmgsdi_util_get_session_type(session_id,
                                               &session_type,
                                               NULL);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return FALSE;
  }

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  mmgsdi_status = mmgsdi_cache_get_cache_ptrs_and_index(mmgsdi_file,
                                                        session_type,
                                                        &cache_table_ptr,
                                                        &cache_size,
                                                        &file_index);

  if((mmgsdi_status != MMGSDI_SUCCESS) || (cache_table_ptr == NULL) ||
     (file_index >= cache_size) ||
     (cache_table_ptr[file_index].init != MMGSDI_CACHE_INIT))
  {
    in_synch = FALSE;
  }
  else
  {
    in_synch = cache_table_ptr[file_index].in_synch;
  }

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return in_synch;
} /* mmgsdi_cache_read_in_synch_status */


/*===========================================================================
  FUNCTION MMGSDI_CACHE_IS_WRITE_DATA_SAME_AS_CACHE

DESCRIPTION
  This utility function check if input write request data is the same
  as data stored in mmgsdi cache is in synch with card data

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS

===========================================================================*/
boolean mmgsdi_cache_is_write_data_same_as_cache(
  mmgsdi_session_id_type         session_id,
  const mmgsdi_access_type      *file_access_ptr,
  mmgsdi_data_type               write_data,
  mmgsdi_len_type                offset,
  mmgsdi_rec_num_type            rec_num
)
{
  mmgsdi_data_type         cache_data;
  mmgsdi_return_enum_type  mmgsdi_status = MMGSDI_SUCCESS;
  boolean                  result        = FALSE;

  /* Some cards require explicit write of the NAMLOCK file for the correct
     functionality of the OTASP procedure. For this reason, the logic to
     skip the write operation cannot be applied */
  if (file_access_ptr == NULL ||
      write_data.data_ptr == NULL || write_data.data_len == 0 ||
      file_access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS ||
      file_access_ptr->file.file_enum == MMGSDI_CSIM_NAM_LOCK ||
      file_access_ptr->file.file_enum == MMGSDI_CDMA_NAM_LOCK)
  {
    return FALSE;
  }

  if(!mmgsdi_cache_read_in_synch_status(file_access_ptr->file.file_enum, session_id))
  {
    return FALSE;
  }

  /* First write to LOCI on the card is required because card can use this as
     a trigger to send dispaly text proactive command */
  if(file_access_ptr->file.file_enum == MMGSDI_USIM_LOCI ||
     file_access_ptr->file.file_enum == MMGSDI_GSM_LOCI)
  {
    mmgsdi_int_app_info_type *app_info_ptr = NULL;

    mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                     NULL,
                                                     NULL,
                                                     &app_info_ptr,
                                                     NULL,
                                                     NULL,
                                                     NULL);
    if(mmgsdi_status == MMGSDI_SUCCESS &&
       app_info_ptr != NULL &&
       app_info_ptr->first_loci_write_after_activation == FALSE)
    {
      app_info_ptr->first_loci_write_after_activation = TRUE;
      return FALSE;
    }
  }

  memset(&cache_data, 0x00, sizeof(mmgsdi_data_type));

  mmgsdi_status = mmgsdi_cache_read_len(
                    session_id,
                    file_access_ptr,
                    rec_num,
                    &cache_data.data_len,
                    NULL);

  if (mmgsdi_status != MMGSDI_SUCCESS ||
      cache_data.data_len == 0 ||
      cache_data.data_len != (write_data.data_len + offset))
  {
    return FALSE;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(cache_data.data_ptr,
                                     cache_data.data_len);
  if(cache_data.data_ptr == NULL)
  {
    return FALSE;
  }

  mmgsdi_status = mmgsdi_cache_read(session_id,
                                    file_access_ptr,
                                    &cache_data,
                                    0,
                                    rec_num,
                                    NULL);

  if (mmgsdi_status == MMGSDI_SUCCESS &&
      (memcmp(write_data.data_ptr, &(cache_data.data_ptr[offset]), write_data.data_len) == 0))
  {
    UIM_MSG_MED_0("write req data is same as mmgsdi cache");
    result = TRUE;
  }

  MMGSDIUTIL_TMC_MEM_FREE(cache_data.data_ptr);
  return result;
} /* mmgsdi_cache_is_write_data_same_as_cache */


/* ==========================================================================
   FUNCTION:      MMGSDI_CACHE_CHECK_FILE_IS_IN_CACHE_LIST_OR_NOT

   DESCRIPTION:
     This function checks the passed file enum is present in MMGSDI cache
     list or not.

   DEPENDENCIES:
     NONE

   LIMITATIONS:
     None

   RETURN VALUE:
     boolean
     TRUE    File is present in cache list
     FALSE   File is not present in cache list

   SIDE EFFECTS:
     None
==========================================================================*/
boolean mmgsdi_cache_check_file_is_in_cache_list_or_not(
  mmgsdi_file_enum_type          file_enum,
  mmgsdi_session_type_enum_type  session_type
)
{
  int32                      file_index      = 0;
  int32                      cache_size      = 0;
  mmgsdi_cache_element_type *cache_table_ptr = NULL;
  mmgsdi_return_enum_type    mmgsdi_status   = MMGSDI_SUCCESS;
  boolean                    ret_val         = TRUE;

  MMGSDICACHE_ENTER_FILE_CACHE_CRIT_SECT;

  mmgsdi_status = mmgsdi_cache_get_cache_ptrs_and_index(file_enum,
                                                        session_type,
                                                        &cache_table_ptr,
                                                        &cache_size,
                                                        &file_index);
  if((mmgsdi_status != MMGSDI_SUCCESS) || (cache_table_ptr == NULL) ||
     (file_index >= cache_size))
  {
    ret_val = FALSE;
  }

  MMGSDICACHE_LEAVE_FILE_CACHE_CRIT_SECT;

  return ret_val;
}/* mmgsdi_cache_check_file_is_in_cache_list_or_not */


#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */


/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_CHECK_AND_WRITE_DEFAULT_ATTR

DESCRIPTION:
  This function checks if the EF attributes are cached if not
  then write default attributes to cache

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_check_and_write_default_attr(
  mmgsdi_session_id_type                   session_id,
  const mmgsdi_access_type                 access,
  mmgsdi_file_enum_type                    file_enum,
  mmgsdi_file_structure_enum_type          file_type,
  mmgsdi_get_file_attr_data_type           raw_attr_data
)
{
  mmgsdi_file_security_access_type   *security_access_ptr = NULL;
  mmgsdi_file_attributes_type         file_attr;
  mmgsdi_decoded_attr_cache_type     *attr_cache_ptr      = NULL;
  mmgsdi_return_enum_type             mmgsdi_status       = MMGSDI_SUCCESS;

  memset(&file_attr, 0x00, sizeof(file_attr));

  file_attr.file_type = file_type;

  /* Read the attribute cache,
     if attributes are not cached, then write the default attributes */
  mmgsdi_status = mmgsdi_cache_attr_read(session_id,
                                         &access,
                                         &attr_cache_ptr,
                                         MMGSDI_SKIP_SECURITY_ATTRIBUTES,
                                         NULL,
                                         NULL,
                                         NULL);

  MMGSDIUTIL_TMC_MEM_FREE(attr_cache_ptr);

  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_file_set_default_sec_attr_per_spec(&file_attr.file_info,
                                                            file_attr.file_type,
                                                            file_enum);

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  if(file_type == MMGSDI_TRANSPARENT_FILE)
  {
    security_access_ptr = &file_attr.file_info.transparent_file.file_security;
  }
  else if(file_type == MMGSDI_LINEAR_FIXED_FILE)
  {
    security_access_ptr = &file_attr.file_info.linear_fixed_file.file_security;
  }
  else if(file_type == MMGSDI_CYCLIC_FILE)
  {
    security_access_ptr = &file_attr.file_info.cyclic_file.file_security;
  }

  mmgsdi_status = mmgsdi_cache_attr_write(session_id,
                                          &access,
                                          security_access_ptr,
                                          FALSE,
                                          0,
                                          0,
                                          raw_attr_data);


  mmgsdi_file_free_security_info(security_access_ptr);
  return mmgsdi_status;
}/* mmgsdi_cache_check_and_write_default_attr */


/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_UPDATE_FILE_ATTRIBUTES

DESCRIPTION:
  This function updates the passed attrbute's field with the value passed in
  argument.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

SIDE EFFECTS:
  None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_cache_update_file_attributes(
  mmgsdi_session_id_type                    session_id,
  mmgsdi_cache_update_attr_field_enum_type  update_field,
  uint16                                    updated_length,
  mmgsdi_get_file_attr_data_type           *raw_attr_ptr
)
{
  mmgsdi_protocol_enum_type  protocol      = MMGSDI_NO_PROTOCOL;
  mmgsdi_slot_id_enum_type   slot_id       = MMGSDI_SLOT_NONE;
  mmgsdi_return_enum_type    mmgsdi_status = MMGSDI_SUCCESS;
  boolean                    update_done   = FALSE;

  MMGSDIUTIL_RETURN_IF_NULL(raw_attr_ptr);

  if(update_field <= MMGSDI_CACHE_NO_UPDATE ||
     update_field >= MMGSDI_CACHE_UPDATE_MAX_FIELD)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(session_id,
                                                   &slot_id,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   NULL);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Check if UICC or ICC */
  protocol = mmgsdi_util_determine_protocol(slot_id);

  if(protocol == MMGSDI_NO_PROTOCOL)
  {
    return MMGSDI_ERROR;
  }

  if(protocol == MMGSDI_UICC)
  {
    /* Skip first 2 bytes for FCP template and FCP length */
    uint32 raw_attr_index = 2;

    while(raw_attr_ptr->data_len > raw_attr_index + 1)
    {
      if(update_field == MMGSDI_CACHE_UPDATE_FILE_SIZE &&
         raw_attr_ptr->data_ptr[raw_attr_index] == GSDI_FILE_SIZE_TAG &&
         raw_attr_ptr->data_len > raw_attr_index + raw_attr_ptr->data_ptr[raw_attr_index + 1] + 1)
      {
        uint8 i = 0;

        for(i = 1; i <= raw_attr_ptr->data_ptr[raw_attr_index + 1]; i++)
        {
          raw_attr_ptr->data_ptr[raw_attr_index + 1 + i] = (uint8)(updated_length >> ((raw_attr_ptr->data_ptr[raw_attr_index + 1] - i) * 8));
        }
        update_done = TRUE;
        /* break here as the passed field is updated */
        break;
      }

      /* Move to next TLV index */
      raw_attr_index = raw_attr_index + raw_attr_ptr->data_ptr[raw_attr_index + 1] + 2;
    }

    if(!update_done)
    {
      return MMGSDI_ERROR;
    }
  }
  else
  {
    if(update_field == MMGSDI_CACHE_UPDATE_FILE_SIZE &&
       raw_attr_ptr->data_len > 4 )
    {
      raw_attr_ptr->data_ptr[2] = (uint8)(0xFF & (updated_length >> 8));
      raw_attr_ptr->data_ptr[3] = (uint8)(0xFF & updated_length);
    }
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_cache_update_file_attributes */


/* ==========================================================================
FUNCTION:      MMGSDI_CACHE_GET_COMMON_ATTR_CACHE_PTR

DESCRIPTION:
  This function returns common attr cache table ptr.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_attr_cache_type*

SIDE EFFECTS:
  None
==========================================================================*/
mmgsdi_attr_cache_type* mmgsdi_cache_common_attr_cache_ptr(
  mmgsdi_slot_id_enum_type slot_id
)
{
  mmgsdi_return_enum_type    mmgsdi_status = MMGSDI_SUCCESS;
  uint8                      slot_index    = MMGSDI_SLOT_1_INDEX;

  mmgsdi_status = mmgsdi_util_get_slot_index(slot_id, &slot_index);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return NULL;
  }

  return (&mmgsdi_slot_common_cache_tbl[slot_index]);
} /* mmgsdi_cache_common_attr_cache_ptr */
