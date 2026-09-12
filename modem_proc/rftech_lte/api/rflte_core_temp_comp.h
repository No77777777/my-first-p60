#ifndef RFLTE_CORE_TEMP_COMP_H
#define RFLTE_CORE_TEMP_COMP_H
/*!
  @file
  rflte_core_temp_comp.h

  @brief
  Provides LTE Temperature Compensation functionality

  @details

*/

/*===========================================================================

Copyright (c) 2010 - 2020 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:27:23 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_core_temp_comp.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/09/20   cv      [CR2769637]Fix mismatch of TX handles during Temp comp
03/03/20   ssl     [CR2635521]Define and un def temp_comp_timer in TX wake up sleep
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
12/07/15   sg      Remove code featurized under RFLTE_BOLT_UL_CA_SUPPORTED
11/06/15   rp      Add API rflte_core_temp_comp_de_init to de-initialize temp comp
11/06/15   rp      Added Txlm Handle based Temp comp algorithm
10/21/15    am     Fix hdet issue wih CDRx timeline optimization changes
10/20/15   ska     Adding support for reading Therm init value(room temp)from NV
06/04/15   utk     Added the band input as it may be needed for temp_comp
05/18/15   JJ      Added ULCA txagc logging support
02/04/14   gvn     Rx temp comp support
04/22/14   cri     Update pin pout for therm reads
08/01/13   gvn     Add Feature Flag for Bolt to avoid fw dependency 
07/24/13   gvn     RSB Cal updates on online mode, iqmc update and IRAT
07/10/13    pv     Support for Therm read in FTM mode.
07/10/13    pv     Support for Therm bin array.
08/29/13   svi     Core Layer Changes to support UL CA
05/24/13   jf      Add static offset for Pin/Pout comp due to temp at CAL 
05/23/13   bar     Added 2D Matrix support for Tx Limiting vs Temp and vs freq
05/21/13   sbm     K sensor changes   
04/24/13   cd      Add support for Therm read as part of Tx Meas Opcode
09/20/12   pl      Moved to internal inc directory from API directory
09/07/12   sbm     Updated rflte_core_temp_comp_update_fw_reads_therm();
05/04/12   pl      New MTPL implemenation
12/20/11   cri     Disable HDET and Thermistor reads for NS stop
10/17/11   pl      Support SMPS temp comp
02/28/11   can     Support for ACQ and TRK mode for LTE Thermistor Reads.
10/12/10   can     Support to introduce linear interpolation for LTE temp. comp.
08/11/10   can     Support for LTE TXAGC logging and code clean-up.
05/10/10    dw     Initial version

============================================================================*/
#include "comdef.h"
#include "nv_items.h"

#ifndef FEATURE_BOLT_MODEM
#include "intf_sys.h"
#include "intf_sys_msg.h"
#endif

#include "rfcommon_locks.h"
#include "rfcom.h"
#include "rfdevice_rxtx_common_class.h"
#include "lm_types.h"

#define RFLTE_CORE_TEMP_COMP_THERM_RD_RESULT_NUM_TRANS 8
#define RFLTE_CORE_THERM_MAX 255
#define RFLTE_CORE_TEMP_BIN_SIZ (RFLTE_CORE_THERM_MAX / (NV_TEMP_TABLE_SIZ-1))

/*--------------------------------------------------------------------------*/
typedef struct
{
    uint16 addr; /*only 8bits for now*/
    uint8 value;
} rflte_core_temp_comp_hw_rd_type;

typedef struct
{
    rflte_core_temp_comp_hw_rd_type therm_rd[RFLTE_CORE_TEMP_COMP_THERM_RD_RESULT_NUM_TRANS];
    uint8 num_trans;
} rflte_core_temp_comp_therm_rd_result_type;

typedef struct
{
    int16 min_temp_comp; /*dB10*/
    int16 max_temp_comp; /*dB10*/
} rflte_core_rx_temp_comp_range_type;


/*--------------------------------------------------------------------------*/
void rflte_core_temp_comp_init
(
  uint32 carrier_idx,
  lm_handle_type txlm_buffer_idx
);
/*--------------------------------------------------------------------------*/
void rflte_core_temp_comp_de_init
(
  uint32 carrier_idx,
  lm_handle_type txlm_buffer_idx
);

void rflte_core_def_temp_comp_loop_timer(uint32 carrier_idx, 
                                         lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_undef_temp_comp_loop_timer(uint32 carrier_idx, 
                                           lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_temp_comp_state_init(uint32 carrier_idx, lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_temp_comp_enable
(
  uint32 carrier_idx, 
  lm_handle_type txlm_buffer_idx,
  boolean enable
);
/*--------------------------------------------------------------------------*/
int16 rflte_core_temp_comp_calc_comp_val(int8 temp_table[], 
                                         uint8 temp_index, 
                                         uint8 temp_rem);
/*--------------------------------------------------------------------------*/
void rflte_core_temp_comp_update_fw_reads_therm(uint8 sub_id,
                                                uint16 therm_raw_value, 
                                                int8 subframe, 
                                                uint32 carrier_idx,
                                                rflte_core_temp_comp_therm_rd_result_type therm_rd_event_result);
/*--------------------------------------------------------------------------*/
void rflte_core_tdet_temp_comp_update_fw_reads(uint8 sub_id,
                                               int8 subframe, 
                                               uint32 carrier_idx,
                                               rflte_core_temp_comp_therm_rd_result_type therm_rd_event_result);
/*--------------------------------------------------------------------------*/
void rflte_core_temp_comp_update(uint8 sub_id,uint32 carrier_idx, rfdevice_rxtx_common_class* device_ptr, lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
uint16  rflte_core_temp_comp_get_room_temp_value_from_nv();
/*--------------------------------------------------------------------------*/
uint16 rflte_core_temp_comp_read_therm(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
#ifndef FEATURE_BOLT_MODEM
void rflte_core_temp_comp_update_iqmc(uint16 therm_raw_value, uint32 carrier_idx, lte_LL1_sys_iqmc_update_req_msg_struct *iqmc_update_req_ptr);
#endif

/* State Machine accessor functions */
void rflte_core_temp_comp_set_timer_enable
(
  lm_handle_type txlm_buffer_idx, 
  boolean enable
);
/* New interface to perform Temp Comp after the state machine has been set*/
void rflte_core_temp_comp_update_post_config(uint8 sub_id,uint32 carrier_idx, lm_handle_type txlm_buffer_idx);

/*--------------------------------------------------------------------------*/
boolean rflte_core_temp_comp_get_update_ready(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_temp_comp_set_update_ready
(
  lm_handle_type txlm_buffer_idx, 
  boolean enable
);
/*--------------------------------------------------------------------------*/
int16 rflte_core_temp_comp_get_value(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint16 rflte_core_temp_comp_get_last_raw(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint16 rflte_core_temp_comp_get_last_scaled(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint8 rflte_core_temp_comp_get_last_index(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint8 rflte_core_temp_comp_get_last_rem(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint16* rflte_core_temp_comp_get_therm_bin_array(uint8 sub_id,uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
int8* rflte_core_temp_comp_get_current_pin_offsets(uint8 sub_id,
                                                   uint32 carrier_index,
                                                   uint8 pa_state);
/*--------------------------------------------------------------------------*/
int8* rflte_core_temp_comp_get_current_pout_offsets(uint8 sub_id,
                                                    uint32 carrier_index,
                                                    uint8 pa_state);
/*--------------------------------------------------------------------------*/
rf_lock_data_type *rflte_core_temp_comp_get_lock(uint8 sub_id,uint32 carrier_idx);

uint8 rflte_core_temp_comp_get_last_index_v2(uint8 sub_id,uint32 carrier_idx, lm_handle_type txlm_buffer_idx);

uint8 rflte_core_temp_comp_get_last_rem_v2(uint8 sub_id,uint32 carrier_idx, lm_handle_type txlm_buffer_idx);


/* Method to scale a raw therm read value*/
uint16 
rflte_core_temp_comp_raw_to_scaled
(
  uint32 carrier_idx,
  uint16 therm_raw_value
);

/* Freq dependent 1D array to hold LTE Static Lim vs freq after interpolating
   over temp from 2D matrix from the NV.
   */
int8 tx_lim_vs_freq_arr[LTE_TX_CAL_CHAN_SIZ];

#endif /* RFLTE_CORE_TEMP_COMP_H */
