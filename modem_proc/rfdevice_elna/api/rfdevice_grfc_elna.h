#ifndef RFDEVICE_GRFC_ELNA_H
#define RFDEVICE_GRFC_ELNA_H


/*===========================================================================

Copyright (c) 2015 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_elna.mpss/1.22/api/rfdevice_grfc_elna.h#1 $
  $DateTime: 2021/03/09 03:41:28 $
  $Author: pwbldsvc $

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
09/02/15   px    Added support new parameter for alternate gain mapping support
03/24/15   vv    Add GRFC eLNA sleep support
03/05/15   fhuo  Changed Extern
02/18/15   vv    Initial version

============================================================================*/

/*===========================================================================
                           DEFINITIONS 
===========================================================================*/


#define RFDEVICE_GRFC_ELNA_SLEEP_BITSHIFT         30      // Shift 30 bits : Bit 30,31
#define RFDEVICE_GRFC_ELNA_SLEEP_BITMASK          0x03

#define RFDEVICE_BITS_PER_GRFC_ELNA_GAIN_STATE    2
#define RFDEVICE_GRFC_ELNA_GAIN0_BITSHIFT         28      // Shift 28 bits : Bit 28,29
#define RFDEVICE_GRFC_ELNA_GAIN1_BITSHIFT         26      // Shift 26 bits : Bit 26,27
#define RFDEVICE_GRFC_ELNA_GAIN2_BITSHIFT         24      // Shift 24 bits : Bit 24,25
#define RFDEVICE_GRFC_ELNA_GAIN3_BITSHIFT         22      // Shift 22 bits : Bit 22,23
#define RFDEVICE_GRFC_ELNA_GAIN4_BITSHIFT         20      // Shift 20 bits : Bit 20,21
#define RFDEVICE_GRFC_ELNA_GAIN5_BITSHIFT         18      // Shift 18 bits : Bit 18,19
#define RFDEVICE_GRFC_ELNA_GAIN_BITMASK           0x03    // 2 Bits

#define RFDEVICE_GRFC_ELNA_VALID_FLAG_BITSHIFT    16      // Shift 16 bits : Bit 16,17
#define RFDEVICE_GRFC_ELNA_VALID_FLAG_BITMASK     0x03    // 2 Bits

#define RFDEVICE_GRFC_ELNA_GRFC_SIG_ID_BITSHIFT   0       // Shift 0 bits : Bits 0 - 15
#define RFDEVICE_GRFC_ELNA_GRFC_SIG_ID_BITMASK    0xFFFF  // 16 bits

#define RFDEVICE_GRFC_ELNA_SLEEP_GET(x,y)           ((x>>y) & RFDEVICE_GRFC_ELNA_SLEEP_BITMASK)
#define RFDEVICE_GRFC_ELNA_GAIN_STATE_GET(x,y)      ((x>>y) & RFDEVICE_GRFC_ELNA_GAIN_BITMASK)
#define RFDEVICE_GRFC_ELNA_VALID_FLAG_GET(x)        ((x>>RFDEVICE_GRFC_ELNA_VALID_FLAG_BITSHIFT) & RFDEVICE_GRFC_ELNA_VALID_FLAG_BITMASK)
#define RFDEVICE_GRFC_ELNA_GRFC_NUMBER_GET(x)       ((x>>RFDEVICE_GRFC_ELNA_GRFC_SIG_ID_BITSHIFT) & RFDEVICE_GRFC_ELNA_GRFC_SIG_ID_BITMASK)

#define RFDEVICE_GRFC_LNA_SIG_ID_SLEEP_G0_TO_G5_LOGIC(sig_name, SLEEP,G0, G1, G2, G3, G4, G5) \
(        ( (sig_name & RFDEVICE_GRFC_ELNA_GRFC_SIG_ID_BITMASK) << RFDEVICE_GRFC_ELNA_GRFC_SIG_ID_BITSHIFT ) | \
         ( (SLEEP & RFDEVICE_GRFC_ELNA_SLEEP_BITMASK ) << RFDEVICE_GRFC_ELNA_SLEEP_BITSHIFT  ) | \
         ( (G0 & RFDEVICE_GRFC_ELNA_GAIN_BITMASK ) << RFDEVICE_GRFC_ELNA_GAIN0_BITSHIFT  ) | \
         ( (G1 & RFDEVICE_GRFC_ELNA_GAIN_BITMASK ) << RFDEVICE_GRFC_ELNA_GAIN1_BITSHIFT  ) | \
         ( (G2 & RFDEVICE_GRFC_ELNA_GAIN_BITMASK ) << RFDEVICE_GRFC_ELNA_GAIN2_BITSHIFT  ) | \
         ( (G3 & RFDEVICE_GRFC_ELNA_GAIN_BITMASK ) << RFDEVICE_GRFC_ELNA_GAIN3_BITSHIFT  ) | \
         ( (G4 & RFDEVICE_GRFC_ELNA_GAIN_BITMASK ) << RFDEVICE_GRFC_ELNA_GAIN4_BITSHIFT  ) | \
         ( (G5 & RFDEVICE_GRFC_ELNA_GAIN_BITMASK ) << RFDEVICE_GRFC_ELNA_GAIN5_BITSHIFT  ) | \
         ( (TRUE & RFDEVICE_GRFC_ELNA_VALID_FLAG_BITMASK) << RFDEVICE_GRFC_ELNA_VALID_FLAG_BITSHIFT ))
/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus

#include "rfc_common.h"
#include "rfdevice_type_defs.h"
#include "rfdevice_lna.h"
#include "rfdevice_lna_cfg.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_physical_third_party.h"
#include "rfdevice_grfc_elna_typedef.h"
#include "rfdevice_elna_typedef.h"
class rfdevice_grfc_elna: public rfdevice_lna
{
  public:

 
    virtual ~rfdevice_grfc_elna();


    rfdevice_grfc_elna(
                  rfdevice_physical_device* phy_obj_ptr,
                  rfc_phy_device_info_type *phy_device_info, 
                  rfc_logical_device_info_type *logical_device_info
                  ); 

    /*Default constructor*/
    rfdevice_grfc_elna(void);

    boolean init_status;

  /*!
    @brief
    interface function for lna sleep

    @details
    rfdevice_lna_sleep()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info  
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
   boolean sleep
   (
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

  /*!
    @brief
    interface function for lna wakeup

    @details
    rfdevice_lna_wakeup()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info  
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
   boolean wakeup
   (
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

  /*!
    @brief
    returns the lna gain settings 

    @details
    returns the lna gain settings 

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
   boolean set_lna_gain
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

  /*!
    @brief
    interface function for lna trigger, to latch the pending settings.

    @details
    rfdevice_lna_trigger()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    time offset for the script where a negative value refers to the time BEFORE the event

    @return
    boolean indicating pass or fail
  */
   boolean trigger
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

   /*!
    @brief
    populates the band map information from rfc

    @details
    populates the band map information from rfc

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param *data_ptr
    band mapping information passed as int32 pointer
    
    @param size
    number of valid words in the *data_ptr
    
    @return
    boolean indicating pass or fail
  */
   boolean set_band_map
   (
    rfdevice_lna_cfg* lna_cfg_p,
    int32 *data_ptr, 
    uint8 size
   );

  /*!
    @brief
    returns the lna gain for the requested system gain

    @details
    the techs would call this api to get the lna gain to system gain mapping for a particular tech and band.

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param lna_gain
    returns an enum indicating the lna gain
    
    @param use_alt_gain
    set to TRUE to use alternate gain mapping (such as for asdiv)
    Currently NOT supported for GRFC eLNA. Modem will force crash if TRUE.
   
    @return
    boolean indicating pass or fail
  */
   boolean get_lna_gain_index
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rfdevice_lna_gain_enum_type *lna_gain,
    boolean use_alt_gain
   );

  protected:

   rfdevice_grfc_elna *rfdevice_grfc_elna_p;

   /*To store the parent physical device object*/
   rfdevice_physical_device* rfdevice_phy_obj_ptr;

   rfc_device_cfg_info_type grfc_elna_device_info;

   rfdevice_elna_grfc_rfc_tbl_band_map_type band_map_grfc_tbl;

   boolean rfdevice_grfc_elna_param_validity
   (
     rfdevice_lna_cfg* lna_cfg_p,
     rf_buffer_intf *buff_obj_ptr,
     rf_device_execution_type dev_action,
     int16 script_timing
   );
    
   boolean rfdevice_grfc_elna_process_grfc_tbl
   ( 
     rfdevice_elna_req_enum_type,
     rf_buffer_intf *buff_obj_ptr, 
     rf_device_execution_type dev_action, 
     int16 script_timing,
     rfdevice_elna_grfc_rfc_tbl_type *rfc_cfg_ptr,
     rfdevice_lna_cfg* lna_cfg_p
   );

   boolean rfdevice_grfc_elna_get_band_map_grfc_tbl
   (
    rfm_device_enum_type rfm_device,
    rfcom_mode_enum_type mode,
    rfcom_band_type_u band,
    rfdevice_elna_grfc_rfc_tbl_type **rfc_grfc_tbl
   );

  private:
 
  };

#endif

#endif /* RFDEVICE_GRFC_ELNA_H */
