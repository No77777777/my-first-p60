
/*
WARNING: This file is auto-generated.

Generated using: pa_autogen.pl
Generated from:  2.1.82 of RFDevice_PA.xlsm
*/

/*=============================================================================

          R F C     A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

Copyright (c) 2009 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //Commercial/MPSS.DI.2.0.1.c1/Main/modem_proc/rfdevice_pa/src/rfdevice_pa_nz5627g_data_ag.cpp#2 $ 


=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include "rfdevice_pa_nz5627g_data_ag.h"


#define RFDEVICE_PA_NZ5627G_NUM_PORTS 18

#define RFDEVICE_PA_NZ5627G_PA_SET_BIAS_NUM_REGS 2
static uint8 rfdevice_pa_nz5627g_pa_set_bias_regs[RFDEVICE_PA_NZ5627G_PA_SET_BIAS_NUM_REGS] =  {0x01,0x02, };
static int16 rfdevice_pa_nz5627g_pa_set_bias_data[RFDEVICE_PA_NZ5627G_NUM_PORTS][4][RFDEVICE_PA_NZ5627G_PA_SET_BIAS_NUM_REGS] =
{
  { /* PORT NUM: 0 MB1 B3*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x55,RF_REG_INVALID, },  /* PA Range: 3 */     
  },  
  { /* PORT NUM: 1 MB2 B1*/
    { 0x66,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x55,RF_REG_INVALID, },  /* PA Range: 3 */        
  },
  { /* PORT NUM: 2 MB3 B4*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x55,RF_REG_INVALID, },  /* PA Range: 3 */       
  },
  { /* PORT NUM: 3 MB4 B2*/
    { 0x66,RF_REG_INVALID,},  /* PA Range: 0 */
    { 0x66,RF_REG_INVALID,},  /* PA Range: 1 */
    { 0x66,RF_REG_INVALID,},  /* PA Range: 2 */
    { 0x55,RF_REG_INVALID,},  /* PA Range: 3 */      
  },
  { /* PORT NUM: 4 MB5 B34&B39*/
    { 0x66,RF_REG_INVALID,},  /* PA Range: 0 */
    { 0x66,RF_REG_INVALID,},  /* PA Range: 1 */
    { 0x66,RF_REG_INVALID,},  /* PA Range: 2 */
    { 0x55,RF_REG_INVALID,},  /* PA Range: 3 */  
  },
  { /* PORT NUM: 5 HB1 B38&41*/
    { 0xFF,0x00, },  /* PA Range: 0 */
    { 0xFF,0x00, },  /* PA Range: 1 */
    { 0xFF,0x00, },  /* PA Range: 2 */
    { 0x99,0x00, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 6 HB2 B40*/
    { 0xFF,0x00, },  /* PA Range: 0 */
    { 0xFF,0x00, },  /* PA Range: 1 */
    { 0xFF,0x00, },  /* PA Range: 2 */
    { 0x99,0x00, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 7 HB3 B40*/
    { 0xFF,0x00, },  /* PA Range: 0 */
    { 0xFF,0x00, },  /* PA Range: 1 */
    { 0xFF,0x00, },  /* PA Range: 2 */
    { 0x99,0x00, },  /* PA Range: 3 */        
  },
  { /* PORT NUM: 8 HB4 B7*/
    { 0xAA,0x00, },  /* PA Range: 0 */
    { 0xAA,0x00, },  /* PA Range: 1 */
    { 0xAA,0x00, },  /* PA Range: 2 */
    { 0x88,0x00, },  /* PA Range: 3 */      
  },
  { /* PORT NUM: 9 LB1 B28B*/
    { 0x88,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x88,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x88,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */    
  },
  { /* PORT NUM: 10 LB2 B28A*/
    { 0x88,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x88,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x88,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 11 LB3 B20*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */ 
  },
  { /* PORT NUM: 12 LB4 B8*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */ 
  },
  { /* PORT NUM: 13 LB5 B5*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 14 LB6 B*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 15 L1 LB1*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 16 L2 LB4*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 17 L1 LB2*/
    { 0x77,RF_REG_INVALID, },  /* PA Range: 0 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 1 */
    { 0x77,RF_REG_INVALID, },  /* PA Range: 2 */
    { 0x66,RF_REG_INVALID, },  /* PA Range: 3 */     
  },
};


#define RFDEVICE_PA_NZ5627G_PA_SET_RANGE_NUM_REGS 1
static uint8 rfdevice_pa_nz5627g_pa_set_range_regs[RFDEVICE_PA_NZ5627G_PA_SET_RANGE_NUM_REGS] =  {0x00, };
static int16 rfdevice_pa_nz5627g_pa_set_range_data[RFDEVICE_PA_NZ5627G_NUM_PORTS][4][RFDEVICE_PA_NZ5627G_PA_SET_RANGE_NUM_REGS] =
{
  { /* PORT NUM: 0   MB1 B3*/
    { 0x34, },  /* PA Range: 0 */
    { 0x34, },  /* PA Range: 1 */
    { 0x34, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */    
  },
  { /* PORT NUM: 1   MB2 B1*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3C, },  /* PA Range: 1 */
    { 0x3C, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */    
  },
  { /* PORT NUM: 2  MB3 B4*/
    { 0x44, },  /* PA Range: 0 */
    { 0x44, },  /* PA Range: 1 */
    { 0x44, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 3   MB4 B2*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4C, },  /* PA Range: 1 */
    { 0x4C, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 4   MB5 NC*/
    { 0x54, },  /* PA Range: 0 */
    { 0x54, },  /* PA Range: 1 */
    { 0x54, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 5   HB1 B38&41*/
    { 0x5C, },  /* PA Range: 0 */
    { 0x5C, },  /* PA Range: 1 */
    { 0x5C, },  /* PA Range: 2 */
    { 0x5E, },  /* PA Range: 3 */    
  },
  { /* PORT NUM: 6   HB2 B40*/
    { 0x64, },  /* PA Range: 0 */
    { 0x64, },  /* PA Range: 1 */
    { 0x64, },  /* PA Range: 2 */
    { 0x66, },  /* PA Range: 3 */    
  },
  { /* PORT NUM: 7   HB3 B40*/
    { 0x6C, },  /* PA Range: 0 */
    { 0x6C, },  /* PA Range: 1 */
    { 0x6C, },  /* PA Range: 2 */
    { 0x6E, },  /* PA Range: 3 */    
  },
  { /* PORT NUM: 8   HB4 B7*/
	  { 0x74, },  /* PA Range: 0 */
    { 0x74, },  /* PA Range: 1 */
    { 0x74, },  /* PA Range: 2 */
    { 0x76, },  /* PA Range: 3 */      
  },
  { /* PORT NUM: 9   LB1   L2->LB1*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0D, },  /* PA Range: 1 */
    { 0x0D, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 10   TX_B28A*/ /*P60 L1 --> LB2*/
    { 0x14, },  /* PA Range: 0 */
    { 0x14, },  /* PA Range: 1 */
    { 0x14, },  /* PA Range: 2 */
    { 0x16, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 11  TX_B20*/ /*P60 L1 --> LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1C, },  /* PA Range: 1 */
    { 0x1C, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */     
  },
  { /* PORT NUM: 12   LB4  L2->LB4*/
    { 0x25, },  /* PA Range: 0 */
    { 0x25, },  /* PA Range: 1 */
    { 0x25, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */    
  },
  { /* PORT NUM: 13   LB5 B5*/ /*P60 L1 --> LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2C, },  /* PA Range: 1 */
    { 0x2C, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */	
  },
  { /* PORT NUM: 14   LB6 B5*/ /*P60 L1 --> LB6*/
    { 0x7C, },  /* PA Range: 0 */
    { 0x7C, },  /* PA Range: 1 */
    { 0x7C, },  /* PA Range: 2 */
    { 0x7E, },  /* PA Range: 3 */	
  },
  { /* PORT NUM: 15   L1 LB1*/
    { 0x8D, },  /* PA Range: 0 */
    { 0x8D, },  /* PA Range: 1 */
    { 0x8D, },  /* PA Range: 2 */
    { 0x8F, },  /* PA Range: 3 */	
  },
  { /* PORT NUM: 16   L2 LB4*/
    { 0xA5, },  /* PA Range: 0 */
    { 0xA5, },  /* PA Range: 1 */
    { 0xA5, },  /* PA Range: 2 */
    { 0xA7, },  /* PA Range: 3 */	
  },
  { /* PORT NUM: 17   L1 LB2*/
    { 0x94, },  /* PA Range: 0 */
    { 0x94, },  /* PA Range: 1 */
    { 0x94, },  /* PA Range: 2 */
    { 0x96, },  /* PA Range: 3 */	
  },
};


#define RFDEVICE_PA_NZ5627G_PA_ON_NUM_REGS 1
static uint8 rfdevice_pa_nz5627g_pa_on_regs[RFDEVICE_PA_NZ5627G_PA_ON_NUM_REGS] =  {0x00 /*Warning: Not Specified*/, };
static int16 rfdevice_pa_nz5627g_pa_on_data[RFDEVICE_PA_NZ5627G_NUM_PORTS][RFDEVICE_PA_NZ5627G_PA_ON_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 1 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 2 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 3 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 4 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 5 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 6 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 7 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 8 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 9 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 10 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 11 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 12 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 13 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 14 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 15 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 16 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 17 */
    RF_REG_INVALID, 
  },
};


#define RFDEVICE_PA_NZ5627G_PA_OFF_NUM_REGS 1
static uint8 rfdevice_pa_nz5627g_pa_off_regs[RFDEVICE_PA_NZ5627G_PA_OFF_NUM_REGS] =  {0x00, };
static int16 rfdevice_pa_nz5627g_pa_off_data[RFDEVICE_PA_NZ5627G_NUM_PORTS][RFDEVICE_PA_NZ5627G_PA_OFF_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x02, 
  },
  { /* PORT NUM: 130 */
    0x02, 
  },
  { /* PORT NUM: 131 */
    0x02, 
  },
  { /* PORT NUM: 132 */
    0x02, 
  },
  { /* PORT NUM: 133 */
    0x02, 
  },
  { /* PORT NUM: 134 */
    0x02, 
  },
  { /* PORT NUM: 135 */
    0x02, 
  },
  { /* PORT NUM: 136 */
    0x02, 
  },
  { /* PORT NUM: 137 */
    0x02, 
  },
  { /* PORT NUM: 138 */
    0x02, 
  },
  { /* PORT NUM: 139 */
    0x02, 
  },
  { /* PORT NUM: 140 */
    0x02, 
  },
  { /* PORT NUM: 141 */
    0x02, 
  },
  { /* PORT NUM: 142 */
    0x02, 
  },
  { /* PORT NUM: 143 */
    0x02, 
  },
  { /* PORT NUM: 15 */
    0x02, 
  },
  { /* PORT NUM: 16 */
    0x02, 
  },
  { /* PORT NUM: 17 */
    0x02, 
  },
};


#define RFDEVICE_PA_NZ5627G_PA_TRIGGER_NUM_REGS 1
static uint8 rfdevice_pa_nz5627g_pa_trigger_regs[RFDEVICE_PA_NZ5627G_PA_TRIGGER_NUM_REGS] =  {0x1C, };
static int16 rfdevice_pa_nz5627g_pa_trigger_data[RFDEVICE_PA_NZ5627G_NUM_PORTS][RFDEVICE_PA_NZ5627G_PA_TRIGGER_NUM_REGS] =
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
  { /* PORT NUM: 8 */
    0x07, 
  },
  { /* PORT NUM: 9 */
    0x07, 
  },
  { /* PORT NUM: 10 */
    0x07, 
  },
  { /* PORT NUM: 11 */
    0x07, 
  },
  { /* PORT NUM: 12 */
    0x07, 
  },
  { /* PORT NUM: 13 */
    0x07, 
  },
  { /* PORT NUM: 14 */
    0x07, 
  },
  { /* PORT NUM: 15 */
    0x07, 
  },
  { /* PORT NUM: 16 */
    0x07, 
  },
  { /* PORT NUM: 17 */
    0x07, 
  },
};

/* singleton instance ptr */
rfdevice_pa_data *rfdevice_pa_nz5627g_data_ag::rfdevice_pa_nz5627g_data_ptr = NULL;

rfdevice_pa_data * rfdevice_pa_nz5627g_data_ag::get_instance()
{
  if (rfdevice_pa_nz5627g_data_ptr == NULL)
  {
    rfdevice_pa_nz5627g_data_ptr = (rfdevice_pa_data *)new rfdevice_pa_nz5627g_data_ag();
  }
  return( (rfdevice_pa_data *)rfdevice_pa_nz5627g_data_ptr);
}

//constructor
rfdevice_pa_nz5627g_data_ag::rfdevice_pa_nz5627g_data_ag()
  :rfdevice_pa_data()
{
}

//destructor
rfdevice_pa_nz5627g_data_ag::~rfdevice_pa_nz5627g_data_ag()
{
  rfdevice_pa_nz5627g_data_ptr = NULL;
}

boolean rfdevice_pa_nz5627g_data_ag::settings_data_get( rfdevice_pa_cfg_params_type *cfg, 
                                                          rfdevice_pa_reg_settings_type *settings)
{
  boolean ret_val = FALSE;

  if (NULL == settings || NULL == cfg)
  {
    return FALSE;
  }

  if (cfg->port >= RFDEVICE_PA_NZ5627G_NUM_PORTS )
  {
    settings->addr = NULL;
    settings->data = NULL;
    settings->num_regs = 0;
    return FALSE;
  }

  if ( (cfg->req == RFDEVICE_PA_SET_BIAS_DATA) && (cfg->pa_range < 4) )
  {
    settings->addr = &(rfdevice_pa_nz5627g_pa_set_bias_regs[0]);
    settings->data = &(rfdevice_pa_nz5627g_pa_set_bias_data[cfg->port][cfg->pa_range][0]);
    settings->num_regs = RFDEVICE_PA_NZ5627G_PA_SET_BIAS_NUM_REGS;
    settings->bias_reg_idx = 0;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_SET_RANGE_DATA) && (cfg->pa_range < 4) )
  {
    settings->addr = &(rfdevice_pa_nz5627g_pa_set_range_regs[0]);
    settings->data = &(rfdevice_pa_nz5627g_pa_set_range_data[cfg->port][cfg->pa_range][0]);
    settings->num_regs = RFDEVICE_PA_NZ5627G_PA_SET_RANGE_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_ON_DATA) )
  {
    settings->addr = &(rfdevice_pa_nz5627g_pa_on_regs[0]);
    settings->data = &(rfdevice_pa_nz5627g_pa_on_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_NZ5627G_PA_ON_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_OFF_DATA) )
  {
    settings->addr = &(rfdevice_pa_nz5627g_pa_off_regs[0]);
    settings->data = &(rfdevice_pa_nz5627g_pa_off_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_NZ5627G_PA_OFF_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_TRIGGER_DATA) )
  {
    settings->addr = &(rfdevice_pa_nz5627g_pa_trigger_regs[0]);
    settings->data = &(rfdevice_pa_nz5627g_pa_trigger_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_NZ5627G_PA_TRIGGER_NUM_REGS;
    ret_val = TRUE;
  }

  return ret_val;
}

boolean rfdevice_pa_nz5627g_data_ag::sequence_data_get( rfdevice_pa_cfg_params_type *cfg, 
                                                          rfdevice_pa_cmd_seq_type *cmd_seq)
{
  boolean ret_val = FALSE;

  if (NULL == cmd_seq || NULL == cfg)
  {
    return FALSE;
  }

  else if (RFDEVICE_PA_RESET_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_PA_THERM_ON_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_PA_THERM_READ_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_PA_THERM_OFF_DATA == cfg->req)
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

boolean rfdevice_pa_nz5627g_data_ag::device_info_get( rfdevice_pa_info_type *pa_info )
{
  boolean ret_val = FALSE;

  if ( NULL == pa_info )
  {
    return FALSE;
  }
  else
  {
    pa_info->mfg_id = 0x034E;
    pa_info->prd_id = 0x05;
    pa_info->prd_rev = 0;
    pa_info->num_ports = RFDEVICE_PA_NZ5627G_NUM_PORTS;
    pa_info->num_pa_ranges = 4;
    ret_val = TRUE;
  }
  return ret_val;
}

boolean rfdevice_pa_nz5627g_data_ag::get_temperature_lut(int16 **lut_addr, uint8 *lut_size)
{
   boolean ret_val = FALSE;   

   if (lut_addr != NULL)  
   {
      *lut_addr = NULL; 
      *lut_size = 0;
      ret_val = TRUE;
   }

   else 
   {
      ret_val = FALSE;
   }

   return ret_val;
}

