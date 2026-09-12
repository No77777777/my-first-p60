/** vi: tw=128 ts=3 sw=3 et
@file dog_hal_root.c
@brief This file contains the API details for the Dog Services, API 2.0
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2017-2018,2020 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/dog/src/dog_hal_root.c#2 $
$DateTime: 2020/05/27 14:12:47 $
$Change: 24056195 $
$Author: pwbldsvc $
===============================================================================*/

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

FUNCTIONS
  HAL_dogEnable
  HAL_dogDisable
  HAL_dogAutokick
  HAL_dogEnableBark
  HAL_dogDisableBark
  HAL_dogRegisterBark
  HAL_dogIsAutokicking
  HAL_dogInit
  HAL_dogForceNMI
  HAL_dogForceBark
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "HALhwio.h"
#include "dog_hal.h"
#include "dog_timeouts.h"
#include DOG_HAL_HEADER
#include "msmhwiobase.h"
#include "DDIInterruptController.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "err.h"
#include "timer.h"
#include "sys_m_smsm.h"
#include "tms_rcevt.h"
#include "rcinit.h"
#include "tms_utils_msg.h"
#include "dog_common_internal.h"

/* If this flag is set to TRUE, hw wdog will NOT be initialized.
*  This flag should be set to TRUE only for DEBUG purposes eg TRACE32 Instrument to Disable hw wdog At Runtime
extern volatile boolean dog_hal_disable_for_bringup;
extern volatile boolean dog_intentional_timeout_flag;
*/

extern volatile dog_disable_state_e dog_disable_state;

/** =====================================================================
 * Function:
 *     HAL_dogEnable
 *
 * Description:
 *     This function enables the hardware dog (NMI and Bite) and does not  
 *     allow enabling if HW_WDOG_DISABLE is set for bring-up/T32 debug
 *
 * Parameters:
 *     none
 *
 * Returns:
 *     none
 * =====================================================================  */
void HAL_dogEnable(void)
{
   if( ! dog_all_bits_set(dog_disable_state, HW_WDOG_DISABLE) )
   {
      // SET NMI TIMEOUT
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_NMI_TIME), DOG_HWIO(WDOG_NMI_TIME_DATA_BMSK), DOG_NMI_ENABLE_TIMEOUT_SCLK);

      // ENABLE NMI
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_CTL), DOG_HWIO(WDOG_CTL_WDOG_TO_NMI_EN_BMSK), 1 << DOG_HWIO(WDOG_CTL_WDOG_TO_NMI_EN_SHFT));

      // SET BITE TIMEOUT
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_BITE_TIME), DOG_HWIO(WDOG_BITE_TIME_DATA_BMSK), DOG_BITE_ENABLE_TIMEOUT_SCLK);

      // ENABLE WDOG
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_CTL), DOG_HWIO(WDOG_CTL_ENABLE_BMSK), 1 << DOG_HWIO(WDOG_CTL_ENABLE_SHFT));
   }
}

/** =====================================================================
 * Function:
 *     HAL_dogEnableOnly
 *
 * Description:
 *     This function enables only the hardware dog bit and does not allow 
 *     enabling if HW_WDOG_DISABLE is set for bring-up/T32 debug
 *
 * Parameters:
 *     none
 *
 * Returns:
 *     none
 * =====================================================================  */
void HAL_dogEnableOnly(void)
{
   if( ! dog_all_bits_set(dog_disable_state, HW_WDOG_DISABLE) )
   {      
      // ENABLE WDOG
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_CTL), DOG_HWIO(WDOG_CTL_ENABLE_BMSK), 1 << DOG_HWIO(WDOG_CTL_ENABLE_SHFT));
   }
}

/** =====================================================================
 * Function:
 *     HAL_dogDisable
 *
 * Description:
 *     This function disables the hardware dog and does not allow 
 *     disabling if HW_KICK_DISABLE is set to trigger intentional timeout
 *
 * Parameters:
 *     none
 *
 * Returns:
 *     none
 * =====================================================================  */
void HAL_dogDisable(void)
{
   if( ! dog_all_bits_set(dog_disable_state, HW_KICK_DISABLE) )
   {
      // DISABLE WDOG
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_CTL), DOG_HWIO(WDOG_CTL_ENABLE_BMSK), 0 << DOG_HWIO(WDOG_CTL_ENABLE_SHFT));
   }
}

/** =====================================================================
 * Function:
 *     HAL_dogEnableBark
 *
 * Description:
 *     Enable the watchdog bark timer.
 *
 * Parameters:
 *     none
 *
 * Returns:
 *     none
 * =====================================================================  */
void HAL_dogEnableBark(void)
{
   // SET BARK TIMEOUT
   HWIO_OUTM(DOG_HWIO_FMT(WDOG_BARK_TIME), DOG_HWIO(WDOG_BARK_TIME_DATA_BMSK), DOG_BARK_ENABLE_TIMEOUT_SCLK);
}

/** =====================================================================
 * Function:
 *     HAL_dogDisableBark
 *
 * Description:
 *     Disable the watchdog bark timer.
 *
 * Parameters:
 *     none
 *
 * Returns:
 *     none
 * =====================================================================  */
void HAL_dogDisableBark(void)
{
   // SET BARK TIMEOUT "NEVER"
   HWIO_OUTM(DOG_HWIO_FMT(WDOG_BARK_TIME), DOG_HWIO(WDOG_BARK_TIME_DATA_BMSK), DOG_HWIO(WDOG_BARK_TIME_DATA_BMSK));
}

/** =====================================================================
* Function:
*     HAL_dogIsAutokicking
*
* Description:
*     Check if dog is autokicking i.e wdog is disabled
*
* Parameters:
*     unsigned int status pointer
*
* Returns:
*     pointer set to 1 if autokicking(wdog disabled), set to 0 (wdog enabled) otherwise
* =====================================================================  */
void HAL_dogIsAutokicking(unsigned int* status)
{
   if (NULL != status)
   {
      if (0 != (HWIO_INM(DOG_HWIO_FMT(WDOG_CTL), DOG_HWIO(WDOG_CTL_ENABLE_BMSK) & DOG_HWIO(WDOG_CTL_ENABLE_BMSK))))
      {
         *status = 0; //wdog is enabled so its NOT "autokicking"
      }
      else
      {
         *status = 1; //wdog is disabled so it is "autokicking"
      }
   }
}

/** =====================================================================
* Function:
*     HAL_dogForceNMI
*
* Description:
*     Force the watchdog to do an NMI
*
* Parameters:
*     none
*
* Returns:
*     none
* =====================================================================  */
void HAL_dogForceNMI(void)
{
   if( ! dog_all_bits_set(dog_disable_state, HW_WDOG_DISABLE) )
   {
      // KICK DOG
      HWIO_OUT(DOG_HWIO_FMT(WDOG_RESET), 1 << DOG_HWIO(WDOG_RESET_WDOG_RESET_SHFT));
      
      // SET BARK TIMEOUT AS MAX
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_BARK_TIME), DOG_HWIO(WDOG_BARK_TIME_DATA_BMSK), DOG_HWIO(WDOG_BARK_TIME_DATA_BMSK));
     
      /* SET BITE TIMEOUT to a low value so we do not have to wait for normal(higher) bite timeout,
         if NMI handler is not able to invoke system monitor fatal notifier API */
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_BITE_TIME), DOG_HWIO(WDOG_BITE_TIME_DATA_BMSK), DOG_BITE_ENABLE_TIMEOUT_SCLK_FORCE);

      // SET MINIMUM NMI TIMEOUT
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_NMI_TIME), DOG_HWIO(WDOG_NMI_TIME_DATA_BMSK), DOG_NMI_ENABLE_TIMEOUT_NOW_SCLK);

      // ENABLE NMI
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_CTL), DOG_HWIO(WDOG_CTL_WDOG_TO_NMI_EN_BMSK), 1 << DOG_HWIO(WDOG_CTL_WDOG_TO_NMI_EN_SHFT));
      
      // ENABLE BITE
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_CTL), DOG_HWIO(WDOG_CTL_ENABLE_BMSK), 1 << DOG_HWIO(WDOG_CTL_ENABLE_SHFT));

      // KICK DOG
      HWIO_OUT(DOG_HWIO_FMT(WDOG_RESET), 1 << DOG_HWIO(WDOG_RESET_WDOG_RESET_SHFT));
      
      dog_set_bit(dog_disable_state, HW_KICK_DISABLE); //Flag to block kicks
   }
}

/** =====================================================================
* Function:
*     HAL_dogForceBark
*
* Description:
*     Force the watchdog to Bark
*
* Parameters:
*     none
*
* Returns:
*     none
* =====================================================================  */
void HAL_dogForceBark(void)
{
   if( ! dog_all_bits_set(dog_disable_state, HW_WDOG_DISABLE) )
   {
      // KICK DOG
      HWIO_OUT(DOG_HWIO_FMT(WDOG_RESET), 1 << DOG_HWIO(WDOG_RESET_WDOG_RESET_SHFT));

      // SET MINIMUM BARK TIMEOUT
      HWIO_OUTM(DOG_HWIO_FMT(WDOG_BARK_TIME), DOG_HWIO(WDOG_BARK_TIME_DATA_BMSK), DOG_BARK_ENABLE_TIMEOUT_NOW_SCLK);

      // KICK DOG
      HWIO_OUT(DOG_HWIO_FMT(WDOG_RESET), 1 << DOG_HWIO(WDOG_RESET_WDOG_RESET_SHFT));

      dog_set_bit(dog_disable_state, HW_KICK_DISABLE); //Flag to block kicks
   }
}

/** =====================================================================
* Function: Wrapper function
*     dog_force_nmi
*
* Description:
*       Force the watchdog to do an NMI
*
* Parameters:
*     none
*
* Returns:
*     none
* =====================================================================  */
void dog_force_nmi(void)
{
   HAL_dogForceNMI();
}

/** =====================================================================
* Function: Wrapper function
*     dog_force_bark
*
* Description:
*       Force the watchdog to do an Bark
*
* Parameters:
*     none
*
* Returns:
*     none
* =====================================================================  */
void dog_force_bark(void)
{
   HAL_dogForceBark();
}
