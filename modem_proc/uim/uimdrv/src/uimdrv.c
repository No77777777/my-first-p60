/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             U I M  D R I V E R

                       User Identity Module Low Level Driver

GENERAL DESCRIPTION

  This module contains the device driver for the UIM.  It handles the low
  level hardware interface to a UIM.

EXTERNALIZED FUNCTIONS

  uim_device_init()
  Turns on the clock regime, places the control lines in a low state and
  selects or enables the control lines.

  uim_power_up()
  Places the control lines in the proper state in the proper order so that
  the driver is ready to reset the UIM.

  uim_reset()
  Prepares the driver for a reset then resets the UIM.

  uim_send_pps()
  Sends a PPS request to the UIM then expects a PPS response.

  uim_update_op_params()
  Changes the baud rate on the I/O line based on a frequency conversion
  factor index and a baud rate adjustment factor index.

  uim_send_command()
  Causes an APDU to be sent to the UIM then waits for a response.  The driver
  may send a "GET RESPONSE" APDU to get the response and/or may send a "FETCH"
  to get a pro-active SIM command.

  uim_power_down()
  Places the control lines in a low state in the proper order for powering
  off the UIM.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  uim_device_init() must be called prior to any of the other functions of
  this module.

Copyright (c) 1999-2017 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimdrv.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/11/17   gm      No GET_RESPONSE for SEEK command and 62 82 status words
06/09/17   nc      Polling fix - handling CM PH EVT
05/19/17   ku      No GET_RESPONSE for SEEK command
05/16/17   nc      Allow polling in FTM mode 
03/13/17   ku      Common path for local and remote card
01/09/17   ks      Clearing timers upon uim power down
11/24/16   ks      Adding support for spcom interface
10/03/16   dd      Removed lint comments
09/22/16   gm      Added support for set APDU behavior
07/22/16   hyo     Napali Chip Info update  
07/22/16   ks      Update UARTDM version before first CR access.
07/05/16   hyo     Napali bring-up changes
06/15/16   gm      Stub out Hotswap related functionality
06/06/16   na      Removed F3 traces
05/23/16   sam     Stub out UIM supply voltage indication code for IOE flavour
05/23/16   na      F3 Optimization
03/28/16   sam     Support for SAM card
01/27/16   sam     clear UIM FIFO upon timeout for T=1 card as well
10/29/15   hyo     Avoid writing enums to EFS 
09/25/15   sam     Conversion of globals to unsigned data types which 
                   cannot have negative values
09/24/15   ll      Add guards depends on UARTDM version and clock tree HW
09/02/15   hyo     Drivers and state machine abstraction
08/18/15   hyo     Restart polling when switching back to RF active mode
08/11/15   hyo     usb uicc removal
07/09/15   sam     modify virtuim_card_emu_reset to accept uim_ptr as argument
06/25/15   ll      Clean up ATLAS HW CR feature flags
05/05/15   ks      Enhancing the voltage supply indication to QMI feature
05/01/15   ll      Atlas HW CR -- Check for status before write to CR
04/15/15   ks      De-Featurizing the FEATURE_UIM_REMOTE/BTSAP_UIM
08/04/15   sam      FR24498: Powerup logging new changes
04/06/15   hyo     Disable polling when RF is inactive
03/24/15   na      Reverting registers write for rumi
03/02/15   na      Fix for implicit declaration of function of RUMI
02/18/15   sam     Fix for KW errors
02/13/15   sam     Decoupling of FEATURE_UIM_T_1_SUPPORT
01/16/15    na     Suppport of RUMI
01/07/14   sam     Modification of msg macros for FR24498-UIM Powerup Logging
01/05/15   ks      UIM handling task stop signal when interface not used
12/09/14   ks      Fixing issue when the interface not used flag is TRUE
12/02/14   sam     Switch to new string based TLMM APIs
11/10/14   akv     Support for user to customize LDO settings
10/27/14   ll      KW
10/15/14   ll      add assert header for core dependent compile error
09/23/14   ks      8909 bring up changes
09/22/14   akv     Code optimization
09/18/14   akv     UIMDRV UT enhancements
08/28/14   akv     UIMDRV-QMI interactions - Indications and acks for LDO events
08/15/14   js      Support for ATR / PPS in card emulator.
08/04/14   ll      Fix to allow the clock stop for Normal and Passive mode
07/25/14   ll      Fix link errors for ASSERT and ERR_FATAL
07/04/14   ssr     Fix for APDU debug Mask
06/16/14   ll      Switch to new string based TLMM APIs
06/11/14   ll      Add interruptible time delay and return value for update params
06/04/14   js      Prevent race condition for clock on/off
06/04/14   na/js   Handling of case 4 commands for non telecom apps
05/12/14   ak      8994 Bring up changes
05/27/14   kr      Feature macro clean up.
05/23/14   ks      Enabling software command response timer
05/13/14   ks      Consolidated fixes for the drivers stuck issue
05/06/14   ks      8936 bring up changes
04/21/14   rm/akv  Fix for glitch seen on the IO line while powering down
04/15/14   ks      Reverting the changes of CR556030 that introduced sw wwr
04/06/14   ks      Introducing software wwt running parallel to h/w wwt
03/27/14   lxu     USB UICC modem implement
03/20/14   sam     Fix to pet dog during power up wait/delay
03/05/14   sam     Fix for KW issues 02/13
03/03/14   ks      Triggering recovery upon recepiton of even
                   single RX break error
02/19/14   ks      Provision for explicit MF or ADF selction
                   And provision for inverse convention bootup
02/20/14   ak      Fix compile errors due to strict compiler on 9x35
02/18/14   ak      Maximize the UART clock frequency by setting it directly
01/29/14   sam     Usage of newer style diag macros
02/08/14   sam     Fix for klocwork issues
02/06/14   ll      Add debug messages of GPIO configs, GPIO values and ATR
01/29/14   akv     RX ISR refactoring changes
01/14/13   nmb     Reset UART when pps contains invalid TCK
01/09/14   akv     Handle to vote for mss bus freq made uim instance specific
12/27/13   ks      8916 bring up changes
12/18/13   ll      Old NULL Byte timer logic clean up for SIM BUSY feature
12/12/13   ks      BTSAP in client mode, intial implementation
12/02/13   akv     Apdu logging during rx isr and flushing buffer in power down
11/27/13   akv     Changes for voting against/for sleep on 9x35
11/21/13   na      Fix for testcase failure for QTFCRM
11/19/13   ks      Masking interrupts before setting the rx_state to IDLE
11/13/13   akv     Program BCR Reg to enable all features for UARTDM 1.4.0
11/11/13   na      UT-Framework support
11/11/13   js      Updated handling of task stop signal
10/24/13   akv     Removed unnecessary feature flags
10/24/13   ks      Secure memory optimization to cater destination buffer overflow
10/22/13   ll      Add UIM BUSY Response feature
10/22/13   ll      Check UIM GPIOs as inputs upon timeout w/recovery disabled
10/21/13   sam     Removed include file virtual_uimdrv.h -> file moved to
                   offtarget location
10/16/13   akv     Fixed KW errors
08/30/13   nmb     Add delay before turning off the HCLK clocking the UART
10/08/13   sam     changes for compatiblity with the new LLVM compiler
10/08/13   ks      Support for clock off for BT SAP
10/03/13   ssr     F3 log reduction
                   Minor correction of Hotswap code
09/30/13   akv     Removal of multiple efs reads performed by uim tasks
09/23/13   nmb     Remove GET_CURR_INSTANCE
09/23/13   ak      Hotswap debug message cleanup
09/23/13   sam     Fix for Klockworck issues
09/19/13   akv     Reference CLK changes to support 4.8MHz
09/11/13   yk      Removed the code for old targets and for old features
09/12/13   js      Fix hotswap for UT
09/09/13   akv     Enablement of stale interrupt only after configuring DMRX
                   and one time configuration of BADR
08/28/13   yt      Deprecate VIRTUIM/VIRTSAT module
08/22/13   ak      Remove FEATURE_DIME_MODEM and cleanup some dead code
08/21/13   hyo/js  Hotswap support and APDU logging on MOB
08/21/13   akv     Remove clk disablement in uim_hotswap_reconfig_uartdm
08/13/31   js      UIMDRV Memory De-Couple changes
08/10/13   ks      Advanced Subscription Manager changes
08/09/13   js      INS byte need to be preserved through  the APDU transaction
08/06/13   rm/ak   Bolt RUMI code
06/08/13   ks      Fix to make ATR 40K clk cycles equivalent timer accurate
08/1/13    sam     APDU logging mask variable moved to uim_instance_global_type
07/25/13   sam     APDU logging mask selection based on run time
                   configuration
07/19/13   nmb     Avoid duplicating streaming apdu commands on prop. apps
07/15/13   ssr     Fixed 62XX status byte for APP selection through
                   QMI_UIM_LOGICAL_CHANNE
07/12/13   nmb     Global Reorganization
07/11/13   ak      Disable UIM 3 for RUMI bring up
07/10/13   nmb     Return from dev_init if the slot is disabled in config nv
07/04/13   na      NULL Byte timer implementation
06/24/13   js      TSTS changes
06/19/13   js      Fixed issue with crash on ICC power up
06/16/13   ssr     Fixed to avoid the overwriting of 0x6C status byte as
                   intermediate status byte
05/30/13   js      For unknown ACK bytes do not switch INS class.
05/30/13   js      NFC:Send response data and SWs if SWs are not recognizaed
05/22/13   akv     Warm reset fixes for Triton
05/22/13   akv     Configuration of UICC shutdown gpio's and feature
                   enablement based on NV
05/16/13   na      Secure memory copy
05/13/13   akv     T=1 changes - Sending bytes using MSMU_ISR_TX_READY instead
                   of MSMU_ISR_TX_COMP
04/04/13   ak      Enable RX_BREAK_ZERO_CHAR_OFF in the UART_DM_MR2 register
                   to avoid receiving junk bytes during rx breaks
02/28/13   na      Sending response data along with its status bytes
                   as 0x63/0x63 warnings for uart_dm
02/22/13   js      8x26 bring up changes
02/19/13   ak      Replace MCPM calls with NPA calls to /clk/mss/config_bus
11/15/13   js      Fixed buffer overflow issue with UIM APDU logging
02/12/13   js      General F3 macro cleanup
02/11/13   akv     UIM API for querying the available/enabled UIM slots
02/08/13   js      Updated UIM USB code for parallel processing
02/08/13   akv     Power optimization fixes
02/06/13   js      Fix issue with dangling ptr while re-select MF
                   after SW1 warnings
01/17/13   js      Fixed APDU logging.
12/26/12   akv     WWT fix for Dime
12/18/12   nmb     Relay SW1=63 to client with additional data for case 4 ins
12/17/12   js      Fixed KW error
12/12/12   mt      Fixed the bug to support the clock stop test cases
12/05/12   js      UIM Parallel Processing changes
11/21/12   rm      Fix to enable the clock after changing the ownership of the
                   clock gpio to UIM controller
11/09/12   akv     BATT ALARM configuration for Dime
11/06/12   ms      Fix to add large delays during power-up
11/01/12   akv     Core cpu vdd voting to avoid Q6 going into power collapse
10/26/12   akv     HW Enumeration changes (without features)
10/26/12   akv     HW Enumeration changes
10/11/12   ms      Fix to handle the error case scenario where pending bytes
                   are seen in RX FIFO while Rx or Parity error.
                   It is fixed by stopping the command response timer after
                   the third unsuccessful attempt of reading of pending byte
                   in RX FIFO.
09/26/12   av      Make UIM_MAX_NUM_NULL_PROC_BYTES count run time configurable
09/26/12   rm      WWT fixes over GCF test failures
09/26/12   js      Dual SIM power up fixes
09/05/12   ms      Fix to replace FEATURE_UIM_SUPPORT_HOTSWAP with the NV item
08/31/12   js      Fixed compiler warnings
08/29/12   nmb     For invalid procedure byte don't reset wwt after we report
08/28/12   js      Initial changes for UT framework
08/26/12   ssr     Moving Hotswap timer_def from timer expire cb to hotswap init
08/22/12   ms      Set clock frequency for Sim clock for UIM1
08/17/12   ms      Set clock frequency for Sim clock
08/15/12   rm      Enable UIM2
08/09/12   js/rm   Fixes for UIM controller hotswap and hardware WWT features
08/06/12   rm/js   Impacts on UIM due to new PMIC NPA interfaces
07/25/12   nmb     End processing once SW 62 83 is encountered
07/16/12   akv     Modification of sim power up sequence
06/08/12   rm      Bring-up fixes for UIM1 and UIM2 on emulation
06/06/12   nmb     Add infinite null check to process procedure bytes dm
05/29/12   nmb     Increase maximum NULL procedure bytes limit
05/21/12   yb      Robust fix to handle 61 XX chained response
05/17/12   tl      Add support for Virtual SIM on power down
05/17/12   js      Fix issue that prevented TCXO shutdown
05/11/12   ak      Handle 62 F3 status words in UART DM handler
05/02/12   av      Replaced FEATURE_UIM_SUPPORT_HOTSWAP with an nvitem
05/02/12   ak      Fixes for dual SIM
05/01/12   ak      Add 100us delays between GSBI CR writes to address a
                   crash during concurrent SPI access
04/30/12   rm      Changing PMIC_NPA_RESOURCE_UIM1/2 to
                   PMIC_NPA_GROUP_ID_UIM1/2
04/24/12   yb      Handle MORE DATA SW2 words for SW1=0x62 and SW1=0x64
04/12/12   rm      Fix to ignore the bytes with parity error in char mode
03/30/12   av      Replace feature flags with NVITEM
03/27/12   nmb     Avoid dividing by zero
03/23/12   rm      Changes for hardware WWT and hotswap support
03/23/12   ak      Handle '62' 'F3' status words
03/22/12   ms      Added fix for failures due to multiple power ons seen in
                   electrical GCF tests
03/15/12   rm      Changes for DIME
03/08/12   nmb     Notify card of logical channel support
02/21/12   rm      Setting card detection polarity based on NV item
03/07/12   js      Correct UART DM timing correction for T=0 and T=1
03/02/12   js      1) Vote for CPU latency node
                   2) Adjust FIFO size for case 2/3 instructions
03/02/12   av      Removed feature FEATURE_UIM_CONTROL_UIM_UART_CLK_REGIME
                   and FEATURE_UIM_SUPPORT_INVERSE_CONVENTION_FOR_TX_IN_HW
03/01/12   js      Handling of SW2_MORE_DATA for UART DM targets
02/29/12   bcho    Virtual SIM clean-up
02/28/12   ak      Change debounce timer to non-deferrable timer
02/18/12   av      Cleaned up remaining ALWAYS OFF/ON feature flags
02/15/12   js      Handle EFS logging issue
02/03/12   js      Reverting UART DM fix
01/24/12   yb      Fixes to save power impact due to delay before and after
                   clock stop in idle mode
01/20/12   js      Changes needed for 8930 UIM bring up
01/11/12   ak      Power off for card removal in the debounce timer callback
01/11/12   rm      Fix to handle 6F C2 as a successful response
01/06/12   ms      Fixes for Thread-safe analysis
12/23/11   shr     Removed featurization for ALWAYS ON features
12/22/11   rm      Fix to interpret 92 XX as successful response
12/21/11   ms      Fixes for Legacy GSDI removal. Modified Card error notification
                   call w.r.t interface of MMGSDI API
12/08/11   ak      Remove software fix for multiple pon GCF failures
12/05/11   js      Changed for 8930 bring up Phase I
12/01/11   rm/ms   Fix to featurize the handling of SW2_MORE_DATA in warnings
                   Handling new / unknown instructions resulting in warnings
11/30/11   yb      Fix to do get response on 62 82 for SEEK in case of GCF
                   but not for card
11/30/11   js      Register and vote for CFPB clock
11/28/11   js      Removed usage of sleep APIs
11/16/11   rm      Fix for start or restart the work waiting timer
11/15/11   ms      Removed redundant code (UIM_POWER_ON)
11/15/11   ms      Fix for failures due to multiple power ons seen in
                   electrical GCF tests
11/03/11   ms      Fix to use small stale timeout value for default baud-rate
10/18/11   av      Removed features that are always OFF
10/07/11   rm      Fixes for T=1 protocol
10/04/11   yk      Change deferrable timers to non-deferrable timers
09/26/11   rm      Fix to bring the UART DM to direct convention upon a hot-swap
09/22/11   yb      Fixed featurization issue on guard-time during PPS
09/19/11   yb      Fix for UIM task dog timeout crash due to uim_rx_isr not
                   releasing uim_clk_ctl critical section.
09/09/11   ssr     Fix to allow the reset command when switch is un-pressed where
                   client want to disable the hotswap interrupt at runtime,
                   Added new API to return the real time card status,
                   Added new API to disable the hotswap interrupt
09/01/11   ak      GPIO hotswap support for 9x15
08/29/11   yb      Fix for UIM task dog timeout crash due to uim_rx_isr not
                   releasing uim_clk_ctl critical section.
                   Added support for complete recovery solution
                   Fix for interrupts getting fired in IDLE state for legacy
                   UART based targets
                   Fixed crash on card removal
08/24/11   ssr     Fixed NV Item support for hotswap configuration
08/24/11   rm      catch-up with nikel branch
08/24/11   ak      Remove code which sets M/N registers directly
08/23/11   ak      Fix uim_set_baud_rate so that uim_FI_current and
                   uim_DI_current get updated
08/19/11   rm      Fix for a compilation error on MOB
08/19/11   ak      Fixes for 9x15 bring-up
08/18/11   rm      Fix for timing issues with stale timeouts between rx level
                   interrupts with a fastest card
08/08/11   ssr     Preventing uim rx isr from getting stuck in infinite loop,
                   after maxing out on Rx break/parity/overrun errors
08/03/11   ssr     Fixed the Rx break error in IDLE mode in ATR phase
08/05/11   rm      Fix for the invalid card issue due to a {512, 64} card
08/04/11   yk      Fixed guardtime configuration problem due to featurization
08/03/11   ssr     Protect UIM global between UIM and ISR TASK
07/08/11   rm      Catch-up with latest on Hot-swap
06/17/11   ssr     Update few debug messages
06/13/11   ssr     Optimize UIM notification code for card removed
06/06/11   ssr     Update the code to do the card insertion based on the
                   NV_UIM_FIRST_INST_CLASS_I NV item
05/24/11   yb      Fix to use clock regime APIs instead of register HWIO for
                   clock register
                   Added feature for Q6 environment changes
                   Added feature for UART_DM bus contention issue
07/19/11   ak      9x15 bring-up code
06/28/11   ssr     Fix the guard time issue for T=1 protocol
05/14/11   yb      Fixes to remove INTLOCK
                   Fixes to remove delays for UART write
04/29/11   nk      Fix compiler warning for functions with no input parameters
04/29/11   rm      Featurize the handling of EOF reached during a SEEK operation
                   for legacy UART targets
04/26/11   rm      Featurize the handling of EOF reached during a SEEK operation
04/25/11   yb      8960 bring-up code
                   Adding support for hotswap
03/22/11   rm      Fix for T=1 issue on GSBI interface
03/16/11   ssr     Featurization the delay IO line in reception mode for 6x15 target
03/15/11   ssr     Fixed compilation error
03/15/11   ssr     Added a extra delay to come the IO line in reception mode
03/08/11   rm      Fix for featurization of T=1 related variables under
                   T=1 feature flag
02/28/11   ssr     Fixed compilation warnings
02/16/11   rm      Fix to not do get response if warnings indicate that file is
                   not found
01/31/11   ssr     Decoupled FEATURE_UIM_JCDMA_RUIM_SUPPORT feature
01/26/11   ps      Fix error fatal when USB is disconnected on 7x30 DSDS
01/12/11   ps      Fix compile issue on 7x30 DSDS
12/22/10   yt      Removed FEATURE_UIM_ZI_MEMORY_REDUCTION
12/07/10   ps      Fix GCF 27.11.1.5 GCF TC failure on SCMM, and some
                   DSDS changes for UART_DM targets
12/03/10   ps      Fix compilation issue due to mem reduction changesk
12/04/10   yb      Fix for 6x15 Bring-up
11/20/10   ps      Fixed uim_rx_state variable when accessed between ISR and UIM task
11/20/10   ps      Adding workaround for unexpected data on FIFO
11/10/10   yt      Memory reduction changes
11/04/10   ps      Removed HWIO_OUT calls and using clk_regime_gsbi calls for SCMM DSDS
10/30/10   ps      Fix to get UIM2 working on SCMM DSDS
10/30/10   yb      Fix for infinite null procedure byte issue
10/29/10   yb      6x15 bring up changes
10/29/10   ps      Merge SCMM Dual-SIM Driver
10/27/10   rm      Fix for a bus error due to UART register access
10/25/10   yk      Fixed compilation error.
10/19/10   yb      Fixed klocwork errors
09/27/10   yb      Fix for dog timeouts due to RX break errors
09/28/10   yb      Removed dependency on clk.h
09/14/10   yb      Fix for crash due to Parity errors with TX ready
09/10/10   ps      Merge UIM Driver DSDS changes
09/09/10   yb      Added Check for NPA handle before issuing command to NPA
09/06/10   yb      Fixed Lint error
09/02/10   yb      Replacing PMIC_NPA_CLIENT_UIM with "uim" for NPA APIs
08/31/10   yb      Fix for crash due to RX break in idle state
                   Added bit in UIM Config NV to ignore RX break errors in idle
08/16/10   yb      Added UIM Config NV parameters
07/18/10   ps      Fix to high Lint errors
07/20/10   rm      Fix for the bad status error
07/16/10   yk      Add more delay before sending data
06/30/10   rm      Add rex_init_crit_sect to initialize the uim_clk_ctl mutex
06/28/10   yb      Removing inclusion of deprecated file tlmm.h
06/18/10   ps      Fix for error Fatal when received invalid UIM channel
06/17/10   yb      Enabled use of PMIC NPA APIs
05/28/10   ps      Fix for no response from GSDI module
05/28/10   yb      8660 Bring up changes
05/27/10   rm      Fix to mask interrupts only when overrun errors max out
05/26/10   yb      Removing inclusion of deprecated files
05/10/10   yb      Fixed UIM APDU logging for Non-UART DM based targets
04/14/10   yb      Revert back changes for Secure SIM crash
04/13/10   rm      Program default BADR value
04/06/09   yk      Featurize the code added for the priority issue
03/29/10   yk      Added a function into the timer callback function to power
                   down UIM when cmd timer expires
03/23/10   yk      Moved the channel management to UIM server
03/23/10   yb      Removed dependency on ts.h
03/22/10   yb      Replacing tlmm gpio APIs with DAL APIs
03/19/10   yk      Fixed KW and Lint errors
03/12/10   ps      Fixed Lint errors
03/12/10   rm      Fixes for timing issues with UART_DM
02/19/10   yb      Fixed lint error
02/17/10   rm      Workaround to send card error when
                   FEATURE_UIM_INTERFACE_NOT_USED is defined
02/17/10   yb      Fix to handle parity errors in ATR for UART DM
02/09/10   rm      Fix to use REX_ISR_LOCK() instead of INTLOCK() to support
                   TASK to IST concurrency control of shared variable access
02/09/10   yb      Fix for buffer overflow while logging UIM APDUs
01/18/10   rm      Fixes to delay sending the R-block
01/13/10   rm      Fixes for 40K ATR issue
12/24/09   rm      Fixes to correct work-waiting time for UART_DM
12/22/09   yb      Fix for diagbuf_check_overrun_pattern() crash
12/17/09   yb      Fixed compilation warning
11/26/09   rm      Work waiting time fixes for T=1 protocol for UART_DM
11/12/09   rm      Fix to add a delay before a UART_DM transmit operation for
                   block mode
11/10/09   rm      Fix to configure UART_DM in SCMM to ignore received bytes
                   with parity error in character mode
11/05/09   rm      Add support to 9K target
10/29/09   rm      Fix to clear the UART IRQ when UIM clock is off to avoid
                   dog reset
10/08/09   yb      In case of stream apdu:
                   1. Report bad p3 status words to the clients.
                   2. Do not perform resend apdu internally.
10/13/09   ps      Add support for PMIC Vote API
10/12/09   kp      Demand Paging Changes
10/06/09   yb      1. Fix to avoid clock stop during recovery
                   2. Fix to add delays before clock stop and after clock restart,
                   to give time to card for finishing any pending internal processing.
09/28/09   ps      Handle unknown ACK procedure byte
09/26/09   kp      ZI memory reduction changes
09/25/09   yk      Add support for 7X30.
09/24/09   yb      Fixed compilation error due to diabling FEATURE_UIM_DEBUG_LOG
09/18/09   yb      Fix to not vote for sleep before registering for sleep.
09/17/09   yb      Removing dependency on tlmm_gpio.h
09/13/09   ps      Add support for MDM9K
09/11/09   rm      Handle TC1 byte in ATR for extra guard time
09/09/09   rm      Correction in handling of unknown procedure bytes
09/04/09   yk      Enable Tx and Rx interrupt after ATR, only applicable to SCMM.
09/01/09   rm      Handle unknown procedure bytes received as a command timeout
08/27/09   yb      Correcting function name uim_check_to_do_get_response_if_seek()
                   to uim_is_get_response_allowed()
08/25/09   yb      Replacing deprecated clock APIs
08/20/09   yb      Removing deprecated BIO APIs
08/10/09   rm      Error handling for T=1 protocol
08/05/09   yb      Fix for setting UIM clock frequency globals to 3.4MHz for
                   MSM6290
08/05/09   yb      Fix to do get response for seek command if a GCF test case
                   is running or NV_ENS_ENABLED_I NV item is enabled.
08/05/09   rm      Add an extra guard time for UART DM
08/03/09   rn      Fixed Linker error with uim_gcd function
07/23/09   mib     Moved include of customer.h before #ifndef
07/20/09   mib     Replaced flag TEST_FRAMEWORK with FEATURE_UIM_TEST_FRAMEWORK
07/07/09   rn      Fixed Lint error
07/01/09   yb      1. Fix to do get response for envelope command when card
                   responds with warnings.
                   2. Fix to report warnings to GSTK on receiving warnings in
                   Envelope response.
06/17/09   rm      Fix for clock power-on timing
06/15/09   rm      Fixes to correct the PULL settings for UIM_DATA and
                   UIM_CLK GPIOs
06/10/09   yb      Fix to send status word as received in get response to AT COP
                   even if card returns error for envelope sent.
06/08/09   yk      1) Fix for UIM2 slot for the targets using UART_DM
                   2) Added SCMM specific changes
                   3) Moved a bit-mask for IPR register to the head files
05/29/09   rm      Fixes to avoid overrun errors due to RX break errors
                   and to set the command response timeout signal appropriately
                   in such cases
05/28/09   sg      Featurized for Test Framework
05/11/09   rm      Fix to correct the feature macro name
05/05/09   ps      Fix high Lint errors
05/04/09   rm      Fix in parity error handling logic for T=1 protocol
05/02/09   ps      Fix 8200 Warnings
04/15/09   ssr     Fix 61XX handling for extra Response Byte for case 2 command.
04/15/09   rm      Fix to add 1ms delay between APDUs
04/14/09   yk      Flushing the FIFO for LCU in case of overrun errors
04/08/09   nb      Fix for multiple select on MF
04/03/09   yk      Bring SCMM drivers
03/30/09   rm      Reset the UART received upon receipt of each RX break error
                   interrupt
03/25/09   rm      Fix for APDU logging leading to a crash
03/19/09   rm      Fix for improper featurization for TD5 macros
03/04/09   rm      Fix for slow initialization of phonebook with UART_DM
03/03/09   nb      Fix to return received warnings when processing Envelope
                   command
02/28/09   ssr     Fixed QTF Lint Errors.
02/25/09   rm      Fix for RX break errors in Idle and Non-Idle
02/20/09   rm      Fix to enable the parity error logic in UART DM which
                   ignores the bytes received with parity / framing error in
                   character mode
02/06/09   ssr     Support for VREG Control with FET
01/29/09   rm      Speed enhancement for slowest bit-rate card for ULC
01/16/09   rm      Fix for update record failure with UART_DM
01/06/09   yk      Bring MSM7X27 drivers to mainline.
12/23/08   rm      Fix for a possible update record failure with UART_DM
11/20/08   rm      Fix for reset issue due to a 2G sim card on LCU
11/19/08   vs      Removed sleep register call and added TLMM config for LCU
11/13/08   rm      Use timer callback for command response timer to handle the
                   work waiting timer expiry immediately in the timer context.
11/11/08   yk      Fixed Lint errors
11/05/08   vs      Update to allow UIM clock stop only after sending the first
                   instruction to the card in passive mode
10/20/08   rm      Fix the problem with NULL Procedure bytes before ACK byte
10/16/08   rm      Correcting RX and TX packet logging for UART DM
10/07/08   vs      Fix to power up UIM LDO after card power down, vote TCXO off
                   permanently when IC-USB interface is selected
09/19/08   tml     Re-select MF to force directory maintainance to MF when
                   received a sw1 warning with normal return
09/23/08   nb      Fixed compiler errors
09/22/08   rm      Fix for the authentication failure issue
09/22/08   rm      Fix for parity errors in UART_DM
09/19/08   rm      Work waiting time fix for UART_DM
09/17/08   js      Implemented UIM Card interface selection based on first
                   instruction class NV
09/12/08   nb      Added support to handle warnings returned by Envelope Command
09/05/08   jk      Fixed Nested comment which caused problems with Strip tool
08/29/08   rm      Fix for divide-by-zero error
08/20/08   ssr     Added support not to do get response for seek on warning
08/15/08   jk      Fix to UART DM featurization
08/08/08   vs      Fixed compile errors related to featurization
08/07/08   rm      Fix for speed improvement.
07/30/08   SSR     Added support not to do get response for seek on warning
07/17/08   vs      Added support for UIM USB UICC ICCD interface
07/03/08   jk      Fix to not allow GPIO 10 to be configured for 1100
06/24/08   rm      Fix to resolve merge problem
06/18/08   jk      Bring QSC11X0 drivers to mainline
06/18/08   rm      Fix to not call pm_vreg_control(PM_OFF_CMD, PM_VREG_RUIM_M)
                   when FEATURE_UIM_USE_ONLY_VREG_ON_PMIC is enabled to set
                   VREG_RUIM to 2.85V. pm_vreg_control(PM_OFF_CMD,
                   PM_VREG_USIM_M) is called via the UIM_POWER_OFF() macro.
06/16/08   rm      Fix to handle Parity errors in Block mode.
06/12/08   rm      Fix to reduce the M/N ratio when D value cannot fit into
                   the DREG and MND registers.
05/23/08   rm      TA1=0x97 feature support
05/02/08   rm      T=1 protocol support for LCU.
04/22/08   rm      Fix for LCU to handle the SIM responses correctly.
                   for all instruction cases.
04/18/08   rm      Fix in SIM powerdown procedure for LCU.
04/01/08   vs      Fixed compiler warning
03/31/08   jar     Fixed compile issue.
03/31/08   rm      Changes to:
                   1. Enable clock stop feature in LCU
                   2. Fix the handling of warning status bytes for LCU
                   3. Correct the UIM_POWER_DOWN macro for LCU
03/30/08   rm      Fix to not call MPP config APIs in the case of 6290
03/21/08   jk      Fixed Compile Warnings
03/13/08   rm/jar  Phase1 LCU UIM bring-up in UART word mode with Data Mover
                   functionality disabled.
                   Pending:
                   1. Enabling clock stop feature
                   2. NULL procedure byte handling for case 2 instructions
                   3. Card boot-up time improvisation
                   4. T=1 protocol support
03/10/08   jk      Added changes to use new SLEEP Voting API
03/05/08   jar     Allow PMIC to put VREG RUIM into LPM MODE when TXCO
                   is off under feature:  FEATURE_UIM_ENABLE_VREG_UIM_LPM
01/08/08   rm      Set UIM clock to 3.84MHz for M6280 under
                   FEATURE_UIM_3P84MHZ_M6280_PLATFORM feature macro
12/27/07   rm      8650 UIM bring up
12/06/07   rm      To fix the RX break error issues due to improper insertion
                   of SIM cards
11/29/07   rm      when no ATR with in 40K + 1 byte time, set the RX_ISR
                   state to "IDLE", reset the UART and mask all the
                   interrupts in the UART IMR register
09/10/07   tml     Add Media Flo support
09/14/07   jk      PMIC HW workaround softare support
07/20/07   jar     Added FEATURE_UIM_DRIVE_MAX_PMIC_VOLTAGE to allow the PMIC
                   to be driven to MAX of 3.05 Volts
06/26/07   jk      Correct uim_clear_timeout_on_response flag to proper name
05/21/07   jk      Reset error counters so that they don't max out again right
                   after reset
05/11/07   wli     Changes for JCDMA requirements
01/19/07   jk      SC-2X Driver bring-up
01/15/07   pv      Consider it an error if there are no more bytes to send
                   and we are processing an ACK byte.
11/30/06   pv      Fix compiler warnings and lint errors for non 7600 builds.
11/27/06   pv      Bring-up for 7600.
09/29/06   sun     Fixed Compile Error for 7600
09/25/06   pv      Lint fixes due to changes in clk_busy_wait signature.
09/13/06   pv      Fix compilation error for 6800 when FEATURE_UIM_MSM6800_TCXO_DIV5 is
                   not defined.
09/08/06   jk      Support for TCXO Div 5 clock on 6800
08/10/06   pv      Use clock regime resource manager when FEATURE_CLKREGIM_RM
                   is defined for 7200 and 7500 targets.
08/09/06   pv      Featurize the usage of members of nv_uim_config_params_type
                   under FEATURE_UIM_NO_ADDITIONAL_CONFIG_PARAMS for
                   targets not picking up the NV dependencies
07/31/06   pv      Do a get response irrespective of the class for CASE 4
                   commands other than for some specific instructions in
                   WIM which could return 6200 indicating warning.
                   no requirement to do a get response.
07/17/06   jk      Support for APDU logging to EFS
07/25/06   pv      Remove dependency on T_MSM6500 for 7200 targets
07/25/06   pv      Featurize API usage for pmic
07/05/06   pv      Use new api for pm_mpp_config_digital_input
06/12/06   pv      Do not leave the receiver disabled because of receiving
                   un-known status words treat it similar to overrun error.
04/24/06   pv      Use uim_config_params.additional_delay_for_atr to wait for
                   additional time after 40,000 UIM clk cycles to receive ATR
04/19/06   pv      7200 GSM bringup changes
04/11/06   pv      Implement function uim_read_status which would do the same
                   operation as the macro UIM_READ_STATUS but would release
                   the dependency on msm.h for the caller.
04/03/06   pv      Lint Fixes
03/29/06   pv      switch the order of uim_send_byte and putting the byte
                   in the diag log packet.  To avoid seeing the sent byte
                   in the same iteration.
                   Do not let a time-out while in idle state.
                   Add more debug variables to keep track of general health.
03/07/06   jk      Bring SC-1X to mainline
02/14/06   nk      Merge For: Fix to ensure we all WWT Timer Expire during RX
                   Break Error and Fake a normal response and do not send the
                   get response down to the card if requested
01/31/06   pv      Do not process rx interrupt when the clock is already
                   turned off.  Since the register reads might return
                   garbage/previous values resulting in an infinite loop
                   in the ISR.
01/18/06   tml     added support for status word 6999
12/14/05   jk      Do not set the state to IDLE after RX BREAK receiver reset
                   to allow timeout
11/23/05   jk      Added missing tlmm init to reset line not present in tlmm
11/22/05   jk      Driver Changes for 7500
11/14/05   pv      Lint Fixes
11/04/05   pv      Featurize code to timeout after 40,000 uim clk cycles
10/27/05   pv      Code to turn off the interface if no ATR is received whthin
                   40,000 UIM clock cycles
09/06/05   jk      Changes to support only allowing reading UART Status while
                   there is still power to UART
08/25/05   pv      Some 3V cards go haywire when supplied 1.8V so, always
                   set the timer if ATR is not received
07/27/05   jk      Made work waiting time array for dual slots
07/18/05   pv      Check to see if the number of bytes indicated in the
                   prologue exceed IFSD - Test case 7.3.5 on IT3 Ver 3.6
07/12/05   jk      Created Clock Regime macros for 6800 R-UIM Bring-up
07/06/05   jk      Added call to sleep_require_5ms_ticks() under
                   TCXO_MUST_BE_ON() macro
06/22/05   pv      Set the transaction timer at the beginning of a UIM
                   transaction and do not clear it untill the entire
                   transaction is done.
06/14/05   pv      Changed UIM_TRANSACTION_TIME_DURATION to 20 seconds to work with
                   cards which are taking 11.5 seconds to respond to the 5th
                   part of a multi-part SMS.
06/02/05   pv      Code for FEATURE_UIM_ISIM & FEATURE_UIM_SESSION_MANAGEMENT
05/13/05   jk      Call sleep_relinquish_5ms_rex_ticks in UIM_TCXO_CAN_BE_OFF
05/10/05   sst     Lint fixes.
04/08/05   jk      Added changes for Raven T=1 Block mode support
04/03/05   pv      Changes to set the stop bit configuration for Ravem
03/18/05   pv      Changes for Jaguar dual-slot multiplevoltage support
03/18/05   pv      Clear the work waiting timer at power down.
02/23/05   pv      Set the timer just after doing a reset receiver because of
                   an rx-break error. Fix compile error when T=1 is turned off
                   for Raven.
02/02/04   pv      Do the max parity error checking only for T=0 protocol
01/26/05   pv      Removed uart_reset_receiver.  This was causing infinite
                   parity errors.  Instead just reset the error status.
01/27/05   jar     Fixed runtime bug due to missing break in switch-case
01/12/05    jk     Added code to process Response properly for Global Platform/WIM
                   cards
12/28/04   pv      Added code to take time stamps and signal a timeout in order
                   to pass the USIM 5 etu WWT exceeded test case
12/27/04   jk      Fixed Compiler warnings.
12/10/04   pv      Cr.Fix 53534.  We need to send a get response inspite of
                   receiving warning message for case 4 commands.
12/08/04   pv      Raven merged to main-line
12/06/04   pv      Externalised variables required for logging timeout info
11/23/04   pv      Code to select appropriate clock source for 6550 based on
                   MSM revision (To avoid data aborts when
                   FEATURE_UIM_GSM_CLK_SRC is selected for 6550 MSM 1.1)
11/21/04   wli     Dual slot merge and debug.
11/15/04   pv      Added definition for UART_TFWR_TX_RX_MODE_MASK to be used to
                   set the UART to block mode for T=1 protocol parity error
                   handling
10/29/04  pv/jar   Removed calling pm_vreg_pull_down_switch(ON) from u
                   uim_dev_init().
09/20/04   pv      Added code to disable the interrupts after maxing errors
                   in order to avoid race conditions
09/01/04   ck      Added support for CSIM.
08/31/04   ck      Fixed a bug where the last sent APDU was re-sent internally
                   by driver with wrong instrn case that resulted in timeout.
08/18/04   pv      Changed uim_clk_source_freq to 26000000 since we are dividing
                   by 8 and the uim clock frequency is 3.25 MHZ
08/12/04   ck      Added delay before sending the command header to the card
                   in the function uim_send_command().
08/10/04   pv      Removed code which re-writes to the waterlevel and
                   added more messages
07/19/04   pv      Moved some definitions to driver header file
06/09/04   ts      Clear the timeout signal when processing a character in
                   case ISR has not run since timeout.
06/03/04   ts      Added a call to configure the UIM GPIOs during
                   initialization for Saber.
05/11/04   ts      Pull down Vreg R-UIM and wait 2ms when powering down.
05/11/04   ts      Added support for controlling UIM power through PMIC Vreg.
05/11/04   ts      Added support for direct connect on Saber.
04/16/04   ts      Fixed problems with error condition processing due to bad
                   merge.
03/26/04   ts      Added a timer to determine when excessive null procedure
                   bytes occur.
                   Added processing to detect excessive receive errors and
                   disable the receiver so the task may bring down the
                   interface.
03/15/04   ts      Fixed rx_value parameter in uim_process_overrun.
03/10/04   wli     Change MSM6500 clock source to 13Mhz.
02/20/04   ts      Fixed lint errors
02/03/04   ts      Added support for PMIC on UIM2.
01/23/04   ts      Changed the voltage reference on the UIM side of the
                   interface depending upon interface voltage.  Added a 2msec
                   delay after powering down the UIM interface because of the
                   amount of time it takes the PMIC Vreg RUIM to power off.
01/13/04   ts      Added support for the PMIC on the UIM interface.  Also
                   included multi-voltage support when using the PMIC.
01/12/04   ck      Fixed compile errors that showed up when FEATURE_UIM_UTIL
                   was defined with FEATURE_UIM_UICC
12/18/03   ck      Merged in the Real fix to use the correct instruction class
                   for GET RESPONSE when done in non-default logical channel.
10/31/03   ts      Added support for file accesses greater than the maximum
                   transaction size.
10/23/03   ck      Merged in changes to support 18etus between reception of a
                   byte and transmission of byte.
10/17/03   ck      Added UART_TFWR_TFW_MASK to Saber definitions.
10/10/03   ts      Removed specializations includes for GPIO and TLMM.
                   Added support for PMIC on UIM1.
                   Added UART MND definitions for MSM6500.
                   Added control for block mode in TFWR.
10/02/03   ts      Fixed the uim_drv_slot parameter name in
                   uim_update_op_params.  This fixes the macros use of the
                   selected slot for TCXO shutdown.
09/29/03   ck      Added support to process 67xx which is not clearly specified
                   in ETSI specifications.
                   Saved the protocol in the cmd buffer when the APDU is built by
                   the driver.
08/27/03   ck      Fixed a compilation issue with uim_clock_control_val
08/21/03   ts      Added a delay in responding to the procedure byte to fix
                   parity errors.
08/21/03   ck      Merged in support for Saber and TCXO/5 for USIM clock.
08/21/03   ts      New fix for response time issue that caused parity errors.
08/15/03   ck      Removed an erroneous paranthesis and featurized uim_rx_isr1
                   and uim_rx_isr2 around the right features.
08/14/03   ck      Merged in the support for dual slots.
07/22/03   ts      Added a table that indicates the maximum BRAF for the CRCF.
07/15/03   ts      Removed target specific include of clock regime definition
                   file.
06/19/03   ck      Added support to flush the bytes upon receiving an
                   overrun error without resetting the card.
06/12/03   ck      Added the check for UIM_UICC in addition to UIM_WCDMA to
                   arrive at the UIM instruction class.
06/03/03   ck      Changed the status from UIM_PROBLEM to UIM_CONVENTION_ERROR
                   when the error is because of change in the convention.
04/29/03   wli     Added missing "\" to end of #define ENABLE_UART_INTERFACE()
04/28/03   ts      Added support for multi-Voltage classes.
04/28/03   ts      Added support for T=1 protocol.
04/25/03   ts      Added support for GSM clock source on MSM6100.
04/25/03   ts      Enable UIM data and clk for MSM6100.
02/26/03   jk      Added UIM UTIL and WIM functionality
03/10/03   djd     Added support for UIM2.
02/24/03   ts      Reset the UART receiver after stopping break.  This keeps
                   the receiver from detecting a character after releasing
                   the break condition.
02/19/03   ts      Fixed lint issues with kjm changes and UIM logging.
02/13/03   ck      Merged in the changes from kjm to improve the stability of
                   the driver.
01/21/03   ts      Removed #define FEATURE_UIM_USE_13MHZ_GSM_CLOCK.
01/20/03   ts      Added run-time support of 13MHz GSM clock.
12/20/02   ts      Fixed Lint issues
12/04/02   ts      Need the clocks to power down the UIM in the case the UIM
                   has already powered down or the clocks are turned off -
                   like when receiving a task stop.
11/26/02   ts      Moved tcxo frequency determination to uimdrv.h for use by
                   uimgen.c
                   Start with maximum frequency allowed when powering up.
11/21/02   ts      Give access to braf_values table for use in determining
                   invalid TA1 values.
10/31/02   ts      Brought the 6100 code into the mainline.
10/24/02   ts      Added support for work waiting time
10/24/02   ts      Added support for re-transmission due to parity errors.
10/24/02   ts      Changed the clock control to start out with 50% duty cycle.
10/04/02   ts      Commented the code that turned off the clock before turning
                   it back on.  This does not work with the cards.
10/02/02   ts      Added a new feature switch that disables converting the
                   transmitted byte to inverse convention.  This is needed to
                   work with SIM glue logic that changes both the transmit
                   and receive bytes with the SIM configuration control.
10/02/02   ts      Throw away retransmitted bytes received from the card.  This
                   is indicated by the parity error status.
10/02/02   ts      Changed MSM_INH to MSM_IN since the read is masked already by
                   MSM_IN.
09/20/02   ts      Reset the receiver when a receive overrun occurs.
09/20/02   ts      Use the last stop clock value to stop the clock when changing
                   the frequency.
09/20/02   ts      Turn off UIM clock and set clock select before turning UIM
                   clock back on.
                   Use shadow register when changing MISC_CTL_0.
                   Put time delays between turning off the UIM interface lines
                   when powering down.
09/16/02   ts      Added control to the stop bit length in the SIM configuration
                   register for the 6100.
09/12/02   ts      Removed enabling UART1_INT_ENABLE_UART_DP_RX_DATA_ENA for the
                   Cheetah. This conflicts with using GPIO55 and is not needed by
                   the UIM server.
08/07/02   wli     Added UIM UART clock regime control.
07/28/02   sb      Merged changes from rev1.22.1.2 by pmarsh & mainline CHEETAH_GSM.
05/28/02   plm     Added reference to T_UIM_NO_RX_WATERLEVEL to disable programming
                   of rx level for some targets when UIM_NO_SIM_GLUE_LOGIC undefined.
05/22/02   plm     Replaced references to T_KOVU, T_MSM5200 and T_MSM6200 with
                   T_UIM_USE_BSP_DEFINES, T_UIM_UART1_USIM_CFG, T_UIM_SIM_GLUE_LOGIC
                   and T_UIM_UART2_NON_SPLIT_MND_REGS target symbols to be defined
                   in targetXXX.h file for GSM and WCDMA.
07/18/02   wli     Put #include "log.h" back to this file. Removed diagbuf_init().
07/09/02   ts      Ported driver to MSM6100.
07/09/02   ck      Added the feature FEATURE_SLEEP_TASK to the featurization
                   around sleep.h
06/10/02   ts      Fixed the MND values to ensure a 50% duty cycle with the
                   UART clock.
05/21/02   ts      Added support for 256 bytes transfers from the UIM card.
05/09/02   ts      Needed to include 6200 in the definitions of the USIM
                   configuration register with 5200 and Kovu.
05/02/02   ts      Moved the UIM pad enable to the end of uim_dev_init.  Even
                   though the code uses MSM_OUT, it should use MSM_OUTSH.  The
                   rest of the code that accesses MISC_CTL_0 should also use
                   MSM_OUTSH and the shadow register needs to be defined.
04/03/02   AT      Added support for CHEETAH_GSM
03/18/02   ts      Moved the MSMU_OUT and MSMUW_OUT from uimdrv.h to this file
                   to avoid multi-definition problems with these macros.
02/06/02   ts      Programmed the clock source for the UART to be tcxo/4 for Leo.
                   Use a different feature switch to configure which clock source
                   the UIM UART uses.
02/29/02   ts      Changed the constant name in programming inverse convention
                   for Leo and PPG.  The constant (UART_SIM_CFG__SIM_CLK_OFF)
                   is used because it is defined for both targets.
02/08/02   ts      Added support for Leo (MSM6050).  Cleaned up the UART
                   configuration register access for SIM Glue logic chips.
02/07/02   wli     Added inverse convetion support for non sim glue logic targets.
01/25/02   ts      Added support for Kovu project.  Kovu is like 5200 with a few
                   changes.
01/23/02   ts      Added support to turn the UIM clock off at a high level and at
                   a low level through uim_update_op_params.
01/11/02   ts      Need to select UART2 as an alternate function when using GPIO
                   lines for UIM.
01/10/02   ts      Defined symbols to use proper GPIO lines for UIM power and UIM reset
                   control for Panther SURF.
12/20/01   ts      Set rx watermark level to 0 when sending bytes while processing
                   procedure bytes.  This keeps the driver from sending a tx
                   byte too soon after receiving a procedure byte.
12/13/01   ts      Fixed alignment of code and comments.
12/04/01   ts      Added support for UIM application toolkit and toolkit
                   download error.
10/26/01   ts      Stopped processing the NULL procedure byte.
10/18/01   ck      Added featurization around bsp header file as 3000 and 3100
                   targets do not use it.
10/10/01   ts      Changed the MSM5200 macro definition for accessing the UART
                   configuration register and the UART1 command register.  Also,
                   featurized (out) the watermaark level while waiting for transmitted
                   bytes for the MSM5200.  It causes the 5200 to lose a received
                   byte.
09/13/01   ts      Added support for inverse convention of UIM cards.  Featurized
                   under FEATURE_UIM_SUPPORT_INVERSE_CONVENTION.
08/10/01   ck      Removed  FEATURE that was prepended to UIM_NO_SIM_GLUE_LOGIC
                   as it is an internally defined feature.
07/27/01   ts      Consolidated the register and constant differences between
                   the targets.  Changed the UIM clock control to start with
                   tcxo/8 and switch to tcxo/4 if UIM supports higher
                   frequency.
07/26/01   wli     Added a time stamp for debug log.
06/26/01   ts      Updated the feature switches to have "FEATURE_" prepended.
                   Selected GPIO lines for reset and power control based on
                   SURF/FFA hardware feature switches.  Added 5200 driver
                   changes.  Changed clock control names and frequencies based
                   on tcxo relationship.
06/26/01   ts      Added checks for parity errors for targets that have SIM glue
                   logic to throw away received bytes with parity errors.
06/04/01   ts      Fixed the clock frequency value used to determine the
                   number of clock cycles for clk_busy_wait.
05/22/01   ck      Merged in support for logging the UIM transactions.
05/18/01   ck      Added support for USIM.
01/15/01   ck      Created file from R-UIM driver version 1.1.
===========================================================================*/

/*===========================================================================

                              INCLUDE FILES

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"              /* Customer Specific Definitions        */

/*---------------------------------------------------------------------------
 The includes below are put outside all compiler switches to avoid
 compilation warnings.
---------------------------------------------------------------------------*/

#include "time_svc.h"
#include "target.h"
#include "uimgen.h"
#include "uimdrv.h"
#include "uimdrv_main.h"
#include "uimdrv_uartdm.h"
#include "uimdrv_gpio.h"
#include "uimdrv_clk.h"

#include "DALSys.h"
#include "assert.h"                /* ASSERT utilities                     */

#include "uimdrv_msg.h"
#include "uimi.h"                  /* UIM Internal header file             */
#include "uimutil.h"

#include "mmgsdilib_p.h"

#include "uimglobals.h"
#include "uimsub_manager.h"
#include "uim_nvcfg.h"
#include "uim_logging.h"
#include "uimdrv_hal.h"
#include "uimdrv_hal_iso.h"
#include "uim_remote.h"

/*===========================================================================
                DEFINITIONS AND DECLARATIONS FOR MODULE
===========================================================================*/

/* Buffer to hold ICCID used in GCF test cases */
static byte const UIM_TEST_ICCID[UIM_ICCID_SIZE] = {0x98, 0x94, 0x00, 0x20, 0x20, 0x41, 0x00, 0x00, 0x40, 0xF5 };
/*===========================================================================
  I N T E R N A L  F U N C T I O N S  P R O T O T Y P E S
===========================================================================*/
void uim_initialize_state
(
  uim_instance_global_type *uim_ptr
);
void uim_register_for_sleep_voting
(
  uim_instance_global_type *uim_ptr
);
/*===========================================================================

FUNCTION UIM_INITIALIZE_STATE                         INTERNAL FUNCTION

DESCRIPTION
  Initialize state variables for the R-UIM device driver.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_initialize_state
(
  uim_instance_global_type *uim_ptr
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize the uim Rx character ISR processing state. */
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_PROCESS_IDLE;

  if (uim_ptr->hardware.uim_ldo.customize_ldo_setting &&
      uim_ptr->hardware.uim_ldo.desired_voltage_class != UIM_VOLTAGE_CLASS_INVALID)
  {
    /* Set the UIM interface voltage to desired voltage class */
    uim_ptr->state.current_voltage_class = uim_ptr->hardware.uim_ldo.desired_voltage_class;
    uim_ptr->state.max_possible_voltage_class = uim_ptr->hardware.uim_ldo.desired_voltage_class;
  }
  else
  {
    /* Set the UIM interface voltage to the minimum voltage class */
    uim_ptr->state.current_voltage_class = UIM_VOLTAGE_CLASS_C;
    uim_ptr->state.max_possible_voltage_class = UIM_VOLTAGE_CLASS_B;
  }
} /* uim_initialize_state */


/*===========================================================================

FUNCTION UIM_DEV_INIT                             EXTERNALIZED FUNCTION

DESCRIPTION
  Initialize the clock regime, configures the GPIOs, MSM UART serial
  interface.  It places the UIM power line low, the UIM reset line low,
  the I/O line low and the clock line low.

DEPENDENCIES
  The routine MUST be called before any other UIM driver function.  It must
  only be called once on power up of the phone.

RETURN VALUE
  None

SIDE EFFECTS
  This function leaves ALL the UIM lines low after it executes.

===========================================================================*/
void uim_dev_init
(
  uim_instance_global_type *uim_ptr
)
{
#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
  #error code not present
#endif
#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
  UIMDRV_MSG_LOW_0(uim_ptr->id, "uim_dev_init" );

  if (uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_INTERFACE_NOT_USED,
                               uim_ptr) == TRUE)
  {
    /* Just return without initializing the UART, PMIC etc */
    return;
  }

  uim_ptr->setting.phy_layer = UIM_ISO;
#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  /*
     1. uimdrv_hw_nv_configuration() is invoked to read nv
        UIM_HW_CONFIG_NV_EF that contains information used to configure
        UIMDRV HW interfaces.
     2. It is instance independant as all instances run from information
        got from nv UIM_HW_CONFIG_NV_EF
     3. The NV is read only once by either of the following depending on who
        calls uimdrv_hw_nv_configuration() first,
        a. Either the first uim thread to reach dev init and in turn invoke
           uimdrv_hw_nv_configuration().
        b. Else by any external client who calls the UIM exposed api to get
           the UIM slot information. The exposed api is uim_get_hw_slot_info()
           which in turn invokes uimdrv_hw_nv_configuration().
   */

  uimdrv_hw_nv_configuration();

  if(m_HWConfig.uim_slot_config[uim_ptr->id].disableUim == TRUE)
  {
    return;
  }
  /* Call to initialize the HW classes */
  memset (&uim_hw_if.uart[uim_ptr->id], 0x00, sizeof(uartdm_interface));
  memset (&uim_hw_if.clkreg[uim_ptr->id], 0x00, sizeof(clkregime_interface));
  memset (&uim_hw_if.pmic[uim_ptr->id], 0x00, sizeof(pmic_interface));
  memset (&uim_hw_if.gpio[uim_ptr->id], 0x00, sizeof(gpio_interface));
  memset (&uim_hw_if.intctrl[uim_ptr->id], 0x00, sizeof(intctrl_interface));

#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
  #error code not present
#endif  
  initialize_hw_classes(uim_ptr);

  /* This function is invoked to create the core cpu handle. */
  uim_register_for_sleep_voting(uim_ptr);

  /*-------------------------------------------------------------------------
   Do device specific initialization of state variables for device indicated
   by port_id.
  -------------------------------------------------------------------------*/
  uim_initialize_state(uim_ptr);
  /*-------------------------------------------------------------------------
   Turn on the clock for the R-UIM interface
  -------------------------------------------------------------------------*/
  UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

  if(uim_hw_if.uart[uim_ptr->id].uart_version == 0)
  {
    /* Read UART_DM_HW_VERSION */
    uim_hw_if.uart[uim_ptr->id].uart_version = UIM_HWIO_IN(uim_hw_if.uart[uim_ptr->id].start_addr
                                                           + UART_DM_HW_VERSION);
  }

  /* Once the HW feature is mainlined, BUA and MPM registration check needs to be removed */
  if(TRUE == uim_check_bua_mpm_irq_reg_applicable() &&
     ((TRUE == uim_hw_if.pmic[uim_ptr->id].m_uiccShutCardRemoval &&
       TRUE == uim_hw_if.intctrl[uim_ptr->id].m_hs_enable) ||
       TRUE == uim_hw_if.pmic[uim_ptr->id].m_uiccShutBattRemoval))
  {
    /* After the clocks are enabled we configure the BATT ALARM functionality only
       if uicc shutdown upon card removal and hotswap is enabled, or uicc shutdown
       due to battery removal is enabled.
     */
    UIM_CONFIGURE_BATT_ALARM_FOR_UICC_SHUTDOWN(uim_ptr);
  }

  /*-------------------------------------------------------------------------
   Set up Clock Regime Management.  UART clock regime controls both UARTs.
  -------------------------------------------------------------------------*/
  UIM_CLOCK_REGIME_ENABLE_SLOT(uim_ptr->id);

  /*-----------------------------------------------------------------------
   Enable UART to drive the interface lines.
  -----------------------------------------------------------------------*/
  ENABLE_UART_INTERFACE_SLOT(uim_ptr);

  /*-------------------------------------------------------------------------
   Select UIM configuration on the UART.  Start with direct convention.
  -------------------------------------------------------------------------*/
  if( uim_nv_is_feature_enabled(UIMDRV_FEATURE_ENABLE_BOOT_UP_IN_INVERSE_CONVENTION, uim_ptr) == TRUE )
  {
    uim_ptr->state.convention = UIM_INVERSE_CONVENTION;
  }
  else
  {
    uim_ptr->state.convention = UIM_DIRECT_CONVENTION;
  }

  /* Backwards compatibility configuration based on UARTDM version */
  UIM_SET_UARTDM_BCR_REG_PER_VER(uim_ptr);
  if (UIM_UART_DM_HW_VERSION_1_4_2 <= uim_hw_if.uart[uim_ptr->id].uart_version)
  {
    /* Disable CR protection */
    UIM_DISABLE_CR_PROTECTION_SLOT(uim_ptr);
  }

  /* Initialize all bits in USIM_CFG register to zero.  Turn off UIM clock and
     keep UIM clock signal low */
  UIM_UART_CNFG_SLOT(uim_ptr,
                     UART_SIM_CFG__CLK_MASK |
                     UART_SIM_CFG__MASK,
                     UART_SIM_CFG__SIM_CLK_OFF );
  /* Set USIM_SEL to turn on USIM mode of operation */
  UIM_UART_CNFG_SLOT(uim_ptr,
                      UART_SIM_CFG__CLK_MASK |
                      UART_SIM_CFG__MASK,
                      UART_SIM_CFG__SIM_CLK_OFF |
                      UART_SIM_CFG__SIM_SEL_MASK );
  /* Set the stop bit number to default (2) */
  UIM_UART_CNFG_SLOT(uim_ptr,
                      UART_SIM_CFG__STOP_BIT_LEN_MASK,
                      UIM_DEFAULT_STOP_BIT_VALUE);

  /*-------------------------------------------------------------------------
   Disable IRDA tranceiver.  This is not used for the UIM.
  -------------------------------------------------------------------------*/
  UIM_DISABLE_IRDA_SLOT (uim_ptr);

  /* Disable the DM functionality */
  UIM_DISABLE_DATA_MOVER_SLOT(uim_ptr);

  /* Program BADR to default */
  UIM_PRG_UART_BADR_SLOT(uim_ptr, UIM_DEFAULT_BADR_VAL);

  /* Set the UART clock to the default values. */
  SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1);

  /* Mask all interrupts from UART. */
  UIM_PRG_IMR_SLOT (uim_ptr, MSMU_IMR_MASK_ALL );

  /* Set the Stale character timeout to maximum possible */
  UIM_PRG_IPR_SLOT(uim_ptr, MSMU_IPR_DEF_STALE_TIMEOUT);

  /*-------------------------------------------------------------------------
   Set the transmit FIFO watermark level.  This is used when sending out block
   transfers.
  -------------------------------------------------------------------------*/
  UIM_TX_WATERLEVEL_SLOT(uim_ptr, 30 );

  /*-------------------------------------------------------------------------
   Disable the RFR and CTS hardware control.
  -------------------------------------------------------------------------*/
  UIM_PRG_MR1_SLOT (uim_ptr, MSMU_MR1_DISABLE_RFR_CTS );

  /*-------------------------------------------------------------------------
   Program for eight bits per character, 2 stop bit and character check error
   mode
  -------------------------------------------------------------------------*/
  /* Enable block mode for first time */
  UIM_PRG_MR2_SLOT(uim_ptr, (MSMU_MR2_8BPC | MSMU_MR2_2SB | MSMU_MR2_EPAR| MSMU_MR2_ERRMODE_BLOCK | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF));

  /*-------------------------------------------------------------------------
   Set the UIM clock signal to the LOW state. (keeping it same for now)
  -------------------------------------------------------------------------*/
  uim_clock_control (uim_ptr->id, UIMDRV_CLK_STOP_LOW);

  /*-------------------------------------------------------------------------
   First, set the GPIO pins used for reset and power enable to a low state.
  -------------------------------------------------------------------------*/
  UIM_SET_RESET_AS_OUTPUT_SLOT(uim_ptr);

  UIM_ASSERT_RESET_SLOT(uim_ptr);

  /*-----------------------------------------------------------------------
   Enable the UART transmitter
  -----------------------------------------------------------------------*/
  UIM_ENABLE_UART_TX_SLOT(uim_ptr);

  /*-------------------------------------------------------------------------
   Allow the clock for the R-UIM interface to be turned off
  -------------------------------------------------------------------------*/
  UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT(uim_ptr);

} /* uim_dev_init */


/*===========================================================================

FUNCTION UIM_POWER_UP                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function turns on the power for the UIM.  It puts the I/O line in
  reception mode (stops sending break on the I/O line) and turns on the
  UIM clock.

DEPENDENCIES
  This function must be executed either after uim_dex_init or after
  uim_power_down.

RETURN VALUE
  None

SIDE EFFECTS
  After this function is executed, the UIM is powered up and has a clock.

===========================================================================*/
void uim_power_up
(
  uim_instance_global_type *uim_ptr
)
{
  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  if((0x00 == uimdrv_hw_nv_config_info.number_of_active_interfaces) ||
     (TRUE == uim_nv_is_feature_enabled(UIMDRV_FEATURE_INTERFACE_NOT_USED,
                                     uim_ptr)))
  {
    /* Send error signal to MMGSDI */
    (void)uim_sm_mmgsdi_notify_card_error( UIM_ERR_S, uim_ptr, FALSE);
    /* Set the card status to "error" */
    uim_ptr->state.status = UIM_ERR_S;
    return;
  }

  uimdrv_hal_power_up(uim_ptr);
} /* uim_power_up */

/*===========================================================================

FUNCTION UIM_RESET                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function asserts the reset line on the UIM.  Any data received by the
  phone will be flushed.  Any transfer from the phone will be terminated and
  the buffers flushed.

DEPENDENCIES
  This function must be executed after uim_dex_init.  The UIM should be
  powered up (via uim_power_up) for this function to have any effect.

RETURN VALUE
  None

SIDE EFFECTS
  The UIM driver is expecting an ATR after this reset.

===========================================================================*/
void uim_reset
(
  uim_rsp_buf_type         *rsp_ptr,    /* Defines where to put the ATR */
  uim_instance_global_type *uim_ptr
)
{
  if (NULL == rsp_ptr || NULL == uim_ptr)
  {
    return;
  }

  uimdrv_hal_reset(rsp_ptr, uim_ptr);
} /* uim_reset */


/*===========================================================================

FUNCTION UIM_UPDATE_OP_PARAMS                         EXTERNALIZED FUNCTION

DESCRIPTION
  This function sets up the operational parameters of the UIM interface.  It
  can change the baud rate of the UIM interface, change the UIM clock
  frequency or change the guardtime.

DEPENDENCIES
  This function changes the operational parameters of the UIM interface.
  Whatever changes are requested in this function shall apply as soon as
  possible.  The interface will reflect the changes.

RETURN VALUE
  Whether CLK stop is performed successfully. This value will be used in the
  caller to change CLK state.

SIDE EFFECTS
  All transfers over the UIM interface will occur at the frequency, baudrate,
  or guardtime as specified in this function call.

===========================================================================*/
boolean uim_update_op_params
(
  uim_op_params_type const *op_params,       /* Proposed operational parameters */
  uim_instance_global_type *uim_ptr
)
{
  if (NULL == op_params  || NULL == uim_ptr)
  {
    return FALSE;
  }

  return uimdrv_hal_update_op_params(op_params, uim_ptr);
} /* uim_update_op_params */


/**
 *  FUNCTION UIMDRV_SET_CARD_CMD_DATA
 *  This function sets up the card command data to the UIM.
 *  
 *  Side effect: If the command was successful, reponse buffer
 *  in UIM global is updated.
 *  
 *  @param uim_req[IN] Pointer to command request buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */

void uimdrv_set_card_cmd_data
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
)
{

  uim_ptr->card_cmd.parity_error_count = 0;
  uim_ptr->card_cmd.overrun_error_count = 0;

    /* Save the protocol of the command to use the correct instruction class
         in a GET RESPONSE or FETCH command */
  uim_ptr->card_cmd.uimdrv_protocol = uim_req->protocol;
    
  /* Initialize the command data and the response pointers */
  uim_ptr->card_cmd.apdu_command_data_ptr = (byte *)&uim_req->req;
  uim_ptr->card_cmd.resp_buf_ptr = uim_req->rsp_ptr;
  uim_ptr->card_cmd.resp_buf_byte_ptr = (byte *)&uim_req->rsp_ptr->rsp;
  uim_ptr->card_cmd.num_resp_bytes_rcvd_total = 0;
    
  /* Reset the warning status indicators */
  uim_ptr->card_cmd.warning1_indicator = FALSE;
  uim_ptr->card_cmd.warning2_indicator = FALSE;
    
  /* Zero the command response size */
  uim_ptr->card_cmd.resp_buf_ptr->cmd_rsp_size = 0;
    
  /* Wait long enough before sending the header */
  uim_clk_busy_wait( uim_ptr->setting.response_byte_delay );
    
  /* Initialize the RX sub-state for T-0 protocol */
  uim_ptr->rxtx_state_machine.rx_sub_state = UIM_RX_PROCESS_IDLE;
  /*
   * If this is the new command, reset the variables to their default values.
   * Else we are retrying the select command, increase the select count and
   * set the warning flag to FALSE.
   */
  if(uim_ptr->flag.sw1_warnings1_normal_end_occured)
   {
     uim_ptr->state.mf_reselect_count++;  
   }
  else
   {
     uim_ptr->state.mf_reselect_count = 1;
   }
    
   uim_ptr->flag.sw1_warnings1_normal_end_occured = FALSE;

}/* uimdrv_set_card_cmd_data */


/*===========================================================================

FUNCTION UIM_SEND_COMMAND                          EXTERNALIZED FUNCTION

DESCRIPTION
  This function sets up the transfer of an APDU to the UIM.  This function
  starts the transfer, which, if all goes well, is finished by the
  uim_rx_isr.

DEPENDENCIES
  This function starts an APDU exchange.  The exchange can only happen after
  the ATR has been received and, optionally, after a PPS exchange.

RETURN VALUE
  None

SIDE EFFECTS
  The UIM driver will continue to send out the APDU.  All the stages of
  sending the APDU are handled by the UIM driver.  Then the driver expects
  the APDU response.  Some commands solicit a GET_RESPONSE or a FETCH command
  to be sent after the APDU.

===========================================================================*/
void uim_send_command
(
  uim_req_buf_type         *uim_req, /* This points to the APDU command buffer, the
                                        command data buffer, the response buffer,
                                        the APDU status and the callback function */
  uim_instance_global_type *uim_ptr
)
{
  if (uim_req == NULL  || uim_ptr == NULL)
  {
    return;
  }

  uimdrv_hal_send_command(uim_req, uim_ptr);
} /* end - uim_send_command */

/*===========================================================================

FUNCTION UIM_SEND_INTERMEDIATE_APDU                          EXTERNALIZED FUNCTION

DESCRIPTION
  This function sets up the transfer of an APDU to the UIM.  This function
  starts the transfer, which, if all goes well, is finished by the
  uim_rx_isr.

DEPENDENCIES
  This function starts an APDU exchange.  The exchange can only happen after
  the ATR has been received and, optionally, after a PPS exchange.

RETURN VALUE
  None

SIDE EFFECTS
  The UIM driver will continue to send out the APDU.  All the stages of
  sending the APDU are handled by the UIM driver.  Then the driver expects
  the APDU response.  Some commands solicit a GET_RESPONSE or a FETCH command
  to be sent after the APDU.

===========================================================================*/
void uim_send_intermediate_apdu
(
  uim_req_buf_type         *uim_req, /* This points to the APDU command buffer, the
                                        command data buffer, the response buffer,
                                        the APDU status and the callback function */
  uim_instance_global_type *uim_ptr
)
{
  if (uim_req == NULL  || uim_ptr == NULL)
  {
    return;
  }
  if(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
  {
    uim_ptr->remote.intermediate_done  = TRUE;
  }
  uimdrv_hal_send_intermediate_apdu(uim_req, uim_ptr);
} /* end - uim_send_command */


/*===========================================================================

FUNCTION UIM_POWER_DOWN                          EXTERNALIZED FUNCTION

DESCRIPTION
  This function deactivates the UIM so it does becomes inactive and does not
  use power.  The electrical lines to the UIM are all set low in the
  appropriate order.

DEPENDENCIES
  This function inactivates the UIM.  The uim_power_up and uim_reset
  functions must be called after this function to bring the UIM back up
  to use.

RETURN VALUE
  None

SIDE EFFECTS
  After this function is executed, the UIM is power down and inactive.

===========================================================================*/
void uim_power_down
(
  uim_instance_global_type *uim_ptr
)
{
  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);
  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_INTERFACE_NOT_USED,
                               uim_ptr) == TRUE)
  {
    /* when the interface is not used, the 'uim_hw_if' has all ZEROes which are accessed
       below in this function. Since uim_power_up is also blocked, not required
       to power down */
    return;
  }

  /* Clear busy and teransaction timers if they are active */
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_POWER_DOWN);
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_POWER_DOWN);

  uimdrv_hal_power_down(uim_ptr);
} /* uim_power_down */


/*===========================================================================

FUNCTION UIM_SWT_CMD_RSP_TIMER_EXPIRY_CB

DESCRIPTION
  This procedure is registered as a call-back associated with the expiry of
  the command response timer. This call-back is called in the context of the
  high priority timer task as soon as the rex timer for the work waiting time
  expires.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Power down of the SIM interface if the command response timeout expired.

===========================================================================*/
void uim_swt_cmd_rsp_timer_expiry_cb (unsigned long instance_ptr)
{
  uim_instance_global_type *uim_ptr = (uim_instance_global_type*)instance_ptr;
  if(uim_ptr == NULL)
  {
    return;
  }

  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_53,uim_ptr->id,"In Software work waiting timer expiry cb");

  UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.clk_ctl, uim_ptr);
  if (FALSE == uim_ptr->state.clock_is_on)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"UIM clock is off, returning from uim_swt_cmd_rsp_timer_expiry_cb");
    UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.clk_ctl, uim_ptr);
    return;
  }
     /* Disable WWT counter */
  uim_uartdm_disable_wwt_counter(uim_ptr);
     /* clear WWT interrupt status */
  uim_uartdm_clear_wwt_int_status(uim_ptr);
  UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.clk_ctl, uim_ptr);

  uim_cmd_rsp_timer_expiry_cb( (unsigned long) uim_ptr);
} /* uim_swt_cmd_rsp_timer_expiry_cb */

/*===========================================================================

FUNCTION UIM_CMD_RSP_TIMER_EXPIRY_CB

DESCRIPTION
  This procedure is registered as a call-back associated with the expiry of
  the command response timer. This call-back is called in the context of the
  high priority timer task as soon as the rex timer for the work waiting time
  expires.

  The logic in this call-back is as follows:
    - (for LCU) check if the count in the rx fifo + the packing buffer increased
        from the last check.
          - If the count increased, then re-start the wwt timer.
          - else, put driver’s uim_rx_state to IDLE, mask all interrupts, reset
            uart, set cmd_rsp_timeout signal and powerdown the sim interface

    - (for non-LCU) check if RXRDY bit is set in the status. If set,
        set cmd_rsp_timeout and return. If not set, put driver’s uim-rx-state to
        idle, mask all interrupts, reset uart, set cmd_rsp_timeout signal and
        power down the sim interface.

  NOTE: The powerdown sequence of the UIM (as in the uim_power_down() function)
        is not started within this call-back as it is a huge-piece of logic
        which might hog the CPU as this call-back is being executed in the
        context of the timer task. Hence, this call-back only signals the
        UIM task to initiate the powerdown sequence, if needed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Power down of the SIM interface if the command response timeout expired.

===========================================================================*/
void uim_cmd_rsp_timer_expiry_cb (unsigned long instance_ptr)
{
  uim_instance_global_type *uim_ptr = (uim_instance_global_type*)instance_ptr;

  ASSERT(uim_ptr);

  if (FALSE == uim_ptr->state.clock_is_on)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"UIM clock is off, returning from uim_cmd_rsp_timer_expiry_cb");
    return;
  }

  /* Ignore the WWT expiry if there are any TX bytes
   * waiting to be sent
   */
  if( uim_bytes_waiting_in_txfifo(uim_ptr) != 0)
  {
    return;
  }
  if (0 != uim_bytes_waiting_in_rxfifo(uim_ptr))
  {
    uim_ptr->card_cmd.uart_dm_interrupt_wait_count++;
    UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_54,uim_ptr->id,"uim_bytes_waiting_in_rxfifo 0x%x  wait cnt 0x%x",
                 uim_bytes_waiting_in_rxfifo(uim_ptr),
                      uim_ptr->card_cmd.uart_dm_interrupt_wait_count);
  }

  /* In UART_DM mode, when work waiting timer (WWT) expires, check if the RXFS
     count changed since the previous count. If the count increased, then
     ignore the WWT timeout. If the count is 0 or remained constant with respect
     to the previous count, then do not ignore the WWT timeout and proceed in
     powering down the SIM interface. */

  if( ((UIM_READ_MISR_SLOT(uim_ptr) & MSMU_IMR_RXSTALE)
        || (0 != uim_bytes_waiting_in_rxfifo(uim_ptr))
        || (UIM_READ_STATUS_SLOT(uim_ptr) & MSMU_SR_RXRDY) )
        && (uim_ptr->card_cmd.uart_dm_interrupt_wait_count  < UIM_UART_DM_INTERUPT_MAX_WAIT_CNT)
    )
  {
    /* Set the command response timer since we are still waiting for more
       bytes. */
    if((uim_ptr->state.DI_current < UIM_BRAF_SIZE) &&
        (uim_ptr->state.FI_current < UIM_CRCF_SIZE))
    {
      uim_start_cmd_rsp_timer( (uim_ptr->card_cmd.work_waiting_time +
                                UIM_UART_DM_WAITING_TIME_CORRECTION),
                               uim_ptr);
    }
    else
    {
      UIMDRV_MSG_ERR_2(uim_ptr->id,"Wrong value of uim_DI_current = 0x%x/uim_FI_current = 0x%x",
                       uim_ptr->state.DI_current,
                       uim_ptr->state.FI_current);
    }
  }
  else
  {
    uim_ptr->card_cmd.uart_dm_interrupt_wait_count = 0;
    /* Let us not clear the timeout signal */
    uim_ptr->flag.clear_timeout_on_response = FALSE;

    /* Mask off all interrupts */
    UIM_PRG_IMR_SLOT(uim_ptr, MSMU_IMR_MASK_ALL );

    /* Reset/flush the UART */
    uim_reset_uart_slot (uim_ptr);

    /* set the RX state to Idle */
    uim_ptr->rxtx_state_machine.rx_state = UIM_RX_PROCESS_IDLE;

     /* Clear the cmd_rsp_timer_ptr first */
    uim_clear_cmd_rsp_timer(uim_ptr);
        /* Set the command response timeout signal so that UIM task can initiate
       the powerdown procedure */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  }

  return;
} /* uim_cmd_rsp_timer_expiry_cb */

/*===========================================================================

FUNCTION UIM_40K_ATR_TIMER_EXPIRY_CB

DESCRIPTION
  This procedure is registered as a call-back associated with the expiry of
  the 40K ATR timer. This call-back is called in the context of the high
  priority timer task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Power down of the SIM interface if the command response timeout expired.

===========================================================================*/
void uim_40k_atr_timer_expiry_cb(timer_cb_data_type param)
{
  uim_instance_global_type *uim_ptr = (uim_instance_global_type*)param;

  ASSERT(uim_ptr);
  /* Return if the UIM clock is OFF */
  if ( !UIM_IS_UIM_CLK_ON(uim_ptr) )
  {
      return;
  }

  /* If we receive no byte power down the interface */
  if ( ! ( (UIM_READ_STATUS_SLOT(uim_ptr) & MSMU_SR_RXRDY)
           ||(uim_ptr->atr.atr_byte_received)
           ||(uim_bytes_waiting_in_rxfifo(uim_ptr) != 0)
         )
     )
  {
    /* Let us not clear the timeout signal */
    uim_ptr->flag.clear_timeout_on_response = FALSE;

    /* set the RX state to Idle */
    uim_ptr->rxtx_state_machine.rx_state = UIM_RX_PROCESS_IDLE;

    /* Reset/flush the UART */
    uim_reset_uart_slot (uim_ptr);

    /* Mask all interrupts from UART. */
    UIM_PRG_IMR_SLOT(uim_ptr, MSMU_IMR_MASK_ALL );

    UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_55,uim_ptr->id, "No ATR in 40,000 clk cycles");

    /* Clear the cmd_rsp_timer_ptr first */
    uim_clear_cmd_rsp_timer(uim_ptr);

    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  }
} /* uim_40k_atr_timer_expiry_cb */


/*===========================================================================

FUNCTION UIM_IS_GET_RESPONSE_ALLOWED

DESCRIPTION
  This procedure determines if get response should be done or not if the last
  sent APDU is SEEK command.

DEPENDENCIES
  None

RETURN VALUE
  TRUE   if SEEK command and ens is enabled or gcf testing is going on
  FALSE  if SEEK command and both ens or gcf testing flags are off.

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_get_response_allowed
(
  uim_instance_global_type *uim_ptr,
  uint8                     sw1,
  uint8                     sw2
)
{
  uint8     channel     = 0;
  boolean   ret_status  = FALSE;

  if(uim_ptr->state.last_instrn_case != UIM_INSTRN_CASE_4 )
  {
    return FALSE;
  }
  if (uim_ptr->debug.last_sent_apdu.instrn == UIM_GENERATE_ASY_KEY_PAIR )
  {
    return FALSE;
  }

  /* As per CR SCPTEC(16)000009r1, CR 102 221 R13: GET RESPONSE after 6282 for SEARCH RECORD 
     Don't do GET RESPONSE for 62 82 status words */
  if((SEEK == uim_ptr->debug.last_sent_apdu.instrn) &&
     (SW1_WARNINGS1 == sw1) &&
     (SW2_EOF_REACHED == sw2))
  {
    return FALSE;
  }  
  
  channel = uim_ptr->command.cmd_ptr->hdr.channel;

  switch (uim_ptr->state.ch_apdu_behavior[channel])
  {
    case UIM_APDU_BEHAVIOR_FOR_TELECOM_APP:
      ret_status = TRUE;
      break;
      
    case UIM_APDU_BEHAVIOR_NO_GET_RESPONSE_FOR_WARNING:
      ret_status = FALSE;
      break;

    case UIM_APDU_BEHAVIOR_AUTOMATIC_GET_RESPONSE_FOR_WARNING:
      ret_status = TRUE;
      if(uim_ptr->card_cmd.num_resp_bytes_rcvd_total)
      {
        ret_status = FALSE;
      }
      break;
  }
  
  return ret_status;
}/* uim_is_get_response_allowed */


/*===========================================================================
  FUNCTION UIM_START_CMD_RSP_TIMER

  DESCRIPTION
   This procedure will start the command response timer.

  DEPENDENCIES


  RETURN VALUE
    None

  SIDE EFFECTS
    None
 ===========================================================================*/
void uim_start_cmd_rsp_timer
(
  dword                     cmd_rsp_time_val,
  uim_instance_global_type *uim_ptr  )
{
  uim_uartdm_set_wwt_val(uim_ptr, uim_ptr->card_cmd.work_waiting_time_etus);
  uim_uartdm_enable_wwt_counter(uim_ptr);
  /* Also start the software wwt timer with a value 100ms more than the hw wwt */
  (void)rex_set_timer(&uim_ptr->command.cmd_rsp_timer, cmd_rsp_time_val + UIM_ADDITIONAL_DELAY_FOR_SWT );
  uim_ptr->flag.active_cmd_rsp_timer = TRUE;
  return;
} /* uim_start_cmd_rsp_timer */

/*===========================================================================

  FUNCTION UIM_CLEAR_CMD_RSP_TIMER

  DESCRIPTION
   This procedure will clear the command response timer

  DEPENDENCIES


  RETURN VALUE
    None

  SIDE EFFECTS
    None
 ===========================================================================*/
void uim_clear_cmd_rsp_timer
(
  uim_instance_global_type *uim_ptr
)
{
  /* Clear the CMD response timer if and only if the timer is active */
  if( TRUE == uim_ptr->flag.active_cmd_rsp_timer)
  {
    uim_uartdm_disable_wwt_counter(uim_ptr);
    (void) rex_clr_timer( &uim_ptr->command.cmd_rsp_timer);
  }
  uim_ptr->flag.active_cmd_rsp_timer = FALSE;
} /* uim_clear_cmd_rsp_timer */


/*===========================================================================
FUNCTION UIM_REGISTER_FOR_SLEEP_VOTING

DESCRIPTION
  Perform client registration for Core cpu node to avoid any latencies
  that may be experienced during power collapse/ xo shutdown.
===========================================================================*/
void uim_register_for_sleep_voting
(
  uim_instance_global_type *uim_ptr
)
{
  switch(uimdrv_hw_nv_config_info.dalChipFamily)
  {
    /* On older chipsets(Dime/Triton) we register for core/cpu/vdd node */
    case DALCHIPINFO_FAMILY_MSM8974:
    case DALCHIPINFO_FAMILY_MSM8974_PRO:
    case DALCHIPINFO_FAMILY_MDM9x25:
    case DALCHIPINFO_FAMILY_MSM8x10:
    case DALCHIPINFO_FAMILY_MSM8x26:
    case DALCHIPINFO_FAMILY_MSM8926:
    case DALCHIPINFO_FAMILY_MSM8x62:
    case DALCHIPINFO_FAMILY_MSM8916:
    case DALCHIPINFO_FAMILY_MSM8936:
    case DALCHIPINFO_FAMILY_MSM8909:
    {
      uim_ptr->hardware.sleep_latency_voting_handle
                    = npa_create_sync_client("/core/cpu/vdd", "uim", NPA_CLIENT_REQUIRED);
      if(uim_ptr->hardware.sleep_latency_voting_handle  == NULL)
      {
        UIMDRV_MSG_ERR_0(uim_ptr->id,"core_cpu_vdd_handle invalid");
      }
      break;
    }

    case DALCHIPINFO_FAMILY_UNKNOWN:
    {
      UIMDRV_MSG_ERR_0(uim_ptr->id,"DAL Chip unknown: Cannot register for core cpu node");
      break;
    }

    default:
      /* For Bolt and newer targets, we register for core/cpu/latency/usec node with interrupt latency
      < 1 usec due to changes in MCPM architecture. */
      uim_ptr->hardware.sleep_latency_voting_handle =
        npa_create_sync_client("/core/cpu/latency/usec", "uim", NPA_CLIENT_REQUIRED);

      if(uim_ptr->hardware.sleep_latency_voting_handle == NULL)
      {
        UIMDRV_MSG_ERR_0(uim_ptr->id,"core_cpu_latency_usec_handle invalid");
      }
      break;
  } /* switch(uimdrv_hw_nv_config_info.dalChipFamily) */
} /* uim_register_for_sleep_voting */


/*===========================================================================
FUNCTION UIM_VOTE_AGAINST_SLEEP

DESCRIPTION
  Votes against sleep by making an npa call with the registered core cpu
  handle based on target. The input parameter along with the handle is the
  max interrupt latency acceptable in units of microseconds.
===========================================================================*/
void uim_vote_against_sleep
(
  uim_instance_global_type *uim_ptr
)
{
  if(NULL != uim_ptr->hardware.sleep_latency_voting_handle)
  {
    npa_issue_required_request(uim_ptr->hardware.sleep_latency_voting_handle,1);
  }
  else
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"uim_vote_against_sleep: NULL Handle");
  }
} /* uim_vote_against_sleep */


/*===========================================================================
FUNCTION UIM_VOTE_FOR_SLEEP

DESCRIPTION
  Votes for sleep by making an npa call indicating a completed request.
===========================================================================*/
void uim_vote_for_sleep
(
  uim_instance_global_type *uim_ptr
)
{
  if(NULL != uim_ptr->hardware.sleep_latency_voting_handle)
  {
    npa_complete_request(uim_ptr->hardware.sleep_latency_voting_handle);
  }
  else
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"uim_vote_for_sleep: NULL Handle");
  }

} /* uim_vote_for_sleep */


/*===========================================================================
FUNCTION UIM_CM_INIT

  DESCRIPTION
   Register UIMDRV as a client of CM

  DEPENDENCIES
   None

  RETURN VALUE
    None

  SIDE EFFECTS
    None
===========================================================================*/
void uim_cm_init(void)
{
  uim_shared_global_type  *shared_global_ptr = uim_get_common_globals_ptr();
  cm_client_status_e_type cm_return_status   = CM_CLIENT_OK;

  /* client init with CM */
  cm_return_status = cm_client_init(CM_CLIENT_TYPE_GSDI,
                                    &shared_global_ptr->cm_client_id);

  if (cm_return_status != CM_CLIENT_OK)
  {
    ERR_FATAL("cm_client_init() returned bad status %d",
              (int32)cm_return_status, 0, 0);
  }

  (void)cm_client_act(shared_global_ptr->cm_client_id);

  /* Register for the events that the device changes the operating mode */
  cm_return_status = cm_client_ph_reg(
      shared_global_ptr->cm_client_id,
      uim_cm_ph_event_cb,
      CM_CLIENT_EVENT_REG,
      CM_PH_EVENT_OPRT_MODE,
      CM_PH_EVENT_OPRT_MODE,
      NULL );
  if (cm_return_status != CM_CLIENT_OK)
  {
    ERR_FATAL("cm_client_ph_reg() returned bad status %d",
              (int32)cm_return_status, 0, 0);
  }
} /* uim_cm_init */


/*===========================================================================
FUNCTION UIM_CM_PH_EVENT_CB

  DESCRIPTION
   Phone (ph) event callback function, called when a ph event is happening.
   This function detects phone event and set appropriate flags.

  DEPENDENCIES
   None

  RETURN VALUE
    None

  SIDE EFFECTS
    None
===========================================================================*/
void uim_cm_ph_event_cb(
  cm_ph_event_e_type          ph_event,
  const cm_ph_info_s_type    *ph_info_ptr)
{
  uim_shared_global_type *shared_global_ptr = uim_get_common_globals_ptr();
  int i = 0;

  if (ph_event == CM_PH_EVENT_OPRT_MODE)
  {
    UIM_MSG_HIGH_1("UIMDRV Phone operating mode changed to 0x%x mode", ph_info_ptr->oprt_mode);

    /* check if the device is in online or FTM mode. Regardless of whether the global is about
       rf_inactive or not, polling would explicitly be performed in FTM mode */
    if (ph_info_ptr->oprt_mode != SYS_OPRT_MODE_ONLINE &&
        ph_info_ptr->oprt_mode != SYS_OPRT_MODE_FTM)
    {
      shared_global_ptr->uim_rf_is_inactive = TRUE;
    }
    else
    {
      shared_global_ptr->uim_rf_is_inactive = FALSE;

      for (i = 0; i < UIM_MAX_INSTANCES; i++)
      { 
        uim_instance_global_type *uim_ptr = uim_get_instance_ptr(i);

        /* Set the poll timer signal only if the timer already expired while
           RF was off. Otherwise, we will wait for the timer to expire */
        if (uim_ptr && uim_ptr->poll.expired_in_rf_off)
        {
          (void) rex_set_sigs(uim_ptr->tcb_ptr, UIM_POLL_TIMER_SIG);
          uim_ptr->poll.expired_in_rf_off = FALSE;
        }
      }
    }
  }
} /* uim_cm_ph_event_cb */


/*===========================================================================
FUNCTION UIM_CHECK_BUA_MPM_IRQ_REG_APPLICABLE

  DESCRIPTION
   Checks if BUA initialization and MPM IRQ registration for card event interrupt
   is applicable by checking the chip family info.
   
   Starting from Hoya architecture, BUA PIN is removed and per slot card event interrupt
   is suppported.

  DEPENDENCIES
   None

  RETURN VALUE
    TRUE  if BUA initialization and MPM IRQ registration is needed.
    FALSE otherwise

  SIDE EFFECTS
    None
===========================================================================*/
boolean uim_check_bua_mpm_irq_reg_applicable(void)
{
  DalDeviceHandle                     *phChipInfo    = NULL;
  DalChipInfoFamilyType               chipFamily     = DALCHIPINFO_FAMILY_UNKNOWN;

  if (DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &phChipInfo) != DAL_SUCCESS)
  {
    UIM_MSG_ERR_0("DAL_DeviceAttach failed in uim_check_bua_applicable");
    return TRUE;
  }

  DalChipInfo_GetChipFamily(phChipInfo, &chipFamily);

  if(chipFamily >= DALCHIPINFO_FAMILY_SDM845)
  {
    return FALSE;
  }
  return TRUE;
} /* uim_check_bua_mpm_irq_reg_applicable */
