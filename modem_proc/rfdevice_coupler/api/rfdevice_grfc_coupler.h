#ifndef RFDEVICE_GRFC_COUPLER_H
#define RFDEVICE_GRFC_COUPLER_H

/*===========================================================================

Copyright (c) 2014,2015 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_coupler.mpss/1.22/api/rfdevice_grfc_coupler.h#1 $
  $DateTime: 2021/03/09 03:37:09 $
  $Author: pwbldsvc $

when       who   what, where, why
--------   ---   --------------------------------------------------------------- 
01/26/15   yb     Fixed bit shift masks
01/23/15   yb     Fixed compiler errors with RFC changes
01/22/15   yb     Added bitmasks and bishifts for RFC data
12/26/14   ndb    Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#define RFDEVICE_GRFC_COUPLER_REV_LOGIC_BITSHIFT        0x16      // Shift 22 bits : Bit 22,23 [24-31 are reserved]
#define RFDEVICE_GRFC_COUPLER_FWD_LOGIC_BITSHIFT        0x14      // Shift 20 bits : Bit 20,21
#define RFDEVICE_GRFC_COUPLER_OPEN_LOGIC_BITSHIFT       0x12      // Shift 18 bits : Bit 18,19
#define RFDEVICE_GRFC_COUPLER_LOGIC_BITMASK             0x3       // 2 Bits
#define RFDEVICE_GRFC_COUPLER_VALID_FLAG_BITSHIFT       0x10      // Shift 16 bits : Bit 16,17
#define RFDEVICE_GRFC_COUPLER_VALID_FLAG_BITMASK        0x3       // 2 Bits
#define RFDEVICE_GRFC_COUPLER_GRFC_SIG_ID_BITSHIFT      0x0       // Shift 0 bits : Bits 0 - 15
#define RFDEVICE_GRFC_COUPLER_GRFC_SIG_ID_BITMASK       0xFFFF    // 16 bits

#define RFDEVICE_GRFC_COUPLER_OPEN_LOGIC_GET(x)        ((x>>RFDEVICE_GRFC_COUPLER_OPEN_LOGIC_BITSHIFT) & RFDEVICE_GRFC_COUPLER_LOGIC_BITMASK)
#define RFDEVICE_GRFC_COUPLER_FWD_LOGIC_GET(x)         ((x>>RFDEVICE_GRFC_COUPLER_FWD_LOGIC_BITSHIFT) & RFDEVICE_GRFC_COUPLER_LOGIC_BITMASK)
#define RFDEVICE_GRFC_COUPLER_REV_LOGIC_GET(x)         ((x>>RFDEVICE_GRFC_COUPLER_REV_LOGIC_BITSHIFT) & RFDEVICE_GRFC_COUPLER_LOGIC_BITMASK)
#define RFDEVICE_GRFC_COUPLER_VALID_FLAG_GET(x)        ((x>>RFDEVICE_GRFC_COUPLER_VALID_FLAG_BITSHIFT) & RFDEVICE_GRFC_COUPLER_VALID_FLAG_BITMASK)
#define RFDEVICE_GRFC_COUPLER_GRFC_NUMBER_GET(x)       ((x>>RFDEVICE_GRFC_COUPLER_GRFC_SIG_ID_BITSHIFT) & RFDEVICE_GRFC_COUPLER_GRFC_SIG_ID_BITMASK)

#define RFDEVICE_GRFC_COUPLER_SIG_ID_OPEN_FWD_REV_LOGIC(sig_name, open_logic, fwd_logic, rev_logic) \
        (( (sig_name & RFDEVICE_GRFC_COUPLER_GRFC_SIG_ID_BITMASK) << RFDEVICE_GRFC_COUPLER_GRFC_SIG_ID_BITSHIFT ) | \
         ( (open_logic & RFDEVICE_GRFC_COUPLER_LOGIC_BITMASK ) << RFDEVICE_GRFC_COUPLER_OPEN_LOGIC_BITSHIFT  ) | \
         ( (fwd_logic & RFDEVICE_GRFC_COUPLER_LOGIC_BITMASK ) << RFDEVICE_GRFC_COUPLER_FWD_LOGIC_BITSHIFT  ) | \
         ( (rev_logic & RFDEVICE_GRFC_COUPLER_LOGIC_BITMASK ) << RFDEVICE_GRFC_COUPLER_REV_LOGIC_BITSHIFT  ) | \
         ( ( (TRUE & RFDEVICE_GRFC_COUPLER_VALID_FLAG_BITMASK) << RFDEVICE_GRFC_COUPLER_VALID_FLAG_BITSHIFT )))

#ifdef __cplusplus


#include "rfc_common.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_coupler.h"
#include "rfdevice_grfc_coupler_typedef.h"

class rfdevice_grfc_coupler: public rfdevice_coupler
{
public:
  rfdevice_grfc_coupler(void);
   
  rfdevice_grfc_coupler
  ( 
    rfdevice_physical_device* phy_obj_ptr,
    rfc_phy_device_info_type *phy_device_info, 
    rfc_logical_device_info_type *logical_device_info
  );   

  virtual ~rfdevice_grfc_coupler();

  boolean config_coupler
  (
    rfdevice_coupler_tech_cfg_type tech_cfg,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );

  void init(void);

  virtual boolean init 
  (
    int32* rfc_data
  );

  boolean sleep
  (
    int32* rfc_data,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );

  boolean wakeup
  (
    int32* rfc_data,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );

  boolean trigger
  (
    int32* rfc_data,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );

  boolean set_band_config_map
  (
    rfcom_mode_enum_type mode,
    rfcom_band_type_u band,
    int32 *data_ptr
  );

  boolean rfdevice_grfc_coupler_validate
 (
   rf_buffer_intf *buff_obj_ptr, 
   rf_device_execution_type dev_action,
   int16 script_timing   
 );
  boolean init_status;
  protected:

  private:

  /*physical device pointer to store the parent physical device object  during physical device creation */
  
  rfdevice_physical_device* rfdevice_phy_obj_ptr; 
  rfdevice_grfc_coupler *rfdevice_grfc_coupler_p;
  rfc_device_cfg_info_type grfc_coupler_device_info;
  rfdevice_grfc_coupler_rfc_grfc_tbl_band_map_type band_map_grfc_tbl;
    
  /* List of Helper API's used by the driver and not exposed */

  boolean rfdevice_grfc_coupler_get_band_map_grfc_tbl
  (
    rfcom_mode_enum_type mode,
    rfcom_band_type_u band,
    rfdevice_grfc_coupler_rfc_grfc_tbl_type **rfc_grfc_tbl
  );

  boolean rfdevice_grfc_coupler_process_grfc_tbl
  ( 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing,
    rfdevice_grfc_coupler_rfc_grfc_tbl_type *rfc_cfg_ptr,
    rfdevice_coupling_direction_type direction
  );
};

#endif

#endif /* RFDEVICE_GRFC_COUPLER_H */
