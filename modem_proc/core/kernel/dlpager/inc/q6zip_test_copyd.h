#pragma once
#include "comdef.h"
extern void q6zip_test_copyd_init (void);
extern void q6zip_test_copyd_fini (void);
/**
 * @brief Handle diagnostic message
 * @param [in] msg Pointer to the message
 * @param [in] len Length (bytes) of the message
 * 
 * @return boolean TRUE if msg was processed successfully, FALSE otherwise 
 */
extern boolean q6zip_test_copyd_handle_diag_msg (uint8 const * msg, uint16 len);
