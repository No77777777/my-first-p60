/*===========================================================================

                                  R R C L O G

GENERAL DESCRIPTION
 The RRCLOG module consists of functions logging tyhe OTA messages in binary 
 format. These binary files can be used for post analysis.

EXTERNALIZED FUNCTIONS

  tdsrrclog_create_log_file()
    This function creates a new data file for write only.

  tdsrrclog_close_log_file()
    This function closes the file.

  tdsrrclog_log_sig_msg()
    This function writes the SDU header first into binary file and then writes
    the SDU data into binary file.

  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrclog.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/13/11   mkh     Added log version to RRC log packets
07/07/11   mkh     Renamed log pkts to TDSxxx
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdsrrclog.h"
#include "tdsrrclogging.h"
#include "stdio.h"
#include "err.h"
#include "tdsrrcllci.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcllc.h"
#include "ds3gpp_api.h"
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
extern tdsrrc_established_rabs_type tdsrrc_est_rabs;

static FILE *fp;                            /* File pointer for log file */
tdsrrclog_file_status_e_type tdsfile_status = TDSRRCLOG_FILE_CLOSED;
                                            /* The file status is 
                                               initialized */


extern uint32 tdsrrcllc_get_dl_ded_rate(tdsrrc_state_e_type  rrc_state, rb_id_type  rb_id);
extern uint32 tdsrrcllc_get_ul_ded_rate(tdsrrc_state_e_type  rrc_state, rb_id_type  rb_id);

/*===========================================================================

FUNCTION tdsrrclog_create_log_file

DESCRIPTION
  This function creates and opens a new data file for write only.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrclog_status_e_type

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclog_status_e_type tdsrrclog_create_log_file
(    
  const char *filename  
)
{
  uint32 count;
  tdsrrclog_file_hdr_type file_hdr = 
                   {{'R','R','C',' '},
                    TDSFILE_FORMAT_VER,       /* File Format Version */
                    TDSMAJOR_SPEC_RELEASE,    /* Major Specification release */
                    TDSMINOR_SPEC_RELEASE,    /* Minor Specification release */
                    TDSMINOR_MINOR_SPEC_RELEASE,
                                             /* Minor minor Spec release */
                    ' ',                   /* A byte reserved for future
                                               expansion */
                    {' ',' ',' ',' ',' ',' '}
                   };                        /* Reserved for future expansion */
  
  tdsrrclog_status_e_type status = TDSRRCLOG_FAILURE;
   /* Checks whether already log file is created or not.*/
  if(tdsfile_status != TDSRRCLOG_FILE_CLOSED)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Log File is already opened");
    return (status);                        /* If log file already created
                                               it returns the failure status*/
  }
  /* Creates a log file if not already created */
  if((fp = fopen(filename, "wb")) == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot open designated file"); 
    return (status);                        /* Returns failure status if it 
                                               is unable open a file */
  }
  count = fwrite(&file_hdr, 1, sizeof(tdsrrclog_file_hdr_type), fp);   
                                            /* Writes Header for the file  
                                               into the log file */
  if (count != sizeof(tdsrrclog_file_hdr_type))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to write into the File");
    return (status);  
  }

  tdsfile_status = TDSRRCLOG_FILE_OPENED;         /* File status is updated */
  status = TDSRRCLOG_SUCCESS;
  return (status);
} /* tdsrrclog_create_log_file */

/*===========================================================================
FUNCTION tdsrrclog_close_log_file

DESCRIPTION
  This function closes the file.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrclog_status_e_type

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclog_status_e_type tdsrrclog_close_log_file
(    
  void
)
{ 
  tdsrrclog_status_e_type status = TDSRRCLOG_SUCCESS;
  if(tdsfile_status == TDSRRCLOG_FILE_CLOSED)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"The log file is not created. Hence can not be closed");
    status = TDSRRCLOG_FAILURE; 
    return (status);
  }
  if(fp != NULL)
  {
    (void)fclose (fp);
    tdsfile_status = TDSRRCLOG_FILE_CLOSED;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid File Pointer. Can not close");
    status = TDSRRCLOG_FAILURE;
    tdsfile_status = TDSRRCLOG_FILE_CLOSED;
  }
  return (status);
} /* tdsrrclog_close_log_file */

/*===========================================================================
FUNCTION tdsrrclog_log_sig_msg

DESCRIPTION
  This function writes the SDU header first into binary file and then writes
  the SDU data into binary file.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclog_status_e_type tdsrrclog_log_sig_msg
(    
  void  *sdu,                                /* SDU data */
  uint32 sdu_size,                           /* SDU size in bytes */
  uint16 rb_id,                              /* Radio Bearer Id to which SDU
                                                belongs */
  tdsrrclog_log_ch_e_type log_ch                /* Logical Channel type to which
                                                SDU belongs */           
)
{
  int i;                                     /* Loop counter */
  tdsrrclog_status_e_type status;
  uint32 count;
  tdsrrclog_sdu_hdr_type sdu_hdr;
   
  /* Initialize the SDU header */
  sdu_hdr.log_ch = (uint16)log_ch;         /* Logical Channel type - typecast
                                           to the right size since enum value is
                                           assumed to be int size. Keeps lint happy.
                                           Logical channel is between 0 and 4. */
  sdu_hdr.rb_id = rb_id;                   /* Radio Bearer Id */
  sdu_hdr.num_bytes = sdu_size;            /* Size of SDU in bytes */

  for (i = 0; i < TDSTIME_STAMP_LENGTH; i++)
  {
    sdu_hdr.time_stamp[i] = 0;             /* Time stamp for future expansion */
  }

  for (i = 0; i < TDSSDU_HDR_RES_LENGTH; i++)
  {
    sdu_hdr.reserved_bytes[i] = 0;         /* Reserved for future expansion */
  }

  status = TDSRRCLOG_FAILURE;

  if(tdsfile_status == TDSRRCLOG_FILE_CLOSED)     /* Checks whether log is opened */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Log File is not opened. Hence Sig message can not be logged");
    return(status);
  }  

  count = fwrite(&sdu_hdr, 1, sizeof(tdsrrclog_sdu_hdr_type), fp);   
                                            /* Writes Header for the SDU 
                                               into the file */
  if (count != sizeof(tdsrrclog_sdu_hdr_type))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to write into the File");
    return (status);
  }
  
  count = fwrite(sdu, 1, sdu_hdr.num_bytes,fp);   
                                           /* Writes the SDU Data into
                                              the file */
  if (count != sdu_hdr.num_bytes)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to write into the File");
    return (status);
  }
  status = TDSRRCLOG_SUCCESS;
  return (status);
} /* tdsrrclog_log_sig_msg */


/*===========================================================================

FUNCTION RRC_SEND_RB_RATE_INFO_PACKET

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
void tdsrrclog_send_rb_rate_info_packet
(
void
)
{
    tdscdma_rrc_rb_rate_info_log_packet_type  *log_rb_rate_ptr;
    uint32                             log_record_length;    /* Log Record Length */
    uint8 tdsnum_rbs = 0;
    uint32 count, count1, count2;
    tdsrrc_state_e_type rrc_state;
    tdsrrc_proc_e_type                 proc_id;
    tdsrrcllc_oc_process_state_e_type  process_state;  
    tdsrrcllc_oc_set_status_e_type     oc_status;

    oc_status = tdsrrcllc_get_ordered_config_state_and_proc(&proc_id, &process_state);

    switch (oc_status) 
    {
      case TDSOC_SET_FOR_CELL_FACH:
      case TDSOC_SET_FOR_DCH_FACH_TRANS:
        rrc_state = TDSRRC_STATE_CELL_FACH;
        break;
      case TDSOC_SET_FOR_CELL_DCH:
        rrc_state = TDSRRC_STATE_CELL_DCH;
        break;
      case TDSOC_SET_FOR_URA_PCH:
      case TDSOC_SET_FOR_DCH_URA_PCH_TRANS:
      case TDSOC_SET_FOR_FACH_URA_PCH_TRANS:
        rrc_state = TDSRRC_STATE_URA_PCH;
        break;
      case TDSOC_SET_FOR_CELL_PCH:
      case TDSOC_SET_FOR_DCH_CELL_PCH_TRANS:
      case TDSOC_SET_FOR_FACH_CELL_PCH_TRANS:
        rrc_state = TDSRRC_STATE_CELL_PCH;
        break;
      default:
        rrc_state = tdsrrc_get_state();
        break;
    }


    /* Get the total number of RBs */
    for (count = 0; count < MAX_RAB_TO_SETUP; count++) 
    {
      if( (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID) &&
          (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 0))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR," RAB_ID %d,tdsnum_rbs %d",tdsrrc_est_rabs.rabs[count].rab_id,tdsrrc_est_rabs.rabs[count].num_rbs_for_rab);
        tdsnum_rbs = tdsnum_rbs + (uint8)tdsrrc_est_rabs.rabs[count].num_rbs_for_rab;
      }
    }
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Total num rbs %d",tdsnum_rbs);

    /*Calculate the record length */
    log_record_length = FPOS(tdscdma_rrc_rb_rate_info_log_packet_type, rb_rate) +\
                          (tdsnum_rbs * ( sizeof(uint32) + sizeof(uint32) + sizeof(uint32) ) );

       /* Allocate memory here.*/
    log_rb_rate_ptr = (tdscdma_rrc_rb_rate_info_log_packet_type   *)log_alloc(TDSCDMA_RRC_RB_RATE_INFO_LOG_PACKET, log_record_length);

    count2 = 0;
    if (log_rb_rate_ptr != NULL)
    {
      log_rb_rate_ptr->version = TDSRRC_LOG_VERSION;
      log_rb_rate_ptr->num_rbs = tdsnum_rbs;
      /* Get the total number of RBs */
      if (tdsnum_rbs > 0)
      {
        for (count = 0; count < MAX_RAB_TO_SETUP; count ++) 
        {
          if( (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID) &&
              (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 0))
          {
            for (count1 = 0; ((count1 < tdsrrc_est_rabs.rabs[count].num_rbs_for_rab) &&
                              (count1 < MAX_RB_PER_RAB)) ; count1++) 
            {
              log_rb_rate_ptr->rb_rate[count2].rb_id = tdsrrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id;
              log_rb_rate_ptr->rb_rate[count2].dl_rb_rate = tdsrrcllc_get_dl_ded_rate(
                rrc_state, (uint16)tdsrrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id);
              log_rb_rate_ptr->rb_rate[count2].ul_rb_rate = tdsrrcllc_get_ul_ded_rate(
                rrc_state, (uint16)tdsrrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id); 
              if (RRC_PS_DOMAIN_CN_ID == tdsrrc_est_rabs.rabs[count].cn_domain)
              {
                ds_tdscdma_configure_ul_data_rate(tdsrrc_est_rabs.rabs[count].rab_id,  
                                      log_rb_rate_ptr->rb_rate[count2].ul_rb_rate,
#ifdef FEATURE_DUAL_SIM
                                      tdsrrc_get_as_id()
#else
                                      0
#endif
                                                  );
              }
              count2++;
            }
          }
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_RB_RATE_INFO_LOG_PACKET");
      /* Commit the buffer to log services.*/
      log_commit(log_rb_rate_ptr);
    }
    else
    {
        TDSRRC_MSG2(MSG_LEGACY_LOW,"Dropped TDSCDMA_RRC_RB_RATE_INFO_LOG_PACKET 0x%x,code enabled: %d\n",TDSCDMA_RRC_RB_RATE_INFO_LOG_PACKET,log_status(TDSCDMA_RRC_RB_RATE_INFO_LOG_PACKET));
    }
}


/*=========================================================================*/
