/*
 * HALclkInitASM.S
 * This file contains the implementation for setting up PLL, Q6 clock of LPASS
 *                   Edit History

   $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdx24/HALclkInitASM.S#1 $

     #when       who                  what, where, why
    --------   -------------       ----------------------------------------------
    01/07/16   dcf                 Created file based on 8996.
 *
 */

#include "HALclkHWIOASM.h"


/////////////////////////////////////////////////////////////////////////////////
//
//       Additional definitions and bit masks not defined in HWIO inc files.
//
/////////////////////////////////////////////////////////////////////////////////


#define MSS_Q6SS_CORE_CLK_PLL_SEL 0x1
#define MSS_Q6SS_CORE_CLK_DIV     0x1

#define BUSYWAIT_ACR_0_ADDR                   0x04120040
#define BUSYWAIT_ACR_1_ADDR                   0x04120044
#define BUSYWAIT_ACR_2_ADDR                   0x04120048

#define HALCLK_WAIT_5_US                      0x60
#define HALCLK_WAIT_10_US                     0xC0

#define MSS_QDSP6SS_PLL_CONFIG_CTL_VAL        0x20485699
#define MSS_QDSP6SS_PLL_CONFIG_CTL_U_VAL      0x00002067

#define MSS_QDSP6SS_PLL_USER_CTL_VAL          0x00000000
#define MSS_QDSP6SS_PLL_USER_CTL_U_VAL        0x00004804

#define MSS_QDSP6SS_PLL_TEST_CTL_VAL          0x40000000
#define MSS_QDSP6SS_PLL_TEST_CTL_U_VAL        0x00000000

#define TCSR_SOC_VERSION_MAJOR_V1_VAL         0x100

#define MSS_QDSP6SS_PLL_OPMODE_RUN           0x1


/////////////////////////////////////////////////////////////////////////////////
//
//                        Data declarations start here.
//
/////////////////////////////////////////////////////////////////////////////////

   //PLL L, and ALPHA VAL REG values
   .equ Q6_PLL_CALIB_L_VAL,                    0x44  // 68 for 1305.6 MHz - Trion PLLs should be calibrated at 1.3GHz
   .equ Q6SS_PLL_L_VAL,                        0x32  // 50 for 960.0 MHz TODO was 940.8 on sdm845.
                                                     //                       Should this be 960, 940.8,
                                                     //                       or 768 (next highest supported freq)

   .equ Q6_PLL_CALIB_L_V2_VAL,                 0x44  //  68 for 1305.6 for 729.6 MHz
   .equ Q6SS_PLL_L_V2_VAL,                     0x32  //  50 for 960.0 MHz

   .equ Q6_PLL_CALIB_L_V3_VAL,                 0x44  //  68 for 1305.6 MHz
   .equ Q6SS_PLL_L_V3_VAL,                     0x32  //  50 for 960.0 MHz


/////////////////////////////////////////////////////////////////////////////////
//
//                 Macros Start Here.
//
/////////////////////////////////////////////////////////////////////////////////


    //
    // MACRO:  WRITE_REGISTER
    //
    // DESCRIPTION:
    //   This macro writes an unmasked value to a register.  Any contents will
    //   be overwritten in the proccess.  r6 and r7 may be clobbered.
    //

    .macro write_register  val
        {
            r6 = \val
            r7 = r0
        }
        memw(r0) = r6
        barrier
    .endm


    //
    // MACRO:  CLEAR_BITS
    //
    // DESCRIPTION:
    //   This macro clears one ore more bits in a register.  r4, r5, r6, and r7
    //   may be clobbered when running this macro.
    //

    .macro clear_bits val_mask
        {
            r7 = r0
            r5.h = #hi(\val_mask)
        }
        {
            r5.l = #lo(\val_mask)
        }
        {
            r4 = memw(r0)
            r5 = not(r5)
        }
        r4 = and(r4, r5)    // clear the mask bits
        {
            memw(r0) = r4  //write the reg value
        }
        barrier
    .endm


    //
    // MACRO:  SET_BITS
    //
    // DESCRIPTION:
    //   This macro sets one or more bits in a register.  r4, r5 and r7 will be
    //   clobbered when using this macro.
    //

    .macro set_bits val_mask
        {
            r7 = r0
            r5.h = #hi(\val_mask)
        }
        {
            r5.l = #lo(\val_mask)
            dcinva(r7)
        }
        {
            r4 = memw(r0)
            r6 = not(r5)
        }
        r4 = and(r4, r6)    // clear the fields
        r4 = or(r4,r5)      // set the value

        {
            memw(r0) = r4  //write the reg value
        }
        barrier
    .endm

    .macro __busywait_init
        r0 = ##BUSYWAIT_ACR_0_ADDR
        memw(r0) = ##HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK
        r0 = ##BUSYWAIT_ACR_1_ADDR
        memw(r0) = ##HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK
        r0 = ##BUSYWAIT_ACR_2_ADDR
        memw(r0) = ##HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK
        barrier
    .endm

    .macro __busywait_deinit
        r0 = ##BUSYWAIT_ACR_0_ADDR
        memw(r0) = ##0x0
        r0 = ##BUSYWAIT_ACR_1_ADDR
        memw(r0) = ##0x0
        r0 = ##BUSYWAIT_ACR_2_ADDR
        memw(r0) = ##0x0
        barrier
    .endm


/////////////////////////////////////////////////////////////////////////////////
//
//         Executable code starts here.
//
/////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------
// FUNCTION: __HEXAGON_BSP_INIT
//
// DESCRIPTION:
//   This is the main API for this file.  This function will check the status of the
//   main MSS Q6 PLL and configure it if it is not running.  Otherwise, it will just
//   add a HW vote on behalf of the MSS Q6 processor.
//
//   The QDSP6 clock will then be initialized if not already set up.  The
//   purpose of this is to ensure the QDSP6 clock is running at maximum
//   frequency under SVS voltage.
//
// ASSUMPTIONS:
//   There is no stack available when this is called, so only device registers can be
//   accessed.
//----------------------------------------------------------------------------------
#if defined(SIM)
  // No initialization.
#else
   .section .start,"ax",@progbits
   .falign
   .global __hexagon_bsp_init
   .type  __hexagon_bsp_init, @function

__hexagon_bsp_init:
   __busywait_init

  jump start_prog

__busywait:
   {
    r8 = r1
    r9 = r2
    r10 = r3
   }
  r2 = r0             // delay_count
  r0 = ##HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR
  r1 = memw(r0)       // start count

__buswait_loop:
  r3 = memw(r0)
  r3 = sub(r3, r1)
  {
    p0 = cmp.gtu(r3,r2)
    if (!p0.new) jump:t __buswait_loop
  }
  {
    r1 = r8
    r2 = r9
    r3 = r10
  }
  jumpr r13

start_prog:

  //*******************************************************************
  // Program the PLL_CONFIG_CTL values based on recommendations.
  //*******************************************************************

   r0 = ##HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR
   memw(r0) = ##MSS_QDSP6SS_PLL_CONFIG_CTL_VAL

   r0 = ##HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR
   memw(r0) = ##MSS_QDSP6SS_PLL_CONFIG_CTL_U_VAL

  //*******************************************************************
  // Program the USER_CTL and USER_CTL_U values based on recommendations.
  //*******************************************************************

   r0 = ##HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR
   memw(r0) = ##MSS_QDSP6SS_PLL_USER_CTL_VAL

   r0 = ##HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR
   memw(r0) = ##MSS_QDSP6SS_PLL_USER_CTL_U_VAL
   barrier


  //*******************************************************************
  // Program the TEST_CTL and TEST_CTL_U values based on recommendations.
  //*******************************************************************

   r0 = ##HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR
   memw(r0) = ##MSS_QDSP6SS_PLL_TEST_CTL_VAL

   r0 = ##HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR
   memw(r0) = ##MSS_QDSP6SS_PLL_TEST_CTL_U_VAL
   barrier

  //*******************************************************************
  //
  // Begin main PLL programming sequence here.
  //
  //*******************************************************************

start_pll:

  //*******************************************************************
  // Check the HW version for the PLL_CONFIG_CTL programming.
  //*******************************************************************

   r0 = ##HWIO_TCSR_SOC_HW_VERSION_ADDR
   r1 = memw(r0)
   r2 = and(r1, ##TCSR_SOC_VERSION_MAJOR_V1_VAL)
   {
     p0 = cmp.eq(r2,##TCSR_SOC_VERSION_MAJOR_V1_VAL)
     if (p0.new) jump:t v1_pll_configs
   }

   // Use the last supported version if a valid
   // version wasn't detected
   jump v1_pll_configs

v1_pll_configs:

   r14 = ##Q6SS_PLL_L_VAL
   r15 = ##Q6_PLL_CALIB_L_VAL
   r16 = ##MSS_Q6SS_CORE_CLK_DIV
   jump program_q6ss_pll

program_q6ss_pll:

  //*******************************************************************
  // Set up the PLL L-value and CAL_L value.
  //*******************************************************************

  {
    r0 = ##HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR
    r1 = r14
  }
  memw(r0) = r1
  barrier

  r0 = ##HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR
  r1 = r15
  memw(r0) = r1
  barrier


  //*******************************************************************
  // Turn off the PLL outputs.
  //*******************************************************************

  r0 = ##HWIO_MSS_QDSP6SS_PLL_MODE_ADDR
  clear_bits HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_BMSK


  //*******************************************************************
  // Put the PLL into Standby mode.
  //*******************************************************************

  r0 = ##HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR
  clear_bits HWIO_MSS_QDSP6SS_PLL_OPMODE_PLL_OPMODE_BMSK

  //*******************************************************************
  // Bring PLL out of reset.
  //*******************************************************************

  r0 = ##HWIO_MSS_QDSP6SS_PLL_MODE_ADDR
  set_bits HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_BMSK

  //*******************************************************************
  // Put the PLL into run mode.
  //*******************************************************************

  r0 = ##HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR
  r1 = memw(r0)
  r2 = ##(MSS_QDSP6SS_PLL_OPMODE_RUN << HWIO_MSS_QDSP6SS_PLL_OPMODE_PLL_OPMODE_SHFT)
  r1 = or(r1,r2)
  memw(r0) = r1
  barrier

  //*******************************************************************
  // Poll on the lock detect value until it transitions to '1'
  //*******************************************************************

  r4 = ##HWIO_MSS_QDSP6SS_PLL_MODE_ADDR
  r2 = ##HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_BMSK

q6_lock_det_loop:

  r0 = ##HALCLK_WAIT_10_US
  r13 = add(PC, #0x8)
  jump __busywait

  r1 = memw(r4)
  r3 = and(r1, r2)
  {
    p0 = cmp.eq(r2,r3)
    if (!p0.new) jump:t q6_lock_det_loop
  }

  //*******************************************************************
  // Enable PLL Outputs
  //*******************************************************************

  r0 = ##HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR
  set_bits HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_MAIN_BMSK

  r0 = ##HWIO_MSS_QDSP6SS_PLL_MODE_ADDR
  set_bits HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_BMSK

  // ...End QDSP6SS PLL Setup...

  //*******************************************************************
  // Switch the Q6 to run on the QDSP6SS_PLL.
  //*******************************************************************

check_qdsp6:
   r0 = ##HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR
   r1 = memw(r0)

   // Clear out the source selection and divider values.
   {
     r2 = ##HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_BMSK
     r3 = ##HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_BMSK
   }
   r2 = or(r2, r3)
   r2 = not(r2)
   r1 = and(r1, r2)

   // Add the new values.
   {
     r2 = ##MSS_Q6SS_CORE_CLK_PLL_SEL<<HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_SHFT
     r3 = asl(r16,#HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_SHFT)
   }
   r2 = or(r2, r3)
   r1 = or(r1, r2)
   memw(r0) = r1
   barrier

   // Ensure Q6 is on.
   r0 = ##HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR
   r1 = memw(r0)

   // Trigger the update
   r2 = ##HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_BMSK
   r1 = or(r1, r2)
   memw(r0) = r1
   barrier

q6_cmd_update:
   r1 = memw(r0)
   r3 = and(r1, r2)
   {
     p0 = cmp.eq(r3, r2)
     if (p0.new) jump:t q6_cmd_update
   }

   __busywait_deinit

end_init:
   Jumpr r31

.size __hexagon_bsp_init, .-__hexagon_bsp_init
#endif // SIM
