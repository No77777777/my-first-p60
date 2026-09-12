#ifndef UIM_LOGGING_H
#define UIM_LOGGING_H
/*==============================================================================
  FILE:         uim_logging.h

  OVERVIEW:     FIle conatins the inclusions and definitions needed for
                logging related functions.

  DEPENDENCIES: N/A

                   Copyright (c) 2014 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
                   All Rights Reserved.
                   QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/

/*=============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_logging.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       ---------------------------------------------------------
08/30/17    gm         Added control byte support in 0x19B7
05/09/17    gm         Boot-up APDU logging
02/17/17    gm         Masking sensitive information
11/30/16    gm         Store response data in buffer and then commit
10/14/16    ku         APDU Logging for extended log packets
06/15/16    gm         Stub out Hotswap related functionality
06/08/16    ks         Reduce Timeout_info_logging buffer size
09/25/15    sam        Conversion of globals to unsigned data types which 
                       cannot have negative values
09/25/15    sam        Remove the logic to store timeout info in EFS
09/10/15    na         Restricting EFS APDU Logging with Feature
02/23/15    ks         Fix with APDU logging in EFS during efs clean-up
12/10/14    na         Making APDU Logging file instance based
04/29/14    nmb        Correct LOG_UIM_MAX_CHARS to pass buffer storage checks
01/23/14    yk         The first revision
==============================================================================*/


/* Number of UIM GPIOs as inputs upon timeout w/recovery disabled*/
#define UIM_NUM_TIMEOUT_INFO                                    2

#define UIM_EFSLOG_MAX_SAVES_TO_EFS 100

/* Max number of bytes can be logged */
#define LOG_UIM_MAX_CHARS           246

/* Time stamp size */
#define LOG_UIM_TSTAMP_SIZE         8

/* EFS LOG File SIZE & RAM Buffer Size */
#define UIM_EFSLOG_FILE_SIZE        1500
#define UIM_EFSLOG_MAX_BUFFER_SIZE  1000
#define UIM_EFSLOG_ASCII_CR         0x0D
#define UIM_EFSLOG_ASCII_LF         0x0A

/* Max size for extended logging */
#define UIM_EXT_LOG_MAX_SIZE        270

/* Attribute of followed data */
typedef enum {
  UIM_LOG_ATTRIB_INIT  = 0x00, /* Init value */
  UIM_LOG_TSTAMP       = 0x01, /* Data is time stamp */
  UIM_LOG_TX_DATA      = 0x10, /* Data is ME to UIM */
  UIM_LOG_TX_REPEAT    = 0x30, /* Re-sent data from ME to UIM */
  UIM_LOG_RX_DATA      = 0x80, /* Data is UIM to ME */
  UIM_LOG_RX_REPEAT    = 0xC0  /* Re-sent data from UIM to ME */
} uim_log_attrib_type;

/* Message types of extended log packets
    values shall not change, as they are used by DIAG */
typedef enum {
  UIM_LOG_COLD_RESET   = 0x01,
  UIM_LOG_WARM_RESET   = 0x02,
  UIM_LOG_POWER_OFF    = 0x03,
  UIM_LOG_TX           = 0x04,
  UIM_LOG_RX           = 0x05,
  UIM_LOG_PPS_TX       = 0x06,
  UIM_LOG_PPS_RX       = 0x07,
  UIM_LOG_ATR_RX       = 0x08,
  UIM_LOG_MAX  
}uim_log_message_type;

/* Voltage of extended log packets */ 
typedef enum {
  UIM_LOG_VOLTAGE_0P0   = 0x00, /* 0.0 V */
  UIM_LOG_VOLTAGE_1P8   = 0x01, /* 1.8 V */
  UIM_LOG_VOLTAGE_3P0   = 0x02  /* 3.0 V */
}uim_log_voltage_type;

/* Frequency of extended log packet */
typedef enum {
  UIM_LOG_CLOCK_0_MHz    = 0x00,  /* 0.00 MHz */
  UIM_LOG_CLOCK_3P84_MHz = 0x01,  /* 3.84 MHz */
  UIM_LOG_CLOCK_4P8_MHz  = 0x02   /* 4.80 MHz */
}uim_log_clock_type;

typedef enum
{
  UIM_FIRST_N_APDU = 0,
  UIM_LAST_N_APDU
}uim_boot_up_apdu_log_cache_mechanism_type;

/* Define data structure */
LOG_RECORD_DEFINE(LOG_UIM_DATA_C)
  byte  length;                                           /* Actual data length */
  byte  data[LOG_UIM_MAX_CHARS + LOG_UIM_TSTAMP_SIZE];    /* APDU data packets */
LOG_RECORD_END

typedef LOG_UIM_DATA_C_type uim_log_data_type;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
/* Extended log data structure */
LOG_RECORD_DEFINE(LOG_UIM_APDU_C)   
  uint8      version;                     /* Version */
  uint16     seq_number;                  /* Sequence number */
  uint8      slot_id;                     /* Slot Id */
  uint8      msg_type;                    /* Message type */
  uint8      control_byte;                /* Control byte */  
  uint8      rfu[4];                      /* Reserved for future */
  uint16     data_length;                 /* Data length of log Packet */
  uint8      data[UIM_EXT_LOG_MAX_SIZE];  /* Data or APDU */
LOG_RECORD_END
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/* Extended log type */
typedef LOG_UIM_APDU_C_type  uim_ext_log_data_type;

typedef struct
{
  uim_boot_up_apdu_log_cache_mechanism_type         cache_mechanism;
  uint16                                            num_boot_up_apdu_log;
}uim_boot_up_apdu_log_config_type;

typedef struct
{
  uim_log_attrib_type     attrib;
  uim_instance_enum_type  slot_id;
  unsigned char           the_char;
} uim_log_char_type;

typedef struct uim_boot_up_apdu_log_note_t
{
  qword                                               timestamp;
  uim_log_message_type                                message_type;
  uint16                                              seq_number;
  uint8                                               slot_id;  
  struct uim_boot_up_apdu_log_note_t                 *next_ptr;
  uint16                                              apdu_data_len;  
  /* While allocating, we will not allocate whole memory. 
     Will allocate only required memory */
  uint8                                               apdu_data[UIM_EXT_LOG_MAX_SIZE];  
} uim_boot_up_apdu_log_node;

typedef struct
{
  rex_crit_sect_type                   boot_up_apdu_log_cache_crit_section;
  uim_boot_up_apdu_log_config_type     boot_up_apdu_log_config;
  uim_boot_up_apdu_log_node           *head_node;
  uim_boot_up_apdu_log_node           *tail_node;
  uint16                               num_node;
  boolean                              is_cache_commit_done;
} uim_boot_up_apdu_log_cache;

/* Type for log mask used for apdu logging*/
typedef enum
{
  INVALID_LOGGING_MASK = 0,
  SINGLE_SLOT_LOGGING_MASK_1098,
  MULTI_SLOT_LOGGING_MASK_14CE
}uimLogMaskType;

typedef struct
{
  uimLogMaskType  log_mask;
}uim_apdu_log_mask_type;

typedef struct {
  char    *data;
  uint16  buffer_size;
} uim_efslog_buffer_type;

typedef struct
{
  uint8     buffer[UIM_EXT_LOG_MAX_SIZE];
  uint16    buffer_offset;
} uim_ext_log_buffer_data_type;

/**
 * Put a byte to APDU log buf
 *
 * @param attr Attribute of logging data
 * @param ch Data to be logged.
 * @param uim_ptr Pointer to the global data common.
 */
void uim_log_put_byte(uim_log_attrib_type attr,
                      char ch,
                      uim_instance_global_type *uim_ptr);

/**
 * Put a time stamp then logging all data in APDU log bug.
 *
 * @param uim_ptr
 */
void uim_log_put_last_byte(uim_instance_global_type *uim_ptr);

/**
 * This function put power off data along with attrib byte to the
 * buffer
 *
 * @param msg_type Message type of logging data
 * @param uim_ptr  Pointer to the global data common.
 */
void uim_log_put_power_off_data(const uim_instance_global_type *uim_ptr);

/**
 * This function put reset data along with attrib byte to the
 * buffer
 *
 * @param is_cold_reset Reset type of logging data
 * @param uim_ptr       Pointer to the global data common.
 */ 
void uim_log_put_reset_data(boolean                          is_cold_reset,
                            const uim_instance_global_type  *uim_ptr);

/**
 * This function puts ATR recieved from the card to the buffer
 *
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param uim_ptr        Pointer to the global data common.
 */
void uim_log_put_atr_data(uint8                           *buffer,
                          uint16                           buffer_len,                           
                          const uim_instance_global_type  *uim_ptr);

/**
 * This function puts PPS to the buffer
 *
 * @param is_tx          Type of PPS data whether recieved from card or send to card
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param uim_ptr        Pointer to the global data common.
 */

void uim_log_put_pps_data(boolean                          is_tx, 
                          uint8                           *buffer,
                          uint16                           buffer_len,  
                          const uim_instance_global_type  *uim_ptr);

/**
 * This function put TX RX data to the
 * buffer
 *
 * @param is_tx          APDU Data depending on whether the packet is transmitted or recieved
 * @param is_commit      Data to commit.
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param uim_ptr        Pointer to the global data common.
 */
void uim_log_put_apdu_data(boolean                          is_tx,
                           boolean                          is_commit,
                           uint8                           *buffer,
                           uint16                           buffer_len,
                           uim_instance_global_type        *uim_ptr);

/**
 * This function commit all the logs in cache to DIAG
 *
 * @param uim_ptr        Pointer to the global data common.
 */
void uim_log_boot_up_commit(void);

/**
 * This file establishes the file in EFS to which EFSLOG will
 * write. The local EFSLOG buffer cannot be purged until this
 * function has completed successfully. This function will also
 * establish the local EFS buffer
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
boolean uim_efslog_init( uim_instance_global_type *uim_ptr );

/**
 * This function takes a character to be stored to the buffer
 * and an attribute,only if EFS Logging is currently in
 * progress. It converts the character to ascii, and checks the
 * attribute to see if it differs from the previous character's
 * attribute. If it does not, then it simply places the ASCII
 * value of the character in the next space in the buffer (One
 * character will become two ASCII bytes).  If the attribute is
 * different, it will place a new line character '\n' followed
 * by the attribute tag, a colon, and then the character (in
 * ASCII format).
 *
 * @param attribute Attribute of logging data
 * @param character Data to be logged.
 * @param uim_ptr Pointer to the global data common.
 */
void uim_efslog_apdu_to_buffer(
  uim_instance_global_type *uim_ptr
);

/**
 * This function gets called when a the local logging buffer
 * reaches a certain threashold, and we want to purge this data
 * to the EFS file.  This makes a blocking call to efs_write.
 * First, we copy into the secondary buffer and then write the
 * contents of the secondary buffer to EFS.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_efslog_save_buffer_to_efs( uim_instance_global_type *uim_ptr );

/**
 * Cleans up EFSLOG when complete
 *
 * @param uim_ptr
 */
void uim_efslog_clean_up( uim_instance_global_type *uim_ptr);

/**
 * Processing of PURGE SIGNAL
 *
 * @param uim_ptr
 */
void uim_efslog_process_purge_sig(uim_instance_global_type *uim_ptr);


#endif /* UIM_LOGGING_H */
