/*===========================================================================

                         Q M I _ U I M . C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2010 - 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim.c#9 $ $DateTime: 2020/12/22 03:01:07 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/18/20    shp    Added stringl.h for memscpy
08/17/20    vgd    Added logic to get EID using both M2M and LPA methods
06/08/20    rkg    Revert defensive check added in subscription manager code
05/07/20    rkg    Skip get eid iteration for inactive slot 
02/20/20    rkg    Return QMI_ERR_NO_EFFECT if no change in mapping
02/13/20    vgd    Get ATR, ICCID, EID from MMGSDI when card_info is populated
10/31/19    vdc    Compiler error while comparing slot info in EID retrieval
09/17/19    vdc    SimLock solution for activation at Point Of Sale
09/17/19    mm     Add EID to SimSlotStatus radio config HAL
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
11/16/18    sg     Change app state to perso from illegal when perso locked 
10/05/18    ra     Add support to send long select AID response
08/17/18    rps    Add support for mini dump phase 2
06/14/18    srm    Dont set ind sent to TRUE when both cards state is error/removed
12/27/17    ra     Disable GSM Security restrictions for Authentication on test card
12/06/17    srm    Send IND if other slot detection has started late
10/18/17    sk     Add error case in GET_ATR if card is not present
10/07/17    av     Send IND to clients after refresh_reset for zero-app eUICC
07/13/17    av     Enhanced simlock RSU solution
07/07/17    sk     Fixed KW errors in qmi_uimi_send_apdu()
06/22/17    me     Added support for card type in card inserted evt
05/11/17    tq     Featurize 3GPP2 code under FEATURE_MMGSDI_3GPP2
05/09/17    me     Added a check for file type in read and write operation
04/25/17    vdc    Added support for refresh enforcement policy
04/21/17    sk     Map provisioning sessions to slots
04/19/17    ku     GET EID for M2M cards
04/18/17    me     Added support to euicc bit in atr
04/14/17    kv     Added pass-through mode support
04/12/17    ar     Add support for retrieve/configure default server address
04/06/17    gm     Added support for extended APDU
04/03/17    ks     UIM automatic slot mapping support
02/21/17    bcho   Set perso state to IN PROGRESS if any app gets stuck at PIN1
02/22/17    ar     Call LPA APIs only if LPA is modem centric
02/06/17    bcho   Don't send QMI UIM refresh indication for Refresh Reset Auto
02/02/17    me     Fixing memory leak in qmi_uim_read_configuration_apdu_aid_list()
01/31/17    me     Send correct message in get file attrib response
01/19/17    me     Populating path internally and parsing IMSI, FPLMN, EHPLMN
12/07/16    av     LPA phase2 initial changes
12/05/16    vdc    Added support for simlock single lock for full srv policy
12/02/16    me     Combined multi-line ASSERT in one line
12/02/16    ar     Query for profile info only if LPA is modem centric
11/29/16    nr     KW error fixes
11/22/16    bcho   Send card status if SIM BUSY occurs during card detection
10/27/16    ar     Handle sending indication when global data is freed
10/24/16    vdc    Added support for auto reg SMS when ESN is changed
10/19/16    ar     Send SUCCESS indication in APP RESET and 3G SESSION RESET
10/10/16    av     Add support for configuring HTTP stack via LPA EFS item
09/22/16    gm     Added support for set APDU behavior
08/24/16    tkl    Remove excessive F3
08/19/16    tkl    Added support for simlock single lock
08/18/16    ar     Add support for simlock storage and emergency only mode
08/17/16    bcho   Return FALSE if CONFIG APDU SEC AID LIST file is not present
08/08/16    av     Ensure better error handling for terminal cap API
07/28/16    ar     Handle auto-activation race cond by passing an extra param
07/28/16    ar     Query internally for profile info in case of add profile
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
07/19/16    bcho   Move refresh_info to heap in qmi_uim_global_type
07/15/16    sp     Seperate ERA-GLONASS and SAP related QMI UIM functionality
07/14/16    ar     Check Blacklist based on AID source
07/11/16    ar     Optimize eUICC logic for non-spec compliant card
07/11/16    ar     Add ISDR-AID in Whitelist/Blacklist as per new NV
07/08/16    ar     Move qmi_uimi_recovery and unify fdn/acl get_status func.
06/30/16    av     Fix slot id to efs context conversion for uim common APIs
06/28/16    vdc    Remove callback user data from qmi uim global
05/26/16    vdc    Move internal enums, structs to new internal header file
05/20/16    ar     Stub out UIM supply voltage indication for IOE flavour
05/12/16    tkl    Remove reference to uim_get_service_object_v01
05/11/16    bcho   Move GBA and encryption related response functions
04/15/16    vdc    Split QMI UIM files for supporting IOE
04/11/16    ar     Support for eUICC
03/15/16    vr     SAM CARD support
03/29/16    kv     Send card_ind for card_error even if other slot is initializing
03/09/16    sp     Remove Excessive F3's
02/22/16    gm     Deprecate mmgsdi_option_type
02/19/16    vdc    Send control key presence in get config presence
02/09/16    gm     Added generic response function
01/21/16    vdc    Fix restrictions of AUTH command for different contexts
01/20/16    gm     Added support to purge temporary identity
01/13/16    vr     Common function for result tlv and send response
01/13/16    ar     Allow read req for more than 4096 bytes
01/12/16    vdc    Populate retries left correctly
01/07/16    ar     Add some debug logs
12/21/15    vdc    Send err resp for recovery if device is in passive mode
12/07/15    sp     Remove unused variables
12/02/15    vr     Delete message from queue if mmgsdi set signal fails
11/19/15    vdc    Reduce stack space for SIMLOCK APIs
11/16/15    yt     Support for second BSF and key status events
11/09/15    vv     Remove unneeded F3's
11/10/15    yt     Update APDU streaming restrictions for default channel
11/03/15    av     Handle card_detection_started event correctly
10/30/15    tkl    Return error when fail to post message to MMGSDI context
10/29/15    bcho   Graceful handling of NULL resp_ptr in simlock callback
10/23/15    bcho   Support for multi byte GID
10/05/15    sp     Reduce signed/unsigned conversions in UIM
10/06/15    vv     Support for reuse code data
10/06/15    kv     Remove diag access to security sensitive NVs in FTM mode
09/30/15    lm     Fix compiler warning
09/22/15    yt     Update gba_bootstrap API parameters
09/21/15    bcho   Use Silent PIN1 NV as enabled if it is not set in efs
09/10/15    ks     Increase max no of non prov session
09/09/15    hh     When open non-prov session check AID value before setting app type
08/28/15    at     Remove duplicated code between QMI CAT and QMI UIM
08/28/15    vv     Enhance perso secure message to check lock status
08/19/15    vv     Added support to retrieve retry attempts left for locking
08/19/15    ks     Using mmgsdi API which returns number of hw slots enabled
08/17/15    av     Featurize simlock/perso code under FEATURE_SIMLOCK
08/17/15    stv    To send temporary unlock status indication
08/14/15    yt     Filter GSM AUTH streaming commands on default channel
08/11/15    lxu    Error code for card not supporting select by partial AID
08/10/15    av     Introduce MMGSDI event for indicating recovery completion
07/31/15    vv     Fix processing out of sync remote simlock config requests
07/13/15    yt     Support for receiving events for hidden CSIM
07/07/15    bcho   Redundant checks for SIMLOCK
07/02/15    vdc    Send app state as PERSO when device is set in emergency mode
06/12/15    ar     Handle QMI UIM SEARCH RECORD pending request
06/10/15    vdc    Fill IMPI file info while sending GBA IMPI read req
05/29/15    vdc    Fix compilation error in personalization
05/18/15    tkl    Moved request_id up to callback function
05/15/15    vv     Support for RSU
05/14/15    vr     Don't send perso ind if there is no perso state change
05/11/15    vv     Fix a merge error
05/11/15    vdc    Enhance parsing logic for black/white list for bootstrapping
05/06/15    vdc    Send SAP disconnected ind if card is removed
05/02/15    vdc    Open ISIM session while bootstrapping if not opened
04/30/15    vdc    Added support for APN for bootstrapping
04/29/15    tkl    Restricted the use of Auth request for GBA security context
04/29/15    av     Do not retry service registration if it fails once
04/27/15    tl     Change IMPI domain from ics to ims for spec compliance
04/25/15    av     Support for hiding CSIM from modem clients
04/20/15    ar     Support for terminate_app TLV in qmi_uim_close_session
04/17/15    at     Updating the QMI perso state correctly when blocked
04/15/15    ks     De-Featurizing the FEATURE_UIM_REMOTE_UIM
04/11/15    ar     Handle back-to-back power_up/down req gracefully
04/08/15    at     Support for SIM detected usecase
04/03/15    kk     Migrate to mcfg EFS wrappers
03/31/15    tl     Additional changes to split QMI initaization
03/27/15    tl     Add support for GBA cancel
03/19/15    at     Adding an optional TLV to reactivate the perso feature
03/14/15    vdc    Allow bootstrapping with empty NAF id
03/11/15    av     Clean up MSG_SPRINTFs from code
03/10/15    vdc    Replace hard coded values with IDL defined values
03/04/15    vv     Added support for new perso status
03/04/15    vdc    Added support for QMI UIM GBA
03/04/15    vdc    Set SAP enabled flag to FALSE when SAP is disconnected
03/03/15    lm     Remove double semicolon at end of the line
02/27/15    vdc    Send correct sap status while doing connect and disconnect
02/25/15    ar     Addition of QMI_UIM_SEARCH_RECORD interface
02/20/15    vv     Remove perso related code
02/03/15    xj     Add USB UICC and SAP CLIENT supported interface
02/03/15    at     Replacement of Secapis to use a derived key & HMAC
01/07/15    vdc    Closed pending comments for handling multi profile cards
12/26/14    vv     Fix enabling a lock with more than 20 codes of lock data
12/18/14    yt     Update RESET refresh handling for non-prov/card sessions
12/16/14    av     Apply auth restr via NV for IIC only if USIM is selected
12/12/14    yt     Remove unused variable
12/10/14    vdc    Added support for handling multiple profiles enabled cards
11/26/14    av     Change default value of NV 70284 to allow GSM auth
11/14/14    tkl    Added check for UIMI_MAX_CLIDS
11/14/14    yt     Send refresh start indication for card slot sessions
11/14/14    hh     Fix compilation warnings
11/13/14    bcho   Send Refresh indication to all in Refresh Reset start stage
11/10/14    yt     Return status words of original PIN OP
11/07/14    lxu    Set default AUTH retrictions to RESTRICTIONS_II_CLA
11/03/14    lxu    Only reject ICC authenticate command with CLA 0xA0
10/29/14    tl     Add extra comparison in diag access check callback
10/29/14    ar     Deregister files with MMGSDI on QMI UIM client de-register
10/21/14    vv     Add support for late simlock config registration
10/16/14    vv     Fix sending extra resp for simlock configuration req
10/14/14    tl     Fix compiler error
10/10/14    tkl    Fix KW error
10/10/14    vv     Fix a merge error
10/10/14    vv     Add support for remote simlock configuration
10/08/14    vdc    Added support for UICC activation status indication
09/30/14    vv     Add support for emergency only command
09/30/14    kk     Enhancements to NV handling
09/29/14    vv     Added support for the remote SFS
09/29/14    av     Featurize all QMI messages
09/29/14    vdc    Send no effect resp when card is powered up for power up cmd
09/25/14    tl     Add support for secure read
09/19/14    kk     Move refresh event info to heap
09/17/14    tl     Add support for perso and deperso secure
09/17/14    vr     Correction in heap variable created in stack reduction
09/17/14    hh     Moved qmi_uim_global_ptr from heap to static global variable
09/11/14    hh     Call rex_undef_timer() when timer is no longer needed
09/09/14    vr     Reduce stack usage
09/03/14    tl     Update app state when perso lock event sent during sub ready
08/29/14    at     Sending response for QMI_UIM_SUPPLY_VOLTAGE
08/29/14    tl     Add support for supply voltage command and indication
08/12/14    yt     Return status words for QMI_UIM_SEND_STATUS
08/11/14    tkl    Added SSM support for restricted NV
08/07/14    hh     Ignore b7 in CLA byte for SEND_APDU_REQ
08/07/14    yt     Fix array bound checking
08/05/14    hh     Added support to retrieve supported proactive commands
07/25/14    vv     Added support for perso command
07/22/14    at     Fix error in ISIM Auth response for different contexts
07/22/14    tkl    Setup diag access check cb for QMI CAT NV
07/08/14    vv     Fix for compiler warnings
06/30/14    vv     Remove Perso code
06/26/14    vv     Fix compiler issue due to missing featurization
06/17/14    vv     Support for slot parameter in deperso command
06/12/14    av     Enhance GET_SERVICE_STATUS to accomodate all service numbers
05/22/14    tl     Added support for recovery complete indications
05/13/14    vv     Fix compiler errors
05/13/14    vdc    Support for reduced card status indications
05/12/14    df     Add QMI support for STATUS cmd
04/22/14    at     Critical KW fixes
04/15/14    vv     Fix crash in unlock msg response processing
03/27/14    tkl    Added checks for client validity
03/27/14    at     Fix for sending refresh reset to non-prov session clients
03/27/14    at     Usage of common QMI APIs from DS header
03/26/14    tl     SIM Lock secondary revisions
03/18/14    tl     Introduce new SIM Lock feature
03/14/14    lj     Support for the updated AUTH restrictions EFS item
03/12/14    yt     Check for NULL response pointer
03/12/14    gm     Moved qmi_uim_state to heap
03/06/14    yt     Allow close session response for invalid session id
03/06/14    gm     Moved to new API for client alloc/dealloc result
02/25/14    yt     Support for QMI_UIM_RESELCT_REQ
02/24/14    at     Support for read transparent long indication
02/24/14    av     Do not rely on UIMI_CMD_MAX when traversing through cmd cb
02/20/14    at     Fix to clean up non-prov session id used for refresh
01/28/14    df     Use q_cnt() instead directly accessing cnt
01/22/13    vs     Fix delayed access for non-prov sessions
01/15/14    ar     Changed MSG_ERROR to UIM_MSG_ERR
01/15/14    ar     Added handler for MMGSDI_SESSION_LEGAL_SUBSCRIPTION_EVT
01/10/14    df     Use v2.0 diag macros
12/24/13    am     Add NV protection to Recovery API
12/21/13    am     Introduce recovery API
12/16/13    yt     Support for ISIM AUTH in HTTP digest security context
12/13/13    tkl    Clean up non-prov session regardless of APDU restrictions
12/09/13    spo    Fix for handling auth request for USIM and ISIM
12/04/13    at     Handling MMGSDI card error code NO_PROTOCOL_AT_CARD_PUP
11/26/13    vdc    Remove triple SIM feature while powering up the card
11/25/13    tkl    Added API for Increase
11/22/13    tl     Update perso retries in app state ready
11/14/13    df     Correct buffer size calculation
11/08/13    tkl    Check for client validity in non-prov session close
11/12/13    vdc    Handle errors in QMI UIM initialization gracefully
10/27/13    df     Add handling for card error type MMGSDI_CARD_ERR_SAP_CONNECTED
10/21/13    yt     Add error checking for command APDUs
10/18/13    vv     Send session closed ind. for the prov. session on the slot
10/09/13    at     Support for the updated APDU restrictions EFS item
10/04/13    vv     Added API to return se13 table release date and source
09/23/13    df     Support for SIM busy
09/16/13    yt     Remove redundant code
09/09/13    yt     Add check for MMGSDI_NON_PROV_SESSION_SLOT_3
09/04/13    at     Keep APDU channel info when open channel response is pending
09/02/13    df     Ensure channel is shared for empty AIDs
08/26/13    at     Check for client validity in session open/close callbacks
08/08/13    ks     Support for subscription manager
07/31/13    yt     QMI_UIM_GET_LABEL should return error for zero-length AID
07/26/13    tkl    Add NV support for enabling use of SMS-PP envelope command
07/25/13    yt     Respond to REFRESH events for APDU sessions
07/24/13    at     Adding pending request for non-prov sessions in linked list
07/15/13    yt     Postpone sending card indications until slot session is ready
07/15/13    av     Introduction of new NULL_BYTES card error type
07/10/13    vdc    Added support for missing instances of triple sim
07/02/13    yt     Dynamically allocate slot and app data
06/24/13    kb     Added QMI malloc and free wrapper functions
06/21/13    kk     Updated support for triple sim
06/20/13    yt     Extract slot id from session changed event payload
06/18/13    vv     Request PIN status only if app is present in global app list
06/13/13    yt     Fix compiler warning
05/21/13    vv     Extend session closed indication for all sessions
05/21/13    tkl    Fix KW errors
05/18/13    spo    Replaced instances of memcpy with memscpy
05/15/13    bcho   Handle Refresh 3G Session Reset as NAA APP Reset with FCN
04/30/13    vv     Reset upin state and retries count upon card error
04/30/13    tl     Return card result to client for pin operations
04/29/13    av     Fix null pointer usage in execute_open_session_for_apdu
04/08/13    at     Added 2 more EFS files into the Diag-restricted list
03/14/13    tl     Allow authentication with non-prov app if SIM locked
02/27/13    yt     Support for MMGSDI_NOT_SUPPORTED return type
02/22/13    yt     Support for slot 3
02/20/13    at     Protect security-related EFS files from Diag access
02/19/13    av     Merge mmgsi,gstk,qmiuim/cat conf files to mmgsdi.conf
02/14/13    tl     Move QMI UIM to MMGSDI context from QMI modem context
02/12/13    at     Added support for file status TLV
02/07/13    yt     Support for activation/deactivation of EFs
02/04/13    at     Fetch NV 4398 from MMGSDI cache
02/04/13    at     Updating Sec APIs to include SSID & CID
01/23/13    at     Reverting the MMGSDI NV cache read operation
01/03/12    abg    Removed blocking NV read operation
12/27/12    av     Block RUN GSM ALGO via send APDU and via uim_qmi_authenticate
12/13/12    at     Correcting 2 TLV types for read transparent indication
12/12/12    av     Fixed compiler critical warnings
12/11/12    vv     Fixed critical KW warnings
12/06/12    yt     Block AUTH request if app state is illegal or perso
11/09/12    yt     Move QMI UIM to UIM heap
11/02/12    yt     Add support for ACL in SET and GET SERVICE STATUS commands
10/21/12    av     Update conf file only if necessary
10/17/12    shr    Added support to drop MMGSDI responses and pending commands
                   when client corresponding to the request is no longer valid
10/16/12    at     Send error reponse for multiple read records if applicable
10/04/12    at     Switch QMI_UIM to use mmgsdi_session_open_ext
10/01/12    yt     Remove limitation on the num of client-registered refresh files
09/18/12    at     Updated error code for deactivating a prov session if
                   already deactivated
09/07/12    at     Removal of private DS functions and headers
09/07/12    tl     Added support for SGLTE dual slots
09/05/12    at     Updated error code for close channel when not open
08/31/12    vv     Send channel close indication based on NV config
08/29/12    at     Optional TLV in SAP connection req to check for active calls
08/29/12    spo    Allow close of session only if AID length is greater than 1
08/28/12    spo    Added support for Session Closed Indication
08/17/12    spo    Fixed length of IMSI before decrypt
08/16/12    at     New command handling for QMI_UIM_REFRESH_REGISTER_ALL
08/16/12    tl     Add session close without app termination functionality
07/30/12    yt     Return appropriate error type for open session failure
07/20/12    at     Check for sub_ok TLV value in QMI_UIM_SUBSCRIPTION_OK
07/13/12    at     Dynamic allocation of APDU channel info in global
06/19/12    at     Fix to clean the client queues before cl_id is deallocated
06/05/12    vv     Set appropriate error when a PIN1 operation is unsupported
06/04/12    at     Fix reachable assert in qmi_uim_refresh_create_deregister_list
05/23/12    hn     Enable 1-byte alignment packing on Windows environment
05/18/12    at     Send refresh indication end stage for App Reset
05/18/12    vv     Fixed incrementing the application index
05/17/12    vv     Update PIN retries and state after a PIN operation
05/14/12    at     Add new TLV to QMI_UIM_GET_CARD_STATUS command
05/10/12    nmb    Correct dangling pointer when deallocating clients
04/17/12    at     Support for opening a non prov session with empty AID
04/16/12    at     Decode extended logical channels correctly for APDU rejection
04/12/12    at     Sending correct value in Result TLV for QMI_UIM_AUTHENTICATE
04/11/12    at     Indication support for sending long APDUs
04/05/12    at     Reject commands on logical channel not opened by the client
04/02/12    vv     Set appropriate error when a PIN2 operation is unsupported
03/26/12    tl     Updated incorrect pointer in MMGSDI_SAP_CARD_READER_STATUS_CNF case
03/26/12    nmb    Enable Secure Channel perso
03/22/12    bcho   Perso status WAIT FOR EXTERNAL PERSO, PERSO FAILURE handled
03/12/12    vv     Send appropriate error in ISIM auth response
03/09/12    vv     Send one indication after a PIN operation
03/08/12    at     Proper cleanup during REFRESH RESET & card error events
03/05/12    tkl    MPSS/QMI support interfaces logging
03/02/12    vv     Added optional TLV in send APDU request
03/02/12    vv     Added support for querying ESN status
03/02/12    at     Change to not send mmgsdi_card_pup if it is already up
03/02/12    at     Minor changes while reading apdu_security_aid_list
02/29/12    at     Check of whitelist/blacklist when opening a logical channel
02/28/12    yt     Fixed compilation error
02/28/12    yt     Limit length of response for READ_TRANSPARENT to 4096 bytes
02/28/12    nb     Support for new API to open session with MF
02/23/12    nmb    QMI QMUX Transition
02/23/12    at     Block APDUs on channels not opened by the client
02/23/12    av     Cleaned up remaining always on features
02/22/12    vv     Send PIN events in case of a decrease in PIN retries count
02/16/12    at     Block SAP APDU command that selects a non-telecom app
02/08/12    at     Block certain APDUs in send APDU command,
                   Added support for NV apdu_security_restrictions
01/27/12    at     Cleanup PIN states after UICC App reset refresh
01/26/12    vv     Fixed error code when PIN2 enabling/disabling is unsupported
01/03/12    nb     Added optional TLV for QMI configuration
12/21/11    nmb    Secure channel perso for apps
12/21/11    bcho   Legacy GSDI removal updates
12/20/11    hn     Add support for off-target testing
12/15/11    nb     Added handling of new card error code
12/05/11    vv     Update PIN information for all apps after PIN error
11/15/11    vv     Reset PIN states in case of deactivation
11/11/11    vv     Update PINs state in sub ready evt if PIN evts are missed
11/01/11    at     Skip perso indication to client if perso is disabled
10/28/11    yt     Return DEVICE_NOT_READY when session id is not available
10/24/11    at     Updated the minor version for QMI_UIM service
10/13/11    nb     Added handling of CARD_REMOVED
10/12/11    at     Handling intermediate get response in SAP commands
10/05/11    yt     Added support for refresh of non provisioning sessions
                   Removed flag to indicate if TLV is mandatory or optional
10/05/11    nb     Differentiate SAP Connect rejection due to active call
09/26/11    nmb    Maintain command buffers internally
09/16/11    yt     Added optional TLV for status of hot swap switch
08/26/11    at     Fixed KW critical warnings
08/18/11    at     Update app state when deactivated in session changed evt
08/16/11    at     Indication support for various commands
08/11/11    nmb    Expanded logical channels
08/05/11    vv     Added support for updating ISIM PIN status
08/03/11    mib    Added featurization for encryption functions
08/01/11    at     Support for keyref_id TLV in PIN commands
08/01/11    at     Support for using session id on an open channel id
08/01/11    vs     Fixed cnf deep copy pointer logic
08/01/11    vs     Support app selection with FCI via logical channel operation
07/11/11    yt     Handle error cases for Authentication failure
06/27/11    at     Added support for get ATR command
06/16/11    at     Send SAP indication on card error/removed
06/10/11    at     NV support for silent PIN1 verification
06/10/11    kk     Fixed compilation error
06/01/11    kk     Handling for card_pup failure
05/13/11    nmb    Added support for ISIM GBA Authentication
04/28/11    mib    Fixed crash due to invalid free reading multiple records
04/27/11    vs     Free user data in case sub ok req fails to get queued
04/26/11    vs     Support for subscription ok request
04/14/11    mib    Zero of allocated dynamic userdata for callbacks
04/07/11    mib    Added support to read multiple records at the same time
04/01/11    mib    Changed TLV for the encrypted PIN passed for verify
03/30/11    at     Fixed compiler warnings
03/30/11    yt     Send indication only if PIN/PUK retries are not exhausted
03/25/11    at     Support for dynamic allocation of userdata if needed
03/09/11    at     Added support for PIN1 encryption/decryption
03/07/11    at     Updating PIN retry counters properly on change pin event
03/03/11    mib    Added support to manage logical channels to send APDUs
03/01/11    at     Added support for SAP request & indication
02/10/11    mib    Fixed parsing of TLVs for Send APDU request
01/12/11    mib    Added support for CSIM file paths
01/04/11    mib    Added support for Compute IP Authentication
12/20/10    at     Added support for SAP connection request
12/17/10    mib    Split APDU request into two TLVs
12/14/10    mib    No perso retries for some error codes
12/08/10    at     Added support for send apdu request
12/09/10    mib    Fixed ASSERT in case of failure to open session
12/07/10    mib    Register for refresh even if 0 files are passed
12/07/10    at     Added new function for handling card removed event
12/05/10    mib    Fixed write record for cyclic files
11/08/10    mib    Fixed update of number of perso retries
11/02/10    mib    Check presence of optional TLV to activate a session
10/29/10    tkl    Return PIN2 restriction when set service failed
10/22/10    mib    Added support for get configuration request
10/22/10    mib    Check if client is still valid during async response
10/12/10    mib    Support for refresh by path
10/05/10    mib    Fixed number of perso retries
09/27/10    mib    Handling of session event for ILLEGAL
09/20/10    mib    Fixed no response for power up without card
09/17/10    mib    Fixed assert in get_label with slot 2 when not present
09/09/10    tkl    Fixed pin state change after PIN unblocked & changed.
09/09/10    mib    Fixed Klocwork warning
09/07/10    yt     Added support for Run CAVE and ISIM authentication context
08/21/10    mib    Fixed check condition during INIT+FCN refresh
                   Update app state during refresh and perso.
08/19/10    mib    Added check if client wants to vote for refresh
                   Added indipendent refresh state for each session
08/19/10    mib    Added support for POSSIBLY_REMOVED error code
08/12/10    mib    Fixed duplication of TLV data when opening a session
08/05/10    tkl    QMI UIM & QMI CAT split from data package
08/03/10    tkl    Updated card error event between power up without card and
                   card removal.
08/03/10    mib    Better handling of Perso error codes
08/02/10    mib    Send indication when card error changes
07/30/10    tkl    Fixed app state changed for after PIN unblocked.
07/26/10    mib    No effect for second event registration
07/26/10    mib    Return permanently blocked when PIN is blocked
07/26/10    yt     Updated request parameters for get_file_attr for MF or DF
07/16/10    mib    Updated number of PIN retries after confirmation
07/16/10    mib    Fixed error code for get_label when card is missing
07/14/10    yt     Process session open for nonprov apps based on client id
07/14/10    mib    Fixed critical Lint warning
07/08/10    mib    Updated minor revision number
07/07/10    yt     Added support for changing provisioning session
07/07/10    mib    Fixed compilation warning
06/25/10    mib    Restored open of 1X primary session only in parallel
06/24/10    tkl    Added checking of gw & 1x pri session id for deperso and
                   added activate session when process susbcription ready
06/21/10    mib    Opening default sessions in sequence
06/17/10    tkl    Fixed refresh registration when file id enum not found
06/16/10    mib    Added support for hidden key
06/11/10    mib    Fixed Lint warnings
06/08/10    mib    Pass card status in write response, in case of error
06/08/10    mib    Fixed TLV id for refresh indication
06/07/10    mib    Added support for GET LABEL and SESSION CHANGE requests
06/02/10    mib    Changed mapping of MMGSDI_INCORRECT_PARAMS. Reset card
                   error value when card is present
05/31/10    mib    Added storage of application label
05/24/10    mib    Modified mechanism to retrieve FDN status.
                   Process client id in QMI context.
05/20/10    tkl    Drop refresh callback for QMI power up operation
05/05/10    mib    Improved mechanism to pass TLV id for responses
04/13/10    mib    Fixed Klocwork issues
04/06/10    mib    Added support for authenticate and close session.
                   Changed TLV ids for consistency with other QMI services
01/11/10    mib    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "customer.h"

#include "nv.h"
#include "uim_msg.h"
#include "err.h"
#include "amssassert.h"
#include "intconv.h"
#include "fs_lib.h"
#include "fs_public.h"
#include "fs_diag_access.h"
#include "fs_stdlib.h"

#include "qmi_framework.h"
#include "qmi_svc_utils.h"
#include "modem_mem.h"
#include "secapi_util.h"
#include "mmgsdilib.h"
#include "mmgsdisessionlib.h"
#include "mmgsdisessionlib_v.h"
#include "mmgsdilib_common.h"
#include "mmgsdilib_p.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim_encryption.h"
#include "qmi_uim_simlock.h"
#include "qmi_uim_util.h"
#include "qmi_uim_gba.h"
#include "qmi_uim_sub_mgr.h"
#include "qmi_uim_recovery.h"
#include "qmi_uim_supply_voltage.h"
#include "ds_qmi_fw_common.h"
#include "qmi_uim.h"
#include "uim_p.h"
#include "qmi_cat.h"
#include "qmi_uim_internal.h"
#include "uimsub_manager.h"
#include "simlock_common.h"
#include "simlock_modem_lib.h"
#include "simlock_modem_p.h"
#include "gstk_exp_v.h"
#include "uim_common_efs.h"

#include "qmi_si.h"
#include "qmi_idl_lib.h"
#include "user_identity_module_v01.h"
#include "user_identity_module_impl_v01.h"
#include "qmi_uim_multi_profile.h"
#include "qmi_uim_sap.h"
#include "qmi_uim_m2m_euicc.h"
#include <stringl/stringl.h>

#ifdef FEATURE_UIM_SSM
#include "ssm.h"
#endif /* FEATURE_UIM_SSM */

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif /* FEATURE_QSH_MDUMP */

/* These includes need to always be at bottom - offtarget use only */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Major and Minor Version Nos for UIM
---------------------------------------------------------------------------*/
#define UIMI_BASE_VER_MAJOR           (1)
#define UIMI_BASE_VER_MINOR           (77)

#define UIMI_ADDENDUM_VER_MAJOR       (0)
#define UIMI_ADDENDUM_VER_MINOR       (0)

/*---------------------------------------------------------------------------
  Defines to be used for opening sessions
---------------------------------------------------------------------------*/
#define UIM_SESSION_OPEN_DEFAULT      (0x00)
#define UIM_SESSION_OPEN_IMPLICIT     (0x01)
#define UIM_SESSION_OPEN_APDU         (0x02)

/*---------------------------------------------------------------------------
  Define SSM access values
---------------------------------------------------------------------------*/
#define QMI_UIM_SSM_CMD_DISALLOW      (1)
#define QMI_UIM_SSM_KEY_NUM           (1)

/* First argument passed to ssm_get_perm, reflects the total num of
   arguments passed to it */
#define QMI_UIM_SSM_NUM_ARGS          (QMI_UIM_SSM_KEY_NUM + 2)

/* item # for restricted NV */
#define QMI_UIM_NV_NUM_HALT_SUB_NV_PATH           (65954)
#define QMI_UIM_NV_NUM_ENCRYPT_SUB_OK_NV_PATH     (67285)
#define QMI_UIM_NV_NUM_CONFIG_APDU_SEC_RESTR      (67312)
#define QMI_UIM_NV_NUM_CONFIG_APDU_SEC_AID_LIST   (67317)
#define QMI_UIM_NV_NUM_CONFIG_SAP_SEC_RESTR       (70283)
#define QMI_UIM_NV_NUM_CONFIG_AUTH_SEC_RESTR      (70284)
#define QMI_UIM_NV_NUM_CATI_BLOCK_SMS_PP_ENV      (71557)
#define QMI_UIM_NV_NUM_CONFIG_SILENT_RECOVERY     (72534)
/* There is no NV number for the enable remote service item
    Hence making this value to 0xFFFF*/
#define  QMI_UIM_NV_NUM_CONFIG_REMOTE_SERVICE     (0xFFFF)


/*---------------------------------------------------------------------------
  Macro used to free the command buffer
---------------------------------------------------------------------------*/
#define QMI_UIM_FREE_CMD_BUF(cmd_buf_p)                                   \
            if (cmd_buf_p != NULL)                                        \
            {                                                             \
              ds_qmi_fw_free_cmd_buf( &cmd_buf_p );                       \
            }

/*---------------------------------------------------------------------------
  Length of authentication data for CAVE algorithm
---------------------------------------------------------------------------*/
#define QMI_UIM_CAVE_AUTH_DATA_LEN      0x11

/*---------------------------------------------------------------------------
  Maximum length of a record
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_RECORD_LEN          255

/*---------------------------------------------------------------------------
  Maximum number of records in a file
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_NUM_RECORD          254

/*---------------------------------------------------------------------------
  First record number
---------------------------------------------------------------------------*/
#define QMI_UIM_RECORD_1                1

/*---------------------------------------------------------------------------
  Values used in SAP response
---------------------------------------------------------------------------*/
#define QMI_UIM_INS_BYTE_GET_RESPONSE       0xC0
#define QMI_UIM_PROCEDURE_BYTE_SW1_NORMAL   0x61
#define QMI_UIM_PROCEDURE_BYTE_SW2_ZERO     0x00

/*---------------------------------------------------------------------------
  AID details in NV apdu_security_aid_list
---------------------------------------------------------------------------*/
#define QMI_UIM_APDU_SEC_NV_MAX_AID_ENTRIES    (30)
#define QMI_UIM_APDU_SEC_NV_MAX_AID_LEN        (16)

/*---------------------------------------------------------------------------
  Terminal Capability related defines
---------------------------------------------------------------------------*/
#define QMI_UIM_ADDITIONAL_INTERFACE_SUPPORTED_TAG      0x82
#define QMI_UIM_ADDITIONAL_INTERFACE_SUPPORTED_LEN      0x01
#define QMI_UIM_LPA_SUPPORTED_TAG                       0x83
#define QMI_UIM_LPA_SUPPORTED_LEN                       0x01
#define QMI_UIM_TERMINAL_POWER_SUPPLY_TAG               0x80
#define QMI_UIM_EXTENDED_LOGICAL_CHANNELS_SUPPORT_TAG   0x81
#define QMI_UIM_TERMINAL_CAPABILITY_MAX_LEN             0xFD

/*===========================================================================

                                DATA TYPES

===========================================================================*/
/* Service type enum */
typedef enum {
  QMI_UIM_SERVICE_FDN                    = 0x00,
  QMI_UIM_SERVICE_ACL,
  QMI_UIM_SERVICE_MAX                    = 0x7F
} qmi_uim_service_enum_type;


/*===========================================================================

                               INTERNAL DATA

===========================================================================*/

/*---------------------------------------------------------------------------
  Card update required type used for card status indications
---------------------------------------------------------------------------*/
typedef struct
{
  boolean card_update_required;
  boolean reduced_card_update_required;
} qmi_uim_card_update_required_type;

qmi_uim_global_type          qmi_uim_global_var = {FALSE};
qmi_uim_global_type  * const qmi_uim_global_ptr = &qmi_uim_global_var;

typedef struct
{
  char *    nv_filename ;
  uint32    nv_item;
}qmi_uim_restricted_nv_files_type;

const static qmi_uim_restricted_nv_files_type  qmi_uim_restricted_nv_files[] =
{
  {UIM_COMMON_EFS_PATH_MMGSDI_HALT_SUB,                         QMI_UIM_NV_NUM_HALT_SUB_NV_PATH},
  {UIM_COMMON_EFS_PATH_MMGSDI_ENCRYPT_SUB,                      QMI_UIM_NV_NUM_ENCRYPT_SUB_OK_NV_PATH},
  {UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_RESTR,    QMI_UIM_NV_NUM_CONFIG_APDU_SEC_RESTR},
  {UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_AID_LIST, QMI_UIM_NV_NUM_CONFIG_APDU_SEC_AID_LIST},
  {UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_SAP_SEC_RESTR,     QMI_UIM_NV_NUM_CONFIG_SAP_SEC_RESTR},
  {UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_AUTH_SEC_RESTR,    QMI_UIM_NV_NUM_CONFIG_AUTH_SEC_RESTR},
  {UIM_COMMON_EFS_PATH_UIMQMI_QMI_CAT_BLOCK_SMS_PP_ENV,         QMI_UIM_NV_NUM_CATI_BLOCK_SMS_PP_ENV},
  {UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_SILENT_RECOVERY,   QMI_UIM_NV_NUM_CONFIG_SILENT_RECOVERY},
  {UIM_COMMON_EFS_PATH_UIMQMI_QMI_UIM_CONFIG_REMOTE_SERVICE,    QMI_UIM_NV_NUM_CONFIG_REMOTE_SERVICE}
};

/*---------------------------------------------------------------------------
  QMI service command handlers
  forward declarations & cmd handler dispatch table definition
---------------------------------------------------------------------------*/
static dsm_item_type* qmi_uimi_reset(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_read_transparent(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_read_record(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_write_transparent(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_write_record(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_file_attributes(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_set_pin_protection(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_verify_pin(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_unblock_pin(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_change_pin(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_refresh_register(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_refresh_ok(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_refresh_complete(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_refresh_get_last_event(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_event_reg(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_card_status(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_power_down(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_power_up(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_authenticate(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_close_session(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_service_status(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_set_service_status(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_change_provisioning_session(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_label(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_configuration(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_send_apdu(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_logical_channel(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_subscription_ok(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_atr(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_open_logical_channel(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_refresh_register_all(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_set_file_status(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_plmn_name_table_info(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_increase(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_reselect(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_send_status(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_search_record(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_purge_temporary_identity(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_switch_slot_ext(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_physical_slot_mapping(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_get_eid(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_terminal_capability(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_set_apdu_behavior(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_uimi_map_provisioning_sessions_to_slots(void*, void*, void*, dsm_item_type **);

#define UIM_HDLR(a,b)  QMI_SVC_HDLR( (uint16)a, (qmi_svc_hdlr_ftype)b )

static qmi_svc_cmd_hdlr_type  qmi_uimi_cmd_callbacks[] =
{
  UIM_HDLR( UIMI_CMD_VAL_RESET,                       qmi_uimi_reset),
  UIM_HDLR( UIMI_CMD_VAL_READ_TRANSPARENT,            qmi_uimi_read_transparent),
  UIM_HDLR( UIMI_CMD_VAL_READ_RECORD,                 qmi_uimi_read_record),
  UIM_HDLR( UIMI_CMD_VAL_WRITE_TRANSPARENT,           qmi_uimi_write_transparent),
  UIM_HDLR( UIMI_CMD_VAL_WRITE_RECORD,                qmi_uimi_write_record),
  UIM_HDLR( UIMI_CMD_VAL_GET_FILE_ATTRIBUTES,         qmi_uimi_get_file_attributes),
  UIM_HDLR( UIMI_CMD_VAL_SET_PIN_PROTECTION,          qmi_uimi_set_pin_protection),
  UIM_HDLR( UIMI_CMD_VAL_VERIFY_PIN,                  qmi_uimi_verify_pin),
  UIM_HDLR( UIMI_CMD_VAL_UNBLOCK_PIN,                 qmi_uimi_unblock_pin),
  UIM_HDLR( UIMI_CMD_VAL_CHANGE_PIN,                  qmi_uimi_change_pin),
  UIM_HDLR( UIMI_CMD_VAL_REFRESH_REGISTER,            qmi_uimi_refresh_register),
  UIM_HDLR( UIMI_CMD_VAL_REFRESH_OK,                  qmi_uimi_refresh_ok),
  UIM_HDLR( UIMI_CMD_VAL_REFRESH_COMPLETE,            qmi_uimi_refresh_complete),
  UIM_HDLR( UIMI_CMD_VAL_REFRESH_GET_LAST_EVENT,      qmi_uimi_refresh_get_last_event),
  UIM_HDLR( UIMI_CMD_VAL_EVENT_REG,                   qmi_uimi_event_reg),
  UIM_HDLR( UIMI_CMD_VAL_GET_CARD_STATUS,             qmi_uimi_get_card_status),
  UIM_HDLR( UIMI_CMD_VAL_POWER_DOWN,                  qmi_uimi_power_down),
  UIM_HDLR( UIMI_CMD_VAL_POWER_UP,                    qmi_uimi_power_up),
  UIM_HDLR( UIMI_CMD_VAL_AUTHENTICATE,                qmi_uimi_authenticate),
  UIM_HDLR( UIMI_CMD_VAL_CLOSE_SESSION,               qmi_uimi_close_session),
  UIM_HDLR( UIMI_CMD_VAL_GET_SERVICE_STATUS,          qmi_uimi_get_service_status),
  UIM_HDLR( UIMI_CMD_VAL_SET_SERVICE_STATUS,          qmi_uimi_set_service_status),
  UIM_HDLR( UIMI_CMD_VAL_CHANGE_PROVISIONING_SESSION, qmi_uimi_change_provisioning_session),
  UIM_HDLR( UIMI_CMD_VAL_GET_LABEL,                   qmi_uimi_get_label),
  UIM_HDLR( UIMI_CMD_VAL_GET_CONFIGURATION,           qmi_uimi_get_configuration),
  UIM_HDLR( UIMI_CMD_VAL_SEND_APDU,                   qmi_uimi_send_apdu),
  UIM_HDLR( UIMI_CMD_VAL_SAP_CONNECTION,              qmi_uimi_sap_connection),
  UIM_HDLR( UIMI_CMD_VAL_SAP_REQUEST,                 qmi_uimi_sap_request),
  UIM_HDLR( UIMI_CMD_VAL_LOGICAL_CHANNEL,             qmi_uimi_logical_channel),
  UIM_HDLR( UIMI_CMD_VAL_SUBSCRIPTION_OK_REQ,         qmi_uimi_subscription_ok),
  UIM_HDLR( UIMI_CMD_VAL_GET_ATR,                     qmi_uimi_get_atr),
  UIM_HDLR( UIMI_CMD_VAL_OPEN_LOGICAL_CHANNEL,        qmi_uimi_open_logical_channel),
  UIM_HDLR( UIMI_CMD_VAL_REFRESH_REGISTER_ALL,        qmi_uimi_refresh_register_all),
  UIM_HDLR( UIMI_CMD_VAL_SET_FILE_STATUS,             qmi_uimi_set_file_status),
  UIM_HDLR( UIMI_CMD_VAL_SWITCH_SLOT,                 qmi_uimi_switch_slot),
  UIM_HDLR( UIMI_CMD_VAL_GET_SLOTS_STATUS,            qmi_uimi_get_slots_status),
  UIM_HDLR( UIMI_CMD_VAL_GET_PLMN_NAME_TABLE_INFO,    qmi_uimi_get_plmn_name_table_info),
  UIM_HDLR( UIMI_CMD_VAL_INCREASE,                    qmi_uimi_increase),
  UIM_HDLR( UIMI_CMD_VAL_RECOVERY,                    qmi_uimi_recovery),
  UIM_HDLR( UIMI_CMD_VAL_RESELECT,                    qmi_uimi_reselect),
  UIM_HDLR( UIMI_CMD_VAL_SEND_STATUS,                 qmi_uimi_send_status),
  UIM_HDLR( UIMI_CMD_VAL_GET_SIM_PROFILE,             qmi_uimi_get_sim_profile),
  UIM_HDLR( UIMI_CMD_VAL_SET_SIM_PROFILE,             qmi_uimi_set_sim_profile),
  UIM_HDLR( UIMI_CMD_VAL_SUPPLY_VOLTAGE,              qmi_uimi_supply_voltage),
#ifdef FEATURE_SIMLOCK
  UIM_HDLR( UIMI_CMD_VAL_PERSONALIZATION,             qmi_uimi_personalization),
  UIM_HDLR( UIMI_CMD_VAL_DEPERSONALIZATION,           qmi_uimi_depersonalization),
  UIM_HDLR( UIMI_CMD_VAL_DEPERSONALIZATION_SECURE,    qmi_uimi_depersonalization_secure),
  UIM_HDLR( UIMI_CMD_VAL_PERSONALIZATION_SECURE,      qmi_uimi_personalization_secure),
  UIM_HDLR( UIMI_CMD_VAL_EMERGENCY_ONLY,              qmi_uimi_emergency_only),
  UIM_HDLR( UIMI_CMD_VAL_SIMLOCK_CONFIGURATION,       qmi_uimi_simlock_configuration),
  UIM_HDLR( UIMI_CMD_VAL_REMOTE_UNLOCK,               qmi_uimi_remote_unlock),
  UIM_HDLR( UIMI_CMD_VAL_VERIFY_IMSI,                 qmi_uimi_verify_imsi),
  UIM_HDLR( UIMI_CMD_VAL_ACTIVATE_SIMLOCK_CONFIG,     qmi_uimi_activate_simlock_configuration),
  UIM_HDLR( UIMI_CMD_VAL_GET_SIMLOCK_TOKEN,           qmi_uimi_get_simlock_token),
#endif /* FEATURE_SIMLOCK */
  UIM_HDLR( UIMI_CMD_VAL_GBA,                         qmi_uimi_gba),
  UIM_HDLR( UIMI_CMD_VAL_GET_GBA_IMPI,                qmi_uimi_get_gba_impi),
  UIM_HDLR( UIMI_CMD_VAL_SEARCH_RECORD,               qmi_uimi_search_record),
  UIM_HDLR( UIMI_CMD_VAL_PURGE_TEMPORARY_IDENTITY,    qmi_uimi_purge_temporary_identity),
  UIM_HDLR( UIMI_CMD_VAL_SWITCH_SLOT_EXT,             qmi_uimi_switch_slot_ext),
  UIM_HDLR( UIMI_CMD_VAL_GET_PHYSICAL_SLOT_MAPPING,   qmi_uimi_get_physical_slot_mapping),
  UIM_HDLR( UIMI_CMD_VAL_GET_EID,                     qmi_uimi_get_eid),
  UIM_HDLR( UIMI_CMD_VAL_DELETE_PROFILE,              qmi_uimi_delete_profile),
  UIM_HDLR( UIMI_CMD_VAL_EUICC_MEMORY_RESET,          qmi_uimi_euicc_memory_reset),
  UIM_HDLR( UIMI_CMD_VAL_ADD_PROFILE,                 qmi_uimi_add_profile),
  UIM_HDLR( UIMI_CMD_VAL_UPDATE_NICKNAME,             qmi_uimi_update_profile_nickname),
  UIM_HDLR( UIMI_CMD_VAL_GET_PROFILE_INFO,            qmi_uimi_get_profile_info),
  UIM_HDLR( UIMI_CMD_VAL_TERMINAL_CAPABILITY,         qmi_uimi_terminal_capability),
  UIM_HDLR( UIMI_CMD_VAL_SET_APDU_BEHAVIOR,           qmi_uimi_set_apdu_behavior),
  UIM_HDLR( UIMI_CMD_VAL_PROFILE_USER_CONSENT,        qmi_uimi_profile_user_consent),
  UIM_HDLR( UIMI_CMD_VAL_EUICC_DEFAULT_SERVER_ADDRESS, qmi_uimi_euicc_default_server_address),
  UIM_HDLR( UIMI_CMD_VAL_MAP_PROVISIONING_SESSIONS_TO_SLOTS, qmi_uimi_map_provisioning_sessions_to_slots)
  /* NOTE: When adding an item to this table, also add it to
?? qmi_uim_is_command_disabled() */
};


/*---------------------------------------------------------------------------
  QMI instance configurations
---------------------------------------------------------------------------*/
qmi_uimi_state_type           *qmi_uim_state_ptr = NULL;

/* Prototype of MMGSDI event function */
static void qmi_uim_mmgsdi_evt_cback(const mmgsdi_event_data_type * event);

#ifdef FEATURE_UIM_SSM
/* Prototype of SSM callback function */
static void qmi_uim_ssm_cback(ssm_err_t err);
#endif /* FEATURE_UIM_SSM */

/* Prototype of QMI UIM generic message processing */
static void qmi_uim_process_message(void);

#ifdef FEATURE_GSTK
/* Prototype of GSTK OTASP registration callback function */
static void qmi_uimi_otasp_reg_callback(
  gstk_status_enum_type      gstk_reg_status,
  gstk_client_ref_data_type  user_data
);

/* Prototype of GSTK OTASP status callback function */
static void qmi_uimi_otasp_status_callback(
  gstk_slot_id_enum_type       slot,
  gstk_otasp_act_status_enum_type  status
);
#endif /* FEATURE_GSTK */


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_DIAG_ACCESS_CHECK_CB()

  DESCRIPTION
    This function is called by the Diag module as a result of any Diag access
    of the QMI_UIM EFS directory. It is initially registered with Diag for
    knowing the access of QMI_UIM EFS directory.

    Note: In order to enable the NVs in the restricted NV files list, the
    corresponding MCFG HW MBN entry needs to be updated.

  PARAMETERS
    file_name_ptr: Name of the file that needs to be permitted
    op_type      : File operation type

  RETURN VALUE
    TRUE:  If we want to allow access
    FALSE: If we want to reject access

  DEPENDENCIES
    QMI UIM service must be registered with Diag module via
    diag_fs_register_access_check API.

  SIDE EFFECTS
    None

===========================================================================*/
static boolean qmi_uim_diag_access_check_cb
(
  char      * file_name_ptr,
  uint32      op_type
)
{
  uint16                       filename_length = 0;
  uint8                        nv_file_count   = 0;
  uint8                        i               = 0;
  uint32                       nv_item         = 0;
#ifdef FEATURE_UIM_SSM
  ssm_perm_data_t              ssm_perm        = QMI_UIM_SSM_CMD_DISALLOW;
  ssm_err_t                    ssm_err         = E_SSM_SUCCESS;
#endif /* FEATURE_UIM_SSM */
  char                       * path_buf_ptr    = NULL;
  char                       * input_path_ptr  = NULL;

  UIM_MSG_HIGH_0("qmi_uim_diag_access_check_cb");
  ASSERT(qmi_uim_global_ptr != NULL);

  if (file_name_ptr == NULL)
  {
    /* Not sure what the file is, so allow this diag request */
    return TRUE;
  }

  /* The conditions for rejecting are these:
     1. File name is of interest to us
     2. Operation is one of write, delete, rename, chmod
     3. Device is not in FTM mode */

  nv_file_count = sizeof(qmi_uim_restricted_nv_files) / sizeof(qmi_uim_restricted_nv_files_type);

  path_buf_ptr = (char*)uimqmi_malloc(FS_DIAG_ACCESS_PATH_MAX+1);
  if (path_buf_ptr == NULL)
  {
    return FALSE;
  }

  /* efs_realpath() will return NULL upon error but an error can mean a number
     of things, for example, if it is not able to find the file path specified
     in the efs which is not really an error in this case. Even in case of error
     however, the second parameter will return the parsed file path or at least
     the best it was able to do until it ran into an error. For our purposes we
     can ignore the return value because the subsequent comparisons will fail
     anyways if the path was not parsed correctly. */
  (void)efs_realpath (file_name_ptr, path_buf_ptr);

  filename_length = (uint16)strnlen(path_buf_ptr, FS_DIAG_ACCESS_PATH_MAX);

  input_path_ptr = (char*)uimqmi_malloc(filename_length+1);
  if (input_path_ptr == NULL)
  {
    uimqmi_free(path_buf_ptr);
    return FALSE;
  }

  (void)memscpy(input_path_ptr,
                filename_length+1,
                path_buf_ptr,
                filename_length+1);

  for (i = 0; i < nv_file_count; i++)
  {
    memset(path_buf_ptr, 0x00, (FS_DIAG_ACCESS_PATH_MAX+1));
    (void)efs_realpath (qmi_uim_restricted_nv_files[i].nv_filename, path_buf_ptr);

    if (filename_length == (uint16)strnlen(path_buf_ptr, FS_DIAG_ACCESS_PATH_MAX) &&
        (memcmp((void*)path_buf_ptr,
                (void*)input_path_ptr,
                filename_length) == 0))
    {
      /* found index */
      nv_item = qmi_uim_restricted_nv_files[i].nv_item;
      break;
    }
  }

  uimqmi_free(path_buf_ptr);
  path_buf_ptr = NULL;
  uimqmi_free(input_path_ptr);
  input_path_ptr = NULL;

  if (i >= nv_file_count )
  {
    /* File name is not of interest to us, allow access */
    return TRUE;
  }

  if(nv_item == QMI_UIM_NV_NUM_CONFIG_REMOTE_SERVICE)
  {
    UIM_MSG_HIGH_0("Access for the remote enable NV is restricted");
    return FALSE;
  }

  if ( (op_type == DIAG_FS_CHECK_OPEN) ||
       (op_type == DIAG_FS_CHECK_DISP_DIRS) ||
       (op_type == DIAG_FS_CHECK_DISP_FILES) ||
       (op_type == DIAG_FS_CHECK_ITERATE) ||
       (op_type == DIAG_FS_CHECK_READ) ||
       (op_type == DIAG_FS_CHECK_GET_ATTRIB) ||
       (op_type == DIAG_FS_CHECK_CLOSE) ||
       (op_type == DIAG_FS_CHECK_OPENDIR) ||
       (op_type == DIAG_FS_CHECK_READDIR) ||
       (op_type == DIAG_FS_CHECK_CLOSEDIR) ||
       (op_type == DIAG_FS_CHECK_STAT) ||
       (op_type == DIAG_FS_CHECK_LSTAT) ||
       (op_type == DIAG_FS_CHECK_FSTAT) ||
       (op_type == DIAG_FS_CHECK_STATFS) ||
       (op_type == DIAG_FS_CHECK_ACCESS))
  {
    /* Allow access */
    return TRUE;
  }

#ifdef FEATURE_UIM_SSM
  if( qmi_uim_global_ptr->ssm_info.qmi_uim_ssm_init_err != E_SSM_SUCCESS )
  {
    UIM_MSG_HIGH_1("SSM init error: 0x%x",
                   qmi_uim_global_ptr->ssm_info.qmi_uim_ssm_init_err);
    return FALSE;
  }

  ssm_err = ssm_get_perm(&ssm_perm,
                         QMI_UIM_SSM_NUM_ARGS,
                         qmi_uim_global_ptr->ssm_info.qmi_uim_ssm_id,
                         nv_item);

  if ( ssm_err != E_SSM_SUCCESS )
  {
    UIM_MSG_HIGH_1("Failed to get ssm perm 0x%x", ssm_err);
    return FALSE;
  }

  if (ssm_perm == QMI_UIM_SSM_CMD_DISALLOW)
  {
    UIM_MSG_HIGH_1("SSM returned deny access to NV file %d", nv_item);
    return FALSE;
  }
#endif /* FEATURE_UIM_SSM */

  return FALSE;
} /* qmi_uim_diag_access_check_cb */


/*===========================================================================
  FUNCTION QMI_UIM_READ_CONFIGURATION_APDU_AID_LIST()

  DESCRIPTION
    Reads the configuration value for APDU security AID list from the EFS.
    This is the default value set based on what is read:
    1. If NV item is not present - empty black list is assumed
    2. Invalid value for type - empty black list is assumed

  PARAMETERS
    apdu_sec_aid_info_ptr: Pointer to the APDU security AID info

  RETURN VALUE
    Boolean that indicates successful read or failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_read_configuration_apdu_aid_list
(
  qmi_uimi_apdu_sec_aid_info_type * apdu_sec_aid_info_ptr
)
{
  uint16          i                                     = 0;
  uint16          j                                     = 0;
  uint16          offset                                = 0;
  uint8           aid_len                               = 0;
  uint8         * aid_list_file_data_ptr                = NULL;
  uint16          aid_list_file_data_len                = 0;
  uint32          alloc_size                            = 0;
  uint8           isdr_aid_initials[]                   = {0xA0, 0x00, 0x00, 0x05,
                                                           0x59, 0x10, 0x10};
  qmi_uimi_aid_list_type  isdr_add_in_list              = UIM_AID_LIST_WHITE_LIST;
  uint8                   lpa_ap_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LEN]
                                                        = {0};
  boolean                 is_read_from_efs              = FALSE;
  boolean                 lpa_modem_solution            = TRUE;

  ASSERT(apdu_sec_aid_info_ptr);

  if (qmi_uim_global_ptr == NULL)
  {
    return FALSE;
  }

  /* Default values if NV item is not present - empty black list */
  apdu_sec_aid_info_ptr->aid_count     = 0;
  apdu_sec_aid_info_ptr->aid_list_ptr  = NULL;
  apdu_sec_aid_info_ptr->aid_list_type = UIM_AID_LIST_BLACK_LIST;

  /* This is the max length that is supported in the NV item file:
     type (uint8) + list_size (uint8) + max AID entries times each entry
     In NV, each AID entry can hold a max of QMI_UIM_APDU_SEC_NV_MAX_AID_LEN bytes */
  aid_list_file_data_len = sizeof(uint8) + sizeof(uint8) +
    (QMI_UIM_APDU_SEC_NV_MAX_AID_ENTRIES * (sizeof(uint8) + QMI_UIM_APDU_SEC_NV_MAX_AID_LEN));

  /* Allocate sufficient memory for the file contents */
  aid_list_file_data_ptr = uimqmi_malloc(aid_list_file_data_len);
  if (aid_list_file_data_ptr == NULL)
  {
    return FALSE;
  }

  memset(aid_list_file_data_ptr, 0, aid_list_file_data_len);

  /* Read the file. If read fails, means there is no list present.
     We need to block ISDR-AID from AP if the NV 73849 byte 2
     is not set */
  if (uim_common_efs_read(
        UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_AID_LIST,
        UIM_COMMON_EFS_ITEM_FILE_TYPE,
        UIM_COMMON_EFS_DEVICE,
        aid_list_file_data_ptr,
        aid_list_file_data_len)!= UIM_COMMON_EFS_SUCCESS)
  {
    UIM_MSG_ERR_0( "Error in efs_get for APDU security AID list");
    uimqmi_free(aid_list_file_data_ptr);
    aid_list_file_data_ptr = NULL;
    apdu_sec_aid_info_ptr->aid_list_type = UIM_AID_LIST_BLACK_LIST;
    isdr_add_in_list = UIM_AID_LIST_BLACK_LIST;
  }
  else
  {
    is_read_from_efs = TRUE;

    /* Start parsing & update the global */
    UIM_MSG_HIGH_2( "APDU security AID list type: 0x%x, list count: 0x%x",
                    *aid_list_file_data_ptr,
                    *(aid_list_file_data_ptr + 1));
    switch(*aid_list_file_data_ptr)
    {
      case 0:
        apdu_sec_aid_info_ptr->aid_list_type = UIM_AID_LIST_WHITE_LIST;
        break;
      case 1:
        apdu_sec_aid_info_ptr->aid_list_type = UIM_AID_LIST_BLACK_LIST;
        break;
      default:
        uimqmi_free(aid_list_file_data_ptr);
        return FALSE;
    }

    /* Second byte has the number of AID entries */
    apdu_sec_aid_info_ptr->aid_count = *(aid_list_file_data_ptr + 1);

    if (apdu_sec_aid_info_ptr->aid_count == 0)
    {
      uimqmi_free(aid_list_file_data_ptr);
      aid_list_file_data_ptr = NULL;

      /* Since the list count is 0, if it is Blacklist
         then add ISDR-AID to the blacklist */
      if (apdu_sec_aid_info_ptr->aid_list_type == UIM_AID_LIST_BLACK_LIST)
      {
        isdr_add_in_list = UIM_AID_LIST_BLACK_LIST;
      }
      else
      {
        return FALSE;
      }
    }
  }

  /* Allocate memory based on the number of AID entries. 1 extra entry space
     is allocated for ISD-R in case of Blacklist because access to ISD-R is
     only through LPA module. Add ISD-R AID in Blacklist explicitly.
     ISD-R AID: A0 00 00 05 59 10 10 FF FF FF FF 89 00 00 01 00 */
  alloc_size = apdu_sec_aid_info_ptr->aid_count * sizeof(qmi_uim_aid_entry_type);

  /* Check LPA support EFS. Byte 2 determines whether LPA functionality
     is controlled by APPs or not. If yes, then ISDR needs to be added
     in Whitelist. If No, then ISDR needs to be added in Blacklist */
  lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);

  if (lpa_modem_solution == FALSE)
  {
    isdr_add_in_list  = UIM_AID_LIST_WHITE_LIST;
  }
  else
  {
    isdr_add_in_list  = UIM_AID_LIST_BLACK_LIST;
  }

  if ((apdu_sec_aid_info_ptr->aid_list_type == UIM_AID_LIST_BLACK_LIST &&
       isdr_add_in_list == UIM_AID_LIST_BLACK_LIST)                         ||
      (apdu_sec_aid_info_ptr->aid_list_type == UIM_AID_LIST_WHITE_LIST &&
       isdr_add_in_list == UIM_AID_LIST_WHITE_LIST))
  {
    alloc_size += sizeof(qmi_uim_aid_entry_type);
  }

  if (alloc_size == 0)
  {
    if(aid_list_file_data_ptr != NULL)
    {
      uimqmi_free(aid_list_file_data_ptr);
    }
    return FALSE;
  }

  apdu_sec_aid_info_ptr->aid_list_ptr = uimqmi_malloc(alloc_size);

  if (apdu_sec_aid_info_ptr->aid_list_ptr == NULL)
  {
    if (aid_list_file_data_ptr != NULL)
    {
      uimqmi_free(aid_list_file_data_ptr);
    }
    apdu_sec_aid_info_ptr->aid_count     = 0;
    return FALSE;
  }

  memset(apdu_sec_aid_info_ptr->aid_list_ptr, 0, alloc_size);

  /* If aid_count retrieved from EFS is non-zero then only add the AIDs
     else directly add the ISDR-AID to blacklist/whitelist */
  if (apdu_sec_aid_info_ptr->aid_count > 0 &&
      aid_list_file_data_ptr != NULL)
  {
    offset = QMI_UIM_APDU_SEC_NV_MAX_AID_LEN + sizeof(aid_len);

    /* Parse & fill up each AID entry, 2nd byte onwards must have the AID entries */
    for (i = 2, j = 0;
         (i + offset) <= aid_list_file_data_len && j < apdu_sec_aid_info_ptr->aid_count;
         i += offset)
    {
      aid_len = *(aid_list_file_data_ptr + i);

      /* Check individual AID length & skip it in case of incorrect length */
      if (aid_len > 0 && aid_len <= MMGSDI_MAX_AID_LEN)
      {
        apdu_sec_aid_info_ptr->aid_list_ptr[j].aid.data_len = aid_len;
        (void)memscpy((void*)apdu_sec_aid_info_ptr->aid_list_ptr[j].aid.data_ptr,
                      sizeof(apdu_sec_aid_info_ptr->aid_list_ptr[j].aid.data_ptr),
                      (void*)(aid_list_file_data_ptr + i + sizeof(aid_len)),
                      apdu_sec_aid_info_ptr->aid_list_ptr[j].aid.data_len);

        apdu_sec_aid_info_ptr->aid_list_ptr[j].from_efs = TRUE;
        j++;
      }
      else
      {
        break;
      }
    }
  }

  if ((apdu_sec_aid_info_ptr->aid_list_type == UIM_AID_LIST_BLACK_LIST &&
         isdr_add_in_list == UIM_AID_LIST_BLACK_LIST)                         ||
        (apdu_sec_aid_info_ptr->aid_list_type == UIM_AID_LIST_WHITE_LIST &&
         isdr_add_in_list == UIM_AID_LIST_WHITE_LIST))
  {
    apdu_sec_aid_info_ptr->aid_list_ptr[j].aid.data_len = sizeof(isdr_aid_initials);
    (void)memscpy((void *)apdu_sec_aid_info_ptr->aid_list_ptr[j].aid.data_ptr,
                  sizeof(apdu_sec_aid_info_ptr->aid_list_ptr[j].aid.data_ptr),
                  (void *)isdr_aid_initials,
                  sizeof(isdr_aid_initials));
    apdu_sec_aid_info_ptr->aid_list_ptr[j].from_efs = FALSE;

    apdu_sec_aid_info_ptr->aid_count++;
  }

  return is_read_from_efs;
} /* qmi_uim_read_configuration_apdu_aid_list */


/*===========================================================================
  FUNCTION QMI_UIM_READ_CONFIGURATION()

  DESCRIPTION
    Reads the configuration value for silent PIN1 verification from the EFS.
    If NV item is not present or has an invalid value, we do silent PIN1
    verification by default.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_read_configuration
(
  boolean                                            * silent_pin1_supported_ptr,
  qmi_uim_apdu_sec_restr_type                        * apdu_sec_value_ptr,
  qmi_uimi_apdu_sec_aid_info_type                    * apdu_sec_aid_info_ptr,
  boolean                                            * close_channel_ind_pref_ptr,
  qmi_uim_auth_sec_restr_type                        * auth_sec_restr_value_ptr,
  boolean                                            * sap_sec_restr_supported_ptr,
  qmi_uim_simlock_display_type                       * simlock_display_for_valid_card_ptr
)
{
  uim_common_efs_status_enum_type status        = UIM_COMMON_EFS_ERROR;
  uint8                           value         = 0;
  uint8                           success_count = 0;

  ASSERT(silent_pin1_supported_ptr &&
         apdu_sec_value_ptr &&
         close_channel_ind_pref_ptr &&
         auth_sec_restr_value_ptr &&
         sap_sec_restr_supported_ptr &&
         simlock_display_for_valid_card_ptr);

  /* Set default values */
  *silent_pin1_supported_ptr          = TRUE;
  *apdu_sec_value_ptr                 = QMI_UIM_APDU_SEC_RESTRICTIONS_ALL;
  *close_channel_ind_pref_ptr         = FALSE;
#ifdef FEATURE_CUST_1
  *auth_sec_restr_value_ptr           = QMI_UIM_AUTH_SEC_RESTRICTIONS_NONE;
#else
  *auth_sec_restr_value_ptr           = QMI_UIM_AUTH_SEC_RESTRICTIONS_II_CLA;
#endif /* FEATURE_CUST_1 */
  *sap_sec_restr_supported_ptr        = TRUE;
  *simlock_display_for_valid_card_ptr = QMI_UIM_SIMLOCK_DISPLAY_DEFAULT;

  /* Retrieve PIN1 configuration from NV file */
  status = uim_common_efs_read(
             UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_SILENT_PIN1_FILE,
             UIM_COMMON_EFS_ITEM_FILE_TYPE,
             UIM_COMMON_EFS_DEVICE,
             &value,
             sizeof(uint8));
  if (status == UIM_COMMON_EFS_SUCCESS)
  {
    success_count++;

    switch(value)
    {
      case 0:
        *silent_pin1_supported_ptr = FALSE;
        break;
      case 1:
      default:
        /* Nothing to do */
        break;
    }
  }

  /* Retrieve APDU security restrictions value from NV file */
  status = uim_common_efs_read(
             UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_APDU_SEC_RESTR,
             UIM_COMMON_EFS_ITEM_FILE_TYPE,
             UIM_COMMON_EFS_DEVICE,
             &value,
             sizeof(uint8));
  if (status == UIM_COMMON_EFS_SUCCESS)
  {
    success_count++;

    switch(value)
    {
      case 0:
        *apdu_sec_value_ptr = QMI_UIM_APDU_SEC_RESTRICTIONS_NONE;
        break;
      case 1:
        *apdu_sec_value_ptr = QMI_UIM_APDU_SEC_RESTRICTIONS_ALL;
        break;
      case 2:
        *apdu_sec_value_ptr = QMI_UIM_APDU_SEC_RESTRICTIONS_II_CLA;
        break;
      default:
        /* Nothing to do, default value is all restrictions */
        break;
    }
  }

  /* Retrieve APDU security AID list info from NV file */
  if (qmi_uim_read_configuration_apdu_aid_list(apdu_sec_aid_info_ptr))
  {
    success_count++;
  }

  /* Retrieve close channel indication preference from NV file */
  status = uim_common_efs_read(
             UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_CLOSE_CHANNEL_SYNC,
             UIM_COMMON_EFS_ITEM_FILE_TYPE,
             UIM_COMMON_EFS_DEVICE,
             &value,
             sizeof(uint8));
  if (status == UIM_COMMON_EFS_SUCCESS)
  {
    success_count++;

    switch(value)
    {
      case 1:
        *close_channel_ind_pref_ptr = TRUE;
        break;
      case 0:
      default:
        /* Nothing to do */
        break;
    }
  }

  /* Retrieve AUTHENTICATE security restrictions value from NV file */
  status = uim_common_efs_read(
             UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_AUTH_SEC_RESTR,
             UIM_COMMON_EFS_ITEM_FILE_TYPE,
             UIM_COMMON_EFS_DEVICE,
             &value,
             sizeof(uint8));
  if (status == UIM_COMMON_EFS_SUCCESS)
  {
    success_count++;

    switch(value)
    {
      case 0:
        *auth_sec_restr_value_ptr = QMI_UIM_AUTH_SEC_RESTRICTIONS_NONE;
        break;
      case 1:
        *auth_sec_restr_value_ptr = QMI_UIM_AUTH_SEC_RESTRICTIONS_ALL;
        break;
      case 2:
        *auth_sec_restr_value_ptr = QMI_UIM_AUTH_SEC_RESTRICTIONS_II_CLA;
        break;
      default:
        /* Nothing to do */
        break;
    }
  }

  /* Retrieve SAP security restrictions value from NV file */
  status = uim_common_efs_read(
             UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_SAP_SEC_RESTR,
             UIM_COMMON_EFS_ITEM_FILE_TYPE,
             UIM_COMMON_EFS_DEVICE,
             &value,
             sizeof(uint8));
  if (status == UIM_COMMON_EFS_SUCCESS)
  {
    success_count++;

    switch(value)
    {
      case 0:
        *sap_sec_restr_supported_ptr = FALSE;
        break;
      case 1:
      default:
        /* Nothing to do */
        break;
    }
  }

  /* Retrieve simlock display value from NV file */
  status = uim_common_efs_read(
             UIM_COMMON_EFS_UIMQMI_QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD,
             UIM_COMMON_EFS_ITEM_FILE_TYPE,
             UIM_COMMON_EFS_DEVICE,
             &value,
             sizeof(uint8));

  if (status == UIM_COMMON_EFS_SUCCESS)
  {
    success_count++;
    switch(value)
    {
      case 0:
         break;
      case 1:
        *simlock_display_for_valid_card_ptr =
             QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ANY_SLOT_POLICY;
        break;
      case 2:
        *simlock_display_for_valid_card_ptr =
             QMI_UIM_SIMLOCK_DISPLAY_FOR_FULL_SERVICE_ON_ANY_SLOT_POLICY;
        break;
      case 3:
        *simlock_display_for_valid_card_ptr =
             QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ALL_SLOT_POLICY;
        break;
      default:
        /* Nothing to do */
        break;
    }
  }

  UIM_MSG_HIGH_6("Silent PIN1 verification configuration: 0x%x,"
                 "APDU security restrictions configuration: 0x%x,"
                 "Close channel sync preference: 0x%x,"
                 "AUTHENTICATE security restrictions configuration: 0x%x,"
                 "SAP security restrictions configuration: 0x%x"
                 "SIMLOCK display for valid card: 0x%x",
                 *silent_pin1_supported_ptr,
                 *apdu_sec_value_ptr,
                 *close_channel_ind_pref_ptr,
                 *auth_sec_restr_value_ptr,
                 *sap_sec_restr_supported_ptr,
                 *simlock_display_for_valid_card_ptr);

  /* If none of the configuration files were read, it's possible that the directory
     is completely missing... in this case, create it */
  if (success_count == 0)
  {
    uint32                          size       = 0;

    UIM_MSG_HIGH_0( "All configuration files missing");

    /* Create directory for the config file */
    if (uim_common_efs_get_dir_stat(
          UIM_COMMON_EFS_MODEM_CONFIG_DIR1,
          &size) != UIM_COMMON_EFS_SUCCESS)
    {
      (void)uim_common_efs_create_dir(UIM_COMMON_EFS_MODEM_CONFIG_DIR1, 0777);
      (void)uim_common_efs_create_dir(UIM_COMMON_EFS_UIMQMI_QMI_CONFIG_DIR2, 0777);
      (void)uim_common_efs_create_dir(UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_DIR4, 0777);
    }
    else if (uim_common_efs_get_dir_stat(
          UIM_COMMON_EFS_UIMQMI_QMI_CONFIG_DIR2,
          &size) != UIM_COMMON_EFS_SUCCESS)
    {
      (void)uim_common_efs_create_dir(UIM_COMMON_EFS_UIMQMI_QMI_CONFIG_DIR2, 0777);
      (void)uim_common_efs_create_dir(UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_DIR4, 0777);
    }
    else if (uim_common_efs_get_dir_stat(
          UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_DIR4,
          &size) != UIM_COMMON_EFS_SUCCESS)
    {
      (void)uim_common_efs_create_dir(UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_DIR4, 0777);
    }
  }
} /* qmi_uim_read_configuration */


/*===========================================================================
  FUNCTION QMI_UIMI_SIMLOCK_GET_STATUS_CALLBACK()

  DESCRIPTION
    This function is executed as callback of SIM get status request. This is
    executed in the context of SIM Lock task and directly update the slot
    policy in qmi_uim_globa_ptr.

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_simlock_get_status_callback
(
  simlock_result_enum_type                    status,
  const simlock_message_response_data_type  * resp_ptr,
  const void                                * user_data_ptr
)
{
  if (status != SIMLOCK_SUCCESS ||
      resp_ptr == NULL ||
      resp_ptr->msg_type != SIMLOCK_GET_STATUS_MSG ||
      qmi_uim_global_ptr == NULL)
  {
    return;
  }

  /* stores the simlock slot policy */
  qmi_uim_global_ptr->simlock_slot_policy = resp_ptr->message.get_status_resp.slot_policy;
}/* qmi_uimi_simlock_get_status_callback */


#ifdef FEATURE_UIM_SSM
/*===========================================================================
  FUNCTION QMI_UIM_SSM_INIT()

  DESCRIPTION
  To register with the Security Services Module.  SSM may access filesystem
  during registration, so ssm_client_init_func() may return
  'E_SSM_CLIENT_INIT_IN_PROCESS' immediately.  QMI UIM will be notified when
  initialization is complete via the qmi_uim_ssm_cback() callback, by returing
  'E_SSM_SUCCESS' or some error code.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_ssm_init
(
   void
)
{
  /* qmi_uim_ssm_id : uint32 :: Always returns a valid client id, whether
  initialization was succesful or not. It is recommended that clients
  check the return value of this init call before using client id.

  With fallback buffer == NULL, ssm client initialization returns
  error unless a valid ssm permission file for current ssm mode exists.
  QMI UIM blocks NV update if ssm client is not ready.

  For QMI UIM, SSM will look for ssm_uim_<mode_num>.ssm

  Following is an example of perm file to allow permission for QMI UIM
  restricted NVs:
  "65954|65954|0|67285|67285|0|67312|67312|0|67317|67317|0|70283|70284|0|71557|71557|0|"
  */
  ssm_err_t    ssm_err = E_SSM_CLIENT_NOT_INITIALIZED;

  ssm_err = ssm_client_init_func( "ssm_uim",
                                  QMI_UIM_SSM_KEY_NUM,
                                  QMI_UIM_SSM_CMD_DISALLOW,
                                  SSM_UINT32,
                                  qmi_uim_ssm_cback,
                                  &(qmi_uim_global_ptr->ssm_info.qmi_uim_ssm_id),
                                  NULL,
                                  0);

  if( (ssm_err != E_SSM_SUCCESS) &&
      (ssm_err != E_SSM_CLIENT_INIT_IN_PROCESS) )
  {
    UIM_MSG_ERR_1("SSM Initialization Failed 0x%x", ssm_err);
  }
} /* qmi_uim_ssm_init */
#endif /* FEATURE_UIM_SSM */


/*===========================================================================
  FUNCTION QMI_UIM_SERVICE_INITIALIZE()

  DESCRIPTION
    Initializes the QMI UIM module. This function is executed only once,
    at the very beginning.

  PARAMETERS
    None

  RETURN VALUE
    Boolean to indicate success or failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_service_initialize
(
  void
)
{
  qmi_uim_card_info_type     * temp_card_info_ptr = NULL;
  mmgsdi_cached_nv_item_data   cached_nv_item;
  uint16                       i                  = 0;
  mmgsdi_return_enum_type      mmgsdi_status      = MMGSDI_SUCCESS;
  static boolean               executed           = FALSE;

#ifdef FEATURE_GSTK
  gstk_status_enum_type        gstk_status        = GSTK_STATUS_MAX;
#endif /* FEATURE_GSTK */

  memset(&cached_nv_item, 0, sizeof(cached_nv_item));

  if (qmi_uim_global_ptr == NULL)
  {
  /* QMI UIM global variable expected to be allocated in the pre_init phase,
     so it should be present now. */
    return FALSE;
  }

  if (executed)
  {
    /* This indicates that function was already executed before, which is not allowed. */
    return FALSE;
  }

  executed = TRUE;

  /* Get count of available slots */
  (void)mmgsdi_get_hw_slots(&qmi_uim_global_ptr->card_state.num_slots);

  if(qmi_uim_global_ptr->card_state.num_slots == 0)
  {
    UIM_MSG_ERR_0("Number of available slots reported is zero");
    return FALSE;
  }

  /* Allocate memory for all slots combined and the divide it into chunks
     for each slot. */
  temp_card_info_ptr = (qmi_uim_card_info_type *)uimqmi_malloc(
                         qmi_uim_global_ptr->card_state.num_slots * sizeof(qmi_uim_card_info_type));
  if (temp_card_info_ptr == NULL)
  {
    return FALSE;
  }
  memset(temp_card_info_ptr,
         0x00,
         qmi_uim_global_ptr->card_state.num_slots * sizeof(qmi_uim_card_info_type));

  for (i = 0; i < qmi_uim_global_ptr->card_state.num_slots &&
              i < QMI_UIM_MAX_CARD_COUNT; i++)
  {
    qmi_uim_global_ptr->card_state.card[i] = temp_card_info_ptr + i;
  }

  /* Get a MMGSDI client id */
  mmgsdi_status = mmgsdi_client_id_and_evt_reg_ext(
                        qmi_uim_mmgsdi_evt_cback,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)NULL,
                        QMI_UIM_CLIENT_NAME,
                        sizeof(QMI_UIM_CLIENT_NAME));
  if (MMGSDI_SUCCESS != mmgsdi_status)
  {
    UIM_MSG_ERR_0("QMI-UIM:Unable to get a client id");
    return FALSE;
  }

#ifdef FEATURE_GSTK
  /* Register with GSTK ot get OTASP status updates */
  gstk_status = gstk_otasp_status_reg(0,
                                      qmi_uimi_otasp_reg_callback,
                                      qmi_uimi_otasp_status_callback);
  if(gstk_status != GSTK_SUCCESS)
  {
    UIM_MSG_ERR_1("QMI-UIM:Unable to register with GSTK for OTASP: 0x%x", gstk_status);
  }
#endif /* FEATURE_GSTK */

  /* Read configuration for silent PIN1 verification & APDU security restrictions*/
  qmi_uim_read_configuration(&qmi_uim_global_ptr->silent_pin1_supported,
                             &qmi_uim_global_ptr->apdu_sec_restr_value,
                             &qmi_uim_global_ptr->apdu_sec_aid_info,
                             &qmi_uim_global_ptr->close_channel_sync,
                             &qmi_uim_global_ptr->auth_sec_restr_value,
                             &qmi_uim_global_ptr->sap_sec_restr_supported,
                             &qmi_uim_global_ptr->simlock_display_for_valid_card);

  /* Retrieve the value fo NV item: 04398 to determine auto/manual selection of apps
     If the read of NV item from cache fails then we default to auto provisioning */
  cached_nv_item.nv_item_enum = MMGSDI_NV_UIM_SELECT_DEFAULT_USIM_APP_I;
  mmgsdi_status = mmgsdi_get_cached_nv_item(&cached_nv_item);
  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    qmi_uim_global_ptr->automatic_provisioning =
      cached_nv_item.nv_item_data.uim_select_default_usim_app;
  }
  else
  {
    qmi_uim_global_ptr->automatic_provisioning = TRUE;
  }

#ifdef FEATURE_UIM_SSM
  /* Initialize SSM globals */
  qmi_uim_global_ptr->ssm_info.qmi_uim_ssm_init_err = E_SSM_CLIENT_NOT_INITIALIZED;
  /* Initialize SSM module for restricted NV checks */
  qmi_uim_ssm_init();
#endif /* FEATURE_UIM_SSM */

  /* Initialize SIMLOCK globals */
  qmi_uim_global_ptr->simlock_remote_sfs_operation_resp_cb = NULL;
  qmi_uim_global_ptr->simlock_slot_policy = SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS;
  memset(&qmi_uim_global_ptr->temp_unlock_info,
         0x00,
         sizeof(qmi_uim_global_ptr->temp_unlock_info));

  if (qmi_uim_global_ptr->simlock_display_for_valid_card ==
          QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ANY_SLOT_POLICY ||
      qmi_uim_global_ptr->simlock_display_for_valid_card ==
          QMI_UIM_SIMLOCK_DISPLAY_FOR_FULL_SERVICE_ON_ANY_SLOT_POLICY ||
      qmi_uim_global_ptr->simlock_display_for_valid_card ==
          QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ALL_SLOT_POLICY)
  {
    (void)simlock_get_status(qmi_uimi_simlock_get_status_callback,
                             NULL);
  }

  /* Try to setup Diag access check callback */
  qmi_uim_global_ptr->efs_directory_list[0] = UIM_COMMON_EFS_PATH_MODEM_QMI_UIM_CONFIG_DIR4;
  qmi_uim_global_ptr->efs_directory_list[1] = UIM_COMMON_EFS_PATH_MODEM_UIM_MMGSDI_CONFIG_DIR;
  qmi_uim_global_ptr->efs_directory_list[2] = UIM_COMMON_EFS_PATH_MODEM_QMI_CAT_CONFIG_DIR3;
  qmi_uim_global_ptr->efs_directory_list[3] = UIM_COMMON_EFS_PATH_MODEM_UIM_REMOTE_CONFIG_DIR;
  diag_fs_register_access_check((const char **)qmi_uim_global_ptr->efs_directory_list,
                                QMI_UIM_EFS_DIRECTORY_LIST_SIZE,
                                qmi_uim_diag_access_check_cb);

  qmi_uim_global_ptr->physical_slots_info.num_slots = 0;
#ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER
  /* Register a callback with drivers for receiving status change of
     physical slots. If it fails, do not continue with any further
     communication with drivers for subscription manager */
  if(UIM_PASS == uim_sm_card_slot_status_evt_reg_sync(qmi_uim_sub_mgr_notify_slots_status_cb))
  {
    /* Get number of physical slots from drivers */
    qmi_uim_global_ptr->physical_slots_info.num_slots = uim_sm_get_num_physical_slots_sync();
    /* Allocate the physical slot status data on the heap */
    if(qmi_uim_global_ptr->physical_slots_info.num_slots > 0)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr =
        (uim_phy_slot_status_type*)modem_mem_alloc(
                                    qmi_uim_global_ptr->physical_slots_info.num_slots * sizeof(uim_phy_slot_status_type),
                                    MODEM_MEM_CLIENT_UIM);

      if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr != NULL)
      {
        memset(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
               0,
               qmi_uim_global_ptr->physical_slots_info.num_slots * sizeof(uim_phy_slot_status_type));
      }

      /* Get physical slots status from uimdrv into our global */
      (void) uim_sm_get_logical_slot_status_sync(&qmi_uim_global_ptr->physical_slots_info.num_slots,
                                                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr);
    }
  }
  else
  {
    /* Do not return FALSE here since this is not severe enough to flag that
       the overall QMI UIM init has failed.. only subscription manager
       functionality won't be available */
    UIM_MSG_MED_0("Error registering with drivers: subscription manager unavailable!");
  }
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */

  return TRUE;
} /* qmi_uim_service_initialize */


/*===========================================================================
QMI_UIM_SIG_HANDLER

DESCRIPTION
  QMI UIM signal handler for MMGSDI task signals

PARAMETERS
  None.

  Returns TRUE if signal should be cleared from the set signal mask,
  FALSE if further signal processing is needed and hence signal
  should not be cleared.
===========================================================================*/
boolean qmi_uim_sig_handler
(
  void
)
{
  ASSERT(qmi_uim_global_ptr != NULL);

  qmi_uim_process_message();

  /* This signal is set asyncronously however the framework handles
     concurrency issues so no need to worry about it here */
  if (q_cnt(&qmi_uim_global_ptr->qmi_uim_q) == 0)
  {
    return TRUE;
  }

  return FALSE;
} /* qmi_uim_sig_handler */


/*===========================================================================
  FUNCTION QMI_UIMI_ALLOC_CLID

  DESCRIPTION
    Allocate client command from the framework.  Post a message
    to handle the allocation in MMGSDI context.

  PARAMETERS
    svc_common_hdr : framework service header

  RETURN VALUE
    True : Message posted successfully
    False: Allocation error

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uimi_alloc_clid
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(svc_common_hdr);

  UIM_MSG_HIGH_1("Incoming Client allocation for instance:0x%x",
                 svc_common_hdr->qmi_instance);

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_ALLOC_CLID);

  if (msg_ptr == NULL)
  {
    return FALSE;
  }

  msg_ptr->data.clid_cb.cmd_hdr = *svc_common_hdr;

  /* Send command to be processed in MMGSDI context */
  if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
     (present_in_queue))
  {
    uimqmi_free(msg_ptr);
    return FALSE;
  }

  msg_ptr = NULL;
  return TRUE;
} /* qmi_uimi_alloc_clid */


/*===========================================================================
  FUNCTION QMI_UIMI_DEALLOC_CLID

  DESCRIPTION
    Deallocate client command from the framework.  Post a message
    to handle the operation in MMGSDI context.

  PARAMETERS
    svc_common_hdr : framework service header

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_dealloc_clid
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(svc_common_hdr);

  UIM_MSG_HIGH_1("Incoming Client deallocation for instance:0x%x",
                 svc_common_hdr->qmi_instance);

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_DEALLOC_CLID);

  if (msg_ptr == NULL)
  {
    return;
  }

  msg_ptr->data.clid_cb.cmd_hdr = *svc_common_hdr;

  /* Send command to be processed in MMGSDI context */
  if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
     (present_in_queue))
  {
    uimqmi_free(msg_ptr);
  }

  msg_ptr = NULL;
} /* qmi_uimi_dealloc_clid */


/*===========================================================================
  FUNCTION QMI_UIMI_INIT_CBACK

  DESCRIPTION
    Callback for the qmi framework registration.

  PARAMETERS
    num_instances : The number of qmi_uim instances

  RETURN VALUE
    None

  DEPENDENCIES
    Must have registered with the qmi framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_init_cback
(
  uint16 num_instances
)
{
  /* Nothing to do here. Callback required by qmi_framework_reg_service() */
  UIM_MSG_LOW_1("qmi_uimi_init_cback instances:0x%x", num_instances);
} /* qmi_uimi_init_cback */


/*===========================================================================
  FUNCTION QMI_UIMI_CMD_HDLR

  DESCRIPTION
    Function to route external commands sent to QMI_UIM service
    to the appropriate internal service handling routine.

  PARAMETERS
    msg_hdr  : CMD data including hdr packaged by qmi framework
    sdu      : Incoming request
===========================================================================*/
static void qmi_uimi_cmd_hdlr
(
  qmi_framework_msg_hdr_type * msg_hdr_ptr,
  dsm_item_type             ** sdu
)
{
  qmi_uim_message_type       *qmi_uim_message_ptr = NULL;
  boolean                     present_in_queue    = FALSE;

  ASSERT(msg_hdr_ptr && sdu);

  qmi_uim_message_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_FRAMEWORK_CMD);

  if(qmi_uim_message_ptr == NULL)
  {
    dsm_free_packet(sdu);
  }
  else
  {
    qmi_uim_message_ptr->data.framework_cmd.cmd.cmd_hdr = *msg_hdr_ptr;
    qmi_uim_message_ptr->data.framework_cmd.cmd.sdu     = *sdu;

    if((!qmi_uim_util_post_message(
          qmi_uim_message_ptr,
          &present_in_queue)) &&
       (present_in_queue))
    {
      uimqmi_free(qmi_uim_message_ptr);
      dsm_free_packet(sdu);
    }
    qmi_uim_message_ptr = NULL;
  }
} /* qmi_uimi_cmd_hdlr */


/*===========================================================================
  FUNCTION QMI_UIM_INIT_PRE_STARTUP()

  DESCRIPTION
    Initializes the QMI UIM global variables and register with MMGSDI

  PARAMETERS
    void

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_init_pre_startup
(
  void
)
{
  uint16  i = 0;

  if(qmi_uim_state_ptr == NULL)
  {
    /*Allocate state variable for QMI_UIM clients*/
    qmi_uim_state_ptr = uimqmi_malloc(sizeof(qmi_uimi_state_type));
    ASSERT(qmi_uim_state_ptr != NULL);
    memset(qmi_uim_state_ptr, 0, sizeof(qmi_uimi_state_type));
  }

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Initialize qmi_uim_global_type if not initialized yet */
  if (FALSE == qmi_uim_global_ptr->init)
  {
    memset(qmi_uim_global_ptr, 0, sizeof(qmi_uim_global_type));

    /* Initialization of client id and session ids */
    qmi_uim_global_ptr->mmgsdi_client_id                 = UIM_INVALID_CLIENT_ID;
    for (i = 0; i < QMI_UIM_MAX_CARD_SESSIONS; i++)
    {
      qmi_uim_global_ptr->mmgsdi_card_slot_session_id[i] = UIM_INVALID_SESSION_ID;
    }

    for (i = 0; i < UIM_REFRESH_NON_PROV_COUNT; i++)
    {
      qmi_uim_global_ptr->non_prov_refresh_session_id[i]        = UIM_INVALID_SESSION_ID;
    }

    /* Initialization of indexes to provisioning app */
    for (i = 0; i < QMI_UIM_MAX_PROV_SESSIONS; i++)
    {
      qmi_uim_global_ptr->mmgsdi_gw_session_id[i] = UIM_INVALID_SESSION_ID;
      qmi_uim_global_ptr->mmgsdi_1x_session_id[i] = UIM_INVALID_SESSION_ID;
      qmi_uim_global_ptr->card_state.index_gw[i]  = UIM_INVALID_APP_INDEX;
      qmi_uim_global_ptr->card_state.index_1x[i]  = UIM_INVALID_APP_INDEX;
    }

    /* Initialize SAP info */
    for (i = 0; i < QMI_UIM_MAX_CARD_COUNT; i++)
    {
      qmi_uim_global_ptr->sap_info[i].is_enabled = FALSE;
      qmi_uim_global_ptr->sap_info[i].sap_state  = MMGSDI_SAP_DISCONNECTED;
    }

    /* Initialize message queue */
    (void) q_init(&qmi_uim_global_ptr->qmi_uim_q);

    qmi_uim_global_ptr->init = TRUE;
  }
} /* qmi_uim_init_pre_startup() */


/*===========================================================================
  FUNCTION QMI_UIM_INIT_POST_STARTUP()

  DESCRIPTION
    Registers the QMI UIM service to the QMI framework to initalize
    the service.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_init_post_startup
(
  void
)
{
  qmi_framework_err_e_type      errval       = QMI_FRAMEWORK_ERR_NONE;
  qmi_framework_svc_config_type qmi_uim_cfg;

  UIM_MSG_HIGH_0("qmi_uim_init_post_startup");

  /* Initialize global data
     We must do this now before we register for service because
     at the time of the reg_service_cb we need to have the
     internal qmi_message q initialized */
  if(qmi_uim_service_initialize() == FALSE)
  {
    return;
  }

  memset(&qmi_uim_cfg, 0, sizeof(qmi_uim_cfg));

  qmi_uim_cfg.base_version.major     = UIMI_BASE_VER_MAJOR;
  qmi_uim_cfg.base_version.minor     = UIMI_BASE_VER_MINOR;

  qmi_uim_cfg.addendum_version.major = UIMI_ADDENDUM_VER_MAJOR;
  qmi_uim_cfg.addendum_version.minor = UIMI_ADDENDUM_VER_MINOR;

  qmi_uim_cfg.cbs.alloc_clid         = qmi_uimi_alloc_clid;
  qmi_uim_cfg.cbs.dealloc_clid       = qmi_uimi_dealloc_clid;
  qmi_uim_cfg.cbs.init_cback         = qmi_uimi_init_cback;
  qmi_uim_cfg.cbs.cmd_hdlr           = qmi_uimi_cmd_hdlr;

  errval = qmi_framework_reg_service(QMUX_SERVICE_UIM, &qmi_uim_cfg);
  if (errval != QMI_FRAMEWORK_ERR_NONE)
  {
    UIM_MSG_ERR_1("Could not register QMI UIM with QMI Framework - errval:0x%x",
                  errval);
    return;
  }

  /* register for MPSS/QMI support interfaces logging */
  (void) qmi_si_register_id ( QMUX_SERVICE_UIM,
                              0, /* Service Instance */
                              UIMI_BASE_VER_MAJOR,
                              UIMI_BASE_VER_MINOR,
                              uim_get_service_impl_v01());
} /* qmi_uim_init_post_startup() */


/*===========================================================================
  FUNCTION QMI_UIM_GET_MESSAGE

  DESCRIPTION
    Retrieve a message from the QMI UIM queue

  PARAMETERS
    None

  RETURN VALUE
    Pointer to the message extracted from the queue

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_uim_message_type * qmi_uim_get_message
(
  void
)
{
  qmi_uim_message_type * uim_message_ptr = NULL;

  ASSERT(qmi_uim_global_ptr != NULL);

  if (q_cnt(&qmi_uim_global_ptr->qmi_uim_q) > 0)
  {
    uim_message_ptr = (qmi_uim_message_type*)q_get(&qmi_uim_global_ptr->qmi_uim_q);
  }
  return uim_message_ptr;
} /* qmi_uim_get_message */


/*===========================================================================
  FUNCTION QMI_UIM_CNF_DEEP_COPY()

  DESCRIPTION
    This function makes a deep copy of the mmgsdi_cnf_type pointer
    passed from MMGSDI to QMI and is required to pass the command in the
    QMI context.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_cnf_deep_copy
(
  mmgsdi_cnf_enum_type    cnf_type,
  mmgsdi_cnf_type        *cnf_dst_ptr,
  const mmgsdi_cnf_type  *cnf_src_ptr
)
{
  const mmgsdi_file_security_access_type* sec_src = NULL;
  mmgsdi_file_security_access_type* sec_dst = NULL;

  if (cnf_dst_ptr == NULL ||
      cnf_src_ptr == NULL)
  {
    return;
  }

  memset(cnf_dst_ptr, 0, sizeof(mmgsdi_cnf_type));

  switch(cnf_type)
  {
    case MMGSDI_CLIENT_ID_AND_EVT_REG_CNF:
      (void)memscpy(&cnf_dst_ptr->client_id_and_evt_reg_cnf,
                    sizeof(mmgsdi_client_id_and_evt_reg_cnf_type),
                    &cnf_src_ptr->client_id_and_evt_reg_cnf,
                    sizeof(mmgsdi_client_id_and_evt_reg_cnf_type));
      break;

    case MMGSDI_SESSION_OPEN_EXT_CNF:
      (void)memscpy(&cnf_dst_ptr->session_open_ext_cnf,
                    sizeof(mmgsdi_session_open_ext_cnf_type),
                    &cnf_src_ptr->session_open_ext_cnf,
                    sizeof(mmgsdi_session_open_ext_cnf_type));
      /* Deep copy of cnf data only in case of success */
      if (cnf_src_ptr->session_open_ext_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
         (void**)&cnf_dst_ptr->session_open_ext_cnf.session_info_ptr,
         (void*)cnf_src_ptr->session_open_ext_cnf.session_info_ptr,
         cnf_src_ptr->session_open_ext_cnf.num_sessions * sizeof(mmgsdi_session_open_info_type));
      }
      else
      {
        cnf_dst_ptr->session_open_ext_cnf.session_info_ptr = NULL;
      }
      break;

    case MMGSDI_SESSION_OPEN_WITH_SELECT_RSP_CNF:
      (void)memscpy(&cnf_dst_ptr->session_open_with_sel_rsp_cnf,
                    sizeof(mmgsdi_session_open_with_sel_rsp_cnf_type),
                    &cnf_src_ptr->session_open_with_sel_rsp_cnf,
                    sizeof(mmgsdi_session_open_with_sel_rsp_cnf_type));
      if (cnf_src_ptr->session_open_with_sel_rsp_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->session_open_with_sel_rsp_cnf.select_rsp.data_ptr,
          (void*)cnf_src_ptr->session_open_with_sel_rsp_cnf.select_rsp.data_ptr,
          (uint32)cnf_src_ptr->session_open_with_sel_rsp_cnf.select_rsp.data_len);
      }
      else
      {
        cnf_dst_ptr->session_open_with_sel_rsp_cnf.select_rsp.data_ptr = NULL;
      }
      break;

    case MMGSDI_SESSION_OPEN_WITH_MF_CNF:
      (void)memscpy(&cnf_dst_ptr->session_open_with_mf_cnf,
                    sizeof(mmgsdi_session_open_with_mf_cnf_type),
                    &cnf_src_ptr->session_open_with_mf_cnf,
                    sizeof(mmgsdi_session_open_with_mf_cnf_type));
      break;

    case MMGSDI_SESSION_CLOSE_CNF:
      (void)memscpy(&cnf_dst_ptr->session_close_cnf,
                    sizeof(mmgsdi_session_close_cnf_type),
                    &cnf_src_ptr->session_close_cnf,
                    sizeof(mmgsdi_session_close_cnf_type));
      break;

    case MMGSDI_READ_CNF:
      (void)memscpy(&cnf_dst_ptr->read_cnf,
                    sizeof(mmgsdi_read_cnf_type),
                    &cnf_src_ptr->read_cnf,
                    sizeof(mmgsdi_read_cnf_type));
      /* Deep copy of read data only in case of success */
      if (cnf_src_ptr->read_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->read_cnf.read_data.data_ptr,
          (void*)cnf_src_ptr->read_cnf.read_data.data_ptr,
          (uint32)cnf_src_ptr->read_cnf.read_data.data_len);
      }
      else
      {
        cnf_dst_ptr->read_cnf.read_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_WRITE_CNF:
      (void)memscpy(&cnf_dst_ptr->write_cnf,
                    sizeof(mmgsdi_write_cnf_type),
                    &cnf_src_ptr->write_cnf,
                    sizeof(mmgsdi_write_cnf_type));
      break;

    case MMGSDI_GET_FILE_ATTR_CNF:
      (void)memscpy(&cnf_dst_ptr->get_file_attr_cnf,
                    sizeof(mmgsdi_get_file_attr_cnf_type),
                    &cnf_src_ptr->get_file_attr_cnf,
                    sizeof(mmgsdi_get_file_attr_cnf_type));
      /* Deep copy only in case of success */
      if (cnf_src_ptr->get_file_attr_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        /* Deep copy of raw data */
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr,
          (void*)cnf_src_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr,
          (uint32)cnf_src_ptr->get_file_attr_cnf.raw_file_attr_data.data_len);
        /* Deep copy of security attributes */
        switch (cnf_src_ptr->get_file_attr_cnf.file_attrib.file_type)
        {
          case MMGSDI_TRANSPARENT_FILE:
            sec_src = &cnf_src_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security;
            sec_dst = &cnf_dst_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security;
            break;
          case MMGSDI_LINEAR_FIXED_FILE:
            sec_src = &cnf_src_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security;
            sec_dst = &cnf_dst_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security;
            break;
          case MMGSDI_CYCLIC_FILE:
            sec_src = &cnf_src_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security;
            sec_dst = &cnf_dst_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security;
            break;
          default:
            break;
        }
        if (sec_src != NULL && sec_dst != NULL)
        {
          uimqmi_deep_copy(
             (void**)&sec_dst->read.protection_pin_ptr,
             (void*)sec_src->read.protection_pin_ptr,
             (uint32)(sec_src->read.num_protection_pin) * sizeof(mmgsdi_pin_enum_type) );
          uimqmi_deep_copy(
             (void**)&sec_dst->write.protection_pin_ptr,
             (void*)sec_src->write.protection_pin_ptr,
             (uint32)(sec_src->write.num_protection_pin) * sizeof(mmgsdi_pin_enum_type) );
          uimqmi_deep_copy(
             (void**)&sec_dst->increase.protection_pin_ptr,
             (void*)sec_src->increase.protection_pin_ptr,
             (uint32)(sec_src->increase.num_protection_pin) * sizeof(mmgsdi_pin_enum_type) );
          uimqmi_deep_copy(
             (void**)&sec_dst->invalidate_deactivate.protection_pin_ptr,
             (void*)sec_src->invalidate_deactivate.protection_pin_ptr,
             (uint32)(sec_src->invalidate_deactivate.num_protection_pin) * sizeof(mmgsdi_pin_enum_type) );
          uimqmi_deep_copy(
             (void**)&sec_dst->rehabilitate_activate.protection_pin_ptr,
             (void*)sec_src->rehabilitate_activate.protection_pin_ptr,
             (uint32)(sec_src->rehabilitate_activate.num_protection_pin) * sizeof(mmgsdi_pin_enum_type) );
        }
      }
      else
      {
        cnf_dst_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_CARD_PDOWN_CNF:
      (void)memscpy(&cnf_dst_ptr->card_pdown_cnf,
                    sizeof(mmgsdi_card_pdown_cnf_type),
                    &cnf_src_ptr->card_pdown_cnf,
                    sizeof(mmgsdi_card_pdown_cnf_type));
      break;

    case MMGSDI_CARD_PUP_CNF:
      (void)memscpy(&cnf_dst_ptr->card_pup_cnf,
                    sizeof(mmgsdi_card_pup_cnf_type),
                    &cnf_src_ptr->card_pup_cnf,
                    sizeof(mmgsdi_card_pup_cnf_type));
      break;

    case MMGSDI_REFRESH_CNF:
      (void)memscpy(&cnf_dst_ptr->refresh_cnf,
                    sizeof(mmgsdi_refresh_cnf_type),
             &cnf_src_ptr->refresh_cnf,
             sizeof(mmgsdi_refresh_cnf_type));
      break;

    case MMGSDI_PIN_OPERATION_CNF:
      (void)memscpy(&cnf_dst_ptr->pin_operation_cnf,
                    sizeof(mmgsdi_pin_operation_cnf_type),
                    &cnf_src_ptr->pin_operation_cnf,
                    sizeof(mmgsdi_pin_operation_cnf_type));
      break;

    case MMGSDI_RUN_CAVE_CNF:
      (void)memscpy(&cnf_dst_ptr->run_cave_cnf,
                    sizeof(mmgsdi_run_cave_cnf_type),
                    &cnf_src_ptr->run_cave_cnf,
                    sizeof(mmgsdi_run_cave_cnf_type));
      break;

    case MMGSDI_SESSION_RUN_GSM_ALGO_CNF:
      (void)memscpy(&cnf_dst_ptr->session_run_gsm_algo_cnf,
                    sizeof(mmgsdi_session_run_gsm_algo_cnf_type),
                    &cnf_src_ptr->session_run_gsm_algo_cnf,
                    sizeof(mmgsdi_session_run_gsm_algo_cnf_type));
      break;

    case MMGSDI_ISIM_AUTH_CNF:
      (void)memscpy(&cnf_dst_ptr->isim_auth_cnf,
                    sizeof(mmgsdi_isim_auth_cnf_type),
                    &cnf_src_ptr->isim_auth_cnf,
                    sizeof(mmgsdi_isim_auth_cnf_type));
      uimqmi_deep_copy(
        (void**)&cnf_dst_ptr->isim_auth_cnf.res.data_ptr,
        (void*)cnf_src_ptr->isim_auth_cnf.res.data_ptr,
        (uint32)cnf_src_ptr->isim_auth_cnf.res.data_len);
      uimqmi_deep_copy(
        (void**)&cnf_dst_ptr->isim_auth_cnf.ck.data_ptr,
        (void*)cnf_src_ptr->isim_auth_cnf.ck.data_ptr,
        (uint32)cnf_src_ptr->isim_auth_cnf.ck.data_len);
      uimqmi_deep_copy(
        (void**)&cnf_dst_ptr->isim_auth_cnf.ik.data_ptr,
        (void*)cnf_src_ptr->isim_auth_cnf.ik.data_ptr,
        (uint32)cnf_src_ptr->isim_auth_cnf.ik.data_len);
      uimqmi_deep_copy(
        (void**)&cnf_dst_ptr->isim_auth_cnf.auts.data_ptr,
        (void*)cnf_src_ptr->isim_auth_cnf.auts.data_ptr,
        (uint32)cnf_src_ptr->isim_auth_cnf.auts.data_len);
      break;

    case MMGSDI_USIM_AUTH_CNF:
      (void)memscpy(&cnf_dst_ptr->usim_auth_cnf,
                    sizeof(mmgsdi_usim_auth_cnf_type),
                    &cnf_src_ptr->usim_auth_cnf,
                    sizeof(mmgsdi_usim_auth_cnf_type));
      uimqmi_deep_copy(
        (void**)&cnf_dst_ptr->usim_auth_cnf.auth_response.data_ptr,
        (void*)cnf_src_ptr->usim_auth_cnf.auth_response.data_ptr,
        (uint32)cnf_src_ptr->usim_auth_cnf.auth_response.data_len);
      break;

    case MMGSDI_COMPUTE_IP_AUTH_CNF:
      (void)memscpy(&cnf_dst_ptr->compute_ip_cnf,
                    sizeof(mmgsdi_compute_ip_auth_cnf_type),
                    &cnf_src_ptr->compute_ip_cnf,
                    sizeof(mmgsdi_compute_ip_auth_cnf_type));
      uimqmi_deep_copy(
        (void**)&cnf_dst_ptr->compute_ip_cnf.cmpt_ip_response_data.data_ptr,
        (void*)cnf_src_ptr->compute_ip_cnf.cmpt_ip_response_data.data_ptr,
        (uint32)cnf_src_ptr->compute_ip_cnf.cmpt_ip_response_data.data_len);
      break;

    case MMGSDI_SESSION_ENABLE_SERVICE_CNF:
      (void)memscpy(&cnf_dst_ptr->session_enable_service_cnf,
                    sizeof(mmgsdi_session_enable_service_cnf_type),
                    &cnf_src_ptr->session_enable_service_cnf,
                    sizeof(mmgsdi_session_enable_service_cnf_type));
      break;

    case MMGSDI_SESSION_DISABLE_SERVICE_CNF:
      (void)memscpy(&cnf_dst_ptr->session_disable_service_cnf,
                    sizeof(mmgsdi_session_disable_service_cnf_type),
                    &cnf_src_ptr->session_disable_service_cnf,
                    sizeof(mmgsdi_session_disable_service_cnf_type));
      break;

    case MMGSDI_SRV_AVAILABLE_CNF:
      (void)memscpy(&cnf_dst_ptr->srv_available_cnf,
                    sizeof(mmgsdi_srv_available_cnf_type),
                    &cnf_src_ptr->srv_available_cnf,
                    sizeof(mmgsdi_srv_available_cnf_type));
      break;

    case MMGSDI_SESSION_GET_APP_CAPABILITIES_CNF:
      (void)memscpy(&cnf_dst_ptr->session_app_capabilities_cnf,
                    sizeof(mmgsdi_session_app_capabilities_cnf_type),
                    &cnf_src_ptr->session_app_capabilities_cnf,
                    sizeof(mmgsdi_session_app_capabilities_cnf_type));
      break;

    case MMGSDI_GET_ALL_PIN_STATUS_CNF:
      (void)memscpy(&cnf_dst_ptr->get_all_pin_status_cnf,
                    sizeof(mmgsdi_get_all_pin_status_cnf_type),
                    &cnf_src_ptr->get_all_pin_status_cnf,
                    sizeof(mmgsdi_get_all_pin_status_cnf_type));
      break;

    case MMGSDI_SESSION_ACTIVATE_OR_SWITCH_PROVISIONING_CNF:
      (void)memscpy(&cnf_dst_ptr->session_activate_or_switch_provisioning_cnf,
                    sizeof(mmgsdi_session_act_or_switch_prov_cnf_type),
                    &cnf_src_ptr->session_activate_or_switch_provisioning_cnf,
                    sizeof(mmgsdi_session_act_or_switch_prov_cnf_type));
      break;

    case MMGSDI_SESSION_DEACTIVATE_CNF:
      (void)memscpy(&cnf_dst_ptr->session_deactivate_cnf,
                    sizeof(mmgsdi_session_deactivate_cnf_type),
                    &cnf_src_ptr->session_deactivate_cnf,
                    sizeof(mmgsdi_session_deactivate_cnf_type));
      break;

    case MMGSDI_SEND_APDU_EXT_CNF:
      (void)memscpy(&cnf_dst_ptr->send_apdu_ext_cnf,
                    sizeof(mmgsdi_send_apdu_ext_cnf_type),
                    &cnf_src_ptr->send_apdu_ext_cnf,
                    sizeof(mmgsdi_send_apdu_ext_cnf_type));
      /* Deep copy of APDU response data only in case of success */
      if (cnf_src_ptr->send_apdu_ext_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->send_apdu_ext_cnf.apdu_data.data_ptr,
          (void*)cnf_src_ptr->send_apdu_ext_cnf.apdu_data.data_ptr,
          (uint32)cnf_src_ptr->send_apdu_ext_cnf.apdu_data.data_len);
      }
      else
      {
        cnf_dst_ptr->send_apdu_ext_cnf.apdu_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_SAP_CONNECT_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_connect_cnf,
                    sizeof(mmgsdi_sap_connect_cnf_type),
                    &cnf_src_ptr->sap_connect_cnf,
                    sizeof(mmgsdi_sap_connect_cnf_type));
      break;

    case MMGSDI_SAP_DISCONNECT_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_disconnect_cnf,
                    sizeof(mmgsdi_sap_disconnect_cnf_type),
                    &cnf_src_ptr->sap_disconnect_cnf,
                    sizeof(mmgsdi_sap_disconnect_cnf_type));
      break;

    case MMGSDI_SAP_GET_ATR_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_get_atr_cnf,
                    sizeof(mmgsdi_sap_get_atr_cnf_type),
                    &cnf_src_ptr->sap_get_atr_cnf,
                    sizeof(mmgsdi_sap_get_atr_cnf_type));
      /* Deep copy of ATR response data only in case of success */
      if (cnf_src_ptr->sap_get_atr_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->sap_get_atr_cnf.atr_data.data_ptr,
          (void*)cnf_src_ptr->sap_get_atr_cnf.atr_data.data_ptr,
          (uint32)cnf_src_ptr->sap_get_atr_cnf.atr_data.data_len);
      }
      else
      {
        cnf_dst_ptr->sap_get_atr_cnf.atr_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_SAP_SEND_APDU_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_send_apdu_cnf,
                    sizeof(mmgsdi_sap_send_apdu_cnf_type),
                    &cnf_src_ptr->sap_send_apdu_cnf,
                    sizeof(mmgsdi_sap_send_apdu_cnf_type));
      /* Deep copy of APDU response data only in case of success */
      if (cnf_src_ptr->sap_send_apdu_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->sap_send_apdu_cnf.apdu_data.data_ptr,
          (void*)cnf_src_ptr->sap_send_apdu_cnf.apdu_data.data_ptr,
          (uint32)cnf_src_ptr->sap_send_apdu_cnf.apdu_data.data_len);
      }
      else
      {
        cnf_dst_ptr->sap_send_apdu_cnf.apdu_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_SAP_POWER_ON_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_power_on_cnf,
                    sizeof(mmgsdi_sap_power_on_cnf_type),
                    &cnf_src_ptr->sap_power_on_cnf,
                    sizeof(mmgsdi_sap_power_on_cnf_type));
      break;

    case MMGSDI_SAP_POWER_OFF_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_power_off_cnf,
                    sizeof(mmgsdi_sap_power_off_cnf_type),
                    &cnf_src_ptr->sap_power_off_cnf,
                    sizeof(mmgsdi_sap_power_off_cnf_type));
      break;

    case MMGSDI_SAP_RESET_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_reset_cnf,
                    sizeof(mmgsdi_sap_reset_cnf_type),
                    &cnf_src_ptr->sap_reset_cnf,
                    sizeof(mmgsdi_sap_reset_cnf_type));
      break;

    case MMGSDI_SAP_CARD_READER_STATUS_CNF:
      (void)memscpy(&cnf_dst_ptr->sap_card_reader_status_cnf,
                    sizeof(mmgsdi_sap_card_reader_status_cnf_type),
                    &cnf_src_ptr->sap_card_reader_status_cnf,
                    sizeof(mmgsdi_sap_card_reader_status_cnf_type));
      /* Deep copy of Card reader status data only in case of success */
      if (cnf_src_ptr->sap_card_reader_status_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_ptr,
          (void*)cnf_src_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_ptr,
          (uint32)cnf_src_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_len);
      }
      else
      {
        cnf_dst_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_SESSION_SUBSCRIPTION_OK_CNF:
      (void)memscpy(&cnf_dst_ptr->session_subscription_ok_cnf,
                    sizeof(mmgsdi_session_subscription_ok_cnf_type),
                    &cnf_src_ptr->session_subscription_ok_cnf,
                    sizeof(mmgsdi_session_subscription_ok_cnf_type));
      break;

    case MMGSDI_GET_ATR_CNF:
      (void)memscpy(&cnf_dst_ptr->get_atr_cnf,
                    sizeof(mmgsdi_get_atr_cnf_type),
                    &cnf_src_ptr->get_atr_cnf,
                    sizeof(mmgsdi_get_atr_cnf_type));
      /* Deep copy of ATR response data only in case of success */
      if (cnf_src_ptr->get_atr_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->get_atr_cnf.atr_data.data_ptr,
          (void*)cnf_src_ptr->get_atr_cnf.atr_data.data_ptr,
          (uint32)cnf_src_ptr->get_atr_cnf.atr_data.data_len);
      }
      else
      {
        cnf_dst_ptr->get_atr_cnf.atr_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_INVALIDATE_CNF:
      (void)memscpy(&cnf_dst_ptr->invalidate_cnf,
                    sizeof(mmgsdi_invalidate_cnf_type),
                    &cnf_src_ptr->invalidate_cnf,
                    sizeof(mmgsdi_invalidate_cnf_type));
      break;

    case MMGSDI_REHABILITATE_CNF:
      (void)memscpy(&cnf_dst_ptr->rehab_cnf,
                    sizeof(mmgsdi_rehabilitate_cnf_type),
                    &cnf_src_ptr->rehab_cnf,
                    sizeof(mmgsdi_rehabilitate_cnf_type));
      break;

    case MMGSDI_INCREASE_CNF:
      (void)memscpy(&cnf_dst_ptr->increase_cnf,
                    sizeof(mmgsdi_increase_cnf_type),
                    &cnf_src_ptr->increase_cnf,
                    sizeof(mmgsdi_increase_cnf_type));
      /* Deep copy of increase data only in case of success */
      if (cnf_src_ptr->increase_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->increase_cnf.increased_data.data_ptr,
          (void*)cnf_src_ptr->increase_cnf.increased_data.data_ptr,
          (uint32)cnf_src_ptr->increase_cnf.increased_data.data_len);
      }
      else
      {
        cnf_dst_ptr->increase_cnf.increased_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_SESSION_SELECT_AID_CNF:
      (void)memscpy(&cnf_dst_ptr->session_select_aid_cnf,
                    sizeof(mmgsdi_session_select_aid_cnf_type),
                    &cnf_src_ptr->session_select_aid_cnf,
                    sizeof(mmgsdi_session_select_aid_cnf_type));
      if (cnf_src_ptr->session_select_aid_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->session_select_aid_cnf.select_rsp.data_ptr,
          (void*)cnf_src_ptr->session_select_aid_cnf.select_rsp.data_ptr,
          (uint32)cnf_src_ptr->session_select_aid_cnf.select_rsp.data_len);
      }
      else
      {
        cnf_dst_ptr->session_select_aid_cnf.select_rsp.data_ptr = NULL;
      }
      break;

    case MMGSDI_CARD_STATUS_CNF:
      (void)memscpy(&cnf_dst_ptr->status_cnf,
                    sizeof(mmgsdi_status_cnf_type),
                    &cnf_src_ptr->status_cnf,
                    sizeof(mmgsdi_status_cnf_type));
      if (cnf_src_ptr->status_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->status_cnf.status_data.data_ptr,
          (void*)cnf_src_ptr->status_cnf.status_data.data_ptr,
          (uint32)cnf_src_ptr->status_cnf.status_data.data_len);
      }
      else
      {
        cnf_dst_ptr->status_cnf.status_data.data_ptr = NULL;
      }
      break;

    case MMGSDI_GET_SIM_PROFILE_CNF:
      memscpy(&cnf_dst_ptr->get_sim_profile_cnf,
              sizeof(cnf_dst_ptr->get_sim_profile_cnf),
              &cnf_src_ptr->get_sim_profile_cnf,
              sizeof(mmgsdi_get_sim_profile_cnf_type));
      break;

    case MMGSDI_SET_SIM_PROFILE_CNF:
      memscpy(&cnf_dst_ptr->set_sim_profile_cnf,
              sizeof(cnf_dst_ptr->set_sim_profile_cnf),
              &cnf_src_ptr->set_sim_profile_cnf,
              sizeof(mmgsdi_set_sim_profile_cnf_type));
      break;

    case MMGSDI_SEARCH_CNF:
      (void)memscpy(&cnf_dst_ptr->search_cnf,
                    sizeof(mmgsdi_search_cnf_type),
                    &cnf_src_ptr->search_cnf,
                    sizeof(mmgsdi_search_cnf_type));
      /* Deep copy of search data only in case of success */
      if (cnf_src_ptr->search_cnf.response_header.mmgsdi_status == MMGSDI_SUCCESS)
      {
        uimqmi_deep_copy(
          (void**)&cnf_dst_ptr->search_cnf.searched_record_nums.data_ptr,
          (void*)cnf_src_ptr->search_cnf.searched_record_nums.data_ptr,
          (uint32)cnf_src_ptr->search_cnf.searched_record_nums.data_len);
      }
      else
      {
        cnf_dst_ptr->search_cnf.searched_record_nums.data_ptr = NULL;
      }
      break;

    case MMGSDI_PURGE_TEMPORARY_IDENTITY_CNF:
      (void) memscpy(&cnf_dst_ptr->purge_temporary_identity_cnf,
                     sizeof(cnf_dst_ptr->purge_temporary_identity_cnf),
                     &cnf_src_ptr->purge_temporary_identity_cnf,
                     sizeof(mmgsdi_purge_temporary_identity_cnf_type));
      break;

    case MMGSDI_SET_APDU_BHEAVIOR_CNF:
      (void) memscpy(&cnf_dst_ptr->set_apdu_behavior_cnf,
                     sizeof(cnf_dst_ptr->set_apdu_behavior_cnf),
                     &cnf_src_ptr->set_apdu_behavior_cnf,
                     sizeof(mmgsdi_set_apdu_behavior_cnf_type));
      break;

    case MMGSDI_GET_M2M_EID_CNF:
      (void)memscpy(&cnf_dst_ptr->get_m2m_eid_cnf,
                    sizeof(mmgsdi_get_m2m_eid_cnf_type),
                    &cnf_src_ptr->get_m2m_eid_cnf,
                    sizeof(mmgsdi_get_m2m_eid_cnf_type));
      break;

    case MMGSDI_MAP_SESSIONS_TO_SLOTS_CNF:
      (void) memscpy(&cnf_dst_ptr->map_sessions_to_slots_cnf,
                     sizeof(cnf_dst_ptr->map_sessions_to_slots_cnf),
                     &cnf_src_ptr->map_sessions_to_slots_cnf,
                     sizeof(mmgsdi_map_sessions_to_slots_cnf_type));
      break;

    default:
      /* Nothing to do. This case should not happen! */
      break;
  }
} /* qmi_uim_cnf_deep_copy */


/*===========================================================================
  FUNCTION QMI_UIM_CNF_FREE_COPY()

  DESCRIPTION
    This function frees the memory that was prevously allocated when a deep
    copy of data inside the reponse was performed in MMGSDI callback.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_cnf_free_copy
(
  mmgsdi_cnf_enum_type    cnf_type,
  mmgsdi_cnf_type        *cnf_ptr
)
{
  mmgsdi_file_security_access_type* sec_ptr = NULL;

  switch(cnf_type)
  {
    case MMGSDI_READ_CNF:
      if (cnf_ptr->read_cnf.read_data.data_ptr != NULL)
      {
        uimqmi_free(cnf_ptr->read_cnf.read_data.data_ptr);
      }
      break;

    case MMGSDI_GET_FILE_ATTR_CNF:
      if (cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr != NULL)
      {
        uimqmi_free(cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr);
      }
      switch (cnf_ptr->get_file_attr_cnf.file_attrib.file_type)
      {
        case MMGSDI_TRANSPARENT_FILE:
          sec_ptr = &cnf_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security;
          break;
        case MMGSDI_LINEAR_FIXED_FILE:
          sec_ptr = &cnf_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security;
          break;
        case MMGSDI_CYCLIC_FILE:
          sec_ptr = &cnf_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security;
          break;
        default:
          break;
      }
      if (sec_ptr != NULL)
      {
        if (sec_ptr->read.protection_pin_ptr)
        {
          uimqmi_free(sec_ptr->read.protection_pin_ptr);
        }
        if (sec_ptr->write.protection_pin_ptr)
        {
          uimqmi_free(sec_ptr->write.protection_pin_ptr);
        }
        if (sec_ptr->increase.protection_pin_ptr)
        {
          uimqmi_free(sec_ptr->increase.protection_pin_ptr);
        }
        if (sec_ptr->invalidate_deactivate.protection_pin_ptr)
        {
          uimqmi_free(sec_ptr->invalidate_deactivate.protection_pin_ptr);
        }
        if (sec_ptr->rehabilitate_activate.protection_pin_ptr)
        {
          uimqmi_free(sec_ptr->rehabilitate_activate.protection_pin_ptr);
        }
      }
      break;

    case MMGSDI_ISIM_AUTH_CNF:
      if (cnf_ptr->isim_auth_cnf.res.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->isim_auth_cnf.res.data_ptr);
      }
      if (cnf_ptr->isim_auth_cnf.ck.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->isim_auth_cnf.ck.data_ptr);
      }
      if (cnf_ptr->isim_auth_cnf.ik.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->isim_auth_cnf.ik.data_ptr);
      }
      if (cnf_ptr->isim_auth_cnf.auts.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->isim_auth_cnf.auts.data_ptr);
      }
      break;

    case MMGSDI_USIM_AUTH_CNF:
      if (cnf_ptr->usim_auth_cnf.auth_response.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->usim_auth_cnf.auth_response.data_ptr);
      }
      break;

    case MMGSDI_COMPUTE_IP_AUTH_CNF:
      if (cnf_ptr->compute_ip_cnf.cmpt_ip_response_data.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->compute_ip_cnf.cmpt_ip_response_data.data_ptr);
      }
      break;

    case MMGSDI_SEND_APDU_EXT_CNF:
      if (cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr);
      }
      break;

    case MMGSDI_SAP_GET_ATR_CNF:
      if (cnf_ptr->sap_get_atr_cnf.atr_data.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->sap_get_atr_cnf.atr_data.data_ptr);
      }
      break;

    case MMGSDI_SAP_SEND_APDU_CNF:
      if (cnf_ptr->sap_send_apdu_cnf.apdu_data.data_ptr != NULL)
      {
        uimqmi_free(cnf_ptr->sap_send_apdu_cnf.apdu_data.data_ptr);
      }
      break;

    case MMGSDI_SAP_CARD_READER_STATUS_CNF:
      if (cnf_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->sap_card_reader_status_cnf.card_reader_status_data.data_ptr);
      }
      break;

    case MMGSDI_GET_ATR_CNF:
      if (cnf_ptr->get_atr_cnf.atr_data.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->get_atr_cnf.atr_data.data_ptr);
      }
      break;

    case MMGSDI_SESSION_OPEN_WITH_SELECT_RSP_CNF:
      if (cnf_ptr->session_open_with_sel_rsp_cnf.select_rsp.data_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->session_open_with_sel_rsp_cnf.select_rsp.data_ptr);
      }
      break;

    case MMGSDI_SESSION_OPEN_EXT_CNF:
      if (cnf_ptr->session_open_ext_cnf.session_info_ptr != NULL)
      {
          uimqmi_free(cnf_ptr->session_open_ext_cnf.session_info_ptr);
      }
      break;

    case MMGSDI_INCREASE_CNF:
      if (cnf_ptr->increase_cnf.increased_data.data_ptr != NULL)
      {
        uimqmi_free(cnf_ptr->increase_cnf.increased_data.data_ptr);
      }
      break;

    case MMGSDI_SESSION_SELECT_AID_CNF:
      if (cnf_ptr->session_select_aid_cnf.select_rsp.data_ptr != NULL)
      {
        uimqmi_free(cnf_ptr->session_select_aid_cnf.select_rsp.data_ptr);
      }
      break;

    case MMGSDI_CARD_STATUS_CNF:
      if (cnf_ptr->status_cnf.status_data.data_ptr != NULL)
      {
        uimqmi_free(cnf_ptr->status_cnf.status_data.data_ptr);
      }
      break;

    case MMGSDI_SEARCH_CNF:
      if (cnf_ptr->search_cnf.searched_record_nums.data_ptr != NULL)
      {
        uimqmi_free(cnf_ptr->search_cnf.searched_record_nums.data_ptr);
      }
      break;

    default:
      /* Other requests don't need to free because they did not
         make allocations during the copy */
      break;
  }
} /* qmi_uim_cnf_free_copy */


/*===========================================================================
  FUNCTION QMI_UIM_EVENT_DEEP_COPY()

  DESCRIPTION
    This function makes a deep copy of the mmgsdi_event_data_type pointer
    passed from MMGSDI to QMI and is required to pass the event in the
    QMI context.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_event_deep_copy
(
  mmgsdi_event_data_type        *event_dst_ptr,
  const mmgsdi_event_data_type  *event_src_ptr
)
{
  ASSERT(event_src_ptr && event_dst_ptr);

  /* Events can be simply memcpy'ed */
  (void)memscpy(event_dst_ptr,
                sizeof(mmgsdi_event_data_type),
                event_src_ptr,
                sizeof(mmgsdi_event_data_type));

  /* A deep copy is required for the REFRESH event, so
     handle it in special way. */
  if (event_src_ptr->evt == MMGSDI_REFRESH_EVT)
  {
    uimqmi_deep_copy(
       (void**)&event_dst_ptr->data.refresh.refresh_files.file_list_ptr,
       (void*)event_src_ptr->data.refresh.refresh_files.file_list_ptr,
       event_src_ptr->data.refresh.refresh_files.num_files * sizeof(mmgsdi_file_enum_type));
    uimqmi_deep_copy(
       (void**)&event_dst_ptr->data.refresh.refresh_files.file_path_ptr,
       (void*)event_src_ptr->data.refresh.refresh_files.file_path_ptr,
       event_src_ptr->data.refresh.refresh_files.file_path_len);
    memset(&event_dst_ptr->data.refresh.plmn_list, 0, sizeof(mmgsdi_data_type));
  }
} /* qmi_uim_event_deep_copy */


/*===========================================================================
  FUNCTION QMI_UIM_EVENT_FREE_COPY()

  DESCRIPTION
    This function makes a deep copy of the mmgsdi_cnf_type pointer
    passed from MMGSDI to QMI and is required to pass the command in the
    QMI context.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_event_free_copy
(
  mmgsdi_event_data_type * event_ptr
)
{
  ASSERT(event_ptr);

  /* Refresh is the only case that requires deep copy and needs
     to be freed */
  if (event_ptr->evt == MMGSDI_REFRESH_EVT)
  {
    if (event_ptr->data.refresh.refresh_files.file_list_ptr != NULL)
    {
      uimqmi_free(event_ptr->data.refresh.refresh_files.file_list_ptr);
      event_ptr->data.refresh.refresh_files.file_list_ptr = NULL;
    }
    if (event_ptr->data.refresh.refresh_files.file_path_ptr != NULL)
    {
      uimqmi_free(event_ptr->data.refresh.refresh_files.file_path_ptr);
      event_ptr->data.refresh.refresh_files.file_path_ptr = NULL;
    }
  }
} /* qmi_uim_event_free_copy */


/*===========================================================================
  FUNCTION QMI_UIM_CHECK_CACHED_RECORD_TYPE

  DESCRIPTION
    This function checks the cached type of a record file.

  PARAMETERS
    session_id             : Session id
    file                   : Path of the file
    record_type_ptr        : Output value with type of the record file

  RETURN VALUE
    boolean                : Indicates if the file was found in the cache

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_check_cached_record_type
(
  mmgsdi_session_id_type             session_id,
  mmgsdi_access_type                 file_access,
  mmgsdi_file_structure_enum_type *  record_type_ptr
)
{
  qmi_uimi_file_record_type * current_ptr  = NULL;

  ASSERT(qmi_uim_global_ptr && record_type_ptr);

  current_ptr = qmi_uim_global_ptr->record_cache_ptr;
  while (current_ptr != NULL)
  {
    if (current_ptr->session_id == session_id &&
        current_ptr->file_access.access_method == file_access.access_method)
    {
      if ((file_access.access_method == MMGSDI_EF_ENUM_ACCESS &&
           current_ptr->file_access.file.file_enum == file_access.file.file_enum) ||
          (file_access.access_method == MMGSDI_BY_PATH_ACCESS &&
           qmi_uim_util_same_file(current_ptr->file_access.file.path_type, file_access.file.path_type)))
      {
        *record_type_ptr = current_ptr->record_type;
        return TRUE;
      }
    }

    current_ptr = current_ptr->next_ptr;
  }

  /* File not found in the cache */
  return FALSE;
} /* qmi_uim_check_cached_record_type */


/*===========================================================================
  FUNCTION QMI_UIM_CACHE_RECORD_TYPE

  DESCRIPTION
    This function add a record type to the cache: this is used for write
    operations, where we need to know if the file is cyclic or linear-fixed.

  PARAMETERS
    session_id             : Session id
    file                   : Path of the file
    record_type            : Type of the record file

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_cache_record_type
(
  mmgsdi_session_id_type             session_id,
  mmgsdi_access_type                 file_access,
  mmgsdi_file_structure_enum_type    record_type
)
{
  mmgsdi_file_structure_enum_type prev_record_type = MMGSDI_LINEAR_FIXED_FILE;
  qmi_uimi_file_record_type *     current_ptr      = NULL;

  ASSERT(qmi_uim_global_ptr);

  /* Cache only cyclic and linear fixed files */
  if (record_type != MMGSDI_LINEAR_FIXED_FILE &&
      record_type != MMGSDI_CYCLIC_FILE)
  {
    return;
  }

  /* Cache only files in provisioning sessions and card sessions */
  if (session_id != qmi_uim_global_ptr->mmgsdi_gw_session_id[0] &&
      session_id != qmi_uim_global_ptr->mmgsdi_gw_session_id[1] &&
      session_id != qmi_uim_global_ptr->mmgsdi_gw_session_id[2] &&
      session_id != qmi_uim_global_ptr->mmgsdi_1x_session_id[0] &&
      session_id != qmi_uim_global_ptr->mmgsdi_1x_session_id[1] &&
      session_id != qmi_uim_global_ptr->mmgsdi_1x_session_id[2] &&
      session_id != qmi_uim_global_ptr->mmgsdi_card_slot_session_id[0] &&
      session_id != qmi_uim_global_ptr->mmgsdi_card_slot_session_id[1] &&
      session_id != qmi_uim_global_ptr->mmgsdi_card_slot_session_id[2])
  {
    return;
  }

  /* Cache only access by enum or by path */
  if (file_access.access_method != MMGSDI_EF_ENUM_ACCESS &&
      file_access.access_method != MMGSDI_BY_PATH_ACCESS)
  {
    return;
  }

  if (qmi_uim_check_cached_record_type(session_id, file_access, &prev_record_type))
  {
    UIM_MSG_HIGH_1("Record type already in cache, type = 0x%x",
                   prev_record_type);
    return;
  }

  /* File is not in the cache and needs to be added */
  current_ptr = uimqmi_malloc(sizeof(qmi_uimi_file_record_type));
  if (current_ptr != NULL)
  {
    memset(current_ptr, 0, sizeof(qmi_uimi_file_record_type));

    /* Populate values of new item of cache */
    current_ptr->session_id = session_id;
    current_ptr->record_type = record_type;
    (void)memscpy(&current_ptr->file_access, sizeof(mmgsdi_access_type),
                  &file_access, sizeof(mmgsdi_access_type));

    /* Add new node on top of the list */
    current_ptr->next_ptr = qmi_uim_global_ptr->record_cache_ptr;
    qmi_uim_global_ptr->record_cache_ptr = current_ptr;

    UIM_MSG_MED_0("Added record type to the cache");
  }
} /* qmi_uim_cache_record_type */


/*===========================================================================
  FUNCTION QMI_UIM_REMOVE_CACHED_RECORD_TYPE

  DESCRIPTION
    This function removes all the items of record type for a specific session.

  PARAMETERS
    session_id             : Session id

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_remove_cached_record_type
(
  mmgsdi_session_id_type             session_id
)
{
  qmi_uimi_file_record_type * current_ptr  = NULL;
  qmi_uimi_file_record_type * previous_ptr = NULL;

  ASSERT(qmi_uim_global_ptr);

  UIM_MSG_MED_0("Removing all cached record types for session");

  current_ptr = qmi_uim_global_ptr->record_cache_ptr;
  while (current_ptr != NULL)
  {
    if (current_ptr->session_id == session_id)
    {
      /* Remove item from the cache */
      if (previous_ptr != NULL)
      {
        previous_ptr->next_ptr = current_ptr->next_ptr;
        uimqmi_free(current_ptr);
        current_ptr = previous_ptr->next_ptr;
      }
      else
      {
        qmi_uim_global_ptr->record_cache_ptr = current_ptr->next_ptr;
        uimqmi_free(current_ptr);
        current_ptr = qmi_uim_global_ptr->record_cache_ptr;
      }
    }
    else
    {
      /* Go to the next item */
      previous_ptr = current_ptr;
      current_ptr  = current_ptr->next_ptr;
    }
  }
} /* qmi_uim_remove_cached_record_type */


/*===========================================================================
  FUNCTION qmi_uim_refresh_clients_count

  DESCRIPTION
    Returns the total number of clients interested in the refresh
    of one specific file

  PARAMETERS
    session_id    : session id
    file          : file

  RETURN VALUE
    uint16        : Total number of clients

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint16 qmi_uim_refresh_clients_count
(
  mmgsdi_session_id_type   session_id,
  mmgsdi_path_type         file
)
{
  uint8                            j                = 0;
  uint16                           result           = 0;
  qmi_uimi_client_state_type     * cl_sp            = NULL;
  qmi_uim_refresh_file_node_type * refresh_file_ptr = NULL;

  ASSERT(qmi_uim_state_ptr);

  /* Loop thru all clients */
  for (j = 0; j < UIMQMI_SVC_MAX_CLIDS; j++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[j];
    if (cl_sp == NULL)
    {
      continue;
    }

    /* Loop thru all files of the client */
    refresh_file_ptr = cl_sp->uim_info.refresh_files_list;
    while(refresh_file_ptr != NULL)
    {
      if (qmi_uim_util_same_file(refresh_file_ptr->file, file) &&
          refresh_file_ptr->session_id == session_id)
      {
        result++;
        break;
      }
      refresh_file_ptr = refresh_file_ptr->next_ptr;
    }
  }

  return result;
} /* qmi_uim_refresh_clients_count */


/*===========================================================================
  FUNCTION QMI_UIM_REFRESH_CREATE_DEREGISTER_LIST

  DESCRIPTION
    Creates a list of files to deregister. The list is composed by all
    those files that are present only in this client and are not present
    in the new file list.

  PARAMETERS
    client_sp           : pointer to client state
    session_id          : session id
    num_files           : number of files in the new file list
    file_list_ptr       : new file list
    deregister_list_ptr : output parameter with list of files to deregister

  RETURN VALUE
    errval          : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_refresh_create_deregister_list
(
  const qmi_uimi_client_state_type * client_sp,
  mmgsdi_session_id_type             session_id,
  const qmi_uim_files_list_type    * file_list_ptr,
  mmgsdi_refresh_file_list_type *    deregister_list_ptr
)
{
  uint32                           j                    = 0;
  uint32                           deregister_list_size = 0;
  qmi_uim_refresh_file_node_type * refresh_file_ptr     = NULL;

  ASSERT (client_sp && deregister_list_ptr);

  /* Memset the output parameter */
  memset(deregister_list_ptr, 0, sizeof(mmgsdi_refresh_file_list_type));

  /* Check if at least one file was previously registered: otherwise
     there is nothing to do! */
  if (client_sp->uim_info.refresh_num_files == 0)
  {
    return QMI_ERR_NONE;
  }

  /* Allocate memory: worst case is all files need to be deregistered */
  deregister_list_size =
    client_sp->uim_info.refresh_num_files * MMGSDI_MAX_PATH_LEN * sizeof(uint16);
  deregister_list_ptr->file_path_ptr = uimqmi_malloc(deregister_list_size);
  if (deregister_list_ptr->file_path_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Loop thru all the existing files of this client */
  refresh_file_ptr = client_sp->uim_info.refresh_files_list;
  while(refresh_file_ptr != NULL)
  {
    boolean present_in_file_list    = FALSE;
    uint16  count                   = 0;

    if (refresh_file_ptr->session_id != session_id)
    {
      /* this file belongs to a different session: nothing to do */
      refresh_file_ptr = refresh_file_ptr->next_ptr;
      continue;
    }

    /* Check if this file is in the new list */
    while (file_list_ptr != NULL)
    {
      if (file_list_ptr->file.access_method == MMGSDI_BY_PATH_ACCESS &&
          qmi_uim_util_same_file(refresh_file_ptr->file, file_list_ptr->file.file.path_type))
      {
        present_in_file_list = TRUE;
        break;
      }
      file_list_ptr = file_list_ptr->next_ptr;
    }

    /* If the file is present in the new list, skip it */
    if (present_in_file_list)
    {
      refresh_file_ptr = refresh_file_ptr->next_ptr;
      continue;
    }

    /* Count number of clients registered for this file */
    count = qmi_uim_refresh_clients_count(
                           session_id,
                           refresh_file_ptr->file);

    /* If count is 1, it means that this is the only client that is registered
       for refresh for this specific file. In this case, since the client is no
       longer interested in the file, we need to deregister it */
    if (count == 1 &&
        (deregister_list_ptr->file_path_len +
           2*refresh_file_ptr->file.path_len) < deregister_list_size)
    {
      for (j = 0; j < refresh_file_ptr->file.path_len &&
                  j < MMGSDI_MAX_PATH_LEN; j++)
      {
        deregister_list_ptr->file_path_ptr[deregister_list_ptr->file_path_len++] =
          (refresh_file_ptr->file.path_buf[j] >> 8) & 0xFF;
        deregister_list_ptr->file_path_ptr[deregister_list_ptr->file_path_len++] =
          (refresh_file_ptr->file.path_buf[j]) & 0xFF;
      }
    }

    refresh_file_ptr = refresh_file_ptr->next_ptr;
  }

  /* If no file needs to be deregistered, we can immediately
     free the memory */
  if (deregister_list_ptr->file_path_len == 0)
  {
    uimqmi_free(deregister_list_ptr->file_path_ptr);
    deregister_list_ptr->file_path_ptr = NULL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_refresh_create_deregister_list */


/*===========================================================================
  FUNCTION QMI_UIM_REFRESH_CREATE_REGISTER_LIST

  DESCRIPTION
    Creates a list of files to register. The list is composed by all
    those files that are present in the new list and that no client
    is currently registered to.

  PARAMETERS
    session_id        : session id
    num_files         : number of files in the new file list
    file_list_ptr     : new file list
    register_list_ptr : output parameter with list of files to register

  RETURN VALUE
    errval          : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_refresh_create_register_list
(
  mmgsdi_session_id_type            session_id,
  const qmi_uim_files_list_type   * file_list_ptr,
  mmgsdi_refresh_file_list_type *   register_list_ptr
)
{
  uint16                          num_files   = 0;
  const qmi_uim_files_list_type * current_ptr = NULL;

  ASSERT (register_list_ptr);

  /* Memset the output parameter */
  memset(register_list_ptr, 0, sizeof(mmgsdi_refresh_file_list_type));

  /* Check if new list is empty: in this case, there is nothing to do! */
  if (file_list_ptr == NULL)
  {
    return QMI_ERR_NONE;
  }

  /* Count the number of files in the list */
  current_ptr = file_list_ptr;
  while (current_ptr != NULL)
  {
    num_files++;
    current_ptr = current_ptr->next_ptr;
  }

  /* Allocate memory: worst case is all files need to be registered */
  register_list_ptr->file_path_ptr = uimqmi_malloc(
            num_files * MMGSDI_MAX_PATH_LEN * sizeof(uint16));
  if (register_list_ptr->file_path_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Loop thru all the existing files of this client */
  while (file_list_ptr != NULL)
  {
    uint16 count = 0;

    /* Verify that access is by path */
    if (file_list_ptr->file.access_method != MMGSDI_BY_PATH_ACCESS)
    {
      file_list_ptr = file_list_ptr->next_ptr;
      continue;
    }

    /* Count number of clients registered for this file */
    count = qmi_uim_refresh_clients_count(session_id,
                                          file_list_ptr->file.file.path_type);

    /* If count is 0, it means that no client is registered. In this
       case, we need to register it */
    if (count == 0)
    {
      mmgsdi_len_type i = 0;
      for (i = 0; i < file_list_ptr->file.file.path_type.path_len &&
                  i < MMGSDI_MAX_PATH_LEN; i++)
      {
        register_list_ptr->file_path_ptr[register_list_ptr->file_path_len++] =
          (file_list_ptr->file.file.path_type.path_buf[i] >> 8) & 0xFF;
        register_list_ptr->file_path_ptr[register_list_ptr->file_path_len++] =
          (file_list_ptr->file.file.path_type.path_buf[i]) & 0xFF;
      }
    }

    file_list_ptr = file_list_ptr->next_ptr;
  }

  /* If no file needs to be registered, we can immediately
     free the memory */
  if (register_list_ptr->file_path_len == 0)
  {
    uimqmi_free(register_list_ptr->file_path_ptr);
    register_list_ptr->file_path_ptr = NULL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_refresh_create_register_list */


/*===========================================================================
  FUNCTION QMI_UIM_REMOVE_CLIENT_REFRESH_FILES

  DESCRIPTION
    Removes all the files of a specific session type from the list of
    refresh files of a given client.

  PARAMETERS
    client_sp       : pointer to client state
    session_id      : session id

  RETURN VALUE
    errval          : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_remove_client_refresh_files
(
  qmi_uimi_client_state_type *     client_sp,
  mmgsdi_session_id_type           session_id
)
{
  qmi_uim_refresh_file_node_type * curr_file_ptr = NULL;
  qmi_uim_refresh_file_node_type * prev_file_ptr = NULL;

  ASSERT(client_sp);

  curr_file_ptr = client_sp->uim_info.refresh_files_list;

  /* Remove matching items from the linked list */
  while(curr_file_ptr != NULL)
  {
    if (curr_file_ptr->session_id == session_id)
    {
      if(prev_file_ptr == NULL)
      {
        /* remove from the head of the list */
        client_sp->uim_info.refresh_files_list = curr_file_ptr->next_ptr;
        uimqmi_free(curr_file_ptr);
        curr_file_ptr = client_sp->uim_info.refresh_files_list;
      }
      else
      {
        /* remove from middle or the tail of the list */
        prev_file_ptr->next_ptr = curr_file_ptr->next_ptr;
        uimqmi_free(curr_file_ptr);
        curr_file_ptr = prev_file_ptr->next_ptr;
      }
      client_sp->uim_info.refresh_num_files--;
    }
    else
    {
      /* maintain pointer to the previous node (to be used while freeing the
         last node) */
      prev_file_ptr = curr_file_ptr;
      curr_file_ptr = curr_file_ptr->next_ptr;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_remove_client_refresh_files */


/*===========================================================================
  FUNCTION QMI_UIM_ADD_CLIENT_REFRESH_FILES

  DESCRIPTION
    Add the files of a list into the list of refresh files of a given client.

  PARAMETERS
    client_sp       : pointer to client state
    session_id      : session id
    num_files       : number of files in the file list
    file_list_ptr   : list of files to be added

  RETURN VALUE
    errval          : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_add_client_refresh_files
(
  qmi_uimi_client_state_type *      client_sp,
  mmgsdi_session_id_type            session_id,
  const qmi_uim_files_list_type   * file_list_ptr
)
{
  qmi_uim_refresh_file_node_type * new_file_ptr      = NULL;
  qmi_uim_refresh_file_node_type * last_file_ptr     = NULL;

  ASSERT(client_sp);

  if(client_sp->uim_info.refresh_num_files > 0)
  {
    last_file_ptr = client_sp->uim_info.refresh_files_list;
  }

  while(last_file_ptr != NULL && last_file_ptr->next_ptr != NULL)
  {
    last_file_ptr = last_file_ptr->next_ptr;
  }

  /* Copy list from TLV into this client */
  while (file_list_ptr != NULL)
  {
    /* Add only files successfully converted into enum */
    if (file_list_ptr->file.access_method == MMGSDI_BY_PATH_ACCESS)
    {
      new_file_ptr = (qmi_uim_refresh_file_node_type *)uimqmi_malloc(
                        sizeof(qmi_uim_refresh_file_node_type));
      if(new_file_ptr == NULL)
      {
        return QMI_ERR_NO_MEMORY;
      }

      memset(new_file_ptr, 0x00, sizeof(qmi_uim_refresh_file_node_type));

      new_file_ptr->session_id = session_id;
      (void)memscpy(&new_file_ptr->file,
                    sizeof(mmgsdi_path_type),
                    &file_list_ptr->file.file.path_type,
                    sizeof(mmgsdi_path_type));

      if(last_file_ptr == NULL)
      {
        client_sp->uim_info.refresh_files_list = new_file_ptr;
        last_file_ptr = new_file_ptr;
      }
      else
      {
        last_file_ptr->next_ptr = new_file_ptr;
        last_file_ptr = last_file_ptr->next_ptr;
      }
      client_sp->uim_info.refresh_num_files++;
    }

    file_list_ptr = file_list_ptr->next_ptr;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_add_client_refresh_files */


/*===========================================================================
  FUNCTION QMI_UIM_FILE_PRESENT_IN_FCN_LIST

  DESCRIPTION
    Check if a file is present in the file list received in the refresh event
    from MMGSDI in path format.

  PARAMETERS
    file            : file to be checked
    file_list_ptr   : array with file list
    file_list_len   : size in bytes of file list

  RETURN VALUE
    boolean         : indicates if the file is present

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_file_present_in_fcn_list
(
  mmgsdi_path_type   file,
  const uint8      * file_list_ptr,
  uint32             file_list_len
)
{
  uint8 i            = 0;
  uint8 sequence_len = 0;
  uint8 sequence_val[MMGSDI_MAX_PATH_LEN * 2];

  ASSERT(file.path_len <= MMGSDI_MAX_PATH_LEN);

  if (file_list_ptr == NULL || file_list_len == 0)
  {
    return FALSE;
  }

  /* Calculate size of sequence of bytes to search in the event */
  memset(&sequence_val, 0, sizeof(sequence_val));
  sequence_len = (uint8)(file.path_len * sizeof(uint16));
  ASSERT(sequence_len <= MMGSDI_MAX_PATH_LEN * 2);

  if (sequence_len > file_list_len)
  {
    /* No need to continue checking */
    return FALSE;
  }

  /* Compose file path as sequence of bytes */
  for (i = 0; i < file.path_len; i++)
  {
    sequence_val[i * 2] =
      (uint8)((file.path_buf[i] >> 8) & 0xFF);
    sequence_val[(i * 2) + 1] =
      (uint8)(file.path_buf[i] & 0xFF);
  }

  for (i = 0; i <= file_list_len - sequence_len; i++)
  {
    if (memcmp(&file_list_ptr[i],
               sequence_val,
               sequence_len) == 0)
    {
      return TRUE;
    }
  }

  return FALSE;
} /* qmi_uim_file_present_in_fcn_list */


/*===========================================================================
  FUNCTION QMI_UIM_CLEAN_UP_REFRESH_EVT_INFO

  DESCRIPTION
    Cleans up refresh evt info if available

  PARAMETERS
     refresh_index    : index to the refresh cache

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_clean_up_refresh_evt_info
(
  uint16 refresh_index
)
{
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
  {
    return;
  }

  /* Clear the old cache */
  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr != NULL)
  {
    /* Clear the old file info cache */
    if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr->refresh_files.file_path_ptr != NULL)
    {
      uimqmi_free(qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr->refresh_files.file_path_ptr);
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr->refresh_files.file_path_ptr = NULL;
    }

    /* Clear the old event cache */
    uimqmi_free(qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr);
    qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr = NULL;
  }
  memset(&qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache,
         0,
         sizeof(qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache));
}/* qmi_uim_clean_up_refresh_evt_info */


/*===========================================================================
  FUNCTION QMI_UIM_CACHE_REFRESH_EVENT

  DESCRIPTION
    Caches the refresh event to be fetched later.

  PARAMETERS
    session_id      : session id
    refresh_evt_ptr : refresh event

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_cache_refresh_event
(
  mmgsdi_session_id_type               session_id,
  const mmgsdi_refresh_evt_info_type * refresh_evt_ptr
)
{
  uint16                         refresh_index   = 0;
  mmgsdi_refresh_evt_info_type * event_cache_ptr = NULL;

  ASSERT(refresh_evt_ptr && qmi_uim_global_ptr);

  /* Retrieve session index for the refresh */
  if (qmi_uim_util_session_id_to_index(&refresh_index, session_id) != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                  session_id);
    return QMI_ERR_INTERNAL;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
  {
    return QMI_ERR_INTERNAL;
  }

  qmi_uim_clean_up_refresh_evt_info(refresh_index);

  /* Copy session id */
  qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.session_id = session_id;

  qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr =
    uimqmi_malloc(sizeof(mmgsdi_refresh_evt_info_type));

  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr != NULL)
  {
    event_cache_ptr = qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr;

    /* Copy refresh event */
    (void)memscpy(event_cache_ptr, sizeof(mmgsdi_refresh_evt_info_type),
                  refresh_evt_ptr, sizeof(mmgsdi_refresh_evt_info_type));

    /* Deep copy of file list */
    if (refresh_evt_ptr->refresh_files.file_path_ptr != NULL &&
        refresh_evt_ptr->refresh_files.file_path_len > 0)
    {
      uint32 size = refresh_evt_ptr->refresh_files.file_path_len;

      event_cache_ptr->refresh_files.file_path_ptr =
                              uimqmi_malloc(size);

      if (event_cache_ptr->refresh_files.file_path_ptr != NULL)
      {
        (void)memscpy(event_cache_ptr->refresh_files.file_path_ptr,
                      size,
                      refresh_evt_ptr->refresh_files.file_path_ptr,
                      size);
      }
    }
    else
    {
      event_cache_ptr->refresh_files.file_path_len = 0;
      event_cache_ptr->refresh_files.file_path_ptr = NULL;
    }
  }
  else
  {
    return QMI_ERR_NO_MEMORY;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_cache_refresh_event */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SESSION_TYPE_BASED_REFRESH_IND

  DESCRIPTION
    Compose the list of clients for specific session type interested in the
    refresh indication for the refresh event received from the modem and sends
    the indication.

  PARAMETERS
    session_id      : session id
    refresh_evt_ptr : refresh event

  RETURN VALUE
    uint16          : number of sent indications

  DEPENDENCIES
    None

  SIDE EFFECTS
    Overwrites the table of services interested in the indication
    in the global variable.
===========================================================================*/
static uint16 qmi_uim_send_session_type_based_refresh_ind
(
  mmgsdi_session_id_type               session_id,
  const mmgsdi_refresh_evt_info_type * refresh_evt_ptr
)
{
  uint8                            j                = 0;
  qmi_uimi_client_state_type *     cl_sp            = NULL;
  uint16                           refresh_index    = 0;
  uint32                           notify_mask      = 0;
  mmgsdi_session_type_enum_type    session_type     = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type          aid              = { 0, };
  qmi_uim_refresh_file_node_type * refresh_file_ptr = NULL;
  boolean                          send_only_ind    = FALSE;
  uint16                           client_count     = 0;

  ASSERT(refresh_evt_ptr && qmi_uim_global_ptr && qmi_uim_state_ptr);

  /* Retrieve session index for the refresh */
  if (qmi_uim_util_session_id_to_index(&refresh_index, session_id) != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                  session_id);
    return 0;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  /* In case of app_reset and 3g session reset, global data is already freed
     at sesison de-activation. We just need to send indication to clients
     here without maintaining the clients list. */
  if ((refresh_evt_ptr->mode == MMGSDI_REFRESH_3G_SESSION_RESET   ||
       refresh_evt_ptr->mode == MMGSDI_REFRESH_NAA_APP_RESET)     &&
      (refresh_evt_ptr->stage == MMGSDI_REFRESH_STAGE_END_SUCCESS ||
       refresh_evt_ptr->stage == MMGSDI_REFRESH_STAGE_END_FAILED) &&
      (qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL))
  {
    send_only_ind = TRUE;
  }

  if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL && !send_only_ind)
  {
    return 0;
  }

  /* Convert session id into session type + AID */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_0("Error converting session id into session type");
    return 0;
  }

  if (!send_only_ind)
  {
    /* Empty the current list of clients */
    qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count = 0;
    memset(qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_list,
           0,
           sizeof(qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_list));
  }

  /* Calculate the refresh mask */
  notify_mask = 1 << refresh_index;

  /* Loop thru all clients */
  for (j = 0; j < UIMQMI_SVC_MAX_CLIDS; j++)
  {
    boolean send_event = FALSE;

    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[j];
    if (cl_sp == NULL)
    {
      continue;
    }

    /* Continue with the next client if both the masks are not set */
    if (((cl_sp->uim_info.refresh_notify_mask & notify_mask) == 0) &&
        ((cl_sp->uim_info.refresh_all_notify_mask & notify_mask) == 0))
    {
      continue;
    }

    /* For FCN mode, check are done as follows:
       1. For clients who have specified the files, match file path against the list.
       2. For clients interested in all files, match against the session type only.
       For all other modes, include all clients interested in the refresh */
    if (refresh_evt_ptr->mode == MMGSDI_REFRESH_NAA_FCN &&
        ((cl_sp->uim_info.refresh_all_notify_mask & notify_mask) == 0) &&
        ((cl_sp->uim_info.refresh_notify_mask & notify_mask) != 0))
    {
      refresh_file_ptr = cl_sp->uim_info.refresh_files_list;

      while(refresh_file_ptr != NULL && !send_event)
      {
        /* Check if the file is for this session */
        if (refresh_file_ptr->session_id != session_id)
        {
          refresh_file_ptr = refresh_file_ptr->next_ptr;
          continue;
        }

        if (qmi_uim_file_present_in_fcn_list(
               refresh_file_ptr->file,
               refresh_evt_ptr->refresh_files.file_path_ptr,
               refresh_evt_ptr->refresh_files.file_path_len))
        {
          send_event = TRUE;
          break;
        }

        refresh_file_ptr = refresh_file_ptr->next_ptr;
      }
    }
    else
    {
      send_event = TRUE;
    }

    /* Check if the client decided to vote */
    if (refresh_evt_ptr->stage == MMGSDI_REFRESH_STAGE_WAIT_FOR_OK_TO_INIT ||
        refresh_evt_ptr->stage == MMGSDI_REFRESH_STAGE_WAIT_FOR_OK_TO_FCN)
    {
      if (cl_sp->uim_info.refresh_vote == FALSE)
      {
        send_event = FALSE;
      }
    }

    /* Send the indication */
    if (send_event)
    {
      dsm_item_type *   indication  = NULL;

      UIM_MSG_HIGH_1("Sending refresh indication (cl_id=0x%x)", j);

      /* Prepare TLV with refresh indication */
      if (qmi_uim_response_refresh_event(
                       UIMI_TLV_RESP_TAG_OPTIONAL_1,
                       &indication,
                       session_type,
                       &aid,
                       refresh_evt_ptr))
      {
        /* We can send the indication if we fail to add the TLV with the
           refresh enforcement policy */
        (void)qmi_uim_response_refresh_enforcement_policy(
                UIMI_TLV_RESP_TAG_OPTIONAL_2,
                &indication,
                refresh_evt_ptr->enforcement_policy);

        /* Send indication to the client */
       (void)uimqmi_send_indication(cl_sp->common.clid,
                                    (uint16)UIMI_CMD_VAL_REFRESH_IND,
                                    indication,
                                    QMUX_SERVICE_UIM);

        if (send_only_ind)
        {
          client_count++;
        }
        else
        {
          /* Add client to the global list */
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_list[qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count] = cl_sp;
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count++;
        }
      }
      else
      {
        /* Ignore error... there is nothing we can do about it.
           The event will not be delivered to the client */
        UIM_MSG_ERR_0("Error sending refresh indication");
        dsm_free_packet(&indication);
      }
    }
  }

  if (!send_only_ind)
  {
    client_count = qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count;
    if (client_count > 0)
    {
      /* Cache of refresh event */
      (void)qmi_uim_cache_refresh_event(session_id, refresh_evt_ptr);
    }
  }

  UIM_MSG_HIGH_1("Refresh indication sent to 0x%x clients", client_count);
  return client_count;
}/* qmi_uim_send_session_type_based_refresh_ind */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_REFRESH_IND

  DESCRIPTION
    Compose the list of clients interested in the refresh indication
    for the specific refresh event received from the modem and sends the
    indication.

  PARAMETERS
    session_id      : session id
    refresh_evt_ptr : refresh event

  RETURN VALUE
    uint16          : number of sent indications

  DEPENDENCIES
    None

  SIDE EFFECTS
    Overwrites the table of services interested in the indication
    in the global variable.
===========================================================================*/
static uint16 qmi_uim_send_refresh_ind
(
  mmgsdi_session_id_type               session_id,
  const mmgsdi_refresh_evt_info_type * refresh_evt_ptr
)
{
  uint8                            prov_index       = 0;
  uint8                            card_index       = 0;
  uint8                            non_prov_index   = 0;
  mmgsdi_slot_id_enum_type         refresh_slot_id  = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_slot_id_enum_type         slot_id          = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_session_type_enum_type    session_type     = MMGSDI_MAX_SESSION_TYPE_ENUM;
  uint16                           ind_count        = 0;

  ASSERT(refresh_evt_ptr && qmi_uim_global_ptr);

  /* Send Refresh indication to all clients who access the sessions, tied to
     slot where Refresh Reset is trigerred, irrespective of their session type
     when refresh stage is START */
  if (refresh_evt_ptr->mode == MMGSDI_REFRESH_RESET &&
      refresh_evt_ptr->stage == MMGSDI_REFRESH_STAGE_START)
  {
    /* first get the slot id for session id, retrieve all provisioning
       session ids on that slot and send refresh indication to clients who
       access that type of prov sessions */
    if (qmi_uim_util_session_id_to_slot(&refresh_slot_id, session_id) != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_1("Error converting session id (0x%x) into corresponding slot",
                  session_id);
      return 0;
    }

    for (prov_index = 0; prov_index < QMI_UIM_MAX_PROV_SESSIONS; prov_index++)
    {
      if (qmi_uim_util_session_id_to_slot(&slot_id, qmi_uim_global_ptr->mmgsdi_gw_session_id[prov_index]) == QMI_ERR_NONE)
      {
        if(refresh_slot_id == slot_id)
        {
          ind_count += qmi_uim_send_session_type_based_refresh_ind(
                         qmi_uim_global_ptr->mmgsdi_gw_session_id[prov_index],
                         refresh_evt_ptr);
        }
      }
    }

    for (prov_index = 0; prov_index < QMI_UIM_MAX_PROV_SESSIONS; prov_index++)
    {
      if (qmi_uim_util_session_id_to_slot(&slot_id, qmi_uim_global_ptr->mmgsdi_1x_session_id[prov_index]) == QMI_ERR_NONE)
      {
        if(refresh_slot_id == slot_id)
        {
          ind_count += qmi_uim_send_session_type_based_refresh_ind(
                         qmi_uim_global_ptr->mmgsdi_1x_session_id[prov_index],
                         refresh_evt_ptr);
        }
      }
    }

    /* Send the START indication to clients registered for card slot sessions. */
    if ((uimqmi_mmgsdi_slot_to_slot_index(refresh_slot_id, &card_index,
           qmi_uim_global_ptr->card_state.num_slots) == QMI_ERR_NONE) &&
        (qmi_uim_global_ptr->mmgsdi_card_slot_session_id[card_index] == session_id))
    {
      ind_count += qmi_uim_send_session_type_based_refresh_ind(
                     qmi_uim_global_ptr->mmgsdi_card_slot_session_id[card_index],
                     refresh_evt_ptr);
    }

    /* Before we clean up the AID in case of refresh reset, we also need to
       send indications to clients registered for non-prov sessions on that slot */
    switch(refresh_slot_id)
    {
      case MMGSDI_SLOT_1:
        session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
        break;
      case MMGSDI_SLOT_2:
        session_type = MMGSDI_NON_PROV_SESSION_SLOT_2;
        break;
      case MMGSDI_SLOT_3:
        session_type = MMGSDI_NON_PROV_SESSION_SLOT_3;
        break;
      default:
        return ind_count;
    }

    for (non_prov_index = 0; non_prov_index < UIM_MAX_NON_PROV_SESSIONS; non_prov_index++)
    {
      if ((qmi_uim_global_ptr->non_prov_session[non_prov_index] != NULL ) &&
          (qmi_uim_global_ptr->non_prov_session[non_prov_index]->session_type      == session_type) &&
          (qmi_uim_global_ptr->non_prov_session[non_prov_index]->mmgsdi_session_id != session_id) &&
          (qmi_uim_global_ptr->non_prov_session[non_prov_index]->mmgsdi_session_id != UIM_INVALID_SESSION_ID))
      {
        uint16 refresh_index = 0;

        /* Also on the particular refresh index */
        if (qmi_uim_util_session_id_to_index(
              &refresh_index,
              qmi_uim_global_ptr->non_prov_session[non_prov_index]->mmgsdi_session_id) != QMI_ERR_NONE)
        {
          UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                        qmi_uim_global_ptr->non_prov_session[non_prov_index]->mmgsdi_session_id);
          continue;
        }

        ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

        if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index] != NULL &&
            (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state == UIM_REFRESH_STATE_IDLE ||
             qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state == UIM_REFRESH_STATE_WAIT_FOR_OK))
        {
          ind_count += qmi_uim_send_session_type_based_refresh_ind(
                         qmi_uim_global_ptr->non_prov_session[non_prov_index]->mmgsdi_session_id,
                         refresh_evt_ptr);
        }
      }
    }
  }
  /* Don't sent any indication to QMI UIM client for Refresh Reset AUTO */
  else if (refresh_evt_ptr->mode != MMGSDI_REFRESH_RESET_AUTO)
  {
    ind_count = qmi_uim_send_session_type_based_refresh_ind(
                  session_id,
                  refresh_evt_ptr);
  }

  return ind_count;
} /* qmi_uim_send_refresh_ind */


/*===========================================================================
  FUNCTION QMI_UIM_UPDATE_PIN_ID()

  DESCRIPTION
    Updates the passed PIN ID pointer with the appropriate enum value after
    validating PIN ID & the Key reference.

  PARAMETERS
    pin_id_ptr       : input/output pointer to pin id
    keyref_id        : input key reference id

  RETURN VALUE
    errval           : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_update_pin_id
(
  mmgsdi_pin_enum_type                 * pin_id_ptr,
  uim_pin_key_ref_enum_v01               keyref_id
)
{
  ASSERT(pin_id_ptr);

  if ((*pin_id_ptr != MMGSDI_PIN1) && (*pin_id_ptr != MMGSDI_PIN2))
  {
    UIM_MSG_ERR_1("QMI_UIM invalid pin id: 0x%x", *pin_id_ptr);
    return QMI_ERR_INVALID_PINID;
  }

  /* Map appropriate PIN enum type */
  switch (keyref_id)
  {
    case UIM_PIN_KEY_REF_APPL_1_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_1 : MMGSDI_KEY_REF_SEC_PIN_APPL_1;
      break;
    case UIM_PIN_KEY_REF_APPL_2_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_2 : MMGSDI_KEY_REF_SEC_PIN_APPL_2;
      break;
    case UIM_PIN_KEY_REF_APPL_3_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_3 : MMGSDI_KEY_REF_SEC_PIN_APPL_3;
      break;
    case UIM_PIN_KEY_REF_APPL_4_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_4 : MMGSDI_KEY_REF_SEC_PIN_APPL_4;
      break;
    case UIM_PIN_KEY_REF_APPL_5_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_5 : MMGSDI_KEY_REF_SEC_PIN_APPL_5;
      break;
    case UIM_PIN_KEY_REF_APPL_6_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_6 : MMGSDI_KEY_REF_SEC_PIN_APPL_6;
      break;
    case UIM_PIN_KEY_REF_APPL_7_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_7 : MMGSDI_KEY_REF_SEC_PIN_APPL_7;
      break;
    case UIM_PIN_KEY_REF_APPL_8_V01:
      *pin_id_ptr = (*pin_id_ptr == MMGSDI_PIN1) ?
        MMGSDI_KEY_REF_PIN_APPL_8 : MMGSDI_KEY_REF_SEC_PIN_APPL_8;
      break;
    default:
      UIM_MSG_ERR_1("QMI_UIM invalid key reference id: 0x%x", keyref_id);
      return QMI_ERR_INVALID_ARG;
  }
  return QMI_ERR_NONE;
} /* qmi_uim_update_pin_id */


/*===========================================================================
  FUNCTION QMI_UIM_REMOVE_CL_SP_FROM_NON_PROV_SESSION_INDEX()

  DESCRIPTION
    Removes the passed in client state pointer from the non-prov. session
    index requested (in the global array).

  PARAMETERS
    index  : non-prov. session index in the global array
    cl_sp  : client state pointer

  RETURN VALUE
    errval : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_remove_cl_sp_from_non_prov_session_index
(
  uint16                       index,
  qmi_uimi_client_state_type * client_sp
)
{
  uint16                       cl_sp_index = 0;

  ASSERT(index < UIM_MAX_NON_PROV_SESSIONS && qmi_uim_global_ptr);

  if((client_sp == NULL) ||
     (qmi_uim_global_ptr->non_prov_session[index] == NULL))
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Go through the cl_sp array, check if passed in cl_sp is already present,
     and also find any empty slot */
  for(cl_sp_index = 0;
      cl_sp_index < (uint16)UIM_MAX_NON_PROV_CLIENTS;
      cl_sp_index++)
  {
    if(qmi_uim_global_ptr->non_prov_session[index]->client_info.opening_cl_sp[cl_sp_index] ==
          client_sp)
    {
      /* Remove the cl_sp */
      qmi_uim_global_ptr->non_prov_session[index]->client_info.opening_cl_sp[cl_sp_index] =
        NULL;
      /* Decrement the Client count */
      qmi_uim_global_ptr->non_prov_session[index]->client_info.client_count--;
      break;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_remove_cl_sp_from_non_prov_session_index */


/*===========================================================================
  FUNCTION QMI_UIM_REMOVE_REFRESH_REGISTRATION()

  DESCRIPTION
    Removes data about refresh registration for a non provisioning session.

  PARAMETERS
    session_id  : session id

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_remove_refresh_registration
(
  mmgsdi_session_id_type   session_id
)
{
  uint16                       i                      = 0;
  qmi_uimi_client_state_type * cl_sp                  = NULL;
  uint16                       refresh_index          = 0;
  uint32                       notify_mask            = 0;
  uint8                        non_prov_refresh_index = 0;

  ASSERT(qmi_uim_global_ptr && qmi_uim_state_ptr);

  if (qmi_uim_util_session_id_to_index(&refresh_index, session_id) != QMI_ERR_NONE)
  {
    /* The session id was not used for refresh... nothing to do */
    return;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  /* Calculate the notify mask */
  notify_mask = 1 << refresh_index;

  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];
    if ( cl_sp != NULL )
    {
      /* remove the notify flag for this session id */
      cl_sp->uim_info.refresh_notify_mask = (cl_sp->uim_info.refresh_notify_mask & (~notify_mask));

      /* Remove the notify all flag for this session id */
      cl_sp->uim_info.refresh_all_notify_mask = (cl_sp->uim_info.refresh_all_notify_mask & (~notify_mask));

      /* Remove files for this session from the list of files that each client
         registered */
      (void)qmi_uim_remove_client_refresh_files(cl_sp, session_id);
    }
  }

  /* Clean the non-prov session id that is used for refresh */
  if (refresh_index >= UIM_REFRESH_PROV_COUNT)
  {
    non_prov_refresh_index = refresh_index - UIM_REFRESH_PROV_COUNT;
    qmi_uim_global_ptr->non_prov_refresh_session_id[non_prov_refresh_index] =
      UIM_INVALID_SESSION_ID;
  }
} /* qmi_uim_remove_refresh_registration */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_APDU_SESSION_CLOSED_IND()

  DESCRIPTION
    This function sends a Session Closed Indication indication to the
    client that has opened the APDU Session.

  PARAMETERS
    index : apdu session index in the global array
    cause : Cause for session closure

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_apdu_session_closed_ind
(
  uint16                                 index,
  mmgsdi_session_closed_cause_enum_type  cause
)
{
  dsm_item_type              * indication = NULL;
  qmi_uimi_client_state_type * client_sp  = NULL;
  mmgsdi_slot_id_enum_type     slot       = MMGSDI_MAX_SLOT_ID_ENUM;
  uint8                        channel_id = 0;

  ASSERT(qmi_uim_global_ptr);

  if(index >= UIM_MAX_APDU_CHANNEL_COUNT)
  {
    UIM_MSG_ERR_1("Invalid apdu session index: 0x%x, cannot send session closed evt",
                  index);
    return;
  }

  client_sp = qmi_uim_global_ptr->apdu_channel[index]->opening_cl_sp;

  /* Check if the Client state is valid */
  if(client_sp == NULL)
  {
    return;
  }

  /* Retrieve the slot, channel info and cause - to be sent as part of the Session
     Closed event */
  slot = qmi_uim_global_ptr->apdu_channel[index]->slot;
  channel_id = qmi_uim_global_ptr->apdu_channel[index]->logical_channel;

  /* Insert channel ID in TLV */
  if (FALSE == qmi_uim_response_logical_channel(
                 UIMI_TLV_RESP_TAG_OPTIONAL_2,
                 &indication,
                 channel_id))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert card slot in TLV */
  if (FALSE == qmi_uim_response_slot(
                   UIMI_TLV_TAG_MANDATORY_1,
                   &indication,
                   slot))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert the cause in the TLV. Indication is sent
     even if the cause TLV is not inserted */
  (void)qmi_uim_response_session_closed_cause(
                   UIMI_TLV_RESP_TAG_OPTIONAL_4,
                   &indication,
                   cause);


  /* Send indication to the client */
  (void)uimqmi_send_indication(client_sp->common.clid,
                               (uint16)UIMI_CMD_VAL_SESSION_CLOSED_IND,
                               indication,
                               QMUX_SERVICE_UIM);

} /* qmi_uim_send_apdu_session_closed_ind */


/*===========================================================================
  FUNCTION QMI_UIM_REMOVE_APDU_SESSION()

  DESCRIPTION
    Removes a non provisioning session previously opened to stream APDUs
    from the global array

  PARAMETERS
    session_id           :  session id
    send_ind             :  Specify if a QMI_UIM_SESSION_CLOSED_IND needs to
                            be sent to the client that has opened the session
                            (TRUE) or not (FALSE)
    session_closed_cause : Cause for session closure

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_remove_apdu_session
(
  mmgsdi_session_id_type                session_id,
  boolean                               send_ind,
  mmgsdi_session_closed_cause_enum_type session_closed_cause
)
{
  uint16  i  = 0;

  ASSERT(qmi_uim_global_ptr);

  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id == session_id)
    {
      /* Send Session Closed Indication to Client */
      if(send_ind)
      {
        qmi_uim_send_apdu_session_closed_ind(i, session_closed_cause);
      }

      /* Remove refresh data of the non prov session */
      qmi_uim_remove_refresh_registration(session_id);

      /* Free channel info. Skip if a response is pending to a client because
         this info is needed during the open session callback */
      if (qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id ==
          UIM_INVALID_SESSION_ID)
      {
        UIM_MSG_HIGH_1("Skipping removal of APDU channel info at index: %d",
                       i);
      }
      else
      {
        uimqmi_free(qmi_uim_global_ptr->apdu_channel[i]);
        qmi_uim_global_ptr->apdu_channel[i] = NULL;
      }

      break;
    }
  }
} /* qmi_uim_remove_apdu_session */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_READ_TRANSPARENT()

  DESCRIPTION
    Reads the content of a transparent file on the card

  PARAMETERS
    command_id        : command id
    session_id        : session id
    file_access       : file to be read
    offset            : offset in the file
    length            : length to be read
    encryption_status : requested encrption type
    token_ptr         : token pointer
    cmd_buf_ptr       : userdata
    clid              : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_execute_read_transparent
(
  qmi_uimi_cmd_val_e_type             command_id,
  mmgsdi_session_id_type              session_id,
  mmgsdi_access_type                  file_access,
  mmgsdi_offset_type                  offset,
  mmgsdi_len_type                     length,
  qmi_uim_requested_encrytion_type    encryption_status,
  const uint32                      * token_ptr,
  void *                              cmd_buf_ptr,
  uint8                               clid
)
{
  qmi_error_e_type               errval            = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status     = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr   = NULL;
  mmgsdi_session_type_enum_type  session_type      = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type        aid               = { 0, };
  mmgsdi_access_type             path_file_access  = file_access;

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  if (encryption_status == QMI_UIM_COMPLETE_ENCRYPTION &&
      (!qmi_uim_util_is_session_provisioning(session_type)))
  {
    UIM_MSG_ERR_1("Only support provisioning sessions for complete read encryption: 0x%x",
                  session_type);
    return QMI_ERR_INVALID_ARG;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(command_id,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }
  cb_userdata_ptr->data.read_transparent.encryption_status = encryption_status;
  cb_userdata_ptr->data.read_transparent.file_access = path_file_access;
  cb_userdata_ptr->data.read_transparent.session_type = session_type;

  /* Execute MMGSDI read transparent */
  mmgsdi_status = mmgsdi_session_read_transparent(
                        session_id,
                        file_access,
                        offset,
                        length,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_read_transparent */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_READ_RECORD()

  DESCRIPTION
    Reads the content of a cyclic or linear fixed file on the card

  PARAMETERS
    session_id   : session id
    file_access  : file to be read
    first_record : first record in the file
    last_record  : last record in the file
    length       : length to be read
    cmd_buf_ptr  : userdata
    clid         : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_read_record
(
  mmgsdi_session_id_type  session_id,
  mmgsdi_access_type      file_access,
  mmgsdi_rec_num_type     first_record,
  mmgsdi_rec_num_type     last_record,
  mmgsdi_len_type         length,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr = NULL;
  mmgsdi_session_type_enum_type  session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type        aid             = { 0, };

  /* Validate the last record */
  if (last_record < first_record ||
      first_record > QMI_UIM_MAX_RECORD_LEN ||
      last_record > QMI_UIM_MAX_RECORD_LEN)
  {
    UIM_MSG_ERR_2("Last record (0x%x) smaller than first record(0x%x)",
                  last_record, first_record);
    return QMI_ERR_INVALID_ARG;
  }

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_READ_RECORD,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Store data about records to read */
  (void)memscpy(&cb_userdata_ptr->data.read_record.file_access,
                sizeof(mmgsdi_access_type),
                &file_access,
                sizeof(mmgsdi_access_type));
  cb_userdata_ptr->data.read_record.first_record  = first_record;
  cb_userdata_ptr->data.read_record.total_records = (uint16)(last_record - first_record) + 1;
  cb_userdata_ptr->data.read_record.read_records  = 0;

  /* In case of multiple records, allocate memory for all the results */
  if (last_record > first_record)
  {
    uint32 size = cb_userdata_ptr->data.read_record.total_records *
                  sizeof(qmi_uim_additional_records_type);

    cb_userdata_ptr->data.read_record.records_cnf =
      (qmi_uim_additional_records_type *)uimqmi_malloc(size);
    if (cb_userdata_ptr->data.read_record.records_cnf == NULL)
    {
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      return QMI_ERR_NO_MEMORY;
    }
    memset(cb_userdata_ptr->data.read_record.records_cnf, 0, size);
  }

  /* Execute MMGSDI read record */
  mmgsdi_status = mmgsdi_session_read_record(
                        session_id,
                        file_access,
                        first_record,
                        length,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_read_record */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_WRITE_TRANSPARENT()

  DESCRIPTION
    Writes the content of a transparent file on the card

  PARAMETERS
    session_id  : session id
    file_access : file to be read
    record      : record in the file
    length      : length to be read
    cmd_buf_ptr : userdata
    clid        : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_write_transparent
(
  mmgsdi_session_id_type  session_id,
  mmgsdi_access_type      file_access,
  mmgsdi_offset_type      offset,
  mmgsdi_write_data_type  data,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr = NULL;
  mmgsdi_session_type_enum_type  session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type        aid             = { 0, };

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_WRITE_TRANSPARENT,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute MMGSDI write transparent */
  mmgsdi_status = mmgsdi_session_write_transparent(
                        session_id,
                        file_access,
                        offset,
                        data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_write_transparent */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_WRITE_RECORD()

  DESCRIPTION
    Writes a record on a cyclic or linear fixed file on the card

  PARAMETERS
    session_id  : session id
    file_access : file to be read
    record      : record in the file
    length      : length to be read
    cmd_buf_ptr : userdata
    clid        : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_write_record
(
  mmgsdi_session_id_type  session_id,
  mmgsdi_access_type      file_access,
  mmgsdi_rec_num_type     record,
  mmgsdi_write_data_type  data,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type                 errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type          mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_file_structure_enum_type  file_type       = MMGSDI_LINEAR_FIXED_FILE;
  qmi_uimi_userdata_type*          cb_userdata_ptr = NULL;
  mmgsdi_session_type_enum_type    session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type          aid             = { 0, };

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_WRITE_RECORD,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Duplicate the details of operation */
  (void)memscpy(&cb_userdata_ptr->data.write_record.file_access,
                sizeof(mmgsdi_access_type),
                &file_access,
                sizeof(mmgsdi_access_type));
  cb_userdata_ptr->data.write_record.record        = record;
  cb_userdata_ptr->data.write_record.data.data_len = data.data_len;
  uimqmi_deep_copy((void**)&cb_userdata_ptr->data.write_record.data.data_ptr,
                   (void*)data.data_ptr,
                   (uint32)data.data_len);

  /* Verify the data was correctly deep-copied */
  if (cb_userdata_ptr->data.write_record.data.data_ptr == NULL)
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    cb_userdata_ptr = NULL;
    return QMI_ERR_NO_MEMORY;
  }

  /* Check if the file format is known */
  if (qmi_uim_check_cached_record_type(session_id,
                                       file_access,
                                       &file_type))
  {
    UIM_MSG_HIGH_1("Write record: type is known (0x%x)", file_type);

    /* Execute MMGSDI write record */
    mmgsdi_status = mmgsdi_session_write_record(
                          session_id,
                          file_access,
                          file_type,
                          record,
                          data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
  }
  else
  {
    UIM_MSG_HIGH_0("Write record: type unknown, get file atteributes first");

    /* Execute MMGSDI get file attributes */
    mmgsdi_status = mmgsdi_session_get_file_attr(
                        session_id,
                        file_access,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_write_record */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_GET_FILE_ATTRIBUTES()

  DESCRIPTION
    Get file attributes of a file on the card

  PARAMETERS
    session_id  : session id
    file_access : file to be read
    record      : record in the file
    length      : length to be read
    cmd_buf_ptr : userdata
    clid        : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_get_file_attributes
(
  mmgsdi_session_id_type  session_id,
  mmgsdi_access_type      file_access,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr = NULL;
  mmgsdi_session_type_enum_type  session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type        aid             = { 0, };

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  /* Populate access parameters in case of DF or MF */
  if(file_access.access_method == MMGSDI_BY_PATH_ACCESS &&
     file_access.file.path_type.path_len <= MMGSDI_MAX_PATH_LEN &&
     file_access.file.path_type.path_len > 1 &&
     file_access.file.path_type.path_buf[file_access.file.path_type.path_len - 1] == 0xFFFF)
  {
    file_access.access_method = MMGSDI_DF_ENUM_ACCESS;

    switch(file_access.file.path_type.path_buf[file_access.file.path_type.path_len - 2])
    {
      case 0x3F00:
        file_access.file.df_enum = MMGSDI_MF_DIR;
        break;
      case 0x7F20:
        file_access.file.df_enum = MMGSDI_GSM_DIR;
        break;
      case 0x7F10:
        file_access.file.df_enum = MMGSDI_TELECOM_DIR;
        break;
      case 0x7FFF:
        file_access.file.df_enum = MMGSDI_LAST_SEL_ADF_DIR;
        break;
      case 0x7F21:
        file_access.file.df_enum = MMGSDI_DCS1800_DIR;
        break;
      case 0x7F25:
        file_access.file.df_enum = MMGSDI_RUIM_DIR;
        break;
      default:
        UIM_MSG_ERR_1("Invalid MF or DF (0x%x) in file access path",
                      file_access.file.path_type.path_buf[file_access.file.path_type.path_len - 2]);
        return QMI_ERR_INTERNAL;
    }
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_FILE_ATTRIBUTES,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Copy file path into the userdata */
  (void)memscpy(&cb_userdata_ptr->data.get_file_attributes.file_access,
                sizeof(mmgsdi_access_type),
                &file_access,
                sizeof(mmgsdi_access_type));

  /* Execute MMGSDI get file attributes */
  mmgsdi_status = mmgsdi_session_get_file_attr(
                        session_id,
                        file_access,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_get_file_attributes */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_SET_PIN_PROTECTION()

  DESCRIPTION
    Enable / Disable the pin on the card

  PARAMETERS
    session_id   : session id
    pin_enable   : indicate if PIN needs to be enabled or disabled
    pin_id       : PIN id
    pin_data     : PIN value
    cmd_buf_ptr  : userdata
    clid         : client id of the control point

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_set_pin_protection
(
  mmgsdi_session_id_type  session_id,
  boolean                 pin_enable,
  mmgsdi_pin_enum_type    pin_id,
  mmgsdi_data_type        pin_data,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type          errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type   mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*   cb_userdata_ptr = NULL;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SET_PIN_PROTECTION,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute MMGSDI function (enable or disable) */
  if (pin_enable)
  {
    mmgsdi_status = mmgsdi_session_enable_pin(
                        session_id,
                        pin_id,
                        pin_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

    /* Store the PIN to encrypt it later */
    if (qmi_uim_global_ptr->silent_pin1_supported &&
        mmgsdi_status == MMGSDI_SUCCESS &&
        pin_id == MMGSDI_PIN1 &&
        pin_data.data_len > 0 &&
        pin_data.data_len <= QMI_UIM_PIN_MAX_V01 &&
        pin_data.data_ptr != NULL)
    {
      cb_userdata_ptr->data.pin_operation.pin1_length = pin_data.data_len;
      (void)memscpy(cb_userdata_ptr->data.pin_operation.pin1_value,
                    sizeof(cb_userdata_ptr->data.pin_operation.pin1_value),
                    pin_data.data_ptr,
                    pin_data.data_len);
    }
  }
  else
  {
    mmgsdi_status = mmgsdi_session_disable_pin(
                        session_id,
                        pin_id,
                        MMGSDI_PIN_NOT_REPLACED_BY_UNIVERSAL,
                        pin_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_set_pin_protection */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_VERIFY_PIN()

  DESCRIPTION
    Verify the PIN

  PARAMETERS
    session_id   : session id
    pin_id       : PIN id
    pin_data     : PIN value
    is_encrypted : PIN value is encrypted or not
    cmd_buf_ptr  : userdata
    clid         : client id of the control point

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_verify_pin
(
  mmgsdi_session_id_type  session_id,
  mmgsdi_pin_enum_type    pin_id,
  mmgsdi_data_type        pin_data,
  boolean                 is_encrypted,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type          errval                = QMI_ERR_NONE;
  mmgsdi_return_enum_type   mmgsdi_status         = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*   cb_userdata_ptr       = NULL;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Perform a check on the PIN value */
  if (!qmi_uim_check_pin_digits(pin_data))
  {
    return QMI_ERR_INCORRECT_PIN;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_VERIFY_PIN,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute MMGSDI verify PIN */
  mmgsdi_status = mmgsdi_session_verify_pin(
                        session_id,
                        pin_id,
                        pin_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Store the PIN to encrypt it later */
  if (qmi_uim_global_ptr->silent_pin1_supported &&
      mmgsdi_status == MMGSDI_SUCCESS &&
      pin_id == MMGSDI_PIN1 &&
      pin_data.data_len > 0 &&
      pin_data.data_len <= QMI_UIM_PIN_MAX_V01 &&
      pin_data.data_ptr != NULL &&
      !is_encrypted)
  {
    cb_userdata_ptr->data.pin_operation.pin1_length = pin_data.data_len;
    (void)memscpy(cb_userdata_ptr->data.pin_operation.pin1_value,
                  sizeof(cb_userdata_ptr->data.pin_operation.pin1_value),
                  pin_data.data_ptr,
                  pin_data.data_len);
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_verify_pin */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_UNBLOCK_PIN()

  DESCRIPTION
    Unblocks the PIN

  PARAMETERS
    session_id   : session id
    pin_id       : PIN id
    puk_data     : PUK value
    pin_data     : new PIN value
    cmd_buf_ptr  : userdata
    clid         : client id of the control point

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_unblock_pin
(
  mmgsdi_session_id_type  session_id,
  mmgsdi_pin_enum_type    pin_id,
  mmgsdi_data_type        puk_data,
  mmgsdi_data_type        pin_data,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type          errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type   mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*   cb_userdata_ptr = NULL;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_UNBLOCK_PIN,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute MMGSDI unblock PIN */
  mmgsdi_status = mmgsdi_session_unblock_pin(
                        session_id,
                        pin_id,
                        puk_data,
                        pin_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Store the PIN to encrypt it later */
  if (qmi_uim_global_ptr->silent_pin1_supported &&
      mmgsdi_status == MMGSDI_SUCCESS &&
      pin_id == MMGSDI_PIN1 &&
      pin_data.data_len > 0 &&
      pin_data.data_len <= QMI_UIM_PIN_MAX_V01 &&
      pin_data.data_ptr != NULL)
  {
    cb_userdata_ptr->data.pin_operation.pin1_length = pin_data.data_len;
    (void)memscpy(cb_userdata_ptr->data.pin_operation.pin1_value,
                  sizeof(cb_userdata_ptr->data.pin_operation.pin1_value),
                  pin_data.data_ptr,
                  pin_data.data_len);
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_unblock_pin */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_CHANGE_PIN()

  DESCRIPTION
    Changes the PIN

  PARAMETERS
    session_id   : session id
    pin_id       : PIN id
    old_pin_data : old PIN value
    new_pin_data : new PIN value
    cmd_buf_ptr  : userdata
    clid         : client id of the control point

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_change_pin
(
  mmgsdi_session_id_type  session_id,
  mmgsdi_pin_enum_type    pin_id,
  mmgsdi_data_type        old_pin_data,
  mmgsdi_data_type        new_pin_data,
  const uint32          * token_ptr,
  void *                  cmd_buf_ptr,
  uint8                   clid
)
{
  qmi_error_e_type          errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type   mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*   cb_userdata_ptr = NULL;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_CHANGE_PIN,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute MMGSDI change PIN */
  mmgsdi_status = mmgsdi_session_change_pin(
                        session_id,
                        pin_id,
                        old_pin_data,
                        new_pin_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Store the PIN to encrypt it later */
  if (qmi_uim_global_ptr->silent_pin1_supported &&
      mmgsdi_status == MMGSDI_SUCCESS &&
      pin_id == MMGSDI_PIN1 &&
      new_pin_data.data_len > 0 &&
      new_pin_data.data_len <= QMI_UIM_PIN_MAX_V01 &&
      new_pin_data.data_ptr != NULL)
  {
    cb_userdata_ptr->data.pin_operation.pin1_length = new_pin_data.data_len;
    (void)memscpy(cb_userdata_ptr->data.pin_operation.pin1_value,
                  sizeof(cb_userdata_ptr->data.pin_operation.pin1_value),
                  new_pin_data.data_ptr,
                  new_pin_data.data_len);
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_change_pin */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_AUTHENTICATE()

  DESCRIPTION
    Executes the authentication algorithm on the card

  PARAMETERS
    session_id      : session id
    auth_context    : authentication context
    authenticate    : data for authenticate command
    cmd_buf_ptr     : userdata
    clid            : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_authenticate
(
  mmgsdi_session_id_type    session_id,
  uim_auth_context_enum_v01 auth_context,
  mmgsdi_data_type          authenticate_data,
  const uint32            * token_ptr,
  void *                    cmd_buf_ptr,
  uint8                     clid
)
{
  qmi_error_e_type                   errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type            mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*            cb_userdata_ptr = NULL;
  mmgsdi_session_type_enum_type      session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  qmi_error_e_type                   result          = QMI_ERR_INTERNAL;
  mmgsdi_static_data_type            aid;

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));

  /* Get session type from Session Id*/
  result = qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id);
  if(result != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_0("Error converting session id into session type");
    return result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_AUTHENTICATE,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Update the context */
  cb_userdata_ptr->data.authenticate.auth_context = auth_context;

  /* Execute MMGSDI authenticate */
  switch(auth_context)
  {
    case UIM_AUTH_CONTEXT_RUN_GSM_ALG_V01:
      mmgsdi_status = mmgsdi_session_run_gsm_algo(
                        session_id,
                        authenticate_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );
      break;

    case UIM_AUTH_CONTEXT_RUN_CAVE_ALG_V01:
#ifdef FEATURE_MMGSDI_3GPP2
      if(authenticate_data.data_len == QMI_UIM_CAVE_AUTH_DATA_LEN &&
         authenticate_data.data_ptr != NULL)
      {
        mmgsdi_cave_rand_type_type         rand_type       = 0;
        mmgsdi_cave_rand_chal_type         rand_chal       = 0;
        mmgsdi_cave_dig_len_type           dig_len         = 0;
        mmgsdi_cave_digits_type            digits          = {0};
        mmgsdi_cave_process_control_type   process_control = 0;
        mmgsdi_cave_esn_type               esn             = {0};

        /* Parse parameters for CAVE algorithm as per 3GPP2 C.S0023-C Section 4 */
        rand_type = (mmgsdi_cave_rand_type_type)authenticate_data.data_ptr[0];
        rand_chal = (mmgsdi_cave_rand_chal_type)((uint32)rand_chal |
                                                 authenticate_data.data_ptr[1]);
        rand_chal = (mmgsdi_cave_rand_chal_type)((uint32)rand_chal << 8);
        rand_chal = (mmgsdi_cave_rand_chal_type)((uint32)rand_chal |
                                                 authenticate_data.data_ptr[2]);
        rand_chal = (mmgsdi_cave_rand_chal_type)((uint32)rand_chal << 8);
        rand_chal = (mmgsdi_cave_rand_chal_type)((uint32)rand_chal |
                                                 authenticate_data.data_ptr[3]);
        rand_chal = (mmgsdi_cave_rand_chal_type)((uint32)rand_chal << 8);
        rand_chal = (mmgsdi_cave_rand_chal_type)((uint32)rand_chal |
                                                 authenticate_data.data_ptr[4]);

        dig_len   = (mmgsdi_cave_dig_len_type)authenticate_data.data_ptr[5];
        digits[0] = authenticate_data.data_ptr[6];
        digits[1] = authenticate_data.data_ptr[7];
        digits[2] = authenticate_data.data_ptr[8];

        process_control = (mmgsdi_cave_process_control_type)authenticate_data.data_ptr[9];

        esn[0] = esn[0] | authenticate_data.data_ptr[10];
        esn[0] = esn[0] << 8;
        esn[0] = esn[0] | authenticate_data.data_ptr[11];
        esn[0] = esn[0] << 8;
        esn[0] = esn[0] | authenticate_data.data_ptr[12];
        esn[1] = esn[1] | authenticate_data.data_ptr[13];
        esn[1] = esn[1] << 8;
        esn[1] = esn[1] | authenticate_data.data_ptr[14];
        esn[1] = esn[1] << 8;
        esn[1] = esn[1] | authenticate_data.data_ptr[15];
        esn[1] = esn[1] << 8;
        esn[1] = esn[1] | authenticate_data.data_ptr[16];

        mmgsdi_status = mmgsdi_session_run_cave_algo(
                          session_id,
                          rand_type,
                          rand_chal,
                          dig_len,
                          digits,
                          process_control,
                          esn,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        UIM_MSG_ERR_1("Incorrect auth_data len : 0x%x",
                      authenticate_data.data_len);
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      }
#else
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
#endif/* FEATURE_MMGSDI_3GPP2 */
      break;

    case UIM_AUTH_CONTEXT_GSM_SEC_V01:
      mmgsdi_status = mmgsdi_session_usim_authenticate(
                        session_id,
                        MMGSDI_AUTN_USIM_GSM_CONTEXT,
                        authenticate_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );
      break;

    case UIM_AUTH_CONTEXT_3G_SEC_V01:
      mmgsdi_status = mmgsdi_session_usim_authenticate(
                        session_id,
                        MMGSDI_AUTN_USIM_3G_CONTEXT,
                        authenticate_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );
      break;

    case UIM_AUTH_CONTEXT_IMS_AKA_SEC_V01:
      mmgsdi_status = mmgsdi_session_isim_authenticate(
                        session_id,
                        authenticate_data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );
      break;

    case UIM_AUTH_CONTEXT_GBA_SEC_BOOTSTRAPPING_V01:
      if(session_type == MMGSDI_GW_PROV_PRI_SESSION ||
         session_type == MMGSDI_GW_PROV_SEC_SESSION ||
         session_type == MMGSDI_GW_PROV_TER_SESSION ||
         qmi_uim_is_usim_aid(&aid, FALSE))
      {
        mmgsdi_status = mmgsdi_session_uicc_authenticate(
                          session_id,
                          MMGSDI_AUTN_USIM_GBA_BOOTSTRAPPING_CONTEXT,
                          authenticate_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else if(qmi_uim_is_isim_aid(&aid))
      {
        mmgsdi_status = mmgsdi_session_uicc_authenticate(
                          session_id,
                          MMGSDI_AUTN_ISIM_GBA_BOOTSTRAPPING_CONTEXT,
                          authenticate_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;

    case UIM_AUTH_CONTEXT_GBA_SEC_NAF_DERIVATION_V01:
      if(session_type == MMGSDI_GW_PROV_PRI_SESSION ||
         session_type == MMGSDI_GW_PROV_SEC_SESSION ||
         session_type == MMGSDI_GW_PROV_TER_SESSION ||
         qmi_uim_is_usim_aid(&aid, FALSE))
      {
        mmgsdi_status = mmgsdi_session_uicc_authenticate(
                          session_id,
                          MMGSDI_AUTN_USIM_GBA_NAF_DERIVATION_CONTEXT,
                          authenticate_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else if(qmi_uim_is_isim_aid(&aid))
      {
        mmgsdi_status = mmgsdi_session_uicc_authenticate(
                          session_id,
                          MMGSDI_AUTN_ISIM_GBA_NAF_DERIVATION_CONTEXT,
                          authenticate_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;

    case UIM_AUTH_CONTEXT_COMPUTE_IP_CHAP_V01:
#ifdef FEATURE_MMGSDI_3GPP2
      if(authenticate_data.data_len > 2 &&
         authenticate_data.data_len <= MMGSDI_MAX_3GPD_CHAP_CHALLENGE_LENGTH + 2 &&
         authenticate_data.data_ptr != NULL)
      {
        mmgsdi_compute_ip_data_type compute_ip_data;
        memset(&compute_ip_data, 0, sizeof(mmgsdi_compute_ip_data_type));

        /* Parse parameters as per 3GPP2 C.S0023-D Section 4.8.1 */
        compute_ip_data.operation_type = MMGSDI_CMPT_IP_SIP_CHAP;
        compute_ip_data.data.sip_chap_data.chap_id =
          authenticate_data.data_ptr[0];
        compute_ip_data.data.sip_chap_data.nai_entry_index =
          authenticate_data.data_ptr[1];
        compute_ip_data.data.sip_chap_data.chap_challenge_length =
          (mmgsdi_len_type)(authenticate_data.data_len - 2);

        (void)memscpy(compute_ip_data.data.sip_chap_data.chap_challenge,
                      sizeof(compute_ip_data.data.sip_chap_data.chap_challenge),
                      &authenticate_data.data_ptr[2],
                      authenticate_data.data_len - 2);

        mmgsdi_status = mmgsdi_session_compute_ip_auth(
                          session_id,
                          compute_ip_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        UIM_MSG_ERR_1("Incorrect auth_data len : 0x%x",
                      authenticate_data.data_len);
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      }
#else
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
#endif/* FEATURE_MMGSDI_3GPP2 */
      break;

    case UIM_AUTH_CONTEXT_COMPUTE_IP_MN_HA_V01:
#ifdef FEATURE_MMGSDI_3GPP2
      if(authenticate_data.data_len > 1 &&
         authenticate_data.data_len <= MMGSDI_MAX_3GPD_MN_HA_REG_DATA_LENGTH + 1 &&
         authenticate_data.data_ptr != NULL)
      {
        mmgsdi_compute_ip_data_type compute_ip_data;
        memset(&compute_ip_data, 0, sizeof(mmgsdi_compute_ip_data_type));

        /* Parse parameters as per 3GPP2 C.S0023-D Section 4.8.1 */
        compute_ip_data.operation_type = MMGSDI_CMPT_IP_MIP_MN_HA;
        compute_ip_data.data.mn_ha_data.nai_entry_index =
          authenticate_data.data_ptr[0];
        compute_ip_data.data.mn_ha_data.registration_data_length =
          (mmgsdi_len_type)(authenticate_data.data_len - 1);
        compute_ip_data.data.mn_ha_data.registration_data_ptr =
          &authenticate_data.data_ptr[1];

        mmgsdi_status = mmgsdi_session_compute_ip_auth(
                          session_id,
                          compute_ip_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        UIM_MSG_ERR_1("Incorrect auth_data len : 0x%x",
                      authenticate_data.data_len);
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      }
#else
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
#endif/* FEATURE_MMGSDI_3GPP2 */
      break;

    case UIM_AUTH_CONTEXT_COMPUTE_IP_MIP_RRQ_HASH_V01:
#ifdef FEATURE_MMGSDI_3GPP2
      if(authenticate_data.data_len > 0 &&
         authenticate_data.data_len <= MMGSDI_MAX_3GPD_HASH_RRQ_DATA_LENGTH &&
         authenticate_data.data_ptr != NULL)
      {
        mmgsdi_compute_ip_data_type compute_ip_data;
        memset(&compute_ip_data, 0, sizeof(mmgsdi_compute_ip_data_type));

        /* Parse parameters as per 3GPP2 C.S0023-D Section 4.8.1 */
        compute_ip_data.operation_type = MMGSDI_CMPT_IP_MIP_RRQ;
        compute_ip_data.data.rrq_data.rrq_data_length =
          (mmgsdi_len_type)authenticate_data.data_len;
        compute_ip_data.data.rrq_data.rrq_data_ptr =
          &authenticate_data.data_ptr[0];

        mmgsdi_status = mmgsdi_session_compute_ip_auth(
                          session_id,
                          compute_ip_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        UIM_MSG_ERR_1("Incorrect auth_data len : 0x%x",
                      authenticate_data.data_len);
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      }
#else
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
#endif/* FEATURE_MMGSDI_3GPP2 */
      break;

    case UIM_AUTH_CONTEXT_COMPUTE_IP_MN_AAA_V01:
#ifdef FEATURE_MMGSDI_3GPP2
      if(authenticate_data.data_len > 1 &&
         authenticate_data.data_len <= MMGSDI_3GPD_MAX_MIP_CHALLENGE_LENGTH + 1 &&
         authenticate_data.data_ptr != NULL)
      {
        mmgsdi_compute_ip_data_type compute_ip_data;
        memset(&compute_ip_data, 0, sizeof(mmgsdi_compute_ip_data_type));

        /* Parse parameters as per 3GPP2 C.S0023-D Section 4.8.1 */
        compute_ip_data.operation_type = MMGSDI_CMPT_IP_MIP_MN_AAA;
        compute_ip_data.data.mn_aaa_data.nai_entry_index =
          authenticate_data.data_ptr[0];
        compute_ip_data.data.mn_aaa_data.challenge_length =
          (mmgsdi_len_type)(authenticate_data.data_len - 1);
        compute_ip_data.data.mn_aaa_data.challenge_ptr =
          &authenticate_data.data_ptr[1];

        mmgsdi_status = mmgsdi_session_compute_ip_auth(
                          session_id,
                          compute_ip_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        UIM_MSG_ERR_1("Incorrect auth_data len : 0x%x",
                      authenticate_data.data_len);
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      }
#else
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
#endif/* FEATURE_MMGSDI_3GPP2 */
      break;

    case UIM_AUTH_CONTEXT_COMPUTE_IP_HRPD_ACCESS_V01:
#ifdef FEATURE_MMGSDI_3GPP2
      if(authenticate_data.data_len > 1 &&
         authenticate_data.data_len <= MMGSDI_MAX_HRPD_CHAP_CHALLENGE_LENGTH + 1 &&
         authenticate_data.data_ptr != NULL)
      {
        mmgsdi_compute_ip_data_type compute_ip_data;
        memset(&compute_ip_data, 0, sizeof(mmgsdi_compute_ip_data_type));

        /* Parse parameters as per 3GPP2 C.S0023-D Section 4.8.1 */
        compute_ip_data.operation_type = MMGSDI_CMPT_IP_HRPD_CHAP;
        compute_ip_data.data.hrpd_chap_data.chap_id = authenticate_data.data_ptr[0];
        compute_ip_data.data.hrpd_chap_data.chap_challenge_length =
          (mmgsdi_len_type)(authenticate_data.data_len - 1);
        compute_ip_data.data.hrpd_chap_data.chap_challenge_ptr =
          &authenticate_data.data_ptr[1];

        mmgsdi_status = mmgsdi_session_compute_ip_auth(
                          session_id,
                          compute_ip_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      }
      else
      {
        UIM_MSG_ERR_1("Incorrect auth_data len : 0x%x",
                      authenticate_data.data_len);
        mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      }
#else
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
#endif/* FEATURE_MMGSDI_3GPP2 */
      break;

    case UIM_AUTH_CONTEXT_HTTP_DIGEST_SEC_V01:
      mmgsdi_status = mmgsdi_session_uicc_authenticate(
                          session_id,
                          MMGSDI_AUTN_ISIM_HTTP_DIGEST_CONTEXT,
                          authenticate_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );
      break;

    default:
      UIM_MSG_ERR_1("Unsupported security context: 0x%x", auth_context);
      mmgsdi_status = MMGSDI_NOT_SUPPORTED;
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_authenticate */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_REFRESH_REGISTER()

  DESCRIPTION
    Registers for refresh

  PARAMETERS
    session_id        : session id
    register_op       : registration or deregistration operation
    vote_for_init     : vote for init
    list_of_files_ptr : list of files
    cmd_buf_ptr       : userdata

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_refresh_register
(
  mmgsdi_session_id_type       session_id,
  boolean                      register_op,
  boolean                      vote_for_init,
  qmi_uim_files_list_type *    list_of_files_ptr,
  qmi_cmd_buf_type *           cmd_buf_ptr
)
{
  uint16                         refresh_index     = 0;
  uint32                         notify_mask       = 0;
  mmgsdi_return_enum_type        mmgsdi_status     = MMGSDI_SUCCESS;
  qmi_uimi_client_state_type *   client_sp         = NULL;
  qmi_error_e_type               errval            = QMI_ERR_NONE;
  mmgsdi_refresh_file_list_type  register_list     = { 0, NULL, NULL, 0};
  mmgsdi_refresh_file_list_type  deregister_list   = { 0, NULL, NULL, 0};

  /* Validate the command buffer to make sure the client is still there */
  ASSERT(cmd_buf_ptr);
  if (cmd_buf_ptr->x_p == NULL || cmd_buf_ptr->x_p->cl_sp == NULL)
  {
    UIM_MSG_HIGH_0("Transaction ptr or client state pointer is NULL");
    return QMI_ERR_INTERNAL;
  }

  /* Extract client sp */
  client_sp = (qmi_uimi_client_state_type *) cmd_buf_ptr->x_p->cl_sp;

  /* In case of deregistration from events, set forcely vote_for_init to
     FALSE and number of files to 0 and continue normally. In this way,
     files will be removed from MMGSDI */
  if (register_op == FALSE)
  {
    vote_for_init = FALSE;
    list_of_files_ptr = NULL;
  }

  /* Calculate the refresh index */
  errval = qmi_uim_util_session_id_to_index(&refresh_index, session_id);
  if (errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                  session_id);
    goto return_result;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  /* Calculate the refresh mask */
  notify_mask = 1 << refresh_index;

  /* Store the indication on refresh events */
  if (register_op)
  {
    client_sp->uim_info.refresh_notify_mask = (client_sp->uim_info.refresh_notify_mask | notify_mask);
  }
  else
  {
    client_sp->uim_info.refresh_notify_mask = (client_sp->uim_info.refresh_notify_mask & (~notify_mask));
  }

  /* Create list of files to deregister for refresh. This list
     is composed by the files that only this client is monitoring
     and are not in the new list */
  errval = qmi_uim_refresh_create_deregister_list(
              client_sp,
              session_id,
              list_of_files_ptr,
              &deregister_list);
  if (errval != QMI_ERR_NONE)
  {
    goto return_result;
  }

  /* Create list of files to register for refresh. This list
     is composed by the files that are in the new list and that
     no client is currently monitoring */
  errval = qmi_uim_refresh_create_register_list(
              session_id,
              list_of_files_ptr,
              &register_list);
  if (errval != QMI_ERR_NONE)
  {
    goto return_result;
  }

  /* Remove old list of files of this client for this session type */
  errval = qmi_uim_remove_client_refresh_files(
                client_sp,
                session_id);
  if (errval != QMI_ERR_NONE)
  {
    goto return_result;
  }

  /* Copy new list of files into this client */
  errval = qmi_uim_add_client_refresh_files(
              client_sp,
              session_id,
              list_of_files_ptr);
  if (errval != QMI_ERR_NONE)
  {
    goto return_result;
  }

  /* Store the vote for init flag: this is done at the end, after verifying that
     lists of files could be handled successfully. */
  client_sp->uim_info.refresh_vote = vote_for_init;

  /* Deregister for refresh. Ignore errors in this commands because if a file is
     not correctly deregistered, nothing happens. */
  if (deregister_list.file_path_len > 0 && deregister_list.file_path_ptr != NULL)
  {
    mmgsdi_status = mmgsdi_session_deregister_for_refresh(
                          session_id,
                          deregister_list,
                          FALSE,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)NULL );
    UIM_MSG_HIGH_1("Deregister for refresh: 0x%x", mmgsdi_status);
    /* Ignore the error in the deregistration.... if this fails, it is not
       critical and we can continue */
  }
  else
  {
    UIM_MSG_MED_0("Deregister for refresh: no file to deregister");
  }

  /* Register for refresh. Passing NULL as userdata because we're sending
     the response immediately, without waiting the callback from MMGSDI. */
  if (register_list.file_path_len > 0 && register_list.file_path_ptr != NULL)
  {
    mmgsdi_status = mmgsdi_session_register_for_refresh(
                          session_id,
                          register_list,
                          TRUE,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)NULL );
  }
  else if (register_op)
  {
    /* We still need to register for refresh in order to receive INIT events
       if incoming req is for registration. Pass ICCID as a fake file. */
    uint8                          file_iccid[]       = { 0x3F, 0x00, 0x2F, 0xE2 };
    mmgsdi_refresh_file_list_type  fake_register_list = { 0, NULL, NULL, 0};

    fake_register_list.file_path_len = sizeof(file_iccid);
    fake_register_list.file_path_ptr = file_iccid;

    mmgsdi_status = mmgsdi_session_register_for_refresh(
                          session_id,
                          fake_register_list,
                          TRUE,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)NULL );
  }

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

return_result:
  /* Free allocated memory */
  if (register_list.file_path_ptr)
  {
    uimqmi_free(register_list.file_path_ptr);
    register_list.file_path_ptr = NULL;
  }
  if (deregister_list.file_path_ptr)
  {
    uimqmi_free(deregister_list.file_path_ptr);
    deregister_list.file_path_ptr = NULL;
  }

  return errval;
} /* qmi_uim_execute_refresh_register */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_FDN_ACL_STATUS()

  DESCRIPTION
    Gets the FDN and ACL service status

  PARAMETERS
    session_type        : session type
    cmd_buf_ptr         : userdata
    clid                : client id of the control point
    service_type        : Service that needs to be checked

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_get_fdn_acl_status
(
  mmgsdi_session_type_enum_type  session_type,
  void *                         cmd_buf_ptr,
  uint8                          clid,
  qmi_uim_service_enum_type      service_type
)
{
  qmi_error_e_type            errval           = QMI_ERR_NONE;
  mmgsdi_session_id_type      session_id       = UIM_INVALID_SESSION_ID;
  mmgsdi_return_enum_type     mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_app_enum_type        card_type        = MMGSDI_APP_NONE;
  mmgsdi_service_enum_type    service_id       = MMGSDI_NONE;
  qmi_uimi_userdata_type   *  cb_userdata_ptr  = NULL;
  mmgsdi_static_data_type     aid;

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));

  /* Only provisioning sessions are supported */
  if (!qmi_uim_util_is_session_provisioning(session_type))
  {
    UIM_MSG_ERR_1("Impossible to get service status for a non provisioning session: 0x%x",
                  session_type);
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   session_type,
                                                   aid);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  /* Retrieve card type */
  errval = qmi_uim_util_session_type_to_card_type(
                           &card_type,
                           session_type,
                           aid);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  if (service_type == QMI_UIM_SERVICE_ACL)
  {
    if (card_type != MMGSDI_APP_USIM)
    {
      return QMI_ERR_INTERNAL;
    }
    else
    {
      service_id = MMGSDI_USIM_SRV_ACL;
    }
  }
  else if (service_type == QMI_UIM_SERVICE_FDN)
  {
    switch(card_type)
    {
      case MMGSDI_APP_SIM:
        service_id = MMGSDI_GSM_SRV_FDN;
        break;
      case MMGSDI_APP_RUIM:
        service_id = MMGSDI_CDMA_SRV_FDN;
        break;
      case MMGSDI_APP_USIM:
        service_id = MMGSDI_USIM_SRV_FDN;
        break;
      case MMGSDI_APP_CSIM:
        service_id = MMGSDI_CSIM_SRV_FDN;
        break;
      default:
        return QMI_ERR_INTERNAL;
    }
  }
  else
  {
    return QMI_ERR_INTERNAL;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_SERVICE_STATUS,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  cb_userdata_ptr->data.get_service_status.service_id = service_id;

  /* Get service status from MMGSDI */
  mmgsdi_status = mmgsdi_session_is_service_available(
                                session_id,
                                service_id,
                                qmi_uimi_mmgsdi_callback,
                                (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_get_fdn_acl_status */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_GET_STATUS_HIDDEN_KEY_AND_INDEX()

  DESCRIPTION
    Gets the hidden key status

  PARAMETERS
    hidden_key          : hidden key is required
    index_in_dir        : index in dir is required
    session_type        : session type
    cmd_buf_ptr         : userdata
    clid                : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_get_status_hidden_key_and_index
(
  boolean                        hidden_key,
  boolean                        index_in_dir,
  mmgsdi_session_type_enum_type  session_type,
  void *                         cmd_buf_ptr,
  uint8                          clid
)
{
  qmi_error_e_type            errval           = QMI_ERR_NONE;
  mmgsdi_session_id_type      session_id       = UIM_INVALID_SESSION_ID;
  mmgsdi_return_enum_type     mmgsdi_status    = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type   *  cb_userdata_ptr  = NULL;
  mmgsdi_static_data_type     aid;

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));

  /* Verify that at least one parameter is requested */
  if (hidden_key == FALSE && index_in_dir == FALSE)
  {
    return QMI_ERR_INTERNAL;
  }

  /* Only provisioning sessions are supported */
  if (!qmi_uim_util_is_session_provisioning(session_type))
  {
    UIM_MSG_ERR_1("Impossible to get hidden key for a non provisioning session: 0x%x",
                  session_type);
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   session_type,
                                                   aid);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_SERVICE_STATUS,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Set values in userdata */
  cb_userdata_ptr->data.get_service_status.hidden_key   = hidden_key;
  cb_userdata_ptr->data.get_service_status.index_in_dir = index_in_dir;

  /* Get service status from MMGSDI */
  mmgsdi_status = mmgsdi_session_get_all_pin_status(
                                session_id,
                                qmi_uimi_mmgsdi_callback,
                                (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_get_status_hidden_key_and_index */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_GET_STATUS_ESN()

  DESCRIPTION
    Gets the ESN status

  PARAMETERS
    session_type        : session type
    esn_status_ptr      : Output parameter with ESN status

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_get_status_esn
(
  mmgsdi_session_type_enum_type  session_type,
  mmgsdi_esn_me_change_enum_type *esn_status_ptr
)
{
  qmi_error_e_type                   errval       = QMI_ERR_NONE;
  mmgsdi_session_id_type             session_id   = UIM_INVALID_SESSION_ID;
  mmgsdi_esn_me_change_enum_type     esn_status   = MMGSDI_ESN_ME_NO_CHANGE;
  mmgsdi_slot_id_enum_type           slot         = MMGSDI_SLOT_1;
  mmgsdi_static_data_type            aid;

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));

  /* Only 1x provisioning sessions are supported */
  if ((session_type != MMGSDI_1X_PROV_PRI_SESSION) &&
      (session_type != MMGSDI_1X_PROV_SEC_SESSION))
  {
    UIM_MSG_ERR_1("ESN status request is invalid for non-1x provisioning"
                  " session: 0x%x", session_type);
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   session_type,
                                                   aid);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  /* Retrieve the slot */
  errval = qmi_uim_util_session_id_to_slot(&slot, session_id);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  /* Get ESN status from MMGSDI */
  esn_status = mmgsdi_get_esn_me_change_flag(slot);
  if (esn_status == MMGSDI_ESN_ME_CHANGE_NOT_SUPPORTED ||
      esn_status == MMGSDI_ESN_ME_UNKNOWN)
  {
    return QMI_ERR_INTERNAL;
  }
  else
  {
    *esn_status_ptr = esn_status;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_execute_get_status_esn */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_GET_SERVICE_STATUS()

  DESCRIPTION
    Gets the service availability status

  PARAMETERS
    session_type          : session type
    service_number        : service number per the spec
    service_available_ptr : service available or not

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_get_service_status
(
  mmgsdi_session_type_enum_type  session_type,
  uint8                          service_number,
  boolean                       *service_available_ptr
)
{
  qmi_error_e_type            errval           = QMI_ERR_NONE;
  mmgsdi_session_id_type      session_id       = UIM_INVALID_SESSION_ID;
  mmgsdi_return_enum_type     mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_app_enum_type        card_type        = MMGSDI_APP_NONE;
  mmgsdi_static_data_type     aid;
  mmgsdi_data_type            service_table_data;
  mmgsdi_file_enum_type       file_name        = MMGSDI_MAX_FILE_ENUM;

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));
  memset(&service_table_data, 0, sizeof(mmgsdi_data_type));

  ASSERT(service_available_ptr);

  *service_available_ptr = FALSE;

  /* Service number must be greater than 0 */
  if(service_number == 0)
  {
    UIM_MSG_ERR_1("Invalid Service Request 0x%x", service_number);
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Only provisioning sessions are supported */
  if (!qmi_uim_util_is_session_provisioning(session_type))
  {
    UIM_MSG_ERR_1("Impossible to get service status for a non provisioning session: 0x%x",
                  session_type);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   session_type,
                                                   aid);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve card type */
  errval = qmi_uim_util_session_type_to_card_type(
                           &card_type,
                           session_type,
                           aid);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Calculate service id based on the MMGSDI EF service enums */
  switch(card_type)
  {
    case MMGSDI_APP_RUIM:
      file_name = MMGSDI_CDMA_SVC_TABLE;
      break;
    case MMGSDI_APP_SIM:
      file_name = MMGSDI_GSM_SST;
      break;
    case MMGSDI_APP_USIM:
      file_name = MMGSDI_USIM_UST;
      break;
    case MMGSDI_APP_CSIM:
      file_name = MMGSDI_CSIM_CST;
      break;
    default:
      UIM_MSG_ERR_1("Unsupported card type: 0x%x", card_type);
      errval = QMI_ERR_INTERNAL;
      goto send_result;
  }

  /* Execute MMGSDI cache read */
  mmgsdi_status = mmgsdi_session_read_cache_file_size(session_id,
                                                      file_name,
                                                      &service_table_data.data_len);
  /* Service table must be at least one byte long per the spec */
  if(mmgsdi_status != MMGSDI_SUCCESS || service_table_data.data_len < 1)
  {
    UIM_MSG_ERR_2("Invalid service table length. mmgsdi_status:0x%x len:0x%x",
                  mmgsdi_status, service_table_data.data_len);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  service_table_data.data_ptr = uimqmi_malloc(service_table_data.data_len);
  if(service_table_data.data_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  mmgsdi_status = mmgsdi_session_read_cache(session_id,
                                            file_name,
                                            service_table_data);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_0("Error reading service table");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  if((card_type == MMGSDI_APP_USIM ||
      card_type == MMGSDI_APP_CSIM) &&
      service_number <= service_table_data.data_len * 8)
  {
    /* We have the raw data from UICC service table. Lets check if the service number
       in question is marked as available per that raw data. for UICC, one bit is
       dedicated to each service, indicating if the service is available. As can be
       inferred from above, for ICC, each bytes contains status of 8 services hence
       the division and modulo performed by 8 below. */
    if(((service_table_data.data_ptr[(service_number - 1) / 8] >> (service_number - 1) % 8) & 0x1) == 0x1)
    {
      *service_available_ptr = TRUE;
    }
  }
  else if((card_type == MMGSDI_APP_SIM ||
           card_type == MMGSDI_APP_RUIM) &&
           service_number <= service_table_data.data_len * 4)
  {
    /* We have the raw data from ICC service table. Lets check if the service number
       in question is marked as available per that raw data. For ICC, two bits are
       dedicated to every service, one indicating if the service is available and the
       other one indicating if it is enabled. Only if both bits are set, we mark the
       service as available. As can be inferred from above, for ICC, each bytes
       contains status of 4 services hence the division and module performed by 4 below. */
    if(((service_table_data.data_ptr[(service_number - 1) / 4] >> ((service_number - 1) % 4) * 2) & 0x3) == 0x3)
    {
      *service_available_ptr = TRUE;
    }
  }

send_result:
  /* Free allocated memory */
  if (service_table_data.data_ptr)
  {
    uimqmi_free(service_table_data.data_ptr);
    service_table_data.data_ptr = NULL;
  }

  return errval;
} /* qmi_uim_execute_get_service_status */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_OPEN_SESSION_FOR_APDU()

  DESCRIPTION
    Open a non provisioning session to stream APDUs

  PARAMETERS
    slot                : slot id
    aid                 : AID of the application
    sel_rsp_type        : format of select response
    cmd_buf_ptr         : userdata
    select_mf           : If MF is to be selected after
                          opening the session
    clid                : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_open_session_for_apdu
(
  mmgsdi_slot_id_enum_type     slot,
  mmgsdi_static_data_type      aid,
  mmgsdi_select_rsp_enum_type  sel_rsp_type,
  void *                       cmd_buf_ptr,
  boolean                      select_mf,
  uint8                        clid
)
{
  uint16                           i                 = 0;
  qmi_error_e_type                 errval            = QMI_ERR_NONE;
  mmgsdi_return_enum_type          mmgsdi_status     = MMGSDI_SUCCESS;
  uint16                           channel_index     = UIM_INVALID_APP_INDEX;
  qmi_uimi_userdata_type         * cb_userdata_ptr   = NULL;
  mmgsdi_app_select_with_rsp_type  app_info;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Check command buffer before dereferencing */
  if (cmd_buf_ptr == NULL || ((qmi_cmd_buf_type *)cmd_buf_ptr)->x_p == NULL)
  {
    UIM_MSG_HIGH_0("Command buffer ptr or Transaction ptr is NULL");
    return QMI_ERR_INTERNAL;
  }

  /* Compose app info */
  memset(&app_info, 0, sizeof(mmgsdi_app_select_with_rsp_type));
  app_info.exclusive_channel    = TRUE;
  app_info.app_id_data.data_len = aid.data_len;
  app_info.app_id_data.data_ptr = aid.data_ptr;
  app_info.select_rsp_type      = sel_rsp_type;

  /* Find a free index in the table */
  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] == NULL)
    {
      /* Allocate the buffer for the channel */
      qmi_uim_global_ptr->apdu_channel[i] = (qmi_uim_apdu_channel_info_type*)uimqmi_malloc(
                                                sizeof(qmi_uim_apdu_channel_info_type));
      if (qmi_uim_global_ptr->apdu_channel[i] == NULL)
      {
        return QMI_ERR_NO_MEMORY;
      }

      channel_index = i;

      /* Initialize the buffer */
      memset(qmi_uim_global_ptr->apdu_channel[i], 0, sizeof(qmi_uim_apdu_channel_info_type));
      qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id  = UIM_INVALID_SESSION_ID;
      qmi_uim_global_ptr->apdu_channel[i]->slot               = slot;
      (void)memscpy(&qmi_uim_global_ptr->apdu_channel[i]->aid,
                    sizeof(qmi_uim_global_ptr->apdu_channel[i]->aid),
                    &aid,
                    sizeof(aid));
      qmi_uim_global_ptr->apdu_channel[i]->opening_cl_sp      =
          (qmi_uimi_client_state_type *)((qmi_cmd_buf_type *)cmd_buf_ptr)->x_p->cl_sp;
      break;
    }
  }
  if (channel_index == UIM_INVALID_APP_INDEX)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Create userdata. Note: since request id is not used in the callback,
     it is set to UIMI_CMD_VAL_OPEN_LOGICAL_CHANNEL */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_OPEN_LOGICAL_CHANNEL,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[channel_index]);
    qmi_uim_global_ptr->apdu_channel[channel_index] = NULL;
    return QMI_ERR_NO_MEMORY;
  }

  /* Update user data with necessary info */
  cb_userdata_ptr->data.session_open_request.channel_index = (uint8)channel_index;
  cb_userdata_ptr->data.session_open_request.session_open_type = UIM_SESSION_OPEN_APDU;
  switch(slot)
  {
    case MMGSDI_SLOT_1:
      cb_userdata_ptr->data.session_open_request.session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
      break;
    case MMGSDI_SLOT_2:
      cb_userdata_ptr->data.session_open_request.session_type = MMGSDI_NON_PROV_SESSION_SLOT_2;
      break;
    case MMGSDI_SLOT_3:
      cb_userdata_ptr->data.session_open_request.session_type = MMGSDI_NON_PROV_SESSION_SLOT_3;
      break;
    default:
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      uimqmi_free(qmi_uim_global_ptr->apdu_channel[channel_index]);
      qmi_uim_global_ptr->apdu_channel[channel_index] = NULL;
      return QMI_ERR_INTERNAL;
  }

  if(!select_mf)
  {
    /* Open the logical channel with exclusive priviledges */
    mmgsdi_status = mmgsdi_session_open_with_select_rsp(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        slot,
                        app_info,
                        qmi_uim_mmgsdi_evt_cback,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr);
  }
  else
  {
    /* Open the logical channel with MF selected */
    mmgsdi_status = mmgsdi_session_open_with_mf(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        slot,
                        qmi_uim_mmgsdi_evt_cback,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr);
  }

  /* Store the information for the response and set the index to init */
  if (MMGSDI_SUCCESS != mmgsdi_status)
  {
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[channel_index]);
    qmi_uim_global_ptr->apdu_channel[channel_index] = NULL;
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_open_session_for_apdu */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_CLOSE_SESSION_FOR_APDU()

  DESCRIPTION
    Close a non provisioning session to stream APDUs

  PARAMETERS
    slot                : slot id
    channel id          : channel id
    cl_sp               : client state pointer
    cmd_buf_ptr         : pointer to command buffer
    clid                : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_close_session_for_apdu
(
  mmgsdi_slot_id_enum_type     slot,
  uint8                        logical_channel,
  boolean                      terminate,
  qmi_uimi_client_state_type * cl_sp,
  void *                       cmd_buf_ptr,
  uint8                        clid
)
{
  uint16                         i                 = 0;
  qmi_error_e_type               errval            = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status     = MMGSDI_SUCCESS;
  uint16                         channel_index     = UIM_INVALID_APP_INDEX;
  qmi_uimi_userdata_type       * cb_userdata_ptr   = NULL;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Find the index */
  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->logical_channel == logical_channel &&
        qmi_uim_global_ptr->apdu_channel[i]->slot == slot)
    {
      channel_index = i;
      break;
    }
  }
  if (channel_index == UIM_INVALID_APP_INDEX)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Check if the logical channel to be closed belongs to requesting client */
  if (qmi_uim_global_ptr->apdu_sec_restr_value != QMI_UIM_APDU_SEC_RESTRICTIONS_NONE)
  {
    /* Check if incoming clid matches with the one allocated for the logical channel */
    if (qmi_uim_global_ptr->apdu_channel[channel_index]->opening_cl_sp != cl_sp)
    {
      UIM_MSG_ERR_0("ADPU rejected due to security restrictions: logical_channel doesnt belong to client");
      return QMI_ERR_ACCESS_DENIED;
    }
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_LOGICAL_CHANNEL,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Update user data with channel index */
  cb_userdata_ptr->data.session_close_request.channel_index = (uint8)channel_index;

  mmgsdi_status = mmgsdi_session_close_terminate(
                            qmi_uim_global_ptr->apdu_channel[channel_index]->mmgsdi_session_id,
                            terminate,
                            qmi_uimi_mmgsdi_callback,
                            (mmgsdi_client_data_type)cb_userdata_ptr);
  if((mmgsdi_status != MMGSDI_SUCCESS) ||
     (qmi_uim_global_ptr->close_channel_sync))
  {
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

    /* Regardless of result, free the information in the table */
    qmi_uim_remove_apdu_session(
            qmi_uim_global_ptr->apdu_channel[channel_index]->mmgsdi_session_id, TRUE,
            MMGSDI_SESSION_CLOSED_CAUSE_CLIENT_REQUEST);
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  return errval;
} /* qmi_uim_execute_close_session_for_apdu */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_REFRESH_REGISTER_ALL()

  DESCRIPTION
    Registers for refresh

  PARAMETERS
    session_id        : session id
    register_op       : registration or deregistration operation
    vote_for_init     : vote for init
    cmd_buf_ptr       : userdata

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_refresh_register_all
(
  mmgsdi_session_id_type       session_id,
  boolean                      register_op,
  boolean                      vote_for_init,
  qmi_cmd_buf_type *           cmd_buf_ptr
)
{
  uint16                         refresh_index     = 0;
  uint32                         notify_mask       = 0;
  mmgsdi_return_enum_type        mmgsdi_status     = MMGSDI_SUCCESS;
  qmi_uimi_client_state_type *   client_sp         = NULL;
  qmi_error_e_type               errval            = QMI_ERR_NONE;

  /* Validate the command buffer to make sure the client is still there */
  ASSERT(cmd_buf_ptr);
  if (cmd_buf_ptr->x_p == NULL || cmd_buf_ptr->x_p->cl_sp == NULL)
  {
    UIM_MSG_HIGH_0("Transaction ptr or client state pointer is NULL");
    return QMI_ERR_INTERNAL;
  }

  /* Extract client sp */
  client_sp = (qmi_uimi_client_state_type *) cmd_buf_ptr->x_p->cl_sp;

  /* In case of deregistration from events, set vote_for_init to FALSE */
  if (register_op == FALSE)
  {
    vote_for_init = FALSE;
  }

  /* Calculate the refresh index */
  errval = qmi_uim_util_session_id_to_index(&refresh_index, session_id);
  if (errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                  session_id);
    return errval;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  /* Calculate the refresh mask */
  notify_mask = 1 << refresh_index;

  /* Store the indication on refresh events */
  if (register_op)
  {
    if (client_sp->uim_info.refresh_all_notify_mask & notify_mask)
    {
      UIM_MSG_ERR_1("Already registered for session id (0x%x)", session_id);
      return QMI_ERR_NO_EFFECT;
    }

    /* Update the mask for registration */
    client_sp->uim_info.refresh_all_notify_mask = (client_sp->uim_info.refresh_all_notify_mask | notify_mask);
  }
  else
  {
    if (!(client_sp->uim_info.refresh_all_notify_mask & notify_mask))
    {
      UIM_MSG_ERR_1("Already deregistered for session id (0x%x)", session_id);
      return QMI_ERR_NO_EFFECT;
    }

    /* Update the mask for deregistration */
    client_sp->uim_info.refresh_all_notify_mask = (client_sp->uim_info.refresh_all_notify_mask & (~notify_mask));
  }

  /* Store the vote for init & registration flags */
  client_sp->uim_info.refresh_vote  = vote_for_init;

  /* Process the registration for refresh. Passing NULL as userdata because we're sending
     the response immediately, without waiting the callback from MMGSDI. */
  mmgsdi_status = mmgsdi_session_register_all_for_refresh(
                          session_id,
                          register_op,
                          vote_for_init,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)NULL );

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
  return errval;
} /* qmi_uim_execute_refresh_register_all */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_SET_SERVICE_STATUS()

  DESCRIPTION
    Enables/disables a service.

  PARAMETERS
    cmd_buf_ptr       : userdata
    session_type      : session type
    tlv_type          : type of service being enabled/disabled
    service_status    : enable or disable
    clid              : client id of the control point

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_set_service_status
(
  void                          * cmd_buf_ptr,
  mmgsdi_session_type_enum_type   session_type,
  uint8                           tlv_type,
  boolean                         service_status,
  uint8                           clid
)
{
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_ERROR;
  mmgsdi_service_enum_type       service_id      = MMGSDI_NONE;
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  qmi_uimi_userdata_type *       cb_userdata_ptr = NULL;
  mmgsdi_app_enum_type           card_type       = MMGSDI_APP_NONE;
  mmgsdi_session_id_type         session_id      = UIM_INVALID_SESSION_ID;
  mmgsdi_static_data_type        aid;

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));

  /* Retrieve session id: currently support only provisioning sessions */
  if (qmi_uim_util_is_session_provisioning(session_type))
  {
    errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                     session_type,
                                                     aid);
    if (errval != QMI_ERR_NONE)
    {
      return errval;
    }
  }
  else
  {
    UIM_MSG_ERR_1("Impossible to set service status for a non provisioning session: 0x%x",
                  session_type);
    return QMI_ERR_INTERNAL;
  }

  /* Retrieve card type */
  errval = qmi_uim_util_session_type_to_card_type(
                             &card_type,
                             session_type,
                             aid);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  /* Retrieve service id */
  if (tlv_type == UIMI_TLV_TYPE_SET_FDN_STATUS)
  {
    switch(card_type)
    {
      case MMGSDI_APP_SIM:
        service_id = MMGSDI_GSM_SRV_FDN;
        break;
      case MMGSDI_APP_RUIM:
        service_id = MMGSDI_CDMA_SRV_FDN;
        break;
      case MMGSDI_APP_USIM:
        service_id = MMGSDI_USIM_SRV_FDN;
        break;
      case MMGSDI_APP_CSIM:
        service_id = MMGSDI_CSIM_SRV_FDN;
        break;
      default:
        UIM_MSG_ERR_1("Unsupported card type: 0x%x", card_type);
        return QMI_ERR_INTERNAL;
    }
  }
  else if (tlv_type == UIMI_TLV_TYPE_SET_ACL_STATUS)
  {
    switch (card_type)
    {
       case MMGSDI_APP_USIM:
         service_id = MMGSDI_USIM_SRV_ACL;
         break;
       default:
         UIM_MSG_ERR_1("Unsupported card type for ACL service: 0x%x",
                       card_type);
         return QMI_ERR_INTERNAL;
    }
  }
  else
  {
    return QMI_ERR_INTERNAL;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SET_SERVICE_STATUS,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Enable or disable FDN */
  if (service_status)
  {
    mmgsdi_status = mmgsdi_session_enable_service(
                                  session_id,
                                  service_id,
                                  qmi_uimi_mmgsdi_callback,
                                  (mmgsdi_client_data_type)cb_userdata_ptr);
  }
  else
  {
    mmgsdi_status = mmgsdi_session_disable_service(
                                  session_id,
                                  service_id,
                                  qmi_uimi_mmgsdi_callback,
                                  (mmgsdi_client_data_type)cb_userdata_ptr);
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_set_service_status */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_SET_FILE_STATUS()

  DESCRIPTION
    Activates/deactivates a file.

  PARAMETERS
    session_id        : session id
    file_access       : file id and path
    file_status       : enable or disable
    cmd_buf_ptr       : userdata
    clid              : client id of the control point

  RETURN VALUE
    errval       : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_set_file_status
(
  mmgsdi_session_id_type          session_id,
  mmgsdi_access_type              file_access,
  uim_file_status_type_v01        file_status,
  void                          * cmd_buf_ptr,
  uint8                           clid
)
{
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_ERROR;
  mmgsdi_slot_id_enum_type       slot            = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_protocol_enum_type      card_protocol   = MMGSDI_MAX_PROTOCOL_ENUM;
  qmi_error_e_type               errval          = QMI_ERR_INTERNAL;
  mmgsdi_session_type_enum_type  session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  qmi_uimi_userdata_type *       cb_userdata_ptr = NULL;
  mmgsdi_static_data_type        aid;

  errval = qmi_uim_util_session_id_to_slot(&slot,
                                           session_id);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  card_protocol = qmi_uim_util_card_protocol(slot);
  if (card_protocol != MMGSDI_UICC)
  {
    UIM_MSG_ERR_1("QMI_UIM_SET_FILE_STATUS not supported for protocol 0x%x",
                  card_protocol);
    return QMI_ERR_NO_EFFECT;
  }

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  memset(&aid, 0x00, sizeof(aid));
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SET_FILE_STATUS,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  if (file_status == UIM_FILE_STATUS_ACTIVATED_V01)
  {
    mmgsdi_status = mmgsdi_session_activate_file(
                      session_id,
                      file_access,
                      qmi_uimi_mmgsdi_callback,
                      (mmgsdi_client_data_type)cb_userdata_ptr);
  }
  else
  {
    mmgsdi_status = mmgsdi_session_deactivate_file(
                      session_id,
                      file_access,
                      qmi_uimi_mmgsdi_callback,
                      (mmgsdi_client_data_type)cb_userdata_ptr);
  }

  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_set_file_status */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_INCREASE()

  DESCRIPTION
    Increase a record on a cyclic or linear fixed file on the card

  PARAMETERS
    session_id  : session id
    file_access : file to be increased
    data        : increase data
    token_ptr   : pointer to token info sent by client
    cmd_buf_ptr : userdata
    clid        : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_increase
(
  mmgsdi_session_id_type     session_id,
  mmgsdi_access_type         file_access,
  mmgsdi_increase_data_type  data,
  const uint32             * token_ptr,
  void *                     cmd_buf_ptr,
  uint8                      clid
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr = NULL;
  mmgsdi_session_type_enum_type  session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type        aid             = { 0, };

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_INCREASE,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute MMGSDI read transparent */
  mmgsdi_status = mmgsdi_session_increase(
                        session_id,
                        file_access,
                        data,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_increase */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_STATUS()

  DESCRIPTION
    Send STATUS command.

  PARAMETERS
    session_id  : session id
    status_mode : Mode in STATUS command
    status_resp : Response requested from STATUS command.
    cmd_buf_ptr : userdata
    clid        : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_status
(
  mmgsdi_session_id_type            session_id,
  mmgsdi_status_me_app_enum_type    status_mode,
  mmgsdi_status_ret_data_enum_type  status_resp,
  void                             *cmd_buf_ptr,
  uint8                             clid
)
{
  qmi_error_e_type         errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type  mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type  *cb_userdata_ptr = NULL;

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SEND_STATUS,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  mmgsdi_status = mmgsdi_session_send_card_status(
                                     session_id,
                                     status_mode,
                                     status_resp,
                                     qmi_uimi_mmgsdi_callback,
                                     (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
} /* qmi_uim_execute_status */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_SEARCH_RECORD()

  DESCRIPTION
    Send SEARCH command to MMGSDI.

  PARAMETERS
    session_id       : session id
    file_access      : file to be read
    search_direction : direction in which we need to perform search
    first_record     : first record in the file
    search_patern    : pattern of data that need to be searched
    token_ptr        : token data
    cmd_buf_ptr      : userdata
    clid             : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_execute_search_record
(
  mmgsdi_session_id_type            session_id,
  mmgsdi_access_type                file_access,
  mmgsdi_search_data_type           search_pattern,
  mmgsdi_rec_num_type               first_record,
  mmgsdi_search_direction_enum_type search_direction,
  const uint32                    * token_ptr,
  void *                            cmd_buf_ptr,
  uint8                             clid
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*        cb_userdata_ptr = NULL;
  mmgsdi_session_type_enum_type  session_type    = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_static_data_type        aid             = { 0, };
  mmgsdi_search_offset_data_type enhanced_search_offset_data;

  /* Validate the first record */
  if (first_record < 1 || first_record > QMI_UIM_MAX_NUM_RECORD ||
      search_pattern.data_len > QMI_UIM_MAX_RECORD_LEN)
  {
    UIM_MSG_ERR_2("First record number(0x%x) is invalid or pattern length (0x%x) exceeds record length",
                  first_record, search_pattern.data_len);
    return QMI_ERR_INVALID_ARG;
  }

  /* Initialize dummy enhanced_search_offset_data to pass in MMGSDI API call */
  memset (&enhanced_search_offset_data, 0x00,
          sizeof(mmgsdi_search_offset_data_type));

  /* Try to convert path into enum. At this point
     we can ignore errors and proceed with access by path */
  if (qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id) == QMI_ERR_NONE)
  {
    (void)qmi_uim_convert_file_access_method(&file_access,
                                             MMGSDI_EF_ENUM_ACCESS,
                                             session_type);
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SEARCH_RECORD,
                                                 cmd_buf_ptr,
                                                 token_ptr,
                                                 clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute MMGSDI search */
  mmgsdi_status = mmgsdi_session_search(
                        session_id,
                        file_access,
                        MMGSDI_UICC_SIMPLE_SEARCH,
                        first_record,
                        search_direction,
                        enhanced_search_offset_data,
                        search_pattern,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  return errval;
}/* qmi_uim_execute_search_record */


/*===========================================================================
  FUNCTION QMI_UIM_OPEN_SESSION()

  DESCRIPTION
    Opens a new non provisioning session on the fly and enqueues
    the command in the list of the session, so it can be executed
    later on, when the session is successfully opened.

  PARAMETERS
    command_id     : command id
    tlv_data_ptr   : pointer to list of TLVs
    tlv_data_count : number of TLVs
    cmd_buf_ptr    : userdata of the command

  RETURN VALUE
    errval         : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_open_session
(
  qmi_uimi_cmd_val_e_type            command_id,
  qmi_uim_tlv_item_type            * tlv_data_ptr,
  uint8                              tlv_data_count,
  void                             * cmd_buf_ptr,
  qmi_uimi_client_state_type       * cl_sp
)
{
  uint16                                 i               = 0;
  uint16                                 non_prov_index  = UIM_INVALID_APP_INDEX;
  qmi_uim_tlv_session_information_type * tlv_session_ptr = NULL;
  qmi_uimi_pending_request_type        * request_ptr     = NULL;
  qmi_uimi_pending_request_type        * current_ptr     = NULL;
  qmi_uimi_userdata_type               * cb_userdata_ptr = NULL;

  ASSERT(tlv_data_ptr && tlv_data_count > 0 && qmi_uim_global_ptr && cl_sp && qmi_uim_state_ptr);

  /* Find session information in the TLVs */
  for (i = 0; i < tlv_data_count; i++)
  {
    if (tlv_data_ptr[i].type == UIMI_TLV_TYPE_SESSION_INFORMATION &&
        tlv_data_ptr[i].present)
    {
      tlv_session_ptr = &tlv_data_ptr[i].data.session_information;
      break;
    }
  }
  if (tlv_session_ptr == NULL)
  {
    UIM_MSG_ERR_0("No session information in TLV");
    return QMI_ERR_INTERNAL;
  }

  /* Check AID validity */
  if(tlv_session_ptr->aid.data_len == QMI_UIM_TLV_CHANNEL_ID_LEN)
  {
    UIM_MSG_ERR_1("Invalid AID length: 0x%x, cannot open session",
                  tlv_session_ptr->aid.data_len);
    return QMI_ERR_INVALID_ARG;
  }

  /* Verify session type is non provisioning */
  if ((tlv_session_ptr->session_type != MMGSDI_NON_PROV_SESSION_SLOT_1 &&
       tlv_session_ptr->session_type != MMGSDI_NON_PROV_SESSION_SLOT_2 &&
       tlv_session_ptr->session_type != MMGSDI_NON_PROV_SESSION_SLOT_3) ||
      tlv_session_ptr->aid.data_len > MMGSDI_MAX_AID_LEN)
  {
    UIM_MSG_ERR_1("Impossible to open a non prov session of type 0x%x",
                  tlv_session_ptr->session_type);
    return QMI_ERR_INTERNAL;
  }

  /* Check if a non provisioning session is already opening or find an
     empty slot to add this new session */
  non_prov_index = UIM_INVALID_APP_INDEX;
  for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
  {
    if ((qmi_uim_global_ptr->non_prov_session[i] == NULL) &&
        non_prov_index == UIM_INVALID_APP_INDEX)
    {
      /* Empty slot found. Keep searching */
      non_prov_index = i;
    }

    if (qmi_uim_global_ptr->non_prov_session[i] != NULL &&
        qmi_uim_global_ptr->non_prov_session[i]->session_type == tlv_session_ptr->session_type)
    {
      if ((qmi_uim_global_ptr->non_prov_session[i]->aid.data_len == 0 &&
           tlv_session_ptr->aid.data_len == 0)||
          (qmi_uim_global_ptr->non_prov_session[i]->aid.data_len > 0 &&
           qmi_uim_global_ptr->non_prov_session[i]->aid.data_len == tlv_session_ptr->aid.data_len &&
           memcmp( (void*)qmi_uim_global_ptr->non_prov_session[i]->aid.data_ptr,
                  (void*)tlv_session_ptr->aid.data_ptr,
                          tlv_session_ptr->aid.data_len ) == 0))
      {
        /* Make sure the non-provisioning session is still opening. This
           is done by verifying the session id */
        if (qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id != UIM_INVALID_SESSION_ID)
        {
          UIM_MSG_ERR_0("Non prov session already exists");
          return QMI_ERR_INTERNAL;
        }

        /* Matching non provisioning session found. Break. */
        UIM_MSG_HIGH_0("Found opening non prov session");
        non_prov_index = i;
        break;
      }
    }
  }

  /* Index should be valid... otherwise it means that we didn't find
     the corresponding session and the array is also full */
  if (non_prov_index == UIM_INVALID_APP_INDEX)
  {
    UIM_MSG_ERR_0("Impossible to open a non prov session");
    return QMI_ERR_NO_MEMORY;
  }

  /* If non provisioning session does not exist yet, open now */
  if (qmi_uim_global_ptr->non_prov_session[non_prov_index] == NULL)
  {
    mmgsdi_return_enum_type          mmgsdi_status = MMGSDI_SUCCESS;
    mmgsdi_app_select_with_rsp_type  app_info;
    mmgsdi_slot_id_enum_type         slot = MMGSDI_SLOT_1;

    if(tlv_session_ptr->session_type == MMGSDI_NON_PROV_SESSION_SLOT_1)
    {
      slot = MMGSDI_SLOT_1;
    }
    else if(tlv_session_ptr->session_type == MMGSDI_NON_PROV_SESSION_SLOT_2)
    {
      slot = MMGSDI_SLOT_2;
    }
    else
    {
      slot = MMGSDI_SLOT_3;
    }


    /* Allocate the buffer for the non prov session */
    qmi_uim_global_ptr->non_prov_session[non_prov_index] =
                       (qmi_uim_non_prov_session_info_type*)uimqmi_malloc(sizeof(qmi_uim_non_prov_session_info_type));
    if (qmi_uim_global_ptr->non_prov_session[non_prov_index] == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    /* Create userdata. Note: since request id is not used in the callback,
       it is set to UIMI_CMD_VAL_WIDTH */
    cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_OPEN_LOGICAL_CHANNEL,
                                                   cmd_buf_ptr,
                                                   NULL,
                                                   cl_sp->common.clid);
    if (cb_userdata_ptr == NULL)
    {
      uimqmi_free(qmi_uim_global_ptr->non_prov_session[non_prov_index]);
      qmi_uim_global_ptr->non_prov_session[non_prov_index] = NULL;
      return QMI_ERR_NO_MEMORY;
    }

    /* Update userdata with necessary info */
    cb_userdata_ptr->data.session_open_request.channel_index     = (uint8)non_prov_index;
    cb_userdata_ptr->data.session_open_request.session_open_type = UIM_SESSION_OPEN_IMPLICIT;
    cb_userdata_ptr->data.session_open_request.session_type      = tlv_session_ptr->session_type;

    /* Compose app info */
    memset(&app_info, 0, sizeof(mmgsdi_app_select_with_rsp_type));
    app_info.exclusive_channel    = FALSE;
    app_info.app_id_data.data_len = tlv_session_ptr->aid.data_len;
    app_info.app_id_data.data_ptr = tlv_session_ptr->aid.data_ptr;
    app_info.select_rsp_type      = MMGSDI_SELECT_RSP_FCP;

    mmgsdi_status = mmgsdi_session_open_with_select_rsp(
                       qmi_uim_global_ptr->mmgsdi_client_id,
                       slot,
                       app_info,
                       qmi_uim_mmgsdi_evt_cback,
                       qmi_uimi_mmgsdi_callback,
                       (mmgsdi_client_data_type)cb_userdata_ptr);

    /* Free userdata in case of error */
    QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      qmi_error_e_type errval =
         qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

      UIM_MSG_ERR_1("Error opening non prov session: 0x%x", mmgsdi_status);
      uimqmi_free(qmi_uim_global_ptr->non_prov_session[non_prov_index]);
      qmi_uim_global_ptr->non_prov_session[non_prov_index] = NULL;
      return errval;
    }

    /* Initialize the values */
    qmi_uim_global_ptr->non_prov_session[non_prov_index]->mmgsdi_session_id = UIM_INVALID_SESSION_ID;
    qmi_uim_global_ptr->non_prov_session[non_prov_index]->session_type = tlv_session_ptr->session_type;
    (void)memscpy(&qmi_uim_global_ptr->non_prov_session[non_prov_index]->aid,
                  sizeof(mmgsdi_static_data_type),
                  &tlv_session_ptr->aid,
                  sizeof(mmgsdi_static_data_type));
  }

  /* Associate the Client with the requested Session */
  (void)qmi_uim_util_update_non_prov_session_cl_sp(
          tlv_data_ptr,
          tlv_data_count,
          cl_sp);

  /* Allocate memory for the new node to be added */
  request_ptr = uimqmi_malloc(sizeof(qmi_uimi_pending_request_type));
  if (request_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  memset(request_ptr, 0, sizeof(qmi_uimi_pending_request_type));

  /* Allocate memory to duplicate TLV data for the non provisioning session */
  request_ptr->tlv_data_ptr = uimqmi_malloc(
                                tlv_data_count * sizeof(qmi_uim_tlv_item_type));
  if (request_ptr->tlv_data_ptr == NULL)
  {
    uimqmi_free(request_ptr);
    return QMI_ERR_NO_MEMORY;
  }

  /* Add the new node to the global linked list. If list already has elements,
     iterate to end of the list to insert the new node */
  current_ptr = qmi_uim_global_ptr->non_prov_session[non_prov_index]->pending_req_ptr;
  if (current_ptr == NULL)
  {
    qmi_uim_global_ptr->non_prov_session[non_prov_index]->pending_req_ptr = request_ptr;
  }
  else
  {
    /* Iterate to the last node & insert */
    while (current_ptr->next_ptr != NULL)
    {
      current_ptr = current_ptr->next_ptr;
    }
    current_ptr->next_ptr = request_ptr;
  }

  /* Duplicate TLV data: this also takes care of zeroing the source
     to avoid invalid pointers around */
  request_ptr->tlv_data_count = tlv_data_count;
  request_ptr->command_id     = command_id;
  request_ptr->cmd_buf_ptr    = cmd_buf_ptr;
  request_ptr->clid           = cl_sp->common.clid;
  request_ptr->client_reg_count =
     qmi_uim_state_ptr->client_reg_count[request_ptr->clid - 1];

  qmi_uim_util_duplicate_tlv_data(request_ptr->tlv_data_ptr,
                                  tlv_data_count * sizeof(qmi_uim_tlv_item_type),
                                  tlv_data_ptr,
                                  tlv_data_count);

  return QMI_ERR_NONE;
} /* qmi_uim_open_session */


/*===========================================================================
  FUNCTION QMI_UIM_CHECK_NON_PROV_AUTH()

  DESCRIPTION
    Checks if there are provisioning applications active and
    valid for authentication on the same slot as non-provisioning
    application requesting authentication

  PARAMETERS
    slot_id : slot of the calling non-prov application

  RETURN VALUE
    TRUE  : proceed with non-prov authentication
    FALSE : deny authentication as provisioning session is blocking

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_check_non_prov_auth
(
  mmgsdi_slot_id_enum_type slot_id
)
{
  /* Initalize to TRUE in case no provisioned application is
     present then proceed with authentication */
  boolean non_prov_auth_pass = TRUE;
  uint8   i                  = 0;

  ASSERT(qmi_uim_global_ptr);

  for (i = 0;  i < qmi_uim_global_ptr->card_state.num_apps &&
       i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
        qmi_uim_global_ptr->card_state.application[i]->slot == slot_id &&
        qmi_uim_util_is_provisioning_app_index(i) != FALSE)
    {
      if((qmi_uim_global_ptr->card_state.application[i]->app_state !=
          QMI_UIM_APP_STATE_ILLEGAL) &&
         (qmi_uim_global_ptr->card_state.application[i]->app_state !=
          QMI_UIM_APP_STATE_PERSO))
      {
        /* If at least one valid provisioning application exists on the slot
           then continue with authentication */
        return TRUE;
      }
      non_prov_auth_pass = FALSE;
    }
  }

  return non_prov_auth_pass;
} /* qmi_uim_check_non_prov_auth */


/*===========================================================================
  FUNCTION QMI_UIM_AUTH_ALLOWED_IN_APP_STATE

  DESCRIPTION
    Check if authentication request is allowed in the current app state.

  PARAMETERS
    session_type  : session type

  RETURN VALUE
    boolean       : TRUE if authentication is allowed

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_auth_allowed_in_app_state
(
  mmgsdi_session_type_enum_type   session_type
)
{
  uint16 index = UIM_INVALID_APP_INDEX;

  ASSERT(qmi_uim_global_ptr);

  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      index = qmi_uim_global_ptr->card_state.index_gw[0];
      break;

    case MMGSDI_1X_PROV_PRI_SESSION:
      index = qmi_uim_global_ptr->card_state.index_1x[0];
      break;

    case MMGSDI_GW_PROV_SEC_SESSION:
      index = qmi_uim_global_ptr->card_state.index_gw[1];
      break;

    case MMGSDI_1X_PROV_SEC_SESSION:
      index = qmi_uim_global_ptr->card_state.index_1x[1];
      break;

    case MMGSDI_GW_PROV_TER_SESSION:
      index = qmi_uim_global_ptr->card_state.index_gw[2];
      break;

    case MMGSDI_1X_PROV_TER_SESSION:
      index = qmi_uim_global_ptr->card_state.index_1x[2];
      break;

    case MMGSDI_NON_PROV_SESSION_SLOT_1:
      return qmi_uim_check_non_prov_auth(MMGSDI_SLOT_1);

    case MMGSDI_NON_PROV_SESSION_SLOT_2:
      return qmi_uim_check_non_prov_auth(MMGSDI_SLOT_2);

    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      return qmi_uim_check_non_prov_auth(MMGSDI_SLOT_3);

    default:
      return FALSE;
  }

  /* Check if the index is valid */
  if (index >= qmi_uim_global_ptr->card_state.num_apps ||
      index >= QMI_UIM_MAX_APP_COUNT ||
      qmi_uim_global_ptr->card_state.application[index] == NULL)
  {
    UIM_MSG_ERR_1("Invalid index: 0x%x", index);
    return FALSE;
  }

  if((qmi_uim_global_ptr->card_state.application[index]->app_state ==
      QMI_UIM_APP_STATE_ILLEGAL) ||
     (qmi_uim_global_ptr->card_state.application[index]->app_state ==
      QMI_UIM_APP_STATE_PERSO))
  {
    return FALSE;
  }

  return TRUE;
} /* qmi_uim_auth_allowed_in_app_state */


/*===========================================================================
  FUNCTION QMI_UIM_UPDATE_PIN_STATE()

  DESCRIPTION
    Updates the PIN state after a PIN operation for the application
    used for the operation.

  PARAMETERS
    pin                 : PIN id to be updated
    pin_status          : PIN status
    upin_replace_status : PIN1 replacement status
    app_index           : index of application to be updated
    card_index          : card index to be updated

  RETURN VALUE
    TRUE   : Indicates there is an update
    FALSE  : Indicates no updates

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_update_pin_state
(
  mmgsdi_pin_enum_type         pin,
  mmgsdi_pin_status_enum_type  pin_status,
  mmgsdi_pin_replace_enum_type upin_replace_status,
  uint16                       app_index,
  uint8                        card_index
)
{
  boolean  result                               = FALSE;
  boolean  update_state[QMI_UIM_MAX_APP_COUNT]  = {FALSE};
  int      i                                    = 0;

  ASSERT(qmi_uim_global_ptr);

  if (pin == MMGSDI_PIN1)
  {
    if (app_index < QMI_UIM_MAX_APP_COUNT &&
        app_index < qmi_uim_global_ptr->card_state.num_apps &&
        qmi_uim_global_ptr->card_state.application[app_index] != NULL)
    {
      mmgsdi_pin_status_enum_type new_pin1_state =
               qmi_uim_global_ptr->card_state.application[app_index]->pin1_state;

      /* PIN state */
      if(pin_status == MMGSDI_PIN_UNBLOCKED)
      {
        new_pin1_state = MMGSDI_PIN_ENABLED_VERIFIED;
      }
      else if (pin_status == MMGSDI_PIN_CHANGED)
      {
        if (qmi_uim_global_ptr->card_state.application[app_index]->pin1_state ==
            MMGSDI_PIN_ENABLED_NOT_VERIFIED)
        {
          new_pin1_state = MMGSDI_PIN_ENABLED_VERIFIED;
        }
      }
      else if (pin_status == MMGSDI_PIN_RETRY_COUNT_DECREASED)
      {
        /* Indicates a decrease in retry count */
      }
      else
      {
        new_pin1_state = pin_status;
      }

      /* Indicates a change in PIN1 state */
      if (new_pin1_state !=
          qmi_uim_global_ptr->card_state.application[app_index]->pin1_state)
      {
        qmi_uim_global_ptr->card_state.application[app_index]->pin1_state = new_pin1_state;

        /* Mark result as true (update) */
        result = TRUE;
      }

      /* Universal PIN boolean */
      if (upin_replace_status == MMGSDI_PIN_REPLACED_BY_UNIVERSAL)
      {
        qmi_uim_global_ptr->card_state.application[app_index]->upin = TRUE;
      }
      else
      {
        qmi_uim_global_ptr->card_state.application[app_index]->upin = FALSE;
        /* update state from PIN event */
        update_state[app_index] = TRUE;
      }
    }
  }

  if (pin == MMGSDI_PIN2)
  {
    if (app_index < QMI_UIM_MAX_APP_COUNT &&
        app_index < qmi_uim_global_ptr->card_state.num_apps &&
        qmi_uim_global_ptr->card_state.application[app_index] != NULL)
    {
      mmgsdi_pin_status_enum_type new_pin2_state =
              qmi_uim_global_ptr->card_state.application[app_index]->pin2_state;

      /* PIN state */
      if(pin_status == MMGSDI_PIN_UNBLOCKED)
      {
        new_pin2_state = MMGSDI_PIN_ENABLED_VERIFIED;
      }
      else if (pin_status == MMGSDI_PIN_CHANGED)
      {
        if (qmi_uim_global_ptr->card_state.application[app_index]->pin2_state ==
            MMGSDI_PIN_ENABLED_NOT_VERIFIED)
        {
          new_pin2_state = MMGSDI_PIN_ENABLED_VERIFIED;
        }
      }
      else if (pin_status == MMGSDI_PIN_RETRY_COUNT_DECREASED)
      {
        /* Indicates a decrease in retry count */
      }
      else
      {
        new_pin2_state = pin_status;
      }

      /* Indicates a change in PIN2 state */
      if (new_pin2_state !=
          qmi_uim_global_ptr->card_state.application[app_index]->pin2_state)
      {
        qmi_uim_global_ptr->card_state.application[app_index]->pin2_state = new_pin2_state;

        /* Mark result as true (update) */
        result = TRUE;
      }
    }
  }

  if (pin == MMGSDI_UNIVERSAL_PIN)
  {
    if (card_index < QMI_UIM_MAX_CARD_COUNT &&
        card_index < qmi_uim_global_ptr->card_state.num_slots &&
        qmi_uim_global_ptr->card_state.card[card_index] != NULL)
    {
      mmgsdi_pin_status_enum_type new_upin_state =
            qmi_uim_global_ptr->card_state.card[card_index]->upin_state;

      /* PIN state */
      if(pin_status == MMGSDI_PIN_UNBLOCKED)
      {
        new_upin_state = MMGSDI_PIN_ENABLED_VERIFIED;
      }
      else if (pin_status == MMGSDI_PIN_CHANGED)
      {
        if (qmi_uim_global_ptr->card_state.card[card_index]->upin_state ==
            MMGSDI_PIN_ENABLED_NOT_VERIFIED)
        {
          new_upin_state = MMGSDI_PIN_ENABLED_VERIFIED;
        }
      }
      else if (pin_status == MMGSDI_PIN_RETRY_COUNT_DECREASED)
      {
        /* Indicates a decrease in retry count */
      }
      else
      {
        new_upin_state = pin_status;
      }

      /* Indicates a change in UPIN state */
      if (new_upin_state !=
          qmi_uim_global_ptr->card_state.card[card_index]->upin_state)
      {
        qmi_uim_global_ptr->card_state.card[card_index]->upin_state = new_upin_state;

        /* Mark result as true (update) */
        result = TRUE;
      }
    } /* card_index < QMI_UIM_MAX_CARD_COUNT */

    /* List of applications to update */
    for (i = 0; i < qmi_uim_global_ptr->card_state.num_apps &&
                i < QMI_UIM_MAX_APP_COUNT; i++)
    {
      if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
          qmi_uim_global_ptr->card_state.application[i]->upin)
      {
        /* Update state from PIN event */
        update_state[i] = TRUE;
      }
    }
  }

  /* Update state of applications based on PIN event */
  for (i = 0; i < qmi_uim_global_ptr->card_state.num_apps &&
              i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (qmi_uim_global_ptr->card_state.application[i] == NULL)
    {
      continue;
    }

    if (update_state[i] == FALSE)
    {
      continue;
    }

    /* Update the app state for prov apps only */
    if (qmi_uim_util_is_provisioning_app_index(i) == FALSE)
    {
      continue;
    }

    switch(pin_status)
    {
      case MMGSDI_PIN_STATUS_NOT_INITIALIZED:
      case MMGSDI_PIN_DISABLED:
      case MMGSDI_PIN_ENABLED_VERIFIED:
      case MMGSDI_PIN_UNBLOCKED:
      case MMGSDI_PIN_CHANGED:
        /* We transition to PERSO state after PIN states, only if PIN
           was required. There might be cases where the event is received
           after initialization is completed. */
        if (qmi_uim_global_ptr->card_state.application[i]->app_state == QMI_UIM_APP_STATE_PIN_REQUIRED ||
            qmi_uim_global_ptr->card_state.application[i]->app_state == QMI_UIM_APP_STATE_PUK1_REQUIRED)
        {
          if(qmi_uim_global_ptr->card_state.application[i]->perso_state == QMI_UIM_PERSO_STATE_READY)
          {
            qmi_uim_global_ptr->card_state.application[i]->app_state = QMI_UIM_APP_STATE_READY;
          }
          else
          {
            qmi_uim_global_ptr->card_state.application[i]->app_state = QMI_UIM_APP_STATE_PERSO;
          }
          result = TRUE;
        }
        break;

      case MMGSDI_PIN_ENABLED_NOT_VERIFIED:
        if (qmi_uim_global_ptr->card_state.application[i]->app_state != QMI_UIM_APP_STATE_PIN_REQUIRED)
        {
          qmi_uim_global_ptr->card_state.application[i]->app_state = QMI_UIM_APP_STATE_PIN_REQUIRED;
          result = TRUE;
        }
        break;

      case MMGSDI_PIN_BLOCKED:
        if (qmi_uim_global_ptr->card_state.application[i]->app_state != QMI_UIM_APP_STATE_PUK1_REQUIRED)
        {
          qmi_uim_global_ptr->card_state.application[i]->app_state = QMI_UIM_APP_STATE_PUK1_REQUIRED;
          result = TRUE;
        }
        break;

      case MMGSDI_PIN_PERM_BLOCKED:
        if (qmi_uim_global_ptr->card_state.application[i]->app_state != QMI_UIM_APP_STATE_BLOCKED)
        {
          qmi_uim_global_ptr->card_state.application[i]->app_state = QMI_UIM_APP_STATE_BLOCKED;
          result = TRUE;
        }
        break;

      case MMGSDI_PIN_RETRY_COUNT_DECREASED:
      default:
        break;
    }
  }

  return result;
} /* qmi_uim_update_pin_state */


/*===========================================================================
  FUNCTION QMI_UIM_UPDATE_PIN1_OTHER_APPS()

  DESCRIPTION
    Updates the number of PIN1 retries and state in the global card status
    after a PIN operation for other applications, other than the one
    used for the operation. This is required in case 2 or more applications
    are sharing the same PIN.

  PARAMETERS
    pin_info_ptr  : PIN info ptr
    slot          : slot id
    updated_index : index of application already updated

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_update_pin1_other_apps
(
  const mmgsdi_pin_info_type * pin_info_ptr,
  mmgsdi_slot_id_enum_type     slot,
  uint16                       updated_index
)
{
  uint16                       i                                   = 0;
  uint16                       index                               = 0;
  uint8                        card_index                          = 0;
  uint16                       num_app                             = 0;
  mmgsdi_app_info_type       * app_info_ptr                        = NULL;
  mmgsdi_return_enum_type      mmgsdi_status                       = MMGSDI_SUCCESS;
  mmgsdi_available_apps_type * available_apps_ptr                  = NULL;

  ASSERT(pin_info_ptr && qmi_uim_global_ptr);

  /* This function is only for PIN1 */
  if (pin_info_ptr->pin_id != MMGSDI_PIN1)
  {
    return;
  }

  /* Check total number of applications on the slot: if there is only 1 application
     this function is useless and we can immediately return */
  for (i = 0; i < qmi_uim_global_ptr->card_state.num_apps &&
              i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
        qmi_uim_global_ptr->card_state.application[i]->slot == slot)
    {
      num_app++;
    }
  }
  if (num_app <= 1)
  {
    return;
  }

  UIM_MSG_HIGH_0("Retrieving other apps to update PIN retries");

  /* Allocate memory for available apps pointer */
  available_apps_ptr = (mmgsdi_available_apps_type *)uimqmi_malloc(
                            sizeof(mmgsdi_available_apps_type));

  if(available_apps_ptr == NULL)
  {
    return;
  }

  /* Retrieve status of all available apps */
  mmgsdi_status = mmgsdi_get_all_available_apps_sync(qmi_uim_global_ptr->mmgsdi_client_id,
                                                     available_apps_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_1("Error retrieving app status: 0x%x", mmgsdi_status);
    uimqmi_free(available_apps_ptr);
    available_apps_ptr = NULL;
    return;
  }

  /* Retrieve information for the corresponding slot */
  switch (slot)
  {
    case MMGSDI_SLOT_1:
      num_app      = (uint16)available_apps_ptr->card1_num_app;
      app_info_ptr = available_apps_ptr->card1_app_info;
      break;
   case MMGSDI_SLOT_2:
     num_app      = (uint16)available_apps_ptr->card2_num_app;
     app_info_ptr = available_apps_ptr->card2_app_info;
     break;
   case MMGSDI_SLOT_3:
     num_app      = (uint16)available_apps_ptr->card3_num_app;
     app_info_ptr = available_apps_ptr->card3_app_info;
     break;
    default:
      uimqmi_free(available_apps_ptr);
      available_apps_ptr = NULL;
      return;
  }

  if ((num_app > MMGSDI_MAX_APP_INFO) ||
      (app_info_ptr == NULL))
  {
    UIM_MSG_ERR_1("Invalid number of applications: 0x%x", num_app);
    uimqmi_free(available_apps_ptr);
    available_apps_ptr = NULL;
    return;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(slot, &card_index,
        qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    uimqmi_free(available_apps_ptr);
    available_apps_ptr = NULL;
    return;
  }

  if (card_index >= QMI_UIM_MAX_CARD_COUNT ||
      card_index > qmi_uim_global_ptr->card_state.num_slots)
  {
    uimqmi_free(available_apps_ptr);
    available_apps_ptr = NULL;
    return;
  }

  /* Loop thru all applications in QMI */
  for (i = 0; i < num_app; i++)
  {
    /* Verify that number of retries for the application is valid */
    if (!app_info_ptr[i].pin1.valid_num_retries &&
        !app_info_ptr[i].pin1.valid_num_unblock_retries)
    {
      continue;
    }

    /* Find the application in QMI table */
    index = qmi_uim_util_find_aid(&app_info_ptr[i].app_data, slot);

    /* Verify that index is valid and is not the same index that we have already updated
       processing the result of the operation */
    if (index == UIM_INVALID_APP_INDEX ||
        index >= QMI_UIM_MAX_APP_COUNT ||
        index >= qmi_uim_global_ptr->card_state.num_apps ||
        index == updated_index ||
        qmi_uim_global_ptr->card_state.application[index] == NULL)
    {
      continue;
    }

    UIM_MSG_HIGH_1("Updating PIN information for app: 0x%x", index);

    (void)qmi_uim_update_pin_state(app_info_ptr[i].pin1.pin_id,
                                   app_info_ptr[i].pin1.status,
                                   app_info_ptr[i].pin1.pin_replacement,
                                   index,
                                   card_index);

    /* Update the number of retries, if needed */
    if(app_info_ptr[i].pin1.valid_num_retries)
    {
      qmi_uim_global_ptr->card_state.application[index]->pin1_num_retries =
        (uint8)app_info_ptr[i].pin1.num_retries;
    }
    if(app_info_ptr[i].pin1.valid_num_unblock_retries)
    {
      qmi_uim_global_ptr->card_state.application[index]->puk1_num_retries =
        (uint8)app_info_ptr[i].pin1.num_unblock_retries;
    }
  }

  uimqmi_free(available_apps_ptr);
  available_apps_ptr = NULL;
} /* qmi_uim_update_pin1_other_apps */


/*===========================================================================
  FUNCTION QMI_UIM_UPDATE_PIN_RETRIES_AND_STATE()

  DESCRIPTION
    Updates the number of PIN retries and state in the global card status after
    a PIN operation

  PARAMETERS
    pin_cnf_ptr : PIN operation confirmation

  RETURN VALUE
    Boolean indicating if there was an update

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_update_pin_retries_and_state
(
  const mmgsdi_pin_operation_cnf_type * pin_cnf_ptr
)
{
  boolean result     = FALSE;
  uint16  app_index  = UIM_INVALID_APP_INDEX;
  uint8   slot_idx   = 0;

  ASSERT(pin_cnf_ptr && qmi_uim_global_ptr);

  /* PIN1 and PIN2 */
  if (pin_cnf_ptr->pin_info.pin_id == MMGSDI_PIN1 ||
      pin_cnf_ptr->pin_info.pin_id == MMGSDI_PIN2)
  {
    uint8                  i          = 0;
    mmgsdi_session_id_type session_id = pin_cnf_ptr->response_header.session_id;

    /* Find the provisioning application index for the session id */
    for (i = 0; i < QMI_UIM_MAX_PROV_SESSIONS; i++)
    {
      if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[i])
      {
        app_index = qmi_uim_global_ptr->card_state.index_gw[i];
        break;
      }
      else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[i])
      {
        app_index = qmi_uim_global_ptr->card_state.index_1x[i];
        break;
      }
    }

    if (app_index < qmi_uim_global_ptr->card_state.num_apps &&
        app_index < QMI_UIM_MAX_APP_COUNT &&
        qmi_uim_global_ptr->card_state.application[app_index] != NULL)
    {
      UIM_MSG_HIGH_2("Updating PIN retries - index: 0x%x, id: 0x%x",
                     app_index, pin_cnf_ptr->pin_info.pin_id);

      /* Update PIN1 */
      if (pin_cnf_ptr->pin_info.pin_id == MMGSDI_PIN1 &&
          !qmi_uim_global_ptr->card_state.application[app_index]->upin)
      {
        if ((pin_cnf_ptr->pin_info.valid_num_retries) &&
            (qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries !=
             (uint8)pin_cnf_ptr->pin_info.num_retries))
        {
          qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries =
            (uint8)pin_cnf_ptr->pin_info.num_retries;
          result = TRUE;
        }
        if ((pin_cnf_ptr->pin_info.valid_num_unblock_retries) &&
            (qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries !=
             (uint8)pin_cnf_ptr->pin_info.num_unblock_retries))
        {
          qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries =
            (uint8)pin_cnf_ptr->pin_info.num_unblock_retries;
          result = TRUE;
        }
      }
      /* Update PIN2 */
      if (pin_cnf_ptr->pin_info.pin_id == MMGSDI_PIN2)
      {
        if ((pin_cnf_ptr->pin_info.valid_num_retries) &&
            (qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries !=
             (uint8)pin_cnf_ptr->pin_info.num_retries))
        {
          qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries =
            (uint8)pin_cnf_ptr->pin_info.num_retries;
          result = TRUE;
        }
        if ((pin_cnf_ptr->pin_info.valid_num_unblock_retries) &&
            (qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries !=
             (uint8)pin_cnf_ptr->pin_info.num_unblock_retries))
        {
          qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries =
            (uint8)pin_cnf_ptr->pin_info.num_unblock_retries;
          result = TRUE;
        }
      }
    }

    /* Other applications on the card might share the same PIN. Update the PIN
       retries and state for these applications as well. No need to set result,
       as it would have been set already for the current application, if needed. */
    if (pin_cnf_ptr->pin_info.pin_id == MMGSDI_PIN1)
    {
      qmi_uim_update_pin1_other_apps(&pin_cnf_ptr->pin_info,
                                     pin_cnf_ptr->response_header.slot_id,
                                     app_index);
    }
  }

  /* Unversal PIN */
  if (pin_cnf_ptr->pin_info.pin_id == MMGSDI_UNIVERSAL_PIN)
  {
    if (uimqmi_mmgsdi_slot_to_slot_index(pin_cnf_ptr->response_header.slot_id,
                                         &slot_idx,
                                         qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
    {
      return FALSE;
    }

    if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
       slot_idx >= qmi_uim_global_ptr->card_state.num_slots ||
       qmi_uim_global_ptr->card_state.card[slot_idx] == NULL)
    {
      return FALSE;
    }

    UIM_MSG_HIGH_1("Updating UPIN retries - slot: 0x%x", slot_idx);

    /* Update UPIN retries for the card */
    if ((pin_cnf_ptr->pin_info.valid_num_retries) &&
        (qmi_uim_global_ptr->card_state.card[slot_idx]->upin_num_retries !=
         (uint8)pin_cnf_ptr->pin_info.num_retries))
    {
      qmi_uim_global_ptr->card_state.card[slot_idx]->upin_num_retries =
        (uint8)pin_cnf_ptr->pin_info.num_retries;
      result = TRUE;
    }
    if ((pin_cnf_ptr->pin_info.valid_num_unblock_retries) &&
        (qmi_uim_global_ptr->card_state.card[slot_idx]->upuk_num_retries !=
         (uint8)pin_cnf_ptr->pin_info.num_unblock_retries))
    {
      qmi_uim_global_ptr->card_state.card[slot_idx]->upuk_num_retries =
        (uint8)pin_cnf_ptr->pin_info.num_unblock_retries;
      result = TRUE;
    }
  }

  /* Update the PIN state for this app */
  if (qmi_uim_update_pin_state(pin_cnf_ptr->pin_info.pin_id,
                               pin_cnf_ptr->pin_info.status,
                               pin_cnf_ptr->pin_info.pin_replacement,
                               app_index, slot_idx))
  {
    result = TRUE;
  }

  return result;
} /* qmi_uim_update_pin_retries_and_state */


/*===========================================================================
  FUNCTION QMI_UIM_UPDATE_CARD_STATUS_VALIDITY()

  DESCRIPTION
    Updates the card status validity flag as a result of any of the incoming
     events from MMGSDI.

  PARAMETERS
    event_ptr : pointer to the event data

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_update_card_status_validity
(
  const mmgsdi_event_data_type * event_ptr
)
{
  uint8                       slot_idx    = 0;
  boolean                     result      = FALSE;
  mmgsdi_slot_id_enum_type    mmsgdi_slot = MMGSDI_SLOT_1;

  ASSERT(event_ptr && qmi_uim_global_ptr);

  switch (event_ptr->evt)
  {
    case MMGSDI_CARD_INSERTED_EVT:
      mmsgdi_slot = event_ptr->data.card_inserted.slot;
      break;
    case MMGSDI_SESSION_CHANGED_EVT:
      mmsgdi_slot = event_ptr->data.session_changed.app_info.slot;
      break;
    case MMGSDI_PIN1_EVT:
    case MMGSDI_PIN2_EVT:
    case MMGSDI_UNIVERSAL_PIN_EVT:
      mmsgdi_slot = event_ptr->data.pin.slot;
      break;
    case MMGSDI_CARD_ERROR_EVT:
      mmsgdi_slot = event_ptr->data.card_error.slot;
      break;
    case MMGSDI_CARD_REMOVED_EVT:
      mmsgdi_slot = event_ptr->data.card_removed.slot;
      break;
    case MMGSDI_SUBSCRIPTION_READY_EVT:
      qmi_uim_util_session_id_to_slot(&mmsgdi_slot, event_ptr->session_id);
      break;
    default:
      /* Unhandled events are not used for checking validity */
      UIM_MSG_HIGH_1( "Unhandled Event 0x%x\n", event_ptr->evt);
      return FALSE;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(mmsgdi_slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return FALSE;
  }

  if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
     slot_idx >= qmi_uim_global_ptr->card_state.num_slots)
  {
    return FALSE;
  }

  /* Update the card status validity status */
  if (!qmi_uim_global_ptr->card_state_valid[slot_idx])
  {
    qmi_uim_global_ptr->card_state_valid[slot_idx] = TRUE;
    result = TRUE;
  }

  return result;
} /* qmi_uim_update_card_status_validity */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_CARD_STATUS_INDICATION()

  DESCRIPTION
    This function sends a card status indication to a specific client

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_card_status_indication
(
  uint8                 clid,
  boolean               legacy_card_status,
  boolean               extended_card_status
)
{
  dsm_item_type* indication = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr);

  /* Insert legacy card status item in TLV */
  if (legacy_card_status)
  {
    if (FALSE == qmi_uim_response_card_status(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &indication,
                   &qmi_uim_global_ptr->card_state))
    {
      /* Ignore error... there is nothing we can do about it.
         The event will not be delivered to the client */
      dsm_free_packet(&indication);
      return;
    }

    /* Insert the card status validity TLV */
    if (FALSE == qmi_uim_response_card_status_validity(
                   UIMI_TLV_RESP_TAG_OPTIONAL_2,
                   &indication,
                   (QMI_UIM_LEGACY_MAX_CARD_COUNT < qmi_uim_global_ptr->card_state.num_slots) ?
                    QMI_UIM_LEGACY_MAX_CARD_COUNT : qmi_uim_global_ptr->card_state.num_slots,
                   qmi_uim_global_ptr->card_state_valid))
    {
      UIM_MSG_HIGH_0("Error in constructing card_status_validity TLV");
    }
  }

  /* Insert extended card status item in TLV */
  if (extended_card_status)
  {
    if (FALSE == qmi_uim_response_extended_card_status(
                     UIMI_TLV_RESP_TAG_OPTIONAL_3,
                     &indication,
                     &qmi_uim_global_ptr->card_state,
                     sizeof(qmi_uim_global_ptr->card_state_valid),
                     qmi_uim_global_ptr->card_state_valid))
    {
      /* Ignore error... there is nothing we can do about it.
         The event will not be delivered to the client */
      dsm_free_packet(&indication);
      return;
    }
  }

  /* Insert the card mode in the TLV. Indication is sent
     even if the card_mode TLV is not inserted */
  (void)qmi_uim_response_card_mode(
                 UIMI_TLV_RESP_TAG_OPTIONAL_4,
                 &indication,
                 &qmi_uim_global_ptr->card_state);
				 
  /* Insert ICCID, EID and ATR */
  if(FALSE == qmi_uim_response_extended_card_info(
                UIMI_TLV_RESP_TAG_OPTIONAL_5,
                &indication,
                &qmi_uim_global_ptr->card_state))
  {
    UIM_MSG_HIGH_0("Error in constructing extended_card_info TLV");
    dsm_free_packet(&indication);
	return;
  }

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_CARD_STATUS_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_card_status_indication */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_CARD_STATUS_INDICATION_TO_ALL()

  DESCRIPTION
    This function sends a card status indication to all QMI_UIM clients

  PARAMETERS
    legacy_card_status_update      : if the indication should be sent only to
                                     clients registered for legacy card status
    send_only_to_postponed_clients : if the indication should be sent only to
                                     clients for which it was postponed earlier
    reduced_card_update_required   : if the indication should be sent only to
                                     clients registered for reduced card status indications

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_card_status_indication_to_all
(
  boolean  legacy_card_status_update,
  boolean  send_only_to_postponed_clients,
  boolean  reduced_card_update_required
)
{
  uint8                       j     = 0;
  qmi_uimi_client_state_type* cl_sp = NULL;
  boolean                     legacy_card_status   = FALSE;
  boolean                     extended_card_status = FALSE;

  if(qmi_uim_state_ptr == NULL)
  {
    return;
  }

  for (j = 0; j < UIMQMI_SVC_MAX_CLIDS; j++)
  {
    legacy_card_status   = FALSE;
    extended_card_status = FALSE;

    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[j];
    if(cl_sp == NULL)
    {
      continue;
    }
    if (cl_sp->uim_info.reg_event_type.card_status &&
         legacy_card_status_update )
    {
      legacy_card_status = TRUE;
    }
    if (cl_sp->uim_info.reg_event_type.extended_card_status)
    {
      extended_card_status = TRUE;
    }
    if ( cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
        (extended_card_status || legacy_card_status) )
    {
      if (qmi_uim_global_ptr->mmgsdi_card_slot_session_id[2] ==
          UIM_INVALID_SESSION_ID)
      {
        /* Postpone sending indication until card slot 3 (last card session
           to be opened) session id is valid. This ensures that card status
           indications are not sent to clients with card state PRESENT and
           clients don't send commands for card slot sessions when the sessions
           are not avaiable. */
        cl_sp->uim_info.card_status_ind_postponed = TRUE;
        UIM_MSG_HIGH_1("Postponing indication for client 0x%x",
                       cl_sp->common.clid);
        continue;
      }
      if (send_only_to_postponed_clients &&
          cl_sp->uim_info.card_status_ind_postponed == FALSE)
      {
        /* Send card status indication only to clients for which it was
           postponed earlier. */
        continue;
      }

      /* If the client registers for reduced card status indications and if
         this indication is not in the reduced set then dont send the indication
         to the client */
      if(cl_sp->uim_info.reg_event_type.reduced_card_status_events &&
         reduced_card_update_required == FALSE)
      {
        continue;
      }

      qmi_uim_send_card_status_indication(cl_sp->common.clid,
                                          legacy_card_status,
                                          extended_card_status);
    }
  }
} /* qmi_uim_send_card_status_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SIM_BUSY_INDICATION()

  DESCRIPTION
    This function sends a sim busy indication to a specific client.

  PARAMETERS
    clid   : Client ID

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_sim_busy_indication
(
  uint8                         clid
)
{
  dsm_item_type* indication = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr);

  /* Insert sim busy state in TLV */
  if (FALSE == qmi_uim_response_sim_busy(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &indication,
                   &qmi_uim_global_ptr->card_state))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_SIM_BUSY_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_sim_busy_indication */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SIM_BUSY_INDICATION_TO_ALL()

  DESCRIPTION
    This function sends a sim busy indication to all QMI_UIM clients who have
    registered for sim busy indications.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_sim_busy_indication_to_all
(
  void
)
{
  uint8                         j           = 0;
  qmi_uimi_client_state_type  * cl_sp       = NULL;

  if(qmi_uim_state_ptr == NULL)
  {
    return;
  }

  for (j = 0; j < UIMQMI_SVC_MAX_CLIDS; j++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[j];

    /* Send sim busy if client requested for them during client registration
       (saved in reg_for_sim_busy_events flag) */
    if ( cl_sp != NULL &&
         cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
         cl_sp->uim_info.reg_event_type.sim_busy_events )
    {
      qmi_uim_send_sim_busy_indication(cl_sp->common.clid);
    }
  }
} /* qmi_uim_send_sim_busy_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_NON_PROV_SESSION_CLOSED_IND()

  DESCRIPTION
    This function sends a Session Closed Indication indication to the
    requested client ID.

  PARAMETERS
    clid    : Client ID
    slot_id : MMGSDI slot information
    aid_ptr : AID information

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_non_prov_session_closed_ind
(
  uint8                                   clid,
  mmgsdi_slot_id_enum_type                slot,
  const mmgsdi_static_data_type         * aid_ptr,
  mmgsdi_session_closed_cause_enum_type   cause
)
{
  dsm_item_type * indication = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr && aid_ptr);

  /* Insert the AID in the TLV */
  if (FALSE == qmi_uim_response_aid(
                 UIMI_TLV_RESP_TAG_OPTIONAL_1,
                 &indication,
                 aid_ptr))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert card slot in TLV */
  if (FALSE == qmi_uim_response_slot(
                   UIMI_TLV_TAG_MANDATORY_1,
                   &indication,
                   slot))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert the cause in the TLV. Indication is sent event
     if the cause TLV is not inserted */
  (void)qmi_uim_response_session_closed_cause(
                   UIMI_TLV_RESP_TAG_OPTIONAL_4,
                   &indication,
                   cause);

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_SESSION_CLOSED_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_non_prov_session_closed_ind */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_NON_PROV_SESSION_CLOSED_IND_TO_ALL()

  DESCRIPTION
    This function sends a Session Closed Indication indication to all clients
    which are associated with the non-prov. session being closed.

  PARAMETERS
    index : non-prov. session index in the global array
    cause : Cause for session closure

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_non_prov_session_closed_ind_to_all
(
  uint16                                index,
  mmgsdi_session_closed_cause_enum_type cause
)
{
  uint16                          i            = 0;
  mmgsdi_slot_id_enum_type        slot         = MMGSDI_MAX_SLOT_ID_ENUM;
  qmi_uimi_client_state_type    * client_sp    = NULL;
  uint16                          client_count = 0;
  const mmgsdi_static_data_type * aid_ptr      = NULL;

  ASSERT(qmi_uim_global_ptr);

  if((index >= UIM_MAX_NON_PROV_SESSIONS) ||
     (qmi_uim_global_ptr->non_prov_session[index] == NULL))
  {
    return;
  }

  /* Retrieve the AID and slot info - to be sent as part of the Session
     Closed event */
  aid_ptr = &qmi_uim_global_ptr->non_prov_session[index]->aid;

  switch(qmi_uim_global_ptr->non_prov_session[index]->session_type)
  {
    case MMGSDI_NON_PROV_SESSION_SLOT_1:
      slot = MMGSDI_SLOT_1;
      break;
    case MMGSDI_NON_PROV_SESSION_SLOT_2:
      slot = MMGSDI_SLOT_2;
      break;
    case MMGSDI_NON_PROV_SESSION_SLOT_3:
      slot = MMGSDI_SLOT_3;
      break;
    default:
      return;
  }

  client_count = qmi_uim_global_ptr->non_prov_session[index]->client_info.client_count;

  for (i = 0;
       (i < (uint16)UIM_MAX_NON_PROV_CLIENTS) && (client_count > 0);
       i++)
  {
    client_sp = qmi_uim_global_ptr->non_prov_session[index]->client_info.opening_cl_sp[i];

    /* Check if the Client state is valid */
    if(client_sp == NULL)
    {
      continue;
    }

    client_count--;

    if (client_sp->common.clid != QMI_SVC_CLID_UNUSED)
    {
      /* Send indication to specific client */
      qmi_uim_send_non_prov_session_closed_ind(client_sp->common.clid,
                                               slot,
                                               aid_ptr,
                                               cause);
    }
  }
} /* qmi_uim_send_non_prov_session_closed_ind_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_PROV_SESSION_CLOSED_IND()

  DESCRIPTION
    This function sends a Session Closed Indication indication to the
    requested client ID.

  PARAMETERS
    clid         : Client ID
    slot_id      : MMGSDI slot information
    session_type : Type of prov session
    cause        : Cause for session closure

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_prov_session_closed_ind
(
  uint8                                  clid,
  mmgsdi_slot_id_enum_type               slot,
  mmgsdi_session_type_enum_type          session_type,
  mmgsdi_session_closed_cause_enum_type  cause
)
{
  dsm_item_type * indication = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr);

  /* Insert card slot in TLV */
  if (FALSE == qmi_uim_response_slot(
                   UIMI_TLV_TAG_MANDATORY_1,
                   &indication,
                   slot))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert the session in the TLV */
  if (FALSE == qmi_uim_response_session_type(
                 UIMI_TLV_RESP_TAG_OPTIONAL_3,
                 &indication,
                 session_type))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert the cause in the TLV */
  if (FALSE == qmi_uim_response_session_closed_cause(
                 UIMI_TLV_RESP_TAG_OPTIONAL_4,
                 &indication,
                 cause))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert the file_id in the TLV */
  if (FALSE == qmi_uim_response_session_closed_file_id(
                 UIMI_TLV_RESP_TAG_OPTIONAL_5,
                 &indication,
                 cause))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_SESSION_CLOSED_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_prov_session_closed_ind */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_PROV_SESSION_CLOSED_IND_TO_ALL()

  DESCRIPTION
    This function sends a Session Closed Indication to all clients, who have
    registered for a provisioning session closed event

  PARAMETERS
    session_id : Session id associated with the prov. session
    slot       : MMGSDI slot information
    cause      : Cause for the session closure

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_prov_session_closed_ind_to_all
(
  mmgsdi_session_id_type                session_id,
  mmgsdi_slot_id_enum_type              slot,
  mmgsdi_session_closed_cause_enum_type cause
)
{
  uint16                          i            = 0;
  qmi_uimi_client_state_type    * client_sp    = NULL;
  mmgsdi_session_type_enum_type   session_type = MMGSDI_GW_PROV_PRI_SESSION;
  qmi_error_e_type                result       = QMI_ERR_NONE;
  mmgsdi_static_data_type         aid;

  ASSERT(qmi_uim_state_ptr);

  memset(&aid, 0, sizeof(mmgsdi_static_data_type));

  /* Convert session id in session type */
  result = qmi_uim_util_session_id_to_session_type(&session_type,
                                              &aid,
                                              session_id);
  if (result != QMI_ERR_NONE)
  {
    return;
  }

  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    client_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];
    if(client_sp == NULL)
    {
      continue;
    }

    if ((client_sp->common.clid != QMI_SVC_CLID_UNUSED) &&
        (client_sp->uim_info.reg_event_type.prov_session_close_event))
    {
       /* Send indication to specific client */
       qmi_uim_send_prov_session_closed_ind(client_sp->common.clid,
                                            slot,
                                            session_type,
                                            cause);
    }
  }
} /* qmi_uim_send_prov_session_closed_ind_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SESSION_CLOSED_FOR_SESSIONS_ON_SLOT()

  DESCRIPTION
    This function sends a Session Closed Indication indication to all clients
    which are associated with all the sessions being closed for a slot

  PARAMETERS
    slot       : MMGSDI slot information
    cause      : Cause for the session closure

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_session_closed_indication_for_sessions_on_slot
(
  mmgsdi_slot_id_enum_type               slot_id,
  mmgsdi_session_closed_cause_enum_type  cause
)
{
  uint16                         i                     = 0;
  mmgsdi_session_type_enum_type  non_prov_session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
  mmgsdi_slot_id_enum_type       prov_session_slot_id  = MMGSDI_SLOT_1;
  qmi_error_e_type               result                = QMI_ERR_NONE;

  ASSERT(qmi_uim_global_ptr);

  switch(slot_id)
  {
    case MMGSDI_SLOT_1:
      non_prov_session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
      break;
    case MMGSDI_SLOT_2:
      non_prov_session_type = MMGSDI_NON_PROV_SESSION_SLOT_2;
      break;
    case MMGSDI_SLOT_3:
      non_prov_session_type = MMGSDI_NON_PROV_SESSION_SLOT_3;
      break;
    default:
      return;
  }

  for (i = 0; i < QMI_UIM_MAX_PROV_SESSIONS; i++)
  {
    if(qmi_uim_global_ptr->mmgsdi_gw_session_id[i] != UIM_INVALID_SESSION_ID)
    {
      result = qmi_uim_util_session_id_to_slot(&prov_session_slot_id,
                                               qmi_uim_global_ptr->mmgsdi_gw_session_id[i]);
      if ((result == QMI_ERR_NONE) &&
          (prov_session_slot_id == slot_id))
      {
        qmi_uim_send_prov_session_closed_ind_to_all(
                      qmi_uim_global_ptr->mmgsdi_gw_session_id[i],
                      slot_id, cause);
      }
    }

    if(qmi_uim_global_ptr->mmgsdi_1x_session_id[i] != UIM_INVALID_SESSION_ID)
    {
      result = qmi_uim_util_session_id_to_slot(&prov_session_slot_id,
                                               qmi_uim_global_ptr->mmgsdi_1x_session_id[i]);
      if ((result == QMI_ERR_NONE) &&
          (prov_session_slot_id == slot_id))
      {
        qmi_uim_send_prov_session_closed_ind_to_all(
                      qmi_uim_global_ptr->mmgsdi_1x_session_id[i],
                      slot_id, cause);
      }
    }
  }

  for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
  {
    if (qmi_uim_global_ptr->non_prov_session[i] != NULL &&
        qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id != UIM_INVALID_SESSION_ID &&
        qmi_uim_global_ptr->non_prov_session[i]->session_type == non_prov_session_type)
    {
      qmi_uim_send_non_prov_session_closed_ind_to_all(i, cause);
    }
  }

  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->slot == slot_id &&
        qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id != UIM_INVALID_SESSION_ID)
    {
      qmi_uim_send_apdu_session_closed_ind(i, cause);
    }
  }
}/* qmi_uim_send_session_closed_indication_for_sessions_on_slot */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_ACTIVATION_STATUS_INDICATION()

  DESCRIPTION
    This function sends card activation status indication to a specific client.

  PARAMETERS
    clid              : Client ID
    mmgsdi_slot       : Slot of cooresponding of recovery
    gstk_otasp_status : Status of OTASP activation from GSTK

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_activation_status_indication
(
  uint8                         clid,
  mmgsdi_slot_id_enum_type      mmgsdi_slot,
  gstk_otasp_act_status_enum_type   gstk_otasp_status
)
{
  dsm_item_type               * indication   = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr);

  /* Insert OTASP activation slot in TLV */
  if (FALSE == qmi_uim_response_slot(
                   UIMI_TLV_IND_TAG_MANDATORY_1,
                   &indication,
                   mmgsdi_slot))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Insert OTASP status in TLV */
  if (FALSE == qmi_uim_response_activation_status(
                   UIMI_TLV_IND_TAG_MANDATORY_2,
                   &indication,
                   gstk_otasp_status))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_CARD_ACTIVATION_STATUS_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_activation_status_indication */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_ACTIVATION_STATUS_INDICATION_TO_ALL()

  DESCRIPTION
    This function indicates card activation status to all registered QMI clients.

  PARAMETERS
     uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_activation_status_indication_to_all
(
  mmgsdi_slot_id_enum_type      mmgsdi_slot,
  gstk_otasp_act_status_enum_type   gstk_otasp_status
)
{
  uint8                        client_index = 0;
  qmi_uimi_client_state_type  *cl_sp        = NULL;

  UIM_MSG_HIGH_0("qmi_uim_send_activation_status_indication_to_all");

  for (client_index = 0; client_index < UIMQMI_SVC_MAX_CLIDS; client_index++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[client_index];

    /* Send OTASP status indication if client requested for them
       during client registration */
    if ( cl_sp != NULL &&
         cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
         cl_sp->uim_info.reg_event_type.reg_for_activation_status_event)
    {
      qmi_uim_send_activation_status_indication(cl_sp->common.clid,
                                                mmgsdi_slot,
                                                gstk_otasp_status);
    }
  }
} /* qmi_uim_send_activation_status_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_OTASP_STATUS_INDICATION

  DESCRIPTION
    This function processes the OTASP status indication from GSTK. It indicates
    that the card activation status to all registered QMI clients.

  PARAMETERS
     uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_otasp_status_indication
(
  qmi_uim_message_type * uim_message_ptr
)
{
  mmgsdi_slot_id_enum_type     mmgsdi_slot  = MMGSDI_MAX_SLOT_ID_ENUM;

  UIM_MSG_HIGH_0("qmi_uim_process_otasp_status_indication");

  ASSERT(uim_message_ptr);

  if(qmi_uim_util_gstk_slot_id_to_mmgsdi_slot_id(uim_message_ptr->data.otasp_status_ind.slot,
                                                 &mmgsdi_slot) != QMI_ERR_NONE)
  {
    return;
  }

  qmi_uim_send_activation_status_indication_to_all(mmgsdi_slot,
                                                   uim_message_ptr->data.otasp_status_ind.status);
} /* qmi_uim_process_otasp_status_indication */


/*===========================================================================
  FUNCTION  QMI_UIM_SEND_PURGE_TEMPORARY_IDENTITY_IND()

  DESCRIPTION
    This function sends the purge temporary identity indication

  PARAMETERS
    clid                    : client identity
    temporary_identity_mask : Temporary identity mask
    mmgsdi_status           : MMGSDI status
    session_id              : Session Id

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_purge_temporary_identity_ind
(
  uint8                                     clid,
  qmi_uim_tlv_temporary_identity_mask_type  temporary_identity_mask,
  mmgsdi_return_enum_type                   mmgsdi_status,
  mmgsdi_session_id_type                    session_id
)
{
  dsm_item_type                           * indication               = NULL;
  qmi_error_e_type                          errval                   = QMI_ERR_NONE;
  qmi_result_e_type                         result                   = QMI_RESULT_SUCCESS;
  boolean                                   retval                   = TRUE;
  mmgsdi_session_type_enum_type             session_type             = MMGSDI_GW_PROV_PRI_SESSION;
  mmgsdi_static_data_type                   aid                      = {0};

  errval = qmi_uim_util_session_id_to_session_type(&session_type, &aid, session_id);

  if (errval != QMI_ERR_NONE)
  {
    return;
  }

  if (FALSE == qmi_uim_response_session_type(UIMI_TLV_IND_TAG_MANDATORY_1,
                                             &indication,
                                             session_type))
  {
    dsm_free_packet(&indication);
    return;
  }

  /* Convert MMGSDI status to QMI err code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  /* Insert result item in the TLV */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&indication, result, errval);

  if (FALSE == retval)
  {
    dsm_free_packet(&indication);
    return;
  }

  if (FALSE == qmi_uim_response_temporary_identity_mask(UIMI_TLV_IND_TAG_MANDATORY_3,
                                                        &indication,
                                                        temporary_identity_mask))
  {
    UIM_MSG_ERR_0("Could not insert temporary identity TLV, not sending indication");
    dsm_free_packet(&indication);
    return;
  }

  retval = uimqmi_send_indication(clid,
                                  UIMI_CMD_VAL_PURGE_TEMPORARY_IDENTITY,
                                  indication,
                                  QMUX_SERVICE_UIM);

  if (FALSE == retval)
  {
    dsm_free_packet(&indication);
  }
} /* qmi_uim_send_purge_temporary_identity_ind */


/*===========================================================================
  FUNCTION  QMI_UIM_SEND_PURGE_TEMPORARY_IDENTITY_IND_TO_ALL()

  DESCRIPTION
    This function sends the purge temporary identity indication to all the clients.

  PARAMETERS
    temporary_identity_mask : Temporary identity mask
    mmgsdi_status           : MMGSDI status
    session_id              : Session Id

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_purge_temporary_identity_ind_to_all
(
  uint16                   purge_guti_mask,
  mmgsdi_return_enum_type  mmgsdi_status,
  mmgsdi_session_id_type   session_id
)
{
  qmi_uimi_client_state_type              * cl_sp                    = NULL;
  uint8                                     client_index             = 0;
  qmi_uim_tlv_temporary_identity_mask_type  temporary_identity_mask;
  uint16                                    purge_guti_mask_index    = 0;

  memset(&temporary_identity_mask, 0x00, sizeof(qmi_uim_tlv_temporary_identity_mask_type));

  if (qmi_uim_util_session_id_to_index(&purge_guti_mask_index, session_id) != QMI_ERR_NONE)
  {
    return;
  }

  for (client_index = 0; client_index < UIMQMI_SVC_MAX_CLIDS; client_index++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[client_index];
    if(cl_sp == NULL ||
       cl_sp->common.clid == QMI_SVC_CLID_UNUSED ||
       (cl_sp->uim_info.purge_guti_mask & purge_guti_mask) == 0)
    {
      continue;
    }

    if(purge_guti_mask & (1 << purge_guti_mask_index))
    {
      temporary_identity_mask.guti =  TRUE;
    }
    else
    {
      continue;
    }

    qmi_uim_send_purge_temporary_identity_ind(cl_sp->common.clid,
                                              temporary_identity_mask,
                                              mmgsdi_status,
                                              session_id);

    /* Reset the temporary identity bit after sending the indication for
       purge temporary identity request */
    cl_sp->uim_info.purge_guti_mask &= ~purge_guti_mask;
  }
}/* qmi_uim_send_purge_temporary_identity_ind_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_INTERNAL_ERROR_RESP()

  DESCRIPTION
    Composes the error response with internal error code. This function
    is used only to send internal errors in asynchronous case. In most
    cases, the handler of each MMGSDI confirmation takes care of it,
    but this function is useful in case there is an error opening a non
    provisioning sessions: in this case, the intended MMGSDI function
    (read, write, PIN,...) is not executed at all because the session
    does not exists, but a response to QMI client is still required.

  PARAMETERS
    cmd_buf_ptr : pointer to command buffer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_internal_error_resp
(
  qmi_cmd_buf_type *  cmd_buf_ptr
)
{
  dsm_item_type *         response  = NULL;

  ASSERT(cmd_buf_ptr);

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_ptr);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_ptr,
                                      response,
                                      QMI_ERR_INTERNAL,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_internal_error_resp */


/*===========================================================================
  FUNCTION QMI_UIM_INTERNAL_SUCCESS_RESP()

  DESCRIPTION
    Composes the success response for those commands that only require
    the status in the response and no other TLVs. This function
    is used only to send success in asynchronous case. In most
    cases, the handler of each MMGSDI confirmation takes care of it,
    but this function is useful in case there is an error opening a non
    provisioning sessions.

  PARAMETERS
    cmd_buf_ptr : pointer to command buffer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_internal_success_resp
(
  qmi_cmd_buf_type *  cmd_buf_ptr
)
{
  dsm_item_type *         response  = NULL;

  ASSERT(cmd_buf_ptr);

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_ptr);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_ptr,
                                      response,
                                      QMI_ERR_NONE,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_internal_success_resp */


/*===========================================================================
  FUNCTION QMI_UIM_PENDING_REQ_ERROR_IND()

  DESCRIPTION
    Composes the error indications for those commands that are pending an
    indication but could not be processed for some reason and hence we
    need to send an error indication to the client

  PARAMETERS
    request_ptr : pointer to the pending request type

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_pending_req_error_ind (
  const qmi_uimi_pending_request_type    * request_ptr
)
{
  uint8                           i                        = 0;
  qmi_uim_tlv_uint32_type       * ind_token_ptr            = NULL;
  dsm_item_type                 * response_ptr             = NULL;

  ASSERT(request_ptr != NULL);

  /* Find the token that came as part of the request */
  for (i = 0; i < request_ptr->tlv_data_count; i++)
  {
    if (!request_ptr->tlv_data_ptr[i].present)
    {
      continue;
    }

    switch (request_ptr->tlv_data_ptr[i].type)
    {
      case UIMI_TLV_TYPE_IND_TOKEN:
        ind_token_ptr = &request_ptr->tlv_data_ptr[i].data.ind_token;
        break;
      default:
        break;
    }
  }

  if (ind_token_ptr == NULL)
  {
    UIM_MSG_LOW_0("Could not find token, not sending indication");
    return;
  }

  if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                               &response_ptr,
                                               ind_token_ptr->value))
  {
    UIM_MSG_LOW_0("Could not insert ind_token TLV, not sending indication");
    dsm_free_packet(&response_ptr);
    return;
  }

  if (FALSE == qmi_svc_put_result_tlv(&response_ptr,
                                      QMI_RESULT_FAILURE,
                                      QMI_ERR_INTERNAL))
  {
    dsm_free_packet(&response_ptr);
    return;
  }

  if (FALSE == uimqmi_send_indication(request_ptr->clid,
                                      (uint16)request_ptr->command_id,
                                      response_ptr,
                                      QMUX_SERVICE_UIM))
  {
    UIM_MSG_LOW_0("Unable to send error indication for pending req");
    dsm_free_packet(&response_ptr);
  }
} /* qmi_uim_pending_req_error_ind */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_PENDING_REQUEST()

  DESCRIPTION
    Executes a specific pending requests for a non provisionin application.

  PARAMETERS
    request_ptr : pointer to the pending request
    session_id  : session id

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_execute_pending_request
(
  const qmi_uimi_pending_request_type * request_ptr,
  mmgsdi_session_id_type                session_id
)
{
  uint8                                      i                        = 0;
  qmi_error_e_type                           errval                   = QMI_ERR_NONE;
  qmi_uim_requested_encrytion_type           encryption_status        = QMI_UIM_NO_ENCRYPTION;
  mmgsdi_access_type                         mmgsdi_access;
  qmi_uim_tlv_file_id_type *                 file_id_ptr              = NULL;
  qmi_uim_tlv_read_transparent_type *        read_transparent_ptr     = NULL;
  qmi_uim_tlv_read_record_type *             read_record_ptr          = NULL;
  qmi_uim_tlv_write_transparent_type *       write_transparent_ptr    = NULL;
  qmi_uim_tlv_write_record_type *            write_record_ptr         = NULL;
  qmi_uim_tlv_set_pin_protection_type *      set_pin_protection_ptr   = NULL;
  qmi_uim_tlv_verify_pin_type *              verify_pin_ptr           = NULL;
  qmi_uim_tlv_unblock_pin_type *             unblock_pin_ptr          = NULL;
  qmi_uim_tlv_change_pin_type *              change_pin_ptr           = NULL;
  qmi_uim_tlv_authenticate_type *            authenticate_ptr         = NULL;
  qmi_uim_tlv_decrypted_pin_type *           decrypted_pin_ptr        = NULL;
  qmi_uim_tlv_read_record_last_type *        read_record_last_ptr     = NULL;
  qmi_uim_tlv_uint32_type *                  ind_token_ptr            = NULL;
  qmi_uim_tlv_register_refresh_type *        register_refresh_ptr     = NULL;
  qmi_uim_tlv_boolean_type *                 encrypted_ptr            = NULL;
  qmi_uim_tlv_boolean_type *                 register_refresh_all_ptr = NULL;
  qmi_uim_tlv_boolean_type *                 vote_for_init_ptr        = NULL;
  qmi_uim_tlv_file_status_type *             file_status_ptr          = NULL;
  qmi_uim_tlv_increase_type *                increase_ptr             = NULL;
  qmi_uim_tlv_status_cmd_mode_type *         status_mode_ptr          = NULL;
  qmi_uim_tlv_status_cmd_resp_type *         status_resp_ptr          = NULL;
  qmi_uim_tlv_boolean_type *                 encrypted_ext_ptr        = NULL;
  qmi_uim_tlv_search_pattern_type *          search_pattern_ptr       = NULL;
  qmi_uim_tlv_search_data_type *             search_data_ptr          = NULL;

  ASSERT(request_ptr && request_ptr->tlv_data_ptr && request_ptr->tlv_data_count > 0);

  /* Find the TLVs. Let's consider only those that we strictly need
     and ignore the others */
  for (i = 0; i < request_ptr->tlv_data_count; i++)
  {
    /* Skip TLVs that were not present in the request */
    if (!request_ptr->tlv_data_ptr[i].present)
    {
      continue;
    }

    switch (request_ptr->tlv_data_ptr[i].type)
    {
      case UIMI_TLV_TYPE_SESSION_INFORMATION:
        /* We can ignore session information, because we already have the
           session id */
        break;
      case UIMI_TLV_TYPE_FILE_ID:
        file_id_ptr = &request_ptr->tlv_data_ptr[i].data.file_id;
        break;
      case UIMI_TLV_TYPE_READ_TRANSPARENT:
        read_transparent_ptr = &request_ptr->tlv_data_ptr[i].data.read_transparent;
        break;
      case UIMI_TLV_TYPE_READ_RECORD:
        read_record_ptr = &request_ptr->tlv_data_ptr[i].data.read_record;
        break;
      case UIMI_TLV_TYPE_WRITE_TRANSPARENT:
        write_transparent_ptr = &request_ptr->tlv_data_ptr[i].data.write_transparent;
        break;
      case UIMI_TLV_TYPE_WRITE_RECORD:
        write_record_ptr = &request_ptr->tlv_data_ptr[i].data.write_record;
        break;
      case UIMI_TLV_TYPE_SET_PIN_PROTECTION:
        set_pin_protection_ptr = &request_ptr->tlv_data_ptr[i].data.set_pin_protection;
        break;
      case UIMI_TLV_TYPE_VERIFY_PIN:
        verify_pin_ptr = &request_ptr->tlv_data_ptr[i].data.verify_pin;
        break;
      case UIMI_TLV_TYPE_UNBLOCK_PIN:
        unblock_pin_ptr = &request_ptr->tlv_data_ptr[i].data.unblock_pin;
        break;
      case UIMI_TLV_TYPE_CHANGE_PIN:
        change_pin_ptr = &request_ptr->tlv_data_ptr[i].data.change_pin;
        break;
      case UIMI_TLV_TYPE_AUTHENTICATE:
        authenticate_ptr = &request_ptr->tlv_data_ptr[i].data.authenticate;
        break;
      case UIMI_TLV_TYPE_DECRYPTED_PIN:
        decrypted_pin_ptr = &request_ptr->tlv_data_ptr[i].data.decrypted_pin;
        break;
      case UIMI_TLV_TYPE_READ_RECORD_LAST:
        read_record_last_ptr = &request_ptr->tlv_data_ptr[i].data.read_record_last;
        break;
      case UIMI_TLV_TYPE_IND_TOKEN:
        ind_token_ptr = &request_ptr->tlv_data_ptr[i].data.ind_token;
        break;
      case UIMI_TLV_TYPE_REGISTER_REFRESH:
        register_refresh_ptr = &request_ptr->tlv_data_ptr[i].data.register_refresh;
        break;
      case UIMI_TLV_TYPE_ENCRYPTED:
        encrypted_ptr = &request_ptr->tlv_data_ptr[i].data.encrypted;
        break;
      case UIMI_TLV_TYPE_REGISTER_FOR_REFRESH:
        register_refresh_all_ptr = &request_ptr->tlv_data_ptr[i].data.refresh_registration;
        break;
      case UIMI_TLV_TYPE_REFRESH_VOTE_FOR_INIT:
        vote_for_init_ptr = &request_ptr->tlv_data_ptr[i].data.refresh_vote_for_init;
        break;
      case UIMI_TLV_TYPE_FILE_STATUS:
        file_status_ptr = &request_ptr->tlv_data_ptr[i].data.file_status;
        break;
      case UIMI_TLV_TYPE_INCREASE:
        increase_ptr = &request_ptr->tlv_data_ptr[i].data.increase_data;
        break;
      case UIMI_TLV_TYPE_STATUS_CMD_MODE:
        status_mode_ptr = &request_ptr->tlv_data_ptr[i].data.status_cmd_mode;
        break;
      case UIMI_TLV_TYPE_STATUS_CMD_RESP:
        status_resp_ptr = &request_ptr->tlv_data_ptr[i].data.status_cmd_resp;
        break;
      case UIMI_TLV_TYPE_ENCRYPTED_EXT:
        encrypted_ext_ptr = &request_ptr->tlv_data_ptr[i].data.encrypted_ext;
        break;
      case UIMI_TLV_TYPE_SEARCH_PATTERN:
        search_pattern_ptr = &request_ptr->tlv_data_ptr[i].data.search_pattern;
        break;
      case UIMI_TLV_TYPE_SEARCH_DATA:
        search_data_ptr = &request_ptr->tlv_data_ptr[i].data.search_data;
        break;
      default:
        /* Ignore other tags */
        break;
    }
  }

  UIM_MSG_HIGH_1("Execute pending request: 0x%x", request_ptr->command_id);

  switch(request_ptr->command_id)
  {
    case UIMI_CMD_VAL_READ_TRANSPARENT:
      ASSERT(file_id_ptr && read_transparent_ptr);

      /* We cannot process both partial encryption and complete encryption read requests */
      if(encrypted_ext_ptr && encrypted_ptr)
      {
        errval = QMI_ERR_MALFORMED_MSG;
        break;
      }

      if (encrypted_ptr && encrypted_ptr->value)
      {
        encryption_status = QMI_UIM_DATA_ONLY_ENCRYPTION;
      }
      else if (encrypted_ext_ptr && encrypted_ext_ptr->value)
      {
        encryption_status = QMI_UIM_COMPLETE_ENCRYPTION;
      }

      errval = qmi_uim_execute_read_transparent(
                         request_ptr->command_id,
                         session_id,
                         file_id_ptr->file_access,
                         read_transparent_ptr->offset,
                         read_transparent_ptr->length,
                         encryption_status,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_READ_RECORD:
      ASSERT(file_id_ptr && read_record_ptr);

      errval = qmi_uim_execute_read_record(
                         session_id,
                         file_id_ptr->file_access,
                         read_record_ptr->record,
                         (read_record_last_ptr != NULL) ?
                           read_record_last_ptr->last_record :
                           read_record_ptr->record,
                         read_record_ptr->length,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_WRITE_TRANSPARENT:
      ASSERT(file_id_ptr && write_transparent_ptr);

      errval = qmi_uim_execute_write_transparent(
                         session_id,
                         file_id_ptr->file_access,
                         write_transparent_ptr->offset,
                         write_transparent_ptr->data,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_WRITE_RECORD:
      ASSERT(file_id_ptr && write_record_ptr);

      errval = qmi_uim_execute_write_record(
                         session_id,
                         file_id_ptr->file_access,
                         write_record_ptr->record,
                         write_record_ptr->data,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_GET_FILE_ATTRIBUTES:
      ASSERT(file_id_ptr);

      errval = qmi_uim_execute_get_file_attributes(
                         session_id,
                         file_id_ptr->file_access,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_SET_PIN_PROTECTION:
      ASSERT(set_pin_protection_ptr);

      errval = qmi_uim_execute_set_pin_protection(
                         session_id,
                         set_pin_protection_ptr->pin_enable,
                         set_pin_protection_ptr->pin_id,
                         set_pin_protection_ptr->pin_data,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_VERIFY_PIN:
      ASSERT(verify_pin_ptr);

      errval = qmi_uim_execute_verify_pin(
                         session_id,
                         verify_pin_ptr->pin_id,
                         (decrypted_pin_ptr != NULL && qmi_uim_global_ptr->silent_pin1_supported) ?
                           decrypted_pin_ptr->decrypted_pin_data :
                           verify_pin_ptr->pin_data,
                         (decrypted_pin_ptr != NULL && qmi_uim_global_ptr->silent_pin1_supported) ?
                           TRUE : FALSE,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_UNBLOCK_PIN:
      ASSERT(unblock_pin_ptr);

      errval = qmi_uim_execute_unblock_pin(
                         session_id,
                         unblock_pin_ptr->pin_id,
                         unblock_pin_ptr->puk_data,
                         unblock_pin_ptr->pin_data,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_CHANGE_PIN:
      ASSERT(change_pin_ptr);

      errval = qmi_uim_execute_change_pin(
                         session_id,
                         change_pin_ptr->pin_id,
                         change_pin_ptr->old_pin_data,
                         change_pin_ptr->new_pin_data,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_AUTHENTICATE:
      ASSERT(authenticate_ptr);

      errval = qmi_uim_execute_authenticate(
                         session_id,
                         authenticate_ptr->auth_context,
                         authenticate_ptr->auth_data,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_REFRESH_REGISTER:
      ASSERT(register_refresh_ptr);

      errval = qmi_uim_execute_refresh_register(
                         session_id,
                         register_refresh_ptr->register_op,
                         register_refresh_ptr->vote_for_init,
                         register_refresh_ptr->file_list_ptr,
                         request_ptr->cmd_buf_ptr);
      /* Register for refresh requires an immediate response also in
         case of success. The error case is already handled below */
      if (errval == QMI_ERR_NONE)
      {
        qmi_uim_internal_success_resp(request_ptr->cmd_buf_ptr);
        return;
      }
      break;

    case UIMI_CMD_VAL_REFRESH_REGISTER_ALL:
      ASSERT(register_refresh_all_ptr && vote_for_init_ptr);

      errval = qmi_uim_execute_refresh_register_all(
                            session_id,
                            register_refresh_all_ptr->value,
                            vote_for_init_ptr->value,
                            request_ptr->cmd_buf_ptr);
      /* Register for refresh all requires an immediate response also in
         case of success. The error case is already handled below */
      if (errval == QMI_ERR_NONE)
      {
        qmi_uim_internal_success_resp(request_ptr->cmd_buf_ptr);
        return;
      }
      break;

    case UIMI_CMD_VAL_SET_FILE_STATUS:
      ASSERT(file_id_ptr && file_status_ptr);

      errval = qmi_uim_execute_set_file_status(session_id,
                                               file_id_ptr->file_access,
                                               file_status_ptr->file_status,
                                               request_ptr->cmd_buf_ptr,
                                               request_ptr->clid);
      break;

    case UIMI_CMD_VAL_INCREASE:
      ASSERT(file_id_ptr && increase_ptr);

      errval = qmi_uim_execute_increase(
                         session_id,
                         file_id_ptr->file_access,
                         increase_ptr->data,
                         ind_token_ptr ? &ind_token_ptr->value : NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_SEND_STATUS:
      /* No ASSERT required */
      errval = qmi_uim_execute_status(session_id,
                                      (status_mode_ptr != NULL) ?
                                        status_mode_ptr->mode :
                                        MMGSDI_STATUS_APP_NO_INDICATION,
                                      (status_resp_ptr != NULL) ?
                                        status_resp_ptr->resp :
                                        MMGSDI_STATUS_NO_DATA,
                                      request_ptr->cmd_buf_ptr,
                                      request_ptr->clid);
      break;

    case UIMI_CMD_VAL_GET_GBA_IMPI:
      if (encrypted_ptr && encrypted_ptr->value)
      {
        encryption_status = QMI_UIM_DATA_ONLY_ENCRYPTION;
      }

      memset(&mmgsdi_access, 0x00, sizeof(mmgsdi_access));

      /* Read EF IMPI */
      mmgsdi_access.access_method = MMGSDI_EF_ENUM_ACCESS;
      mmgsdi_access.file.file_enum = MMGSDI_ISIM_IMPI;

      errval = qmi_uim_execute_read_transparent(
                         request_ptr->command_id,
                         session_id,
                         mmgsdi_access,
                         0,
                         0,
                         encryption_status,
                         NULL,
                         request_ptr->cmd_buf_ptr,
                         request_ptr->clid);
      break;

    case UIMI_CMD_VAL_SEARCH_RECORD:
      ASSERT(file_id_ptr && search_pattern_ptr);

      errval = qmi_uim_execute_search_record(
                       session_id,
                       file_id_ptr->file_access,
                       search_pattern_ptr->pattern,
                       (search_data_ptr != NULL) ?
                         search_data_ptr->first_record_num : QMI_UIM_RECORD_1,
                       (search_data_ptr != NULL) ?
                         search_data_ptr->direction : MMGSDI_SEARCH_FORWARD_FROM_REC_NUM,
                       ind_token_ptr ? &ind_token_ptr->value : NULL,
                       request_ptr->cmd_buf_ptr,
                       request_ptr->clid);
      break;

    default:
      /* We should not receive any other command than in the
         list above. Anyway, handle that command sending an
         internal error */
      errval = QMI_ERR_INTERNAL;
      break;
  }

  /* In case of error, send response. In case of success, this will be
     done in the MMGSDI callback */
  if (errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_2("Operation 0x%x on non prov application failed: 0x%x",
                  request_ptr->command_id, errval);
    if (request_ptr->cmd_buf_ptr != NULL)
    {
      qmi_uim_internal_error_resp(request_ptr->cmd_buf_ptr);
    }
    else
    {
      qmi_uim_pending_req_error_ind(request_ptr);
    }
  }
} /* qmi_uim_execute_pending_request */


/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_ALL_PENDING_REQUESTS()

  DESCRIPTION
    Executes all pending requests for a non provisionin application. The
    function might be used also to send the ERROR_INTERNAL code to the
    application, when required.

  PARAMETERS
    session_id  : session id
    execute     : specify if the request needs to be executes (TRUE)
                  or simply deleted (FALSE)

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_execute_all_pending_requests
(
  mmgsdi_session_id_type  session_id,
  boolean                 execute

)
{
  uint16                          i           = 0;
  uint16                          index       = UIM_INVALID_APP_INDEX;
  qmi_uimi_pending_request_type * next_ptr    = NULL;
  qmi_uimi_pending_request_type * current_ptr = NULL;

  ASSERT(qmi_uim_global_ptr);

  /* Find the non provisioning application with the given
     session id */
  for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
  {
    if (qmi_uim_global_ptr->non_prov_session[i] != NULL &&
        qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id == session_id)
    {
      index = i;
      break;
    }
  }

  if (index == UIM_INVALID_APP_INDEX)
  {
    UIM_MSG_ERR_1("Unable to execute pending requests for session id 0x%x",
                  session_id);
    return;
  }

  /* Set the global list head pointer to NULL */
  current_ptr = qmi_uim_global_ptr->non_prov_session[index]->pending_req_ptr;
  qmi_uim_global_ptr->non_prov_session[index]->pending_req_ptr = NULL;

  /* Loop thru all the pending requests & free the nodes */
  while (current_ptr != NULL)
  {
    /* First check if the requested client is still valid, the pending request
       is dropped if the Client ID is no longer valid */
    if (TRUE == qmi_uim_util_check_clid(
                  current_ptr->clid,
                  current_ptr->client_reg_count))
    {
      if (execute)
      {
        qmi_uim_execute_pending_request(current_ptr, session_id);
      }
      else
      {
        /* Send internal error for the pending request */
        qmi_cmd_buf_type* cmd_buf_ptr = (qmi_cmd_buf_type*)current_ptr->cmd_buf_ptr;
        if (cmd_buf_ptr)
        {
          qmi_uim_internal_error_resp(cmd_buf_ptr);
        }
        else
        {
          qmi_uim_pending_req_error_ind(current_ptr);
        }
      }
    }

    /* Free the TLV */
    if (current_ptr->tlv_data_ptr != NULL)
    {
      qmi_uim_util_free_tlv_content(current_ptr->tlv_data_ptr,
                                    current_ptr->tlv_data_count);

      uimqmi_free(current_ptr->tlv_data_ptr);
      current_ptr->tlv_data_ptr = NULL;
    }

    /* Remove the current node & move to the next node */
    next_ptr = current_ptr->next_ptr;
    uimqmi_free(current_ptr);
    current_ptr = next_ptr;
  }
} /* qmi_uim_execute_all_pending_requests */


/*===========================================================================
  FUNCTION QMI_UIM_REMOVE_NON_PROVISIONING_SESSION()

  DESCRIPTION
    Removes a non provisioning session from the global array, cancelling
    all the pending requests.

  PARAMETERS
    session_id           : session id
    send_ind             : Specify if a QMI_UIM_SESSION_CLOSED_IND needs to be sent
                           to the clients accessing the session (TRUE) or not (FALSE)
    session_closed_cause : Cause for session closure

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_remove_non_provisioning_session
(
  mmgsdi_session_id_type                session_id,
  boolean                               send_ind,
  mmgsdi_session_closed_cause_enum_type session_closed_cause
)
{
  uint16  i  = 0;

  ASSERT(qmi_uim_global_ptr);

  for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
  {
    if (qmi_uim_global_ptr->non_prov_session[i] != NULL &&
        qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id == session_id)
    {
      /* Remove refresh data of the non prov session */
      qmi_uim_remove_refresh_registration(session_id);

      /* Complete pending requests */
      qmi_uim_execute_all_pending_requests(session_id, FALSE);

      /* Send Session Closed Indication to Clients */
      if(send_ind)
      {
        qmi_uim_send_non_prov_session_closed_ind_to_all(i, session_closed_cause);
      }

      uimqmi_free(qmi_uim_global_ptr->non_prov_session[i]);
      qmi_uim_global_ptr->non_prov_session[i] = NULL;

      break;
    }
  }
} /* qmi_uim_remove_non_provisioning_session */


/*===========================================================================
  FUNCTION QMI_UIM_REMOVE_NON_PROVISIONING_AND_APDU_SESSION()

  DESCRIPTION
    Removes all non provisioning sessions info previously opened that belong
    to the provided slot from the global array.

  PARAMETERS
    slot_id  : slot id

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_remove_non_provisioning_and_apdu_session
(
  mmgsdi_slot_id_enum_type      slot_id
)
{
  uint16                          i            = 0;
  mmgsdi_session_type_enum_type   session_type = MMGSDI_MAX_SESSION_TYPE_ENUM;

  ASSERT(qmi_uim_global_ptr);

  /* Check slot id */
  switch(slot_id)
  {
    case MMGSDI_SLOT_1:
      session_type = MMGSDI_NON_PROV_SESSION_SLOT_1;
      break;
    case MMGSDI_SLOT_2:
      session_type = MMGSDI_NON_PROV_SESSION_SLOT_2;
      break;
    case MMGSDI_SLOT_3:
      session_type = MMGSDI_NON_PROV_SESSION_SLOT_3;
      break;
    default:
      return;
  }

  /* Clear info of all non-provisioning sessions that belongs to the slot */
  for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
  {
    if (qmi_uim_global_ptr->non_prov_session[i] != NULL &&
        qmi_uim_global_ptr->non_prov_session[i]->session_type == session_type)
    {
      /* QMI_UIM_SESSION_CLOSED_IND shall not be sent out since the session
         clean-up was not trigerred by an MMGSDI_SESSION_CLOSE_EVT */
      qmi_uim_remove_non_provisioning_session(
          qmi_uim_global_ptr->non_prov_session[i]->mmgsdi_session_id,
          FALSE,
          MMGSDI_SESSION_CLOSED_CAUSE_UNKNOWN);
    }
  }

  /* Clear info of all APDU sessions that belongs to the slot */
  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->slot == slot_id)
    {
      /* QMI_UIM_SESSION_CLOSED_IND shall not be sent out since the session
         clean-up was not trigerred by an MMGSDI_SESSION_CLOSE_EVT */
      qmi_uim_remove_apdu_session(
          qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id,
          FALSE,
          MMGSDI_SESSION_CLOSED_CAUSE_UNKNOWN);
    }
  }
} /* qmi_uim_remove_non_provisioning_and_apdu_session */


/*===========================================================================
  FUNCTION QMI_UIM_GET_HALT_SUBSCRIPTION_VALUE

  DESCRIPTION
    Fetches the halt subscription config value from EFS. If config file is
    not found, a default value of FALSE is returned.

  PARAMETERS
    None

  RETURN VALUE
    boolean          : TRUE if the config value was 1, FALSE in all
                       other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_get_halt_subscription_value
(
  void
)
{
  boolean    halt_sub_config        = FALSE;

  if (uim_common_efs_read(
        UIM_COMMON_EFS_MMGSDI_HALT_SUBSCRIPTION,
        UIM_COMMON_EFS_ITEM_FILE_TYPE,
        UIM_COMMON_EFS_DEVICE,
        &halt_sub_config,
        sizeof(boolean)) != UIM_COMMON_EFS_SUCCESS)
  {
    /* In case of failure, explicitly set it to FALSE */
    halt_sub_config = FALSE;
  }

  UIM_MSG_HIGH_1("Halt subscription config set to 0x%x", halt_sub_config);

  return halt_sub_config ? TRUE : FALSE;
} /* qmi_uim_get_halt_subscription_value */


/*===========================================================================
  FUNCTION QMI_UIM_GET_ENCRYPTED_SUB_OK_VALUE

  DESCRIPTION
    Fetches the encrypted sub ok config value from EFS. If config file is
    not found, a default value of FALSE is returned.

  PARAMETERS
    None

  RETURN VALUE
    boolean          : TRUE if the config value was 1, FALSE in all
                       other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_get_encrypted_sub_ok_value
(
  void
)
{
  boolean    encrypt_sub_ok       = FALSE;

  /* Get halt subscription config value from EFS */
  if (uim_common_efs_read(
        UIM_COMMON_EFS_MMGSDI_ENCRYPT_SUB_OK,
        UIM_COMMON_EFS_ITEM_FILE_TYPE,
        UIM_COMMON_EFS_DEVICE,
        &encrypt_sub_ok,
        sizeof(boolean)) != UIM_COMMON_EFS_SUCCESS)
  {
    /* In case of failure, explicitly set it to FALSE */
    encrypt_sub_ok = FALSE;
  }

  UIM_MSG_HIGH_1("Encrypted sub_ok config set to 0x%x", encrypt_sub_ok);

  return encrypt_sub_ok ? TRUE : FALSE;
} /* qmi_uim_get_encrypted_sub_ok_value */


/*===========================================================================
  FUNCTION QMI_UIM_IS_APDU_REQUEST_REJECTED()

  DESCRIPTION
    If APDU security restrictions are enabled on the device, this function
    parses the incoming APDU request and checks the below criteria to
    determine if it should be rejected:
    1. if it is either a MANAGE CHANNEL or
    2. a SELECT  using the DF name (P1 = 0x04) request or
    3. logical channel in request doesnt belongs to the requesting client

  PARAMETERS
    slot_id             : Slot id
    apdu_data           : Incoming APDU request
    logical_ch_tlv_ptr  : Logical channel TLV pointer
    cl_sp               : Client state pointer

  RETURN VALUE
    Boolean indicating if the request is to be rejected or not.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_is_apdu_request_rejected
(
  mmgsdi_slot_id_enum_type      slot,
  mmgsdi_send_apdu_data_type    apdu_data,
  qmi_uim_tlv_item_type       * logical_ch_tlv_ptr,
  qmi_uimi_client_state_type  * cl_sp,
  boolean                     * default_channel_streaming_ptr
)
{
  uint16                    i               = 0;
  uint8                     logical_channel = 0;
  uint16                    channel_index   = UIM_INVALID_APP_INDEX;
  uint8                     card_index      = 0;

  ASSERT(qmi_uim_global_ptr && logical_ch_tlv_ptr && default_channel_streaming_ptr);

  if ((apdu_data.data_ptr == NULL) || (apdu_data.data_len < 4))
  {
    UIM_MSG_ERR_0("ADPU rejected due to invalid size");
    return TRUE;
  }

  *default_channel_streaming_ptr = FALSE;

  if (uimqmi_mmgsdi_slot_to_slot_index(slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) == QMI_ERR_NONE)
  {
    /* Don't check APDU restrictions for non telecom card */
    if (qmi_uim_global_ptr->card_state.card[card_index]->is_non_telecom_card)
    {
      return FALSE;
    }
  }

  /* Get the logical channel on which this APDU is being intended
     to be sent */
  if (logical_ch_tlv_ptr->present)
  {
    logical_channel = logical_ch_tlv_ptr->data.logical_channel.logical_channel;
  }
  else
  {
    /* Handle coding of extended logical channels also */
    if(apdu_data.data_ptr[0] & 0x40)
    {
      logical_channel = (apdu_data.data_ptr[0] & 0x0F) + 4;
    }
    else
    {
      logical_channel = apdu_data.data_ptr[0] & 0x03;
    }
  }

  UIM_MSG_MED_1("logical_channel: 0x%x", logical_channel);

  /* Block AUTHENTICATE APDUs based on authenticate security restrictions */
  if (apdu_data.data_ptr[1] == QMI_UIM_APDU_INS_BYTE_AUTHENTICATE)
  {
    boolean                   reject_apdu     = FALSE;
    mmgsdi_protocol_enum_type card_protocol   = qmi_uim_util_card_protocol(slot);
    boolean                   is_usim_channel = FALSE;

    /* Loop through the channels opened for apdu streaming and see if there
       is any USIM opened on the channel on which this APDU is intended to
       be sent. If the channel is the default channel or there is a USIM
       on this channel, and if the APDU is a AUTH APDU, and NV is set to 2,
       block the APDU for non prop II CLA */
    for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
    {
      if (qmi_uim_global_ptr->apdu_channel[i] &&
          qmi_uim_global_ptr->apdu_channel[i]->slot == slot &&
          qmi_uim_global_ptr->apdu_channel[i]->logical_channel == logical_channel)
      {
        if (qmi_uim_is_usim_aid(&(qmi_uim_global_ptr->apdu_channel[i]->aid), TRUE))
        {
          is_usim_channel = TRUE;
        }
        break;
      }
    }

    switch(card_protocol)
    {
      case MMGSDI_ICC:
        /* For ICC cards, block RUN GSM algorithm/RUN CAVE*/
        if (apdu_data.data_ptr[0] == QMI_UIM_APDU_CLA_BYTE_ICC)
        {
          if (qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_ALL ||
              qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_II_CLA)
          {
            reject_apdu = TRUE;
          }
        }
        /* Some ICC card works like UICC card which can perform command with 0x0X or 0x8X CLA,
           we should have the same restrictions for ICC as UICC's. */
        else if (apdu_data.data_ptr[3] == QMI_UIM_APDU_P2_BYTE_USIM_AUTHENTICATE_GSM_ALGO)
        {
          if (qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_ALL ||
              (qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_II_CLA &&
               !(apdu_data.data_ptr[0] & 0x80) &&
               (is_usim_channel || (logical_channel == 0x00)) ))
          {
            reject_apdu = TRUE;
          }
        }
        break;

      case MMGSDI_UICC:
        /* For UICC card, reject only when context is GSM algorithm */
        if (apdu_data.data_ptr[3] == QMI_UIM_APDU_P2_BYTE_USIM_AUTHENTICATE_GSM_ALGO)
        {
          if (qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_ALL ||
              (qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_II_CLA &&
               !(apdu_data.data_ptr[0] & 0x80) &&
               (is_usim_channel || (logical_channel == 0x00)) ))
          {
            reject_apdu = TRUE;
          }
        }
        break;

      default:
        /* Error case, reject the APDU */
        reject_apdu = TRUE;
        break;
    }
    if (reject_apdu)
    {
      UIM_MSG_ERR_0("ADPU rejected due to security restrictions");
      return TRUE;
    }
  }

  /* If no APDU security restrictions, nothing to check */
  if (qmi_uim_global_ptr->apdu_sec_restr_value == QMI_UIM_APDU_SEC_RESTRICTIONS_NONE)
  {
    *default_channel_streaming_ptr = TRUE;
    return FALSE;
  }

  /* If APDU security restrictions are set for only Inter Industry CLA, check the
     CLA b8 to allow non-inter industry CLAs:
     Byte 1: CLA - bit 8 set to 1 is a proprietary class (Per Spec ISO 7816-5) */
  if ((qmi_uim_global_ptr->apdu_sec_restr_value == QMI_UIM_APDU_SEC_RESTRICTIONS_II_CLA) &&
      (apdu_data.data_ptr[0] & 0x80))
  {
    return FALSE;
  }

  /*  Check these criteria to see if it needs to be rejected:
      1. Byte 2: INS - MANAGE CHANNEL or SELECT
      2. Byte 3: P1  - 0x04, if INS is SELECT
  */
  if ((apdu_data.data_ptr[1] == QMI_UIM_APDU_INS_BYTE_MANAGE_CHANNEL) ||
      ((apdu_data.data_ptr[1] == QMI_UIM_APDU_INS_BYTE_SELECT_FILE) &&
       (apdu_data.data_ptr[2] == QMI_UIM_APDU_P1_BYTE_SELECT_DF_NAME)))
  {
    UIM_MSG_ERR_2("ADPU rejected due to security restrictions, INS byte: 0x%x, P1 byte: 0x%x",
                  apdu_data.data_ptr[1], apdu_data.data_ptr[2]);
    return TRUE;
  }

  /* Next check if the logical channel belongs to requesting client */
  /* If any match is found, the request is OK to proceed */
  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->slot == slot &&
        qmi_uim_global_ptr->apdu_channel[i]->logical_channel == logical_channel &&
        qmi_uim_global_ptr->apdu_channel[i]->opening_cl_sp == cl_sp)
    {
      channel_index = i;
      break;
    }
  }
  if (channel_index == UIM_INVALID_APP_INDEX)
  {
    UIM_MSG_ERR_0("ADPU rejected due to security restrictions: logical_channel doesnt belong to client");
    return TRUE;
  }

  return FALSE;
} /* qmi_uim_is_apdu_request_rejected */


/*===========================================================================
  FUNCTION QMI_UIM_IS_LOGICAL_CHANNEL_REQUEST_REJECTED()

  DESCRIPTION
    If APDU security restrictions are enabled on the device, this function
    parses the incoming request for opening of logical channels and checks
    if the application is restricted or not. Access to the input AID is
    granted or denied based on the NV values of blacklist/whitelist.

  PARAMETERS
    input_aid  : Incoming AID

  RETURN VALUE
    Boolean indicating if the request is to be rejected or not.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_is_logical_channel_request_rejected
(
  mmgsdi_static_data_type    input_aid
)
{
  uint16                    i                = 0;
  boolean                   is_rejected      = TRUE;
  qmi_uim_aid_entry_type  * temp_aid_ptr     = NULL;

  ASSERT(qmi_uim_global_ptr);

  if (qmi_uim_global_ptr->apdu_sec_restr_value == QMI_UIM_APDU_SEC_RESTRICTIONS_NONE)
  {
    return FALSE;
  }

  /* Sanity check */
  if (input_aid.data_len > sizeof(input_aid.data_ptr))
  {
    UIM_MSG_ERR_1("Request to select AID rejected: Invalid AID length: 0x%x",
                  input_aid.data_len);
    return TRUE;
  }

  /* Input APDU can be empty only in case of empty blacklist */
  if ((input_aid.data_len == 0) &&
      (qmi_uim_global_ptr->apdu_sec_aid_info.aid_list_type == UIM_AID_LIST_WHITE_LIST))
  {
    UIM_MSG_ERR_0("Request to select AID rejected: Empty AID not allowed");
    return TRUE;
  }

  temp_aid_ptr = qmi_uim_global_ptr->apdu_sec_aid_info.aid_list_ptr;

  /* First check what type of AID list we read from NV */
  if (qmi_uim_global_ptr->apdu_sec_aid_info.aid_list_type == UIM_AID_LIST_BLACK_LIST)
  {
    /* For an empty blacklist, all AIDs are allowed */
    is_rejected  = FALSE;

    /* Loop through the blacklist of AIDs to check these rules:
       1. If AID from EFS, then partial/exact AID matches, requested AID is rejected
       2. If AID is added by QMI UIM, then allow the partial AID only.
       3. If requested AID doesnt match any of the blacklisted AIDs, it is allowed
       4. Empty blacklist means all AIDs are allowed
    */
    for (i = 0;
         (i < qmi_uim_global_ptr->apdu_sec_aid_info.aid_count) && (temp_aid_ptr != NULL);
         i++, temp_aid_ptr++)
    {
      /* First check which of the AIDs is shorter, since we need to
         account for partial AID selection */
      uint16 shortest_aid_len = (uint16)(input_aid.data_len <= temp_aid_ptr->aid.data_len ?
                                          input_aid.data_len : temp_aid_ptr->aid.data_len);

      if (temp_aid_ptr->from_efs == FALSE &&
          temp_aid_ptr->aid.data_len > input_aid.data_len)
      {
        continue;
      }
      if (memcmp(input_aid.data_ptr, temp_aid_ptr->aid.data_ptr, shortest_aid_len) == 0)
      {
        /* Found a match, reject it */
        UIM_MSG_ERR_0("Request to select AID rejected: AID blacklisted");
        return TRUE;
      }
    }
    UIM_MSG_MED_0("Request to select AID accepted: AID not blacklisted");
  }
  else if (qmi_uim_global_ptr->apdu_sec_aid_info.aid_list_type == UIM_AID_LIST_WHITE_LIST)
  {
    /* For an empty whitelist, any AID is not allowed */
    is_rejected = TRUE;

    /* Loop through the blacklist of AIDs to check these rules:
       1. If exact AID matches or partial match with shorter whitelisted AID, it is allowed
       2. If anything else, it is rejected (no partial matches with shorter requested AID)
       3. Empty whitelist means any AID is rejected
    */
    for (i = 0;
         (i < qmi_uim_global_ptr->apdu_sec_aid_info.aid_count) && (temp_aid_ptr != NULL);
         i++, temp_aid_ptr++)
    {
      if (input_aid.data_len >= temp_aid_ptr->aid.data_len)
      {
        if (memcmp(input_aid.data_ptr, temp_aid_ptr->aid.data_ptr, temp_aid_ptr->aid.data_len) == 0)
        {
          UIM_MSG_HIGH_0("Request to select AID accepted: AID whitelisted");
          return FALSE;
        }
      }
    }
    UIM_MSG_ERR_0("Request to select AID rejected: AID not whitelisted");
  }

  return is_rejected;
} /* qmi_uim_is_logical_channel_request_rejected */


/*===========================================================================
  FUNCTION QMI_UIM_INIT_CLIENT_STATE()

  DESCRIPTION
    Initializes the state of a specific QMI UIM client. This function is
    executed at client id deallocation and after a reset command.

  PARAMETERS
    cl_sp : service provided client state pointer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_init_client_state
(
  qmi_uimi_client_state_type * cl_sp
)
{
  uint16                     i             = 0;
  mmgsdi_slot_id_enum_type   mmgsdi_slot   = MMGSDI_MAX_SLOT_ID_ENUM;
  qmi_error_e_type           errval        = QMI_ERR_INTERNAL;

  ASSERT(cl_sp && qmi_uim_global_ptr);

  /* If this client is the last registered client
     for the remote SFS events, then deregister with simlock */
  if(cl_sp->uim_info.reg_event_type.simlock_remote_sfs_events)
  {
    qmi_uim_simlock_check_and_deregister_remote_sfs_operation(cl_sp->common.clid);
  }

  memset(&(cl_sp->uim_info.reg_event_type),
         0,
         sizeof(cl_sp->uim_info.reg_event_type));

  cl_sp->uim_info.sap_intermediate_get_resp     = FALSE;
  cl_sp->uim_info.refresh_notify_mask           = 0;
  cl_sp->uim_info.refresh_vote                  = FALSE;
  cl_sp->uim_info.refresh_num_files             = 0;
  cl_sp->uim_info.refresh_all_notify_mask       = 0;
  cl_sp->uim_info.card_status_ind_postponed     = FALSE;

  while(cl_sp->uim_info.refresh_files_list != NULL)
  {
    qmi_uim_refresh_file_node_type  * refresh_file_ptr =
      cl_sp->uim_info.refresh_files_list;
    cl_sp->uim_info.refresh_files_list = refresh_file_ptr->next_ptr;
    uimqmi_free(refresh_file_ptr);
  }

  /* Remove client references to non-prov. sessions opened */
  for (i = 0; i < UIM_MAX_NON_PROV_SESSIONS; i++)
  {
    if (qmi_uim_global_ptr->non_prov_session[i] != NULL)
    {
      (void)qmi_uim_remove_cl_sp_from_non_prov_session_index(i, cl_sp);
    }
  }

  /* If security restrictions are not enforced, nothing else to do */
  if (qmi_uim_global_ptr->apdu_sec_restr_value == QMI_UIM_APDU_SEC_RESTRICTIONS_NONE)
  {
    return;
  }

  /* Close the client's logical channel, if any & clean up */
  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->opening_cl_sp == cl_sp)
    {
      mmgsdi_session_close(qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id,
                           qmi_uimi_mmgsdi_callback,
                           (mmgsdi_client_data_type)NULL);
      /* Regardless of result, free the information in the table */
      qmi_uim_remove_apdu_session(
          qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id, FALSE,
          MMGSDI_SESSION_CLOSED_CAUSE_UNKNOWN);
    }
  }

  /* Check to make sure this was not the last client to ack, if so notify
     drivers to proceed with power down. */
  for (i = 0; i < QMI_UIM_MAX_CARD_COUNT; i++)
  {
    if (cl_sp->uim_info.supply_voltage_pending_ack[i])
    {
      cl_sp->uim_info.supply_voltage_pending_ack[i] = FALSE;

      errval = qmi_uim_util_slot_index_to_mmgsdi_slot(i,
                                                      &mmgsdi_slot);
      if (errval != QMI_ERR_NONE)
      {
        continue;
      }

      (void)qmi_uim_all_reg_clients_ack_vcc_deactivation(mmgsdi_slot);
    }
  }
} /* qmi_uim_init_client_state */


/*===========================================================================
  FUNCTION QMI_UIM_LONG_APDU_REQUEST()

  DESCRIPTION
    If long_apdu TLV is present in QMI_UIM_SEND_APDU, then this fucntion will take care
    handing data chunks

  PARAMETERS
    send_apdu_tlv_ptr       : Parsed TLV data
    send_apdu_tlv_count     : Number of parsed TLV data
    cl_sp                   : Client state pointer

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_long_apdu_request
(
  qmi_uim_tlv_item_type       * send_apdu_tlv_ptr,
  uint8                         send_apdu_tlv_count,
  qmi_uimi_client_state_type  * cl_sp
)
{
  ASSERT(qmi_uim_global_ptr && send_apdu_tlv_ptr && cl_sp && send_apdu_tlv_count >= 5);

  if(send_apdu_tlv_ptr[4].data.long_apdu_request.total_length == 0)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Clear cache data and handle new request if token is different */
  if(cl_sp->uim_info.extended_apdu.apdu_data.data_ptr != NULL &&
     cl_sp->uim_info.extended_apdu.token != send_apdu_tlv_ptr[4].data.long_apdu_request.token)
  {
    uimqmi_free(cl_sp->uim_info.extended_apdu.apdu_data.data_ptr);
    memset(&cl_sp->uim_info.extended_apdu, 0, sizeof(cl_sp->uim_info.extended_apdu));
  }

  /* When first chunks of data is received, allocate the memory with total_length */
  if(cl_sp->uim_info.extended_apdu.apdu_data.data_ptr == NULL)
  {
    cl_sp->uim_info.extended_apdu.apdu_data.data_ptr = uimqmi_malloc(send_apdu_tlv_ptr[4].data.long_apdu_request.total_length);

    if(cl_sp->uim_info.extended_apdu.apdu_data.data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    cl_sp->uim_info.extended_apdu.apdu_data.data_len = send_apdu_tlv_ptr[4].data.long_apdu_request.total_length;
    cl_sp->uim_info.extended_apdu.token = send_apdu_tlv_ptr[4].data.long_apdu_request.token;
    cl_sp->uim_info.extended_apdu.received_len = 0;
    cl_sp->uim_info.extended_apdu.slot.slot = send_apdu_tlv_ptr[0].data.slot.slot;
    cl_sp->uim_info.extended_apdu.channel_id_valid = FALSE;

    if(send_apdu_tlv_ptr[2].present)
    {
      cl_sp->uim_info.extended_apdu.channel_id = send_apdu_tlv_ptr[2].data.logical_channel.logical_channel;
      cl_sp->uim_info.extended_apdu.channel_id_valid = TRUE;
    }
  }

  /* Return malformed message if channel_id is present but matching or
     if channel_id is not present but channel_id was set */
  if((send_apdu_tlv_ptr[2].present &&
       cl_sp->uim_info.extended_apdu.channel_id != send_apdu_tlv_ptr[2].data.logical_channel.logical_channel) ||
      (!send_apdu_tlv_ptr[2].present &&
       cl_sp->uim_info.extended_apdu.channel_id_valid))
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Return if slot is different */
  if(cl_sp->uim_info.extended_apdu.slot.slot != send_apdu_tlv_ptr[0].data.slot.slot)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Return if data_len or offset is more than total_len */
  if(cl_sp->uim_info.extended_apdu.apdu_data.data_len != send_apdu_tlv_ptr[4].data.long_apdu_request.total_length ||
     send_apdu_tlv_ptr[4].data.long_apdu_request.offset >= cl_sp->uim_info.extended_apdu.apdu_data.data_len  ||
     (cl_sp->uim_info.extended_apdu.apdu_data.data_len - send_apdu_tlv_ptr[4].data.long_apdu_request.offset <
      send_apdu_tlv_ptr[1].data.send_apdu.apdu_data.data_len))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  (void)memscpy(cl_sp->uim_info.extended_apdu.apdu_data.data_ptr + send_apdu_tlv_ptr[4].data.long_apdu_request.offset,
                cl_sp->uim_info.extended_apdu.apdu_data.data_len - send_apdu_tlv_ptr[4].data.long_apdu_request.offset,
                send_apdu_tlv_ptr[1].data.send_apdu.apdu_data.data_ptr,
                send_apdu_tlv_ptr[1].data.send_apdu.apdu_data.data_len);
  cl_sp->uim_info.extended_apdu.received_len += send_apdu_tlv_ptr[1].data.send_apdu.apdu_data.data_len;

  /* received more data than expected */
  if(cl_sp->uim_info.extended_apdu.received_len > cl_sp->uim_info.extended_apdu.apdu_data.data_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Not received the complete data, waiting for more data */
  return QMI_ERR_NONE;
}/* qmi_uim_long_apdu_request */


/*===========================================================================
  FUNCTION QMI_UIMI_RESET_CLIENT()

  DESCRIPTION
    Resets the state for the given client

  PARAMETERS
    cl_sp :  client state pointer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_reset_client
(
  qmi_uimi_client_state_type * cl_sp
)
{
  ASSERT(cl_sp);

  ds_qmi_fw_common_reset_client((qmi_common_client_state_type *)cl_sp);
  qmi_uim_init_client_state(cl_sp);
} /* qmi_uimi_reset_client() */


/*===========================================================================

                             HANDLER FUNCTIONS

===========================================================================*/


/*===========================================================================
  FUNCTION QMI_UIMI_RESET()

  DESCRIPTION
    Reset the issuing client's UIM state

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_reset
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *      response = NULL;
  boolean              retval   = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  /* Reset the client's state */
  qmi_uimi_reset_client(cl_sp);

  /* Build the response from back to front
     No error possible in reset, so result is success */
  retval = qmi_svc_put_result_tlv(&response,
                                  QMI_RESULT_SUCCESS,
                                  QMI_ERR_NONE);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_reset */


/*===========================================================================
  FUNCTION QMI_UIMI_READ_TRANSPARENT()

  DESCRIPTION
    Reads the content of a transparent file on the card. Allow read req
    for more than 4096 bytes as we have support for long indications

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_read_transparent
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                  errval          = QMI_ERR_NONE;
  mmgsdi_session_id_type            session_id      = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type      token_info      = {FALSE, 0};
  qmi_uim_tlv_item_type             tlv_data[6];
  qmi_uim_requested_encrytion_type  encrypt_status  = QMI_UIM_NO_ENCRYPTION;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_READ_TRANSPARENT;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[4].type      = UIMI_TLV_TYPE_ENCRYPTED;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[5].type      = UIMI_TLV_TYPE_ENCRYPTED_EXT;
  tlv_data[5].tag       = UIMI_TLV_TAG_OPTIONAL_3;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 6);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* We cannot process both data only encryption and complete ecryption read requests */
  if(tlv_data[4].present && tlv_data[5].present)
  {
    errval = QMI_ERR_MALFORMED_MSG;
    goto send_result;
  }

  if (tlv_data[4].present && tlv_data[4].data.encrypted.value)
  {
    encrypt_status = QMI_UIM_DATA_ONLY_ENCRYPTION;
  }
  else if (tlv_data[5].present && tlv_data[5].data.encrypted_ext.value)
  {
    encrypt_status = QMI_UIM_COMPLETE_ENCRYPTION;
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_READ_TRANSPARENT,
                                  tlv_data, 4,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_read_transparent(
                            UIMI_CMD_VAL_READ_TRANSPARENT,
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            tlv_data[2].data.read_transparent.offset,
                            tlv_data[2].data.read_transparent.length,
                            encrypt_status,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       4,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 6);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_read_transparent() */


/*===========================================================================
  FUNCTION QMI_UIMI_READ_RECORD()

  DESCRIPTION
    Reads a record from a cyclic or linear fixed file on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_read_record
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[5];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_READ_RECORD;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;
  tlv_data[3].type      = UIMI_TLV_TYPE_READ_RECORD_LAST;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[4].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 5);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Update token info */
  token_info.is_valid = tlv_data[4].present;
  token_info.token = tlv_data[4].data.ind_token.value;

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_READ_RECORD,
                                  tlv_data, 5,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_read_record(
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            tlv_data[2].data.read_record.record,
                            (tlv_data[3].present) ?
                              tlv_data[3].data.read_record_last.last_record :
                              tlv_data[2].data.read_record.record,
                            tlv_data[2].data.read_record.length,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       5,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 5);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_4);
} /* qmi_uimi_read_record() */


/*===========================================================================
  FUNCTION QMI_UIMI_WRITE_TRANSPARENT()

  DESCRIPTION
    Writes the content of a transparent file on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_write_transparent
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[4];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_WRITE_TRANSPARENT;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_WRITE_TRANSPARENT,
                                  tlv_data, 4,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_write_transparent(
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            tlv_data[2].data.write_transparent.offset,
                            tlv_data[2].data.write_transparent.data,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       4,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_2);
} /* qmi_uimi_write_transparent() */


/*===========================================================================
  FUNCTION QMI_UIMI_WRITE_RECORD()

  DESCRIPTION
    Writes a record on a cyclic or linear fixed file on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_write_record
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[4];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_WRITE_RECORD;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_WRITE_RECORD,
                                  tlv_data, 4,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_write_record(
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            tlv_data[2].data.write_record.record,
                            tlv_data[2].data.write_record.data,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       4,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_2);
} /* qmi_uimi_write_record() */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_FILE_ATTRIBUTES()

  DESCRIPTION
    Get file attributes of a file on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_file_attributes
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[3];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Update token info */
  token_info.is_valid = tlv_data[2].present;
  token_info.token = tlv_data[2].data.ind_token.value;

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_GET_FILE_ATTRIBUTES,
                                  tlv_data, 3,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_get_file_attributes(
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       3,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_get_file_attributes() */


/*===========================================================================
  FUNCTION QMI_UIMI_SET_PIN_PROTECTION()

  DESCRIPTION
    Enable / Disable the pin on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_set_pin_protection
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[4];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_SET_PIN_PROTECTION;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_PIN_KEYREF_ID;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* If keyref_id TLV is present, map to appropriate MMGSDI pin_id */
  if (tlv_data[2].present)
  {
    errval = qmi_uim_update_pin_id(&tlv_data[1].data.set_pin_protection.pin_id,
                                   tlv_data[2].data.pin_keyref_id.keyref_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session: valid for all PINs except hidden key */
    if (tlv_data[1].data.verify_pin.pin_id != MMGSDI_HIDDENKEY)
    {
      errval = qmi_uim_open_session(UIMI_CMD_VAL_SET_PIN_PROTECTION,
                                    tlv_data, 4,
                                    token_info.is_valid ? NULL : cmd_buf_p,
                                    cl_sp);
    }
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_set_pin_protection(
                            session_id,
                            tlv_data[1].data.set_pin_protection.pin_enable,
                            tlv_data[1].data.set_pin_protection.pin_id,
                            tlv_data[1].data.set_pin_protection.pin_data,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       4,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data: this also clears the PIN
     information for security reason */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_set_pin_protection() */


/*===========================================================================
  FUNCTION QMI_UIMI_VERIFY_PIN()

  DESCRIPTION
    Verify the PIN

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_verify_pin
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[5];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_VERIFY_PIN;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_DECRYPTED_PIN;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[3].type      = UIMI_TLV_TYPE_PIN_KEYREF_ID;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[4].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_3;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 5);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Update token info */
  token_info.is_valid = tlv_data[4].present;
  token_info.token = tlv_data[4].data.ind_token.value;

  /* If keyref_id TLV is present, map to appropriate MMGSDI pin_id */
  if (tlv_data[3].present)
  {
    errval = qmi_uim_update_pin_id(&tlv_data[1].data.verify_pin.pin_id,
                                   tlv_data[3].data.pin_keyref_id.keyref_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session: valid for all PINs except hidden key */
    if (tlv_data[1].data.verify_pin.pin_id != MMGSDI_HIDDENKEY)
    {
      errval = qmi_uim_open_session(UIMI_CMD_VAL_VERIFY_PIN,
                                    tlv_data, 5,
                                    token_info.is_valid ? NULL : cmd_buf_p,
                                    cl_sp);
    }
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_verify_pin(
                            session_id,
                            tlv_data[1].data.verify_pin.pin_id,
                            (tlv_data[2].present && qmi_uim_global_ptr->silent_pin1_supported) ?
                              tlv_data[2].data.decrypted_pin.decrypted_pin_data :
                              tlv_data[1].data.verify_pin.pin_data,
                            (tlv_data[2].present && qmi_uim_global_ptr->silent_pin1_supported) ?
                              TRUE : FALSE,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       5,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data: this also clears the PIN
     information for security reason */
  qmi_uim_util_free_tlv_content(tlv_data, 5);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_verify_pin() */


/*===========================================================================
  FUNCTION QMI_UIMI_UNBLOCK_PIN()

  DESCRIPTION
    Unblocks the PIN

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_unblock_pin
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[4];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_UNBLOCK_PIN;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_PIN_KEYREF_ID;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* If keyref_id TLV is present, map to appropriate MMGSDI pin_id */
  if (tlv_data[2].present)
  {
    errval = qmi_uim_update_pin_id(&tlv_data[1].data.unblock_pin.pin_id,
                                   tlv_data[2].data.pin_keyref_id.keyref_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_UNBLOCK_PIN,
                                  tlv_data, 4,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_unblock_pin(
                            session_id,
                            tlv_data[1].data.unblock_pin.pin_id,
                            tlv_data[1].data.unblock_pin.puk_data,
                            tlv_data[1].data.unblock_pin.pin_data,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       4,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data: this also clears the PIN
     information for security reason */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_unblock_pin() */


/*===========================================================================
  FUNCTION QMI_UIMI_CHANGE_PIN()

  DESCRIPTION
    Changes the PIN

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_change_pin
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[4];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_CHANGE_PIN;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_PIN_KEYREF_ID;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* If keyref_id TLV is present, map to appropriate MMGSDI pin_id */
  if (tlv_data[2].present)
  {
    errval = qmi_uim_update_pin_id(&tlv_data[1].data.change_pin.pin_id,
                                   tlv_data[2].data.pin_keyref_id.keyref_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session: valid for all PINs except hidden key */
    if (tlv_data[1].data.verify_pin.pin_id != MMGSDI_HIDDENKEY)
    {
      errval = qmi_uim_open_session(UIMI_CMD_VAL_CHANGE_PIN,
                                    tlv_data, 4,
                                    token_info.is_valid ? NULL : cmd_buf_p,
                                    cl_sp);
    }
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_change_pin(
                            session_id,
                            tlv_data[1].data.change_pin.pin_id,
                            tlv_data[1].data.change_pin.old_pin_data,
                            tlv_data[1].data.change_pin.new_pin_data,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       4,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data: this also clears the PIN
     information for security reason */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_change_pin() */


/*===========================================================================
  FUNCTION qmi_uimi_event_reg()

  DESCRIPTION
    Registers for card events

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_event_reg
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type             errval     = QMI_ERR_NONE;
  dsm_item_type *              response   = NULL;
  boolean                      retval     = TRUE;
  qmi_result_e_type            result     = QMI_RESULT_SUCCESS;
  qmi_uimi_client_state_type * client_sp  = NULL;
  uint8                        changes    = 0;
  qmi_uim_tlv_item_type        tlv_data[1];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_EVENT_REG;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Update registration flag for this client. */
  client_sp = (qmi_uimi_client_state_type *) cl_sp;
  if (client_sp->uim_info.reg_event_type.card_status != tlv_data[0].data.event_reg.card_status)
  {
    client_sp->uim_info.reg_event_type.card_status = tlv_data[0].data.event_reg.card_status;
    changes++;
  }
  if (client_sp->uim_info.reg_event_type.sap_events != tlv_data[0].data.event_reg.sap_events)
  {
    client_sp->uim_info.reg_event_type.sap_events = tlv_data[0].data.event_reg.sap_events;
    changes++;
  }
  if (client_sp->uim_info.reg_event_type.extended_card_status != tlv_data[0].data.event_reg.extended_card_status)
  {
    client_sp->uim_info.reg_event_type.extended_card_status = tlv_data[0].data.event_reg.extended_card_status;
    changes++;
  }
  if(client_sp->uim_info.reg_event_type.prov_session_close_event != tlv_data[0].data.event_reg.provisioning_session_close_event)
  {
    client_sp->uim_info.reg_event_type.prov_session_close_event = tlv_data[0].data.event_reg.provisioning_session_close_event;
    changes++;
  }
  if (client_sp->uim_info.reg_event_type.physical_slots_status != tlv_data[0].data.event_reg.physical_slots_status)
  {
    client_sp->uim_info.reg_event_type.physical_slots_status = tlv_data[0].data.event_reg.physical_slots_status;
    changes++;
  }
  if(client_sp->uim_info.reg_event_type.sim_busy_events != tlv_data[0].data.event_reg.sim_busy_events)
  {
    client_sp->uim_info.reg_event_type.sim_busy_events = tlv_data[0].data.event_reg.sim_busy_events;
    changes++;
  }
  if(client_sp->uim_info.reg_event_type.reduced_card_status_events != tlv_data[0].data.event_reg.reduced_card_status_events)
  {
    client_sp->uim_info.reg_event_type.reduced_card_status_events = tlv_data[0].data.event_reg.reduced_card_status_events;
    changes++;
  }
  if(client_sp->uim_info.reg_event_type.recovery_complete_event != tlv_data[0].data.event_reg.recovery_complete_event)
  {
    client_sp->uim_info.reg_event_type.recovery_complete_event = tlv_data[0].data.event_reg.recovery_complete_event;
    changes++;
  }
  if(client_sp->uim_info.reg_event_type.supply_voltage_events != tlv_data[0].data.event_reg.supply_voltage_events)
  {
    client_sp->uim_info.reg_event_type.supply_voltage_events = tlv_data[0].data.event_reg.supply_voltage_events;
    changes++;
  }
  if(client_sp->uim_info.reg_event_type.reg_for_activation_status_event != tlv_data[0].data.event_reg.activation_status_event)
  {
    client_sp->uim_info.reg_event_type.reg_for_activation_status_event = tlv_data[0].data.event_reg.activation_status_event;
    changes++;
  }

#ifdef FEATURE_SIMLOCK
  if(client_sp->uim_info.reg_event_type.simlock_remote_sfs_events != tlv_data[0].data.event_reg.simlock_remote_sfs_events)
  {
    client_sp->uim_info.reg_event_type.simlock_remote_sfs_events = tlv_data[0].data.event_reg.simlock_remote_sfs_events;
    changes++;

    /* A client registered for remote SFS events. So register with
       simlock for performing remote SFS operation */
    if (tlv_data[0].data.event_reg.simlock_remote_sfs_events)
    {
      qmi_uim_simlock_register_for_remote_sfs_operation();
    }
    else
    {
      qmi_uim_simlock_check_and_deregister_remote_sfs_operation(client_sp->common.clid);
    }
  }
#endif /* FEATURE_SIMLOCK */

  if(client_sp->uim_info.reg_event_type.simlock_temp_unlock_status != tlv_data[0].data.event_reg.simlock_temp_unlock_status)
  {
    client_sp->uim_info.reg_event_type.simlock_temp_unlock_status = tlv_data[0].data.event_reg.simlock_temp_unlock_status;
    changes++;
  }

  /* Check if something was updated */
  if (changes == 0)
  {
    UIM_MSG_MED_0("Event reg: no effect");
    errval = QMI_ERR_NO_EFFECT;
  }

  /* Compose response payload */
  if (FALSE == qmi_uim_response_event_reg(
                  UIMI_TLV_RESP_TAG_OPTIONAL_1,
                  &response,
                  client_sp->uim_info.reg_event_type))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  /* Send the response */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_event_reg() */


/*===========================================================================
  FUNCTION qmi_uimi_get_card_status()

  DESCRIPTION
    Get current card events

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_card_status
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type     errval        = QMI_ERR_NONE;
  dsm_item_type *      response      = NULL;
  boolean              retval        = TRUE;
  qmi_result_e_type    result        = QMI_RESULT_SUCCESS;
  uint8                slot          = 0;
  uint8                rt_status_len = 0;
  uim_rt_status_enum   uim_rt_status[QMI_UIM_MAX_CARD_COUNT];
  qmi_uim_tlv_item_type tlv_data[1];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_EXTENDED_CARD_STATUS;
  tlv_data[0].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  for (slot = 0; slot < QMI_UIM_MAX_CARD_COUNT; slot++)
  {
    uim_rt_status[slot] = UIM_RT_STATUS_UNKNOWN;
    if (slot < qmi_uim_global_ptr->card_state.num_slots)
    {
      uim_rt_status[slot] = uim_get_rt_status((uim_slot_type)(UIM_SLOT_1 + slot));
    }
  }

  if (tlv_data[0].present &&
      tlv_data[0].data.extended_card_status.value)
  {
    /* Insert extended card status item in TLV */
    if (FALSE == qmi_uim_response_extended_card_status(UIMI_TLV_RESP_TAG_OPTIONAL_4,
                                                       &response,
                                                       &qmi_uim_global_ptr->card_state,
                                                       sizeof(qmi_uim_global_ptr->card_state_valid),
                                                       qmi_uim_global_ptr->card_state_valid))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }

    /* Insert status of hot swap switch in TLV */
    rt_status_len = (QMI_UIM_MAX_CARD_COUNT < qmi_uim_global_ptr->card_state.num_slots) ?
                      QMI_UIM_MAX_CARD_COUNT : qmi_uim_global_ptr->card_state.num_slots;
    if (FALSE == qmi_uim_response_hot_swap_status(UIMI_TLV_RESP_TAG_OPTIONAL_5,
                                                  &response,
                                                  TRUE,
                                                  rt_status_len,
                                                  uim_rt_status))
    {
      UIM_MSG_HIGH_0("Error in constructing hot_swap_status; ignoring TLV");
    }
  }
  else
  {
    /* Insert legacy card status item in TLV */
    if (FALSE == qmi_uim_response_card_status(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                              &response,
                                              &qmi_uim_global_ptr->card_state))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }

    /* Insert status of hot swap switch in TLV */
    rt_status_len = (QMI_UIM_LEGACY_MAX_CARD_COUNT < qmi_uim_global_ptr->card_state.num_slots) ?
                      QMI_UIM_LEGACY_MAX_CARD_COUNT : qmi_uim_global_ptr->card_state.num_slots;
    if (FALSE == qmi_uim_response_hot_swap_status(UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                                  &response,
                                                  FALSE,
                                                  rt_status_len,
                                                  uim_rt_status))
    {
      UIM_MSG_HIGH_0("Error in constructing hot_swap_status; ignoring TLV");
    }

    /* Insert the card status validity TLV */
    if (FALSE == qmi_uim_response_card_status_validity(
                   UIMI_TLV_RESP_TAG_OPTIONAL_3,
                   &response,
                   (QMI_UIM_LEGACY_MAX_CARD_COUNT < qmi_uim_global_ptr->card_state.num_slots) ?
                    QMI_UIM_LEGACY_MAX_CARD_COUNT : qmi_uim_global_ptr->card_state.num_slots,
                   qmi_uim_global_ptr->card_state_valid))
    {
      UIM_MSG_HIGH_0("Error in constructing card_status_validity TLV");
    }
  }

  /* Insert the sim busy TLV */
  if (FALSE == qmi_uim_response_sim_busy(
                 UIMI_TLV_RESP_TAG_OPTIONAL_6,
                 &response,
                 &qmi_uim_global_ptr->card_state))
  {
    UIM_MSG_HIGH_0("Error in constructing sim_busy TLV");
  }

  /* Insert the card mode TLV */
  if (FALSE == qmi_uim_response_card_mode(
                 UIMI_TLV_RESP_TAG_OPTIONAL_7,
                 &response,
                 &qmi_uim_global_ptr->card_state))
  {
    UIM_MSG_HIGH_0("Error in constructing card_mode TLV");
  }
  
  /* Insert ICCID, EID and ATR TLV */
  if(FALSE == qmi_uim_response_extended_card_info(
                UIMI_TLV_RESP_TAG_OPTIONAL_8,
		&response,
		&qmi_uim_global_ptr->card_state))
  {
    UIM_MSG_HIGH_0("Error in constructing extended_card_info TLV");
    dsm_free_packet(&response);
    errval = QMI_ERR_NO_MEMORY;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  /* Insert result item in the TLV */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_card_status() */


/*===========================================================================
  FUNCTION QMI_UIMI_REFRESH_REGISTER()

  DESCRIPTION
    Register for refresh. This function sends immediately the response to
    the application, even if MMGSDI still needs to process it. This is
    consistent with all other refresh commands.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_refresh_register
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                response        = NULL;
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  qmi_result_e_type              result          = QMI_RESULT_SUCCESS;
  boolean                        retval          = TRUE;
  mmgsdi_session_id_type         session_id      = UIM_INVALID_SESSION_ID;
  boolean                        opening_session = FALSE;
  qmi_uim_tlv_item_type          tlv_data[2];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_REGISTER_REFRESH;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Make sure that we are not using an APDU session */
  if ((tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_1 ||
       tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_2 ||
       tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_3) &&
       tlv_data[0].data.session_information.aid.data_len == QMI_UIM_TLV_CHANNEL_ID_LEN)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_REFRESH_REGISTER,
                                  tlv_data, 2,
                                  cmd_buf_p, cl_sp);
    if (errval == QMI_ERR_NONE)
    {
      opening_session = TRUE;
    }
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_refresh_register(
                            session_id,
                            tlv_data[1].data.register_refresh.register_op,
                            tlv_data[1].data.register_refresh.vote_for_init,
                            tlv_data[1].data.register_refresh.file_list_ptr,
                            (qmi_cmd_buf_type *) cmd_buf_p);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       2,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  if (opening_session)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  /* Send result */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_refresh_register() */


/*===========================================================================
  FUNCTION QMI_UIMI_REFRESH_OK()

  DESCRIPTION
    Vote for refresh

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_refresh_ok
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  uint16                         i               = 0;
  dsm_item_type *                response        = NULL;
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  qmi_result_e_type              result          = QMI_RESULT_SUCCESS;
  boolean                        retval          = TRUE;
  mmgsdi_session_id_type         session_id      = UIM_INVALID_SESSION_ID;
  uint16                         refresh_index   = 0;
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_client_state_type *   client_sp       = NULL;
  boolean                        right_to_vote   = FALSE;
  qmi_uim_tlv_item_type          tlv_data[2];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_OK_FOR_REFRESH;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session index for the refresh */
  errval = qmi_uim_util_session_id_to_index(&refresh_index, session_id);
  if (errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                  session_id);
    goto send_result;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check current refresh state */
  if( qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_WAIT_FOR_OK)
  {
    UIM_MSG_ERR_1("Cannot vote for refresh in this state: 0x%x",
                  qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check count of clients that need to vote */
  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count == 0)
  {
    UIM_MSG_ERR_0("No client is expected to vote");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check if this client has right to vote, and in case
     remove it from the list */
  client_sp = (qmi_uimi_client_state_type *) cl_sp;
  for (i = 0; i < (uint16)UIM_REFRESH_MAX_CLIENTS; i++)
  {
    if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_list[i] == client_sp)
    {
      right_to_vote = TRUE;
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_list[i] = NULL;
      break;
    }
  }
  if (!right_to_vote)
  {
    UIM_MSG_ERR_0("This client does not have the right to vote");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Store vote in global */
  if (!tlv_data[1].data.ok_to_refresh.value)
  {
    qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->ok_to_init = FALSE;
  }

  /* Decrease counter of missing clients */
  qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count--;

  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count == 0)
  {
    UIM_MSG_HIGH_1("QMI_UIM: all clients have voted. Result = 0x%x",
                   qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->ok_to_init);

    /* Call MMGSDI APIs to vote */
    mmgsdi_status = mmgsdi_session_ok_to_refresh(
                                session_id,
                                qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->ok_to_init,
                                qmi_uimi_mmgsdi_callback,
                                (mmgsdi_client_data_type)NULL );

    /* Convert MMGSDI status in QMI code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
  }
  else
  {
    UIM_MSG_HIGH_1("QMI_UIM: 0x%x clients still need to vote",
                   qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count);
    errval = QMI_ERR_NONE;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  /* Send result */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_refresh_ok() */


/*===========================================================================
  FUNCTION QMI_UIMI_REFRESH_COMPLETE()

  DESCRIPTION
    Refresh complete

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_refresh_complete
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  uint16                         i                 = 0;
  dsm_item_type *                response          = NULL;
  qmi_error_e_type               errval            = QMI_ERR_NONE;
  qmi_result_e_type              result            = QMI_RESULT_SUCCESS;
  boolean                        retval            = TRUE;
  mmgsdi_session_id_type         session_id        = UIM_INVALID_SESSION_ID;
  uint16                         refresh_index     = 0;
  mmgsdi_return_enum_type        mmgsdi_status     = MMGSDI_SUCCESS;
  qmi_uimi_client_state_type *   client_sp         = NULL;
  boolean                        right_to_complete = FALSE;
  qmi_uim_tlv_item_type          tlv_data[2];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_REFRESH_COMPLETE;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session index for the refresh */
  errval = qmi_uim_util_session_id_to_index(&refresh_index, session_id);
  if (errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                  session_id);
    goto send_result;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check current refresh state */
  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_WAIT_FOR_COMPLETE)
  {
    UIM_MSG_ERR_1("Cannot complete refresh in this state: 0x%x",
                  qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check count of clients that need to complete */
  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count == 0)
  {
    UIM_MSG_ERR_0("No client is expected to complete refresh");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check if this client has right to complete, and in case
     remove it from the list */
  client_sp = (qmi_uimi_client_state_type *) cl_sp;
  for (i = 0; i < (uint16)UIM_REFRESH_MAX_CLIENTS; i++)
  {
    if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_list[i] == client_sp)
    {
      right_to_complete = TRUE;
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_list[i] = NULL;
      break;
    }
  }
  if (!right_to_complete)
  {
    UIM_MSG_ERR_0("This client does not have the right to complete");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Store success in global */
  if (!tlv_data[1].data.refresh_complete.value)
  {
    qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->complete = FALSE;
  }

  /* For now, ignore the client list and assume that each completing client
     has the right to do it */
  qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count--;

  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count == 0)
  {
    UIM_MSG_HIGH_1("QMI_UIM: all clients have completed. Result = 0x%x",
                   qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->complete);

    /* Call MMGSDI APIs to vote */
    mmgsdi_status = mmgsdi_session_refresh_complete(
                                session_id,
                                qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->complete,
                                qmi_uimi_mmgsdi_callback,
                                (mmgsdi_client_data_type)NULL );

    /* Convert MMGSDI status in QMI code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
  }
  else
  {
    UIM_MSG_HIGH_1("QMI_UIM: 0x%x clients still need to complete refresh",
                   qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count);
    errval = QMI_ERR_NONE;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  /* Send result */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_refresh_complete() */


/*===========================================================================
  FUNCTION QMI_UIMI_REFRESH_GET_LAST_EVENT()

  DESCRIPTION
    Get the last refresh event

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_refresh_get_last_event
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                response        = NULL;
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  qmi_result_e_type              result          = QMI_RESULT_SUCCESS;
  boolean                        retval          = TRUE;
  mmgsdi_session_id_type         session_id      = UIM_INVALID_SESSION_ID;
  uint16                         refresh_index   = 0;
  qmi_uim_tlv_item_type          tlv_data[1];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session index for the refresh */
  errval = qmi_uim_util_session_id_to_index(&refresh_index, session_id);
  if (errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                  session_id);
    goto send_result;
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check if refresh event is cached */
  if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr == NULL)
  {
    UIM_MSG_ERR_0("No refresh event is cached");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check if session type is matching */
  if (session_id != qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.session_id)
  {
    UIM_MSG_ERR_2("Session id is not matching: 0x%x != 0x%x",
                  session_id,
                  qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.session_id);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Compose the response */
  if (FALSE == qmi_uim_response_refresh_event(
                         UIMI_TLV_RESP_TAG_OPTIONAL_1,
                         &response,
                         tlv_data[0].data.session_information.session_type,
                         &tlv_data[0].data.session_information.aid,
                         qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Ignore error while adding the refresh enforcement policy, as this is not
     strictly required */
  (void)qmi_uim_response_refresh_enforcement_policy(
          UIMI_TLV_RESP_TAG_OPTIONAL_2,
          &response,
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->cache.event_ptr->enforcement_policy);

  /* In all cases, keep the event in the cache, as it might be fetched by
     other clients later. */

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  /* Send result */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_refresh_get_last_event() */


/*===========================================================================
  FUNCTION QMI_UIMI_POWER_DOWN()

  DESCRIPTION
    Power down the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_power_down
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type            errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type     mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*     cb_userdata_ptr = NULL;
  qmi_uim_tlv_item_type       tlv_data[1];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_POWER_DOWN,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Execute MMGSDI function to power down */
  mmgsdi_status = mmgsdi_card_pdown(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        tlv_data[0].data.slot.slot,
                        qmi_uimi_mmgsdi_callback,
                        MMGSDI_CARD_POWER_DOWN_NOTIFY_GSDI,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status to QMI err code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_power_down() */


/*===========================================================================
  FUNCTION QMI_UIMI_POWER_UP()

  DESCRIPTION
    Power up the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_power_up
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                  errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type           mmgsdi_status   = MMGSDI_SUCCESS;
  uint8                             slot_idx        = 0;
  qmi_uimi_userdata_type*           cb_userdata_ptr = NULL;
  uim_slot_type                     uim_slot        = UIM_SLOT_NONE;
  mmgsdi_card_pup_options_enum_type card_pup_option = MMGSDI_CARD_POWER_UP_INITIAL_PUP;
  qmi_uim_tlv_item_type             tlv_data[3];


  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_IGNORE_HOTSWAP_SWITCH;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[2].type      = UIMI_TLV_TYPE_CARD_POWERUP_MODE;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  if(qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id(tlv_data[0].data.slot.slot,
                                                &uim_slot) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
     slot_idx >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[slot_idx] == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  if(tlv_data[1].present &&
     tlv_data[1].data.ignore_hotswap_switch.value)
  {
    uim_disable_hotswap_interrupt(uim_slot);
  }

  /* if the card mode is telecom card then set the mode to passthrough */
  if (tlv_data[2].present)
  {
    card_pup_option = tlv_data[2].data.card_pup_option.card_pup_value;
  }

  /* Check if power up is already in progress */
  if (qmi_uim_global_ptr->power_up_state[slot_idx] != QMI_UIM_PUP_NOT_IN_PROGRESS)
  {
    UIM_MSG_ERR_2("Power up in progress with pup state 0x%x for slot 0x%x",
                  qmi_uim_global_ptr->power_up_state[slot_idx], slot_idx);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Check if card is already powered up. Returning NO_EFFECT might be
     misleading here as there is no guarentee that card will power up correctly */
  if (qmi_uim_global_ptr->card_state.card[slot_idx]->card_state == QMI_UIM_CARD_STATE_PRESENT)
  {
    UIM_MSG_HIGH_1( "Card is already powered up for slot 0x%x", slot_idx);
    errval = QMI_ERR_NO_EFFECT;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_POWER_UP,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Execute MMGSDI function to power up */
  mmgsdi_status = mmgsdi_card_pup(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        tlv_data[0].data.slot.slot,
                        qmi_uimi_mmgsdi_callback,
                        card_pup_option,
                        (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Mark power up in progress */
  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    qmi_uim_global_ptr->power_up_state[slot_idx] = QMI_UIM_PUP_MMGSDI_RSP_PENDING;
  }

  /* Convert MMGSDI status to QMI err code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_power_up() */


/*===========================================================================
  FUNCTION QMI_UIMI_AUTHENTICATE()

  DESCRIPTION
    Executes the authentication algorithm on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_authenticate
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  struct fs_stat                 file_stat     = {0};
  qmi_uim_tlv_item_type          tlv_data[3];
  boolean                        is_test_card  = FALSE;
  mmgsdi_return_enum_type        mmgsdi_status = MMGSDI_ERROR;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_AUTHENTICATE;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (qmi_uim_util_is_session_provisioning(tlv_data[0].data.session_information.session_type))
  {
    /* Retrieve session id */
    errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                     tlv_data[0].data.session_information.session_type,
                                                     tlv_data[0].data.session_information.aid);
    if(errval == QMI_ERR_NONE)
    {
      if (mmgsdi_session_is_test_card_used(session_id,
                                           &is_test_card) == MMGSDI_SUCCESS)
      {
        UIM_MSG_LOW_1("Authentication attempted on card with test status: 0x%x", is_test_card);
      }
    }
  }

  /* Check if request needs to be rejected due to security restrictions */
  if ((tlv_data[1].data.authenticate.auth_context == UIM_AUTH_CONTEXT_RUN_GSM_ALG_V01 ||
       tlv_data[1].data.authenticate.auth_context == UIM_AUTH_CONTEXT_GSM_SEC_V01 ||
       tlv_data[1].data.authenticate.auth_context == UIM_AUTH_CONTEXT_RUN_CAVE_ALG_V01) &&
      (qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_ALL ||
       qmi_uim_global_ptr->auth_sec_restr_value == QMI_UIM_AUTH_SEC_RESTRICTIONS_II_CLA) &&
       !is_test_card)
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* In case of NAF derivation context, if FQDN white list or black list file is
     present then reject the request.This prevents a client from performing NAF
     derivation from AP, when this might be restricted to TZ only */
  if(tlv_data[1].data.authenticate.auth_context == UIM_AUTH_CONTEXT_GBA_SEC_NAF_DERIVATION_V01 &&
     (efs_stat(QMI_UIM_CONFIG_GBA_FQDN_WHITE_LIST, &file_stat) == 0 ||
      efs_stat(QMI_UIM_CONFIG_GBA_FQDN_BLACK_LIST, &file_stat) == 0))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  if(qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                      tlv_data[0].data.session_information.aid,
                                      cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Update token info */
  token_info.is_valid = tlv_data[2].present;
  token_info.token = tlv_data[2].data.ind_token.value;

  /* Block request if authentication is allowed for the app for this session */
  if(!qmi_uim_auth_allowed_in_app_state(tlv_data[0].data.session_information.session_type))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_AUTHENTICATE,
                                  tlv_data, 3,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_authenticate(
                            session_id,
                            tlv_data[1].data.authenticate.auth_context,
                            tlv_data[1].data.authenticate.auth_data,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       3,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_authenticate() */


/*===========================================================================
  FUNCTION QMI_UIMI_CLOSE_SESSION()

  DESCRIPTION
    Closes a non provisioning session

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_close_session
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                response      = NULL;
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  qmi_result_e_type              result        = QMI_RESULT_SUCCESS;
  boolean                        retval        = TRUE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  mmgsdi_return_enum_type        mmgsdi_status = MMGSDI_SUCCESS;
  boolean                        terminate_app = FALSE;
  qmi_uim_tlv_item_type          tlv_data[2];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_TERMINATE_APP;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check AID validity */
  if(tlv_data[0].data.session_information.aid.data_len == QMI_UIM_TLV_CHANNEL_ID_LEN)
  {
    UIM_MSG_ERR_1("Invalid AID length: 0x%x, cannot close session",
                  tlv_data[0].data.session_information.aid.data_len);
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Retrieve session id: only non provisining sessions can be closed */
  if (tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_1 ||
      tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_2 ||
      tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_3)
  {
    errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                     tlv_data[0].data.session_information.session_type,
                                                     tlv_data[0].data.session_information.aid);
    if (errval != QMI_ERR_NONE)
    {
      /* Update the error code if there is no need to send close channel */
      errval = QMI_ERR_NO_EFFECT;
      goto send_result;
    }
  }
  else
  {
    UIM_MSG_ERR_1("Impossible to close a provisioning session: 0x%x",
                  tlv_data[0].data.session_information.session_type);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* If the terminate_application TLV is missing, then terminate the app
     only if App is USIM/CSIM/ISIM */
  if (tlv_data[1].present)
  {
    terminate_app = tlv_data[1].data.terminate_app.value;
  }
  else
  {
    if (tlv_data[0].data.session_information.aid.data_len > 0)
    {
      if ((qmi_uim_is_usim_aid(&(tlv_data[0].data.session_information.aid), FALSE)) ||
          (qmi_uim_is_csim_aid(&(tlv_data[0].data.session_information.aid), FALSE)) ||
          (qmi_uim_is_isim_aid(&(tlv_data[0].data.session_information.aid))))
      {
        terminate_app = TRUE;
      }
    }
  }

  /* Close the session */
  mmgsdi_status = mmgsdi_session_close_terminate(session_id,
                                                 terminate_app,
                                                 qmi_uimi_mmgsdi_callback,
                                                 (mmgsdi_client_data_type)NULL);

  /* Immediately remove the session from the array.
     QMI_UIM_SESSION_CLOSED_IND shall not be sent out since the session
     clean-up was not trigerred by an MMGSDI_SESSION_CLOSE_EVT */
  qmi_uim_remove_non_provisioning_session(session_id, FALSE,
                                          MMGSDI_SESSION_CLOSED_CAUSE_UNKNOWN);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  /* Send result */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_close_session() */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_SERVICE_STATUS()

  DESCRIPTION
    Get the status of services on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_service_status
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *             response          = NULL;
  qmi_error_e_type            errval            = QMI_ERR_NONE;
  boolean                     retval            = TRUE;
  uint16                      request_count     = 0;
  qmi_uim_tlv_item_type       tlv_data[3];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_CAPABILITIES_MASK;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_SERVICE_NUMBER;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Check number of requests: right now one and only one
     service can be requested */
  request_count = (tlv_data[1].data.capabilities_mask.fdn_status ? 1 : 0) +
                  (tlv_data[1].data.capabilities_mask.hidden_key_status ? 1 : 0) +
                  (tlv_data[1].data.capabilities_mask.index_in_dir ? 1 : 0) +
                  (tlv_data[1].data.capabilities_mask.esn_me_status ? 1 : 0) +
                  (tlv_data[1].data.capabilities_mask.acl_status ? 1 : 0) +
                  (tlv_data[1].data.capabilities_mask.service_status ? 1 : 0);
  if (request_count != 1)
  {
    UIM_MSG_ERR_1("Invalid number of requested services: 0x%x", request_count);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* FDN status */
  if (tlv_data[1].data.capabilities_mask.fdn_status)
  {
    errval = qmi_uim_execute_get_fdn_acl_status(
                   tlv_data[0].data.session_information.session_type,
                   cmd_buf_p,
                   ((qmi_uimi_client_state_type *)cl_sp)->common.clid,
                   QMI_UIM_SERVICE_FDN);
  }

  /* Hidden key status */
  if (tlv_data[1].data.capabilities_mask.hidden_key_status ||
      tlv_data[1].data.capabilities_mask.index_in_dir)
  {
    errval = qmi_uim_execute_get_status_hidden_key_and_index(
                   tlv_data[1].data.capabilities_mask.hidden_key_status,
                   tlv_data[1].data.capabilities_mask.index_in_dir,
                   tlv_data[0].data.session_information.session_type,
                   cmd_buf_p,
                   ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  }

  /* ESN status */
  if (tlv_data[1].data.capabilities_mask.esn_me_status)
  {
#ifdef FEATURE_MMGSDI_3GPP2
    mmgsdi_esn_me_change_enum_type esn_status  = MMGSDI_ESN_ME_NO_CHANGE;
    qmi_result_e_type              result      = QMI_RESULT_SUCCESS;

    errval = qmi_uim_execute_get_status_esn(
                   tlv_data[0].data.session_information.session_type, &esn_status);
    if (errval == QMI_ERR_NONE)
    {
      /* Free the buffer used for data */
      qmi_uim_util_free_tlv_content(tlv_data, 3);

      if (FALSE == qmi_uim_response_esn_status(UIMI_TLV_RESP_TAG_OPTIONAL_4,
                                               &response, esn_status))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }

      /* Insert result item in the TLV */
      result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                        : QMI_RESULT_FAILURE);

      retval = qmi_svc_put_result_tlv(&response, result, errval);
      CHECK_RETVAL();

      return response;
    }
#else
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
#endif/* FEATURE_MMGSDI_3GPP2 */
  }

  /* ACL status */
  if (tlv_data[1].data.capabilities_mask.acl_status)
  {
    errval = qmi_uim_execute_get_fdn_acl_status(
                   tlv_data[0].data.session_information.session_type,
                   cmd_buf_p,
                   ((qmi_uimi_client_state_type *)cl_sp)->common.clid,
                   QMI_UIM_SERVICE_ACL);
  }

  /* Service status */
  if (tlv_data[1].data.capabilities_mask.service_status)
  {
    boolean                     service_available = FALSE;
    qmi_result_e_type           result            = QMI_RESULT_SUCCESS;

    if(!tlv_data[2].present)
    {
      errval = QMI_ERR_MISSING_ARG;
      goto send_result;
    }
    /* Retrieve the service availability status synchronously and send the
       response synchronously */
    errval = qmi_uim_execute_get_service_status(
                   tlv_data[0].data.session_information.session_type,
                   tlv_data[2].data.service_number.service_number,
                   &service_available);
    if (errval == QMI_ERR_NONE)
    {
      /* Free the buffer used for data */
      qmi_uim_util_free_tlv_content(tlv_data, 3);

      if (FALSE == qmi_uim_response_boolean(
                         UIMI_TLV_RESP_TAG_OPTIONAL_6,
                         &response,
                         service_available))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }

      /* Insert result item in the TLV */
      result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                        : QMI_RESULT_FAILURE);

      retval = qmi_svc_put_result_tlv(&response, result, errval);
      CHECK_RETVAL();

      return response;
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_get_service_status() */


/*===========================================================================
  FUNCTION QMI_UIMI_SET_SERVICE_STATUS()

  DESCRIPTION
    Set the status of services on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_set_service_status
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  qmi_uim_tlv_item_type          tlv_data[3];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_SET_FDN_STATUS;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[2].type      = UIMI_TLV_TYPE_SET_ACL_STATUS;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Verify that one and only one service is present */
  if ((tlv_data[1].present && tlv_data[2].present) ||
      (!tlv_data[1].present && !tlv_data[2].present))
  {
    UIM_MSG_ERR_0("Exactly one service can be enabled/disabled at a time");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* FDN status */
  if (tlv_data[1].present)
  {
    errval = qmi_uim_execute_set_service_status(
                             cmd_buf_p,
                             tlv_data[0].data.session_information.session_type,
                             UIMI_TLV_TYPE_SET_FDN_STATUS,
                             tlv_data[1].data.set_fdn_status.value,
                             ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* ACL status */
  if (tlv_data[2].present)
  {
    errval = qmi_uim_execute_set_service_status(
                             cmd_buf_p,
                             tlv_data[0].data.session_information.session_type,
                             UIMI_TLV_TYPE_SET_ACL_STATUS,
                             tlv_data[2].data.set_acl_status.value,
                             ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_set_service_status() */


/*===========================================================================
  FUNCTION QMI_UIMI_CHANGE_PROVISIONING_SESSION()

  DESCRIPTION
    Change a provisioning session on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_change_provisioning_session
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  mmgsdi_return_enum_type      mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_session_id_type       session_id      = UIM_INVALID_SESSION_ID;
  qmi_uimi_userdata_type*      cb_userdata_ptr = NULL;
  qmi_error_e_type             errval          = QMI_ERR_NONE;
  qmi_uim_tlv_item_type        tlv_data[2];
  mmgsdi_data_type             app_id_data;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_CHANGE;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_APPLICATION_INFORMATION;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  UIM_MSG_HIGH_3("QMI_UIM_CHANGE_PROV, activate: 0x%x session_type: 0x%x Slot 0x%x",
                 tlv_data[0].data.session_change.activate,
                 tlv_data[0].data.session_information.session_type,
                 tlv_data[1].present? tlv_data[1].data.application_information.slot : 0xFF);

  /* Retrieve session id: supports only provisioning sessions */
  if (qmi_uim_util_is_session_provisioning(tlv_data[0].data.session_information.session_type))
  {
    errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                     tlv_data[0].data.session_information.session_type,
                                                     tlv_data[1].data.application_information.aid);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }
  else
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* In case of activation, the optional TLV with Application information
     must be present */
  if (tlv_data[0].data.session_change.activate &&
      !tlv_data[1].present)
  {
    UIM_MSG_ERR_0("Missing TLV with application information");
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  /* In case of de-activation, update error code if the request is to
     deactivate a prov session that is is already deactivated */
  if (!tlv_data[0].data.session_change.activate)
  {
    mmgsdi_static_data_type  aid        = { 0, };
    mmgsdi_app_enum_type     card_type  = MMGSDI_APP_NONE;

    /* This function returns an error if any prov session index was invalid */
    if (qmi_uim_util_session_type_to_card_type(&card_type,
                                               tlv_data[0].data.session_information.session_type,
                                               aid) != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_1("Session inactive for prov session: 0x%x",
                    tlv_data[0].data.session_information.session_type);
      errval = QMI_ERR_NO_EFFECT;
      goto send_result;
    }
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_CHANGE_PROVISIONING_SESSION,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  if(tlv_data[0].data.session_change.activate)
  {
    memset(&app_id_data, 0, sizeof(mmgsdi_data_type));
    if(tlv_data[1].data.application_information.aid.data_len > 0)
    {
      app_id_data.data_len = tlv_data[1].data.application_information.aid.data_len;
      app_id_data.data_ptr = tlv_data[1].data.application_information.aid.data_ptr;
    }

    /* Execute MMGSDI activate or switch provisioning */
    mmgsdi_status = mmgsdi_session_activate_or_switch_provisioning(
                                  session_id,
                                  tlv_data[1].data.application_information.slot,
                                  app_id_data,
                                  FALSE,
                                  qmi_uimi_mmgsdi_callback,
                                  (mmgsdi_client_data_type)cb_userdata_ptr);
  }
  else
  {
    /* Execute MMGSDI deactivate provisioning */
    mmgsdi_status = mmgsdi_session_deactivate_provisioning(
                                  session_id,
                                  qmi_uimi_mmgsdi_callback,
                                  (mmgsdi_client_data_type)cb_userdata_ptr );
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_change_provisioning_session */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_LABEL()

  DESCRIPTION
    Retrieve the label of an application on a UICC card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_label
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  uint16                       i          = 0;
  uint16                       app_index  = UIM_INVALID_APP_INDEX;
  qmi_error_e_type             errval     = QMI_ERR_NONE;
  dsm_item_type *              response   = NULL;
  boolean                      retval     = TRUE;
  qmi_result_e_type            result     = QMI_RESULT_SUCCESS;
  uint8                        slot_idx   = 0;
  qmi_uim_tlv_item_type        tlv_data[1];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_APPLICATION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.application_information.slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
     slot_idx >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[slot_idx] == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  if (tlv_data[0].data.application_information.aid.data_len == 0)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Check card status */
  if (qmi_uim_global_ptr->card_state.card[slot_idx]->card_state != QMI_UIM_CARD_STATE_PRESENT)
  {
    UIM_MSG_ERR_1("Invalid card status: 0x%x",
                  qmi_uim_global_ptr->card_state.card[slot_idx]->card_state);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Search matching application. Cannot use qmi_uim_util_find_aid() because
     we don't know the app type */
  for (i = 0; i < qmi_uim_global_ptr->card_state.num_apps &&
              i < QMI_UIM_MAX_APP_COUNT; i++)
  {
    if (qmi_uim_global_ptr->card_state.application[i] != NULL &&
        qmi_uim_global_ptr->card_state.application[i]->slot ==
            tlv_data[0].data.application_information.slot &&
        qmi_uim_global_ptr->card_state.application[i]->aid.data_len ==
            tlv_data[0].data.application_information.aid.data_len)
    {
      if (memcmp( (void*)qmi_uim_global_ptr->card_state.application[i]->aid.data_ptr,
                  (void*)tlv_data[0].data.application_information.aid.data_ptr,
                  tlv_data[0].data.application_information.aid.data_len ) == 0)
      {
        app_index = i;
        break;
      }
    }
  }

  if (app_index != UIM_INVALID_APP_INDEX)
  {
    /* Matching application was found */
    if (FALSE == qmi_uim_response_label(
                    UIMI_TLV_RESP_TAG_OPTIONAL_1,
                    &response,
                    &qmi_uim_global_ptr->card_state.application[app_index]->label))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }
  else
  {
    /* Application was not found */
    errval = QMI_ERR_INVALID_ARG;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  /* Send the response */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_label */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_CONFIGURATION()

  DESCRIPTION
    Retrieve the UIM configuration

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_configuration
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type             errval                = QMI_ERR_NONE;
  boolean                      retval                = TRUE;
  mmgsdi_return_enum_type      mmgsdi_status         = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*      cb_userdata_ptr       = NULL;
  qmi_uim_tlv_item_type        tlv_data[1];
  dsm_item_type              * response              = NULL;
  qmi_result_e_type            result                = QMI_RESULT_SUCCESS;
  mmgsdi_cached_nv_item_data   cached_nv_item;
  simlock_result_enum_type     simlock_status        = SIMLOCK_SUCCESS;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(&cached_nv_item, 0, sizeof(mmgsdi_cached_nv_item_data));
  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_CONFIGURATION_MASK;
  tlv_data[0].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_CONFIGURATION,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  if(tlv_data[0].present)
  {
    cb_userdata_ptr->data.get_configuration.automatic_selection_request    =
                     tlv_data[0].data.configuration_mask.automatic_selection;
    cb_userdata_ptr->data.get_configuration.personalization_request        =
                     tlv_data[0].data.configuration_mask.personalization_status;
    cb_userdata_ptr->data.get_configuration.halt_subscription_request      =
                     tlv_data[0].data.configuration_mask.halt_subscription_status;
    cb_userdata_ptr->data.get_configuration.usb_uicc_supported_request     =
                     tlv_data[0].data.configuration_mask.usb_uicc_supported_status;
    cb_userdata_ptr->data.get_configuration.sap_client_supported_request   =
                     tlv_data[0].data.configuration_mask.sap_client_supported_status;
    cb_userdata_ptr->data.get_configuration.perso_reactivation_request     =
                     tlv_data[0].data.configuration_mask.perso_reactivation_status;
    cb_userdata_ptr->data.get_configuration.remote_simlock_storage_request =
                     tlv_data[0].data.configuration_mask.remote_simlock_storage;
    cb_userdata_ptr->data.get_configuration.emergency_only_mode_request    =
                     tlv_data[0].data.configuration_mask.emergency_only_mode;
    cb_userdata_ptr->data.get_configuration.extended_apdu_request          =
                     tlv_data[0].data.configuration_mask.extended_apdu;
    cb_userdata_ptr->data.get_configuration.inactive_simlock_config        =
                     tlv_data[0].data.configuration_mask.inactive_simlock_config;
  }
  else
  {
    cb_userdata_ptr->data.get_configuration.automatic_selection_request     = TRUE;
    cb_userdata_ptr->data.get_configuration.personalization_request         = TRUE;
    cb_userdata_ptr->data.get_configuration.halt_subscription_request       = TRUE;
    cb_userdata_ptr->data.get_configuration.usb_uicc_supported_request      = TRUE;
    cb_userdata_ptr->data.get_configuration.sap_client_supported_request    = TRUE;
    cb_userdata_ptr->data.get_configuration.perso_reactivation_request      = TRUE;
    cb_userdata_ptr->data.get_configuration.remote_simlock_storage_request  = TRUE;
    cb_userdata_ptr->data.get_configuration.emergency_only_mode_request     = TRUE;
    cb_userdata_ptr->data.get_configuration.extended_apdu_request           = TRUE;
    cb_userdata_ptr->data.get_configuration.inactive_simlock_config         = TRUE;
  }

  /* Initialize automatic selection to TRUE: this is the value in case
     NV item does not exist */
  cb_userdata_ptr->data.get_configuration.automatic_selection = TRUE;

  if(cb_userdata_ptr->data.get_configuration.automatic_selection_request)
  {
    /* If configuration mask is not present or if mask is present and info is requested
       Retrieve the NV item for automatic selection using MMGSDI APIs directly */
    cached_nv_item.nv_item_enum = MMGSDI_NV_UIM_SELECT_DEFAULT_USIM_APP_I;
    mmgsdi_status = mmgsdi_get_cached_nv_item(&cached_nv_item);
    if (mmgsdi_status == MMGSDI_SUCCESS &&
        cached_nv_item.nv_item_data.uim_select_default_usim_app == FALSE)
    {
      cb_userdata_ptr->data.get_configuration.automatic_selection = FALSE;
    }
  }

  /* Initialize halt subscription to FALSE: this is the value in case
     the config file in EFS does not exist */
  cb_userdata_ptr->data.get_configuration.halt_subscription = FALSE;
  if(cb_userdata_ptr->data.get_configuration.halt_subscription_request)
  {
    /* If configuration mask is not present or if mask is present and info is requested */
    /* Get halt subscription config value from EFS */
    /* Get halt subscription config value from EFS */
    cb_userdata_ptr->data.get_configuration.halt_subscription =
         qmi_uim_get_halt_subscription_value();
  }

  /* Initialize USB UICC supported variable to FALSE */
  cb_userdata_ptr->data.get_configuration.usb_uicc_supported = FALSE;
  if(cb_userdata_ptr->data.get_configuration.usb_uicc_supported_request)
  {
    cb_userdata_ptr->data.get_configuration.usb_uicc_supported =
        uim_is_usb_uicc_supported();
  }

  /* Initialize SAP CLIENT supported variable to FALSE */
  cb_userdata_ptr->data.get_configuration.sap_client_supported = FALSE;
  if(cb_userdata_ptr->data.get_configuration.sap_client_supported_request)
  {
    cb_userdata_ptr->data.get_configuration.sap_client_supported =
        uim_is_sap_client_supported();
  }

  if((cb_userdata_ptr->data.get_configuration.personalization_request)        ||
     (cb_userdata_ptr->data.get_configuration.perso_reactivation_request)     ||
     (cb_userdata_ptr->data.get_configuration.remote_simlock_storage_request) ||
     (cb_userdata_ptr->data.get_configuration.emergency_only_mode_request) ||
     (cb_userdata_ptr->data.get_configuration.inactive_simlock_config))
  {
    simlock_status = simlock_get_status_ext(
                       cb_userdata_ptr->data.get_configuration.inactive_simlock_config,
                       qmi_uimi_simlock_callback,
                       cb_userdata_ptr);

    if(simlock_status != SIMLOCK_SUCCESS)
    {
      /* Convert MMGSDI status in QMI code */
      errval = qmi_uim_simlock_convert_simlock_status_to_errval(simlock_status);
    }
    else
    {
      return QMI_SVC_RESPONSE_PENDING;
    }
  }

  /* Initialize extended APDU supported variable to TRUE because
     extended APDU is always supported */
  cb_userdata_ptr->data.get_configuration.extended_apdu = TRUE;

  /* If configuration mask is present and personalisation data is not requested then
     response can be posted here itself */
  if(cb_userdata_ptr->data.get_configuration.automatic_selection_request)
  {
    if (FALSE == qmi_uim_response_boolean(
                    UIMI_TLV_RESP_TAG_OPTIONAL_1,
                    &response,
                    cb_userdata_ptr->data.get_configuration.automatic_selection))
    {
      errval = QMI_ERR_NO_MEMORY;
      goto send_result;
    }
  }

  if(cb_userdata_ptr->data.get_configuration.halt_subscription_request)
  {
    /* Insert TLV with halt subscription configuration */
    if (FALSE == qmi_uim_response_boolean(
                      UIMI_TLV_RESP_TAG_OPTIONAL_3,
                      &response,
                      cb_userdata_ptr->data.get_configuration.halt_subscription))
    {
      errval = QMI_ERR_NO_MEMORY;
      goto send_result;
    }
  }

  if(cb_userdata_ptr->data.get_configuration.extended_apdu_request)
  {
    if (FALSE == qmi_uim_response_boolean(
                    UIMI_TLV_RESP_TAG_OPTIONAL_12,
                    &response,
                    cb_userdata_ptr->data.get_configuration.extended_apdu))
    {
      errval = QMI_ERR_NO_MEMORY;
      goto send_result;
    }
  }

send_result:
  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_configuration */


/*===========================================================================
  FUNCTION QMI_UIMI_SEND_APDU()

  DESCRIPTION
    Send an APDU to the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_send_apdu
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  uint16                               i                         = 0;
  qmi_error_e_type                     errval                    = QMI_ERR_NONE;
  mmgsdi_return_enum_type              mmgsdi_status             = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*              cb_userdata_ptr           = NULL;
  mmgsdi_send_apdu_options_enum_type   apdu_option               = MMGSDI_SEND_APDU_RETURN_IMPLICIT_STATUS_WORDS;
  uint16                               channel_index             = UIM_INVALID_APP_INDEX;
  boolean                              default_channel_streaming = FALSE;
  qmi_uimi_client_state_type          *client_sp                 = NULL;
  dsm_item_type *                      response                  = NULL;
  qmi_result_e_type                    result                    = QMI_RESULT_SUCCESS;
  boolean                              retval                    = TRUE;
  mmgsdi_send_apdu_data_type          *apdu_data_ptr             = NULL;
  qmi_uim_tlv_item_type                tlv_data[5];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_APDU_REQUEST;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_LOGICAL_CHANNEL;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[3].type      = UIMI_TLV_TYPE_SKIP_PROCEDURE_BYTES;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[4].type      = UIMI_TLV_TYPE_LONG_APDU_REQUEST;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_3;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 5);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Handle the optional long apdu data */
  if (tlv_data[4].present)
  {
    client_sp = (qmi_uimi_client_state_type*)cl_sp;
    errval = qmi_uim_long_apdu_request(tlv_data, 5, client_sp);

    /* Send immediate response in case of error or in case of incomplete APDU */
    if (errval != QMI_ERR_NONE ||
        client_sp->uim_info.extended_apdu.received_len < client_sp->uim_info.extended_apdu.apdu_data.data_len)
    {
      goto send_result;
    }
  }

  if (tlv_data[4].present)
  {
    apdu_data_ptr = &(client_sp->uim_info.extended_apdu.apdu_data);
  }
  else
  {
    apdu_data_ptr = &(tlv_data[1].data.send_apdu.apdu_data);
  }

  /* This check is needed to keep KW happy, even if not strictly necessary */
  if (apdu_data_ptr == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Handle the optional channel id */
  if (tlv_data[2].present)
  {
    /* Check if the channel id is valid */
    for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
    {
      if (qmi_uim_global_ptr->apdu_channel[i] &&
          qmi_uim_global_ptr->apdu_channel[i]->slot == tlv_data[0].data.slot.slot &&
          qmi_uim_global_ptr->apdu_channel[i]->logical_channel == tlv_data[2].data.logical_channel.logical_channel)
      {
        channel_index = i;
        break;
      }
    }
    if (channel_index == UIM_INVALID_APP_INDEX)
    {
      errval = QMI_ERR_INVALID_ARG;
      goto send_result;
    }

    /* Insert channel id in the APDU */
    if (apdu_data_ptr->data_len > 0 &&
        apdu_data_ptr->data_ptr != NULL)
    {
      if(tlv_data[2].data.logical_channel.logical_channel < 4)
      {
        /* For logical channel number from 0 to 3, b7 of CLA byte should be 0 */
        apdu_data_ptr->data_ptr[0] &= 0xBC;
        /* After clearing the two lowest bits. Set the channel */
        apdu_data_ptr->data_ptr[0] |=
          tlv_data[2].data.logical_channel.logical_channel;
      }
      else
      {
        apdu_data_ptr->data_ptr[0] &= 0xF0;
        /* Extended channels 4 - 19 are represented 0000 - 1111
           in the lower byte of the class byte.
           After clearing the four lowest bits set the channel
           equal to the channel id - the offset(4) and set b7 of
           the class byte to indicate extended logical channels */
        apdu_data_ptr->data_ptr[0] |=
          (tlv_data[2].data.logical_channel.logical_channel - 4) | 0x40;
      }
    }
  }

  /* Check if APDU security restrictions are enforced */
  if (qmi_uim_is_apdu_request_rejected(tlv_data[0].data.slot.slot,
                                         *apdu_data_ptr,
                                         &tlv_data[2],
                                         cl_sp,
                                         &default_channel_streaming))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SEND_APDU,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  if (tlv_data[3].present)
  {
    if (tlv_data[3].data.skip_procedure_bytes.value)
    {
      apdu_option = MMGSDI_SEND_APDU_NO_IMPLICIT_STATUS_WORDS;
    }
    else
    {
      apdu_option = MMGSDI_SEND_APDU_RETURN_IMPLICIT_STATUS_WORDS;
    }
  }

  /* Call MMGSDI function to send APDU */
  mmgsdi_status = mmgsdi_send_apdu_ext(qmi_uim_global_ptr->mmgsdi_client_id,
                                       tlv_data[0].data.slot.slot,
                                      *apdu_data_ptr,
                                       apdu_option,
                                       default_channel_streaming,
                                       qmi_uimi_mmgsdi_callback,
                                       (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
  if (mmgsdi_status == MMGSDI_INCORRECT_PARAMS)
  {
    errval = QMI_ERR_INVALID_ARG;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 5);

  /* Free memory allocated for extended APDU request
     If error response or after sending request to MMGSDI */
  if((errval != QMI_ERR_NONE           &&
      client_sp != NULL                     &&
      client_sp->uim_info.extended_apdu.apdu_data.data_ptr != NULL) ||
     (errval == QMI_ERR_NONE        &&
      client_sp != NULL             &&
      client_sp->uim_info.extended_apdu.apdu_data.data_ptr != NULL &&
      client_sp->uim_info.extended_apdu.received_len == client_sp->uim_info.extended_apdu.apdu_data.data_len))
  {
    uimqmi_free(client_sp->uim_info.extended_apdu.apdu_data.data_ptr);
    memset(&client_sp->uim_info.extended_apdu, 0, sizeof(client_sp->uim_info.extended_apdu));
  }

  /* Userdata pointer is not NULL
     only when the request was sent successfully to MMGSDI,
     So we need to wait for the response */
  if(cb_userdata_ptr != NULL)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_send_apdu */


/*===========================================================================
  FUNCTION QMI_UIMI_SET_APDU_BEHAVIOR()

  DESCRIPTION
    Send an set APDU behavior

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_set_apdu_behavior
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  uint16                               i                         = 0;
  qmi_error_e_type                     errval                    = QMI_ERR_NONE;
  mmgsdi_return_enum_type              mmgsdi_status             = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*              cb_userdata_ptr           = NULL;
  uint16                               channel_index             = UIM_INVALID_APP_INDEX;
  qmi_uim_tlv_item_type                tlv_data[3];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_LOGICAL_CHANNEL;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_APDU_BEHAVIOR_FLAG;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if the channel id is valid */
  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] &&
        qmi_uim_global_ptr->apdu_channel[i]->slot == tlv_data[0].data.slot.slot &&
        qmi_uim_global_ptr->apdu_channel[i]->logical_channel == tlv_data[1].data.logical_channel.logical_channel &&
        qmi_uim_global_ptr->apdu_channel[i]->opening_cl_sp == cl_sp)
    {
      channel_index = i;
      break;
    }
  }
  if (channel_index == UIM_MAX_APDU_CHANNEL_COUNT)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SET_APDU_BEHAVIOR,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Call MMGSDI function to set APDU behavior*/
  mmgsdi_status = mmgsdi_set_apdu_behavior(qmi_uim_global_ptr->mmgsdi_client_id,
                                           tlv_data[0].data.slot.slot,
                                           tlv_data[1].data.logical_channel.logical_channel,
                                           tlv_data[2].data.apdu_behavior.apdu_behavior,
                                           qmi_uimi_mmgsdi_callback,
                                           (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
  if (mmgsdi_status == MMGSDI_INCORRECT_PARAMS)
  {
    errval = QMI_ERR_INVALID_ARG;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_set_apdu_behavior */


/*===========================================================================
  FUNCTION QMI_UIMI_LOGICAL_CHANNEL()

  DESCRIPTION
    Open and closes a logical channel on the UICC card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_logical_channel
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type             errval          = QMI_ERR_NONE;
  dsm_item_type *              response        = NULL;
  qmi_result_e_type            result          = QMI_RESULT_SUCCESS;
  boolean                      retval          = TRUE;
  boolean                      respond_now     = FALSE;
  qmi_uim_tlv_item_type        tlv_data[5];
  mmgsdi_select_rsp_enum_type  sel_rsp_type    = MMGSDI_SELECT_RSP_FCP;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_AID;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[2].type      = UIMI_TLV_TYPE_LOGICAL_CHANNEL;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[3].type      = UIMI_TLV_TYPE_FILE_CONTROL_INFO;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_3;
  tlv_data[4].type      = UIMI_TLV_TYPE_TERMINATE_APP;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_4;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 5);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check that only one of the first two optional TLVs is present */
  if ((!tlv_data[1].present && !tlv_data[2].present) ||
      (tlv_data[1].present && tlv_data[2].present))
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Open logical channel */
  if (tlv_data[1].present)
  {
    /* Check if request needs to be rejected due to security restrictions */
    if (qmi_uim_is_logical_channel_request_rejected(tlv_data[1].data.aid.aid))
    {
      errval = QMI_ERR_ACCESS_DENIED;
    }
    else
    {
      if (tlv_data[3].present)
      {
        sel_rsp_type = tlv_data[3].data.file_control_info.fci_type;
      }

      errval = qmi_uim_execute_open_session_for_apdu(
                           tlv_data[0].data.slot.slot,
                           tlv_data[1].data.aid.aid,
                           sel_rsp_type,
                           cmd_buf_p,
                           FALSE,
                           ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
    }
  }

  /* Close logical channel */
  if (tlv_data[2].present)
  {
    boolean terminate_app = TRUE;
    if(tlv_data[4].present)
    {
      terminate_app = tlv_data[4].data.terminate_app.value;
    }
    errval = qmi_uim_execute_close_session_for_apdu(
                         tlv_data[0].data.slot.slot,
                         tlv_data[2].data.logical_channel.logical_channel,
                         terminate_app,
                         cl_sp,
                         cmd_buf_p,
                         ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* For close channel, respond based on NV configuration */
    respond_now = qmi_uim_global_ptr->close_channel_sync;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 5);

  if (errval == QMI_ERR_NONE && respond_now == FALSE)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_logical_channel */


/*===========================================================================
  FUNCTION QMI_UIMI_SUBSCRIPTION_OK()

  DESCRIPTION
    Indicate OK or not OK to publish subscription to a provisioing application

  PARAMETERS
    sp          : service provided state pointer
    cmd_buf_ptr : ptr to cmd buffer
    cl_sp       : client state pointer
    sdu_in      : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_subscription_ok
(
  void *            sp,
  void *            cmd_buf_ptr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval                = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id            = UIM_INVALID_SESSION_ID;
  mmgsdi_return_enum_type        mmgsdi_status         = MMGSDI_ERROR;
  qmi_uim_tlv_item_type          tlv_data[3];
  qmi_uimi_userdata_type*        cb_userdata_ptr       = NULL;
  qmi_uim_imsi_type              imsi_data             = {0, };

  ASSERT(sp);
  ASSERT(cmd_buf_ptr);
  ASSERT(cl_sp);
  ASSERT(sdu_in);
  ASSERT(qmi_uim_global_ptr);

  memset(tlv_data, 0x00, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_SUBSCRIPTION_OK;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_DECRYPTED_IMSI;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (qmi_uim_get_halt_subscription_value() == FALSE)
  {
    UIM_MSG_HIGH_0("SUB OK req not supported.Halt sub config set to FALSE");
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
    goto send_result;
  }
  else
  {
    UIM_MSG_LOW_0("SUB OK req supported");
  }

  /* Only provisioning sessions are supported */
  if (!qmi_uim_util_is_session_provisioning(tlv_data[0].data.session_information.session_type))
  {
    UIM_MSG_ERR_1("Impossible to send SUB OK for a non provisioning session: 0x%x",
                  tlv_data[0].data.session_information.session_type);
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check the UIM configuration support for Trust Zone sub ok requests by
     getting encrypted IMSI required for sub ok value from EFS */
  if(qmi_uim_get_encrypted_sub_ok_value() && tlv_data[1].data.ok_for_subscription.value)
  {
    if(!tlv_data[2].present)
    {
      UIM_MSG_HIGH_0("SUB OK requires encrypted IMSI but none was supplied with request");
      errval = QMI_ERR_MISSING_ARG;
      goto send_result;
    }

    /* Read the IMSI through MMGSDI to compare to the encrypted IMSI */
    errval = qmi_uim_util_read_imsi_for_session_id(session_id, &imsi_data);
    if(errval != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_0("Could not read IMSI for encrypted sub ok");
      goto send_result;
    }

    if(tlv_data[2].data.decrypted_imsi.imsi_data.len != imsi_data.len ||
       memcmp(tlv_data[2].data.decrypted_imsi.imsi_data.imsi, imsi_data.imsi, imsi_data.len) != 0)
    {
      UIM_MSG_HIGH_0("Decrypted IMSI does not match card");
      errval = QMI_ERR_AUTHENTICATION_FAILED;
      goto send_result;
    }
    UIM_MSG_MED_0("Decrypted IMSI matches card. Proceed with sub ok");
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SUBSCRIPTION_OK_REQ,
                                                 cmd_buf_ptr,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Call MMGSDI API session subscription OK */
  mmgsdi_status = mmgsdi_session_subscription_ok(
                              session_id,
                              tlv_data[1].data.ok_for_subscription.value,
                              qmi_uimi_mmgsdi_callback,
                              (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_subscription_ok() */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_ATR()

  DESCRIPTION
    Retrieves the ATR of a specific card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_atr
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type         mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*         cb_userdata_ptr = NULL;
  uint8                           slot_index      = 0;
  qmi_uim_tlv_item_type           tlv_data[1];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Error if CARD_INSERTED is not sent out yet*/
  errval = uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if(errval != QMI_ERR_NONE ||
     slot_index >= QMI_UIM_MAX_CARD_COUNT ||
     slot_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[slot_index] == NULL)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }
  if (qmi_uim_global_ptr->card_state.card[slot_index]->card_state != QMI_UIM_CARD_STATE_PRESENT)
  {
    errval = QMI_ERR_SIM_NOT_INITIALIZED;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_ATR,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Execute MMGSDI function to get the ATR */
  mmgsdi_status = mmgsdi_get_atr(
                    qmi_uim_global_ptr->mmgsdi_client_id,
                    tlv_data[0].data.slot.slot,
                    qmi_uimi_mmgsdi_callback,
                    (mmgsdi_client_data_type)cb_userdata_ptr );

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_get_atr */


/*===========================================================================
  FUNCTION QMI_UIMI_OPEN_LOGICAL_CHANNEL()

  DESCRIPTION
    This function opens a logical channel to an application on a UICC card,
    with the specified AID value. The logical channel is exclusive for the
    control point and is not shared with any other client. The logical channel
    can be used to stream APDUs without interfering with the logical channels
    used by the modem or by other clients. The channel ID can be used in the
    QMI_UIM_SEND_APDU request to send the APDU on the logical channel.

    If the optional TLV with the AID is missing, the logical channel is created
    without selecting any application.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
  static dsm_item_type * qmi_uimi_open_logical_channel
  (
    void *            sp,
    void *            cmd_buf_p,
    void *            cl_sp,
    dsm_item_type **  sdu_in
  )
  {
    qmi_error_e_type             errval          = QMI_ERR_NONE;
    dsm_item_type *              response        = NULL;
    qmi_result_e_type            result          = QMI_RESULT_SUCCESS;
    boolean                      retval          = TRUE;
    qmi_uim_tlv_item_type        tlv_data[3];
    mmgsdi_select_rsp_enum_type  sel_rsp_type    = MMGSDI_SELECT_RSP_FCP;

    ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

    memset(tlv_data, 0, sizeof(tlv_data));
    tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
    tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
    tlv_data[1].type      = UIMI_TLV_TYPE_AID;
    tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;
    tlv_data[2].type      = UIMI_TLV_TYPE_FILE_CONTROL_INFO;
    tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_2;

    /* Parse TLV */
    errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }

    /* Check if request needs to be rejected due to security restrictions */
    if (qmi_uim_is_logical_channel_request_rejected(tlv_data[1].data.aid.aid))
    {
      errval = QMI_ERR_ACCESS_DENIED;
      goto send_result;
    }

    /* Open logical channel - This API cannot be used to close session */
    if (tlv_data[2].present)
    {
      sel_rsp_type = tlv_data[2].data.file_control_info.fci_type;
    }

    /* If AID is missing the session open is intended to select MF */
    errval = qmi_uim_execute_open_session_for_apdu(
                         tlv_data[0].data.slot.slot,
                         tlv_data[1].data.aid.aid,
                         sel_rsp_type,
                         cmd_buf_p,
                         (tlv_data[1].present == FALSE) ? TRUE : FALSE,
                         ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

send_result:
    /* Free the buffer used for data */
    qmi_uim_util_free_tlv_content(tlv_data, 3);

    if (errval == QMI_ERR_NONE)
    {
      return QMI_SVC_RESPONSE_PENDING;
    }

    result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                     : QMI_RESULT_FAILURE);
    retval = qmi_svc_put_result_tlv(&response, result, errval);
    CHECK_RETVAL();

    return response;
} /* qmi_uimi_open_logical_channel */


/*===========================================================================
  FUNCTION QMI_UIMI_REFRESH_REGISTER_ALL()

  DESCRIPTION
    Register for refresh of all files. This function sends immediately the
    response to the client, even if MMGSDI still needs to process it. This
    is consistent with all other refresh commands.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_refresh_register_all
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                response        = NULL;
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  qmi_result_e_type              result          = QMI_RESULT_SUCCESS;
  boolean                        retval          = TRUE;
  mmgsdi_session_id_type         session_id      = UIM_INVALID_SESSION_ID;
  boolean                        opening_session = FALSE;
  qmi_uim_tlv_item_type          tlv_data[3];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_REGISTER_FOR_REFRESH;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_REFRESH_VOTE_FOR_INIT;
  tlv_data[2].tag       = UIMI_TLV_RESP_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Make sure that we are not using an APDU session */
  if ((tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_1 ||
       tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_2 ||
       tlv_data[0].data.session_information.session_type == MMGSDI_NON_PROV_SESSION_SLOT_3) &&
       tlv_data[0].data.session_information.aid.data_len == QMI_UIM_TLV_CHANNEL_ID_LEN)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_REFRESH_REGISTER_ALL,
                                  tlv_data, 3,
                                  cmd_buf_p, cl_sp);
    if (errval == QMI_ERR_NONE)
    {
      opening_session = TRUE;
    }
  }
  else
  {
    /* Execute MMGSDI function. If vote for init TLV is absent,
       we default the vote to FALSE */
    errval = qmi_uim_execute_refresh_register_all(
                            session_id,
                            tlv_data[1].data.refresh_registration.value,
                            tlv_data[2].present ? tlv_data[2].data.refresh_vote_for_init.value : FALSE,
                            (qmi_cmd_buf_type *) cmd_buf_p);
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  if (opening_session)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  /* Send result */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_refresh_register_all() */


/*===========================================================================
  FUNCTION QMI_UIMI_SET_FILE_STATUS()

  DESCRIPTION
    Activate or deactivate a file.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_set_file_status
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                response        = NULL;
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  qmi_result_e_type              result          = QMI_RESULT_SUCCESS;
  boolean                        retval          = TRUE;
  mmgsdi_session_id_type         session_id      = UIM_INVALID_SESSION_ID;
  qmi_uim_tlv_item_type          tlv_data[3];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_FILE_STATUS;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_SET_FILE_STATUS,
                                  tlv_data, 3,
                                  cmd_buf_p, cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_set_file_status(
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            tlv_data[2].data.file_status.file_status,
                            cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       3,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  if (errval == QMI_ERR_NONE)
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  /* Send result */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_set_file_status() */


/*===========================================================================
  FUNCTION qmi_uimi_get_plmn_name_table_info()

  DESCRIPTION
    Gets the SE13 Table Release Date and Source

  PARAMETERS

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    QMI UIM service must be initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_plmn_name_table_info
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                   errval      = QMI_ERR_NONE;
  dsm_item_type *                    response    = NULL;
  boolean                            retval      = TRUE;
  qmi_result_e_type                  result      = QMI_RESULT_SUCCESS;
  mmgsdi_se13_table_details_info     se13_table_details_info;

  memset(&se13_table_details_info, 0x00, sizeof(mmgsdi_se13_table_details_info));

  if(MMGSDI_SUCCESS == mmgsdi_get_se13_table_release_info(&se13_table_details_info))
  {
    if (FALSE == qmi_uim_response_se13_table_release_date(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                                          &response,
                                                          &se13_table_details_info))
    {
      errval = QMI_ERR_INTERNAL;
      dsm_free_packet(&response);
    }

    if (FALSE == qmi_uim_response_se13_table_release_source(UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                                            &response,
                                                            &se13_table_details_info))
    {
      errval = QMI_ERR_INTERNAL;
      dsm_free_packet(&response);
    }
  }
  else
  {
    errval = QMI_ERR_INTERNAL;
  }

  /* Insert result item in the TLV */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_plmn_name_table_info */


/*===========================================================================
  FUNCTION QMI_UIMI_INCREASE()

  DESCRIPTION
    Increase the file on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_increase
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[4];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_INCREASE;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;
  tlv_data[3].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Update token info */
  token_info.is_valid = tlv_data[3].present;
  token_info.token = tlv_data[3].data.ind_token.value;

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_INCREASE,
                                  tlv_data, 4,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_increase(
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            tlv_data[2].data.increase_data.data,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       4,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
} /* qmi_uimi_increase() */


/*===========================================================================
  FUNCTION QMI_UIMI_RESELECT()

  DESCRIPTION
    Reselects an AID on the given channel

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_reselect
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type        mmgsdi_status   = MMGSDI_ERROR;
  qmi_uim_tlv_item_type          tlv_data[3];
  qmi_uimi_userdata_type*        cb_userdata_ptr = NULL;
  uint8                          i               = 0;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_LOGICAL_CHANNEL;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_SELECT_MODE;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (tlv_data[2].data.select_mode.mode != MMGSDI_SELECT_MODE_NEXT)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  for (i = 0; i < UIM_MAX_APDU_CHANNEL_COUNT; i++)
  {
    if (qmi_uim_global_ptr->apdu_channel[i] != NULL &&
        qmi_uim_global_ptr->apdu_channel[i]->slot ==
          tlv_data[0].data.slot.slot &&
        qmi_uim_global_ptr->apdu_channel[i]->logical_channel ==
          tlv_data[1].data.logical_channel.logical_channel &&
        qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id !=
         UIM_INVALID_SESSION_ID)
    {
      break;
    }
  }

  if (i == UIM_MAX_APDU_CHANNEL_COUNT)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  if (qmi_uim_global_ptr->apdu_sec_restr_value != QMI_UIM_APDU_SEC_RESTRICTIONS_NONE &&
      qmi_uim_global_ptr->apdu_channel[i]->opening_cl_sp->common.clid !=
          ((qmi_uimi_client_state_type *)cl_sp)->common.clid)
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_RESELECT,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Pack the channel index that can later be retrieve to updated the AID
     (in the global for this apdu_channel) returned by the card in SELECT
     response */
  cb_userdata_ptr->data.reselect_request.channel_index = i;

  mmgsdi_status = mmgsdi_session_reselect_aid(qmi_uim_global_ptr->apdu_channel[i]->mmgsdi_session_id,
                                              tlv_data[2].data.select_mode.mode,
                                              qmi_uimi_mmgsdi_callback,
                                              (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval, FALSE, 0, 0);
} /* qmi_uimi_reselect */


/*===========================================================================
  FUNCTION QMI_UIMI_SEND_STATUS()

  DESCRIPTION
    Sends the status command.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_send_status
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                 errval          = QMI_ERR_NONE;
  qmi_uim_tlv_item_type            tlv_data[3];
  mmgsdi_session_id_type           session_id;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_STATUS_CMD_MODE;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[2].type      = UIMI_TLV_TYPE_STATUS_CMD_RESP;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 3);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);
  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_SEND_STATUS,
                                  tlv_data,
                                  3,
                                  cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_status(
                            session_id,
                            (tlv_data[1].present) ?
                              tlv_data[1].data.status_cmd_mode.mode :
                              MMGSDI_STATUS_APP_NO_INDICATION,
                            (tlv_data[2].present) ?
                              tlv_data[2].data.status_cmd_resp.resp :
                              MMGSDI_STATUS_NO_DATA,
                            cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       3,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 3);

  return qmi_uimi_util_populate_response(errval, FALSE, 0, 0);
} /* qmi_uimi_send_status */


/*===========================================================================
  FUNCTION QMI_UIMI_SEARCH_RECORD()

  DESCRIPTION
    Search a pattern in records of a cyclic or linear fixed file on the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_search_record
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  mmgsdi_session_id_type         session_id    = UIM_INVALID_SESSION_ID;
  qmi_uimi_ind_token_info_type   token_info    = {FALSE, 0};
  qmi_uim_tlv_item_type          tlv_data[5];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_FILE_ID;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_SEARCH_PATTERN;
  tlv_data[2].tag       = UIMI_TLV_TAG_MANDATORY_3;
  tlv_data[3].type      = UIMI_TLV_TYPE_SEARCH_DATA;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[4].type      = UIMI_TLV_TYPE_IND_TOKEN;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 5);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Check if request needs to be rejected due to security restrictions */
  if (qmi_uim_util_is_request_rejected(tlv_data[0].data.session_information.session_type,
                                       tlv_data[0].data.session_information.aid,
                                       cl_sp))
  {
    errval = QMI_ERR_ACCESS_DENIED;
    goto send_result;
  }

  /* Set the path if the path length is 0. */
  if (tlv_data[1].data.file_id.file_access.file.path_type.path_len == 0)
  {
    errval = qmi_uim_util_set_file_path_from_file_id(tlv_data[0].data.session_information.session_type,
                                                     &tlv_data[1].data.file_id);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Update token info */
  token_info.is_valid = tlv_data[4].present;
  token_info.token = tlv_data[4].data.ind_token.value;

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    if(errval == QMI_ERR_DEVICE_NOT_READY)
    {
      goto send_result;
    }

    /* Try to open session */
    errval = qmi_uim_open_session(UIMI_CMD_VAL_SEARCH_RECORD,
                                  tlv_data, 5,
                                  token_info.is_valid ? NULL : cmd_buf_p,
                                  cl_sp);
  }
  else
  {
    /* Execute MMGSDI function */
    errval = qmi_uim_execute_search_record(
                            session_id,
                            tlv_data[1].data.file_id.file_access,
                            tlv_data[2].data.search_pattern.pattern,
                            (tlv_data[3].present) ?
                              tlv_data[3].data.search_data.first_record_num :
                              QMI_UIM_RECORD_1,
                            (tlv_data[3].present) ?
                              tlv_data[3].data.search_data.direction :
                              MMGSDI_SEARCH_FORWARD_FROM_REC_NUM,
                            token_info.is_valid ? &token_info.token : NULL,
                            token_info.is_valid ? NULL : cmd_buf_p,
                            ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

    /* Associate the Client with the requested Session (if it is a
       non-prov. Session) */
    if(errval == QMI_ERR_NONE)
    {
      (void)qmi_uim_util_update_non_prov_session_cl_sp(tlv_data,
                                                       5,
                                                       cl_sp);
    }
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 5);

  return qmi_uimi_util_populate_response(errval,
                                         token_info.is_valid,
                                         token_info.token,
                                         UIMI_TLV_RESP_TAG_OPTIONAL_3);
}/* qmi_uimi_search_record */


/*===========================================================================
  FUNCTION QMI_UIMI_PURGE_TEMPORARY_IDENTITY()

  DESCRIPTION
    This function purge guti from EFS and EF_EPSLOCI
    It verifies the temporary identity mask and returns an error
    if any of them is not verified.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type *  qmi_uimi_purge_temporary_identity
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                      errval                 = QMI_ERR_NONE;
  qmi_uim_tlv_item_type                 tlv_data[2];
  mmgsdi_session_id_type                session_id             = UIM_INVALID_SESSION_ID;
  mmgsdi_return_enum_type               mmgsdi_status          = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type              * cb_userdata_ptr        = NULL;
  mmgsdi_temporary_identity_enum_type   temporary_identity     = MMGSDI_PURGE_TEMPORARY_IDENTITY_UNKNOWN;
  boolean                               retval                 = TRUE;
  qmi_result_e_type                     result                 = QMI_RESULT_SUCCESS;
  dsm_item_type                       * response               = NULL;
  uint16                                purge_guti_mask        = 0;
  uint16                                purge_guti_mask_index  = 0;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SESSION_INFORMATION;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_TEMPORARY_IDENTITY_MASK;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve session id */
  errval = qmi_uim_util_session_type_to_session_id(&session_id,
                                                   tlv_data[0].data.session_information.session_type,
                                                   tlv_data[0].data.session_information.aid);

  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = qmi_uim_util_session_id_to_index(&purge_guti_mask_index, session_id);

  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Purge of GUTI is applicable only for GW sessions */
  if(tlv_data[1].data.temporary_identity_mask.guti)
  {
    switch (tlv_data[0].data.session_information.session_type)
    {
      case MMGSDI_GW_PROV_PRI_SESSION:
      case MMGSDI_GW_PROV_SEC_SESSION:
      case MMGSDI_GW_PROV_TER_SESSION:
        purge_guti_mask |= (1 << purge_guti_mask_index);
        temporary_identity = MMGSDI_PURGE_TEMPORARY_IDENTITY_GUTI;
        break;
      default:
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
    }
  }
  else
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Checking if client has already requested for the same mask,
     If client has requested for the same mask then return SUCCESS */
  if (((qmi_uimi_client_state_type *)cl_sp)->uim_info.purge_guti_mask &
       purge_guti_mask)
  {
    errval = QMI_ERR_NONE;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_PURGE_TEMPORARY_IDENTITY,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);

  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Store temporary identity mask */
  cb_userdata_ptr->data.purge_temporary_identity.purge_guti_mask =
    purge_guti_mask;

  mmgsdi_status = mmgsdi_session_purge_temporary_identity(session_id,
                                                          temporary_identity,
                                                          qmi_uimi_mmgsdi_callback,
                                                          (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  /* Send the response */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_purge_temporary_identity */


/*===========================================================================
  FUNCTION QMI_UIMI_SWITCH_SLOT_EXT()

  DESCRIPTION
    Switches the physical slot, logical slot mapping

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_switch_slot_ext
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *               response               = NULL;
  qmi_error_e_type              errval                 = QMI_ERR_INTERNAL;
  boolean                       retval                 = TRUE;
  qmi_uim_tlv_item_type         tlv_data;
  uim_logical_slot_config_type  logical_phy_mapping;
  uint8                         i                      = 0;
  boolean                       is_mapping_different   = FALSE;  

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&logical_phy_mapping, 0, sizeof(logical_phy_mapping));
  memset(&tlv_data, 0, sizeof(tlv_data));
  tlv_data.type      = UIMI_TLV_TYPE_SWITCH_SLOT_EXT;
  tlv_data.tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, &tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

#ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER
  for (i = 0; i < tlv_data.data.switch_slot_ext.logical_slot_len &&
              i < QMI_UIM_MAX_CARD_COUNT &&
              i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    logical_phy_mapping.slot[i] =
          tlv_data.data.switch_slot_ext.logical_slot[i];

    logical_phy_mapping.num_slots++;

    /* If the physical slot that is requested to be switched to or to be
       activated is already active */
    if(!qmi_uim_sub_mgr_is_physical_slot_active_for_logical_slot((uim_slot_type)(i + 1), logical_phy_mapping.slot[i]))
    {
      is_mapping_different = TRUE;
    }      
  }

  /* If the physical slot that is requested to be switched to or to be
     activated is already active, return */
  if(!is_mapping_different)
  {
    errval = QMI_ERR_NO_EFFECT;
    goto send_result;
  }

  /* Execute UIMDRV function to switch the slot */
  if(UIM_PASS != uim_switch_slot_mapping(&logical_phy_mapping, UIMDRV_SUB_MGR_CLIENT_QMI_UIM))
  {
    errval = QMI_ERR_INTERNAL;
  }
#else
  errval = QMI_ERR_NOT_SUPPORTED;
  goto send_result;
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */  

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(&tlv_data, 1);

  retval = qmi_svc_put_result_tlv(
             &response,
             (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
             errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_switch_slot_ext */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_PHYSICAL_SLOT_MAPPING()

  DESCRIPTION
    Gets current logical to physical slot mapping

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_uimi_get_physical_slot_mapping
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *               response        = NULL;
  qmi_error_e_type              errval          = QMI_ERR_NONE;
  boolean                       retval          = TRUE;
  uim_logical_slot_config_type  logical_phy_mapping;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(&logical_phy_mapping, 0, sizeof(logical_phy_mapping));

  /* Execute UIMDRV function to get the physical slot mapping */
  if((UIM_PASS != uim_get_logical_to_physical_mapping(&logical_phy_mapping)) ||
     (0 == logical_phy_mapping.num_slots) ||
     (QMI_UIM_MAX_CARD_COUNT < logical_phy_mapping.num_slots))
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* update the response with mapping info */
  if (FALSE == qmi_uim_response_get_physical_slot_mapping(
                  UIMI_TLV_RESP_TAG_OPTIONAL_1,
                  &response,
                  logical_phy_mapping))
  {
    errval = QMI_ERR_INTERNAL;
    dsm_free_packet(&response);
  }

send_result:
  retval = qmi_svc_put_result_tlv(
             &response,
             (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
             errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_physical_slot_mapping */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_EID()

  DESCRIPTION
    Get the eUICC ID for that Slot. The command will go to LPA or MMGSDI
    based on the flag whether card is consumer eUICC or M2M eUICC..

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_get_eid
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type         errval             = QMI_ERR_NONE;
  qmi_uim_tlv_item_type    tlv_data[1];
  uint8                    slot_index         = 0;
  uint8                    i                  = 0;
  boolean                  idx_found          = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot,
                                            &slot_index,
                                            qmi_uim_global_ptr->card_state.num_slots);

  if (errval != QMI_ERR_NONE )
  {
    goto send_result;
  }

  if (slot_index >= QMI_UIM_MAX_CARD_COUNT)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* If EID is already cached, return that synchronously */
  if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr != NULL)
  {
    for(i = 0; i < qmi_uim_global_ptr->physical_slots_info.num_slots; i++)
    {
      mmgsdi_slot_id_enum_type mmgsdi_slot = MMGSDI_SLOT_NONE;

      if (qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[i].activity_status == UIM_SLOT_STATE_INACTIVE)
      {
        continue;
      }

      errval = qmi_uim_util_uim_slot_id_to_mmgsdi_slot_id(
                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[i].logical_slot,
                 &mmgsdi_slot);
      if (errval != QMI_ERR_NONE)
      {
        goto send_result;
      }

      if(tlv_data[0].data.slot.slot == mmgsdi_slot)
      {
        idx_found = TRUE;
        break;
      }
    }
    if(TRUE == idx_found)
    {
      if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[i].eid_len > 0)
      {
        do
        {
          dsm_item_type              *response_ptr       = NULL;
          boolean                     retval             = TRUE;

          PACKED struct PACKED_POST
          {
            uint8 eid_len;
            uint8 eid[QMI_UIM_EID_LEN_V01];
          }tlv_cached_eid;

          tlv_cached_eid.eid_len = sizeof(tlv_cached_eid.eid);
          memscpy((void *)tlv_cached_eid.eid,
                  sizeof(tlv_cached_eid.eid),
                  qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[i].eid,
                  qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[i].eid_len);

          if(FALSE == qmi_svc_put_param_tlv(&response_ptr,
                                            UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                            sizeof(tlv_cached_eid),
                                            (void *)&tlv_cached_eid))
          {
            errval = QMI_ERR_NO_MEMORY;
            dsm_free_packet(&response_ptr);
            goto send_result;
          }

          qmi_uim_util_free_tlv_content(tlv_data, 1);

          retval = qmi_svc_put_result_tlv(&response_ptr,
                                          (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
                                          errval);
          return response_ptr;
        }while(0);
      }
    }
  }

  /* If EID is already cached, return that synchronously, this cached value is
     populated when get_eid request is received for first time */

  if(qmi_uim_global_ptr->eid_data[slot_index].eid_valid)
  {
    dsm_item_type              *response_ptr       = NULL;
    boolean                     retval             = TRUE;

    PACKED struct PACKED_POST
    {
      uint8 eid_len;
      uint8 eid[QMI_UIM_EID_LEN_V01];
    }tlv_cached_eid;

    tlv_cached_eid.eid_len = sizeof(tlv_cached_eid.eid);
    memscpy((void *)tlv_cached_eid.eid,
            sizeof(tlv_cached_eid.eid),
            qmi_uim_global_ptr->eid_data[slot_index].eid,
            qmi_uim_global_ptr->eid_data[slot_index].eid_len);

    if(FALSE == qmi_svc_put_param_tlv(&response_ptr,
                                       UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                       sizeof(tlv_cached_eid),
                                       (void *)&tlv_cached_eid))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response_ptr);
      goto send_result;
    }

    qmi_uim_util_free_tlv_content(tlv_data, 1);

    retval = qmi_svc_put_result_tlv(&response_ptr,
                                    (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
                                    errval);
    return response_ptr;
  }

  errval = qmi_uimi_execute_m2m_get_eid(tlv_data[0].data.slot.slot,
                                        cmd_buf_p,
                                        cl_sp);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
}/* qmi_uimi_get_eid */


/*===========================================================================

                             EVENT FUNCTIONS

===========================================================================*/


/*===========================================================================
  FUNCTION QMI_UIM_UPDATE_CARD_STATUS_BY_REFRESH_EVT

  DESCRIPTION
    Updates the status of the card when a refresh is started

  PARAMETERS
    session_id      : session id
    refresh_evt_ptr : refresh event

  RETURN VALUE
    uint16          : number of sent indications

  DEPENDENCIES
    None

  SIDE EFFECTS
    Overwrites the table of services interested in the indication
    in the global variable.
===========================================================================*/
static void qmi_uim_update_card_status_by_refresh_evt
(
  const mmgsdi_refresh_evt_info_type * refresh_evt_ptr
)
{
  uint16    app_index          = UIM_INVALID_APP_INDEX;

  ASSERT(qmi_uim_global_ptr && refresh_evt_ptr != NULL && refresh_evt_ptr->stage == MMGSDI_REFRESH_STAGE_START);

  switch(refresh_evt_ptr->mode)
  {
    case MMGSDI_REFRESH_RESET:
    case MMGSDI_REFRESH_RESET_AUTO:
      qmi_uim_util_remove_all_aid_in_slot(refresh_evt_ptr->slot);
      qmi_uim_remove_non_provisioning_and_apdu_session(refresh_evt_ptr->slot);
      break;

    case MMGSDI_REFRESH_NAA_INIT:
    case MMGSDI_REFRESH_NAA_INIT_FCN:
    case MMGSDI_REFRESH_NAA_INIT_FULL_FCN:
      app_index = qmi_uim_util_find_aid(&refresh_evt_ptr->aid, refresh_evt_ptr->slot);
      if (app_index < QMI_UIM_MAX_APP_COUNT &&
          qmi_uim_global_ptr->card_state.application[app_index] != NULL)
      {
        qmi_uim_global_ptr->card_state.application[app_index]->app_state = QMI_UIM_APP_STATE_DETECTED;
        qmi_uim_global_ptr->card_state.application[app_index]->perso_state = QMI_UIM_PERSO_STATE_UNKNOWN;
      }
      break;

    case MMGSDI_REFRESH_NAA_APP_RESET:
    case MMGSDI_REFRESH_3G_SESSION_RESET:
      app_index = qmi_uim_util_find_aid(&refresh_evt_ptr->aid, refresh_evt_ptr->slot);
      if (app_index < QMI_UIM_MAX_APP_COUNT &&
          qmi_uim_global_ptr->card_state.application[app_index] != NULL)
      {
        qmi_uim_global_ptr->card_state.application[app_index]->app_state = QMI_UIM_APP_STATE_DETECTED;
        qmi_uim_global_ptr->card_state.application[app_index]->perso_state = QMI_UIM_PERSO_STATE_UNKNOWN;
        qmi_uim_global_ptr->card_state.application[app_index]->pin1_state = MMGSDI_PIN_STATUS_NOT_INITIALIZED;
        qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries = 0;
        qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries = 0;
        qmi_uim_global_ptr->card_state.application[app_index]->pin2_state = MMGSDI_PIN_STATUS_NOT_INITIALIZED;
        qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries = 0;
        qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries = 0;
      }
      break;

    default:
      /* Nothing to do */
      break;
  }
} /* qmi_uim_update_card_status_by_refresh_evt */


/*===========================================================================
  FUNCTION QMI_UIM_CHECK_SLOT_DETECTION_VALIDITY

  DESCRIPTION
    Checks the status of all the available slots to see if a detection started
    flag was set. If so, returns TRUE indicating that the IND can be skipped.
    Note that the IND will be skipped until the slowest slot receives an
    inserted or error event.

  PARAMETERS
    None

  RETURN VALUE
    boolean        : indicating if the INS can be skipped

  DEPENDENCIES
    None

  SIDE EFFECTS
    Skip of card status IND possible.
===========================================================================*/
static boolean qmi_uim_check_slot_detection_validity
(
  void
)
{
  uint8         slot_id  = 0;

  ASSERT(qmi_uim_global_ptr);

  for (slot_id = 0;
       (slot_id < qmi_uim_global_ptr->card_state.num_slots) &&
       (slot_id < QMI_UIM_MAX_CARD_COUNT);
       slot_id++)
  {
    if (qmi_uim_global_ptr->card_state.card[slot_id] == NULL)
    {
      continue;
    }

    /* If card on one slot is in SIM BUSY state, we should not block status
       indication for card on other slot. */
    if (qmi_uim_global_ptr->card_state.card[slot_id]->sim_detection_started &&
        !qmi_uim_global_ptr->card_state.card[slot_id]->sim_busy)
    {
      return TRUE;
    }
  }

  return FALSE;
} /* qmi_uim_check_slot_detection_validity */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_CARD_DETECTION_STARTED_EVT()

  DESCRIPTION
    This function processes the card detection started event from MMGSDI. Note
    that we don't send any card status INDs for this MMGSDI event.

  PARAMETERS
    card_detection_started_evt_ptr        : pointer to event

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_card_detection_started_evt
(
  const mmgsdi_card_detection_started_evt_info_type * card_detection_started_evt_ptr
)
{
  uint8                             card_index       = 0;

  ASSERT(card_detection_started_evt_ptr && qmi_uim_global_ptr);

  UIM_MSG_HIGH_1("qmi_uim_process_card_detection_started_evt, slot 0x%x",
                 card_detection_started_evt_ptr->slot);

  if (uimqmi_mmgsdi_slot_to_slot_index(card_detection_started_evt_ptr->slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return;
  }

  /* MMGSDI sends detection started event before any other card event on that slot.
     We re-confirm it before storing */
  if ((qmi_uim_global_ptr->card_state.card[card_index]->sim_detection_started == FALSE) &&
      (qmi_uim_global_ptr->card_state.card[card_index]->card_state != QMI_UIM_CARD_STATE_PRESENT))
  {
    qmi_uim_global_ptr->card_state.card[card_index]->sim_detection_started = TRUE;
  }
} /* qmi_uim_process_card_detection_started_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_CARD_INSERTED_EVT()

  DESCRIPTION
    This function processes the card inserted event from MMGSDI

  PARAMETERS
    card_inserted_evt_ptr            : pointer to event

  RETURN VALUE
    qmi_uim_card_update_required_type : status of card update required booleans

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_uim_card_update_required_type qmi_uim_process_card_inserted_evt
(
  const mmgsdi_card_inserted_evt_info_type * card_inserted_evt_ptr
)
{
  qmi_uim_card_update_required_type result                        = {FALSE, FALSE};
  uint16                            i                             = 0;
  uint8                             card_index                    = 0;
  boolean                           prov_app_present              = FALSE;
  boolean                           lpa_modem_solution            = TRUE;
  uint8                             lpa_ap_support_nv_content[UIM_COMMON_LPA_EFS_ITEM_LEN]
                                                                  = {0};

  ASSERT(card_inserted_evt_ptr && qmi_uim_global_ptr);

  UIM_MSG_HIGH_1("qmi_uim_process_card_inserted_evt, slot 0x%x",
                 card_inserted_evt_ptr->slot);

  if (uimqmi_mmgsdi_slot_to_slot_index(card_inserted_evt_ptr->slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return result;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return result;
  }

  /* Update card state */
  if (qmi_uim_global_ptr->card_state.card[card_index]->card_state != QMI_UIM_CARD_STATE_PRESENT)
  {
    qmi_uim_global_ptr->card_state.card[card_index]->card_state = QMI_UIM_CARD_STATE_PRESENT;
    qmi_uim_global_ptr->card_state.card[card_index]->card_error = QMI_UIM_CARD_ERROR_UNKNOWN;
    result.card_update_required = TRUE;
  }

  /* Card inserted implies end of sim busy & detection started phase,
     so clear those flags for this slot */
  qmi_uim_global_ptr->card_state.card[card_index]->sim_busy = FALSE;
  qmi_uim_global_ptr->card_state.card[card_index]->sim_detection_started = FALSE;

  /* Remove applications on that slot... they should be already zero */
  qmi_uim_util_remove_all_aid_in_slot(card_inserted_evt_ptr->slot);

  qmi_uim_global_ptr->is_euicc_card[card_index] = card_inserted_evt_ptr->is_euicc;

  /* There is no need to check qmi_uim_is_euicc_card because it is possible
     that eUICC was removed and the new card is a normal card. Profile info
     was anyways cleared during card removed handling but to be on safe side,
     we need to clear the profile data here also */
  qmi_uim_clear_profile_info(card_inserted_evt_ptr->slot);

  /* Check LPA support EFS. Byte 2 determines whether LPA functionality
     is controlled by APPs or not. If LPA is not AP centric, query profile info */
  if (qmi_uim_global_ptr->is_euicc_card[card_index])
  {
    lpa_is_lpa_supported(NULL, NULL, NULL, NULL, &lpa_modem_solution);
  }

  /* If card is eUICC, then cache the profile information as profile_id
     to ICC-ID mapping is required for any LPA operation. We are doing
     this at card_inserted so that during handling of LPA request response
     will be quick  */
  if (qmi_uim_global_ptr->is_euicc_card[card_index] && lpa_modem_solution)
  {
    qmi_error_e_type         errval          = QMI_ERR_INTERNAL;
    qmi_uimi_userdata_type  *cb_userdata_ptr = NULL;

    cb_userdata_ptr = uimqmi_malloc(sizeof(qmi_uimi_userdata_type));
    if (cb_userdata_ptr != NULL)
    {
      memset(cb_userdata_ptr, 0, sizeof(qmi_uimi_userdata_type));
      cb_userdata_ptr->data.lpa.slot             = card_inserted_evt_ptr->slot;
      cb_userdata_ptr->data.lpa.is_profile_query = TRUE;

      errval = qmi_uim_query_euicc_profile_info(cb_userdata_ptr);
      UIM_MSG_MED_1("Internally triggered get_profile_info result: 0x%x", errval);
    }
  }

  /* Add applications to the global structure */
  for (i = 0; i < card_inserted_evt_ptr->num_aids_avail && i < MMGSDI_MAX_APP_INFO; i++)
  {
    if (qmi_uim_util_add_aid(&card_inserted_evt_ptr->aid_info[i],
                             card_inserted_evt_ptr->slot) != UIM_INVALID_APP_INDEX)
    {
      result.card_update_required = TRUE;

      /* If there is at least 1 prov app then we dont require to send the
         reduced card status indication */
      if(card_inserted_evt_ptr->aid_info[i].app_type == MMGSDI_APP_SIM ||
         card_inserted_evt_ptr->aid_info[i].app_type == MMGSDI_APP_RUIM ||
         card_inserted_evt_ptr->aid_info[i].app_type == MMGSDI_APP_USIM ||
         card_inserted_evt_ptr->aid_info[i].app_type == MMGSDI_APP_CSIM)
      {
        prov_app_present = TRUE;
      }
    }
  }

  /* If the card type is non telecom then mark the card as non telecom */
  if(card_inserted_evt_ptr->card_type == MMGSDI_CARD_NON_TELECOM)
  {
    qmi_uim_global_ptr->card_state.card[card_index]->is_non_telecom_card = TRUE;
  }

  if(qmi_uim_global_ptr->is_euicc_card[card_index] &&
     card_inserted_evt_ptr->num_aids_avail == 0)
  {
    /* we should send status change indication to the clients in case
       this is an eUICC with no apps.
       This special case handling is required to cover the following case:
       1. eUICC is already present with some profile/apps on it
       2. REFRESH_RESET occurs such that the eUICC now doesnt have
          any activate profile on it (so, number_aids_avail is zero).
       3. MMGDSI sends out CARD_INSERTED_EVT (note that card_state in QMIUIM
          is already QMI_UIM_CARD_STATE_PRESENT before QMIUIM receives this
          CARD_INSERTED_EVT from MMGSDI).
       4. Now, we reach this special case if-condition and make sure to update
          the card_update_required variable so that the clients who have
          registered for "Card Status" or "Reduced Card Status" indications,
          receive the indication as a result of this CARD_INSERTED_EVT.
       NOTE that we have this special case if-condition for eUICC only.
       This is because for non-eUICC, MMGSDI is never expected to send out
       CARD_INSERTED_EVT and hence QMIUIM doesnt need to take that case into
       account. */
    result.card_update_required = TRUE;
  }

  /* we send the reduced status change indication to the clients if
     1) Card update is required.
     2) No app / no prov app is present in EF DIR or provisioning is manual
     to select the app(s) for activation */
  if(result.card_update_required &&
     (!prov_app_present || !qmi_uim_global_ptr->automatic_provisioning))
  {
    result.reduced_card_update_required = TRUE;
  }

  return result;
} /* qmi_uim_process_card_inserted_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SESSION_CHANGED_EVT()

  DESCRIPTION
    This function processes the session changed event from MMGSDI

  PARAMETERS
    session_id                       : session id information
    session_changed_evt_ptr          : pointer to event

  RETURN VALUE
    qmi_uim_card_update_required_type : status of card update required booleans

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_uim_card_update_required_type qmi_uim_process_session_changed_evt
(
  mmgsdi_session_id_type                       session_id,
  const mmgsdi_session_changed_evt_info_type * session_changed_evt_ptr
)
{
  uint8                             card_index     = 0;
  uint16                            session_index  = 0;
  uint16                            index          = 0;
  uint16                            app_index      = UIM_INVALID_APP_INDEX;
  uint16                            refresh_index  = 0;
  uint16*                           prov_index_ptr = NULL;
  boolean                           app_present    = FALSE;
  qmi_uim_card_update_required_type result         = {FALSE, FALSE};

  ASSERT(session_changed_evt_ptr && qmi_uim_global_ptr);

  UIM_MSG_HIGH_3("qmi_uim_process_session_changed_evt, slot 0x%x, app_type 0x%x, activated 0x%x",
                 session_changed_evt_ptr->app_info.slot,
                 session_changed_evt_ptr->app_info.app_data.app_type,
                 session_changed_evt_ptr->activated);

  if (uimqmi_mmgsdi_slot_to_slot_index(session_changed_evt_ptr->app_info.slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return result;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return result;
  }

  /* Update card state, find or add aid on in case of activation */
  if (session_changed_evt_ptr->activated)
  {
    if (qmi_uim_global_ptr->card_state.card[card_index]->card_state != QMI_UIM_CARD_STATE_PRESENT)
    {
      qmi_uim_global_ptr->card_state.card[card_index]->card_state = QMI_UIM_CARD_STATE_PRESENT;
      qmi_uim_global_ptr->card_state.card[card_index]->card_error = QMI_UIM_CARD_ERROR_UNKNOWN;
      result.card_update_required = TRUE;
    }
  }

  /* Actions to perform in case of session being de-activated */
  if (!session_changed_evt_ptr->activated)
  {
    /* Clear the refresh status for the session */
    if (qmi_uim_util_session_id_to_index(&refresh_index, session_id) == QMI_ERR_NONE)
    {
      ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

      qmi_uim_clean_up_refresh_evt_info(refresh_index);
      if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] != NULL)
      {
        uimqmi_free(qmi_uim_global_ptr->refresh_info_ptr[refresh_index]);
        qmi_uim_global_ptr->refresh_info_ptr[refresh_index] = NULL;
      }
    }

    /* Remove cached record types */
    qmi_uim_remove_cached_record_type(session_id);

    qmi_uim_send_purge_temporary_identity_ind_to_all(0xFFFF, MMGSDI_ERROR, session_id);
  }

  /* Find the AID in the card state structure or add it */
  app_index = qmi_uim_util_find_aid(&session_changed_evt_ptr->app_info.app_data,
                                    session_changed_evt_ptr->app_info.slot);

  /* Add application if not found, only if this is an activated case */
  if (app_index == UIM_INVALID_APP_INDEX &&
      session_changed_evt_ptr->activated)
  {
    app_index = qmi_uim_util_add_aid(&session_changed_evt_ptr->app_info.app_data,
                                     session_changed_evt_ptr->app_info.slot);
  }

  /* Check index */
  if (app_index == UIM_INVALID_APP_INDEX ||
      app_index >= QMI_UIM_MAX_APP_COUNT ||
      qmi_uim_global_ptr->card_state.application[app_index] == NULL)
  {
    UIM_MSG_ERR_0("Impossible to find/add application");
    return result;
  }

  /* Reset the App state in case of deactivation */
  if ((!session_changed_evt_ptr->activated) &&
      (qmi_uim_global_ptr->card_state.application[app_index]->app_state != QMI_UIM_APP_STATE_DETECTED))
  {
    qmi_uim_global_ptr->card_state.application[app_index]->app_state   = QMI_UIM_APP_STATE_DETECTED;
    qmi_uim_global_ptr->card_state.application[app_index]->perso_state = QMI_UIM_PERSO_STATE_UNKNOWN;
    result.card_update_required = TRUE;
  }

  /* Update the PINs state in case of deactivation */
  if (!session_changed_evt_ptr->activated)
  {
    qmi_uim_global_ptr->card_state.application[app_index]->pin1_state =
     MMGSDI_PIN_STATUS_NOT_INITIALIZED;
    qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries = 0;
    qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries = 0;

    qmi_uim_global_ptr->card_state.application[app_index]->pin2_state =
      MMGSDI_PIN_STATUS_NOT_INITIALIZED;
    qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries = 0;
    qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries = 0;

    qmi_uim_global_ptr->card_state.application[app_index]->upin = FALSE;
  }

  /* Find the index for this session id: handle only
     provisioning sessions */
  for (session_index = 0; session_index < QMI_UIM_MAX_PROV_SESSIONS; session_index++)
  {
    if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[session_index])
    {
      prov_index_ptr = &qmi_uim_global_ptr->card_state.index_gw[session_index];
      break;
    }
    else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[session_index])
    {
      prov_index_ptr = &qmi_uim_global_ptr->card_state.index_1x[session_index];
      break;
    }
  }

  /* Update the subscription app index */
  if (prov_index_ptr != NULL)
  {
    uint16 old_value = *prov_index_ptr;

    if (session_changed_evt_ptr->activated)
    {
      *prov_index_ptr = app_index;
    }
    else
    {
      *prov_index_ptr = UIM_INVALID_APP_INDEX;
    }

    if (*prov_index_ptr != old_value)
    {
      result.card_update_required = TRUE;
    }
  }

  if(!result.card_update_required)
  {
    return result;
  }

  /* For reduced card status indications:
     1) In case of manual provisioning: QMI UIM sends out the indication once it receives the
        session changed event (irrespective of app type / activation status).
     2) In case of auto provisioning:
        a) If QMI UIM receives session changed event with activation status as deactivated then
           it sends the card status indication (irrespective of app type)
        b) If QMI UIM receives session changed event for app type SIM/USIM with activation status
           as activated then it will wait for the RUIM's/CSIM's session changed event (if RUIM/CSIM
           app is available  in the card) with successful activation before sending the reduced card
           status indication.
        c) If QMI UIM receives session changed event for app type RUIM/CSIM with activation status
           as activated then it will wait for the SIM's/USIM's session changed event (if SIM/USIM
           app is available  in the card) with successful activation before sending the reduced card
           status indication.
        d) If QMI UIM received session changed event with activation status as activated for SIM/USIM
           app and dont receive subsequent session changed event for RUIM/CSIM app then it will not
           send the reduced card status indication. SIM/USIM app will continue its initialization sequence
           and the indication will be postponed until next card status indication. This is a know limitation
           and this is a corner case where the probability of seeing this in practical scenarios is very less */

  if(!session_changed_evt_ptr->activated)
  {
    qmi_uim_send_prov_session_closed_ind_to_all(session_id,
                                                session_changed_evt_ptr->app_info.slot,
                                                session_changed_evt_ptr->cause);
    result.reduced_card_update_required = TRUE;
  }
  else
  {
    if(qmi_uim_global_ptr->automatic_provisioning)
    {
      if(session_changed_evt_ptr->app_info.app_data.app_type == MMGSDI_APP_USIM ||
         session_changed_evt_ptr->app_info.app_data.app_type == MMGSDI_APP_SIM)
      {
        for(app_index = 0; app_index < qmi_uim_global_ptr->card_state.num_apps; app_index++)
        {
          if((qmi_uim_global_ptr->card_state.application[app_index]->app_type == MMGSDI_APP_CSIM ||
              qmi_uim_global_ptr->card_state.application[app_index]->app_type == MMGSDI_APP_RUIM) &&
             qmi_uim_global_ptr->card_state.application[app_index]->slot == session_changed_evt_ptr->app_info.slot)
          {
            for(session_index = 0; session_index < QMI_UIM_MAX_PROV_SESSIONS; session_index++)
            {
              index = qmi_uim_global_ptr->card_state.index_1x[session_index];
              if(index < QMI_UIM_MAX_APP_COUNT &&
                 index < qmi_uim_global_ptr->card_state.num_apps &&
                 qmi_uim_global_ptr->card_state.application[index]->slot == session_changed_evt_ptr->app_info.slot)
              {
                result.reduced_card_update_required = TRUE;
                return result;
              }
            }
            app_present = TRUE;
            break;
          }
        }
        if(!app_present)
        {
          result.reduced_card_update_required = TRUE;
        }
      }
      else if(session_changed_evt_ptr->app_info.app_data.app_type == MMGSDI_APP_CSIM ||
              session_changed_evt_ptr->app_info.app_data.app_type == MMGSDI_APP_RUIM)
      {
        for(app_index = 0; app_index < qmi_uim_global_ptr->card_state.num_apps; app_index++)
        {
          if((qmi_uim_global_ptr->card_state.application[app_index]->app_type == MMGSDI_APP_USIM ||
              qmi_uim_global_ptr->card_state.application[app_index]->app_type == MMGSDI_APP_SIM) &&
             qmi_uim_global_ptr->card_state.application[app_index]->slot == session_changed_evt_ptr->app_info.slot)
          {
            for(session_index = 0; session_index < QMI_UIM_MAX_PROV_SESSIONS; session_index++)
            {
              index = qmi_uim_global_ptr->card_state.index_gw[session_index];
              if(index < QMI_UIM_MAX_APP_COUNT &&
                 index < qmi_uim_global_ptr->card_state.num_apps &&
                 qmi_uim_global_ptr->card_state.application[index]->slot == session_changed_evt_ptr->app_info.slot)
              {
                result.reduced_card_update_required = TRUE;
                return result;
              }
            }
            app_present = TRUE;
            break;
          }
        }
        if(!app_present)
        {
          result.reduced_card_update_required = TRUE;
        }
      }
    }
    else
    {
      result.reduced_card_update_required = TRUE;
    }
  }

  return result;
} /* qmi_uim_process_session_changed_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_PIN_EVTS()

  DESCRIPTION
    This function processes the PIN events from MMGSDI

  PARAMETERS
    evt_enum                          : event type
    pin_evt_ptr                       : pointer to event

  RETURN VALUE
    qmi_uim_card_update_required_type : status of card update required booleans

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_uim_card_update_required_type qmi_uim_process_pin_evts
(
  mmgsdi_events_enum_type          evt_enum,
  const mmgsdi_pin_evt_info_type * pin_evt_ptr
)
{
  uint16                            i           = 0;
  uint16                            app_index   = UIM_INVALID_APP_INDEX;
  uint8                             card_index  = 0;
  qmi_uim_card_update_required_type result      = {FALSE, FALSE};

  ASSERT(pin_evt_ptr && qmi_uim_global_ptr);

  UIM_MSG_HIGH_1("qmi_uim_process_pin_evts, pin_evt: 0x%x", evt_enum);

  if (pin_evt_ptr->pin_info.status == MMGSDI_PIN_NOT_FOUND)
  {
    UIM_MSG_HIGH_1("qmi_uim_process_pin_evts: ignore PIN evt %d",
                   pin_evt_ptr->pin_info.status);
    return result;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(pin_evt_ptr->slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return result;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return result;
  }

  /* Update card state */
  qmi_uim_global_ptr->card_state.card[card_index]->card_state = QMI_UIM_CARD_STATE_PRESENT;
  qmi_uim_global_ptr->card_state.card[card_index]->card_error = QMI_UIM_CARD_ERROR_UNKNOWN;

  switch (evt_enum)
  {
    case MMGSDI_PIN1_EVT:
      for (i = 0; i < pin_evt_ptr->num_aids && i < MMGSDI_MAX_APP_INFO; i++)
      {
        /* Update all the apps that are using this PIN1 */
        app_index = qmi_uim_util_find_aid(&pin_evt_ptr->aid_type[i], pin_evt_ptr->slot);
        if (app_index < QMI_UIM_MAX_APP_COUNT &&
            qmi_uim_global_ptr->card_state.application[app_index] != NULL)
        {
          if (qmi_uim_update_pin_state(pin_evt_ptr->pin_info.pin_id,
                                       pin_evt_ptr->pin_info.status,
                                       pin_evt_ptr->pin_info.pin_replacement,
                                       app_index, card_index))
          {
            result.card_update_required = TRUE;
          }

          /* Number of retries */
          if ((pin_evt_ptr->pin_info.valid_num_retries) &&
              (qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries !=
              (uint8)pin_evt_ptr->pin_info.num_retries))
          {
            qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries =
              (uint8)pin_evt_ptr->pin_info.num_retries;

            /* Mark card update required as true (update) */
            result.card_update_required = TRUE;
          }
          if ((pin_evt_ptr->pin_info.valid_num_unblock_retries) &&
              (qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries !=
              (uint8)pin_evt_ptr->pin_info.num_unblock_retries))
          {
            qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries =
              (uint8)pin_evt_ptr->pin_info.num_unblock_retries;

            /* Mark card update required as true (update) */
            result.card_update_required = TRUE;
          }

          /* Update PIN1 retries and state of all the apps sharing PIN1 */
          qmi_uim_update_pin1_other_apps(&pin_evt_ptr->pin_info,
                                         pin_evt_ptr->slot,
                                         app_index);

          /* QMI UIM sends the reduced card status indication if
             1) PIN1 is not replaced by UPIN
             1) card update is required and
             2) App state is Ready / Illegal.
             3) PIN1 state is not disabed and not enabled verified.*/
          if((result.card_update_required) &&
             (qmi_uim_global_ptr->card_state.application[app_index]->upin == FALSE) &&
             (qmi_uim_global_ptr->card_state.application[app_index]->app_state == QMI_UIM_APP_STATE_READY ||
              qmi_uim_global_ptr->card_state.application[app_index]->app_state == QMI_UIM_APP_STATE_ILLEGAL ||
              (!(qmi_uim_global_ptr->card_state.application[app_index]->pin1_state == MMGSDI_PIN_DISABLED ||
                 qmi_uim_global_ptr->card_state.application[app_index]->pin1_state == MMGSDI_PIN_ENABLED_VERIFIED))))
          {
            result.reduced_card_update_required = TRUE;
          }
        }
      }
      break;

    case MMGSDI_PIN2_EVT:
      /* Each PIN2 can be associated with 1 app only */
      if (pin_evt_ptr->num_aids == 1)
      {
        app_index = qmi_uim_util_find_aid(&pin_evt_ptr->aid_type[i], pin_evt_ptr->slot);
        if (app_index < QMI_UIM_MAX_APP_COUNT &&
            qmi_uim_global_ptr->card_state.application[app_index] != NULL)
        {
          if (qmi_uim_update_pin_state(pin_evt_ptr->pin_info.pin_id,
                                       pin_evt_ptr->pin_info.status,
                                       pin_evt_ptr->pin_info.pin_replacement,
                                       app_index, card_index))
          {
            result.card_update_required = TRUE;
          }

          /* Number of retries */
          if ((pin_evt_ptr->pin_info.valid_num_retries) &&
              (qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries !=
              (uint8)pin_evt_ptr->pin_info.num_retries))
          {
            qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries =
              (uint8)pin_evt_ptr->pin_info.num_retries;

            /* Mark card update required as true (update) */
            result.card_update_required = TRUE;
          }
          if ((pin_evt_ptr->pin_info.valid_num_unblock_retries) &&
              (qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries !=
              (uint8)pin_evt_ptr->pin_info.num_unblock_retries))
          {
            qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries =
              (uint8)pin_evt_ptr->pin_info.num_unblock_retries;

            /* Mark card update required as true (update) */
            result.card_update_required = TRUE;
          }

          /* If card update is required and app state is ready / illegal then QMI UIM will send the reduced card indication */
          if(result.card_update_required &&
             (qmi_uim_global_ptr->card_state.application[app_index]->app_state == QMI_UIM_APP_STATE_READY ||
              qmi_uim_global_ptr->card_state.application[app_index]->app_state == QMI_UIM_APP_STATE_ILLEGAL))
          {
            result.reduced_card_update_required = TRUE;
          }
        }
      }
      else
      {
        UIM_MSG_ERR_1("Invalid number of applications for PIN2 event (0x%x)",
                      pin_evt_ptr->num_aids);
      }
      break;

    case MMGSDI_UNIVERSAL_PIN_EVT:
      if (qmi_uim_update_pin_state(pin_evt_ptr->pin_info.pin_id,
                                   pin_evt_ptr->pin_info.status,
                                   pin_evt_ptr->pin_info.pin_replacement,
                                   app_index, card_index))
      {
        result.card_update_required = TRUE;
      }

      /* Number of retries */
      if ((pin_evt_ptr->pin_info.valid_num_retries) &&
          (qmi_uim_global_ptr->card_state.card[card_index]->upin_num_retries !=
          (uint8)pin_evt_ptr->pin_info.num_retries))
      {
        qmi_uim_global_ptr->card_state.card[card_index]->upin_num_retries =
          (uint8)pin_evt_ptr->pin_info.num_retries;

        /* Mark card update required as true (update) */
        result.card_update_required = TRUE;
      }
      if ((pin_evt_ptr->pin_info.valid_num_unblock_retries) &&
          (qmi_uim_global_ptr->card_state.card[card_index]->upuk_num_retries !=
          (uint8)pin_evt_ptr->pin_info.num_unblock_retries))
      {
        qmi_uim_global_ptr->card_state.card[card_index]->upuk_num_retries =
          (uint8)pin_evt_ptr->pin_info.num_unblock_retries;

        /* Mark card update required as true (update) */
        result.card_update_required = TRUE;
      }

      if(result.card_update_required)
      {
        for (i = 0; i < pin_evt_ptr->num_aids && i < MMGSDI_MAX_APP_INFO; i++)
        {
          app_index = qmi_uim_util_find_aid(&pin_evt_ptr->aid_type[i], pin_evt_ptr->slot);

          /* QMI UIM sends the reduced card status indication if
             1) PIN1 is replaced by UPIN and
             2) The card update is required and
             3) app state is ready or illegal or
             4) UPIN state is  not disabed and not enabled verified */
          if ((app_index < QMI_UIM_MAX_APP_COUNT) &&
              (qmi_uim_global_ptr->card_state.application[app_index] != NULL) &&
              (qmi_uim_global_ptr->card_state.application[app_index]->upin) &&
              (qmi_uim_global_ptr->card_state.application[app_index]->app_state == QMI_UIM_APP_STATE_READY ||
               qmi_uim_global_ptr->card_state.application[app_index]->app_state == QMI_UIM_APP_STATE_ILLEGAL ||
               (!(qmi_uim_global_ptr->card_state.card[card_index]->upin_state == MMGSDI_PIN_DISABLED ||
                  qmi_uim_global_ptr->card_state.card[card_index]->upin_state == MMGSDI_PIN_ENABLED_VERIFIED))))
          {
            result.reduced_card_update_required = TRUE;
          }
        }
      }
      break;

    default:
      return result;
  }

  return result;
} /* qmi_uim_process_pin_evts() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_PERSO_EVT()

  DESCRIPTION
    This function processes the PERSO events from MMGSDI

  PARAMETERS
    perso_evt_ptr : pointer to event

  RETURN VALUE
    qmi_uim_card_update_required_type : status of card update required booleans

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_uim_card_update_required_type qmi_uim_process_perso_evt
(
  mmgsdi_session_id_type             session_id,
  const mmgsdi_perso_evt_info_type * perso_evt_ptr
)
{
  uint16                            prov_app_index = UIM_INVALID_APP_INDEX;
  qmi_uim_card_update_required_type result         = {FALSE, FALSE};
  uint8                             session_index  = 0;

  ASSERT(perso_evt_ptr && qmi_uim_global_ptr);

  UIM_MSG_HIGH_2("qmi_uim_process_perso_evt, status 0x%x, feature 0x%x",
                 perso_evt_ptr->status,
                 perso_evt_ptr->feature);

  /* Find the provisioning application index for the session id */
  for (session_index = 0; session_index < QMI_UIM_MAX_PROV_SESSIONS; session_index++)
  {
    if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[session_index])
    {
      prov_app_index = qmi_uim_global_ptr->card_state.index_gw[session_index];
      break;
    }
    else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[session_index])
    {
      prov_app_index = qmi_uim_global_ptr->card_state.index_1x[session_index];
      break;
    }
  }

  /* Check index */
  if (prov_app_index >= QMI_UIM_MAX_APP_COUNT ||
      qmi_uim_global_ptr->card_state.application[prov_app_index] == NULL)
  {
    UIM_MSG_ERR_0("Could not find provisioning application");
    return result;
  }

  /* Update app status: perso event can be received only when PIN is
     already verified. During refresh, it is possible that app state
     is back to detected. */
  if (qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state < QMI_UIM_APP_STATE_PERSO)
  {
    qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state = QMI_UIM_APP_STATE_PERSO;
    result.card_update_required = TRUE;
    result.reduced_card_update_required = TRUE;
  }

  /* Update perso status */
  switch(perso_evt_ptr->status)
  {
    case MMGSDI_PERSO_STATUS_DONE:
      /* Dont send the reduced card status indication if the PERSO state is done */
      result.reduced_card_update_required = FALSE;
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state !=
             QMI_UIM_PERSO_STATE_READY)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state =
          QMI_UIM_PERSO_STATE_READY;
        result.card_update_required = TRUE;
      }
      break;

    case MMGSDI_PERSO_STATUS_WAIT_FOR_EXT_PERSO:
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state !=
        QMI_UIM_PERSO_STATE_READY)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state =
          QMI_UIM_PERSO_STATE_READY;
        result.card_update_required = TRUE;
      }
      /* Perform app state transition once app is locked after app state ready. */
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state == QMI_UIM_APP_STATE_READY)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state = QMI_UIM_APP_STATE_PERSO;
        result.reduced_card_update_required = TRUE;
      }
      break;

    case MMGSDI_PERSO_STATUS_LOCKED:
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state !=
            QMI_UIM_PERSO_STATE_CODE_REQUIRED)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state =
          QMI_UIM_PERSO_STATE_CODE_REQUIRED;
        result.card_update_required = TRUE;
      }
      /* Perform app state transition once app is locked after app state ready/illegal. */
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state == QMI_UIM_APP_STATE_READY ||
          qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state == QMI_UIM_APP_STATE_ILLEGAL)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state = QMI_UIM_APP_STATE_PERSO;
        result.reduced_card_update_required = TRUE;
      }
      break;

    case MMGSDI_PERSO_STATUS_BLOCKED:
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state !=
            QMI_UIM_PERSO_STATE_PERM_BLOCKED)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state =
          QMI_UIM_PERSO_STATE_PERM_BLOCKED;
        result.reduced_card_update_required = TRUE;
        result.card_update_required = TRUE;
      }
      break;

    case MMGSDI_PERSO_STATUS_IN_PROGRESS:
      /* Dont send the reduced card status indication if the PERSO state is in progress */
      result.reduced_card_update_required = FALSE;
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state !=
            QMI_UIM_PERSO_STATE_IN_PROGRESS)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_state =
          QMI_UIM_PERSO_STATE_IN_PROGRESS;
      }

      /* Perform app state transition after device is set in emergency mode. */
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state == QMI_UIM_APP_STATE_READY)
      {
        qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state = QMI_UIM_APP_STATE_PERSO;
        result.reduced_card_update_required = TRUE;
        result.card_update_required = TRUE;
      }
      break;

    default:
      UIM_MSG_ERR_1("Invalid perso state: 0x%x", perso_evt_ptr->status);
      return result;
  }

  /* Update other perso fields */
  if (qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_retries !=
         perso_evt_ptr->num_retries)
  {
    qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_retries =
      perso_evt_ptr->num_retries;
    result.card_update_required = TRUE;
  }

  if (qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_feature !=
          perso_evt_ptr->feature)
  {
    qmi_uim_global_ptr->card_state.application[prov_app_index]->perso_feature =
      perso_evt_ptr->feature;
    result.card_update_required = TRUE;
  }

  return result;
} /* qmi_uim_process_perso_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SIM_BUSY_EVT()

  DESCRIPTION
    This function processes the sim busy event from MMGSDI. It passes the
    sim_busy state (TRUE / FALSE) to all QMI clients.

  PARAMETERS
    slot     : Card slot
    sim_busy : Indicates if SIM is busy or not.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_sim_busy_evt
(
  mmgsdi_slot_id_enum_type        slot,
  boolean                         sim_busy
)
{
  uint8  card_index  = 0;

  UIM_MSG_HIGH_2("qmi_uim_process_sim_busy_evt: slot = 0x%x, sim_busy = 0x%x",
                 slot, sim_busy);

  ASSERT(qmi_uim_global_ptr);

  if (uimqmi_mmgsdi_slot_to_slot_index(slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return;
  }

  /* MMGSDI only sends sim busy events if there has been a change in the sim
     busy state of one of the slots. Regardless, here we re-confirm there has been
     a change. */
  if (sim_busy != qmi_uim_global_ptr->card_state.card[card_index]->sim_busy)
  {
    /* Sim busy state changed so update internal variables and inform
       the QMI clients who, during QMI_UIM_EVT_REG, asked to be notified
       of sim busy events */
    qmi_uim_global_ptr->card_state.card[card_index]->sim_busy = sim_busy;

    qmi_uim_send_sim_busy_indication_to_all();
  }
} /* qmi_uim_process_sim_busy_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_CHECK_CARD_ERROR_ON_ALL_SLOTS

  DESCRIPTION
    Checks whether all the slots have received card error/card removed event

  PARAMETERS
    None

  RETURN VALUE
    boolean        : indicating if all the slots have card state as not present

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_check_card_error_on_all_slots
(
  void
)
{
  uint8         slot_id  = 0;

  ASSERT(qmi_uim_global_ptr);

  for (slot_id = 0;
       (slot_id < qmi_uim_global_ptr->card_state.num_slots) &&
       (slot_id < QMI_UIM_MAX_CARD_COUNT);
       slot_id++)
  {
    if (qmi_uim_global_ptr->card_state.card[slot_id] == NULL)
    {
      continue;
    }

    if (qmi_uim_global_ptr->card_state.card[slot_id]->card_state == QMI_UIM_CARD_STATE_PRESENT)
    {
      return FALSE;
    }
  }

  return TRUE;
} /* qmi_uim_check_card_error_on_all_slots */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_CARD_ERROR_EVT()

  DESCRIPTION
    This function processes the card error event from MMGSDI

  PARAMETERS
    error_info     : error type
    slot           : card slot

  RETURN VALUE
    Boolean indicating if update is required.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_process_card_error_evt
(
  mmgsdi_card_err_info_enum_type  error_info,
  mmgsdi_slot_id_enum_type        slot
)
{
  uint8                                 card_index     = 0;
  qmi_uim_card_error_type               new_card_error = QMI_UIM_CARD_ERROR_UNKNOWN;
  boolean                               result         = FALSE;
  mmgsdi_session_closed_cause_enum_type cause          = MMGSDI_SESSION_CLOSED_CAUSE_UNKNOWN;

  ASSERT(qmi_uim_global_ptr);

  UIM_MSG_HIGH_2("qmi_uim_process_card_error_evt: slot = 0x%x, cause = 0x%x",
                 slot, error_info);

  if (uimqmi_mmgsdi_slot_to_slot_index(slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return FALSE;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return FALSE;
  }

  /* Update error indication for the card */
  switch(error_info)
  {
    case MMGSDI_CARD_ERR_PWR_DN_CMD_NOTIFY:
      new_card_error = QMI_UIM_CARD_ERROR_POWER_DOWN;
      break;

    case MMGSDI_CARD_ERR_POLL_ERROR:
      new_card_error = QMI_UIM_CARD_ERROR_POLL_ERROR;
      break;

    case MMGSDI_CARD_ERR_NO_ATR_RCVD_AT_MAX_VOLT:
    case MMGSDI_CARD_ERR_NO_ATR_RCVD_AFTER_RESET:
    case MMGSDI_CARD_ERR_CORRUPT_ATR_RCVD_MAX_TIMES:
    case MMGSDI_CARD_ERR_NO_PROTOCOL_AT_CARD_PUP:
      /* To distingish between card removal & power up without card */
      if(qmi_uim_global_ptr->card_state.card[card_index]->card_state == QMI_UIM_CARD_STATE_PRESENT)
      {
        new_card_error = QMI_UIM_CARD_ERROR_UNKNOWN_POSSIBLY_REMOVED;
      }
      else
      {
        new_card_error = QMI_UIM_CARD_ERROR_NO_ATR_RECEIVED;
      }
      break;

    case MMGSDI_CARD_ERR_VOLT_MISMATCH:
      new_card_error = QMI_UIM_CARD_ERROR_VOLT_MISMATCH;
      break;

    case MMGSDI_CARD_ERR_MAXED_PARITY_ERROR:
    case MMGSDI_CARD_ERR_MAXED_RX_BREAK_ERR:
    case MMGSDI_CARD_ERR_MAXED_OVERRUN_ERR:
    case MMGSDI_CARD_ERR_MAX_PARITY_IN_PASSIVE:
    case MMGSDI_CARD_ERR_MAX_RXBRK_IN_PASSIVE:
    case MMGSDI_CARD_ERR_MAX_OVERRUN_IN_PASSIVE:
      new_card_error = QMI_UIM_CARD_ERROR_PARITY_ERROR;
      break;

    case MMGSDI_CARD_ERR_TECHNICAL_PROBLEM:
      new_card_error = QMI_UIM_CARD_ERROR_CODE_SIM_TECHNICAL_PROBLEMS;
      break;

    case MMGSDI_CARD_ERR_NULL_BYTES:
      new_card_error = QMI_UIM_CARD_ERROR_NULL_BYTES;
      break;

    case MMGSDI_CARD_ERR_SAP_CONNECTED:
      new_card_error = QMI_UIM_CARD_ERROR_SAP_CONNECTED;
      break;

    case MMGSDI_CARD_ERR_CMD_TIMEOUT:
      new_card_error = QMI_UIM_CARD_ERROR_CMD_TIMEOUT;
      break;

    case MMGSDI_CARD_ERR_UNKNOWN_ERROR:
    default:
      new_card_error = QMI_UIM_CARD_ERROR_UNKNOWN;
      break;
  }

  /* Update card error information */
  if (new_card_error != qmi_uim_global_ptr->card_state.card[card_index]->card_error)
  {
    qmi_uim_global_ptr->card_state.card[card_index]->card_error = new_card_error;
    result = TRUE;
  }

  /* Update card status */
  if (qmi_uim_global_ptr->card_state.card[card_index]->card_state != QMI_UIM_CARD_STATE_ERROR)
  {
    qmi_uim_global_ptr->card_state.card[card_index]->card_state = QMI_UIM_CARD_STATE_ERROR;
    result = TRUE;
  }

  /* Card error implies end of sim busy & detection started phase,
     so clear those flags for this slot */
  qmi_uim_global_ptr->card_state.card[card_index]->sim_busy = FALSE;
  qmi_uim_global_ptr->card_state.card[card_index]->sim_detection_started = FALSE;

  /* Reset indication sent to FALSE when card state is not present in all
     available slots */
  if(qmi_uim_check_card_error_on_all_slots())
  {
    qmi_uim_global_ptr->indication_sent = FALSE;
  }

  /* Reset the UPIN state and retries */
  qmi_uim_global_ptr->card_state.card[card_index]->upin_state = MMGSDI_PIN_STATUS_NOT_INITIALIZED;
  qmi_uim_global_ptr->card_state.card[card_index]->upin_num_retries = 0;
  qmi_uim_global_ptr->card_state.card[card_index]->upuk_num_retries = 0;

  /* Reset the non telcom card flag */
  qmi_uim_global_ptr->card_state.card[card_index]->is_non_telecom_card = FALSE;

  if (result)
  {
    if(error_info == MMGSDI_CARD_ERR_PWR_DN_CMD_NOTIFY)
    {
      cause = MMGSDI_SESSION_CLOSED_CAUSE_CARD_POWER_DOWN;
    }
    else
    {
      cause = MMGSDI_SESSION_CLOSED_CAUSE_CARD_ERROR;
    }

    /* Send the session closed indication to all session on slot */
    qmi_uim_send_session_closed_indication_for_sessions_on_slot(slot, cause);
  }

  /* Wipe out information about applications on that slot */
  qmi_uim_util_remove_all_aid_in_slot(slot);

  /* Cleat the flag as card state is error */
  qmi_uim_global_ptr->is_euicc_card[card_index] = FALSE;

  /* Clear the profiles data as part of card_error */
  qmi_uim_clear_profile_info(slot);

  /* Invalidate EID when card error event is received */
  qmi_uim_global_ptr->eid_data[card_index].eid_valid = FALSE;

  /* Clear non-prov & APDU session info for that slot */
  qmi_uim_remove_non_provisioning_and_apdu_session(slot);

  return result;
} /* qmi_uim_process_card_error_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_CARD_REMOVED_EVT()

  DESCRIPTION
    This function processes the card removed event from MMGSDI

  PARAMETERS
    slot           : card slot

  RETURN VALUE
    Boolean indicating if update is required.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_process_card_removed_evt
(
  mmgsdi_slot_id_enum_type        slot
)
{
  uint8                     card_index     = 0;
  boolean                   result         = FALSE;

  ASSERT(qmi_uim_global_ptr);

  UIM_MSG_HIGH_1("qmi_uim_process_card_removed_evt, slot 0x%x", slot);

  if (uimqmi_mmgsdi_slot_to_slot_index(slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return FALSE;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return FALSE;
  }

  /* Update card status */
  if (qmi_uim_global_ptr->card_state.card[card_index]->card_state != QMI_UIM_CARD_STATE_ABSENT)
  {
    qmi_uim_global_ptr->card_state.card[card_index]->card_state = QMI_UIM_CARD_STATE_ABSENT;
    qmi_uim_global_ptr->card_state.card[card_index]->card_error = QMI_UIM_CARD_ERROR_UNKNOWN;
    result = TRUE;
  }

  /* Card removed implies end of sim busy & detection started phase,
     so clear those flags for this slot */
  qmi_uim_global_ptr->card_state.card[card_index]->sim_busy = FALSE;
  qmi_uim_global_ptr->card_state.card[card_index]->sim_detection_started = FALSE;

  /* Reset indication sent to FALSE when card state is not present in all
     available slots */
  if(qmi_uim_check_card_error_on_all_slots())
  {
    qmi_uim_global_ptr->indication_sent = FALSE;
  }

  /* Reset the UPIN state and retries */
  qmi_uim_global_ptr->card_state.card[card_index]->upin_state = MMGSDI_PIN_STATUS_NOT_INITIALIZED;
  qmi_uim_global_ptr->card_state.card[card_index]->upin_num_retries = 0;
  qmi_uim_global_ptr->card_state.card[card_index]->upuk_num_retries = 0;

  /* Reset the non telcom card flag */
  qmi_uim_global_ptr->card_state.card[card_index]->is_non_telecom_card = FALSE;

  if (result)
  {
    /* Send the session closed indication to all session on slot */
    qmi_uim_send_session_closed_indication_for_sessions_on_slot(slot,
                                                                MMGSDI_SESSION_CLOSED_CAUSE_CARD_REMOVED);
  }

  /* Wipe out information about applications on that slot */
  qmi_uim_util_remove_all_aid_in_slot(slot);

  /* Cleat the flag as card is removed */
  qmi_uim_global_ptr->is_euicc_card[card_index] = FALSE;

  /* Clear the profiles data as part of card removal */
  qmi_uim_clear_profile_info(slot);

  /* Invalidate EID when card removed event is received */
  qmi_uim_global_ptr->eid_data[card_index].eid_valid = FALSE;

  /* Clear non-prov & APDU session info for that slot */
  qmi_uim_remove_non_provisioning_and_apdu_session(slot);

  return result;
} /* qmi_uim_process_card_removed_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_MANAGE_ILLEGAL_CARD_EVT()

  DESCRIPTION
    This function processes the legal and illegal card events from MMGSDI

  PARAMETERS
    session_id             : session id
    legal_status           : status of card illegal or legal

  RETURN VALUE
    Boolean indicating if update is required.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_process_manage_illegal_card_evt
(
  mmgsdi_session_id_type                session_id,
  mmgsdi_session_app_legal_status_type  legal_status
)
{
  uint16 prov_app_index = UIM_INVALID_APP_INDEX;
  uint8  i              = 0;

  ASSERT(qmi_uim_global_ptr);

  UIM_MSG_HIGH_1("qmi_uim_process_manage_illegal_card_evt with legal_status as 0x%x",
                 legal_status);

  /* Find the provisioning application index for the session id */
  for (i = 0; i < QMI_UIM_MAX_PROV_SESSIONS; i++)
  {
    if (session_id == qmi_uim_global_ptr->mmgsdi_gw_session_id[i])
    {
      prov_app_index = qmi_uim_global_ptr->card_state.index_gw[i];
      break;
    }
    else if (session_id == qmi_uim_global_ptr->mmgsdi_1x_session_id[i])
    {
      prov_app_index = qmi_uim_global_ptr->card_state.index_1x[i];
      break;
    }
  }

  /* Check index */
  if (prov_app_index >= QMI_UIM_MAX_APP_COUNT ||
      qmi_uim_global_ptr->card_state.application[prov_app_index] == NULL)
  {
    UIM_MSG_ERR_0("Could not find application index for the session id");
    return FALSE;
  }

  switch(legal_status)
  {
    case MMGSDI_SESSION_APP_IS_ILLEGAL:
      /* Check if the app state is already ILLEGAL: in this case,
         there is nothing to do */
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state == QMI_UIM_APP_STATE_ILLEGAL)
      {
        return FALSE;
      }
      /* Update application status */
      qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state = QMI_UIM_APP_STATE_ILLEGAL;
      return TRUE;

    case MMGSDI_SESSION_APP_IS_LEGAL:
      /* Check if the app state is not illegal: in this case, there is nothing to do */
      if (qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state != QMI_UIM_APP_STATE_ILLEGAL)
      {
        return FALSE;
      }
      /* Update application status */
      qmi_uim_global_ptr->card_state.application[prov_app_index]->app_state = QMI_UIM_APP_STATE_READY;
      return TRUE;

    default:
      break;
  }
  return FALSE;
} /* qmi_uim_process_manage_illegal_card_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SUBSCRIPTION_READY_EVT()

  DESCRIPTION
    This function processes the subscription ready event
    from MMGSDI

  PARAMETERS
    subscription_ready_evt_ptr: pointer to event

  RETURN VALUE
    Boolean indicating if update is required.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_process_subscription_ready_evt
(
  mmgsdi_session_id_type                          session_id,
  const mmgsdi_subscription_ready_evt_info_type * subscription_ready_evt_ptr
)
{
  uint8                                 card_index             = 0;
  uint16                                app_index              = 0;
  boolean                               result                 = FALSE;
  qmi_uim_card_update_required_type     session_changed_result = {FALSE, FALSE};
  mmgsdi_session_changed_evt_info_type  session_changed;

  ASSERT(subscription_ready_evt_ptr && qmi_uim_global_ptr);

  UIM_MSG_HIGH_2("qmi_uim_process_subscription_ready_evt, slot 0x%x, app_type 0x%x",
                 subscription_ready_evt_ptr->app_info.slot,
                 subscription_ready_evt_ptr->app_info.app_data.app_type);

  /* Process session changed event again to avoid client getting
     subscription_ready_evt without session_changed_evt */
  memset(&session_changed, 0, sizeof(mmgsdi_session_changed_evt_info_type));
  session_changed.activated = TRUE;

  (void)memscpy(&session_changed.app_info,
                sizeof(session_changed.app_info),
                &subscription_ready_evt_ptr->app_info,
                sizeof(mmgsdi_app_info_type));

  session_changed_result = qmi_uim_process_session_changed_evt(session_id,
                                                               &session_changed);
  result = session_changed_result.card_update_required;

  if (uimqmi_mmgsdi_slot_to_slot_index(subscription_ready_evt_ptr->app_info.slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return result;
  }

  if(card_index >= QMI_UIM_MAX_CARD_COUNT ||
     card_index >= qmi_uim_global_ptr->card_state.num_slots ||
     qmi_uim_global_ptr->card_state.card[card_index] == NULL)
  {
    return result;
  }

  /* Update card state */
  if (qmi_uim_global_ptr->card_state.card[card_index]->card_state != QMI_UIM_CARD_STATE_PRESENT)
  {
    qmi_uim_global_ptr->card_state.card[card_index]->card_state = QMI_UIM_CARD_STATE_PRESENT;
    qmi_uim_global_ptr->card_state.card[card_index]->card_error = QMI_UIM_CARD_ERROR_UNKNOWN;
    result = TRUE;
  }

  /* Find the AID in the card state structure or add it */
  app_index = qmi_uim_util_find_aid(&subscription_ready_evt_ptr->app_info.app_data,
                                    subscription_ready_evt_ptr->app_info.slot);
  if (app_index == UIM_INVALID_APP_INDEX)
  {
    app_index = qmi_uim_util_add_aid(&subscription_ready_evt_ptr->app_info.app_data,
                                     subscription_ready_evt_ptr->app_info.slot);
  }

  /* Check index */
  if (app_index == UIM_INVALID_APP_INDEX ||
      app_index >= QMI_UIM_MAX_APP_COUNT ||
      qmi_uim_global_ptr->card_state.application[app_index] == NULL)
  {
    UIM_MSG_ERR_0("Impossible to find/add application");
    return result;
  }

  qmi_uim_global_ptr->card_state.application[app_index]->app_state   = QMI_UIM_APP_STATE_READY;
  qmi_uim_global_ptr->card_state.application[app_index]->perso_state = QMI_UIM_PERSO_STATE_READY;

  /* In case QMI UIM misses PIN events (from mmgsdi), possibly due to
     QMI taking long time to initialize and/or open the sessions, PINs state
     is updated here */

  /* PIN1 state */
  if (qmi_uim_global_ptr->card_state.application[app_index]->pin1_state !=
      subscription_ready_evt_ptr->app_info.pin1.status)
  {
    qmi_uim_global_ptr->card_state.application[app_index]->pin1_state =
        subscription_ready_evt_ptr->app_info.pin1.status;

    /* Number of retries */
    if (subscription_ready_evt_ptr->app_info.pin1.valid_num_retries)
    {
      qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries =
        (uint8)subscription_ready_evt_ptr->app_info.pin1.num_retries;
    }
    if (subscription_ready_evt_ptr->app_info.pin1.valid_num_unblock_retries)
    {
      qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries =
        (uint8)subscription_ready_evt_ptr->app_info.pin1.num_unblock_retries;
    }

    /* Universal PIN boolean */
    if (subscription_ready_evt_ptr->app_info.pin1.pin_replacement == MMGSDI_PIN_REPLACED_BY_UNIVERSAL)
    {
      qmi_uim_global_ptr->card_state.application[app_index]->upin = TRUE;
    }
    else
    {
      qmi_uim_global_ptr->card_state.application[app_index]->upin = FALSE;
    }
  }

  /* PIN2 state */
  if (qmi_uim_global_ptr->card_state.application[app_index]->pin2_state !=
      subscription_ready_evt_ptr->app_info.pin2.status)
  {
    qmi_uim_global_ptr->card_state.application[app_index]->pin2_state =
      subscription_ready_evt_ptr->app_info.pin2.status;

    /* Number of retries */
    if (subscription_ready_evt_ptr->app_info.pin2.valid_num_retries)
    {
      qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries =
        (uint8)subscription_ready_evt_ptr->app_info.pin2.num_retries;
    }
    if (subscription_ready_evt_ptr->app_info.pin2.valid_num_unblock_retries)
    {
      qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries =
        (uint8)subscription_ready_evt_ptr->app_info.pin2.num_unblock_retries;
    }
  }

  /* Universal PIN state */
  if ((qmi_uim_global_ptr->card_state.card[card_index]->upin_state !=
       subscription_ready_evt_ptr->app_info.universal_pin.status) &&
      (subscription_ready_evt_ptr->app_info.pin1.pin_replacement ==
       MMGSDI_PIN_REPLACED_BY_UNIVERSAL))

  {
    qmi_uim_global_ptr->card_state.card[card_index]->upin_state =
        subscription_ready_evt_ptr->app_info.universal_pin.status;

    /* Number of retries */
    if (subscription_ready_evt_ptr->app_info.universal_pin.valid_num_retries)
    {
      qmi_uim_global_ptr->card_state.card[card_index]->upin_num_retries =
        (uint8)subscription_ready_evt_ptr->app_info.universal_pin.num_retries;
    }
    if (subscription_ready_evt_ptr->app_info.universal_pin.valid_num_unblock_retries)
    {
      qmi_uim_global_ptr->card_state.card[card_index]->upuk_num_retries =
        (uint8)subscription_ready_evt_ptr->app_info.universal_pin.num_unblock_retries;
    }
  }

  return TRUE;
} /* qmi_uim_process_subscription_ready_evt() */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SESSION_CLOSED_EVT()

  DESCRIPTION
    This function processes the session closed events from MMGSDI

  PARAMETERS
    session_id     : session id
    close_evt_ptr  : pointer to event

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_session_closed_evt
(
  mmgsdi_session_id_type                     session_id,
  const mmgsdi_session_close_evt_info_type * close_evt_ptr
)
{
  ASSERT(close_evt_ptr);

  /* We can receive the session closed event only for non
     provisioning sessions */
  UIM_MSG_HIGH_1("close event cause 0x%x", close_evt_ptr->cause);

  /* Clean up global info belonging to the non prov session and send out
     QMI_UIM_SESSION_CLOSED_IND to the clients linked to the session.
     In case of card error or refresh reset, session info may have been
     already cleaned */
  qmi_uim_remove_non_provisioning_session(session_id, TRUE, close_evt_ptr->cause);
  qmi_uim_remove_apdu_session(session_id, TRUE, close_evt_ptr->cause);
} /* qmi_uim_process_session_closed_evt */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_REFRESH_EVT()

  DESCRIPTION
    This function processes the refresh events from MMGSDI

  PARAMETERS
    session_id     : session id
    refresh_evt_ptr: pointer to event

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_refresh_evt
(
  mmgsdi_session_id_type               session_id,
  const mmgsdi_refresh_evt_info_type * refresh_evt_ptr
)
{
  mmgsdi_slot_id_enum_type   slot            = MMGSDI_SLOT_1;
  uint16                     refresh_index   = 0;
  boolean                    is_apdu_session = FALSE;
  uint8                      card_index      = 0;

  ASSERT(refresh_evt_ptr && qmi_uim_global_ptr);

  UIM_MSG_HIGH_3("Refresh event: stage=0x%x, mode=0x%x, slot=0x%x",
                 refresh_evt_ptr->stage,
                 refresh_evt_ptr->mode,
                 refresh_evt_ptr->slot);

  /* Retrieve the slot id and slot index */
  if (qmi_uim_util_session_id_to_slot(&slot, session_id) != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into slot",
                  session_id);
    return;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(slot,
                                       &card_index,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  /* Retrieve session index for the refresh */
  if (qmi_uim_util_session_id_to_index(&refresh_index, session_id) != QMI_ERR_NONE)
  {
    is_apdu_session = qmi_uim_util_is_apdu_session(session_id);
    if (!is_apdu_session)
    {
      UIM_MSG_ERR_1("Impossible to convert session id (0x%x) into refresh index",
                    session_id);
      return;
    }
  }
  ASSERT(refresh_index < UIM_REFRESH_SESSION_COUNT);

  UIM_MSG_HIGH_2("Refresh event: slot=0x%x, refresh_index=0x%x",
                 slot, refresh_index);

  switch(refresh_evt_ptr->stage)
  {
    case MMGSDI_REFRESH_STAGE_WAIT_FOR_OK_TO_INIT:
    case MMGSDI_REFRESH_STAGE_WAIT_FOR_OK_TO_FCN:
       /* Vote yes for APDU sessions and return without sending indication */
      if (is_apdu_session)
      {
        (void)mmgsdi_session_ok_to_refresh(
                             session_id,
                             TRUE,
                             qmi_uimi_mmgsdi_callback,
                             (mmgsdi_client_data_type)NULL );
        break;
      }

      if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
      {
        qmi_uim_global_ptr->refresh_info_ptr[refresh_index] =
          uimqmi_malloc(sizeof(qmi_uim_refresh_info_type));

        if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
        {
          break;
        }
      }

      /* Check refresh state */
      if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_IDLE &&
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_WAIT_FOR_OK)
      {
        UIM_MSG_ERR_1("Refresh event: cannot process event in current state (0x%x)",
                      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state);
        break;
      }

      /* Compose list of clients for refresh and send indication */
      (void)qmi_uim_send_refresh_ind(session_id, refresh_evt_ptr);

      /* Set ok flag to TRUE... we will set to false only if one
         client votes false */
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->ok_to_init = TRUE;

      /* If no client is interested, immediately vote */
      if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count == 0)
      {
        (void)mmgsdi_session_ok_to_refresh(
                             session_id,
                             TRUE,
                             qmi_uimi_mmgsdi_callback,
                             (mmgsdi_client_data_type)NULL );
      }

      /* Update refresh state */
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state = UIM_REFRESH_STATE_WAIT_FOR_OK;
      break;

    case MMGSDI_REFRESH_STAGE_START:
      /* Send refresh complete for APDU sessions and return without sending
         indication */
      if (is_apdu_session)
      {
        (void)mmgsdi_session_refresh_complete(
                      session_id,
                      TRUE,
                      qmi_uimi_mmgsdi_callback,
                      (mmgsdi_client_data_type)NULL );
        break;
      }

      if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
      {
        qmi_uim_global_ptr->refresh_info_ptr[refresh_index] =
          uimqmi_malloc(sizeof(qmi_uim_refresh_info_type));

        if(qmi_uim_global_ptr->refresh_info_ptr[refresh_index] == NULL)
        {
         break;
        }
      }

      /* Check refresh state */
      if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_IDLE &&
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_WAIT_FOR_OK)
      {
        UIM_MSG_ERR_1("Refresh event: cannot process event in current state (0x%x)",
                      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state);
        break;
      }

      /* Compose list of clients for refresh and send indication */
      (void)qmi_uim_send_refresh_ind(session_id, refresh_evt_ptr);

      /* Set complete flag to TRUE... we will set to false only if one
         client passes false */
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->complete = TRUE;

      /* If no client is interested, immediately send completed */
      if (qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->client_count == 0)
      {
        (void)mmgsdi_session_refresh_complete(
                      session_id,
                      TRUE,
                      qmi_uimi_mmgsdi_callback,
                      (mmgsdi_client_data_type)NULL );
      }

      /* Send the session closed indication */
      if ((refresh_evt_ptr->mode == MMGSDI_REFRESH_RESET) ||
          (refresh_evt_ptr->mode == MMGSDI_REFRESH_RESET_AUTO))
      {
        /* Cleat the flag as refresh RESET is triggered */
        qmi_uim_global_ptr->is_euicc_card[card_index] = FALSE;

        /* Clear all the profile info stored for eUICC profiles */
        qmi_uim_clear_profile_info(slot);

        qmi_uim_send_prov_session_closed_ind_to_all(session_id, slot,
                                                    MMGSDI_SESSION_CLOSED_CAUSE_REFRESH);
      }

      /* Update card status based on refresh event */
      qmi_uim_update_card_status_by_refresh_evt(refresh_evt_ptr);

      /* Update refresh state */
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state = UIM_REFRESH_STATE_WAIT_FOR_COMPLETE;
      break;

    case MMGSDI_REFRESH_STAGE_END_SUCCESS:
    case MMGSDI_REFRESH_STAGE_END_FAILED:
      if (is_apdu_session)
      {
        /* Nothing to do for APDU sessions */
        break;
      }

      if (refresh_evt_ptr->mode != MMGSDI_REFRESH_NAA_APP_RESET       &&
          refresh_evt_ptr->mode != MMGSDI_REFRESH_3G_SESSION_RESET    &&
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index] != NULL &&
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_WAIT_FOR_OK &&
          qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state != UIM_REFRESH_STATE_WAIT_FOR_COMPLETE)
      {
        UIM_MSG_ERR_1("Refresh event: cannot process event in current state (0x%x)",
                      qmi_uim_global_ptr->refresh_info_ptr[refresh_index]->state);
        break;
      }

      /* Compose list of clients for refresh and send indication */
      (void)qmi_uim_send_refresh_ind(session_id, refresh_evt_ptr);

      /* Update refresh state */
      qmi_uim_clean_up_refresh_evt_info(refresh_index);
      uimqmi_free(qmi_uim_global_ptr->refresh_info_ptr[refresh_index]);
      qmi_uim_global_ptr->refresh_info_ptr[refresh_index] = NULL;
      break;

    default:
      /* Ingore other stages... nothing to do! */
      break;
  }
} /* qmi_uim_process_refresh_evt */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_PURGE_TEMPORARY_IDENTITY_EVENT()

  DESCRIPTION
    This function processes the purge temporary identity event

  PARAMETERS
    evt_ptr : pointer to event

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_purge_temporary_identity_event
(
  const mmgsdi_event_data_type *evt_ptr
)
{
  uint16                          purge_guti_mask           = 0;
  uint16                          purge_guti_mask_index     = 0;

  ASSERT(evt_ptr);

  UIM_MSG_HIGH_2("qmi_uim_process_purge_temporary_identity_event, status 0x%x, temp identity 0x%x",
                 evt_ptr->data.purge_temporary_identity.status,
         evt_ptr->data.purge_temporary_identity.temporary_identity);

  if (qmi_uim_util_session_id_to_index(&purge_guti_mask_index, evt_ptr->session_id) != QMI_ERR_NONE)
  {
    return;
  }

  switch(evt_ptr->data.purge_temporary_identity.temporary_identity)
  {
    case MMGSDI_PURGE_TEMPORARY_IDENTITY_GUTI:
      purge_guti_mask |= (1 << purge_guti_mask_index);
      break;
    default:
      return;
  }

  qmi_uim_send_purge_temporary_identity_ind_to_all(
                                              purge_guti_mask,
                                              evt_ptr->data.purge_temporary_identity.status,
                                              evt_ptr->session_id);
} /* qmi_uim_process_purge_temporary_identity_event */


/*===========================================================================
  FUNCTION QMI_UIMI_TERMINAL_CAPABILITY()

  DESCRIPTION
    Provides a capability to override the terminal capability, or to get the
    terminal capability that was last sent to the card or will be sent to the
    card in the next power up

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_terminal_capability
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                   response        = NULL;
  qmi_error_e_type                  errval          = QMI_ERR_NONE;
  qmi_uim_tlv_item_type             tlv_data[2];
  uim_slot_type                     uim_slot_id      = UIM_SLOT_1;
  uint32                            tlv_index        = 0;
  boolean                           retval           = TRUE;
  uint8*                            write_data_ptr   = NULL;
  uint16                            write_data_len   = 0;
  uint16                            i                = 2;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));

  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_TERMINAL_CAPABILITY;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id(tlv_data[0].data.slot.slot,
                                                      &uim_slot_id);
  if(errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if(!tlv_data[1].present)
  {
    /* This is a GET request... */
    uint8*   tlvs_data_ptr    = NULL;
    uint8    tlvs_data_len    = QMI_UIM_TERMINAL_CAPABILITY_MAX_LEN;
    uint8    num_tlvs         = 0;

    /* Allocate the pointer used for retrieving the TC TLVs. Ofcourse, the data
       to be sent out in TC can at max be 255 bytes... hence, malloc being
       done for 255 minus 2 bytes (for TC master tag 0xA9 and the length)*/
    tlvs_data_ptr = uimqmi_malloc(tlvs_data_len);
    if(tlvs_data_ptr == NULL)
    {
      errval = QMI_ERR_NO_MEMORY;
      goto send_result;
    }

    uim_retrieve_terminal_capability(
      uim_slot_id,
      &num_tlvs,
      &tlvs_data_len,
      tlvs_data_ptr);

    if(tlvs_data_len > QMI_UIM_TERMINAL_CAPABILITY_MAX_LEN ||
       !qmi_uim_response_terminal_capability(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                             &response,
                                             num_tlvs,
                                             tlvs_data_len,
                                             tlvs_data_ptr))
    {
      errval = QMI_ERR_INTERNAL;
    }

    uimqmi_free(tlvs_data_ptr);
    tlvs_data_ptr = NULL;
    goto send_result;
  }

  if(tlv_data[1].data.terminal_capability.num_tlvs == 0)
  {
    uint8 write_data_zero[UIM_COMMON_TERMINAL_CAPABILITY_EFS_LEN_BYTES] = {0x02, 0x00};

    /* Request is received for wiping off the terminal capability that was set
       during the previous invocation of this command... Lets go ahead and
       empty the corresponding EFS file (write to the first two bytes
       indicating the length of the file) */
    if(uim_common_efs_write(
         UIM_COMMON_EFS_UIMQMI_QMI_UIM_TERMINAL_CAPABILITY,
         UIM_COMMON_EFS_REGULAR_FILE_TYPE,
         qmi_uim_util_convert_mmgsdi_slot_to_efs_context(tlv_data[0].data.slot.slot),
         (const char*)&write_data_zero,
         sizeof(write_data_zero))
       != UIM_COMMON_EFS_SUCCESS)
    {
      errval = QMI_ERR_INTERNAL;
    }
    goto send_result;
  }

  /* This is a SET request. Make sure the terminal capabilities tags are all
     over-writeable via QMIUIM */
  write_data_len = sizeof(tlv_data[1].data.terminal_capability.num_tlvs) +
                     (tlv_data[1].data.terminal_capability.num_tlvs * sizeof(qmi_uim_terminal_capability_tlv_type));
  write_data_ptr = uimqmi_malloc(write_data_len);
  if(write_data_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  for(tlv_index = 0;
      tlv_index < tlv_data[1].data.terminal_capability.num_tlvs &&
      i < write_data_len;
      tlv_index ++)
  {
    /* Tags 0x80 and 0x81 are not configurable via client... so, return error
       if such tags are found in the command */
    if(tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].tag ==
         QMI_UIM_TERMINAL_POWER_SUPPLY_TAG ||
       tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].tag ==
         QMI_UIM_EXTENDED_LOGICAL_CHANNELS_SUPPORT_TAG)
    {
      errval = QMI_ERR_INVALID_ARG;
      goto send_result;
    }

    /* Additional interface support can be only 1 byte long */
    if(tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].tag ==
         QMI_UIM_ADDITIONAL_INTERFACE_SUPPORTED_TAG &&
       tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value_len !=
         QMI_UIM_ADDITIONAL_INTERFACE_SUPPORTED_LEN &&
       !tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].remove_tlv)
    {
      errval = QMI_ERR_INVALID_ARG;
      goto send_result;
    }

    /* LPA support can be only 1 byte long */
    if(tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].tag ==
         QMI_UIM_LPA_SUPPORTED_TAG &&
       tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value_len !=
         QMI_UIM_LPA_SUPPORTED_LEN &&
       !tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].remove_tlv)
    {
      errval = QMI_ERR_INVALID_ARG;
      goto send_result;
    }

    /* Lets write the TLV to the buffer write_data_ptr that will then be written
       to the EFS file for uimdrv to later read as needed */
    if(write_data_len < tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value_len ||
       i > (write_data_len - tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value_len - 3))
    {
      break;
    }

    write_data_ptr[i++] =
      tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].tag;
    write_data_ptr[i++] =
      (uint8) tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value_len;

    (void)memscpy(&write_data_ptr[i],
                  write_data_len - i,
                  tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value,
                  tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value_len);
    i += tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].value_len;
    write_data_ptr[i++] =
      tlv_data[1].data.terminal_capability.tlv_ptr[tlv_index].remove_tlv;
  }

  /* Write the file size in the first two bytes of the file (in little
     endian format) */
  write_data_ptr[1] = (uint8) (i >> 0x08);
  write_data_ptr[0] = (uint8) (i & 0x00FF);

  UIM_MSG_HIGH_2("Writing 0x%x bytes to slot 0x%x", i, tlv_data[0].data.slot.slot);
  /* Write data to the EFS file */
  if(uim_common_efs_write(
       UIM_COMMON_EFS_UIMQMI_QMI_UIM_TERMINAL_CAPABILITY,
       UIM_COMMON_EFS_REGULAR_FILE_TYPE,
       qmi_uim_util_convert_mmgsdi_slot_to_efs_context(tlv_data[0].data.slot.slot),
       (const char*)write_data_ptr,
       i)
     != UIM_COMMON_EFS_SUCCESS)
  {
    errval = QMI_ERR_INTERNAL;
  }

send_result:
  /* Free the buffer used for data */
  if(write_data_ptr)
  {
    uimqmi_free(write_data_ptr);
    write_data_ptr = NULL;
  }

  qmi_uim_util_free_tlv_content(tlv_data, 2);

  retval = qmi_svc_put_result_tlv(
             &response,
             (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
             errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_terminal_capability */


/*===========================================================================

                             RESPONSE FUNCTIONS

===========================================================================*/


/*===========================================================================
  FUNCTION QMI_UIM_CLIENT_ID_EVT_REG_CONF()

  DESCRIPTION
    Handles the confirmation of client id and event registration command.

  PARAMETERS
    status            : status of MMGSDI operation
    client_id_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    It's executed in the MMGSDI context, directly after the callback
===========================================================================*/
static void qmi_uim_client_id_evt_reg_conf
(
  mmgsdi_return_enum_type                       status,
  const mmgsdi_client_id_and_evt_reg_cnf_type * client_id_cnf_ptr
)
{
  uint8   i                 = 0;
  uint32  session_type_mask = 0;

  ASSERT(client_id_cnf_ptr && qmi_uim_global_ptr);

  if (status != MMGSDI_SUCCESS ||
      client_id_cnf_ptr->response_header.mmgsdi_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_0("Error opening a MMGSDI client id");
    ASSERT(0);
  }

  /* Store the client id */
  qmi_uim_global_ptr->mmgsdi_client_id =
    client_id_cnf_ptr->response_header.client_id;

  /* Allow sending of events for a hidden CSIM from MMGSDI */
  (void)mmgsdi_receive_hidden_csim_events(qmi_uim_global_ptr->mmgsdi_client_id,
                                          TRUE);

  /* Open the first 2 provisioning sessions, so they will be ready in short
     time. Use the userdata field to differentiate. Additional sessions
     will be opened in sequence to avoid flooding MMGSDI queue */

  /* Include masks of all the 6 types */
  session_type_mask = MMGSDI_GW_PROV_PRI_SESSION_MASK |
                      MMGSDI_GW_PROV_SEC_SESSION_MASK |
                      MMGSDI_GW_PROV_TER_SESSION_MASK |
                      MMGSDI_1X_PROV_PRI_SESSION_MASK |
                      MMGSDI_1X_PROV_SEC_SESSION_MASK |
                      MMGSDI_1X_PROV_TER_SESSION_MASK |
                      MMGSDI_CARD_SESSION_SLOT_1_MASK |
                      MMGSDI_CARD_SESSION_SLOT_2_MASK |
                      MMGSDI_CARD_SESSION_SLOT_3_MASK;

  status = mmgsdi_session_open_ext(
               qmi_uim_global_ptr->mmgsdi_client_id,
               session_type_mask,
               qmi_uim_mmgsdi_evt_cback,
               0,
               qmi_uimi_mmgsdi_callback,
               0);

  if (status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_1("Error opening sessions!! (0x%x)", status);

    for (i = 0; i < QMI_UIM_MAX_PROV_SESSIONS; i++)
    {
      qmi_uim_global_ptr->mmgsdi_gw_session_id[i]        = UIM_INVALID_SESSION_ID;
      qmi_uim_global_ptr->mmgsdi_1x_session_id[i]        = UIM_INVALID_SESSION_ID;
    }
    for (i = 0; i < QMI_UIM_MAX_CARD_SESSIONS; i++)
    {
      qmi_uim_global_ptr->mmgsdi_card_slot_session_id[i] = UIM_INVALID_SESSION_ID;
    }
  }
} /* qmi_uim_client_id_evt_reg_conf */


/*===========================================================================
  FUNCTION QMI_UIM_CLIENT_OPEN_SESSION_IMPLICIT_CONF()

  DESCRIPTION
    Handles the confirmation of open session command for the non provisioning
    sessions open by default by QMI UIM when an operation on these sessions
    is required (for example, file access).

  PARAMETERS
    status               : status of MMGSDI operation
    session_type         : session type
    index                : index in the array of non provisioning sessions
    session_id           : MMGSDI session id

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_client_open_session_implicit_conf
(
  mmgsdi_return_enum_type              status,
  mmgsdi_session_type_enum_type        session_type,
  uint8                                index,
  mmgsdi_session_id_type               session_id
)
{
  ASSERT(qmi_uim_global_ptr);

  if (session_type != MMGSDI_NON_PROV_SESSION_SLOT_1 &&
      session_type != MMGSDI_NON_PROV_SESSION_SLOT_2 &&
      session_type != MMGSDI_NON_PROV_SESSION_SLOT_3)
  {
    UIM_MSG_ERR_1("Invalid session type (0x%x) for implicit type",
                  session_type);
    return;
  }

  ASSERT(index < UIM_MAX_NON_PROV_SESSIONS);

  if (qmi_uim_global_ptr->non_prov_session[index] == NULL ||
      qmi_uim_global_ptr->non_prov_session[index]->session_type != session_type)
  {
    UIM_MSG_ERR_1("Non-provisioning session not initialized for index 0x%x",
                  index);
    return;
  }

  if (status == MMGSDI_SUCCESS)
  {
    /* Store the session id */
    qmi_uim_global_ptr->non_prov_session[index]->mmgsdi_session_id = session_id;

    /* Request the PIN status only if this application is one of the
       applications in the global applications list */
    if (qmi_uim_global_ptr->non_prov_session[index]->aid.data_len > 0)
    {
      mmgsdi_aid_type          aid_value = { MMGSDI_APP_NONE };
      mmgsdi_slot_id_enum_type slot      = MMGSDI_SLOT_1;
      uint16                   app_index = UIM_INVALID_APP_INDEX;
      qmi_error_e_type         errval    = QMI_ERR_NONE;

      aid_value.app_type = MMGSDI_APP_UNKNOWN;

      if (qmi_uim_is_usim_aid(&qmi_uim_global_ptr->non_prov_session[index]->aid, TRUE))
      {
        aid_value.app_type = MMGSDI_APP_USIM;
      }
      else if (qmi_uim_is_csim_aid(&qmi_uim_global_ptr->non_prov_session[index]->aid, TRUE))
      {
        aid_value.app_type = MMGSDI_APP_CSIM;
      }

      (void)memscpy(&aid_value.aid,
                    sizeof(mmgsdi_static_data_type),
                    &qmi_uim_global_ptr->non_prov_session[index]->aid,
                    sizeof(mmgsdi_static_data_type));

      /* Retrieve the slot */
      errval = qmi_uim_util_session_id_to_slot(&slot, session_id);
      if (errval == QMI_ERR_NONE)
      {
        app_index = qmi_uim_util_find_aid(&aid_value, slot);
        if (app_index != UIM_INVALID_APP_INDEX)
        {
          /* Get pin status from MMGSDI */
          status = mmgsdi_session_get_all_pin_status(
                                     session_id,
                                     qmi_uimi_mmgsdi_callback,
                                     (mmgsdi_client_data_type)NULL );
          if (status != MMGSDI_SUCCESS)
          {
            UIM_MSG_ERR_1("Error: mmgsdi_session_get_all_pin_status,0x%x",
                          status);
          }
        }
      }
    }

    /* Execute all pending requests */
    qmi_uim_execute_all_pending_requests(session_id, TRUE);
  }
  else
  {
    /* Use a fake session id */
    qmi_uim_global_ptr->non_prov_session[index]->mmgsdi_session_id = UIM_FAKE_SESSION_ID;

    /* Remove non provisioning session.
       QMI_UIM_SESSION_CLOSED_IND shall not be sent out since the session
       clean-up was not trigerred by an MMGSDI_SESSION_CLOSE_EVT */
    qmi_uim_remove_non_provisioning_session(UIM_FAKE_SESSION_ID, FALSE,
                                            MMGSDI_SESSION_CLOSED_CAUSE_UNKNOWN);
  }
} /* qmi_uim_client_open_session_implicit_conf */


/*===========================================================================
  FUNCTION QMI_UIM_CLIENT_OPEN_SESSION_APDU_CONF()

  DESCRIPTION
    Handles the confirmation of open session command related to
    logical channel operations

  PARAMETERS
    status               : status of MMGSDI operation
    index                : index of the APDU channel array
    open_session_cnf_ptr : result of MMGSDI operation
    clid                 : Client ID
    client_reg_count     : Total number of registered clients
    cmd_buf_p            : Pointer to the original command  buffer

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_client_open_session_apdu_conf
(
  mmgsdi_return_enum_type                           status,
  uint8                                             index,
  const mmgsdi_session_open_with_sel_rsp_cnf_type * open_session_cnf_ptr,
  uint8                                             clid,
  uint32                                            client_reg_count,
  qmi_cmd_buf_type                                * cmd_buf_p
)
{
  dsm_item_type *          response        = NULL;
  qmi_error_e_type         errval          = QMI_ERR_NONE;

  ASSERT(open_session_cnf_ptr && qmi_uim_global_ptr && index < UIM_MAX_APDU_CHANNEL_COUNT);

  if (!qmi_uim_global_ptr->apdu_channel[index] ||
      qmi_uim_global_ptr->apdu_channel[index]->mmgsdi_session_id !=
        UIM_INVALID_SESSION_ID)
  {
    UIM_MSG_ERR_1("Non-provisioning session not initialized for index 0x%x",
                  index);
    return;
  }

  if ((cmd_buf_p == NULL) ||
      (FALSE == qmi_uim_util_check_clid(clid, client_reg_count)) ||
      (cmd_buf_p->x_p == NULL || cmd_buf_p->x_p->cl_sp == NULL))
  {
    UIM_MSG_HIGH_0("Error in response cmd_buf ptr or clid");
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[index]);
    qmi_uim_global_ptr->apdu_channel[index] = NULL;
    return;
  }

  if (status == MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_0("Session open to app for APDU transaction");

    qmi_uim_global_ptr->apdu_channel[index]->mmgsdi_session_id =
      open_session_cnf_ptr->session_id;
    qmi_uim_global_ptr->apdu_channel[index]->logical_channel   =
      (uint8)open_session_cnf_ptr->channel_id;

    /* It is possible that the original open channel request was sent
       with a partial AID to MMGSDI. If the open channel request was
       sent expecting a FCP response in the session open response,
       and a non-zero AID is present in the response, update the
       AID for the index */
    if (open_session_cnf_ptr->select_rsp_type == MMGSDI_SELECT_RSP_FCP &&
        open_session_cnf_ptr->app_info.aid.data_len > 0 &&
        open_session_cnf_ptr->app_info.aid.data_len < MMGSDI_MAX_AID_LEN)
    {
      (void)memscpy(&qmi_uim_global_ptr->apdu_channel[index]->aid,
                    sizeof(qmi_uim_global_ptr->apdu_channel[index]->aid),
                    &open_session_cnf_ptr->app_info.aid,
                    sizeof(open_session_cnf_ptr->app_info.aid));
    }

    if (FALSE == qmi_uim_response_logical_channel(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &response,
                   (uint8)open_session_cnf_ptr->channel_id))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
      goto send_result;
    }

    if (FALSE == qmi_uim_response_card_result(
                   UIMI_TLV_RESP_TAG_OPTIONAL_2,
                   &response,
                   open_session_cnf_ptr->response_header.status_word))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
      goto send_result;
    }

    if(open_session_cnf_ptr->select_rsp.data_len <= QMI_UIM_SELECT_RESPONSE_MAX_V01)
    {
      if (FALSE == qmi_uim_response_generic_payload_8bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_3,
                     &response,
                     open_session_cnf_ptr->select_rsp,
                     FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      if (FALSE == qmi_uim_response_generic_payload_16bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_4,
                     &response,
                     open_session_cnf_ptr->select_rsp,
                     FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
  }
  else
  {
    /* Free the item in the table */
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[index]);
    qmi_uim_global_ptr->apdu_channel[index] = NULL;

    UIM_MSG_ERR_3("Session open failure, status=0x%x, sw1=0x%x, sw2=0x%x",
                  status,
                  open_session_cnf_ptr->response_header.status_word.sw1,
                  open_session_cnf_ptr->response_header.status_word.sw2);

    if (status == MMGSDI_SELECT_AID_FAILED)
    {
      UIM_MSG_LOW_0("Session open failed due to SELECT AID failure");

      errval = QMI_ERR_SIM_FILE_NOT_FOUND;

      if (FALSE == qmi_uim_response_card_result(
                     UIMI_TLV_RESP_TAG_OPTIONAL_2,
                     &response,
                     open_session_cnf_ptr->response_header.status_word))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else if (status == MMGSDI_MANAGE_CHANNEL_FAILED)
    {
      UIM_MSG_LOW_0("Session open failed due to MANAGE CHANNEL failure");

      errval = QMI_ERR_INSUFFICIENT_RESOURCES;

      if (FALSE == qmi_uim_response_card_result(
                     UIMI_TLV_RESP_TAG_OPTIONAL_2,
                     &response,
                     open_session_cnf_ptr->response_header.status_word))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else if (status == MMGSDI_APP_NOT_SHAREABLE)
    {
      UIM_MSG_LOW_0("Session open failed because app is not shareable");

      errval = QMI_ERR_INCOMPATIBLE_STATE;

      if (FALSE == qmi_uim_response_card_result(
                     UIMI_TLV_RESP_TAG_OPTIONAL_2,
                     &response,
                     open_session_cnf_ptr->response_header.status_word))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      errval = QMI_ERR_INTERNAL;
    }
  }

send_result:
  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_client_open_session_apdu_conf */


/*===========================================================================
  FUNCTION QMI_UIM_CLIENT_OPEN_SESSION_WITH_SELECT_RSP_CONF()

  DESCRIPTION
    Handles the confirmation of open session command related to
    logical channel operations

  PARAMETERS
    status               : status of MMGSDI operation
    open_session_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_client_open_session_with_select_rsp_conf
(
  mmgsdi_return_enum_type                           status,
  const mmgsdi_session_open_with_sel_rsp_cnf_type * open_session_cnf_ptr
)
{
  uint8                          index             = 0;
  uint8                          session_open_type = 0;
  uint8                          clid              = QMI_SVC_CLID_UNUSED;
  uint32                         client_reg_count  = 0;
  qmi_cmd_buf_type *             cmd_buf_p         = NULL;
  qmi_uimi_userdata_type       * cb_userdata_ptr   = NULL;
  mmgsdi_session_type_enum_type  session_type      = MMGSDI_MAX_SESSION_TYPE_ENUM;

  ASSERT(open_session_cnf_ptr);

  /* Retrieve userdata */
  cb_userdata_ptr =
    (qmi_uimi_userdata_type*)open_session_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid              = cb_userdata_ptr->clid;
  client_reg_count  = cb_userdata_ptr->client_reg_count;
  cmd_buf_p         = cb_userdata_ptr->cmd_buf_ptr;
  index             = cb_userdata_ptr->data.session_open_request.channel_index;
  session_open_type = cb_userdata_ptr->data.session_open_request.session_open_type;
  session_type      = cb_userdata_ptr->data.session_open_request.session_type;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  UIM_MSG_HIGH_3("Open session with select rsp conf: status = 0x%x, open type = 0x%x, index = 0x%x",
                 status, session_open_type, index);

  switch(session_open_type)
  {
    case UIM_SESSION_OPEN_IMPLICIT:
      qmi_uim_client_open_session_implicit_conf(status,
                                                session_type,
                                                index,
                                                open_session_cnf_ptr->session_id);
      break;

    case UIM_SESSION_OPEN_APDU:
      qmi_uim_client_open_session_apdu_conf(status,
                                            index,
                                            open_session_cnf_ptr,
                                            clid,
                                            client_reg_count,
                                            cmd_buf_p);
      break;

    default:
      UIM_MSG_ERR_1("Invalid session open type = 0x%x", session_open_type);
      break;
  }
} /* qmi_uim_client_open_session_with_select_rsp_conf */


/*===========================================================================
  FUNCTION QMI_UIM_CLIENT_OPEN_SESSION_EXT_CONF()

  DESCRIPTION
    Handles the confirmation of open session ext command.

  PARAMETERS
    status                   : status of MMGSDI operation
    open_session_ext_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_client_open_session_ext_conf
(
  mmgsdi_return_enum_type                  status,
  const mmgsdi_session_open_ext_cnf_type * open_session_ext_cnf_ptr
)
{
  uint8 index = 0;

  ASSERT(open_session_ext_cnf_ptr);

  if (status != MMGSDI_SUCCESS ||
      open_session_ext_cnf_ptr->response_header.mmgsdi_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_1("Error opening MMGSDI session ids, status = 0x%x", status);
    return;
  }

  if (open_session_ext_cnf_ptr->session_info_ptr == NULL)
  {
    UIM_MSG_ERR_0("NULL session_info_ptr, not updating");
    return;
  }

  /* Store the session id that were opened */
  for (index = 0; index < open_session_ext_cnf_ptr->num_sessions; index++)
  {
    mmgsdi_session_type_enum_type session_type =
      open_session_ext_cnf_ptr->session_info_ptr[index].session_type;
    mmgsdi_session_id_type session_id =
      open_session_ext_cnf_ptr->session_info_ptr[index].session_id;

    switch (session_type)
    {
      case MMGSDI_GW_PROV_PRI_SESSION:
        qmi_uim_global_ptr->mmgsdi_gw_session_id[0] = session_id;
        break;

      case MMGSDI_1X_PROV_PRI_SESSION:
        qmi_uim_global_ptr->mmgsdi_1x_session_id[0] = session_id;
        break;

      case MMGSDI_GW_PROV_SEC_SESSION:
        qmi_uim_global_ptr->mmgsdi_gw_session_id[1] = session_id;
        break;

      case MMGSDI_1X_PROV_SEC_SESSION:
        qmi_uim_global_ptr->mmgsdi_1x_session_id[1] = session_id;
        break;

      case MMGSDI_GW_PROV_TER_SESSION:
        qmi_uim_global_ptr->mmgsdi_gw_session_id[2] = session_id;
        break;

      case MMGSDI_1X_PROV_TER_SESSION:
        qmi_uim_global_ptr->mmgsdi_1x_session_id[2] = session_id;
        break;

      case MMGSDI_CARD_SESSION_SLOT_1:
        qmi_uim_global_ptr->mmgsdi_card_slot_session_id[0] = session_id;
        break;

      case MMGSDI_CARD_SESSION_SLOT_2:
        qmi_uim_global_ptr->mmgsdi_card_slot_session_id[1] = session_id;
        break;

      case MMGSDI_CARD_SESSION_SLOT_3:
        qmi_uim_global_ptr->mmgsdi_card_slot_session_id[2] = session_id;
        break;

      default:
        break;
    }
  }

  /* Send indication to clients if it was postponed earlier */
  qmi_uim_send_card_status_indication_to_all(TRUE, TRUE, TRUE);
} /* qmi_uim_client_open_session_ext_conf */


/*===========================================================================
  FUNCTION QMI_UIM_CLIENT_OPEN_SESSION_WITH_MF_CONF()

  DESCRIPTION
    Handles the confirmation of open session with MF command.

  PARAMETERS
    status                  : status of MMGSDI operation
    open_session_mf_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_client_open_session_with_mf_conf
(
  mmgsdi_return_enum_type                      status,
  const mmgsdi_session_open_with_mf_cnf_type * open_session_mf_cnf_ptr
)
{
  uint8                         index             = 0;
  qmi_cmd_buf_type *            cmd_buf_p         = NULL;
  dsm_item_type *               response          = NULL;
  qmi_error_e_type              errval            = QMI_ERR_NONE;
  uint8                         clid              = QMI_SVC_CLID_UNUSED;
  uint32                        client_reg_count  = 0;
  qmi_uimi_userdata_type      * cb_userdata_ptr   = NULL;

  ASSERT(open_session_mf_cnf_ptr && qmi_uim_global_ptr);

  if (status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_1("Error opening a MMGSDI session id, type = 0x%x",
                  open_session_mf_cnf_ptr->response_header.client_data);
  }

  /* Retrieve userdata */
  cb_userdata_ptr =
    (qmi_uimi_userdata_type*)open_session_mf_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = cb_userdata_ptr->cmd_buf_ptr;
  index            = cb_userdata_ptr->data.session_open_request.channel_index;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  UIM_MSG_HIGH_1("Open confirmation: index = 0x%x", index);

  ASSERT(index < UIM_MAX_APDU_CHANNEL_COUNT);

  if (!qmi_uim_global_ptr->apdu_channel[index] ||
       qmi_uim_global_ptr->apdu_channel[index]->mmgsdi_session_id != UIM_INVALID_SESSION_ID)
  {
    UIM_MSG_ERR_1("Non-provisioning session not initialized for index 0x%x",
                  index);
    return;
  }

  if (cmd_buf_p == NULL)
  {
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[index]);
    qmi_uim_global_ptr->apdu_channel[index] = NULL;
    return;
  }

  /* Check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[index]);
    qmi_uim_global_ptr->apdu_channel[index] = NULL;
    return;
  }

  /* Validate the SP in command buffer */
  if (cmd_buf_p->x_p == NULL || cmd_buf_p->x_p->cl_sp == NULL)
  {
    UIM_MSG_HIGH_0("Transaction ptr or client state pointer is NULL");
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[index]);
    qmi_uim_global_ptr->apdu_channel[index] = NULL;
    return;
  }

  if (status == MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_0("Session open to MF for APDU transaction");

    qmi_uim_global_ptr->apdu_channel[index]->mmgsdi_session_id = open_session_mf_cnf_ptr->session_id;
    qmi_uim_global_ptr->apdu_channel[index]->logical_channel   = (uint8)open_session_mf_cnf_ptr->channel_id;

    if (FALSE == qmi_uim_response_logical_channel(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &response,
                   (uint8)open_session_mf_cnf_ptr->channel_id))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
      goto send_result;
    }

    if (FALSE == qmi_uim_response_card_result(
                   UIMI_TLV_RESP_TAG_OPTIONAL_2,
                   &response,
                   open_session_mf_cnf_ptr->response_header.status_word))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }
  else
  {
    /* Free the item in the table */
    uimqmi_free(qmi_uim_global_ptr->apdu_channel[index]);
    qmi_uim_global_ptr->apdu_channel[index] = NULL;

    UIM_MSG_ERR_3("Session open failure, status=0x%x, sw1=0x%x, sw2=0x%x",
                  status,
                  open_session_mf_cnf_ptr->response_header.status_word.sw1,
                  open_session_mf_cnf_ptr->response_header.status_word.sw2);

    if (status == MMGSDI_MANAGE_CHANNEL_FAILED)
    {
      UIM_MSG_LOW_0("Session open failed due to MANAGE CHANNEL failure");
      errval = QMI_ERR_INSUFFICIENT_RESOURCES;
      if (FALSE == qmi_uim_response_card_result(
                     UIMI_TLV_RESP_TAG_OPTIONAL_2,
                     &response,
                     open_session_mf_cnf_ptr->response_header.status_word))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      errval = QMI_ERR_INTERNAL;
    }
  }

  send_result:
    /* Update result TLV and Send response */
    uimqmi_put_result_send_response_ind(cmd_buf_p,
                                        response,
                                        errval,
                                        QMUX_SERVICE_UIM,
                                        FALSE,
                                        QMI_SVC_CLID_UNUSED,
                                        (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_client_open_session_with_mf_conf */


/*===========================================================================
  FUNCTION QMI_UIM_CLIENT_CLOSE_SESSION_RESP()

  DESCRIPTION
    Handles the confirmation of close session command.

  PARAMETERS
    status                : status of MMGSDI operation
    close_session_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_client_close_session_resp
(
  mmgsdi_return_enum_type               status,
  const mmgsdi_session_close_cnf_type * close_session_cnf_ptr
)
{
  qmi_cmd_buf_type *         cmd_buf_p        = NULL;
  dsm_item_type *            response         = NULL;
  qmi_error_e_type           errval           = QMI_ERR_NONE;
  uint8                      index            = 0;
  uint8                      clid             = QMI_SVC_CLID_UNUSED;
  uint32                     client_reg_count = 0;
  qmi_uimi_userdata_type   * cb_userdata_ptr  = NULL;

  ASSERT(qmi_uim_global_ptr && close_session_cnf_ptr);

  UIM_MSG_HIGH_1("Session close confirmation. Status = 0x%x", status);

  /* Retrieve userdata */
  cb_userdata_ptr =
    (qmi_uimi_userdata_type*)close_session_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = cb_userdata_ptr->cmd_buf_ptr;
  index            = cb_userdata_ptr->data.session_close_request.channel_index;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Check the close channel indication preference set in NV */
  if(qmi_uim_global_ptr->close_channel_sync)
  {
    /* No need to do any processing on the close CNF because
       response to application was sent synchronously */
    return;
  }

  ASSERT(index < UIM_MAX_APDU_CHANNEL_COUNT);

  if (cmd_buf_p == NULL)
  {
    return;
  }

  /* Check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  if (status != MMGSDI_SUCCESS)
  {
    /* Convert MMGSDI into QMI error code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);

  /* Free the information in the table and send the
     indication */
  if (qmi_uim_global_ptr->apdu_channel[index] != NULL &&
      qmi_uim_global_ptr->apdu_channel[index]->mmgsdi_session_id != UIM_INVALID_SESSION_ID)
  {
    qmi_uim_remove_apdu_session(
              qmi_uim_global_ptr->apdu_channel[index]->mmgsdi_session_id, TRUE,
              MMGSDI_SESSION_CLOSED_CAUSE_CLIENT_REQUEST);
  }
  else
  {
    UIM_MSG_ERR_1("Non-provisioning session not initialized for index 0x%x",
                  index);
  }
} /* qmi_uim_client_close_session_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_READ_ADDITIONAL_RECORDS()

  DESCRIPTION
    Checks if additional records need to be read and, in case, starts
    reading the next record.

  PARAMETERS
    cb_userdata_ptr : userdata (already extracted from confirmation)
    status          : status of MMGSDI operation
    read_cnf_ptr    : result of MMGSDI operation

  RETURN VALUE
    qmi_error_e_type: error code for the result of reading additional
                      records

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uimi_read_additional_records
(
  qmi_uimi_userdata_type     * cb_userdata_ptr,
  mmgsdi_return_enum_type      status,
  const mmgsdi_read_cnf_type * read_cnf_ptr
)
{
  uint16                   index          = 0;
  uint32                   total_read_len = 0;
  mmgsdi_return_enum_type  mmgsdi_status  = MMGSDI_SUCCESS;

  UIM_MSG_HIGH_0("Read additional records");

  ASSERT( read_cnf_ptr && cb_userdata_ptr && cb_userdata_ptr->data.read_record.records_cnf);

  if (cb_userdata_ptr->data.read_record.read_records >=
      cb_userdata_ptr->data.read_record.total_records)
  {
    UIM_MSG_ERR_0("Too many records were read");
    return QMI_ERR_INTERNAL;
  }

  index = cb_userdata_ptr->data.read_record.read_records;

  /* Store the current record */
  cb_userdata_ptr->data.read_record.records_cnf[index].mmgsdi_status = status;
  cb_userdata_ptr->data.read_record.records_cnf[index].status_word =
    read_cnf_ptr->response_header.status_word;
  if (status == MMGSDI_SUCCESS)
  {
    cb_userdata_ptr->data.read_record.records_cnf[index].read_data.data_len =
      read_cnf_ptr->read_data.data_len;
    uimqmi_deep_copy(
      (void**)&cb_userdata_ptr->data.read_record.records_cnf[index].read_data.data_ptr,
      (void*)read_cnf_ptr->read_data.data_ptr,
      (uint32)read_cnf_ptr->read_data.data_len);
  }

  /* Increase number of read records */
  cb_userdata_ptr->data.read_record.read_records++;

  /* We need to abort further record reads if read length exceeds max allocated per IDL */
  total_read_len = (cb_userdata_ptr->data.read_record.total_records - 1) *
                     read_cnf_ptr->read_data.data_len;
  if (total_read_len > QMI_UIM_CONTENT_ADDITIONAL_RECORD_MAX_V01)
  {
    UIM_MSG_ERR_1("Total length of record read request exceeds defined: 0x%x",
                  total_read_len);
    return QMI_ERR_INSUFFICIENT_RESOURCES;
  }

  /* Start reading the next record, if needed */
  if (status == MMGSDI_SUCCESS &&
      cb_userdata_ptr->data.read_record.read_records <
      cb_userdata_ptr->data.read_record.total_records)
  {
    mmgsdi_rec_num_type next_record = cb_userdata_ptr->data.read_record.first_record +
                                      cb_userdata_ptr->data.read_record.read_records;

    UIM_MSG_HIGH_1("Ready to read the next record: 0x%x", next_record);

    /* Start a new read */
    mmgsdi_status = mmgsdi_session_read_record(
                          read_cnf_ptr->response_header.session_id,
                          cb_userdata_ptr->data.read_record.file_access,
                          next_record,
                          read_cnf_ptr->read_data.data_len,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr );

    /* In case of success, return */
    if (mmgsdi_status == MMGSDI_SUCCESS)
    {
      return QMI_ERR_NONE;
    }
    UIM_MSG_ERR_0("Error to read next record");
  }

  return QMI_ERR_INTERNAL;
} /* qmi_uimi_read_additional_records */


/*===========================================================================
  FUNCTION  QMI_UIM_SEND_LONG_READ_IND()

  DESCRIPTION
    Responsible for sending the read transparent long indications.

  PARAMETERS
    clid                : Client ID
    encrypted           : Pointer to output encrypted message
    status_word         : Status word bytes
    read_data           : Read data from MMGSDI

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_long_read_ind
(
  uint8                              clid,
  qmi_uim_requested_encrytion_type   encryption_status,
  mmgsdi_sw_type                     status_word,
  mmgsdi_data_type                   in_read_data
)
{
  uint16                i                   = 0;
  uint16                offset              = 0;
  uint16                num_of_chunks       = 0;
  uint16                total_len           = 0;
  boolean               needs_encryption    = FALSE;
  mmgsdi_data_type      encrypted_read_data = { 0, NULL };

  if ((in_read_data.data_len == 0) || (in_read_data.data_ptr == NULL))
  {
    return;
  }

  /* Encrypt the data if needs to be */
  if (encryption_status == QMI_UIM_DATA_ONLY_ENCRYPTION)
  {
    needs_encryption = qmi_uim_encrypt_payload(in_read_data,
                                               &encrypted_read_data);
  }
  else if (encryption_status == QMI_UIM_COMPLETE_ENCRYPTION)
  {
    UIM_MSG_HIGH_0("Complete read encryption is not supported for long read indications");
    return;
  }

  /* Update total length & calculate number of chunks needed */
  total_len     = needs_encryption ? (uint16)encrypted_read_data.data_len :
                    (uint16)in_read_data.data_len;
  num_of_chunks = (total_len - 1) / QMI_UIM_CONTENT_TRANSPARENT_MAX_V01 + 1;

  /* Send one chunk of read response per indication until entire data is sent */
  for (i = 0; i < num_of_chunks && offset < total_len; i++)
  {
    dsm_item_type         * indication = NULL;
    mmgsdi_data_type        read_data  = { 0, NULL };

    /* Update data pointer & length that is used for the chunk */
    read_data.data_ptr = offset +
      (needs_encryption ? encrypted_read_data.data_ptr : in_read_data.data_ptr);
    read_data.data_len = ((total_len - offset) > QMI_UIM_CONTENT_TRANSPARENT_MAX_V01) ?
                           QMI_UIM_CONTENT_TRANSPARENT_MAX_V01 : (total_len - offset) ;

    UIM_MSG_HIGH_3("Sending read transparent in Long Indication, chunk[0x%x], offset: 0x%x, data_len: 0x%x",
                   i, offset, read_data.data_len);

    /* Construct the TLVs - Long response chunk TLV, card result TLV &
       Encryption TLV */
    if ((FALSE == qmi_uim_response_long_response_chunk(
                    UIMI_TLV_TAG_MANDATORY_1,
                    &indication,
                    qmi_uim_global_ptr->long_read_response_token,
                    total_len,
                    offset,
                    read_data)) ||
        (FALSE == qmi_uim_response_card_result(
                    UIMI_TLV_RESP_TAG_OPTIONAL_1,
                    &indication,
                    status_word)) ||
        (needs_encryption && (FALSE == qmi_uim_response_boolean(
                                         UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                         &indication,
                                         TRUE))))
    {
      /* Clean up allocated buffers & return */
      UIM_MSG_HIGH_0("Unable to construct TLVs");
      dsm_free_packet(&indication);
      break;
    }

    /* Send indication to the client */
    if (FALSE == uimqmi_send_indication(clid,
                                        (uint16)UIMI_CMD_VAL_READ_TRANSPARENT_LONG_IND,
                                        indication,
                                        QMUX_SERVICE_UIM))
    {
      UIM_MSG_HIGH_0("Unable to send indication for read transparent");
      dsm_free_packet(&indication);
    }

    /* Jump to the next chunk */
    offset += read_data.data_len;
  }

  /* Clean up encrypted buffer if allocated */
  if (encrypted_read_data.data_ptr)
  {
    uimqmi_free(encrypted_read_data.data_ptr);
    encrypted_read_data.data_ptr = NULL;
  }
} /* qmi_uim_send_long_read_ind */


/*===========================================================================
  FUNCTION QMI_UIMI_READ_RESP()

  DESCRIPTION
    Composes either the response or indication for the read transparent or
    record function

  PARAMETERS
    status       : status of MMGSDI operation
    read_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_read_resp
(
  mmgsdi_return_enum_type      status,
  const mmgsdi_read_cnf_type * read_cnf_ptr
)
{
  qmi_cmd_buf_type *                cmd_buf_p          = NULL;
  qmi_uimi_userdata_type *          cb_userdata_ptr    = NULL;
  dsm_item_type *                   response           = NULL;
  qmi_error_e_type                  errval             = QMI_ERR_NONE;
  mmgsdi_return_enum_type           first_read_status  = MMGSDI_SUCCESS;
  mmgsdi_sw_type                    first_status_word  = { FALSE, 0, 0 };
  mmgsdi_data_type                  first_read_data    = { 0, NULL };
  boolean                           ind_required       = FALSE;
  boolean                           long_ind_required  = FALSE;
  qmi_uim_requested_encrytion_type  encryption_status  = FALSE;
  uint8                             clid               = QMI_SVC_CLID_UNUSED;
  qmi_error_e_type                  multiple_rec_res   = QMI_ERR_NONE;
  uint32                            client_reg_count   = 0;
  qmi_uimi_cmd_val_e_type           cmd_id             = UIMI_CMD_VAL_WIDTH;

  UIM_MSG_HIGH_0("Sending response for read");

  ASSERT( read_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)read_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid                = cb_userdata_ptr->clid;
  client_reg_count    = cb_userdata_ptr->client_reg_count;
  cmd_id              = cb_userdata_ptr->request_id;
  cmd_buf_p           = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  encryption_status   = cb_userdata_ptr->data.read_transparent.encryption_status;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if the operation included the read of multiple records */
  if (cmd_id == UIMI_CMD_VAL_READ_RECORD &&
      cb_userdata_ptr->data.read_record.total_records > 1)
  {
    multiple_rec_res = qmi_uimi_read_additional_records(cb_userdata_ptr,
                                                        status,
                                                        read_cnf_ptr);
    if (multiple_rec_res == QMI_ERR_NONE)
    {
      /* If a new read was started, we can immediately return */
      return;
    }
    UIM_MSG_ERR_0("No new records to read");
  }

  if (cmd_id == UIMI_CMD_VAL_READ_RECORD &&
      cb_userdata_ptr->data.read_record.total_records > 1)
  {
    ASSERT(cb_userdata_ptr->data.read_record.records_cnf);

    first_read_status  = cb_userdata_ptr->data.read_record.records_cnf[0].mmgsdi_status;
    first_status_word  = cb_userdata_ptr->data.read_record.records_cnf[0].status_word;
    first_read_data    = cb_userdata_ptr->data.read_record.records_cnf[0].read_data;
  }
  else
  {
    first_read_status           = status;
    first_status_word           = read_cnf_ptr->response_header.status_word;
    first_read_data.data_ptr    = read_cnf_ptr->read_data.data_ptr;
    first_read_data.data_len    = read_cnf_ptr->read_data.data_len;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   first_status_word))
  {
    /* Insert read result in the TLV: only in case of success */
    if (first_read_status == MMGSDI_SUCCESS)
    {
      /* If length of the data read as part of multiple records is more than
         4096 bytes, return QMI_ERR_INSUFFICIENT_RESOURCES */
      if(cmd_id == UIMI_CMD_VAL_READ_RECORD &&
         multiple_rec_res == QMI_ERR_INSUFFICIENT_RESOURCES)
      {
        errval = QMI_ERR_INSUFFICIENT_RESOURCES;
        goto send_result;
      }

      /* If length of the data read from the card is more than 4096 bytes,
         return QMI_ERR_INSUFFICIENT_RESOURCES along with total length of the
         file */
      if(cmd_id == UIMI_CMD_VAL_READ_TRANSPARENT &&
         first_read_data.data_len > QMI_UIM_CONTENT_TRANSPARENT_MAX_V01)
      {
        /* Include the File length TLV */
        if(qmi_uim_response_file_length(
             ind_required ? UIMI_TLV_RESP_TAG_OPTIONAL_4 : UIMI_TLV_RESP_TAG_OPTIONAL_5,
             &response,
             (uint16)first_read_data.data_len))
        {
          errval = QMI_ERR_INSUFFICIENT_RESOURCES;

          /* Include the Long data token. Note that if the inclusion of this
             TLV fails, Long Indication message will not be sent and also
             there is no need to free the response pointer */
          qmi_uim_global_ptr->long_read_response_token++;
          long_ind_required = qmi_uim_util_response_ind_token(
            ind_required ? UIMI_TLV_RESP_TAG_OPTIONAL_5 : UIMI_TLV_RESP_TAG_OPTIONAL_6,
            &response,
            qmi_uim_global_ptr->long_read_response_token);
        }
        else
        {
          errval = QMI_ERR_NO_MEMORY;
          dsm_free_packet(&response);
        }
        goto send_result;
      }

      /* Check if the read is transparent and the result should be
         encrypted before it is returned */
      if(cmd_id == UIMI_CMD_VAL_READ_TRANSPARENT &&
         encryption_status == QMI_UIM_DATA_ONLY_ENCRYPTION)
      {
        mmgsdi_data_type             read_data    = { 0, NULL };

        read_data.data_ptr = read_cnf_ptr->read_data.data_ptr;
        read_data.data_len = read_cnf_ptr->read_data.data_len;

        errval = qmi_uim_encrypt_read_rsp(read_data,
                                          &response,
                                          ind_required);
      }
      else if(cmd_id == UIMI_CMD_VAL_READ_TRANSPARENT &&
              encryption_status == QMI_UIM_COMPLETE_ENCRYPTION)
      {
        errval = qmi_uim_complete_encrypt_read_rsp(cmd_id,
                                                   cb_userdata_ptr->data.read_transparent.session_type,
                                                   cb_userdata_ptr->data.read_transparent.file_access,
                                                   read_cnf_ptr->read_data,
                                                   &response);
      }
      else
      {
        if (FALSE == qmi_uim_response_generic_payload_16bit(UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                                            &response,
                                                            first_read_data,
                                                            TRUE))
        {
          errval = QMI_ERR_NO_MEMORY;
          dsm_free_packet(&response);
        }
      }

      /* Append data for additional records (if needed) */
      if (cmd_id == UIMI_CMD_VAL_READ_RECORD &&
          cb_userdata_ptr->data.read_record.total_records > 1 &&
          cb_userdata_ptr->data.read_record.read_records > 1)
      {
        /* This is an optional TLV... we can ignore the error */
        (void)qmi_uim_response_read_additional_records_result(
                 UIMI_TLV_RESP_TAG_OPTIONAL_3,
                 &response,
                 cb_userdata_ptr->data.read_record.read_records,
                 cb_userdata_ptr->data.read_record.records_cnf);
      }

      /* If file is FPLMN or EHPLMN then parse the read data */
      if (read_cnf_ptr->access.file.file_enum == MMGSDI_USIM_FPLMN ||
          read_cnf_ptr->access.file.file_enum == MMGSDI_GSM_FPLMN ||
          read_cnf_ptr->access.file.file_enum == MMGSDI_USIM_EHPLMN)
      {
        (void)qmi_uim_response_parsed_plmn_list(
           ind_required ? UIMI_TLV_RESP_TAG_OPTIONAL_7 : UIMI_TLV_RESP_TAG_OPTIONAL_8,
           &response,
           first_read_data);
      }

      /* If file is IMSI then get number of mnc number digits and parse the read data */
      if (read_cnf_ptr->access.file.file_enum == MMGSDI_USIM_IMSI ||
          read_cnf_ptr->access.file.file_enum == MMGSDI_GSM_IMSI)
      {
        uint8                  num_mnc_digits     = 0;

        if(qmi_uim_util_get_num_of_mnc_digits(read_cnf_ptr->response_header.session_id,
                                              &num_mnc_digits))
        {
          (void)qmi_uim_response_parsed_imsi(
             ind_required ? UIMI_TLV_RESP_TAG_OPTIONAL_8 : UIMI_TLV_RESP_TAG_OPTIONAL_9,
             &response,
             first_read_data,
             num_mnc_digits);
        }
      }
    }
    else
    {
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(first_read_status);
      if (first_read_status == MMGSDI_NOT_SUPPORTED)
      {
        errval = QMI_ERR_NOT_PROVISIONED;
      }
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

send_result:
  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)cmd_id);

  /* Send Long Indication if needed */
  if (long_ind_required)
  {
    qmi_uim_send_long_read_ind(clid,
                               encryption_status,
                               first_status_word,
                               first_read_data);
  }
} /* qmi_uimi_read_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_WRITE_RESP()

  DESCRIPTION
    Composes either the response or indication for the write transparent or
    record function

  PARAMETERS
    status        : status of MMGSDI operation
    write_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_write_resp
(
  mmgsdi_return_enum_type       status,
  const mmgsdi_write_cnf_type * write_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  qmi_uimi_cmd_val_e_type  request_id       = UIMI_CMD_VAL_WIDTH;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for write");

  ASSERT( write_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)write_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  request_id       = cb_userdata_ptr->request_id;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   write_cnf_ptr->response_header.status_word))
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    if (status == MMGSDI_NOT_SUPPORTED)
    {
      errval = QMI_ERR_NOT_PROVISIONED;
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)request_id);
} /* qmi_uimi_write_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_FILE_ATTRIBUTES_RESP()

  DESCRIPTION
    Composes either the response or indication for the get file attributes
    function

  PARAMETERS
    status                : status of MMGSDI operation
    get_file_attr_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_get_file_attributes_resp
(
  mmgsdi_return_enum_type               status,
  const mmgsdi_get_file_attr_cnf_type * get_file_attr_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for get file attributes");

  ASSERT( get_file_attr_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)get_file_attr_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Store the file type in the cache, so it will be available later, if needed */
  if (status == MMGSDI_SUCCESS)
  {
    UIM_MSG_HIGH_1("Caching record type: 0x%x",
                   get_file_attr_cnf_ptr->file_attrib.file_type);
    qmi_uim_cache_record_type(get_file_attr_cnf_ptr->response_header.session_id,
                              cb_userdata_ptr->data.get_file_attributes.file_access,
                              get_file_attr_cnf_ptr->file_attrib.file_type);
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   get_file_attr_cnf_ptr->response_header.status_word))
  {
    /* Insert needed TLVs only in case of success */
    if (status == MMGSDI_SUCCESS)
    {
      /* Insert file attributes TLV */
      if (FALSE == qmi_uim_response_get_file_attributes(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              get_file_attr_cnf_ptr->file_attrib,
                              get_file_attr_cnf_ptr->raw_file_attr_data))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
      /* Insert File Status TLV upon successfully adding previous TLV */
      else
      {
        /* This is an optional TLV... we can ignore the error */
        (void)qmi_uim_response_file_status(
                ind_required ? UIMI_TLV_RESP_TAG_OPTIONAL_3 : UIMI_TLV_RESP_TAG_OPTIONAL_4,
                &response,
                &get_file_attr_cnf_ptr->file_attrib);
      }
    }
    else
    {
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
      if (status == MMGSDI_NOT_SUPPORTED)
      {
        errval = QMI_ERR_NOT_PROVISIONED;
      }
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)cb_userdata_ptr->request_id);

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;
} /* qmi_uimi_get_file_attributes_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_WRITE_RECORD_AFTER_GET_FILE_ATTRIBUTES()

  DESCRIPTION
    Handle the response for get file attributes before a write record.
    It takes care of sending the response in case of error, as if the
    original request was a get_file_attributes, or initiate a write
    record, if the attributes were retrieved successfully.

  PARAMETERS
    status                : status of MMGSDI operation
    get_file_attr_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_write_record_after_get_file_attributes
(
  mmgsdi_return_enum_type               status,
  const mmgsdi_get_file_attr_cnf_type * get_file_attr_cnf_ptr
)
{
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  mmgsdi_return_enum_type  mmgsdi_status    = MMGSDI_SUCCESS;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Handling response of get file attributes for write record");

  ASSERT( get_file_attr_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)get_file_attr_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Check if the file type in the file attributes is transparent */
  if (status == MMGSDI_SUCCESS &&
      get_file_attr_cnf_ptr->file_attrib.file_type == MMGSDI_TRANSPARENT_FILE)
  {
    status = MMGSDI_EF_INCONSISTENT;
  }

  if (status != MMGSDI_SUCCESS)
  {
    /* Error while retrieving file attributes */
    UIM_MSG_ERR_1("Error in file attributes before write record: 0x%x",
                  status);

    /* Handle it as a normal error during get file attributes.
       The userdata will be freed there. */
    qmi_uimi_get_file_attributes_resp(status, get_file_attr_cnf_ptr);
    return;
  }

  /* Store the file type in the cache, so it will be available later, if needed */
  UIM_MSG_HIGH_1("Caching record type: 0x%x",
                 get_file_attr_cnf_ptr->file_attrib.file_type);
  qmi_uim_cache_record_type(get_file_attr_cnf_ptr->response_header.session_id,
                            cb_userdata_ptr->data.write_record.file_access,
                            get_file_attr_cnf_ptr->file_attrib.file_type);

  /* Initiate the write record */
  mmgsdi_status = mmgsdi_session_write_record(
                           get_file_attr_cnf_ptr->response_header.session_id,
                           cb_userdata_ptr->data.write_record.file_access,
                           get_file_attr_cnf_ptr->file_attrib.file_type,
                           cb_userdata_ptr->data.write_record.record,
                           cb_userdata_ptr->data.write_record.data,
                           qmi_uimi_mmgsdi_callback,
                           get_file_attr_cnf_ptr->response_header.client_data );

  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_1("Unable to write record (0x%x): sending internal error",
                  mmgsdi_status);

    /* Retrieve parameters */
    clid             = cb_userdata_ptr->clid;
    client_reg_count = cb_userdata_ptr->client_reg_count;
    cmd_buf_p = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

    /* Free userdata */
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    cb_userdata_ptr = NULL;

    /* First check if the requested client is still valid */
    if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
    {
      return;
    }

    qmi_uim_internal_error_resp(cmd_buf_p);
  }
} /* qmi_uimi_write_record_after_get_file_attributes */


/*===========================================================================
  FUNCTION QMI_UIMI_POWER_UP_RESP()

  DESCRIPTION
    Composes the response for the get power up function

  PARAMETERS
    status           : status of MMGSDI operation
    card_pup_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_power_up_resp
(
  mmgsdi_return_enum_type          status,
  const mmgsdi_card_pup_cnf_type * card_pup_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  uint8                    slot_idx         = 0;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for card power up");

  ASSERT( card_pup_cnf_ptr );

  if (uimqmi_mmgsdi_slot_to_slot_index(card_pup_cnf_ptr->response_header.slot_id,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  /* If rsp is already sent to client then no need to send another rsp */
  if (qmi_uim_global_ptr->power_up_state[slot_idx] == QMI_UIM_PUP_NOT_IN_PROGRESS ||
      qmi_uim_global_ptr->power_up_state[slot_idx] == QMI_UIM_PUP_RSP_SENT_TO_CLIENT)
  {
    /* card pup encountered error, trigger internal card error QCRIL */
    if(status == MMGSDI_CAN_NOT_REFRESH)
    {
      if (qmi_uim_process_card_error_evt(
                         MMGSDI_CARD_ERR_NO_PROTOCOL_AT_CARD_PUP,
                         card_pup_cnf_ptr->response_header.slot_id))
      {
        if (card_pup_cnf_ptr->response_header.slot_id <= MMGSDI_SLOT_2)
        {
          qmi_uim_send_card_status_indication_to_all(TRUE, FALSE, TRUE);
        }
        else
        {
          qmi_uim_send_card_status_indication_to_all(FALSE, FALSE, TRUE);
        }
      }
    }
    UIM_MSG_HIGH_1("No power_up rsp pending... dropping callback, power_up state: 0x%x",
                   qmi_uim_global_ptr->power_up_state[slot_idx]);
    qmi_uim_global_ptr->power_up_state[slot_idx] = QMI_UIM_PUP_NOT_IN_PROGRESS;
    return;
  }

  /* Per design, modem returns 2 callbacks during power up, so
     dropping one of the refresh callback */
  if (status == MMGSDI_REFRESH_SUCCESS)
  {
    UIM_MSG_HIGH_1("Drop the refresh callback for power up operation, power_up state: 0x%x",
                   qmi_uim_global_ptr->power_up_state[slot_idx]);

    /* If state is QMI_UIM_PUP_MMGSDI_RSP_PENDING then don't send the rsp to
       client now. Rsp will be sent on recieving the second rsp from MMGSDI
       after the REFRESH is done else set the state to no pup in progress */
    if (qmi_uim_global_ptr->power_up_state[slot_idx] == QMI_UIM_PUP_MMGSDI_RSP_PENDING)
    {
      qmi_uim_global_ptr->power_up_state[slot_idx] = QMI_UIM_PUP_CLIENT_RSP_PENDING;
    }
    else
    {
      qmi_uim_global_ptr->power_up_state[slot_idx] = QMI_UIM_PUP_NOT_IN_PROGRESS;
    }
    return;
  }

  /* If rsp is not sent to client then send the response, free the cb_userdata
     and update the power_up state to QMI_UIM_PUP_RSP_SENT_TO_CLIENT if this
     is the first response and QMI_UIM_PUP_NOT_IN_PROGRESS if this is the
     second rsp */
  if (qmi_uim_global_ptr->power_up_state[slot_idx] != QMI_UIM_PUP_CLIENT_RSP_PENDING &&
      qmi_uim_global_ptr->power_up_state[slot_idx] != QMI_UIM_PUP_MMGSDI_RSP_PENDING)
  {
    return;
  }

  if (qmi_uim_global_ptr->power_up_state[slot_idx] == QMI_UIM_PUP_CLIENT_RSP_PENDING)
  {
    qmi_uim_global_ptr->power_up_state[slot_idx] = QMI_UIM_PUP_NOT_IN_PROGRESS;
  }
  else
  {
    qmi_uim_global_ptr->power_up_state[slot_idx] = QMI_UIM_PUP_RSP_SENT_TO_CLIENT;
  }

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)card_pup_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_power_up_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_REFRESH_RESP()

  DESCRIPTION
    Composes the response for the refresh function

  PARAMETERS
    status          : status of MMGSDI operation
    refresh_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_refresh_resp
(
  mmgsdi_return_enum_type         status,
  const mmgsdi_refresh_cnf_type * refresh_cnf_ptr
)
{
  /* Right now, no need to do any processing on the refresh CNF because
     response to application is sent synchronously */

  ASSERT( refresh_cnf_ptr );

  UIM_MSG_LOW_2("Refresh confirmation for orig 0x%x. Status = 0x%x",
                 refresh_cnf_ptr->orig_refresh_req, status);

} /* qmi_uimi_refresh_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_PIN_OPERATION_RESP()

  DESCRIPTION
    Composes either response or indication for the PIN operation functions

  PARAMETERS
    status                : status of MMGSDI operation
    pin_operation_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_pin_operation_resp
(
  mmgsdi_return_enum_type               status,
  const mmgsdi_pin_operation_cnf_type * pin_operation_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p              = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr        = NULL;
  dsm_item_type *          response               = NULL;
  qmi_error_e_type         errval                 = QMI_ERR_NONE;
  boolean                  card_ind_required      = FALSE;
  uint8                    pin1_length            = 0;
  uint8                    pin1_value[QMI_UIM_PIN_MAX_V01];
  boolean                  result_ind_required    = FALSE;
  uint8                    clid                   = QMI_SVC_CLID_UNUSED;
  qmi_uimi_cmd_val_e_type  request_id             = UIMI_CMD_VAL_WIDTH;
  uint32                   client_reg_count       = 0;

  UIM_MSG_HIGH_0("Sending response for pin operation");

  ASSERT( pin_operation_cnf_ptr && qmi_uim_global_ptr != NULL);

  memset(pin1_value, 0, sizeof(pin1_value));

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)pin_operation_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  request_id       = cb_userdata_ptr->request_id;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Copy PIN1 value from userdata */
  if (cb_userdata_ptr->data.pin_operation.pin1_length > 0 &&
      cb_userdata_ptr->data.pin_operation.pin1_length <= QMI_UIM_PIN_MAX_V01)
  {
    pin1_length = cb_userdata_ptr->data.pin_operation.pin1_length;
    (void)memscpy(pin1_value, sizeof(pin1_value),
                  cb_userdata_ptr->data.pin_operation.pin1_value, pin1_length);
  }

  UIM_MSG_HIGH_3("PIN operation: 0x%x, with PIN retries 0x%x, unblock retries 0x%x",
                 pin_operation_cnf_ptr->pin_op,
                 pin_operation_cnf_ptr->pin_info.num_retries,
                 pin_operation_cnf_ptr->pin_info.num_unblock_retries);

  /* Check if an indication needs to be sent instead of a response */
  result_ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (result_ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert retries in the TLV only in case of error */
  if (status != MMGSDI_SUCCESS)
  {
    /* Convert MMGSDI into QMI error code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);

    /* In cases where enable/disable operation of PIN1 or PIN2 is not supported,
       appropriate error code needs to be set */
    /* As per GSM spec 11.11, sec 11.3.3 to disable PIN1 on a 2G card,
       the service n must be "allocated and activated". If not, the
       operation is unsupported */
    if (((pin_operation_cnf_ptr->pin_info.pin_id == MMGSDI_PIN2) &&
         (status == MMGSDI_INCORRECT_PARAMS ||
          status == MMGSDI_WARNING_NO_INFO_GIVEN ||
          status == MMGSDI_NOT_SUPPORTED)) ||
        ((pin_operation_cnf_ptr->pin_info.pin_id == MMGSDI_PIN1) &&
         (status == MMGSDI_NOT_SUPPORTED)))
    {
      mmgsdi_app_enum_type          app_type     = MMGSDI_APP_NONE;
      mmgsdi_session_type_enum_type session_type = MMGSDI_MAX_SESSION_TYPE_ENUM;
      mmgsdi_static_data_type       aid          = { 0, };
      qmi_error_e_type              temp_errval  = QMI_ERR_NONE;

      /* Get the session type in order to get the app_type */
      temp_errval = qmi_uim_util_session_id_to_session_type(&session_type,
                                                            &aid,
                                                            pin_operation_cnf_ptr->response_header.session_id);
      if(temp_errval == QMI_ERR_NONE)
      {
        /* Use the session type to get the app_type */
        temp_errval = qmi_uim_util_session_type_to_card_type(&app_type, session_type, aid);
      }

      if (temp_errval == QMI_ERR_NONE)
      {
        /* PIN2 cannot be disabled on ICC cards */
        if (app_type == MMGSDI_APP_SIM || app_type == MMGSDI_APP_RUIM)
        {
          errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
        }
        else if (app_type == MMGSDI_APP_USIM || app_type == MMGSDI_APP_CSIM)
        {
          if (pin_operation_cnf_ptr->response_header.status_word.present &&
              ((pin_operation_cnf_ptr->response_header.status_word.sw1 == 0x6B &&
                pin_operation_cnf_ptr->response_header.status_word.sw2 == 0x00)||
               (pin_operation_cnf_ptr->response_header.status_word.sw1 == 0x62 &&
                pin_operation_cnf_ptr->response_header.status_word.sw2 == 0x00)||
               (pin_operation_cnf_ptr->response_header.status_word.sw1 == 0x6A &&
                pin_operation_cnf_ptr->response_header.status_word.sw2 == 0x81)))
          {
            errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
          }
        }
      }
    }

    /* Take care of case where PIN is permanently blocked */
    if (errval == QMI_ERR_PIN_BLOCKED &&
        pin_operation_cnf_ptr->pin_info.num_retries == 0 &&
        pin_operation_cnf_ptr->pin_info.num_unblock_retries == 0)
    {
      UIM_MSG_HIGH_0("Force error code to permanent blocked");
      errval = QMI_ERR_PIN_PERM_BLOCKED;
    }

    /* Add TLV with number of retries */
    if (FALSE == qmi_uim_response_retries_left(
                    UIMI_TLV_RESP_TAG_OPTIONAL_1,
                    &response,
                    pin_operation_cnf_ptr->pin_info.num_retries,
                    pin_operation_cnf_ptr->pin_info.num_unblock_retries))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }
  else
  {
    /* Encrypt the PIN1 and return it */
    if ((pin1_length > 0) && (qmi_uim_global_ptr->silent_pin1_supported))
    {
      uint8   encrypted_pin1_length    = QMI_UIM_TLV_MAX_ENCRYPTED_PIN1_LEN;
      uint8 * encrypted_pin1_value_ptr = NULL;

      encrypted_pin1_value_ptr = (uint8*)uimqmi_malloc(encrypted_pin1_length);
      if (encrypted_pin1_value_ptr != NULL)
      {
        UIM_MSG_HIGH_0("Encrypting PIN1 value");

        memset(encrypted_pin1_value_ptr, 0, encrypted_pin1_length);

        if (qmi_uim_encrypt_pin(pin1_length,
                                pin1_value,
                                &encrypted_pin1_length,
                                encrypted_pin1_value_ptr))
        {
          /* Add encrypted PIN1 TLV */
          if (FALSE == qmi_uim_response_encrypted_pin1(
                           UIMI_TLV_RESP_TAG_OPTIONAL_2,
                           &response,
                           encrypted_pin1_length,
                           encrypted_pin1_value_ptr))
          {
            UIM_MSG_HIGH_0("Error in constructing encrypted pin1, ignoring TLV");
          }
        }
        uimqmi_free(encrypted_pin1_value_ptr);
      }
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (FALSE == qmi_uim_response_card_result(
                 (result_ind_required) ? UIMI_TLV_RESP_TAG_OPTIONAL_3 : UIMI_TLV_RESP_TAG_OPTIONAL_4,
                 &response,
                 pin_operation_cnf_ptr->response_header.status_word))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Update PIN retries and state in the global variable */
  card_ind_required = qmi_uim_update_pin_retries_and_state(pin_operation_cnf_ptr);

  /* Free userdata, this also takes care of clearing temporarily stored PIN1 */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      result_ind_required,
                                      clid,
                                      (uint16)request_id);

  /* If indication is required, send it to all clients */
  if (card_ind_required)
  {
    if (pin_operation_cnf_ptr->response_header.slot_id <= MMGSDI_SLOT_2)
    {
      qmi_uim_send_card_status_indication_to_all(TRUE, FALSE, TRUE);
    }
    else
    {
      qmi_uim_send_card_status_indication_to_all(FALSE, FALSE, TRUE);
    }
  }
} /* qmi_uimi_pin_operation_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_RUN_CAVE_RESP()

  DESCRIPTION
    Composes either the response or indication for the authenticate function

  PARAMETERS
    status           : status of MMGSDI operation
    run_cave_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_run_cave_resp
(
  mmgsdi_return_enum_type          status,
  const mmgsdi_run_cave_cnf_type * run_cave_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for run cave");

  ASSERT( run_cave_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)run_cave_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   run_cave_cnf_ptr->response_header.status_word))
  {
    /* Insert authenticate body in the TLV: only in case of success */
    if (status == MMGSDI_SUCCESS)
    {
      if (FALSE == qmi_uim_response_authenticate_cave(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              run_cave_cnf_ptr->run_cave_response_data))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)UIMI_CMD_VAL_AUTHENTICATE);
} /* qmi_uimi_run_cave_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_RUN_GSM_ALGO_RESP()

  DESCRIPTION
    Composes either the response or indication for the authenticate function

  PARAMETERS
    status               : status of MMGSDI operation
    run_gsm_algo_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_run_gsm_algo_resp
(
  mmgsdi_return_enum_type                      status,
  const mmgsdi_session_run_gsm_algo_cnf_type * run_gsm_algo_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for run GSM algo");

  ASSERT( run_gsm_algo_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)run_gsm_algo_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   run_gsm_algo_cnf_ptr->response_header.status_word))
  {
    /* Insert authenticate body in the TLV: only in case of success */
    if (status == MMGSDI_SUCCESS)
    {
      if (FALSE == qmi_uim_response_authenticate_gsm_algo(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              run_gsm_algo_cnf_ptr->sres_resp,
                              run_gsm_algo_cnf_ptr->kc_resp))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)UIMI_CMD_VAL_AUTHENTICATE);
} /* qmi_uimi_run_gsm_algo_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_ISIM_AUTH_RESP()

  DESCRIPTION
    Composes either the response or indication for the authenticate function

  PARAMETERS
    status            : status of MMGSDI operation
    isim_auth_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_isim_auth_resp
(
  mmgsdi_return_enum_type           status,
  const mmgsdi_isim_auth_cnf_type * isim_auth_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for ISIM authenticate");

  ASSERT( isim_auth_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)isim_auth_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   isim_auth_cnf_ptr->response_header.status_word))
  {
    /* Insert authenticate body in the TLV: in case of success or
       Authentication Sequence Counter failure */
    if (status == MMGSDI_SUCCESS ||
        status == MMGSDI_AUTS_FAIL)
    {
      if (FALSE == qmi_uim_response_authenticate_isim(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              status,
                              cb_userdata_ptr->data.authenticate.auth_context,
                              isim_auth_cnf_ptr->res,
                              isim_auth_cnf_ptr->ck,
                              isim_auth_cnf_ptr->ik,
                              isim_auth_cnf_ptr->auts))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    if (status != MMGSDI_SUCCESS)
    {
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)UIMI_CMD_VAL_AUTHENTICATE);
} /* qmi_uimi_isim_auth_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_USIM_AUTH_RESP()

  DESCRIPTION
    Composes either the response or indication for the authenticate function

  PARAMETERS
    status            : status of MMGSDI operation
    usim_auth_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_usim_auth_resp
(
  mmgsdi_return_enum_type           status,
  const mmgsdi_usim_auth_cnf_type * usim_auth_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  dsm_item_type *          response         = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for USIM authenticate");

  ASSERT( usim_auth_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)usim_auth_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   usim_auth_cnf_ptr->response_header.status_word))
  {
    /* Insert authenticate body in the TLV */
    if (usim_auth_cnf_ptr->auth_response.data_len > 0 &&
        usim_auth_cnf_ptr->auth_response.data_ptr != NULL)
    {
      if (FALSE == qmi_uim_response_authenticate_generic(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              usim_auth_cnf_ptr->auth_response))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)UIMI_CMD_VAL_AUTHENTICATE);
} /* qmi_uimi_usim_auth_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_COMPUTE_IP_AUTH_RESP()

  DESCRIPTION
    Composes either the response or indication for the authenticate function

  PARAMETERS
    status                  : status of MMGSDI operation
    compute_ip_auth_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_compute_ip_auth_resp
(
  mmgsdi_return_enum_type                 status,
  const mmgsdi_compute_ip_auth_cnf_type * compute_ip_auth_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  dsm_item_type *          response         = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for Compute IP authentication");

  ASSERT( compute_ip_auth_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)compute_ip_auth_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV: this is always present */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   compute_ip_auth_cnf_ptr->response_header.status_word))
  {
    /* Insert authenticate body in the TLV */
    if (compute_ip_auth_cnf_ptr->cmpt_ip_response_data.data_len > 0 &&
        compute_ip_auth_cnf_ptr->cmpt_ip_response_data.data_ptr != NULL)
    {
      if (FALSE == qmi_uim_response_authenticate_generic(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              compute_ip_auth_cnf_ptr->cmpt_ip_response_data))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)UIMI_CMD_VAL_AUTHENTICATE);
} /* qmi_uimi_compute_ip_auth_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_SERVICE_ENABLED_RESP()

  DESCRIPTION
    Composes the response for the get service function

  PARAMETERS
    status                   : status of MMGSDI operation
    app_capabilities_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_get_service_enabled_resp
(
  mmgsdi_return_enum_type                          status,
  const mmgsdi_session_app_capabilities_cnf_type * app_capabilities_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;
  mmgsdi_service_enum_type service_id       = MMGSDI_NONE;

  UIM_MSG_HIGH_0("Sending response for get service");

  ASSERT( app_capabilities_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)app_capabilities_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  service_id = cb_userdata_ptr->data.get_service_status.service_id;

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Insert FDN status in the TLV */
  if (status == MMGSDI_SUCCESS)
  {
    /* We currently support only FDN and ACL. */
    if ((service_id == MMGSDI_GSM_SRV_FDN ||
         service_id == MMGSDI_CDMA_SRV_FDN ||
         service_id == MMGSDI_CSIM_SRV_FDN ||
         service_id == MMGSDI_USIM_SRV_FDN) &&
        FALSE == qmi_uim_response_service_status(
                            UIMI_TLV_RESP_TAG_OPTIONAL_1,
                            &response,
                            TRUE,
                            app_capabilities_cnf_ptr->fdn_enabled))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
    else if ((service_id == MMGSDI_USIM_SRV_ACL) &&
             FALSE == qmi_uim_response_service_status(
                            UIMI_TLV_RESP_TAG_OPTIONAL_5,
                            &response,
                            TRUE,
                            app_capabilities_cnf_ptr->acl_enabled))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }
  else
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_get_service_enabled_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_SERVICE_AVAILABLE_RESP()

  DESCRIPTION
    Composes the response for the get service status function

  PARAMETERS
    status                : status of MMGSDI operation
    srv_available_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_get_service_available_resp
(
  mmgsdi_return_enum_type               status,
  const mmgsdi_srv_available_cnf_type * srv_available_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p         = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr   = NULL;
  dsm_item_type *          response          = NULL;
  mmgsdi_return_enum_type  mmgsdi_status     = MMGSDI_SUCCESS;
  qmi_error_e_type         errval            = QMI_ERR_NONE;
  uint8                    clid              = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count  = 0;
  mmgsdi_service_enum_type service_id        = MMGSDI_NONE;

  UIM_MSG_HIGH_0("Get service available status");

  ASSERT( srv_available_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)srv_available_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* We currently support only FDN, so there is no need to make
     any check on which service is being requested. It needs to
     be improved in future, if additional services are supported */

  if (status == MMGSDI_SUCCESS)
  {
    /* if service is available, get service status */
    if (srv_available_cnf_ptr->srv_available)
    {
      mmgsdi_status = mmgsdi_session_get_app_capabilities(
                      srv_available_cnf_ptr->response_header.session_id,
                      qmi_uimi_mmgsdi_callback,
                      (mmgsdi_client_data_type)cb_userdata_ptr);
      if (mmgsdi_status == MMGSDI_SUCCESS)
      {
        UIM_MSG_HIGH_0("Retrieving FDN status");
        return;
      }
      /* Convert MMGSDI status to QMI err code */
      errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
    }
  }
  else
  {
  /* Convert MMGSDI status to QMI err code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  /* FDN/ACL is not available or there was an error... proceed with response */
  UIM_MSG_HIGH_0("FDN/ACL not available or error");

  service_id = cb_userdata_ptr->data.get_service_status.service_id;

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Insert service status result in the TLV */
  if (errval == QMI_ERR_NONE)
  {
    if (service_id == MMGSDI_GSM_SRV_FDN ||
             service_id == MMGSDI_CDMA_SRV_FDN ||
             service_id == MMGSDI_USIM_SRV_FDN ||
             service_id == MMGSDI_CSIM_SRV_FDN)
    {
      /* The reuqest was to find out if the FDN service is available and enabled.
         We reached here because MMGSDI indicated the service is not available,
         hence, no need to proceed with get_app_capabilities to find out if the
         service is enabled */
      if (FALSE == qmi_uim_response_service_status(
                    UIMI_TLV_RESP_TAG_OPTIONAL_1,
                    &response,
                    FALSE,
                    FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else if (service_id == MMGSDI_USIM_SRV_ACL)
    {
      /* The reuqest was to find out if the ACL service is available and enabled.
         We reached here because MMGSDI indicated the service is not available,
         hence, no need to proceed with get_app_capabilities to find out if the
         service is enabled */
      if (FALSE == qmi_uim_response_service_status(
                      UIMI_TLV_RESP_TAG_OPTIONAL_5,
                      &response,
                      FALSE,
                      FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_get_service_available_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_ALL_PIN_STATUS_RESP()

  DESCRIPTION
    Composes the response for the get all pin status function

  PARAMETERS
    status                     : status of MMGSDI operation
    get_all_pin_status_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_get_all_pin_status_resp
(
  mmgsdi_return_enum_type                    status,
  const mmgsdi_get_all_pin_status_cnf_type * get_all_pin_status_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p       = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr = NULL;
  dsm_item_type *          response        = NULL;
  qmi_error_e_type         errval          = QMI_ERR_NONE;
  boolean                  hidden_key      = FALSE;
  boolean                  index_in_dir    = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Get all pin status");

  ASSERT( get_all_pin_status_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)get_all_pin_status_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Extract details about request */
  hidden_key   = cb_userdata_ptr->data.get_service_status.hidden_key;
  index_in_dir = cb_userdata_ptr->data.get_service_status.index_in_dir;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;


  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Insert hidden key status in the TLV */
  if (status == MMGSDI_SUCCESS)
  {
    if (hidden_key)
    {
      if (FALSE == qmi_uim_response_hidden_key_status(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              get_all_pin_status_cnf_ptr->hiddenkey.status))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    if (index_in_dir)
    {
      if (FALSE == qmi_uim_response_index_in_dir(
                              UIMI_TLV_RESP_TAG_OPTIONAL_3,
                              &response,
                              get_all_pin_status_cnf_ptr->dir_index))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
  }
  else
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_get_all_pin_status_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_NONPROV_GET_ALL_PIN_STATUS()

  DESCRIPTION
    Updates the global pin status for a nonprov session

  PARAMETERS
    status                     : status of MMGSDI operation
    get_all_pin_status_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_nonprov_get_all_pin_status
(
  mmgsdi_return_enum_type                    status,
  const mmgsdi_get_all_pin_status_cnf_type * get_all_pin_status_cnf_ptr
)
{
  qmi_error_e_type               errval        = QMI_ERR_NONE;
  boolean                        notify        = FALSE;
  mmgsdi_session_type_enum_type  session_type  = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmgsdi_aid_type                aid_value     = { MMGSDI_APP_NONE };
  uint16                         app_index     = UIM_INVALID_APP_INDEX;

  UIM_MSG_HIGH_0("Nonprov get all pin status response");

  ASSERT(get_all_pin_status_cnf_ptr && qmi_uim_global_ptr);

  /* Check response status */
  if (status != MMGSDI_SUCCESS)
  {
    UIM_MSG_ERR_1("Error in get all pin status response: 0x%x", status);
    return;
  }

  /* Retrieve AID for a nonprov session */
  errval = qmi_uim_util_session_id_to_session_type(
                       &session_type,
                       &aid_value.aid,
                       get_all_pin_status_cnf_ptr->response_header.session_id);
  if(errval != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_1("Error retrieving aid: 0x%x", errval);
    return;
  }

  if((session_type != MMGSDI_NON_PROV_SESSION_SLOT_1) &&
     (session_type != MMGSDI_NON_PROV_SESSION_SLOT_2) &&
     (session_type != MMGSDI_NON_PROV_SESSION_SLOT_3))
  {
    UIM_MSG_ERR_1("Invalid session type: 0x%x", session_type);
    return;
  }

  if(aid_value.aid.data_len == 0)
  {
    UIM_MSG_ERR_0("No PIN to update for empty AID length");
    return;
  }

  aid_value.app_type = MMGSDI_APP_UNKNOWN;

  /* Find the application */
  app_index = qmi_uim_util_find_aid(&aid_value,
                                    get_all_pin_status_cnf_ptr->response_header.slot_id);
  if (app_index < QMI_UIM_MAX_APP_COUNT &&
      qmi_uim_global_ptr->card_state.application[app_index] != NULL)
  {
    if(!qmi_uim_global_ptr->card_state.application[app_index]->upin)
    {
      /* Update global PIN1 status */
      if(qmi_uim_global_ptr->card_state.application[app_index]->pin1_state !=
            get_all_pin_status_cnf_ptr->pin1.status)
      {
        qmi_uim_global_ptr->card_state.application[app_index]->pin1_state =
            get_all_pin_status_cnf_ptr->pin1.status;
        notify = TRUE;
      }
      if((get_all_pin_status_cnf_ptr->pin1.valid_num_retries) &&
         (qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries !=
            get_all_pin_status_cnf_ptr->pin1.num_retries) )
      {
         qmi_uim_global_ptr->card_state.application[app_index]->pin1_num_retries =
            get_all_pin_status_cnf_ptr->pin1.num_retries;
         notify = TRUE;
      }
      if((get_all_pin_status_cnf_ptr->pin1.valid_num_unblock_retries) &&
         (qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries !=
            get_all_pin_status_cnf_ptr->pin1.num_unblock_retries))
      {
        qmi_uim_global_ptr->card_state.application[app_index]->puk1_num_retries =
            get_all_pin_status_cnf_ptr->pin1.num_unblock_retries;
        notify = TRUE;
      }

      /* Update global PIN2 status */
      if(qmi_uim_global_ptr->card_state.application[app_index]->pin2_state !=
              get_all_pin_status_cnf_ptr->pin2.status)
      {
        qmi_uim_global_ptr->card_state.application[app_index]->pin2_state =
              get_all_pin_status_cnf_ptr->pin2.status;
        notify = TRUE;
      }
      if((get_all_pin_status_cnf_ptr->pin2.valid_num_retries) &&
         (qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries !=
            get_all_pin_status_cnf_ptr->pin2.num_retries))
      {
         qmi_uim_global_ptr->card_state.application[app_index]->pin2_num_retries =
            get_all_pin_status_cnf_ptr->pin2.num_retries;
         notify = TRUE;
      }
      if((get_all_pin_status_cnf_ptr->pin2.valid_num_unblock_retries) &&
         (qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries !=
            get_all_pin_status_cnf_ptr->pin2.num_unblock_retries))
      {
        qmi_uim_global_ptr->card_state.application[app_index]->puk2_num_retries =
            get_all_pin_status_cnf_ptr->pin2.num_unblock_retries;
        notify = TRUE;
      }
    }
  }

  /* If update is required, send it to all clients */
  if(notify)
  {
    if (session_type == MMGSDI_NON_PROV_SESSION_SLOT_1 ||
        session_type == MMGSDI_NON_PROV_SESSION_SLOT_2 ||
        session_type == MMGSDI_NON_PROV_SESSION_SLOT_3)
    {
      qmi_uim_send_card_status_indication_to_all(TRUE, FALSE, TRUE);
    }
    else
    {
      qmi_uim_send_card_status_indication_to_all(FALSE, FALSE, TRUE);
    }
  }
}/* qmi_uimi_nonprov_get_all_pin_status */


/*===========================================================================
  FUNCTION QMI_UIMI_SEND_APDU_RESP()

  DESCRIPTION
    Composes the response for send APDU function

  PARAMETERS
    status            : status of MMGSDI operation
    send_apdu_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_send_apdu_resp
(
  mmgsdi_return_enum_type           status,
  const mmgsdi_send_apdu_ext_cnf_type * send_apdu_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  boolean                  ind_required     = FALSE;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for send apdu");

  ASSERT(send_apdu_cnf_ptr && qmi_uim_global_ptr);

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)send_apdu_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Insert APDU response in the TLV, in case of success */
  if (status == MMGSDI_SUCCESS)
  {
    mmgsdi_sw_type   procedure_bytes;
    memset(&procedure_bytes, 0, sizeof(mmgsdi_sw_type));

    /* Sanity check on the data size */
    if (send_apdu_cnf_ptr->apdu_data.data_len <= 0)
    {
      UIM_MSG_HIGH_1( "Invalid data_len in response: 0x%X",
                      send_apdu_cnf_ptr->apdu_data.data_len);
      errval = QMI_ERR_INTERNAL;
    }
    else if (send_apdu_cnf_ptr->apdu_data.data_len > QMI_UIM_APDU_DATA_MAX_V01)
    {
      /* Check if data size exceeds the max value supported by this response.
         In such cases, send long response TLV with appropriate error code */
      qmi_uim_global_ptr->apdu_response_token++;
      if (FALSE == qmi_uim_response_apdu_long_response(
                            UIMI_TLV_RESP_TAG_OPTIONAL_2,
                            &response,
                            qmi_uim_global_ptr->apdu_response_token,
                            send_apdu_cnf_ptr->apdu_data.data_len))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
      else
      {
        errval       = QMI_ERR_INSUFFICIENT_RESOURCES;
        ind_required = TRUE;
      }
    }
    else
    {
      if (FALSE == qmi_uim_response_send_apdu_result(
                            UIMI_TLV_RESP_TAG_OPTIONAL_1,
                            &response,
                            send_apdu_cnf_ptr->apdu_data,
                            procedure_bytes))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
  }
  else
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);

  /* Send indications in certain cases */
  if (ind_required)
  {
    uint16  i             = 0;
    uint16  offset        = 0;
    uint16  num_of_chunks = 0;
    uint16  total_len     = (uint16)send_apdu_cnf_ptr->apdu_data.data_len;

    num_of_chunks = (total_len - 1) / QMI_UIM_APDU_DATA_MAX_V01 + 1;

    /* Send one chunk of APDU response per indication until entire data is sent */
    for (i = 0; i < num_of_chunks && offset < total_len; i++)
    {
      dsm_item_type               * indication = NULL;
      mmgsdi_send_apdu_data_type    apdu_data;

      memset(&apdu_data, 0, sizeof(mmgsdi_send_apdu_data_type));
      apdu_data.data_ptr = send_apdu_cnf_ptr->apdu_data.data_ptr + offset;
      apdu_data.data_len = ((total_len - offset) > QMI_UIM_APDU_DATA_MAX_V01) ?
                             QMI_UIM_APDU_DATA_MAX_V01 : (total_len - offset) ;

      UIM_MSG_HIGH_3("Sending APDUs in indication, chunk[%d], token: 0x%x, offset: 0x%x",
                     i, qmi_uim_global_ptr->apdu_response_token, offset);

      /* Prepare the TLV */
      if (FALSE == qmi_uim_response_long_response_chunk(
                            UIMI_TLV_TAG_MANDATORY_1,
                            &indication,
                            qmi_uim_global_ptr->apdu_response_token,
                            total_len,
                            offset,
                            apdu_data))
      {
        UIM_MSG_HIGH_0("Unable to construct indication for send apdu");
        dsm_free_packet(&indication);
        return;
      }

      /* Send indication to the client */
      if (FALSE == uimqmi_send_indication(clid,
                                          (uint16)UIMI_CMD_VAL_SEND_APDU,
                                          indication,
                                          QMUX_SERVICE_UIM))
      {
        UIM_MSG_HIGH_0("Unable to send indication for send apdu");
        dsm_free_packet(&indication);
      }

      /* Jump to the next chunk */
      offset += apdu_data.data_len;
    }
  }
} /* qmi_uimi_send_apdu_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_SAP_REQUEST_RESP()

  DESCRIPTION
    Composes the response for various SAP requests

  PARAMETERS
    status              : status of MMGSDI operation
    cnf_type            : confirmation type
    cnf_ptr             : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_sap_request_resp
(
  mmgsdi_return_enum_type                 status,
  mmgsdi_cnf_enum_type                    cnf_type,
  const mmgsdi_cnf_type                 * cnf_ptr
)
{
  qmi_cmd_buf_type *                  cmd_buf_p          = NULL;
  qmi_uimi_userdata_type            * cb_userdata_ptr    = NULL;
  dsm_item_type *                     response           = NULL;
  qmi_error_e_type                    errval             = QMI_ERR_NONE;
  qmi_uimi_client_state_type        * cl_sp              = NULL;
  uint8                               ins_byte           = 0;
  uint8                               clid               = QMI_SVC_CLID_UNUSED;
  uint32                              client_reg_count   = 0;

  UIM_MSG_HIGH_0("Sending response for sap request");

  ASSERT( cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Retrieve INS byte info */
  ins_byte          = cb_userdata_ptr->data.sap_request.ins_value;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Convert result initially */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);

  /* Retreive client state pointer */
  cl_sp = (qmi_uimi_client_state_type *)cmd_buf_p->x_p->cl_sp;

  switch (cnf_type)
  {
    case MMGSDI_SAP_GET_ATR_CNF:
      /* Insert ATR value TLV on successful conf */
      if (status == MMGSDI_SUCCESS)
      {
        if (FALSE == qmi_uim_response_generic_payload_8bit(
                              UIMI_TLV_RESP_TAG_OPTIONAL_1,
                              &response,
                              cnf_ptr->sap_get_atr_cnf.atr_data,
                              FALSE))
        {
          errval = QMI_ERR_NO_MEMORY;
          dsm_free_packet(&response);
        }
      }
      break;
    case MMGSDI_SAP_SEND_APDU_CNF:
      /* Insert APDU result TLV on successful conf */

      UIM_MSG_HIGH_3("implicit get response bytes: sw1:0x%x, sw2: 0x%x, data_len: 0x%x",
                     cnf_ptr->sap_send_apdu_cnf.implicit_get_rsp_sw1,
                     cnf_ptr->sap_send_apdu_cnf.implicit_get_rsp_sw2,
                     cnf_ptr->sap_send_apdu_cnf.apdu_data.data_len);

      if (status == MMGSDI_SUCCESS)
      {
        boolean          send_apdu_res = TRUE;
        mmgsdi_sw_type   procedure_bytes;
        memset(&procedure_bytes, 0, sizeof(mmgsdi_sw_type));

        /* Update pending status only for these cases */
        if (cl_sp->uim_info.sap_intermediate_get_resp)
        {
          if ((cnf_ptr->sap_send_apdu_cnf.implicit_get_rsp_sw1 == QMI_UIM_PROCEDURE_BYTE_SW1_NORMAL) &&
              (cnf_ptr->sap_send_apdu_cnf.implicit_get_rsp_sw2 != QMI_UIM_PROCEDURE_BYTE_SW2_ZERO))
          {
            /* Update procedure bytes when valid */
            procedure_bytes.present = TRUE;
            procedure_bytes.sw1 = (uint8)cnf_ptr->sap_send_apdu_cnf.implicit_get_rsp_sw1;
            procedure_bytes.sw2 = (uint8)cnf_ptr->sap_send_apdu_cnf.implicit_get_rsp_sw2;

            /* For first response, only procedure bytes are sent,
               for subsequent responses, sw1 sw2 bytes are appended to data,
               for last response only data is sent */
            if (ins_byte != QMI_UIM_INS_BYTE_GET_RESPONSE)
            {
              send_apdu_res = FALSE;
              if (FALSE == qmi_uim_response_procedure_bytes(
                                UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                &response,
                                procedure_bytes))
              {
                errval = QMI_ERR_NO_MEMORY;
                dsm_free_packet(&response);
              }
            }
          }
        }

        /* Send APDU response and if needed, also procedure bytes */
        if (send_apdu_res)
        {
          if (FALSE == qmi_uim_response_send_apdu_result(
                              UIMI_TLV_RESP_TAG_OPTIONAL_2,
                              &response,
                              cnf_ptr->sap_send_apdu_cnf.apdu_data,
                              procedure_bytes))
          {
            errval = QMI_ERR_NO_MEMORY;
            dsm_free_packet(&response);
          }
        }
      }
      break;
    case MMGSDI_SAP_CARD_READER_STATUS_CNF:
      /* Insert card reader status TLV on successful conf */
      if (status == MMGSDI_SUCCESS)
      {
        if (FALSE == qmi_uim_response_generic_payload_8bit(
                              UIMI_TLV_RESP_TAG_OPTIONAL_3,
                              &response,
                              cnf_ptr->sap_card_reader_status_cnf.card_reader_status_data,
                              FALSE))
        {
          errval = QMI_ERR_NO_MEMORY;
          dsm_free_packet(&response);
        }
      }
      break;
    case MMGSDI_SAP_POWER_ON_CNF:
    case MMGSDI_SAP_POWER_OFF_CNF:
    case MMGSDI_SAP_RESET_CNF:
      /* Nothing to do */
      break;
    default:
      UIM_MSG_HIGH_0("Invalid cnf_type in response");
      return;
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_sap_request_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GET_ATR_RESP()

  DESCRIPTION
    Composes the response for get ATR request

  PARAMETERS
    status              : status of MMGSDI operation
    cnf_ptr             : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_get_atr_resp
(
  mmgsdi_return_enum_type                 status,
  const mmgsdi_get_atr_cnf_type         * get_atr_cnf_ptr
)
{
  qmi_cmd_buf_type *                  cmd_buf_p        = NULL;
  qmi_uimi_userdata_type            * cb_userdata_ptr  = NULL;
  dsm_item_type *                     response         = NULL;
  qmi_error_e_type                    errval           = QMI_ERR_NONE;
  uint8                               clid             = QMI_SVC_CLID_UNUSED;
  uint32                              client_reg_count = 0;
  uint8                               slot_idx         = 0;

  UIM_MSG_HIGH_0("Sending response for get atr");

  ASSERT( get_atr_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)get_atr_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Insert APDU response in the TLV, in case of success */
  if (status == MMGSDI_SUCCESS)
  {
    if (FALSE == qmi_uim_response_generic_payload_8bit(
                            UIMI_TLV_RESP_TAG_OPTIONAL_1,
                            &response,
                            get_atr_cnf_ptr->atr_data,
                            TRUE))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }

    /* Insert the eUICC support TLV for the slot.
       We can ignore errors during this operation as it's an optional TLV */
    if (uimqmi_mmgsdi_slot_to_slot_index(get_atr_cnf_ptr->response_header.slot_id,
                                         &slot_idx,
                                         qmi_uim_global_ptr->card_state.num_slots) == QMI_ERR_NONE)
    {
      (void)qmi_uim_response_boolean(
              UIMI_TLV_RESP_TAG_OPTIONAL_2,
              &response,
              qmi_uim_global_ptr->is_euicc_card[slot_idx]);
    }
  }
  else
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_get_atr_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_INCREASE_RESP()

  DESCRIPTION
    Composes either the response or indication for the increase function

  PARAMETERS
    status       : status of MMGSDI operation
    increase_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_increase_resp
(
  mmgsdi_return_enum_type          status,
  const mmgsdi_increase_cnf_type * increase_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  qmi_uimi_cmd_val_e_type  request_id       = UIMI_CMD_VAL_WIDTH;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for increase");

  ASSERT( increase_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)increase_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  request_id       = cb_userdata_ptr->request_id;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   increase_cnf_ptr->response_header.status_word))
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    if (status == MMGSDI_NOT_SUPPORTED)
    {
      errval = QMI_ERR_NOT_PROVISIONED;
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Insert increase result in the TLV */
  if (increase_cnf_ptr->increased_data.data_len <= QMI_UIM_INCREASE_DATA_MAX_V01)
  {
    if (FALSE == qmi_uim_response_increase_result(UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                                  &response,
                                                  increase_cnf_ptr->increased_data))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }
  else
  {
    errval = QMI_ERR_INTERNAL;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)request_id);
} /* qmi_uimi_increase_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_RESELECT_RESP

  DESCRIPTION
    Handles the confirmation of reselect request

  PARAMETERS
    status             : status of MMGSDI operation
    select_aid_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_reselect_resp
(
  mmgsdi_return_enum_type                    status,
  const mmgsdi_session_select_aid_cnf_type * select_aid_cnf_ptr
)
{
  uint8                          clid              = QMI_SVC_CLID_UNUSED;
  uint32                         client_reg_count  = 0;
  qmi_cmd_buf_type *             cmd_buf_p         = NULL;
  qmi_uimi_userdata_type       * cb_userdata_ptr   = NULL;
  dsm_item_type *                response          = NULL;
  qmi_error_e_type               errval            = QMI_ERR_NONE;
  uint8                          index             = 0;

  ASSERT(select_aid_cnf_ptr);

  /* Retrieve userdata */
  cb_userdata_ptr =
    (qmi_uimi_userdata_type*)select_aid_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid              = cb_userdata_ptr->clid;
  client_reg_count  = cb_userdata_ptr->client_reg_count;
  index             = cb_userdata_ptr->data.reselect_request.channel_index;
  cmd_buf_p         = cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  UIM_MSG_ERR_3("RESELECT AID status=0x%x, sw1=0x%x, sw2=0x%x",
                status,
                select_aid_cnf_ptr->response_header.status_word.sw1,
                select_aid_cnf_ptr->response_header.status_word.sw2);

  if (cmd_buf_p == NULL)
  {
    return;
  }

  /* Check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  if (FALSE == qmi_uim_response_card_result(
                 UIMI_TLV_RESP_TAG_OPTIONAL_1,
                 &response,
                 select_aid_cnf_ptr->response_header.status_word))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  if (status == MMGSDI_SUCCESS)
  {
    /* It is possible that the original open channel request was sent
       with a partial AID to MMGSDI. So, AID stored in the global for
       this apdu_channel could be a partial AID. Now is the opportunity
       to update the global with the actual AID returned as part of select */
    if (select_aid_cnf_ptr->app_id.aid.data_len > 0 &&
        select_aid_cnf_ptr->app_id.aid.data_len < MMGSDI_MAX_AID_LEN &&
        index < UIM_MAX_APDU_CHANNEL_COUNT)
    {
      (void)memscpy(&qmi_uim_global_ptr->apdu_channel[index]->aid,
                    sizeof(qmi_uim_global_ptr->apdu_channel[index]->aid),
                    &select_aid_cnf_ptr->app_id.aid,
                    sizeof(select_aid_cnf_ptr->app_id.aid));
    }

    if(select_aid_cnf_ptr->select_rsp.data_len <= QMI_UIM_SELECT_RESPONSE_MAX_V01)
    {
      if (FALSE == qmi_uim_response_generic_payload_8bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_2,
                     &response,
                     select_aid_cnf_ptr->select_rsp,
                     FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      if (FALSE == qmi_uim_response_generic_payload_16bit(
                     UIMI_TLV_RESP_TAG_OPTIONAL_3,
                     &response,
                     select_aid_cnf_ptr->select_rsp,
                     FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
  }
  else
  {
    if (status == MMGSDI_SELECT_AID_FAILED)
    {
      errval = QMI_ERR_SIM_FILE_NOT_FOUND;
    }
    else if (status == MMGSDI_APP_NOT_SHAREABLE)
    {
      errval = QMI_ERR_INCOMPATIBLE_STATE;
    }
    else if (status == MMGSDI_NOT_SUPPORTED)
    {
      errval = QMI_ERR_NOT_SUPPORTED;
    }
    else
    {
      errval = QMI_ERR_INTERNAL;
    }
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_reselect_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_SEND_STATUS_RESP

  DESCRIPTION
    Handles the confirmation from MMGSDI of the send status command.

  PARAMETERS
    status         : status of MMGSDI operation
    status_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_send_status_resp
(
  mmgsdi_return_enum_type       status,
  const mmgsdi_status_cnf_type *status_cnf_ptr
)
{
  uint8                          clid              = QMI_SVC_CLID_UNUSED;
  uint32                         client_reg_count  = 0;
  qmi_cmd_buf_type *             cmd_buf_p         = NULL;
  qmi_uimi_userdata_type       * cb_userdata_ptr   = NULL;
  dsm_item_type *                response          = NULL;
  qmi_error_e_type               errval            = QMI_ERR_NONE;

  UIM_MSG_HIGH_0("Sending response for send status");

  ASSERT(status_cnf_ptr);

  /* Retrieve userdata */
  cb_userdata_ptr =
    (qmi_uimi_userdata_type*)status_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid              = cb_userdata_ptr->clid;
  client_reg_count  = cb_userdata_ptr->client_reg_count;
  cmd_buf_p         = cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Insert APDU response in the TLV, in case of success */
  if (status == MMGSDI_SUCCESS)
  {
    if(status_cnf_ptr->status_data.data_len <= QMI_UIM_SELECT_RESPONSE_MAX_V01)
    {
      if (FALSE == qmi_uim_response_generic_payload_8bit(
                              UIMI_TLV_RESP_TAG_OPTIONAL_1,
                              &response,
                              status_cnf_ptr->status_data,
                              FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
    }
    else
    {
      if (FALSE == qmi_uim_response_generic_payload_16bit(
                              UIMI_TLV_RESP_TAG_OPTIONAL_3,
                              &response,
                              status_cnf_ptr->status_data,
                              FALSE))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }  
    }
  }
  else
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
  }

  if (FALSE == qmi_uim_response_card_result(
                 UIMI_TLV_RESP_TAG_OPTIONAL_2,
                 &response,
                 status_cnf_ptr->response_header.status_word))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_send_status_resp */



/*===========================================================================
  FUNCTION QMI_UIMI_SEARCH_RECORD_RESP()

  DESCRIPTION
    Composes the response for search record request

  PARAMETERS
    status                  : status of MMGSDI operation
    search_record_cnf_ptr   : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_search_record_resp
(
  mmgsdi_return_enum_type                 status,
  const mmgsdi_search_cnf_type           *search_record_cnf_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  qmi_error_e_type         errval           = QMI_ERR_NONE;
  boolean                  ind_required     = FALSE;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  qmi_uimi_cmd_val_e_type  request_id       = UIMI_CMD_VAL_WIDTH;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for search record");

  ASSERT( search_record_cnf_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)search_record_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  request_id       = cb_userdata_ptr->request_id;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    return;
  }

  /* Check if an indication needs to be sent instead of a response */
  ind_required = cb_userdata_ptr->ind_token.is_valid;
  if (ind_required)
  {
    if (FALSE == qmi_uim_util_response_ind_token(UIMI_TLV_IND_TAG_MANDATORY_1,
                                                 &response,
                                                 cb_userdata_ptr->ind_token.token))
    {
      UIM_MSG_ERR_0("Could not insert ind_token TLV, not sending indication");
      qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
      dsm_free_packet(&response);
      return;
    }
  }

  /* Insert card result in the TLV */
  if (qmi_uim_response_card_result(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                   &response,
                                   search_record_cnf_ptr->response_header.status_word))
  {
    /* Convert MMGSDI status to QMI err code */
    errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);
    if (status == MMGSDI_NOT_SUPPORTED)
    {
      errval = QMI_ERR_NOT_PROVISIONED;
    }
  }
  else
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }

  /* Insert search result in the TLV */
  if (search_record_cnf_ptr->searched_record_nums.data_len <= QMI_UIM_MAX_NUM_RECORD)
  {
    if (FALSE == qmi_uim_response_generic_payload_8bit(
                   UIMI_TLV_RESP_TAG_OPTIONAL_2,
                   &response,
                   search_record_cnf_ptr->searched_record_nums,
                   FALSE))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }
  else
  {
    errval = QMI_ERR_INTERNAL;
    dsm_free_packet(&response);
  }

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* Insert result item in the TLV Send response or indication */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      ind_required,
                                      clid,
                                      (uint16)request_id);
} /* qmi_uimi_search_record_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_PURGE_TEMPORARY_IDENTITY_RESP()

  DESCRIPTION
    Composes the response for PURGE_TEMPORARY_IDENTITY request

  PARAMETERS
    status                           : status of MMGSDI operation
    purge_temporary_identity_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_purge_temporary_identity_resp
(
  mmgsdi_return_enum_type                              status,
  const mmgsdi_purge_temporary_identity_cnf_type     * purge_temporary_identity_cnf_ptr
)
{
  qmi_cmd_buf_type                        * cmd_buf_p               = NULL;
  qmi_uimi_userdata_type                  * cb_userdata_ptr         = NULL;
  uint8                                     clid                    = QMI_SVC_CLID_UNUSED;
  qmi_uimi_cmd_val_e_type                   request_id              = UIMI_CMD_VAL_WIDTH;
  uint32                                    client_reg_count        = 0;
  qmi_uimi_client_state_type              * cl_sp                   = NULL;
  uint16                                    purge_guti_mask         = 0;
  uint16                                    purge_guti_mask_index   = 0;
  qmi_uim_tlv_temporary_identity_mask_type  temporary_identity_mask;

  UIM_MSG_HIGH_0("Sending response for purge temporary identity");

  ASSERT( purge_temporary_identity_cnf_ptr );

  memset(&temporary_identity_mask, 0x00, sizeof(qmi_uim_tlv_temporary_identity_mask_type));

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)purge_temporary_identity_cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid                    = cb_userdata_ptr->clid;
  client_reg_count        = cb_userdata_ptr->client_reg_count;
  request_id              = cb_userdata_ptr->request_id;
  cmd_buf_p               = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;
  purge_guti_mask         = cb_userdata_ptr->data.purge_temporary_identity.purge_guti_mask;
  QMI_UIM_UTIL_VALIDATE_CMD_BUF_IN_USERDATA(cb_userdata_ptr);

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (status == MMGSDI_SUCCESS)
  {
    /* Update the event reg mask for this client as the purge req succeeded and
        indication will be sent when session is deactivated or GUTI is purged.*/
    cl_sp = qmi_uim_state_ptr->client[clid - 1];
    if (cl_sp != NULL)
    {
      cl_sp->uim_info.purge_guti_mask |= purge_guti_mask;
    }
    return;
  }

  if (qmi_uim_util_session_id_to_index(&purge_guti_mask_index,
                                       purge_temporary_identity_cnf_ptr->response_header.session_id) != QMI_ERR_NONE)
  {
    return;
  }

  if(purge_guti_mask & (1 << purge_guti_mask_index))
  {
    temporary_identity_mask.guti =  TRUE;
  }
  else
  {
    return;
  }

  /* Send indication if status is ERROR */
  qmi_uim_send_purge_temporary_identity_ind(clid,
                                            temporary_identity_mask,
                                            status,
                                            purge_temporary_identity_cnf_ptr->response_header.session_id);
} /* qmi_uimi_purge_temporary_identity_resp */


/*===========================================================================
  FUNCTION QMI_UIMI_GENERIC_NO_PAYLOAD_RESP

  DESCRIPTION
    Generic function to composes response with no payload

  PARAMETERS
    uim_message_ptr : Pointer to message from QMI UIM message

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_generic_no_payload_resp
(
  qmi_uim_message_type * uim_message_ptr
)
{
  uint8                          clid              = QMI_SVC_CLID_UNUSED;
  uint32                         client_reg_count  = 0;
  qmi_cmd_buf_type *             cmd_buf_p         = NULL;
  qmi_uimi_userdata_type       * cb_userdata_ptr   = NULL;
  dsm_item_type *                response          = NULL;
  qmi_error_e_type               errval            = QMI_ERR_NONE;
  mmgsdi_return_enum_type        status            = MMGSDI_ERROR;
  mmgsdi_cnf_type              * cnf_ptr           = NULL;
  mmgsdi_cnf_enum_type           cnf_type          = MMGSDI_MAX_CNF_ENUM;

  ASSERT(uim_message_ptr);

  status   = uim_message_ptr->data.cnf.status;
  cnf_ptr  = &uim_message_ptr->data.cnf.cnf_value;
  cnf_type = uim_message_ptr->data.cnf.cnf_type;

  UIM_MSG_HIGH_1("Sending generic response for cmd : 0x%x", cnf_type);

  /* Retrieve userdata */
  cb_userdata_ptr =
    (qmi_uimi_userdata_type*)cnf_ptr->response_header.client_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve info */
  clid              = cb_userdata_ptr->clid;
  client_reg_count  = cb_userdata_ptr->client_reg_count;
  cmd_buf_p         = cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    UIM_MSG_HIGH_0("Invalid Client, not sending indication or response");
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Convert MMGSDI status to QMI err code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(status);

  switch (cnf_type)
  {
    case MMGSDI_SESSION_ENABLE_SERVICE_CNF:
    case MMGSDI_SESSION_DISABLE_SERVICE_CNF:
      /* Convert to a specific result code*/
      if (status == MMGSDI_ACCESS_DENIED)
      {
        UIM_MSG_HIGH_0("QMI Set Service with error in PIN2 restriction");
        errval = QMI_ERR_PIN2_RESTRICTION;
      }
      break;
    case MMGSDI_SAP_CONNECT_CNF:
    case MMGSDI_SAP_DISCONNECT_CNF:
      if(status == MMGSDI_SIM_BUSY)
      {
        /* Keep impact cofined to SAP, may have different meaning for other commands */
        errval = QMI_ERR_DEVICE_NOT_READY;
      }
      break;
    default:
      break;
  }

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      errval,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uimi_generic_no_payload_resp */


/*===========================================================================

                             CALLBACK FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_UIMI_MMGSDI_CALLBACK()

  DESCRIPTION
    This function is executed as callback of MMGSDI API. This is
    executed in the context of MMGSDI task and needs to make a deep
    copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_mmgsdi_callback
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf_type,
  const mmgsdi_cnf_type  *cnf_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  /* Copy content into a buffer and post a message to QMI task */
  UIM_MSG_HIGH_1("Got response from MMGSDI for cnf_type: 0x%x", cnf_type);

  switch(cnf_type)
  {
    case MMGSDI_CLIENT_ID_AND_EVT_REG_CNF:
    case MMGSDI_SESSION_OPEN_EXT_CNF:
    case MMGSDI_SESSION_CLOSE_CNF:
    case MMGSDI_READ_CNF:
    case MMGSDI_WRITE_CNF:
    case MMGSDI_GET_FILE_ATTR_CNF:
    case MMGSDI_CARD_PDOWN_CNF:
    case MMGSDI_CARD_PUP_CNF:
    case MMGSDI_REFRESH_CNF:
    case MMGSDI_PIN_OPERATION_CNF:
    case MMGSDI_RUN_CAVE_CNF:
    case MMGSDI_SESSION_RUN_GSM_ALGO_CNF:
    case MMGSDI_ISIM_AUTH_CNF:
    case MMGSDI_USIM_AUTH_CNF:
    case MMGSDI_COMPUTE_IP_AUTH_CNF:
    case MMGSDI_SESSION_ENABLE_SERVICE_CNF:
    case MMGSDI_SESSION_DISABLE_SERVICE_CNF:
    case MMGSDI_SRV_AVAILABLE_CNF:
    case MMGSDI_SESSION_GET_APP_CAPABILITIES_CNF:
    case MMGSDI_GET_ALL_PIN_STATUS_CNF:
    case MMGSDI_SESSION_ACTIVATE_OR_SWITCH_PROVISIONING_CNF:
    case MMGSDI_SESSION_DEACTIVATE_CNF:
    case MMGSDI_SEND_APDU_EXT_CNF:
    case MMGSDI_SAP_CONNECT_CNF:
    case MMGSDI_SAP_DISCONNECT_CNF:
    case MMGSDI_SAP_GET_ATR_CNF:
    case MMGSDI_SAP_SEND_APDU_CNF:
    case MMGSDI_SAP_POWER_ON_CNF:
    case MMGSDI_SAP_POWER_OFF_CNF:
    case MMGSDI_SAP_RESET_CNF:
    case MMGSDI_SAP_CARD_READER_STATUS_CNF:
    case MMGSDI_SESSION_SUBSCRIPTION_OK_CNF:
    case MMGSDI_GET_ATR_CNF:
    case MMGSDI_SESSION_OPEN_WITH_SELECT_RSP_CNF:
    case MMGSDI_SESSION_OPEN_WITH_MF_CNF:
    case MMGSDI_REHABILITATE_CNF:
    case MMGSDI_INVALIDATE_CNF:
    case MMGSDI_INCREASE_CNF:
    case MMGSDI_SESSION_SELECT_AID_CNF:
    case MMGSDI_CARD_STATUS_CNF:
    case MMGSDI_GET_SIM_PROFILE_CNF:
    case MMGSDI_SET_SIM_PROFILE_CNF:
    case MMGSDI_SEARCH_CNF:
    case MMGSDI_PURGE_TEMPORARY_IDENTITY_CNF:
    case MMGSDI_SET_APDU_BHEAVIOR_CNF:
    case MMGSDI_GET_M2M_EID_CNF:
    case MMGSDI_MAP_SESSIONS_TO_SLOTS_CNF:
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_CNF);
      if (msg_ptr != NULL)
      {
        msg_ptr->data.cnf.cnf_type  = cnf_type;
        msg_ptr->data.cnf.status    = status;

        /* Make deep copy of confirmation */
        qmi_uim_cnf_deep_copy(
          cnf_type,
          &msg_ptr->data.cnf.cnf_value,
          cnf_ptr);

        /* Send command */
        if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
           (present_in_queue))
        {
          qmi_uim_cnf_free_copy(cnf_type, &msg_ptr->data.cnf.cnf_value);
          uimqmi_free(msg_ptr);
        }
        msg_ptr = NULL;
      }
      break;

    default:
      UIM_MSG_MED_0("Message cnf not handled");
      break;
  }
} /* qmi_uimi_mmgsdi_callback */


#ifdef FEATURE_UIM_SSM
/*===========================================================================
  FUNCTION QMI_UIM_SSM_CBACK

  DESCRIPTION
    This callback is used to notify QMI UIM asynchronosly about the state of
    initialization.  'qmi_uim_ssm_id' is only valid when this callback returns
    'E_SSM_SUCCESS'.  SSM can transition to different modes, and this callback
    will be used to notify Diag if 'qmi_uim_ssm_id' is valid or not.

  PARAMETERS
    ssm_err_t

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_ssm_cback
(
  ssm_err_t err
)
{
  UIM_MSG_MED_1("qmi_uim_ssm_cback with err= 0x%x", err);
  if (qmi_uim_global_ptr != NULL)
  {
    qmi_uim_global_ptr->ssm_info.qmi_uim_ssm_init_err = err;
  }
} /* qmi_uim_ssm_cback */
#endif /* FEATURE_UIM_SSM */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_MMGSDI_CALLBACK_MSG()

  DESCRIPTION
    This function is called as a result of a message posted to QMI UIM
    queue from a callback from MMGSDI.

  PARAMETERS
    uim_message_ptr: Pointer to message from QMI UIM message queue
  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_mmgsdi_callback_msg
(
  qmi_uim_message_type * uim_message_ptr
)
{
  mmgsdi_return_enum_type  status   = MMGSDI_ERROR;
  mmgsdi_cnf_type        * cnf_ptr  = NULL;
  mmgsdi_cnf_enum_type     cnf_type = MMGSDI_MAX_CNF_ENUM;

  qmi_uimi_userdata_type * cb_userdata_ptr = NULL;

  ASSERT(uim_message_ptr != NULL);

  status   = uim_message_ptr->data.cnf.status;
  cnf_ptr  = &uim_message_ptr->data.cnf.cnf_value;
  cnf_type = uim_message_ptr->data.cnf.cnf_type;

  switch(cnf_type)
  {
    case MMGSDI_CLIENT_ID_AND_EVT_REG_CNF:
      qmi_uim_client_id_evt_reg_conf(status, &cnf_ptr->client_id_and_evt_reg_cnf);
      break;

    case MMGSDI_SESSION_OPEN_EXT_CNF:
      qmi_uim_client_open_session_ext_conf(status, &cnf_ptr->session_open_ext_cnf);
      break;

    case MMGSDI_SESSION_OPEN_WITH_SELECT_RSP_CNF:
      qmi_uim_client_open_session_with_select_rsp_conf(
        status, &cnf_ptr->session_open_with_sel_rsp_cnf);
      break;

    case MMGSDI_SESSION_OPEN_WITH_MF_CNF:
      qmi_uim_client_open_session_with_mf_conf(status, &cnf_ptr->session_open_with_mf_cnf);
      break;

    case MMGSDI_SESSION_CLOSE_CNF:
      qmi_uim_client_close_session_resp(status, &cnf_ptr->session_close_cnf);
      break;

    case MMGSDI_READ_CNF:
      cb_userdata_ptr = (qmi_uimi_userdata_type*)cnf_ptr->response_header.client_data;
      if(cb_userdata_ptr == NULL)
      {
        return;
      }
      if(cb_userdata_ptr->request_id == UIMI_CMD_VAL_GET_GBA_IMPI)
      {
        qmi_uim_gba_read_impi_resp(status, &cnf_ptr->read_cnf);
      }
      else
      {
        qmi_uimi_read_resp(status, &cnf_ptr->read_cnf);
      }
      break;

    case MMGSDI_WRITE_CNF:
      qmi_uimi_write_resp(status, &cnf_ptr->write_cnf);
      break;

    case MMGSDI_GET_FILE_ATTR_CNF:
      cb_userdata_ptr = (qmi_uimi_userdata_type*)cnf_ptr->response_header.client_data;
      if (cb_userdata_ptr == NULL)
      {
        return;
      }
      if (cb_userdata_ptr->request_id == UIMI_CMD_VAL_WRITE_RECORD)
      {
        qmi_uimi_write_record_after_get_file_attributes(status, &cnf_ptr->get_file_attr_cnf);
      }
      else
      {
        qmi_uimi_get_file_attributes_resp(status, &cnf_ptr->get_file_attr_cnf);
      }
      break;

    case MMGSDI_CARD_PDOWN_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_CARD_PUP_CNF:
      qmi_uimi_power_up_resp(status, &cnf_ptr->card_pup_cnf);
      break;

    case MMGSDI_REFRESH_CNF:
      qmi_uimi_refresh_resp(status, &cnf_ptr->refresh_cnf);
      break;

    case MMGSDI_PIN_OPERATION_CNF:
      qmi_uimi_pin_operation_resp(status, &cnf_ptr->pin_operation_cnf);
      break;

    case MMGSDI_RUN_CAVE_CNF:
      qmi_uimi_run_cave_resp(status, &cnf_ptr->run_cave_cnf);
      break;

    case MMGSDI_SESSION_RUN_GSM_ALGO_CNF:
      qmi_uimi_run_gsm_algo_resp(status, &cnf_ptr->session_run_gsm_algo_cnf);
      break;

    case MMGSDI_ISIM_AUTH_CNF:
      qmi_uimi_isim_auth_resp(status, &cnf_ptr->isim_auth_cnf);
      break;

    case MMGSDI_USIM_AUTH_CNF:
      qmi_uimi_usim_auth_resp(status, &cnf_ptr->usim_auth_cnf);
      break;

    case MMGSDI_COMPUTE_IP_AUTH_CNF:
      qmi_uimi_compute_ip_auth_resp(status, &cnf_ptr->compute_ip_cnf);
      break;

    case MMGSDI_SESSION_ENABLE_SERVICE_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_SESSION_DISABLE_SERVICE_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_SRV_AVAILABLE_CNF:
      qmi_uimi_get_service_available_resp(status, &cnf_ptr->srv_available_cnf);
      break;

    case MMGSDI_SESSION_GET_APP_CAPABILITIES_CNF:
      qmi_uimi_get_service_enabled_resp(status, &cnf_ptr->session_app_capabilities_cnf);
      break;

    case MMGSDI_GET_ALL_PIN_STATUS_CNF:
      cb_userdata_ptr = (qmi_uimi_userdata_type*)cnf_ptr->response_header.client_data;
      if (cb_userdata_ptr == NULL)
      {
        qmi_uimi_nonprov_get_all_pin_status(status, &cnf_ptr->get_all_pin_status_cnf);
      }
      else
      {
        qmi_uimi_get_all_pin_status_resp(status, &cnf_ptr->get_all_pin_status_cnf);
      }
      break;

    case MMGSDI_SESSION_ACTIVATE_OR_SWITCH_PROVISIONING_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_SESSION_DEACTIVATE_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_SEND_APDU_EXT_CNF:
      qmi_uimi_send_apdu_resp(status, &cnf_ptr->send_apdu_ext_cnf);
      break;

    case MMGSDI_SAP_CONNECT_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_SAP_DISCONNECT_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_SAP_GET_ATR_CNF:
    case MMGSDI_SAP_SEND_APDU_CNF:
    case MMGSDI_SAP_CARD_READER_STATUS_CNF:
    case MMGSDI_SAP_POWER_ON_CNF:
    case MMGSDI_SAP_POWER_OFF_CNF:
    case MMGSDI_SAP_RESET_CNF:
      qmi_uimi_sap_request_resp(status, cnf_type, cnf_ptr);
      break;

    case MMGSDI_SESSION_SUBSCRIPTION_OK_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_GET_ATR_CNF:
      qmi_uimi_get_atr_resp(status, &cnf_ptr->get_atr_cnf);
      break;

    case MMGSDI_REHABILITATE_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_INVALIDATE_CNF:
       qmi_uimi_generic_no_payload_resp(uim_message_ptr);
       break;

    case MMGSDI_INCREASE_CNF:
      qmi_uimi_increase_resp(status, &cnf_ptr->increase_cnf);
      break;

    case MMGSDI_SESSION_SELECT_AID_CNF:
      qmi_uimi_reselect_resp(status, &cnf_ptr->session_select_aid_cnf);
      break;

    case MMGSDI_CARD_STATUS_CNF:
      qmi_uimi_send_status_resp(status, &cnf_ptr->status_cnf);
      break;

    case MMGSDI_GET_SIM_PROFILE_CNF:
      qmi_uimi_get_sim_profile_resp(status, &cnf_ptr->get_sim_profile_cnf);
      break;

    case MMGSDI_SET_SIM_PROFILE_CNF:
      cb_userdata_ptr = (qmi_uimi_userdata_type*)cnf_ptr->set_sim_profile_cnf.response_header.client_data;
      if (cb_userdata_ptr == NULL)
      {
        return;
      }
      qmi_uimi_generic_result_resp(qmi_uim_util_convert_mmgsdi_status_to_errval(status),
                                   cb_userdata_ptr);
      break;

    case MMGSDI_SEARCH_CNF:
      qmi_uimi_search_record_resp(status, &cnf_ptr->search_cnf);
      break;

    case MMGSDI_PURGE_TEMPORARY_IDENTITY_CNF:
      qmi_uimi_purge_temporary_identity_resp(status, &cnf_ptr->purge_temporary_identity_cnf);
      break;

    case MMGSDI_SET_APDU_BHEAVIOR_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    case MMGSDI_GET_M2M_EID_CNF:
      qmi_uimi_get_m2m_eid_resp(uim_message_ptr);
      break;

    case MMGSDI_MAP_SESSIONS_TO_SLOTS_CNF:
      qmi_uimi_generic_no_payload_resp(uim_message_ptr);
      break;

    default:
      /* This case should not happen! */
      break;
  }

  /* Free memory allocated for deep copy */
  qmi_uim_cnf_free_copy(cnf_type, cnf_ptr);
} /* qmi_uim_process_mmgsdi_callback_msg */


/*===========================================================================
  FUNCTION QMI_UIM_MMGSDI_EVT_CBACK()

  DESCRIPTION
    This function is executed as callback from MMGSDI whenever there
    is an event. It is executed in the context of MMGSDI task and needs
    to make a deep copy of the event and post a command to the QMI task
    to be processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_mmgsdi_evt_cback
(
  const mmgsdi_event_data_type * event
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  ASSERT(event);

  switch(event->evt)
  {
    /* Post events to the QMI context. */
    case MMGSDI_CARD_INSERTED_EVT:
    case MMGSDI_CARD_ERROR_EVT:
    case MMGSDI_CARD_REMOVED_EVT:
    case MMGSDI_SIM_BUSY_EVT:
    case MMGSDI_CARD_DETECTION_STARTED_EVT:
    case MMGSDI_PIN1_EVT:
    case MMGSDI_PIN2_EVT:
    case MMGSDI_UNIVERSAL_PIN_EVT:
    case MMGSDI_SESSION_CHANGED_EVT:
    case MMGSDI_SUBSCRIPTION_READY_EVT:
    case MMGSDI_SESSION_CLOSE_EVT:
    case MMGSDI_PERSO_EVT:
    case MMGSDI_SESSION_ILLEGAL_SUBSCRIPTION_EVT:
    case MMGSDI_SESSION_LEGAL_SUBSCRIPTION_EVT:
    case MMGSDI_REFRESH_EVT:
    case MMGSDI_SAP_CONNECT_EVT:
    case MMGSDI_SAP_DISCONNECT_EVT:
    case MMGSDI_RECOVERY_COMPLETE_EVT:
    case MMGSDI_PERSO_TEMPORARY_UNLOCK_EVT:
    case MMGSDI_PURGE_TEMPORARY_IDENTITY_STATUS_EVT:
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_EVENT);
      if (msg_ptr == NULL)
      {
        return;
      }

      qmi_uim_event_deep_copy(&msg_ptr->data.event.evt, event);

      /* Send command */
      if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
         (present_in_queue))
      {
        qmi_uim_event_free_copy(&msg_ptr->data.event.evt);
        uimqmi_free(msg_ptr);
      }
      msg_ptr = NULL;
      break;

    /* All other cases are not handled, so it's useless to make a copy
       and post it to the QMI context. Simply drop unnecessary events */
    default:
      break;
  }
} /* qmi_uim_mmgsdi_evt_cback */


#ifdef FEATURE_GSTK
/*===========================================================================
  FUNCTION QMI_UIMI_OTASP_REG_CALLBACK()

  DESCRIPTION
    This function is executed as callback from GSTK and it determines whether
    registration for OTASP status is success.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_otasp_reg_callback
(
  gstk_status_enum_type     gstk_reg_status,
  gstk_client_ref_data_type user_data
)
{
  /* Ignore this callback as there is no action to be taken by QMI UIM */
  (void)gstk_reg_status;
  (void)user_data;
} /* qmi_uimi_otasp_reg_callback */


/*===========================================================================
  FUNCTION QMI_UIMI_OTASP_STATUS_CALLBACK()

  DESCRIPTION
    This function is executed as callback from GSTK and it provides OTASP
    status. It is executed in the context of GSTK task and needs to make
    a deep copy of the event and post a command to the QMI task to be processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uimi_otasp_status_callback
(
  gstk_slot_id_enum_type       slot,
  gstk_otasp_act_status_enum_type  status
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  UIM_MSG_HIGH_2("Received qmi_uimi_otasp_status_callback from GSTK, slot: 0x%x, status: 0x%x",
                 slot, status);

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_OTASP_STATUS_IND);
  if (msg_ptr == NULL)
  {
    return;
  }

  msg_ptr->data.otasp_status_ind.slot = slot;
  msg_ptr->data.otasp_status_ind.status = status;

  /* Send command */
  if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
     (present_in_queue))
  {
    uimqmi_free(msg_ptr);
  }
  msg_ptr = NULL;
} /* qmi_uimi_otasp_status_callback */
#endif /* FEATURE_GSTK */


/*===========================================================================
  FUNCTION QMI_UIMI_MAP_PROVISIONING_SESSIONS_TO_SLOTS()

  DESCRIPTION
    Map provisioning sessions to slots

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_uimi_map_provisioning_sessions_to_slots
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  mmgsdi_return_enum_type                mmgsdi_status   = MMGSDI_SUCCESS;
  qmi_uimi_userdata_type*                cb_userdata_ptr = NULL;
  qmi_error_e_type                       errval          = QMI_ERR_NONE;
  qmi_uim_tlv_item_type                  tlv_data[5];
  mmgsdi_sessions_to_slots_mapping_type  slot_mapping    = {MMGSDI_SLOT_NONE,
                                                            MMGSDI_SLOT_NONE,
                                                            MMGSDI_SLOT_NONE};

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[2].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_3;
  tlv_data[3].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_4;
  tlv_data[4].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[4].tag       = UIMI_TLV_TAG_OPTIONAL_5;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 5);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Populate the slot mapping struct with first three TLVs */
  if (tlv_data[0].present)
  {
    slot_mapping.pri_slot = tlv_data[0].data.slot.slot;
  }
  if (tlv_data[1].present)
  {
    slot_mapping.sec_slot = tlv_data[1].data.slot.slot;
  }
  if (tlv_data[2].present)
  {
    slot_mapping.ter_slot = tlv_data[2].data.slot.slot;
  }

  /* Return error if two session types map to the same slot */
  if ((slot_mapping.pri_slot != MMGSDI_SLOT_NONE &&
       (slot_mapping.pri_slot == slot_mapping.sec_slot ||
        slot_mapping.pri_slot == slot_mapping.ter_slot)) ||
      (slot_mapping.sec_slot != MMGSDI_SLOT_NONE &&
       slot_mapping.sec_slot == slot_mapping.ter_slot))
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* MMGSDI currently supports only primary to tertiary sessions,
     so return error if additional TLVs are present */
  if (tlv_data[3].present || tlv_data[4].present)
  {
    errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_MAP_PROVISIONING_SESSIONS_TO_SLOTS,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Call MMGSDI function to map sessions to slots*/
  mmgsdi_status = mmgsdi_map_sessions_to_slots(qmi_uim_global_ptr->mmgsdi_client_id,
                                               slot_mapping,
                                               qmi_uimi_mmgsdi_callback,
                                               (mmgsdi_client_data_type)cb_userdata_ptr);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 5);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_map_provisioning_sessions_to_slots */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_MMGSDI_EVENT_MSG()

  DESCRIPTION
    This function handles messages from the
    QMI UIM queue for events from MMGSDI

  PARAMETERS
    uim_message_ptr: Pointer to QMI UIM message from message queue

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_mmgsdi_event_msg
(
  qmi_uim_message_type * uim_message_ptr
)
{
  boolean                           sap_update_required   = FALSE;
  mmgsdi_event_data_type           *event_ptr             = NULL;
  mmgsdi_slot_id_enum_type          slot_id               = MMGSDI_MAX_SLOT_ID_ENUM;
  qmi_uim_card_update_required_type result                = {FALSE, FALSE};
  boolean                           any_slot_initializing = FALSE;

  ASSERT(uim_message_ptr);

  event_ptr = &uim_message_ptr->data.event.evt;

  /* Update the card status */
  switch(event_ptr->evt)
  {
    case MMGSDI_CARD_DETECTION_STARTED_EVT:
      qmi_uim_process_card_detection_started_evt(&event_ptr->data.card_detection);
      break;

    case MMGSDI_CARD_INSERTED_EVT:
      slot_id = event_ptr->data.card_inserted.slot;
      result =
        qmi_uim_process_card_inserted_evt(&event_ptr->data.card_inserted);
      break;

    case MMGSDI_SESSION_CHANGED_EVT:
      slot_id = event_ptr->data.session_changed.app_info.slot;
      result =
        qmi_uim_process_session_changed_evt(event_ptr->session_id,
                                            &event_ptr->data.session_changed);
      break;

    case MMGSDI_PIN1_EVT:
    case MMGSDI_PIN2_EVT:
    case MMGSDI_UNIVERSAL_PIN_EVT:
      result =
        qmi_uim_process_pin_evts(event_ptr->evt,
                                 &event_ptr->data.pin);
      break;

    case MMGSDI_SIM_BUSY_EVT:
      qmi_uim_process_sim_busy_evt(event_ptr->data.sim_busy.slot,
                                   event_ptr->data.sim_busy.sim_busy);
      break;

    case MMGSDI_CARD_ERROR_EVT:
      slot_id = event_ptr->data.card_error.slot;
      if(event_ptr->data.card_error.info == MMGSDI_CARD_ERR_CARD_REMOVED)
      {
        result.card_update_required =
          qmi_uim_process_card_removed_evt(event_ptr->data.card_removed.slot);
      }
      else
      {
        result.card_update_required =
          qmi_uim_process_card_error_evt(event_ptr->data.card_error.info,
                                         event_ptr->data.card_error.slot);
      }
      result.reduced_card_update_required = result.card_update_required;

      /* Check if SAP disconnect event needs to be sent */
      sap_update_required = qmi_uim_process_sap_evt(event_ptr);
      break;

    case MMGSDI_CARD_REMOVED_EVT:
      slot_id = event_ptr->data.card_removed.slot;
      result.card_update_required =
        qmi_uim_process_card_removed_evt(event_ptr->data.card_removed.slot);
      /* Check if SAP disconnect event needs to be sent */
      sap_update_required = qmi_uim_process_sap_evt(event_ptr);
      result.reduced_card_update_required = result.card_update_required;
      break;

    case MMGSDI_SESSION_ILLEGAL_SUBSCRIPTION_EVT:
      result.card_update_required =
        qmi_uim_process_manage_illegal_card_evt(event_ptr->session_id,
                                                MMGSDI_SESSION_APP_IS_ILLEGAL);
      result.reduced_card_update_required = result.card_update_required;
      break;

    case MMGSDI_SESSION_LEGAL_SUBSCRIPTION_EVT:
      result.card_update_required =
        qmi_uim_process_manage_illegal_card_evt(event_ptr->session_id,
                                                MMGSDI_SESSION_APP_IS_LEGAL);
      result.reduced_card_update_required = result.card_update_required;
      break;

    case MMGSDI_PERSO_EVT:
#ifndef FEATURE_SIMLOCK
      if (qmi_uim_get_halt_subscription_value())
#endif /* !FEATURE_SIMLOCK */
      {
        result =
          qmi_uim_process_perso_evt(event_ptr->session_id,
                                    &event_ptr->data.perso);
      }
      break;

    case MMGSDI_SUBSCRIPTION_READY_EVT:
      result.card_update_required =
        qmi_uim_process_subscription_ready_evt(event_ptr->session_id,
                                               &event_ptr->data.subscription_ready);
      result.reduced_card_update_required = result.card_update_required;
      break;

    case MMGSDI_SESSION_CLOSE_EVT:
      qmi_uim_process_session_closed_evt(event_ptr->session_id,
                                         &event_ptr->data.session_close);
      break;

    case MMGSDI_REFRESH_EVT:
      qmi_uim_process_refresh_evt(event_ptr->session_id,
                                  &event_ptr->data.refresh);
      break;

    case MMGSDI_SAP_CONNECT_EVT:
      slot_id = event_ptr->data.sap_connect.slot;
      sap_update_required = qmi_uim_process_sap_evt(event_ptr);
      break;

    case MMGSDI_SAP_DISCONNECT_EVT:
      slot_id = event_ptr->data.sap_disconnect.slot;
      sap_update_required = qmi_uim_process_sap_evt(event_ptr);
      break;

    case MMGSDI_RECOVERY_COMPLETE_EVT:
      qmi_uim_recovery_process_complete_evt(
         event_ptr->data.recovery_complete.slot);
      break;

#ifdef FEATURE_SIMLOCK
    case MMGSDI_PERSO_TEMPORARY_UNLOCK_EVT:
      qmi_uim_simlock_process_temporary_unlock_event(event_ptr);
      break;
#endif /* FEATURE_SIMLOCK */

    case MMGSDI_PURGE_TEMPORARY_IDENTITY_STATUS_EVT:
      qmi_uim_process_purge_temporary_identity_event(event_ptr);
      break;

    default:
      /* This should never happen: unhandled events are not sent
         to the QMI content! */
      return;
  }

  /* If card status of respective slot is valid then we only set card update required.
     We intentionally dont set reduced card update required as this change is of no interest */
  if (qmi_uim_update_card_status_validity(event_ptr))
  {
    result.card_update_required = TRUE;
  }

  /* Free the copy of the event (if deep copy was needed) */
  qmi_uim_event_free_copy(event_ptr);

  /* If the event is card error for current slot and card_update is required,
     irrespective of other slot is initializing or not, send the indication.  */
  if((event_ptr->evt == MMGSDI_CARD_REMOVED_EVT ||
      (event_ptr->evt == MMGSDI_CARD_ERROR_EVT &&
       (event_ptr->data.card_error.info == MMGSDI_CARD_ERR_PWR_DN_CMD_NOTIFY ||
        event_ptr->data.card_error.info == MMGSDI_CARD_ERR_CARD_REMOVED))) &&
     result.card_update_required)
  {
    any_slot_initializing = FALSE;
  }
  else
  {
    any_slot_initializing = qmi_uim_check_slot_detection_validity();
    if (qmi_uim_global_ptr->indication_dropped && !any_slot_initializing)
    {
      /* This means that an indication (due to change to some slot) was dropped
         in the past and so needs to be sent now, even if there isn't any newer
         change wrt the current slot.
         NOTE that any_slot_initializing will be FALSE only if we are here
         due to some CARD event (card_inserted/removed/error event) */
      qmi_uim_global_ptr->indication_dropped = FALSE;
      result.card_update_required = TRUE;
    }
  }

  /* Send update to clients */
  if (result.card_update_required)
  {
    if (event_ptr->session_id != 0)
    {
      if (qmi_uim_util_session_id_to_slot(&slot_id,
                                          event_ptr->session_id) != QMI_ERR_NONE)
      {
        UIM_MSG_ERR_1("Unable to determine slot for MMGSDI event 0x%x",
                      event_ptr->evt);
      }
    }

    /* Skip the IND if we haven't received MMGSDI events for any of the other slots
       (whose detection has started). The reason for this is because in scenarios
       where the card was absent, then inserted & was intializaing slowly, we
       incorrectly send the IND stating that the slot was still absent. Note that
       this approach has a drawback that the first IND after a reset/insertion
       will be sent only after the inserted/error event of the other slots,
       if applicable 
       If one or more indications were already sent, then continue sending
       indications, even if one slot is initializing, as it means that slots
       don't need to be synchronized.*/
    if (any_slot_initializing == FALSE || qmi_uim_global_ptr->indication_sent)
    {
      /* Mark indication sent as TRUE only when all slots are not card error.
         We do this because the subsequent indications for that slot can be
         sent even though the other slot detection is in progress */
      if(!qmi_uim_check_card_error_on_all_slots())
      {
        qmi_uim_global_ptr->indication_sent = TRUE;
      }

      /* Legacy card status indication is sent to clients who only register
         for legacy card status if the update is only to the legacy (first two)
         slots. Extended indication is sent to clients who register for it. */
      if (slot_id <= MMGSDI_SLOT_2)
      {
        qmi_uim_send_card_status_indication_to_all(TRUE,
                                                   FALSE,
                                                   result.reduced_card_update_required);
      }
      else
      {
        qmi_uim_send_card_status_indication_to_all(FALSE,
                                                   FALSE,
                                                   result.reduced_card_update_required);
      }
    }
    else
    {
      /* Mark that the indication due to this event on this slot is dropped
         until all the slots have reached beyond slot detection stage. */
      qmi_uim_global_ptr->indication_dropped = TRUE;
    }
  }
  if (sap_update_required)
  {
    qmi_uim_send_sap_status_indication_to_all(slot_id);
  }
} /* qmi_uim_process_mmgsdi_event_msg */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_ALLOC_CLID()

  DESCRIPTION
    Process client allocation from QMI Framework

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                     containing alloc_clid data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_alloc_clid
(
  qmi_uim_message_type * uim_message_ptr
)
{
  qmi_uimi_client_state_type * cl_sp     = NULL;
  qmi_result_e_type            result    = QMI_RESULT_FAILURE;
  uint8                        client_id = QMI_FRAMEWORK_SVC_MAX_CLIENTS;

  ASSERT(uim_message_ptr && qmi_uim_state_ptr);

  client_id = uim_message_ptr->data.clid_cb.cmd_hdr.client_id;

  UIM_MSG_HIGH_1( "Process clid allocation for clid:0x%x", client_id );

  if ((client_id == QMI_SVC_CLID_UNUSED ) ||
      (client_id > UIMQMI_SVC_MAX_CLIDS ))
  {
    qmi_framework_svc_send_alloc_clid_result_ex(result, &uim_message_ptr->data.clid_cb.cmd_hdr,
                                                QMI_ERR_INVALID_CLIENT_ID);
    return;
  }

  if (uim_message_ptr->data.clid_cb.cmd_hdr.service != QMUX_SERVICE_UIM)
  {
    qmi_framework_svc_send_alloc_clid_result_ex(result, &uim_message_ptr->data.clid_cb.cmd_hdr,
                                                QMI_ERR_INTERNAL);
    return;
  }

  cl_sp = uimqmi_malloc(sizeof(qmi_uimi_client_state_type));
  if (cl_sp == NULL)
  {
    qmi_framework_svc_send_alloc_clid_result_ex(result, &uim_message_ptr->data.clid_cb.cmd_hdr,
                                                QMI_ERR_NO_MEMORY);
    return;
  }
  memset(cl_sp, 0, sizeof(qmi_uimi_client_state_type));

  qmi_uim_state_ptr->client[client_id - 1] = cl_sp;
  ds_qmi_fw_common_cl_init(qmi_uim_state_ptr, (qmi_common_client_state_type *)cl_sp);
  cl_sp->common.clid = client_id;
  cl_sp->instance    = (uint16)uim_message_ptr->data.clid_cb.cmd_hdr.qmi_instance;
  cl_sp->service_id  = uim_message_ptr->data.clid_cb.cmd_hdr.service;

  UIM_MSG_HIGH_1("QMI UIM clid successful for clid:0x%x", cl_sp->common.clid );
  result = QMI_RESULT_SUCCESS;

  /* Notifying the clid allocation back to client via QMI Framework*/
  qmi_framework_svc_send_alloc_clid_result_ex(result, &uim_message_ptr->data.clid_cb.cmd_hdr,
                                              QMI_ERR_NONE);
} /* qmi_uim_process_alloc_clid */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_DEALLOC_CLID()

  DESCRIPTION
    Process client deallocation from QMI Framework

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                     containing dealloc_clid data

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_dealloc_clid
(
  qmi_uim_message_type * uim_message_ptr
)
{
  qmi_uimi_client_state_type * cl_sp     = NULL;
  qmi_result_e_type            result    = QMI_RESULT_FAILURE;
  uint16                       client_id = QMI_FRAMEWORK_SVC_MAX_CLIENTS;

  ASSERT(uim_message_ptr && qmi_uim_state_ptr);

  client_id = uim_message_ptr->data.clid_cb.cmd_hdr.client_id;

  UIM_MSG_HIGH_1("QMI_UIM process deallocation for Client_id: 0x%x",
                 client_id );

  if ((client_id == QMI_SVC_CLID_UNUSED ) ||
      (client_id > UIMQMI_SVC_MAX_CLIDS ))
  {
    UIM_MSG_ERR_1("Invalid client ID clid:0x%x", client_id );
    qmi_framework_svc_send_dealloc_clid_result_ex(result, &uim_message_ptr->data.clid_cb.cmd_hdr,
                                                  QMI_ERR_INVALID_CLIENT_ID);
    return;
  }

  cl_sp = (qmi_uimi_client_state_type *)
           qmi_uim_state_ptr->client[client_id  - 1];

  if (cl_sp != NULL && cl_sp->common.clid == client_id)
  {
    /* Increment Client reg. count when deactivation occurs. This ensures
       that when responses are recevied from MMGSDI for any pending command
       w.r.t. this client, no response is built */
    qmi_uim_state_ptr->client_reg_count[client_id - 1]++;
    qmi_uimi_reset_client(cl_sp);
    q_destroy( &cl_sp->common.x_free_q );
    q_destroy( &cl_sp->common.cmd_free_q );
    cl_sp->common.clid = QMI_SVC_CLID_UNUSED;
    cl_sp->instance    = UIMQMI_SVC_INVALID_INSTANCE;
    uimqmi_free(cl_sp);
    qmi_uim_state_ptr->client[client_id  - 1] = NULL;
    cl_sp = NULL;
    UIM_MSG_HIGH_1("QMI UIM clid deallocation successful for clid:0x%x",
                   client_id );
    result = QMI_RESULT_SUCCESS;
  }
  else
  {
    UIM_MSG_ERR_0("INVALID CLIENT ID" );
  }

  /* Notifying the clid deallocation back to client via QMI Framework*/
  qmi_framework_svc_send_dealloc_clid_result_ex(result, &uim_message_ptr->data.clid_cb.cmd_hdr,
                                                QMI_ERR_NONE);
} /* qmi_uim_process_dealloc_clid */


/*===========================================================================
  FUNCTION  QMI_UIM_IS_COMMAND_DISABLED()

  DESCRIPTION
    Utility function to determine if the command is disabled at compile
    time or not

  PARAMETERS
    cmd_type : QMI UIM command

  RETURN VALUE
    boolean

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_is_command_disabled
(
  uint16 cmd_type
)
{
  boolean is_disabled = FALSE;

  if(!(cmd_type != UIMI_CMD_VAL_WIDTH
#ifdef REMOVE_QMI_UIM_RESET_V01
     && cmd_type != UIMI_CMD_VAL_RESET
#endif /* REMOVE_QMI_UIM_RESET_V01 */
#ifdef REMOVE_QMI_UIM_READ_TRANSPARENT_V01
      && cmd_type != UIMI_CMD_VAL_READ_TRANSPARENT
#endif /* REMOVE_QMI_UIM_READ_TRANSPARENT_V01 */
#ifdef REMOVE_QMI_UIM_READ_RECORD_V01
      && cmd_type != UIMI_CMD_VAL_READ_RECORD
#endif /* REMOVE_QMI_UIM_READ_RECORD_V01 */
#ifdef REMOVE_QMI_UIM_WRITE_TRANSPARENT_V01
      && cmd_type != UIMI_CMD_VAL_WRITE_TRANSPARENT
#endif /* REMOVE_QMI_UIM_WRITE_TRANSPARENT_V01 */
#ifdef REMOVE_QMI_UIM_WRITE_RECORD_V01
      && cmd_type != UIMI_CMD_VAL_WRITE_RECORD
#endif /* REMOVE_QMI_UIM_WRITE_RECORD_V01 */
#ifdef REMOVE_QMI_UIM_GET_FILE_ATTRIBUTES_V01
      && cmd_type != UIMI_CMD_VAL_GET_FILE_ATTRIBUTES
#endif /* REMOVE_QMI_UIM_GET_FILE_ATTRIBUTES_V01 */
#ifdef REMOVE_QMI_UIM_SET_PIN_PROTECTION_V01
      && cmd_type != UIMI_CMD_VAL_SET_PIN_PROTECTION
#endif /* REMOVE_QMI_UIM_SET_PIN_PROTECTION_V01 */
#ifdef REMOVE_QMI_UIM_VERIFY_PIN_V01
      && cmd_type != UIMI_CMD_VAL_VERIFY_PIN
#endif /* REMOVE_QMI_UIM_VERIFY_PIN_V01 */
#ifdef REMOVE_QMI_UIM_UNBLOCK_PIN_V01
      && cmd_type != UIMI_CMD_VAL_UNBLOCK_PIN
#endif /* REMOVE_QMI_UIM_UNBLOCK_PIN_V01 */
#ifdef REMOVE_QMI_UIM_CHANGE_PIN_V01
      && cmd_type != UIMI_CMD_VAL_CHANGE_PIN
#endif /* REMOVE_QMI_UIM_CHANGE_PIN_V01 */
#ifdef REMOVE_QMI_UIM_DEPERSONALIZATION_V01
      && cmd_type != UIMI_CMD_VAL_DEPERSONALIZATION
#endif /* REMOVE_QMI_UIM_DEPERSONALIZATION_V01 */
#ifdef REMOVE_QMI_UIM_REFRESH_REGISTER_V01
      && cmd_type != UIMI_CMD_VAL_REFRESH_REGISTER
#endif /* REMOVE_QMI_UIM_REFRESH_REGISTER_V01 */
#ifdef REMOVE_QMI_UIM_REFRESH_OK_V01
      && cmd_type != UIMI_CMD_VAL_REFRESH_OK
#endif /* REMOVE_QMI_UIM_REFRESH_OK_V01 */
#ifdef REMOVE_QMI_UIM_REFRESH_COMPLETE_V01
      && cmd_type != UIMI_CMD_VAL_REFRESH_COMPLETE
#endif /* REMOVE_QMI_UIM_REFRESH_COMPLETE_V01 */
#ifdef REMOVE_QMI_UIM_REFRESH_GET_LAST_EVENT_V01
      && cmd_type != UIMI_CMD_VAL_REFRESH_GET_LAST_EVENT
#endif /* REMOVE_QMI_UIM_REFRESH_GET_LAST_EVENT_V01 */
#ifdef REMOVE_QMI_UIM_EVENT_REG_V01
      && cmd_type != UIMI_CMD_VAL_EVENT_REG
#endif /* REMOVE_QMI_UIM_EVENT_REG_V01 */
#ifdef REMOVE_QMI_UIM_GET_CARD_STATUS_V01
      && cmd_type != UIMI_CMD_VAL_GET_CARD_STATUS
#endif /* REMOVE_QMI_UIM_GET_CARD_STATUS_V01 */
#ifdef REMOVE_QMI_UIM_POWER_DOWN_V01
      && cmd_type != UIMI_CMD_VAL_POWER_DOWN
#endif /* REMOVE_QMI_UIM_POWER_DOWN_V01 */
#ifdef REMOVE_QMI_UIM_POWER_UP_V01
      && cmd_type != UIMI_CMD_VAL_POWER_UP
#endif /* REMOVE_QMI_UIM_POWER_UP_V01 */
#ifdef REMOVE_QMI_UIM_AUTHENTICATE_V01
      && cmd_type != UIMI_CMD_VAL_AUTHENTICATE
#endif /* REMOVE_QMI_UIM_AUTHENTICATE_V01 */
#ifdef REMOVE_QMI_UIM_CLOSE_SESSION_V01
      && cmd_type != UIMI_CMD_VAL_CLOSE_SESSION
#endif /* REMOVE_QMI_UIM_CLOSE_SESSION_V01 */
#ifdef REMOVE_QMI_UIM_GET_SERVICE_STATUS_V01
      && cmd_type != UIMI_CMD_VAL_GET_SERVICE_STATUS
#endif /* REMOVE_QMI_UIM_GET_SERVICE_STATUS_V01 */
#ifdef REMOVE_QMI_UIM_SET_SERVICE_STATUS_V01
      && cmd_type != UIMI_CMD_VAL_SET_SERVICE_STATUS
#endif /* REMOVE_QMI_UIM_SET_SERVICE_STATUS_V01 */
#ifdef REMOVE_QMI_UIM_CHANGE_PROVISIONING_SESSION_V01
      && cmd_type != UIMI_CMD_VAL_CHANGE_PROVISIONING_SESSION
#endif /* REMOVE_QMI_UIM_CHANGE_PROVISIONING_SESSION_V01 */
#ifdef REMOVE_QMI_UIM_GET_LABEL_V01
      && cmd_type != UIMI_CMD_VAL_GET_LABEL
#endif /* REMOVE_QMI_UIM_GET_LABEL_V01 */
#ifdef REMOVE_QMI_UIM_GET_CONFIGURATION_V01
      && cmd_type != UIMI_CMD_VAL_GET_CONFIGURATION
#endif /* REMOVE_QMI_UIM_GET_CONFIGURATION_V01 */
#ifdef REMOVE_QMI_UIM_SEND_APDU_V01
      && cmd_type != UIMI_CMD_VAL_SEND_APDU
#endif /* REMOVE_QMI_UIM_SEND_APDU_V01 */
#ifdef REMOVE_QMI_UIM_SAP_CONNECTION_V01
      && cmd_type != UIMI_CMD_VAL_SAP_CONNECTION
#endif /* REMOVE_QMI_UIM_SAP_CONNECTION_V01 */
#ifdef REMOVE_QMI_UIM_SAP_REQUEST_V01
      && cmd_type != UIMI_CMD_VAL_SAP_REQUEST
#endif /* REMOVE_QMI_UIM_SAP_REQUEST_V01 */
#ifdef REMOVE_QMI_UIM_LOGICAL_CHANNEL_V01
      && cmd_type != UIMI_CMD_VAL_LOGICAL_CHANNEL
#endif /* REMOVE_QMI_UIM_LOGICAL_CHANNEL_V01 */
#ifdef REMOVE_QMI_UIM_SUBSCRIPTION_OK_V01
      && cmd_type != UIMI_CMD_VAL_SUBSCRIPTION_OK_REQ
#endif /* REMOVE_QMI_UIM_SUBSCRIPTION_OK_V01 */
#ifdef REMOVE_QMI_UIM_GET_ATR_V01
      && cmd_type != UIMI_CMD_VAL_GET_ATR
#endif /* REMOVE_QMI_UIM_GET_ATR_V01 */
#ifdef REMOVE_QMI_UIM_OPEN_LOGICAL_CHANNEL_V01
      && cmd_type != UIMI_CMD_VAL_OPEN_LOGICAL_CHANNEL
#endif /* REMOVE_QMI_UIM_OPEN_LOGICAL_CHANNEL_V01 */
#ifdef REMOVE_QMI_UIM_REFRESH_REGISTER_ALL_V01
      && cmd_type != UIMI_CMD_VAL_REFRESH_REGISTER_ALL
#endif /* REMOVE_QMI_UIM_REFRESH_REGISTER_ALL_V01 */
#ifdef REMOVE_QMI_UIM_SET_FILE_STATUS_V01
      && cmd_type != UIMI_CMD_VAL_SET_FILE_STATUS
#endif /* REMOVE_QMI_UIM_SET_FILE_STATUS_V01 */
#ifdef REMOVE_QMI_UIM_SWITCH_SLOT_V01
      && cmd_type != UIMI_CMD_VAL_SWITCH_SLOT
#endif /* REMOVE_QMI_UIM_SWITCH_SLOT_V01 */
#ifdef REMOVE_QMI_UIM_GET_SLOTS_STATUS_V01
      && cmd_type != UIMI_CMD_VAL_GET_SLOTS_STATUS
#endif /* REMOVE_QMI_UIM_GET_SLOTS_STATUS_V01 */
#ifdef REMOVE_QMI_UIM_GET_PLMN_NAME_TABLE_INFO_V01
      && cmd_type != UIMI_CMD_VAL_GET_PLMN_NAME_TABLE_INFO
#endif /* REMOVE_QMI_UIM_GET_PLMN_NAME_TABLE_INFO_V01 */
#ifdef REMOVE_QMI_UIM_PERSONALIZATION_V01
      && cmd_type != UIMI_CMD_VAL_PERSONALIZATION
#endif /* REMOVE_QMI_UIM_PERSONALIZATION_V01 */
#ifdef REMOVE_QMI_UIM_INCREASE_V01
      && cmd_type != UIMI_CMD_VAL_INCREASE
#endif /* REMOVE_QMI_UIM_INCREASE_V01 */
#ifdef REMOVE_QMI_UIM_RECOVERY_V01
      && cmd_type != UIMI_CMD_VAL_RECOVERY
#endif /* REMOVE_QMI_UIM_RECOVERY_V01 */
#ifdef REMOVE_QMI_UIM_RESELECT_V01
      && cmd_type != UIMI_CMD_VAL_RESELECT
#endif /* REMOVE_QMI_UIM_RESELECT_V01 */
#ifdef REMOVE_QMI_UIM_SEND_STATUS_V01
      && cmd_type != UIMI_CMD_VAL_SEND_STATUS
#endif /* REMOVE_QMI_UIM_SEND_STATUS_V01 */
#ifdef REMOVE_QMI_UIM_GET_SIM_PROFILE_V01
      && cmd_type != UIMI_CMD_VAL_GET_SIM_PROFILE
#endif /* REMOVE_QMI_UIM_GET_SIM_PROFILE_V01 */
#ifdef REMOVE_QMI_UIM_SET_SIM_PROFILE_V01
      && cmd_type != UIMI_CMD_VAL_SET_SIM_PROFILE
#endif /* REMOVE_QMI_UIM_SET_SIM_PROFILE_V01 */
#ifdef REMOVE_QMI_UIM_SUPPLY_VOLTAGE_V01
      && cmd_type != UIMI_CMD_VAL_SUPPLY_VOLTAGE
#endif /* REMOVE_QMI_UIM_SUPPLY_VOLTAGE_V01 */
#ifdef REMOVE_QMI_UIM_DEPERSONALIZATION_SECURE_V01
      && cmd_type != UIMI_CMD_VAL_DEPERSONALIZATION_SECURE
#endif /* REMOVE_QMI_UIM_DEPERSONALIZATION_SECURE_V01 */
#ifdef REMOVE_QMI_UIM_PERSONALIZATION_SECURE_V01
      && cmd_type != UIMI_CMD_VAL_PERSONALIZATION_SECURE
#endif /* REMOVE_QMI_UIM_PERSONALIZATION_SECURE_V01 */
#ifdef REMOVE_QMI_UIM_EMERGENCY_ONLY_V01
      && cmd_type != UIMI_CMD_VAL_EMERGENCY_ONLY
#endif /* REMOVE_QMI_UIM_EMERGENCY_ONLY_V01 */
#ifdef REMOVE_QMI_UIM_SIMLOCK_CONFIGURATION_V01
      && cmd_type != UIMI_CMD_VAL_SIMLOCK_CONFIGURATION
#endif /* REMOVE_QMI_UIM_SIMLOCK_CONFIGURATION_V01 */
#ifdef REMOVE_QMI_UIM_SEARCH_RECORD_V01
      && cmd_type != UIMI_CMD_VAL_SEARCH_RECORD
#endif /* REMOVE_QMI_UIM_SEARCH_RECORD_V01 */
#ifdef REMOVE_QMI_UIM_GBA_V01
      && cmd_type != UIMI_CMD_VAL_GBA
#endif /* REMOVE_QMI_UIM_GBA_V01 */
#ifdef REMOVE_QMI_UIM_GBA_IMPI_V01
      && cmd_type != UIMI_CMD_VAL_GET_GBA_IMPI
#endif /* REMOVE_QMI_UIM_GBA_IMPI_V01 */
#ifdef REMOVE_QMI_UIM_REMOTE_UNLOCK_V01
      && cmd_type != UIMI_CMD_VAL_REMOTE_UNLOCK
#endif /* REMOVE_QMI_UIM_REMOTE_UNLOCK_V01 */
#ifdef REMOVE_QMI_UIM_VERIFY_IMSI_V01
      && cmd_type != UIMI_CMD_VAL_VERIFY_IMSI
#endif /* REMOVE_QMI_UIM_VERIFY_IMSI_V01 */
#ifdef REMOVE_QMI_UIM_TEMPORARY_UNLOCK_STATUS_IND_V01
      && cmd_type != UIMI_CMD_VAL_TEMPORARY_UNLOCK_STATUS_IND
#endif /* REMOVE_QMI_UIM_TEMPORARY_UNLOCK_STATUS_IND_V01 */
#ifdef REMOVE_QMI_UIM_PURGE_TEMPORARY_IDENTITY_V01
      && cmd_type != UIMI_CMD_VAL_PURGE_TEMPORARY_IDENTITY
#endif /* REMOVE_QMI_UIM_PURGE_TEMPORARY_IDENTITY_V01 */
#ifdef REMOVE_QMI_UIM_SWITCH_SLOT_EXT_V01
      && cmd_type != UIMI_CMD_VAL_SWITCH_SLOT_EXT
#endif /* REMOVE_QMI_UIM_SWITCH_SLOT_EXT_V01 */
#ifdef REMOVE_QMI_UIM_GET_PHYSICAL_SLOT_MAPPING_V01
      && cmd_type != UIMI_CMD_VAL_GET_PHYSICAL_SLOT_MAPPING
#endif /* REMOVE_QMI_UIM_GET_PHYSICAL_SLOT_MAPPING_V01 */
#ifdef REMOVE_QMI_UIM_GET_EID_V01
      && cmd_type != UIMI_CMD_VAL_GET_EID
#endif /* REMOVE_QMI_UIM_GET_EID_V01 */
#ifdef REMOVE_QMI_UIM_DELETE_PROFILE_V01
      && cmd_type != UIMI_CMD_VAL_DELETE_PROFILE
#endif /* REMOVE_QMI_UIM_DELETE_PROFILE_V01 */
#ifdef REMOVE_QMI_UIM_EUICC_MEMORY_RESET_V01
      && cmd_type != UIMI_CMD_VAL_EUICC_MEMORY_RESET
#endif /* REMOVE_QMI_UIM_EUICC_MEMORY_RESET_V01 */
#ifdef REMOVE_QMI_UIM_ADD_PROFILE_V01
      && cmd_type != UIMI_CMD_VAL_ADD_PROFILE
#endif /* REMOVE_QMI_UIM_ADD_PROFILE_V01 */
#ifdef REMOVE_QMI_UIM_UPDATE_NICKNAME_V01
      && cmd_type != UIMI_CMD_VAL_UPDATE_NICKNAME
#endif /* REMOVE_QMI_UIM_UPDATE_NICKNAME_V01 */
#ifdef REMOVE_QMI_UIM_GET_PROFILE_INFO_V01
      && cmd_type != UIMI_CMD_VAL_GET_PROFILE_INFO
#endif /* REMOVE_QMI_UIM_GET_PROFILE_INFO_V01 */
#ifdef REMOVE_QMI_UIM_TERMINAL_CAPABILITY_V01
      && cmd_type != UIMI_CMD_VAL_TERMINAL_CAPABILITY
#endif /* REMOVE_QMI_UIM_TERMINAL_CAPABILITY_V01 */
#ifdef REMOVE_QMI_UIM_SET_APDU_BEHAVIOR_REQ_V01
      && cmd_type != UIMI_CMD_VAL_SET_APDU_BEHAVIOR
#endif /* REMOVE_QMI_UIM_SET_APDU_BEHAVIOR_REQ_V01 */
#ifdef REMOVE_QMI_UIM_PROFILE_USER_CONSENT_REQ_V01
      && cmd_type != UIMI_CMD_VAL_PROFILE_USER_CONSENT
#endif /* REMOVE_QMI_UIM_PROFILE_USER_CONSENT_REQ_V01 */
#ifdef REMOVE_QMI_UIM_EUICC_DEFAULT_SERVER_ADDRESS_REQ_V01
          && cmd_type != UIMI_CMD_VAL_EUICC_DEFAULT_SERVER_ADDRESS
#endif /* REMOVE_QMI_UIM_EUICC_DEFAULT_SERVER_ADDRESS_REQ_V01 */
#ifdef REMOVE_QMI_UIM_MAP_PROVISIONING_SESSIONS_TO_SLOTS_REQ_V01
      && cmd_type != UIMI_CMD_VAL_MAP_PROVISIONING_SESSIONS_TO_SLOTS
#endif /* REMOVE_QMI_UIM_MAP_PROVISIONING_SESSIONS_TO_SLOTS_REQ_V01 */
#ifdef REMOVE_QMI_UIM_ACTIVATE_SIMLOCK_CONFIG_REQ_V01
          && cmd_type != UIMI_CMD_VAL_ACTIVATE_SIMLOCK_CONFIG
#endif /* QMI_UIM_ACTIVATE_SIMLOCK_CONFIG_REQ_V01 */
#ifdef REMOVE_QMI_UIM_GET_SIMLOCK_TOKEN_REQ_V01
          && cmd_type != UIMI_CMD_VAL_GET_SIMLOCK_TOKEN
#endif /* QMI_UIM_GET_SIMLOCK_TOKEN_REQ_V01 */
    ))
  {
    is_disabled = TRUE;
  }

  return is_disabled;
} /* qmi_uim_is_command_disabled */


/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_MESSAGE()

  DESCRIPTION
    Generic QMI processing for events and confirmations

  PARAMETERS
    cmd_ptr : ptr to the cmd enum passed to QMI

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_process_message
(
  void
)
{
  qmi_uim_message_type * uim_message_ptr = NULL;

  uim_message_ptr = qmi_uim_get_message();
  if (uim_message_ptr == NULL)
  {
    UIM_MSG_ERR_0("No message in QMI UIM queue");
    return;
  }

  switch(uim_message_ptr->message_type)
  {
    case QMI_UIM_MESSAGE_CNF:
      qmi_uim_process_mmgsdi_callback_msg(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_EVENT:
      qmi_uim_process_mmgsdi_event_msg(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_PHYSICAL_SLOTS_STATUS:
      qmi_uim_sub_mgr_process_physical_slots_status_msg(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_ALLOC_CLID:
      qmi_uim_process_alloc_clid(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_DEALLOC_CLID:
      qmi_uim_process_dealloc_clid(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_FRAMEWORK_CMD:
      uimqmi_process_framework_cmd(
        &uim_message_ptr->data.framework_cmd.cmd,
        (qmi_common_svc_state_type*)qmi_uim_state_ptr,
        (qmi_common_client_state_type*)
          qmi_uim_state_ptr->client[uim_message_ptr->data.framework_cmd.cmd.cmd_hdr.common_hdr.client_id - 1],
        qmi_uimi_cmd_callbacks,
        sizeof(qmi_uimi_cmd_callbacks) / sizeof(qmi_uimi_cmd_callbacks[0]),
        qmi_uim_is_command_disabled);
      break;
    case QMI_UIM_MESSAGE_RECOVERY_CB:
      qmi_uim_recovery_process_callback(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_SUPPLY_VOLTAGE_IND:
      qmi_uim_process_supply_voltage_ind(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_OTASP_STATUS_IND:
      qmi_uim_process_otasp_status_indication(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_GBA_CB:
      qmi_uim_gba_process_gba_cb(uim_message_ptr);
      break;
#ifdef FEATURE_SIMLOCK
    case QMI_UIM_MESSAGE_SIMLOCK_CB:
      qmi_uim_simlock_process_simlock_cb(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_SIMLOCK_REMOTE_SFS_CB:
      qmi_uim_simlock_process_remote_sfs_cb(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_SIMLOCK_SET_REMOTE_SFS_CB:
      qmi_uim_simlock_process_set_remote_sfs_resp(uim_message_ptr->data.simlock_set_remote_sfs_resp.status,
                                                  (qmi_uimi_userdata_type*)uim_message_ptr->data.simlock_set_remote_sfs_resp.user_data_ptr);
      break;
    case QMI_UIM_MESSAGE_REMOTE_UNLOCK_CB:
      qmi_uim_simlock_process_remote_unlock_resp(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_SIMLOCK_TEMPORARY_UNLOCK_CB:
      qmi_uim_simlock_process_temporary_unlock_cb(uim_message_ptr);
      break;
#endif /* FEATURE_SIMLOCK */
    case QMI_UIM_MESSAGE_GBA_EVENT:
      qmi_uim_gba_process_gba_event_cb(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_LPA_CB:
      qmi_uim_process_lpa_cb(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_LPA_EVENT:
      qmi_uim_process_lpa_event_cb(uim_message_ptr);
      break;
    case QMI_UIM_MESSAGE_LPA_PROFILE_INFO_CB:
      qmi_uim_process_lpa_profile_info_cb(uim_message_ptr);
      break;
    default:
      break;
  }

  uimqmi_free(uim_message_ptr);
  uim_message_ptr = NULL;
} /* qmi_uim_process_message */


#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
  FUNCTION  UIM_QSH_MDUMP_UIMQMI

  DESCRIPTION
    This function is used to collect qmi module globals.

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
QSH_MDUMP_FN_ATTR void uim_qsh_mdump_uimqmi(void)
{
  uint8 index = 0;

  if(qmi_uim_global_ptr != NULL)
  {
    qsh_mdump_collect_high(qmi_uim_global_ptr, sizeof(qmi_uim_global_type));
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr != NULL)
    {
      qsh_mdump_collect_high(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr, sizeof(uim_phy_slot_status_type));
    }

    for(index = 0; index < UIM_MAX_NON_PROV_SESSIONS; index++)
    {
      if(qmi_uim_global_ptr->non_prov_session[index] != NULL)
      {
        qsh_mdump_collect_high(qmi_uim_global_ptr->non_prov_session[index], sizeof(qmi_uim_non_prov_session_info_type));
      }
    }
  }
}/* uim_qsh_mdump_uimqmi */
#endif /* FEATURE_QSH_MDUMP */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
