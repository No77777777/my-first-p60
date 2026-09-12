
#ifndef __TDSRRC_DEPENDANCY__
#define __TDSRRC_DEPENDANCY__
/*===========================================================================
                         TD RRC dependency

DESCRIPTION

 This module has the definations for the internal macros used while accessing 
Objective system ASN1.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS



Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrc_dependancy.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/11   ps      Checked in changes to remove OSSUINT32
===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define          tdsrrc_DL_DCCH_Message_PDU 1
#define          tdsrrc_UL_DCCH_Message_PDU 2
#define          tdsrrc_DL_CCCH_Message_PDU 3
#define          tdsrrc_UL_CCCH_Message_PDU 4
#define          tdsrrc_PCCH_Message_PDU 5
#define          tdsrrc_DL_SHCCH_Message_PDU 6
#define          tdsrrc_UL_SHCCH_Message_PDU 7
#define          tdsrrc_BCCH_FACH_Message_PDU 8
#define          tdsrrc_BCCH_BCH_Message_PDU 9
#define          tdsrrc_MCCH_Message_PDU 10
#define          tdsrrc_MSCH_Message_PDU 11
#define          tdsrrc_HandoverToUTRANCommand_PDU 12
#define          tdsrrc_InterRATHandoverInfo_PDU 13
#define          tdsrrc_InterRATHandoverInfo_r3_add_ext_IEs_PDU 14
#define          tdsrrc_RRCConnectionSetupComplete_r3_add_ext_IEs_PDU 15
#define          tdsrrc_RRC_FailureInfo_PDU 16
#define          tdsrrc_UECapabilityInformation_r3_add_ext_IEs_PDU 17
#define          tdsrrc_UE_CapabilityContainer_IEs_PDU 18
#define          tdsrrc_UE_RadioAccessCapabilityInfo_PDU 19
#define          tdsrrc_UL_PhysChCapabilityFDD_r6_PDU 20
#define          tdsrrc_GSMCellGroupingList_PDU 21
#define          tdsrrc_UE_Positioning_GANSS_Data_PDU 22
#define          tdsrrc_MasterInformationBlock_PDU 23
#define          tdsrrc_SysInfoType1_PDU 24
#define          tdsrrc_SysInfoType2_PDU 25
#define          tdsrrc_SysInfoType3_PDU 26
#define          tdsrrc_SysInfoType4_PDU 27
#define          tdsrrc_SysInfoType5bis_PDU 28
#define          tdsrrc_SysInfoType6_PDU 29
#define          tdsrrc_SysInfoType7_PDU 30
#define          tdsrrc_SysInfoType8_PDU 31
#define          tdsrrc_SysInfoType9_PDU 32
#define          tdsrrc_SysInfoType10_PDU 33
#define          tdsrrc_SysInfoType11_PDU 34
#define          tdsrrc_SysInfoType11bis_PDU 35
#define          tdsrrc_SysInfoType12_PDU 36
#define          tdsrrc_SysInfoType13_PDU 37
#define          tdsrrc_SysInfoType13_1_PDU 38
#define          tdsrrc_SysInfoType13_2_PDU 39
#define          tdsrrc_SysInfoType13_3_PDU 40
#define          tdsrrc_SysInfoType13_4_PDU 41
#define          tdsrrc_SysInfoType14_PDU 42
#define          tdsrrc_SysInfoType15_PDU 43
#define          tdsrrc_SysInfoType15bis_PDU 44
#define          tdsrrc_SysInfoType15_1_PDU 45
#define          tdsrrc_SysInfoType15_1bis_PDU 46
#define          tdsrrc_SysInfoType15_2_PDU 47
#define          tdsrrc_SysInfoType15_2bis_PDU 48
#define          tdsrrc_SysInfoType15_2ter_PDU 49
#define          tdsrrc_SysInfoType15_3_PDU 50
#define          tdsrrc_SysInfoType15_3bis_PDU 51
#define          tdsrrc_SysInfoType15_4_PDU 52
#define          tdsrrc_SysInfoType15_5_PDU 53
#define          tdsrrc_SysInfoType15_6_PDU 54
#define          tdsrrc_SysInfoType15_7_PDU 55
#define          tdsrrc_SysInfoType15_8_PDU 56
#define          tdsrrc_SysInfoType16_PDU 57
#define          tdsrrc_SysInfoType17_PDU 58
#define          tdsrrc_SysInfoType18_PDU 59
#define          tdsrrc_SysInfoType19_PDU 60
#define          tdsrrc_SysInfoType20_PDU 61
#define          tdsrrc_SysInfoTypeSB1_PDU 62
#define          tdsrrc_SysInfoTypeSB2_PDU 63
#define          tdsrrc_UE_HistoryInformation_PDU 64
#define          tdsrrc_System_Information_Container_PDU 65


#define TDSRRCLLC_TRCH_ID_MAX_VALUE    32


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                          Macro Definitions
** ======================================================================= */

#define TDSRRC_GET_BITS_STRING_LEN_PTR(msg) \
  (msg->numbits)

#define TDSRRC_GET_BITS_PTR_STRING_PTR(msg) \
  (msg->data)
  

#define TDSRRC_GET_OCTET_STRING_LEN(msg) \
  (msg.numocts)

#define TDSRRC_GET_OCTET_STRING_PTR(msg) \
  (msg.data)

#define TDSRRC_MSG_LIST_BITMASK_IE(msg,struct_name,tag) \
  ((msg)->elem[idx].m.tag##Present)
  
#define TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(msg,tag) \
  (msg->elem[idx].m.tag##Present)

#define TDSRRC_CHECK_LIST_MSG_TYPE(msg,OB_type) \
  (msg->elem[idx].t == T_##OB_type)

#define TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(msg,struct_name,tag) \
  (msg->m.tag##Present)

#define TDSRRC_SIB_TYPE(sib_type)\
  (tdsrrc_SIB_Type_##sib_type)

#define TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg,struct_name,tag) \
  (msg.m.tag##Present = 1)

#define TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg,tag) \
    (msg.m.tag##Present = 1)

#define TDSRRC_MSG_COMMON_SET_BITMASK_IE_PTR(msg,struct_name,tag) \
  (msg->m.tag##Present = 1)
    
#define TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2_PTR(msg,tag) \
  (msg->m.tag##Present = 1)

#define TDSRRC_CHECK_COMMON_MSG_TYPE(msg,OB_type) \
    (msg.t == T_tds##OB_type)

#define TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(msg,OB_type) \
    (msg->t == T_tds##OB_type)

#define TDSRRC_CHECK_COMMON_MSG_TYPE2(msg,struct_name,OB_type) \
    (msg.t == T_tds##struct_name##_##OB_type)

#define TDSRRC_CHECK_COMMON_MSG_TYPE2_PTR(msg,struct_name,OB_type) \
    (msg->t == T_tds##struct_name##_##OB_type)

#define TDSRRC_SET_COMMON_MSG_TYPE(msg,OB_type) \
    (msg.t = T_tds##OB_type)

#define TDSRRC_SET_COMMON_MSG_TYPE_PTR(msg,OB_type) \
    (msg->t = T_tds##OB_type)

#define TDSRRC_SET_COMMON_MSG_TYPE2(msg,struct_name,OB_type) \
    (msg.t = T_tds##struct_name##_##OB_type)

#define TDSRRC_SET_COMMON_MSG_TYPE2_PTR(msg,struct_name,OB_type) \
    (msg->t = T_tds##struct_name##_##OB_type)


#define TDSRRC_MSG_COMMON_BITMASK_IE_PTR(msg,struct_name,tag) \
  (msg->m.tag##Present)

#define TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(msg,tag) \
  (msg->m.tag##Present)
#define TDSRRC_MSG_COMMON_BITMASK_IE(msg,struct_name,tag) \
    (msg.m.tag##Present)
  
#define TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg,tag) \
    (msg.m.tag##Present)

/* Macros to check presence of I.E's*/
#define TDSRRC_RESET_MSG_IE_PRESENT(msg) \
    (memset(&msg.m, 0, sizeof(msg.m)))

#define TDSRRC_RESET_MSG_IE_PRESENT_PTR(msg) \
    (memset(&msg->m, 0, sizeof(msg->m)))

#define TDSRRC_GET_MSG_TYPE(msg) \
      (msg.t)
      
#define TDSRRC_GET_MSG_TYPE_PTR(msg) \
        (msg->t)

#define TDSRRC_CHECK_MSG_TYPE(msg,OB_type) \
    (msg.t == OB_type)


#define TDSRRC_CHECK_MSG_TYPE_IE(msg, OB_type) \
    (msg.m.OB_type)

#define TDSRRC_CHECK_MSG_TYPE_IE_PTR(msg, OB_type) \
    (msg->m.OB_type)

#define TDSRRCSET_MSG_TYPE(msg, OB_type) \
    (msg.t = OB_type)
    
#define TDSRRCSET_MSG_TYPE_IE_PTR(msg, OB_type) \
    (msg->m.OB_type = 1)


#define TDSRRCSET_R3_MSG_IE_PRESENT(msg,msg_struct_name,msg_bitfield) \
  (msg.m.msg_bitfield##Present = 1)
  
#define TDSRRCSET_R7_MSG_IE_PRESENT(msg,msg_struct_name,msg_bitfield) \
  (msg.m.msg_bitfield##Present = 1)
  
#define TDSRRCGET_PLMN_TYPE_TAG(msg) \
  (T_tdsrrc_PLMN_Type_##msg)

#define TDSRRCGET_PLMN_TYPE(msg) \
    (msg->t)

#define TDSRRCGET_SystemInformation_BCH_payload_TYPE(msg) \
  (T_tdsrrc_SystemInformation_BCH_payload_##msg)

#define TDSRRCGET_SystemInformation_FACH_payload_TYPE(msg) \
  (T_tdsrrc_SystemInformation_FACH_payload_##msg)

#define TDSRRCGET_SIB_OFF_VALUE(msg) \
  (tdsrrc_SibOFF_##msg)

#define TDSRRCGET_SIB_TYPE_AND_TAG_VALUE_SYS_INFO_TYPE(msg) \
  (T_tdsrrc_SIBSb_TypeAndTag_##msg)

#define TDSRRCGET_COMMON_CHOICE_TYPE(msg) \
  (T_tds##msg)

#define TDSRRCGET_SIB_TYPE_AND_TAG(msg) \
  (msg->t)

#define TDSRRCGET_SCHD_INFO_TAG(msg) \
  (msg->t)

#define TDSRRCGET_SCHD_INFO_SIB_POS(msg) \
  (T_tdsrrc_SchedulingInformation_sib_Pos_##msg)

#define TDSRRCCHECK_MSG_PRESENT(msg,msg_struct_name,msg_bitfield) \
  (msg->t == msg_bitfield##Present)
  
#define TDSRRCCHECK_SIB_TypeExt(msg,value)\
  (msg->t == T_tdsrrc_SIB_TypeExt_##value)

#define TDSRRCCHECK_SIBSb_TypeAndTag(msg,value)\
  (msg->t == T_tdsrrc_SIBSb_TypeAndTag_##value)

#define TDSRRCCHECK_SIB_MSG_IE_PRESENT(msg,msg_struct_name,msg_bitfield) \
  (msg->m.msg_bitfield##Present)

#define TDSRRCCHECK_TIMER_CONST_PRESENT_R5(msg,msg_struct_name,msg_bitfield) 1
  //(msg->m.msg_bitfield##Present)

/* Macros to extract pointers for message I.E's from tdsrrc_DL_DCCH_Message*/
#define TDSRRC_EXTRACT_R3_MSG_POINTER(dcch_msg_ptr, msg_name, msg_ie_struct) \
   (&(dcch_msg_ptr->message.u.msg_name.u.r3.msg_ie_struct))

#define TDSRRC_EXTRACT_R5_MSG_POINTER(dcch_msg_ptr, msg_name, msg_ie_struct) \
   (&(dcch_msg_ptr->message.u.msg_name.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.\
      r5.msg_ie_struct))
#define TDSRRC_EXTRACT_R5_MSG_POINTER_UMI(dcch_msg_ptr, msg_name, msg_ie_struct) \
     (&(dcch_msg_ptr->message.u.msg_name.u.later_than_r3.criticalExtensions.u.\
        r5.msg_ie_struct))

#define TDSRRC_EXTRACT_R6_MSG_POINTER(dcch_msg_ptr, msg_name, msg_ie_struct) \
   (&(dcch_msg_ptr->message.u.msg_name.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.\
      criticalExtensions.u.r6.msg_ie_struct))
      
#define TDSRRC_EXTRACT_R7_MSG_POINTER(dcch_msg_ptr, msg_name, msg_ie_struct) \
   (&(dcch_msg_ptr->message.u.msg_name.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.\
      criticalExtensions.u.criticalExtensions.u.r7.msg_ie_struct))
      
#define TDSRRC_EXTRACT_R7_MSG_POINTER_UMI_URA(dcch_msg_ptr, msg_name, msg_ie_struct) \
   (&(dcch_msg_ptr->message.u.msg_name.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.\
      r7.msg_ie_struct))
      
#define TDSRRC_EXTRACT_R8_MSG_POINTER(dcch_msg_ptr, msg_name, msg_ie_struct) \
   (&(dcch_msg_ptr->message.u.msg_name.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.\
      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.msg_ie_struct))

#define TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(msg_ptr)\
    ((mib_ptr->m.v690NonCriticalExtensionsPresent)&& \
     (mib_ptr->v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent)&& \
     (mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.\
        masterInformationBlock_v6b0ext.m.extSIBTypeInfoSchedulingInfo_ListPresent)\
    )
    
#define TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(msg_ptr, msg_type_prefix1,msg_type_prefix2)\
        ((msg_ptr->m.v6b0NonCriticalExtensionsPresent)&&\
         (msg_ptr->v6b0NonCriticalExtensions.##msg_type_prefix2##_v6b0ext.m.extSIBTypeInfoSchedulingInfo_ListPresent)\
        )
      


/* =======================================================================
**                        Function Declarations
** ======================================================================= */



/*==========================================================================*/




#endif
