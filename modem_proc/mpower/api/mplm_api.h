#ifndef MPLM_API_H_
#define MPLM_API_H_
/*=========================================================================

            M O D E M   P O W E R   L O G   M A N A G E R

              E X T E R N A L   A P I   F I L E


  GENERAL DESCRIPTION
    This file contains the external interface functions for the client inside of
    MODEM SW but outside of MPLM.

  EXTERNALIZED FUNCTIONS


  INITIALIZATION AND SEQUENCING REQUIREMENTS
    Invoke the MPLM_Init function to initialize the MPLM.

        Copyright (c) 2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.



==========================================================================*/
/*==========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------
08/21/2014 sz      Initial version.
==========================================================================*/

/*
 * MPLM_MCPM_Buffer_Type
 *
 * mplm ECS software buffer type.
 *
 * lTimeTick             - time stamp in ticks from MCPM.
 * Vdd_cx           - Vdd_CX value in mV.
 * Vdd_mx           - VDD_MX value in mV.
 * Vdd_mss          - VDD_MSS value in mV.
 * Vdd_q6           - VDD_Q6 value in mV.
 * Clk_snoc         - IPA_SNOC value in frequency.
 * Clk_bimc         - Q6_BIMC value in frequency.
 * Clk_q6           - Q6 frequency.
 * Clk_mdm          - CLK_MDM  frequency.
 * Clk_tdec         - CLK_TDEC frequency.
 * Clk_pcnoc        - PCNOC frequency.
 */
typedef struct
{
  uint64             lTimeTick;
  uint32             Vdd_mss;
  uint32             Vdd_q6;
  uint32             Vdd_mx;
  uint32             Vdd_cx;
  uint32             Clk_mdm;
  uint32             Clk_tdec;
  uint32             Clk_q6;
  uint32             Clk_bimc;
  uint32             Clk_snoc;
  uint32             Clk_pcnoc;
}MPLM_MCPM_Buffer_Type;




#endif /* MPLM_API_H_ */
