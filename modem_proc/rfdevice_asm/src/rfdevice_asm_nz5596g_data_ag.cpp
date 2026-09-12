
/*
WARNING: This file is auto-generated.

Generated using: asm_autogen.pl
Generated from:  v2.3.136 of RFDevice_ASM.xlsm
*/

/*=============================================================================

          R F C     A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

Copyright (c) 2009, 2010, 2011, 2012 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/asm/main/latest/etc/asm_autogen.pl#10 n

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include "rfdevice_asm_nz5596g_data_ag.h"


#define RFDEVICE_ASM_NZ5596G_NUM_PORTS 17

#define RFDEVICE_ASM_NZ5596G_ASM_ON_NUM_REGS 2
static uint8 rfdevice_asm_nz5596g_asm_on_regs[RFDEVICE_ASM_NZ5596G_ASM_ON_NUM_REGS] =  {0x00,0x02 };
static int16 rfdevice_asm_nz5596g_asm_on_data[RFDEVICE_ASM_NZ5596G_NUM_PORTS][RFDEVICE_ASM_NZ5596G_ASM_ON_NUM_REGS] =
{
  { /* PORT NUM: 0 TRX1 *//*B41_TX*/ /* p60: LB --> TRX1*/
     //0x00, // reg0, no use SPDT
     0x30, // reg0, LB_SWOUT
     //0x38, //reg0, HB_SWOUT
     0x10, //reg2
  },
  { /* PORT NUM: 1 TRX2*//*B40_TX*/ /* p60: LB --> TRX2*/
     //0x00, // reg0, no use SPDT
     0x30, // reg0, LB_SWOUT
     //0x38, //reg0, HB_SWOUT
     0x11, //reg2
  },
  { /* PORT NUM: 2 TRX3 *//*B8*/ /* p60: LB --> TRX3*/
     //0x00, // reg0, no use SPDT
     0x30, // reg0, LB_SWOUT
     //0x3B, //reg0, HB_SWOUT
     0x0C, //reg2
  },
  { /* PORT NUM: 3 TRX4*//*B20/17*/
     //0x00, // reg0, no use SPDT
     0x30, // reg0, LB_SWOUT
     //0x3B, //reg0, HB_SWOUT
     0x08, //reg2
  },
  { /* PORT NUM: 4 TRX5 *//*B28a*/ /* p60: HB --> TRX5*/
     //0x00, // reg0, no use SPDT
     //0x30, // reg0, LB_SWOUT
     0x30, //reg0, HB_SWOUT
     0x04, //reg2
  },
  { /* PORT NUM: 5 TRX6*//*B28b*/  /* p60: HB --> TRX6*/
      // 0x00, // reg0, no use SPDT
     //0x33, // reg0, LB_SWOUT
     0x30, //reg0, HB_SWOUT
     0x06, //reg2
  },
  { /* PORT NUM: 6 TRX7*//*B2*/ /* p60: HB --> TRX7*/
     // 0x00, // reg0, no use SPDT
     //0x33, // reg0, LB_SWOUT
     0x30, //reg0, HB_SWOUT
     0x05, //reg2
  },
  { /* PORT NUM: 7 TRX8 *//*B5*/ /* p60: HB --> TRX8*/
     // 0x00, // reg0, no use SPDT
     // 0x00, // reg0, LB_SWOUT
     0x30, //reg0, HB_SWOUT
     0x07, //reg2
  },
  { /* PORT NUM: 8 TRX9 *//*B3*/ /* p60: HB --> TRX9*/
      // 0x00, // reg0, no use SPDT
      //0x33, // reg0, LB_SWOUT
      0x30, //reg0, HB_SWOUT
      0x02, //reg2
  },
  { /* PORT NUM: 9 TRX10 *//*B1*/ /* p60: No --> TRX10*/
      0x00, // reg0, no use SPDT
      //0x33, // reg0, LB_SWOUT
      // 0x30, //reg0, HB_SWOUT
      0x01, //reg2
  },
  { /* PORT NUM: 10 TRX11*//*B4*/
      //0x00, // reg0, not use SPDT
      //0x33, // reg0, LB_SWOUT
      0x30, //reg0, HB_SWOUT
      0x12, //reg2
  },
  { /* PORT NUM: 11 TRX12*//*B7*/
      0x00, // reg0, not use SPDT
      //0x33, // reg0, LB_SWOUT
      //0x3B, //reg0, HB_SWOUT
      0x13, //reg2
  },
  { /* PORT NUM: 12 TRX13 *//*B40_RX*/ /* p60: No --> TRX13*/
      0x00, // reg0, no use SPDT
      //0x33, // reg0, LB_SWOUT
      //0x3B, //reg0, HB_SWOUT
      0x14, //reg2
  },
  { /* PORT NUM: 13 TRX14*//*B41_RX*/ /* p60: No --> TRX14*/
      0x00, // reg0, no use SPDT
      //0x33, // reg0, LB_SWOUT
      //0x3B, //reg0, HB_SWOUT
      0x15, //reg2
  },
  { /* PORT NUM: 14 LB TX*/ /* p60: No --> TRX2*/
      0x00, 0x11, 
  },
  { /* PORT NUM: 15 HB TX*/
      0x00, 0x0B, 
  },
  { /* PORT NUM: 16 LB TX*/
      0x00, 0x09, 
  },

};


#define RFDEVICE_ASM_NZ5596G_ASM_OFF_NUM_REGS 2
static uint8 rfdevice_asm_nz5596g_asm_off_regs[RFDEVICE_ASM_NZ5596G_ASM_OFF_NUM_REGS] =  { 0x00,0x02 };
static int16 rfdevice_asm_nz5596g_asm_off_data[RFDEVICE_ASM_NZ5596G_NUM_PORTS][RFDEVICE_ASM_NZ5596G_ASM_OFF_NUM_REGS] =
{
  { /* PORT NUM: 0 TRX1 */
    0x00, 0x00
  },
  { /* PORT NUM: 1 TRX2 */
    0x00, 0x00
  },
  { /* PORT NUM: 2 TRX3 */
    0x00, 0x00
  },
  { /* PORT NUM: 3 TRX4 */
    0x00, 0x00
  },
  { /* PORT NUM: 4 TRX5 */
    0x00, 0x00
  },
  { /* PORT NUM: 5 TRX6 */
    0x00, 0x00
  },
  { /* PORT NUM: 6 TRX7 */
    0x00, 0x00
  },
  { /* PORT NUM: 7 TRX8 */
    0x00, 0x00
  },
  { /* PORT NUM: 8 TRX9*/
    0x00, 0x00
  },
  { /* PORT NUM: 9 TRX10*/
    0x00, 0x00
  },
  { /* PORT NUM: 10 TRX11*/
    0x00, 0x00
  },
  { /* PORT NUM: 11 TRX12*/
    0x00, 0x00
  },
  { /* PORT NUM: 12 TRX13*/
    0x00, 0x00
  },
  { /* PORT NUM: 13 TRX14*/
    0x00, 0x00
  },
  { /* PORT NUM: 14 LB TX*/
    0x00, 0x00 
  },
  { /* PORT NUM: 15 HB TX*/
    0x00, 0x00
  },
  { /* PORT NUM: 16 LB TX*/
    0x00, 0x00 
  },
  


};


#define RFDEVICE_ASM_NZ5596G_ASM_TRIGGER_NUM_REGS 1
static uint8 rfdevice_asm_nz5596g_asm_trigger_regs[RFDEVICE_ASM_NZ5596G_ASM_TRIGGER_NUM_REGS] =  {0x1C, };
static int16 rfdevice_asm_nz5596g_asm_trigger_data[RFDEVICE_ASM_NZ5596G_NUM_PORTS][RFDEVICE_ASM_NZ5596G_ASM_TRIGGER_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x07, 
  },
  { /* PORT NUM: 1 */
    0x07, 
  },
  { /* PORT NUM: 2 */
    0x07, 
  },
  { /* PORT NUM: 3 */
    0x07, 
  },
  { /* PORT NUM: 4 */
    0x07, 
  },
  { /* PORT NUM: 5 */
    0x07, 
  },
  { /* PORT NUM: 6 */
    0x07, 
  },
  { /* PORT NUM: 7 */
    0x07, 
  },
  { /* PORT NUM: 8  */
    0x07,
  },
  { /* PORT NUM: 9  */
    0x07,
  },
  { /* PORT NUM: 10  */
    0x07,
  },
  { /* PORT NUM: 11  */
    0x07,
  },
  { /* PORT NUM: 12  */
    0x07,
  },
  { /* PORT NUM: 13  */
    0x07,
  },
  { /* PORT NUM: 14  */
    0x07,
  },
  { /* PORT NUM: 15  */
    0x07,
  },
  { /* PORT NUM: 16  */
    0x07,
  },
};

/* singleton instance ptr */
rfdevice_asm_data *rfdevice_asm_nz5596g_data_ag::rfdevice_asm_nz5596g_data_ptr = NULL;

rfdevice_asm_data * rfdevice_asm_nz5596g_data_ag::get_instance()
{
  if (rfdevice_asm_nz5596g_data_ptr == NULL)
  {
    rfdevice_asm_nz5596g_data_ptr = (rfdevice_asm_data *)new rfdevice_asm_nz5596g_data_ag();
  }
  return( (rfdevice_asm_data *)rfdevice_asm_nz5596g_data_ptr);
}

//constructor
rfdevice_asm_nz5596g_data_ag::rfdevice_asm_nz5596g_data_ag()
  :rfdevice_asm_data()
{
}

//destructor
rfdevice_asm_nz5596g_data_ag::~rfdevice_asm_nz5596g_data_ag()
{
  rfdevice_asm_nz5596g_data_ptr = NULL;
}

boolean rfdevice_asm_nz5596g_data_ag::settings_data_get( rfdevice_asm_cfg_params_type *cfg, 
                                                          rfdevice_asm_reg_settings_type *settings)
{
  boolean ret_val = FALSE;

  if (NULL == settings || NULL == cfg)
  {
    return FALSE;
  }

  if (cfg->port >= RFDEVICE_ASM_NZ5596G_NUM_PORTS)
  {
    settings->addr = NULL;
    settings->data = NULL;
    settings->num_regs = 0;
    return FALSE;
  }

  if (cfg->req == RFDEVICE_ASM_ON_DATA)
  {
    settings->addr = &(rfdevice_asm_nz5596g_asm_on_regs[0]);
    settings->data = &(rfdevice_asm_nz5596g_asm_on_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_NZ5596G_ASM_ON_NUM_REGS;
    ret_val = TRUE;
  }

  if (cfg->req == RFDEVICE_ASM_OFF_DATA)
  {
    settings->addr = &(rfdevice_asm_nz5596g_asm_off_regs[0]);
    settings->data = &(rfdevice_asm_nz5596g_asm_off_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_NZ5596G_ASM_OFF_NUM_REGS;
    ret_val = TRUE;
  }

  if (cfg->req == RFDEVICE_ASM_TRIGGER_DATA)
  {
    settings->addr = &(rfdevice_asm_nz5596g_asm_trigger_regs[0]);
    settings->data = &(rfdevice_asm_nz5596g_asm_trigger_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_NZ5596G_ASM_TRIGGER_NUM_REGS;
    ret_val = TRUE;
  }

  return ret_val;
}

boolean rfdevice_asm_nz5596g_data_ag::sequence_data_get( rfdevice_asm_cfg_params_type *cfg, 
                                                          rfdevice_asm_cmd_seq_type *cmd_seq)
{
  boolean ret_val = FALSE;

  if (NULL == cmd_seq || NULL == cfg)
  {
    return FALSE;
  }

  else if (RFDEVICE_ASM_RESET_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_ASM_THERM_ON_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_ASM_THERM_READ_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_ASM_THERM_OFF_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
  }

  return ret_val;
}

boolean rfdevice_asm_nz5596g_data_ag::get_temperature_lut(int16 **lut_addr, uint8 *lut_size)
{
   boolean ret_val = FALSE;   

   if (lut_addr != NULL)  
   {
      *lut_addr = NULL; 
      *lut_size = -1;
      ret_val = TRUE;
   }

   else 
   {
      ret_val = FALSE;
   }

   return ret_val;
}

boolean rfdevice_asm_nz5596g_data_ag::device_info_get( rfdevice_asm_info_type *asm_info )
{
  boolean ret_val = FALSE;

  if ( NULL == asm_info )
  {
    return FALSE;
  }
  else
  {
    asm_info->mfg_id = 0x034E;
    asm_info->prd_id = 0x04;
    asm_info->prd_rev = 0;
    asm_info->num_ports = RFDEVICE_ASM_NZ5596G_NUM_PORTS;
    ret_val = TRUE;
  }
  return ret_val;
}

