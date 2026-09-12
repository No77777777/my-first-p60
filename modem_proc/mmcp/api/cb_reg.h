#ifndef cb_reg_h
#define cb_reg_h

/*******************************************************************************
 *  Description:    Cell Broadcast <-> REG interface
 *  ------------
 *  
 *  Comment:
 *  --------
 * 
 *  $Header: //components/rel/mmcp.mpss/7.9.0/api/cb_reg.h#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $
 * 
 *  Modification History:
 *  ---------------------
 *  Date       Vers  Auth  Details
 *  ------     ----  ----  -------
 *  12-Dec-2008   1.0   nagt  Original version
 *
 *
 ******************************************************************************/


#include "sys.h"


/***********************************************************************/
/*                                                                     */
/*                         REG to CB Primitive IDs                      */
/*                                                                     */
/***********************************************************************/



/**********************************************/
/*                                            */
/*         REG_SERVICE_STATE_IND                 */
/*                                            */
/**********************************************/

enum        /* message_id of inter-task messages */
{
   REG_SERVICE_STATE_IND    = 0x00,
   REG_MODE_CHANGE_STATE_IND= 0x01,
   REG_DDS_INFO_IND         = 0x02,
   REG_3GPP2_STATUS_IND   = 0x03
};

typedef struct
{
   IMH_T    message_header;    /* 4 bytes */
   sys_srv_status_e_type service_state ;
   sys_radio_access_tech_e_type active_rat;
   boolean reg_inactive;
   sys_modem_as_id_e_type as_id;
}reg_service_state_ind_T;

typedef struct cb_reg_stat_chgd_req_s
{
  IMH_T    message_header;
  byte     active_subs;
} cb_reg_stat_chgd_req_s_type;

typedef struct cb_reg_dds_info_s
{
  IMH_T    message_header;
  boolean  dds_sub;
  sys_modem_as_id_e_type as_id;
}cb_reg_dds_info_s_type;

typedef struct cb_reg_3gpp2_stack_status_ind_s
{
  IMH_T    message_header;
  boolean  is_stack_active;
  sys_sys_mode_e_type active_mode;
  sys_modem_as_id_e_type as_id;
}cb_reg_3gpp2_stack_status_ind_s_type;

#endif
