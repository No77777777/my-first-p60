#ifndef IPA_TEST_UTIL_CIPH_SNOW_3G_H
#define IPA_TEST_UTIL_CIPH_SNOW_3G_H

#include <iostream>
#include <fstream>
#include "IPA_test_util_ciph_clbctypes.h"

namespace qcrypto
{

const int32 NUM_ROW_SR        = 256;
const int32 NUM_ROW_SQ        = 256;
const int32 NUM_ROW_MUL_ALPHA = 256;
const int32 NUM_ROW_DIV_ALPHA = 256;

//! Snow3G Keystream Generator TS35.216
class Snow3G {

public:

  /*=======
    METHODS
    =======*/

  /*=========================================================*/
  // FUNCTION: Snow3G() Constructor
  //
  //! Constructor for the Snow3G class
  //
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  Snow3G();

  /*=========================================================*/
  // FUNCTION: Snow3G() Destructor
  //
  //! Destructor for the Snow3G class
  //
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  ~Snow3G(){}

  /*=========================================================*/
  // FUNCTION: void initialize(uint32 key[4], uint32 IV[4])
  //
  //! initialize all the LFSRs and FSM (4.1 of TS35.216)
  //
  /*!
    All the LFSRs and FSM are initialized for key generation.
    \param k[4]  Four 32-bit words making up 128-bit key.
    \param IV[4] Four 32-bit words making 128-bit initialization variable.
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  void initialize(uint32 k[4], uint32 IV[4]);

  /*=========================================================*/
  // FUNCTION: void generateKeystream(uint32 n, uint32* z)
  //
  //! generate a keystream (4.2 of TS35.216)
  //
  /*!
    - generate a keystream n 32-bit words long filled in z
    - assumes memory for z is allocated already
    \param uint32  n number of 32-bit words of keystream.
    \param uint32* z space for the generated keystream
    \param cont true if the keystream is continuous from the last call
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  void generateKeystream(uint32 n, uint32 *z, bool cont);

  /*=========================================================*/
  // FUNCTION: static void printAllTableInHWFormat(ostream& ost)
  //
  //! print all lookup table in SNOW3G in HW formate
  //
  /*!
    - print out all lookup table for HW verification
    \param ost reference to the output stream
   */
  // REVISION HISTORY
  // 21 August 2007. Created by Samson Jim
  /*=========================================================*/
  //static void printAllTableInHWFormat(std::ostream& ost);

private:

  /*=========
  METHODS
  =========*/

  /*=========================================================*/
  // FUNCTION: uint8 MULx(uint8 V, uint8 c)
  //
  //! finite field multiplication (3.1.1 of TS35.216)
  //
  /*!
    - multiplication with the primitive element of an extension of degree 8 of GF(2)
    \f[
    MUL_x(V,c)=\left\{
    \begin{array}{ll}
      (V\ll_81)\oplus c & \textrm{if $V\gg7==1$}\\
     V\ll_81 & \textrm{else}
    \end{array} \right.
    \f]
    \param  V an 8-bit input
    \param  c an 8-bit input
    \return an 8-bit output
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  uint8 MULx(uint8 V, uint8 c);

  /*=========================================================*/
  // FUNCTION: uint8 MULxPOW(uint8 V, uint8 i, uint8 c)
  //
  //! finite field multiplication to the power of an positive integer i.(3.1.2 of TS35.216)
  //
  /*!
    - the multiplication with the primitive element raised to the power of an positive integer i.
    \f[
    MUL_xPOW(V,i,c)=\left\{
    \begin{array}{ll}
     V & i==0\\
     MUL_x(MUL_xPOW(V,i,c),c) & i != 0
    \end{array} \right.
    \f]
    \param  V an 8-bit input
    \param  i an 8-bit input
    \param  c an 8-bit input
    \return an 8-bit output
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  uint8 MULxPOW(uint8 V, uint8 i, uint8 c);

  /*=========================================================*/
  // FUNCTION: uint32 MULalpha(uint8 c)
  //
  //! \f$MUL_\alpha\f$ (3.4.2 of TS35.216)
  //
  /*!
    - a function of mapping 8-bit to 32-bit
    \f[
    MUL_\alpha(c)=(MUL_xPOW(c,23,0xA9)\|MUL_xPOW(c,245,0xA9)\|MUL_xPOW(c,48,0xA9)\|MUL_xPOW(c,239,0xA9))
    \f]
    \param  uint8 c an 8-bit input
    \return uint32  a 32-bit output
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  uint32 MULalpha(uint8 c);

  /*=========================================================*/
  // FUNCTION: uint32 DIValpha(uint8 c)
  //
  //! \f$DIV_\alpha\f$ (3.4.3 of TS35.216)
  //
  /*!
    - a function of mapping 8-bit to 32-bit
    \f[
    DIV_\alpha(c)=(MUL_xPOW(c,16,0xA9)\|MUL_xPOW(c,39,0xA9)\|MUL_xPOW(c,6,0xA9)\|MUL_xPOW(c,64,0xA9))
    \f]
    \param  uint8 c an 8-bit input
    \return uint32 a 32-bit output
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  uint32 DIValpha(uint8 c);

  /*=========================================================*/
  // FUNCTION: uint32 S1(uint32 w)
  //
  //! S-Box S1 mapping (3.3.1 of TS35.216)
  //
  /*!
    \f[
    \left.
    \begin{array}{lllll}
    r_0= & MUL_X(S_R(w_0),0x1B)                & \oplus S_R(w_1)                            & \oplus S_R(w_2)                            & \oplus MUL_X(S_R(w_3),0x1B)\oplus S_R(w_3) \\
    r_1= & MUL_X(S_R(w_0),0x1B)\oplus S_R(w_0) & \oplus MUL_X(S_R(w_1),0x1B)                & \oplus S_R(w_2)                            & \oplus S_R(w_3) \\
    r_2= & S_R(w_0)                            & \oplus MUL_X(S_R(w_1),0x1B)\oplus S_R(w_1) & \oplus MUL_X(S_R(w_2),0x1B)                & \oplus S_R(w_3) \\
    r_3= & S_R(w_0)                            & \oplus S_R(w_1)                            & \oplus MUL_X(S_R(w_2),0x1B)\oplus S_R(w_2) & \oplus MUL_X(S_R(w_3),0x1B)
    \end{array}
    \right.
    \f]
    \param  uint32 w a 32-bit input
    \return uint32 a 32-bit output
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  uint32 S1(uint32 w);

  /*=========================================================*/
  // FUNCTION: uint32 S1(uint32 w)
  //
  //! S-Box S2 mapping (3.3.2 of TS35.216)
  //
  /*!
    \f[
    \left.
    \begin{array}{lllll}
    r_0= & MUL_X(S_Q(w_0),0x69)                & \oplus S_Q(w_1)                            & \oplus S_Q(w_2)                            & \oplus MUL_X(S_Q(w_3),0x69)\oplus S_Q(w_3) \\
    r_1= & MUL_X(S_Q(w_0),0x69)\oplus S_Q(w_0) & \oplus MUL_X(S_Q(w_1),0x69)                & \oplus S_Q(w_2)                            & \oplus S_Q(w_3) \\
    r_2= & S_Q(w_0)                            & \oplus MUL_X(S_Q(w_1),0x69)\oplus S_Q(w_1) & \oplus MUL_X(S_Q(w_2),0x69)                & \oplus S_Q(w_3) \\
    r_3= & S_Q(w_0)                            & \oplus S_Q(w_1)                            & \oplus MUL_X(S_Q(w_2),0x69)\oplus S_Q(w_2) & \oplus MUL_X(S_Q(w_3),0x69)
    \end{array}
    \right.
    \f]
    \param  uint32 w a 32-bit input
    \return uint32 a 32-bit output
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  uint32 S2(uint32 w);

  /*=========================================================*/
  // FUNCTION: void leftShiftRegister(uint32 v)
  //
  //! left shifting registers S0 - S15, S15=v
  //
  /*!
    \param uint32 v a 32-bit input, S15=v
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  void leftShiftRegisterSs(uint32 v);

  /*=========================================================*/
  // FUNCTION: void ClockLFSRInitializationMode(uint32 F)
  //
  //! clocking LFSR in initialization mode.(3.4.4 of TS35.216)
  //
  /*!
    - LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
    \param  uint32 F a 32-bit word comes from output of FSM
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  void clockLFSRInitializationMode(uint32 F);

  /*=========================================================*/
  // FUNCTION: void ClockLFSRKeyStreamMode()
  //
  //! clocking LFSR in keystream mode.(3.4.5 of TS35.216)
  //
  /*!
    - LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  void clockLFSRKeyStreamMode();

  /*=========================================================*/
  // FUNCTION: uint32 clockFSM()
  //
  //! clocking FSM.(3.4.6 of TS35.216)
  //
  /*!
    - updates FSM registers R1, R2 and R3
    \return a 32-bit word F
   */
  // REVISION HISTORY
  // 2 March 2007. Created by Samson Jim
  /*=========================================================*/
  uint32 clockFSM();

  /*=========
  VARIABLES
  =========*/

  uint32 LFSR_S0;   //!< left shift register \f$S_0\f$
  uint32 LFSR_S1;   //!< left shift register \f$S_1\f$
  uint32 LFSR_S2;   //!< left shift register \f$S_2\f$
  uint32 LFSR_S3;   //!< left shift register \f$S_3\f$
  uint32 LFSR_S4;   //!< left shift register \f$S_4\f$
  uint32 LFSR_S5;   //!< left shift register \f$S_5\f$
  uint32 LFSR_S6;   //!< left shift register \f$S_6\f$
  uint32 LFSR_S7;   //!< left shift register \f$S_7\f$
  uint32 LFSR_S8;   //!< left shift register \f$S_8\f$
  uint32 LFSR_S9;   //!< left shift register \f$S_9\f$
  uint32 LFSR_S10;  //!< left shift register \f$S_{10}\f$
  uint32 LFSR_S11;  //!< left shift register \f$S_{11}\f$
  uint32 LFSR_S12;  //!< left shift register \f$S_{12}\f$
  uint32 LFSR_S13;  //!< left shift register \f$S_{13}\f$
  uint32 LFSR_S14;  //!< left shift register \f$S_{14}\f$
  uint32 LFSR_S15;  //!< left shift register \f$S_{15}\f$

  uint32 FSM_R1;   //!< finite state machine register \f$R_1\f$
  uint32 FSM_R2;   //!< finite state machine register \f$R_2\f$
  uint32 FSM_R3;   //!< finite state machine register \f$R_3\f$

  //! Rijndael S-box \f$S_R\f$
  const static uint8 SR[256];

  //! S-box \f$S_Q\f$
  const static uint8 SQ[256];

  //!\f$MUL_\alpha\f$ table (mapping 8-bit to 32-bit)
  const static uint32 MULalpha_table[256];

  //! \f$DIV_\alpha\f$ table (mapping 8-bit to 32-bit)
  const static uint32 DIValpha_table[256];

}; // Ends [class Snow3G]
}
#endif // Ends [#ifndef _SNOW3G_H]
