/*================================================================================

DESCRIPTION


Copyright (c) 2009-2017, 2019 - 2020, 2024 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_parsing.c#6 $$ $DateTime: 2024/02/26 16:03:48 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/04/24   cj      Hide CSIM from indication when CDMA_LESS NV is enabled
02/13/20   vgd     Get ATR, ICCID, EID from MMGSDI when card_info is populated
02/11/20   vdc     Return QMI_ERR_MISSING_ARG if pin or puk length is zero
12/28/19   sg      Added support for NS+SP category
09/19/19   sg      Added support for USIM GSM EF's path 
09/17/19   vdc     SimLock solution for activation at Point Of Sale
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
06/07/19   sg      Move perso to in progress from blocked based on slot policy
04/25/19   mm      Added support for extended simlock data
04/05/19   vm      Send perso state IN PROGRESS if app on other slot is detected
07/13/17   av      Enhanced simlock RSU solution
05/10/17   tq      Featurize 3GPP2 code under FEATURE_MMGSDI_3GPP2
04/21/17   vdc     Added support for refresh enforcement policy
04/19/17   tq      Added Support for MCPTT,ACDC,ePDG,Launch Pad EFs in QMI
04/18/17   me      Added support to euicc bit in atr
04/14/17   kv      Added pass-through mode support
04/12/17   ar      Add support for retrieve/configure default server address
04/06/17   gm      Added support for extended APDU
02/21/17   bcho    Set perso state to IN PROGRESS if any app gets stuck at PIN1
01/19/17   me      Populating path internally and parsing IMSI, FPLMN, EHPLMN
12/07/16   av      LPA phase2 initial changes
12/05/16   vdc     Added support for simlock single lock for full srv policy
12/02/16   me      Combined multi-line ASSERT in one line
11/03/16   bcho    Added support for ICCID simlock
10/24/16   vdc     Added support for auto reg SMS when ESN is changed
10/06/16   ar      Added support for SP + EHPLMN simlock
09/22/16   gm      Added support for set APDU behavior
08/19/16   tkl     Added support for simlock single lock
08/18/16   ar      Add support for simlock storage and emergency only mode
08/08/16   av      Ensure better error handling for terminal cap API
08/03/16   vdc     Added support for SPN based SIMLOCK feature
07/15/16   sp      Seperate SAP and ERA-GLONASS related functionality
06/30/16   av      Add max supported slots check while parsing slot TLV
06/16/16   vdc     Split QMI UIM parsing files for supporting IOE
06/16/16   av      Support for setting terminal capability from AP
05/26/16   sp      F3 logging improvements
05/20/16   ar      Reducing F3s for the parsing error
05/10/16   ar      Return QMI_ERR_ARG_TOO_LONG for invalid arguments
04/28/16   sp      Fix compile warnings
04/27/16   sp      Fix low level compiler warnings
04/26/16   vdc     Do not send temporary unlock TLV if no feature is active
04/20/16   sam     Feature TSDS support
04/14/16   vdc     Split QMI UIM files for supporting IOE
04/11/16   ar      Support for eUICC
03/15/16   vr      SAM CARD support
03/09/16   sp      Remove Excessive F3's
02/19/16   vdc     Send control key presence in get config presence
02/01/16   av      Check service availability before allowing EF GSM MBI read
01/20/16   gm      Added support to purge temporary identity
01/12/16   vdc     Populate verify retries correctly
11/16/15   yt      Support for second BSF and key status events
11/09/15   vv      Remove unneeded F3's
10/23/15   bcho    Support for multi byte GID
10/12/15   vdc     Added support for USAT application pairing
10/07/15   sp      Reduce signed/unsigned conversions in UIM
10/05/15   vv      Support for reuse code data
10/05/15   ar      Add EPSLOCI and EPSNSC file in USIM access conversion table
08/28/15   hh      Removed JCDMA related code
08/19/15   vv      Added support to retrieve retry attempts left for locking
08/17/15   av      Featurize simlock/perso code under FEATURE_SIMLOCK
08/12/15   stv     To send temporary unlock status indication
07/15/15   vdc     Decrease the check for minimum perso code len to 15
07/07/15   bcho    Redundant checks for SIMLOCK
06/29/15   lm      Enable memory leak detection
05/15/15   vv      Support for RSU
04/30/15   vdc     Added support for APN for bootstrapping
04/20/15   ar      Support for terminate_app TLV in qmi_uim_close_session
03/25/15   ar      Support for cross-mapping in multisim_auto_provisioning NV
03/19/15   at      Introducing new relock feature in SIMLock
03/10/15   vdc     Replace hard coded values with IDL defined values
03/04/15   vdc     Added support for QMI UIM GBA
03/03/15   lm      Remove double semicolon at end of the line
02/25/15   ar      Addition of QMI_UIM_SEARCH_RECORD interface
02/21/15   vdc     Add USIM MBI EF to USIM path/enum conversion table
02/20/15   vv      Remove perso related code
02/09/15   vv      Check the result of simlock config read operation
02/03/15   xj      Add USB UICC and SAP CLIENT supported interface
02/03/15   at      Replacement of Secapis to use a derived key & HMAC
01/14/15   bcho    Remove use of SIMLOCK_MIN_GID_LEN
01/07/15   vdc     Closed pending comments for handling multi profile cards
12/30/14   bcho    Support for more then 1 bytes of GID1/GID2
12/26/14   vv      Fix enabling a lock with more than 20 codes of lock data
12/16/14   av      Apply auth restr via NV for IIC only if USIM is selected
12/10/14   vdc     Added support for handling multiple profiles enabled cards
11/14/14   hh      Fix compilation warnings
10/14/14   tl      Add featurization to strip encryption and decryption
10/14/14   vv      Fix decrypting the simlock config operation
10/13/14   tl      Change error returned when CK length is zero
10/10/14   vv      Fix a merge error
10/10/14   vv      Add support for remote simlock configuration
10/08/14   vdc     Added support for UICC activation status indication
09/30/14   vv      Add support for emergency only command
09/25/14   tl      Add support for secure read
09/22/14   tl      Assign the decrypted PIN limit before decryption
09/17/14   tl      Add support for perso and deperso secure
08/29/14   tl      Add support for supply voltage command and indication
08/11/14   vv      Fixed compiler warnings
07/25/14   vv      Added support for perso command
07/22/14   at      Fix error in ISIM Auth response for different contexts
06/30/14   vv      Remove perso code
06/12/14   av      Enhance GET_SERVICE_STATUS to accomodate all service numbers
06/12/14   av      Cache mandatory EFs used by NVRUIM for critical 1x NV reads
05/22/14   tl      Added support for recovery complete indications
05/13/14   yt      Add CDMA PRL and EPRL files to RUIM enum-to-path table
05/13/14   vdc     Support for reduced card status indications
05/12/14   df      Add QMI support for status cmd
04/03/14   yt      Calculate card status TLV length based on number of apps
03/26/14   tl      SIM Lock secondary revisions
03/18/14   tl      Introduce new SIM Lock feature
02/25/14   yt      Support for QMI_UIM_RESELCT_REQ
02/25/14   bcho    Added support to store OPLMN List in EFS and cache it
02/24/14   at      Support for read transparent long indication
01/10/14   df      Use v2.0 diag macros
12/16/13   yt      Support for ISIM AUTH in HTTP digest security context
12/09/13   spo     Fix for handling auth request for USIM and ISIM
11/25/13   ar      Added Conversion from Path to Enum in CSIM table
11/20/13   tkl     Added API for Increase
10/09/13   spo     Fixed conversion of file access method from enum to path
10/09/13   spo     Fixed conversion from path to enum for third slot
10/04/13   vv      Added API to return se13 table release date and source
09/23/13   df      Support for SIM busy
09/16/13   yt      Remove redundant code
09/11/13   av      Handle new hotswap status CARD_UNKNOWN
09/02/13   df      Correct comments
08/08/13   ks      Support for subscription manager
07/26/13   av      Fix qmi_uim_tlv_aid to validate against the proper max AID
07/10/13   vdc     Added support for missing triple sim instances
07/02/13   yt      Dynamically allocate slot and app data
07/02/13   kb      Fix compilation error
06/28/13   kb      Added QMI malloc and free wrapper functions
06/27/13   spo     Added support for files under DF TELECOM
06/25/13   av      Add CSIM/CDMA SF_EUIMID to path conversion tables for caching
06/25/13   av      Support path to enum conversion for 7F66_PROP1_ACT_HPLMN EFs
06/21/13   kk      Updated support for triple sim
06/19/13   vv      Do not udpate PIN2 state based on app state
05/21/13   vv      Extend session closed indication for all sessions
04/30/13   spo     Replaced instances of memcpy with memscpy
02/22/13   yt      Support for slot 3
02/12/13   at      Added support for file status TLV
02/07/13   yt      Support for activation/deactivation of EFs
12/07/12   tl      Add SST/UST/CST EFs to path/enum conversion tables
11/09/12   yt      Move QMI UIM to UIM heap
11/02/12   yt      Add support for ACL in SET and GET SERVICE STATUS commands
09/10/12   at      Fixed compiler warnings
09/07/12   at      Removal of private DS functions and headers
09/07/12   at      Fixed parsing of SAP Connection Condition TLV
09/07/12   tl      Added support for SGLTE dual slots
08/29/12   at      Optional TlV in SAP connection req to check for active calls
08/28/12   spo     Added support for Session Closed Indication
08/16/12   at      New command handling for QMI_UIM_REFRESH_REGISTER_ALL
08/16/12   tl      Add session close without app termination functionality
05/23/12   hn      Enable 1-byte alignment packing on Windows environment
05/18/12   tl      Fixed Run Cave Algo response package
05/14/12   at      Add new TLV to QMI_UIM_GET_CARD_STATUS command
04/27/12   av      Removed FEATURE_UIM_SUPPORT_HOTSWAP feature flag
04/11/12   at      Indication support for sending long APDUs
03/02/12   vv      Added optional TLV in send APDU request
03/02/12   vv      Added support for querying ESN status
02/27/12   yt      Add function to construct file_length TLV
02/23/12   nmb     QMI QMUX Transition
01/03/12   nb      Added optional TLV for QMI configuration
12/21/11   nmb     Secure channel perso for apps
12/21/11   shr     Fixed compilation errors - GSDI Removal
12/20/11   hn      Add support for off-target testing
10/12/11   at      Handling intermediate get response in SAP commands
10/05/11   yt      Added support for refresh of non provisioning sessions
                   Removed flag to indicate if TLV is mandatory or optional
09/19/11   yt      Fixed compilation error
09/16/11   yt      Added optional TLV for status of hot swap switch
08/16/11   at      Indication support for various commands
08/01/11   at      Support for keyref_id TLV in PIN commands
08/01/11   at      Support for using session id on an open channel id
08/01/11   vs      Updating generic response copy to avoid write overflow
06/27/11   at      Renamed qmi_uim_response_sap_generic()
05/13/11   nmb     Added support for ISIM GBA Authentication
04/28/11   mib     Fixed crash due to invalid free reading multiple records
04/26/11   vs      Support for subscription ok request
04/25/11   mib     Removed mapping of some TELECOM files into MMGSDI enum
04/07/11   mib     Added support to read multiple records at the same time
04/01/11   mib     Changed TLV for the encrypted PIN passed for verify
03/29/11   mib     Fixed possible memory leak in refresh registration
03/15/11   mib     Added support for ISIM app type
03/09/11   at      Added support for PIN1 encryption/decryption
03/03/11   mib     Added support to manage logical channels to send APDUs
03/01/11   at      Added support for SAP request & indication
01/12/11   mib     Added support for CSIM file paths
01/04/11   mib     Added support for Compute IP Authentication
12/21/10   at      Added support for SAP connection request
12/17/10   mib     Split APDU request into two TLVs
12/09/10   at      Added support for send APDU request
11/08/10   mib     Fixed Klokwork warning in refresh indication
11/02/10   mib     Added additional check on path length
10/22/10   mib     Added support for get configuration request
10/12/10   mib     Support for refresh by path
09/20/10   mib     Fixed authenticate response
09/09/10   mib     Changed PIN status in TLV when PIN is not initialized
08/25/10   yt      Updated file_type for MF and DF in get file attr response
08/05/10   at      Updated GSM & USIM EF mapping tables with new EFs
08/05/10   tkl     QMI UIM & QMI CAT split from data package
08/04/10   at      Fixed current pointer updation for register refresh
07/26/10   mib     Return QMI_ERR_INVALID_ARG for wrong path length
07/08/10   mib     Added check on slot 2 for single slot devices
07/08/10   mib     Added support for QMI_ERR_INVALID_ARG
07/07/10   yt      Updated TLV parsing for session change
07/07/10   mib     Better error handling for QMI_ERR_ARG_TOO_LONG
06/18/10   tkl     Added enums support for refresh and handle telecom files
                   conversion
06/18/10   mib     Fixed Klokwork warnings: uninitialized security mask
                   Fixed AID value in case of multiple applications
06/16/10   mib     Added support for hidden key
06/10/10   mib     Fixed number of retries for PIN operations
06/07/10   mib     Added support for GET LABEL and SESSION CHANGE requests
06/03/10   mib     Fixed compilation warnings
05/15/10   mib     Improved mechanism to pass TLV id for responses
05/13/10   mib     Fixed SW1/SW2 in the response
04/15/10   mib     Fixed session type for 1X provisioning.
04/06/10   mib     Added support for authenticate and close session.
                   Changed TLV ids for consistency with other QMI services
04/05/10   at      Fixed handling for TLV length in refresh indications
01/13/10   mib     Initial version
=============================================================================*/


#include "uim_variation.h"
#include "customer.h"
#include "uim_msg.h"

#include "comdef.h"
#include "dsm.h"
#include "amssassert.h"
#include "modem_mem.h"
#include <stringl/stringl.h>

#include "qmi_svc_defs.h"
#include "qmi_svc_utils.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_util.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim_encryption.h"
#include "qmi_uim_simlock_parsing.h"
#include "qmi_uim_gba_parsing.h"
#include "qmi_uim_sub_mgr_parsing.h"
#include "qmi_uim_sub_mgr.h"
#include "uimi.h"
#include "user_identity_module_v01.h"
#include "qmi_uim_multi_profile_parsing.h"
#include "qmi_uim_sap_parsing.h"
#include "qmi_uim_internal.h"
#include "mmgsdisessionlib_v.h"
#include "mmgsdilib_p.h"
#include "mmgsdi_nv.h"

/* These includes need to always be at bottom - offtarget use only */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*---------------------------------------------------------------------------
  Minimum length of Request TLVs with variable size
---------------------------------------------------------------------------*/
#define QMI_UIM_TLV_MIN_LEN_SESSION_INFORMATION       2
#define QMI_UIM_TLV_MIN_LEN_FILE_ID                   3
#define QMI_UIM_TLV_MIN_LEN_SET_PIN_PROTECTION        3
#define QMI_UIM_TLV_MIN_LEN_VERIFY_PIN                2
#define QMI_UIM_TLV_MIN_LEN_UNBLOCK_PIN               3
#define QMI_UIM_TLV_MIN_LEN_CHANGE_PIN                3
#define QMI_UIM_TLV_MIN_LEN_APPLICATION_INFORMATION   2
#define QMI_UIM_TLV_MIN_LEN_AID                       1
#define QMI_UIM_TLV_MIN_LEN_ENCRYPTED_PIN             1
#define QMI_UIM_TLV_MIN_LEN_SWITCH_SLOT_EXT           5

/* PLMN length when coded as defined in TS 24.008 */
#define QMI_UIM_PLMN_ID_LEN                           3

/* PLMN MAX bytes (3 for MCC, 1 MNC length, 3 for MNC) */
#define QMI_UIM_MAX_PLMN_BYTES                        7

/*---------------------------------------------------------------------------
  Structure with map between file path and file enum
---------------------------------------------------------------------------*/
typedef struct
{
  uint16                 file_id;
  mmgsdi_file_enum_type  mmgsdi_enum;
} qmi_uim_ef_map_type;

/*---------------------------------------------------------------------------
  Structure used for TLV composition of card status
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8  card_state;
  uint8  upin_state;
  uint8  upin_retries;
  uint8  upuk_retries;
  uint8  error_code;
  uint8  num_app;
} tlv_data_card_state_type;

typedef PACKED struct PACKED_POST
{
  uint8  app_type;
  uint8  app_state;
  uint8  perso_state;
  uint8  perso_feature;
  uint8  perso_retries;
  uint8  perso_unblock_retries;
  uint8  aid_len;
} tlv_data_app_state_top_type;

typedef PACKED struct PACKED_POST
{
  uint8  univ_pin;
  uint8  pin1_state;
  uint8  pin1_retries;
  uint8  puk1_retries;
  uint8  pin2_state;
  uint8  pin2_retries;
  uint8  puk2_retries;
} tlv_data_app_state_bottom_type;

/*---------------------------------------------------------------------------
  Tables with map between file path and file enum
---------------------------------------------------------------------------*/
#define QMI_UIM_GSM_TABLE(table, path)          \
         const uint16 path[] =                  \
         { 0x3F00, 0x7F20 };                    \
         const qmi_uim_ef_map_type table[] =    \
         {                                      \
           { 0x6F05 , MMGSDI_GSM_LP          }, \
           { 0x6F07 , MMGSDI_GSM_IMSI        }, \
           { 0x6F13 , MMGSDI_GSM_CFF         }, \
           { 0x6F14 , MMGSDI_GSM_ONS         }, \
           { 0x6F15 , MMGSDI_GSM_CSP         }, \
           { 0x6F20 , MMGSDI_GSM_KC          }, \
           { 0x6F30 , MMGSDI_GSM_PLMN        }, \
           { 0x6F31 , MMGSDI_GSM_HPLMN       }, \
           { 0x6F37 , MMGSDI_GSM_ACM_MAX     }, \
           { 0x6F38 , MMGSDI_GSM_SST         }, \
           { 0x6F39 , MMGSDI_GSM_ACM         }, \
           { 0x6F3E , MMGSDI_GSM_GID1        }, \
           { 0x6F3F , MMGSDI_GSM_GID2        }, \
           { 0x6F46 , MMGSDI_GSM_SPN         }, \
           { 0x6F41 , MMGSDI_GSM_PUCT        }, \
           { 0x6F45 , MMGSDI_GSM_CBMI        }, \
           { 0x6F52 , MMGSDI_GSM_KCGPRS      }, \
           { 0x6F53 , MMGSDI_GSM_LOCIGPRS    }, \
           { 0x6F74 , MMGSDI_GSM_BCCH        }, \
           { 0x6F78 , MMGSDI_GSM_ACC         }, \
           { 0x6F7B , MMGSDI_GSM_FPLMN       }, \
           { 0x6F7E , MMGSDI_GSM_LOCI        }, \
           { 0x6FAD , MMGSDI_GSM_AD          }, \
           { 0x6FAE , MMGSDI_GSM_PHASE       }, \
           { 0x6FB1 , MMGSDI_GSM_VGCS        }, \
           { 0x6FC5 , MMGSDI_GSM_PNN         }, \
           { 0x6FC6 , MMGSDI_GSM_OPL         }, \
           { 0x6FC7 , MMGSDI_GSM_MBDN        }, \
           { 0x6FC8 , MMGSDI_GSM_EXT6        }, \
           { 0x6FC9 , MMGSDI_GSM_MBI         }, \
           { 0x6FB2 , MMGSDI_GSM_VGCSS       }, \
           { 0x6FB3 , MMGSDI_GSM_VBS         }, \
           { 0x6FB4 , MMGSDI_GSM_VBSS        }, \
           { 0x6FB5 , MMGSDI_GSM_EMLPP       }, \
           { 0x6FB6 , MMGSDI_GSM_AAEM        }, \
           { 0x6F50 , MMGSDI_GSM_CBMIR       }, \
           { 0x6F32 , MMGSDI_GSM_CNL         }, \
           { 0x6F51 , MMGSDI_GSM_NIA         }, \
           { 0x6F63 , MMGSDI_GSM_CPBCCH      }, \
           { 0x6FCA , MMGSDI_GSM_MWIS        }, \
           { 0x6FCD , MMGSDI_GSM_SPDI        }, \
           { 0x6FCB , MMGSDI_GSM_CFIS        }, \
           { 0x6F61 , MMGSDI_GSM_OPLMNWACT   }  \
         };

#define QMI_UIM_RUIM_TABLE(table, path)                     \
         const uint16 path[] =                              \
         { 0x3F00, 0x7F25 };                                \
         const qmi_uim_ef_map_type table[] =                \
         {                                                  \
           { 0x6F22 , MMGSDI_CDMA_IMSI_M                 }, \
           { 0x6F23 , MMGSDI_CDMA_IMSI_T                 }, \
           { 0x6F41 , MMGSDI_CDMA_HOME_SVC_PVDR_NAME     }, \
           { 0x6F74 , MMGSDI_CDMA_SF_EUIM_ID             }, \
           { 0x6F30 , MMGSDI_CDMA_PRL                    }, \
           { 0x6F5A , MMGSDI_CDMA_EPRL                   }  \
         };

#define QMI_UIM_USIM_TABLE(table, path)         \
         const uint16 path[] =                  \
         { 0x3F00, 0x7FFF };                    \
         const qmi_uim_ef_map_type table[] =    \
         {                                      \
           { 0x6F05 , MMGSDI_USIM_LI         }, \
           { 0x6F07 , MMGSDI_USIM_IMSI       }, \
           { 0x6F13 , MMGSDI_USIM_CFF        }, \
           { 0x6F14 , MMGSDI_USIM_ONS        }, \
           { 0x6F15 , MMGSDI_USIM_CSP        }, \
           { 0x6F20 , MMGSDI_USIM_KEYS       }, \
           { 0x6F08 , MMGSDI_USIM_KEYS       }, \
           { 0x6F09 , MMGSDI_USIM_KEYSPS     }, \
           { 0x6F60 , MMGSDI_USIM_PLMNWACT   }, \
           { 0x6F30 , MMGSDI_USIM_PLMNWACT   }, \
           { 0x6F31 , MMGSDI_USIM_HPLMN      }, \
           { 0x6F37 , MMGSDI_USIM_ACM_MAX    }, \
           { 0x6F38 , MMGSDI_USIM_UST        }, \
           { 0x6F39 , MMGSDI_USIM_ACM        }, \
           { 0x6F3E , MMGSDI_USIM_GID1       }, \
           { 0x6F3F , MMGSDI_USIM_GID2       }, \
           { 0x6F46 , MMGSDI_USIM_SPN        }, \
           { 0x6F41 , MMGSDI_USIM_PUCT       }, \
           { 0x6F45 , MMGSDI_USIM_CBMI       }, \
           { 0x6F78 , MMGSDI_USIM_ACC        }, \
           { 0x6F7B , MMGSDI_USIM_FPLMN      }, \
           { 0x6F7E , MMGSDI_USIM_LOCI       }, \
           { 0x6FAD , MMGSDI_USIM_AD         }, \
           { 0x6F48 , MMGSDI_USIM_CBMID      }, \
           { 0x6FB7 , MMGSDI_USIM_ECC        }, \
           { 0x6F50 , MMGSDI_USIM_CBMIR      }, \
           { 0x6F73 , MMGSDI_USIM_PSLOCI     }, \
           { 0x6F3B , MMGSDI_USIM_FDN        }, \
           { 0x6F3C , MMGSDI_USIM_SMS        }, \
           { 0x6F40 , MMGSDI_USIM_MSISDN     }, \
           { 0x6F42 , MMGSDI_USIM_SMSP       }, \
           { 0x6FC5 , MMGSDI_USIM_PNN        }, \
           { 0x6FC6 , MMGSDI_USIM_OPL        }, \
           { 0x6FC7 , MMGSDI_USIM_MBDN       }, \
           { 0x6FC8 , MMGSDI_USIM_EXT6       }, \
           { 0x6FCB , MMGSDI_USIM_CFIS       }, \
           { 0x6FCA , MMGSDI_USIM_MWIS       }, \
           { 0x6F4E , MMGSDI_USIM_EXT5       }, \
           { 0x6F49 , MMGSDI_USIM_SDN        }, \
           { 0x6F4B , MMGSDI_USIM_EXT2       }, \
           { 0x6F4C , MMGSDI_USIM_EXT3       }, \
           { 0x6F4D , MMGSDI_USIM_BDN        }, \
           { 0x6F55 , MMGSDI_USIM_EXT4       }, \
           { 0x6F81 , MMGSDI_USIM_OCI        }, \
           { 0x6F80 , MMGSDI_USIM_ICI        }, \
           { 0x6F47 , MMGSDI_USIM_SMSR       }, \
           { 0x6FB5 , MMGSDI_USIM_EMLPP      }, \
           { 0x6FB6 , MMGSDI_USIM_AAEM       }, \
           { 0x6F56 , MMGSDI_USIM_EST        }, \
           { 0x6F57 , MMGSDI_USIM_ACL        }, \
           { 0x6F32 , MMGSDI_USIM_CNL        }, \
           { 0x6FCD , MMGSDI_USIM_SPDI       }, \
           { 0x6FCF , MMGSDI_USIM_EXT8       }, \
           { 0x6FD6 , MMGSDI_USIM_GBABP      }, \
           { 0x6FDB , MMGSDI_USIM_EHPLMNPI   }, \
           { 0x6FDC , MMGSDI_USIM_LRPLMNSI   }, \
           { 0x6FDE , MMGSDI_USIM_SPNI       }, \
           { 0x6FDF , MMGSDI_USIM_PNNI       }, \
           { 0x6FE2 , MMGSDI_USIM_NCP_IP     }, \
           { 0x6F62 , MMGSDI_USIM_HPLMNWACT  }, \
           { 0x6F4F , MMGSDI_USIM_CCP2       }, \
           { 0x6FC3 , MMGSDI_USIM_HIDDENKEY  }, \
           { 0x6F61 , MMGSDI_USIM_OPLMNWACT  }, \
           { 0x6FC9 , MMGSDI_USIM_MBI        }, \
           { 0x6FE3 , MMGSDI_USIM_EPSLOCI    }, \
           { 0x6FE4 , MMGSDI_USIM_EPSNSC     }, \
           { 0X6FF1 , MMGSDI_USIM_IPS        }, \
           { 0x6FD9 , MMGSDI_USIM_EHPLMN            }, \
           { 0x6FF3 , MMGSDI_USIM_EPDG_ID           }, \
           { 0x6FF4 , MMGSDI_USIM_EPDG_SELECTION    }, \
           { 0x6FF5 , MMGSDI_USIM_EPDG_IDEM         }, \
           { 0x6FF6 , MMGSDI_USIM_EPDG_SELECTION_EM }  \
         };

#define QMI_UIM_CSIM_TABLE(table, path)          \
         const uint16 path[] =                   \
         { 0x3F00, 0x7FFF };                     \
         const qmi_uim_ef_map_type table[] =     \
         {                                       \
           { 0x6F22 , MMGSDI_CSIM_IMSI_M      }, \
           { 0x6F23 , MMGSDI_CSIM_IMSI_T      }, \
           { 0x6F31 , MMGSDI_CSIM_RUIM_ID     }, \
           { 0x6F32 , MMGSDI_CSIM_CST         }, \
           { 0x6F75 , MMGSDI_CSIM_EST         }, \
           { 0x6F3B , MMGSDI_CSIM_FDN         }, \
           { 0x6F44 , MMGSDI_CSIM_MSISDN      }, \
           { 0x6F30 , MMGSDI_CSIM_PRL         }, \
           { 0x6F5A , MMGSDI_CSIM_EPRL        }, \
           { 0x6F45 , MMGSDI_CSIM_MAXIMUM_PRL }, \
           { 0x6F7A , MMGSDI_CSIM_EXT2        }, \
           { 0x6F7B , MMGSDI_CSIM_EXT3        }, \
           { 0x6F69 , MMGSDI_CSIM_MMS_UCP     }, \
           { 0x6F6D , MMGSDI_CSIM_GID1        }, \
           { 0x6F6E , MMGSDI_CSIM_GID2        }, \
           { 0x6F6F , MMGSDI_CSIM_CNL         }, \
           { 0x6F7D , MMGSDI_CSIM_OCI         }, \
           { 0x6F7C , MMGSDI_CSIM_ICI         }, \
           { 0x6F7E , MMGSDI_CSIM_EXT5        }, \
           { 0x6F8D , MMGSDI_CSIM_JDL         }, \
           { 0x6F41 , MMGSDI_CSIM_SPN         }, \
           { 0x6F74 , MMGSDI_CSIM_SF_EUIM_ID  }, \
           { 0x6F47 , MMGSDI_CSIM_ECC         }, \
           { 0x6F3A , MMGSDI_CSIM_LANG_IND    }, \
           { 0x6F76 , MMGSDI_CSIM_HIDDEN_KEY  }  \
         };

#define QMI_UIM_MF_TABLE(table, path)          \
         const uint16 path[]  =                \
         { 0x3F00 };                           \
         const qmi_uim_ef_map_type table[] =   \
         {                                     \
           { 0x2FE2, MMGSDI_ICCID     },       \
           { 0x2F05, MMGSDI_ELP       },       \
           { 0x2F00, MMGSDI_DIR       },       \
           { 0x2F06, MMGSDI_ARR       }        \
         };

#define QMI_UIM_TELECOM_TABLE(table, path)      \
         const uint16 path[] =                  \
         { 0x3F00, 0x7F10 };                    \
         const qmi_uim_ef_map_type table[] =    \
         {                                      \
           { 0x6F3B , MMGSDI_TELECOM_FDN     }, \
           { 0x6F3C , MMGSDI_TELECOM_SMS     }, \
           { 0x6F4F , MMGSDI_TELECOM_ECCP    }, \
           { 0x6F40 , MMGSDI_TELECOM_MSISDN  }, \
           { 0x6F42 , MMGSDI_TELECOM_SMSP    }, \
           { 0x6F43 , MMGSDI_TELECOM_SMSS    }, \
           { 0x6F44 , MMGSDI_TELECOM_LND     }, \
           { 0x6F49 , MMGSDI_TELECOM_SDN     }, \
           { 0x6F4C , MMGSDI_TELECOM_EXT3    }, \
           { 0x6F4D , MMGSDI_TELECOM_BDN     }, \
           { 0x6F4E , MMGSDI_TELECOM_EXT4    }, \
           { 0x6F47 , MMGSDI_TELECOM_SMSR    }, \
           { 0x6FE0 , MMGSDI_TELECOM_ICE_DN  }, \
           { 0x6FE1 , MMGSDI_TELECOM_ICE_FF  }  \
         };

#define QMI_UIM_TELECOM_GRAPHICS_TABLE(table, path)     \
         const uint16 path[] =                          \
         { 0x3F00, 0x7F10, 0x5F50 };                    \
         const qmi_uim_ef_map_type table[] =            \
         {                                              \
           { 0x4F21, MMGSDI_TELECOM_ICE_GRAPHICS }      \
         };

#define QMI_UIM_TELECOM_MCPTT_TABLE(table, path)        \
         const uint16 path[] =                          \
         { 0x3F00, 0x7F10, 0x5F3D };                    \
         const qmi_uim_ef_map_type table[] =            \
         {                                              \
           { 0x4F01, MMGSDI_TELECOM_MCPTT_MST }         \
         };

#define QMI_UIM_PROP_7F66_2G_TABLE(table, path)          \
         const uint16 path[] =                           \
         { 0x3F00, 0x7F66, 0x5F30 };                     \
         const qmi_uim_ef_map_type table[] =             \
         {                                               \
           { 0x4F34, MMGSDI_SIM_7F66_PROP1_ACT_HPLMN  }  \
         };

#define QMI_UIM_PROP_7F66_3G_TABLE(table, path)         \
         const uint16 path[] =                          \
         { 0x3F00, 0x7FFF, 0x7F66, 0x5F30 };            \
         const qmi_uim_ef_map_type table[] =            \
         {                                              \
           { 0x4F34, MMGSDI_USIM_7F66_PROP1_ACT_HPLMN } \
         };

#define QMI_UIM_USIM_ACDC_TABLE(table, path)            \
         const uint16 path[] =                          \
         { 0x3F00, 0x7FFF, 0x5FA0 };                    \
         const qmi_uim_ef_map_type table[] =            \
         {                                              \
           { 0x4F01, MMGSDI_USIM_ACDC_LIST }            \
         };

#define QMI_UIM_CD_TABLE(table, path)           \
         const uint16 path[] =                  \
         { 0x3F00, 0x7F11 };                    \
         const qmi_uim_ef_map_type table[] =    \
         {                                      \
           { 0x6F01 , MMGSDI_CD_LAUNCH_PAD }    \
         };

#define QMI_UIM_USIM_GSM_ACCESS_TABLE(table, path)      \
         const uint16 path[] =                          \
         { 0x3F00, 0x7FFF, 0x5F3B};                     \
         const qmi_uim_ef_map_type table[] =            \
         {                                              \
           { 0x4F52, MMGSDI_USIM_KCGPRS },              \
           { 0x4F20, MMGSDI_USIM_KC  },                 \
           { 0x4F63, MMGSDI_USIM_CPBCCH },              \
           { 0x4F64, MMGSDI_USIM_INVSCAN}               \
         };

/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_TABLE()

  DESCRIPTION
    Converts file access method between path and MMGSDI enum
    using a specified input table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method
    table_ptr       : table to be used for the conversion
    table_size      : table size

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_table
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method,
  const qmi_uim_ef_map_type      * table_ptr,
  uint16                           table_size,
  const uint16                   * table_path_ptr,
  uint8                            table_path_len
)
{
  uint32 index    = 0;

  ASSERT(file_access_ptr && table_ptr && table_size > 0 &&
         table_path_ptr && table_path_len < MMGSDI_MAX_PATH_LEN);

  /* Check file access method */
  if (file_access_ptr->access_method == access_method)
  {
    /* Input format matches output format, so we can
       immediately return success */
    UIM_MSG_HIGH_0("No file access conversion required");
    return TRUE;
  }

  if (file_access_ptr->access_method == MMGSDI_BY_PATH_ACCESS &&
      access_method == MMGSDI_EF_ENUM_ACCESS)
  {
    /* Conversion from PATH to ENUM */

    /* Check length */
    if ((file_access_ptr->file.path_type.path_len != table_path_len + 1) ||
        memcmp(file_access_ptr->file.path_type.path_buf, table_path_ptr, table_path_len * sizeof(uint16)) != 0)
    {
      UIM_MSG_HIGH_1("QMI_UIM invalid path or path len: 0x%x",
                     file_access_ptr->file.path_type.path_len);
      return FALSE;
    }

    /* loop on all items in the table */
    for (index = 0; index < table_size; index++)
    {
      if (table_ptr[index].file_id == file_access_ptr->file.path_type.path_buf[table_path_len])
      {
        file_access_ptr->access_method = MMGSDI_EF_ENUM_ACCESS;
        file_access_ptr->file.file_enum = table_ptr[index].mmgsdi_enum;
        return TRUE;
      }
    }
    UIM_MSG_HIGH_0("QMI_UIM conversion from path to enum failed");
  }
  else if (file_access_ptr->access_method == MMGSDI_EF_ENUM_ACCESS &&
           access_method == MMGSDI_BY_PATH_ACCESS)
  {
    if((table_path_len + 1) * sizeof(uint16) > sizeof(file_access_ptr->file.path_type.path_buf))
    {
      return FALSE;
    }

    /* Conversion from ENUM to PATH */

    /* loop on all items in the table */
    for (index = 0; index < table_size; index++)
    {
      if (table_ptr[index].mmgsdi_enum == file_access_ptr->file.file_enum)
      {
        file_access_ptr->access_method = MMGSDI_BY_PATH_ACCESS;
        file_access_ptr->file.path_type.path_len = table_path_len + 1;
        (void)memscpy((void*)file_access_ptr->file.path_type.path_buf,
                      sizeof(file_access_ptr->file.path_type.path_buf),
                      (void*)table_path_ptr,
                      table_path_len * sizeof(uint16));
        file_access_ptr->file.path_type.path_buf[table_path_len] = table_ptr[index].file_id;
        return TRUE;
      }
    }
    UIM_MSG_HIGH_0("QMI_UIM conversion from enum to path failed");
  }
  else
  {
    /* Invalid conversion */
    UIM_MSG_HIGH_2("QMI_UIM conversion from 0x%x to 0x%x is impossible",
                   file_access_ptr->access_method, access_method);
  }

  return FALSE;
} /* qmi_uim_convert_file_access_method_table */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_TELECOM()

  DESCRIPTION
    Creates the table with TELECOM files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_telecom
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_TELECOM_TABLE(telecom_table, telecom_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                telecom_table,
                sizeof(telecom_table) / sizeof(qmi_uim_ef_map_type),
                telecom_path,
                sizeof(telecom_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_telecom */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_GSM()

  DESCRIPTION
    Creates the table with GSM files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_gsm
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_GSM_TABLE(gsm_table, gsm_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                gsm_table,
                sizeof(gsm_table) / sizeof(qmi_uim_ef_map_type),
                gsm_path,
                sizeof(gsm_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_gsm */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_RUIM()

  DESCRIPTION
    Creates the table with RUIM files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_ruim
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  boolean result = FALSE;
  QMI_UIM_RUIM_TABLE(ruim_table, ruim_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                ruim_table,
                sizeof(ruim_table) / sizeof(qmi_uim_ef_map_type),
                ruim_path,
                sizeof(ruim_path) / sizeof(uint16));

  return result;
#else 
  (void)file_access_ptr;
  (void)access_method;
  return FALSE;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* qmi_uim_convert_file_access_method_ruim */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_USIM()

  DESCRIPTION
    Creates the table with USIM files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_usim
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_USIM_TABLE(usim_table, usim_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                usim_table,
                sizeof(usim_table) / sizeof(qmi_uim_ef_map_type),
                usim_path,
                sizeof(usim_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_usim */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_CSIM()

  DESCRIPTION
    Creates the table with CSIM files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_csim
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
#ifdef FEATURE_MMGSDI_3GPP2
  boolean result = FALSE;
  QMI_UIM_CSIM_TABLE(csim_table, csim_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                csim_table,
                sizeof(csim_table) / sizeof(qmi_uim_ef_map_type),
                csim_path,
                sizeof(csim_path) / sizeof(uint16));

  return result;
#else
  (void)file_access_ptr;
  (void)access_method;
  return FALSE;
#endif /* FEATURE_MMGSDI_3GPP2 */
} /* qmi_uim_convert_file_access_method_csim */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_MF()

  DESCRIPTION
    Creates the table with miscellaneous files (files not under
    GSM, USIM or CDMA) and invokes the function to execute the
    conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_mf
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_MF_TABLE(mf_table, mf_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                mf_table,
                sizeof(mf_table) / sizeof(qmi_uim_ef_map_type),
                mf_path,
                sizeof(mf_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_mf */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_PROP_7F66_2G()

  DESCRIPTION
    Creates the table with propprietary 2G files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_prop_7f66_2g
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_PROP_7F66_2G_TABLE(prop_7f66_2g_table, prop_7f66_2g_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                prop_7f66_2g_table,
                sizeof(prop_7f66_2g_table) / sizeof(qmi_uim_ef_map_type),
                prop_7f66_2g_path,
                sizeof(prop_7f66_2g_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_prop_7f66_2g */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_PROP_7F66_3G()

  DESCRIPTION
    Creates the table with proprietary 3G files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_prop_7f66_3g
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_PROP_7F66_3G_TABLE(prop_7f66_3g_table, prop_7f66_3g_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                prop_7f66_3g_table,
                sizeof(prop_7f66_3g_table) / sizeof(qmi_uim_ef_map_type),
                prop_7f66_3g_path,
                sizeof(prop_7f66_3g_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_prop_7f66_3g */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_USIM_GSM_ACCESS()

  DESCRIPTION
    Creates the table with DF-GSM ACCESS files under 
    USIM and invokes the function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_usim_gsm_access
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_USIM_GSM_ACCESS_TABLE(usim_df_gsm_access_table, usim_df_gsm_access_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                usim_df_gsm_access_table,
                sizeof(usim_df_gsm_access_table) / sizeof(qmi_uim_ef_map_type),
                usim_df_gsm_access_path,
                sizeof(usim_df_gsm_access_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_usim_gsm_access */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_TELECOM_GRAPHICS()

  DESCRIPTION
    Creates the table with Telecom Graphics files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_telecom_graphics
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_TELECOM_GRAPHICS_TABLE(telecom_graphics_table, telecom_graphics_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                telecom_graphics_table,
                sizeof(telecom_graphics_table) / sizeof(qmi_uim_ef_map_type),
                telecom_graphics_path,
                sizeof(telecom_graphics_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_telecom_graphics */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_TELECOM_MCPTT()

  DESCRIPTION
    Creates the table with Telecom MCPTT files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_telecom_mcptt
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;
  QMI_UIM_TELECOM_MCPTT_TABLE(telecom_mcptt_table, telecom_mcptt_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                telecom_mcptt_table,
                sizeof(telecom_mcptt_table) / sizeof(qmi_uim_ef_map_type),
                telecom_mcptt_path,
                sizeof(telecom_mcptt_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_telecom_mcptt */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_USIM_ACDC()

  DESCRIPTION
    Creates the table with ACDC files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_usim_acdc(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;

  QMI_UIM_USIM_ACDC_TABLE(usim_acdc_table, usim_acdc_path);
  
  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                usim_acdc_table,
                sizeof(usim_acdc_table) / sizeof(qmi_uim_ef_map_type),
                usim_acdc_path,
                sizeof(usim_acdc_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_usim_acdc */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD_CD()

  DESCRIPTION
    Creates the table with Configuration data files and invokes the
    function to execute the conversion based on that table

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_convert_file_access_method_cd
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method
)
{
  boolean result = FALSE;

  QMI_UIM_CD_TABLE(cd_table, cd_path);

  ASSERT(file_access_ptr);

  result = qmi_uim_convert_file_access_method_table(
                file_access_ptr,
                access_method,
                cd_table,
                sizeof(cd_table) / sizeof(qmi_uim_ef_map_type),
                cd_path,
                sizeof(cd_path) / sizeof(uint16));

  return result;
} /* qmi_uim_convert_file_access_method_cd */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD()

  DESCRIPTION
    Converts file access method between path and MMGSDI enum

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method
    session_type    : session type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_convert_file_access_method
(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method,
  mmgsdi_session_type_enum_type    session_type
)
{
  ASSERT(file_access_ptr);

  /* Check file access method */
  if (file_access_ptr->access_method == access_method)
  {
    /* Input format matches output format, so we can
       immediately return success */
    UIM_MSG_HIGH_0("No file access conversion required");
    return TRUE;
  }

  switch(file_access_ptr->access_method)
  {
    case MMGSDI_BY_PATH_ACCESS:
      if (file_access_ptr->file.path_type.path_len == 2 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00)
      {
        return qmi_uim_convert_file_access_method_mf(file_access_ptr,
                                                     access_method);
      }
      if (file_access_ptr->file.path_type.path_len == 3 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00)
      {
        /* Look at the second item in the path to understand which
           function is required */
        if (file_access_ptr->file.path_type.path_buf[1] == 0x7F10)
        {
          return qmi_uim_convert_file_access_method_telecom(file_access_ptr,
                                                            access_method);
        }
        if (file_access_ptr->file.path_type.path_buf[1] == 0x7F11)
        {
          return qmi_uim_convert_file_access_method_cd(file_access_ptr,
                                                       access_method);
        }
        if (file_access_ptr->file.path_type.path_buf[1] == 0x7F20)
        {
          return qmi_uim_convert_file_access_method_gsm(file_access_ptr,
                                                        access_method);
        }
        if (file_access_ptr->file.path_type.path_buf[1] == 0x7F25)
        {
          return qmi_uim_convert_file_access_method_ruim(file_access_ptr,
                                                         access_method);
        }
        if (file_access_ptr->file.path_type.path_buf[1] == 0x7FFF)
        {
          if (session_type == MMGSDI_GW_PROV_PRI_SESSION ||
              session_type == MMGSDI_GW_PROV_SEC_SESSION ||
              session_type == MMGSDI_GW_PROV_TER_SESSION)
          {
            return qmi_uim_convert_file_access_method_usim(file_access_ptr,
                                                           access_method);
          }
          else if (session_type == MMGSDI_1X_PROV_PRI_SESSION ||
                   session_type == MMGSDI_1X_PROV_SEC_SESSION ||
                   session_type == MMGSDI_1X_PROV_TER_SESSION)
          {
            return qmi_uim_convert_file_access_method_csim(file_access_ptr,
                                                           access_method);
          }
          else
          {
            /* For non-provisioning sessions, don't convert the path
               into enum: nothing to do! */
          }
        }
      }
      if (file_access_ptr->file.path_type.path_len == 4 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00 &&
          file_access_ptr->file.path_type.path_buf[1] == 0x7F66 &&
          file_access_ptr->file.path_type.path_buf[2] == 0x5F30)
      {
        return qmi_uim_convert_file_access_method_prop_7f66_2g(file_access_ptr,
                                                               access_method);
      }
      if (file_access_ptr->file.path_type.path_len == 5 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00 &&
          file_access_ptr->file.path_type.path_buf[1] == 0x7FFF &&
          file_access_ptr->file.path_type.path_buf[2] == 0x7F66 &&
          file_access_ptr->file.path_type.path_buf[3] == 0x5F30)
      {
        if (session_type == MMGSDI_GW_PROV_PRI_SESSION ||
            session_type == MMGSDI_GW_PROV_SEC_SESSION ||
            session_type == MMGSDI_GW_PROV_TER_SESSION)
        {
          return qmi_uim_convert_file_access_method_prop_7f66_3g(file_access_ptr,
                                                                 access_method);
        }
      }
      if (file_access_ptr->file.path_type.path_len == 4 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00 &&
          file_access_ptr->file.path_type.path_buf[1] == 0x7F10 &&
          file_access_ptr->file.path_type.path_buf[2] == 0x5F50)
      {
          return qmi_uim_convert_file_access_method_telecom_graphics(file_access_ptr,
                                                                     access_method);
      }
      if (file_access_ptr->file.path_type.path_len == 4 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00 &&
          file_access_ptr->file.path_type.path_buf[1] == 0x7FFF &&
          file_access_ptr->file.path_type.path_buf[2] == 0x5F3B)
      {
        return qmi_uim_convert_file_access_method_usim_gsm_access(file_access_ptr,
                                                                  access_method);
      }
      if (file_access_ptr->file.path_type.path_len == 4 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00 &&
          file_access_ptr->file.path_type.path_buf[1] == 0x7F10 &&
          file_access_ptr->file.path_type.path_buf[2] == 0x5F3D)
      {
        return qmi_uim_convert_file_access_method_telecom_mcptt(file_access_ptr,
                                                                access_method);
      }
      if (file_access_ptr->file.path_type.path_len == 4 &&
          file_access_ptr->file.path_type.path_buf[0] == 0x3F00 &&
          file_access_ptr->file.path_type.path_buf[1] == 0x7FFF &&
          file_access_ptr->file.path_type.path_buf[2] == 0x5FA0)
      {
        if (session_type == MMGSDI_GW_PROV_PRI_SESSION ||
            session_type == MMGSDI_GW_PROV_SEC_SESSION ||
            session_type == MMGSDI_GW_PROV_TER_SESSION)
        {
          return qmi_uim_convert_file_access_method_usim_acdc(file_access_ptr, access_method);
        }
      }
      break;

    case MMGSDI_EF_ENUM_ACCESS:
      /* Look at the range of the enum to understand which
         function is required */
      if (file_access_ptr->file.file_enum >= MMGSDI_ICCID &&
          file_access_ptr->file.file_enum <= MMGSDI_ARR)
      {
        return qmi_uim_convert_file_access_method_mf(file_access_ptr,
                                                     access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_IMAGE_FILE &&
          file_access_ptr->file.file_enum <= MMGSDI_TELECOM_ICE_GRAPHICS)
      {
        return qmi_uim_convert_file_access_method_telecom_graphics(file_access_ptr,
                                                                   access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_TELECOM_ADN &&
          file_access_ptr->file.file_enum <= MMGSDI_TELECOM_MMSSMODE)
      {
        return qmi_uim_convert_file_access_method_telecom(file_access_ptr,
                                                          access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_GSM_LP &&
          file_access_ptr->file.file_enum <= MMGSDI_GSM_IMG3INST3)
      {
        return qmi_uim_convert_file_access_method_gsm(file_access_ptr,
                                                      access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_CDMA_CC &&
          file_access_ptr->file.file_enum <= MMGSDI_CDMA_3GPD_OP_CAP)
      {
        return qmi_uim_convert_file_access_method_ruim(file_access_ptr,
                                                       access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_USIM_LI &&
          file_access_ptr->file.file_enum <= MMGSDI_USIM_EPDG_SELECTION_EM)
      {
        return qmi_uim_convert_file_access_method_usim(file_access_ptr,
                                                       access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_CSIM_ARR &&
          file_access_ptr->file.file_enum <= MMGSDI_CSIM_PUID)
      {
        return qmi_uim_convert_file_access_method_csim(file_access_ptr,
                                                       access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_SIM_7F66_PROP1_ACT_HPLMN &&
          file_access_ptr->file.file_enum <= MMGSDI_SIM_7F66_PROP1_SPT_TABLE)
      {
        return qmi_uim_convert_file_access_method_prop_7f66_2g(file_access_ptr,
                                                               access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_USIM_7F66_PROP1_ACT_HPLMN &&
          file_access_ptr->file.file_enum <= MMGSDI_USIM_7F66_PROP1_RAT)
      {
        return qmi_uim_convert_file_access_method_prop_7f66_3g(file_access_ptr,
                                                               access_method);
      }
      if (file_access_ptr->file.file_enum >= MMGSDI_USIM_KC &&
          file_access_ptr->file.file_enum <= MMGSDI_USIM_INVSCAN)
      {
        return qmi_uim_convert_file_access_method_usim_gsm_access(file_access_ptr,
                                                                  access_method);
      }
      if (file_access_ptr->file.file_enum == MMGSDI_USIM_ACDC_LIST)
      {
        return qmi_uim_convert_file_access_method_usim_acdc(file_access_ptr,
                                                            access_method);
      }
      if (file_access_ptr->file.file_enum == MMGSDI_CD_LAUNCH_PAD)
      {
        return qmi_uim_convert_file_access_method_cd(file_access_ptr,
                                                     access_method);
      }
      if (file_access_ptr->file.file_enum == MMGSDI_TELECOM_MCPTT_MST)
      {
        return qmi_uim_convert_file_access_method_telecom_mcptt(file_access_ptr,
                                                                access_method);
      }
      break;

    default:
      /* Not supported */
      break;
  }

  return FALSE;
} /* qmi_uim_convert_file_access_method */


/*===========================================================================
  FUNCTION QMI_UIM_CHECK_PIN_DIGITS()

  DESCRIPTION
    Checks the digits of PIN value. Allowed digits are only from
    '0' to '9'. This function does not check the length of the PIN.

  PARAMETERS
    pin_value : value of the pin

  RETURN VALUE
    boolean to indicate success or failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_check_pin_digits
(
  mmgsdi_data_type  pin_value
)
{
  mmgsdi_len_type i = 0;

  if (pin_value.data_ptr == NULL)
  {
    return FALSE;
  }

  for (i = 0; i < pin_value.data_len; i++)
  {
    if (pin_value.data_ptr[i] < '0' ||
        pin_value.data_ptr[i] > '9')
    {
      UIM_MSG_ERR_1("QMI_UIM invalid PIN digit: 0x%x", pin_value.data_ptr[i]);
      return FALSE;
    }
  }

  return TRUE;
} /* qmi_uim_check_pin_digits */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_BOOLEAN()

  DESCRIPTION
    Parses a boolean TLV

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    boolean_ptr       : value of the boolean

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_boolean
(
  dsm_item_type **                 sdu_in,
  uint16                           len,
  qmi_uim_tlv_boolean_type       * boolean_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  value;
  } tlv_data;

  ASSERT(sdu_in && boolean_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Ok to refresh */
  switch (tlv_data.value)
  {
    case 0:
      boolean_ptr->value = FALSE;
      break;
    case 1:
      boolean_ptr->value = TRUE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_boolean */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SESSION_INFORMATION()

  DESCRIPTION
    Parses the TLV item of session information

  PARAMETERS
    sdu_in                  : input sdu
    len                     : length of the TLV item
    session_information_ptr : output with parsed values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_session_information
(
  dsm_item_type **                       sdu_in,
  uint16                                 len,
  qmi_uim_tlv_session_information_type * session_information_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  session_type;
    uint8  aid_len;
    uint8  aid_value[QMI_UIM_AID_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && session_information_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_SESSION_INFORMATION)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Session type */
  switch (tlv_data.session_type)
  {
    case UIM_SESSION_TYPE_PRIMARY_GW_V01:
      session_information_ptr->session_type = MMGSDI_GW_PROV_PRI_SESSION;
      break;
    case UIM_SESSION_TYPE_PRIMARY_1X_V01:
      session_information_ptr->session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;
    case UIM_SESSION_TYPE_SECONDARY_GW_V01:
      session_information_ptr->session_type = MMGSDI_GW_PROV_SEC_SESSION;
      break;
    case UIM_SESSION_TYPE_SECONDARY_1X_V01:
      session_information_ptr->session_type = MMGSDI_1X_PROV_SEC_SESSION;
      break;
    case UIM_SESSION_TYPE_NONPROVISIONING_SLOT_1_V01:
      session_information_ptr->session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
      break;
    case UIM_SESSION_TYPE_NONPROVISIONING_SLOT_2_V01:
      session_information_ptr->session_type = MMGSDI_NON_PROV_SESSION_SLOT_2;
      break;
    case UIM_SESSION_TYPE_CARD_ON_SLOT_1_V01:
      session_information_ptr->session_type = MMGSDI_CARD_SESSION_SLOT_1;
      break;
    case UIM_SESSION_TYPE_CARD_ON_SLOT_2_V01:
      session_information_ptr->session_type = MMGSDI_CARD_SESSION_SLOT_2;
      break;
    case UIM_SESSION_TYPE_CHANNEL_ID_SLOT_1_V01:
      session_information_ptr->session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
      break;
    case UIM_SESSION_TYPE_CHANNEL_ID_SLOT_2_V01:
      session_information_ptr->session_type = MMGSDI_NON_PROV_SESSION_SLOT_2;
      break;
    case UIM_SESSION_TYPE_TERTIARY_GW_V01:
      session_information_ptr->session_type = MMGSDI_GW_PROV_TER_SESSION;
      break;
    case UIM_SESSION_TYPE_TERTIARY_1X_V01:
      session_information_ptr->session_type = MMGSDI_1X_PROV_TER_SESSION;
      break;
    case UIM_SESSION_TYPE_QUATERNARY_GW_V01:
    case UIM_SESSION_TYPE_QUATERNARY_1X_V01:
    case UIM_SESSION_TYPE_QUINARY_GW_V01:
    case UIM_SESSION_TYPE_QUINARY_1X_V01:
      return QMI_ERR_NOT_SUPPORTED;
    case UIM_SESSION_TYPE_NONPROVISIONING_SLOT_3_V01:
      session_information_ptr->session_type = MMGSDI_NON_PROV_SESSION_SLOT_3;
      break;
    case UIM_SESSION_TYPE_NONPROVISIONING_SLOT_4_V01:
    case UIM_SESSION_TYPE_NONPROVISIONING_SLOT_5_V01:
      return QMI_ERR_NOT_SUPPORTED;
    case UIM_SESSION_TYPE_CARD_ON_SLOT_3_V01:
      session_information_ptr->session_type = MMGSDI_CARD_SESSION_SLOT_3;
      break;
    case UIM_SESSION_TYPE_CARD_ON_SLOT_4_V01:
    case UIM_SESSION_TYPE_CARD_ON_SLOT_5_V01:
      return QMI_ERR_NOT_SUPPORTED;
    case UIM_SESSION_TYPE_CHANNEL_ID_SLOT_3_V01:
      session_information_ptr->session_type = MMGSDI_NON_PROV_SESSION_SLOT_3;
      break;
    case UIM_SESSION_TYPE_CHANNEL_ID_SLOT_4_V01:
    case UIM_SESSION_TYPE_CHANNEL_ID_SLOT_5_V01:
      return QMI_ERR_NOT_SUPPORTED;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  /* AID length */
  session_information_ptr->aid.data_len = (mmgsdi_len_type)tlv_data.aid_len;
  if (session_information_ptr->aid.data_len + QMI_UIM_TLV_MIN_LEN_SESSION_INFORMATION != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (session_information_ptr->aid.data_len > QMI_UIM_AID_MAX_V01 &&
      session_information_ptr->aid.data_len > MMGSDI_MAX_AID_LEN)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }
  /* Validate Channel ID length, reject length other than 1 byte */
  if ((tlv_data.session_type == UIM_SESSION_TYPE_CHANNEL_ID_SLOT_1_V01) ||
      (tlv_data.session_type == UIM_SESSION_TYPE_CHANNEL_ID_SLOT_2_V01) ||
      (tlv_data.session_type == UIM_SESSION_TYPE_CHANNEL_ID_SLOT_3_V01))
  {
    if (session_information_ptr->aid.data_len != QMI_UIM_TLV_CHANNEL_ID_LEN)
    {
      return QMI_ERR_INVALID_ARG;
    }
  }

  /* AID value */
  if(session_information_ptr->aid.data_len > 0)
  {
    (void)memscpy((void*)session_information_ptr->aid.data_ptr,
                  sizeof(session_information_ptr->aid.data_ptr),
                  (void*)tlv_data.aid_value,
                  (uint32)session_information_ptr->aid.data_len);
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_session_information */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_FILE_ID()

  DESCRIPTION
    Parses the TLV item of file id

  PARAMETERS
    sdu_in      : input sdu
    len         : length of the TLV item
    file_id_ptr : output with file id value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_file_id
(
  dsm_item_type **           sdu_in,
  uint16                     len,
  qmi_uim_tlv_file_id_type * file_id_ptr
)
{
  int32                          temp;

  PACKED struct PACKED_POST
  {
    uint16 file_id;
    uint8  path_len;
    uint8  path_value[QMI_UIM_PATH_MAX_V01 - 2];
  } tlv_data;

  ASSERT(sdu_in && file_id_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_FILE_ID)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Set the file id. */
  file_id_ptr->file_id = tlv_data.file_id;

  /* Set access by path */
  file_id_ptr->file_access.access_method = MMGSDI_BY_PATH_ACCESS;

  if (tlv_data.path_len == 0)
  {
    /* Return error if the file id is not 0x6Fxx and 0x2Fxx */
    if((uint8) (tlv_data.file_id >> 8) != 0x6F && 
       (uint8) (tlv_data.file_id >> 8) != 0x2F)
    {
      return QMI_ERR_INVALID_ARG;
    }
    return QMI_ERR_NONE;
  }

  /* Verify path length value */
  if (((tlv_data.path_len & 0x01) != 0) || 
       (tlv_data.path_len + QMI_UIM_TLV_MIN_LEN_FILE_ID != len))
  {
    return QMI_ERR_INVALID_ARG;
  }
  if ( (tlv_data.path_len > (MMGSDI_MAX_PATH_LEN - 1) * sizeof(uint16)) )
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Path length (in numbers of uint16) and value */
  file_id_ptr->file_access.file.path_type.path_len =
    ((mmgsdi_len_type)tlv_data.path_len) / sizeof(uint16);
  (void)memscpy((void*)file_id_ptr->file_access.file.path_type.path_buf,
                sizeof(file_id_ptr->file_access.file.path_type.path_buf),
                (void*)tlv_data.path_value,
                tlv_data.path_len );

  /* Add file id to the path */
  file_id_ptr->file_access.file.path_type.path_buf[file_id_ptr->file_access.file.path_type.path_len] = tlv_data.file_id;
  file_id_ptr->file_access.file.path_type.path_len += 1;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_file_id */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_READ_TRANSPARENT()

  DESCRIPTION
    Parses the TLV item of read transparent

  PARAMETERS
    sdu_in               : input sdu
    len                  : length of the TLV item
    read_transparent_ptr : output with read tranparent value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_read_transparent
(
  dsm_item_type **                    sdu_in,
  uint16                              len,
  qmi_uim_tlv_read_transparent_type * read_transparent_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint16 offset;
    uint16 length;
  } tlv_data;

  ASSERT(sdu_in && read_transparent_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Offset */
  read_transparent_ptr->offset = (mmgsdi_offset_type)tlv_data.offset;

  /* Length */
  read_transparent_ptr->length = (mmgsdi_len_type)tlv_data.length;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_read_transparent */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_READ_RECORD()

  DESCRIPTION
    Parses the TLV item of read record

  PARAMETERS
    sdu_in          : input sdu
    len             : length of the TLV item
    read_record_ptr : output with read record value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_read_record
(
  dsm_item_type **               sdu_in,
  uint16                         len,
  qmi_uim_tlv_read_record_type * read_record_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint16 record;
    uint16 length;
  } tlv_data;

  ASSERT(sdu_in && read_record_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Offset */
  read_record_ptr->record = (mmgsdi_rec_num_type)tlv_data.record;

  /* Record */
  read_record_ptr->length = (mmgsdi_len_type)tlv_data.length;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_read_record */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_READ_RECORD_LAST()

  DESCRIPTION
    Parses the TLV item of read record last

  PARAMETERS
    sdu_in               : input sdu
    len                  : length of the TLV item
    read_record_last_ptr : output with read record last value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_read_record_last
(
  dsm_item_type **                    sdu_in,
  uint16                              len,
  qmi_uim_tlv_read_record_last_type * read_record_last_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint16 last_record;
  } tlv_data;

  ASSERT(sdu_in && read_record_last_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Record */
  read_record_last_ptr->last_record = (mmgsdi_rec_num_type)tlv_data.last_record;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_read_record_last */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_WRITE_TRANSPARENT()

  DESCRIPTION
    Parses the TLV item of write transparent

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    write_transparent_ptr : output with write tranparent value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    Allocates memory that needs to be freed by the caller

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_write_transparent
(
  dsm_item_type **                     sdu_in,
  uint16                               len,
  qmi_uim_tlv_write_transparent_type * write_transparent_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint16 offset;
    uint16 length;
  } tlv_data;

  ASSERT(sdu_in && write_transparent_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Offset */
  write_transparent_ptr->offset = (mmgsdi_offset_type)tlv_data.offset;

  /* Length */
  write_transparent_ptr->data.data_len = (mmgsdi_len_type)tlv_data.length;

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract body to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  if (tlv_data.length > 0)
  {
    write_transparent_ptr->data.data_ptr =
          (uint8*)uimqmi_malloc(tlv_data.length);
    if (write_transparent_ptr->data.data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    temp = dsm_pullup(sdu_in,
                      (void*)write_transparent_ptr->data.data_ptr,
                      tlv_data.length);
    if (temp != tlv_data.length)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_write_transparent */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_WRITE_RECORD()

  DESCRIPTION
    Parses the TLV item of write record

  PARAMETERS
    sdu_in           : input sdu
    len              : length of the TLV item
    write_record_ptr : output with write record value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    Allocates memory that needs to be freed by the caller

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_write_record
(
  dsm_item_type **                sdu_in,
  uint16                          len,
  qmi_uim_tlv_write_record_type * write_record_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint16 record;
    uint16 length;
  } tlv_data;

  ASSERT(sdu_in && write_record_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Record */
  write_record_ptr->record = (mmgsdi_rec_num_type)tlv_data.record;

  /* Length */
  write_record_ptr->data.data_len = (mmgsdi_len_type)tlv_data.length;

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract body to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  if (tlv_data.length > 0)
  {
    write_record_ptr->data.data_ptr =
      (uint8*)uimqmi_malloc(tlv_data.length);
    if (write_record_ptr->data.data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    temp = dsm_pullup(sdu_in,
                      (void*)write_record_ptr->data.data_ptr,
                      tlv_data.length);
    if (temp != tlv_data.length)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_write_record */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SET_PIN_PROTECTION()

  DESCRIPTION
    Parses the TLV item of set pin protection

  PARAMETERS
    sdu_in                 : input sdu
    len                    : length of the TLV item
    set_pin_protection_ptr : output with set pin protection value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_set_pin_protection
(
  dsm_item_type **                      sdu_in,
  uint16                                len,
  qmi_uim_tlv_set_pin_protection_type * set_pin_protection_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  pin_id;
    uint8  pin_operation;
    uint8  pin_len;
    uint8  pin_value[QMI_UIM_PIN_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && set_pin_protection_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_SET_PIN_PROTECTION)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Pin id */
  switch(tlv_data.pin_id)
  {
    case UIM_PIN_ID_PIN_1_V01:
      set_pin_protection_ptr->pin_id = MMGSDI_PIN1;
      break;
    case UIM_PIN_ID_PIN_2_V01:
      set_pin_protection_ptr->pin_id = MMGSDI_PIN2;
      break;
    case UIM_PIN_ID_UNIVERSAL_V01:
      set_pin_protection_ptr->pin_id = MMGSDI_UNIVERSAL_PIN;
      break;
    case UIM_PIN_ID_HIDDEN_KEY_V01:
      set_pin_protection_ptr->pin_id = MMGSDI_HIDDENKEY;
      break;
    default:
      return QMI_ERR_INVALID_PINID;
  }

  /* Pin operation */
  switch (tlv_data.pin_operation)
  {
    case UIM_PIN_OPERATION_DISABLE_V01:
      set_pin_protection_ptr->pin_enable = FALSE;
      break;
    case UIM_PIN_OPERATION_ENABLE_V01:
      set_pin_protection_ptr->pin_enable = TRUE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  /* Pin length */
  set_pin_protection_ptr->pin_data.data_len = (mmgsdi_len_type)tlv_data.pin_len;

  /* Check data length */
  if (len != QMI_UIM_TLV_MIN_LEN_SET_PIN_PROTECTION +
             set_pin_protection_ptr->pin_data.data_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (set_pin_protection_ptr->pin_data.data_len == 0)
  {
    return QMI_ERR_MISSING_ARG;
  }
  if (set_pin_protection_ptr->pin_data.data_len > QMI_UIM_PIN_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Pin value */
  (void)memscpy((void*)set_pin_protection_ptr->pin_value,
                sizeof(set_pin_protection_ptr->pin_value),
               (void*)tlv_data.pin_value,
               (uint32)set_pin_protection_ptr->pin_data.data_len);
  set_pin_protection_ptr->pin_data.data_ptr = set_pin_protection_ptr->pin_value;

  /* Check PIN value */
  if (!qmi_uim_check_pin_digits(set_pin_protection_ptr->pin_data))
  {
    return QMI_ERR_INCORRECT_PIN;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_set_pin_protection */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_VERIFY_PIN()

  DESCRIPTION
    Parses the TLV item of verify pin

  PARAMETERS
    sdu_in         : input sdu
    len            : length of the TLV item
    verify_pin_ptr : output with verify pin value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_verify_pin
(
  dsm_item_type **              sdu_in,
  uint16                        len,
  qmi_uim_tlv_verify_pin_type * verify_pin_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  pin_id;
    uint8  pin_len;
    uint8  pin_data[QMI_UIM_PIN_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && verify_pin_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_VERIFY_PIN)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Pin id */
  switch(tlv_data.pin_id)
  {
    case UIM_PIN_ID_PIN_1_V01:
      verify_pin_ptr->pin_id = MMGSDI_PIN1;
      break;
    case UIM_PIN_ID_PIN_2_V01:
      verify_pin_ptr->pin_id = MMGSDI_PIN2;
      break;
    case UIM_PIN_ID_UNIVERSAL_V01:
      verify_pin_ptr->pin_id = MMGSDI_UNIVERSAL_PIN;
      break;
    case UIM_PIN_ID_HIDDEN_KEY_V01:
      verify_pin_ptr->pin_id = MMGSDI_HIDDENKEY;
      break;
    default:
      return QMI_ERR_INVALID_PINID;
  }

  /* Pin length */
  verify_pin_ptr->pin_data.data_len = (mmgsdi_len_type)tlv_data.pin_len;

  /* Check data length */
  if (len != QMI_UIM_TLV_MIN_LEN_VERIFY_PIN +
             verify_pin_ptr->pin_data.data_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Accept PIN length as 0 only in case of PIN1 because the request
     might contain also an encrypted PIN value */
  if (verify_pin_ptr->pin_data.data_len == 0 &&
      verify_pin_ptr->pin_id != MMGSDI_PIN1)
  {
    return QMI_ERR_INVALID_ARG;
  }

  if (verify_pin_ptr->pin_data.data_len > QMI_UIM_PIN_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Pin value */
  (void)memscpy((void*)verify_pin_ptr->pin_value,
                sizeof(verify_pin_ptr->pin_value),
                (void*)tlv_data.pin_data,
                (uint32)verify_pin_ptr->pin_data.data_len);
  verify_pin_ptr->pin_data.data_ptr = verify_pin_ptr->pin_value;

  /* Check PIN value */
  if (!qmi_uim_check_pin_digits(verify_pin_ptr->pin_data))
  {
    return QMI_ERR_INCORRECT_PIN;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_verify_pin */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_UNBLOCK_PIN()

  DESCRIPTION
    Parses the TLV item of unblock pin

  PARAMETERS
    sdu_in          : input sdu
    len             : length of the TLV item
    unblock_pin_ptr : output with unblock pin value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_unblock_pin
(
  dsm_item_type **               sdu_in,
  uint16                         len,
  qmi_uim_tlv_unblock_pin_type * unblock_pin_ptr
)
{
  int32             temp;
  uint8             temp_uint8;

  /* The payload has two variable items and one of them is located in the middle
     so we cannot easily use a packed structure and use a simple buffer */
  uint8  tlv_data[QMI_UIM_TLV_MIN_LEN_UNBLOCK_PIN + (2 * QMI_UIM_PIN_MAX_V01)];
  uint8* current = tlv_data;

  ASSERT(sdu_in && unblock_pin_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_UNBLOCK_PIN)
  {
   return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Pin id */
  (void)memscpy((void*)&temp_uint8, sizeof(uint8), current, sizeof(uint8));
  current += sizeof(uint8);
  switch(temp_uint8)
  {
    case UIM_PIN_ID_PIN_1_V01:
      unblock_pin_ptr->pin_id = MMGSDI_PIN1;
      break;
    case UIM_PIN_ID_PIN_2_V01:
      unblock_pin_ptr->pin_id = MMGSDI_PIN2;
      break;
    case UIM_PIN_ID_UNIVERSAL_V01:
      unblock_pin_ptr->pin_id = MMGSDI_UNIVERSAL_PIN;
      break;
    default:
      return QMI_ERR_INVALID_PINID;
  }

  /* Puk length */
  (void)memscpy((void*)&temp_uint8, sizeof(uint8), current, sizeof(uint8));
  current += sizeof(uint8);
  unblock_pin_ptr->puk_data.data_len = (mmgsdi_len_type)temp_uint8;

  /* Check data length */
  if (len < QMI_UIM_TLV_MIN_LEN_UNBLOCK_PIN +
            unblock_pin_ptr->puk_data.data_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (unblock_pin_ptr->puk_data.data_len == 0)
  {
    return QMI_ERR_MISSING_ARG;
  }
  if (unblock_pin_ptr->puk_data.data_len > QMI_UIM_PIN_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Puk value */
  (void)memscpy((void*)unblock_pin_ptr->puk_value,
                sizeof(unblock_pin_ptr->puk_value),
                (void*)current,
                (uint32)unblock_pin_ptr->puk_data.data_len);
  current += unblock_pin_ptr->puk_data.data_len;
  unblock_pin_ptr->puk_data.data_ptr = unblock_pin_ptr->puk_value;

  /* Check PUK value */
  if (!qmi_uim_check_pin_digits(unblock_pin_ptr->puk_data))
  {
    return QMI_ERR_INCORRECT_PIN;
  }

  /* Pin length */
  (void)memscpy((void*)&temp_uint8, sizeof(uint8), current, sizeof(uint8));
  current += sizeof(uint8);
  unblock_pin_ptr->pin_data.data_len = (mmgsdi_len_type)temp_uint8;

  /* Check pin length */
  if (len != QMI_UIM_TLV_MIN_LEN_UNBLOCK_PIN +
             unblock_pin_ptr->puk_data.data_len +
             unblock_pin_ptr->pin_data.data_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (unblock_pin_ptr->pin_data.data_len == 0)
  {
    return QMI_ERR_MISSING_ARG;
  }
  if (unblock_pin_ptr->pin_data.data_len > QMI_UIM_PIN_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Pin value */
  (void)memscpy((void*)unblock_pin_ptr->pin_value,
                sizeof(unblock_pin_ptr->pin_value),
                (void*)current,
                (uint32)unblock_pin_ptr->pin_data.data_len);
  current += unblock_pin_ptr->pin_data.data_len;
  unblock_pin_ptr->pin_data.data_ptr = unblock_pin_ptr->pin_value;

  /* Check PIN value */
  if (!qmi_uim_check_pin_digits(unblock_pin_ptr->pin_data))
  {
    return QMI_ERR_INCORRECT_PIN;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_unblock_pin */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_CHANGE_PIN()

  DESCRIPTION
    Parses the TLV item of change pin

  PARAMETERS
    sdu_in         : input sdu
    len            : length of the TLV item
    change_pin_ptr : output with change pin value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_change_pin
(
  dsm_item_type **              sdu_in,
  uint16                        len,
  qmi_uim_tlv_change_pin_type * change_pin_ptr
)
{
  int32             temp;
  uint8             temp_uint8;

  /* The payload has two variable items and one of them is located in the middle
     so we cannot easily use a packed structure and use a simple buffer */
  uint8  tlv_data[QMI_UIM_TLV_MIN_LEN_CHANGE_PIN + (2 * QMI_UIM_PIN_MAX_V01)];
  uint8* current = tlv_data;

  ASSERT(sdu_in && change_pin_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_CHANGE_PIN)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Pin id */
  (void)memscpy((void*)&temp_uint8, sizeof(uint8), current, sizeof(uint8));
  current += sizeof(uint8);
  switch(temp_uint8)
  {
    case UIM_PIN_ID_PIN_1_V01:
      change_pin_ptr->pin_id = MMGSDI_PIN1;
      break;
    case UIM_PIN_ID_PIN_2_V01:
      change_pin_ptr->pin_id = MMGSDI_PIN2;
      break;
    case UIM_PIN_ID_UNIVERSAL_V01:
      change_pin_ptr->pin_id = MMGSDI_UNIVERSAL_PIN;
      break;
    case UIM_PIN_ID_HIDDEN_KEY_V01:
      change_pin_ptr->pin_id = MMGSDI_HIDDENKEY;
      break;
    default:
      return QMI_ERR_INVALID_PINID;
  }

  /* Old pin length */
  (void)memscpy((void*)&temp_uint8, sizeof(uint8), current, sizeof(uint8));
  current += sizeof(uint8);
  change_pin_ptr->old_pin_data.data_len = (mmgsdi_len_type)temp_uint8;

  /* Check data length */
  if (len < QMI_UIM_TLV_MIN_LEN_CHANGE_PIN +
            change_pin_ptr->old_pin_data.data_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (change_pin_ptr->old_pin_data.data_len == 0)
  {
    return QMI_ERR_MISSING_ARG;
  }
  if (change_pin_ptr->old_pin_data.data_len > QMI_UIM_PIN_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Old pin value */
  (void)memscpy((void*)change_pin_ptr->old_pin_value,
                sizeof(change_pin_ptr->old_pin_value),
                (void*)current,
                (uint32)change_pin_ptr->old_pin_data.data_len);
  current += change_pin_ptr->old_pin_data.data_len;
  change_pin_ptr->old_pin_data.data_ptr = change_pin_ptr->old_pin_value;

  /* Check old PIN value */
  if (!qmi_uim_check_pin_digits(change_pin_ptr->old_pin_data))
  {
    return QMI_ERR_INCORRECT_PIN;
  }

  /* New pin length */
  (void)memscpy((void*)&temp_uint8, sizeof(uint8), current, sizeof(uint8));
  current += sizeof(uint8);
  change_pin_ptr->new_pin_data.data_len = (mmgsdi_len_type)temp_uint8;

  /* Check new pin length */
  if (len != QMI_UIM_TLV_MIN_LEN_CHANGE_PIN +
             change_pin_ptr->old_pin_data.data_len +
             change_pin_ptr->new_pin_data.data_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (change_pin_ptr->new_pin_data.data_len == 0)
  {
    return QMI_ERR_MISSING_ARG;
  }
  if (change_pin_ptr->new_pin_data.data_len > QMI_UIM_PIN_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* New pin value */
  (void)memscpy((void*)change_pin_ptr->new_pin_value,
                sizeof(change_pin_ptr->new_pin_value),
                (void*)current,
                (uint32)change_pin_ptr->new_pin_data.data_len);
  current += change_pin_ptr->new_pin_data.data_len;
  change_pin_ptr->new_pin_data.data_ptr = change_pin_ptr->new_pin_value;

  /* Check new PIN value */
  if (!qmi_uim_check_pin_digits(change_pin_ptr->new_pin_data))
  {
    return QMI_ERR_INCORRECT_PIN;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_change_pin */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SLOT()

  DESCRIPTION
    Parses the TLV item of slot

  PARAMETERS
    sdu_in   : input sdu
    len      : length of the TLV item
    slot_ptr : output with slot value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_slot
(
  dsm_item_type **        sdu_in,
  uint16                  len,
  qmi_uim_tlv_slot_type * slot_ptr
)
{
  int32             temp          = 0;
  qmi_error_e_type  qmi_err       = QMI_ERR_NONE;

  PACKED struct PACKED_POST
  {
    uint8  slot;
  } tlv_data;

  ASSERT(sdu_in && slot_ptr && qmi_uim_global_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Derive slot id */
  qmi_err = qmi_uim_convert_qmi_slot_to_mmgsdi_slot(tlv_data.slot,
                                                    &(slot_ptr->slot));
  /* Check if the device supports the slot id */
  if(qmi_err == QMI_ERR_NONE)
  {
    if(slot_ptr->slot > qmi_uim_global_ptr->card_state.num_slots)
    {
      qmi_err = QMI_ERR_NOT_SUPPORTED;
    }
  }

  return qmi_err;
} /* qmi_uim_tlv_slot */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_EVENT_REG()

  DESCRIPTION
    Parses the TLV item of event reg

  PARAMETERS
    sdu_in        : input sdu
    len           : length of the TLV item
    event_reg_ptr : output with event reg value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_event_reg
(
  dsm_item_type **             sdu_in,
  uint16                       len,
  qmi_uim_tlv_event_reg_type * event_reg_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  bitmask;
  } tlv_data;

  ASSERT(sdu_in && event_reg_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Set all flags to false by default */
  memset(event_reg_ptr, 0, sizeof(qmi_uim_tlv_event_reg_type));

  /* Legacy card status */
  if ((tlv_data.bitmask & 0x00000001) != 0)
  {
    event_reg_ptr->card_status = TRUE;
  }

  /* Modem events */
  if ((tlv_data.bitmask & 0x00000002) != 0)
  {
    event_reg_ptr->sap_events = TRUE;
  }

  /* Extended card status */
  if ((tlv_data.bitmask & 0x00000004) != 0)
  {
    event_reg_ptr->extended_card_status = TRUE;
  }

  /* Provisioning session close indication */
  if((tlv_data.bitmask & 0x00000008) != 0)
  {
    event_reg_ptr->provisioning_session_close_event = TRUE;
  }

  /* Physical slots status */
  if ((tlv_data.bitmask & 0x00000010) != 0)
  {
    event_reg_ptr->physical_slots_status = TRUE;
  }

  /* Sim Busy events */
  if((tlv_data.bitmask & 0x00000020) != 0)
  {
    event_reg_ptr->sim_busy_events = TRUE;
  }

  /* Reduced card status */
  if((tlv_data.bitmask & 0x00000040) != 0)
  {
    event_reg_ptr->reduced_card_status_events = TRUE;
  }

  /* Recovery Complete indication */
  if((tlv_data.bitmask & 0x00000080) != 0)
  {
    event_reg_ptr->recovery_complete_event = TRUE;
  }

  /* Supply voltage events */
  if((tlv_data.bitmask & 0x00000100) != 0)
  {
    event_reg_ptr->supply_voltage_events = TRUE;
  }

  /* Card activation status indication */
  if((tlv_data.bitmask & 0x00000200) != 0)
  {
    event_reg_ptr->activation_status_event = TRUE;
  }

  /* Simlock remote SFS events */
  if((tlv_data.bitmask & 0x00000400) != 0)
  {
    event_reg_ptr->simlock_remote_sfs_events = TRUE;
  }

  /* Simlock remote SFS events */
  if((tlv_data.bitmask & 0x00000800) != 0)
  {
    event_reg_ptr->simlock_temp_unlock_status = TRUE;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_event_reg */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_AUTHENTICATE()

  DESCRIPTION
    Parses the TLV item of event reg

  PARAMETERS
    sdu_in        : input sdu
    len           : length of the TLV item
    event_reg_ptr : output with event reg value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_authenticate
(
  dsm_item_type **                sdu_in,
  uint16                          len,
  qmi_uim_tlv_authenticate_type * authenticate_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  context;
    uint16 length;
  } tlv_data;

  ASSERT(sdu_in && authenticate_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if(tlv_data.context > UIM_AUTH_CONTEXT_COMPUTE_IP_HRPD_ACCESS_V01)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Context */
  authenticate_ptr->auth_context = (uim_auth_context_enum_v01)tlv_data.context;

  /* Length */
  authenticate_ptr->auth_data.data_len = (mmgsdi_len_type)tlv_data.length;

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract body for authenticate. The memory allocated here needs to be freed by
     the caller, after using the data */
  if (tlv_data.length > 0)
  {
    authenticate_ptr->auth_data.data_ptr =
      (uint8*)uimqmi_malloc(tlv_data.length);
    if (authenticate_ptr->auth_data.data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    temp = dsm_pullup(sdu_in,
                      (void*)authenticate_ptr->auth_data.data_ptr,
                      tlv_data.length);
    if (temp != tlv_data.length)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_authenticate */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_CAPABILITIES_MASK()

  DESCRIPTION
    Parses the TLV item of capabilities mask

  PARAMETERS
    sdu_in        : input sdu
    len           : length of the TLV item
    event_reg_ptr : output with capabilities mask value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_capabilities_mask
(
  dsm_item_type **                     sdu_in,
  uint16                               len,
  qmi_uim_tlv_capabilities_mask_type * capabilities_mask_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  bitmask;
  } tlv_data;

  ASSERT(sdu_in && capabilities_mask_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Set all flags to false by default */
  memset(capabilities_mask_ptr, 0, sizeof(qmi_uim_tlv_capabilities_mask_type));

  /* Card status */
  if ((tlv_data.bitmask & 0x00000001) != 0)
  {
    capabilities_mask_ptr->fdn_status = TRUE;
  }
  if ((tlv_data.bitmask & 0x00000002) != 0)
  {
    capabilities_mask_ptr->hidden_key_status = TRUE;
  }
  if ((tlv_data.bitmask & 0x00000004) != 0)
  {
    capabilities_mask_ptr->index_in_dir = TRUE;
  }
  if ((tlv_data.bitmask & 0x00000008) != 0)
  {
    capabilities_mask_ptr->esn_me_status = TRUE;
  }
  if ((tlv_data.bitmask & 0x00000010) != 0)
  {
    capabilities_mask_ptr->acl_status = TRUE;
  }
  if ((tlv_data.bitmask & 0x00000020) != 0)
  {
    capabilities_mask_ptr->service_status = TRUE;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_capabilities_mask */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_APPLICATION_INFORMATION()

  DESCRIPTION
    Parses the TLV item of application information

  PARAMETERS
    sdu_in        : input sdu
    len           : length of the TLV item
    app_info_ptr  : output with application information value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_application_information
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_app_information_type * app_info_ptr
)
{
  int32             temp   = 0;
  qmi_error_e_type  errval = QMI_ERR_NONE;

  PACKED struct PACKED_POST
  {
    uint8  slot;
    uint8  aid_len;
    uint8  aid_value[QMI_UIM_AID_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && app_info_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_APPLICATION_INFORMATION)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Slot id */
  errval = qmi_uim_convert_qmi_slot_to_mmgsdi_slot(tlv_data.slot,
                                                   &(app_info_ptr->slot));
  if(errval != QMI_ERR_NONE)
  {
    return errval;
  }

  /* AID length */
  app_info_ptr->aid.data_len = (mmgsdi_len_type)tlv_data.aid_len;
  if (app_info_ptr->aid.data_len + QMI_UIM_TLV_MIN_LEN_APPLICATION_INFORMATION != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (app_info_ptr->aid.data_len > MMGSDI_MAX_AID_LEN)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* AID value */
  if (app_info_ptr->aid.data_len > 0)
  {
    (void)memscpy((void*)app_info_ptr->aid.data_ptr,
                  sizeof(app_info_ptr->aid.data_ptr),
                  (void*)tlv_data.aid_value,
                  (uint32)app_info_ptr->aid.data_len);
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_application_information */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SESSION_CHANGE()

  DESCRIPTION
    Parses the TLV item of session change

  PARAMETERS
    sdu_in             : input sdu
    len                : length of the TLV item
    session_change_ptr : output with session change value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_session_change
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_session_change_type  * session_change_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  session_type;
    uint8  activate;
  } tlv_data;

  ASSERT(sdu_in && session_change_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Session type */
  switch (tlv_data.session_type)
  {
    case UIM_SESSION_TYPE_PRIMARY_GW_V01:
      session_change_ptr->session_type = MMGSDI_GW_PROV_PRI_SESSION;
      break;
    case UIM_SESSION_TYPE_PRIMARY_1X_V01:
      session_change_ptr->session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;
    case UIM_SESSION_TYPE_SECONDARY_GW_V01:
      session_change_ptr->session_type = MMGSDI_GW_PROV_SEC_SESSION;
      break;
    case UIM_SESSION_TYPE_SECONDARY_1X_V01:
      session_change_ptr->session_type = MMGSDI_1X_PROV_SEC_SESSION;
      break;
    case UIM_SESSION_TYPE_TERTIARY_GW_V01:
      session_change_ptr->session_type = MMGSDI_GW_PROV_TER_SESSION;
      break;
    case UIM_SESSION_TYPE_TERTIARY_1X_V01:
      session_change_ptr->session_type = MMGSDI_1X_PROV_TER_SESSION;
      break;
    case UIM_SESSION_TYPE_QUATERNARY_GW_V01:
    case UIM_SESSION_TYPE_QUATERNARY_1X_V01:
    case UIM_SESSION_TYPE_QUINARY_GW_V01:
    case UIM_SESSION_TYPE_QUINARY_1X_V01:
      return QMI_ERR_NOT_SUPPORTED;
    default:
      /* Only provisioning sessions are valid for session change */
      return QMI_ERR_INVALID_ARG;
  }

  /* Activate */
  switch(tlv_data.activate)
  {
    case 0:
      session_change_ptr->activate = FALSE;
      break;
    case 1:
      session_change_ptr->activate = TRUE;
      break;
    default:
      /* Only provisioning sessions are valid for session change */
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_session_change */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_APDU_REQUEST()

  DESCRIPTION
    Parses the TLV item of APDU request

  PARAMETERS
    sdu_in        : input sdu
    len           : length of the TLV item
    app_info_ptr  : output with APDU request value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_apdu_request
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_apdu_type            * apdu_info_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint16  apdu_len;
  } tlv_data;

  ASSERT(sdu_in && apdu_info_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* APDU data length */
  apdu_info_ptr->apdu_data.data_len = (mmgsdi_len_type)tlv_data.apdu_len;

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.apdu_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract APDU data to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  if (tlv_data.apdu_len > 0)
  {
    apdu_info_ptr->apdu_data.data_ptr =
      (uint8*)uimqmi_malloc(tlv_data.apdu_len);

    if (apdu_info_ptr->apdu_data.data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    temp = dsm_pullup(sdu_in,
                      (void*)apdu_info_ptr->apdu_data.data_ptr,
                      tlv_data.apdu_len);
    if (temp != tlv_data.apdu_len)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_apdu_request */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_CHANNEL_ID()

  DESCRIPTION
    Parses the TLV item of channel id

  PARAMETERS
    sdu_in               : input sdu
    len                  : length of the TLV item
    apdu_channel_id_ptr  : output with APDU channel id value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_channel_id
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_logical_channel_type * logical_channel_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  logical_channel;
  } tlv_data;

  ASSERT(sdu_in && logical_channel_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  logical_channel_ptr->logical_channel = tlv_data.logical_channel;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_channel_id */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_AID()

  DESCRIPTION
    Parses the TLV item of channel id for a close request

  PARAMETERS
    sdu_in               : input sdu
    len                  : length of the TLV item
    aid_ptr              : output with AID value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_aid
(
  dsm_item_type **       sdu_in,
  uint16                 len,
  qmi_uim_tlv_aid_type * aid_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  aid_len;
    uint8  aid_value[MMGSDI_MAX_APP_ID_LEN];
  } tlv_data;

  ASSERT(sdu_in && aid_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_AID)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (len > sizeof(tlv_data))
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* AID length */
  aid_ptr->aid.data_len = (mmgsdi_len_type)tlv_data.aid_len;
  if (aid_ptr->aid.data_len + QMI_UIM_TLV_MIN_LEN_AID != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }
  if (aid_ptr->aid.data_len > MMGSDI_MAX_APP_ID_LEN)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* AID value */
  if (aid_ptr->aid.data_len > 0)
  {
    (void)memscpy((void*)aid_ptr->aid.data_ptr,
                  sizeof(aid_ptr->aid.data_ptr),
                  (void*)tlv_data.aid_value,
                  (uint32)aid_ptr->aid.data_len);
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_aid */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_PIN()

  DESCRIPTION
    Parses the TLV item of encrypted PIN and decodes it.

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    decrypted_pin_ptr           : output with encrypted PIN

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_decrypted_pin
(
  dsm_item_type **                        sdu_in,
  uint16                                  len,
  qmi_uim_tlv_decrypted_pin_type        * decrypted_pin_ptr
)
{
  int32             temp          = 0;
  uint8             decrypted_len = 0;
  uint8           * pin_value_ptr = NULL;
  qmi_error_e_type  qmi_err       = QMI_ERR_NONE;

  PACKED struct PACKED_POST
  {
    uint8  pin_len;
  } tlv_data;

  ASSERT(sdu_in && decrypted_pin_ptr);

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_ENCRYPTED_PIN)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract length */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.pin_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.pin_len == 0)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract PIN data */
  pin_value_ptr = (uint8*)uimqmi_malloc(tlv_data.pin_len);
  if (pin_value_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  temp = dsm_pullup(sdu_in, (void*)pin_value_ptr, tlv_data.pin_len);
  if (temp != tlv_data.pin_len)
  {
    qmi_err = QMI_ERR_MALFORMED_MSG;
    goto return_decrypt_pin;
  }

  memset(decrypted_pin_ptr, 0x00, sizeof(qmi_uim_tlv_decrypted_pin_type));

  decrypted_len = sizeof(decrypted_pin_ptr->decrypted_pin_value);

  /* Decrypt the PIN and update the TLV */
  if (!qmi_uim_decrypt_pin(tlv_data.pin_len,
                           pin_value_ptr,
                           (uint8*)&decrypted_len,
                           decrypted_pin_ptr->decrypted_pin_value))
  {
    qmi_err = QMI_ERR_INCORRECT_PIN;
    goto return_decrypt_pin;
  }

  decrypted_pin_ptr->decrypted_pin_data.data_len = (mmgsdi_len_type)decrypted_len;

  /* Check decrypted pin length */
  if (decrypted_pin_ptr->decrypted_pin_data.data_len == 0)
  {
    qmi_err = QMI_ERR_MISSING_ARG;
    goto return_decrypt_pin;
  }
  if (decrypted_pin_ptr->decrypted_pin_data.data_len > QMI_UIM_PIN_MAX_V01)
  {
    qmi_err = QMI_ERR_ARG_TOO_LONG;
    goto return_decrypt_pin;
  }

  decrypted_pin_ptr->decrypted_pin_data.data_ptr = decrypted_pin_ptr->decrypted_pin_value;
  qmi_err = QMI_ERR_NONE;

return_decrypt_pin:
  uimqmi_free(pin_value_ptr);
  return qmi_err;
} /* qmi_uim_tlv_decrypted_pin */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PIN_KEYREF_ID()

  DESCRIPTION
    Parses the TLV item of key reference id

  PARAMETERS
    sdu_in                 : input sdu
    len                    : length of the TLV item
    set_pin_protection_ptr : output with key reference id value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_pin_keyref_id
(
  dsm_item_type **                      sdu_in,
  uint16                                len,
  qmi_uim_tlv_pin_keyref_id_type      * pin_keyref_id_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  keyref_id;
  } tlv_data;

  ASSERT(sdu_in && pin_keyref_id_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if(tlv_data.keyref_id > UIM_PIN_KEY_REF_APPL_8_V01)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Key reference id */
  pin_keyref_id_ptr->keyref_id = (uim_pin_key_ref_enum_v01)tlv_data.keyref_id;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_pin_keyref_id */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_UINT32()

  DESCRIPTION
    Parses the uint32 TLV

  PARAMETERS
    sdu_in                 : input sdu
    len                    : length of the TLV item
    uint32_value_ptr       : value of uint32

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_uint32
(
  dsm_item_type **                          sdu_in,
  uint16                                    len,
  qmi_uim_tlv_uint32_type                 * uint32_value_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  value;
  } tlv_data;

  ASSERT(sdu_in && uint32_value_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Update token */
  uint32_value_ptr->value    = tlv_data.value;

  UIM_MSG_HIGH_1("qmi_uim_tlv_uint32: parsed value: 0x%x",
                 uint32_value_ptr->value);

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_uint32 */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_UINT8()

  DESCRIPTION
    Parses the uint8 TLV

  PARAMETERS
    sdu_in                 : input sdu
    len                    : length of the TLV item
    uint8_value_ptr        : value of uint8

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_uint8
(
  dsm_item_type **                          sdu_in,
  uint16                                    len,
  qmi_uim_tlv_uint8_type                  * uint8_value_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  value;
  } tlv_data;

  ASSERT(sdu_in && uint8_value_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Update token */
  uint8_value_ptr->value    = tlv_data.value;

  UIM_MSG_HIGH_1("qmi_uim_tlv_uint8: parsed value: 0x%x",
                 uint8_value_ptr->value);

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_uint8 */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_REGISTER_REFRESH()

  DESCRIPTION
    Parses the TLV item of register for refresh

  PARAMETERS
    sdu_in               : input sdu
    len                  : length of the TLV item
    register_refresh_ptr : output with register for refresh value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_register_refresh
(
  dsm_item_type **                    sdu_in,
  uint16                              len,
  qmi_uim_tlv_register_refresh_type * register_refresh_ptr
)
{
  int32                     temp               = 0;
  uint16                    i                  = 0;
  uint8*                    file_list_data     = NULL;
  uint8*                    current            = NULL;
  uint16                    file_list_size     = 0;
  uint16                    file_count         = 0;
  qmi_error_e_type          ret_value          = QMI_ERR_NONE;
  qmi_uim_files_list_type * last_file_node_ptr = NULL;
  qmi_uim_files_list_type * new_file_node_ptr  = NULL;

  PACKED struct PACKED_POST
  {
    uint8  register_op;
    uint8  vote_for_init;
    uint16 num_files;
  } tlv_data;

  PACKED struct PACKED_POST
  {
    uint16 file_id;
    uint8  path_len;
  } tlv_data_file_id;

  ASSERT(sdu_in && register_refresh_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Register or deregister */
  switch (tlv_data.register_op)
  {
    case 0:
      register_refresh_ptr->register_op = FALSE;
      break;
    case 1:
      register_refresh_ptr->register_op = TRUE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  /* Vote for init */
  switch (tlv_data.vote_for_init)
  {
    case 0:
      register_refresh_ptr->vote_for_init = FALSE;
      break;
    case 1:
      register_refresh_ptr->vote_for_init = TRUE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  /* Initialize the refresh list. */
  register_refresh_ptr->file_list_ptr = NULL;

  /* Basic check on size... we cannot check the size right now,
     as each file might have a different path length. Continue with
     each file is at least sizeof(uint16) */
  if ( len < sizeof(tlv_data) + (tlv_data.num_files * sizeof(uint16)) )
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* The case with zero files in the refresh list is
     perfectly valid: we can immediately return */
  if (tlv_data.num_files == 0)
  {
    return QMI_ERR_NONE;
  }

  /* Allocate memory */
  file_list_size = len - sizeof(tlv_data);
  file_list_data =
    (uint8*)uimqmi_malloc(file_list_size);
  if (file_list_data == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Extract file list from payload */
  temp = dsm_pullup(sdu_in, (void*)file_list_data, len - sizeof(tlv_data));
  if (temp != len - sizeof(tlv_data))
  {
    uimqmi_free(file_list_data);
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Parse each file */
  current = file_list_data;
  for (i = 0; i < tlv_data.num_files; i++)
  {
    mmgsdi_access_type  file_access;

    memset(&file_access, 0, sizeof(mmgsdi_access_type));
    file_access.access_method = MMGSDI_BY_PATH_ACCESS;

    /* Check size */
    if (current + sizeof(tlv_data_file_id) > file_list_data + file_list_size)
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }

    /* Extract file id and size */
    (void)memscpy(&tlv_data_file_id, sizeof(tlv_data_file_id),
                  current, sizeof(tlv_data_file_id));
    current += sizeof(tlv_data_file_id);

    /* Check size for path */
    if ( (current + tlv_data_file_id.path_len > file_list_data + file_list_size) ||
         ((tlv_data_file_id.path_len & 0x01) != 0) )
    {
      ret_value = QMI_ERR_MALFORMED_MSG;
      break;
    }
    if ( tlv_data_file_id.path_len > (MMGSDI_MAX_PATH_LEN - 1) * sizeof(uint16) )
    {
      ret_value = QMI_ERR_ARG_TOO_LONG;
      break;
    }

    /* Path length (in numbers of uint16) and value */
    file_access.file.path_type.path_len = tlv_data_file_id.path_len / sizeof(uint16);
    (void)memscpy(file_access.file.path_type.path_buf, sizeof(file_access.file.path_type.path_buf),
                  current, tlv_data_file_id.path_len);
    current += tlv_data_file_id.path_len;

    /* Add file id to the path */
    file_access.file.path_type.path_buf[file_access.file.path_type.path_len] =
      tlv_data_file_id.file_id;
    file_access.file.path_type.path_len += 1;

    /* Allocate node in the linked list */
    new_file_node_ptr =
      (qmi_uim_files_list_type*)uimqmi_malloc(sizeof(qmi_uim_files_list_type));
    if (new_file_node_ptr == NULL)
    {
      continue;
    }

    /* Copy file details into the new note */
    new_file_node_ptr->next_ptr = NULL;
    (void)memscpy((void*)&new_file_node_ptr->file,
                  sizeof(mmgsdi_access_type),
                  (void*)&file_access,
                  sizeof(mmgsdi_access_type) );

    /* Append new node to the end of the list */
    if (last_file_node_ptr == NULL)
    {
      register_refresh_ptr->file_list_ptr = new_file_node_ptr;
    }
    else
    {
      last_file_node_ptr->next_ptr = new_file_node_ptr;
    }
    last_file_node_ptr = new_file_node_ptr;

    file_count++;
  }

  UIM_MSG_HIGH_1("QMI_UIM files in the refresh list: 0x%x", file_count);

  /* Free temporary memory */
  uimqmi_free(file_list_data);

  return ret_value;
} /* qmi_uim_tlv_register_refresh */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_FILE_CONTROL_INFO()

  DESCRIPTION
    Parses the TLV item of file control information request type

  PARAMETERS
    sdu_in      : input sdu
    len         : length of the TLV item
    sub_ok_ptr  : output with ok file control information request type value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_file_control_info
(
  dsm_item_type **                      sdu_in,
  uint16                                len,
  qmi_uim_tlv_file_control_info_type  * fci_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8 fci;
  } tlv_data;

  ASSERT(sdu_in && fci_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract file control information requested */
  switch (tlv_data.fci)
  {
    case UIM_FILE_CONTROL_INFORMATION_NO_DATA_V01:
      fci_ptr->fci_type = MMGSDI_SELECT_RSP_NO_DATA;
      break;
    case UIM_FILE_CONTROL_INFORMATION_FCP_V01:
      fci_ptr->fci_type = MMGSDI_SELECT_RSP_FCP;
      break;
    case UIM_FILE_CONTROL_INFORMATION_FCI_V01:
      fci_ptr->fci_type = MMGSDI_SELECT_RSP_FCI;
      break;
    case UIM_FILE_CONTROL_INFORMATION_FCI_WITH_INTERFACES_V01:
      fci_ptr->fci_type = MMGSDI_SELECT_RSP_FCI_WITH_INTERFACES;
      break;
    case UIM_FILE_CONTROL_INFORMATION_FMD_V01:
      fci_ptr->fci_type = MMGSDI_SELECT_RSP_FMD;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }
  return QMI_ERR_NONE;
}/* qmi_uim_tlv_file_control_info */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_IMSI()

  DESCRIPTION
    Parses the TLV item for encrypted IMSI and decodes the IMSI

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decrypted_imsi_ptr        : output decoded IMSI

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_decrypted_imsi
(
  dsm_item_type **                         sdu_in,
  uint16                                   len,
  qmi_uim_tlv_decrypted_imsi_type *        decrypted_imsi_ptr
)
{
  int32             temp                = 0;
  uint32            decrypted_imsi_len  = 0;

  PACKED struct PACKED_POST
  {
    uint8  length;
    uint8  encrypted_imsi_data[QMI_UIM_ENCRYPTED_IMSI_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && decrypted_imsi_ptr);

  /* Check length */
  if (len < sizeof(tlv_data.length) || len > sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data and ensure it is formed properly */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len ||
      (len != (sizeof(tlv_data.length) + tlv_data.length)) ||
      tlv_data.length == 0)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  decrypted_imsi_len = (uint32)sizeof(decrypted_imsi_ptr->imsi_data.imsi);

  /* check error result */
  if (!qmi_uim_decrypt_payload(tlv_data.encrypted_imsi_data,
                               tlv_data.length,
                               decrypted_imsi_ptr->imsi_data.imsi,
                               &decrypted_imsi_len))
  {
    return QMI_ERR_INTERNAL;
  }

  if (decrypted_imsi_len > QMI_UIM_MAX_IMSI_LEN)
  {
    return QMI_ERR_INTERNAL;
  }

  decrypted_imsi_ptr->imsi_data.len = (uint8)decrypted_imsi_len;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_decrypted_imsi */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_GET_CONFIG_MASK()

  DESCRIPTION
    Parses the TLV item get config mask

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    get_config_mask_ptr       : output config mask

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_get_config_mask
(
  dsm_item_type                          ** sdu_in,
  uint16                                    len,
  qmi_uim_tlv_get_configuration_mask_type * get_config_mask_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  config_mask;
  } tlv_data;

  ASSERT(sdu_in && get_config_mask_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  get_config_mask_ptr->automatic_selection      =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_AUTOMATIC_SELECTION_V01) ? TRUE : FALSE;
  get_config_mask_ptr->personalization_status   =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_PERSONALIZATION_STATUS_V01) ? TRUE : FALSE;
  get_config_mask_ptr->halt_subscription_status =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_HALT_SUBSCRIPTION_V01) ? TRUE: FALSE;
  get_config_mask_ptr->usb_uicc_supported_status =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_USB_UICC_SUPPORTED_V01) ? TRUE: FALSE;
  get_config_mask_ptr->sap_client_supported_status =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_SAP_CLIENT_SUPPORTED_V01) ? TRUE: FALSE;
  get_config_mask_ptr->perso_reactivation_status =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_PERSO_REACTIVATION_STATUS_V01) ? TRUE: FALSE;
  get_config_mask_ptr->remote_simlock_storage =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_REMOTE_SIMLOCK_STORAGE_V01) ? TRUE: FALSE;
  get_config_mask_ptr->emergency_only_mode =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_EMERGENCY_ONLY_V01) ? TRUE: FALSE;
  get_config_mask_ptr->extended_apdu =
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_EXTENDED_APDU_V01) ? TRUE: FALSE;
  get_config_mask_ptr->inactive_simlock_config=
           (tlv_data.config_mask & UIM_GET_CONFIGURATION_INACTIVE_SIMLOCK_CONFIG_V01) ? TRUE: FALSE;           

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_get_config_mask */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_FILE_STATUS()

  DESCRIPTION
    Parses the TLV item of set file status

  PARAMETERS
    sdu_in          : input sdu
    len             : length of the TLV item
    file_status_ptr : output with file status value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_file_status
(
  dsm_item_type **               sdu_in,
  uint16                         len,
  qmi_uim_tlv_file_status_type * file_status_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  file_status;
  } tlv_data;

  ASSERT(sdu_in && file_status_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if(tlv_data.file_status > UIM_FILE_STATUS_ACTIVATED_V01)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* File status */
  file_status_ptr->file_status = (uim_file_status_type_v01)tlv_data.file_status;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_file_status */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_INCREASE()

  DESCRIPTION
    Parses the TLV item of increase

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    increase_ptr          : output with increase value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    Allocates memory that needs to be freed by the caller

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_increase
(
  dsm_item_type **                     sdu_in,
  uint16                               len,
  qmi_uim_tlv_increase_type          * increase_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8 length;
  } tlv_data;

  ASSERT(sdu_in && increase_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Check data length */
  if (len == 0 ||
      len != sizeof(tlv_data) + tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Length */
  increase_ptr->data.data_len = (mmgsdi_len_type)tlv_data.length;

  /* Extract body to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  increase_ptr->data.data_ptr =
        (uint8*)uimqmi_malloc(tlv_data.length);
  if (increase_ptr->data.data_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  temp = dsm_pullup(sdu_in,
                    (void*)increase_ptr->data.data_ptr,
                    tlv_data.length);
  if (temp != tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_increase */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SELECT_MODE()

  DESCRIPTION
    Parses the TLV item for mode of selection

  PARAMETERS
    sdu_in          : input sdu
    len             : length of the TLV item
    select_mode_ptr : output with selection mode value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_select_mode
(
  dsm_item_type **               sdu_in,
  uint16                         len,
  qmi_uim_tlv_select_mode_type * select_mode_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  select_mode;
  } tlv_data;

  ASSERT(sdu_in && select_mode_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Card status */
  switch(tlv_data.select_mode)
  {
    case UIM_SELECT_MODE_FIRST_OR_ONLY_V01:
      select_mode_ptr->mode = MMGSDI_SELECT_MODE_FIRST_OR_ONLY;
      break;
    case UIM_SELECT_MODE_LAST_V01:
      select_mode_ptr->mode = MMGSDI_SELECT_MODE_LAST;
      break;
    case UIM_SELECT_MODE_NEXT_V01:
      select_mode_ptr->mode = MMGSDI_SELECT_MODE_NEXT;
      break;
    case UIM_SELECT_MODE_PREVIOUS_V01:
      select_mode_ptr->mode = MMGSDI_SELECT_MODE_PREVIOUS;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_select_mode */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_STATUS_CMD_MODE()

  DESCRIPTION
    Parses the TLV item for status cmd mode

  PARAMETERS
    sdu_in          : input sdu
    len             : length of the TLV item
    select_mode_ptr : output with selection mode value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_status_cmd_mode
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_status_cmd_mode_type * status_cmd_mode_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32 status_cmd_mode;
  } tlv_data;

  ASSERT(sdu_in && status_cmd_mode_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Card status */
  switch(tlv_data.status_cmd_mode)
  {
    case UIM_STATUS_CMD_NO_INDICATION_V01:
      status_cmd_mode_ptr->mode = MMGSDI_STATUS_APP_NO_INDICATION;
      break;
    case UIM_STATUS_CMD_APP_INITIALIZED_V01:
      status_cmd_mode_ptr->mode = MMGSDI_STATUS_APP_INITIALIZED;
      break;
    case UIM_STATUS_CMD_WILL_TERMNATE_APP_V01:
      status_cmd_mode_ptr->mode = MMGSDI_STATUS_APP_TERMINATED;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_status_cmd_mode */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_STATUS_CMD_RESP()

  DESCRIPTION
    Parses the TLV item for status cmd responce

  PARAMETERS
    sdu_in          : input sdu
    len             : length of the TLV item
    select_mode_ptr : output with selection mode value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_status_cmd_resp
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_status_cmd_resp_type * status_cmd_resp_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32 status_cmd_resp;
  } tlv_data;

  ASSERT(sdu_in && status_cmd_resp_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Card status */
  switch(tlv_data.status_cmd_resp)
  {
    case UIM_STATUS_CMD_FCP_RESP_V01:
      status_cmd_resp_ptr->resp = MMGSDI_STATUS_DATA_SELECT;
      break;
    case UIM_STATUS_CMD_AID_RESP_V01:
      status_cmd_resp_ptr->resp = MMGSDI_STATUS_TLV_DATA;
      break;
    case UIM_STATUS_CMD_NO_DATA_RESP_V01:
      status_cmd_resp_ptr->resp = MMGSDI_STATUS_NO_DATA;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_status_cmd_resp */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SERVICE_NUMBER()

  DESCRIPTION
    Parses the TLV item of service number

  PARAMETERS
    sdu_in              : input sdu
    len                 : length of the TLV item
    service_number_ptr  : output with service number value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_service_number
(
  dsm_item_type                      ** sdu_in,
  uint16                                len,
  qmi_uim_tlv_service_number_type     * service_number_ptr
)
{
  int32 temp;

  PACKED struct PACKED_POST
  {
    uint8 service_number;
  } tlv_data;

  ASSERT(sdu_in && service_number_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Copy over service number */
  service_number_ptr->service_number = tlv_data.service_number;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_service_number */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SEARCH_PATTERN()

  DESCRIPTION
    Parses the pattern TLV item of search_pattern

  PARAMETERS
    sdu_in                   : input sdu
    len                      : length of the TLV item
    search_pattern_ptr       : output with search pattern value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_search_pattern
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_search_pattern_type   *search_pattern_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  pattern_length;
  } tlv_data;

  ASSERT(sdu_in && search_pattern_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.pattern_length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Length */
  search_pattern_ptr->pattern.data_len = (mmgsdi_len_type)tlv_data.pattern_length;

  /* Extract body to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  search_pattern_ptr->pattern.data_ptr =
        (uint8*)uimqmi_malloc(tlv_data.pattern_length);
  if (search_pattern_ptr->pattern.data_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  temp = dsm_pullup(sdu_in,
                    (void*)search_pattern_ptr->pattern.data_ptr,
                    tlv_data.pattern_length);
  if (temp != tlv_data.pattern_length)
  {
    uimqmi_free(search_pattern_ptr->pattern.data_ptr);
    return QMI_ERR_MALFORMED_MSG;
  }

  return QMI_ERR_NONE;
}/* qmi_uim_tlv_search_pattern */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SEARCH_DATA()

  DESCRIPTION
    Parses the Search Data TLV item

  PARAMETERS
    sdu_in                   : input sdu
    len                      : length of the TLV item
    search_data_ptr          : output with direction and first record value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_search_data
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_search_data_type      *search_data_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32 search_type;
    uint8  first_record;
  } tlv_data;

  ASSERT(sdu_in && search_data_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Record */
  search_data_ptr->first_record_num = (mmgsdi_rec_num_type)tlv_data.first_record;

  /* Search Type */
  switch (tlv_data.search_type)
  {
    case 0:
      search_data_ptr->direction = MMGSDI_SEARCH_FORWARD_FROM_REC_NUM;
      break;
    case 1:
      search_data_ptr->direction = MMGSDI_SEARCH_BACKWARD_FROM_REC_NUM;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
}/* qmi_uim_tlv_search_data */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_TEMPORARY_IDENTITY_MASK()

  DESCRIPTION
    Parses the TLV item of temporary identity mask

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    temporary_identity_mask_ptr : output with temporary identity mask value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_temporary_identity_mask
(
  dsm_item_type **                           sdu_in,
  uint16                                     len,
  qmi_uim_tlv_temporary_identity_mask_type * temporary_identity_mask_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  temporary_identity_mask;
  } tlv_data;

  ASSERT(sdu_in && temporary_identity_mask_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Set all flags to false by default */
  memset(temporary_identity_mask_ptr, 0, sizeof(qmi_uim_tlv_temporary_identity_mask_type));

  /* temporary identity mask */
  if ((tlv_data.temporary_identity_mask & UIM_PURGE_TEMPORARY_IDENTITY_REQ_GUTI_MASK_V01) != 0)
  {
    temporary_identity_mask_ptr->guti = TRUE;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_temporary_identity_mask */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SWITCH_SLOT_EXT()

  DESCRIPTION
    Parses the TLV item of physical to logical slot switch request

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    imsi_data_ptr     : output with logical to physical slot mapping

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_switch_slot_ext
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_switch_slot_ext_type           *  switch_slot_ext_ptr
)
{
  int32            temp      = 0;
  uint8            i         = 0;

  PACKED struct PACKED_POST
  {
    uint8  length;
    uint32 physical_slot[QMI_UIM_EXTENDED_CARDS_MAX_V01];
  } tlv_data;

  ASSERT(sdu_in && switch_slot_ext_ptr);

  memset(switch_slot_ext_ptr, 0, sizeof(qmi_uim_tlv_switch_slot_ext_type));

  /* Check length */
  if (len < QMI_UIM_TLV_MIN_LEN_SWITCH_SLOT_EXT ||
      len > sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data and ensure it is formed properly */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.length == 0 ||
      tlv_data.length > QMI_UIM_EXTENDED_CARDS_MAX_V01 ||
      len != (sizeof(tlv_data.length) + (tlv_data.length * sizeof(tlv_data.physical_slot[0]))))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract logical slot to physical slot mapping */
  for (i = 0; i < tlv_data.length &&
              i < QMI_UIM_EXTENDED_CARDS_MAX_V01 &&
              i < QMI_UIM_MAX_CARD_COUNT; i++)
  {
    /* Physical Slot */
    switch(tlv_data.physical_slot[i])
    {
      case UIM_PHY_SLOT_1_V01:
        switch_slot_ext_ptr->logical_slot[i] = UIM_SLOT_1;
        break;
      case UIM_PHY_SLOT_2_V01:
        switch_slot_ext_ptr->logical_slot[i] = UIM_SLOT_2;
        break;
      case UIM_PHY_SLOT_3_V01:
        switch_slot_ext_ptr->logical_slot[i] = UIM_SLOT_3;
        break;
      case UIM_PHY_SLOT_4_V01:
      case UIM_PHY_SLOT_5_V01:
        return QMI_ERR_NOT_SUPPORTED;
      default:
        return QMI_ERR_INVALID_ARG;
    }
  }

  switch_slot_ext_ptr->logical_slot_len = tlv_data.length;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_switch_slot_ext */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_TERMINAL_CAPABILITY()

  DESCRIPTION
    Parses the TLV item of terminal capability

  PARAMETERS
    sdu_in                  : input sdu
    len                     : length of the TLV item
    terminal_capability_ptr : output with terminal capability TLV values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_terminal_capability
(
  dsm_item_type **                           sdu_in,
  uint16                                     len,
  qmi_uim_tlv_terminal_capability_tlv_type * terminal_capability_ptr
)
{
  boolean           *tc_tags_map_ptr  = NULL;
  qmi_error_e_type   qmi_error        = QMI_ERR_NONE;
  int32              temp             = 0;
  uint8              index            = 0;

  PACKED struct PACKED_POST
  {
    uint8             num_of_tlvs;
  } tlv_data_num_of_tlvs;

  PACKED struct PACKED_POST
  {
    uint8           tlv_tag;
    uint8           tlv_len;
  } tlv_data_tag_len;

  PACKED struct PACKED_POST
  {
    uint8            remove_tlv;
  } tlv_data_remove_tlv;

  ASSERT(sdu_in && terminal_capability_ptr);

  /* Check length */
  if(len < sizeof(tlv_data_num_of_tlvs))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  memset(&tlv_data_num_of_tlvs, 0x00, sizeof(tlv_data_num_of_tlvs));
  memset(&tlv_data_tag_len, 0x00, sizeof(tlv_data_tag_len));
  memset(&tlv_data_remove_tlv, 0x00, sizeof(tlv_data_remove_tlv));

  temp = dsm_pullup(sdu_in,
                    (void*)&tlv_data_num_of_tlvs,
                    sizeof(tlv_data_num_of_tlvs));
  if(temp != sizeof(tlv_data_num_of_tlvs))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if(tlv_data_num_of_tlvs.num_of_tlvs > QMI_UIM_TERMINAL_CAPABILITY_TLVS_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  terminal_capability_ptr->num_tlvs = tlv_data_num_of_tlvs.num_of_tlvs;

  if(terminal_capability_ptr->num_tlvs == 0)
  {
    return QMI_ERR_NONE;
  }

  terminal_capability_ptr->tlv_ptr =
    (qmi_uim_terminal_capability_tlv_type*) uimqmi_malloc(terminal_capability_ptr->num_tlvs * sizeof(qmi_uim_terminal_capability_tlv_type));
  if(terminal_capability_ptr->tlv_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* A tag is a uint8, that is, its max value can be 255. Hence, it is safe to
     use a map (where each index corresponds to a tag) 256 bytes long. */
  tc_tags_map_ptr = uimqmi_malloc(256 * sizeof(boolean));
  if(tc_tags_map_ptr == NULL)
  {
    /* The pointer terminal_capability_ptr->tlv_ptr malloc'd earlier will
       be freed anyways inside qmi_util_free_tlv_content(). So, no need to
       worry about free'ing it here */
    return QMI_ERR_NO_MEMORY;
  }

  /* Terminal Capability */
  for (index = 0; index < terminal_capability_ptr->num_tlvs; index++)
  {
    /* Pull the tag and length of TLV in one shot */
    temp = dsm_pullup(sdu_in,
                      (void*)&tlv_data_tag_len,
                      sizeof(tlv_data_tag_len));

    if (temp != sizeof(tlv_data_tag_len) ||
        tlv_data_tag_len.tlv_len > QMI_UIM_TERMINAL_CAPABILITY_VALUE_MAX_V01)
    {
      qmi_error = QMI_ERR_MALFORMED_MSG;
      break;
    }

    if(tc_tags_map_ptr[tlv_data_tag_len.tlv_tag])
    {
      UIM_MSG_ERR_1("Multiple entries found for tag: 0x%x", tlv_data_tag_len.tlv_tag);
      qmi_error = QMI_ERR_INVALID_ARG;
      break;
    }
    terminal_capability_ptr->tlv_ptr[index].tag = tlv_data_tag_len.tlv_tag;
    tc_tags_map_ptr[tlv_data_tag_len.tlv_tag] = TRUE;
    terminal_capability_ptr->tlv_ptr[index].value_len = tlv_data_tag_len.tlv_len;

    /* Pull the value part of the TLV as per the value_len retrieved
       during previous pull */
    if(terminal_capability_ptr->tlv_ptr[index].value_len > 0)
    {
      temp = dsm_pullup(sdu_in,
                        (void*)terminal_capability_ptr->tlv_ptr[index].value,
                        terminal_capability_ptr->tlv_ptr[index].value_len);
      if (temp != terminal_capability_ptr->tlv_ptr[index].value_len)
      {
        /* No need to free the allocated tlv_ptr here as it is done inside
           the qmi_uim_util_free_tlv_content function in the caller */
        qmi_error = QMI_ERR_MALFORMED_MSG;
        break;
      }
    }

    /* Pull the byte indicating whether to remove this TLV from the terminal
       capability altogether or not */
    temp = dsm_pullup(sdu_in,
                      (void*)&tlv_data_remove_tlv,
                      sizeof(tlv_data_remove_tlv));
    if (temp != sizeof(tlv_data_remove_tlv) ||
        (tlv_data_remove_tlv.remove_tlv != 0x00 &&
         tlv_data_remove_tlv.remove_tlv != 0x01))
    {
      qmi_error = QMI_ERR_MALFORMED_MSG;
      break;
    }
    terminal_capability_ptr->tlv_ptr[index].remove_tlv = (tlv_data_remove_tlv.remove_tlv == 0x01);
  }

  uimqmi_free(tc_tags_map_ptr);
  tc_tags_map_ptr = NULL;

  return qmi_error;
} /* qmi_uim_tlv_terminal_capability */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_APDU_BEHAVIOR()

  DESCRIPTION
    Parses the TLV item of APDU behavior

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    apdu_behavior_ptr : output with APDU behavior value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_apdu_behavior
(
  dsm_item_type **                    sdu_in,
  uint16                              len,
  qmi_uim_tlv_apdu_behavior_type    * apdu_behavior_ptr
)
{
  int32             temp          = 0;
  qmi_error_e_type  qmi_err       = QMI_ERR_NONE;

  PACKED struct PACKED_POST
  {
    uint32  apdu_behavior;
  } tlv_data;

  ASSERT(sdu_in && apdu_behavior_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Derive APDU behavior */
  switch (tlv_data.apdu_behavior)
  {
    case UIM_APDU_BEHAVIOR_NO_GET_RESPONSE_FOR_WARNING_V01:
      apdu_behavior_ptr->apdu_behavior = MMGSDI_APDU_BEHAVIOR_NO_GET_RESPONSE_FOR_WARNING;
      break;

    case UIM_APDU_BEHAVIOR_AUTOMATIC_GET_RESPONSE_FOR_WARNING_V01:
      apdu_behavior_ptr->apdu_behavior = MMGSDI_APDU_BEHAVIOR_AUTOMATIC_GET_RESPONSE_FOR_WARNING;
      break;

    default:
      qmi_err = QMI_ERR_INVALID_ARG;
      break;
  }

  return qmi_err;
} /* qmi_uim_tlv_apdu_behavior */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_CARD_MODE()

  DESCRIPTION
    Parses the TLV item of Card mode.

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    card_mode_ptr     : output with parsed values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_card_mode
(
  dsm_item_type **                 sdu_in,
  uint16                           len,
  qmi_uim_tlv_card_mode_type     * card_mode_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  card_pup_value;
  } tlv_data;

  ASSERT(sdu_in && card_mode_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  switch (tlv_data.card_pup_value)
  {
    case UIM_CARD_MODE_TELECOM_CARD_V01:
      card_mode_ptr->card_pup_value = MMGSDI_CARD_POWER_UP_INITIAL_PUP;
      break;
    case UIM_CARD_MODE_NON_TELECOM_CARD_V01:
      card_mode_ptr->card_pup_value = MMGSDI_CARD_POWER_UP_NON_TELECOM;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_card_mode */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_LONG_APDU_REQUEST()

  DESCRIPTION
    Parses the TLV item of long request in SEND_APDU

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    long_request_ptr  : output with long request value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_tlv_long_apdu_request
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_long_apdu_request_type           *long_request_ptr
)
{
  int32            temp      = 0;
  uint8            i         = 0;

  PACKED struct PACKED_POST
  {
    uint16        total_length;
    uint32        token;
    uint16        offset;
  } tlv_data;

  ASSERT(sdu_in && long_request_ptr);

  memset(long_request_ptr, 0, sizeof(qmi_uim_tlv_long_apdu_request_type));

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data and ensure it is formed properly */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  long_request_ptr->total_length = tlv_data.total_length;
  long_request_ptr->token = tlv_data.token;
  long_request_ptr->offset = tlv_data.offset;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_long_apdu_request */


/*===========================================================================
  FUNCTION QMI_UIM_PARSE_REQUEST()

  DESCRIPTION
    Parses the TLV content of QMI_UIM message

  PARAMETERS
    sdu_in          : input sdu
    parsed_data_ptr : output with parsed values
    count           : number of values to be parsed

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_parse_request
(
  dsm_item_type **        sdu_in,
  qmi_uim_tlv_item_type * parsed_data_ptr,
  uint16                  count
)
{
  uint16            index  = 0;
  qmi_error_e_type  result = QMI_ERR_NONE;

  ASSERT(sdu_in && parsed_data_ptr && count > 0);

  while (*sdu_in)
  {
    uint8            type             = 0;
    uint16           len              = 0;
    boolean          parsing_required = FALSE;

    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    /* Check if tag is required (mandatory or optional) */
    for (index = 0; index < count; index++)
    {
      if (parsed_data_ptr[index].tag == type &&
          parsed_data_ptr[index].present == FALSE)
      {
        parsing_required = TRUE;
        break;
      }
    }

    if (!parsing_required)
    {
      /* We need to pullout this item */
      if (len != dsm_pullup(sdu_in, NULL, len))
      {
        result = QMI_ERR_MALFORMED_MSG;
        break;
      }
      continue;
    }

    switch(parsed_data_ptr[index].type)
    {
      case UIMI_TLV_TYPE_SESSION_INFORMATION:
        result = qmi_uim_tlv_session_information(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.session_information));
        break;

      case UIMI_TLV_TYPE_FILE_ID:
        result = qmi_uim_tlv_file_id(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.file_id));
        break;

      case UIMI_TLV_TYPE_READ_TRANSPARENT:
        result = qmi_uim_tlv_read_transparent(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.read_transparent));
        break;

      case UIMI_TLV_TYPE_READ_RECORD:
        result = qmi_uim_tlv_read_record(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.read_record));
        break;

      case UIMI_TLV_TYPE_WRITE_TRANSPARENT:
        result = qmi_uim_tlv_write_transparent(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.write_transparent));
        break;

      case UIMI_TLV_TYPE_WRITE_RECORD:
        result = qmi_uim_tlv_write_record(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.write_record));
        break;

      case UIMI_TLV_TYPE_REGISTER_REFRESH:
        result = qmi_uim_tlv_register_refresh(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.register_refresh));
        break;

      case UIMI_TLV_TYPE_OK_FOR_REFRESH:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.ok_to_refresh));
        break;

      case UIMI_TLV_TYPE_REFRESH_COMPLETE:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.refresh_complete));
        break;

      case UIMI_TLV_TYPE_SET_PIN_PROTECTION:
        result = qmi_uim_tlv_set_pin_protection(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.set_pin_protection));
        break;

      case UIMI_TLV_TYPE_VERIFY_PIN:
        result = qmi_uim_tlv_verify_pin(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.verify_pin));
        break;

      case UIMI_TLV_TYPE_UNBLOCK_PIN:
        result = qmi_uim_tlv_unblock_pin(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.unblock_pin));
        break;

      case UIMI_TLV_TYPE_CHANGE_PIN:
        result = qmi_uim_tlv_change_pin(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.change_pin));
        break;

      case UIMI_TLV_TYPE_SLOT:
        result = qmi_uim_tlv_slot(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.slot));
        break;

      case UIMI_TLV_TYPE_EVENT_REG:
        result = qmi_uim_tlv_event_reg(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.event_reg));
        break;

      case UIMI_TLV_TYPE_AUTHENTICATE:
        result = qmi_uim_tlv_authenticate(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.authenticate));
        break;

      case UIMI_TLV_TYPE_CAPABILITIES_MASK:
        result = qmi_uim_tlv_capabilities_mask(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.capabilities_mask));
        break;

      case UIMI_TLV_TYPE_SET_FDN_STATUS:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.set_fdn_status));
        break;

      case UIMI_TLV_TYPE_APPLICATION_INFORMATION:
        result = qmi_uim_tlv_application_information(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.application_information));
        break;

      case UIMI_TLV_TYPE_SESSION_CHANGE:
        result = qmi_uim_tlv_session_change(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.session_change));
        break;

      case UIMI_TLV_TYPE_APDU_REQUEST:
        result = qmi_uim_tlv_apdu_request(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.send_apdu));
        break;

      case UIMI_TLV_TYPE_SAP_CONNECT:
        result = qmi_uim_tlv_sap_connect(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.sap_connection));
        break;

      case UIMI_TLV_TYPE_SAP_DISCONNECT_MODE:
        result = qmi_uim_tlv_sap_disconnect_mode(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.sap_disconnect_mode));
        break;

      case UIMI_TLV_TYPE_SAP_REQUEST:
        result = qmi_uim_tlv_sap_request(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.sap_request));
        break;

      case UIMI_TLV_TYPE_LOGICAL_CHANNEL:
        result = qmi_uim_tlv_channel_id(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.logical_channel));
        break;

      case UIMI_TLV_TYPE_AID:
        result = qmi_uim_tlv_aid(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.aid));
        break;

      case UIMI_TLV_TYPE_DECRYPTED_PIN:
        result = qmi_uim_tlv_decrypted_pin(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.decrypted_pin));
        break;

      case UIMI_TLV_TYPE_READ_RECORD_LAST:
        result = qmi_uim_tlv_read_record_last(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.read_record_last));
        break;

      case UIMI_TLV_TYPE_SUBSCRIPTION_OK:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.ok_for_subscription));
        break;

      case UIMI_TLV_TYPE_FILE_CONTROL_INFO:
        result = qmi_uim_tlv_file_control_info(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.file_control_info));
        break;

      case UIMI_TLV_TYPE_PIN_KEYREF_ID:
        result = qmi_uim_tlv_pin_keyref_id(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.pin_keyref_id));
        break;

      case UIMI_TLV_TYPE_IND_TOKEN:
        result = qmi_uim_tlv_uint32(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.ind_token));
        break;

      case UIMI_TLV_TYPE_IGNORE_HOTSWAP_SWITCH:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.ignore_hotswap_switch));
        break;

     case UIMI_TLV_TYPE_CARD_POWERUP_MODE:
        result = qmi_uim_tlv_card_mode(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.card_pup_option));
        break;

      case UIMI_TLV_TYPE_SAP_INTERMEDIATE_GET_RESP:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.sap_intermediate_get_resp));
        break;

      case UIMI_TLV_TYPE_ENCRYPTED:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.encrypted));
        break;

      case UIMI_TLV_TYPE_DECRYPTED_IMSI:
        result = qmi_uim_tlv_decrypted_imsi(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.decrypted_imsi));
        break;

      case UIMI_TLV_TYPE_CONFIGURATION_MASK:
        result = qmi_uim_tlv_get_config_mask(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.configuration_mask));
        break;

      case UIMI_TLV_TYPE_SKIP_PROCEDURE_BYTES:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.skip_procedure_bytes));
        break;

      case UIMI_TLV_TYPE_TERMINATE_APP:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.terminate_app));
        break;

      case UIMI_TLV_TYPE_REGISTER_FOR_REFRESH:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.refresh_registration));
        break;

      case UIMI_TLV_TYPE_REFRESH_VOTE_FOR_INIT:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.refresh_vote_for_init));
        break;

      case UIMI_TLV_TYPE_SAP_CONNECTION_CONDITION:
        result = qmi_uim_tlv_sap_connection_condition(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.sap_connection_condtion));
        break;

      case UIMI_TLV_TYPE_SET_ACL_STATUS:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.set_acl_status));
        break;

      case UIMI_TLV_TYPE_FILE_STATUS:
        result = qmi_uim_tlv_file_status(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.file_status));
        break;

      case UIMI_TLV_TYPE_EXTENDED_CARD_STATUS:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.extended_card_status));
        break;

      case UIMI_TLV_TYPE_PHYSICAL_SLOT:
        result = qmi_uim_tlv_physical_slot(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.physical_slot));
        break;

      case UIMI_TLV_TYPE_INCREASE:
        result = qmi_uim_tlv_increase(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.increase_data));
        break;

      case UIMI_TLV_TYPE_SELECT_MODE:
        result = qmi_uim_tlv_select_mode(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.select_mode));
        break;

      case UIMI_TLV_TYPE_STATUS_CMD_MODE:
        result = qmi_uim_tlv_status_cmd_mode(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.status_cmd_mode));
        break;

      case UIMI_TLV_TYPE_STATUS_CMD_RESP:
        result = qmi_uim_tlv_status_cmd_resp(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.status_cmd_resp));
        break;

      case UIMI_TLV_TYPE_SERVICE_NUMBER:
        result = qmi_uim_tlv_service_number(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.service_number));
        break;

#ifdef FEATURE_SIMLOCK
      case UIMI_TLV_TYPE_DEPERSONALIZATION:
        result = qmi_uim_tlv_depersonalization(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.depersonalization));
        break;

      case UIMI_TLV_TYPE_PERSONALIZATION:
        result = qmi_uim_tlv_personalization_ck(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.personalization_ck));
        break;

      case UIMI_TLV_TYPE_GW_NW_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_NW,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_NS_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_NS,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_SP_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_SP,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_SP_EXT_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_SP,
                    QMI_UIM_CATEGORY_TYPE_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_SP_EXT2_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_SP,
                    QMI_UIM_CATEGORY_TYPE_EXT2,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_CP_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_CP,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_CP_EXT_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_CP,
                    QMI_UIM_CATEGORY_TYPE_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_CP_EXT2_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_CP,
                    QMI_UIM_CATEGORY_TYPE_EXT2,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_SIM_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_SIM,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_SPN_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_SPN,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_ICCID_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_ICCID,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_GW_SP_EHPLMN_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP_SP_EHPLMN,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_1X_NW_TYPE1_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP2_NW_TYPE1,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_1X_NW_TYPE2_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP2_NW_TYPE2,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_1X_RUIM_PERSO_CODE:
        result = qmi_uim_tlv_personalization_code(
                    sdu_in,
                    len,
                    SIMLOCK_CATEGORY_3GPP2_RUIM,
                    QMI_UIM_CATEGORY_TYPE_NO_EXT,
                    &(parsed_data_ptr[index].data.personalization_code));
        break;

      case UIMI_TLV_TYPE_PERSO_ITERATION_CNT:
        result = qmi_uim_tlv_uint32(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.personalization_iteration_cnt));
        break;

      case UIMI_TLV_TYPE_PERSO_UNLOCK_RETRIES_CNT:
        result = qmi_uim_tlv_uint32(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.personalization_retries_cnt));
        break;

      case UIMI_TLV_TYPE_DECRYPTED_PERSO_SECURE:
        result = qmi_uim_tlv_decrypted_perso_secure(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.decrypted_perso_secure));
        break;

      case UIMI_TLV_TYPE_DECRYPTED_DEPERSO_SECURE:
        result = qmi_uim_tlv_decrypted_deperso_secure(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.decrypted_deperso_secure));
        break;

      case UIMI_TLV_TYPE_DECRYPTED_EMERGENCY_ONLY:
        result = qmi_uim_tlv_decrypted_emergency_only(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.decrypted_emergency_only));
        break;

      case UIMI_TLV_TYPE_DECRYPTED_SIMLOCK_CONFIG:
        result = qmi_uim_tlv_decrypted_simlock_config(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.decrypted_simlock_config));
        break;

      case UIMI_TLV_TYPE_REMOTE_UNLOCK_CONFIG:
      case UIMI_TLV_TYPE_REMOTE_UNLOCK_CONFIG_EXT:
        result = qmi_uim_tlv_remote_unlock_config(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.remote_unlock_config));
        break;
      case UIMI_TLV_TYPE_REMOTE_UNLOCK_OPERATION:
        result = qmi_uim_tlv_remote_unlock_operation(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.remote_unlock_operation));
        break;
      case UIMI_TLV_TYPE_IMSI:
        result = qmi_uim_tlv_imsi(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.imsi));
        break;

      case UIMI_TLV_TYPE_PERSO_REACTIVATE_FEATURE:
        result = qmi_uim_tlv_personalization_category(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.personalization_reactivate));
        break;

      case UIMI_TLV_TYPE_PERSO_REUSE_CODE_DATA:
        result = qmi_uim_tlv_personalization_category(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.personalization_reuse_code));
        break;
#endif /* FEATURE_SIMLOCK */

      case UIMI_TLV_TYPE_ENCRYPTED_EXT:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.encrypted_ext));
        break;

      case UIMI_TLV_TYPE_PROFILE_ID:
        result = qmi_uim_tlv_profile_id(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.sim_profile));
        break;

      case UIMI_TLV_TYPE_SEARCH_PATTERN:
        result = qmi_uim_tlv_search_pattern(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.search_pattern));
        break;

      case UIMI_TLV_TYPE_SEARCH_DATA:
        result = qmi_uim_tlv_search_data(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.search_data));
        break;

      case UIMI_TLV_TYPE_NAF_ID:
        result = qmi_uim_tlv_naf_id(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.gba_naf_id));
        break;

      case UIMI_TLV_TYPE_FORCE_BOOTSTRAPPING:
        result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.gba_force_bootstrapping));
        break;

      case UIMI_TLV_TYPE_ACCESS_POINT_NAME:
        result = qmi_uim_tlv_apn(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.apn));
        break;

      case UIMI_TLV_TYPE_BSF_INFO:
         result = qmi_uim_tlv_bsf_info(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.gba_bsf_info));
         break;

      case UIMI_TLV_TYPE_GBA_KEY_STATUS_IND_REG:
         result = qmi_uim_tlv_boolean(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.gba_key_status_ind_reg));
         break;

      case UIMI_TLV_TYPE_TEMPORARY_IDENTITY_MASK:
        result = qmi_uim_tlv_temporary_identity_mask(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.temporary_identity_mask));
        break;

      case UIMI_TLV_TYPE_SWITCH_SLOT_EXT:
         result = qmi_uim_tlv_switch_slot_ext(
                    sdu_in,
                    len,
                    &(parsed_data_ptr[index].data.switch_slot_ext));
         break;

      case UIMI_TLV_TYPE_EUICC_PROFILE_NAME:
        result = qmi_uim_tlv_euicc_profile_name(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.euicc_name));
        break;

      case UIMI_TLV_TYPE_SET_UNSET_PROFILE:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.set_unset_profile));
        break;

      case UIMI_TLV_TYPE_EUICC_DOWNLOAD_CODE:
      case UIMI_TLV_TYPE_EUICC_CONFIRMATION_CODE:
        result = qmi_uim_tlv_euicc_download_code(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.euicc_download_code));
        break;

      case UIMI_TLV_TYPE_TERMINAL_CAPABILITY:
        result = qmi_uim_tlv_terminal_capability(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.terminal_capability));
        break;

      case UIMI_TLV_TYPE_APDU_BEHAVIOR_FLAG:
        result = qmi_uim_tlv_apdu_behavior(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.apdu_behavior));
        break;

      case UIMI_TLV_TYPE_EUICC_RESET_OPTION_MASK:
        result = qmi_uim_tlv_euicc_reset_option_mask(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.euicc_reset_option_mask));
        break;

      case UIMI_TLV_TYPE_PROFILE_USER_CONSENT:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.euicc_user_consent));
        break;

      case UIMI_TLV_TYPE_USER_CONSENT_SUPPORTED:
        result = qmi_uim_tlv_boolean(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.euicc_user_consent_supported));
        break;

      case UIMI_TLV_TYPE_LONG_APDU_REQUEST:
        result = qmi_uim_tlv_long_apdu_request(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.long_apdu_request));
        break;

      case UIMI_TLV_TYPE_EUICC_DEFAULT_ADDRESS:
        result = qmi_uim_tlv_euicc_default_address(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.euicc_default_address));
        break;

      case UIMI_TLV_TYPE_CONFIG_ID:
        result = qmi_uim_tlv_uint8(
                   sdu_in,
                   len,
                   &(parsed_data_ptr[index].data.config_id));
        break;

      default:
        /* This should not happen because unwanted TLV types were already
           detected above, when checked if TLV was mandatory or optional */
        result = QMI_ERR_MALFORMED_MSG;
        break;
    }

    if (result != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_2("Error 0x%x parsing TLV type 0x%x", result, type);
      break;
    }

    parsed_data_ptr[index].present = TRUE;
  }

  /* In case of success, check that all mandatory TLV were received */
  if (result == QMI_ERR_NONE)
  {
    for (index = 0; index < count; index++)
    {
      boolean mandatory = FALSE;

      /* Check if each TLV is mandatory from the tag */
      if (parsed_data_ptr[index].tag >= UIMI_TLV_TAG_MANDATORY_1 &&
          parsed_data_ptr[index].tag < UIMI_TLV_TAG_OPTIONAL_1)
      {
        mandatory = TRUE;
      }

      if (mandatory &&
          !parsed_data_ptr[index].present)
      {
        result = QMI_ERR_MISSING_ARG;
        break;
      }
    }
  }

  /* In case of error, clean the TLV data, as it might contain
     sensitive information, such as PIN values, or allocated
     memory, such as write content */
  if (result != QMI_ERR_NONE)
  {
    qmi_uim_util_free_tlv_content(parsed_data_ptr, count);
  }

  return result;
} /* qmi_uim_parse_request */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_RESULT()

  DESCRIPTION
    Composes the TLV content of card status

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    status_word   : status word, as received from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_result
(
  byte            tlv_id,
  dsm_item_type** response_pptr,
  mmgsdi_sw_type  status_word
)
{
  PACKED struct PACKED_POST
  {
    uint8 sw1;
    uint8 sw2;
  } tlv_data;

  ASSERT(response_pptr);

  /* SW code is not present... simply return TRUE.
     This TLV is optional and will be skipped */
  if (status_word.present == FALSE)
  {
    return TRUE;
  }

  tlv_data.sw1 = status_word.sw1;
  tlv_data.sw2 = status_word.sw2;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_card_result */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_READ_ADDITIONAL_RECORDS_RESULT()

  DESCRIPTION
    Composes the TLV content of additional records result

  PARAMETERS
    tlv_id         : id to be used for the TLV
    response       : output sdu
    num_records    : number of records in the array
    records_ptr    : array of records from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_read_additional_records_result
(
  byte                              tlv_id,
  dsm_item_type **                  response_pptr,
  uint16                            num_records,
  qmi_uim_additional_records_type * records_ptr
)
{
  uint16  i            = 0;
  uint16  expected_len = 0;
  boolean ret_value    = TRUE;
  uint16  tlv_len      = 0;
  uint8*  tlv_data_ptr = NULL;
  uint8*  curr_ptr     = NULL;
  uint16  temp_uint16  = 0;

  ASSERT(response_pptr);

  /* At least 2 records should be present. We can still return TRUE
     as this is an optional TLV */
  if (num_records <= 1 ||
      records_ptr == NULL)
  {
    return TRUE;
  }

  /* All records should have the size of the first record. We can still
     return TRUE as this is an optional TLV */
  expected_len = (uint16)records_ptr[0].read_data.data_len;
  if (expected_len == 0)
  {
    return TRUE;
  }

  /* Calculate amount of memory: all records must have the same length as
     the first one. The first record should not be copied. */
  tlv_len = sizeof(uint16);
  for (i = 1; i < num_records; i++)
  {
    if (records_ptr[i].mmgsdi_status == MMGSDI_SUCCESS &&
        records_ptr[i].read_data.data_len == expected_len &&
        records_ptr[i].read_data.data_ptr != NULL)
    {
      tlv_len += expected_len;
    }
    else
    {
      break;
    }
  }

  /* Allocate memory */
  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }
  curr_ptr = tlv_data_ptr;

  /* Copy length */
  temp_uint16 = tlv_len - sizeof(uint16);
  (void)memscpy((void*)tlv_data_ptr, tlv_len, &temp_uint16, sizeof(uint16));
  curr_ptr += sizeof(uint16);

  /* Copy data (except first record) */
  for (i = 1; i < num_records; i++)
  {
    if (records_ptr[i].mmgsdi_status == MMGSDI_SUCCESS &&
        records_ptr[i].read_data.data_len == expected_len &&
        records_ptr[i].read_data.data_ptr != NULL)
    {
      ASSERT(curr_ptr + expected_len <= tlv_data_ptr + tlv_len);
      (void)memscpy(curr_ptr,
                    tlv_len - (curr_ptr - tlv_data_ptr),
                    records_ptr[i].read_data.data_ptr,
                    records_ptr[i].read_data.data_len);
      curr_ptr += records_ptr[i].read_data.data_len;
    }
    else
    {
      break;
    }
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_read_additional_records_result */


/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_SECURITY_ATTRIBUTES()

  DESCRIPTION
security values to be used in the TLV response
  for get file attributes

  PARAMETERS
    file_security       : MMGSDI structure with file security info
    security_method_ptr : output parameter with security method
    security_mask_ptr   : output parameter with security mask

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_convert_security_attributes
(
  mmgsdi_file_security_type file_security,
  uint8  *                  security_method_ptr,
  uint16 *                  security_mask_ptr
)
{
  ASSERT(security_method_ptr && security_mask_ptr);

  /* Compose the security method */
  switch(file_security.protection_method)
  {
    case MMGSDI_ALWAYS_ALLOWED:
      *security_method_ptr = UIM_SEC_CONDITION_ALWAYS_V01;
      break;

    case MMGSDI_NEVER_ALLOWED:
      *security_method_ptr = UIM_SEC_CONDITION_NEVER_V01;
      break;

    case MMGSDI_AND_ALLOWED:
      *security_method_ptr = UIM_SEC_CONDITION_AND_COND_V01;
      break;

    case MMGSDI_OR_ALLOWED:
      *security_method_ptr = UIM_SEC_CONDITION_OR_COND_V01;
      break;

    case MMGSDI_SINGLE_ALLOWED:
      *security_method_ptr = UIM_SEC_CONDITION_SINGLE_COND_V01;
      break;

    default:
      *security_method_ptr = UIM_SEC_CONDITION_ALWAYS_V01;
      break;
  }

  /* Compose the security mask */
  *security_mask_ptr = 0;
  if (file_security.protection_method == MMGSDI_AND_ALLOWED ||
      file_security.protection_method == MMGSDI_OR_ALLOWED ||
      file_security.protection_method == MMGSDI_SINGLE_ALLOWED)
  {
    if (file_security.num_protection_pin != 0)
    {
      uint16 i = 0;
      for (i = 0; i < file_security.num_protection_pin; i++)
      {
        switch(file_security.protection_pin_ptr[i])
        {
          case MMGSDI_PIN1:
            *security_mask_ptr |= 0x1;
            break;

          case MMGSDI_PIN2:
            *security_mask_ptr |= 0x2;
            break;

          case MMGSDI_UNIVERSAL_PIN:
            *security_mask_ptr |= 0x4;
            break;

          default:
            /* Handle all ADM values in the same way */
            *security_mask_ptr |= 0x8;
            break;
        }
      }
    }
  }
} /* qmi_uim_convert_security_attributes */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_FILE_ATTRIBUTES()

  DESCRIPTION
    Composes the TLV content of read result

  PARAMETERS
    tlv_id         : id to be used for the TLV
    response_pptr  : output sdu
    attribute_data : file attributes from the card
    raw_value      : attributes raw value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_file_attributes
(
  byte                           tlv_id,
  dsm_item_type **               response_pptr,
  mmgsdi_file_attributes_type    attribute_data,
  mmgsdi_get_file_attr_data_type raw_value
)
{
  boolean                            ret_value         = TRUE;
  uint16                             tlv_len           = 0;
  uint8*                             tlv_data_complete = NULL;
  mmgsdi_file_security_access_type * security_access   = NULL;

  PACKED struct PACKED_POST
  {
    uint16 file_size;
    uint16 file_id;
    uint8  file_type;
    uint16 rec_size;
    uint16 rec_count;
    uint8  security_read_method;
    uint16 security_read_mask;
    uint8  security_write_method;
    uint16 security_write_mask;
    uint8  security_increase_method;
    uint16 security_increase_mask;
    uint8  security_deactivate_method;
    uint16 security_deactivate_mask;
    uint8  security_activate_method;
    uint16 security_activate_mask;
    uint16 raw_len;
  } tlv_data;

  ASSERT(response_pptr);

  tlv_len = sizeof(tlv_data);
  if (raw_value.data_len > 0 && raw_value.data_ptr != NULL)
  {
     tlv_len += (uint16)raw_value.data_len;
  }

  tlv_data_complete = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_complete == NULL)
  {
    return FALSE;
  }

  tlv_data.file_size = (uint16)attribute_data.file_size;
  tlv_data.file_id   = (attribute_data.file_id[0] << 8) + attribute_data.file_id[1];
  switch (attribute_data.file_type)
  {
    case MMGSDI_LINEAR_FIXED_FILE:
      tlv_data.file_type = UIM_FILE_TYPE_LINEAR_FIXED_V01;
      tlv_data.rec_size  = (uint16)attribute_data.file_info.linear_fixed_file.rec_len;
      tlv_data.rec_count = (uint16)attribute_data.file_info.linear_fixed_file.num_of_rec;
      security_access    = &attribute_data.file_info.linear_fixed_file.file_security;
      break;

    case MMGSDI_CYCLIC_FILE:
      tlv_data.file_type = UIM_FILE_TYPE_CYCLIC_V01;
      tlv_data.rec_size  = (uint16)attribute_data.file_info.cyclic_file.rec_len;
      tlv_data.rec_count = (uint16)attribute_data.file_info.cyclic_file.num_of_rec;
      security_access    = &attribute_data.file_info.cyclic_file.file_security;
      break;

    case MMGSDI_TRANSPARENT_FILE:
      tlv_data.file_type = UIM_FILE_TYPE_TRANSPARENT_V01;
      tlv_data.rec_size  = 0;
      tlv_data.rec_count = 0;
      security_access    = &attribute_data.file_info.transparent_file.file_security;
      break;

    case MMGSDI_MASTER_FILE:
      tlv_data.file_type = UIM_FILE_TYPE_MASTER_FILE_V01;
      tlv_data.rec_size  = 0;
      tlv_data.rec_count = 0;
      break;

    case MMGSDI_DEDICATED_FILE:
      tlv_data.file_type = UIM_FILE_TYPE_DEDICATED_FILE_V01;
      tlv_data.rec_size  = 0;
      tlv_data.rec_count = 0;
      break;

    default:
      uimqmi_free(tlv_data_complete);
      return FALSE;
  }

  if (security_access != NULL)
  {
    uint8  temp_security_method = 0;
    uint16 temp_security_mask   = 0;

    /* Read */
    qmi_uim_convert_security_attributes(security_access->read,
                                        &temp_security_method,
                                        &temp_security_mask);
    tlv_data.security_read_method = temp_security_method;
    tlv_data.security_read_mask   = temp_security_mask;

    /* Write */
    qmi_uim_convert_security_attributes(security_access->write,
                                        &temp_security_method,
                                        &temp_security_mask);
    tlv_data.security_write_method = temp_security_method;
    tlv_data.security_write_mask   = temp_security_mask;

    /* Increase */
    qmi_uim_convert_security_attributes(security_access->increase,
                                        &temp_security_method,
                                        &temp_security_mask);
    tlv_data.security_increase_method = temp_security_method;
    tlv_data.security_increase_mask   = temp_security_mask;

    /* Deactivate */
    qmi_uim_convert_security_attributes(security_access->invalidate_deactivate,
                                        &temp_security_method,
                                        &temp_security_mask);
    tlv_data.security_deactivate_method = temp_security_method;
    tlv_data.security_deactivate_mask   = temp_security_mask;

    /* Activate */
    qmi_uim_convert_security_attributes(security_access->rehabilitate_activate,
                                        &temp_security_method,
                                        &temp_security_mask);
    tlv_data.security_activate_method = temp_security_method;
    tlv_data.security_activate_mask   = temp_security_mask;
  }
  else
  {
    /* Set secutrity as always for default */
    tlv_data.security_read_method       = UIM_SEC_CONDITION_ALWAYS_V01;
    tlv_data.security_read_mask         = 0;
    tlv_data.security_write_method      = UIM_SEC_CONDITION_ALWAYS_V01;
    tlv_data.security_write_mask        = 0;
    tlv_data.security_increase_method   = UIM_SEC_CONDITION_ALWAYS_V01;
    tlv_data.security_increase_mask     = 0;
    tlv_data.security_deactivate_method = UIM_SEC_CONDITION_ALWAYS_V01;
    tlv_data.security_deactivate_mask   = 0;
    tlv_data.security_activate_method   = UIM_SEC_CONDITION_ALWAYS_V01;
    tlv_data.security_activate_mask     = 0;
  }

  tlv_data.raw_len = (uint16)raw_value.data_len;

  /* Copy packed structure in buffer to be sent */
  (void)memscpy((void*)tlv_data_complete, tlv_len, &tlv_data, sizeof(tlv_data));
  if (raw_value.data_len > 0 && raw_value.data_ptr != NULL)
  {
    (void)memscpy((void*)(tlv_data_complete + sizeof(tlv_data)),
                  tlv_len - sizeof(tlv_data),
                  (void*)raw_value.data_ptr,
                  (uint32)raw_value.data_len);
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_complete);

  uimqmi_free(tlv_data_complete);
  return ret_value;
} /* qmi_uim_response_get_file_attributes */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_RETRIES_LEFT()

  DESCRIPTION
    Composes the TLV content of PIN retries left

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    num_retries         : number of PIN retries
    num_unblock_retries : number of PUK retries

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_retries_left
(
  byte             tlv_id,
  dsm_item_type ** response_pptr,
  uint32           num_retries,
  uint32           num_unblock_retries
)
{
  PACKED struct PACKED_POST
  {
    uint8 num_retries;
    uint8 num_unblock_retries;
  } tlv_data;

  ASSERT(response_pptr);

  if(num_retries >= QMI_UIM_MAX_RETRIES)
  {
    tlv_data.num_retries = QMI_UIM_MAX_RETRIES;
  }
  else
  {
    tlv_data.num_retries = (uint8)num_retries;
  }

  if(num_unblock_retries >= QMI_UIM_MAX_RETRIES)
  {
    tlv_data.num_unblock_retries = QMI_UIM_MAX_RETRIES;
  }
  else
  {
    tlv_data.num_unblock_retries = (uint8)num_unblock_retries;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_retries_left */


/*===========================================================================
  FUNCTION QMI_UIM_AID_MATCH()

  DESCRIPTION
    Matches the two AIDs and return result

  PARAMETERS
    app_aid_ptr         : Value of USIM/CSIM/ISIM AID
    app_aid_len         : Length of USIM/CSIM/ISIM AID
    aid_ptr             : Value of the request AID
    partial_aid_match_ok: Dictates whether caller is ok with going ahead
                          with checking for a partial AID match incase
                          complete AID did not match with USIM/CSIM/ISIM

  RETURN VALUE
    TRUE if matches. FALSE if not

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_aid_match
(
  const uint8*                    app_aid_ptr,
  uint8                           app_aid_len,
  const mmgsdi_static_data_type * aid_ptr,
  boolean                         partial_aid_match_ok
)
{
  boolean     match_found      = FALSE;

  ASSERT(aid_ptr != NULL && app_aid_ptr != NULL);

  if (aid_ptr->data_len > MMGSDI_MAX_AID_LEN)
  {
    return FALSE;
  }

  if (aid_ptr->data_len >= app_aid_len)
  {
    if (memcmp(aid_ptr->data_ptr, app_aid_ptr, app_aid_len) == 0)
    {
      /* AID matched completely with a USIM AID (first 7 bytes)*/
      match_found = TRUE;
    }
    return match_found;
  }

  /* Input AID seems to be a partial AID (shorter than 7 bytes). If the
     caller is ok with checking for a partial-AID match, lets see if the
     input AID matches partially with the USIM/CSIM/ISIM aid and return
     TRUE in that case. */
  if (partial_aid_match_ok)
  {
    /* If zero-length AID is passed in as input, lets mark it as a (partial)
       match */
    if (aid_ptr->data_len == 0 ||
        memcmp(aid_ptr->data_ptr, app_aid_ptr, aid_ptr->data_len) == 0)
    {
      match_found = TRUE;
    }
  }
  return match_found;
} /* qmi_uim_aid_match */


/*===========================================================================
  FUNCTION QMI_UIM_IS_ISIM_AID()

  DESCRIPTION
    Checks if the current AID belongs to a ISIM or not

  PARAMETERS
    aid_ptr       : value of the AID

  RETURN VALUE
    TRUE for ISIM. FALSE in all other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_is_isim_aid
(
  const mmgsdi_static_data_type * aid_ptr
)
{
  const uint8 isim_aid_value[] = { 0xA0, 0x00, 0x00, 0x00, 0x87,
                                   0x10, 0x04 };

  return qmi_uim_aid_match(isim_aid_value,
                           sizeof(isim_aid_value),
                           aid_ptr,
                           FALSE);
} /* qmi_uim_is_isim_aid */


/*===========================================================================
  FUNCTION QMI_UIM_IS_USIM_AID()

  DESCRIPTION
    Checks if the current AID belongs to a USIM or not.

  PARAMETERS
    aid_ptr             : value of the AID
    partial_aid_match_ok: dictates whether caller is ok with going ahead
                          with checking for a partial AID match incase
                          complete AID did not match with USIM

  RETURN VALUE
    boolean
      TRUE        : if the AID matched (completely or partially depending on
                    input parameter).
      FALSE       : in all other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_is_usim_aid
(
  const   mmgsdi_static_data_type * aid_ptr,
  boolean                           partial_aid_match_ok
)
{
  const uint8 usim_aid_value[] = { 0xA0, 0x00, 0x00, 0x00, 0x87,
                                   0x10, 0x02 };

  return qmi_uim_aid_match(usim_aid_value,
                           sizeof(usim_aid_value),
                           aid_ptr,
                           partial_aid_match_ok);
} /* qmi_uim_is_usim_aid */


/*===========================================================================
  FUNCTION QMI_UIM_IS_CSIM_AID()

  DESCRIPTION
    Checks if the current AID belongs to a CSIM or not.

  PARAMETERS
    aid_ptr             : value of the AID
    partial_aid_match_ok: dictates whether caller is ok with going ahead
                          with checking for a partial AID match incase
                          complete AID did not match with USIM

  RETURN VALUE
    boolean
      TRUE        : if the AID matched (completely or partially depending on
                    input parameter).
      FALSE       : in all other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_is_csim_aid
(
  const   mmgsdi_static_data_type * aid_ptr,
  boolean                           partial_aid_match_ok
)
{
  const uint8 csim_aid_value[] = { 0xA0, 0x00, 0x00, 0x03, 0x43,
                                   0x10, 0x02 };

  return qmi_uim_aid_match(csim_aid_value,
                           sizeof(csim_aid_value),
                           aid_ptr,
                           partial_aid_match_ok);
} /* qmi_uim_is_csim_aid */


/*===========================================================================
  FUNCTION QMI_UIM_POPULATE_SIMLOCK_DISPLAY_FOR_SLOT_POLICY()

  DESCRIPTION
    This function updates the perso state of current slot application if at
    least one other slot has card inserted and any applications is in PIN
    state or in PERSO check state with perso state <= in progress or in other
    than PERSO check state.

  PARAMETERS
    card_state_ptr         : pointer to card state
    current_slot_index     : current slot index in card state
    current_slot_app_index : current slot app index in card state
    max_card_count         : maximum card count for this target

  RETURN VALUE
    qmi_uim_perso_state_type : perso state to be set for current
                               slot appplication

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_uim_perso_state_type qmi_uim_populate_simlock_display_for_slot_policy
(
  const qmi_uim_card_state * card_state_ptr,
  uint8                      current_slot_index,
  uint8                      current_slot_app_index,
  uint8                      max_card_count
 )
{
  uint8                        other_slot_index     = 0;
  uint8                        other_slot_app_index = 0;
  boolean                      update_perso_state   = FALSE;
  uint8                        app_slot_index       = 0;
  qmi_uim_simlock_display_type simlock_display      = QMI_UIM_SIMLOCK_DISPLAY_DEFAULT;

  if (card_state_ptr == NULL ||
      current_slot_app_index >= QMI_UIM_MAX_APP_COUNT)
  {
    return QMI_UIM_PERSO_STATE_UNKNOWN;
  }

  if((card_state_ptr->application[current_slot_app_index]->perso_state !=
        QMI_UIM_PERSO_STATE_CODE_REQUIRED &&
     card_state_ptr->application[current_slot_app_index]->perso_state !=
        QMI_UIM_PERSO_STATE_PERM_BLOCKED) || 
     max_card_count >= QMI_UIM_MAX_CARD_COUNT ||
     max_card_count <= 1)
  {
    return card_state_ptr->application[current_slot_app_index]->perso_state;
  }

  simlock_display = qmi_uim_util_is_simlock_display_for_valid_card_enabled();

  /* check other slots and all app states */
  for (other_slot_index = 0;
       update_perso_state == FALSE && other_slot_index < max_card_count;
       other_slot_index++)
  {
    /* Update perso state when at least one other slot has card inserted and
       any applications stuck in QMI_UIM_APP_STATE_PIN_REQUIRED or
       in QMI_UIM_APP_STATE_PERSO with perso_state <= QMI_UIM_PERSO_STATE_IN_PROGRESS*/
    if (card_state_ptr->card[other_slot_index] == NULL ||
        (other_slot_index == current_slot_index) ||
        (other_slot_index != current_slot_index &&
         card_state_ptr->card[other_slot_index]->card_state != QMI_UIM_CARD_STATE_PRESENT))
    {
      continue;
    }

    /* check for apps from other_slot_index */
    for (other_slot_app_index = 0;
         (other_slot_app_index < card_state_ptr->num_apps &&
          other_slot_app_index < QMI_UIM_MAX_APP_COUNT);
         other_slot_app_index++)
    {
      /* Pick only applications with other_slot_index*/
      if (card_state_ptr->application[other_slot_app_index] == NULL ||
          uimqmi_mmgsdi_slot_to_slot_index(card_state_ptr->application[other_slot_app_index]->slot,
                                           &app_slot_index,
                                           QMI_UIM_MAX_CARD_COUNT) != QMI_ERR_NONE ||
          app_slot_index != other_slot_index)
      {
        continue;
      }

      /* Update perso status to in progress in following cases:
         1) NV & policy set to wait for full service, other card's perso status is unknown/in progress/ready.
         2) NV & policy set to wait for valid card, other card's app state is PIN required or
            app state is perso and perso is in progress. 
         3) NV & policy set to wait for valid card on any slot, other card's app state Detected. */
      if(simlock_display == QMI_UIM_SIMLOCK_DISPLAY_FOR_FULL_SERVICE_ON_ANY_SLOT_POLICY &&
         (card_state_ptr->application[other_slot_app_index]->app_state != QMI_UIM_APP_STATE_PERSO ||
          card_state_ptr->application[other_slot_app_index]->perso_state <= QMI_UIM_PERSO_STATE_READY))
      {
        update_perso_state = TRUE;
        break;
      }
      else if(((simlock_display == QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ANY_SLOT_POLICY ||
               (simlock_display == QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ALL_SLOT_POLICY &&
               card_state_ptr->application[current_slot_app_index]->perso_state != QMI_UIM_PERSO_STATE_PERM_BLOCKED)) &&
              (card_state_ptr->application[other_slot_app_index]->app_state == QMI_UIM_APP_STATE_PIN_REQUIRED ||
               (card_state_ptr->application[other_slot_app_index]->app_state == QMI_UIM_APP_STATE_PERSO &&
                card_state_ptr->application[other_slot_app_index]->perso_state <= QMI_UIM_PERSO_STATE_IN_PROGRESS))) ||
               (simlock_display == QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ANY_SLOT_POLICY  &&
                card_state_ptr->application[other_slot_app_index]->app_state == QMI_UIM_APP_STATE_DETECTED))
      {
        update_perso_state = TRUE;
        break;
      }
    }
  }

  if (update_perso_state)
  {
    return QMI_UIM_PERSO_STATE_IN_PROGRESS;
  }

  return card_state_ptr->application[current_slot_app_index]->perso_state;
} /* qmi_uim_populate_simlock_display_for_slot_policy */


/*===========================================================================
  FUNCTION QMI_UIM_POPULATE_CARD_AND_APP_STATE()

  DESCRIPTION
    Composes the card and app state info for card status indication or
    get card status request

  PARAMETERS
    tlv_len                 : length of tlv_data_ptr
    tlv_data_ptr            : memory to fill in TLV data
    card_state_ptr          : card status
    app_count_per_slot_len  : length of apps present per slot array
    app_count_per_slot_ptr  : apps present per slot
    extended_card_status    : if the TLV is for extended card status
    validit_len             : length of card status validity array
    validity_ptr            : card status validity array

  RETURN VALUE
    boolean

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_populate_card_and_app_state
(
  uint16                     tlv_len,
  uint8                    * tlv_data_ptr,
  const qmi_uim_card_state * card_state_ptr,
  uint8                      app_count_per_slot_len,
  uint8                    * app_count_per_slot_ptr,
  boolean                    extended_card_status,
  uint8                      validity_len,
  const boolean            * validity_ptr
)
{
  uint8                           i                          = 0;
  uint8                           j                          = 0;
  uint8                           slot_index                 = 0;
  uint8                         * curr_ptr                   = NULL;
  uint8                           max_card_count             = 0;
  uint8                           max_app_count              = 0;
  uint8                           app_count                  = 0;
  qmi_uim_simlock_display_type    simlock_display            = QMI_UIM_SIMLOCK_DISPLAY_DEFAULT;
  tlv_data_card_state_type        tlv_data_card_state;
  tlv_data_app_state_top_type     tlv_data_app_state_top;
  tlv_data_app_state_bottom_type  tlv_data_app_state_bottom;
#ifndef FEATURE_CUST_2
  mmgsdi_nv_context_type          nv_context                 = MMGSDI_NV_CONTEXT_INVALID;
#endif /* FEATURE_CUST_2 */

  ASSERT(card_state_ptr && tlv_data_ptr && app_count_per_slot_ptr);

  curr_ptr = tlv_data_ptr;

  if (extended_card_status)
  {
    max_card_count = (card_state_ptr->num_slots < QMI_UIM_MAX_CARD_COUNT) ?
                       card_state_ptr->num_slots : QMI_UIM_MAX_CARD_COUNT;
    max_app_count = QMI_UIM_MAX_APPS_PER_CARD;
  }
  else
  {
    max_card_count = (card_state_ptr->num_slots < QMI_UIM_LEGACY_MAX_CARD_COUNT) ?
                       card_state_ptr->num_slots : QMI_UIM_LEGACY_MAX_CARD_COUNT;
    max_app_count = QMI_UIM_LEGACY_MAX_APPS_PER_CARD;
  }

  if(app_count_per_slot_len < max_card_count)
  {
    return FALSE;
  }

  simlock_display = qmi_uim_util_is_simlock_display_for_valid_card_enabled();

  /* Loop thru all the cards */
  for (i = 0; i < max_card_count; i++)
  {
    if (card_state_ptr->card[i] == NULL)
    {
      return FALSE;
    }

    /* Card state */
    if (extended_card_status && (validity_ptr != NULL) && (i < validity_len))
    {
      if (validity_ptr[i])
      {
        tlv_data_card_state.card_state = (uint8)card_state_ptr->card[i]->card_state;
      }
      else
      {
        tlv_data_card_state.card_state = UIM_EXTENDED_CARD_STATE_UNKNOWN_V01;
      }
    }
    else
    {
      tlv_data_card_state.card_state = (uint8)card_state_ptr->card[i]->card_state;
    }

    switch(card_state_ptr->card[i]->upin_state)
    {
      case MMGSDI_PIN_ENABLED_NOT_VERIFIED:
        tlv_data_card_state.upin_state = UIM_PIN_STATE_ENABLED_NOT_VERIFIED_V01;
        break;
      case MMGSDI_PIN_ENABLED_VERIFIED:
        tlv_data_card_state.upin_state = UIM_PIN_STATE_ENABLED_VERIFIED_V01;
        break;
      case MMGSDI_PIN_DISABLED:
        tlv_data_card_state.upin_state = UIM_PIN_STATE_DISABLED_V01;
        break;
      case MMGSDI_PIN_BLOCKED:
        tlv_data_card_state.upin_state = UIM_PIN_STATE_BLOCKED_V01;
        break;
      case MMGSDI_PIN_PERM_BLOCKED:
        tlv_data_card_state.upin_state = UIM_PIN_STATE_PERMANENTLY_BLOCKED_V01;
        break;
      case MMGSDI_PIN_STATUS_NOT_INITIALIZED:
      default:
        tlv_data_card_state.upin_state = UIM_PIN_STATE_UNKNOWN_V01;
        break;
    }
    tlv_data_card_state.upin_retries = card_state_ptr->card[i]->upin_num_retries;
    tlv_data_card_state.upuk_retries = card_state_ptr->card[i]->upuk_num_retries;
    tlv_data_card_state.error_code   = (uint8)card_state_ptr->card[i]->card_error;
    tlv_data_card_state.num_app      = app_count_per_slot_ptr[i];

    /* Copy card state in the TLV */
    ASSERT(curr_ptr + sizeof(tlv_data_card_state) <= tlv_data_ptr + tlv_len);
    (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                  (void*)&tlv_data_card_state, sizeof(tlv_data_card_state));
    curr_ptr += sizeof(tlv_data_card_state);

    /* Loop thru all the applications */
    app_count = 0;
    for (j = 0; j < card_state_ptr->num_apps && j < QMI_UIM_MAX_APP_COUNT; j++)
    {
      if (app_count >= max_app_count)
      {
        break;
      }

      if (card_state_ptr->application[j] == NULL)
      {
        return FALSE;
      }

      if (extended_card_status == FALSE &&
          (card_state_ptr->application[j]->slot != MMGSDI_SLOT_1 &&
           card_state_ptr->application[j]->slot != MMGSDI_SLOT_2))
      {
        continue;
      }

      /* Pick only applications for this card */
      if (uimqmi_mmgsdi_slot_to_slot_index(card_state_ptr->application[j]->slot,
                                           &slot_index,
                                           QMI_UIM_MAX_CARD_COUNT) != QMI_ERR_NONE)
      {
        continue;
      }

      if (slot_index != i)
      {
        continue;
      }

#ifndef FEATURE_CUST_2
      /* If CDMA_LESS feature is enabled then do not populate CSIM/RUIM app info. */
      nv_context = mmgsdi_util_get_efs_item_index_for_slot(
                     card_state_ptr->application[j]->slot);
      if(nv_context != MMGSDI_NV_CONTEXT_INVALID &&
         (mmgsdi_nv_get_slot_feature_status(
            MMGSDI_FEATURE_CDMA_LESS, nv_context) == MMGSDI_FEATURE_ENABLED) &&
         ((card_state_ptr->application[j]->app_type == MMGSDI_APP_CSIM) || 
          (card_state_ptr->application[j]->app_type == MMGSDI_APP_RUIM)))
      {
        tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_UNKNOWN_V01;
        continue;
      }
#endif /* FEATURE_CUST_2 */

      /* App type */
      switch(card_state_ptr->application[j]->app_type)
      {
        case MMGSDI_APP_SIM:
          tlv_data_app_state_top.app_type = UIM_APP_TYPE_SIM_V01;
          break;
        case MMGSDI_APP_RUIM:
          tlv_data_app_state_top.app_type = UIM_APP_TYPE_RUIM_V01;
          break;
        case MMGSDI_APP_USIM:
          tlv_data_app_state_top.app_type = UIM_APP_TYPE_USIM_V01;
          break;
        case MMGSDI_APP_CSIM:
          tlv_data_app_state_top.app_type = UIM_APP_TYPE_CSIM_V01;
          break;
        case MMGSDI_APP_UNKNOWN:
          if (qmi_uim_is_isim_aid(&card_state_ptr->application[j]->aid))
          {
            tlv_data_app_state_top.app_type = UIM_APP_TYPE_ISIM_V01;
          }
          else
          {
            tlv_data_app_state_top.app_type = UIM_APP_TYPE_UNKNOWN_V01;
          }
          break;
        default:
          tlv_data_app_state_top.app_type = UIM_APP_TYPE_UNKNOWN_V01;
          break;
      }

      /* App state */
      tlv_data_app_state_top.app_state =
        (uint8)card_state_ptr->application[j]->app_state;

      /* Perso state */
      tlv_data_app_state_top.perso_state =
        (uint8)card_state_ptr->application[j]->perso_state;

      /* Check if simlock status update for display is needed */
      if (simlock_display == QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ANY_SLOT_POLICY ||
          simlock_display == QMI_UIM_SIMLOCK_DISPLAY_FOR_FULL_SERVICE_ON_ANY_SLOT_POLICY ||
          simlock_display == QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ALL_SLOT_POLICY)
      {
        tlv_data_app_state_top.perso_state =
          (uint8)qmi_uim_populate_simlock_display_for_slot_policy(card_state_ptr,
                                                                  i,
                                                                  j,
                                                                  max_card_count);
      }

      /* Perso feature */
      switch(card_state_ptr->application[j]->perso_feature)
      {
        case MMGSDI_PERSO_NW:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_NETWORK_V01;
          break;

        case MMGSDI_PERSO_NS:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_NETWORK_SUBSET_V01;
          break;

        case MMGSDI_PERSO_SP:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_SERVICE_PROVIDER_V01;
          break;

        case MMGSDI_PERSO_CP:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_CORPORATE_V01;
          break;

        case MMGSDI_PERSO_SIM:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_UIM_V01;
          break;

        case MMGSDI_PERSO_SPN:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_SERVICE_PROVIDER_NAME_V01;
          break;

        case MMGSDI_PERSO_SP_EHPLMN:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_SP_EHPLMN_V01;
          break;

        case MMGSDI_PERSO_ICCID:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_ICCID_V01;
          break;

        case MMGSDI_PERSO_NS_SP:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_GW_NETWORK_SUBSET_SERVICE_PROVIDER_V01;
          break;

        case MMGSDI_PERSO_RUIM_NW1:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_1X_NETWORK_TYPE_1_V01;
          break;

        case MMGSDI_PERSO_RUIM_NW2:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_1X_NETWORK_TYPE_2_V01;
          break;

        case MMGSDI_PERSO_RUIM_HRPD:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_1X_HRPD_V01;
          break;

        case MMGSDI_PERSO_RUIM_SP:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_1X_SERVICE_PROVIDER_V01;
          break;

        case MMGSDI_PERSO_RUIM_CP:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_1X_CORPORATE_V01;
          break;

        case MMGSDI_PERSO_RUIM_RUIM:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_1X_RUIM_V01;
          break;

        default:
          tlv_data_app_state_top.perso_feature = UIM_PERSO_FEATURE_STATUS_UNKNOWN_V01;
          break;
      }

      /* Perso retries */
      if(card_state_ptr->application[j]->perso_retries >= QMI_UIM_MAX_RETRIES)
      {
        tlv_data_app_state_top.perso_retries = QMI_UIM_MAX_RETRIES;
      }
      else
      {
        tlv_data_app_state_top.perso_retries =
           (uint8)card_state_ptr->application[j]->perso_retries;
      }

      /* Perso unblock retries is no longer supported with current SimLock engine.
         So setting always to 0 to indicate that unblock functionality cannot
         be used */
      tlv_data_app_state_top.perso_unblock_retries = 0;

      /* AID length */
      tlv_data_app_state_top.aid_len =
        (uint8)card_state_ptr->application[j]->aid.data_len;

      /* UPIN */
      tlv_data_app_state_bottom.univ_pin =
        (card_state_ptr->application[j]->upin ?
           UIM_UNIV_PIN_UPIN_REPLACES_PIN1_V01 : UIM_UNIV_PIN_PIN1_USED_V01);

      /* PIN 1 state */
      switch(card_state_ptr->application[j]->pin1_state)
      {
        case MMGSDI_PIN_ENABLED_NOT_VERIFIED:
          tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_ENABLED_NOT_VERIFIED_V01;
          break;
        case MMGSDI_PIN_ENABLED_VERIFIED:
          tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_ENABLED_VERIFIED_V01;
          break;
        case MMGSDI_PIN_DISABLED:
          tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_DISABLED_V01;
          break;
        case MMGSDI_PIN_BLOCKED:
          tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_BLOCKED_V01;
          break;
        case MMGSDI_PIN_PERM_BLOCKED:
          tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_PERMANENTLY_BLOCKED_V01;
          break;
        case MMGSDI_PIN_STATUS_NOT_INITIALIZED:
          if (card_state_ptr->application[j]->app_state == QMI_UIM_APP_STATE_READY ||
              card_state_ptr->application[j]->app_state == QMI_UIM_APP_STATE_PERSO ||
              card_state_ptr->application[j]->app_state == QMI_UIM_APP_STATE_ILLEGAL)
          {
            tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_DISABLED_V01;
          }
          else
          {
            tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_UNKNOWN_V01;
          }
          break;
        default:
          tlv_data_app_state_bottom.pin1_state = UIM_PIN_STATE_UNKNOWN_V01;
          break;
      }

      /* PIN1 retries */
      tlv_data_app_state_bottom.pin1_retries =
        card_state_ptr->application[j]->pin1_num_retries;
      tlv_data_app_state_bottom.puk1_retries =
        card_state_ptr->application[j]->puk1_num_retries;

      /* PIN 2 state */
      switch(card_state_ptr->application[j]->pin2_state)
      {
        case MMGSDI_PIN_ENABLED_NOT_VERIFIED:
          tlv_data_app_state_bottom.pin2_state = UIM_PIN_STATE_ENABLED_NOT_VERIFIED_V01;
          break;
        case MMGSDI_PIN_ENABLED_VERIFIED:
          tlv_data_app_state_bottom.pin2_state = UIM_PIN_STATE_ENABLED_VERIFIED_V01;
          break;
        case MMGSDI_PIN_DISABLED:
          tlv_data_app_state_bottom.pin2_state = UIM_PIN_STATE_DISABLED_V01;
          break;
        case MMGSDI_PIN_BLOCKED:
          tlv_data_app_state_bottom.pin2_state = UIM_PIN_STATE_BLOCKED_V01;
          break;
        case MMGSDI_PIN_PERM_BLOCKED:
          tlv_data_app_state_bottom.pin2_state = UIM_PIN_STATE_PERMANENTLY_BLOCKED_V01;
          break;
        case MMGSDI_PIN_STATUS_NOT_INITIALIZED:
        default:
          /* If PIN2 state is not initialized or unknown,
             do not update based on app state */
          tlv_data_app_state_bottom.pin2_state = UIM_PIN_STATE_UNKNOWN_V01;
          break;
      }

      /* PIN2 retries */
      tlv_data_app_state_bottom.pin2_retries =
        card_state_ptr->application[j]->pin2_num_retries;
      tlv_data_app_state_bottom.puk2_retries =
        card_state_ptr->application[j]->puk2_num_retries;

      /* Copy top part to TLV */
      ASSERT(curr_ptr + sizeof(tlv_data_app_state_top) <= tlv_data_ptr + tlv_len);
      (void)memscpy((void*)curr_ptr,
                    tlv_len - (curr_ptr - tlv_data_ptr),
                    (void*)&tlv_data_app_state_top,
                    sizeof(tlv_data_app_state_top));
      curr_ptr += sizeof(tlv_data_app_state_top);

      /* AID value */
      if (card_state_ptr->application[j]->aid.data_len > 0)
      {
        ASSERT(curr_ptr + card_state_ptr->application[j]->aid.data_len <= tlv_data_ptr + tlv_len);
        (void)memscpy((void*)curr_ptr,
                      tlv_len - (curr_ptr - tlv_data_ptr),
                      (void*)card_state_ptr->application[j]->aid.data_ptr,
                      (uint32)card_state_ptr->application[j]->aid.data_len);
        curr_ptr += card_state_ptr->application[j]->aid.data_len;
      }

      /* Copy bottom part to TLV */
      ASSERT(curr_ptr + sizeof(tlv_data_app_state_bottom) <= tlv_data_ptr + tlv_len);
      (void)memscpy((void*)curr_ptr,
                    tlv_len - (curr_ptr - tlv_data_ptr),
                    (void*)&tlv_data_app_state_bottom,
                    (uint32)sizeof(tlv_data_app_state_bottom));
      curr_ptr += sizeof(tlv_data_app_state_bottom);

      app_count++;
    } /* for (j = 0; j < card_state_ptr->num_apps && j < QMI_UIM_MAX_APP_COUNT; j++) */
  } /* for (i = 0; i < num_slots; i++) */

  return TRUE;
} /* qmi_uim_populate_card_and_app_state */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_STATUS()

  DESCRIPTION
    Composes the TLV content for card status indication or
    get card status request

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    card_state    : card status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_status
(
  byte                       tlv_id,
  dsm_item_type **           response_pptr,
  const qmi_uim_card_state * card_state_ptr
)
{
  uint8   i            = 0;
  boolean ret_value    = TRUE;
  uint16  tlv_len      = 0;
  uint8*  tlv_data_ptr = NULL;
  uint8*  curr_ptr     = NULL;
  uint8   slot_index   = 0;
  uint8   num_slots    = 0;
  uint8   app_count_per_slot[QMI_UIM_LEGACY_MAX_CARD_COUNT];
#ifndef FEATURE_CUST_2
  mmgsdi_nv_context_type nv_context  = MMGSDI_NV_CONTEXT_INVALID;
#endif /* FEATURE_CUST_2 */


  PACKED struct PACKED_POST
  {
    uint16 index_gw_pri;
    uint16 index_1x_pri;
    uint16 index_gw_sec;
    uint16 index_1x_sec;
    uint8  num_slot;
  } tlv_data_intro;

  ASSERT(response_pptr && card_state_ptr &&
         card_state_ptr->num_apps <= QMI_UIM_MAX_APP_COUNT &&
         card_state_ptr->num_slots > 0);

  memset(app_count_per_slot, 0, sizeof(app_count_per_slot));
  num_slots = (QMI_UIM_LEGACY_MAX_CARD_COUNT < card_state_ptr->num_slots) ?
                QMI_UIM_LEGACY_MAX_CARD_COUNT : card_state_ptr->num_slots;

  /* Initialize TLV length. Total length will be calculated after taking into
     account the total number of apps. */
  tlv_len = sizeof(tlv_data_intro);
  tlv_len += (sizeof(tlv_data_card_state_type) * num_slots);

  /* Intro: set default values for indexes and number of slots */
  tlv_data_intro.index_gw_pri = 0xFFFF;
  tlv_data_intro.index_1x_pri = 0xFFFF;
  tlv_data_intro.index_gw_sec = 0xFFFF;
  tlv_data_intro.index_1x_sec = 0xFFFF;
  tlv_data_intro.num_slot     = num_slots;

  /* Count the number of applications for each slot and
     populate the indexes to be used in the TLV */
  for (i = 0; i < card_state_ptr->num_apps && i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (card_state_ptr->application[i] == NULL)
    {
      continue;
    }

    if (card_state_ptr->application[i]->slot != MMGSDI_SLOT_1 &&
        card_state_ptr->application[i]->slot != MMGSDI_SLOT_2)
    {
      continue;
    }

    if (uimqmi_mmgsdi_slot_to_slot_index(card_state_ptr->application[i]->slot,
                                         &slot_index,
                                         QMI_UIM_MAX_CARD_COUNT) != QMI_ERR_NONE)
    {
      continue;
    }

    if (app_count_per_slot[slot_index] >= QMI_UIM_LEGACY_MAX_APPS_PER_CARD)
    {
      continue;
    }

#ifndef FEATURE_CUST_2
      nv_context = mmgsdi_util_get_efs_item_index_for_slot(
                     card_state_ptr->application[i]->slot);
      if(nv_context != MMGSDI_NV_CONTEXT_INVALID &&
          (mmgsdi_nv_get_slot_feature_status(
             MMGSDI_FEATURE_CDMA_LESS, nv_context) == MMGSDI_FEATURE_ENABLED) &&
             (card_state_ptr->application[i]->app_type == MMGSDI_APP_CSIM ||
              card_state_ptr->application[i]->app_type == MMGSDI_APP_RUIM))
      {
        continue;
      }
#endif /* FEATURE_CUST_2 */

    if (i == card_state_ptr->index_gw[0])
    {
      tlv_data_intro.index_gw_pri = (uint16)((uint16)slot_index << 8) +
                                    (uint16)app_count_per_slot[slot_index];
    }
    if (i == card_state_ptr->index_1x[0])
    {
      tlv_data_intro.index_1x_pri = (uint16)((uint16)slot_index << 8) +
                                    (uint16)app_count_per_slot[slot_index];
    }
    if (i == card_state_ptr->index_gw[1])
    {
      tlv_data_intro.index_gw_sec = (uint16)((uint16)slot_index << 8) +
                                    (uint16)app_count_per_slot[slot_index];
    }
    if (i == card_state_ptr->index_1x[1])
    {
      tlv_data_intro.index_1x_sec = (uint16)((uint16)slot_index << 8) +
                                    (uint16)app_count_per_slot[slot_index];
    }
    app_count_per_slot[slot_index]++;

    tlv_len += sizeof(tlv_data_app_state_top_type) + sizeof(tlv_data_app_state_bottom_type) +
                 (uint16)card_state_ptr->application[i]->aid.data_len;
  }

  /* Allocate memory for TLV */
  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }
  curr_ptr = tlv_data_ptr;

  /* Copy intro in the TLV */
  ASSERT(curr_ptr + sizeof(tlv_data_intro) <= tlv_data_ptr + tlv_len);
  (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                (void*)&tlv_data_intro, sizeof(tlv_data_intro));
  curr_ptr += sizeof(tlv_data_intro);

  ret_value = qmi_uim_populate_card_and_app_state(tlv_len - sizeof(tlv_data_intro),
                                                  curr_ptr,
                                                  card_state_ptr,
                                                  QMI_UIM_LEGACY_MAX_CARD_COUNT,
                                                  app_count_per_slot,
                                                  FALSE,
                                                  0,
                                                  NULL);
  if (ret_value)
  {
    ret_value = qmi_svc_put_param_tlv(response_pptr,
                                      tlv_id,
                                      tlv_len,
                                      (void *)tlv_data_ptr);
  }

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_card_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EXTENDED_CARD_STATUS()

  DESCRIPTION
    Composes the extended card status TLV content for card status indication
    or get card status request

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    card_state_ptr  : card status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_extended_card_status
(
  byte                       tlv_id,
  dsm_item_type **           response_pptr,
  const qmi_uim_card_state * card_state_ptr,
  uint8                      validity_len,
  const boolean            * validity_ptr
)
{
  uint8   i                     = 0;
  uint8   j                     = 0;
  boolean ret_value             = TRUE;
  uint16  tlv_len               = 0;
  uint8*  tlv_data_ptr          = NULL;
  uint8*  curr_ptr              = NULL;
  uint8   slot_index            = 0;
  uint8   num_slots             = 0;
  uint8   tlv_data_gw_prov_len  = 0;
  uint8   tlv_data_1x_prov_len  = 0;
  uint8   tlv_data_intro_len    = 0;
  uint8   num_prov_sessions     = 0;
  uint8   app_count_per_slot[QMI_UIM_MAX_CARD_COUNT];
#ifndef FEATURE_CUST_2
  mmgsdi_nv_context_type nv_context  = MMGSDI_NV_CONTEXT_INVALID;
#endif /* FEATURE_CUST_2 */

  PACKED struct PACKED_POST
  {
    uint8    index_gw_len;
    uint16   index_gw[QMI_UIM_MAX_PROV_SESSIONS];
  } tlv_data_gw_prov;

  PACKED struct PACKED_POST
  {
    uint8    index_1x_len;
    uint16   index_1x[QMI_UIM_MAX_PROV_SESSIONS];
  } tlv_data_1x_prov;

  PACKED struct PACKED_POST
  {
    uint8    card_info_len;
  } tlv_data_card_info_len;

  ASSERT(response_pptr && card_state_ptr &&
         card_state_ptr->num_apps <= QMI_UIM_MAX_APP_COUNT &&
         card_state_ptr->num_slots > 0);

  memset(app_count_per_slot, 0, sizeof(app_count_per_slot));
  num_slots = (QMI_UIM_MAX_CARD_COUNT < card_state_ptr->num_slots) ?
                QMI_UIM_MAX_CARD_COUNT : card_state_ptr->num_slots;
  num_prov_sessions = (QMI_UIM_MAX_PROV_SESSIONS < card_state_ptr->num_slots) ?
                         QMI_UIM_MAX_PROV_SESSIONS : card_state_ptr->num_slots;

  /* Calculate actual lengths of index data that needs to be copied to
     the final TLV */
  tlv_data_gw_prov_len = sizeof(uint8) + (sizeof(uint16) * num_prov_sessions);
  tlv_data_1x_prov_len = sizeof(uint8) + (sizeof(uint16) * num_prov_sessions);
  tlv_data_intro_len = tlv_data_gw_prov_len + tlv_data_1x_prov_len +
                         sizeof(tlv_data_card_info_len);

  /* Calculate memory requirement. Here we do not take into consideration
     the max number of apps per card and in worst case may allocate more
     memory than required. */
  tlv_len = tlv_data_intro_len;
  tlv_len += (sizeof(tlv_data_card_state_type) * num_slots);
  for (i = 0; i < card_state_ptr->num_apps && i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (card_state_ptr->application[i] != NULL)
    {
#ifndef FEATURE_CUST_2
      /* If CDMA_LESS feature is enabled then do not populate CSIM/RUIM app info. */
      nv_context = mmgsdi_util_get_efs_item_index_for_slot(
                     card_state_ptr->application[i]->slot);
      if(nv_context != MMGSDI_NV_CONTEXT_INVALID &&
         (mmgsdi_nv_get_slot_feature_status(
            MMGSDI_FEATURE_CDMA_LESS, nv_context) == MMGSDI_FEATURE_ENABLED) &&
         ((card_state_ptr->application[i]->app_type == MMGSDI_APP_CSIM) || 
          (card_state_ptr->application[i]->app_type == MMGSDI_APP_RUIM)))
      {
        continue;
      }
#endif /* FEATURE_CUST_2 */
      tlv_len += sizeof(tlv_data_app_state_top_type) + sizeof(tlv_data_app_state_bottom_type) +
                 (uint16)card_state_ptr->application[i]->aid.data_len;
    }
  }

  /* Allocate memory for TLV */
  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);

  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }
  curr_ptr = tlv_data_ptr;

  /* Intro: set default values for indexes and number of slots */
  tlv_data_gw_prov.index_gw_len = num_prov_sessions;
  memset(tlv_data_gw_prov.index_gw, 0xFF, sizeof(tlv_data_gw_prov.index_gw));
  tlv_data_1x_prov.index_1x_len = num_prov_sessions;
  memset(tlv_data_1x_prov.index_1x, 0xFF, sizeof(tlv_data_1x_prov.index_1x));

  tlv_data_card_info_len.card_info_len = num_slots;

  /* Count the number of applications for each slot and
     populate the indexes to be used in the TLV */
  for (i = 0; i < card_state_ptr->num_apps && i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (card_state_ptr->application[i] == NULL)
    {
      continue;
    }

    if (uimqmi_mmgsdi_slot_to_slot_index(card_state_ptr->application[i]->slot,
                                         &slot_index,
                                         QMI_UIM_MAX_CARD_COUNT) != QMI_ERR_NONE)
    {
      continue;
    }

#ifndef FEATURE_CUST_2
      nv_context = mmgsdi_util_get_efs_item_index_for_slot(
                       card_state_ptr->application[i]->slot);
      if(nv_context != MMGSDI_NV_CONTEXT_INVALID &&
             (mmgsdi_nv_get_slot_feature_status(
                 MMGSDI_FEATURE_CDMA_LESS, nv_context) == MMGSDI_FEATURE_ENABLED) &&
                   (card_state_ptr->application[i]->app_type == MMGSDI_APP_CSIM ||
                     card_state_ptr->application[i]->app_type == MMGSDI_APP_RUIM))
      {
        continue;
      }
#endif /* FEATURE_CUST_2 */

    for (j = 0; j < QMI_UIM_MAX_PROV_SESSIONS && j < card_state_ptr->num_slots; j++)
    {
      if (i == card_state_ptr->index_gw[j])
      {
        tlv_data_gw_prov.index_gw[j] = (uint16)((uint16)slot_index << 8) +
                                         (uint16)app_count_per_slot[slot_index];
      }
      if (i == card_state_ptr->index_1x[j])
      {
        tlv_data_1x_prov.index_1x[j] = (uint16)((uint16)slot_index << 8) +
                                         (uint16)app_count_per_slot[slot_index];
      }
    }
    app_count_per_slot[slot_index]++;
  }

  /* Copy intro in the TLV. */
  ASSERT(curr_ptr + tlv_data_intro_len <= tlv_data_ptr + tlv_len);
  (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                (void*)&tlv_data_gw_prov , tlv_data_gw_prov_len);
  curr_ptr += tlv_data_gw_prov_len;
  (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                (void*)&tlv_data_1x_prov , tlv_data_1x_prov_len);
  curr_ptr += tlv_data_1x_prov_len;
  (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                (void*)&tlv_data_card_info_len , sizeof(tlv_data_card_info_len));
  curr_ptr += sizeof(tlv_data_card_info_len);

  ret_value = qmi_uim_populate_card_and_app_state(tlv_len - tlv_data_intro_len,
                                                  curr_ptr,
                                                  card_state_ptr,
                                                  QMI_UIM_MAX_CARD_COUNT,
                                                  app_count_per_slot,
                                                  TRUE,
                                                  validity_len,
                                                  validity_ptr);
  if (ret_value)
  {
    ret_value = qmi_svc_put_param_tlv(response_pptr,
                                      tlv_id,
                                      tlv_len,
                                      (void *)tlv_data_ptr);
  }

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_extended_card_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EVENT_REG()

  DESCRIPTION
    Composes the TLV content of event reg

  PARAMETERS
    tlv_id                   : id to be used for the TLV
    response_pptr            : output sdu
    reg_event_type           : event registered type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_event_reg
(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  qmi_uim_registration_event_type    reg_event_type
)
{
  PACKED struct PACKED_POST
  {
    uint32 mask;
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.mask = 0x00;

  if (reg_event_type.card_status)
  {
    tlv_data.mask |= 0x01;
  }
  if (reg_event_type.sap_events)
  {
    tlv_data.mask |= 0x02;
  }
  if (reg_event_type.extended_card_status)
  {
    tlv_data.mask |= 0x04;
  }
  if(reg_event_type.prov_session_close_event)
  {
    tlv_data.mask |= 0x08;
  }
  if (reg_event_type.physical_slots_status)
  {
    tlv_data.mask |= 0x10;
  }
  if(reg_event_type.sim_busy_events)
  {
    tlv_data.mask |= 0x20;
  }
  if(reg_event_type.reduced_card_status_events)
  {
    tlv_data.mask |= 0x40;
  }
  if(reg_event_type.recovery_complete_event)
  {
    tlv_data.mask |= 0x80;
  }
  if(reg_event_type.supply_voltage_events)
  {
    tlv_data.mask |= 0x100;
  }
  if(reg_event_type.reg_for_activation_status_event)
  {
    tlv_data.mask |= 0x200;
  }
  if(reg_event_type.simlock_remote_sfs_events)
  {
    tlv_data.mask |= 0x400;
  }
  if(reg_event_type.simlock_temp_unlock_status)
  {
    tlv_data.mask |= 0x800;
  }
  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_event_reg */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_REFRESH_EVENT()

  DESCRIPTION
    Composes the TLV content of refresh event

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    session_type        : session type
    aid_ptr             : AID value
    refresh_evt_ptr     : refresh event

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_refresh_event
(
  byte                                 tlv_id,
  dsm_item_type **                     response_pptr,
  mmgsdi_session_type_enum_type        session_type,
  const mmgsdi_static_data_type *      aid_ptr,
  const mmgsdi_refresh_evt_info_type * refresh_evt_ptr
)
{
  uint16   i               = 0;
  uint16   num_files       = 0;
  uint16   tlv_len         = 0;
  uint8 *  tlv_data_ptr    = NULL;
  uint8*   curr_ptr        = NULL;
  boolean  ret_value       = FALSE;

  PACKED struct PACKED_POST
  {
    uint8  stage;
    uint8  mode;
    uint8  session_type;
    uint8  aid_len;
  } tlv_data_static_top;

  PACKED struct PACKED_POST
  {
    uint16 num_of_files;
  } tlv_data_static_bottom;

  PACKED struct PACKED_POST
  {
    uint16 file_id;
    uint8  path_len;
  } tlv_data_static_file;

  ASSERT(response_pptr && aid_ptr && refresh_evt_ptr);

  /* Calculate number of files, counting the number of 3F00 */
  num_files = 0;
  if (refresh_evt_ptr->refresh_files.file_path_ptr != NULL)
  {
    for (i = 0; i < refresh_evt_ptr->refresh_files.file_path_len - 1; i += 2)
    {
      if (refresh_evt_ptr->refresh_files.file_path_ptr[i] == 0x3F &&
          refresh_evt_ptr->refresh_files.file_path_ptr[i + 1] == 0x00)
      {
        num_files++;
      }
    }
  }

  /* Calculate maximum memory requirement */
  tlv_len = sizeof(tlv_data_static_top) +
            sizeof(tlv_data_static_bottom) +
            (uint16)aid_ptr->data_len +
            (uint16)(num_files *
               (sizeof(tlv_data_static_file) + ((MMGSDI_MAX_PATH_LEN - 1) * sizeof(uint16))));

  /* Allocate memory for TLV */
  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }
  memset(tlv_data_ptr, 0, tlv_len);

  /* Set pointer to start of buffer */
  curr_ptr = tlv_data_ptr;


  /* Stage */
  switch(refresh_evt_ptr->stage)
  {
    case MMGSDI_REFRESH_STAGE_WAIT_FOR_OK_TO_INIT:
    case MMGSDI_REFRESH_STAGE_WAIT_FOR_OK_TO_FCN:
      tlv_data_static_top.stage = UIM_REFRESH_STAGE_WAIT_FOR_OK_V01;
      break;

    case MMGSDI_REFRESH_STAGE_START:
      tlv_data_static_top.stage = UIM_REFRESH_STAGE_START_V01;
      break;

    case MMGSDI_REFRESH_STAGE_END_SUCCESS:
      tlv_data_static_top.stage = UIM_REFRESH_STAGE_END_WITH_SUCCESS_V01;
      break;

    case MMGSDI_REFRESH_STAGE_END_FAILED:
      tlv_data_static_top.stage = UIM_REFRESH_STAGE_END_WITH_FAILURE_V01;
      break;

    default:
      /* Some stages are not sent to QMI clients */
      uimqmi_free(tlv_data_ptr);
      return FALSE;
  }

  /* Mode */
  switch(refresh_evt_ptr->mode)
  {
    case MMGSDI_REFRESH_RESET:
    case MMGSDI_REFRESH_RESET_AUTO:
      tlv_data_static_top.mode = UIM_REFRESH_MODE_RESET_V01;
      break;

    case MMGSDI_REFRESH_NAA_INIT:
      tlv_data_static_top.mode = UIM_REFRESH_MODE_INIT_V01;
      break;

    case MMGSDI_REFRESH_NAA_INIT_FCN:
      tlv_data_static_top.mode = UIM_REFRESH_MODE_INIT_FCN_V01;
      break;

    case MMGSDI_REFRESH_NAA_FCN:
      tlv_data_static_top.mode = UIM_REFRESH_MODE_FCN_V01;
      break;

    case MMGSDI_REFRESH_NAA_INIT_FULL_FCN:
      tlv_data_static_top.mode = UIM_REFRESH_MODE_INIT_FULL_FCN_V01;
      break;

    case MMGSDI_REFRESH_NAA_APP_RESET:
      tlv_data_static_top.mode = UIM_REFRESH_MODE_APP_RESET_V01;
      break;

    case MMGSDI_REFRESH_3G_SESSION_RESET:
      tlv_data_static_top.mode = UIM_REFRESH_MODE_3G_RESET_V01;
      break;

    default:
      /* Some modes are not sent to QMI clients */
      uimqmi_free(tlv_data_ptr);
      return FALSE;
  }

  /* Session type */
  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_PRIMARY_GW_V01;
      break;

    case MMGSDI_1X_PROV_PRI_SESSION:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_PRIMARY_1X_V01;
      break;

    case MMGSDI_GW_PROV_SEC_SESSION:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_SECONDARY_GW_V01;
      break;

    case MMGSDI_1X_PROV_SEC_SESSION:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_SECONDARY_1X_V01;
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_1:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_NONPROVISIONING_SLOT_1_V01;
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_2:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_NONPROVISIONING_SLOT_2_V01;
      break;

    case MMGSDI_CARD_SESSION_SLOT_1:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_CARD_ON_SLOT_1_V01;
      break;

    case MMGSDI_CARD_SESSION_SLOT_2:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_CARD_ON_SLOT_2_V01;
      break;

    case MMGSDI_GW_PROV_TER_SESSION:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_TERTIARY_GW_V01;
      break;

    case MMGSDI_1X_PROV_TER_SESSION:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_TERTIARY_1X_V01;
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_NONPROVISIONING_SLOT_3_V01;
      break;

    case MMGSDI_CARD_SESSION_SLOT_3:
      tlv_data_static_top.session_type = UIM_SESSION_TYPE_CARD_ON_SLOT_3_V01;
      break;

    default:
      /* Some session types are not sent to QMI clients */
      uimqmi_free(tlv_data_ptr);
      return FALSE;
  }

  /* AID length */
  tlv_data_static_top.aid_len = (uint8)aid_ptr->data_len;

  /* Set number of files to zero initially */
  tlv_data_static_bottom.num_of_files = 0;

  /* Copy top part in the TLV */
  ASSERT(curr_ptr + sizeof(tlv_data_static_top) <= tlv_data_ptr + tlv_len);
  (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                &tlv_data_static_top, sizeof(tlv_data_static_top));
  curr_ptr += sizeof(tlv_data_static_top);

  /* Copy AID in the TLV */
  ASSERT(curr_ptr + aid_ptr->data_len <= tlv_data_ptr + tlv_len);
  (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                aid_ptr->data_ptr, (uint32)aid_ptr->data_len);
  curr_ptr += aid_ptr->data_len;

  /* Copy bottom part in the TLV */
  ASSERT(curr_ptr + sizeof(tlv_data_static_bottom) <= tlv_data_ptr + tlv_len);
  (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                &tlv_data_static_bottom, (uint32)sizeof(tlv_data_static_bottom));
  curr_ptr += sizeof(tlv_data_static_bottom);

  /* Loop thru all the files */
  if (refresh_evt_ptr->refresh_files.file_path_ptr != NULL)
  {
    boolean            error = FALSE;
    mmgsdi_path_type   file_path;

    memset(&file_path, 0, sizeof(file_path));

    for (i = 0; i < refresh_evt_ptr->refresh_files.file_path_len - 1; i += 2)
    {
      /* Check if there is the start of a new file */
      if (refresh_evt_ptr->refresh_files.file_path_ptr[i] == 0x3F &&
          refresh_evt_ptr->refresh_files.file_path_ptr[i + 1] == 0x00)
      {
        /* Store the previous file in the TLV */
        if (file_path.path_len > 0 && file_path.path_len <= MMGSDI_MAX_PATH_LEN && !error)
        {
          /* Increase total number of files */
          tlv_data_static_bottom.num_of_files++;

          /* Calculate file id and path len for TLV */
          tlv_data_static_file.file_id = file_path.path_buf[file_path.path_len - 1];
          tlv_data_static_file.path_len = (uint8)((file_path.path_len - 1) * sizeof(uint16));

          /* Copy file id and path len in the TLV */
          ASSERT(curr_ptr + sizeof(tlv_data_static_file) <= tlv_data_ptr + tlv_len);
          (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                        &tlv_data_static_file, sizeof(tlv_data_static_file));
          curr_ptr += sizeof(tlv_data_static_file);

          /* Copy path in the TLV */
          if (file_path.path_len > 1)
          {
            ASSERT(curr_ptr + tlv_data_static_file.path_len <= tlv_data_ptr + tlv_len);
            (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                          &file_path.path_buf[0], tlv_data_static_file.path_len);
            curr_ptr += tlv_data_static_file.path_len;
          }
        }

        /* Reset file path to start a new file */
        memset(&file_path, 0, sizeof(file_path));
        error = FALSE;
      }

      /* Insert data in the current file */
      if (file_path.path_len < MMGSDI_MAX_PATH_LEN)
      {
        file_path.path_buf[file_path.path_len]  = (uint16)(refresh_evt_ptr->refresh_files.file_path_ptr[i] & 0xFF);
        file_path.path_buf[file_path.path_len]  = file_path.path_buf[file_path.path_len] << 8;
        file_path.path_buf[file_path.path_len] += (uint16)(refresh_evt_ptr->refresh_files.file_path_ptr[i + 1] & 0xFF);

        file_path.path_len += 1;
      }
      else
      {
        UIM_MSG_ERR_0("Path too long... will be discarded");
        error = TRUE;
      }
    }

    /* Insert the last file in the TLV */
    if (file_path.path_len > 0 && file_path.path_len <= MMGSDI_MAX_PATH_LEN && !error)
    {
      /* Increase total number of files */
      tlv_data_static_bottom.num_of_files++;

      /* Calculate file id and path len for TLV */
      tlv_data_static_file.file_id = file_path.path_buf[file_path.path_len - 1];
      tlv_data_static_file.path_len = (uint8)((file_path.path_len - 1) * sizeof(uint16));

      /* Copy file id and path len in the TLV */
      ASSERT(curr_ptr + sizeof(tlv_data_static_file) <= tlv_data_ptr + tlv_len);
      (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                    &tlv_data_static_file, sizeof(tlv_data_static_file));
      curr_ptr += sizeof(tlv_data_static_file);

      /* Copy path in the TLV */
      if (file_path.path_len > 1)
      {
        ASSERT(curr_ptr + tlv_data_static_file.path_len <= tlv_data_ptr + tlv_len);
        (void)memscpy(curr_ptr, tlv_len - (curr_ptr - tlv_data_ptr),
                      &file_path.path_buf[0], tlv_data_static_file.path_len);
        curr_ptr += tlv_data_static_file.path_len;
      }
    }
  }

  /* We need to copy the static part of the TLV containing the
     number of files again into the TLV because the number of files
     might have changed */
  (void)memscpy((void*)(tlv_data_ptr + sizeof(tlv_data_static_top) + aid_ptr->data_len),
                 tlv_len - (sizeof(tlv_data_static_top) + aid_ptr->data_len),
                 (void*)&tlv_data_static_bottom,
                 sizeof(tlv_data_static_bottom));

  /* Update actual length of TLV */
  tlv_len = (uint16)(curr_ptr - tlv_data_ptr);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_refresh_event */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_REFRESH_ENFORCEMENT_POLICY()

  DESCRIPTION
    Composes the TLV content of refresh enforcement policy

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    enforcement_policy  : refresh enforcement policy

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_refresh_enforcement_policy
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  mmgsdi_refresh_enforcement_policy_type    enforcement_policy
)
{
  PACKED struct PACKED_POST
  {
    uint64 refresh_enforcement_policy_mask;
  } tlv_data;

  ASSERT(response_pptr);

  memset(&tlv_data, 0x00, sizeof(tlv_data));

  /* If refresh enforcement is not present then nothing to do */
  if(!enforcement_policy.is_enforcement_present)
  {
    return TRUE;
  }

  if(enforcement_policy.data.force_refresh_navigating_menu)
  {
    tlv_data.refresh_enforcement_policy_mask |= UIM_REFRESH_ENFORCEMENT_POLICY_NAVIGATING_MENU_V01;
  }

  if(enforcement_policy.data.force_refresh_data_call)
  {
    tlv_data.refresh_enforcement_policy_mask |= UIM_REFRESH_ENFORCEMENT_POLICY_DATA_CALL_V01;
  }

  if(enforcement_policy.data.force_refresh_voice_call)
  {
    tlv_data.refresh_enforcement_policy_mask |= UIM_REFRESH_ENFORCEMENT_POLICY_VOICE_CALL_V01;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_refresh_enforcement_policy */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_CAVE()

  DESCRIPTION
    Composes the TLV content of cave authentication

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    cave_result         : result of CAVE algorithm

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_cave
(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  mmgsdi_cave_auth_resp_type  cave_result
)
{
  PACKED struct PACKED_POST
  {
    uint16 cave_len;
    uint8  cave_result[3];
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.cave_len       = sizeof(tlv_data.cave_result);
  tlv_data.cave_result[0] = (uint8)((cave_result >> 16) & 0xFF);
  tlv_data.cave_result[1] = (uint8)((cave_result >> 8) & 0xFF);
  tlv_data.cave_result[2] = (uint8)(cave_result & 0xFF);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_authenticate_cave */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_GSM_ALGO()

  DESCRIPTION
    Composes the TLV content of GSM authentication

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    sres_ptr            : SRES value
    kc_ptr              : Kc value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_gsm_algo
(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  const uint8 *               sres_ptr,
  const uint8 *               kc_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint16 content_len;
    uint8  content[MMGSDI_GSM_ALGO_SRES_LEN + MMGSDI_GSM_ALGO_KC_LEN];
  } tlv_data;

  ASSERT(response_pptr && sres_ptr && kc_ptr);

  tlv_data.content_len = MMGSDI_GSM_ALGO_SRES_LEN + MMGSDI_GSM_ALGO_KC_LEN;

  /* Reconstruct the response from card */
  (void)memscpy((void*)tlv_data.content,
                sizeof(tlv_data.content),
                (void*)sres_ptr,
                MMGSDI_GSM_ALGO_SRES_LEN);
  (void)memscpy((void*)(tlv_data.content + MMGSDI_GSM_ALGO_SRES_LEN),
                sizeof(tlv_data.content) - MMGSDI_GSM_ALGO_SRES_LEN,
                (void*)kc_ptr,
                MMGSDI_GSM_ALGO_KC_LEN);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_authenticate_gsm_algo */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_GENERIC()

  DESCRIPTION
    Composes the TLV content of a generic authentication where entire
    response is passed as it is.

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    auth_response       : result of authenticate

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_generic
(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  mmgsdi_data_type            auth_response
)
{
  boolean  ret_value    = FALSE;
  uint16   tlv_len      = 0;
  uint8  * tlv_data_ptr = NULL;

  ASSERT(response_pptr);

  /* It might be ok to have empty data. In this case, the TLV is not
     added, but we still return SUCCESS */
  if (auth_response.data_len == 0 ||
      auth_response.data_ptr == NULL)
  {
    return TRUE;
  }

  /* Allocate payload */
  tlv_data_ptr = uimqmi_malloc(sizeof(uint16) + auth_response.data_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  tlv_len = (uint16)auth_response.data_len;
  (void)memscpy(tlv_data_ptr, sizeof(uint16) + auth_response.data_len, &tlv_len, sizeof(uint16));
  (void)memscpy(tlv_data_ptr + sizeof(uint16), auth_response.data_len,
                auth_response.data_ptr, auth_response.data_len);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    (uint16)(sizeof(uint16) + auth_response.data_len),
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_authenticate_generic */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_ISIM()

  DESCRIPTION
    Composes the TLV content of ISIM authentication

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    status        : status
    auth_context  : auth context
    res           : RES value
    ck            : CK value
    ik            : IK value
    auts          : AUTS value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_isim
(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  mmgsdi_return_enum_type     status,
  uim_auth_context_enum_v01   auth_context,
  mmgsdi_data_type            res,
  mmgsdi_data_type            ck,
  mmgsdi_data_type            ik,
  mmgsdi_data_type            auts
)
{
  boolean  ret_value    = FALSE;
  uint16   index        = 0;
  uint16   tlv_len      = 0;
  uint8 *  tlv_data_ptr = NULL;
  uint16   len_value    = 0;

  ASSERT(response_pptr);

  /* Compose payload */
  if (status == MMGSDI_SUCCESS)
  {
    ASSERT(res.data_ptr);

    /* Update response length based on the contexts requested:
       - ISIM AKA Authentication returns ck and ik in addition to res
       - HTTP Digest context returns res and ck
       - GBA Auth contexts ck & ik are not needed, so skip it altogether */
    switch (auth_context)
    {
      case UIM_AUTH_CONTEXT_GBA_SEC_BOOTSTRAPPING_V01:
      case UIM_AUTH_CONTEXT_GBA_SEC_NAF_DERIVATION_V01:
        len_value = (uint16)(res.data_len + 2);
        break;
      case UIM_AUTH_CONTEXT_HTTP_DIGEST_SEC_V01:
        len_value = (uint16)(res.data_len + ck.data_len + 3);
        break;
      case UIM_AUTH_CONTEXT_IMS_AKA_SEC_V01:
        len_value = (uint16)(res.data_len + ck.data_len + ik.data_len + 4);
        break;
      default:
        return FALSE;
    }

    tlv_len = len_value + 2;
    tlv_data_ptr = uimqmi_malloc(tlv_len);

    if (tlv_data_ptr == NULL)
    {
      return FALSE;
    }

    (void)memscpy(tlv_data_ptr, tlv_len, &len_value, sizeof(uint16));
    index += 2;

    /* Reconstruct the response from card. Success
       for all ISIM AUTH operations is 0xDB */
    tlv_data_ptr[index++] = 0xDB;

    tlv_data_ptr[index++] = (uint8)res.data_len;
    (void)memscpy((void*)(tlv_data_ptr + index), tlv_len - index,
                  (void*)res.data_ptr, (uint32)res.data_len);
    index += (uint16)res.data_len;

    /* Only ISIM AKA Authentication returns ck and ik in addition to res */
    if ((auth_context == UIM_AUTH_CONTEXT_IMS_AKA_SEC_V01) ||
        (auth_context == UIM_AUTH_CONTEXT_HTTP_DIGEST_SEC_V01))
    {
      tlv_data_ptr[index++] = (uint8)ck.data_len;
      if((ck.data_len > 0) && (ck.data_ptr != NULL))
      {
        (void)memscpy((void*)(tlv_data_ptr + index),
                      tlv_len - index,
                      (void*)ck.data_ptr,
                      (uint32)ck.data_len);
        index += (uint16)ck.data_len;
      }
    }
    if (auth_context == UIM_AUTH_CONTEXT_IMS_AKA_SEC_V01)
    {
      tlv_data_ptr[index++] = (uint8)ik.data_len;
      if ((ik.data_len > 0) && (ik.data_ptr != NULL))
      {
        (void)memscpy((void*)(tlv_data_ptr + index), tlv_len - index,
                      (void*)ik.data_ptr, (uint32)ik.data_len);
        index += (uint16)ik.data_len;
      }
    }
  }
  else if (status == MMGSDI_AUTS_FAIL)
  {
    ASSERT(auts.data_ptr);

    len_value = (uint16)(auts.data_len + 2);

    tlv_len = len_value + 2;
    tlv_data_ptr = uimqmi_malloc(tlv_len);

    if (tlv_data_ptr == NULL)
    {
      return FALSE;
    }

    (void)memscpy(tlv_data_ptr, tlv_len,
                  &len_value, sizeof(uint16));
    index += 2;

    /* Reconstruct the response from card */
    tlv_data_ptr[index++] = 0xDC;

    tlv_data_ptr[index++] = (uint8)auts.data_len;
    (void)memscpy((void*)(tlv_data_ptr + index), tlv_len - index,
                  (void*)auts.data_ptr, (uint32)auts.data_len);
    index += (uint16)auts.data_len;
  }
  else
  {
    return FALSE;
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);

  return ret_value;
} /* qmi_uim_response_authenticate_isim */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SERVICE_STATUS()

  DESCRIPTION
    Composes the TLV content of FDN,ACL service status

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    svc_available : Service is available or not
    svc_enabled   : Service is enabled or not

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_service_status
(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  boolean                     svc_available,
  boolean                     svc_enabled
)
{
  PACKED struct PACKED_POST
  {
    uint8 svc_status;
  } tlv_data;

  ASSERT(response_pptr);

  if (svc_available)
  {
    if (svc_enabled)
    {
      tlv_data.svc_status = 2;
    }
    else
    {
      tlv_data.svc_status = 1;
    }
  }
  else
  {
    tlv_data.svc_status = 0;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_service_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_BOOLEAN()

  DESCRIPTION
    Composes the TLV content from the value of boolean that is passed in

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    is_true       : boolean

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_boolean
(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  boolean                     is_true
)
{
  PACKED struct PACKED_POST
  {
    uint8 is_true;
  } tlv_data;

  ASSERT(response_pptr);

  if (is_true)
  {
    tlv_data.is_true = 1;
  }
  else
  {
    tlv_data.is_true = 0;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_boolean */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_LABEL()

  DESCRIPTION
    Composes the TLV content of application label

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    label_ptr     : label

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_label
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  const mmgsdi_static_data_type * label_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint8 label_len;
    uint8 lable_value[QMI_UIM_LABEL_MAX_V01];
  } tlv_data;

  ASSERT(response_pptr && label_ptr);

  /* Check length */
  if (label_ptr->data_len > MMGSDI_MAX_AID_LEN)
  {
    return FALSE;
  }

  tlv_data.label_len = (uint8)label_ptr->data_len;
  (void)memscpy(tlv_data.lable_value,
                sizeof(tlv_data.lable_value),
                label_ptr->data_ptr,
                tlv_data.label_len);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_data.label_len + sizeof(uint8),
                               (void *)&tlv_data);
} /* qmi_uim_response_label */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_HIDDEN_KEY_STATUS()

  DESCRIPTION
    Composes the TLV content of hidden key status

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    hidden_key_status  : hidden key status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_hidden_key_status
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  mmgsdi_pin_status_enum_type     hidden_key_status
)
{
  PACKED struct PACKED_POST
  {
    uint8 hidden_key_status;
  } tlv_data;

  ASSERT(response_pptr);

  switch(hidden_key_status)
  {
    case MMGSDI_PIN_NOT_FOUND:
    case MMGSDI_PIN_STATUS_NOT_INITIALIZED:
      tlv_data.hidden_key_status = UIM_HIDDEN_KEY_STATUS_NOT_SUPPORTED_V01;
      break;

    case MMGSDI_PIN_ENABLED_NOT_VERIFIED:
      tlv_data.hidden_key_status = UIM_HIDDEN_KEY_STATUS_ENABLED_NOT_VERIFIED_V01;
      break;

    case MMGSDI_PIN_ENABLED_VERIFIED:
      tlv_data.hidden_key_status = UIM_HIDDEN_KEY_STATUS_ENABLED_VERIFIED_V01;
      break;

    case MMGSDI_PIN_DISABLED:
      tlv_data.hidden_key_status = UIM_HIDDEN_KEY_STATUS_DISABLED_V01;
      break;

    case MMGSDI_PIN_BLOCKED:
    case MMGSDI_PIN_PERM_BLOCKED:
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_hidden_key_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_INDEX_IN_DIR()

  DESCRIPTION
    Composes the TLV content of index in EF-DIR

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    index_in_dir       : index in EF-DIR file

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_index_in_dir
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint8                           index_in_dir
)
{
  PACKED struct PACKED_POST
  {
    uint8 index_in_dir;
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.index_in_dir = index_in_dir;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_index_in_dir */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_ESN_STATUS()

  DESCRIPTION
    Composes the TLV content of ESN status

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    esn_status    : ESN is changed or not

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_esn_status
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  mmgsdi_esn_me_change_enum_type  esn_status
)
{
  PACKED struct PACKED_POST
  {
    uint8 esn_status;
  } tlv_data;

  ASSERT(response_pptr);

  switch(esn_status)
  {
    case MMGSDI_ESN_ME_NO_CHANGE:
      tlv_data.esn_status = UIM_ESN_STATUS_NO_CHANGE_V01;
      break;
    case MMGSDI_ESN_ME_CHANGE:
    case MMGSDI_ESN_ME_UPDATE_PENDING:
      tlv_data.esn_status = UIM_ESN_STATUS_CHANGED_V01;
      break;
    default:
      /* Invalid value: return without adding any TLV */
      return TRUE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_esn_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SEND_APDU_RESULT()

  DESCRIPTION
    Composes the TLV content of send APDU result

  PARAMETERS
    tlv_id                  : id to be used for the TLV
    response_pptr           : output sdu
    apdu_data               : response APDU data from the card
    procedure_bytes         : procedure bytes, if applicable

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_send_apdu_result
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  mmgsdi_send_apdu_data_type    apdu_data,
  mmgsdi_sw_type                procedure_bytes
)
{
  boolean ret_value   = TRUE;
  uint16  tlv_len     = 0;
  uint8*  tlv_data    = NULL;
  uint16  temp_uint16 = 0;

  ASSERT(response_pptr);

  /* Data is not present... simply return TRUE.
     This TLV is optional and will be skipped */
  if (apdu_data.data_len == 0 || apdu_data.data_ptr == NULL)
  {
    return TRUE;
  }

  /* Find the total length */
  temp_uint16 = (uint16)apdu_data.data_len;
  if (procedure_bytes.present)
  {
    temp_uint16 += 2 * sizeof(uint8);
  }

  tlv_len = temp_uint16 + sizeof(uint16);

  /* Allocate buffer needed */
  tlv_data = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data == NULL)
  {
    return FALSE;
  }

  /* Copy length */
  (void)memscpy((void*)tlv_data, tlv_len, &temp_uint16, sizeof(uint16));

  /* Copy APDU response from the card */
  (void)memscpy((void*)(tlv_data + sizeof(uint16)),
                tlv_len - sizeof(uint16),
         (void*)apdu_data.data_ptr,
         (uint32)apdu_data.data_len);

  /* Append procedure bytes if needed */
  if (procedure_bytes.present)
  {
    /* We append procedure bytes sw1 & sw2 for the subsequent partial data request,
       Example - <req_data_bytes> 0x61 0x<remaining_len> */
    uint16 offset            = sizeof(uint16) + (uint16)apdu_data.data_len;
    *(tlv_data + offset)     = procedure_bytes.sw1;
    *(tlv_data + offset + 1) = procedure_bytes.sw2;
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data);

  uimqmi_free(tlv_data);

  return ret_value;
} /* qmi_uim_response_send_apdu_result */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PROCEDURE_BYTES()

  DESCRIPTION
    Composes the TLV content of procedure bytes

  PARAMETERS
    tlv_id                  : id to be used for the TLV
    response_pptr           : output sdu
    procedure_bytes         : procedure bytes to be sent

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_procedure_bytes
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  mmgsdi_sw_type                procedure_bytes
)
{
  PACKED struct PACKED_POST
  {
    uint16  apdu_len;
    uint8   apdu_data[2 * sizeof(uint8)];
  } tlv_data;

  ASSERT(response_pptr);

  /* In this case, SW1 and SW2 should be there */
  if (!procedure_bytes.present)
  {
    return FALSE;
  }

  memset(&tlv_data, 0, sizeof(tlv_data));

  /* Update the TLV */
  tlv_data.apdu_len     = 2 * sizeof(uint8);
  tlv_data.apdu_data[0] = procedure_bytes.sw1;
  tlv_data.apdu_data[1] = procedure_bytes.sw2;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_procedure_bytes */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GENERIC_PAYLOAD_8BIT()

  DESCRIPTION
    Composes the TLV content of variable data passed

  PARAMETERS
    tlv_id            : id to be used for the TLV
    response_pptr     : output sdu
    response_data     : response data from MMGSDI for the request
    payload_mandatory : specifies if response_data is to be checked for
                        validity

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_generic_payload_8bit
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  mmgsdi_data_type              response_data,
  boolean                       payload_mandatory
)
{
  boolean ret_value   = TRUE;
  uint16  tlv_len     = 0;
  uint8*  tlv_data    = NULL;
  uint8   temp_uint8  = 0;

  ASSERT(response_pptr);

  /* Check for validity of the payload data */
  if (response_data.data_len == 0 || response_data.data_ptr == NULL)
  {
    /* If payload is mandatory for the response of the called function,
       this is an error condition.
       Otherwise, return TRUE since this TLV is optional & will be skipped */
    if (payload_mandatory)
    {
      ret_value = FALSE;
    }
    return ret_value;
  }

  /* Length value is 1 byte for both of these TLV types */
  tlv_len = (uint8)response_data.data_len + sizeof(uint8);

  tlv_data = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data == NULL)
  {
    return FALSE;
  }

  /* Copy length */
  temp_uint8 = (uint8)response_data.data_len;
  (void)memscpy((void*)tlv_data, tlv_len, &temp_uint8, sizeof(uint8));

  /* Copy response from the card */
  (void)memscpy((void*)(tlv_data + sizeof(uint8)),
                tlv_len - sizeof(uint8),
                (void*)response_data.data_ptr,
                temp_uint8);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data);

  uimqmi_free(tlv_data);

  return ret_value;
} /* qmi_uim_response_generic_payload_8bit */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GENERIC_PAYLOAD_16BIT()

  DESCRIPTION
    Composes the TLV content of variable data passed

  PARAMETERS
    tlv_id            : id to be used for the TLV
    response_pptr     : output sdu
    response_data     : response data from MMGSDI for the request
    payload_mandatory : specifies if response_data is to be checked for
                        validity

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_generic_payload_16bit
(
  byte                  tlv_id,
  dsm_item_type **      response_pptr,
  mmgsdi_data_type      response_data,
  boolean               payload_mandatory
)
{
  boolean ret_value   = TRUE;
  uint16  tlv_len     = 0;
  uint8*  tlv_data    = NULL;
  uint16  temp_uint16 = 0;

  ASSERT(response_pptr);

  /* Check for validity of the payload data */
  if (response_data.data_len == 0 || response_data.data_ptr == NULL)
  {
    /* If payload is mandatory for the response of the called function,
       this is an error condition.
       Otherwise, return TRUE since this TLV is optional & will be skipped */
    if (payload_mandatory)
    {
      ret_value = FALSE;
    }
    return ret_value;
  }

  /* Length value is 2 bytes for both of these TLV types */
  tlv_len = (uint16)response_data.data_len + sizeof(uint16);

  tlv_data = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data == NULL)
  {
    return FALSE;
  }

  /* Copy length */
  temp_uint16 = (uint16)response_data.data_len;
  (void)memscpy((void*)tlv_data, tlv_len, &temp_uint16, sizeof(uint16));

  /* Copy content */
  (void)memscpy((void*)(tlv_data + sizeof(uint16)),
                tlv_len - sizeof(uint16),
                (void*)response_data.data_ptr,
                temp_uint16);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data);

  uimqmi_free(tlv_data);
  return ret_value;
} /* qmi_uim_response_generic_payload_16bit */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_LOGICAL_CHANNEL()

  DESCRIPTION
    Composes the TLV content of logical channel

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    logical_channel    : logical channel

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_logical_channel
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint8                           logical_channel
)
{
  PACKED struct PACKED_POST
  {
    uint8 logical_channel;
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.logical_channel = logical_channel;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_logical_channel */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_HOT_SWAP_STATUS()

  DESCRIPTION
    Composes the hot swap switch status TLV content for get card status
    request

  PARAMETERS
    tlv_id                : id to be used for the TLV
    response_pptr         : output sdu
    extended_status       : if it is extended hot swap status
    hot_swap_status_len   : length of hot_swap_status_ptr array
    hot_swap_status_ptr   : array with status of hot swap switch for each slot

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_hot_swap_status
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  boolean                         extended_status,
  uint8                           hot_swap_status_len,
  uim_rt_status_enum *            hot_swap_status_ptr
)
{
  uint8              slot         = 0;
  uint8              tlv_len      = 0;
  uint8              hot_swap_len = 0;

  PACKED struct PACKED_POST
  {
    uint8 num_cards;
    uint8 hot_swap[QMI_UIM_MAX_CARD_COUNT];
  } tlv_data;

  uim_rt_status_enum status  = UIM_RT_STATUS_UNKNOWN;

  ASSERT(response_pptr && hot_swap_status_ptr);

  if (extended_status)
  {
    hot_swap_len = (QMI_UIM_MAX_CARD_COUNT < hot_swap_status_len) ?
                     QMI_UIM_MAX_CARD_COUNT : hot_swap_status_len;
  }
  else
  {
    hot_swap_len = (QMI_UIM_LEGACY_MAX_CARD_COUNT < hot_swap_status_len) ?
                     QMI_UIM_LEGACY_MAX_CARD_COUNT : hot_swap_status_len;
  }

  tlv_data.num_cards = hot_swap_len;
  for(slot = 0;
      slot < QMI_UIM_MAX_CARD_COUNT && slot < hot_swap_len;
      slot++)
  {
    status = hot_swap_status_ptr[slot];
    switch(status)
    {
      case UIM_RT_STATUS_CARD_PRESENT:
        tlv_data.hot_swap[slot] = UIM_HOT_SWAP_STATUS_PRESENT_V01;
        break;
      case UIM_RT_STATUS_CARD_ABSENT:
        tlv_data.hot_swap[slot] = UIM_HOT_SWAP_STATUS_NOT_PRESENT_V01;
        break;
      case UIM_RT_STATUS_NOT_SUPPORTED:
      default:
        tlv_data.hot_swap[slot] = UIM_HOT_SWAP_NOT_SUPPORTED_V01;
        break;
    }
  }

  tlv_len = sizeof(uint8) + (sizeof(uint8) * hot_swap_len);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data);
}/* qmi_uim_response_hot_swap_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_FILE_LENGTH()

  DESCRIPTION
    Composes the file length TLV

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    file_length        : file length

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_file_length
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint16                          file_length
)
{
  PACKED struct PACKED_POST
  {
    uint16 file_length;
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.file_length = file_length;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_file_length */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_APDU_LONG_RESPONSE()

  DESCRIPTION
    Composes the TLV content of send APDU long response

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    token         : Token used in the subsequent indications
    total_len     : Total length of the response APDU from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_apdu_long_response
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  uint32                        token,
  uint16                        total_len
)
{
  boolean   ret_value    = TRUE;

  PACKED struct PACKED_POST
  {
    uint16 total_len;
    uint32 token;
  } tlv_data;

  ASSERT(response_pptr);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.total_len = total_len;
  tlv_data.token     = token;

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    sizeof(tlv_data),
                                    (void *)&tlv_data);

  return ret_value;
} /* qmi_uim_response_apdu_long_response */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_LONG_RESPONSE_CHUNK()

  DESCRIPTION
    Composes the TLV content of a long response chunk. This is currently used
    in cases of send APDU and read transparent indications.

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    token         : Token used in the previous long response
    total_len     : Total length of the response data from the card
    offset        : Offset of the current chunk of response data
    long_data     : One chunk of the long response data from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_long_response_chunk
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  uint32                        token,
  uint16                        total_len,
  uint16                        offset,
  mmgsdi_data_type              long_data
)
{
  boolean   ret_value    = TRUE;
  uint16    tlv_len      = 0;
  uint8   * tlv_data_ptr = NULL;

  PACKED struct PACKED_POST
  {
    uint32 token;
    uint16 total_len;
    uint16 offset;
    uint16 data_len;
  } tlv_data;

  ASSERT(response_pptr);

  /* If data is not present... return FALSE.
     This TLV is mandatory and will be rejected */
  if (long_data.data_len == 0 || long_data.data_ptr == NULL)
  {
    return FALSE;
  }

  /* First update the static fields */
  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.token     = token;
  tlv_data.total_len = total_len;
  tlv_data.offset    = offset;
  tlv_data.data_len  = (uint16)long_data.data_len;

  tlv_len      = tlv_data.data_len + sizeof(tlv_data);
  tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  memset(tlv_data_ptr, 0, tlv_len);

  /* Copy static fields of TLV */
  (void)memscpy((void*)tlv_data_ptr, tlv_len, &tlv_data, sizeof(tlv_data));

  /* Copy APDU data */
  (void)memscpy((void*)(tlv_data_ptr+sizeof(tlv_data)),
                tlv_len - sizeof(tlv_data),
                (void*)long_data.data_ptr,
                (uint32)tlv_data.data_len);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);

  return ret_value;
} /* qmi_uim_response_long_response_chunk */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_STATUS_VALIDITY()

  DESCRIPTION
    Composes the card status validity TLV content for get card status
    request

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    validity_len    : length of validity status array
    validity_ptr    : pointer to validity status array

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_status_validity
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint8                           validity_len,
  boolean                       * validity_ptr
)
{
  uint8              slot     = 0;
  uint8              tlv_len  = 0;
  boolean            skip_tlv = TRUE;

  PACKED struct PACKED_POST
  {
    uint8 num_cards;
    uint8 card_status_valid[QMI_UIM_MAX_CARD_COUNT];
  } tlv_data;

  ASSERT(response_pptr);

  /* Skip TLV on error input */
  if ((validity_ptr == NULL) ||
      (validity_len == 0) ||
      (validity_len > QMI_UIM_MAX_CARD_COUNT))
  {
    return FALSE;
  }

  /* Skip this TLV in case status in all slots are determined to be valid */
  for(slot = 0; slot < validity_len; slot++)
  {
    if (!validity_ptr[slot])
    {
      skip_tlv = FALSE;
      break;
    }
  }

  if (skip_tlv)
  {
    return TRUE;
  }

  memset(&tlv_data, 0, sizeof(tlv_data));

  tlv_data.num_cards = validity_len;
  tlv_len = sizeof(uint8) + sizeof(uint8)*validity_len;

  for(slot = 0; slot < QMI_UIM_MAX_CARD_COUNT && slot < validity_len; slot++)
  {
    tlv_data.card_status_valid[slot] = (validity_ptr[slot]) ? 1 : 0;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data);
} /* qmi_uim_response_card_status_validity */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SIM_BUSY()

  DESCRIPTION
    Composes the sim busy TLV content for get card status request.

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    card_state_ptr  : structure containing num of cards and sim busy status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_sim_busy
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  const qmi_uim_card_state      * card_state_ptr
)
{
  uint8          slot         = 0;
  uint8          tlv_len      = 0;

  PACKED struct PACKED_POST
  {
    uint8    num_cards;
    uint8    busy_status[QMI_UIM_MAX_CARD_COUNT];
  } tlv_data;

  ASSERT(response_pptr && card_state_ptr);

  memset(&tlv_data, 0, sizeof(tlv_data));

  tlv_data.num_cards = card_state_ptr->num_slots;

  tlv_len = sizeof(uint8);

  for(slot = 0; slot < QMI_UIM_MAX_CARD_COUNT &&
                slot < card_state_ptr->num_slots ; slot++)
  {
    tlv_data.busy_status[slot] = ( card_state_ptr->card[slot]->sim_busy ) ? 1 : 0;
    tlv_len += sizeof(uint8);
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data);
} /* qmi_uim_response_sim_busy */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_MODE()

  DESCRIPTION
    Composes the card mode TLV content for get card status response and indication.

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    card_state_ptr  : structure containing num of cards and card mode

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_mode
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  const qmi_uim_card_state      * card_state_ptr
)
{
  uint8          slot         = 0;
  uint8          tlv_len      = 0;

  PACKED struct PACKED_POST
  {
    uint8     num_cards;
    uint32    card_mode_status[QMI_UIM_MAX_CARD_COUNT];
  } tlv_data;

  ASSERT(response_pptr && card_state_ptr);

  memset(&tlv_data, 0, sizeof(tlv_data));

  tlv_data.num_cards = card_state_ptr->num_slots;

  tlv_len = sizeof(uint8);

  for(slot = 0; slot < QMI_UIM_MAX_CARD_COUNT &&
                slot < card_state_ptr->num_slots ; slot++)
  {
    if (card_state_ptr->card[slot]->card_state == QMI_UIM_CARD_STATE_PRESENT &&
        card_state_ptr->card[slot]->is_non_telecom_card)
    {
      tlv_data.card_mode_status[slot] = UIM_CARD_MODE_NON_TELECOM_CARD_V01;
    }
    else
    {
      tlv_data.card_mode_status[slot] = UIM_CARD_MODE_TELECOM_CARD_V01;
    }
    tlv_len += sizeof(uint32);
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data);
} /* qmi_uim_response_card_mode */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SLOT()

  DESCRIPTION
    Composes the TLV content for Slot ID

  PARAMETERS
    slot_id : MMGSDI slot information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_slot
(
  byte                        tlv_id,
  dsm_item_type            ** response_pptr,
  mmgsdi_slot_id_enum_type    slot
)
{
  PACKED struct PACKED_POST
  {
    uint8 slot;
  } tlv_data;

  ASSERT(response_pptr);

  /* Update slot ID */
  switch(slot)
  {
    case MMGSDI_SLOT_1:
      tlv_data.slot = UIM_SLOT_1_V01;
      break;
    case MMGSDI_SLOT_2:
      tlv_data.slot = UIM_SLOT_2_V01;
      break;
    case MMGSDI_SLOT_3:
      tlv_data.slot = UIM_SLOT_3_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_slot */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_ACTIVATION_STATUS()

  DESCRIPTION
    Composes the TLV content for card activation status

  PARAMETERS
    tlv_id            : id to be used for the TLV
    response_pptr     : output sdu
    gstk_otasp_status : contains GSTK OTASP activation status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_activation_status
(
  byte                           tlv_id,
  dsm_item_type               ** response_pptr,
  gstk_otasp_act_status_enum_type    gstk_otasp_status
)
{
  PACKED struct PACKED_POST
  {
    uint32 otasp_status;
  } tlv_data;

  ASSERT(response_pptr);

  /* Update OTASP status */
  switch(gstk_otasp_status)
  {
    case GSTK_OTASP_ACT_STATUS_START:
      tlv_data.otasp_status = CARD_ACTIVATION_START_V01;
      break;
    case GSTK_OTASP_ACT_STATUS_SUCCESS:
      tlv_data.otasp_status = CARD_ACTIVATION_END_WITH_SUCCESS_V01;
      break;
    case GSTK_OTASP_ACT_STATUS_PERM_FAIL:
      tlv_data.otasp_status = CARD_ACTIVATION_END_WITH_FAILURE_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_activation_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AID()

  DESCRIPTION
    Composes the TLV content for AID

  PARAMETERS
    aid_ptr : AID information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_aid
(
  byte                             tlv_id,
  dsm_item_type                 ** response_pptr,
  const mmgsdi_static_data_type  * aid_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint8 aid_len;
    uint8 aid_value[QMI_UIM_AID_MAX_V01];
  } tlv_data;

  ASSERT(response_pptr && aid_ptr);

  /* Check for AID length validity */
  if (aid_ptr->data_len > MMGSDI_MAX_AID_LEN)
  {
    return FALSE;
  }

  tlv_data.aid_len = (uint8)aid_ptr->data_len;
  (void)memscpy(tlv_data.aid_value,
                sizeof(tlv_data.aid_value),
                aid_ptr->data_ptr,
                tlv_data.aid_len);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_data.aid_len + sizeof(uint8),
                               (void *)&tlv_data);
} /* qmi_uim_response_aid */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_FILE_STATUS()

  DESCRIPTION
    Composes the TLV content for file status

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    file_attrib_ptr  : Pointer to file attributes response

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_file_status
(
  byte                                 tlv_id,
  dsm_item_type                     ** response_pptr,
  const mmgsdi_file_attributes_type  * file_attrib_ptr
)
{
  boolean file_invalidated = FALSE;

  PACKED struct PACKED_POST
  {
    uint32 file_status;
  } tlv_data;

  ASSERT(response_pptr && file_attrib_ptr);

  switch (file_attrib_ptr->file_type)
  {
     case MMGSDI_LINEAR_FIXED_FILE:
       file_invalidated =
         file_attrib_ptr->file_info.linear_fixed_file.file_status.file_invalidated;
       break;
     case MMGSDI_CYCLIC_FILE:
       file_invalidated =
         file_attrib_ptr->file_info.cyclic_file.file_status.file_invalidated;
       break;
     case MMGSDI_TRANSPARENT_FILE:
       file_invalidated =
         file_attrib_ptr->file_info.transparent_file.file_status.file_invalidated;
       break;
     default:
       return FALSE;
  }

  /* Update the value */
  tlv_data.file_status = file_invalidated ? 0: 1;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_file_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_TYPE()

  DESCRIPTION
    Composes the TLV content for Session type

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    session_type     : MMGSDI session type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_type
(
  byte                            tlv_id,
  dsm_item_type                ** response_pptr,
  mmgsdi_session_type_enum_type   session_type
)
{
  PACKED struct PACKED_POST
  {
    uint8 session_type;
  } tlv_data;

  ASSERT(response_pptr);

  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      tlv_data.session_type = UIM_SESSION_TYPE_PRIMARY_GW_V01;
      break;
    case MMGSDI_1X_PROV_PRI_SESSION:
      tlv_data.session_type = UIM_SESSION_TYPE_PRIMARY_1X_V01;
      break;
    case MMGSDI_GW_PROV_SEC_SESSION:
      tlv_data.session_type = UIM_SESSION_TYPE_SECONDARY_GW_V01;
      break;
    case MMGSDI_1X_PROV_SEC_SESSION:
      tlv_data.session_type = UIM_SESSION_TYPE_SECONDARY_1X_V01;
      break;
    case MMGSDI_GW_PROV_TER_SESSION:
      tlv_data.session_type = UIM_SESSION_TYPE_TERTIARY_GW_V01;
      break;
    case MMGSDI_1X_PROV_TER_SESSION:
      tlv_data.session_type = UIM_SESSION_TYPE_TERTIARY_1X_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_session_type */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_CLOSED_CAUSE()

  DESCRIPTION
    Composes the TLV content for session closure cause

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    cause            : Session close cause

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_closed_cause
(
  byte                                    tlv_id,
  dsm_item_type                        ** response_pptr,
  mmgsdi_session_closed_cause_enum_type   cause
)
{
  PACKED struct PACKED_POST
  {
    uint32 cause;
  } tlv_data;

  ASSERT(response_pptr);

  switch(cause)
  {
    case MMGSDI_SESSION_CLOSED_CAUSE_CLIENT_REQUEST:
    case MMGSDI_SESSION_CLOSED_CAUSE_MISMATCH_FOUND:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_CLIENT_REQUEST_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_CARD_ERROR:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_CARD_ERROR_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_CARD_POWER_DOWN:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_CARD_POWER_DOWN_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_CARD_REMOVED:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_CARD_REMOVED_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_REFRESH:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_REFRESH_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_PIN_STATUS_FAIL:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_PIN_STATUS_FAIL_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_RECOVERY:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_RECOVERY_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_FDN_SRV_ENABLED_WITH_NO_FDN_SUPPORT:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_FDN_SRV_ENABLED_WITH_NO_FDN_SUPPORT_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_PERSO_OPERATION_FAIL:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_PERSO_OPERATION_FAIL_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_FILE_CONTENT_INVALID_IMSI:
    case MMGSDI_SESSION_CLOSED_CAUSE_FILE_CONTENT_INVALID_ACC:
    case MMGSDI_SESSION_CLOSED_CAUSE_FILE_CONTENT_INVALID_PRL:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_FILE_CONTENT_INVALID_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_AD:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_IMSI:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_ACC:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_HPPLMN:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_LOCI:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_PSLOCI:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_KEYS:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_FPLMN:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_START_HFN:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_THRESHOLD:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_KC:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_PHASE:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_CST:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_RUIM_ID:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_SERVICE_PREF:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_IMSI_M:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_IMSI_T:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_ACCOLC:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_DIST_BASED_REGN_IND:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_SYS_REGN_IND:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_OTAPA_SPC_ENABLE:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_NAM_LOCK:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_HOME_SID_NID:
    case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_ZONE_BASED_REGN_IND:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_V01;
      break;
    case MMGSDI_SESSION_CLOSED_CAUSE_UNKNOWN:
    default:
      tlv_data.cause = UIM_SESSION_CLOSED_CAUSE_UNKNOWN_V01;
      break;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_session_closed_cause */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_CLOSED_FILE_ID()

  DESCRIPTION
    Composes the TLV content for the File ID

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    cause            : Session close cause

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_closed_file_id
(
  byte                                   tlv_id,
  dsm_item_type                       ** response_pptr,
  mmgsdi_session_closed_cause_enum_type  cause
)
{
  PACKED struct PACKED_POST
  {
    uint16 file_id;
  } tlv_data;

  ASSERT(response_pptr);

  switch (cause)
  {
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_AD:
       tlv_data.file_id = 0x6FAD;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_IMSI:
     case MMGSDI_SESSION_CLOSED_CAUSE_FILE_CONTENT_INVALID_IMSI:
       tlv_data.file_id = 0x6F07;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_ACC:
     case MMGSDI_SESSION_CLOSED_CAUSE_FILE_CONTENT_INVALID_ACC:
       tlv_data.file_id = 0x6F78;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_HPPLMN:
       tlv_data.file_id = 0x6F31;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_LOCI:
       tlv_data.file_id = 0x6F7E;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_PSLOCI:
       tlv_data.file_id = 0x6F73;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_KEYS:
       tlv_data.file_id = 0x6F08;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_FPLMN:
       tlv_data.file_id = 0x6F7B;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_START_HFN:
       tlv_data.file_id = 0x6F5B;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_THRESHOLD:
       tlv_data.file_id = 0x6F5C;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_PHASE:
       tlv_data.file_id = 0x6FAE;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_CST:
       tlv_data.file_id = 0x6F32;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_RUIM_ID:
       tlv_data.file_id = 0x6F31;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_SERVICE_PREF:
       tlv_data.file_id = 0x6F37;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_IMSI_M:
       tlv_data.file_id = 0x6F22;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_IMSI_T:
       tlv_data.file_id = 0x6F23;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_ACCOLC:
       tlv_data.file_id = 0x6F2C;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_DIST_BASED_REGN_IND:
       tlv_data.file_id = 0x6F2B;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_SYS_REGN_IND:
       tlv_data.file_id = 0x6F2A;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_OTAPA_SPC_ENABLE:
       tlv_data.file_id = 0x6F34;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_NAM_LOCK:
       tlv_data.file_id = 0x6F35;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_HOME_SID_NID:
       tlv_data.file_id = 0x6F28;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_MANDATORY_FILE_MISSING_ZONE_BASED_REGN_IND:
       tlv_data.file_id = 0x6F29;
       break;
     case MMGSDI_SESSION_CLOSED_CAUSE_FILE_CONTENT_INVALID_PRL:
       tlv_data.file_id = 0x6F30;
       break;
     default:
       return TRUE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_session_closed_file_id */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SE13_TABLE_RELEASE_DATE()

  DESCRIPTION
    Composes the TLV content for the SE13 table release date

  PARAMETERS
    tlv_id                          : id to be used for the TLV
    response_pptr                   : output sdu
    se13_table_release_info_ptr     : contains se13 release date and source

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_se13_table_release_date
(
  byte                                     tlv_id,
  dsm_item_type                         ** response_pptr,
  const mmgsdi_se13_table_details_info   * se13_table_details_info_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint8   se13_release_month;
    uint8   se13_release_day;
    uint16  se13_release_year;
  } tlv_data;

  ASSERT(response_pptr && se13_table_details_info_ptr);

  tlv_data.se13_release_month = se13_table_details_info_ptr->se13_date.mmgsdi_se13_release_month;
  tlv_data.se13_release_day = se13_table_details_info_ptr->se13_date.mmgsdi_se13_release_day;
  tlv_data.se13_release_year = se13_table_details_info_ptr->se13_date.mmgsdi_se13_release_year;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_se13_table_release_date */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SE13_TABLE_RELEASE_SOURCE()

  DESCRIPTION
    Composes the TLV content for the SE.13 table source

  PARAMETERS
    tlv_id                          : id to be used for the TLV
    response_pptr                   : output sdu
    se13_table_release_info_ptr     : contains se13 release date and source

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_se13_table_release_source
(
  byte                                      tlv_id,
  dsm_item_type                          ** response_pptr,
  const mmgsdi_se13_table_details_info    * se13_table_details_info_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint32 source;
  } tlv_data;

  ASSERT(response_pptr && se13_table_details_info_ptr);

  switch(se13_table_details_info_ptr->se13_source)
  {
    case MMGSDI_SE13_HARDCODED_SOURCE:
      tlv_data.source = UIM_PLMN_NAME_TABLE_SOURCE_HARDCODED_V01;
      break;
    case MMGSDI_SE13_EFS_SOURCE:
      tlv_data.source = UIM_PLMN_NAME_TABLE_SOURCE_EFS_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_se13_table_release_source */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_INCREASE_RESULT()

  DESCRIPTION
    Composes the TLV content of read result

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    increase_result : increase data from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_increase_result
(
  byte                         tlv_id,
  dsm_item_type             ** response_pptr,
  mmgsdi_increase_data_type    increase_data
)
{
  boolean ret_value    = TRUE;
  uint8   tlv_len      = 0;
  uint8*  tlv_data     = NULL;
  uint8   temp_uint8   = 0;

  ASSERT(response_pptr);

  /* Data is not present... simply return TRUE.
     This TLV is optional and will be skipped */
  if (increase_data.data_len == 0 || increase_data.data_ptr == NULL)
  {
    return TRUE;
  }

  tlv_len = (uint8)increase_data.data_len + sizeof(uint8);
  tlv_data = (uint8*)uimqmi_malloc(tlv_len);
  if (tlv_data == NULL)
  {
    return FALSE;
  }

  /* Copy length */
  temp_uint8 = (uint8)increase_data.data_len;
  (void)memscpy((void*)tlv_data, tlv_len, &temp_uint8, sizeof(uint8));

  /* Copy content */
  (void)memscpy((void*)(tlv_data + sizeof(uint8)),
                tlv_len - sizeof(uint8),
                (void*)increase_data.data_ptr,
                (uint32)increase_data.data_len);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data);

  uimqmi_free(tlv_data);
  return ret_value;
} /* qmi_uim_response_increase_result */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_VCC_STATE()

  DESCRIPTION
    Composes the TLV content for Vcc state

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    ldo_state       : UIM LDO state information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_vcc_state
(
  byte                              tlv_id,
  dsm_item_type                  ** response_pptr,
  uimdrv_qmi_indications_type       ldo_state
)
{
  PACKED struct PACKED_POST
  {
    uint32 ldo_state;
  } tlv_data;

  ASSERT(response_pptr);

  switch(ldo_state)
  {
    case UIMDRV_LDO_ACTIVATED_IND:
      tlv_data.ldo_state = UIM_SUPPLY_VOLTAGE_ACTIVATE_V01;
      break;
    case UIMDRV_LDO_AWAITING_DEACTIVATION_IND:
      tlv_data.ldo_state = UIM_SUPPLY_VOLTAGE_DEACTIVATE_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_vcc_state */


/*===========================================================================
  FUNCTION  QMI_UIM_RESPONSE_ENCRYPTED_STATUS()

  DESCRIPTION
    Encrypts and builds the response returned to security sensitive clients

  PARAMETERS
    tlv_id             : id to be used for the TLV
    cmd_id             : Command id
    request_id         : Request id
    result             : SimLock result status
    response_pptr      : Output sdu

  RETURN VALUE
    errval             : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_response_encrypted_status
(
  byte                         tlv_id,
  uint16                       cmd_id,
  uint16                       request_id,
  qmi_error_e_type             error,
  dsm_item_type **             response_pptr
)
{
  qmi_error_e_type        errval              = QMI_ERR_NONE;
  mmgsdi_data_type        rsp_data            = { 0, NULL };
  mmgsdi_data_type        encrypted_rsp_data  = { 0, NULL };
  boolean                 encrypted           = FALSE;

  PACKED struct PACKED_POST
  {
    uint16   cmd_id;
    uint16   request_id;
    uint8    rfu[QMI_UIM_ENCRYPTED_MSG_RFU_SIZE];
    uint16   result;
    uint16   error;
  } rsp_data_buffer;

  if(response_pptr == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  memset(&rsp_data_buffer, 0x00, sizeof(rsp_data_buffer));

  rsp_data_buffer.cmd_id = cmd_id;
  rsp_data_buffer.request_id = request_id;

  if (error != QMI_ERR_NONE)
  {
    rsp_data_buffer.result = QMI_RESULT_FAILURE;
  }
  else
  {
    rsp_data_buffer.result = QMI_RESULT_SUCCESS;
  }

  rsp_data_buffer.error = error;

  rsp_data.data_len = sizeof(rsp_data_buffer);
  rsp_data.data_ptr = (uint8*)&rsp_data_buffer;

  encrypted = qmi_uim_encrypt_payload(rsp_data,
                                      &encrypted_rsp_data);

  if (encrypted &&
      encrypted_rsp_data.data_len <= QMI_UIM_ENCRYPTED_RESP_MAX &&
      encrypted_rsp_data.data_ptr != NULL)
  {
    if (FALSE == qmi_uim_response_generic_payload_16bit(
                   tlv_id,
                   response_pptr,
                   encrypted_rsp_data,
                   FALSE))
    {
      dsm_free_packet(response_pptr);
      errval = QMI_ERR_NO_MEMORY;
    }
  }
  else
  {
    UIM_MSG_ERR_2("Encrypted: 0x%x, Encrypted data length: 0x%x",
                  encrypted,
                  encrypted_rsp_data.data_len);
    errval = QMI_ERR_INTERNAL;
  }

  if(encrypted_rsp_data.data_ptr != NULL)
  {
    uimqmi_free(encrypted_rsp_data.data_ptr);
    encrypted_rsp_data.data_ptr = NULL;
  }

  return errval;
} /* qmi_uim_response_encrypted_status */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_ENCRYPTED_PIN1()

  DESCRIPTION
    Composes the TLV content of the encrypted PIN1 value

  PARAMETERS
    tlv_id                : id to be used for the TLV
    response_pptr         : output sdu
    encrypted_pin1_length : encrypted PIN1 data length
    encrypted_pin1_ptr    : encrypted PIN1 data pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_encrypted_pin1
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  uint8                         encrypted_pin1_length,
  const uint8 *                 encrypted_pin1_ptr
)
{
  uint8     * encr_pin1_value_ptr = NULL;
  boolean     ret_value           = TRUE;

  ASSERT(response_pptr);

  /* Data is not present... simply return TRUE.
     This TLV is optional and will be skipped */
  if (encrypted_pin1_length == 0 || encrypted_pin1_ptr == NULL)
  {
    return TRUE;
  }

  /* Allocate buffer needed (one more byte of the length field ) */
  encr_pin1_value_ptr = (uint8*)uimqmi_malloc(encrypted_pin1_length + sizeof(uint8));
  if (encr_pin1_value_ptr == NULL)
  {
    return FALSE;
  }

  /* Copy length */
  (void)memscpy((void*)encr_pin1_value_ptr,
                encrypted_pin1_length,
                &encrypted_pin1_length,
                sizeof(uint8));

  /* Copy PIN data */
  (void)memscpy((void*)(encr_pin1_value_ptr + sizeof(uint8)),
                encrypted_pin1_length,
                (void*)encrypted_pin1_ptr,
                (uint32)encrypted_pin1_length);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    encrypted_pin1_length  + sizeof(uint8),
                                    (void *)encr_pin1_value_ptr);
  uimqmi_free(encr_pin1_value_ptr);
  return ret_value;
} /* qmi_uim_response_encrypted_pin1 */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_INFORMATION()

  DESCRIPTION
    Composes the TLV content for Session information

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    session_type     : MMGSDI session type
    aid_len          : Length of AID
    aid_ptr          : Pointer to AID data

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_information
(
  byte                             tlv_id,
  dsm_item_type                 ** response_pptr,
  mmgsdi_session_type_enum_type    session_type,
  uint8                            aid_len,
  const uint8                    * aid_ptr
)
{
  uint16   tlv_len      = 0;

  PACKED struct PACKED_POST
  {
    uint8 session_type;
    uint8 aid_len;
    uint8 aid_value[QMI_UIM_AID_MAX_V01];
  } tlv_data_session;

  ASSERT(response_pptr);

  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      tlv_data_session.session_type = UIM_SESSION_TYPE_PRIMARY_GW_V01;
      tlv_data_session.aid_len = 0;
      break;
    case MMGSDI_1X_PROV_PRI_SESSION:
      tlv_data_session.session_type = UIM_SESSION_TYPE_PRIMARY_1X_V01;
      tlv_data_session.aid_len = 0;
      break;
    case MMGSDI_GW_PROV_SEC_SESSION:
      tlv_data_session.session_type = UIM_SESSION_TYPE_SECONDARY_GW_V01;
      tlv_data_session.aid_len = 0;
      break;
    case MMGSDI_1X_PROV_SEC_SESSION:
      tlv_data_session.session_type = UIM_SESSION_TYPE_SECONDARY_1X_V01;
      tlv_data_session.aid_len = 0;
      break;
    case MMGSDI_GW_PROV_TER_SESSION:
      tlv_data_session.session_type = UIM_SESSION_TYPE_TERTIARY_GW_V01;
      tlv_data_session.aid_len = 0;
      break;
    case MMGSDI_1X_PROV_TER_SESSION:
      tlv_data_session.session_type = UIM_SESSION_TYPE_TERTIARY_1X_V01;
      tlv_data_session.aid_len = 0;
      break;
    case MMGSDI_NON_PROV_SESSION_SLOT_1:
      if (aid_ptr == NULL || aid_len == 0 || aid_len > QMI_UIM_AID_MAX_V01)
      {
        return FALSE;
      }
      tlv_data_session.session_type = UIM_SESSION_TYPE_NONPROVISIONING_SLOT_1_V01;
      tlv_data_session.aid_len = aid_len;
      break;
    case MMGSDI_NON_PROV_SESSION_SLOT_2:
      if (aid_ptr == NULL || aid_len == 0 || aid_len > QMI_UIM_AID_MAX_V01)
      {
        return FALSE;
      }
      tlv_data_session.session_type = UIM_SESSION_TYPE_NONPROVISIONING_SLOT_2_V01;
      tlv_data_session.aid_len = aid_len;
      break;
    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      if (aid_ptr == NULL || aid_len == 0 || aid_len > QMI_UIM_AID_MAX_V01)
      {
        return FALSE;
      }
      tlv_data_session.session_type = UIM_SESSION_TYPE_NONPROVISIONING_SLOT_3_V01;
      tlv_data_session.aid_len = aid_len;
      break;
    default:
      UIM_MSG_ERR_1("Invalid session type: 0x%x", session_type);
      return FALSE;
  }

  tlv_len = QMI_UIM_TLV_MIN_LEN_SESSION_INFORMATION + tlv_data_session.aid_len;

  if (tlv_data_session.aid_len > 0)
  {
    (void)memscpy((void *)tlv_data_session.aid_value,
                  sizeof(tlv_data_session.aid_value) ,
                  (void *)aid_ptr,
                  aid_len);
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data_session);
} /* qmi_uim_response_session_information */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_TEMPORARY_IDENTITY_MASK()

  DESCRIPTION
    Composes the TLV content for temporary identity mask

  PARAMETERS
    tlv_id                     : TLV id to be sent
    response_pptr              : Pointer to response pointer
    temporary_identity         : temporary identity

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_temporary_identity_mask
(
  byte                                          tlv_id,
  dsm_item_type                              ** response_pptr,
  qmi_uim_tlv_temporary_identity_mask_type      temporary_identity
)
{
  PACKED struct PACKED_POST
  {
    uint32 temporary_identity_mask;
  } tlv_data;

  ASSERT(response_pptr);

  memset(&tlv_data, 0x00, sizeof(tlv_data));

  if(temporary_identity.guti)
  {
    tlv_data.temporary_identity_mask |= UIM_PURGE_TEMPORARY_IDENTITY_REQ_GUTI_MASK_V01;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_temporary_identity_mask */


/*===========================================================================
  FUNCTION  QMI_UIM_RESPONSE_GET_PHYSICAL_SLOT_MAPPING()

  DESCRIPTION
    Composes logical physical slot mapping TLV

  PARAMETERS
    tlv_id                       : ID to be used for TLV
    response_pptr                : Pointer to Indication pointer
    logical_phy_mapping          : Logical physical slot mapping

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_physical_slot_mapping
(
  byte                                     tlv_id,
  dsm_item_type                          **response_pptr,
  uim_logical_slot_config_type             logical_phy_mapping
)
{
  uint8    i                                        = 0;
  uint8    tlv_len                                  = 0;

  PACKED struct PACKED_POST
  {
    uint8  logical_slot_len;
    uint32 logical_slot[QMI_UIM_MAX_CARD_COUNT];
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.logical_slot_len = logical_phy_mapping.num_slots;
  for (i = 0; i < tlv_data.logical_slot_len &&
              i < QMI_UIM_MAX_CARD_COUNT &&
              i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    switch(logical_phy_mapping.slot[i])
    {
      case UIM_SLOT_1:
        tlv_data.logical_slot[i] = UIM_PHY_SLOT_1_V01;
        break;
      case UIM_SLOT_2:
        tlv_data.logical_slot[i] = UIM_PHY_SLOT_2_V01;
        break;
      case UIM_SLOT_3:
        tlv_data.logical_slot[i] = UIM_PHY_SLOT_3_V01;
        break;
      default:
        return FALSE;
    }
  }

  tlv_len = sizeof(tlv_data.logical_slot_len) +
            sizeof(tlv_data.logical_slot[0]) * tlv_data.logical_slot_len;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data);
} /* qmi_uim_response_get_physical_slot_mapping */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_EID_INFO()

  DESCRIPTION
    Composes the TLV for EID retrieved

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_eid_resp_ptr : profile confirmation pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_eid_info
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  const lpa_get_eid_resp_type           *get_eid_resp_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint8 eid_len;
    uint8 eid[QMI_UIM_EID_LEN_V01];
  }tlv_data_eid;

  ASSERT(response_pptr && get_eid_resp_ptr);

  tlv_data_eid.eid_len = sizeof(tlv_data_eid.eid);
  (void)memscpy((void *)tlv_data_eid.eid,
                sizeof(tlv_data_eid.eid),
                (void *)get_eid_resp_ptr->eid,
                sizeof(get_eid_resp_ptr->eid));

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data_eid),
                               (void *)&tlv_data_eid);
} /* qmi_uim_response_get_eid_info */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_TERMINAL_CAPABILITY()

  DESCRIPTION
    Composes the TLV content of GET terminal capability

  PARAMETERS
    tlv_id                : id to be used for the TLV
    response_pptr         : output sdu
    num_tlvs              : Number of TLVs
    tlvs_data_len         : Total length of the ptr containing the TLVs
    tlvs_data_ptr         : Ptr containing the TLVs

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_terminal_capability
(
  byte                                       tlv_id,
  dsm_item_type **                           response_pptr,
  uint8                                      num_tlvs,
  uint8                                      tlvs_data_len,
  uint8*                                     tlvs_data_ptr
)
{
  uint16    in_index             = 0;
  uint8     out_index            = 0;
  uint8     num_tlvs_counter     = 0;
  uint8    *tlv_data_out_ptr     = NULL;
  uint32    tlv_data_out_len     = 0;
  boolean   ret_val              = FALSE;
  uint8     current_tlv_len      = 0;

  /* We generally use packed structures to create the output that goes into
     the QMI response, hence creating this packed struct that can be used
     with the sizeof operator while calculating the length of QMI response */
  PACKED struct PACKED_POST
  {
    uint8 num_tlvs;
  } tlv_data_num_tlvs;

  ASSERT(response_pptr);

  /* One byte for the number of TLVs + the number of bytes containing the TLVs */
  tlv_data_out_len = sizeof(tlv_data_num_tlvs) + tlvs_data_len;

  tlv_data_out_ptr = uimqmi_malloc(tlv_data_out_len);
  if(tlv_data_out_ptr == NULL)
  {
    return FALSE;
  }

  /* Populate the first byte containing the number of TLVs */
  tlv_data_out_ptr[0] = num_tlvs;

  if(num_tlvs > 0 &&
     tlvs_data_len > 0 &&
     tlvs_data_ptr)
  {
    /* Start from out_index 1 as zeroth index contains already the number
       of terminal capability TLVs */
    for(out_index = 1;
        out_index < (tlv_data_out_len - 1) &&
          in_index < (tlvs_data_len - 1);
        out_index += current_tlv_len)
    {
      current_tlv_len      = 0;

      /* Byte corresponding to the Tag of the TLV */
      tlv_data_out_ptr[out_index++] = tlvs_data_ptr[in_index++];
      /* Byte corresponding to the Length of the TLV */
      current_tlv_len = tlvs_data_ptr[in_index];
      tlv_data_out_ptr[out_index++] = tlvs_data_ptr[in_index++];

      if (current_tlv_len > 0)
      {
        if (current_tlv_len
              < QMI_UIM_TERMINAL_CAPABILITY_VALUE_MAX_V01 &&
            current_tlv_len <=
              (tlvs_data_len - in_index) &&
            current_tlv_len <=
              (tlv_data_out_len - out_index))
        {
          /* Bytes corresponding to the Value of the TLV */
          memscpy(&tlv_data_out_ptr[out_index],
                  tlv_data_out_len - out_index,
                  &tlvs_data_ptr[in_index],
                  current_tlv_len);
          in_index += current_tlv_len;
        }
        else
        {
          uimqmi_free(tlv_data_out_ptr);
          tlv_data_out_ptr = NULL;
          return FALSE;
        }
      }
      num_tlvs_counter++;
    }
  }

  if(out_index == (tlvs_data_len + 1) &&
     num_tlvs_counter == num_tlvs)
  {
    ret_val = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    out_index,
                                    (void *)tlv_data_out_ptr);
  }

  uimqmi_free(tlv_data_out_ptr);
  tlv_data_out_ptr = NULL;

  return ret_val;
} /* qmi_uim_response_terminal_capability */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PARSED_IMSI()

  DESCRIPTION
    This function parses the IMSI data to MCC, MNC and MSIN ascii values and
    composes the TLV content of parsed IMSI.

  PARAMETERS
    None

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_parsed_imsi
(
  byte                           tlv_id,
  dsm_item_type**                response_pptr,
  mmgsdi_data_type               response_data,
  uint8                          num_mnc_digits
)
{
  qmi_uim_imsi_digits_data_type   imsi_digits_data        = {0, };
  qmi_uim_imsi_type               imsi_data               = {0, };
  uint16                          i                       = 0;
  uint8                           index                   = 0;
  uint8                           tlv_data[QMI_UIM_MCC_LEN_V01 + 1 + QMI_UIM_MNC_MAX_V01 + 1 + QMI_UIM_MSIN_MAX_V01] = {0};
 
  ASSERT(response_pptr);

  if ((num_mnc_digits != QMI_UIM_MNC_2_DIGITS && 
       num_mnc_digits != QMI_UIM_MNC_3_DIGITS) ||
       response_data.data_ptr == NULL || 
       response_data.data_len == 0 || 
       response_data.data_len > QMI_UIM_MAX_IMSI_LEN)
  {
    return FALSE;
  }
  
  imsi_data.len = response_data.data_len;
  (void)memscpy(imsi_data.imsi,
                sizeof(imsi_data.imsi),
                response_data.data_ptr,
                response_data.data_len);

  /* Convert IMSI data in to IMSI digits format */
  if ((FALSE == qmi_uim_util_convert_imsi_to_digits(&imsi_data, &imsi_digits_data)) ||
      (num_mnc_digits == QMI_UIM_MNC_2_DIGITS && imsi_digits_data.imsi_digits_len < 5) ||
      (num_mnc_digits == QMI_UIM_MNC_3_DIGITS && imsi_digits_data.imsi_digits_len < 6) || 
      imsi_digits_data.imsi_digits_len > QMI_UIM_IMSI_DIGIT_LEN)
  {
    return FALSE;
  }

  /* Populate MCC digits */
  tlv_data[index++] = imsi_digits_data.imsi_digits[0];
  tlv_data[index++] = imsi_digits_data.imsi_digits[1];
  tlv_data[index++] = imsi_digits_data.imsi_digits[2];

  /* Populate MNC length and MNC digits */
  tlv_data[index++] = num_mnc_digits;
  tlv_data[index++] = imsi_digits_data.imsi_digits[3];
  tlv_data[index++] = imsi_digits_data.imsi_digits[4];

  /* If MNC length is 3 then populate the 3rd byte */
  if (num_mnc_digits == QMI_UIM_MNC_3_DIGITS)
  {
    tlv_data[index++] = imsi_digits_data.imsi_digits[5];
  }

  /* Set MSIN length */
  tlv_data[index++] = imsi_digits_data.imsi_digits_len - QMI_UIM_MCC_LEN_V01 - num_mnc_digits;

  for (i = QMI_UIM_MCC_LEN_V01 + num_mnc_digits; 
       i < imsi_digits_data.imsi_digits_len &&
       i < QMI_UIM_IMSI_DIGIT_LEN && 
       index < sizeof(tlv_data); i++)
  {
    tlv_data[index++] = imsi_digits_data.imsi_digits[i];
  }
  
  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               index,
                               (void *)tlv_data);
} /* qmi_uim_response_parsed_imsi */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PARSED_PLMN_LIST()

  DESCRIPTION
    This function parses the FPLMN and EHPLMN data to number of plmns and
    plmns(MCC, MNC) in ascii values and composes the TLV content of
    parsed PLMN list.
 
  PARAMETERS
    None

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_parsed_plmn_list
(
  byte                     tlv_id,
  dsm_item_type**          response_pptr,
  mmgsdi_data_type         response_data
)
{
  uint8     offset         = 0;
  uint8     index          = 0;
  uint8*    tlv_data_ptr   = NULL;
  uint8     tlv_index      = 1;
  boolean   ret_value      = TRUE;
  uint16    tlv_data_len   = 0;
  char      digit[6]       = {0};
  uint8     mnc_len        = 0;
  uint8     digit_index    = 0;

  ASSERT(response_pptr);
  
  if (response_data.data_len == 0)
  {
    return TRUE;
  }

  if ((response_data.data_len % QMI_UIM_PLMN_ID_LEN) != 0 || 
       response_data.data_ptr == NULL)
  {
    return FALSE;
  }
  
  if (response_data.data_len > (QMI_UIM_PLMN_NUM_MAX_V01 * QMI_UIM_PLMN_ID_LEN))
  {
    return FALSE;
  }

  /* Allocate memory for TLV, considering the worst case where all PLMNs have 3 digits MNC */
  tlv_data_len = sizeof(uint8) + (QMI_UIM_MAX_PLMN_BYTES * (response_data.data_len / QMI_UIM_PLMN_ID_LEN));
  tlv_data_ptr = uimqmi_malloc(sizeof(uint8) + tlv_data_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }
  
  do
  {
    ret_value = TRUE;

    offset = index * QMI_UIM_PLMN_ID_LEN;

    ASSERT(offset + 2 < response_data.data_len);
	
    ASSERT(tlv_index + QMI_UIM_MAX_PLMN_BYTES <= tlv_data_len);

    /* Populate MCC */
    digit[0] = ((response_data.data_ptr[offset] & QMI_UIM_IMSI_LOW_BYTE_MASK) + '0');
    digit[1] = (((response_data.data_ptr[offset] & QMI_UIM_IMSI_HIGH_BYTE_MASK) >> 4) + '0');
    digit[2] = ((response_data.data_ptr[offset + 1] & QMI_UIM_IMSI_LOW_BYTE_MASK) + '0');
    
    /* Populate MNC */
    digit[3] = ((response_data.data_ptr[offset + 2] & QMI_UIM_IMSI_LOW_BYTE_MASK) + '0');
    digit[4] = (((response_data.data_ptr[offset + 2] & QMI_UIM_IMSI_HIGH_BYTE_MASK) >> 4) + '0');
    mnc_len = 2;

    /* Populate MNC and Check length of MNC and decode 3rd MNC byte accordingly */
    if((response_data.data_ptr[offset + 1] & QMI_UIM_IMSI_HIGH_BYTE_MASK) != 0xF0)
    {
      mnc_len = 3;
      digit[5] = (((response_data.data_ptr[offset + 1] & QMI_UIM_IMSI_HIGH_BYTE_MASK) >> 4) + '0');
    }
    
    /* Validate all the digits */
    for (digit_index = 0; digit_index < 3 + mnc_len; digit_index++)
    {
      if(!(digit[digit_index] >= '0' && digit[digit_index] <= '9'))
      {
        ret_value = FALSE;
        index++;
        break;
      }
    }
    
    /* Skip the iteration of adding PLMN to the TLV if any digit is invalid in that plmn */
    if (ret_value == FALSE)
    {
      continue;
    }
   
    /* Copy the digits to the TLV */
    for (digit_index = 0; digit_index < 3 + mnc_len && digit_index < sizeof(digit); digit_index++)
    {
      if (digit_index == 3)
      {
        tlv_data_ptr[tlv_index++] = mnc_len;
      }
      tlv_data_ptr[tlv_index++] = digit[digit_index];
    }
    index++;

    /* Populate number of plmns */
    tlv_data_ptr[0]++;
  }while((index < response_data.data_len / QMI_UIM_PLMN_ID_LEN) && (index < QMI_UIM_PLMN_NUM_MAX_V01));
  
  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_index,
                                    (void *)tlv_data_ptr);


  uimqmi_free(tlv_data_ptr);
  return ret_value;
} /* qmi_uim_response_parsed_plmn_list */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EXTENDED_CARD_INFO()

  DESCRIPTION
    Composes the extended info TLV content, ICCID, EID and ATR for card status 
	indication or get card status request

  PARAMETERS
    tlv_id           : id to be used for the TLV
    response_pptr    : output sdu
    card_state_ptr   : logical slots information

  RETURN VALUE
    boolean          : TRUE if extended_card_info is filled succsully
                       else FALSE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_extended_card_info
(
  byte                             tlv_id,
  dsm_item_type **                 response_pptr,
  const qmi_uim_card_state       * card_state_ptr
)
{
  uint8                   tlv_len                            = 0;
  uint8                   *tlv_data_ptr                      = NULL;
  uint8                   *cur_ptr                           = NULL;
  boolean                 ret_value                          = TRUE;
  mmgsdi_data_type        iccid_data[QMI_UIM_MAX_CARD_COUNT];
  mmgsdi_data_type        atr_data[QMI_UIM_MAX_CARD_COUNT];
  mmgsdi_data_type        eid_data[QMI_UIM_MAX_CARD_COUNT];
  mmgsdi_return_enum_type mmgsdi_status                      = MMGSDI_ERROR;
  uint8                   slot_index                         = 0;
  qmi_error_e_type        errval                             = QMI_ERR_INTERNAL;
  uint8                   num_slots                          = 0;
  
  
  PACKED struct PACKED_POST
  {
    uint8 iccid_len;
    uint8 atr_len;
    uint8 eid_len;
  }tlv_extended_card_info_data;

  memset(iccid_data, 0x00, QMI_UIM_MAX_CARD_COUNT * sizeof(mmgsdi_data_type));
  memset(atr_data, 0x00, QMI_UIM_MAX_CARD_COUNT * sizeof(mmgsdi_data_type));
  memset(eid_data, 0x00, QMI_UIM_MAX_CARD_COUNT * sizeof(mmgsdi_data_type));

  num_slots = (QMI_UIM_MAX_CARD_COUNT < card_state_ptr->num_slots) ?
                QMI_UIM_MAX_CARD_COUNT : card_state_ptr->num_slots;
  
  do
  {
    for(slot_index = 0; slot_index < num_slots ; slot_index++)
    {
      mmgsdi_slot_id_enum_type          mmgsdi_slot = MMGSDI_SLOT_NONE;

      errval = qmi_uim_util_slot_index_to_mmgsdi_slot(slot_index,
                                                      &mmgsdi_slot);
      if (errval != QMI_ERR_NONE)
      {
        ret_value = FALSE;
        break;
      }
      
      /* Get ICCID length from MMGSDI cache */
      mmgsdi_status =  mmgsdi_session_read_cache_file_size(
                        qmi_uim_global_ptr->mmgsdi_card_slot_session_id[slot_index],
                        MMGSDI_ICCID,
                        &iccid_data[slot_index].data_len);

      if(mmgsdi_status == MMGSDI_SUCCESS &&
         iccid_data[slot_index].data_len)
      {

        iccid_data[slot_index].data_ptr = (uint8 *)uimqmi_malloc(
                                            iccid_data[slot_index].data_len);

        if(iccid_data[slot_index].data_ptr == NULL)
        {
          ret_value = FALSE;
          break;
        }
        memset(iccid_data[slot_index].data_ptr,
               0x00,
               iccid_data[slot_index].data_len);

        /* Get ICCID data from MMGSDI cache, even if the reading of ICCID fails
           coninue by filling ICCID length as 0 in extended_card_info TLV */
        mmgsdi_status = mmgsdi_session_read_cache(
                  qmi_uim_global_ptr->mmgsdi_card_slot_session_id[slot_index],
                  MMGSDI_ICCID,
                  iccid_data[slot_index]);
        if(mmgsdi_status != MMGSDI_SUCCESS)
        {
          iccid_data[slot_index].data_len = 0;
        }
      }

      atr_data[slot_index].data_ptr = (uint8 *)uimqmi_malloc(UIM_MAX_ATR_CHARS);
      if(atr_data[slot_index].data_ptr == NULL)
      {
        ret_value = FALSE;
        break;
      }
    
      memset(atr_data[slot_index].data_ptr, 0x00, UIM_MAX_ATR_CHARS);

      /* Get ATR length and data from MMGSDI, even if the reading of ATR fails
         coninue as the ATR length will be filled as 0 in extended_card_info
         TLV */
      (void)mmgsdi_get_atr_sync(qmi_uim_global_ptr->mmgsdi_client_id,
                                mmgsdi_slot,
                                &atr_data[slot_index],
                                UIM_MAX_ATR_CHARS);

      eid_data[slot_index].data_ptr = (uint8 *)uimqmi_malloc(UIM_MAX_EID_SIZE);
      if(eid_data[slot_index].data_ptr == NULL)
      {
        ret_value = FALSE;
        break;
      }
      memset(eid_data[slot_index].data_ptr, 0x00, UIM_MAX_EID_SIZE);
      
      /* Get EID length and data from MMGSDI, even if the reading of EID fails
         coninue as the EID length will be filled as 0 in extended_card_info
         TLV */
      (void)mmgsdi_get_eid_sync(qmi_uim_global_ptr->mmgsdi_client_id,
                                mmgsdi_slot,
                                &eid_data[slot_index],
                                UIM_MAX_EID_SIZE);
    }

    if(ret_value == FALSE)
    {
      break;
    }

    tlv_len = sizeof(num_slots);

    /* Calculate the length to allocate to the TLV and the number of active
       slots. We need to allocate bytes to ICCID, ATR and EID only if the card
       state is Present */
    for(slot_index = 0; slot_index < num_slots; slot_index++)
    {
      if(card_state_ptr->card[slot_index] == NULL)
      {
        continue;
      }

      if(iccid_data[slot_index].data_len > QMI_UIM_ICCID_LEN_MAX_V01 ||
         atr_data[slot_index].data_len > QMI_UIM_ATR_MAX_V01 ||
         eid_data[slot_index].data_len > QMI_UIM_EID_LEN_V01)
      {
        ret_value = FALSE;
        break;
      }

      tlv_len += sizeof(tlv_extended_card_info_data);

      if(card_state_ptr->card[slot_index]->card_state == QMI_UIM_CARD_STATE_PRESENT)
      {
        tlv_len += iccid_data[slot_index].data_len + atr_data[slot_index].data_len + eid_data[slot_index].data_len;
      }
    }

    tlv_data_ptr = (uint8*)uimqmi_malloc(tlv_len);
    if(tlv_data_ptr == NULL)
    {
      ret_value = FALSE;
      break;
    }
    cur_ptr = tlv_data_ptr;
  
    ASSERT(cur_ptr + sizeof(num_slots) <= tlv_data_ptr + tlv_len);
    *cur_ptr = num_slots;
    cur_ptr += sizeof(num_slots);
  
    for(slot_index = 0; slot_index < num_slots; slot_index++)
    {
      if(card_state_ptr->card[slot_index] == NULL)
      {
        continue;
      }
      
      if(card_state_ptr->card[slot_index]->card_state == QMI_UIM_CARD_STATE_PRESENT)
      {
        tlv_extended_card_info_data.iccid_len = iccid_data[slot_index].data_len;
        ASSERT(cur_ptr + sizeof(tlv_extended_card_info_data.iccid_len) <= tlv_data_ptr + tlv_len);
        (void)memscpy(cur_ptr,
                      tlv_len - (cur_ptr - tlv_data_ptr),
                      &tlv_extended_card_info_data.iccid_len,
                      sizeof(tlv_extended_card_info_data.iccid_len));
        cur_ptr += sizeof(tlv_extended_card_info_data.iccid_len);
        if(iccid_data[slot_index].data_len > 0)
        {
          ASSERT(cur_ptr + iccid_data[slot_index].data_len <= tlv_data_ptr + tlv_len);
          (void)memscpy(cur_ptr,
                        tlv_len - (cur_ptr - tlv_data_ptr),
                        (void*)iccid_data[slot_index].data_ptr,
                        iccid_data[slot_index].data_len);
          cur_ptr += iccid_data[slot_index].data_len;
        }
        
        tlv_extended_card_info_data.atr_len = atr_data[slot_index].data_len;
        ASSERT(cur_ptr + sizeof(tlv_extended_card_info_data.atr_len) <= tlv_data_ptr + tlv_len);
        (void)memscpy(cur_ptr,
                      tlv_len - (cur_ptr - tlv_data_ptr),
                      &tlv_extended_card_info_data.atr_len,
                      sizeof(tlv_extended_card_info_data.atr_len));
        cur_ptr += sizeof(tlv_extended_card_info_data.atr_len);
        if(atr_data[slot_index].data_len > 0)
        {
          ASSERT(cur_ptr + atr_data[slot_index].data_len <= tlv_data_ptr + tlv_len);
          (void)memscpy(cur_ptr,
                        tlv_len - (cur_ptr - tlv_data_ptr),
                        (void*)atr_data[slot_index].data_ptr,
                        atr_data[slot_index].data_len);
          cur_ptr += atr_data[slot_index].data_len;
        }
        
        tlv_extended_card_info_data.eid_len = eid_data[slot_index].data_len;
        ASSERT(cur_ptr + sizeof(tlv_extended_card_info_data.eid_len) <= tlv_data_ptr + tlv_len);
        (void)memscpy(cur_ptr,
                      tlv_len - (cur_ptr - tlv_data_ptr),
                      &tlv_extended_card_info_data.eid_len,
                      sizeof(tlv_extended_card_info_data.eid_len));
        cur_ptr += sizeof(tlv_extended_card_info_data.eid_len);
        if(eid_data[slot_index].data_len > 0)
        {
          ASSERT(cur_ptr + eid_data[slot_index].data_len <= tlv_data_ptr + tlv_len);
          (void)memscpy(cur_ptr,
                        tlv_len - (cur_ptr - tlv_data_ptr),
                        (void*)eid_data[slot_index].data_ptr,
                        eid_data[slot_index].data_len);
          cur_ptr += eid_data[slot_index].data_len;
        }
      }
      else
      {
        memset(cur_ptr, 0x00, sizeof(tlv_extended_card_info_data));
        cur_ptr += sizeof(tlv_extended_card_info_data);
      }
    }
  }while(0);

  
  for(slot_index = 0; slot_index < num_slots; slot_index++)
  {
    if(iccid_data[slot_index].data_ptr)
    {
      uimqmi_free(iccid_data[slot_index].data_ptr);
      iccid_data[slot_index].data_ptr = NULL;
    }
    if(atr_data[slot_index].data_ptr)
    {
      uimqmi_free(atr_data[slot_index].data_ptr);
      atr_data[slot_index].data_ptr = NULL;
    }
    if(eid_data[slot_index].data_ptr)
    {
      uimqmi_free(eid_data[slot_index].data_ptr);
      eid_data[slot_index].data_ptr = NULL;
    }
  }

  if(ret_value)
  {
    ret_value = qmi_svc_put_param_tlv(response_pptr,
                                      tlv_id,
                                      tlv_len,
                                      (void *)tlv_data_ptr);
  }
  
  if(tlv_data_ptr != NULL)
  {
    uimqmi_free(tlv_data_ptr);
    tlv_data_ptr = NULL;
  }

  return ret_value;
} /* qmi_uim_response_extended_card_info */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
