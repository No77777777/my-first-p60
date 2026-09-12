#ifndef RF_REG_DUMP_H
#define RF_REG_DUMP_H

#ifndef FEATURE_TABASCO_MODEM
#include "rf_buffer_intf.h"

#ifdef __cplusplus
extern "C" {
#endif

boolean rf_add_script_to_dump
(  
   void* script, 
   uint8 start_index, 
   uint8 stop_index, 
   rf_buffer_script_type script_type,
   uint32 carrier_index,
   uint32 path,
   uint8* misc_data_ptr,
   uint32 num_misc_data_bytes,
   uint32** reg_dump_handle_ptr
);

boolean rf_deregister_free_reg_dump_tbl
(
   void *reg_dump_tbl_handle
);

boolean rf_initialize_reg_dump(void);


#ifdef __cplusplus
}  // extern "C"
#endif 

#endif
#endif