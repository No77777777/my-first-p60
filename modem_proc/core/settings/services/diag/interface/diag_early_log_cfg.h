#ifndef DIAG_EARLY_LOG_CFG_H
#define DIAG_EARLY_LOG_CFG_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      DIAG Bootup Log Configuration

GENERAL DESCRIPTION
  DAL Device Configuration support for enabling diag boot up logs

Copyright (c) 2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*==========================================================================

                        EDIT HISTORY FOR MODULE
$Header: //components/rel/core.mpss/10.0/settings/services/diag/interface/diag_early_log_cfg.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
04/29/18   ph      Enable diag boot up logging.
04/29/18   ph      Created the new file.

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*===========================================================================

                     EXTERNAL DATA DECLARATIONS

===========================================================================*/
/* Enum for early logging control */
typedef enum
{
  DIAG_EARLY_LOGGING_DISABLED                 = 0x0,
  DIAG_EARLY_LOGGING_ENABLED                  = 0x1
} diag_early_log_control_type;

/* Enum for early logging source */
typedef enum
{
  DIAG_EARLY_LOG_CONFIG_SRC_NONE   = 0,
  DIAG_EARLY_LOG_CONFIG_SRC_DEVCFG = 1,
  DIAG_EARLY_LOG_CONFIG_SRC_NV     = 2
}diag_early_log_src_type;

/* Diag early message config type */
typedef struct diag_early_msg_config_t diag_early_msg_config;

/* Time in milliseconds to set the early logging timer */
#define DIAG_EARLY_LOG_TIMER_LEN         50000

struct diag_early_msg_config_t
{
   uint32 ssid; /* SSID of the MSG */
   uint32 level; /* Level of the MSG */
};

#endif /* DIAG_EARLY_LOG_CFG_H */
