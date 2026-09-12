
/*
WARNING: This file is auto-generated.

Generated using: asm_autogen.pl
Generated from:  v3.0.118 of RFDevice_ASM.xlsm
*/

/*=============================================================================

          R F C     A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

Copyright (c) 2009, 2010, 2011, 2012 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/asm/main/1.32/etc/asm_autogen.pl#2 n

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include "rfdevice_asm_rsw8505_qdm2301_data_ag.h"


#define RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS 19

#define RFDEVICE_ASM_RSW8505_QDM2301_ASM_ON_NUM_REGS 2
static uint8 rfdevice_asm_rsw8505_qdm2301_asm_on_regs[RFDEVICE_ASM_RSW8505_QDM2301_ASM_ON_NUM_REGS] =  {0x01, 0x02, };
static int16 rfdevice_asm_rsw8505_qdm2301_asm_on_data[RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS][RFDEVICE_ASM_RSW8505_QDM2301_ASM_ON_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x01, 0x00, 
  },
  { /* PORT NUM: 1 */
    0x02, 0x00, 
  },
  { /* PORT NUM: 2 */
    0x04, 0x00, 
  },
  { /* PORT NUM: 3 */
    0x08, 0x00, 
  },
  { /* PORT NUM: 4 */
    0x10, 0x00, 
  },
  { /* PORT NUM: 5 */
    0x20, 0x00, 
  },
  { /* PORT NUM: 6 */
    0x00, 0x01, 
  },
  { /* PORT NUM: 7 */
    0x00, 0x02, 
  },
  { /* PORT NUM: 8 */
    0x00, 0x04, 
  },
  { /* PORT NUM: 9 */
    0x00, 0x08, 
  },
  { /* PORT NUM: 10 */
    0x00, 0x10, 
  },
  { /* PORT NUM: 11 */
    0x00, 0x20, 
  },
  { /* PORT NUM: 12 */
    0x00, 0x40, 
  },
  { /* PORT NUM: 13 */
    0x00, 0x80, 
  },
  { /* PORT NUM: 14 */
    0x00, 0x05, 
  },
  { /* PORT NUM: 15 */
    0x00, 0x41, 
  },
  { /* PORT NUM: 16 */
    0x00, 0x81, 
  },
  { /* PORT NUM: 17 */
    0x00, 0x90, 
  },
  { /* PORT NUM: 18 */
    0x00, 0x44, 
  },
};


#define RFDEVICE_ASM_RSW8505_QDM2301_ASM_OFF_NUM_REGS 2
static uint8 rfdevice_asm_rsw8505_qdm2301_asm_off_regs[RFDEVICE_ASM_RSW8505_QDM2301_ASM_OFF_NUM_REGS] =  {0x01, 0x02, };
static int16 rfdevice_asm_rsw8505_qdm2301_asm_off_data[RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS][RFDEVICE_ASM_RSW8505_QDM2301_ASM_OFF_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 1 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 2 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 3 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 4 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 5 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 6 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 7 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 8 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 9 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 10 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 11 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 12 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 13 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 14 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 15 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 16 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 17 */
    0x00, 0x00, 
  },
  { /* PORT NUM: 18 */
    0x00, 0x00, 
  },
};


#define RFDEVICE_ASM_RSW8505_QDM2301_ASM_WAKEUP_NUM_REGS 1
static uint8 rfdevice_asm_rsw8505_qdm2301_asm_wakeup_regs[RFDEVICE_ASM_RSW8505_QDM2301_ASM_WAKEUP_NUM_REGS] =  {0x1C, };
static int16 rfdevice_asm_rsw8505_qdm2301_asm_wakeup_data[RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS][RFDEVICE_ASM_RSW8505_QDM2301_ASM_WAKEUP_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x00, 
  },
  { /* PORT NUM: 1 */
    0x00, 
  },
  { /* PORT NUM: 2 */
    0x00, 
  },
  { /* PORT NUM: 3 */
    0x00, 
  },
  { /* PORT NUM: 4 */
    0x00, 
  },
  { /* PORT NUM: 5 */
    0x00, 
  },
  { /* PORT NUM: 6 */
    0x00, 
  },
  { /* PORT NUM: 7 */
    0x00, 
  },
  { /* PORT NUM: 8 */
    0x00, 
  },
  { /* PORT NUM: 9 */
    0x00, 
  },
  { /* PORT NUM: 10 */
    0x00, 
  },
  { /* PORT NUM: 11 */
    0x00, 
  },
  { /* PORT NUM: 12 */
    0x00, 
  },
  { /* PORT NUM: 13 */
    0x00, 
  },
  { /* PORT NUM: 14 */
    0x00, 
  },
  { /* PORT NUM: 15 */
    0x00, 
  },
  { /* PORT NUM: 16 */
    0x00, 
  },
  { /* PORT NUM: 17 */
    0x00, 
  },
  { /* PORT NUM: 18 */
    0x00, 
  },
};


#define RFDEVICE_ASM_RSW8505_QDM2301_ASM_SLEEP_NUM_REGS 1
static uint8 rfdevice_asm_rsw8505_qdm2301_asm_sleep_regs[RFDEVICE_ASM_RSW8505_QDM2301_ASM_SLEEP_NUM_REGS] =  {0x1C, };
static int16 rfdevice_asm_rsw8505_qdm2301_asm_sleep_data[RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS][RFDEVICE_ASM_RSW8505_QDM2301_ASM_SLEEP_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x80, 
  },
  { /* PORT NUM: 1 */
    0x80, 
  },
  { /* PORT NUM: 2 */
    0x80, 
  },
  { /* PORT NUM: 3 */
    0x80, 
  },
  { /* PORT NUM: 4 */
    0x80, 
  },
  { /* PORT NUM: 5 */
    0x80, 
  },
  { /* PORT NUM: 6 */
    0x80, 
  },
  { /* PORT NUM: 7 */
    0x80, 
  },
  { /* PORT NUM: 8 */
    0x80, 
  },
  { /* PORT NUM: 9 */
    0x80, 
  },
  { /* PORT NUM: 10 */
    0x80, 
  },
  { /* PORT NUM: 11 */
    0x80, 
  },
  { /* PORT NUM: 12 */
    0x80, 
  },
  { /* PORT NUM: 13 */
    0x80, 
  },
  { /* PORT NUM: 14 */
    0x80, 
  },
  { /* PORT NUM: 15 */
    0x80, 
  },
  { /* PORT NUM: 16 */
    0x80, 
  },
  { /* PORT NUM: 17 */
    0x80, 
  },
  { /* PORT NUM: 18 */
    0x80, 
  },
};


#define RFDEVICE_ASM_RSW8505_QDM2301_ASM_TRIGGER_NUM_REGS 1
static uint8 rfdevice_asm_rsw8505_qdm2301_asm_trigger_regs[RFDEVICE_ASM_RSW8505_QDM2301_ASM_TRIGGER_NUM_REGS] =  {0x1C, };
static int16 rfdevice_asm_rsw8505_qdm2301_asm_trigger_data[RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS][RFDEVICE_ASM_RSW8505_QDM2301_ASM_TRIGGER_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x01, 
  },
  { /* PORT NUM: 1 */
    0x01, 
  },
  { /* PORT NUM: 2 */
    0x01, 
  },
  { /* PORT NUM: 3 */
    0x01, 
  },
  { /* PORT NUM: 4 */
    0x01, 
  },
  { /* PORT NUM: 5 */
    0x01, 
  },
  { /* PORT NUM: 6 */
    0x01, 
  },
  { /* PORT NUM: 7 */
    0x01, 
  },
  { /* PORT NUM: 8 */
    0x01, 
  },
  { /* PORT NUM: 9 */
    0x01, 
  },
  { /* PORT NUM: 10 */
    0x01, 
  },
  { /* PORT NUM: 11 */
    0x01, 
  },
  { /* PORT NUM: 12 */
    0x01, 
  },
  { /* PORT NUM: 13 */
    0x01, 
  },
  { /* PORT NUM: 14 */
    0x01, 
  },
  { /* PORT NUM: 15 */
    0x01, 
  },
  { /* PORT NUM: 16 */
    0x01, 
  },
  { /* PORT NUM: 17 */
    0x01, 
  },
  { /* PORT NUM: 18 */
    0x01, 
  },
};

/* singleton instance ptr */
rfdevice_asm_data *rfdevice_asm_rsw8505_qdm2301_data_ag::rfdevice_asm_rsw8505_qdm2301_data_ptr = NULL;

rfdevice_asm_data * rfdevice_asm_rsw8505_qdm2301_data_ag::get_instance()
{
  if (rfdevice_asm_rsw8505_qdm2301_data_ptr == NULL)
  {
    rfdevice_asm_rsw8505_qdm2301_data_ptr = (rfdevice_asm_data *)new rfdevice_asm_rsw8505_qdm2301_data_ag();
  }
  return( (rfdevice_asm_data *)rfdevice_asm_rsw8505_qdm2301_data_ptr);
}

//constructor
rfdevice_asm_rsw8505_qdm2301_data_ag::rfdevice_asm_rsw8505_qdm2301_data_ag()
  :rfdevice_asm_data()
{
}

//destructor
rfdevice_asm_rsw8505_qdm2301_data_ag::~rfdevice_asm_rsw8505_qdm2301_data_ag()
{
  rfdevice_asm_rsw8505_qdm2301_data_ptr = NULL;
}

boolean rfdevice_asm_rsw8505_qdm2301_data_ag::settings_data_get( rfdevice_asm_cfg_params_type *cfg, 
                                                          rfdevice_asm_reg_settings_type *settings)
{
  boolean ret_val = FALSE;

  if (NULL == settings || NULL == cfg)
  {
    return FALSE;
  }

  if (cfg->port >= RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS)
  {
    settings->addr = NULL;
    settings->data = NULL;
    settings->num_regs = 0;
    return FALSE;
  }

  if (cfg->req == RFDEVICE_ASM_ON_DATA)
  {
    settings->addr = &(rfdevice_asm_rsw8505_qdm2301_asm_on_regs[0]);
    settings->data = &(rfdevice_asm_rsw8505_qdm2301_asm_on_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_RSW8505_QDM2301_ASM_ON_NUM_REGS;
    ret_val = TRUE;
  }

  else if (cfg->req == RFDEVICE_ASM_OFF_DATA)
  {
    settings->addr = &(rfdevice_asm_rsw8505_qdm2301_asm_off_regs[0]);
    settings->data = &(rfdevice_asm_rsw8505_qdm2301_asm_off_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_RSW8505_QDM2301_ASM_OFF_NUM_REGS;
    ret_val = TRUE;
  }

  else if (cfg->req == RFDEVICE_ASM_WAKEUP_DATA)
  {
    settings->addr = &(rfdevice_asm_rsw8505_qdm2301_asm_wakeup_regs[0]);
    settings->data = &(rfdevice_asm_rsw8505_qdm2301_asm_wakeup_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_RSW8505_QDM2301_ASM_WAKEUP_NUM_REGS;
    ret_val = TRUE;
  }

  else if (cfg->req == RFDEVICE_ASM_SLEEP_DATA)
  {
    settings->addr = &(rfdevice_asm_rsw8505_qdm2301_asm_sleep_regs[0]);
    settings->data = &(rfdevice_asm_rsw8505_qdm2301_asm_sleep_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_RSW8505_QDM2301_ASM_SLEEP_NUM_REGS;
    ret_val = TRUE;
  }

  else if (cfg->req == RFDEVICE_ASM_TRIGGER_DATA)
  {
    settings->addr = &(rfdevice_asm_rsw8505_qdm2301_asm_trigger_regs[0]);
    settings->data = &(rfdevice_asm_rsw8505_qdm2301_asm_trigger_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_ASM_RSW8505_QDM2301_ASM_TRIGGER_NUM_REGS;
    ret_val = TRUE;
  }

   else
   {
     cfg->ag_script_present = FALSE;
   }
   
  return ret_val;
}

boolean rfdevice_asm_rsw8505_qdm2301_data_ag::sequence_data_get( rfdevice_asm_cfg_params_type *cfg, 
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

boolean rfdevice_asm_rsw8505_qdm2301_data_ag::get_temperature_lut(int16 **lut_addr, uint8 *lut_size)
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

boolean rfdevice_asm_rsw8505_qdm2301_data_ag::device_info_get( rfdevice_asm_info_type *asm_info )
{
  boolean ret_val = FALSE;

  if ( NULL == asm_info )
  {
    return FALSE;
  }
  else
  {
    asm_info->mfg_id = 0x217;
    asm_info->prd_id = 0x49A;
    asm_info->prd_rev = 1;
    asm_info->num_ports = RFDEVICE_ASM_RSW8505_QDM2301_NUM_PORTS;
    asm_info->separate_regs_for_ca = FALSE;	
    ret_val = TRUE;
  }
  return ret_val;
}

