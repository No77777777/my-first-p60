/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               N V    R - U I M    S U B S Y S T E M

GENERAL DESCRIPTION
  Provides I/O services for NVM items on the R-UIM.

EXTERNALIZED FUNCTIONS
  nvruim_read
    Read an NVM item on the R-UIM

  nvruim_write
    Write an NVM item on the R-UIM

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2016, 2018-2019 by QUALCOMM Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/nvruim/src/nvruim.c#3 $ $DateTime: 2019/12/23 23:05:39 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/24/19   cj      Initialize SID NID buffer to 0 before sending response
09/17/18   sg      Assign 20 sec to dormant timer if value in card is less than 20
12/19/16   ar      Re-setting WMS service globals on sub inactive
11/02/16   dd      Linux compilation error fix
11/02/16   me      NVRUIM F3 reduction  
10/10/16   bcho    Queue NVRUIM read requests to high priority command queue
10/03/16   me      Removed additional F3 message 
09/28/16   ar      Adding support for as_id in NVRUIM APIs for DS
10/03/16   dd      Removed lint comments
09/21/16   bcho    Handling of parallel requests in NVRUIM
09/01/16   vdc     Reduce frequent logging in NVRUIM
08/20/16   ar      Send NVRUIM_FAIL when nvruim access fails
07/20/16   ar      Remove GPP2_SUPPORT_ON_SEC_SUB featurization from code
04/26/16   lm      Fix compiler error
04/13/16   ar      CDMA/HDR support on 2nd SUB
03/22/16   ssr     Removal of NVRUIM Task for IOE variant
03/09/16   bcho    Process requests after NVRUIM task has started
12/09/15   sp      Fix for KW warning
10/05/15   sp      Reduce signed/unsigned conversions in UIM
10/02/15   kk      Redirect requests to NV during QCN Restore operation
09/21/15   ar      Add check for 1X_PRI session for NVRUIM control flag init
08/28/15   hh      Removed JCDMA related code
08/20/15   at      Increase NVRUIM timers to 20
07/01/15   yt      Remove always-on flag FEATURE_SD20
04/29/15   av      Make NVRUIM sanity timer logic more robust
03/03/15   lm      Remove double semicolon at end of the line
02/20/15   vv      Remove perso related code
02/03/15   ar      Checking for task stop before setting the signal for task
02/02/15   ar      Mechanism to provide lock to lower priority task like diag
11/14/14   hh      Fix compilation warnings
10/29/14   am      Handle MMGSDI_NV_PREF_MODE_NOT_FROM_RUIM_I as device specific
10/10/14   av      Clean up NVRUIM crit sect macro definition
09/11/14   hh      Call rex_undef_timer() when timer is no longer needed
09/04/14   ar      Fix KW error
09/04/14   ar      Remove NVRUIM cache
07/15/14   yt      Support for NV refresh
07/08/14   tl      Remove ALWAYS OFF feature NVRUIM_DEBUG_FORCE_ADDR_NUM
06/16/14   av      Remove support for 1x secondary session
06/10/14   tl      Introduce EF 3GPDUPPExt
04/28/14   av      Fix to return appropriate error code if access denied
02/27/14   av      NVRUIM headers cleanup
02/13/14   at      Fix for KW Critical errors
12/13/13   av      Replace UIM enums with MMGSDI enums
01/14/14   nmb     Remove ACCESS_F and related unused functionality
01/10/14   df      Use v2.0 diag macros
11/01/13   at      Support for guaranteed response for each NVRUIM request
10/17/13   at      SIM busy support in NVRUIM
08/23/13   av      Optimize read of NV HOME_SID_NID_I
05/28/13   kb      Added support for memory leak detection
05/16/13   na      Secure memory copy
05/13/13   vs      Decoupling NV-EFS from NV-RUIM
04/16/13   av      Compiler warning due to dog HB header inclusions
04/08/13   av      Remove unnecessary efs reads for features status list item
04/08/13   av      Change dog logic to use heart-beat mechanism
02/10/13   at      Fixed compiler warnings
01/31/13   at      Switch NVRUIM to use mmgsdi_session_open_ext
01/29/13   abg     Updated Offset value in nvruim_read_count
01/23/13   tl      Fix offset for NXTREG in EFaloc
01/02/13   tl      Fixed record length parameter of call count write
12/12/12   av      Fixed compiler critical warnings
08/25/12   spo     Added nvruim_crit_sect_init for off-target suppot
07/03/12   av      Provided a nvruim_init() routine for crit sect init
03/08/12   av      Replaced feature flag with NVITEM
02/23/12   av      Removed ALWAYS OFF feature FEATURE_NVRUIM_ADM_ACCESS and
                   FEATURE_NVRUIM_DEBUG_FORCE_DIR_ENUM
02/23/12   bcho    Macros added for critical section acquisition and release
                   and its logging
02/06/12   av      Removed Always ON 1x feature flags
02/06/12   av      Reverting the esn_usage flag logic and skipping of 855 check
                   for NVRUIM offtarget test; moving them back to the test
02/02/12   av      Added NVRUIM Sanity test for MoB
12/23/11   shr     Removed featurization for ALWAYS ON features
12/07/11   ssr     extern mmgsdi_init_rtre_config function
12/02/11   av      Skip check for 855 when NV read/write is for offtarget
11/29/11   ssr     Added more message and Added code to send RTRE request again
                   after session deactivation
11/17/11   ssr     Fixed incorrect setting of RUIMID cache bit
11/11/11   ssr     Ignore the fallback request for non CT card
10/18/11   av      Removed features that are always OFF
09/24/11   ssr     Fixed lint warnings
08/03/11   ssr     Update data length for Simple IP PAP SS data writing
07/08/11   ssr     Fixed Simple IP PAP SS data writing
06/27/11   yt      Removed featurization from UIM public headers
05/16/11   ssr     Added NVRUIM thread safe support
04/26/11   yt      Replaced references to TMC heap with Modem heap
03/28/11   nmb     Fixed compilation warning
03/15/11   js      Fix for NVRUIM-MMGSDI race condition
02/25/11   js      Fixed compilation error
02/22/11   js      Renaming fusion features
02/08/11   ssr     Moved HRPD check from nvruim to mmgsdi layer
02/01/11   ssr     Fixed compilation warning
01/31/11   ssr     Decoupled FEATURE_UIM_JCDMA_RUIM_SUPPORT feature
01/12/11   yt      Protected access to NVRUIM globals using critical sections
01/10/11   ssr     Update the default initialization based on the RTRE mode
01/05/11   ssr     Fixed SW1 and SW2 status byte for error scenario
01/03/11   kk      Bringing back header decoupling changes
12/29/10   kk      Fixed nvruim dir present flag on receiving sub ready evt
12/29/10   kk      Reverting header decoupling changes temporarily
12/10/10   ssr     Temp fixes to handle the card error scenario in middle of
                   processing an nv request
11/30/10   yt      Including private UIMDRV header
11/22/10   adp     NVRUIM READ for NV_HOME_SID_NID_I always reads first record
11/02/10   vs      Update to process NVRUIM cmds via MMGSDI high priority q
10/29/10   ssr     Fixed compilation error
10/27/10   nmb     Deny PIN protected access before subscription ready
09/01/10   vs      Check programmed indicator bit for all IMSI items
08/20/10   yt      Modified previous Lint fix
08/18/10   yt      Fixed Lint errors
08/12/10   ssr     Fixed nvruim 1x primary session for slot2
08/10/10   js      Updated nvruim get data sync to support 3gpd control data
07/08/10   js      Update ESN Usage Indicator for Fusion Slave
06/28/10   yt      Fixed compiler warnings
06/18/10   ssr     Add non 3gpd cdma card check
06/15/10   js      Add support to fetch NVRUIM data from fusion master
06/14/10   ssr     Fixed compilation error when FEATURE_NV_RUIM is undefined
06/09/10   js      Handle Subscription Ready Event
05/18/10   tkl     Lint fixes
05/13/10   vs      Fix for NV and GSDI deadlock
05/12/10   ssr     Fixed nvruim clear cache
05/04/10   yt      Lint fixes
04/23/10   ssr     Fixed UIM 3GPD NV support
04/23/10   yt      Fixed KW errors
04/14/10   yk      Fixed UIM dog fatal due to a typo in access api
04/06/10   shr     Clean up REFRESH RESET and REFRESH RESET_AUTO
04/07/10   yt      Fixed compilation warnings
03/25/10   ssr     Added nvruim_data_3gpd_ext_support
03/19/10   nmb     Removed eight extra 0's from nvruim_an_cave_nai
03/02/10   ssr     Fixed compilation error
02/19/10   ssr     Fixed NV_SMS_MO_ON_TRAFFIC_CHANNEL_I reading
02/19/10   vs      Fixed double definition compiler error
02/16/10   nb      Fixed Compilation Error, removed inclusion of nvi.h
02/15/10   nb      Moved Dual Slot Changes
01/14/10   ssr     Set ESN usage to USE_ESN for RUIM mode
12/07/09   js      Lint Fixes
11/11/09   yb      Fixed bad RUIM card issue with HRPD Service n5 in CDMA
                   Service Table improperly set
09/08/09   ssr     Fixed write MSISDN nv item
09/02/09   ssr     Removed private header.
08/28/09   ssr     Fixed compliation errors
08/27/09   ssr     Fixed compilation errors (for MOB).
08/24/09   shr     Fixed compliation errors (for CDMA only)
07/02/09   ssr     Operator MCC Check for 3GPD
07/01/09   rn      Removed unused headers
06/02/09   ssr     Fixed Linker Error
05/21/09   ps      Fixed Diag crash during QPST read in RUIM
05/18/09   ssr     Fixed compilation warning.
04/22/09   ssr     Initialize the nv buffer to 0x00 for sippapss.
04/22/09   ssr     Fixed  PRL validity code for FEATURE_2ND_PRL
04/06/09   ssr     Fixed IPv4 and IPv6 address parsing.
03/25/09   ssr     Allow task contexts other than NV to enter nvruim_read
                   and nvruim_write
03/21/09   ssr     Fixed sippapss parsing
03/26/09   rm      Fix for a compilation error
03/23/09   rm      Fix for dog reset due to slow sim response
03/16/09   ssr     Converting 8 Bit ASCII number to hex for PDE and MPC server
                   address
03/02/09   sun     Remove header file inclusion
02/24/09   ssr     Fixed compiler warning
02/24/09   nb      Common Modem Interface
02/16/09   ssr     Fixed QTF Lint Error: Not Referenced
02/10/09   ssr     Changed default NAI domain name for  China Telecom
02/03/09   ssr     Removed EF_MDN checked as mandatory file
01/29/09   nb      Virtuim support in QTF
11/26/08   nb      Klockwork Fixes
11/19/08   nb      Klockwork Fixes
10/22/08   ssr     Fixed lint errors
10/14/08   jk      Fixed Compile Warnings
09/05/08   ssr     Fixed nvsim_read fucntion parameter
09/02/08   ssr     Fixed Multiple profile support for SIPUPP and SIPAPASS
06/23/08   ssr     Fixed first prl size while reading second prl.
06/20/08   ssr     Support of OMH LBS Items
05/16/08   jk      Fixed featurization of SIP NV items
05/05/08   sun     Fixed calculation for pap password length
05/01/08   nk      Fixed and added Messages to nvruim_clear_cache
04/29/08   vs      Fixed lint errors
04/25/08   ssr     Support of OMH Data Item
04/21/08   nk/jk   Fixed Parsing of 3GPDOPM Data, Added Sanity Checks
03/26/08   jk      OMH Phase 1B Support - Multiple SIP Profiles
02/26/08   jk      Added additional handling of SMS items as per OMH POC Request
02/06/08   nk      Added CDMA Refresh Support
02/04/08   nk      Fixed Compilation Warning
12/13/07   vs      Updated function nvruim_read_smscap() based on OMH spec
11/08/07   vs/jk   Fixed 3GPD featurization related compiler errors and EUIMID
                   related lint errors
10/16/07   jk      Fixed Bug for BC-SMS User Config Write
10/10/07   jk      Code changes to support Open Market Handset
08/16/07   jk      Rearranged featuization to support DATA Auth on 7K
08/03/07   nk      Updated read on esn and meid for JCDMA and EUIMID support
07/30/07   nk      Updated nvruim_read_esn for MEID support
07/10/07   nk      Added EUIMID Support
05/11/07   wli     Changes for JCDMA requirements
05/03/07   jk      Fixed compile warning
03/14/07   sun     Fixed Compile Error
1/24/07    jk      Fixed Fatal Error
11/07/06   sun     Reorganized Functions
05/11/06   wli     Re-arrange features for perso
11/22/05   jk      Added support for multiprocessor RPC
03/07/05    jk     Added featurization for RTRE init check (to avoid compile
                   issue when RTRE off)
12/16/04    wli    Fixed size problem when reading second prl.
12/08/04    jk     Slight correction related to CR53836 fix
12/07/04    jk     CR# 53836 - Full 15 digit conversion for CAVE AN AUTH NAI
11/22/04    jk     Finished adding support for 3GPD Simple/Mobile IP
10/22/04   wli     Reinitialized nvruim_sw1 and nvruim_sw2 when read from cache.
09/16/04   wli     Added a check to prevent from accessing card before knowing
                   RTRE configuration.
09/07/04   wli     Get the size info from PRL when FEATURE_2ND_PRL defined.
09/01/04   jk      Added support for HRPD AN Authentication services in R-UIM
07/20/04   wli     Reset CDMA dir present if a mandatory CDMA file is not present
                   (Merged from 6300 brance).
07/15/04   jk      Added support to map NV_HDR_AN_AUTH_USER_ID_LONG_I and
                   NV_HDR_AN_AUTH_USER_ID_I to imsi@unicomcdma.com.cn for China
                   Unicom's AN Cave Authentication
07/14/04   wli     Support feature 2ND_PRL
06/09/04   wli     Changed NV_MODE_DIGITAL_PREF to NV_MODE_AUTOMATIC since CM
                   no longer supports NV_MODE_DIGITAL_PREF.
05/8/04    wli     Added feature that allow to update IMSI_M, IMSI_T, and PRL
05/8/04    wli     Fixed a bug that IMSI_T related items were not correctly
                   filled.
03/10/04   ck      Set the slot in the ACCESS command to AUTOMATIC.
12/19/03   jk      Added processing for R-UIM 3GPD Services.
11/19/03   jqi     CR37168
08/27/03   wli     Fixed missing EF name for NAM_LOCK write.
07/29/03   ck      Added support for old ACCOLC type for targets that want to
                   use mainline NVRUIM with featurization.
07/17/03   jqi     Updated prl_validate API.
04/03/03   wli     Added comments.
03/27/03   ts      Removed MSG ERROR in switch statement when determining if
                   items read successfully are valid.  Also, removed list of
                   NV items from switch as there is nothing to do for these
                   items.
02/13/03   wli     Corrected lint errors.
01/30/03   wli     Assign UIM_FAIL to nvruim_ruim_status_report in nvruim_
                   access().
12/10/02   ts      Fail accesses to the CDMA DF if the CDMA DF is not present
                   on the card.  Do not send the command to the UIM server.
12/03/02   ck      Removed the ACCOLC feature and mainlined it.
12/03/02   wli     Added feature for ACCOLC type.
11/25/02   ts      Use the status set by the UIM power-up/wake-up procedures
                   to determine if CDMA DF is present for RTRE.
10/31/02   ck      Featurized the processing of the NV item NV_PREF_MODE_I
                   under the feature !FEATURE_NVRUIM_PREF_MODE_NOT_FROM_RUIM
06/19/02   ts      Added processing for RTRE configuration.
03/28/02   wli     Changed NAM_LOCK access to meet IS820 Addendum 1 changes.
03/06/02   wli     Use to convert between ascii and alphabet format for
                   phonebook
02/22/02   wli     Added phonebook tag value conversion between ascii and
                   alphabet format. The UIM phonebook is in alphabet format.
                   fixed the bug that phone number was not located right
                   position in phonebook records.
02/13/02   wli     Added new function to set various DFs existing indicator
                   and allow to access phone book when
                   FEATURE_UIM_RUIM_W_GSM_ACCESS is turned ON and card is not
                   RUIM.
12/18/01   ck      Added support to check the status words and fall back to
                   NV if the file is not found in R-UIM when the feature
                   FEATURE_UIM_RUIM_W_GSM_ACCESS is turned ON.
12/13/01   wli     Added funciton to flush SMS cache.
07/23/01   ck      Added support for zeroing ESN as an option based on the esn
                   usage indicator.
06/26/01   ts      Changed debug feature switch names to have "FEATURE_"
                   prepended.
06/11/01   ts      Added NV_SID_NID_I item processing in the CDMA Home SID/NID
                   EF.
06/01/01   ts      Use proper PRL validate procedure when SD2.0 is enabled.
05/31/01   ts      Fixed reference to service preferences for EF.
05/21/01   ck      Modified the command and report processing to use the UIM
                   server Interface.
03/26/01   ts      Added cache'ing for the most recent SMS message.
03/14/01   ts      Removed "imaginary buffer" line that got in when comparing
                   nvruim.c files.
03/13/01   wli     Casted "freq_block" to eliminate ARM25 compiler warnings.
03/07/01   wli     Replaced real numbers with constants.
03/06/01   wli     Support more NV items for registration indicators.
03/06/01   ts      Changed cache control bit to indicate not-cache'd if write
                   to EF fails for cache'd buffer.  Also, fixed addr_num field
                   read request to return proper addr_num bits.
02/22/01   ck      Fixed the bug where OTAPA enable and SPC enable was written
                   and read incorrectly.
02/08/01   wli     Deleted invalidate and rehabilitate functions. Fixed the
                   order problem when call history read is in "absolute" mode
                   and write is in "previous" mode. Unlike speed dial numbers
                   call history information are stored in ruim card only.
1/28/01    wli     Added phone book support which includes speed dial (NV_DIAL_I)
                   and call history (NV_STACK_I).
                   Added a new interface function that enable user to invalidate
                   or rehabilitate an EF.
                   Fixed a CR so that imsi_addr_num is 0xFF when the class is zero.
01/11/01   ts      Added cache'ing for IMSI_M, UIM_ID, IMSI_T, CALL_TERM_MODE_PREF
                   and OTAPA_SPC_ENABLE.
01/08/01   ts      Fixed nvruim_read so that the esn checksum is returned with
                   the proper value whether or not the RUIM fails.  Changed the
                   reformat section of nvruim_read to allow forcing items to
                   debug values and reformat the data only if the value was read
                   properly from the RUIM.
01/04/01   ts      Changed writes to ESN and ESN checksum to return UNSUPPORTED.
                   This allows the ESN to be written to the NV memory.
12/19/00   ck      Changed the size of a 683A PRL returned on a read to reflect
                   the number of bits in the PRL rather than number of bytes.
12/13/00   wli     Copied ACCOLC, MOB_TERM_HOME, MOB_TERM_FOR_SID, and
                   MOB_TERM_FOR_NID to AMPS index.
                   Removed debug feature around NV_COUNT_I write.
                   Added debug code to force MIN1 and MIN2.
                   Fixed otapa_enabled flag bug.
11/28/00   wli     Suppot more nv items and SMS feature.
11/15/00   ck      Removed the parsing of result for MSISDN from Telecom dir
                   Added debug code to Unblock CHV
                   Grouped the processing of READ_ONLY RUIM items.
                   Added the function nvruim_set_esn_usage() that sets the
                   esn usage indicator to use ESN or UIM_ID.
11/06/00   ts      Added access to the MSISDN file in CDMA directory for
                   NV_DIR_NUMBER_I and NV_DIR_NUMBER_PCS_I.
10/13/00   ck      Grouped the items that return the write status as READONLY.
                   Modified the default Dir number to 858-555-1212.
                   Removed support for MS-ISDN.
10/11/00   ts      Changed failed RUIM accesses as NOT ACTIVE from FAILED.
                   Changed accesses to RUIM status to match new report type.
                   Copied MIN1 and MIN2 to index 0 of the MIN arrays to support
                   AMPS.  Added options for ESN failure.
09/18/00   ts      Added an ifndef around temp variables in nvruim_read to
                   prevent warning messages when NVRUIM_DEBUG_FORCE_DIR_NUM
                   is defined.
08/08/00   ms      Added nvruim_free to intercept NV_FREE_F calls
08/04/00   ms      Added SMS, SMSS, and SMSP support
06/22/00   ck      Changed the ifdef around writing Dir number to ifndef.
06/13/00   ck      Changed the feature around writing call count to
                   FEATURE_RUIM_ADM_ACCESS to prevent the Count from being
                   to the card as the interface needs to be changed.
06/12/00   ck      Changed the parsing of MSISDN to derive the MDN.
06/06/00   ck      Added the debug feature for writing dir number which
                   broke the service programming.
06/06/00   ck      Increased the array size from 10 to 11 to remove a compiler
                   warning.
06/06/00   ck      Rewrite the status to NOTACTIVE while reading NV_COUNT_I
                   when the debug feature is defined.
06/01/00   ck      Rewrite status to NOTACTIVE to support upper layer error
                   recovery and added a debug feature for writing call count
                   to the RUIM.
05/24/00   jah     Add MSISDN support.  Feature-ized debug support.
05/15/00   jah     Added short-cut exit for ADM protected EF writes
04/24/00   jah     Initial Revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "customer.h"
#include "intconv.h"
#ifdef FEATURE_UIM_RUN_TIME_ENABLE
#include "nv.h"
#endif /* FEATURE_UIM_RUN_TIME_ENABLE */

#include "comdef.h"
#include "err.h"
#include "msg.h"
#include "uim_selective_logging.h"
#include <stringl/stringl.h>
#include "nv.h"
#include "nvruimi.h"
#include "nvruim.h"
#include "nvruim_p.h"
#include "task.h"
#include "prl.h"
#include "modem_mem.h"
#include "fs_public.h"

#include "bit.h"
#include "qw.h"

#include "mmgsdilib.h"
#include "mmgsdilib_v.h"
#include "mmgsdisessionlib.h"
#include "mmgsdilib_p.h"
#include "mmgsdisessionlib_v.h"
#include "nvruim_task.h"
#include "nvruim_request.h"
#include "nvruim_util.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                         DATA DEFINITIONS                                */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define NVRUIM_CDMA_FILE_LOOKUP_TABLE_SIZE    46

/* Mask of bit indicating the IMSI was programmed */
#define NVRUIM_IMSI_PROGRAMMED              0x80

/* Mask of bits for the IMSI Addr Num */
#define NVRUIM_IMSI_ADDR_NUM                0x07

/* Mask of the OTAPA_enable but in OTAPA/SPC_enable EF */
#define NVRUIM_OTAPA_ENABLE_MASK            0xFE

/* OTAPA disabled by setting last bit to 1 */
#define NVRUIM_OTAPA_DISABLE                0x01

/* Mask of the SPC_enable but in OTAPA/SPC_enable EF */
#define NVRUIM_SPC_ENABLE_MASK              0xEF

/* SPC is disabled by setting the bit to 1 */
#define NVRUIM_SPC_DISABLE                  0x10

/* Mask of the NAM_LOCK bit in NAM_LOCK EF */
#define NVRUIM_NAM_LOCK_MASK                0xFD

/* SPC is disabled by setting the bit to 1 */
#define NVRUIM_NAM_LOCK                     0x02

/* Mask for Analog/Digital Selection */
#define NVRUIM_PREF_MODE_MASK               0x70

/* Mask for A/B Selection */
#define NVRUIM_SYSTEM_PREF_MASK             0x07

/* Mask for band class */
#define NVRUIM_BAND_CLASS_MASK              0xF8

/* Service Preferences bits */
#define NVRUIM_ANALOG_PREF                  0x10
#define NVRUIM_CDMA_PREF                    0x20
#define NVRUIM_ANALOG_ONLY                  0x50
#define NVRUIM_CDMA_ONLY                    0x60
#define NVRUIM_A_ONLY                       0x05
#define NVRUIM_B_ONLY                       0x06
#define NVRUIM_NO_PREF                      0x00

/* MSISDN total size */
#define NVRUIM_MSISDN_SIZE                    11

/* Number of digits byte offset from beginning  of MSISDN */
#define NVRUIM_MSISDN_ND_INDEX                 0

/* MDN byte offset from beginning of MSISDN */
#define NVRUIM_MSISDN_MDN_INDEX                1

/* RUIM access is limited to 255 byte maximum */
#define MAX_BYTES_PER_ACCESS                 255

/* HOME SID NID PAIR record size */
#define SID_NID_REC_SIZE                       5

#define NVRUIM_MAX_BCSMS_TABLE_ENTRY_SIZE   255
#define NVRUIM_MAX_BCSMS_TABLE_ENTRIES      128
#define NVRUIM_BCSMS_PARM_SIZE              2
#define NVRUIM_SMSCAP_SIZE                  4
#define NVRUIM_SMSCAP_ON_ACCESS_CH_MASK     0x01
#define NVRUIM_SMSCAP_ON_TRAFFIC_CH_MASK    0x02

/* The following are masks, constants, and shift values used when parsing
elementary files for 3GPD */
#define NVRUIM_MAX_SIPUPP_SIZE      255
#define NVRUIM_MAX_MIPUPP_SIZE      255
#define NVRUIM_MAX_NUM_MIP_PROFILES 16
#define NVRUIM_MAX_SIPPAPSS_SIZE    255
#define NVRUIM_MAX_SIPPAPSS_SS_SIZE 31
#define NVRUIM_MAX_TCPCONFIG_SIZE   2
#define NVRUIM_MAX_DGC_SIZE         3
#define NVRUIM_MAX_NUM_SIP_PROFILES 16
#define NVRUIM_MAX_UPPEXT_SIZE      255

/* Since each file can contain at most 255 bytes, the max number of bits is
255 x 8 = 2040.  The following value is reserved to indicate a non valid bit value */
#define NVRUIM_NO_PROFILE_INDEX_SET    0xFFFF

#define NVRUIM_SIPPAPSS_NUM_NAI_MASK        0xF0
#define NVRUIM_SIPUPP_NUM_NAI_MASK          0xF0
#define NVRUIM_UPPEXT_NUM_NAI_MASK          0xF0
#define NVRUIM_MIPUPP_RETRY_INCL_MASK       0x80
#define NVRUIM_MIPUPP_MAX_RETRY_MASK        0x60
#define NVRUIM_MIPUPP_RETRY_INT_MASK        0x1C

#define NVRUIM_3GPDOPM_MASK                 0x3

#define NVRUIM_MIPSP_SHIFT                          4
#define NVRUIM_SIPSP_SHIFT                          4
#define NVRUIM_RETRY_INFO_PRESENT_SHIFT             7
#define NVRUIM_MAX_NUM_RETRY_SHIFT                  5
#define NVRUIM_MIP_RETRY_INT_SHIFT                  2
#define NVRUIM_MIPFLAGS_DRS_OPT_SHIFT               1
#define NVRUIM_MIPFLAGS_HANDDOWN_SHIFT              2
#define NVRUIM_MIPFLAGS_RRQ_IF_TRFK_SHIFT           3


#define NVRUIM_MAX_HRPDUPP_SIZE      255

/* Call Count record length */
#define NVRUIM_COUNT_REC_LEN         2

#define NVRUIM_SLOT_INDEX_INVALID    (0xFF)

/* Max number of Timers that can be installed by NVRUIM for requests */
#define NVRUIM_NUM_TIMERS            20

/* Timer elapse value in msecs */
#define NVRUIM_MMGSDI_REQ_TIMER_VAL  8000

#if defined (FEATURE_UIM_CAVE_AN_AUTH)

/* Length of String for China Unicom AM Cave NAI */
#define NVRUIM_CHINA_UNICOM_AN_CAVE_NAI_LENGTH 25
/* String constant used to build NAI string for China-Unicom, AN Cave Auth
The string is of the format IMSI@unicomcdma.com.cn.  The IMSI is of the format
46003+MIN.  This string constant is in ASCII, and is
"460030000000000@unicomcdma.com.cn".  Digits 6-15 will be re-written with the
MIN once it is read from the R-UIM.*/
const byte nvruim_an_cave_nai[NVRUIM_CHINA_UNICOM_AN_CAVE_NAI_LENGTH] =
{
 '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
 '@', 'm', 'y', 'c', 'd', 'm', 'a', '.', 'c', 'n'
};

/* Constants for MIN conversion routines */
#define NVRUIM_PREFIX_MASK    0x00FFC000
#define NVRUIM_THOUSANDS_MASK 0x00003C00
#define NVRUIM_LAST_3_MASK    0x000003FF

#define NVRUIM_PREFIX_SHIFT          14
#define NVRUIM_THOUSANDS_SHIFT       10
#define NVRUIM_LAST_3_SHIFT           0

const byte nvruim_min_to_num[] =
{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

const byte nvruim_bcd_to_num[] =
{ 0xFF, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#endif /* FEATURE_UIM_CAVE_AN_AUTH */

/* Constants and data for LBS  */
/* The following are masks, constants, and shift values used during parsing of
elementary files for LBS */

/* Constant belonging to EF-LBSV2Config EF */
#define NVRUIM_MAX_LBS_V2_CONFIG_SIZE              1
#define NVRUIM_LBS_FLAG_GPS1_SEED_POSITION_SHIFT   1
#define NVRUIM_LBS_FLAG_GPS1_DYNAMIC_MODE_SHIFT    2

/* Constant belonging to EF-LBSXTRAConfig EF */
#define NVRUIM_MAX_LBS_XTRA_CONFIG_SIZE                 4
#define MAX_VALUE_NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I      168
#define MAX_VALUE_NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I  10
#define MAX_VALUE_NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I  120

/* Constant belonging to EF-LBS XTRA Server URLs EF */
#define NVRUIM_LBS_XTRA_URL_TAG                 0x80
#define NVRUIM_LBS_XTRA_URL_LEN_TAG_82          0x82
#define NVRUIM_LBS_XTRA_URL_LEN_TAG_81          0x81
#define NVRUIM_MAX_LBS_URL_TAG_LEN_INFO         NVRUIM_LBS_XTRA_URL_LEN_TAG_82
#define NVRUIM_LBS_XTRA_URL_FIRST               0x01
#define NVRUIM_LBS_XTRA_URL_SECOND              0x02
#define NVRUIM_LBS_XTRA_URL_THIRD               0x03
#define NVRUIM_INVALID_LBS_URL_TAG_LEN_INFO     0x80

/* Constant Belonging to LBS PDE and MPC Address Type and its maximum Length*/
#define NVRUIM_MAX_LBS_DOMAIN_ADDRESS_LENGTH    128
/* IPv4 addresses are usually written in dot-decimal notation,
   which consists of the four octets of the address expressed in decimal
   and separated by periods. for example 121.121.121.121 */
#define NVRUIM_MAX_LBS_IPV4_ADDRESS_LENGTH       15
/* IPv6 addresses are normally written as eight groups of four hexadecimal
   digits, where each group is separated by a colon (:). eg:
   2001:1234:0F12:089B:0000:0000:1234:5678 */
#define NVRUIM_MAX_LBS_IPV6_ADDRESS_LENGTH       39
#define NVRUIM_LBS_ADDRESS_TYPE_DOMAIN_NAME       1
#define NVRUIM_LBS_ADDRESS_TYPE_IPV4              2
#define NVRUIM_LBS_ADDRESS_TYPE_IPV6              3

/* Constant belonging to EF-LBS V2 PDE ADDRESS EF */
#define NVRUIM_MAX_LBS_V2_PDE_ADDRESS_SIZE      132

/* Constant belonging to EF-LBS V2 MPC ADDRESS EF */
#define NVRUIM_MAX_LBS_V2_MPC_ADDRESS_SIZE      132


/* constant belonging to EF-LBS XTRA SERVER URLS */
#define NVRUIM_MAX_LBS_XTRA_SERVER_URL_LENGTH   128

#define NVRUIM_SW1_REFERENCE                     ((uint8)SW1_REFERENCE) /* SW1 for Reference management problem */
#define NVRUIM_SW2_NOT_FOUND                     ((uint8)SW2_NOT_FOUND) /* SW2 for File ID/pattern not found */
#define NVRUIM_SW1_CMD_NOT_ALLOWED               ((uint8)SW1_CMD_NOT_ALLOWED ) /* SW1 for Command not allowed */
#define NVRUIM_SW2_SECURITY_STATUS_NOT_SATISFIED ((uint8)SW2_SECURITY_NOT_SATISFIED) /* SW2 for Access violation */

/* Minimum value of data dormant timer according to spec C.S0065-B v1.0 */
#define NVRUIM_MIN_DATA_DORMANT_TIMER_VALUE     20

/* ----------------------------------------------------------------------------

   ENUM:      MMGSDI_TASK_ENUM_TYPE

   DESCRIPTION:
     Describes the NVRUIM request's status
-------------------------------------------------------------------------------*/
typedef enum {
  NVRUIM_REQ_STATE_NONE               = 0x00,
  NVRUIM_REQ_STATE_TIMER_CREATED,
  NVRUIM_REQ_STATE_PENDING_MMGSDI_RSP,
  NVRUIM_REQ_STATE_TIMER_EXPIRED
} nvruim_request_state_type;

/* ----------------------------------------------------------------------------

   ENUM:      NVRUIM_RPT_STATUS

   DESCRIPTION:
     Describes the status reported by card for each command
-------------------------------------------------------------------------------*/
typedef enum {
  NVRUIM_FAIL = 0,                /* FAIL */
  NVRUIM_PASS = 1                 /* PASS */
} nvruim_rpt_status_enum_type;


/* ----------------------------------------------------------------------------
   STRUCTURE:      NVRUIM_TIMER_INFO_TYPE

   DESCRIPTION:
     The structure contains timer info for NVRUIM
-------------------------------------------------------------------------------*/
typedef struct {
  nvruim_request_state_type     req_state;
  rex_timer_type                nvruim_req_timer;
  nvruim_nv_context_type        req_context;
} nvruim_timer_info_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_CMD_TYPE

   DESCRIPTION:
     The structure contains information regarding the current NVRUIM command
-------------------------------------------------------------------------------*/
typedef struct {
  cmd_hdr_type                cmd_hdr;
  nvruim_cmd_enum_type        cmd_type;
  mmgsdi_data_type            data;
  mmgsdi_file_enum_type       file_enum;
  uint32                      offset_or_recnum;
  uint32                      user_data;
  mmgsdi_rec_num_type         num_records;
  mmgsdi_sw_type              status_word;
  nvruim_rpt_status_enum_type report_status;
  nvruim_nv_context_type      nv_context;
} nvruim_cmd_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_IMSI_TYPE

   DESCRIPTION:
     IMSI EF structure for IMSI_M and IMSI_T accesses
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte imsi_class;
  byte imsi_s2[2];
  byte imsi_s1[3];
  byte imsi_11_12;
  byte imsi_addr_num;
  byte imsi_mcc[2];
} nvruim_imsi_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_ANALOG_CH_PREF

   DESCRIPTION:
     Analog Channel Preferences EF structure (EF 6F2F)
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte analog_firstchp[2];
  byte analog_first_dcc_a[2];
  byte analog_first_dcc_b[2];
  byte analog_num_of_dcc;
} nvruim_analog_ch_pref;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_TMSI_TYPE

   DESCRIPTION:
     TMSI EF structure
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte length;
  byte zone[8];
  byte code[4];
  byte time[3];
} nvruim_tmsi_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_ANALOG_REG_IND_TYPE

   DESCRIPTION:
     Analog Location and Registration Indicators EF structure
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte nxtreg[3];
  byte sid[2];
  byte locaid_pureg[2];
} nvruim_analog_reg_ind_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_ZONE_REG_IND_TYPE

   DESCRIPTION:
     CDMA Zone-Based Registration Indicators
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte reg_zone[2];
  byte sid[2];
  byte nid[2];
  byte freq_block;
  byte band_class;
} nvruim_zone_reg_ind_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_SYS_REG_IND_TYPE

   DESCRIPTION:
     CDMA System/Network Registration Indicators
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte sid[2];
  byte nid[2];
  byte freq_block;
  byte band_class;
} nvruim_sys_reg_ind_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_DIST_REG_IND_TYPE

   DESCRIPTION:
     CDMA Distance-Based Registration Indicators
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte base_lat_reg[3];
  byte base_long_reg[3];
  byte reg_dist_reg[2];
} nvruim_dist_reg_ind_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_SIPUPP_EF_FIELDS

   DESCRIPTION:
     Typedef to represent the number of bits for each field in a 3GPD EF
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte length_of_block_field[8];
  byte num_nai_field[4];
  byte profile_num_field[4];
  byte nai_length_field[8];
  byte auth_algo_field[4];
} nvruim_sipupp_ef_fields;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_MIPUPP_EF_FIELDS

   DESCRIPTION:
     Typedef to represent the number of bits for each field in a 3GPD EF
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte length_of_block_field[8];
  byte retry_info_incl_field;
  byte max_num_retry_field[2];
  byte retry_timeout_field[3];
  byte rereg_threshold_field[6];
  byte num_nai_field[4];
  byte nai_entry_index_field[4];
  byte nai_length_field[8];
  byte tunnel_bit_field;
  byte home_addr_field[32];
  byte primary_ha_field[32];
  byte secondary_ha_field[32];
  byte mn_aaa_auth_algo_field[4];
  byte mn_aaa_spi_indicator_field;
  byte mn_aaa_spi_field[32];
  byte mn_ha_auth_algo_field[4];
  byte mn_ha_spi_indicator_field;
  byte mn_ha_spi_field[32];
} nvruim_mipupp_ef_fields;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_SIPAPSS_EF_FIELDS

   DESCRIPTION:
     Typedef to represent the number of bits for each field in a 3GPD EF
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte length_of_block_field[8];
  byte num_nai_field[4];
  byte profile_num_field[4];
  byte ss_length_field[5];
} nvruim_sippapss_ef_fields;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_UPPEXT_EF_FIELDS

   DESCRIPTION:
     Typedef to represent the number of bits for each field in a 3GPD EF
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  byte num_nai_field[4];
  byte profile_num_field[4];
  byte applications_field[32];
  byte priority_field[8];
  byte data_rate_mode_field[4];
  byte data_bearer_field[4];
} nvruim_uppext_ef_fields;

/* ----------------------------------------------------------------------------
   STRUCTURE:      NVRUIM_SID_NID_TYPE

   DESCRIPTION:
     CDMAHOME  EF structure
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  word sid;
  word nid;
  byte band_class;
} nvruim_sid_nid_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_FILE_LOOKUP_TYPE

   DESCRIPTION:
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_file_enum_type            mmgsdi_ruim_file;
  mmgsdi_file_enum_type            mmgsdi_csim_file;
  mmgsdi_file_structure_enum_type  file_struct;
  boolean                          pin_protected;
} nvruim_file_lookup_type;

/* Global structure for session based info for NVRUIM task */
nvruim_session_data_type  *nvruim_session_data_ptr[NVRUIM_NUM_1X_SESSION_MAX]  = {NULL};

/* Constants and data for 3GPD */
/* This contains the default sip profile.  The current DATA implementation
only supports one SIP profile so references to the generic NV SIP profile
always map to profile 0.  If functionality is expanded in the future to
multiple profiles, this will be dynamically assigned by a parameter assigned
in an NV Access. */
byte nvruim_preferred_sip_profile = UIM_CURRENTLY_ACTIVE_SIP_PROFILE;

/* Indicate existing DFs (MF, CDMA, GSM, or DCS 1800 ) */
nvruim_cdma_subscription_type  nvruim_dir_present[NVRUIM_MAX_CARD_COUNT] = {NVRUIM_CDMA_SUBSCRIPTION_NOT_PRESENT};

static mmgsdi_client_id_type     nvruim_mmgsdi_client_id                          = MMGSDI_CLIENT_ID_ZERO;
boolean                          nvruim_sim_busy[NVRUIM_MAX_CARD_COUNT]           = {FALSE};
static nvruim_timer_info_type    nvruim_timer_info[NVRUIM_NUM_TIMERS];

static nvruim_file_lookup_type
  nvruim_cdma_file_lookup_table[NVRUIM_CDMA_FILE_LOOKUP_TABLE_SIZE] =
{
  {MMGSDI_CDMA_RUIM_ID,MMGSDI_CSIM_RUIM_ID,
   MMGSDI_TRANSPARENT_FILE, FALSE},
  {MMGSDI_CDMA_CC,MMGSDI_CSIM_CALL_COUNT,
   MMGSDI_CYCLIC_FILE, TRUE},
  {MMGSDI_CDMA_IMSI_M,MMGSDI_CSIM_IMSI_M,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_IMSI_T,MMGSDI_CSIM_IMSI_T,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_HOME_SID_NID,MMGSDI_CSIM_CDMA_HOME_SID_NID,
   MMGSDI_LINEAR_FIXED_FILE, TRUE},
  {MMGSDI_CDMA_MSISDN,MMGSDI_CSIM_MSISDN,
   MMGSDI_LINEAR_FIXED_FILE, TRUE},
  {MMGSDI_CDMA_ANALOG_CHAN_PREF,MMGSDI_CSIM_ANALOG_CHAN_PREF,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_ANALOG_HOME_SID,MMGSDI_CSIM_ANALOG_HOME_SID,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_ACCOLC,MMGSDI_CSIM_ACCOLC,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_SCI,MMGSDI_CSIM_SSCI,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_CALL_TERM_MODE_PREF,MMGSDI_CSIM_CALL_TERM_MODE_PREF,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_MAXIMUM_PRL,MMGSDI_CSIM_MAXIMUM_PRL,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_PRL,MMGSDI_CSIM_PRL,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_OTAPA_SPC_ENABLE,MMGSDI_CSIM_OTAPA_SPC_ENABLE,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_NAM_LOCK,MMGSDI_CSIM_NAM_LOCK,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_SERVICE_PREF,MMGSDI_CSIM_SERVICE_PREF,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_TMSI,MMGSDI_CSIM_TMSI,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND,MMGSDI_CSIM_ANALOG_LOCN_AND_REGN_IND,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_ZONE_BASED_REGN_IND,MMGSDI_CSIM_CDMA_ZONE_BASED_REGN_IND,
   MMGSDI_LINEAR_FIXED_FILE, TRUE},
  {MMGSDI_CDMA_SYS_REGN_IND,MMGSDI_CSIM_CDMA_SYS_REGN_IND,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_DIST_BASED_REGN_IND,MMGSDI_CSIM_CDMA_DIST_BASED_REGN_IND,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_3GPDOPM,MMGSDI_CSIM_3GPD_3GPDOPM,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_UPPEXT,MMGSDI_CSIM_3GPD_UPPEXT,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_SIPUPP,MMGSDI_CSIM_3GPD_SIPUPP,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_MIPUPP,MMGSDI_CSIM_3GPD_MIPUPP,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_MIPFLAGS,MMGSDI_CSIM_3GPD_MIPFLAGS,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_TCPCONFIG,MMGSDI_CSIM_3GPD_TCPCONFIG,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_LBS_XSURL,MMGSDI_CSIM_LBS_XSURL,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_LBS_XCONFIG,MMGSDI_CSIM_LBS_XCONFIG,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_LBS_V2CONFIG,MMGSDI_CSIM_LBS_V2CONFIG,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_LBS_V2PDEADDR,MMGSDI_CSIM_LBS_V2PDEADDR,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_LBS_V2MPCADDR,MMGSDI_CSIM_LBS_V2MPCADDR,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_DGC,MMGSDI_CSIM_3GPD_DGC,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_MIPSP,MMGSDI_CSIM_3GPD_MIPSP,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_SIPSP,MMGSDI_CSIM_3GPD_SIPSP,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_3GPD_SIPPAPSS,MMGSDI_CSIM_3GPD_SIPPAPSS,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_HRPD_HRPDUPP,MMGSDI_CSIM_HRPDUPP,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_SF_EUIM_ID,MMGSDI_CSIM_SF_EUIM_ID,
   MMGSDI_TRANSPARENT_FILE, FALSE},
  {MMGSDI_CDMA_SMS_CAP,MMGSDI_CSIM_SMS_CAP,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_BCSMS_CONFIG,MMGSDI_CSIM_BCSMS_CONFIG,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_BCSMS_PREF,MMGSDI_CSIM_BCSMS_PREF,
   MMGSDI_TRANSPARENT_FILE, TRUE},
  {MMGSDI_CDMA_BCSMS_PARAMS,MMGSDI_CSIM_BCSMS_PARAMS,
   MMGSDI_LINEAR_FIXED_FILE, TRUE},
  {MMGSDI_CDMA_BCSMS_TABLE,MMGSDI_CSIM_BCSMS_TABLE,
   MMGSDI_LINEAR_FIXED_FILE, TRUE},
  {MMGSDI_CDMA_SMS,MMGSDI_CSIM_SMS,
   MMGSDI_LINEAR_FIXED_FILE, TRUE},
  {MMGSDI_CDMA_SMS_PARAMS,MMGSDI_CSIM_SMS_PARAMS,
   MMGSDI_LINEAR_FIXED_FILE, TRUE},
  {MMGSDI_CDMA_SVC_TABLE,MMGSDI_CSIM_CST,
   MMGSDI_TRANSPARENT_FILE, TRUE}
};


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                         FUNCTION DEFINITIONS                            */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static inline void nvruim_memscpy
(
  void       *dst_ptr,
  size_t     dst_len,
  const void *src_ptr,
  size_t     src_len
);

static boolean nvruim_get_mmgsdi_item_and_file_struct
(
  mmgsdi_file_enum_type             mmgsdi_item_ptr,
  mmgsdi_file_structure_enum_type  *file_struct_ptr,
  mmgsdi_access_type               *nvruim_mmgsdi_access_ptr,
  nvruim_nv_context_type            context
);

static void nvruim_mmgsdi_access_cb
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
);

static void nvruim_mmgsdi_reg_cb
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
);

static void nvruim_mmgsdi_evt_cb
(
  const mmgsdi_event_data_type *event
);

static boolean nvruim_mmgsdi_item_is_pin_protected
(
  mmgsdi_file_enum_type mmgsdi_item
);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                               Macros                                    */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*===========================================================================

MACRO NVRUIM_SIZE


DESCRIPTION
  Size of an NV item

PARAMETERS
  eee   NV item being sized

DEPENDENCIES
  None

RETURN VALUE
  size of the NV item

SIDE EFFECTS
  None

===========================================================================*/
#define NVRUIM_SIZE(eee) \
  ((word)(sizeof(((nv_item_type *) 0)-> eee )))

/*===========================================================================

MACRO NVRUIM_SESSION_NOT_VALID


DESCRIPTION
  Check whether the session is 1X session or not

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define NVRUIM_SESSION_NOT_VALID(nvruim_session)    \
  (nvruim_session != NVRUIM_1X_PROV_PRI_SESSION &&  \
   nvruim_session != NVRUIM_1X_PROV_SEC_SESSION &&  \
   nvruim_session != NVRUIM_1X_PROV_TER_SESSION)


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                             Functions                                   */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*===========================================================================
FUNCTION NVRUIM_IS_MMGSDI_TASK_CONTEXT

DESCRIPTION
  This function checks if the current call stack is an MMGSDI task

DEPENDENCIES
  None

RETURN VALUE
  TRUE   - If the current call stack is in MMGSDI tasks' context
  FALSE  - If otherwise

SIDE EFFECTS
  None
============================================================================*/
static boolean nvruim_is_mmgsdi_task_context(
  rex_tcb_type *in_tcb_ptr
)
{
  if(TASK_HANDLE(gsdi) == in_tcb_ptr ||
     TASK_HANDLE(mmgsdi_1) == in_tcb_ptr ||
     TASK_HANDLE(mmgsdi_2) == in_tcb_ptr ||
     TASK_HANDLE(mmgsdi_3) == in_tcb_ptr)
  {
    return TRUE;
  }
  return FALSE;
} /* nvruim_is_mmgsdi_task_context */


/*===========================================================================

FUNCTION NVRUIM_DECODE_MIN

DESCRIPTION
     Decode MIN 1 & 2 to a vector of digits

PARAMETERS
   digits_ptr      where the decoded MIN is placed
   digits_len      Length of array pointed by digits_ptr
   mcc             MCC being decoded
   mnc             MNC being decoded
   min1            MIN1 being decoded
   min2            MIN2 being decoded

RETURN VALUE
   TRUE -          MIN was decoded
   FALSE -         Decoding failed

SIDE EFFECTS
  None

===========================================================================*/
static boolean nvruim_decode_min
(
  byte   *digits_ptr,              /* pointer to converted digits */
  uint16  digits_len,               /* length of array pointed by digits_ptr */
  uint16  mcc,                     /* mcc */
  uint8   mnc,                     /* mnc */
  uint32  min1,                    /* min1 */
  uint16  min2                     /* min2 */
)
{
#if defined (FEATURE_UIM_CAVE_AN_AUTH)
  byte digit = 0;
  byte digit_pointer = 0;
  word second_three = 0;
  byte thousands = 0;
  word last_three = 0;

  if (digits_ptr == NULL || digits_len < NVRUIM_NUM_MIN_DIGITS)
  {
    return FALSE;
  }

  UIM_MSG_HIGH_4("Decode min: mcc 0x%x, mnc 0x%x, min1 0x%x, min2 0x%x", mcc, mnc, min1, min2);

  memset(digits_ptr, 0x0, NVRUIM_NUM_MIN_DIGITS);
  if(mcc <= 999)
  {
    /* Converting Mobile Country Code */
    digit = nvruim_min_to_num[mcc/100];
    digits_ptr[digit_pointer++] = digit;
    mcc %= 100;

    digit = nvruim_min_to_num[mcc / 10];
    digits_ptr[digit_pointer++] = digit;

    digit = nvruim_min_to_num[mcc % 10];
    digits_ptr[digit_pointer++] = digit;

  }
  else
  {
    /* Error in converting Mobile Country Code of IMSI */
    UIM_MSG_HIGH_0("Error in converting IMSI");
    return FALSE;
  }
  if(mnc <= 99)
  {
    /* Convert Mobile Network Code */
    digit = nvruim_min_to_num[mnc / 10];
    digits_ptr[digit_pointer++] = digit;

    digit = nvruim_min_to_num[mnc % 10];
    digits_ptr[digit_pointer++] = digit;
  }
  else
  {
    /* Error in converting Mobile Network Code of IMSI */
    UIM_MSG_HIGH_0("Error in converting IMSI");
    return FALSE;
  }
  if (min2 <= 999)
  {
    /* Convert First Three Digits of IMSI */
    digit = nvruim_min_to_num[min2 / 100];
    digits_ptr[digit_pointer++] = digit;
    min2 %= 100;

    digit = nvruim_min_to_num[min2 / 10];
    digits_ptr[digit_pointer++] = digit;

    digit = nvruim_min_to_num[min2 % 10];
    digits_ptr[digit_pointer++] = digit;
  }
  else
  {
    /* Error in converting First 3 Digits of IMSI */
    UIM_MSG_HIGH_0("Error in converting IMSI");
    return FALSE;
  }

  if (min1 > 0)
  {
    /* Convert MIN 1 Portion of IMSI */
    second_three = (min1 & NVRUIM_PREFIX_MASK) >> NVRUIM_PREFIX_SHIFT;
    thousands = (min1 & NVRUIM_THOUSANDS_MASK) >> NVRUIM_THOUSANDS_SHIFT;
    last_three = (min1 & NVRUIM_LAST_3_MASK) >> NVRUIM_LAST_3_SHIFT;

    thousands = nvruim_bcd_to_num[thousands];
    if ( (thousands != 0xFF)
    &&   (second_three <= 999)
    &&   (last_three <= 999) )
    {
      /* IMSI data is of the right format */
      digit = nvruim_min_to_num[second_three / 100];
      digits_ptr[digit_pointer++] = digit;

      second_three %= 100;

      digit = nvruim_min_to_num[second_three / 10];
      digits_ptr[digit_pointer++] = digit;

      digit = nvruim_min_to_num[second_three % 10];
      digits_ptr[digit_pointer++] = digit;

      digits_ptr[digit_pointer++] = thousands;

      digit = nvruim_min_to_num[last_three / 100];
      digits_ptr[digit_pointer++] = digit;

      last_three %= 100;

      digit = nvruim_min_to_num[last_three / 10];
      digits_ptr[digit_pointer++] = digit;

      digit = nvruim_min_to_num[last_three % 10];
      digits_ptr[digit_pointer++] = digit;
    } /* end if - last 7 digits of IMSI are correct format */
  } /* end if MIN 1 is a valid number */

  if (digit_pointer != NVRUIM_NUM_MIN_DIGITS)
  {
    UIM_MSG_HIGH_0("Error Converting IMSI");
    memset(digits_ptr, 0x0, NVRUIM_NUM_MIN_DIGITS);
    return FALSE;
  }
  return TRUE;
#else
  (void)digits_ptr;
  (void)digits_len;
  (void)mcc;
  (void)mnc;
  (void)min1;
  (void)min2;
  return FALSE;
#endif /* defined (FEATURE_UIM_CAVE_AN_AUTH) */
} /* nvruim_decode_min */


/*===========================================================================

FUNCTION NVRUIM_MEM_ALLOC

DESCRIPTION
  This function calls modem_mem_calloc() to allocate from Modem Heap.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  a pointer is allocated.
===========================================================================*/
void * nvruim_mem_malloc
(
  dword size
)
{
  return modem_mem_calloc(1, size, MODEM_MEM_CLIENT_UIM);
}/* nvruim_mem_malloc */


/*===========================================================================

FUNCTION NVRUIM_MEM_FREE_NULL_OK

DESCRIPTION
  The NVRUIM_MEM_FREE free the pointer from the Modem Heap. If the pointer
  intended to be free'd is already NULL, the functions just prints a
  message and returns.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  the pointer is freed, if non-null.
===========================================================================*/
void nvruim_mem_free_null_ok
(
  void * ptr
)
{
  if (ptr == NULL)
  {
    return;
  }
  modem_mem_free(ptr, MODEM_MEM_CLIENT_UIM);
}/* nvruim_mem_free_null_ok */


/*===========================================================================

FUNCTION                  NVRUIM_GET_TIMER_REQ_STATE

DESCRIPTION
  Fetches the MMGSDI request state of the requested timer index.

DEPENDENCIES
  None

RETURN VALUE
  nvruim_request_state_type

SIDE EFFECTS
  None

===========================================================================*/
static nvruim_request_state_type nvruim_get_timer_req_state
(
  uint8                     index_timer
)
{
  nvruim_request_state_type req_state = NVRUIM_REQ_STATE_NONE;

  if (index_timer < NVRUIM_NUM_TIMERS)
  {
    req_state = nvruim_timer_info[index_timer].req_state;
  }
  return req_state;
} /* nvruim_get_timer_req_state */


/*===========================================================================

FUNCTION                  NVRUIM_UPDATE_TIMER_REQ_STATE

DESCRIPTION
  Updates the global MMGSDI request state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void nvruim_update_timer_req_state
(
  uint8                     index_timer,
  nvruim_request_state_type in_req_state
)
{
  /* Find out the request's status */
  if ((index_timer < NVRUIM_NUM_TIMERS) &&
      (nvruim_timer_info[index_timer].req_state != in_req_state))
  {
    nvruim_timer_info[index_timer].req_state = in_req_state;
  }
} /* nvruim_update_timer_req_state */


/*===========================================================================

FUNCTION                  NVRUIM_MMGSDI_REQ_TIMER_EXPIRY

DESCRIPTION
  Callback function for the MMGSDI response timer expiry to handle such a
  scenario. Currently, in such a scenario, an error response is sent to the
  caller so that we can eliminate fatal errors like dog timeouts etc.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void nvruim_mmgsdi_req_timer_expiry
(
  unsigned long index_timer
)
{
  UIM_MSG_HIGH_1("NVRUIM: req_timer_expiry, index_timer: 0x%x", index_timer);

  /* Sanity check */
  if (index_timer >= NVRUIM_NUM_TIMERS || nvruim_generic_data_ptr == NULL)
  {
    return;
  }

  /* Find out the request's status */
  NVRUIM_ENTER_TIMER_CRIT_SECT;
  if (nvruim_timer_info[index_timer].req_state != NVRUIM_REQ_STATE_NONE)
  {
    /* At this point, if the req was already sent to MMGSDI, cancel it. Also
       update the timer state to EXPIRED so that any READ/WRITE/SEARCH request
       received by NVRUIM fails because timer is in expired state */
    if (nvruim_timer_info[index_timer].req_state == NVRUIM_REQ_STATE_PENDING_MMGSDI_RSP)
    {
      nvruim_send_mmgsdi_cancel(nvruim_timer_info[index_timer].req_context);
    }
    nvruim_update_timer_req_state(index_timer, NVRUIM_REQ_STATE_TIMER_EXPIRED);
  }
  NVRUIM_LEAVE_TIMER_CRIT_SECT;
} /* nvruim_mmgsdi_req_timer_expiry */


/*===========================================================================

FUNCTION                  NVRUIM_SETUP_TIMER

DESCRIPTION
  Sets up a timer to track the request so that we can guarantee a response
  to the caller based on the specified timer value.

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean nvruim_setup_timer
(
  uint8                  *timer_index_ptr,
  nvruim_nv_context_type  nv_context
)
{
  uint8             index_timer = 0;
  boolean           found       = FALSE;
  rex_timer_type  * timer_ptr   = NULL;


  if(NVRUIM_CONTEXT_NOT_VALID(nv_context))
  {
    return FALSE;
  }

  if (timer_index_ptr == NULL)
  {
    return FALSE;
  }

  for (index_timer = 0; index_timer < NVRUIM_NUM_TIMERS ; index_timer++)
  {
    if (nvruim_timer_info[index_timer].req_state == NVRUIM_REQ_STATE_NONE)
    {
      found = TRUE;
      break;
    }
  }

  if (!found)
  {
    UIM_MSG_HIGH_0("NVRUIM: Cannot setup timer");
    return FALSE;
  }

  UIM_SELECTIVE_MSG_HIGH_1(UIM_LOG_NVRUIM, 
                           "NVRUIM: Setting up timer for index: 0x%x", index_timer);

  /*Setup Timer on the free index found above */
  timer_ptr = &nvruim_timer_info[index_timer].nvruim_req_timer;
  (void)rex_undef_timer(timer_ptr);

  rex_def_timer_ex(timer_ptr, nvruim_mmgsdi_req_timer_expiry, (uint32)index_timer);
  (void)rex_set_timer(timer_ptr, NVRUIM_MMGSDI_REQ_TIMER_VAL);

  /* Also update the status */
  *timer_index_ptr = index_timer;
  nvruim_update_timer_req_state(index_timer, NVRUIM_REQ_STATE_TIMER_CREATED);
  nvruim_timer_info[index_timer].req_context = nv_context;

  return TRUE;
} /* nvruim_setup_timer */


/*===========================================================================

FUNCTION                  NVRUIM_CLEAR_TIMER

DESCRIPTION
  Clears the timer based on the specifid index that was previously setup.

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean nvruim_clear_timer
(
  uint8       index_timer
)
{
  UIM_SELECTIVE_MSG_HIGH_1(UIM_LOG_NVRUIM, 
                           "NVRUIM: Clearing timer for index: 0x%x", index_timer);

  if (index_timer >= NVRUIM_NUM_TIMERS)
  {
    return FALSE;
  }

  /* Clear the Timer on the passed index */
  (void)rex_undef_timer(&nvruim_timer_info[index_timer].nvruim_req_timer);

  /* Also update the status */
  nvruim_update_timer_req_state(index_timer, NVRUIM_REQ_STATE_NONE);

  return TRUE;
} /* nvruim_clear_timer */


/*===========================================================================
FUNCTION NVRUIM_ACCESS_SYNCHRONOUS

DESCRIPTION
  This function invokes synchronous cache access APIs for Write(transparent
  files) and Read(record based /transparent files). For other operations it
  returns error.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type nvruim_access_synchronous(
  nvruim_cmd_type                 *cmd_ptr,
  mmgsdi_file_structure_enum_type  nvruim_mmgsdi_file_struct,
  const mmgsdi_access_type        *nvruim_mmgsdi_access_ptr,
  nvruim_nv_context_type           context
)
{
  mmgsdi_return_enum_type nvruim_mmgsdi_status = MMGSDI_ERROR;

  if(cmd_ptr == NULL || nvruim_mmgsdi_access_ptr == NULL ||
     NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return MMGSDI_ERROR;
  }

  /* Send the request to MMGSDI */
  if ((cmd_ptr->cmd_type == NVRUIM_READ) &&
      (nvruim_mmgsdi_access_ptr->access_method == MMGSDI_EF_ENUM_ACCESS) &&
      ((nvruim_mmgsdi_file_struct == MMGSDI_LINEAR_FIXED_FILE) ||
       (nvruim_mmgsdi_file_struct == MMGSDI_CYCLIC_FILE) ||
       (nvruim_mmgsdi_file_struct == MMGSDI_TRANSPARENT_FILE)))
  {
    nvruim_mmgsdi_status = MMGSDI_SUCCESS;

    /* If length is 0, it is expected that client has provided data buffer to
       copy full file data */
    if(cmd_ptr->data.data_len == 0)
    {
      nvruim_mmgsdi_status = mmgsdi_session_read_cache_file_size(
                               nvruim_session_data_ptr[context]->session_id,
                               nvruim_mmgsdi_access_ptr->file.file_enum,
                               &cmd_ptr->data.data_len);
    }

    if(nvruim_mmgsdi_status == MMGSDI_SUCCESS)
    {
      nvruim_mmgsdi_status = mmgsdi_session_read_cache_ext(
                               nvruim_session_data_ptr[context]->session_id,
                               nvruim_mmgsdi_access_ptr->file.file_enum,
                               &cmd_ptr->data,
                               (mmgsdi_offset_type)cmd_ptr->offset_or_recnum,
                               (mmgsdi_rec_num_type)cmd_ptr->offset_or_recnum);
    }
  }
  else if ((cmd_ptr->cmd_type == NVRUIM_WRITE) &&
           (nvruim_mmgsdi_access_ptr->access_method == MMGSDI_EF_ENUM_ACCESS) &&
           (cmd_ptr->offset_or_recnum == 0) &&
           (nvruim_mmgsdi_file_struct == MMGSDI_TRANSPARENT_FILE))
  {
    nvruim_mmgsdi_status = mmgsdi_session_write_cache(
                             nvruim_session_data_ptr[context]->session_id,
                             nvruim_mmgsdi_access_ptr->file.file_enum,
                             cmd_ptr->data);
  }
  else
  {
    nvruim_mmgsdi_status = MMGSDI_ERROR;
  }

  cmd_ptr->report_status = NVRUIM_FAIL;

  if(nvruim_mmgsdi_status == MMGSDI_ACCESS_DENIED)
  {
    /* In some cases, MMGSDI may first validate the access by reading the EF-ARR
       for the file and if MMGSDI finds out the access is denied, it sends
       MMGSDI_ACCESS_DENIED to its clients (without populating the status word).
       In that case, lets populate the NVRUIM sw1 and sw2 as 0x69 0x82 for error
       processing in NVRUIM */
    cmd_ptr->status_word.present = TRUE;
    cmd_ptr->status_word.sw1 = NVRUIM_SW1_CMD_NOT_ALLOWED;
    cmd_ptr->status_word.sw2 = NVRUIM_SW2_SECURITY_STATUS_NOT_SATISFIED;
  }
  else if (nvruim_mmgsdi_status == MMGSDI_SUCCESS)
  {
    cmd_ptr->report_status = NVRUIM_PASS;
  }
  return nvruim_mmgsdi_status;
}/* nvruim_access_synchronous */


/*===========================================================================
FUNCTION NVRUIM_ACCESS_ASYNCHRONOUS

DESCRIPTION
  This function sends the command to MMGSDI session APIs and waits for the
  response.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type nvruim_access_asynchronous(
  nvruim_cmd_type                 *cmd_ptr,
  mmgsdi_file_structure_enum_type  nvruim_mmgsdi_file_struct,
  const mmgsdi_access_type        *nvruim_mmgsdi_access_ptr,
  nvruim_nv_context_type           context
)
{
  mmgsdi_return_enum_type nvruim_mmgsdi_status = MMGSDI_ERROR;

  if(cmd_ptr == NULL || nvruim_mmgsdi_access_ptr == NULL ||
     NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return MMGSDI_ERROR;
  }

  /* Send the request to MMGSDI */
  if ((cmd_ptr->cmd_type == NVRUIM_READ) &&
      (nvruim_mmgsdi_file_struct == MMGSDI_TRANSPARENT_FILE))
  {
    nvruim_mmgsdi_status = mmgsdi_session_read_transparent(
                             nvruim_session_data_ptr[context]->session_id,
                             *nvruim_mmgsdi_access_ptr,
                             (mmgsdi_offset_type)cmd_ptr->offset_or_recnum,
                             cmd_ptr->data.data_len,
                             nvruim_mmgsdi_access_cb,
                             (mmgsdi_client_data_type)cmd_ptr);
  }
  else if ((cmd_ptr->cmd_type == NVRUIM_READ) &&
           ((nvruim_mmgsdi_file_struct == MMGSDI_LINEAR_FIXED_FILE) ||
            (nvruim_mmgsdi_file_struct == MMGSDI_CYCLIC_FILE)))
  {
    nvruim_mmgsdi_status = mmgsdi_session_read_record(
                             nvruim_session_data_ptr[context]->session_id,
                             *nvruim_mmgsdi_access_ptr,
                             (mmgsdi_rec_num_type)cmd_ptr->offset_or_recnum,
                             cmd_ptr->data.data_len,
                             nvruim_mmgsdi_access_cb,
                             (mmgsdi_client_data_type)cmd_ptr);
  }
  else if ((cmd_ptr->cmd_type == NVRUIM_WRITE) &&
           (nvruim_mmgsdi_file_struct == MMGSDI_TRANSPARENT_FILE))
  {
    nvruim_mmgsdi_status = mmgsdi_session_write_transparent(
                             nvruim_session_data_ptr[context]->session_id,
                             *nvruim_mmgsdi_access_ptr,
                             (mmgsdi_offset_type)cmd_ptr->offset_or_recnum,
                             cmd_ptr->data,
                             nvruim_mmgsdi_access_cb,
                             (mmgsdi_client_data_type)cmd_ptr);
  }
  else if ((cmd_ptr->cmd_type == NVRUIM_WRITE) &&
           ((nvruim_mmgsdi_file_struct == MMGSDI_LINEAR_FIXED_FILE) ||
            (nvruim_mmgsdi_file_struct == MMGSDI_CYCLIC_FILE)))
  {
    nvruim_mmgsdi_status = mmgsdi_session_write_record(
                             nvruim_session_data_ptr[context]->session_id,
                             *nvruim_mmgsdi_access_ptr,
                             nvruim_mmgsdi_file_struct,
                             (mmgsdi_rec_num_type)cmd_ptr->offset_or_recnum,
                             cmd_ptr->data,
                             nvruim_mmgsdi_access_cb,
                             (mmgsdi_client_data_type)cmd_ptr);
  }
  else if (cmd_ptr->cmd_type == NVRUIM_FILE_ATTR)
  {
    nvruim_mmgsdi_status = mmgsdi_session_get_file_attr(
                             nvruim_session_data_ptr[context]->session_id,
                             *nvruim_mmgsdi_access_ptr,
                             nvruim_mmgsdi_access_cb,
                             (mmgsdi_client_data_type)cmd_ptr);
  }
  else
  {
    nvruim_mmgsdi_status = MMGSDI_ERROR;
  }
  return nvruim_mmgsdi_status;
}/* nvruim_access_asynchronous */


/*===========================================================================

FUNCTION NVRUIM_ACCESS

DESCRIPTION
  This function sends the command to MMGSDI session APIs and waits for the
  response when task context is not MMGSDI. For MMGSDI task context, it invokes
  synchronous cache access APIs.

DEPENDENCIES
  Non-reentrant, only works for NAM=0

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void nvruim_access
(
  nvruim_cmd_type                 *cmd_ptr,
  uint8                            index_timer,
  nvruim_nv_context_type           context
)
{
  mmgsdi_file_structure_enum_type nvruim_mmgsdi_file_struct =
                                      MMGSDI_MAX_FILE_STRUCTURE_ENUM;
  mmgsdi_access_type              nvruim_mmgsdi_access_item;
  mmgsdi_return_enum_type         nvruim_mmgsdi_status = MMGSDI_ERROR;
  uint8                           slot_index           = NVRUIM_SLOT_INDEX_INVALID;
  boolean                         mmgsdi_task_context  = FALSE;

  if(cmd_ptr == NULL)
  {
    return;
  }

  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if (nvruim_session_data_ptr[context] == NULL || nvruim_generic_data_ptr == NULL)
  {
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  /* Initialize status word present flag */
  cmd_ptr->status_word.present = FALSE;

  /* Make sure it is a 1x file */
  if((((int)cmd_ptr->file_enum >> 8) != MMGSDI_RUIM_EF_START) &&
     (((int)cmd_ptr->file_enum >> 8) != MMGSDI_CSIM_EF_START))
  {
    /* Fail the command immediately */
    UIM_MSG_ERR_1("Access failed. 0x%x not a CDMA EF", cmd_ptr->file_enum);
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if (nvruim_convert_slot_id_to_index(nvruim_session_data_ptr[context]->mmgsdi_slot_id,
                                      &slot_index) == FALSE)
  {
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  /* Check if CDMA subscription is not present or not ready */
  if (nvruim_dir_present[slot_index] != NVRUIM_CDMA_SUBSCRIPTION_PRESENT  &&
      nvruim_dir_present[slot_index] != NVRUIM_CDMA_SUBSCRIPTION_READY)
  {
    /* Fail the command immediately */
    UIM_MSG_ERR_0("Access failed. CDMA subscription not present or not active");
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if (nvruim_session_data_ptr[context]->mmgsdi_session_active == FALSE)
  {
    UIM_MSG_ERR_0("Access failed. Session id is not valid");
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if ((cmd_ptr->cmd_type != NVRUIM_READ)&&
      (cmd_ptr->cmd_type != NVRUIM_WRITE)&&
      (cmd_ptr->cmd_type != NVRUIM_FILE_ATTR))
  {
    UIM_MSG_ERR_1("Access failed. 0x%x access type", cmd_ptr->cmd_type);
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if (nvruim_dir_present[slot_index] != NVRUIM_CDMA_SUBSCRIPTION_READY)
  {
    if (cmd_ptr->cmd_type != NVRUIM_FILE_ATTR &&
        (nvruim_mmgsdi_item_is_pin_protected(cmd_ptr->file_enum)))
    {
      /* Fail the command if the file is pin protected and sub is not ready */
      UIM_MSG_ERR_0("Access failed. CDMA subscription not ready");
      cmd_ptr->report_status = NVRUIM_FAIL;
      return;
    }
  }

  if (!nvruim_get_mmgsdi_item_and_file_struct(cmd_ptr->file_enum,
                                              &nvruim_mmgsdi_file_struct,
                                              &nvruim_mmgsdi_access_item,
                                              context))
  {
    UIM_MSG_ERR_1("Access failed. 0x%x EF not supported by NVRUIM",
                  cmd_ptr->file_enum);
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  /* If SIM is already busy, return an error to the caller */
  if ((slot_index < NVRUIM_MAX_CARD_COUNT) && (nvruim_sim_busy[slot_index] == TRUE))
  {
    UIM_MSG_ERR_1("Access failed since card is busy on slot_id: 0x%x",
                  nvruim_session_data_ptr[context]->mmgsdi_slot_id);
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if(nvruim_is_task_nvruim(cmd_ptr->cmd_hdr.task_ptr))
  {
    cmd_ptr->cmd_hdr.sigs = NVRUIM_MMGSDI_ACCESS_SIG;
  }

  /* Clear out the rex sig */
  (void) rex_clr_sigs(cmd_ptr->cmd_hdr.task_ptr, cmd_ptr->cmd_hdr.sigs);

  /* If the timer already expired by this time, clean up & return error */
  NVRUIM_ENTER_TIMER_CRIT_SECT;
  if (nvruim_get_timer_req_state(index_timer) == NVRUIM_REQ_STATE_TIMER_EXPIRED)
  {
    NVRUIM_LEAVE_TIMER_CRIT_SECT;
    cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  mmgsdi_task_context = nvruim_is_mmgsdi_task_context(cmd_ptr->cmd_hdr.task_ptr);

  if(mmgsdi_task_context)
  {
    nvruim_mmgsdi_status = nvruim_access_synchronous(cmd_ptr,
                                                     nvruim_mmgsdi_file_struct,
                                                     &nvruim_mmgsdi_access_item,
                                                     context);
  }
  else
  {
    nvruim_generic_data_ptr->current_commands_in_context[context]++;

    nvruim_mmgsdi_status = nvruim_access_asynchronous(cmd_ptr,
                                                      nvruim_mmgsdi_file_struct,
                                                      &nvruim_mmgsdi_access_item,
                                                      context);
    if (nvruim_mmgsdi_status == MMGSDI_SUCCESS)
    {
      /* Update the request status only on success */
      nvruim_update_timer_req_state(index_timer,
                                    NVRUIM_REQ_STATE_PENDING_MMGSDI_RSP);
    }
    else
    {
      nvruim_generic_data_ptr->current_commands_in_context[context]--;
    }
  }

  NVRUIM_LEAVE_TIMER_CRIT_SECT;

  if (nvruim_mmgsdi_status != MMGSDI_SUCCESS)
  {
    cmd_ptr->report_status           = NVRUIM_FAIL;
    return;
  }

  if(mmgsdi_task_context == FALSE)
  {
    /* If request came from NVRUIM task (as a result of async req) */
    if (nvruim_is_task_nvruim(cmd_ptr->cmd_hdr.task_ptr))
    {
      nvruim_wait_for_mmgsdi_access();
    } /* if (&nvruim_tcb == task_ptr) */
    else
    {
      /* For other tasks */
      (void) rex_wait(cmd_ptr->cmd_hdr.sigs);
    } /* if (&nvruim_tcb != task_ptr) */
  }
} /* nvruim_access */


/*===========================================================================

FUNCTION NVRUIM_WRITE_SW

DESCRIPTION
get the SW for Write Cmd

DEPENDENCIES

RETURN VALUE
None

SIDE EFFECTS
None

===========================================================================*/
static nv_ruim_support_status nvruim_write_sw
(
  nv_cmd_type                *nv_cmd_ptr,
  nv_stat_enum_type          *op_status,
  mmgsdi_sw_type              status_word,
  nvruim_rpt_status_enum_type report_status
)
{
  /* Even though these two pointers can never be NULL because there are
     checks at the top level access functions, as a good practise, lets
     make sure they are not NULL */
  if(!op_status || !nv_cmd_ptr)
  {
    if(op_status)
    {
      *op_status = NV_FAIL_S;
    }
    else if(nv_cmd_ptr)
    {
      nv_cmd_ptr->status = NV_FAIL_S;
    }
    return NV_RUIM_SUPPORTS_ITEM;
  }

  *op_status = NV_FAIL_S; /* Default status is "failed" */

  switch(report_status)
  {
    case NVRUIM_PASS:
      *op_status = NV_DONE_S;     /* Request completed okay     */
      break;

    case NVRUIM_FAIL:
      if(status_word.present &&
         (status_word.sw1 == NVRUIM_SW1_CMD_NOT_ALLOWED) &&
         (status_word.sw2 == NVRUIM_SW2_SECURITY_STATUS_NOT_SATISFIED))
      {
        /* Security conditions not satisfied... lets return NV_READONLY_S
           instead of returning NV_FAIL_S. Here is why we want to return a
           NV_READONLY_S in this case and not NV_FAIL_S:
           If this WRITE request came in as part of QCN back up (that involves
           writing to an EF on the card for some reason), unless NVRUIM returns
           NV_DONE_S, NV_READONLY_S or NV_BADPARM_S, tools will fail QCN backup
           procedure. In order to keep tools happy and continue with QCN backup
           procedure for a failure like this one, returning NV_READONLY_S as that
           that seems to be the most appropriate code out of the three codes
           accepted by tools as mentioned above.
           NOTE that for files that are pin-protected, if a READ/WRITE comes to
           NVRUIM before SUB_READY, NVRUIM does not even send the request to
           MMGSDI and so sw1, sw2 in that case are 00, 00... so, we wont enter
           this condition in that case and NVRUIM will continue to set the status
           to NV_FAIL_S as before for that case */
        *op_status = NV_READONLY_S;
      }
      break;
  }
  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_write_sw */


/*===========================================================================

FUNCTION NVRUIM_WRITE_ITEM

DESCRIPTION
Write to Given item

DEPENDENCIES


RETURN VALUE
 none

SIDE EFFECTS
  none
===========================================================================*/
static void nvruim_write_item
(
  byte                    *data_ptr,
  word                    data_len,
  word                    offset,
  mmgsdi_file_enum_type   item,
  uint8                   timer_index,
  nvruim_nv_context_type  context,
  nvruim_cmd_type        *nvruim_cmd_ptr
)
{
  if(nvruim_cmd_ptr == NULL)
  {
    return;
  }

  nvruim_cmd_ptr->data.data_ptr    = data_ptr;
  nvruim_cmd_ptr->data.data_len    = data_len;
  nvruim_cmd_ptr->offset_or_recnum = offset;
  nvruim_cmd_ptr->file_enum        = item;
  nvruim_cmd_ptr->cmd_type         = NVRUIM_WRITE;
  nvruim_access(nvruim_cmd_ptr, timer_index, context);
} /* nvruim_write_item */


/*===========================================================================

FUNCTION NVRUIM_WRITE_REC_ITEM

DESCRIPTION
Write to Given record of the given item

DEPENDENCIES


RETURN VALUE
 none

SIDE EFFECTS
  none
===========================================================================*/
static void nvruim_write_rec_item
(
  byte                   *data_ptr,
  word                    data_len,
  word                    offset,
  mmgsdi_file_enum_type   item,
  uint8                   timer_index,
  nvruim_nv_context_type  context,
  nvruim_cmd_type        *nvruim_cmd_ptr
)
{
  if(nvruim_cmd_ptr == NULL)
  {
    return;
  }

  nvruim_cmd_ptr->data.data_ptr    = data_ptr;
  nvruim_cmd_ptr->data.data_len    = data_len;
  nvruim_cmd_ptr->offset_or_recnum = offset;
  nvruim_cmd_ptr->file_enum        = item;
  nvruim_cmd_ptr->cmd_type         = NVRUIM_WRITE;

  /* rec_mode is always ABSOLUTE here */
  nvruim_access(nvruim_cmd_ptr, timer_index, context);
} /* nvruim_write_rec_item */


/*===========================================================================

FUNCTION NVRUIM_READ_ITEM

DESCRIPTION
Read the  Given item

DEPENDENCIES


RETURN VALUE
 none

SIDE EFFECTS
  none
===========================================================================*/
static void nvruim_read_item
(
  byte                   *data_ptr,
  word                    data_len,
  word                    offset,
  mmgsdi_file_enum_type   item,
  uint8                   timer_index,
  nvruim_nv_context_type  context,
  nvruim_cmd_type        *nvruim_cmd_ptr
)
{
  if(nvruim_cmd_ptr == NULL)
  {
   return;
  }

  nvruim_cmd_ptr->data.data_ptr    = data_ptr;
  nvruim_cmd_ptr->data.data_len    = data_len;
  nvruim_cmd_ptr->offset_or_recnum = offset;
  nvruim_cmd_ptr->file_enum        = item;
  nvruim_cmd_ptr->cmd_type         = NVRUIM_READ;
  nvruim_access(nvruim_cmd_ptr, timer_index, context);
} /* nvruim_read_item */


/*===========================================================================

FUNCTION NVRUIM_SEARCH_FILE

DESCRIPTION
Searches for a pattern in a record based file. This function only runs
search for a 3G (CSIM) EF and not for a 2G (RUIM) EF.

DEPENDENCIES
 none

RETURN VALUE
 none

SIDE EFFECTS
  none
===========================================================================*/
static void nvruim_search_file
(
  nvruim_cmd_type                   *cmd_ptr,
  mmgsdi_rec_num_type               rec_num,
  mmgsdi_search_data_type           srch_pattern,
  uint8                             index_timer,
  nvruim_nv_context_type            context,
  nvruim_cmd_type                  *nvruim_cmd_ptr
)
{
  mmgsdi_file_structure_enum_type   nvruim_mmgsdi_file_struct;
  mmgsdi_access_type                nvruim_mmgsdi_access;
  mmgsdi_return_enum_type           nvruim_mmgsdi_status = MMGSDI_ERROR;
  mmgsdi_search_offset_data_type    srch_offset;
  uint8                             slot_index     = NVRUIM_SLOT_INDEX_INVALID;

  if(nvruim_cmd_ptr == NULL)
  {
    return;
  }

  memset(&srch_offset, 0x00, sizeof(srch_offset));

  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL_2(nvruim_session_data_ptr[context], nvruim_generic_data_ptr);

  if(nvruim_is_mmgsdi_task_context(cmd_ptr->cmd_hdr.task_ptr))
  {
    nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if(!nvruim_get_mmgsdi_item_and_file_struct(cmd_ptr->file_enum,
                                             &nvruim_mmgsdi_file_struct,
                                             &nvruim_mmgsdi_access,
                                             context))
  {
    UIM_MSG_ERR_1("Access failed.%d EF not supported by NVRUIM", cmd_ptr->file_enum);
    nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if(nvruim_mmgsdi_file_struct != MMGSDI_LINEAR_FIXED_FILE &&
     nvruim_mmgsdi_file_struct != MMGSDI_CYCLIC_FILE)
  {
    UIM_MSG_ERR_1("Search attempted on a wrong file type: 0x%x",
                  nvruim_mmgsdi_file_struct);
    nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  /* Check if non-CSIM EF is being accessed. SEARCH is not supported
     for RUIM EFs so we should allow queueing SEARCH requests only
     for CSIM EFs */
  if(nvruim_mmgsdi_access.file.file_enum < MMGSDI_CSIM_ARR ||
     nvruim_mmgsdi_access.file.file_enum > MMGSDI_CSIM_PUID)
  {
    /* Fail the command immediately */
    UIM_MSG_ERR_1("Access failed.%d not a CSIM EF", cmd_ptr->file_enum);
     nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if (nvruim_convert_slot_id_to_index(nvruim_session_data_ptr[context]->mmgsdi_slot_id,
                                      &slot_index) == FALSE)
  {
    return;
  }

  /* Check if CDMA subscription is not present or not ready */
  if (nvruim_dir_present[slot_index] != NVRUIM_CDMA_SUBSCRIPTION_PRESENT  &&
      nvruim_dir_present[slot_index] != NVRUIM_CDMA_SUBSCRIPTION_READY)
  {
    /* Fail the command immediately */
    UIM_MSG_ERR_0("Access failed. CDMA subscription not present or not active");
    nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  if(nvruim_dir_present[slot_index] != NVRUIM_CDMA_SUBSCRIPTION_READY)
  {
    if(nvruim_mmgsdi_item_is_pin_protected(cmd_ptr->file_enum))
    {
      /* Fail the command if the file is pin protected and sub is not ready */
      UIM_MSG_ERR_0("Access failed. CDMA subscription not ready");
      nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
      return;
    }
  }

  if(nvruim_session_data_ptr[context]->mmgsdi_session_active == FALSE)
  {
    UIM_MSG_ERR_0("Access failed. Session id is not valid");
    nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  /* Populate the sig if the request is in NVRUIM task context. If the
     request is a sync request (in client's context), the assumption per
     the legacy code is that the client must provide a sig and we would
     use their sig in that case */
  if(nvruim_is_task_nvruim(cmd_ptr->cmd_hdr.task_ptr))
  {
    cmd_ptr->cmd_hdr.sigs = NVRUIM_MMGSDI_ACCESS_SIG;
  }

  /* Clear out the rex sig */
  (void) rex_clr_sigs(cmd_ptr->cmd_hdr.task_ptr, cmd_ptr->cmd_hdr.sigs);

  /* If the timer already expired by this time, clean up & return error */
  NVRUIM_ENTER_TIMER_CRIT_SECT;
  if (nvruim_get_timer_req_state(index_timer) == NVRUIM_REQ_STATE_TIMER_EXPIRED)
  {
    NVRUIM_LEAVE_TIMER_CRIT_SECT;
    nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    return;
  }

  nvruim_generic_data_ptr->current_commands_in_context[context]++;

  nvruim_mmgsdi_status = mmgsdi_session_search(
                           nvruim_session_data_ptr[context]->session_id,
                           nvruim_mmgsdi_access,
                           MMGSDI_UICC_SIMPLE_SEARCH,
                           rec_num,
                           MMGSDI_SEARCH_FORWARD_FROM_REC_NUM,
                           srch_offset,
                           srch_pattern,
                           nvruim_mmgsdi_access_cb,
                           (mmgsdi_client_data_type)nvruim_cmd_ptr);

  if (nvruim_mmgsdi_status == MMGSDI_SUCCESS)
  {
    /* Update the request status only on success */
    nvruim_update_timer_req_state(index_timer,
                                  NVRUIM_REQ_STATE_PENDING_MMGSDI_RSP);
  }

  NVRUIM_LEAVE_TIMER_CRIT_SECT;

  if(nvruim_mmgsdi_status != MMGSDI_SUCCESS)
  {
    nvruim_cmd_ptr->report_status           = NVRUIM_FAIL;
    nvruim_generic_data_ptr->current_commands_in_context[context]--;
    return;
  }

  /* If request came from NVRUIM task (as a result of async req) */
  if(nvruim_is_task_nvruim(cmd_ptr->cmd_hdr.task_ptr))
  {
    nvruim_wait_for_mmgsdi_access();
  }
  else
  {
    /* For other tasks */
    (void) rex_wait(cmd_ptr->cmd_hdr.sigs);
  }
} /* nvruim_search_file */


/*===========================================================================

FUNCTION NVRUIM_WRITE_SCI

DESCRIPTION
Write SCI info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_sci
(
  nv_cmd_type            *nv_cmd_ptr,
  nv_stat_enum_type      *op_status,
  uint8                   timer_index,
  nvruim_nv_context_type  context,
  nvruim_cmd_type        *nvruim_cmd_ptr
)
{
  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_write_item((byte *) &(nv_cmd_ptr->data_ptr->slot_cycle_index),
                     1,
                     0,
                     MMGSDI_CDMA_SCI,
                     timer_index,
                     context,
                     nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_sci */


/*===========================================================================

FUNCTION NVRUIM_WRITE_SID_NID

DESCRIPTION
Write IMSI_T info to card

DEPENDENCIES

RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_sid_nid
(
  nv_cmd_type            *nv_cmd_ptr,
  nv_stat_enum_type      *op_status,
  uint8                   timer_index,
  nvruim_nv_context_type  context,
  nvruim_cmd_type        *nvruim_cmd_ptr
)
{
  uint8               i = 0;
  nvruim_sid_nid_type sid_nid_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&sid_nid_buf, 0x00, sizeof(nvruim_sid_nid_type));

  switch(nv_cmd_ptr->item)
  {
    case NV_SID_NID_I:
      /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->sid_nid.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }

      if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
      {
        return NV_RUIM_SUPPORTS_ITEM;
      }
      sid_nid_buf.sid =
         nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][0].sid;
      sid_nid_buf.nid =
        nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][0].nid;
    break;

    case NV_HOME_SID_NID_I:
     /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->home_sid_nid.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }

      if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
      {
        return NV_RUIM_SUPPORTS_ITEM;
      }
      sid_nid_buf.sid = nv_cmd_ptr->data_ptr->home_sid_nid.pair[0].sid;
      sid_nid_buf.nid = nv_cmd_ptr->data_ptr->home_sid_nid.pair[0].nid;
      break;

    default:
      return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nvruim_write_rec_item((byte *) &sid_nid_buf,
                        SID_NID_REC_SIZE,
                        1,
                        MMGSDI_CDMA_HOME_SID_NID,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);


  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    switch(nv_cmd_ptr->item)
    {
      case NV_SID_NID_I:
        /* Write the rest of records. Since we don't know how many records
        total, we just write until it fails */

        for(i = 1;
        (i < NV_MAX_SID_NID) && (sid_nid_buf.sid != 0); i++)
        {
          sid_nid_buf.sid =
            nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][i].sid;
          sid_nid_buf.nid =
            nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][i].nid;

          nvruim_write_rec_item((byte *) &sid_nid_buf,
                                SID_NID_REC_SIZE,
                                i+1,
                                MMGSDI_CDMA_HOME_SID_NID,
                                timer_index,
                                context,
                                nvruim_cmd_ptr);

          if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
          {
            /* Report NVRUIM_PASS since at least the first write is good */
            nvruim_cmd_ptr->report_status = NVRUIM_PASS;
            break;
          } /* if */
        } /* for */
        break;

      case NV_HOME_SID_NID_I:
        /* Write the rest of records. Since we don't know how many records
        total, we just write until it fails */
        for(i = 1;
        (i < NV_MAX_HOME_SID_NID) && (sid_nid_buf.sid != 0); i++)
        {

          sid_nid_buf.sid =
            nv_cmd_ptr->data_ptr->home_sid_nid.pair[i].sid;
          sid_nid_buf.nid =
            nv_cmd_ptr->data_ptr->home_sid_nid.pair[i].nid;

          nvruim_write_rec_item((byte *) &sid_nid_buf,
                                SID_NID_REC_SIZE,
                                i+1,
                                MMGSDI_CDMA_HOME_SID_NID,
                                timer_index,
                                context,
                                nvruim_cmd_ptr);

          if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
          {
            /* Report NVRUIM_PASS since at least the first write is good */
            nvruim_cmd_ptr->report_status = NVRUIM_PASS;
            break;
          } /* if */
        } /* for */
        break;
      default:
        break;
    }
  } /* if */
  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_sid_nid */


/*===========================================================================

FUNCTION NVRUIM_WRITE_MSISDN

DESCRIPTION
Write MSISDN info to card

DEPENDENCIES

RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_msisdn
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  /* Holds DIR NUM digit as its being processed */
  uint8   digit                             = 0;
  uint8   i                                 = 0;
  uint8   msisdn_buffer[NVRUIM_MSISDN_SIZE] = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->dir_number.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Initialize the MSISDN buffer to all F's */
  for(i = NVRUIM_MSISDN_MDN_INDEX; i < NVRUIM_MSISDN_SIZE; i++)
  {
    msisdn_buffer[i] = 0xFF;
  }

  /* Copy the DIR NUMBER to the MSISDN buffer.
  *
  * Convert the digits as they are copied.  */
  for(i=0; (i < NV_DIR_NUMB_SIZ); i++)
  {
    /* Convert the digit to the MSISDN format */
    digit = nv_cmd_ptr->data_ptr->dir_number.dir_number[i];

    /* Convert '0' to 0x0A. */
    if(digit == '0')
    {
      digit = 0xA;
    }
    /* Convert '*' to 0x0B. */
    else if(digit == '*')
    {
      digit = 0xB;
    }
    /* Convert '#' to 0x0C. */
    else if(digit == '#')
    {
      digit = 0xC;
    }
    /* Convert digits 1-9 to BCD. */
    else if((digit > '0') && (digit <= '9'))
    {
      digit = digit - '0';
    }
    /* If there are less than 10 digits, the buffer will contain an invalid
    value (e.g. zero).  This is used to determine the end of the DIR
    NUMBER string. */
    else
    {
      break;
    }

    /* Place the digit in the byte - based on whether the index is
    even or odd */
    if(i & 0x1)
    {
      /* Fill the MSNibble in the proper byte of the MSISDN */
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] &= 0x0F;
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] |=
        ((digit & 0x0F) << 4);
    }
    else
    { /* The digit index is even */
      /* Fill the LSnibble in the proper byte of the MSISDN */
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] &= 0xF0;
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] |=
        (digit & 0x0F);
    } /* end if - fill the buffer based on whether i is even or odd */
  } /* end for - copy the digits to the MSISDN bufer */

  /* This is the number of BCD digits that are programmed */
  msisdn_buffer[ NVRUIM_MSISDN_ND_INDEX ] = i;

  nvruim_write_rec_item(msisdn_buffer,
                        sizeof ( msisdn_buffer ),
                        1,
                        MMGSDI_CDMA_MSISDN,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_msisdn */


/*===========================================================================

FUNCTION NVRUIM_WRITE_DIR_NUMBER_PCS

DESCRIPTION
Write MSISDN info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_dir_number_pcs
(
  nv_cmd_type            *nv_cmd_ptr,
  nv_stat_enum_type      *op_status,
  uint8                   timer_index,
  nvruim_nv_context_type  context,
  nvruim_cmd_type        *nvruim_cmd_ptr
)
{
  uint8 i                                 = 0;
  uint8 msisdn_buffer[NVRUIM_MSISDN_SIZE] = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->mob_dir_number.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  /* Initialize the MSISDN buffer to all F's */
  for(i = NVRUIM_MSISDN_MDN_INDEX; i < NVRUIM_MSISDN_SIZE; i++)
  {
    msisdn_buffer[i] = 0xFF;
  }

  /* Copy the DIR NUMBER to the MSISDN */

  /* This is the number of BCD digits to program into the MSISDN */
  msisdn_buffer[ NVRUIM_MSISDN_ND_INDEX ] =
  nv_cmd_ptr->data_ptr->mob_dir_number.n_digits & 0x0F;

  /* Copy the digits from the MSISDN to the DIR NUM buffer. */
  for(i = 0; (i < (nv_cmd_ptr->data_ptr->mob_dir_number.n_digits & 0x0F)); i++)
  {
    /* Retrieve the digit from the byte based on whether the index is
    even or odd */
    if(i & 0x1)
    {
      /* Fill the MSNibble in the proper byte of the MSISDN */
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] &= 0x0F;
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] |=
      (byte) (nv_cmd_ptr->data_ptr->mob_dir_number.digitn[i] << 4);
    }
    else
    { /* The digit index is even */
      /* Fill the LSnibble in the proper byte of the MSISDN */
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] &= 0xF0;
      msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] |=
      nv_cmd_ptr->data_ptr->mob_dir_number.digitn[i] & 0x0F;
    } /* end if - index is odd */

  } /* end for - copy all the digits to the DIR NUM buffer */
  nvruim_write_rec_item(msisdn_buffer,
                      sizeof ( msisdn_buffer ),
                      1,
                      MMGSDI_CDMA_MSISDN,
                      timer_index,
                      context,
                      nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_dir_number_pcs */


/*===========================================================================

FUNCTION NVRUIM_WRITE_COUNT

DESCRIPTION
Write COUNT info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_count
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8 msisdn_buffer[NVRUIM_MSISDN_SIZE] = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Use the MS-ISDN buffer to send the 2 bytes. NV gets only one byte in
     the command */
  msisdn_buffer[0]        = 0;
  msisdn_buffer[1]        = nv_cmd_ptr->data_ptr->count.count;

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_write_rec_item(msisdn_buffer,
                        NVRUIM_COUNT_REC_LEN,
                        0,
                        MMGSDI_CDMA_CC,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_count */


/*===========================================================================

FUNCTION NVRUIM_WRITE_ANALOG_FIRSTCHIP

DESCRIPTION
Write Analog Chan Pref Info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status
===========================================================================*/
static nv_ruim_support_status nvruim_write_analog_firstchip(
  nv_cmd_type              *nv_cmd_ptr,
  nv_stat_enum_type        *op_status,
  uint8                     timer_index,
  nvruim_nv_context_type    context,
  nvruim_cmd_type          *nvruim_cmd_ptr
)
{
  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->analog_firstchp.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_write_item((byte *) &(nv_cmd_ptr->data_ptr->analog_firstchp.channel),
                     NVRUIM_SIZE(analog_firstchp.channel),
                     (word) FPOS(nvruim_analog_ch_pref, analog_firstchp),
                     MMGSDI_CDMA_ANALOG_CHAN_PREF,
                     timer_index,
                     context,
                     nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_analog_firstchip */


/*===========================================================================
FUNCTION NVRUIM_WRITE_HOME_SID

DESCRIPTION
Write HOME_SID info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_home_sid
(
  nv_cmd_type               *nv_cmd_ptr,
  nv_stat_enum_type         *op_status,
  uint8                      timer_index,
  nvruim_nv_context_type     context,
  nvruim_cmd_type           *nvruim_cmd_ptr
)
{
  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->analog_home_sid.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_write_item((byte *) &(nv_cmd_ptr->data_ptr->analog_home_sid.sid),
                     NVRUIM_SIZE(analog_home_sid.sid),
                     0,
                     MMGSDI_CDMA_ANALOG_HOME_SID,
                     timer_index,
                     context,
                     nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_home_sid */


/*===========================================================================
FUNCTION NVRUIM_WRITE_MOB_TERM

DESCRIPTION
Write MOB Term info to the card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_mob_term
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8    nvruim_call_term_mode_pref_buf = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* MOB_TERM_HOME, MOB_TERM_FOR_SID, and MOB_TERM_FOR_NID are in
     the same byte. First read the byte and modify the bit */

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_read_item(&nvruim_call_term_mode_pref_buf,
                     1,
                     0,
                     MMGSDI_CDMA_CALL_TERM_MODE_PREF,
                     timer_index,
                     context,
                     nvruim_cmd_ptr);

  switch(nv_cmd_ptr->item)
  {
    case NV_MOB_TERM_HOME_I:
        /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->mob_term_home.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
      /* Check if the read succeeded. */
      if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
      {
        /* Reset MOB_TERM_HOME bit (bit 2) */
        nvruim_call_term_mode_pref_buf &= 0xFB;

        if(nv_cmd_ptr->data_ptr->mob_term_home.enabled[NV_CDMA_MIN_INDEX])
        {
          /*  MOB_TERM_HOME bit (bit 2) */
          nvruim_call_term_mode_pref_buf |= 0x04;
        }
      }
      break;

    case NV_MOB_TERM_FOR_SID_I:
      /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->mob_term_for_sid.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
      /* Check if the read succeeded. */
      if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
      {
        /* Reset MOB_TERM_FOR_SID bit (bit 1) */
        nvruim_call_term_mode_pref_buf &= 0xFD;

        if(nv_cmd_ptr->data_ptr->mob_term_for_sid.enabled[NV_CDMA_MIN_INDEX])
        {
          /*  MOB_TERM_FOR_SID bit (bit 1) */
          nvruim_call_term_mode_pref_buf |= 0x02;
        }
      }
      break;

    case NV_MOB_TERM_FOR_NID_I:
      /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->mob_term_for_nid.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
      /* Check if the read succeeded. */
      if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
      {
        /* Reset MOB_TERM_FOR_SID bit (bit 0) */
        nvruim_call_term_mode_pref_buf &= 0xFE;

        if(nv_cmd_ptr->data_ptr->mob_term_for_nid.enabled[NV_CDMA_MIN_INDEX])
        {
          /*  MOB_TERM_FOR_SID bit (bit 0) */
          nvruim_call_term_mode_pref_buf |= 0x01;
        }
      }
      break;
    default:
      return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    nvruim_write_item(&nvruim_call_term_mode_pref_buf,
                       1,
                       0,
                       MMGSDI_CDMA_CALL_TERM_MODE_PREF,
                       timer_index,
                       context,
                       nvruim_cmd_ptr);
  }
  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_mob_term */


/*===========================================================================
FUNCTION NVRUIM_WRITE_SPC_ENABLE

DESCRIPTION
Write SPC ENABLE info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status
===========================================================================*/
static nv_ruim_support_status nvruim_write_spc_enable
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8 nvruim_otapa_spc_enable_buf = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_read_item(&nvruim_otapa_spc_enable_buf,
                   1,
                   0,
                   MMGSDI_CDMA_OTAPA_SPC_ENABLE,
                   timer_index,
                   context,
                   nvruim_cmd_ptr);

  /* Check if the read succeeded. */
  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    switch(nv_cmd_ptr->item)
    {
      case NV_OTAPA_ENABLED_I:
        /* Only process the NAM parameters for the RUIM
        This check assumes the first byte of the data contains the NAM */
        if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->otapa_enabled.nam)
        {
          /* Go to NV for this item */
          return NV_RUIM_ITEM_NOT_SUPPORTED;
        }

        /* Reset the otapa enable bit which indicates that OTAPA is enabled */
        nvruim_otapa_spc_enable_buf &= NVRUIM_OTAPA_ENABLE_MASK;

        /* A value of 0 indicates that otapa is enabled and '1' indicates
        that otapa is disabled */
        if(!nv_cmd_ptr->data_ptr->otapa_enabled.enabled)
        {
          /*  OTAPA_Enable bit (bit 0) */
          nvruim_otapa_spc_enable_buf |= NVRUIM_OTAPA_DISABLE;
        }
        break;

      case NV_SPC_CHANGE_ENABLED_I:
        /* Reset SPC_Change_Enable bit (bit 4) */
        nvruim_otapa_spc_enable_buf &= NVRUIM_SPC_ENABLE_MASK;

        /* A value of 0 indicates that SPC is enabled and '1' indicates
        that SPC is disabled */
        if(!nv_cmd_ptr->data_ptr->spc_change_enabled)
        {
          nvruim_otapa_spc_enable_buf |= NVRUIM_SPC_DISABLE;
        }
        break;
      default:
        return NV_RUIM_ITEM_NOT_SUPPORTED;
    }

    nvruim_write_item(&nvruim_otapa_spc_enable_buf,
                      1,
                      0,
                      MMGSDI_CDMA_OTAPA_SPC_ENABLE,
                      timer_index,
                      context,
                      nvruim_cmd_ptr);
  }
  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_spc_enable */


/*===========================================================================
FUNCTION NVRUIM_WRITE_NAM_LOCK

DESCRIPTION
Write NAM LOCK info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_nam_lock
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8              nvruim_nam_lock_buf = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
     This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->nam_lock.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_read_item(&nvruim_nam_lock_buf,
                   1,
                   0,
                   MMGSDI_CDMA_NAM_LOCK,
                   timer_index,
                   context,
                   nvruim_cmd_ptr);

  /* Check if the read succeeded. */
  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    /* Reset NAM_LOCK bit (bit 2) */
    nvruim_nam_lock_buf &= NVRUIM_NAM_LOCK_MASK;

    if(nv_cmd_ptr->data_ptr->nam_lock.enabled)
    {
      nvruim_nam_lock_buf |= NVRUIM_NAM_LOCK;
    }

    nvruim_write_item(&nvruim_nam_lock_buf,
                       1,
                       0,
                       MMGSDI_CDMA_NAM_LOCK,
                       timer_index,
                       context,
                       nvruim_cmd_ptr);
  }
  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_nam_lock */


/*===========================================================================
FUNCTION NVRUIM_WRITE_PREF

DESCRIPTION
Write MODE PREF, SYS PREF, CDMA PREf, ANALOG PREF  info to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_pref
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8   temp_service_pref_buf = 0x00;
  uint8   nvruim_svc_pref_buf   = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_read_item(&nvruim_svc_pref_buf,
                   sizeof(nvruim_svc_pref_buf),
                   0,
                   MMGSDI_CDMA_SERVICE_PREF,
                   timer_index,
                   context,
                   nvruim_cmd_ptr);

  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    switch(nv_cmd_ptr->item)
    {
      case NV_PREF_MODE_I:
        switch(nv_cmd_ptr->data_ptr->pref_mode.mode)
        {
          /* The IS-820 doesn't match the NV structure.
          Find closest matches here*/
          case NV_MODE_ANALOG_PREF:
            temp_service_pref_buf = NVRUIM_ANALOG_PREF;
            break;
          case NV_MODE_AUTOMATIC:
            temp_service_pref_buf = NVRUIM_CDMA_PREF;
            break;
          case NV_MODE_ANALOG_ONLY:
            temp_service_pref_buf = NVRUIM_ANALOG_ONLY;
            break;
          case NV_MODE_DIGITAL_ONLY:
          case NV_MODE_PCS_CDMA_ONLY:
          case NV_MODE_CELL_CDMA_ONLY:
            temp_service_pref_buf = NVRUIM_CDMA_ONLY;
            break;
          default: /* Set to NV_MODE_AUTOMATIC */
            temp_service_pref_buf = NVRUIM_NO_PREF;
            break;
        }
        nvruim_svc_pref_buf &= ~NVRUIM_PREF_MODE_MASK;
        nvruim_svc_pref_buf |= temp_service_pref_buf;
        break;

      case NV_SYSTEM_PREF_I:
        switch(nv_cmd_ptr->data_ptr->system_pref.sys)
        {
          /* The IS-820 doesn't match the NV structure.
          Find closest matches here*/
          case NV_SP_A_ONLY:
            temp_service_pref_buf = NVRUIM_A_ONLY;
            break;
          case NV_SP_B_ONLY:
            temp_service_pref_buf = NVRUIM_B_ONLY;
            break;
          default: /* Set to no preference */
            temp_service_pref_buf = NVRUIM_NO_PREF;
            break;
        }

        nvruim_svc_pref_buf &= ~NVRUIM_SYSTEM_PREF_MASK;
        nvruim_svc_pref_buf |= temp_service_pref_buf;
        break;

      default:
        return NV_RUIM_ITEM_NOT_SUPPORTED;
    }

    nvruim_write_item(&nvruim_svc_pref_buf,
                      sizeof(nvruim_svc_pref_buf),
                      0,
                      MMGSDI_CDMA_SERVICE_PREF,
                      timer_index,
                      context,
                      nvruim_cmd_ptr);

  }
  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_pref */


/*===========================================================================
FUNCTION NVRUIM_WRITE_TMSI

DESCRIPTION
Write TMSI INFO to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_tmsi
(
  nv_cmd_type               *nv_cmd_ptr,
  nv_stat_enum_type         *op_status,
  uint8                      timer_index,
  nvruim_nv_context_type     context,
  nvruim_cmd_type           *nvruim_cmd_ptr
)
{
  uint8               i = 0;
  nvruim_tmsi_type    nvruim_tmsi_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_tmsi_buf, 0x00, sizeof(nvruim_tmsi_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  switch(nv_cmd_ptr->item)
  {
    case NV_ASSIGNING_TMSI_ZONE_LEN_I:
      /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->assigning_tmsi_zone_length.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
      nvruim_tmsi_buf.length =
        nv_cmd_ptr->data_ptr->assigning_tmsi_zone_length.length;

      nvruim_write_item((byte *) &(nvruim_tmsi_buf.length),
                       1,
                       0,
                       MMGSDI_CDMA_TMSI,
                       timer_index,
                       context,
                       nvruim_cmd_ptr);
      break;

    case NV_ASSIGNING_TMSI_ZONE_I:
      /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->assigning_tmsi_zone.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }

      /* Copy qword */
      for(i=0; i<8; i++)
      {
        nvruim_tmsi_buf.zone[i] =
        QW_BYTE(nv_cmd_ptr->data_ptr->assigning_tmsi_zone.zone ,i);
      }

      nvruim_write_item((byte *)&nvruim_tmsi_buf.zone[0],
                         8,
                         1,
                         MMGSDI_CDMA_TMSI,
                         timer_index,
                         context,
                         nvruim_cmd_ptr);
      break;

    case NV_TMSI_CODE_I:
      /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->tmsi_code.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
      nvruim_tmsi_buf.code[3] =
        (byte) (nv_cmd_ptr->data_ptr->tmsi_code.code >> 24);
      nvruim_tmsi_buf.code[2] =
        (byte) (nv_cmd_ptr->data_ptr->tmsi_code.code >> 16);
      nvruim_tmsi_buf.code[1] =
        (byte) (nv_cmd_ptr->data_ptr->tmsi_code.code >> 8);
      nvruim_tmsi_buf.code[0] =
        (byte) (nv_cmd_ptr->data_ptr->tmsi_code.code);

      nvruim_write_item((byte *)&nvruim_tmsi_buf.code[0],
                        4,
                        9,
                        MMGSDI_CDMA_TMSI,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);

      break;

    case NV_TMSI_EXP_I:
      /* Only process the NAM parameters for the RUIM
      This check assumes the first byte of the data contains the NAM */
      if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->tmsi_exp_timer.nam)
      {
        /* Go to NV for this item */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
      nvruim_tmsi_buf.time[2] =
        (byte) (nv_cmd_ptr->data_ptr->tmsi_exp_timer.time >> 16);
      nvruim_tmsi_buf.time[1] =
        (byte) (nv_cmd_ptr->data_ptr->tmsi_exp_timer.time >> 8);
      nvruim_tmsi_buf.time[0] =
        (byte) (nv_cmd_ptr->data_ptr->tmsi_exp_timer.time);

      nvruim_write_item((byte *) &nvruim_tmsi_buf.time[0],
                       3,
                       13,
                       MMGSDI_CDMA_TMSI,
                       timer_index,
                       context,
                       nvruim_cmd_ptr);

      break;

    default:
      return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_tmsi */


/*===========================================================================
FUNCTION NVRUIM_WRITE_LOCN_REGN_IND

DESCRIPTION
Write LOCN REGN IND to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_locn_regn_ind
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nvruim_analog_reg_ind_type nvruim_analog_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_analog_reg_ind_buf, 0x00, sizeof(nvruim_analog_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  switch(nv_cmd_ptr->item)
  {
    case NV_NXTREG_I:
      nvruim_analog_reg_ind_buf.nxtreg[2] =
        (byte) (nv_cmd_ptr->data_ptr->nxtreg >> 16);
      nvruim_analog_reg_ind_buf.nxtreg[1] =
        (byte) (nv_cmd_ptr->data_ptr->nxtreg >> 8);
      nvruim_analog_reg_ind_buf.nxtreg[0] =
        (byte) (nv_cmd_ptr->data_ptr->nxtreg);

      nvruim_write_item((byte *) nvruim_analog_reg_ind_buf.nxtreg,
                        3,
                        0,
                        MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);
      break;

    case NV_LSTSID_I:
      nvruim_analog_reg_ind_buf.sid[1] =
        (byte) (nv_cmd_ptr->data_ptr->lstsid >> 8);
      nvruim_analog_reg_ind_buf.sid[0] =
         (byte) (nv_cmd_ptr->data_ptr->lstsid);

      nvruim_write_item((byte *) &nvruim_analog_reg_ind_buf.sid[0],
                        2,
                        3,
                        MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);

      break;

    case NV_LOCAID_I:

      nvruim_read_item((byte *)nvruim_analog_reg_ind_buf.locaid_pureg,
                       2,
                       5,
                       MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND,
                       timer_index,
                       context,
                       nvruim_cmd_ptr);

      if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
      {
        nvruim_analog_reg_ind_buf.locaid_pureg[1] |=
          (byte) ((nv_cmd_ptr->data_ptr->locaid >> 8) & 0x0F);
        nvruim_analog_reg_ind_buf.locaid_pureg[0] =
          (byte) (nv_cmd_ptr->data_ptr->locaid);

        nvruim_write_item((byte *)nvruim_analog_reg_ind_buf.locaid_pureg,
                          2,
                          5,
                          MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND,
                          timer_index,
                          context,
                          nvruim_cmd_ptr);
      }
      break;

    case NV_PUREG_I:
      nvruim_read_item((byte *)nvruim_analog_reg_ind_buf.locaid_pureg,
                       2,
                       5,
                       MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND,
                       timer_index,
                       context,
                       nvruim_cmd_ptr);

      if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
      {
        if(nv_cmd_ptr->data_ptr->pureg)
          nvruim_analog_reg_ind_buf.locaid_pureg[1] |= 0x080;
        else
          nvruim_analog_reg_ind_buf.locaid_pureg[1] &= 0x07F;

        nvruim_write_item((byte *)nvruim_analog_reg_ind_buf.locaid_pureg,
                          2,
                          5,
                          MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND,
                          timer_index,
                          context,
                          nvruim_cmd_ptr);
      }
      break;
    default:
      return NV_RUIM_ITEM_NOT_SUPPORTED;

  }
  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_locn_regn_ind */


/*===========================================================================
FUNCTION NVRUIM_WRITE_ZONE_LIST

DESCRIPTION
Write LOCN ZONE LIST to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_zone_list
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nvruim_zone_reg_ind_type nvruim_zone_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_zone_reg_ind_buf, 0x00, sizeof(nvruim_zone_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_zone_reg_ind_buf.reg_zone[1] =
    (byte) (nv_cmd_ptr->data_ptr->zone_list.reg_zone >> 8);
  nvruim_zone_reg_ind_buf.reg_zone[0] =
    (byte) (nv_cmd_ptr->data_ptr->zone_list.reg_zone);

  nvruim_zone_reg_ind_buf.sid[1] =
    (byte) (nv_cmd_ptr->data_ptr->zone_list.sid >> 8);
  nvruim_zone_reg_ind_buf.sid[0] =
    (byte) (nv_cmd_ptr->data_ptr->zone_list.sid);

  nvruim_zone_reg_ind_buf.nid[1] =
    (byte) (nv_cmd_ptr->data_ptr->zone_list.nid >> 8);
  nvruim_zone_reg_ind_buf.nid[0] =
    (byte) (nv_cmd_ptr->data_ptr->zone_list.nid);

  if(nv_cmd_ptr->data_ptr->zone_list.band_class != 0)
  {
    /* PCS band or others. The Cellulaer band information can be
    retreaved from the sid.  */
    nvruim_zone_reg_ind_buf.freq_block =
      (byte) (nv_cmd_ptr->data_ptr->zone_list.block_or_sys);
  }

  nvruim_zone_reg_ind_buf.band_class =
    nv_cmd_ptr->data_ptr->zone_list.band_class;

  nvruim_write_rec_item((byte *) &(nvruim_zone_reg_ind_buf),
                        sizeof(nvruim_zone_reg_ind_type),
                        1,
                        MMGSDI_CDMA_ZONE_BASED_REGN_IND,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_zone_list */


/*===========================================================================
FUNCTION NVRUIM_WRITE_SID_NID_LIST

DESCRIPTION
Write SID NID List  to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_sid_nid_list
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nvruim_sys_reg_ind_type nvruim_sys_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_sys_reg_ind_buf, 0x00, sizeof(nvruim_sys_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_read_item((byte *) &(nvruim_sys_reg_ind_buf),
                   sizeof(nvruim_sys_reg_ind_type),
                   1,
                   MMGSDI_CDMA_SYS_REGN_IND,
                   timer_index,
                   context,
                   nvruim_cmd_ptr);

  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    nvruim_sys_reg_ind_buf.sid[1] =
      (byte) ((nv_cmd_ptr->data_ptr->sid_nid_list.sid & 0x7FFF) >> 8);
    nvruim_sys_reg_ind_buf.sid[0] =
      (byte) (nv_cmd_ptr->data_ptr->sid_nid_list.sid);

    nvruim_sys_reg_ind_buf.nid[1] =
      (byte) (nv_cmd_ptr->data_ptr->sid_nid_list.nid >> 8);
    nvruim_sys_reg_ind_buf.nid[0] =
      (byte) (nv_cmd_ptr->data_ptr->sid_nid_list.nid);

    /* Preserve the 3 LSB bits of ZONE_TIMER */
    nvruim_sys_reg_ind_buf.band_class &= ~NVRUIM_BAND_CLASS_MASK;
    nvruim_sys_reg_ind_buf.band_class |=
      (byte) (nv_cmd_ptr->data_ptr->sid_nid_list.band_class << 3);

    if(nv_cmd_ptr->data_ptr->sid_nid_list.band_class != 0)
    {
      /* PCS band or others. The Cellulaer band information can be
         retreaved from the sid.  */
      nvruim_sys_reg_ind_buf.freq_block =
         (byte) (nv_cmd_ptr->data_ptr->sid_nid_list.block_or_sys);
    }

    nvruim_write_item((byte *) &(nvruim_sys_reg_ind_buf),
                      sizeof(nvruim_sys_reg_ind_type),
                      1,
                      MMGSDI_CDMA_SYS_REGN_IND,
                      timer_index,
                      context,
                      nvruim_cmd_ptr);

  }

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_sid_nid_list */


/*===========================================================================
FUNCTION NVRUIM_WRITE_DIST_REG

DESCRIPTION
Write DIST REG  to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_dist_reg
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nvruim_dist_reg_ind_type nvruim_dist_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_dist_reg_ind_buf, 0x00, sizeof(nvruim_dist_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_dist_reg_ind_buf.base_lat_reg[2] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.base_lat_reg >> 16);
  nvruim_dist_reg_ind_buf.base_lat_reg[1] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.base_lat_reg >> 8);
  nvruim_dist_reg_ind_buf.base_lat_reg[0] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.base_lat_reg);

  nvruim_dist_reg_ind_buf.base_long_reg[2] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.base_long_reg >> 16);
  nvruim_dist_reg_ind_buf.base_long_reg[1] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.base_long_reg >> 8);
  nvruim_dist_reg_ind_buf.base_long_reg[0] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.base_long_reg);

  nvruim_dist_reg_ind_buf.reg_dist_reg[1] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.reg_dist_reg >> 8);
  nvruim_dist_reg_ind_buf.reg_dist_reg[0] =
    (byte) (nv_cmd_ptr->data_ptr->dist_reg.reg_dist_reg);

  nvruim_write_item((byte *) &(nvruim_dist_reg_ind_buf),
                    sizeof(nvruim_dist_reg_ind_type),
                    0,
                    MMGSDI_CDMA_DIST_BASED_REGN_IND,
                    timer_index,
                    context,
                    nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_dist_reg */


/*===========================================================================
FUNCTION NVRUIM_WRITE_DS_QSMIP

DESCRIPTION
Write DS QCMIP to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_ds_qcmip
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8                   nvruim_3GPD_op_mode_buf = 0x00;
  sys_modem_as_id_e_type  as_id                   = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_3GPD_op_mode_buf = (nv_cmd_ptr->data_ptr->ds_qcmip) & NVRUIM_3GPDOPM_MASK;
  nvruim_write_item((byte *) &(nvruim_3GPD_op_mode_buf),
                    1,
                    0,
                    MMGSDI_CDMA_3GPD_3GPDOPM,
                    timer_index,
                    context,
                    nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_ds_qcmip */


/*===========================================================================
FUNCTION NVRUIM_WRITE_DS_MIP_ACTIVE_PROF

DESCRIPTION
Write ACTIVE PROF  to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_ds_mip_active_prof
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8                   nvruim_mipsp_buf = 0x00;
  sys_modem_as_id_e_type  as_id            = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_NV_SIP_RUIM == uim_3gpd_support_per_sub(as_id))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_mipsp_buf = (uint8)((nv_cmd_ptr->data_ptr->ds_mip_active_prof) << NVRUIM_MIPSP_SHIFT);

  nvruim_write_item((byte *) &(nvruim_mipsp_buf),
                    1,
                    0,
                    MMGSDI_CDMA_3GPD_MIPSP,
                    timer_index,
                    context,
                    nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_ds_mip_active_prof */


/*===========================================================================
FUNCTION NVRUIM_WRITE_DS_MIP_ACTIVE_PROF

DESCRIPTION
Write ACTIVE PROF  to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_ds_sip_active_profile(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8                   nvruim_sipsp_buf = 0x00;
  sys_modem_as_id_e_type  as_id            = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_RUIM_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_sipsp_buf = (uint8)((nv_cmd_ptr->data_ptr->ds_sip_active_profile_index) << NVRUIM_SIPSP_SHIFT);

  nvruim_write_item((byte *) &(nvruim_sipsp_buf),
                    1,
                    0,
                    MMGSDI_CDMA_3GPD_SIPSP,
                    timer_index,
                    context,
                    nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
}/* nvruim_write_ds_sip_active_profile */


/*===========================================================================
FUNCTION NVRUIM_WRITE_PPP_PASSWD

DESCRIPTION
Write PPP Passwd  to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_ppp_passwd
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  boolean  skip_write = FALSE; /* Indicates write to RUIM should be skipped bcos
  it is not needed due to the pre-read failing */
  uint8    i                = 0;
  uint16   src_bit_count    = 0;
  uint16   dest_bit_count   = 0;
  uint8    num_nai          = 0;
  uint8    nai_count        = 0;
  uint8    profile_number   = 0;
  uint8    ss_length        = 0;
  uint8    stored_ss_size   = 0;
  uint32   sippapss_ef_size = 0;
  uint8    old_nvruim_sippapss_ef_buf[NVRUIM_MAX_SIPPAPSS_SIZE]             = {0};
  uint16   nvruim_sip_pap_ss_profile_bit_index[NVRUIM_MAX_NUM_SIP_PROFILES] = {0};
  uint8    nvruim_sippapss_buf[NVRUIM_MAX_SIPPAPSS_SIZE]                    = {0};
  sys_modem_as_id_e_type  as_id                                             = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_RUIM_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_read_item((byte *)old_nvruim_sippapss_ef_buf,
                   NVRUIM_MAX_SIPPAPSS_SIZE,
                   0,
                   MMGSDI_CDMA_3GPD_SIPPAPSS,
                   timer_index,
                   context,
                   nvruim_cmd_ptr);

  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    /* nvruim_read_item will read the entire data of the file , Get the file size */
    sippapss_ef_size = nvruim_cmd_ptr->data.data_len;

    /* The src and dest counter are "bit pointers" used as cursors in the buffers */
    /* Skip the first two fields to get to the first profile */
    src_bit_count = FSIZ(nvruim_sippapss_ef_fields,length_of_block_field) +
      FSIZ(nvruim_sippapss_ef_fields, num_nai_field);
    dest_bit_count = src_bit_count;
    nai_count = 0;
    /* The number of NAIs is specified in bits 9-12 of the file */
    num_nai = (old_nvruim_sippapss_ef_buf[1] & NVRUIM_SIPPAPSS_NUM_NAI_MASK) >> 4;
    /* Clear out the buffer with zeros */
    memset(nvruim_sippapss_buf, 0x0, NVRUIM_MAX_SIPPAPSS_SIZE);
    /* Copy the number of NAIs from the old buffer to the new buffer */
    b_copy((void*) old_nvruim_sippapss_ef_buf,
      FSIZ(nvruim_sippapss_ef_fields, length_of_block_field),
      (void*) nvruim_sippapss_buf,
      FSIZ(nvruim_sippapss_ef_fields, length_of_block_field),
      FSIZ(nvruim_sippapss_ef_fields, num_nai_field));
    /* Clear out the buffer containing ptrs to SIP Profiles */
    for(i = 0; i < NVRUIM_MAX_NUM_SIP_PROFILES; i++)
    {
      nvruim_sip_pap_ss_profile_bit_index[i] = NVRUIM_NO_PROFILE_INDEX_SET;
    }
    /* Initialize skip_write to TRUE (so the write fails), when the profile
    to be overwritten is found, set skip_write to FALSE */
    skip_write = TRUE;
    while(nai_count < num_nai)
    {
      profile_number = b_unpackb(old_nvruim_sippapss_ef_buf, src_bit_count,
                            FSIZ(nvruim_sippapss_ef_fields, profile_num_field));
      b_copy((void*) old_nvruim_sippapss_ef_buf, src_bit_count,
             (void*) nvruim_sippapss_buf, dest_bit_count,
             FSIZ(nvruim_sippapss_ef_fields, profile_num_field));
      if (profile_number >= NVRUIM_MAX_NUM_SIP_PROFILES)
      {
        UIM_MSG_ERR_1("Invalid profile number 0x%x", profile_number);
        *op_status = NV_FAIL_S;
        return NV_RUIM_SUPPORTS_ITEM;
      }
      nvruim_sip_pap_ss_profile_bit_index[profile_number] = dest_bit_count;
      src_bit_count += FSIZ(nvruim_sippapss_ef_fields, profile_num_field);
      dest_bit_count += FSIZ(nvruim_sippapss_ef_fields, profile_num_field);
      if((nv_cmd_ptr->item == NV_PPP_PASSWORD_I) &&
          profile_number == nvruim_preferred_sip_profile)
      {
        /* Legacy Item for Single SIP Profile Support */
        skip_write = FALSE;
        if(nv_cmd_ptr->data_ptr->ppp_password.password_len >
           NVRUIM_MAX_SIPPAPSS_SS_SIZE)
        {
           UIM_MSG_HIGH_0("SIPPAPSS is too large for card.  Truncating");
           stored_ss_size = NVRUIM_MAX_SIPPAPSS_SS_SIZE;
        }
        else
        {
           stored_ss_size = nv_cmd_ptr->data_ptr->ppp_password.password_len;
        }
        b_packb(stored_ss_size,
                nvruim_sippapss_buf, dest_bit_count,
                FSIZ(nvruim_sippapss_ef_fields, ss_length_field));
        dest_bit_count += FSIZ(nvruim_sippapss_ef_fields, ss_length_field);
        /* Bypass the old SS size, and SS for this profile */
        src_bit_count += (b_unpackb(old_nvruim_sippapss_ef_buf, src_bit_count,
                         FSIZ(nvruim_sippapss_ef_fields, ss_length_field)) * 8) +
                         FSIZ(nvruim_sippapss_ef_fields, ss_length_field);
        b_copy((void*) nv_cmd_ptr->data_ptr->ppp_password.password, 0,
               (void*) nvruim_sippapss_buf, dest_bit_count,
               stored_ss_size * 8);
        dest_bit_count += (stored_ss_size * 8);

      } /* Legacy Item */
      else if((nv_cmd_ptr->item == NV_DS_SIP_PPP_SS_INFO_I) &&
          profile_number == nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.index)
      {
        skip_write = FALSE;
        if(nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss_length >
           NVRUIM_MAX_SIPPAPSS_SS_SIZE)
        {
           UIM_MSG_HIGH_0("SIPPAPSS is too large for card.  Truncating");
           stored_ss_size = NVRUIM_MAX_SIPPAPSS_SS_SIZE;
        }
        else
        {
           stored_ss_size = nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss_length;
        }
        b_packb(stored_ss_size,
                nvruim_sippapss_buf, dest_bit_count,
                FSIZ(nvruim_sippapss_ef_fields, ss_length_field));
        dest_bit_count += FSIZ(nvruim_sippapss_ef_fields, ss_length_field);
        /* Bypass the old SS size, and SS for this profile */
        src_bit_count += (b_unpackb(old_nvruim_sippapss_ef_buf, src_bit_count,
                         FSIZ(nvruim_sippapss_ef_fields, ss_length_field)) * 8) +
                         FSIZ(nvruim_sippapss_ef_fields, ss_length_field);
        b_copy((void*) nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss, 0,
               (void*) nvruim_sippapss_buf, dest_bit_count,
               stored_ss_size * 8);
        dest_bit_count += (stored_ss_size * 8);

      } /* Multiple Profiles Support */
      else
      {
        ss_length = b_unpackb(old_nvruim_sippapss_ef_buf, src_bit_count,
                      FSIZ(nvruim_sippapss_ef_fields, ss_length_field));
        b_copy((void*) old_nvruim_sippapss_ef_buf, src_bit_count,
               (void*) nvruim_sippapss_buf, dest_bit_count,
               (FSIZ(nvruim_sippapss_ef_fields, ss_length_field) + (ss_length * 8)));
        src_bit_count += (FSIZ(nvruim_sippapss_ef_fields, ss_length_field) + (ss_length * 8));
        dest_bit_count += (FSIZ(nvruim_sippapss_ef_fields, ss_length_field) + (ss_length * 8));
      }
      nai_count++;
    } /* end While */
    /* Make sure that the byte count includes a rounded up count of bits */
    if(dest_bit_count / 8 != 0)
    {
      nvruim_sippapss_buf[0] = uint16touint8((dest_bit_count / 8) + 1);
    }
    else
    {
      nvruim_sippapss_buf[0] = uint16touint8(dest_bit_count / 8);
    }
    if (sippapss_ef_size < (uint32)(nvruim_sippapss_buf[0] + 1))
    {
      skip_write = FALSE;
      UIM_MSG_ERR_2("SIPPAPSS EF does not have enough size to store the new data [File size 0x%x, Size require: 0x%x]",
                    sippapss_ef_size, (nvruim_sippapss_buf[0] + 1));
    }
  }
  else
  {
    skip_write = TRUE;
  }

  if(!skip_write)
  {
   /* ------------------------------------------------
     File Structure of SIMPAPSS file
     File size: 1+X
     Bytes      Description                            M/O     Length
     1          Length of SimpleIP PAP SS Parameter    M       1 bytes
     2-X+1      SimpleIP PAP SS Parameter Block        M       X bytes
    ---------------------------------------------------*/
    nvruim_write_item((byte *)nvruim_sippapss_buf,
                      sippapss_ef_size,
                      0,
                      MMGSDI_CDMA_3GPD_SIPPAPSS,
                      timer_index,
                      context,
                      nvruim_cmd_ptr);
  }

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_ppp_passwd */


/*===========================================================================
FUNCTION nvruim_write_bcsms_pref

DESCRIPTION
  Write BCSMS preference  to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_bcsms_pref
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nv_cmd_ptr == NULL || op_status == NULL || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  if(nvruim_session_data_ptr[context]->bcsms_svc == FALSE )
  {
    UIM_MSG_HIGH_0("Card doesn't support BCSMS Extensions, falling back to NV");
    return NV_RUIM_ITEM_NOT_SUPPORTED;

  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_write_item((byte*)(& nv_cmd_ptr->data_ptr->sms_bc_user_pref),
                    1,
                    0,
                    MMGSDI_CDMA_BCSMS_PREF,
                    timer_index,
                    context,
                    nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_bcsms_pref */


/*===========================================================================
FUNCTION nvruim_write_bcsms_table

DESCRIPTION
  Write BCSMS preference  to card

DEPENDENCIES


RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_write_bcsms_table
(
  nv_cmd_type             *nv_cmd_ptr,
  nv_stat_enum_type       *op_status,
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8                    rec_num = 0;
  uint8                    nvruim_bcsms_parm_buf[NVRUIM_BCSMS_PARM_SIZE] = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nv_cmd_ptr == NULL || op_status == NULL || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  if(nvruim_session_data_ptr[context]->bcsms_svc == FALSE )
  {
    UIM_MSG_HIGH_0("Card doesn't support BCSMS Extensions, falling back to NV");
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  if( nv_cmd_ptr->item == NV_SMS_BC_SERVICE_TABLE_I )
  {
    rec_num = nv_cmd_ptr->data_ptr->sms_bc_service_table.index + 1;
    nvruim_bcsms_parm_buf[0] = nv_cmd_ptr->data_ptr->sms_bc_service_table.selected;
    nvruim_bcsms_parm_buf[1] = nv_cmd_ptr->data_ptr->sms_bc_service_table.priority;
  }
  else
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nvruim_write_rec_item(nvruim_bcsms_parm_buf,
                        NVRUIM_BCSMS_PARM_SIZE,
                        rec_num, /* record number */
                        MMGSDI_CDMA_BCSMS_PARAMS,
                        timer_index,
                        context,
                        nvruim_cmd_ptr);

  return nvruim_write_sw(nv_cmd_ptr,
                         op_status,
                         nvruim_cmd_ptr->status_word,
                         nvruim_cmd_ptr->report_status);
} /* nvruim_write_bcsms_table */


/*===========================================================================

FUNCTION NVRUIM_WRITE

DESCRIPTION
Check to see if the item is supported by the R-UIM.
If it is supported, write it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the write.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
The command data buffer is re-formatted.

===========================================================================*/
nv_ruim_support_status nvruim_write
(
  nv_cmd_type               *nv_cmd_ptr,      /* command block    */
  nvruim_nv_context_type     context,
  nv_stat_enum_type         *op_status        /* status of the I/O operation*/
)
{
  nv_ruim_support_status   nvruim_write_sprt_status = NV_RUIM_ITEM_NOT_SUPPORTED;
  boolean                  timer_status             = FALSE;
  uint8                    timer_index              = 0;
  nvruim_session_enum_type nvruim_session           = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_cmd_type         *nvruim_cmd_ptr           = NULL;
  sys_modem_as_id_e_type   as_id                    = SYS_MODEM_AS_ID_1;

  if(!op_status || !nv_cmd_ptr || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Try to setup a timer for every incoming request */
  NVRUIM_ENTER_TIMER_CRIT_SECT;
  timer_status = nvruim_setup_timer(&timer_index, context);
  NVRUIM_LEAVE_TIMER_CRIT_SECT;

  if (!timer_status)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nvruim_session = nvruim_map_context_to_nvruim_session(context);
  as_id = nvruim_convert_context_to_as_id(context);

  /* Determine if the CDMA subscription is on the card */
  if(NV_RTRE_CONTROL_USE_RUIM != nvruim_get_rtre_control_value(nvruim_session))
  {
    /* Do not try to access the R-UIM card.  Get item from NV */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nvruim_cmd_ptr = nvruim_mem_malloc(sizeof(nvruim_cmd_type));

  if (nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
  /* No need to read the mmgsdi features status nvitem into thr nvruim
     global here as it is already read in nvruim task's post init function */

  /* Take the task pointer signal and tcb from nv_cmd_ptr and put into the cmd */
  nvruim_cmd_ptr->cmd_hdr.sigs = nv_cmd_ptr->sigs;
  nvruim_cmd_ptr->cmd_hdr.task_ptr = rex_self();
  nvruim_cmd_ptr->nv_context = context;

  /* Find-out if the NV item is an R-UIM item */
  /* If it is an R-UIM item:  */
  /*     Initialize the buffer with item pointer & size */
  /*     Reformat the data for R-UIM (reverse bytes & set flags)*/
  /*     Select the offset into the EF  */

  switch(nv_cmd_ptr->item)
  {
    /*************HANDLE READ ONLY ITEMS FIRST**************************/
    /*-------------ESN READ ONLY Items --------------------------------*/
    case NV_ESN_CHKSUM_I:
    case NV_ESN_I:
    case NV_MEID_I:
      break;

#ifdef FEATURE_UIM_SUPPORT_LBS
    case NV_GPS1_CAPABILITIES_I:
    case NV_GPS1_SEEDPOS_OPTION_I:
    case NV_GPS1_DYNAMIC_MODE_I:
    case NV_GPS1_XTRA_ENABLED_I:
    case NV_GPS1_XTRA_AUTO_DOWNLOAD_ENABLED_I:
    case NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I:
    case NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I:
    case NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I:
    case NV_GPS1_PDE_ADDRESS_I:
    case NV_GPS1_PDE_PORT_I:
    case NV_CGPS_1X_PDE_SERVER_ADDR_IPV4_I:
    case NV_CGPS_1X_PDE_SERVER_PORT_I:
    case NV_CGPS_1X_MPC_SERVER_ADDR_IPV4_I:
    case NV_CGPS_1X_MPC_SERVER_PORT_I:
    case NV_GPS1_XTRA_PRIMARY_SERVER_URL_I:
    case NV_GPS1_XTRA_SECONDARY_SERVER_URL_I:
    case NV_GPS1_XTRA_TERTIARY_SERVER_URL_I:
    case NV_CGPS_1X_PDE_SERVER_ADDR_IPV6_I:
    case NV_CGPS_1X_MPC_SERVER_ADDR_IPV6_I:
    case NV_CGPS_1X_PDE_SERVER_ADDR_URL_I:
    case NV_CGPS_1X_MPC_SERVER_ADDR_URL_I:
      /* Checking of Boolean nvruim_lbs_support, it indicate whether n24 is
         is supported or not  */
      if(nvruim_session_data_ptr[context]->nvruim_lbs_support)
      {
        /* Return READ ONLY */
        *op_status = NV_READONLY_S;
        nv_cmd_ptr->status = *op_status;
        nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;
#endif /* FEATURE_UIM_SUPPORT_LBS */

    case  NV_HYSTERISIS_ACTIVATION_TIMER_I:
    case  NV_ACTIVE_EPZID_TYPE_I:
    case  NV_TCP_KEEPALIVE_IDLE_TIME_I:
    case  NV_DSAT707_CTA_TIMER_I:
      /* Checking of Boolean ext_support_3gpd,
         it indicate whether n15 is supported or not */
      if(nvruim_data_3gpd_ext_support_per_sub(as_id) &&
         NVRUIM_3GPD_MIP_NV_SIP_NV != uim_3gpd_support_per_sub(as_id))
      {
        /* Return READ ONLY */
        *op_status = NV_READONLY_S;
        nv_cmd_ptr->status = *op_status;
        nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;

    /*-------------SIP 3GPD READ ONLY Items --------------------------------*/
    case NV_DS_SIP_PROFILE_I:
      /* If OMH 3GPD Extensions are not supported, allow them to fallback to NV */
      if(nvruim_data_3gpd_ext_support_per_sub(as_id) &&
         NVRUIM_3GPD_MIP_RUIM_SIP_NV != uim_3gpd_support_per_sub(as_id) &&
         NVRUIM_3GPD_MIP_NV_SIP_NV != uim_3gpd_support_per_sub(as_id))
      {
        *op_status = NV_READONLY_S;
        nv_cmd_ptr->status = *op_status;
        nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;

    case NV_DS_SIP_NUM_VALID_PROFILES_I:
    case NV_DS_SIP_NAI_INFO_I:
    case NV_PPP_USER_ID_I:
      if(NVRUIM_3GPD_MIP_RUIM_SIP_NV != uim_3gpd_support_per_sub(as_id) &&
         NVRUIM_3GPD_MIP_NV_SIP_NV != uim_3gpd_support_per_sub(as_id))
      {
        /* Return READ ONLY */
        *op_status = NV_READONLY_S;
        nv_cmd_ptr->status = *op_status;
        nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;

    /*-------------MIP 3GPD READ ONLY Items --------------------------------*/
    /* The following items are 3GPD Extended parameters for OMH */
    case NV_DS_MIP_QC_DRS_OPT_I:
    case NV_DS_MIP_QC_HANDDOWN_TO_1X_OPT_I:
    case NV_DS_MIP_RRQ_IF_TFRK_I:
    case NV_TCP_GRACEFUL_DORMANT_CLOSE_I:
    /* If OMH 3GPD Extensions are not supported, allow them to fallback to
       NV with the exception of 2002BIS, which should never fallback to NV when
       using RUIM.  If 3GPD extensions not supported, return READ ONLY for 2002BIS*/
      if((NVRUIM_3GPD_MIP_NV_SIP_NV != uim_3gpd_support_per_sub(as_id))   &&
         (NVRUIM_3GPD_MIP_NV_SIP_RUIM != uim_3gpd_support_per_sub(as_id)) &&
         nvruim_data_3gpd_ext_support_per_sub(as_id))
      {
        /* Return READ ONLY */
        *op_status = NV_READONLY_S;
        nv_cmd_ptr->status = *op_status;
        nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;

    case NV_DS_MIP_2002BIS_MN_HA_AUTH_I:
    case NV_DS_MIP_NUM_PROF_I:
    case NV_DS_MIP_GEN_USER_PROF_I:
    case NV_DS_MIP_RETRIES_I:
    case NV_DS_MIP_RETRY_INT_I:
    case NV_DS_MIP_PRE_RE_RRQ_TIME_I:
    case NV_DS_MIP_ENABLE_PROF_I:
    /* The following case is not truly read-only, it is
       actually neither readable nor writable in an R-UIM
       implementation, Nevertheless, NV_READONLY_S is being
       returned */
    case NV_DS_MIP_SS_USER_PROF_I:

    /* The following 2 cases relate to items not used
       with an R-UIM implementation.  They will return read-only,
       if a write is attempted, and not active if a read is attempted */
    case NV_DS_MIP_DMU_PKOID_I:
    case NV_DS_MIP_DMU_MN_AUTH_I:
      if(NVRUIM_3GPD_MIP_NV_SIP_NV != uim_3gpd_support_per_sub(as_id) &&
         NVRUIM_3GPD_MIP_NV_SIP_RUIM != uim_3gpd_support_per_sub(as_id))
      {
        /* Return READ ONLY */
        *op_status = NV_READONLY_S;
        nv_cmd_ptr->status = *op_status;
        nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;

    /*-------------HRPD AN READ ONLY Items --------------------------------*/
    /* These four HRPD related items MUST come after the 3GPD items and before the
       other Read only items, because they also require their own runtime check!!!
       The following items store the HRPD NAI (read-only for an R-UIM implementation
       NV_HDR_AN_AUTH_USER_ID_LONG_I is the preferred NV item, although some may still
       be using the older item */
    case NV_HDR_AN_AUTH_USER_ID_LONG_I:
    case NV_HDR_AN_AUTH_NAI_I:
    case NV_HDR_AN_AUTH_PASSWD_LONG_I:
    case NV_HDR_AN_AUTH_PASSWORD_I:
      if(NVRUIM_AN_HRPD_NO_SUPPORT != uim_an_hrpd_support_per_sub(as_id))
      {
        /* Return READ ONLY */
        *op_status = NV_READONLY_S;
        nv_cmd_ptr->status = *op_status;
        nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;

    case NV_MIN1_I:
    case NV_IMSI_T_S1_I:
    case NV_IMSI_T_S2_I:
    case NV_MIN2_I:
    case NV_IMSI_ADDR_NUM_I:
    case NV_IMSI_T_ADDR_NUM_I:
    case NV_IMSI_11_12_I:
    case NV_IMSI_T_11_12_I:
    case NV_IMSI_MCC_I:
    case NV_IMSI_T_MCC_I:
    case NV_ACCOLC_I:
      *op_status = NV_READONLY_S;
      nv_cmd_ptr->status = *op_status;
      nvruim_write_sprt_status = NV_RUIM_SUPPORTS_ITEM;
      break;

    /************************END READ ONLY ITEMS***************************/
    /************************Writable ITEMS********************************/
    case NV_SLOT_CYCLE_INDEX_I:
      if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_RUIM_SUPPORT_SCI_I,
                                       context))
      {
        nvruim_write_sprt_status = nvruim_write_sci(nv_cmd_ptr,
                                                    op_status,
                                                    timer_index,
                                                    context,
                                                    nvruim_cmd_ptr);
      }
      break;

    case NV_SID_NID_I:
    case NV_HOME_SID_NID_I:
      nvruim_write_sprt_status = nvruim_write_sid_nid(nv_cmd_ptr,
                                                      op_status,
                                                      timer_index,
                                                      context,
                                                      nvruim_cmd_ptr);
      break;


    case NV_DIR_NUMBER_I:
      nvruim_write_sprt_status = nvruim_write_msisdn(nv_cmd_ptr,
                                                     op_status,
                                                     timer_index,
                                                     context,
                                                     nvruim_cmd_ptr);
      break;


    case NV_DIR_NUMBER_PCS_I:
      nvruim_write_sprt_status = nvruim_write_dir_number_pcs(nv_cmd_ptr,
                                                             op_status,
                                                             timer_index,
                                                             context,
                                                             nvruim_cmd_ptr);
      break;


    case NV_COUNT_I:
      nvruim_write_sprt_status = nvruim_write_count(nv_cmd_ptr,
                                                    op_status,
                                                    timer_index,
                                                    context,
                                                    nvruim_cmd_ptr);
      break;


    case NV_ANALOG_FIRSTCHP_I:
      nvruim_write_sprt_status = nvruim_write_analog_firstchip(nv_cmd_ptr,
                                                               op_status,
                                                               timer_index,
                                                               context,
                                                               nvruim_cmd_ptr);
      break;


    case NV_ANALOG_HOME_SID_I:
      nvruim_write_sprt_status = nvruim_write_home_sid(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;


    case NV_MOB_TERM_HOME_I:
    case NV_MOB_TERM_FOR_SID_I:
    case NV_MOB_TERM_FOR_NID_I:
      nvruim_write_sprt_status = nvruim_write_mob_term(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;

    case NV_OTAPA_ENABLED_I:
    case NV_SPC_CHANGE_ENABLED_I:
      nvruim_write_sprt_status = nvruim_write_spc_enable(nv_cmd_ptr,
                                                         op_status,
                                                         timer_index,
                                                         context,
                                                         nvruim_cmd_ptr);
      break;

    case NV_NAM_LOCK_I:
      nvruim_write_sprt_status = nvruim_write_nam_lock(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;

    /* Service Preferences */
    case  NV_PREF_MODE_I:
      if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_PREF_MODE_NOT_FROM_RUIM_I,
                                       context) == FALSE)
      {
        nvruim_write_sprt_status = nvruim_write_pref(nv_cmd_ptr,
                                                     op_status,
                                                     timer_index,
                                                     context,
                                                     nvruim_cmd_ptr);
      }
      break;

    case  NV_SYSTEM_PREF_I:
      nvruim_write_sprt_status = nvruim_write_pref(nv_cmd_ptr,
                                                   op_status,
                                                   timer_index,
                                                   context,
                                                   nvruim_cmd_ptr);
      break;

    /* TMSI */
    case  NV_ASSIGNING_TMSI_ZONE_LEN_I:
    case  NV_ASSIGNING_TMSI_ZONE_I:
    case  NV_TMSI_CODE_I:
    case  NV_TMSI_EXP_I:
      nvruim_write_sprt_status = nvruim_write_tmsi(nv_cmd_ptr,
                                                   op_status,
                                                   timer_index,
                                                   context,
                                                   nvruim_cmd_ptr);
      break;

    /* Analog Location and Registration Indicators */
    case  NV_NXTREG_I:
    case  NV_LSTSID_I:
    case  NV_LOCAID_I:
    case  NV_PUREG_I:
      nvruim_write_sprt_status = nvruim_write_locn_regn_ind(nv_cmd_ptr,
                                                            op_status,
                                                            timer_index,
                                                            context,
                                                            nvruim_cmd_ptr);
      break;

    /* CDMA Zone-Based Registration Indicators */
    case  NV_ZONE_LIST_I:
      nvruim_write_sprt_status = nvruim_write_zone_list(nv_cmd_ptr,
                                                        op_status,
                                                        timer_index,
                                                        context,
                                                        nvruim_cmd_ptr);
      break;

    /* CDMA System/Network Registration Indicators */
    case  NV_SID_NID_LIST_I:
      nvruim_write_sprt_status = nvruim_write_sid_nid_list(nv_cmd_ptr,
                                                           op_status,
                                                           timer_index,
                                                           context,
                                                           nvruim_cmd_ptr);
      break;

    /* CDMA Distance-Based Registration Indicators */
    case  NV_DIST_REG_I:
      nvruim_write_sprt_status = nvruim_write_dist_reg(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;

    /* 3GPD Operational Mode Parameters */
    case NV_DS_QCMIP_I:
      nvruim_write_sprt_status = nvruim_write_ds_qcmip(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;


    /* Mobile IP Status Parameters */
    case NV_DS_MIP_ACTIVE_PROF_I:
      nvruim_write_sprt_status = nvruim_write_ds_mip_active_prof(nv_cmd_ptr,
                                                                 op_status,
                                                                 timer_index,
                                                                 context,
                                                                 nvruim_cmd_ptr);
      break;

    /* Simple IP Status Parameters */
    case NV_DS_SIP_ACTIVE_PROFILE_INDEX_I:
      nvruim_write_sprt_status = nvruim_write_ds_sip_active_profile(nv_cmd_ptr,
                                                                    op_status,
                                                                    timer_index,
                                                                    context,
                                                                    nvruim_cmd_ptr);
      break;
      /* Simple IP PAP SS Parameters */
      /* Please note that the following 2 NV items are used to refer to a generic
         password used for both PAP and CHAP.  In R-UIM, this item refers
         only to the PAP password.  The CHAP password is held internall by the
         card and is not accessible to the ME.  It is used indirectly only when
         a CHAP Challenge is executed with the corresponding NAI-Entry-Index.
         The NV_PPP_PASSWORD_I is the legacy NV item used by a single SIP Profile
         scenario, and mapps to the preferred SS on the card.  The
         NV_DS_SIP_PPP_SS_INFO_I item refers to the multiple SIP Profile solution*/

    case NV_PPP_PASSWORD_I:
    case NV_DS_SIP_PPP_SS_INFO_I:
      /* No need to write PPP NAI\Password Legacy NV items to
         RUIM for OMH card*/
      if((nv_cmd_ptr->item != NV_PPP_PASSWORD_I) ||
         !nvruim_data_3gpd_ext_support_per_sub(as_id))
      {
        nvruim_write_sprt_status = nvruim_write_ppp_passwd(nv_cmd_ptr,
                                                           op_status,
                                                           timer_index,
                                                           context,
                                                           nvruim_cmd_ptr);
      }
      break;

    case NV_SMS_BC_USER_PREF_I:
      nvruim_write_sprt_status = nvruim_write_bcsms_pref(nv_cmd_ptr,
                                                         op_status,
                                                         timer_index,
                                                         context,
                                                         nvruim_cmd_ptr);
      break;

    case NV_SMS_BC_SERVICE_TABLE_I:
      nvruim_write_sprt_status = nvruim_write_bcsms_table(nv_cmd_ptr,
                                                          op_status,
                                                          timer_index,
                                                          context,
                                                          nvruim_cmd_ptr);
      break;

    default:
      /* Item is not supported by R-UIM */
      break;
  }

  nvruim_mem_free_null_ok(nvruim_cmd_ptr);

  /* Now clear the timer that was setup earlier */
  NVRUIM_ENTER_TIMER_CRIT_SECT;
  (void)nvruim_clear_timer(timer_index);
  NVRUIM_LEAVE_TIMER_CRIT_SECT;

  return nvruim_write_sprt_status;
} /* nvruim_write */


/*===========================================================================
FUNCTION NVRUIM_CHECK_MNANDATORY

DESCRIPTION
Check to see if the the namdatory files are present or not.

DEPENDENCIES
Non-reentrant

RETURN VALUE
None


SIDE EFFECTS
None

===========================================================================*/
static nv_ruim_support_status nvruim_check_mandatory
(
  const nv_cmd_type       *nv_cmd_ptr,
  nvruim_nv_context_type   context,
  mmgsdi_sw_type           status_word

)
{
  switch(nv_cmd_ptr->item)
  {
    /* JCDMA: this is a NV item */
    case NV_ROAMING_LIST_683_I:
    case NV_ESN_CHKSUM_I:
    case NV_ESN_I:
    case NV_COUNT_I:
    case NV_MIN1_I:
    case NV_MIN2_I:
    case NV_IMSI_11_12_I:
    case NV_IMSI_MCC_I:
    case NV_IMSI_ADDR_NUM_I:
    case NV_IMSI_T_S1_I:
    case NV_IMSI_T_S2_I:
    case NV_IMSI_T_MCC_I:
    case NV_IMSI_T_11_12_I:
    case NV_IMSI_T_ADDR_NUM_I:
    case NV_SID_NID_I:
    case NV_ANALOG_FIRSTCHP_I:
    case NV_ANALOG_HOME_SID_I:
    case NV_ACCOLC_I:
    case NV_MOB_TERM_HOME_I   :
    case NV_MOB_TERM_FOR_SID_I:
    case NV_MOB_TERM_FOR_NID_I:
    case NV_HOME_SID_NID_I:
    case NV_OTAPA_ENABLED_I:
    case NV_SPC_CHANGE_ENABLED_I:
    case NV_NAM_LOCK_I:
    case NV_PREF_MODE_I:
    case NV_SYSTEM_PREF_I:
    case NV_CDMA_PREF_SERV_I:
    case NV_ANALOG_PREF_SERV_I:
    case NV_ASSIGNING_TMSI_ZONE_LEN_I:
    case NV_ASSIGNING_TMSI_ZONE_I:
    case NV_TMSI_CODE_I:
    case NV_TMSI_EXP_I:
    case NV_NXTREG_I:
    case NV_LSTSID_I:
    case NV_LOCAID_I:
    case NV_PUREG_I:
    case NV_ZONE_LIST_I:
    case NV_SID_NID_LIST_I:
    case NV_MEID_I:

      if(status_word.present && 
         (status_word.sw1 == NVRUIM_SW1_REFERENCE) &&
         (status_word.sw2 == NVRUIM_SW2_NOT_FOUND))
      {
        uint8 slot_index = NVRUIM_SLOT_INDEX_INVALID;

        /* File not found */
        if (NVRUIM_CONTEXT_NOT_VALID(context) || nvruim_convert_slot_id_to_index(nvruim_session_data_ptr[context]->mmgsdi_slot_id, &slot_index) == FALSE)
        {
          return NV_RUIM_SUPPORTS_ITEM;
        }
        nvruim_dir_present[slot_index] = NVRUIM_CDMA_SUBSCRIPTION_NOT_PRESENT;
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
      else
        break;

    default:
      break;
  }
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_check_mandatory */


/*===========================================================================
FUNCTION NVRUIM_READ_SW

DESCRIPTION
Parses the SW from UIM

DEPENDENCIES
Non-reentrant

RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_sw
(
  nv_stat_enum_type           *op_status,   /* status of the I/O operation*/
  nvruim_rpt_status_enum_type  report_status

)
{
  /* Translate the R-UIM status into an NV status     */
  /* Data processing may alter this status */

  *op_status = NV_NOTACTIVE_S; /* Default status is "not active" to
  support upper layer fault handling  */
  switch(report_status)
  {
    case NVRUIM_PASS:
      *op_status = NV_DONE_S;     /* Request completed okay     */
      break;

      /* Other statuses:*/
      /* NV_BUSY_SRequest is queued*/
      /* NV_BADCMD_S    Unrecognizable command field  */
      /* NV_FULL_SThe NVM is full */
      /* NV_FAIL_S,     Command failed, reason other than NVM was full*/
      /* NV_BADPARM_S   Bad parameter in command block*/
      /* NV_READONLY_S  Parameter is read only*/
      /* NV_BADTG_S     Item not valid for Target     */
      /* NV_NOMEM_S     free memory exhausted */
      /* NV_NOTALLOC_S  address is not a valid allocation */

    case NVRUIM_FAIL:
      break;

  }
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_sw */


/*===========================================================================
FUNCTION NVRUIM_READ_AND_CHECK_RSP

DESCRIPTION
Read the Rsp from NVRUIM

DEPENDENCIES
Non-reentrant

RETURN VALUE
 nv_ruim_support_status

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_and_check_rsp
(
  const nv_cmd_type      *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type      *op_status,    /* status of the I/O operation*/
  uint8                   timer_index,
  nvruim_nv_context_type  context,
  nvruim_cmd_type        *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_ITEM_NOT_SUPPORTED;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* If req type is GET_FILE_ATTR, no need to change it to READ req.*/
  if(nvruim_cmd_ptr->cmd_type != NVRUIM_FILE_ATTR)
  {
    nvruim_cmd_ptr->cmd_type = NVRUIM_READ; /* "Read" command */
  }

  nvruim_access(nvruim_cmd_ptr, timer_index, context);  /* Access the R-UIM     */

  support_status = nvruim_check_mandatory(nv_cmd_ptr,
                                          context,
                                          nvruim_cmd_ptr->status_word);

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }
  return nvruim_read_sw(op_status, nvruim_cmd_ptr->report_status);
} /* nvruim_read_and_check_rsp */


/*===========================================================================
FUNCTION NVRUIM_READ_ESN

DESCRIPTION
Check to see if the ESN is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  Nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_esn
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  nv_esn_type              nvruim_ruim_id_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_ruim_id_buf, 0x00, sizeof(nv_esn_type));

  /* If the usage indicator is NV_RUIM_USE_MEID then the pESN from
     the handset should be returned not the pUIMID value from the card */
  if(NVRUIM_CONTEXT_NOT_VALID(context)                               ||
     nvruim_session_data_ptr[context]->esn_usage == NV_RUIM_USE_ESN  ||
     nvruim_session_data_ptr[context]->esn_usage == NV_RUIM_USE_MEID ||
     nvruim_session_data_ptr[context]->esn_usage == NV_RUIM_USE_SF_EUIMID_ESN)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
  else if(nvruim_session_data_ptr[context]->esn_usage == NV_RUIM_ZERO_ESN)
  {
    if(nv_cmd_ptr->item == NV_ESN_CHKSUM_I)
    {
      nv_cmd_ptr->data_ptr->esn_chksum.chksum =
       (dword) NV_VALID_ESN_CHKSUM;
    }
    else
    {
      nv_cmd_ptr->data_ptr->esn.esn = 0;
    }
    *op_status = NV_DONE_S;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr,op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Setup the command structure to read the RUIM_ID EF from the RUIM */
  nvruim_cmd_ptr->data.data_len    = NVRUIM_SIZE(esn.esn);
  nvruim_cmd_ptr->data.data_ptr    = (byte *) &(nvruim_ruim_id_buf);
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_RUIM_ID;
  nvruim_cmd_ptr->offset_or_recnum = 1; /* EF offset  */

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  if(nv_cmd_ptr->item == NV_ESN_I)
  {
    if(*op_status == NV_NOTACTIVE_S)
    {
      if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_RUIM_ALLOW_ESN_FAIL_I,
                                       context))
      {
        /* The builder wants to know when the ESN read fails. */
        *op_status = NV_FAIL_S;
      }
      else if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_RUIM_ZERO_ESN_FAIL_I,
                                            context))
      {
        /* The builder wants a zero ESN when the read fails. */
        nv_cmd_ptr->data_ptr->esn.esn = 0;
        *op_status = NV_DONE_S;
      }
      else
      {
        /* The builder wants the ESN out of EEPROM. */
        return NV_RUIM_ITEM_NOT_SUPPORTED;
      }
    } /* end if - the read from the RUIM has failed. */

    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the ESN from the read data buffer */
      nv_cmd_ptr->data_ptr->esn.esn = nvruim_ruim_id_buf.esn;
    } /* end if - the read was successful. */
  }
  else if(nv_cmd_ptr->item == NV_ESN_CHKSUM_I)
  {
    nv_cmd_ptr->data_ptr->esn_chksum.chksum = (dword)NV_VALID_ESN_CHKSUM;
  }

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_esn */


/*===========================================================================

FUNCTION NVRUIM_READ_COUNT

DESCRIPTION
Check to see if the CALL Count is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_count
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status                    = NV_RUIM_SUPPORTS_ITEM;
  uint8                  msisdn_buffer[NVRUIM_MSISDN_SIZE] = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Use the MSISDN buffer */
  nvruim_cmd_ptr->data.data_ptr    = msisdn_buffer;
  nvruim_cmd_ptr->data.data_len    = NVRUIM_SIZE(count.count);
  nvruim_cmd_ptr->offset_or_recnum = 1;
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_CC;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  /* Reformat the data only if the read was successful. */
  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->count.count =
       msisdn_buffer[1] & 0x3F;  /* 6 LSB bits */
  }
  nv_cmd_ptr->status = *op_status;
  return support_status;
} /* nvruim_read_count */


/*===========================================================================

FUNCTION NVRUIM_READ_IMSI_M

DESCRIPTION
Check to see if IMSI_M is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_imsi_m
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;
  nvruim_imsi_type       nvruim_imsi_m_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_imsi_m_buf, 0x00, sizeof(nvruim_imsi_type));

  /* Only process the NAM parameters for the RUIM
     This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->min1.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Setup the command structure to read the IMSI_M EF from the RUIM */
  nvruim_cmd_ptr->data.data_len    = sizeof(nvruim_imsi_type);
  nvruim_cmd_ptr->data.data_ptr    = (byte *) &(nvruim_imsi_m_buf);
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_IMSI_M;
  nvruim_cmd_ptr->offset_or_recnum = 0;    /* EF offset*/

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  if((nvruim_imsi_m_buf.imsi_addr_num & NVRUIM_IMSI_PROGRAMMED) == 0)
  {
    UIM_MSG_HIGH_0("IMSI_M_PROGRAMMED bit is 0.IMSI_M not programmed");
    *op_status = NV_NOTACTIVE_S;
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Note whether or not the IMSI was programmed  */
  if(nv_cmd_ptr->item == NV_IMSI_ADDR_NUM_I)
  {
    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      if(nvruim_imsi_m_buf.imsi_class == 0)
      {
        nv_cmd_ptr->data_ptr->imsi_addr_num.num = 0xFF;
      }
      else
      {
        nv_cmd_ptr->data_ptr->imsi_addr_num.num = (nvruim_imsi_m_buf.imsi_addr_num &
                                                   NVRUIM_IMSI_ADDR_NUM);
      }
    } /* end if - the read was successful. */
  }
  else if(nv_cmd_ptr->item == NV_IMSI_11_12_I)
  {
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->imsi_11_12.imsi_11_12 =
        nvruim_imsi_m_buf.imsi_11_12;
    } /* end if - the read was successful. */
  }
  else if(nv_cmd_ptr->item == NV_MIN1_I)
  {
    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->min1.min1 [NV_CDMA_MIN_INDEX] =
              (nvruim_imsi_m_buf.imsi_s1[2] << 16) |
              (nvruim_imsi_m_buf.imsi_s1[1] << 8)  |
              (nvruim_imsi_m_buf.imsi_s1[0]);
    } /* end if - the read was successful. */
    /* Copy the contents of CDMA MIN1 to Amps MIN1 */
    nv_cmd_ptr->data_ptr->min1.min1[0] =
    nv_cmd_ptr->data_ptr-> min1.min1[NV_CDMA_MIN_INDEX] ;
  }
  else if(nv_cmd_ptr->item == NV_MIN2_I)
  {
    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->min2.min2 [NV_CDMA_MIN_INDEX] =
            (nvruim_imsi_m_buf.imsi_s2[1] << 8) |
            nvruim_imsi_m_buf.imsi_s2[0];
    } /* end if - the read was successful. */
  /* Copy the contents of CDMA MIN2 to Amps MIN2 */
    nv_cmd_ptr->data_ptr->min2.min2[0] =
    nv_cmd_ptr->data_ptr-> min2.min2[NV_CDMA_MIN_INDEX] ;
  }
  else if(nv_cmd_ptr->item == NV_IMSI_MCC_I)
  {
    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->imsi_mcc.imsi_mcc =
          (nvruim_imsi_m_buf.imsi_mcc[1] << 8) |
          (nvruim_imsi_m_buf.imsi_mcc[0]);
    } /* end if - the read was successful. */
  }
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_imsi_m */


/*===========================================================================

FUNCTION NVRUIM_READ_IMSI_T

DESCRIPTION
Check to see if IMSI_T is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_imsi_t
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;
  nvruim_imsi_type       nvruim_imsi_t_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_imsi_t_buf, 0x00, sizeof(nvruim_imsi_type));

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->imsi_t_s1.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Setup the command structure to read the IMSI_M EF from the RUIM */
  nvruim_cmd_ptr->data.data_len     = sizeof(nvruim_imsi_type);
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_imsi_t_buf);
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_IMSI_T;
  nvruim_cmd_ptr->offset_or_recnum  = 0; /* EF offset  */

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }
  /* Note whether or not the IMSI was programmed  */
  if((nvruim_imsi_t_buf.imsi_addr_num & NVRUIM_IMSI_PROGRAMMED) == 0)
  {
    *op_status = NV_NOTACTIVE_S;
    UIM_MSG_HIGH_0("IMSI_T_PROGRAMMED bit is 0.IMSI_T not programmed");
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Reformat the data only if the read was successful. */
  if(nv_cmd_ptr->item == NV_IMSI_T_ADDR_NUM_I &&
     NV_DONE_S == *op_status)
  {
    if(nvruim_imsi_t_buf.imsi_class == 0)
    {
      nv_cmd_ptr->data_ptr->imsi_t_addr_num.num = 0xFF;
    }
    else
    {
      nv_cmd_ptr->data_ptr->imsi_t_addr_num.num =
      nvruim_imsi_t_buf.imsi_addr_num & NVRUIM_IMSI_ADDR_NUM;
    }
  }

  if(nv_cmd_ptr->item == NV_IMSI_T_S2_I)
  {
    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->imsi_t_s2.min2 [NV_CDMA_MIN_INDEX] =
              (nvruim_imsi_t_buf.imsi_s2[1] << 8) |
              (nvruim_imsi_t_buf.imsi_s2[0]);
    } /* end if - the read was successful. */
  }

  else if(nv_cmd_ptr->item == NV_IMSI_T_S1_I)
  {
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->imsi_t_s1.min1 [NV_CDMA_MIN_INDEX] =
             (nvruim_imsi_t_buf.imsi_s1[2] << 16) |
             (nvruim_imsi_t_buf.imsi_s1[1] << 8) |
             (nvruim_imsi_t_buf.imsi_s1[0]);
    } /* end if - the read was successful. */
  }

  else if(nv_cmd_ptr->item == NV_IMSI_T_11_12_I)
  {
    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->imsi_t_11_12.imsi_11_12 =
      nvruim_imsi_t_buf.imsi_11_12;
    } /* end if - the read was successful. */
  }

  else if(nv_cmd_ptr->item == NV_IMSI_T_MCC_I)
  {
    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the item from the read data buffer. */
      nv_cmd_ptr->data_ptr->imsi_t_mcc.imsi_mcc =
             (nvruim_imsi_t_buf.imsi_mcc[1] << 8) |
             (nvruim_imsi_t_buf.imsi_mcc[0]);
    } /* end if - the read was successful. */
  }
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_imsi_t */


/*===========================================================================

FUNCTION NVRUIM_READ_SID_NID

DESCRIPTION
Check to see if SID_NID is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_sid_nid
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint16                 i              = 0;
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;
  nvruim_sid_nid_type    sid_nid_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&sid_nid_buf, 0x00, sizeof(nvruim_sid_nid_type));

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->sid_nid.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_ptr    = (byte *) &sid_nid_buf;
  nvruim_cmd_ptr->data.data_len    = SID_NID_REC_SIZE;
  nvruim_cmd_ptr->offset_or_recnum = 1;
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_HOME_SID_NID;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  /* First pair */
  nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][0].sid =
    sid_nid_buf.sid;
  nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][0].nid =
    sid_nid_buf.nid;

  /* Read the rest of records. Please note that NV_MAX_SID_NID
     is currently defined as 1... so, NVRUIM will not attempt
     reading further records (hence following for loop will not
     be entered at all) until value of NV_MX_SIN_NID is modified
     (at compile time) to greater than 1 */
  for(i = 1; i < NV_MAX_SID_NID ; i++)
  {
    nvruim_read_item((byte *) &(sid_nid_buf),
                     SID_NID_REC_SIZE,
                     i+1 /* Record number */,
                     MMGSDI_CDMA_HOME_SID_NID,
                     timer_index,
                     context, nvruim_cmd_ptr);

    if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
    {
      nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][i].sid =
        sid_nid_buf.sid;
      nv_cmd_ptr->data_ptr->sid_nid.pair[NV_CDMA_MIN_INDEX][i].nid =
        sid_nid_buf.nid;
      (void)sid_nid_buf.band_class;
    }
    else
      break;   /* Break at any bad read */
  }
  (void)nvruim_read_sw(op_status, nvruim_cmd_ptr->report_status);

  /* Report NVRUIM_PASS since the first read is good */
  nvruim_cmd_ptr->report_status = NVRUIM_PASS;
  nv_cmd_ptr->status= *op_status;
  return support_status;
} /* nvruim_read_sid_nid */


/*===========================================================================

FUNCTION NVRUIM_READ_MSISDN

DESCRIPTION
Check to see if MSISDN is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_msisdn
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  uint8                  temp_msisdn_buffer[NV_DIR_NUMB_SIZ] = {0x00};/* buffer to parse the MSISDN */
  uint8                  num_bcd_digits                      = 0; /* length of BCD number in MSISDN */
  uint16                 i=0, j = 0;
  nv_ruim_support_status support_status                      = NV_RUIM_SUPPORTS_ITEM;
  uint8                  msisdn_buffer[NVRUIM_MSISDN_SIZE]   = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->dir_number.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_ptr    = msisdn_buffer;
  nvruim_cmd_ptr->data.data_len    = sizeof(msisdn_buffer);
  nvruim_cmd_ptr->offset_or_recnum = 1;
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_MSISDN;
  nvruim_cmd_ptr->cmd_type         = NVRUIM_READ;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  if(nv_cmd_ptr->item == NV_DIR_NUMBER_I)
  {
    /* Copy the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the MDN from the MSISDN
      *
      * The buffer index is based on the size of the MSISDN EF.
      * Copy no more than the lesser of the size of the DN or
      * the destination buffer.
      */
      /* Initialize the temp buffer to character '0' */
      for(i = 0; i < NV_DIR_NUMB_SIZ; i++)
      {
        temp_msisdn_buffer[i] = '0';
      }

      /* this is the number of BCD digits in the MDN that are returned */
      num_bcd_digits = msisdn_buffer[ NVRUIM_MSISDN_ND_INDEX ] & 0x0F;

      /* Determine which (the number of digits or DIR NUM buffer size) is
      larger */
      if(NV_DIR_NUMB_SIZ > num_bcd_digits)
      {
        /* Determine number of digits that are not returned from RUIM */
        j = NV_DIR_NUMB_SIZ - num_bcd_digits;
        /* The MSISDN digit index starts at the first digit. */
        i = 0;
      }
      else /* num_bcd_digits >= NV_DIR_NUMB_SIZ */
      {
        /* The DIR NUM index starts at the first byte. */
        j = 0;
        /* The MSISDN index starts passed the MSnibbles to fill out the DIR
        NUM */
        i = (num_bcd_digits - NV_DIR_NUMB_SIZ);
      } /* end if - the number of bcd digits is less than the DIR NUMBER
      size. */

      /* Copy the digits from the MSISDN to the temporary DIR NUM buffer.
         Do not exceed either buffer. */
      for(; (i < num_bcd_digits) && (j < NV_DIR_NUMB_SIZ); i++, j++)
      {
        /* Retrieve the digit from the byte - based on whether the index is
        even or odd */
        if(i & 0x1)
        {
          /* Retrieve the MSNibble from the proper byte of the MSISDN */
          temp_msisdn_buffer[j] |=
            ((msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] & 0xF0) >> 4);
        }
        else /* The digit index is even */
        {
          /* Retrieve the LSnibble from the proper byte of the MSISDN */
          temp_msisdn_buffer[j] |=
            (msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] & 0x0F);
        } /* end if - the index into the MSISDN buffer is odd. */

        /* Convert the '0' of the MDN. */
        if(temp_msisdn_buffer[j] == 0x3A)
        {
          temp_msisdn_buffer[j] = '0';
        } /* end if - digit is '0'. */
        /* Convert the '*' of the MDN. */
        if(temp_msisdn_buffer[j] == 0x3B)
        {
           temp_msisdn_buffer[j] = '*';
        } /* end if - digit is '*'. */
        /* Convert the '#' of the MDN. */
        if(temp_msisdn_buffer[j] == 0x3C)
        {
          temp_msisdn_buffer[j] = '#';
        } /* end if - digit is '#'. */
      } /* end for - process all of the digits */

      /* copy the contents of the temp msisdn buffer */
      nvruim_memscpy( (void *) (nv_cmd_ptr->data_ptr->dir_number.dir_number),
                       sizeof(nv_cmd_ptr->data_ptr->dir_number.dir_number),
                       (const void *)temp_msisdn_buffer,
                       NV_DIR_NUMB_SIZ);

    } /* end if - the read was successful. */
  }
  else if(nv_cmd_ptr->item == NV_DIR_NUMBER_PCS_I)
  {
    /* Copy the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* -----------------------------------------------------
      * Copy the MDN from the MSISDN
      *
      * The buffer index is based on the size of the MSISDN EF.
      * Copy no more than the lesser of the size of the DN or
      * the destination buffer.
      * Initialize the temp buffer to character pad digits
      --------------------------------------------------------*/
      for(i = 0; i < NV_DIR_NUMB_PCS_SIZ; i++)
      {
        nv_cmd_ptr->data_ptr->mob_dir_number.digitn[i] = 0xf;
      }
      /* this is the number of BCD digits in the MDN that are returned */
      num_bcd_digits = msisdn_buffer[ NVRUIM_MSISDN_ND_INDEX ] & 0x0F;

      /* Copy the digits from the MSISDN to the DIR NUM buffer.
      Do not exceed either buffer. */
      for(i = 0; (i < num_bcd_digits); i++)
      {
        /* Retrieve the digit from the byte based on whether the index is
           even or odd */
        if(i & 0x1)
        {
          /* Retrieve the MSNibble from the proper byte of the MSISDN */
          nv_cmd_ptr->data_ptr->mob_dir_number.digitn[i] =
            ((msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] & 0xF0) >> 4);
        }
        else /* The digit index is even */
        {
          /* Retrieve the LSnibble from the proper byte of the MSISDN */
          nv_cmd_ptr->data_ptr->mob_dir_number.digitn[i] =
            (msisdn_buffer[ (i/2) + NVRUIM_MSISDN_MDN_INDEX ] & 0x0F);
        } /* end if - index is odd */
      } /* end for - copy all the digits to the DIR NUM buffer */

      /* Give the number of digits read from the card. */
      nv_cmd_ptr->data_ptr->mob_dir_number.n_digits = num_bcd_digits;
    } /* end if - the read was successful. */
  }
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_msisdn */


/*===========================================================================
FUNCTION NVRUIM_READ_ANALOG_CHAN_PREF

DESCRIPTION
Check to see ANALOG CHAN PREF item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_analog_chan_pref
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
     This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->analog_firstchp.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }


  nvruim_cmd_ptr->data.data_len     = NVRUIM_SIZE(analog_firstchp.channel);
  nvruim_cmd_ptr->data.data_ptr     =
      (byte *) &(nv_cmd_ptr->data_ptr->analog_firstchp.channel);
  nvruim_cmd_ptr->offset_or_recnum  = (word) FPOS(nvruim_analog_ch_pref, analog_firstchp);
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_ANALOG_CHAN_PREF;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);
  nv_cmd_ptr->status = *op_status;
  return support_status;
} /* nvruim_read_analog_chan_pref */


/*===========================================================================

FUNCTION NVRUIM_READ_HOME_SID

DESCRIPTION
Check to see if HOME SID is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_home_sid
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->analog_home_sid.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }


  nvruim_cmd_ptr->data.data_len     = NVRUIM_SIZE(analog_home_sid.sid);
  nvruim_cmd_ptr->data.data_ptr     =
    (byte *) &(nv_cmd_ptr->data_ptr->analog_home_sid.sid);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_ANALOG_HOME_SID;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);
  nv_cmd_ptr->status = *op_status;
  return support_status;
} /* nvruim_read_home_sid */


/*===========================================================================

FUNCTION NVRUIM_READ_ACCOLC

DESCRIPTION
Check to see if ACOLC is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_accolc
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
     This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->accolc.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nv_cmd_ptr->data_ptr->accolc.ACCOLCpClass[NV_CDMA_MIN_INDEX]);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_ACCOLC;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  /* Copy to ACCOLC */
  nv_cmd_ptr->data_ptr->accolc.ACCOLCpClass[NV_ANALOG_MIN_INDEX] =
  nv_cmd_ptr->data_ptr->accolc.ACCOLCpClass[NV_CDMA_MIN_INDEX];

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_accolc */


/*===========================================================================

FUNCTION NVRUIM_READ_SCI

DESCRIPTION
Check to see if SCI is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_sci
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nv_cmd_ptr->data_ptr->slot_cycle_index);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_SCI;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  nv_cmd_ptr->data_ptr->slot_cycle_index &= 0x07;
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_sci */


/*===========================================================================

FUNCTION NVRUIM_READ_CALL_TERM

DESCRIPTION
Check to see if CALL TERM is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_call_term
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)

{
  nv_ruim_support_status  support_status                 = NV_RUIM_SUPPORTS_ITEM;
  uint8                   nvruim_call_term_mode_pref_buf = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
       This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->mob_term_home.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = &nvruim_call_term_mode_pref_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_CALL_TERM_MODE_PREF;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  if(nv_cmd_ptr->item == NV_MOB_TERM_HOME_I)
  {
    if(NV_DONE_S == *op_status)
    {
      if(nvruim_call_term_mode_pref_buf & 0x04) /* Bit 2 */
      {
        nv_cmd_ptr->data_ptr->mob_term_home.enabled[NV_CDMA_MIN_INDEX]
          = TRUE;
      }
      else
      {
        nv_cmd_ptr->data_ptr->mob_term_home.enabled[NV_CDMA_MIN_INDEX]
          = FALSE;
      }
      nv_cmd_ptr->data_ptr->mob_term_home.enabled[NV_ANALOG_MIN_INDEX] =
        nv_cmd_ptr->data_ptr->mob_term_home.enabled[NV_CDMA_MIN_INDEX];
    } /* end if - the read was successful. */
  }

  else if(nv_cmd_ptr->item == NV_MOB_TERM_FOR_SID_I)
  {
    if(NV_DONE_S == *op_status)
    {
      if(nvruim_call_term_mode_pref_buf & 0x02) /* Bit 1 */
      {
        nv_cmd_ptr->data_ptr->mob_term_for_sid.enabled[NV_CDMA_MIN_INDEX]
          = TRUE;
      }
      else
      {
        nv_cmd_ptr->data_ptr->mob_term_for_sid.enabled[NV_CDMA_MIN_INDEX]
          = FALSE;
      }
      nv_cmd_ptr->data_ptr->mob_term_for_sid.enabled[NV_ANALOG_MIN_INDEX] =
        nv_cmd_ptr->data_ptr->mob_term_for_sid.enabled[NV_CDMA_MIN_INDEX];
    } /* end if - the read was successful. */
  }

  else if(nv_cmd_ptr->item == NV_MOB_TERM_FOR_NID_I)
  {
    if(NV_DONE_S == *op_status)
    {
      if(nvruim_call_term_mode_pref_buf & 0x01) /* Bit 0 */
      {
        nv_cmd_ptr->data_ptr->mob_term_for_nid.enabled[NV_CDMA_MIN_INDEX]
            = TRUE;
      }
      else
      {
        nv_cmd_ptr->data_ptr->mob_term_for_nid.enabled[NV_CDMA_MIN_INDEX]
          = FALSE;
      }
      nv_cmd_ptr->data_ptr->mob_term_for_nid.enabled[NV_ANALOG_MIN_INDEX] =
        nv_cmd_ptr->data_ptr->mob_term_for_nid.enabled[NV_CDMA_MIN_INDEX];
    } /* end if - the read was successful. */
  }
  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_call_term */


/*===========================================================================

FUNCTION NVRUIM_READ_HOME_SID_NID

DESCRIPTION
Check to see if HOME_SID_NID is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_home_sid_nid
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  uint16                   i              = 0;
  nvruim_sid_nid_type      sid_nid_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&sid_nid_buf, 0x00, sizeof(nvruim_sid_nid_type));

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->home_sid_nid.nam || 
     NVRUIM_CONTEXT_NOT_VALID(context))
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_ptr    = (byte *) &sid_nid_buf;
  nvruim_cmd_ptr->data.data_len    = SID_NID_REC_SIZE;
  nvruim_cmd_ptr->offset_or_recnum = 1;
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_HOME_SID_NID;
  nvruim_cmd_ptr->cmd_type         = NVRUIM_READ;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  /* Make all sid and nid pairs to 0x00 before assigning values */
  memset(&nv_cmd_ptr->data_ptr->home_sid_nid.pair[0], 0x00, sizeof(nv_cmd_ptr->data_ptr->home_sid_nid.pair));

  /* First pair */
  nv_cmd_ptr->data_ptr->home_sid_nid.pair[0].sid = sid_nid_buf.sid;
  nv_cmd_ptr->data_ptr->home_sid_nid.pair[0].nid = sid_nid_buf.nid;

  /* If the first record was read succesfully, *op_status would not be
     NOT_ACTIVE. At this point, if it is 2G (RUIM) EF, we will go ahead
     and read the rest of the records. However, if it is a 3G (CSIM) EF,
     we queue a search command to MMGSDI (for 3G CSIM) so that we can
     filter out the records having pre-known default value and read only
     the records that do not match that default value pattern so as to save
     some unnecessary read APDUs to and from the card.
     Please NOTE that the optimization is NOT introduced for RUIM because
     the equivalent SEEK command wouldn't really help in reducing the number
     of transactions with the card. SEEK only returns the first record that
     matches the pattern. Moreover, SEEK does not return any data on Phase1
     RUIM cards! */
  if(*op_status != NV_NOTACTIVE_S)
  {
    boolean                           matches_srch_pattern = FALSE;
    uint8                             matched_rec_idx      = 0;
    mmgsdi_data_type                  srch_rsp_data;
    mmgsdi_search_data_type           srch_pattern;
    /* CDMAHOME record is 5 bytes long. First two bytes signify SID,
       next two bytes signify NID and the last byte signifies BAND CLASS.
       Setting 0x00s as default value for SID,NID and BandClass here. */
    uint8                             default_data_buf[]   = {0x00, 0x00, 0x00, 0x00, 0x00};

    srch_rsp_data.data_len = 0;
    srch_rsp_data.data_ptr = NULL;

    srch_pattern.data_len = sizeof(default_data_buf)/sizeof(default_data_buf[0]);
    srch_pattern.data_ptr = default_data_buf;

    /* Make sure the data ptr is NULL. If the following search succeeds and a valid
       number of records matched the search, this data_ptr will be malloc'd in the
       nvruim_mmgsdi_access_cb and the corresponding data_len will also be set
       accordingly */
    nvruim_cmd_ptr->data.data_ptr    = NULL;
    nvruim_cmd_ptr->data.data_len    = 0;
    nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_HOME_SID_NID;

    if(nvruim_session_data_ptr[context]->uim_instr_prot == MMGSDI_UICC)
    {
      /* Perform SEARCH on 3G CSIM EF. We deliberately do not have a
         return for this search API as we can rely on the global
         nvruim_report_status which will contain FAIL status if the
         search failed. */
      nvruim_search_file(nvruim_cmd_ptr,
                         2,  /* Start searching from rec#2 */
                         srch_pattern,
                         timer_index,
                         context,
                         nvruim_cmd_ptr);

      if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
      {
        nvruim_mem_free_null_ok(nvruim_cmd_ptr->data.data_ptr);
        nvruim_cmd_ptr->data.data_ptr = NULL;
        nvruim_cmd_ptr->data.data_len = 0;
        nv_cmd_ptr->status = *op_status;
        return NV_RUIM_SUPPORTS_ITEM;
      }

      /* Copy the search response data from the nvruim access global because that
         global will be overwritten when processing the following READ requests */
      if(nvruim_cmd_ptr->data.data_ptr != NULL &&
         nvruim_cmd_ptr->data.data_len > 0)
      {
        srch_rsp_data.data_ptr = nvruim_mem_malloc(nvruim_cmd_ptr->data.data_len);
        if (srch_rsp_data.data_ptr != NULL)
        {
          srch_rsp_data.data_len = nvruim_cmd_ptr->data.data_len;
          nvruim_memscpy((void*)srch_rsp_data.data_ptr,
                         srch_rsp_data.data_len,
                         (const void*)nvruim_cmd_ptr->data.data_ptr,
                         nvruim_cmd_ptr->data.data_len);
        }
      }
      nvruim_mem_free_null_ok(nvruim_cmd_ptr->data.data_ptr);
      nvruim_cmd_ptr->data.data_ptr = NULL;
      nvruim_cmd_ptr->data.data_len = 0;
    }

    for(i = 2; i <= NV_MAX_HOME_SID_NID; i++)
    {
      matches_srch_pattern = FALSE;

      /* For 3G EF, if the search succeeded, lets skip reading from the card
         the records returned in the search response. It the search failed,
         srch response data ptr will be NULL and we should continue reading
         all the records from the card in that case.
         2G EF, we won't even queue the search request to MMGSDI and
         srch response data ptr will be NULL. We should try reading all the
         records in that case. */
      if(srch_rsp_data.data_ptr != NULL)
      {
        for (matched_rec_idx = 0; matched_rec_idx < srch_rsp_data.data_len; matched_rec_idx++)
        {
          if (srch_rsp_data.data_ptr[matched_rec_idx] == i)
          {
            /* Record number i matched with the record(s) returned by SEARCH. So
               skip reading it. */
            matches_srch_pattern = TRUE;
            break;
          }
        }
        if(matches_srch_pattern == TRUE &&
           sizeof(nv_cmd_ptr->data_ptr->home_sid_nid.pair[i-1]) <= sizeof(default_data_buf))
        {
          /* Skip reading the record from the card and write the default SID
             NIDs instead. Note that the first four bytes in default_data_buf
             correspond to SID and NID.*/
          nvruim_memscpy(&nv_cmd_ptr->data_ptr->home_sid_nid.pair[i-1],
                         sizeof(nv_cmd_ptr->data_ptr->home_sid_nid.pair[i-1]),
                         default_data_buf,
                         sizeof(nv_cmd_ptr->data_ptr->home_sid_nid.pair[i-1]));
          continue;
        }
      }

      /* It was a valid record, read it */
      nvruim_read_item((byte *) &(sid_nid_buf),
                       SID_NID_REC_SIZE,
                       i /* Record number */,
                       MMGSDI_CDMA_HOME_SID_NID,
                       timer_index,
                       context, nvruim_cmd_ptr);

      if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
      {
        nv_cmd_ptr->data_ptr->home_sid_nid.pair[i-1].sid =
          sid_nid_buf.sid;
        nv_cmd_ptr->data_ptr->home_sid_nid.pair[i-1].nid =
          sid_nid_buf.nid;
      }
      else
      {
        nvruim_mem_free_null_ok(srch_rsp_data.data_ptr);
        nv_cmd_ptr->status = *op_status;
        return NV_RUIM_SUPPORTS_ITEM;
      }
    }
    nvruim_mem_free_null_ok(srch_rsp_data.data_ptr);
  }
  (void)nvruim_read_sw(op_status, nvruim_cmd_ptr->report_status);
  /* Report NVRUIM_PASS since the first read is good */
  nvruim_cmd_ptr->report_status = NVRUIM_PASS;
  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_home_sid_nid */


/*===========================================================================

FUNCTION NVRUIM_READ_PRL

DESCRIPTION
Check to see if PRL is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_prl
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status      = NV_RUIM_SUPPORTS_ITEM;
  uint32                 i                   = 0;
  /* RUIM prl size */
  uint16                 pr_list_size        = 0;
  /* First prl size */
  uint16                 pr_list_size1       = 0;
  /* Tracking ruim card offset */
  uint16                 offset              = 0;
  /* Devide a long EF to multiple accessers */
  uint8                  num_of_accesses     = 0;
  /* Remainder */
  uint8                  num_of_bytes_remain = 0;

  /* Validate all sspr p rev version with P REV 1 first.*/
  prl_sspr_p_rev_e_type  sspr_p_rev          = PRL_SSPR_P_REV_1;
  boolean                is_full_validation  = TRUE;
  /* RUIM PRL first 2 bytes */
  uint8                  prl_size[2]         = {0};
  /* Pointer to buffer */
  uint8                 *prl_ptr             = NULL;
  /* RUIM max prl size */
  uint16                 max_prl_size        = 0;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
       This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->roaming_list.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_read_item(prl_size,
                   2,
                   0,
                   MMGSDI_CDMA_MAXIMUM_PRL,
                   timer_index,
                   context, nvruim_cmd_ptr);

  /* Check if the read succeeded. */
  if(nvruim_cmd_ptr->report_status == NVRUIM_PASS)
  {
    max_prl_size = (prl_size[0] << 8) | prl_size[1];
    /* Should not be bigger than buffer */
    if(max_prl_size > NV_ROAMING_LIST_MAX_SIZE)
      max_prl_size = NV_ROAMING_LIST_MAX_SIZE;
  }
  else
  {
    /* Continue to RFprl even failing to read this file */
    max_prl_size = NV_ROAMING_LIST_MAX_SIZE;
  }

  /* Read the PR_LIST_SIZE */
  nvruim_cmd_ptr->data.data_len     = 2;
  nvruim_cmd_ptr->data.data_ptr     = prl_size;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_PRL;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  /* Read the rest of PRL */

  /* Fill up list size */
  pr_list_size = (prl_size[0] << 8) | prl_size[1];

  /* Check to see if the list size is valid */
  if((pr_list_size == 0) || (pr_list_size > max_prl_size))
  {
    nvruim_cmd_ptr->report_status = NVRUIM_FAIL;
    UIM_MSG_LOW_1("RUIM PRL Size = %d", pr_list_size);
    nv_cmd_ptr->status= *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Read the prl to a buffer */
  prl_ptr   = (byte *)nv_cmd_ptr->data_ptr->roaming_list.roaming_list;
  offset    = 0;

  /* 255 byte each read. Figure out how many read needed */
  /* Include tow bytes of PR_LIST_MAX_SIZE */
  num_of_accesses     = (byte) (pr_list_size / MAX_BYTES_PER_ACCESS);
  num_of_bytes_remain = (byte) (pr_list_size % MAX_BYTES_PER_ACCESS);

  /* Read all blocks */
  for(i=0; i<num_of_accesses; i++)
  {
    nvruim_read_item(prl_ptr,
                     MAX_BYTES_PER_ACCESS,
                     offset,
                     MMGSDI_CDMA_PRL,
                     timer_index,
                     context, nvruim_cmd_ptr);

    prl_ptr += MAX_BYTES_PER_ACCESS;
    offset  += MAX_BYTES_PER_ACCESS;

    if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
    {
      nv_cmd_ptr->status = *op_status;
      return NV_RUIM_SUPPORTS_ITEM;
    }
  } /* for */

  if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
  {
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  if(num_of_bytes_remain != 0)
  {
    /* The last read of the prl */
    nvruim_read_item(prl_ptr,
                     num_of_bytes_remain,
                     offset,
                     MMGSDI_CDMA_PRL,
                     timer_index,
                     context, nvruim_cmd_ptr);
  }

  if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
  {
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* We have read the first prl so we need to update the size here */
  prl_ptr   = (byte *)nv_cmd_ptr->data_ptr->roaming_list.roaming_list;
  /* Check the validity of the first PRL */
  if(PRL_VALID == prl_validate((byte*) prl_ptr, NULL, &sspr_p_rev,
                                is_full_validation))
  {
    /* Fill up list size
    The first 2 byte of the roaming list contains the first prl size */
    nv_cmd_ptr->data_ptr->roaming_list.size  = prl_ptr[0];
    nv_cmd_ptr->data_ptr->roaming_list.size <<= 8;
    nv_cmd_ptr->data_ptr->roaming_list.size |=  prl_ptr[1];

    /* Present the size of the 683A PRL in bits */
    nv_cmd_ptr->data_ptr->roaming_list.size *= 8;

    /* Fill up list id
    The 3rd and 4th byte contains the version information */
    nv_cmd_ptr->data_ptr->roaming_list.prl_version  = prl_ptr[2];
    nv_cmd_ptr->data_ptr->roaming_list.prl_version <<= 0x08;
    nv_cmd_ptr->data_ptr->roaming_list.prl_version |= prl_ptr[3];

    /* Point to the end of first PRL */
    prl_ptr   = (byte *)nv_cmd_ptr->data_ptr->roaming_list.roaming_list
                + pr_list_size;
    /* Remember the first prl size */
    pr_list_size1 = pr_list_size;
  } /* if First PRL is valid */
  else
  {
    pr_list_size1 = 0;
    nv_cmd_ptr->data_ptr->roaming_list.size  = 0;
  }
  /* EF Next Offset */
  offset    = pr_list_size;
  /* Read the the second PR_LIST_SIZE */
  nvruim_read_item(prl_size,
                   2,
                   pr_list_size,
                   MMGSDI_CDMA_PRL,
                   timer_index,
                   context, nvruim_cmd_ptr);


  if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
  {
    /* We had good first PRL so don't fail here, reset to PASS*/
    nvruim_cmd_ptr->report_status = NVRUIM_PASS;
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Fill up list size */
  pr_list_size = (prl_size[0] << 8) | prl_size[1];

  if((pr_list_size == 0)||(pr_list_size > max_prl_size))
  {
    UIM_MSG_HIGH_1("Invalid second RUIM PRL Size = %d", pr_list_size);
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }
  /* Include two bytes of total CRC to prl size */
  pr_list_size += 2;

  /* Check to see if the list size is valid. */
  if((pr_list_size + pr_list_size1) > max_prl_size)
  {
    UIM_MSG_HIGH_1("Invalid second RUIM PRL Size = %d", pr_list_size);
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* 255 byte each read. Figure out how many read needed */
  /* Include tow bytes of PR_LIST_MAX_SIZE */
  num_of_accesses     = (byte) (pr_list_size / MAX_BYTES_PER_ACCESS);
  num_of_bytes_remain = (byte) (pr_list_size % MAX_BYTES_PER_ACCESS);

  /* Read all blocks */
  for(i=0; i<num_of_accesses; i++)
  {
    nvruim_read_item(prl_ptr,
                     MAX_BYTES_PER_ACCESS,
                     offset,
                     MMGSDI_CDMA_PRL,
                     timer_index,
                     context, nvruim_cmd_ptr);

    prl_ptr += MAX_BYTES_PER_ACCESS;
    offset  += MAX_BYTES_PER_ACCESS;

    if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
    {
      /* We had good first PRL so don't fail here */
      nvruim_cmd_ptr->report_status = NVRUIM_PASS;
      nv_cmd_ptr->status = *op_status;
      return NV_RUIM_SUPPORTS_ITEM;
    }
  } /* for */

  if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
  {
    /* We had good first PRL so don't fail here */
    nvruim_cmd_ptr->report_status = NVRUIM_PASS;
    nv_cmd_ptr->status = *op_status;
    return NV_RUIM_SUPPORTS_ITEM;
  }


  if(num_of_bytes_remain != 0)
  {
    /* The last read of the prl */
    nvruim_read_item(prl_ptr,
                     num_of_bytes_remain,
                     offset,
                     MMGSDI_CDMA_PRL,
                     timer_index,
                     context, nvruim_cmd_ptr);
  }
  if(nvruim_cmd_ptr->report_status != NVRUIM_PASS)
  {
    /* We had good first PRL so don't fail here */
    nvruim_cmd_ptr->report_status = NVRUIM_PASS;
  }

  (void)nvruim_read_sw(op_status, nvruim_cmd_ptr->report_status);

  /* Move to Second PRL */
  prl_ptr   = (byte *)nv_cmd_ptr->data_ptr->roaming_list.roaming_list
              + pr_list_size1;
  /* Check the validity of the second PRL */
  if(PRL_VALID == prl_validate((byte*) prl_ptr, NULL,&sspr_p_rev,
                               is_full_validation))
  {
    /* When First PRL is invalid */
    if( 0x00 == pr_list_size1)
    {
      /* Fill up list size
         The first 2 byte of the roaming list contains the first prl size */
      nv_cmd_ptr->data_ptr->roaming_list.size  = prl_ptr[0];
      nv_cmd_ptr->data_ptr->roaming_list.size <<= 8;
      nv_cmd_ptr->data_ptr->roaming_list.size |=  prl_ptr[1];
      /* Present the size of the 683A PRL in bits */
      nv_cmd_ptr->data_ptr->roaming_list.size *= 8;
      /* Fill up list id
         The 3rd and 4th byte contains the version information */
      nv_cmd_ptr->data_ptr->roaming_list.prl_version  = prl_ptr[2];
      nv_cmd_ptr->data_ptr->roaming_list.prl_version <<= 0x08;
      nv_cmd_ptr->data_ptr->roaming_list.prl_version |= prl_ptr[3];
    }
    else
    {
      /* Both are valid */
      pr_list_size = (prl_size[0] << 8) | prl_size[1];
      nv_cmd_ptr->data_ptr->roaming_list.size =  pr_list_size1 + pr_list_size + 2;
      /* Present the size of the 683A PRL in bits */
      nv_cmd_ptr->data_ptr->roaming_list.size *= 8;
    }
  } /* second PRL validation */
  /* When second PRL is invalid then,
     No need to update the PRL length as we already update it for first PRL */

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_prl */


/*===========================================================================

FUNCTION NVRUIM_READ_SPC_ENABLED

DESCRIPTION
Check to see if SPC_ENABLED is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_spc_enabled
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status              = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_otapa_spc_enable_buf = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nv_cmd_ptr->item == NV_OTAPA_ENABLED_I)
  {
    /* Only process the NAM parameters for the RUIM
    This check assumes the first byte of the data contains the NAM */
    if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->otapa_enabled.nam)
    {
      /* Go to NV for this item */
      return NV_RUIM_ITEM_NOT_SUPPORTED;
    }
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = &nvruim_otapa_spc_enable_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_OTAPA_SPC_ENABLE;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  switch(nv_cmd_ptr->item)
  {
    case NV_SPC_CHANGE_ENABLED_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->spc_change_enabled =
          ((nvruim_otapa_spc_enable_buf & NVRUIM_SPC_DISABLE) == 0);
      } /* end if - the read was successful. */
      break;

    case NV_OTAPA_ENABLED_I:
      /* Copy the data only if the read was successful. */
      if(NV_DONE_S == *op_status)
      {
        /* The enable field is set to TRUE when the bit in the EF is set to
          zero */
        nv_cmd_ptr->data_ptr->otapa_enabled.enabled =
            ((nvruim_otapa_spc_enable_buf & NVRUIM_OTAPA_DISABLE) == 0);
      } /* end if - the read was successful. */
      break;
    default:
      break;
  }
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_spc_enabled */


/*===========================================================================

FUNCTION NVRUIM_READ_NAM_LOCK

DESCRIPTION
Check to see if NAM_LOCK is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_nam_lock
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status      = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_nam_lock_buf = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
  This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->nam_lock.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }


  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = & nvruim_nam_lock_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_NAM_LOCK;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->nam_lock.enabled =
      (( nvruim_nam_lock_buf & NVRUIM_NAM_LOCK) == NVRUIM_NAM_LOCK);
  } /* end if - the read was successful. */

  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_nam_lock */


/*===========================================================================

FUNCTION NVRUIM_READ_SVC_PREF

DESCRIPTION
Check to see if SVC_PREF is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_svc_pref
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status        = NV_RUIM_SUPPORTS_ITEM;
  uint8                  temp_service_pref_buf = 0x00;
  uint8                  nvruim_svc_pref_buf  = 0x00;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Only process the NAM parameters for the RUIM
     This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->pref_mode.nam){
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }


  nvruim_cmd_ptr->data.data_len     = sizeof(nvruim_svc_pref_buf);
  nvruim_cmd_ptr->data.data_ptr     = &nvruim_svc_pref_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_SERVICE_PREF;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  switch(nv_cmd_ptr->item)
  {
    case  NV_PREF_MODE_I:
      /* Analog/Digital selection */
      if(NV_DONE_S == *op_status
         && !nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_PREF_MODE_NOT_FROM_RUIM_I, context))
      {
        temp_service_pref_buf = nvruim_svc_pref_buf;
        temp_service_pref_buf &= NVRUIM_PREF_MODE_MASK;

        switch(temp_service_pref_buf)
        {
          case NVRUIM_ANALOG_PREF:
            /* Analog preferred */
            nv_cmd_ptr->data_ptr->pref_mode.mode = NV_MODE_ANALOG_PREF;
            break;
          case NVRUIM_CDMA_PREF:
            /* CDMA preferred */
            nv_cmd_ptr->data_ptr->pref_mode.mode = NV_MODE_AUTOMATIC;
            break;
          case NVRUIM_ANALOG_ONLY:
            /* Analog only */
            nv_cmd_ptr->data_ptr->pref_mode.mode = NV_MODE_ANALOG_ONLY;
            break;
          case NVRUIM_CDMA_ONLY:
            /* CDMA only */
            nv_cmd_ptr->data_ptr->pref_mode.mode = NV_MODE_DIGITAL_ONLY;
            break;
          default:
            nv_cmd_ptr->data_ptr->pref_mode.mode = NV_MODE_AUTOMATIC;
            break;
        }
      }
      break;

    case  NV_SYSTEM_PREF_I:
      /* System A/B selection */
      if(NV_DONE_S == *op_status)
      {
        temp_service_pref_buf = nvruim_svc_pref_buf;
        temp_service_pref_buf &= NVRUIM_SYSTEM_PREF_MASK;

        switch(temp_service_pref_buf)
        {
          case NVRUIM_A_ONLY:
            /* A only */
            nv_cmd_ptr->data_ptr->system_pref.sys = NV_SP_A_ONLY;
            break;
          case NVRUIM_B_ONLY:
            /* B only */
            nv_cmd_ptr->data_ptr->system_pref.sys = NV_SP_B_ONLY;
            break;
          default:
            nv_cmd_ptr->data_ptr->system_pref.sys = NV_SP_STANDARD;
            break;
        }
      }
      break;
      default:
        break;
  }
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_svc_pref */


/*===========================================================================

FUNCTION NVRUIM_READ_TMSI

DESCRIPTION
Check to see if TMSI is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_tmsi
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;
  uint32                 i              = 0;
  nvruim_tmsi_type       nvruim_tmsi_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_tmsi_buf, 0x00, sizeof(nvruim_tmsi_type));

  /* Only process the NAM parameters for the RUIM
    This check assumes the first byte of the data contains the NAM */
  if(NV_RUIM_NAM != nv_cmd_ptr->data_ptr->assigning_tmsi_zone_length.nam)
  {
    /* Go to NV for this item */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }


  nvruim_cmd_ptr->data.data_len     = sizeof(nvruim_tmsi_type);
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_tmsi_buf);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_TMSI;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  switch(nv_cmd_ptr->item)
  {
    case  NV_ASSIGNING_TMSI_ZONE_LEN_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->assigning_tmsi_zone_length.length =
          nvruim_tmsi_buf.length;
      }
      break;

    case  NV_ASSIGNING_TMSI_ZONE_I:
      if(NV_DONE_S == *op_status)
      {
        /* Copy to qword */
        for(i=0; i<8; i++)
        {
          QW_BYTE(nv_cmd_ptr->data_ptr->assigning_tmsi_zone.zone, i) =
            nvruim_tmsi_buf.zone[i];
        }
      }
      break;

    case  NV_TMSI_CODE_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->tmsi_code.code =
          ((uint32)(nvruim_tmsi_buf.code[3]) << 24) |
          ((uint32)(nvruim_tmsi_buf.code[2]) << 16) |
          ((uint32)(nvruim_tmsi_buf.code[1]) << 8)  |
          nvruim_tmsi_buf.code[0];
      }
      break;

    case  NV_TMSI_EXP_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->tmsi_exp_timer.time =
          (nvruim_tmsi_buf.time[2] << 16) |
          (nvruim_tmsi_buf.time[1] << 8)  |
          nvruim_tmsi_buf.time[0];
      }
      break;
    default:
      break;
  }
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_tmsi */


/*===========================================================================

FUNCTION NVRUIM_READ_LOC_REGN_IND

DESCRIPTION
Check to see if LOC REGN IND is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_loc_regn_ind
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status      support_status = NV_RUIM_SUPPORTS_ITEM;
  nvruim_analog_reg_ind_type  nvruim_analog_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_analog_reg_ind_buf, 0x00, sizeof(nvruim_analog_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }


  nvruim_cmd_ptr->data.data_len     = sizeof(nvruim_analog_reg_ind_type);
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_analog_reg_ind_buf);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_ANALOG_LOCN_AND_REGN_IND;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  switch(nv_cmd_ptr->item)
  {
    case  NV_NXTREG_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->nxtreg =
          (nvruim_analog_reg_ind_buf.nxtreg[2] << 16) |
          (nvruim_analog_reg_ind_buf.nxtreg[1] << 8) |
          nvruim_analog_reg_ind_buf.nxtreg[0];
      }
      break;
    case  NV_LSTSID_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->lstsid =
          (nvruim_analog_reg_ind_buf.sid[1] << 8) |
          nvruim_analog_reg_ind_buf.sid[0];
      }
      break;

    case  NV_LOCAID_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->locaid =
          ((nvruim_analog_reg_ind_buf.locaid_pureg[1] & 0x7F) << 8) |
            nvruim_analog_reg_ind_buf.locaid_pureg[0];
      }
      break;

    case  NV_PUREG_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->pureg =
          ((nvruim_analog_reg_ind_buf.locaid_pureg[1] & 0x80) != 0);
      }
      break;
    default:
      break;
  }
  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_loc_regn_ind */


/*===========================================================================

FUNCTION NVRUIM_READ_ZONE_BASED_IND

DESCRIPTION
Check to see if ZONE BASED IND is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_zone_based_ind
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  nvruim_zone_reg_ind_type nvruim_zone_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_zone_reg_ind_buf, 0x00, sizeof(nvruim_zone_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_zone_reg_ind_buf);
  nvruim_cmd_ptr->data.data_len     = sizeof(nvruim_zone_reg_ind_type);
  nvruim_cmd_ptr->offset_or_recnum  = 1;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_ZONE_BASED_REGN_IND;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->zone_list.reg_zone =
     ( nvruim_zone_reg_ind_buf.reg_zone[1]  << 8 ) |
       nvruim_zone_reg_ind_buf.reg_zone[0];
    nv_cmd_ptr->data_ptr->zone_list.reg_zone &= 0x0FFF;

    nv_cmd_ptr->data_ptr->zone_list.sid =
      ( nvruim_zone_reg_ind_buf.sid[1]  << 8  ) |
      nvruim_zone_reg_ind_buf.sid[0];
    nv_cmd_ptr->data_ptr->zone_list.sid &= 0x7FFF;

    nv_cmd_ptr->data_ptr->zone_list.nid =
      ( nvruim_zone_reg_ind_buf.nid[1] << 8) |
      nvruim_zone_reg_ind_buf.nid[0];

    nv_cmd_ptr->data_ptr->zone_list.band_class =
      nvruim_zone_reg_ind_buf.band_class;

    if(nv_cmd_ptr->data_ptr->zone_list.band_class == 0)
    {                                                /* Cellular band */
      /* Derive system A or B from SID */
      if(nv_cmd_ptr->data_ptr->zone_list.sid & 0x01)
        nv_cmd_ptr->data_ptr->zone_list.block_or_sys = NV_CELLULAR_SYS_A;
      else
        nv_cmd_ptr->data_ptr->zone_list.block_or_sys = NV_CELLULAR_SYS_B;
    }
    else
    { /* PCS band and others */
      nv_cmd_ptr->data_ptr->zone_list.block_or_sys =
        (nv_block_or_sys_enum_type) nvruim_zone_reg_ind_buf.freq_block;
    }
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_zone_based_ind */


/*===========================================================================

FUNCTION NVRUIM_READ_SYS_REGN_IND

DESCRIPTION
Check to see if SYS REGN IND is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_sys_regn_ind
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status  support_status = NV_RUIM_SUPPORTS_ITEM;
  nvruim_sys_reg_ind_type nvruim_sys_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_sys_reg_ind_buf, 0x00, sizeof(nvruim_sys_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_ptr    = (byte *) &(nvruim_sys_reg_ind_buf);
  nvruim_cmd_ptr->data.data_len    = sizeof(nvruim_sys_reg_ind_type);
  /* Ignore the size since we only read one entry */
  nvruim_cmd_ptr->offset_or_recnum = 1;
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_SYS_REGN_IND;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->sid_nid_list.sid =
      ( nvruim_sys_reg_ind_buf.sid[1] << 8 ) |
      nvruim_sys_reg_ind_buf.sid[0];
    nv_cmd_ptr->data_ptr->sid_nid_list.sid &= 0x7FFF;

    nv_cmd_ptr->data_ptr->sid_nid_list.nid =
      ( nvruim_sys_reg_ind_buf.nid[1] << 8 ) |
      nvruim_sys_reg_ind_buf.nid[0];

    nv_cmd_ptr->data_ptr->sid_nid_list.band_class =
    nvruim_sys_reg_ind_buf.band_class >> 3;

    if(nv_cmd_ptr->data_ptr->sid_nid_list.band_class == 0)
    {                                                   /* Cellular band */
      /* Derive system A or B from SID */
      if(nv_cmd_ptr->data_ptr->sid_nid_list.sid & 0x01)
        nv_cmd_ptr->data_ptr->sid_nid_list.block_or_sys = NV_CELLULAR_SYS_A;
      else
        nv_cmd_ptr->data_ptr->sid_nid_list.block_or_sys = NV_CELLULAR_SYS_B;
    }
    else
    { /* PCS band or others */
      nv_cmd_ptr->data_ptr->sid_nid_list.block_or_sys =
        (nv_block_or_sys_enum_type) nvruim_sys_reg_ind_buf.freq_block;
    }
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_sys_regn_ind */


/*===========================================================================

FUNCTION NVRUIM_READ_DIST_REGN_IND

DESCRIPTION
Check to see if DIST REGN IND is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_dist_regn_ind
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  nvruim_dist_reg_ind_type nvruim_dist_reg_ind_buf;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_dist_reg_ind_buf, 0x00, sizeof(nvruim_dist_reg_ind_type));

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = sizeof(nvruim_dist_reg_ind_type);
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_dist_reg_ind_buf);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_DIST_BASED_REGN_IND;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->dist_reg.base_lat_reg =
      (nvruim_dist_reg_ind_buf.base_lat_reg[2] << 16) |
      (nvruim_dist_reg_ind_buf.base_lat_reg[1] << 8) |
      nvruim_dist_reg_ind_buf.base_lat_reg[0];

    nv_cmd_ptr->data_ptr->dist_reg.base_long_reg =
      (nvruim_dist_reg_ind_buf.base_long_reg[2] << 16) |
      (nvruim_dist_reg_ind_buf.base_long_reg[1] << 8) |
      nvruim_dist_reg_ind_buf.base_long_reg[0];

    nv_cmd_ptr->data_ptr->dist_reg.reg_dist_reg =
      (nvruim_dist_reg_ind_buf.reg_dist_reg[1] << 8) |
      nvruim_dist_reg_ind_buf.reg_dist_reg[0];
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_dist_regn_ind */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPD

DESCRIPTION
Check to see if 3GPD is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status          = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_3GPD_op_mode_buf = 0x00;
  sys_modem_as_id_e_type as_id                   = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_3GPD_op_mode_buf);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_3GPDOPM;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->ds_qcmip = nvruim_3GPD_op_mode_buf & NVRUIM_3GPDOPM_MASK;
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPDUPPEXT

DESCRIPTION
Check to see if the 3GPDUPPEXT item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_uppext
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status = NV_RUIM_SUPPORTS_ITEM;
  uint16                 src_bit_count  = 0;
  uint8                  num_nai        = 0;
  uint8                  nai_count      = 0;
  uint8                  profile_number = 0;
  uint16                 i              = 0;
  sys_modem_as_id_e_type as_id          = SYS_MODEM_AS_ID_1;
  uint16 nvruim_sip_ext_profile_bit_index[NVRUIM_MAX_NUM_SIP_PROFILES] = {0x00};
  uint8  nvruim_uppext_buf[NVRUIM_MAX_UPPEXT_SIZE]                     = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if((op_status == NULL)||(nv_cmd_ptr == NULL))
  {
    UIM_MSG_ERR_0("Either op_status or nvruim_read_3gpd_uppext is NULL in nvruim_read_3gpd_uppext");
    if(op_status != NULL)
    {
      *op_status = NV_DONE_S;
    }
    return(NV_RUIM_SUPPORTS_ITEM);
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_RUIM_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_data_3gpd_ext_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  if (nv_cmd_ptr->data_ptr->ds_sip_profile.index >= NVRUIM_MAX_NUM_SIP_PROFILES)
  {
    UIM_MSG_ERR_1("Invalid profile number 0x%x",
                  nv_cmd_ptr->data_ptr->ds_sip_profile.index);
    *op_status = NV_FAIL_S;
    return NV_RUIM_SUPPORTS_ITEM;
  }
  nvruim_cmd_ptr->data.data_len     = NVRUIM_MAX_UPPEXT_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_uppext_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_UPPEXT;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    /* The src counter is a "bit pointer" used as a cursor in the buffer */
    src_bit_count = FSIZ(nvruim_uppext_ef_fields, num_nai_field); /* Skip to first profile */
    nai_count = 0;
    /* The number of NAIs is specified in bits 0-4 of the file */
    num_nai = (nvruim_uppext_buf[0] & NVRUIM_UPPEXT_NUM_NAI_MASK) >> 4;

    /* Clear out the buffer containing ptrs to SIP Profiles */
    for(i = 0; i < NVRUIM_MAX_NUM_SIP_PROFILES; i++)
    {
      nvruim_sip_ext_profile_bit_index[i] = NVRUIM_NO_PROFILE_INDEX_SET;
    } /* end for */

    while(nai_count < num_nai)
    {
      profile_number = b_unpackb(nvruim_uppext_buf, src_bit_count,
                                 FSIZ(nvruim_uppext_ef_fields, profile_num_field));
      if (profile_number >=  NVRUIM_MAX_NUM_SIP_PROFILES)
      {
        UIM_MSG_ERR_1("Invalid profile number 0x%x", profile_number);
        *op_status = NV_FAIL_S;
        return NV_RUIM_SUPPORTS_ITEM;
      }
      nvruim_sip_ext_profile_bit_index[profile_number] = src_bit_count;
      src_bit_count += FSIZ(nvruim_uppext_ef_fields, profile_num_field) +
                      FSIZ(nvruim_uppext_ef_fields, applications_field) +
                      FSIZ(nvruim_uppext_ef_fields, priority_field) +
                      FSIZ(nvruim_uppext_ef_fields, data_rate_mode_field) +
                      FSIZ(nvruim_uppext_ef_fields, data_bearer_field);
      nai_count++;
    } /* end while */

    /* Copy Profile Info into NV structure */
    if(nvruim_sip_ext_profile_bit_index[nv_cmd_ptr->data_ptr->ds_sip_profile.index]
                      != NVRUIM_NO_PROFILE_INDEX_SET)
    {
      /* set src_bit_count to point to requested profile */
      src_bit_count = nvruim_sip_ext_profile_bit_index[nv_cmd_ptr->data_ptr->ds_sip_profile.index];
      /* skip over nai_index */
      src_bit_count += FSIZ(nvruim_uppext_ef_fields, profile_num_field);
      /* Applications field */
      nv_cmd_ptr->data_ptr->ds_sip_profile.app_type =
          b_unpackd(nvruim_uppext_buf, src_bit_count,
                    FSIZ(nvruim_uppext_ef_fields, applications_field));
      src_bit_count += FSIZ(nvruim_uppext_ef_fields, applications_field);
      /* Priority Field */
      nv_cmd_ptr->data_ptr->ds_sip_profile.app_priority =
      b_unpackb(nvruim_uppext_buf, src_bit_count,
                FSIZ(nvruim_uppext_ef_fields, priority_field));
      src_bit_count += FSIZ(nvruim_uppext_ef_fields, priority_field);
      /* Data Rate Mode Field */
      nv_cmd_ptr->data_ptr->ds_sip_profile.data_rate_mode =
      b_unpackb(nvruim_uppext_buf, src_bit_count,
                FSIZ(nvruim_uppext_ef_fields, data_rate_mode_field));
      src_bit_count += FSIZ(nvruim_uppext_ef_fields, data_rate_mode_field);
      /* Data Bearer */
      nv_cmd_ptr->data_ptr->ds_sip_profile.data_bearer =
      b_unpackb(nvruim_uppext_buf, src_bit_count,
                FSIZ(nvruim_uppext_ef_fields, data_bearer_field));
      src_bit_count += FSIZ(nvruim_uppext_ef_fields, data_bearer_field);

    } /* Requested Profile Found */
    else
    {
      UIM_MSG_HIGH_0("Reqeusted SIP NAI not found on card");
    }
  } /* end if - NV_DONE_S == *op_status */
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_uppext */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPDSIPUPP

DESCRIPTION
Check to see if the 3GPDSIPUPP item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_sipupp
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status       = NV_RUIM_SUPPORTS_ITEM;
  uint16                 src_bit_count        = 0;
  uint8                  num_nai              = 0;
  uint8                  nai_count            = 0;
  uint8                  nai_length           = 0;
  uint8                  nai_length_shortened = 0;
  uint8                  profile_number       = 0;
  uint16                 i                    = 0;
  uint16 nvruim_sip_profile_bit_index[NVRUIM_MAX_NUM_SIP_PROFILES] = {0x00};
  uint8  nvruim_sipupp_buf[NVRUIM_MAX_SIPUPP_SIZE]                 = {0x00};
  sys_modem_as_id_e_type as_id                = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if((nv_cmd_ptr == NULL) || (op_status == NULL))
  {
    return(NV_RUIM_SUPPORTS_ITEM);
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_RUIM_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  if ((nv_cmd_ptr->item == NV_DS_SIP_NAI_INFO_I) &&
      (nv_cmd_ptr->data_ptr->ds_sip_nai_info.index >= NVRUIM_MAX_NUM_SIP_PROFILES))
  {
    UIM_MSG_ERR_1("Invalid profile number 0x%x",
                  nv_cmd_ptr->data_ptr->ds_sip_nai_info.index);
    *op_status = NV_FAIL_S;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = NVRUIM_MAX_SIPUPP_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_sipupp_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_SIPUPP;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    /* If EF DATA Has been refreshed, our Buffer must be reset */
    /* The src counter is a "bit pointer" used as a cursor in the buffer */
    src_bit_count = FSIZ(nvruim_sipupp_ef_fields, length_of_block_field) +
                    FSIZ(nvruim_sipupp_ef_fields, num_nai_field); /* Skip to first profile */
    nai_count = 0;
    /* The number of NAIs is specified in bits 9-12 of the file */
    num_nai = (nvruim_sipupp_buf[1] & NVRUIM_SIPUPP_NUM_NAI_MASK) >> 4;

    /* Clear out the buffer containing ptrs to SIP Profiles */
    for(i = 0; i < NVRUIM_MAX_NUM_SIP_PROFILES; i++)
    {
      nvruim_sip_profile_bit_index[i] = NVRUIM_NO_PROFILE_INDEX_SET;
    } /* end for */

    while(nai_count < num_nai)
    {
      profile_number = b_unpackb(nvruim_sipupp_buf, src_bit_count,
                                 FSIZ(nvruim_sipupp_ef_fields, profile_num_field));
      if (profile_number >= NVRUIM_MAX_NUM_SIP_PROFILES)
      {
        UIM_MSG_ERR_1("Invalid profile number 0x%x", profile_number);
        *op_status = NV_FAIL_S;
        return NV_RUIM_SUPPORTS_ITEM;
      }
      nvruim_sip_profile_bit_index[profile_number] = src_bit_count;
      src_bit_count += FSIZ(nvruim_sipupp_ef_fields, profile_num_field);
      nai_length = b_unpackb(nvruim_sipupp_buf, src_bit_count,
                             FSIZ(nvruim_sipupp_ef_fields, nai_length_field));
      src_bit_count += FSIZ(nvruim_sipupp_ef_fields, nai_length_field) +
                      (nai_length * 8) +
                      FSIZ(nvruim_sipupp_ef_fields, auth_algo_field);
      nai_count++;
    } /* end while */

    switch(nv_cmd_ptr->item)
    {
      case NV_PPP_USER_ID_I:
        /* LEGACY CASE:  When Multiple SIP Profiles is NOT supported,
        DATA will ask for a legacy NV item, which is NV_PPP_USER_ID_I.
        This is always mapped to the index stored in
        nvruim_preferred_sip_profile.  For backwards compatibility,
        we still support returning this value when that NV item is read */
        if(nvruim_sip_profile_bit_index[nvruim_preferred_sip_profile]
                      != NVRUIM_NO_PROFILE_INDEX_SET)
          {
            src_bit_count = nvruim_sip_profile_bit_index[nvruim_preferred_sip_profile];
            src_bit_count += FSIZ(nvruim_sipupp_ef_fields, profile_num_field);
            nai_length = b_unpackb(nvruim_sipupp_buf, src_bit_count,
                             FSIZ(nvruim_sipupp_ef_fields, nai_length_field));
            src_bit_count += FSIZ(nvruim_sipupp_ef_fields, nai_length_field);
            if(nai_length > NV_MAX_AN_PPP_USER_ID_LENGTH)
            {
              /* Make sure that the NAI on the card is not longer than
              What the NV structure can handle.  If it is, truncate it */
              nai_length_shortened = NV_MAX_AN_PPP_USER_ID_LENGTH;
            }
            else
            {
              nai_length_shortened = nai_length;
            }
            nv_cmd_ptr->data_ptr->ppp_user_id.user_id_len = nai_length_shortened;
            b_copy((void*) nvruim_sipupp_buf, src_bit_count,
               (void*) nv_cmd_ptr->data_ptr->ppp_user_id.user_id, 0,
               nai_length_shortened * 8);
          }
          else
          {
             UIM_MSG_HIGH_0("Preferred SIP NAI not found on card");
             nv_cmd_ptr->data_ptr->ppp_user_id.user_id_len = 0;
          }
        break;
      case NV_DS_SIP_NUM_VALID_PROFILES_I:
        /* Skip to NUM Profile Parameter */
        nai_count = b_unpackb(nvruim_sipupp_buf,
                           FSIZ(nvruim_sipupp_ef_fields, length_of_block_field),
                           FSIZ(nvruim_sipupp_ef_fields, num_nai_field));
        nv_cmd_ptr->data_ptr->ds_sip_num_valid_profiles = nai_count;
        break;
      case NV_DS_SIP_NAI_INFO_I:
        if(nvruim_sip_profile_bit_index[nv_cmd_ptr->data_ptr->ds_sip_nai_info.index]
                    != NVRUIM_NO_PROFILE_INDEX_SET)
        {
          src_bit_count = nvruim_sip_profile_bit_index[nv_cmd_ptr->data_ptr->ds_sip_nai_info.index];
          src_bit_count += FSIZ(nvruim_sipupp_ef_fields, profile_num_field);
          nai_length = b_unpackb(nvruim_sipupp_buf, src_bit_count,
                           FSIZ(nvruim_sipupp_ef_fields, nai_length_field));
          src_bit_count += FSIZ(nvruim_sipupp_ef_fields, nai_length_field);
          if(nai_length > NV_MAX_AN_PPP_USER_ID_LENGTH)
          {
            /* Make sure that the NAI on the card is not longer than
            What the NV structure can handle.  If it is, truncate it */
            nai_length_shortened = NV_MAX_AN_PPP_USER_ID_LENGTH;
          }
          else
          {
              nai_length_shortened = nai_length;
          }
          nv_cmd_ptr->data_ptr->ds_sip_nai_info.nai_length = nai_length_shortened;
          b_copy((void*) nvruim_sipupp_buf, src_bit_count,
             (void*) nv_cmd_ptr->data_ptr->ds_sip_nai_info.nai, 0,
             nai_length_shortened * 8);
        }
        else
        {
           UIM_MSG_HIGH_0("Reqeusted SIP NAI not found on card");
           nv_cmd_ptr->data_ptr->ds_sip_nai_info.nai_length = 0;
        }
        break;
      default:
        UIM_MSG_HIGH_0("Unsupported NV ITEM Cannot be handled in nvruim_read_3gpd_sipupp");
        return NV_RUIM_ITEM_NOT_SUPPORTED;
    }
  } /* end if - NV_DONE_S == *op_status */
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_sipupp */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPDMIPUPP

DESCRIPTION
Check to see if the 3gPDMIPUPP item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_mipupp
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status       = NV_RUIM_SUPPORTS_ITEM;
  uint16                 src_bit_count        = 0;
  uint8                  num_nai              = 0;
  uint8                  nai_length           = 0;
  uint8                  nai_length_shortened = 0;
  uint8                  nai_count            = 0;
  uint8                  profile_number       = 0;
  uint8                  i                    = 0;
  uint16 nvruim_mip_profile_bit_index[NVRUIM_MAX_NUM_MIP_PROFILES] = {0x00};
  uint8  nvruim_mipupp_buf[NVRUIM_MAX_MIPUPP_SIZE]                 = {0x00};
  sys_modem_as_id_e_type as_id                = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_NV_SIP_RUIM == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len    = NVRUIM_MAX_MIPUPP_SIZE;
  nvruim_cmd_ptr->data.data_ptr    = (byte *)nvruim_mipupp_buf;
  nvruim_cmd_ptr->offset_or_recnum = 0;
  nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_3GPD_MIPUPP;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    /* In this case, the bit pointer/index array is no longer valid, and
    the following structures may not contain valid data:
    nvruim_mip_profile_bit_index
    */

    /* The src counter is a "bit pointer" used as a cursor in the buffer */

    if((nvruim_mipupp_buf[1] & NVRUIM_MIPUPP_RETRY_INCL_MASK) >>
       NVRUIM_RETRY_INFO_PRESENT_SHIFT)
    {
      /* Retry info is included in this EF */
      /* Skip to first profile */
      src_bit_count = FSIZ(nvruim_mipupp_ef_fields, length_of_block_field) +
                  FSIZ(nvruim_mipupp_ef_fields, retry_info_incl_field) +
                  FSIZ(nvruim_mipupp_ef_fields, max_num_retry_field) +
                  FSIZ(nvruim_mipupp_ef_fields, retry_timeout_field) +
                  FSIZ(nvruim_mipupp_ef_fields, rereg_threshold_field) +
                  FSIZ(nvruim_mipupp_ef_fields, num_nai_field);
      num_nai = b_unpackb(nvruim_mipupp_buf,
                      FSIZ(nvruim_mipupp_ef_fields, length_of_block_field) +
                      FSIZ(nvruim_mipupp_ef_fields, retry_info_incl_field) +
                      FSIZ(nvruim_mipupp_ef_fields, max_num_retry_field) +
                      FSIZ(nvruim_mipupp_ef_fields, retry_timeout_field) +
                      FSIZ(nvruim_mipupp_ef_fields, rereg_threshold_field),
                      FSIZ(nvruim_mipupp_ef_fields, num_nai_field));
    } /* endif - retry info is included */
    else
    {
      src_bit_count = FSIZ(nvruim_mipupp_ef_fields, length_of_block_field) +
                  FSIZ(nvruim_mipupp_ef_fields, retry_info_incl_field) +
                  FSIZ(nvruim_mipupp_ef_fields, num_nai_field);

      num_nai = b_unpackb(nvruim_mipupp_buf,
                      FSIZ(nvruim_mipupp_ef_fields, length_of_block_field) +
                      FSIZ(nvruim_mipupp_ef_fields, retry_info_incl_field),
                      FSIZ(nvruim_mipupp_ef_fields, num_nai_field));
    }

    nai_count = 0;

    /* Clear out the buffer containing ptrs to MIP Profiles */
    for(i = 0; i < NVRUIM_MAX_NUM_MIP_PROFILES; i++)
    {
      nvruim_mip_profile_bit_index[i] = NVRUIM_NO_PROFILE_INDEX_SET;
    } /* end for */

    while(nai_count < num_nai)
    {
      profile_number = b_unpackb(nvruim_mipupp_buf, src_bit_count,
                             FSIZ(nvruim_mipupp_ef_fields, nai_entry_index_field));
      if (profile_number >= NVRUIM_MAX_NUM_MIP_PROFILES)
      {
        UIM_MSG_ERR_1("Invalid profile number 0x%x", profile_number);
        *op_status = NV_FAIL_S;
        return NV_RUIM_SUPPORTS_ITEM;
      }
      nvruim_mip_profile_bit_index[profile_number] = src_bit_count;

      src_bit_count += FSIZ(nvruim_mipupp_ef_fields, nai_entry_index_field);
      nai_length = b_unpackb(nvruim_mipupp_buf, src_bit_count,
                         FSIZ(nvruim_mipupp_ef_fields, nai_length_field));

      /* Skip past this profile */
      src_bit_count += ((nai_length * 8) +
                    FSIZ(nvruim_mipupp_ef_fields, nai_length_field) +
                    FSIZ(nvruim_mipupp_ef_fields, tunnel_bit_field) +
                    FSIZ(nvruim_mipupp_ef_fields, home_addr_field) +
                    FSIZ(nvruim_mipupp_ef_fields, primary_ha_field) +
                    FSIZ(nvruim_mipupp_ef_fields, secondary_ha_field) +
                    FSIZ(nvruim_mipupp_ef_fields, mn_aaa_auth_algo_field));
      if(b_unpackb(nvruim_mipupp_buf, src_bit_count,
               FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_indicator_field)))
      {
        /* The MN-HA Indicator is set */
        src_bit_count += (FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_indicator_field) +
                      FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_field) +
                      FSIZ(nvruim_mipupp_ef_fields, mn_ha_auth_algo_field));
      }
      else
      {
        src_bit_count+= (FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_indicator_field) +
                     FSIZ(nvruim_mipupp_ef_fields, mn_ha_auth_algo_field));
      }
      if(b_unpackb(nvruim_mipupp_buf, src_bit_count,
               FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_indicator_field)))
      {
        /* The MN-AAA Indicator is set */
        src_bit_count += (FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_indicator_field) +
                      FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_field));
      }
      else
      {
         src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_indicator_field);
      }
      nai_count++;
    } /* end while */
  }

  switch(nv_cmd_ptr->item)
  {
    case  NV_DS_MIP_NUM_PROF_I:
      if(NV_DONE_S == *op_status)
      {
        if((nvruim_mipupp_buf[1] & NVRUIM_MIPUPP_RETRY_INCL_MASK) >>
           NVRUIM_RETRY_INFO_PRESENT_SHIFT)
        {
          /* Retry Info is included in this EF */
          /* Skip to NUM Profile Parameter */
          nai_count = b_unpackb(nvruim_mipupp_buf,
                                (FSIZ(nvruim_mipupp_ef_fields, length_of_block_field) +
                                 FSIZ(nvruim_mipupp_ef_fields, retry_info_incl_field) +
                                 FSIZ(nvruim_mipupp_ef_fields, max_num_retry_field) +
                                 FSIZ(nvruim_mipupp_ef_fields, retry_timeout_field) +
                                 FSIZ(nvruim_mipupp_ef_fields, rereg_threshold_field)),
                                 FSIZ(nvruim_mipupp_ef_fields, num_nai_field));
        } /* endif - retry info is included */
        else
        {
          nai_count = b_unpackb(nvruim_mipupp_buf,
                                FSIZ(nvruim_mipupp_ef_fields, length_of_block_field) +
                                FSIZ(nvruim_mipupp_ef_fields, retry_info_incl_field),
                                FSIZ(nvruim_mipupp_ef_fields, num_nai_field));
        }
        nv_cmd_ptr->data_ptr->ds_mip_num_prof = nai_count;
      }
      break;

    case  NV_DS_MIP_GEN_USER_PROF_I:
      if(NV_DONE_S == *op_status)
      {
        profile_number = nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.index;
        if (profile_number >= NVRUIM_MAX_NUM_MIP_PROFILES)
        {
          UIM_MSG_ERR_1("Invalid profile number 0x%x", profile_number);
          *op_status = NV_FAIL_S;
          return NV_RUIM_SUPPORTS_ITEM;
        }
        if(nvruim_mip_profile_bit_index[profile_number] != NVRUIM_NO_PROFILE_INDEX_SET)
        {
          /* The request profile is actually available.  Set the profile buffer */
          src_bit_count = nvruim_mip_profile_bit_index[profile_number] +
                          FSIZ(nvruim_mipupp_ef_fields, nai_entry_index_field);
          nai_length = b_unpackb(nvruim_mipupp_buf, src_bit_count,
                                 FSIZ(nvruim_mipupp_ef_fields, nai_length_field));
          if(nai_length >= NV_MAX_NAI_LENGTH)
          {
             nai_length_shortened = NV_MAX_NAI_LENGTH - 1;
          }
          else
          {
             nai_length_shortened = nai_length;
          }
          nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.nai_length = nai_length_shortened;
          src_bit_count += FSIZ(nvruim_mipupp_ef_fields, nai_length_field);

          b_copy((void*) nvruim_mipupp_buf, src_bit_count,
                 (void*) nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.nai, 0,
                 nai_length_shortened * 8);
          nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.nai[nai_length_shortened] = 0; /* null terminate */
          src_bit_count += nai_length * 8;
          if(b_unpackb(nvruim_mipupp_buf, src_bit_count,
                       FSIZ(nvruim_mipupp_ef_fields, tunnel_bit_field)))
          {
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.rev_tun_pref = TRUE;
          }
          else
          {
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.rev_tun_pref = FALSE;
          }
          src_bit_count+= FSIZ(nvruim_mipupp_ef_fields, tunnel_bit_field);
          nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.home_addr =
          b_unpackd(nvruim_mipupp_buf, src_bit_count,
                    FSIZ(nvruim_mipupp_ef_fields, home_addr_field));
          src_bit_count += FSIZ(nvruim_mipupp_ef_fields, home_addr_field);

          nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.primary_ha_addr =
          b_unpackd(nvruim_mipupp_buf, src_bit_count,
                    FSIZ(nvruim_mipupp_ef_fields, primary_ha_field));
          src_bit_count += FSIZ(nvruim_mipupp_ef_fields, primary_ha_field);

          nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.secondary_ha_addr =
          b_unpackd(nvruim_mipupp_buf, src_bit_count,
                    FSIZ(nvruim_mipupp_ef_fields, secondary_ha_field));
          src_bit_count += FSIZ(nvruim_mipupp_ef_fields, secondary_ha_field);

          src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_aaa_auth_algo_field);
          if(b_unpackb(nvruim_mipupp_buf, src_bit_count,
                       FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_indicator_field)))
          {
            src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_indicator_field);
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.mn_aaa_spi_set = TRUE;
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.mn_aaa_spi =
            b_unpackd(nvruim_mipupp_buf, src_bit_count,
                      FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_field));
            src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_field);
          }
          else
          {
            src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_aaa_spi_indicator_field);
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.mn_aaa_spi_set = FALSE;
          }

          src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_ha_auth_algo_field);

          if(b_unpackb(nvruim_mipupp_buf, src_bit_count,
                       FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_indicator_field)))
          {
            src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_indicator_field);
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.mn_ha_spi_set = TRUE;
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.mn_ha_spi =
            b_unpackd(nvruim_mipupp_buf, src_bit_count,
                      FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_field));
            src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_field);
          }
          else
          {
            src_bit_count += FSIZ(nvruim_mipupp_ef_fields, mn_ha_spi_indicator_field);
            nv_cmd_ptr->data_ptr->ds_mip_gen_user_prof.mn_ha_spi_set = FALSE;
          }
        } /* end if - the index pointed to a valid profile */
        else
        {
          /* Something was wrong - the profile was not found */
          UIM_MSG_HIGH_0("Profile not found in MIPUPP");
          *op_status = NV_NOTACTIVE_S;
        }
      }
      break;

    case NV_DS_MIP_RETRIES_I:
      if(NV_DONE_S == *op_status)
      {
        if((nvruim_mipupp_buf[1] & NVRUIM_MIPUPP_RETRY_INCL_MASK) >>
           NVRUIM_RETRY_INFO_PRESENT_SHIFT)
        {
          /* Retry info is included in this EF */
          nv_cmd_ptr->data_ptr->ds_mip_retries =
            ((nvruim_mipupp_buf[1] & NVRUIM_MIPUPP_MAX_RETRY_MASK) >>
              NVRUIM_MAX_NUM_RETRY_SHIFT);
        } /* endif - retry info is included */
        else
        {
          UIM_MSG_HIGH_0("Retry Info not found in SIPUPP");
          *op_status = NV_NOTACTIVE_S;
        }
      }
      break;

    case NV_DS_MIP_RETRY_INT_I:
      if(NV_DONE_S == *op_status)
      {
        if((nvruim_mipupp_buf[1] & NVRUIM_MIPUPP_RETRY_INCL_MASK) >>
           NVRUIM_RETRY_INFO_PRESENT_SHIFT)
        {
          /* Retry info is included in this EF */
          nv_cmd_ptr->data_ptr->ds_mip_retry_int =
              ((nvruim_mipupp_buf[1] & NVRUIM_MIPUPP_RETRY_INT_MASK) >>
               NVRUIM_MIP_RETRY_INT_SHIFT);
        } /* endif - retry info is included */
        else
        {
          UIM_MSG_HIGH_0("Retry Info not found in SIPUPP");
          *op_status = NV_NOTACTIVE_S;
        }
      }
      break;

    case NV_DS_MIP_PRE_RE_RRQ_TIME_I:
      if(NV_DONE_S == *op_status)
      {
        if((nvruim_mipupp_buf[1] & NVRUIM_MIPUPP_RETRY_INCL_MASK) >>
           NVRUIM_RETRY_INFO_PRESENT_SHIFT)
        {
          /* Retry info is included in this EF */
          nv_cmd_ptr->data_ptr->ds_mip_pre_re_rrq_time =
          b_unpackb(nvruim_mipupp_buf,
                    FSIZ(nvruim_mipupp_ef_fields, length_of_block_field) +
                    FSIZ(nvruim_mipupp_ef_fields, retry_info_incl_field) +
                    FSIZ(nvruim_mipupp_ef_fields, max_num_retry_field) +
                    FSIZ(nvruim_mipupp_ef_fields, retry_timeout_field),
                    FSIZ(nvruim_mipupp_ef_fields, rereg_threshold_field));
        } /* endif - retry info is included */
        else
        {
          UIM_MSG_HIGH_0("Retry Info not found in SIPUPP");
          *op_status = NV_NOTACTIVE_S;
        }
      }
      break;
    default:
      break;
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_mipupp */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPD_MIPFLAGS

DESCRIPTION
Check to see if the 3GPD MIP FLAGS item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_mipflags
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status              = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_3gpd_mipflags_ef_buf = 0x00;
  sys_modem_as_id_e_type as_id                       = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if((nv_cmd_ptr == NULL) || (op_status == NULL))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  /* 1. First check if 3GPD MIP support should come from NV.  If so, fallback.
     2. Next, if UIM support is indicated, make sure that the service is actually
        present, and it isn't just that neither MIP nor fallback is supported.  If so,
        just return RUIM Support and DONE (which will block a fallback).
     3. Lastly, check to see if 3GPD Extensions support is present, if not, allow
     this item to fallback */

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_NV_SIP_RUIM == uim_3gpd_support_per_sub(as_id))
  {
    UIM_MSG_HIGH_0("Card does not support 3GPD, falling back to NV");
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_data_3gpd_ext_support_per_sub(as_id))
  {
    if(nv_cmd_ptr->item == NV_DS_MIP_2002BIS_MN_HA_AUTH_I)
    {
      /* If this item is not supported on the card, and the ME is using RUIM for MIP Auth,
         then only 2002BIS is supported */
      *op_status = NV_DONE_S;
      nv_cmd_ptr->data_ptr->ds_mip_2002bis_mn_ha_auth = TRUE;
      return NV_RUIM_SUPPORTS_ITEM;
    }

    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }


  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_3gpd_mipflags_ef_buf);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_MIPFLAGS;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  switch(nv_cmd_ptr->item)
  {
    case  NV_DS_MIP_2002BIS_MN_HA_AUTH_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->ds_mip_2002bis_mn_ha_auth = nvruim_3gpd_mipflags_ef_buf & 0x01;
      }
      break;

    case  NV_DS_MIP_QC_DRS_OPT_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->ds_mip_qc_drs_opt =
            (nvruim_3gpd_mipflags_ef_buf >>  NVRUIM_MIPFLAGS_DRS_OPT_SHIFT) & 0x01;
      }
      break;

    case NV_DS_MIP_QC_HANDDOWN_TO_1X_OPT_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->ds_mip_qc_handdown_to_1x_opt =
            (nvruim_3gpd_mipflags_ef_buf >> NVRUIM_MIPFLAGS_HANDDOWN_SHIFT) & 0x01;
      }
      break;

    case NV_DS_MIP_RRQ_IF_TFRK_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->ds_mip_rrq_if_tfrk =
            (nvruim_3gpd_mipflags_ef_buf >> NVRUIM_MIPFLAGS_RRQ_IF_TRFK_SHIFT) & 0x01;
      }
      break;
    default:
      break;
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_mipflags */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPD_TCP_CONFIG

DESCRIPTION
Check to see if the 3GPD TCP CONFIG items are supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_tcp_config
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status                                        = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_3gpd_tcp_config_buf[NVRUIM_MAX_TCPCONFIG_SIZE] = {0x00};
  sys_modem_as_id_e_type as_id                                                 = SYS_MODEM_AS_ID_1;

  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  /* 1. First check if 3GPD MIP support should come from NV.  If so, fallback.
     2. Next, if UIM support is indicated, make sure that the service is actually
        present, and it isn't just that neither MIP nor fallback is supported.  If so,
        just return RUIM Support and DONE (which will block a fallback).
     3. Lastly, check to see if 3GPD Extensions support is present, if not, allow
     this item to fallback */

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_NV_SIP_RUIM == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_data_3gpd_ext_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }


  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = NVRUIM_MAX_TCPCONFIG_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_3gpd_tcp_config_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_TCPCONFIG;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }
  /*
        Byte Index  Description
        1           TCP Flags
        2           TCP Keep Alive Idle Timer
  */
  switch(nv_cmd_ptr->item)
  {
    case  NV_TCP_GRACEFUL_DORMANT_CLOSE_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->tcp_graceful_dormant_close = nvruim_3gpd_tcp_config_buf[0] & 0x01;
      }
      break;
    case  NV_TCP_KEEPALIVE_IDLE_TIME_I:
      if(NV_DONE_S == *op_status)
      {
        nv_cmd_ptr->data_ptr->tcp_keepalive_idle_time = nvruim_3gpd_tcp_config_buf[1];
      }
      break;
    default:
      break;
  }/* End of switch(nv_cmd_ptr->item)*/
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_tcp_config */


#ifdef FEATURE_UIM_SUPPORT_LBS
/*===========================================================================

FUNCTION NVRUIM_READ_LBS_XTRA_SERVER_URLS

DESCRIPTION
Check to see if the LBS XTRA SERVER URLS File Data Items are supported by the
R-UIM. If it is supported, read it.

DEPENDENCIES
Non-reentrant
TLV should be in proper and valid format in EF. Wrongly written data will not be
read and may be generate useless information.
This code is only support the maximum length of 3 bytes for length parameter and
if the value of length is greater than 128 byte,
then only 128 byte will be return and remaining value will be drop.

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED   : Item is not supported
NV_RUIM_SUPPORTS_ITEM        : Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_lbs_xtra_server_urls
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status    = NV_RUIM_SUPPORTS_ITEM;
  uint16                 ef_offset         = 0;
/* Taking 4 byte TLV Data Buffer, Assuming that Length will never exseed 0xFFFF
   value, so We need 1 byte for Tag, 3 byte information for length for example
   0x80 0x82 0xFF 0xFF */
  byte                   tl_data_buffer[4] = {0xFF,0x00,0x00,0x00};
  byte                  *v_data_buffer     = NULL;
/* As the file have 3 TLV structure, to read any URL TLV we may need to jump to
   reach to the specified TLV Data.
   For the Tertiary XTRA Server url_occurance value will be 0x03.
   To read the 3rd URL TLV, we need to jump 2 times.
   For the Seconday XTRA Server url_occurance value will be 0x02.
   To read the 2nd URL TLV we need to jump 1 times.
   For the Primary XTRA Server url_occurance value will be 0x01.
   No jump require we read the current TLV. */
  byte                     url_occurance  = 0;
  nvruim_session_enum_type nvruim_session = nvruim_map_context_to_nvruim_session(context);

/*   Checking for Null Value   */
  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) ||
     (nv_cmd_ptr->data_ptr == NULL) || (op_status == NULL) ||
     NVRUIM_CONTEXT_NOT_VALID(context) ||
     (nvruim_session_data_ptr[context] == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }/* End of if((nv_cmd_ptr == NULL) || (op_status == NULL))*/

  /* Checking of rtre configuration, RTRE should initialized  */
  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  /*  Checking of Boolean nvruim_lbs_support, it indicate whether n24 is
  is supported or not
  Checking the RTRE Mode whether it is NV Mode or not  */
  if((!nvruim_session_data_ptr[context]->nvruim_lbs_support) ||
     (NV_RTRE_CONTROL_NO_RUIM == nvruim_get_rtre_control_value(nvruim_session)))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }
  /* Preparing command to read data form the file
   we only need the 4 byte to get the first TLV Data */
  nvruim_cmd_ptr->data.data_len     = 0x04;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)tl_data_buffer;
  nvruim_cmd_ptr->offset_or_recnum  = ef_offset;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_LBS_XSURL;
  support_status        = nvruim_read_and_check_rsp(nv_cmd_ptr,
                                                    op_status,
                                                    timer_index,
                                                    context,
                                                    nvruim_cmd_ptr);
  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr     = NULL;
  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  /* Reading of 4 bytes information from file is successful.
     Now we are setting the url_occurance and data buffer to access the
     requested XTRA server Url.
     As we are not caching the entire data for URL so every time
     we need to read the file again */
  switch(nv_cmd_ptr->item)
  {
    case NV_GPS1_XTRA_PRIMARY_SERVER_URL_I:
      if(NV_DONE_S == *op_status)
      {
        url_occurance = NVRUIM_LBS_XTRA_URL_FIRST;
        v_data_buffer = (byte *)&(nv_cmd_ptr->data_ptr->gps1_xtra_primary_server_url[0x00]);
      }
      break;

    case NV_GPS1_XTRA_SECONDARY_SERVER_URL_I:
      if(NV_DONE_S == *op_status)
      {
        url_occurance = NVRUIM_LBS_XTRA_URL_SECOND;
        v_data_buffer = (byte *)&(nv_cmd_ptr->data_ptr->gps1_xtra_secondary_server_url[0x00]);
      }
      break;

    case NV_GPS1_XTRA_TERTIARY_SERVER_URL_I:
      if(NV_DONE_S == *op_status)
      {
        url_occurance = NVRUIM_LBS_XTRA_URL_THIRD;
        v_data_buffer = (byte *)&(nv_cmd_ptr->data_ptr->gps1_xtra_tertiary_server_url[0x00]);
      }
      break;

    default:
      url_occurance = 0x00;
      UIM_MSG_ERR_1("NV Item not found in nvruim_read_lbs_xtra_server_urls 0x%x",
                    nv_cmd_ptr->item);
      break;

  }/* End of switch(nv_cmd_ptr->item) */

  /* url_occurance should be greater than zero.
     when the value of url_occurance will become 0x01
     that means we reach the requested url
     else we need to jump to the next url.
  */
  while(url_occurance)
  {
    uint16 length_value=0x00;
    /* Start parsing the TLV Information     */
    /* If the tag is not 0x80, that means it is invalid,
       break no need to continue */
    /* Folowing combination of Length Information are allowed
     Length-Byte  Range
        1         0x00-0x7F
        2         0x81 0x80 -- 0xFF
        3         0x82 0x100 -- 0xXXXX
     */
    /* Checking for invalid Tag Value, Max Length Value and Invalid Length
       Information*/
    if ((NVRUIM_LBS_XTRA_URL_TAG != tl_data_buffer[0]) ||
        (NVRUIM_MAX_LBS_URL_TAG_LEN_INFO < tl_data_buffer[1]) ||
        (NVRUIM_INVALID_LBS_URL_TAG_LEN_INFO == tl_data_buffer[1]))
    {
      /* Invalid value and No need to read the data */
      /* Break the loop and we are also not able to jump to the next Tag */
      break;
    }
    /* Tag and length is valid, now we calculate the length of Url Information)*/
    switch (tl_data_buffer[1])
    {
      case NVRUIM_LBS_XTRA_URL_LEN_TAG_82:
        /* in case when url tag is
           0x80   (TAG)
           0x82 0x01 0x00  (Length Information) */
        length_value = tl_data_buffer[2];
        length_value <<= 0x08;
        length_value |= tl_data_buffer[3];
        ef_offset = ef_offset + 0x04; /* 3 bytes for length and 1 for tag */
        break;

      case NVRUIM_LBS_XTRA_URL_LEN_TAG_81:
        /* in case when url tag is
           0x80 (TAF)
           0x81 0x80 (Length Information)*/
        length_value = tl_data_buffer[2];
        ef_offset = ef_offset + 0x03;/* 2 bytes for length and 1 for tag */
        break;

      default:
        length_value = tl_data_buffer[1];
        ef_offset = ef_offset + 0x02;/* 1 byte for length and 1 for tag */
    }/* End of  switch (tl_data_buffer[1])*/

    if (url_occurance == 0x01)
    {
      /* No jump require we got the url tag and now we only need to read the url
      information (Value part of the TLV) from file  */
      if (length_value > NVRUIM_MAX_LBS_XTRA_SERVER_URL_LENGTH )
      {
        /* MAXIMUM only 128 URL Lenght is supported so read only 128 byte data*/
        length_value = NVRUIM_MAX_LBS_XTRA_SERVER_URL_LENGTH;
      }

      if (length_value > 0x00)
      {
        nvruim_read_item(v_data_buffer, length_value, ef_offset,
                         MMGSDI_CDMA_LBS_XSURL, timer_index,
                         context, nvruim_cmd_ptr);
        /* No need to check the status of the reading, because after
           this step uim_occuranc will become 0x00 and loop will be Terminate */
      }/*End of if (length_value > 0x00) */
    }/*End of IF (url_occurance == 0x01)*/
    else
    {
      /* Need to jump for next URL  */
      ef_offset= ef_offset + length_value;
      /* Intialization the TL value, It is required to break the loop,
         if the file does not have the data */
      tl_data_buffer[0] = 0xFF;
      tl_data_buffer[1] = 0x00;
      tl_data_buffer[2] = 0x00;
      tl_data_buffer[3] = 0x00;
      nvruim_read_item(tl_data_buffer, 0x04, ef_offset,
                       MMGSDI_CDMA_LBS_XSURL, timer_index,
                       context, nvruim_cmd_ptr);
      /* No need to check the status of the reading,
         for example if the file does not have any data then the 0xFF tag value
         will break the loop in next step. */
    }/*end of else */
    /* Decrement the url_occurance by one  */
    url_occurance--;
  }/* End of  while(url_occurance)  */
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
}/* nvruim_read_lbs_xtra_server_urls */


/*===========================================================================

FUNCTION NVRUIM_READ_LBS_XTRA_CONFIG

DESCRIPTION
Check to see if the LBS XTRA Configuration File Data Items are supported by the
R-UIM. If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_lbs_xtra_config
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  uint8                    nvruim_lbs_xtra_config_buf[NVRUIM_MAX_LBS_XTRA_CONFIG_SIZE] = {0x00};
  nvruim_session_enum_type nvruim_session = nvruim_map_context_to_nvruim_session(context);

  /*   Checking for Null Value   */
  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     (NVRUIM_CONTEXT_NOT_VALID(context))         ||
     (nvruim_session_data_ptr[context] == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
  /* Checking of retre configuration, RTRE should initialized  */
  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  /*  Checking of Boolean nvruim_lbs_support, it indicate whether n24 is
  is supported or not
  Checking the RTRE Mode whether it is NV Mode or not  */
  if((!nvruim_session_data_ptr[context]->nvruim_lbs_support) ||
     (NV_RTRE_CONTROL_NO_RUIM == nvruim_get_rtre_control_value(nvruim_session)))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }
 /* Preparing command to read data form the file */
  nvruim_cmd_ptr->data.data_len     = NVRUIM_MAX_LBS_XTRA_CONFIG_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_lbs_xtra_config_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_LBS_XCONFIG;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  switch(nv_cmd_ptr->item)
  {
    /* LBS Xtra Configuration File
    Bytes       Description                        M/O    Length
    1           XTRA Flags                         M    1 byte
    2           gpsOneXTRA Download Interval       M    1 byte
    3           gpsOneXTRA Download Retries        M    1 byte
    4           gpsOneXTRA Download Retry Interval M    1 byte
    XTRA Flags  Coding (0 - disabled; 1 - enabled):
    Bit     Parameter Indicated
    1       Allow gpsOneXTRA    [Default Value: 1 - enabled]
    2       Allow gpsOneXTRA Automatic Download [Default Value: 0 - disabled]
    3-8     Reserved for future use

    gpsOneXTRA Download Interval coding :8-bit integer. Range: 1 to 168
    (168 hours is a week).[Default Value:24 hours]

    gpsOneXTRA Download Retries - Coding: 8-bit integer. Range: 0 to 10.
    [Default Values: 3 retries]

    gpsOneXTRA Download Retry Interval - Coding: 8-bit integer. Range: 1 to 120.
    [Default Value: 10 minutes]
    */
    case  NV_GPS1_XTRA_ENABLED_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 1st bit of 1byte (0th offset) of EF LBSXTRA Buffer */
        nv_cmd_ptr->data_ptr->gps1_xtra_enabled =
            (nvruim_lbs_xtra_config_buf[0] & 0x01)? TRUE : FALSE;
      }/* end of if (NV_DONE_S == *op_status)*/
      break;

    case  NV_GPS1_XTRA_AUTO_DOWNLOAD_ENABLED_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 2nd bit of 1byte (0th offset) of EF LBSXTRA Buffer */
        nv_cmd_ptr->data_ptr->gps1_xtra_auto_download_enabled =
                  (nvruim_lbs_xtra_config_buf[0] & 0x02)? TRUE : FALSE;

      }/* end of if (NV_DONE_S == *op_status)*/
      break;

    case  NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I :
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 2byte (1st offset) of EF LBSXTRA Buffer */
        if (nvruim_lbs_xtra_config_buf[1] > MAX_VALUE_NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I)
        {
          nv_cmd_ptr->data_ptr->gps1_xtra_download_interval = MAX_VALUE_NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I;
        }
        else
        {
          nv_cmd_ptr->data_ptr->gps1_xtra_download_interval = nvruim_lbs_xtra_config_buf[1];
        }
      }/* end of if (NV_DONE_S == *op_status)*/
      break;

    case  NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 3byte (2nd offset) of EF LBSXTRA Buffer */
        if(nvruim_lbs_xtra_config_buf[2] > MAX_VALUE_NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I)
        {
          nv_cmd_ptr->data_ptr->gps1_xtra_num_download_attempts = MAX_VALUE_NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I;
        }
        else
        {
          nv_cmd_ptr->data_ptr->gps1_xtra_num_download_attempts = nvruim_lbs_xtra_config_buf[2];
        }
      }/* end of if (NV_DONE_S == *op_status)*/
      break;

    case  NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 4byte (3rd offset) of EF LBSXTRA Buffer */
        if(nvruim_lbs_xtra_config_buf[3] > MAX_VALUE_NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I)
        {
          nv_cmd_ptr->data_ptr->gps1_xtra_time_between_attempts = MAX_VALUE_NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I;
        }
        else
        {
          nv_cmd_ptr->data_ptr->gps1_xtra_time_between_attempts = nvruim_lbs_xtra_config_buf[3];
        }
      }/* end of if (NV_DONE_S == *op_status)*/
      break;

    default:
      UIM_MSG_ERR_1("NV Item not found in nvruim_read_lbs_xtra_config 0x%x",
                    nv_cmd_ptr->item);
      break;
  }/* end of switch(nv_cmd_ptr->item) */
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
}  /* nvruim_read_lbs_xtra_config */


/*===========================================================================

FUNCTION NVRUIM_READ_LBS_V2_CONFIG

DESCRIPTION
Check to see if the LBS V2 Configuration File Data Items are supported by the
R-UIM. If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    : Item is not supported
NV_RUIM_SUPPORTS_ITEM         : Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_lbs_v2_config
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  uint8                    nvruim_lbs_v2_config_buf[NVRUIM_MAX_LBS_V2_CONFIG_SIZE] = {0x00};
  nvruim_session_enum_type nvruim_session = nvruim_map_context_to_nvruim_session(context);

  /*   Checking for Null Value   */
  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     (NVRUIM_CONTEXT_NOT_VALID(context))         ||
     (nvruim_session_data_ptr[context] == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
 /* Checking of retre configuration, RTRE should initialized  */
  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  /*  Checking of Boolean nvruim_lbs_support, it indicate whether n24 is
  is supported or not
  Checking the RTRE Mode whether it is NV Mode or not  */
  if((!nvruim_session_data_ptr[context]->nvruim_lbs_support) ||
     (NV_RTRE_CONTROL_NO_RUIM == nvruim_get_rtre_control_value(nvruim_session)))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }
 /* Preparing command to read data form the file */
  nvruim_cmd_ptr->data.data_len     = NVRUIM_MAX_LBS_V2_CONFIG_SIZE ;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_lbs_v2_config_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_LBS_V2CONFIG;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  switch(nv_cmd_ptr->item)
  {
    /* Data Generic Configuration File
    Bytes Description                M/O        Length
    1    V2 LBS Flags                M        1 byte
    "V2 LBS Flags  Coding (0 - disabled; 1 - enabled):
    Bit  Parameter Indicated
    1    Allow Sending System Parameter Info Message [Default Value: 0 - disabled]
    2    Allow gpsOne Seed Position Use [Default Value: 1 - enabled]
    3    Allow gpsOne Dynamic Mode [Default Value: 1 - enabled]
    4-8  Reserved for future use
    */
    case  NV_GPS1_CAPABILITIES_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 1st bit of 1byte (0th offset) of EF LBSV2 Buffer */
        nv_cmd_ptr->data_ptr->gps1_capabilities = nvruim_lbs_v2_config_buf[0]& 0x01;
      }
      break;

    case  NV_GPS1_SEEDPOS_OPTION_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 2nd bit of 1byte (0th offset) of EF LBSV2 Buffer */
        nv_cmd_ptr->data_ptr->gps1_seedpos_option =
        (nvruim_lbs_v2_config_buf[0] >> NVRUIM_LBS_FLAG_GPS1_SEED_POSITION_SHIFT)& 0x01;
      }
      break;

    case  NV_GPS1_DYNAMIC_MODE_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 3rd bit of 1byte (0th offset) of EF LBSV2 Buffer */
        nv_cmd_ptr->data_ptr->gps1_dynamic_mode =
        (nvruim_lbs_v2_config_buf[0] >>NVRUIM_LBS_FLAG_GPS1_DYNAMIC_MODE_SHIFT )& 0x01;
      }
      break;

    default:
      UIM_MSG_ERR_1("NV Item not found in nvruim_read_lbs_v2_config 0x%x",
                    nv_cmd_ptr->item);
      break;
  }/* end of switch(nv_cmd_ptr->item) */
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
}  /* nvruim_read_lbs_v2_config */


/*===========================================================================

FUNCTION NVRUIM_READ_LBS_V2_PDE_ADDRESS

DESCRIPTION
Check to see if the LBS V2 PDE AddressFile Data Items are supported by the
R-UIM. If it is supported, read it.

DEPENDENCIES
Non-reentrant
Assuming this is the Transparent File

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED   : Item is not supported
NV_RUIM_SUPPORTS_ITEM        : Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_lbs_v2_pde_adress
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status                  = NV_RUIM_SUPPORTS_ITEM;
  uint8                    lbs_pde_address_type            = 0x00;
  uint8                    lbs_pde_address_length          = 0x00;
  uint8                    port_offset                     = 0x00;
  nvruim_session_enum_type nvruim_session = nvruim_map_context_to_nvruim_session(context);
  uint8 nvruim_lbs_v2_pde_address_buf[NVRUIM_MAX_LBS_V2_PDE_ADDRESS_SIZE] = {0x00};

  /*   Checking for Null Value   */
  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     (NVRUIM_CONTEXT_NOT_VALID(context))         ||
     (nvruim_session_data_ptr[context] == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
  if (NULL == nv_cmd_ptr->data_ptr)
  {
    UIM_MSG_ERR_1("NV Item Data_ptr is null for NV Item 0x%x",
                  nv_cmd_ptr->item);
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
  /* Checking of retre configuration, RTRE should initialized  */
  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  /*  Checking of Boolean nvruim_lbs_support, it indicate whether n24 is
  is supported or not
  Checking the RTRE Mode whether it is NV Mode or not  */
  if((!nvruim_session_data_ptr[context]->nvruim_lbs_support) ||
     (NV_RTRE_CONTROL_NO_RUIM == nvruim_get_rtre_control_value(nvruim_session)))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }
  /* Data Structure of the File
    Bytes            Description                    M/O        Length
    1                V2 LBS PDE Address Type        M          1 byte
                                                               1: Domain Name
                                                               2: IPV4 Address
                                                               3: IPV6 Address
    2                V2 LBS PDE Address Length      M          1 byte
    3 to X+2         V2 LBS PDE Address Information M          X bytes
    X+3 to X+4       V2 LBS PDE Port Number         M          2 bytes */

  /* Preparing command to read entire data form the file */
  nvruim_cmd_ptr->data.data_len     =  NVRUIM_MAX_LBS_V2_PDE_ADDRESS_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_lbs_v2_pde_address_buf;
  nvruim_cmd_ptr->offset_or_recnum  =  0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_LBS_V2PDEADDR;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }
  /* Reading address type infromation from 0th offset of ef buffer */
  lbs_pde_address_type = nvruim_lbs_v2_pde_address_buf[0x00];

  /* Reading address length infromation from 1st offset of ef buffer */
  lbs_pde_address_length = nvruim_lbs_v2_pde_address_buf[0x01];

  /* Update the port offset */
  port_offset =  lbs_pde_address_length + 2;

  switch(nv_cmd_ptr->item)
  {
    case NV_GPS1_PDE_ADDRESS_I:
      if(NV_DONE_S == *op_status)
      {
        /*If this is requested, the value from the card should be returned
          if the address type is �2� (IPV4).
          If the address type is something else, we should fill this with 0�s.
          The Address length should not be more than 15 bytes.
        */
        if (lbs_pde_address_length > NVRUIM_MAX_LBS_IPV4_ADDRESS_LENGTH)
        {
          /* We are only supporting 4 byte information and ignoring rest of data */
          lbs_pde_address_length = NVRUIM_MAX_LBS_IPV4_ADDRESS_LENGTH;
        }

        if(lbs_pde_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV4)
        {
          /* nvruim_lbs_v2_pde_address_buf[2] is pointing to the data */
          nv_cmd_ptr->data_ptr->gps1_pde_address =
          nvruim_get_ipv4_digit_char_string(&nvruim_lbs_v2_pde_address_buf[2],
                                            lbs_pde_address_length);
        }/* End of If (lbs_pde_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV4) */
      }/* end of  if(NV_DONE_S == *op_status)*/
      break;

    case NV_GPS1_PDE_PORT_I:
      if(NV_DONE_S == *op_status)
      {
        /* To prevent buffer overflow, port number should be returned only
           when LBS V2 PDE Address is less than 128 bytes*/
        if(port_offset < NVRUIM_MAX_LBS_V2_PDE_ADDRESS_SIZE - 1)
        {
          /* Reading 2nd and 3 byte of the EF LBSV2 PDE ADDRESSS Buffer */
          nv_cmd_ptr->data_ptr->gps1_pde_port =
                       nvruim_lbs_v2_pde_address_buf[port_offset];
          /* Left Sift to the higher byte */
          nv_cmd_ptr->data_ptr->gps1_pde_port <<= 0x08;
          /* Adding 3 byte of the the EF LBSV2 PDE ADDRESSS Buffer */
          nv_cmd_ptr->data_ptr->gps1_pde_port |=
                       nvruim_lbs_v2_pde_address_buf[port_offset + 0x01];
        }
        else
        {
          UIM_MSG_ERR_0("LBS V2 PDE address longer than 128 bytes");
          return NV_RUIM_ITEM_NOT_SUPPORTED;
        } /* End of If (port_offset < NVRUIM_MAX_LBS_V2_PDE_ADDRESS_SIZE - 1)*/
      }
      break;

    case NV_CGPS_1X_PDE_SERVER_ADDR_IPV4_I:
      if(NV_DONE_S == *op_status)
      {
        /*If this is requested, the value from the card should be returned
          if the address type is �2� (IPV4).
          If the address type is something else, we should fill this with 0�s.
          The Address length should not be more than 15 bytes.      */
        if (lbs_pde_address_length > NVRUIM_MAX_LBS_IPV4_ADDRESS_LENGTH)
        {
          /* We are only supporting 15 byte information and ignoring rest of data */
          lbs_pde_address_length = NVRUIM_MAX_LBS_IPV4_ADDRESS_LENGTH;
        }

        if(lbs_pde_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV4)
        {
          /* nvruim_lbs_v2_pde_address_buf[2] is pointing to the data */
          nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_ipv4 =
          nvruim_get_ipv4_digit_char_string(&nvruim_lbs_v2_pde_address_buf[2],
                                            lbs_pde_address_length);
        }/* End of If (lbs_pde_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV4) */
      }/*end of if(NV_DONE_S == *op_status)*/
      break;

    case NV_CGPS_1X_PDE_SERVER_ADDR_IPV6_I:
      if(NV_DONE_S == *op_status)
      {
        /* If this is requested, the value from the card should be returned
           if the address type is �3� (IPV6).
           If the address type is something else, we should fill this with 0�s.
           The Address length should not be more than 39 bytes */
        if (lbs_pde_address_length > NVRUIM_MAX_LBS_IPV6_ADDRESS_LENGTH)
        {
           /* We are only supporting 39 byte information and ignoring rest data */
           lbs_pde_address_length = NVRUIM_MAX_LBS_IPV6_ADDRESS_LENGTH;
        }

        if(lbs_pde_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV6)
        {
          /* 3 offset is pointing to the data */
          if(FALSE ==
             nvruim_get_ipv6_hex_char_string(&nvruim_lbs_v2_pde_address_buf[2],
               lbs_pde_address_length,
               (uint16 *)(&nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_ipv6[0]))
            )
          {
            UIM_MSG_ERR_0("Invalid IPv6 address");
            memset((uint16 *)(nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_ipv6),
                   0x0000,sizeof(nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_ipv6));
          }
        }/* End of if((lbs_pde_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV6) */
      }/* end of  if(NV_DONE_S == *op_status)*/
      break;

    case NV_CGPS_1X_PDE_SERVER_ADDR_URL_I:
      if(NV_DONE_S == *op_status)
      {
        /* If this is requested, the value from the card should be returned
        if the address type is �1� (Domain Name).
        If the address type is something else, we should just insert a NULL character.
        The address length can be anything, as long as it is less than 128 bytes.
        It should be null terminated. */
        memset((uint8 *)(nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_url),
             0x00, sizeof(nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_url));
        if (lbs_pde_address_length > NVRUIM_MAX_LBS_DOMAIN_ADDRESS_LENGTH)
        {
          /* We are only supporting 128 byte information and ignoring
          rest of data */
          lbs_pde_address_length = NVRUIM_MAX_LBS_DOMAIN_ADDRESS_LENGTH;
        }
        if(lbs_pde_address_type == NVRUIM_LBS_ADDRESS_TYPE_DOMAIN_NAME)
        {
          /* copy the ef buffer data to mpc server url */
          nvruim_memscpy((void *)(nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_url),
                          sizeof(nv_cmd_ptr->data_ptr->cgps_1x_pde_server_addr_url),
                          (const void *)(&nvruim_lbs_v2_pde_address_buf[2]),
                          lbs_pde_address_length);

        }/* end of if((lbs_pde_address_type ==
                                       NVRUIM_LBS_ADDRESS_TYPE_DOMAIN_NAME)*/
      }/* end of if(NV_DONE_S == *op_status) */
      break;

    case NV_CGPS_1X_PDE_SERVER_PORT_I:
      if(NV_DONE_S == *op_status)
      {
        /* To prevent buffer overflow, port number should be returned only
           when LBS V2 PDE Address is less than 128 bytes*/
        if(port_offset < NVRUIM_MAX_LBS_V2_PDE_ADDRESS_SIZE - 1)
        {
          /* Reading 2nd and 3 byte of the LBSV2 PDE ADDRESSS Buffer */
          nv_cmd_ptr->data_ptr->cgps_1x_pde_server_port =
                                    nvruim_lbs_v2_pde_address_buf[port_offset];
          /* Left Sift to the higher byte */
          nv_cmd_ptr->data_ptr->cgps_1x_pde_server_port <<= 0x08;
          /* Adding 3 byte of the the EF LBSV2 PDE ADDRESSS Buffer */
          nv_cmd_ptr->data_ptr->cgps_1x_pde_server_port |=
                              nvruim_lbs_v2_pde_address_buf[port_offset + 0x01];
        }
        else
        {
          UIM_MSG_ERR_0("LBS V2 PDE address longer than 128 bytes");
          return NV_RUIM_ITEM_NOT_SUPPORTED;
        } /* End of If (port_offset < NVRUIM_MAX_LBS_V2_PDE_ADDRESS_SIZE - 1)*/
      }
      break;

    default:
      UIM_MSG_ERR_1("NV Item not found in  nvruim_read_lbs_v2_pde_adress 0x%x",
                    nv_cmd_ptr->item);
      break;
   }/* end of switch(nv_cmd_ptr->item) */

  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
}/* end of nvruim_read_lbs_v2_pde_adress */


/*===========================================================================

FUNCTION NVRUIM_READ_LBS_V2_MPC_ADDRESS

DESCRIPTION
Check to see if the LBS V2 MPC AddressFile Data Items are supported by the
R-UIM. If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    : Item is not supported
NV_RUIM_SUPPORTS_ITEM         : Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_lbs_v2_mpc_adress
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status                  = NV_RUIM_SUPPORTS_ITEM;
  uint8                    lbs_mpc_address_type            = 0x00;
  uint8                    lbs_mpc_address_length          = 0x00;
  uint8                    port_offset                     = 0x00;
  nvruim_session_enum_type nvruim_session = nvruim_map_context_to_nvruim_session(context);
  uint8 nvruim_lbs_v2_mpc_address_buf[NVRUIM_MAX_LBS_V2_MPC_ADDRESS_SIZE] = {0x00};

  /*   Checking for Null Value   */
  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     (NVRUIM_CONTEXT_NOT_VALID(context))         ||
     (nvruim_session_data_ptr[context] == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if (NULL == nv_cmd_ptr->data_ptr)
  {
    UIM_MSG_ERR_1("NV Item Data_ptr is null for NV Item 0x%x",
                  nv_cmd_ptr->item);
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Checking of retre configuration, RTRE should initialized  */
  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
  /*  Checking of Boolean nvruim_lbs_support, it indicate whether n24 is
  is supported or not
  Checking the RTRE Mode whether it is NV Mode or not  */
  if((!nvruim_session_data_ptr[context]->nvruim_lbs_support) ||
     (NV_RTRE_CONTROL_NO_RUIM == nvruim_get_rtre_control_value(nvruim_session)))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }
  /* Preparing command to read data form the file */
    /* Data Structure of the File
    Bytes                Description                        M/O       Length
    1                    V2 LBS MPC Address Type            M         1 byte
                                                                      1: Domain Name
                                                                      2: IPV4 Address
                                                                      3: IPV6 Address
    2                    V2 LBS MPC Address Length          M         1 byte
    3 to X+2             V2 LBS MPC Address Information     M         X bytes
    X+3 to X+4           V2 LBS MPC Port Number             M         2 bytes */

  /* Preparing command to read byte data form the file */
  nvruim_cmd_ptr->data.data_len     =  NVRUIM_MAX_LBS_V2_MPC_ADDRESS_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_lbs_v2_mpc_address_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_LBS_V2MPCADDR;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }
  /* Reading address type infromation from 0th offset of ef buffer */
  lbs_mpc_address_type = nvruim_lbs_v2_mpc_address_buf[0x00];
  /* Reading address length infromation from 1st offset of ef buffer */
  lbs_mpc_address_length = nvruim_lbs_v2_mpc_address_buf[0x01];
  /* Update the port offset */
  port_offset =  lbs_mpc_address_length + 2;

  switch(nv_cmd_ptr->item)
  {
    case NV_CGPS_1X_MPC_SERVER_ADDR_IPV4_I:
      if(NV_DONE_S == *op_status)
      {
        /* If these are requested, the value from the card should be returned
           if the address type is �2� (IPV4).
           If the address type is something else, we should fill this with 0�s.
           The Address length should be not more than 15 bytes              */
        nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_ipv4 =0x00000000;
        if (lbs_mpc_address_length > NVRUIM_MAX_LBS_IPV4_ADDRESS_LENGTH)
        {
          /* We are only supporting 15 byte information and ignoring rest of data */
          lbs_mpc_address_length = NVRUIM_MAX_LBS_IPV4_ADDRESS_LENGTH;
        }
        if(lbs_mpc_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV4)
        {
          nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_ipv4 =
          nvruim_get_ipv4_digit_char_string(&nvruim_lbs_v2_mpc_address_buf[2],
                                            lbs_mpc_address_length);
        }/* End of If (lbs_mpc_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV4) */
      }/* End of if(NV_DONE_S == *op_status) */
      break;

    case NV_CGPS_1X_MPC_SERVER_ADDR_IPV6_I:
      if(NV_DONE_S == *op_status)
      {
        /* If these are requested, the value from the card should be returned
        if the address type is �3� (IPV6).
        If the address type is something else, we should fill this with 0�s.
           The Address length should be not more than 39 bytes              */
        if (lbs_mpc_address_length > NVRUIM_MAX_LBS_IPV6_ADDRESS_LENGTH)
        {
          /* We are only supporting 39 byte information and ignoring rest data */
          lbs_mpc_address_length = NVRUIM_MAX_LBS_IPV6_ADDRESS_LENGTH;
        }
        if(lbs_mpc_address_type == NVRUIM_LBS_ADDRESS_TYPE_IPV6)
        {
          if(FALSE ==
             nvruim_get_ipv6_hex_char_string(&nvruim_lbs_v2_mpc_address_buf[2],
               lbs_mpc_address_length,
               (uint16 *)(&nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_ipv6[0]))
            )
          {
            UIM_MSG_ERR_0("Invalid IPv6 address");
            memset((uint16 *)(nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_ipv6),
                    0x0000, sizeof(nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_ipv6));
          }
        }/* End of If */
      }/* end of  if(NV_DONE_S == *op_status)*/
      break;

    case NV_CGPS_1X_MPC_SERVER_ADDR_URL_I:
      if(NV_DONE_S == *op_status)
      {
        /* If these are requested, the value from the card should be returned
           if the address type is �1� (Domain Name).
           If the address type is something else, we should just insert a NULL character.
           The address length can be anything, as long as it is less than 128 bytes.
           It should be null terminated. */
        memset((uint8 *)(nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_url),
                0x00, sizeof(nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_url));
        if(lbs_mpc_address_length > NVRUIM_MAX_LBS_DOMAIN_ADDRESS_LENGTH)
        {
          /* We are only supporting 128 byte information and ignoring rest of data */
          lbs_mpc_address_length = NVRUIM_MAX_LBS_DOMAIN_ADDRESS_LENGTH;
        }
        if(lbs_mpc_address_type == NVRUIM_LBS_ADDRESS_TYPE_DOMAIN_NAME)
        {
          /* copy the ef buffer data to mpc server url */
          nvruim_memscpy((void *)(nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_url),
                          sizeof(nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_addr_url),
                          (const void *)(&nvruim_lbs_v2_mpc_address_buf[2]),
                          lbs_mpc_address_length);

         }/* end of if((lbs_mpc_address_type ==
                                         NVRUIM_LBS_ADDRESS_TYPE_DOMAIN_NAME)*/
      }/* end of if(NV_DONE_S == *op_status) */
      break;

    case NV_CGPS_1X_MPC_SERVER_PORT_I:
      if(NV_DONE_S == *op_status)
      {
        /* To prevent buffer overflow, port number should be returned only
           when LBS V2 MPC Address is less than 128 bytes*/
        if(port_offset < NVRUIM_MAX_LBS_V2_MPC_ADDRESS_SIZE - 1)
        {
          /* Reading 2nd and 3 byte of the F LBSV2 PDE ADDRESSS Buffer */
          nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_port =
                                      nvruim_lbs_v2_mpc_address_buf[port_offset];
          /* Left Sift to the higher byte */
          nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_port <<= 0x08;
          /* Adding 3 byte of the the EF LBSV2 PDE ADDRESSS Buffer */
          nv_cmd_ptr->data_ptr->cgps_1x_mpc_server_port |=
                               nvruim_lbs_v2_mpc_address_buf[port_offset + 0x01];
        }
        else
        {
          UIM_MSG_ERR_0("LBS V2 MPC address longer than 128 bytes");
          return NV_RUIM_ITEM_NOT_SUPPORTED;
        } /* End of If (port_offset < NVRUIM_MAX_LBS_V2_MPC_ADDRESS_SIZE - 1)*/
      }
      break;

    default:
      UIM_MSG_ERR_1("NV Item not found in  nvruim_read_lbs_v2_mpc_adress 0x%x",
                    nv_cmd_ptr->item);
      break;
   }/* end of switch(nv_cmd_ptr->item) */
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
}  /* nvruim_read_lbs_v2_mpc_adress */
#endif /* FEATURE_UIM_SUPPORT_LBS  */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPD_DATA_GENERIC_CONFIG

DESCRIPTION
Check to see if the 3GPD Data Generic CONFIG items are supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_data_generic_config
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status                           = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_3gpd_dgc_buf[NVRUIM_MAX_DGC_SIZE] = {0x00};
  sys_modem_as_id_e_type as_id                                    = SYS_MODEM_AS_ID_1;

  /*   Checking for Null Value   */
  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  /* Checking for control_3gpd Variable,
  It determines whether support for 3GPD should come from the R-UIM or from NV. */

  if (NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }
  /*  Checking of Boolean ext_support_3gpd,
    it indicate whether n15 is supported or not   */
  if(!nvruim_data_3gpd_ext_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }
 /* Checking of retre configuration, RTRE should initialized  */
  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }
 /* Preparing command to read data form the file */
  nvruim_cmd_ptr->data.data_len     = NVRUIM_MAX_DGC_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_3gpd_dgc_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_DGC;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  switch(nv_cmd_ptr->item)
  {
    /* Data Generic Configuration File
    Byte Index  Description
    1           Data Dormant Timer
    2           EPZID Type Information
    3           Hysterisis Activation Timer
    */
    case NV_DSAT707_CTA_TIMER_I:
    if(NV_DONE_S == *op_status)
    {
      /* Reading the 1st byte (0 offset) of the EF DGC 
         According to spec (C.S0065-B v1.0) the value of data dormant timer should be
         atleast 20 sec. So if the value in card is less than 20 sec,
         set the dormant timer to 20 sec. */
      if(nvruim_3gpd_dgc_buf[0] < NVRUIM_MIN_DATA_DORMANT_TIMER_VALUE)
      {
        nv_cmd_ptr->data_ptr->dsat707_cta_timer = NVRUIM_MIN_DATA_DORMANT_TIMER_VALUE;
      }
      else
      {
        nv_cmd_ptr->data_ptr->dsat707_cta_timer = nvruim_3gpd_dgc_buf[0];
      }
    }
    break;
    case  NV_ACTIVE_EPZID_TYPE_I:
    if(NV_DONE_S == *op_status)
    {
      /*  Reading the 2nd byte (1 offset) of the EF DGC  */
      nv_cmd_ptr->data_ptr->active_epzid_type = nvruim_3gpd_dgc_buf[1];
    }
    break;
    case  NV_HYSTERISIS_ACTIVATION_TIMER_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reading the 3rd byte (2nd offset) of the EF DGC */
        nv_cmd_ptr->data_ptr->hysterisis_activation_timer = nvruim_3gpd_dgc_buf[2];
      }
      break;
    default:
      break;
  }/* end of switch(nv_cmd_ptr->item) */

  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
}  /* nvruim_read_3gpd_data_generic_config */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPD_MIPSP

DESCRIPTION
Check to see if the item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_mipsp
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status   = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_mipsp_buf = 0x00;
  sys_modem_as_id_e_type as_id            = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_NV_SIP_RUIM == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_mipsp_buf);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_MIPSP;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }
  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->ds_mip_active_prof = (nvruim_mipsp_buf) >> NVRUIM_MIPSP_SHIFT;
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_mipsp */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPD_SIPSP

DESCRIPTION
Check to see if the item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_sipsp
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status   = NV_RUIM_SUPPORTS_ITEM;
  uint8                  nvruim_sipsp_buf = 0x00;
  sys_modem_as_id_e_type as_id            = SYS_MODEM_AS_ID_1;

  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL))
  {
    return(NV_RUIM_SUPPORTS_ITEM);
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_RUIM_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = (byte *) &(nvruim_sipsp_buf);
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_SIPSP;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }
  if(NV_DONE_S == *op_status)
  {
    nv_cmd_ptr->data_ptr->ds_sip_active_profile_index = (nvruim_sipsp_buf) >> NVRUIM_SIPSP_SHIFT;
  }
  nv_cmd_ptr->status = *op_status;
  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_sipsp */


/*===========================================================================

FUNCTION NVRUIM_READ_3GPDSIPPAPSS

DESCRIPTION
Check to see if the 3GPDSIPPAPSS item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_3gpd_sippapss
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status support_status      = NV_RUIM_SUPPORTS_ITEM;
  uint16                 src_bit_count       = 0;
  uint8                  num_nai             = 0;
  uint8                  nai_count           = 0;
  uint8                  profile_number      = 0;
  uint8                  ss_length           = 0;
  uint8                  ss_length_shortened = 0;
  uint16                 i                   = 0;
  uint16 nvruim_sip_pap_ss_profile_bit_index[NVRUIM_MAX_NUM_SIP_PROFILES] = {0x00};
  uint8  nvruim_sippapss_buf[NVRUIM_MAX_SIPPAPSS_SIZE]                    = {0x00};
  sys_modem_as_id_e_type as_id               = SYS_MODEM_AS_ID_1;

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_3GPD_MIP_NV_SIP_NV == uim_3gpd_support_per_sub(as_id) ||
     NVRUIM_3GPD_MIP_RUIM_SIP_NV==uim_3gpd_support_per_sub(as_id))
  {
    return(NV_RUIM_ITEM_NOT_SUPPORTED);
  }

  if(nv_cmd_ptr->item == NV_PPP_PASSWORD_I)
  {
    nv_cmd_ptr->data_ptr->ppp_password.password_len = 0;
  }
  else if(nv_cmd_ptr->item == NV_DS_SIP_PPP_SS_INFO_I)
  {
    nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss_length = 0;
    if (nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.index >= NVRUIM_MAX_NUM_SIP_PROFILES)
    {
      UIM_MSG_ERR_1("Invalid profile number 0x%x",
                    nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.index);
      *op_status = NV_FAIL_S;
      return NV_RUIM_SUPPORTS_ITEM;
    }
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  nvruim_cmd_ptr->data.data_len     = NVRUIM_MAX_SIPPAPSS_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = (byte *)nvruim_sippapss_buf;
  nvruim_cmd_ptr->offset_or_recnum  = 0;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_3GPD_SIPPAPSS;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return(support_status);
  }

  if(NV_DONE_S == *op_status)
  {
    /* The src counter is a "bit pointer" used as a cursor in the buffer */
    src_bit_count = FSIZ(nvruim_sippapss_ef_fields, length_of_block_field) +
                    FSIZ(nvruim_sippapss_ef_fields, num_nai_field); /* Skip to first profile */
    nai_count = 0;
    /* The number of NAIs is specified in bits 9-12 of the file */
    num_nai = (nvruim_sippapss_buf[1] & NVRUIM_SIPPAPSS_NUM_NAI_MASK) >> 4;

    /* Clear out the buffer containing ptrs to SIP Profiles */
    for(i = 0; i < NVRUIM_MAX_NUM_SIP_PROFILES; i++)
    {
      nvruim_sip_pap_ss_profile_bit_index[i] = NVRUIM_NO_PROFILE_INDEX_SET;
    } /* end for */

    while(nai_count < num_nai)
    {
      profile_number = b_unpackb(nvruim_sippapss_buf, src_bit_count,
                                 FSIZ(nvruim_sippapss_ef_fields, profile_num_field));
      if (profile_number >= NVRUIM_MAX_NUM_SIP_PROFILES)
      {
        UIM_MSG_ERR_1("Invalid profile number 0x%x", profile_number);
        *op_status = NV_FAIL_S;
        return NV_RUIM_SUPPORTS_ITEM;
      }
      nvruim_sip_pap_ss_profile_bit_index[profile_number] = src_bit_count;
      src_bit_count += FSIZ(nvruim_sippapss_ef_fields, profile_num_field);
      ss_length = b_unpackb(nvruim_sippapss_buf, src_bit_count,
                            FSIZ(nvruim_sippapss_ef_fields, ss_length_field));
      src_bit_count += FSIZ(nvruim_sippapss_ef_fields, ss_length_field)
                    + ss_length *8;
      nai_count++;
    } /* end while */

    if((nv_cmd_ptr->item == NV_PPP_PASSWORD_I) &&
       nvruim_sip_pap_ss_profile_bit_index[nvruim_preferred_sip_profile]
                                    != NVRUIM_NO_PROFILE_INDEX_SET)
    {
      src_bit_count = nvruim_sip_pap_ss_profile_bit_index[nvruim_preferred_sip_profile];
      src_bit_count += FSIZ(nvruim_sippapss_ef_fields, profile_num_field);
      ss_length = b_unpackb(nvruim_sippapss_buf, src_bit_count,
                              FSIZ(nvruim_sippapss_ef_fields, ss_length_field));
      src_bit_count += FSIZ(nvruim_sippapss_ef_fields, ss_length_field);
      if(ss_length > NVRUIM_MAX_SIPPAPSS_SS_SIZE)
      {
         ss_length_shortened = NVRUIM_MAX_SIPPAPSS_SS_SIZE;
      }
      else
      {
         ss_length_shortened = ss_length;
      }
      memset((byte *)nv_cmd_ptr->data_ptr->ppp_password.password,
              0x00, sizeof(nv_cmd_ptr->data_ptr->ppp_password.password));
      nv_cmd_ptr->data_ptr->ppp_password.password_len = ss_length_shortened;

      b_copy((void*) nvruim_sippapss_buf, src_bit_count,
               (void*) nv_cmd_ptr->data_ptr->ppp_password.password, 0,
               ss_length_shortened * 8);
    } /* end if - there was valid data in the buffer */
    else if((nv_cmd_ptr->item == NV_DS_SIP_PPP_SS_INFO_I) &&
       nvruim_sip_pap_ss_profile_bit_index
       [nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.index] != NVRUIM_NO_PROFILE_INDEX_SET)
    {
       src_bit_count = nvruim_sip_pap_ss_profile_bit_index
           [nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.index];
       src_bit_count += FSIZ(nvruim_sippapss_ef_fields, profile_num_field);
       ss_length = b_unpackb(nvruim_sippapss_buf, src_bit_count,
                             FSIZ(nvruim_sippapss_ef_fields, ss_length_field));
       if(ss_length > NVRUIM_MAX_SIPPAPSS_SS_SIZE)
       {
          ss_length_shortened = NVRUIM_MAX_SIPPAPSS_SS_SIZE;
       }
       else
       {
          ss_length_shortened = ss_length;
       }
       src_bit_count += FSIZ(nvruim_sippapss_ef_fields, ss_length_field);
       memset((byte *)nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss, 0x00,
              sizeof(nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss));
       nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss_length = ss_length_shortened;
       b_copy((void*) nvruim_sippapss_buf, src_bit_count,
               (void*) nv_cmd_ptr->data_ptr->ds_sip_ppp_ss_info.ss, 0,
               ss_length_shortened * 8);
    }
    else
    {
      /* Something was wrong - the profile was not found */
      UIM_MSG_HIGH_0("Profile not found or invalid NV item in SIPPAPSS");
      *op_status = NV_NOTACTIVE_S;
    } /* end else */
  } /* end if - NV_DONE_S == *op_status */

  nv_cmd_ptr->status = *op_status;

  return(NV_RUIM_SUPPORTS_ITEM);
} /* nvruim_read_3gpd_sippapss */


/*===========================================================================

FUNCTION NVRUIM_READ_HRPD_HRPDUPP

DESCRIPTION
Check to see if the HRPDUPP item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_hrpd_hrpdupp
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status                              = NV_RUIM_SUPPORTS_ITEM;
  nvruim_imsi_type         nvruim_imsi_m_buf;
  uint8                    nvruim_hrpdupp_buf[NVRUIM_MAX_HRPDUPP_SIZE] = {0x00};
  sys_modem_as_id_e_type   as_id                                       = SYS_MODEM_AS_ID_1;
#ifdef FEATURE_UIM_CAVE_AN_AUTH
  uint32                   min1                                        = 0;
  uint16                   min2                                        = 0;
  uint8                    mnc                                         = 0;
  uint16                   mcc                                         = 0;
#endif /* FEATURE_UIM_CAVE_AN_AUTH */

  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  memset(&nvruim_imsi_m_buf, 0x00, sizeof(nvruim_imsi_type));

  if(NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  as_id = nvruim_convert_context_to_as_id(context);

  if(NVRUIM_AN_HRPD_SUPPORT == uim_an_hrpd_support_per_sub(as_id))
  {
    if(!nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_AN_HRPD_FALLBACK_I, context)
       && !nvruim_session_data_ptr[context]->hrpd_cdma_svc)
    {
      if(nv_cmd_ptr->item == NV_HDR_AN_AUTH_USER_ID_LONG_I)
      {
        nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id_len = 0;
      }
      else
      {
        nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai_len = 0;
      }
      *op_status = NV_DONE_S;
      return NV_RUIM_SUPPORTS_ITEM;
    }

    nvruim_cmd_ptr->data.data_len    = NVRUIM_MAX_HRPDUPP_SIZE;
    nvruim_cmd_ptr->data.data_ptr    = (byte *)nvruim_hrpdupp_buf;
    nvruim_cmd_ptr->offset_or_recnum = 0;
    nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_HRPD_HRPDUPP;
  }
  else if(NVRUIM_AN_HRPD_NO_SUPPORT == uim_an_hrpd_support_per_sub(as_id))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }
#ifdef FEATURE_UIM_CAVE_AN_AUTH
  else
  { /* --------------------------------------------------------------
       Use Cave
      If AN Cave Authentication is used (for China Unicom),
      the HDR NAI/User ID should be constucted as <IMSI>@unicomcdma.com.cn
      The following case for IMSI read also applies for AN CAVE NAI read
      Setup the command structure to read the IMSI_M EF from the RUIM
      ----------------------------------------------------------------*/
    nvruim_cmd_ptr->data.data_len    = sizeof(nvruim_imsi_type);
    nvruim_cmd_ptr->data.data_ptr    = (byte *) &(nvruim_imsi_m_buf);
    nvruim_cmd_ptr->file_enum        = MMGSDI_CDMA_IMSI_M;
    /* EF offset  */
    nvruim_cmd_ptr->offset_or_recnum = 0;
  }
#else /* AN CAVE is not defined */
  else
  {
    UIM_MSG_HIGH_0("Error is NVRUIM.  Unknown HRPD preference");
  }
#endif /* AN CAVE is not defined */

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  switch(nv_cmd_ptr->item)
  {
    case NV_HDR_AN_AUTH_USER_ID_LONG_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reformat the data only if the read was successful. */
        if(NVRUIM_AN_HRPD_SUPPORT == uim_an_hrpd_support_per_sub(as_id))
        {
          if(nvruim_hrpdupp_buf[1] > NV_MAX_AN_AUTH_USER_ID_LONG_LENGTH)
          {
            /* If the HRPD User Name is longer than the NV structure can take, truncate it */
            UIM_MSG_HIGH_0("AN Auth Username on card is too long!");
            nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id_len =
              NV_MAX_AN_AUTH_USER_ID_LONG_LENGTH;
            nvruim_memscpy((void *)nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id,
                            sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id),
                            (const void *)&(nvruim_hrpdupp_buf[2]),
                            NV_MAX_AN_AUTH_USER_ID_LONG_LENGTH);
          }
          else
          {
            nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id_len =
            nvruim_hrpdupp_buf[1];
            nvruim_memscpy((void *)nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id,
                            sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id),
                            (const void *)&(nvruim_hrpdupp_buf[2]),
                            nvruim_hrpdupp_buf[1]);
          }
        }
        else
        {
#ifdef FEATURE_UIM_CAVE_AN_AUTH
        /* --------------------------------------------------------------------
           Use CAVE
           Retrive MIN1, MIN2, MCC and MNC and build NAI string here
           Copy the item from the read data buffer.
           ------------------------------------------------------------------*/
          nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id_len =
            NVRUIM_CHINA_UNICOM_AN_CAVE_NAI_LENGTH;
          min1 = (nvruim_imsi_m_buf.imsi_s1[2] << 16) |
                 (nvruim_imsi_m_buf.imsi_s1[1] << 8) |
                 (nvruim_imsi_m_buf.imsi_s1[0]);

          min2 = (nvruim_imsi_m_buf.imsi_s2[1] << 8) |
                  nvruim_imsi_m_buf.imsi_s2[0];
          mnc = nvruim_imsi_m_buf.imsi_11_12;
          mcc = (nvruim_imsi_m_buf.imsi_mcc[1] << 8) |
                 nvruim_imsi_m_buf.imsi_mcc[0];

         /* Copy generic China Unicom AN Cave NAI format:
            <000000000000000>@unicomcdma.com.cn */
          nvruim_memscpy((void *)nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id,
                          sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id),
                          (const void *)nvruim_an_cave_nai,
                          NVRUIM_CHINA_UNICOM_AN_CAVE_NAI_LENGTH);

          if(!(nvruim_decode_min(
                 (byte*)(nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id),
                 sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_user_id_long.user_id),
                 mcc, mnc, min1, min2)))
          {
            *op_status = NV_NOTACTIVE_S;
          } /* The decode was unsuccessful */
#else /* No Cave Support */
          UIM_MSG_HIGH_0("Unknown HRPD preference.");
#endif /* FEATURE_UIM_CAVE_AN_AUTH */
        } /* end else */
      } /* end if - read successful */
      else
      {
        *op_status = NV_NOTACTIVE_S;
      }
      if(*op_status == NV_NOTACTIVE_S)
      {
        UIM_MSG_HIGH_0("NAI String Not Built Properly for AN CAVE Auth");
      }
      break;

    case NV_HDR_AN_AUTH_NAI_I:
      if(NV_DONE_S == *op_status)
      {
        /* Reformat the data only if the read was successful. */
        if(NVRUIM_AN_HRPD_SUPPORT == uim_an_hrpd_support_per_sub(as_id))
        {
          if(nvruim_hrpdupp_buf[1] > NV_MAX_AN_AUTH_NAI_LENGTH)
          {
            nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai_len =
             NV_MAX_AN_AUTH_NAI_LENGTH;
            nvruim_memscpy((void *)nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai,
                            sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai),
                            (const void *)&(nvruim_hrpdupp_buf[2]),
                            NV_MAX_AN_AUTH_NAI_LENGTH);
          }
          else
          {
            nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai_len =
               nvruim_hrpdupp_buf[1];
            nvruim_memscpy((void *)nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai,
                            sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai),
                            (const void *)&(nvruim_hrpdupp_buf[2]),
                            nvruim_hrpdupp_buf[1]);
          }
        }
        else
        {
#ifdef FEATURE_UIM_CAVE_AN_AUTH
        /*---------------------------------------------------------------------
         Retrive  MIN1, MIN2, MCC and MNC and build NAI string here
         Copy the item from the read data buffer.
         --------------------------------------------------------------------*/
          nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai_len =
            NVRUIM_CHINA_UNICOM_AN_CAVE_NAI_LENGTH;
          min1 = (nvruim_imsi_m_buf.imsi_s1[2] << 16) |
                 (nvruim_imsi_m_buf.imsi_s1[1] << 8) |
                 (nvruim_imsi_m_buf.imsi_s1[0]);

          min2 = (nvruim_imsi_m_buf.imsi_s2[1] << 8) |
                  nvruim_imsi_m_buf.imsi_s2[0];
          mnc = nvruim_imsi_m_buf.imsi_11_12;
          mcc = (nvruim_imsi_m_buf.imsi_mcc[1] << 8) |
                 nvruim_imsi_m_buf.imsi_mcc[0];

         /* Copy generic China Unicom AN Cave NAI format:
            <000000000000000>@unicomcdma.com.cn */
          nvruim_memscpy((void *)nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai,
                          sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai),
                          (const void *)nvruim_an_cave_nai,
                          NVRUIM_CHINA_UNICOM_AN_CAVE_NAI_LENGTH);

          if(!(nvruim_decode_min(
                 (byte*)(nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai),
                 sizeof(nv_cmd_ptr->data_ptr->hdr_an_auth_nai.nai),
                 mcc, mnc, min1, min2)))
          {
            *op_status = NV_NOTACTIVE_S;
          } /* The decode was unsuccessful */
#else /* No Cave Support */
          UIM_MSG_HIGH_0("Unknown HRPD preference.");
#endif /* FEATURE_UIM_CAVE_AN_AUTH */
        } /* end else */
      } /* end if - read successful */
      else
      {
        *op_status = NV_NOTACTIVE_S;
      }
      if(*op_status == NV_NOTACTIVE_S)
      {
        UIM_MSG_HIGH_0("NAI String Not Built Properly for AN CAVE Auth");
      }
      break;
    default:
      break;
  }

  nv_cmd_ptr->status = *op_status;
  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_hrpd_hrpdupp */


/*===========================================================================
FUNCTION NVRUIM_READ_MEID

DESCRIPTION
Check to see if the MEID is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  Nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_meid
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status        = NV_RUIM_SUPPORTS_ITEM;
  qword                    nvruim_sf_euim_id_buf = {0x00};

  if(nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if (nvruim_generic_data_ptr == NULL ||
      nvruim_session_data_ptr[context] == NULL ||
      (nvruim_session_data_ptr[context]->esn_usage != NV_RUIM_USE_SF_EUIMID &&
       nvruim_session_data_ptr[context]->esn_usage != NV_RUIM_USE_SF_EUIMID_ESN))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(!nvruim_check_rtre_config(nv_cmd_ptr, op_status, context))
  {
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Setup the command structure to read the SF_RUIM_ID EF from the RUIM */
  nvruim_cmd_ptr->data.data_len     = NVRUIM_SIZE(meid);
  nvruim_cmd_ptr->data.data_ptr     = (byte *) nvruim_sf_euim_id_buf;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_SF_EUIM_ID;
  nvruim_cmd_ptr->offset_or_recnum  = 0; /* EF offset  */

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    return support_status;
  }

  if(nv_cmd_ptr->item == NV_MEID_I)
  {
    if(*op_status == NV_NOTACTIVE_S)
    {
      return NV_RUIM_ITEM_NOT_SUPPORTED;
    } /* end if - the read from the SF_EUIMD has failed. */

    /* Reformat the data only if the read was successful. */
    if(NV_DONE_S == *op_status)
    {
      /* Copy the MEID from the read data buffer */
      qw_equ(nv_cmd_ptr->data_ptr->meid, nvruim_sf_euim_id_buf);
    } /* end if - the read was successful. */
  }

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_meid */


/*===========================================================================
FUNCTION nvruim_read_smscap

DESCRIPTION
  Check to see if SMS Capability is supported by the R-UIM.
  If it is supported, read it.

DEPENDENCIES
  Non-reentrant

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status indicates the success/failure of the read.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_smscap
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status                        = NV_RUIM_SUPPORTS_ITEM;
  uint8                    nvruim_smscap_buf[NVRUIM_SMSCAP_SIZE] = {0x00};

  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nvruim_session_data_ptr[context]->smscap_svc == FALSE )
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Setup the command structure to read the EF from the RUIM */
  nvruim_cmd_ptr->data.data_len     = NVRUIM_SMSCAP_SIZE;
  nvruim_cmd_ptr->data.data_ptr     = nvruim_smscap_buf;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_SMS_CAP;
  nvruim_cmd_ptr->offset_or_recnum  = 0; /* EF offset  */

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    nvruim_session_data_ptr[context]->smscap_svc = FALSE;
    return support_status;
  }

  switch( nv_cmd_ptr->item )
  {
    case NV_SMS_MO_RETRY_PERIOD_I:
      nv_cmd_ptr->data_ptr->sms_mo_retry_period = nvruim_smscap_buf[0];
      break;

    case NV_SMS_MO_RETRY_INTERVAL_I:
      nv_cmd_ptr->data_ptr->sms_mo_retry_interval = nvruim_smscap_buf[1];
      break;
    case NV_SMS_MO_ON_ACCESS_CHANNEL_I:
      nv_cmd_ptr->data_ptr->sms_mo_on_access_channel = nvruim_smscap_buf[2]
           & NVRUIM_SMSCAP_ON_ACCESS_CH_MASK;
      break;
    case NV_SMS_MO_ON_TRAFFIC_CHANNEL_I:
      if (nvruim_smscap_buf[2] & NVRUIM_SMSCAP_ON_TRAFFIC_CH_MASK)
      {
        nv_cmd_ptr->data_ptr->sms_mo_on_traffic_channel = TRUE;
      }
      else
      {
        nv_cmd_ptr->data_ptr->sms_mo_on_traffic_channel = FALSE;
      }
      break;
    case NV_SMS_SERVICE_OPTION_I:
      nv_cmd_ptr->data_ptr->sms_service_option = nvruim_smscap_buf[3];
      break;
    default:
      UIM_MSG_ERR_1("Invalid NV item for EF-SMSCAP: %d", nv_cmd_ptr->item);
      return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;

} /* nvruim_read_smscap() */


/*===========================================================================
FUNCTION nvruim_read_bcsms_config

DESCRIPTION
  Check to see if BC-SMS Config is supported by the R-UIM.
  If it is supported, read it.

DEPENDENCIES
  Non-reentrant

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status indicates the success/failure of the read.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_bcsms_config
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  byte                     bcsms_config   = 0;

  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nvruim_session_data_ptr[context]->bcsms_svc == FALSE )
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Setup the command structure to read the EF from the RUIM */
  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = & bcsms_config;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_BCSMS_CONFIG;
  nvruim_cmd_ptr->offset_or_recnum  = 0; /* EF offset  */

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr     = NULL;
  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    nvruim_session_data_ptr[context]->bcsms_svc = FALSE;
    return support_status;
  }

  nv_cmd_ptr->data_ptr->sms_bc_config = bcsms_config;

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;

} /* nvruim_read_bcsms_config */


/*===========================================================================
FUNCTION nvruim_read_bcsms_pref

DESCRIPTION
  Check to see if BC-SMS Pref is supported by the R-UIM.
  If it is supported, read it.

DEPENDENCIES
  Non-reentrant

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status_ptr indicates the success/failure of the read.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_bcsms_pref
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status   support_status = NV_RUIM_SUPPORTS_ITEM;
  byte                     bcsms_pref     = 0;

  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nvruim_session_data_ptr[context]->bcsms_svc == FALSE )
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Setup the command structure to read the EF from the RUIM */
  nvruim_cmd_ptr->data.data_len     = 1;
  nvruim_cmd_ptr->data.data_ptr     = & bcsms_pref;
  nvruim_cmd_ptr->file_enum         = MMGSDI_CDMA_BCSMS_PREF;
  nvruim_cmd_ptr->offset_or_recnum  = 0; /* EF offset  */

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr     = NULL;
  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    nvruim_session_data_ptr[context]->bcsms_svc = FALSE;
    return support_status;
  }

  nv_cmd_ptr->data_ptr->sms_bc_user_pref = bcsms_pref;

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_bcsms_pref() */


/*===========================================================================
FUNCTION nvruim_read_bcsms_table

DESCRIPTION
  Check to see if BC-SMS table items are supported by the R-UIM.
  If it is supported, read it.

DEPENDENCIES
  Non-reentrant

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status_ptr indicates the success/failure of the read.  For items
  which are "not supported", the value of op_statusis undefined.

SIDE EFFECTS
  nv_cmd_ptr will be modified with the op_status

===========================================================================*/
static nv_ruim_support_status nvruim_read_bcsms_table
(
  nv_cmd_type             *nv_cmd_ptr,  /* command block    */
  nv_stat_enum_type       *op_status,   /* status of the I/O operation*/
  uint8                    timer_index,
  nvruim_nv_context_type   context,
  nvruim_cmd_type         *nvruim_cmd_ptr
)
{
  nv_ruim_support_status  support_status       = NV_RUIM_SUPPORTS_ITEM;
  uint8   nvruim_bcsms_table_num_entries       = 0;
  uint8   nvruim_bcsms_table_buf[NVRUIM_MAX_BCSMS_TABLE_ENTRY_SIZE] = {0x00};
  uint8   nvruim_bcsms_parm_buf[NVRUIM_BCSMS_PARM_SIZE]             = {0x00};

  if((nvruim_cmd_ptr == NULL) || (nv_cmd_ptr == NULL) || (op_status == NULL) ||
     NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  if(nvruim_session_data_ptr[context]->bcsms_svc == FALSE )
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Setup the command structure to read the EF from the RUIM */
  nvruim_cmd_ptr->data.data_ptr       = nvruim_bcsms_parm_buf;
  nvruim_cmd_ptr->data.data_len       = NVRUIM_BCSMS_PARM_SIZE;
  nvruim_cmd_ptr->file_enum           = MMGSDI_CDMA_BCSMS_PARAMS;
  nvruim_cmd_ptr->cmd_type            = NVRUIM_FILE_ATTR;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM || nvruim_cmd_ptr->num_records == 0)
  {
    nvruim_session_data_ptr[context]->bcsms_svc = FALSE;
    return support_status;
  }
  nvruim_bcsms_table_num_entries = nvruim_cmd_ptr->num_records;

  if( nv_cmd_ptr->item == NV_SMS_BC_SERVICE_TABLE_SIZE_I )
  {
    nv_cmd_ptr->data_ptr->sms_bc_service_table_size = nvruim_bcsms_table_num_entries;
    * op_status = NV_DONE_S;
    return NV_RUIM_SUPPORTS_ITEM;
  }

  /* Set up record number */
  if( nv_cmd_ptr->item == NV_SMS_BC_SERVICE_TABLE_I )
  {
    nvruim_cmd_ptr->offset_or_recnum = nv_cmd_ptr->data_ptr->sms_bc_service_table.index+1; /* record number */
  }
  else
  {
    nvruim_cmd_ptr->offset_or_recnum = nv_cmd_ptr->data_ptr->sms_bc_service_table_options.index+1; /* record number */
  }

  /* Read BCSMS_PARAMS first */
  nvruim_cmd_ptr->cmd_type   = NVRUIM_READ;
  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    nvruim_session_data_ptr[context]->bcsms_svc = FALSE;
    return support_status;
  }

  /* Read BCSMS_TABLE next */
  /* Setup the command structure to read the BCSMS_TABLE from the RUIM */
  nvruim_cmd_ptr->data.data_ptr  = nvruim_bcsms_table_buf;
  nvruim_cmd_ptr->data.data_len  = NVRUIM_MAX_BCSMS_TABLE_ENTRY_SIZE;
  nvruim_cmd_ptr->file_enum      = MMGSDI_CDMA_BCSMS_TABLE;
  nvruim_cmd_ptr->cmd_type       = NVRUIM_READ;

  support_status = nvruim_read_and_check_rsp(nv_cmd_ptr, op_status, timer_index,
                                             context, nvruim_cmd_ptr);

  /*pointer set back to NULL before the variable goes out of scope*/
  nvruim_cmd_ptr->data.data_ptr = NULL;

  if(support_status != NV_RUIM_SUPPORTS_ITEM)
  {
    nvruim_session_data_ptr[context]->bcsms_svc = FALSE;
    return support_status;
  }

  /* Fill in NV read result from data retrieved from the card */
  if( nv_cmd_ptr->item == NV_SMS_BC_SERVICE_TABLE_I )
  {
    nv_cmd_ptr->data_ptr->sms_bc_service_table.active_service =
            nvruim_bcsms_table_buf[0];
    nv_cmd_ptr->data_ptr->sms_bc_service_table.service =
            nvruim_bcsms_table_buf[1] | nvruim_bcsms_table_buf[2]<<8;
    nv_cmd_ptr->data_ptr->sms_bc_service_table.language =
            nvruim_bcsms_table_buf[3];
    nv_cmd_ptr->data_ptr->sms_bc_service_table.selected =
            nvruim_bcsms_parm_buf[0];
    nv_cmd_ptr->data_ptr->sms_bc_service_table.priority =
            nvruim_bcsms_parm_buf[1];

    memset( (void*)nv_cmd_ptr->data_ptr->sms_bc_service_table.label,
            0,
            NV_SMS_BC_SRV_LABEL_SIZE);
    nvruim_memscpy( (void*)nv_cmd_ptr->data_ptr->sms_bc_service_table.label,
                    sizeof(nv_cmd_ptr->data_ptr->sms_bc_service_table.label),
                    (const void *)(nvruim_bcsms_table_buf+7),
                    MIN(NV_SMS_BC_SRV_LABEL_SIZE, nvruim_cmd_ptr->data.data_len - 7) );
  }
  else if( nv_cmd_ptr->item == NV_SMS_BC_SERVICE_TABLE_OPTIONS_I )
  {
    nv_cmd_ptr->data_ptr->sms_bc_service_table_options.max_messages =
            nvruim_bcsms_table_buf[4];
    nv_cmd_ptr->data_ptr->sms_bc_service_table_options.bc_alert =
            nvruim_bcsms_table_buf[5];
    nv_cmd_ptr->data_ptr->sms_bc_service_table_options.label_encoding =
            nvruim_bcsms_table_buf[6];
  }
  else
  {
    UIM_MSG_ERR_1("Invalid NV item for BCSMS: %d", nv_cmd_ptr->item);
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nv_cmd_ptr->status = *op_status;

  return NV_RUIM_SUPPORTS_ITEM;
} /* nvruim_read_bcsms_table */


/*===========================================================================

FUNCTION NVRUIM_READ

DESCRIPTION
Check to see if the item is supported by the R-UIM.
If it is supported, read it.

DEPENDENCIES
Non-reentrant

RETURN VALUE
NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
NV_RUIM_SUPPORTS_ITEM Item is supported, see op_status

op_status indicates the success/failure of the read.  For items
which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
None

===========================================================================*/
nv_ruim_support_status nvruim_read
(
  nv_cmd_type           *nv_cmd_ptr,  /* command block    */
  nvruim_nv_context_type context,
  nv_stat_enum_type     *op_status    /* status of the I/O operation*/
)
{
  nv_ruim_support_status   nvruim_read_support_status = NV_RUIM_ITEM_NOT_SUPPORTED;
  uint8                    i                          = 0;
  uint8                    timer_index                = 0;
  boolean                  timer_status               = FALSE;
  nvruim_session_enum_type nvruim_session             = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_cmd_type         *nvruim_cmd_ptr             = NULL;
  sys_modem_as_id_e_type   as_id                      = SYS_MODEM_AS_ID_1;

  if(!op_status || !nv_cmd_ptr || nvruim_generic_data_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* Try to setup the timer for every incoming request */
  NVRUIM_ENTER_TIMER_CRIT_SECT;
  timer_status = nvruim_setup_timer(&timer_index, context);
  NVRUIM_LEAVE_TIMER_CRIT_SECT;

  if (!timer_status)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nvruim_session = nvruim_map_context_to_nvruim_session(context);
  as_id          = nvruim_convert_context_to_as_id(context);

  /* Determine if the CDMA subscription is on the card */
  if (NV_RTRE_CONTROL_USE_RUIM != nvruim_get_rtre_control_value(nvruim_session))
  {
    /* Do not try to access the R-UIM card.  Get item from NV */
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  /* No need to read the mmgsdi features status nvitem into thr nvruim
     global here as it is already read in nvruim task's post init function */

  /* Take the task pointer signal from nv_cmd_ptr and put into the cmd */

  nvruim_cmd_ptr = nvruim_mem_malloc(sizeof(nvruim_cmd_type));

  if (nvruim_cmd_ptr == NULL)
  {
    return NV_RUIM_ITEM_NOT_SUPPORTED;
  }

  nvruim_cmd_ptr->cmd_hdr.sigs = nv_cmd_ptr->sigs;
  nvruim_cmd_ptr->cmd_hdr.task_ptr = rex_self();
  nvruim_cmd_ptr->nv_context = context;

  /* Find-out if the NV item is an R-UIM item */
  /* If it is an R-UIM item:  */
  /*     Initialize the buffer with item pointer & size */
  /*     Select the EF*/
  /*     Select the offset into the EF  */

  switch(nv_cmd_ptr->item)
  {

    case NV_ESN_CHKSUM_I:
      /* ESN checksum read returns the status of the checksum,  */
      /* which is signaled by a successful read from the R-UIM. */
      /* FALLTHROUGH */

    case NV_ESN_I:
      nvruim_read_support_status = nvruim_read_esn(nv_cmd_ptr,
                                                   op_status,
                                                   timer_index,
                                                   context,
                                                   nvruim_cmd_ptr);
      break;

    case NV_COUNT_I:
      nvruim_read_support_status = nvruim_read_count(nv_cmd_ptr,
                                                     op_status,
                                                     timer_index,
                                                     context,
                                                     nvruim_cmd_ptr);
      break;

    case NV_MIN1_I:
    case NV_MIN2_I:
    case NV_IMSI_11_12_I:
    case NV_IMSI_MCC_I:
    case NV_IMSI_ADDR_NUM_I:
      nvruim_read_support_status =  nvruim_read_imsi_m(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;

    case NV_IMSI_T_S1_I:
    case NV_IMSI_T_S2_I:
    case NV_IMSI_T_MCC_I:
    case NV_IMSI_T_11_12_I:
    case NV_IMSI_T_ADDR_NUM_I:
      nvruim_read_support_status =  nvruim_read_imsi_t(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;

    case NV_SID_NID_I:
      nvruim_read_support_status =  nvruim_read_sid_nid(nv_cmd_ptr,
                                                        op_status,
                                                        timer_index,
                                                        context,
                                                        nvruim_cmd_ptr);
      break;

    case NV_DIR_NUMBER_I:
    case NV_DIR_NUMBER_PCS_I:
      nvruim_read_support_status = nvruim_read_msisdn(nv_cmd_ptr,
                                                      op_status,
                                                      timer_index,
                                                      context,
                                                      nvruim_cmd_ptr);
      break;

    case NV_ANALOG_FIRSTCHP_I:
      nvruim_read_support_status =  nvruim_read_analog_chan_pref(nv_cmd_ptr,
                                                                 op_status,
                                                                 timer_index,
                                                                 context,
                                                                 nvruim_cmd_ptr);
      break;

    case NV_ANALOG_HOME_SID_I:
      nvruim_read_support_status =  nvruim_read_home_sid(nv_cmd_ptr,
                                                         op_status,
                                                         timer_index,
                                                         context,
                                                         nvruim_cmd_ptr);
      break;

    case NV_ACCOLC_I:
      nvruim_read_support_status =  nvruim_read_accolc(nv_cmd_ptr,
                                                       op_status,
                                                       timer_index,
                                                       context,
                                                       nvruim_cmd_ptr);
      break;

#ifdef FEATURE_UIM_SUPPORT_LBS
    case NV_GPS1_CAPABILITIES_I:
    case NV_GPS1_SEEDPOS_OPTION_I:
    case NV_GPS1_DYNAMIC_MODE_I:
      nvruim_read_support_status = nvruim_read_lbs_v2_config (nv_cmd_ptr,
                                                              op_status,
                                                              timer_index,
                                                              context,
                                                              nvruim_cmd_ptr);
      break;

    case NV_GPS1_XTRA_ENABLED_I:
    case NV_GPS1_XTRA_AUTO_DOWNLOAD_ENABLED_I:
    case NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I:
    case NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I:
    case NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I:
      nvruim_read_support_status = nvruim_read_lbs_xtra_config (nv_cmd_ptr,
                                                                op_status,
                                                                timer_index,
                                                                context,
                                                                nvruim_cmd_ptr);
      break;

    case NV_GPS1_XTRA_PRIMARY_SERVER_URL_I:
    case NV_GPS1_XTRA_SECONDARY_SERVER_URL_I:
    case NV_GPS1_XTRA_TERTIARY_SERVER_URL_I:
      nvruim_read_support_status = nvruim_read_lbs_xtra_server_urls(nv_cmd_ptr,
                                                                    op_status,
                                                                    timer_index,
                                                                    context,
                                                                    nvruim_cmd_ptr);
      break;

    case NV_GPS1_PDE_ADDRESS_I:
    case NV_GPS1_PDE_PORT_I:
    case NV_CGPS_1X_PDE_SERVER_ADDR_IPV4_I:
    case NV_CGPS_1X_PDE_SERVER_PORT_I:
    case NV_CGPS_1X_PDE_SERVER_ADDR_IPV6_I:
    case NV_CGPS_1X_PDE_SERVER_ADDR_URL_I:
      nvruim_read_support_status = nvruim_read_lbs_v2_pde_adress (nv_cmd_ptr,
                                                                  op_status,
                                                                  timer_index,
                                                                  context,
                                                                  nvruim_cmd_ptr);
      break;

    case NV_CGPS_1X_MPC_SERVER_ADDR_IPV4_I:
    case NV_CGPS_1X_MPC_SERVER_PORT_I:
    case NV_CGPS_1X_MPC_SERVER_ADDR_IPV6_I:
    case NV_CGPS_1X_MPC_SERVER_ADDR_URL_I:
      nvruim_read_support_status = nvruim_read_lbs_v2_mpc_adress (nv_cmd_ptr,
                                                                  op_status,
                                                                  timer_index,
                                                                  context,
                                                                  nvruim_cmd_ptr);
     break;
#endif /* FEATURE_UIM_SUPPORT_LBS*/

    case NV_SLOT_CYCLE_INDEX_I:
      if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_RUIM_SUPPORT_SCI_I, 
                                       context))
      {
        nvruim_read_support_status =  nvruim_read_sci(nv_cmd_ptr,
                                                      op_status,
                                                      timer_index,
                                                      context,
                                                      nvruim_cmd_ptr);
      }
      break;

    case NV_MOB_TERM_HOME_I   :
    case NV_MOB_TERM_FOR_SID_I:
    case NV_MOB_TERM_FOR_NID_I:
      nvruim_read_support_status =  nvruim_read_call_term(nv_cmd_ptr,
                                                          op_status,
                                                          timer_index,
                                                          context,
                                                          nvruim_cmd_ptr);
      break;

    case NV_HOME_SID_NID_I:
      nvruim_read_support_status = nvruim_read_home_sid_nid(nv_cmd_ptr,
                                                            op_status,
                                                            timer_index,
                                                            context,
                                                            nvruim_cmd_ptr);
      break;

    case NV_ROAMING_LIST_683_I:
      nvruim_read_support_status = nvruim_read_prl(nv_cmd_ptr,
                                                   op_status,
                                                   timer_index,
                                                   context,
                                                   nvruim_cmd_ptr);
      (void) nv_update_prl_version_data(nv_cmd_ptr);
      break;

    case NV_OTAPA_ENABLED_I:
    case NV_SPC_CHANGE_ENABLED_I:
      nvruim_read_support_status = nvruim_read_spc_enabled(nv_cmd_ptr,
                                                           op_status,
                                                           timer_index,
                                                           context,
                                                           nvruim_cmd_ptr);
      break;

    case NV_NAM_LOCK_I:
      nvruim_read_support_status = nvruim_read_nam_lock(nv_cmd_ptr,
                                                        op_status,
                                                        timer_index,
                                                        context,
                                                        nvruim_cmd_ptr);
      break;

    /* Service Preferences */
    case  NV_PREF_MODE_I:
      if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_PREF_MODE_NOT_FROM_RUIM_I,
                                       context) == FALSE)
      {
        nvruim_read_support_status = nvruim_read_svc_pref(nv_cmd_ptr,
                                                          op_status,
                                                          timer_index,
                                                          context,
                                                          nvruim_cmd_ptr);
      }
      break;

    case  NV_SYSTEM_PREF_I:
      nvruim_read_support_status = nvruim_read_svc_pref(nv_cmd_ptr,
                                                        op_status,
                                                        timer_index,
                                                        context,
                                                        nvruim_cmd_ptr);
      break;

    /* TMSI */
    case  NV_ASSIGNING_TMSI_ZONE_LEN_I:
    case  NV_ASSIGNING_TMSI_ZONE_I:
    case  NV_TMSI_CODE_I:
    case  NV_TMSI_EXP_I:
      nvruim_read_support_status = nvruim_read_tmsi(nv_cmd_ptr,
                                                    op_status,
                                                    timer_index,
                                                    context,
                                                    nvruim_cmd_ptr);
      break;

    /* Analog Location and Registration Indicators */
    case  NV_NXTREG_I:
    case  NV_LSTSID_I:
    case  NV_LOCAID_I:
    case  NV_PUREG_I:
      nvruim_read_support_status = nvruim_read_loc_regn_ind(nv_cmd_ptr,
                                                            op_status,
                                                            timer_index,
                                                            context,
                                                            nvruim_cmd_ptr);
      break;

    /* CDMA Zone-Based Registration Indicators */
    case  NV_ZONE_LIST_I:
      nvruim_read_support_status = nvruim_read_zone_based_ind(nv_cmd_ptr,
                                                              op_status,
                                                              timer_index,
                                                              context,
                                                              nvruim_cmd_ptr);
      break;

    /* CDMA System/Network Registration Indicators */
    case  NV_SID_NID_LIST_I:
      nvruim_read_support_status = nvruim_read_sys_regn_ind(nv_cmd_ptr,
                                                            op_status,
                                                            timer_index,
                                                            context,
                                                            nvruim_cmd_ptr);
      break;

    /* CDMA Distance-Based Registration Indicators */
    case  NV_DIST_REG_I:
      nvruim_read_support_status = nvruim_read_dist_regn_ind(nv_cmd_ptr,
                                                             op_status,
                                                             timer_index,
                                                             context,
                                                             nvruim_cmd_ptr);
      break;

    case  NV_HYSTERISIS_ACTIVATION_TIMER_I:
    case  NV_ACTIVE_EPZID_TYPE_I:
    case  NV_DSAT707_CTA_TIMER_I:
      nvruim_read_support_status = nvruim_read_3gpd_data_generic_config(nv_cmd_ptr,
                                                                        op_status,
                                                                        timer_index,
                                                                        context,
                                                                        nvruim_cmd_ptr);
      break;

      /* This item is supported on the card, but it is not readable - do not allow this
         to go to NV */
    case NV_DS_MIP_SS_USER_PROF_I:
      /* The following items are not supported in an R-UIM implementation, do not
         allow these items to go to NV */
    case NV_DS_MIP_DMU_PKOID_I:
    case NV_DS_MIP_DMU_MN_AUTH_I:
      if(NVRUIM_3GPD_MIP_NV_SIP_NV != uim_3gpd_support_per_sub(as_id) &&
         NVRUIM_3GPD_MIP_NV_SIP_RUIM != uim_3gpd_support_per_sub(as_id))
      {
        *op_status = NV_NOTACTIVE_S;
        nvruim_read_support_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;
    case NV_DS_MIP_ENABLE_PROF_I:
      if(NVRUIM_3GPD_MIP_NV_SIP_NV != uim_3gpd_support_per_sub(as_id) &&
         NVRUIM_3GPD_MIP_NV_SIP_RUIM != uim_3gpd_support_per_sub(as_id))
      {
        *op_status = NV_DONE_S;
        for(i=0; i < NV_DS_MIP_MAX_NUM_PROF; i++)
        {
          nv_cmd_ptr->data_ptr->ds_mip_enable_prof[i] = 1;
        }
        nvruim_read_support_status = NV_RUIM_SUPPORTS_ITEM;
      }
      break;

    /* The following items CAN be read on the card */
    case NV_DS_QCMIP_I:
      nvruim_read_support_status = nvruim_read_3gpd(nv_cmd_ptr,
                                                    op_status,
                                                    timer_index,
                                                    context,
                                                    nvruim_cmd_ptr);
      break;

    case NV_PPP_USER_ID_I:
    case NV_DS_SIP_NUM_VALID_PROFILES_I:
    case NV_DS_SIP_NAI_INFO_I:
      /* Return the NV value for NV_PPP_USER_ID_I and NV_PPP_PASSWORD_I
         for OMH card */
      if((nv_cmd_ptr->item != NV_PPP_USER_ID_I) ||
         !nvruim_data_3gpd_ext_support_per_sub(as_id))
      {
        nvruim_read_support_status = nvruim_read_3gpd_sipupp(nv_cmd_ptr,
                                                             op_status,
                                                             timer_index,
                                                             context,
                                                             nvruim_cmd_ptr);
      }
      break;

    case NV_DS_SIP_ACTIVE_PROFILE_INDEX_I:
      nvruim_read_support_status = nvruim_read_3gpd_sipsp(nv_cmd_ptr,
                                                          op_status,
                                                          timer_index,
                                                          context,
                                                          nvruim_cmd_ptr);
      break;

    case NV_DS_SIP_PROFILE_I:
      nvruim_read_support_status = nvruim_read_3gpd_uppext(nv_cmd_ptr,
                                                           op_status,
                                                           timer_index,
                                                           context,
                                                           nvruim_cmd_ptr);
      break;

    case NV_DS_MIP_GEN_USER_PROF_I:
    case NV_DS_MIP_NUM_PROF_I:
    case NV_DS_MIP_RETRIES_I:
    case NV_DS_MIP_RETRY_INT_I:
    case NV_DS_MIP_PRE_RE_RRQ_TIME_I:
      nvruim_read_support_status = nvruim_read_3gpd_mipupp(nv_cmd_ptr,
                                                           op_status,
                                                           timer_index,
                                                           context,
                                                           nvruim_cmd_ptr);
      break;

    case NV_DS_MIP_ACTIVE_PROF_I:
      nvruim_read_support_status = nvruim_read_3gpd_mipsp(nv_cmd_ptr,
                                                          op_status,
                                                          timer_index,
                                                          context,
                                                          nvruim_cmd_ptr);
      break;

    /* The following item refers to a generic SIP password in NV, but refers only
       to a PAP password in the R-UIM.  The CHAP password is kept separately on the card, and
       cannot be read */
    case NV_PPP_PASSWORD_I:
    case NV_DS_SIP_PPP_SS_INFO_I:
      if((nv_cmd_ptr->item != NV_PPP_PASSWORD_I) ||
         !nvruim_data_3gpd_ext_support_per_sub(as_id))
      {
        nvruim_read_support_status = nvruim_read_3gpd_sippapss(nv_cmd_ptr,
                                                               op_status,
                                                               timer_index,
                                                               context,
                                                               nvruim_cmd_ptr);
      }
      break;

    case NV_DS_MIP_2002BIS_MN_HA_AUTH_I:
    case NV_DS_MIP_QC_DRS_OPT_I:
    case NV_DS_MIP_QC_HANDDOWN_TO_1X_OPT_I:
    case NV_DS_MIP_RRQ_IF_TFRK_I:
      nvruim_read_support_status = nvruim_read_3gpd_mipflags(nv_cmd_ptr,
                                                             op_status,
                                                             timer_index,
                                                             context,
                                                             nvruim_cmd_ptr);
      break;

    case NV_TCP_GRACEFUL_DORMANT_CLOSE_I:
    case NV_TCP_KEEPALIVE_IDLE_TIME_I:
      nvruim_read_support_status = nvruim_read_3gpd_tcp_config(nv_cmd_ptr,
                                                               op_status,
                                                               timer_index,
                                                               context,
                                                               nvruim_cmd_ptr);
      break;

    case NV_HDR_AN_AUTH_USER_ID_LONG_I:
    case NV_HDR_AN_AUTH_NAI_I:
      nvruim_read_support_status = nvruim_read_hrpd_hrpdupp(nv_cmd_ptr,
                                                            op_status,
                                                            timer_index,
                                                            context,
                                                            nvruim_cmd_ptr);
      break;

    case NV_MEID_I:
      nvruim_read_support_status = nvruim_read_meid(nv_cmd_ptr,
                                                    op_status,
                                                    timer_index,
                                                    context,
                                                    nvruim_cmd_ptr);
      break;

    case NV_SMS_BC_CONFIG_I:
      nvruim_read_support_status = nvruim_read_bcsms_config(nv_cmd_ptr,
                                                            op_status,
                                                            timer_index,
                                                            context,
                                                            nvruim_cmd_ptr);
      break;

    case NV_SMS_BC_USER_PREF_I:
      nvruim_read_support_status = nvruim_read_bcsms_pref(nv_cmd_ptr,
                                                          op_status,
                                                          timer_index,
                                                          context,
                                                          nvruim_cmd_ptr);
      break;

    case NV_SMS_BC_SERVICE_TABLE_I:
    case NV_SMS_BC_SERVICE_TABLE_SIZE_I:
    case NV_SMS_BC_SERVICE_TABLE_OPTIONS_I:
      nvruim_read_support_status = nvruim_read_bcsms_table(nv_cmd_ptr,
                                                           op_status,
                                                           timer_index,
                                                           context,
                                                           nvruim_cmd_ptr);
      break;

    case NV_SMS_MO_RETRY_PERIOD_I:
    case NV_SMS_MO_RETRY_INTERVAL_I:
    case NV_SMS_MO_ON_TRAFFIC_CHANNEL_I:
    case NV_SMS_MO_ON_ACCESS_CHANNEL_I:
    case NV_SMS_SERVICE_OPTION_I:
      nvruim_read_support_status = nvruim_read_smscap(nv_cmd_ptr,
                                                      op_status,
                                                      timer_index,
                                                      context,
                                                      nvruim_cmd_ptr);
      break;

    default:
      /* Item is not supported by R-UIM */
      break;
  }

  nvruim_mem_free_null_ok(nvruim_cmd_ptr);

  /* Now clear the timer that was setup earlier */
  NVRUIM_ENTER_TIMER_CRIT_SECT;
  (void)nvruim_clear_timer(timer_index);
  NVRUIM_LEAVE_TIMER_CRIT_SECT;

  return nvruim_read_support_status;
} /* nvruim_read */


/*===========================================================================

FUNCTION NVRUIM_SET_UIM_DIR_PRESENT

DESCRIPTION
The function set the flag to indicate which of the MF, GSM DF, DCS
1800 DF, CDMA subscription (CDMA DF or CSIM ADF) exist.

DEPENDENCIES
None.

RETURN VALUE
None

SIDE EFFECTS
Updates the variable nvruim_dir_present.
===========================================================================*/
void nvruim_set_uim_dir_present
(
  nvruim_cdma_subscription_type  cdma_sub_state,
  mmgsdi_slot_id_enum_type       slot_id
)
{
  uint8 slot_index = NVRUIM_SLOT_INDEX_INVALID;

  if (nvruim_convert_slot_id_to_index(slot_id, &slot_index))
  {
    nvruim_dir_present[slot_index] = cdma_sub_state;
  }
}


/*===========================================================================

FUNCTION NVRUIM_INIT_NON_3GPD_CDMA_CARD

DESCRIPTION
  This function is called from mmgsdi task during post pin initialization.
  It passes in a boolean, where
    TRUE : it is 3GPD disabled card
    FALSE: it is 3GPD enabled  card

DEPENDENCIES
None.

RETURN VALUE

SIDE EFFECTS
none
===========================================================================*/
void nvruim_init_non_3gpd_cdma_card
(
  boolean                       non_3gpd_cdma_card,
  nvruim_session_enum_type      nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

#ifdef FEATURE_UIM_RUN_TIME_ENABLE
  if(NV_RTRE_CONTROL_USE_RUIM != nvruim_get_rtre_control_value(nvruim_session))
  {
    /* NV mode, Set to No card */
    nvruim_session_data_ptr[context]->non_3gpd_cdma_card = NVRUIM_NO_CARD;
  }
  else
#endif /* FEATURE_UIM_RUN_TIME_ENABLE */
  {
    /* RUIM mode, set flag for cdma card */
    UIM_MSG_HIGH_2("Non 3GPD CDMA Card 0x%x, session: 0x%x", non_3gpd_cdma_card, nvruim_session);
    if ( TRUE == non_3gpd_cdma_card)
    {
      nvruim_session_data_ptr[context]->non_3gpd_cdma_card = NVRUIM_NON_3GPD_CARD;
      /* Reset the 3GPD credential to NV */
      nvruim_session_data_ptr[context]->control_3gpd       = NVRUIM_3GPD_MIP_NV_SIP_NV;
    }
    else
    {
      nvruim_session_data_ptr[context]->non_3gpd_cdma_card = NVRUIM_3GPD_CARD;
    }
  }
} /* nvruim_init_non_3gpd_cdma_card */


/*===========================================================================

FUNCTION NVRUIM_CHECK_NON_3GPD_CDMA_CARD

DESCRIPTION
 This function returns the non legacy cdma card presence.

DEPENDENCIES
This function should only be called after SUBSCRIPTION_READY evt

RETURN VALUE
  nvruim_3gpd_nv_card_status where
  NVRUIM_NO_CARD        : NO Card
  NVRUIM_NON_3GPD_CARD  : 3GPD disabled card
  NVRUIM_3GPD_CARD      : 3GPD enabled card

SIDE EFFECTS
none
===========================================================================*/
nvruim_3gpd_nv_card_status nvruim_check_non_3gpd_cdma_card
(
  void
)
{
  if (nvruim_generic_data_ptr != NULL)
  {
    return nvruim_check_non_3gpd_cdma_card_per_sub(
              nvruim_convert_session_to_as_id(nvruim_generic_data_ptr->nvruim_active_session));
  }
  return NVRUIM_NO_CARD;
} /* nvruim_check_non_3gpd_cdma_card */


/*===========================================================================

FUNCTION NVRUIM_CHECK_NON_3GPD_CDMA_CARD_PER_SUB

DESCRIPTION
  This function returns the non 3gpd cdma card presence for given sub

DEPENDENCIES
  This function should only be called after SUBSCRIPTION_READY evt

RETURN VALUE
  nvruim_3gpd_nv_card_status where
  NVRUIM_NO_CARD        : NO Card
  NVRUIM_NON_3GPD_CARD  : 3GPD disabled card
  NVRUIM_3GPD_CARD      : 3GPD enabled card

SIDE EFFECTS
  None
===========================================================================*/
nvruim_3gpd_nv_card_status nvruim_check_non_3gpd_cdma_card_per_sub(
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type    nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_nv_context_type      context        = NVRUIM_NV_CONTEXT_INVALID;

  UIM_MSG_LOW_1("nvruim_check_non_3gpd_cdma_card_extension, as_id: 0x%x",
                as_id);

  nvruim_session = nvruim_convert_as_id_to_nvruim_session(as_id);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session))
  {
    return NVRUIM_NO_CARD;
  }

  context  = nvruim_map_nvruim_session_to_context(nvruim_session);
  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NVRUIM_NO_CARD;
  }

  if (nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->non_3gpd_cdma_card;
  }

  return NVRUIM_NO_CARD;
} /* nvruim_check_non_3gpd_cdma_card_per_sub */


/*===========================================================================

FUNCTION UIM_INIT_3GPD_PREFERENCE

DESCRIPTION
This function is called from MMGSDI task during initialization for 3GPD.
It passes in a boolean which indicates whether or not the RUIM provides
for 3GPD services

DEPENDENCIES
None.

RETURN VALUE

SIDE EFFECTS
Sets the control_3gpd global variable for RUIM support of 3GPD
Sets the nvruim_3gpd_mip_svc and nvruim_3gpd_sip_svc
global variables to indicate service support
===========================================================================*/
void uim_init_3gpd_preference
(
  boolean                       service_3gpd_sip,
  boolean                       service_3gpd_mip,
  nvruim_session_enum_type      nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

  nvruim_session_data_ptr[context]->sip_svc_3gpd = service_3gpd_sip;
  nvruim_session_data_ptr[context]->mip_svc_3gpd = service_3gpd_mip;

  UIM_MSG_HIGH_2("3GPD services SIP - 0x%x, MIP - 0x%x",
                 nvruim_session_data_ptr[context]->sip_svc_3gpd,
                 nvruim_session_data_ptr[context]->mip_svc_3gpd);
#ifdef FEATURE_UIM_RUN_TIME_ENABLE
  if(NV_RTRE_CONTROL_USE_RUIM != nvruim_get_rtre_control_value(nvruim_session))
  {
    /* Use NV for 3GPD support */
    nvruim_session_data_ptr[context]->control_3gpd = NVRUIM_3GPD_MIP_NV_SIP_NV;
  }
  else
#endif /* FEATURE_UIM_RUN_TIME_ENABLE */

  if(service_3gpd_sip && service_3gpd_mip)
  {
    /* Use R-UIM for 3GPD Support */
    nvruim_session_data_ptr[context]->control_3gpd = NVRUIM_3GPD_MIP_RUIM_SIP_RUIM;
  }
  else if(service_3gpd_sip)
  {
    nvruim_session_data_ptr[context]->control_3gpd = NVRUIM_3GPD_MIP_NV_SIP_RUIM;
  }
  else if(service_3gpd_mip)
  {
    nvruim_session_data_ptr[context]->control_3gpd = NVRUIM_3GPD_MIP_RUIM_SIP_NV;
  }
  else
  {
    nvruim_session_data_ptr[context]->control_3gpd = NVRUIM_3GPD_MIP_NV_SIP_NV;
  }

  if(NVRUIM_NON_3GPD_CARD == nvruim_session_data_ptr[context]->non_3gpd_cdma_card)
  {
    /* set nv_ruim_3gpd control flag to
       UIM_3GPD_MIP_NV_SIP_NV for non 3gpd card*/
    nvruim_session_data_ptr[context]->control_3gpd = NVRUIM_3GPD_MIP_NV_SIP_NV;
  }
} /* uim_init_3gpd_preference */


/*===========================================================================

FUNCTION UIM_3GPD_SUPPORT

DESCRIPTION
This function returns the preferred source for 3GPD operations.


DEPENDENCIES
None.

RETURN VALUE
nvruim_3gpd_support_status - either NVRUIM_3GPD_SUPPORT or
NVRUIM_3GPD_MIP_NV_SIP_NV

SIDE EFFECTS
None.

===========================================================================*/
nvruim_3gpd_support_status uim_3gpd_support
(
  void
)
{
  if (nvruim_generic_data_ptr != NULL)
  {
    return uim_3gpd_support_per_sub(
             nvruim_convert_session_to_as_id(nvruim_generic_data_ptr->nvruim_active_session));
  }
  return NVRUIM_3GPD_MIP_NV_SIP_NV;
} /* uim_3gpd_support */


/*===========================================================================

FUNCTION UIM_3GPD_SUPPORT_PER_SUB

DESCRIPTION
  This function returns the preferred source for 3GPD operations
  for given sub.

DEPENDENCIES
  None.

RETURN VALUE
  nvruim_3gpd_support_status - either NVRUIM_3GPD_SUPPORT or
  NVRUIM_3GPD_MIP_NV_SIP_NV

SIDE EFFECTS
  None.

===========================================================================*/
nvruim_3gpd_support_status uim_3gpd_support_per_sub
(
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_nv_context_type   context        = NVRUIM_NV_CONTEXT_INVALID;

  nvruim_session = nvruim_convert_as_id_to_nvruim_session(as_id);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session))
  {
    return NVRUIM_3GPD_MIP_NV_SIP_NV;
  }

  context  = nvruim_map_nvruim_session_to_context(nvruim_session);
  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NVRUIM_3GPD_MIP_NV_SIP_NV;
  }

  if (nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->control_3gpd;
  }

  return NVRUIM_3GPD_MIP_NV_SIP_NV;
} /* uim_3gpd_support_per_sub */


/*===========================================================================

FUNCTION NVRUIM_LBS_INIT_SUPPORT

DESCRIPTION
  This function is called from MMGSDI/GPS task during initialization for LBS
  Support. It passes in a boolean value which indicates whether or not the
  RUIM card have support for service 24 (LBS)in its CDMA Service Table.
  The requirement on the RUIM card for this serviec is defined in Open Market
  Handset RUIM Specification(CDG Document 166).

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Sets the nvruim_lbs_support global variable for LBS support
===========================================================================*/
void nvruim_lbs_init_support
(
  boolean                       service_lbs,
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_nv_context_type   context        = nvruim_map_nvruim_session_to_context(nvruim_session);

  nvruim_session = nvruim_convert_as_id_to_nvruim_session(as_id);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) ||
      NVRUIM_CONTEXT_NOT_VALID(context)        ||
      nvruim_session_data_ptr[context] == NULL)
  {
    return;
  }
  nvruim_session_data_ptr[context]->nvruim_lbs_support = service_lbs;
} /* nvruim_lbs_init_support */


/*===========================================================================

FUNCTION NVRUIM_DATA_3GPD_INIT_EXTENSIONS_SUPPORT

DESCRIPTION
  This function is called from the Data task during initialization for 3GPD
  Extentions. It passes in a boolean which indicates whether or not the RUIM
  provides support for service 15 (Messagind and 3GPD Extentions).
  The requirement on the RUIM card for this serviec is defined in Open Market
  Handset RUIM Specification(CDG Document 166).

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Sets the ext_support_3gpd global variable for extended 3GPD RUIM support
===========================================================================*/
void nvruim_data_3gpd_init_extensions_support
(
  boolean                       service_3gpd_extensions,
  nvruim_session_enum_type      nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

  nvruim_session_data_ptr[context]->ext_support_3gpd = service_3gpd_extensions;
} /* nvruim_data_3gpd_init_extensions_support */


/*===========================================================================

FUNCTION UIM_INIT_AN_HRPD_PREFERENCE

DESCRIPTION
This function is called from MMGSDI/3GPP2 task during initialization for HRPD.
It passes in a boolean which indicates whether or not the RUIM provides
for HRPD services, and a boolean which indicates whether cave is to be used

DEPENDENCIES
None.

RETURN VALUE

SIDE EFFECTS
Sets the hrpd_control global variable for RUIM support of HRPD
Sets the hrpd_cdma_svc global variable to indicate CDMA service support
===========================================================================*/
void uim_init_an_hrpd_preference
(
  boolean                       service_hrpd,
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_nv_context_type   context        = NVRUIM_NV_CONTEXT_INVALID;

  UIM_MSG_LOW_2("uim_init_an_hrpd_preference, service_hrpd: 0x%x, as_id: 0x%x",
                 service_hrpd, as_id);
  nvruim_session = nvruim_convert_as_id_to_nvruim_session(as_id);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session))
  {
    return;
  }

  context  = nvruim_map_nvruim_session_to_context(nvruim_session);
  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

  nvruim_session_data_ptr[context]->hrpd_cdma_svc = service_hrpd;

#ifdef FEATURE_UIM_RUN_TIME_ENABLE
  if(NV_RTRE_CONTROL_USE_RUIM != nvruim_get_rtre_control_value(nvruim_session))
  {
    /* Use NV for HRPD support */
    nvruim_session_data_ptr[context]->hrpd_control = NVRUIM_AN_HRPD_NO_SUPPORT;
    return;
  }
  else
#endif /* FEATURE_UIM_RUN_TIME_ENABLE */
  if((service_hrpd
      && nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_MISCONFIG_RUIM_N5_WORKAROUND_I, context)
      && (!(nvruim_session_data_ptr[context]->hrpd_disabled_card_flag ||
            nvruim_session_data_ptr[context]->uim_use_cave_default))) ||
     (service_hrpd
      && !nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_MISCONFIG_RUIM_N5_WORKAROUND_I, context)))
  {
    /* Use R-UIM for HRPD Support */
    nvruim_session_data_ptr[context]->hrpd_control = NVRUIM_AN_HRPD_SUPPORT;
    return;
  }
#ifdef FEATURE_UIM_CAVE_AN_AUTH
  else
  {
    nvruim_session_data_ptr[context]->hrpd_control = NVRUIM_AN_HRPD_USE_CAVE;
    return;
  }
#else
  if(!nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_AN_HRPD_FALLBACK_I, context))
  {
    nvruim_session_data_ptr[context]->hrpd_control = NVRUIM_AN_HRPD_SUPPORT;
  }
  else
  {
    nvruim_session_data_ptr[context]->hrpd_control = NVRUIM_AN_HRPD_NO_SUPPORT;
  }
#endif /* FEATURE_UIM_CAVE_AN_AUTH */
} /* uim_init_an_hrpd_preference */


/*===========================================================================

FUNCTION UIM_AN_HRPD_SUPPORT

DESCRIPTION
This function returns the preferred source for HRPD operations.


DEPENDENCIES
None.

RETURN VALUE
uim_an_hrpd_support_status - either NVRUIM_AN_HRPD_SUPPORT, NVRUIM_AN_HRPD_NO_SUPPORT
or UIM_AN_HRPD_USE_CAVE

SIDE EFFECTS
None.

===========================================================================*/
nvruim_an_hrpd_support_status uim_an_hrpd_support
(
  void
)
{
  if (nvruim_generic_data_ptr != NULL)
  {
    return uim_an_hrpd_support_per_sub(
             nvruim_convert_session_to_as_id(nvruim_generic_data_ptr->nvruim_active_session));
  }
  return NVRUIM_AN_HRPD_NO_SUPPORT;
} /* uim_an_hrpd_support */


/*===========================================================================

FUNCTION UIM_AN_HRPD_SUPPORT_PER_SUB

DESCRIPTION
  This function checks whether HRPD data and processing should come from the
  RUIM or from NV for given sub.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
nvruim_an_hrpd_support_status uim_an_hrpd_support_per_sub
(
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_nv_context_type   context        = NVRUIM_NV_CONTEXT_INVALID;

  nvruim_session = nvruim_convert_as_id_to_nvruim_session(as_id);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session))
  {
    return NVRUIM_AN_HRPD_NO_SUPPORT;
  }

  context  = nvruim_map_nvruim_session_to_context(nvruim_session);
  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NVRUIM_AN_HRPD_NO_SUPPORT;
  }

  if (nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->hrpd_control;
  }

  return NVRUIM_AN_HRPD_NO_SUPPORT;
} /* uim_an_hrpd_support_per_sub */


/*===========================================================================

FUNCTION NVRUIM_SET_ESN_USAGE

DESCRIPTION
This function sets the usage indicator that is used by the NV-RUIM module
to decide whether ESN or R-UIM ID is to be returned for an ESN read .

DEPENDENCIES
None.

RETURN VALUE
None.

SIDE EFFECTS
Updates the usage indicator variable esn_usage.

===========================================================================*/
void nvruim_set_esn_usage
(
  nvruim_esn_usage_type         esn_usage,
  nvruim_session_enum_type      nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

  nvruim_session_data_ptr[context]->esn_usage = esn_usage;
} /* nvruim_set_esn_usage */


/*===========================================================================

FUNCTION NVRUIM_GET_ESN_USAGE

DESCRIPTION
This function gets the usage indicator that is used by the NV-RUIM module
to decide whether ESN or R-UIM ID is to be returned for an ESN read .

DEPENDENCIES
Non-reentrant

RETURN VALUE
None.

SIDE EFFECTS

===========================================================================*/
nvruim_esn_usage_type nvruim_get_esn_usage
(
  nvruim_session_enum_type nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return NV_RUIM_USE_ESN;
  }

  if (nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->esn_usage;
  }
  return NV_RUIM_USE_ESN;
}/* nvruim_get_esn_usage */


/*===========================================================================

FUNCTION NVRUIM_GET_MMGSDI_ITEM_AND_FILE_STRUCT

DESCRIPTION
  This function will determine the file structure for an EF and for UICC cards
  provide the equivalent CSIM file type

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean nvruim_get_mmgsdi_item_and_file_struct
(
  mmgsdi_file_enum_type             mmgsdi_item_ptr,
  mmgsdi_file_structure_enum_type  *file_struct_ptr,
  mmgsdi_access_type               *nvruim_mmgsdi_access_ptr,
  nvruim_nv_context_type            context
)
{
  uint8                    file_index = 0;

  if (file_struct_ptr == NULL           ||
      nvruim_mmgsdi_access_ptr == NULL  ||
      NVRUIM_CONTEXT_NOT_VALID(context) ||
      nvruim_session_data_ptr[context] == NULL)
  {
    return FALSE;
  }

  for (file_index=0;file_index<NVRUIM_CDMA_FILE_LOOKUP_TABLE_SIZE;file_index++)
  {
    if (mmgsdi_item_ptr == nvruim_cdma_file_lookup_table[file_index].mmgsdi_ruim_file)
    {
      *file_struct_ptr = nvruim_cdma_file_lookup_table[file_index].file_struct;
      nvruim_mmgsdi_access_ptr->access_method = MMGSDI_EF_ENUM_ACCESS;

      if (nvruim_session_data_ptr[context]->uim_instr_prot == MMGSDI_UICC)
      {
        nvruim_mmgsdi_access_ptr->file.file_enum =
          nvruim_cdma_file_lookup_table[file_index].mmgsdi_csim_file;
      }
      else
      {
        nvruim_mmgsdi_access_ptr->file.file_enum =
          nvruim_cdma_file_lookup_table[file_index].mmgsdi_ruim_file;
      }
      return TRUE;
    }
  }
  return FALSE;
} /* nvruim_get_mmgsdi_item_and_file_struct */


/*===========================================================================

FUNCTION NVRUIM_MMGSDI_ACCESS_CB

DESCRIPTION
  This function will be called by MMGSDI after the access operation is complete

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void nvruim_mmgsdi_access_cb
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
)
{
  nvruim_cmd_type                 *cmd_ptr = NULL;

  NVRUIM_RETURN_IF_NULL(nvruim_generic_data_ptr);

  if(cnf_ptr  == NULL)
  {
    return;
  }

  cmd_ptr = (nvruim_cmd_type *)cnf_ptr->response_header.client_data;

  if(cmd_ptr  == NULL)
  {
    return;
  }

  if(NVRUIM_CONTEXT_NOT_VALID(cmd_ptr->nv_context))
  {
    return;
  }

  UIM_MSG_HIGH_2("nvruim_mmgsdi_access_cb: Cnf type 0x%x, Status 0x%x",
                 cnf, status);

  cmd_ptr->report_status = NVRUIM_FAIL;

  if(cnf_ptr && cnf_ptr->response_header.status_word.present)
  {
    cmd_ptr->status_word.present = TRUE;
    cmd_ptr->status_word.sw1 = cnf_ptr->response_header.status_word.sw1;
    cmd_ptr->status_word.sw2 = cnf_ptr->response_header.status_word.sw2;
  }
  else if(status == MMGSDI_ACCESS_DENIED)
  {
    /* In some cases, MMGSDI may first validate the access by reading the EF-ARR
       for the file and if MMGSDI finds out the access is denied, it sends
       MMGSDI_ACCESS_DENIED to its clients (without populating the status word).
       In that case, lets populate the NVRUIM sw1 and sw2 as 0x69 0x82 for error
       processing in NVRUIM */
    cmd_ptr->status_word.present = TRUE;
    cmd_ptr->status_word.sw1 = NVRUIM_SW1_CMD_NOT_ALLOWED;
    cmd_ptr->status_word.sw2 = NVRUIM_SW2_SECURITY_STATUS_NOT_SATISFIED;
  }

  if ((status == MMGSDI_SUCCESS) && (cnf_ptr != NULL))
  {
    cmd_ptr->report_status = NVRUIM_PASS;

    if (cnf == MMGSDI_READ_CNF)
    {
      if ((cmd_ptr->data.data_len >
             cnf_ptr->read_cnf.read_data.data_len) ||
          (cmd_ptr->data.data_len == 0))
      {
        cmd_ptr->data.data_len = cnf_ptr->read_cnf.read_data.data_len;
      }

      if ((cnf_ptr->read_cnf.read_data.data_ptr != NULL) &&
          (cmd_ptr->data.data_len > 0) &&
          (cmd_ptr->data.data_ptr != NULL))
      {
        /* The memory allocation has already occurred in the calling function
           so directly copying content of cnf ptr */

        nvruim_memscpy((void*)cmd_ptr->data.data_ptr,
                       cmd_ptr->data.data_len,
                       (const void*)cnf_ptr->read_cnf.read_data.data_ptr,
                       cmd_ptr->data.data_len);
      }

    }
    else if (cnf == MMGSDI_WRITE_CNF)
    {
      /* Do nothing */
    }
    else if (cnf == MMGSDI_SEARCH_CNF)
    {
      /* Copy the data returned in search CNF to the user data global*/
      cmd_ptr->data.data_len = cnf_ptr->search_cnf.searched_record_nums.data_len;

      if (cnf_ptr->search_cnf.searched_record_nums.data_ptr != NULL &&
          cmd_ptr->data.data_len > 0)
      {
        cmd_ptr->data.data_ptr = nvruim_mem_malloc(cmd_ptr->data.data_len);
        if (cmd_ptr->data.data_ptr != NULL )
        {
          nvruim_memscpy((void*)cmd_ptr->data.data_ptr,
                         cmd_ptr->data.data_len,
                         (const void*)cnf_ptr->search_cnf.searched_record_nums.data_ptr,
                         cnf_ptr->search_cnf.searched_record_nums.data_len);
        }
      }
    }
    else if (cnf == MMGSDI_GET_FILE_ATTR_CNF)
    {
      cmd_ptr->num_records = cnf_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.num_of_rec;
    }
  }

  /* Notify calling task that the access action is complete */
  nvruim_generic_data_ptr->current_commands_in_context[cmd_ptr->nv_context]--;
  (void) rex_set_sigs(cmd_ptr->cmd_hdr.task_ptr, cmd_ptr->cmd_hdr.sigs);
} /* nvruim_mmgsdi_access_cb */


/*===========================================================================

FUNCTION NVRUIM_SEND_MMGSDI_CANCEL

DESCRIPTION
  This function is responsible for cancelling NVRUIM's pending command(s)
  with MMGSDI.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void nvruim_send_mmgsdi_cancel
(
  nvruim_nv_context_type    context
)
{
  mmgsdi_return_enum_type  mmgsdi_status  = MMGSDI_ERROR;

  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }
  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

  mmgsdi_status = mmgsdi_session_cancel_requests(nvruim_session_data_ptr[context]->session_id);

  UIM_MSG_MED_1 ("Cancel request to MMGSDI from NVRUIM, status: 0x%x",
                 mmgsdi_status);
} /* nvruim_send_mmgsdi_cancel */


/*===========================================================================

FUNCTION NVRUIM_SESSION_INIT

DESCRIPTION
  This function will register with MMGSDI and obtain a client id.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void nvruim_session_init
(
  void
)
{
  static boolean           nvruim_req_for_session_init = FALSE;
  mmgsdi_return_enum_type  nvruim_mmgsdi_status        = MMGSDI_ERROR;

  /* Do Client registration only if not already done */
  if((nvruim_mmgsdi_client_id == MMGSDI_CLIENT_ID_ZERO) &&
     (FALSE == nvruim_req_for_session_init))
  {
    nvruim_mmgsdi_status = mmgsdi_client_id_and_evt_reg_ext(
                             nvruim_mmgsdi_evt_cb,
                             nvruim_mmgsdi_reg_cb,
                             (mmgsdi_client_data_type)MMGSDI_NVRUIM_CLIENT_CMD,
                             NVRUIM_CLIENT_NAME,
                             sizeof(NVRUIM_CLIENT_NAME));
    if (nvruim_mmgsdi_status != MMGSDI_SUCCESS)
    {
      /* No further action necessary since without a client id
         there will be no session id and CDMA subscription and
         hence all nvruim access attempts will fail */
      UIM_MSG_ERR_0("nvruim_session_init:Client reg failed");
    }
    nvruim_req_for_session_init = TRUE;
  }
} /* nvruim_session_init */


/*===========================================================================

FUNCTION NVRUIM_MMGSDI_REG_CB

DESCRIPTION
  This function will be called by MMGSDI when the NVRUIM registration is
  complete and a client id is available or when session open to 1x
  provisioning app is complete and session id is available

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void nvruim_mmgsdi_reg_cb
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
)
{
  mmgsdi_return_enum_type       nvruim_mmgsdi_status = MMGSDI_ERROR;

  if (status != MMGSDI_SUCCESS)
  {
    /* No further action necessary since without a client id or session id
       there will be no access to a CDMA subscription and hence all nvruim
       access attempts will fail */
    UIM_MSG_ERR_2("nvruim_mmgsd_reg_cb:Reg type 0x%x failed.Status 0x%x",
                  cnf, status);
    return;
  }

  if (cnf == MMGSDI_CLIENT_ID_AND_EVT_REG_CNF)
  {
    nvruim_mmgsdi_client_id =
      cnf_ptr->client_id_and_evt_reg_cnf.response_header.client_id;
    /* Do Session registration only if client registration is already done */
    if(nvruim_mmgsdi_client_id != MMGSDI_CLIENT_ID_ZERO)
    {
      /* Include masks of the 2 session types needed */
      uint32 session_type_mask = MMGSDI_1X_PROV_PRI_SESSION_MASK |
                                 MMGSDI_1X_PROV_SEC_SESSION_MASK |
                                 MMGSDI_1X_PROV_TER_SESSION_MASK;

      nvruim_mmgsdi_status =
        mmgsdi_session_open_ext(nvruim_mmgsdi_client_id,
                                session_type_mask,
                                nvruim_mmgsdi_evt_cb,
                                0,
                                nvruim_mmgsdi_reg_cb,
                                (mmgsdi_client_data_type)MMGSDI_NVRUIM_CLIENT_CMD);
      if (nvruim_mmgsdi_status != MMGSDI_SUCCESS)
      {
        /* No further action necessary since without a session id there will
        be no access to a CDMA subscription and hence all nvruim access
        attempts will fail */
        UIM_MSG_ERR_1("nvruim_session_open:Session open ext failed.Status 0x%x",
                      nvruim_mmgsdi_status);
      }
    }
    else
    {
      UIM_MSG_ERR_1("nvruim_session_open: No valid client Id available 0x%x",
                    nvruim_mmgsdi_status);
    }
  }
  else if (cnf == MMGSDI_SESSION_OPEN_EXT_CNF)
  {
    uint8 i = 0;

    if (cnf_ptr->session_open_ext_cnf.session_info_ptr == NULL)
    {
      UIM_MSG_ERR_0("nvruim_mmgsdi_reg_cb: NULL session_info_ptr, not updating");
      return;
    }

    /* Store the session id that were opened */
    for (i = 0; i < cnf_ptr->session_open_ext_cnf.num_sessions; i++)
    {
      mmgsdi_session_type_enum_type session_type =
        cnf_ptr->session_open_ext_cnf.session_info_ptr[i].session_type;
      mmgsdi_session_id_type session_id =
        cnf_ptr->session_open_ext_cnf.session_info_ptr[i].session_id;

      switch (session_type)
      {
        case MMGSDI_1X_PROV_PRI_SESSION:
          nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_PRIMARY]->session_id = session_id;
          break;

        case MMGSDI_1X_PROV_SEC_SESSION:
          nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_SECONDARY]->session_id = session_id;
          break;

        case MMGSDI_1X_PROV_TER_SESSION:
          nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_TERTIARY]->session_id = session_id;
          break;

        default:
          break;
      }
    }
  }
}/* nvruim_mmgsdi_reg_cb */


/*===========================================================================

FUNCTION NVRUIM_MMGSDI_EVT_CB

DESCRIPTION
  This function will be called by MMGSDI to notify NVRUIM about card
  and application related events

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void nvruim_mmgsdi_evt_cb
(
  const mmgsdi_event_data_type *event_ptr
)
{
  uint8                         slot_index          = NVRUIM_SLOT_INDEX_INVALID;
  mmgsdi_nv_context_type        mmgsdi_nv_context   = MMGSDI_NV_CONTEXT_INVALID;

  if(!event_ptr)
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL_2(nvruim_generic_data_ptr,
                          nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_PRIMARY]);
  NVRUIM_RETURN_IF_NULL_2(nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_SECONDARY],
                          nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_TERTIARY]);

  switch (event_ptr->evt)
  {
    case MMGSDI_CARD_ERROR_EVT:
      UIM_MSG_ERR_1("nvruim_mmgsdi_evt_cb: Card error event for slot 0x%x",
                    event_ptr->data.card_inserted.slot);
      if ((nvruim_convert_slot_id_to_index(event_ptr->data.card_inserted.slot,
                                           &slot_index)) &&
          (slot_index < NVRUIM_MAX_CARD_COUNT))
      {
        nvruim_sim_busy[slot_index] = FALSE;
      }
      break;

    case MMGSDI_SESSION_CHANGED_EVT:
      UIM_MSG_HIGH_3("nvruim_mmgsdi_evt_cb: session changed evt for app_type: 0x%x, slot: 0x%x, activated: 0x%x",
                     event_ptr->data.session_changed.app_info.app_data.app_type,
                     event_ptr->data.session_changed.app_info.slot,
                     event_ptr->data.session_changed.activated);

      if (event_ptr->session_id == nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_PRIMARY]->session_id)
      {
        mmgsdi_nv_context   = MMGSDI_NV_CONTEXT_PRIMARY;
      }
      else if (event_ptr->session_id == nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_SECONDARY]->session_id)
      {
        mmgsdi_nv_context   = MMGSDI_NV_CONTEXT_SECONDARY;
      }
      else if (event_ptr->session_id == nvruim_session_data_ptr[NVRUIM_NV_CONTEXT_TERTIARY]->session_id)
      {
        mmgsdi_nv_context   = MMGSDI_NV_CONTEXT_TERTIARY;
      }
      else
      {
        break;
      }

      if (event_ptr->data.session_changed.activated)
      {
        nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_session_active= TRUE;
        nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_slot_id = event_ptr->data.session_changed.app_info.slot;
        /* Set instruction protocol */
        if (event_ptr->data.session_changed.app_info.app_data.app_type ==
              MMGSDI_APP_CSIM)
        {
          nvruim_session_data_ptr[mmgsdi_nv_context]->uim_instr_prot = MMGSDI_UICC;
        }
        else if (event_ptr->data.session_changed.app_info.app_data.app_type ==
                   MMGSDI_APP_RUIM)
        {
          nvruim_session_data_ptr[mmgsdi_nv_context]->uim_instr_prot = MMGSDI_ICC;
        }

        /* Set subscription as available */
        if (nvruim_convert_slot_id_to_index(nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_slot_id,
                                            &slot_index))
        {
          nvruim_dir_present[slot_index] = NVRUIM_CDMA_SUBSCRIPTION_PRESENT;
          UIM_MSG_HIGH_1("nvruim session is up on slot_index 0x%x", slot_index);
        }
      }
      else
      {
        (void)nvruim_convert_slot_id_to_index(nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_slot_id,
                                              &slot_index);
        /* Set subscription as unavailable */
        nvruim_dir_present[slot_index] = NVRUIM_CDMA_SUBSCRIPTION_NOT_PRESENT;
        nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_session_active = FALSE;

        /* Reset SMSCAP and BCSMS service. WMS will again set the value when
           CDMA subscription become active */
        nvruim_session_data_ptr[mmgsdi_nv_context]->bcsms_svc = FALSE;
        nvruim_session_data_ptr[mmgsdi_nv_context]->smscap_svc = FALSE;

        mmgsdi_init_rtre_config_per_context(mmgsdi_nv_context);
        /* Also update the Slot ID & corresponding SIM Busy info */
        nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_slot_id = MMGSDI_MAX_SLOT_ID_ENUM;
        if ((nvruim_convert_slot_id_to_index(event_ptr->data.session_changed.app_info.slot,
                                             &slot_index)) &&
            (slot_index < NVRUIM_MAX_CARD_COUNT))
        {
          nvruim_sim_busy[slot_index] = FALSE;
        }
      }
      break;

    case MMGSDI_SUBSCRIPTION_READY_EVT:
      /* In case NVRUIM registers late with MMGSDI then it shall only receive
         subcription ready event. It needs to handle this event in same way as
         it handles SESSION_CHANGED */
      if (event_ptr->session_id == nvruim_session_data_ptr[MMGSDI_NV_CONTEXT_PRIMARY]->session_id)
      {
        mmgsdi_nv_context   = MMGSDI_NV_CONTEXT_PRIMARY;
      }
      else if (event_ptr->session_id == nvruim_session_data_ptr[MMGSDI_NV_CONTEXT_SECONDARY]->session_id)
      {
        mmgsdi_nv_context   = MMGSDI_NV_CONTEXT_SECONDARY;
      }
      else if (event_ptr->session_id == nvruim_session_data_ptr[MMGSDI_NV_CONTEXT_TERTIARY]->session_id)
      {
        mmgsdi_nv_context   = MMGSDI_NV_CONTEXT_TERTIARY;
      }
      else
      {
        break;
      }

      UIM_MSG_HIGH_3("nvruim_mmgsdi_evt_cb: subscrip ready evt for app_type: 0x%x, slot: 0x%x, mmgsdi_nv_context: 0x%x",
                     event_ptr->data.subscription_ready.app_info.app_data.app_type,
                     event_ptr->data.subscription_ready.app_info.slot,
                     mmgsdi_nv_context);

      nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_session_active = TRUE;
      nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_slot_id = event_ptr->data.subscription_ready.app_info.slot;
      if (event_ptr->data.subscription_ready.app_info.app_data.app_type ==
          MMGSDI_APP_CSIM)
      {
        nvruim_session_data_ptr[mmgsdi_nv_context]->uim_instr_prot = MMGSDI_UICC;
      }
      else if (event_ptr->data.subscription_ready.app_info.app_data.app_type ==
               MMGSDI_APP_RUIM)
      {
        nvruim_session_data_ptr[mmgsdi_nv_context]->uim_instr_prot = MMGSDI_ICC;
      }
      /* Set subscription as available and ready */
      if (nvruim_convert_slot_id_to_index(nvruim_session_data_ptr[mmgsdi_nv_context]->mmgsdi_slot_id,
                                          &slot_index))
      {
        nvruim_dir_present[slot_index] = NVRUIM_CDMA_SUBSCRIPTION_READY;
      }
      break;

    case MMGSDI_SIM_BUSY_EVT:
      /* Update the SIM busy status */
      if (nvruim_convert_slot_id_to_index(event_ptr->data.sim_busy.slot,
                                          &slot_index))
      {
        mmgsdi_nv_context_type current_context = MMGSDI_NV_CONTEXT_INVALID;
        rex_sigs_type          sigs            = 0;

        if ((slot_index < NVRUIM_MAX_CARD_COUNT) &&
            (nvruim_sim_busy[slot_index] != event_ptr->data.sim_busy.sim_busy))
        {
          nvruim_sim_busy[slot_index] = event_ptr->data.sim_busy.sim_busy;

          for(current_context = MMGSDI_NV_CONTEXT_PRIMARY; current_context <= NVRUIM_NV_CONTEXT_TERTIARY; current_context++)
          {
            if(event_ptr->data.sim_busy.slot == nvruim_session_data_ptr[current_context]->mmgsdi_slot_id)
            {
              break;
            }
          }

          if (NVRUIM_CONTEXT_NOT_VALID(current_context))
          {
            break;
          }

          /* Set signal as par context */
          if(current_context == MMGSDI_NV_CONTEXT_PRIMARY)
          {
            sigs = NVRUIM_SIM_BUSY_PRIMARY_SIG;
          }
          else if(current_context == MMGSDI_NV_CONTEXT_SECONDARY)
          {
            sigs = NVRUIM_SIM_BUSY_SECONDARY_SIG;
          }
          else
          {
            sigs = NVRUIM_SIM_BUSY_TERTIARY_SIG;
          }

          /* If card on that slot is busy & there is a pending command on that
             slot, notify NVRUIM task so that those requests can be cancelled.
             MMGSDI is expected to responds back with an error in the callback */
          if ((nvruim_sim_busy[slot_index]) &&
              (nvruim_generic_data_ptr->current_commands_in_context[current_context] > 0))
          {
            UIM_MSG_HIGH_2("Received SIM BUSY, cancelling pending 0x%x requests on slot 0x%x",
                           nvruim_generic_data_ptr->current_commands_in_context[current_context],
                           event_ptr->data.sim_busy.slot);
            (void) rex_set_sigs(UIM_NVRUIM_TCB, sigs);
          }
        }
      }
      break;

    case MMGSDI_CARD_INSERTED_EVT:
      /* Update the SIM busy status */
      UIM_MSG_HIGH_1("nvruim_mmgsdi_evt_cb: Card inserted evt for slot 0x%x",
                     event_ptr->data.card_inserted.slot);

      if ((nvruim_convert_slot_id_to_index(event_ptr->data.card_inserted.slot,
                                           &slot_index)) &&
          (slot_index < NVRUIM_MAX_CARD_COUNT))
      {
        nvruim_sim_busy[slot_index] = FALSE;
      }
      break;

    default:
      break;
  }
}/* nvruim_mmgsdi_evt_cb */


/*===========================================================================

FUNCTION NVRUIM_MMGSDI_ITEM_IS_PIN_PROTECTED

DESCRIPTION
  This function will determine the file is PIN protected for the given access

DEPENDENCIES
  None

RETURN VALUE
  boolean:  TRUE  if the file is PIN protected
            FALSE if the file is not PIN protected

SIDE EFFECTS
  None

===========================================================================*/
static boolean nvruim_mmgsdi_item_is_pin_protected (
  mmgsdi_file_enum_type mmgsdi_item
)
{
  uint8  file_index = 0;

  for (file_index = 0; file_index < NVRUIM_CDMA_FILE_LOOKUP_TABLE_SIZE; file_index++)
  {
    if (mmgsdi_item == nvruim_cdma_file_lookup_table[file_index].mmgsdi_ruim_file)
    {
      return nvruim_cdma_file_lookup_table[file_index].pin_protected;
    }
  }

  UIM_MSG_ERR_1("mmgsdi_file 0x%x not found in nvruim_cdma_file_lookup_table",
                mmgsdi_item);
  return FALSE;
} /* nvruim_mmgsdi_item_is_pin_protected */


/*===================================================================
FUNCTION NVRUIM_PPP_CAVE_FALLBACK

DESCRIPTION
  Sets the hrpd control variable to Cave

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==================================================================*/
void nvruim_ppp_cave_fallback (
  void
)
{
  NVRUIM_RETURN_IF_NULL(nvruim_generic_data_ptr);

  nvruim_ppp_cave_fallback_per_sub(
     nvruim_convert_session_to_as_id(nvruim_generic_data_ptr->nvruim_active_session));
} /* nvruim_ppp_cave_fallback */


/*===================================================================
FUNCTION NVRUIM_PPP_CAVE_FALLBACK_PER_SUB

DESCRIPTION
  Sets the hrpd control variable to Cave for given sub

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==================================================================*/
void nvruim_ppp_cave_fallback_per_sub
(
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_nv_context_type   context        = NVRUIM_NV_CONTEXT_INVALID;

  UIM_MSG_LOW_1("nvruim_ppp_cave_fallback_per_sub, as_id: 0x%x",
                as_id);

  nvruim_session = nvruim_convert_as_id_to_nvruim_session(as_id);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session))
  {
    return;
  }

  context  = nvruim_map_nvruim_session_to_context(nvruim_session);
  if (NVRUIM_CONTEXT_NOT_VALID(context) || nvruim_session_data_ptr[context] == NULL)
  {
    return;
  }

  if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_MISCONFIG_RUIM_N5_WORKAROUND_I,
                                   context))
  {
    if (nvruim_session_data_ptr[context]->hrpd_allow_to_fallback_to_cave  == TRUE)
    {
      UIM_MSG_HIGH_0("MD5 failed, falling back to cave");
      nvruim_session_data_ptr[context]->hrpd_control  = NVRUIM_AN_HRPD_USE_CAVE;
      nvruim_session_data_ptr[context]->uim_use_cave_default = TRUE;
    }
    else
    {
      UIM_MSG_HIGH_0("nvruim_ppp_cave_fallback is not allowed");
      nvruim_session_data_ptr[context]->uim_use_cave_default = FALSE;
    }
  }
}/* nvruim_ppp_cave_fallback_per_sub */


/*===========================================================================

FUNCTION NVRUIM_DATA_3GPD_EXT_SUPPORT

DESCRIPTION
This function returns 3GPD_EXT service availability

DEPENDENCIES
This function should only be called after SUBSCRIPTION_READY evt

RETURN VALUE
 TRUE : When card has 3GPD_EXT service available
 FALSE: When card does not have 3GPD_EXT service

SIDE EFFECTS
None.

===========================================================================*/
boolean nvruim_data_3gpd_ext_support (
  void
)
{
  if (nvruim_generic_data_ptr != NULL)
  {
    return nvruim_data_3gpd_ext_support_per_sub(
              nvruim_convert_session_to_as_id(nvruim_generic_data_ptr->nvruim_active_session));
  }
  return FALSE;
} /* nvruim_data_3gpd_ext_support */


/*===========================================================================

FUNCTION NVRUIM_DATA_3GPD_EXT_SUPPORT_PER_SUB

DESCRIPTION
  This function returns 3GPD_EXT service availability for given sub

DEPENDENCIES
  This function should only be called after SUBSCRIPTION_READY evt

RETURN VALUE
  TRUE : When card has 3GPD_EXT service available
  FALSE: When card does not have 3GPD_EXT service

SIDE EFFECTS
  None.

===========================================================================*/
boolean nvruim_data_3gpd_ext_support_per_sub (
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
  nvruim_nv_context_type   context        = NVRUIM_NV_CONTEXT_INVALID;

  nvruim_session = nvruim_convert_as_id_to_nvruim_session(as_id);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session))
  {
    return FALSE;
  }

  context  = nvruim_map_nvruim_session_to_context(nvruim_session);
  if (NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return FALSE;
  }

  if (nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->ext_support_3gpd;
  }

  return FALSE;
} /* nvruim_data_3gpd_ext_support_per_sub */


/*===========================================================================

FUNCTION NVRUIM_INIT_HRPD_CAPABILITY

DESCRIPTION
  This function is called from the GSDI task during nvruim initialization.
  It passes in a boolean which indicates whether or not the RUIM have a
  HRPD capability

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Sets the hrpd_control global variable for RUIM support of HRPD
===========================================================================*/
void nvruim_init_hrpd_capability (
  boolean                       hrpd_disabled_card_flag,
  nvruim_session_enum_type      nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

  if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_MISCONFIG_RUIM_N5_WORKAROUND_I,
                                   context))
  {
    nvruim_session_data_ptr[context]->hrpd_disabled_card_flag = hrpd_disabled_card_flag;
    UIM_MSG_HIGH_1("HRPD disabled card flag is 0x%x",
                   nvruim_session_data_ptr[context]->hrpd_disabled_card_flag);
  }
} /* nvruim_init_hrpd_capability */


/*===========================================================================

FUNCTION NVRUIM_INIT_FALLBACK_TO_CAVE

DESCRIPTION
  This function is called from the GSDI task during nvruim initialization.
  It passes in a boolean which indicates fallback to CAVE is allowed or
  not after the MD5 failure

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void nvruim_init_fallback_to_cave (
  boolean                       allow_to_fallback,
  nvruim_session_enum_type      nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) || NVRUIM_CONTEXT_NOT_VALID(context))
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL(nvruim_session_data_ptr[context]);

  if(nvruim_is_mmgsdi_item_enabled(MMGSDI_NV_UIM_MISCONFIG_RUIM_N5_WORKAROUND_I,
                                   context))
  {
    nvruim_session_data_ptr[context]->hrpd_allow_to_fallback_to_cave = allow_to_fallback;
    UIM_MSG_HIGH_1("fallback to cave flag is 0x%x",
                   nvruim_session_data_ptr[context]->hrpd_allow_to_fallback_to_cave);
  }
} /* nvruim_init_fallback_to_cave */


/*===========================================================================

FUNCTION nvruim_is_bcsms_svc_avail

DESCRIPTION
  Gets the bcsms_svc state

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_bcsms_svc_avail (
  nvruim_nv_context_type     context
)
{
  if (!NVRUIM_CONTEXT_NOT_VALID(context) && nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->bcsms_svc;
  }
  return FALSE;
} /* nvruim_is_bcsms_svc_avail() */


/*===========================================================================

FUNCTION nvruim_is_smscap_svc_avail

DESCRIPTION
  Gets the smscap_svc state

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_smscap_svc_avail (
  nvruim_nv_context_type      context
)
{
  if (!NVRUIM_CONTEXT_NOT_VALID(context) && nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->smscap_svc;
  }
  return FALSE;
} /* nvruim_is_smscap_svc_avail() */


/*===========================================================================

FUNCTION NVRUIM_IS_LBS_SUPPORT_AVAIL

DESCRIPTION
  The function returns whether or not the RUIM/CSIM card have support for the
  LBS service the service table

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_lbs_support_avail (
  nvruim_nv_context_type    context
)
{
  if (!NVRUIM_CONTEXT_NOT_VALID(context) && nvruim_session_data_ptr[context] != NULL)
  {
    return nvruim_session_data_ptr[context]->nvruim_lbs_support;
  }
  return FALSE;
} /* nvruim_is_lbs_support_avail */


/*===========================================================================

FUNCTION NVRUIM_MEMSCPY

DESCRIPTION
  This Function provides the wrapper to memscpy, which is a secure version of
  memory copy

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static inline void nvruim_memscpy (
  void *dst_ptr,
  size_t dst_len,
  const void *src_ptr,
  size_t src_len
)
{
  memscpy(dst_ptr,
          dst_len,
          src_ptr,
          src_len);
} /* nvruim_memscpy */


/*===========================================================================

FUNCTION nvruim_is_mmgsdi_item_enabled

DESCRIPTION
  Checks if the NV/EFS item is enabled for the appropriate index

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_mmgsdi_item_enabled
(
  mmgsdi_cached_nv_item_enum_type  item,
  nvruim_nv_context_type           context
)
{
  mmgsdi_cached_nv_item_data mmgsdi_item;
  uint8                      slot_index     = 0;

  if (NVRUIM_CONTEXT_NOT_VALID(context) ||
      nvruim_generic_data_ptr == NULL   ||
      nvruim_session_data_ptr[context] == NULL)
  {
    return FALSE;
  }

  if (nvruim_session_data_ptr[context]->mmgsdi_slot_id < MMGSDI_MAX_SLOT_ID_ENUM)
  {
    (void)nvruim_convert_slot_id_to_index(nvruim_session_data_ptr[context]->mmgsdi_slot_id,
                                          &slot_index);
  }

  memset(&mmgsdi_item, 0x00, sizeof(mmgsdi_item));
  mmgsdi_item.nv_item_enum = item;

  if (slot_index >= NVRUIM_MAX_CARD_COUNT)
  {
    return FALSE;
  }

  if (mmgsdi_item.nv_item_enum == MMGSDI_NV_PREF_MODE_NOT_FROM_RUIM_I)
  {
    if (mmgsdi_get_cached_nv_item(&mmgsdi_item)!= MMGSDI_SUCCESS)
    {
      return FALSE;
    }
  }
  else if (mmgsdi_get_cached_nv_item_ext(&mmgsdi_item, slot_index) != MMGSDI_SUCCESS)
  {
    return FALSE;
  }

  switch (item)
  {
    case  MMGSDI_NV_PREF_MODE_NOT_FROM_RUIM_I:
      return mmgsdi_item.nv_item_data.pref_mode_not_from_ruim;
    case  MMGSDI_NV_RUIM_ALLOW_ESN_FAIL_I:
      return mmgsdi_item.nv_item_data.ruim_allow_esn_fail;
    case  MMGSDI_NV_RUIM_ZERO_ESN_FAIL_I:
      return mmgsdi_item.nv_item_data.ruim_zero_esn_fail;
    case  MMGSDI_NV_UIM_RUIM_SUPPORT_SCI_I:
      return mmgsdi_item.nv_item_data.uim_ruim_support_sci;
    case  MMGSDI_NV_UIM_AN_HRPD_FALLBACK_I:
      return mmgsdi_item.nv_item_data.uim_an_hrpd_fallback;
    case  MMGSDI_NV_UIM_MISCONFIG_RUIM_N5_WORKAROUND_I:
      return mmgsdi_item.nv_item_data.uim_misconfig_ruim_n5_workaround;
    default:
      return FALSE;
  }
} /* nvruim_is_mmgsdi_item_enabled */


/*===========================================================================
FUNCTION NVRUIM_MMGSDI_INIT

DESCRIPTION
  Function called from the MMGSDI task when NVRUIM task is not running, 
  to allow registration of NVRUIM module with NV.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called once during task initialization.
=========================================================================== */
void nvruim_mmgsdi_init (
  void
)
{
  /* This function is used only when NVRUIM code is not compiled.
     So nothing to do in this case. */
} /* nvruim_mmgsdi_init */


/*===========================================================================

FUNCTION NVRUIM_PROCESS_SWITCH_1X_SESSION

DESCRIPTION
  This function process the NVRUIM_SWITCH_SESSION req to switch to the
  input session type

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void nvruim_process_switch_1x_session
(
  nvruim_request_enum_type     nvruim_req,
  nvruim_session_enum_type     nvruim_session
)
{
  NVRUIM_RETURN_IF_NULL(nvruim_generic_data_ptr);

  if (nvruim_req != NVRUIM_SWITCH_SESSION ||
      (NVRUIM_SESSION_NOT_VALID(nvruim_session)) ||
      nvruim_session == nvruim_generic_data_ptr->nvruim_active_session)
  {
    /* Nothing to do as req is not switch or nvruim_active_session is same
       as per req */
    return;
  }

  nvruim_generic_data_ptr->nvruim_active_session = nvruim_session;
} /* nvruim_process_switch_1x_session */


/*===========================================================================

FUNCTION NVRUIM_SWITCH_1X_SESSION

DESCRIPTION
  This function is called by client to switch to the incoming 1X session for
  any communication with MMGSDI.

DEPENDENCIES
  None.

RETURN VALUE
  nvruim_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
nvruim_return_enum_type nvruim_switch_1x_session
(
  nvruim_session_enum_type nvruim_session_type
)
{
  nv_stat_enum_type          nv_op_status   = NV_STAT_ENUM_MAX;
  nvruim_req_type           *nvruim_req_ptr = NULL;

  if (nvruim_session_type != NVRUIM_1X_PROV_PRI_SESSION &&
      nvruim_session_type != NVRUIM_1X_PROV_SEC_SESSION &&
      nvruim_session_type != NVRUIM_1X_PROV_TER_SESSION)
  {
    return NVRUIM_ERROR;
  }

  nvruim_req_ptr = nvruim_mem_malloc(sizeof(nvruim_req_type));

  if (nvruim_req_ptr == NULL)
  {
    return NVRUIM_ERROR;
  }

  nvruim_req_ptr->nvruim_req         = NVRUIM_SWITCH_SESSION;
  nvruim_req_ptr->cmd.nvruim_session = nvruim_session_type;

  nv_op_status = nvruim_cmd(nvruim_req_ptr);

  UIM_MSG_HIGH_3("Switch_session_req received, current_session: 0x%x, req_session: 0x%x, nv_opstatus: 0x%x",
                 nvruim_generic_data_ptr->nvruim_active_session, nvruim_session_type, nv_op_status);

  if (nv_op_status != NV_DONE_S)
  {
    nvruim_mem_free_null_ok(nvruim_req_ptr);
    return NVRUIM_ERROR;
  }
  return NVRUIM_SUCCESS;
} /* nvruim_switch_1x_session */


/*===========================================================================

FUNCTION nvruim_init_wms_svc_items

DESCRIPTION
  This function is called from the WMS task during initialization for SMS.
  It passes in boolean values to indicate whether or not the RUIM provides
  for BCSMS and SMS Capabilities provisioning.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Sets the bcsms_svc and smscap_svc
  global variables to indicate service support.

===========================================================================*/
void nvruim_init_wms_svc_items
(
  boolean                       service_bcsms,
  boolean                       service_smscap,
  nvruim_session_enum_type      nvruim_session
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) ||
      NVRUIM_CONTEXT_NOT_VALID(context)        ||
      nvruim_session_data_ptr[context] == NULL)
  {
    return;
  }

  nvruim_session_data_ptr[context]->bcsms_svc   = service_bcsms;
  nvruim_session_data_ptr[context]->smscap_svc  = service_smscap;
} /* nvruim_init_wms_svc_items */


/*===========================================================================

FUNCTION NVRUIM_GET_MIP_SIP_SVC_STATUS

DESCRIPTION
  This function will return the status of SIP and MIP service

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS

===========================================================================*/
void nvruim_get_mip_sip_svc_status(
  nvruim_session_enum_type  nvruim_session,
  boolean                  *mip_svc_status,
  boolean                  *sip_svc_status
)
{
  nvruim_nv_context_type   context  = nvruim_map_nvruim_session_to_context(nvruim_session);

  if (NVRUIM_SESSION_NOT_VALID(nvruim_session) ||
      NVRUIM_CONTEXT_NOT_VALID(context)        ||
      nvruim_session_data_ptr[context] == NULL)
  {
    return;
  }

  NVRUIM_RETURN_IF_NULL_2(mip_svc_status, sip_svc_status);

  *mip_svc_status = nvruim_session_data_ptr[context]->mip_svc_3gpd;
  *sip_svc_status = nvruim_session_data_ptr[context]->sip_svc_3gpd;
} /* nvruim_get_mip_sip_svc_status */
