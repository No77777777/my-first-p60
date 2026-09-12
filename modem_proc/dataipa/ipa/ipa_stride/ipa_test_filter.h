#ifndef IPA_TEST_FILTER_H
#define IPA_TEST_FILTER_H
/*=========================================================================*/
/*!
  @file
  IPA_TEST_FILTER.h

  @brief
  This file provides all the filtr utilities required by the random test engine.

  @details
  Services provided in this file include management of various tasks.

  @see

  Copyright (c) 2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/15/14   mi      IPA 2.1 Regression test fixes.
03/26/14   mi      Regression test fixes.
03/06/14   mi      Added fragmentation test changes.
01/17/14   SaCh    CR#600855: Expecting SIO_stream_id instead of uid value in 
                   ipa_ipfltr_alloc_rule_handle API for QOS rule.
10/25/13   rp      Adding supoort for multiple engine_run within a test
08/26/13   mi      Created module

===========================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include "IPA_test_engine.h"
#include "IPA_test.h"

extern "C"
{
#include<ipa_ipfltr.h>
#include<ipa_ctl.h>
}

/*==============================================================================

                         MACROS

==============================================================================*/
#define IPA_TEST_FILTER_DL_MAX_RULES      IPA_IPFLTR_MAX_NUM_SOCKET_RULES
#define IPA_TEST_FILTER_UL_QOS_MAX_RULES  IPA_IPFLTR_MAX_NUM_QOS_RULES
#define IPA_TEST_FILTER_MAX_RULES         IPA_TEST_FILTER_DL_MAX_RULES + IPA_TEST_FILTER_UL_QOS_MAX_RULES

#define IPA_TEST_FILTER_MAX_QOS_V4_RULES  7
#define IPA_TEST_FILTER_MAX_QOS_V6_RULES  15
#define IPA_TEST_FILTER_MAX_DL_V4_RULES   8
#define IPA_TEST_FILTER_MAX_DL_V6_RULES   19

#define IPA_TEST_MAX_INITIAL_OFFSET       136
#define IPA_TEST_FILTER_V4_PROT_OFFSET    9
#define IPA_TEST_FILTER_V6_PROT_OFFSET    6
#define IPA_TEST_FILTER_TOS_OFFSET        1
#define IPA_TEST_MAX_UL_STATIC_FILTERS    16
#define IPA_TEST_MAX_RAND_FILTER_TYPE     20

/*===========================================================================
                      Parameters needed by random filter module 
===========================================================================*/

typedef struct
{
  /*! Bitmap for all the equations for IPV4/IPV6  */
  uint16 tos_eq               : 1;
  uint16 tc_eq                : 1;
  uint16 fl_eq                : 1;
  uint16 protocol_eq          : 1;
  uint16 offset_meq_32        : 1;
  uint16 offset_meq_128       : 1;
  uint16 ihl_offset_range_16  : 1;
  uint16 ihl_offset_eq_16     : 1;
  uint16 ihl_offset_eq_32     : 1;
  uint16 ihl_offset_meq_32    : 1;
}ipa_test_filter_bitmap_s;

/*! The count of a few equations */
typedef struct
{
  uint8 num_offset_meq_32;
  uint8 num_offset_meq_128;
  uint8 num_ihl_offset_range_16;
  uint8 num_ihl_offset_meq_32;
}ipa_test_filter_eq_count_s;

typedef union
{
  /*! Bitmap for all the equations for IPV4/IPV6 */
  ipa_test_filter_bitmap_s      bits;
  uint16                        value;
}ipa_test_filter_bitmap_u;

/*! @brief Offset and 16 bit comparison equation */
typedef struct 
{
  uint8 offset;
  uint16 value;
} ipa_test_filter_eq_16_s;

/*! @brief Offset and 32 bit comparison equation */
typedef struct 
{
  uint8 offset;
  uint32 value;
} ipa_test_filter_eq_32_s;

/*! @brief Offset and 128 bit masked comparison equation */
typedef struct 
{
  uint8 offset;
  uint8 mask[16];
  uint8 value[16];
} ipa_test_filter_mask_eq_128_s;

/*! @brief Offset and 32 bit masked comparison equation */
typedef struct 
{
  uint8 offset;
  uint32 mask;
  uint32 value;
} ipa_test_filter_mask_eq_32_s;

/*! @brief Equation for identifying a range. Ranges are inclusive */
typedef struct 
{
  uint8 offset;
  uint16 range_low;
  uint16 range_high;
} ipa_test_filter_range_eq_16_s;

typedef struct
{
  /*! IP type of the rule */
  ipa_ipfltr_ip_type_e              ip_type;
  /*! Rule type of the rule*/
  ipa_ipfltr_rule_type_e            rule_type;
  /*! Bitmask for the particular rule*/
  ipa_test_filter_bitmap_u           bitmap;
  /*! The IHL value to be used for the rule*/
  uint32                             ihl_value;
  /*! The equation count for a few equations */
  ipa_test_filter_eq_count_s         eq_count;
  /*! TOS value for IPV4 filter*/
  uint8                              tos;
  /*! Protocol value for IPV4 filter */
  uint8                              protocol;
  /*! Flow Label value for IPV6 filter */
  uint32                             flow_label;
  /*! Traffic class for IPV6 filter*/
  uint8                              traffic_class;
  /*! Equation meq32 structure for the equations present in the rule*/
  ipa_test_filter_mask_eq_32_s       meq_32[2];
  /*! Mask Equation 128 structure for the equations present in the rule*/
  ipa_test_filter_mask_eq_128_s      meq_128[2];
  /*! Equation 16 structure for the equations present in the rule*/
  ipa_test_filter_eq_16_s            ihl_offset_eq_16;
  /*! Range equation 16 structure present in the rule 
      with the offset, range values*/
  ipa_test_filter_range_eq_16_s      ihl_range_eq_16[2];
  /*! Equation 32 structure for the equations present in the rule*/
  ipa_test_filter_eq_32_s            ihl_eq_32;
  /*! Equation meq32 structure for the equations present in the rule*/
  ipa_test_filter_mask_eq_32_s       ihl_meq_32[2];
  /*! Rule result for the particular rule*/
  uint8                              rule_result;
  /*!UID provided for the particular filter rule*/
  uint32                             usr_data;
}ipa_test_filter_rule_cfg_s;

typedef enum
{
  ICMP = 0,
  TCP,
  UDP,
  IPA_TEST_FILTER_PROT_MAX
}ipa_test_filter_prot_e;

typedef enum
{
  IPV6_FRAG = 0,
  IPV6_ICMP_REQUEST = 1,
  IPV6_ICMP_REPLY = 2,
  IPV6_ICMP_NDP = 3,
  IPV6_DHCP  = 4,
}ipa_test_filter_static_v6_rule_e;

typedef enum
{ IPV4_FRAG = 5,
  IPV4_DHCP = 6,
  IPV4_CLAT_ANY_ICMP = 7,
  IPV4_CLAT_UDP_CHKSUM_ZERO = 8, 
  IPV4_CLAT_DNS_PROXY = 9,
  IPV4_CLAT_TTL = 10,
  IPV4_CLAT_FRAG_V4 = 11,
  IPV4_TCP_ACK = 12,
  IPV6_TCP_ACK = 13,
}ipa_test_filter_static_v4_rule_e;

/*!Index into the QOS Rule config table*/
typedef enum
{
   IPV4_QOS_RULE1 = 0,     /*! TOS Equation */
   IPV4_QOS_RULE2 = 1,     /*! Protocol Equation */
   IPV4_QOS_RULE3 = 2,     /*! 2 MEQ32 Equation */ 
   IPV4_QOS_RULE4 = 3,     /*! 2 MEQ128 Equation */
   IPV4_QOS_RULE5 = 4,     /*! 2 IHL_RANGE_16 Equation */
   IPV4_QOS_RULE6 = 5,     /*! IHL_OFFSET_16 Equation */
   IPV4_QOS_RULE7 = 6,     /*! EQ32 Equation */
   IPV4_QOS_RULE8 = 7,     /*! 2 IHL_MEQ_32*/

   IPV6_QOS_RULE9 = 8,     /*! IHL_EQ32 Equation */
   IPV6_QOS_RULE10 = 9,    /*! IHL_MEQ_32 Equation*/
   IPV6_QOS_RULE11 = 10,   /*! MEQ_32 Equation */
   IPV6_QOS_RULE12 = 11,   /*! MEQ128 Equation */
   IPV6_QOS_RULE13 = 12,   /*! IHL_OFFSET_RANGE_EQ16 Equation*/
   IPV6_QOS_RULE14 = 13,   /*! IHL_OFFSET_EQ16 Equation */
   IPV6_QOS_RULE15 = 14,   /*! Traffic Class Equation */
   IPV6_QOS_RULE16 = 15,   /*! Flow Equation */

}ipa_test_filter_qos_rule_e;;
typedef enum
{
  IPV4_TCP_ACK_FLTR_RES    = 0x00,
  IPV4_DHCP_FLTR_RES       = 0x0E,
  IPV6_TCP_ACK_FLTR_RES    = 0x00,
  IPV6_ICMP_REQ_FLTR_RES   = 0x0F,
  IPV6_ICMP_REPLY_FLTR_RES = 0x1A,
  IPV6_ICMP_NDP_FLTR_RES   = 0x1C,
  IPV6_DHCP_FLTR_RES       = 0x1D,
  IPV4_FRAG_FLTR_RES       = 0x0A,
  IPV6_FRAG_FLTR_RES       = 0x0B,
  IPV4_CLAT_ANY_ICMP_RES        = 0xAA,
  IPV4_CLAT_UDP_CHKSUM_ZERO_RES = 0xBB,
  IPV4_CLAT_TTL_RES             = 0xCC,
  IPV4_CLAT_DNS_PROXY_RES       = 0xDD,
  IPV4_CLAT_FRAG_V4_RES         = 0xEE,
  IPV4_MAX_FLTR_RES             = 0xFF,
 }ipa_test_filter_static_rule_result_e;



/*===========================================================================
                         Input Params to the filter module
===========================================================================*/
typedef struct
{
  /*! The rules the packets need to match - It can be provided by user 
   *  or can be generated randomly by the engine*/
  uint8                              uid;
  /*!Filter rule category type*/
  ipa_test_engine_fltr_cat_type_e    fltr_rule_cat_type; 
  /*! The type of filter type the packet needs to match*/
  ipa_test_engine_fltr_type_e        fltr_rule_type; /*dir_test_type*/
  /*! Pointer to the ip pkt info structures for the 
      packets to be matched with the rule*/
  ipa_test_engine_ip_pkt_info_s      *ip_pkt_info_ptr;
  /*! Static rule to match*/
}ipa_test_filter_pkt_gen_cfg_s;

typedef struct
{

  /*! Fltr Rule Category type */
  ipa_test_engine_fltr_cat_type_e    fltr_rule_cat_type;
  /*! Fltr Rule type in a Category*/
  ipa_test_engine_fltr_type_e        fltr_rule_type;/*dir_test_type*/
  /*! Test Type*/
  ipa_test_engine_test_type_e        test_type;
  /*! Test to match TCP ACK + QOS rule*/
  boolean                            is_tcp_ack_qos_rule; 
  /*!Number of bearers*/
  uint8                              bearer_cnt;
  /*! UID array to be filled*/
  uint8                              *uid_array;
  /*! Number of DL filters to be installed*/
  uint16                             num_dl_sckt_fltr;
  /*!Number of UL QOS filters to be installed*/
  uint16                             num_ul_qos_fltr;
  /*! Apply Static configurations */
  uint8                              is_static_config;
  boolean                            filter_pkt_gen;
  uint8                              total_pkts; 
  boolean                            is_tcp_ack_only;
  /*! Apply CLAT static filter rules if CLAT is enabled */
  boolean                            is_clat_enabled;
  /*! If CLAT enabled, the value of IPV4 subnet mask configured  */
  uint32                             v4_src_subnet_mask;
} ipa_test_filter_init_cfg_s;

/*==========================================================================

                                 FUNCTIONS

==========================================================================*/

/*==========================================================================

 FUNCTION: ipa_test_filter_gen_filter_cfg

==========================================================================*/

/*!
  @brief
  
  @return
  None
*/
/*=========================================================================*/

ipa_test_ret_code_e ipa_test_filter_init
(
  ipa_test_filter_init_cfg_s *init_cfg_ptr
);

/*==========================================================================

 FUNCTION: ipa_test_filter_run_init

==========================================================================*/

/*!
  @brief
  Initializes the filter run params at the begining of each run

  @return
  None
*/
/*=========================================================================*/
void ipa_test_filter_run_init();

/*==========================================================================

 FUNCTION: ipa_test_filter_gen_filter_cfg

==========================================================================*/

/*!
  @brief
  
  @return
  None
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_static_filter_init
(
  ipa_test_filter_init_cfg_s *init_cfg_ptr
);

/*==========================================================================

 FUNCTION: ipa_test_filter_ul_process_ip_pkt

==========================================================================*/

/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/

void ipa_test_filter_ul_process_ip_pkt
(
  ipa_test_filter_pkt_gen_cfg_s   pkt_gen_cfg
);

/*==========================================================================

 FUNCTION: ipa_test_filter_ul_static_gen_ip_pkt

==========================================================================*/

/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/
void ipa_test_filter_ul_static_gen_ip_pkt
(
  ipa_test_filter_pkt_gen_cfg_s   pkt_gen_cfg,
  uint8                           pkt_ip_type,
  uint8                           rand_filter
);
/*==========================================================================

 FUNCTION: ipa_test_filter_get_ul_static_rand_filter

==========================================================================*/

/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/
uint8 ipa_test_filter_get_ul_static_rand_filter
(
  ipa_test_engine_fltr_type_e fltr_rule_type
);

/*==========================================================================

 FUNCTION: ipa_test_filter_get_qos_rule_cfg_index

==========================================================================*/

/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/
uint8 ipa_test_filter_get_qos_rule_cfg_index
(
  ipa_test_engine_fltr_type_e fltr_rule_type
);

/*==========================================================================

 FUNCTION: ipa_test_filter_ul_qos_gen_ip_pkt

==========================================================================*/

/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/
void ipa_test_filter_ul_qos_gen_ip_pkt
(
  ipa_test_filter_pkt_gen_cfg_s   pkt_gen_cfg,
  uint8                           pkt_ip_type,
  uint8                           rule_match_index
);


/*==========================================================================

 FUNCTION: ipa_test_filter_dl_process_ip_pkt

==========================================================================*/

/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/

void ipa_test_filter_dl_process_ip_pkt
(
  ipa_test_filter_pkt_gen_cfg_s   pkt_gen_cfg
);


/*==========================================================================

 FUNCTION: ipa_test_filter_dl_gen_ip_pkt

==========================================================================*/
void ipa_test_filter_dl_gen_ip_pkt
(
  ipa_test_filter_pkt_gen_cfg_s   pkt_gen_cfg,
   ipa_ipfltr_ip_type_e           filter_ip_type,
  uint8                           rule_match_index
);

/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/

/*==========================================================================

 FUNCTION: ipa_test_filter_destroy

==========================================================================*/

/*!
  @brief
  Removes all the filter rules configured at init time.

  @return
  IPA_TEST_RET_CODE_SUCCESS if all the filter rules are successfully destroyed.
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_filter_destroy();

/*============================================================================== 
                   Filter matching packet generation
================================================================================*/

/*==============================================================================

  FUNCTION:  ipa_test_filter_match_32

==============================================================================*/
/*!
  @brief
  Writes a 32-bit number to the provided memory location.

  The bytes will be written in the appropriate packet order.

  @return
  Pointer to the end of the word
*/
/*============================================================================*/
void ipa_test_filter_match_32
(
  /*! The word to match */
  uint32 word_to_match,
  /*! Where the word is to be written to */
  uint8* rule_dest_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_filter_match_16

==============================================================================*/
/*!
  @brief
  Writes a 16-bit number to the provided memory location.

  The bytes will be written in the appropriate packet order.

  @return
  Pointer to the end of the word
*/
/*============================================================================*/
void ipa_test_filter_match_16
(
  /*! The half-word to match */
  uint16 half_word,
  /*! Where the word is to be written to */
  uint8* rule_dest_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_filter_match_8

==============================================================================*/
/*!
  @brief
  Writes a 8-bit number to the provided memory location.

  The bytes will be written in the appropriate packet order.

  @return
  Pointer to the end of the word
*/
/*============================================================================*/
void ipa_test_filter_match_8
(
  /*! The byte to match */
  uint8 byte_to_match,
  /*! Where the word is to be written to */
  uint8* rule_dest_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_filter_match_meq_32

==============================================================================*/
/*!
  @brief
  Writes a mask eq 32 rule to the provided memory location.

  The rule will be written in the ipa hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_meq_32
(
  /*! The rule to match */
  ipa_test_filter_mask_eq_32_s* eqn,
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match,
  /*! Rule offset */
  uint32 offset
);
/*==============================================================================

  FUNCTION:  ipa_test_filter_match_eq_32

==============================================================================*/
/*!
  @brief
  Writes a eq 32 rule to the provided memory location.

  The rule will be written in the a2 hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_eq32
(
  /*! The rule to match */
  const ipa_test_filter_eq_32_s *eqn,
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match,
  /*! Rule offset */
  uint32 offset
);

/*==============================================================================

  FUNCTION:  ipa_test_filter_match_meq_128

==============================================================================*/
/*!
  @brief
  Writes a mask eq 32 rule to the provided memory location.

  The rule will be written in the ipa hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_meq_128
(
  /*! The rule to match */
  ipa_test_filter_mask_eq_128_s *eqn,
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match
);
/*==============================================================================

  FUNCTION:  ipa_test_filter_match_eq_16

==============================================================================*/
/*!
  @brief
  Writes a mask eq 16 rule to the provided memory location.

  The rule will be written in the ipa hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_eq_16
(
  /*! The rule to match */
  ipa_test_filter_eq_16_s *eqn,
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match,
  /*! Rule offset */
  uint32 offset
);
/*==============================================================================

  FUNCTION:  ipa_test_filter_match_range_16

==============================================================================*/
/*!
  @brief
  Writes a mask eq 16 rule to the provided memory location.

  The rule will be written in the a2 hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_range_16
(
  /*! The rule to match */
  ipa_test_filter_range_eq_16_s *eqn,
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match,
  /*! Rule offset */
  uint32 offset
);
/*==============================================================================

  FUNCTION:  ipa_test_filter_match_protocol_eq

==============================================================================*/
/*!
  @brief
  Writes a protocol equation rule to the provided memory location.

  The rule will be written in the ipa hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_protocol_eq
(
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*!protocol value*/
  uint8 protocol,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match
);

/*==============================================================================

  FUNCTION:  ipa_test_filter_match_tos_eq

==============================================================================*/
/*!
  @brief
  Writes a TOS equation rule to the provided memory location.

  The rule will be written in the ipa hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_tos_eq
(
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*!Traffic class value*/
  uint8 tos,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match
);
/*==============================================================================

  FUNCTION:  ipa_test_filter_match_tc_eq

==============================================================================*/
/*!
  @brief
  Writes a Traffic class equation rule to the provided memory location.

  The rule will be written in the ipa hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_tc_eq
(
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*!Traffic class value*/
  uint8 traffic_class,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match
);

/*==============================================================================

  FUNCTION:  ipa_test_filter_match_fl_eq

==============================================================================*/
/*!
  @brief
  Writes a flow label equation rule to the provided memory location.

  The rule will be written in the ipa hardware memory format.

  @return
  Pointer to the end of the rule.
*/
/*============================================================================*/
void ipa_test_filter_match_fl_eq
(
  /*! Where the rule it to be written to */
  uint8* match_dest,
  /*!flow label value*/
  uint32 flow_label,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match
);

/*==========================================================================

 FUNCTION: ipa_test_ul_static_filter_init

==========================================================================*/

/*!
  @brief
  Install all UL static filters.
 
  @return
  IPA_TEST_RET_CODE_SUCCESS if all the filter rules are successfully installed.
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_ul_static_filter_init();

/*==========================================================================

 FUNCTION: ipa_test_ul_qos_filter_init

==========================================================================*/

/*!
  @brief
  Install all UL static filters.
 
  @return
  IPA_TEST_RET_CODE_SUCCESS if all the filter rules are successfully installed.
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_filter_ul_qos_filter_init
(
  ipa_test_filter_init_cfg_s   *init_cfg_ptr
);

/*==========================================================================

 FUNCTION: ipa_test_filter_dl_socket_filter_init

==========================================================================*/

/*!
  @brief
  Install all DL socket filters.
 
  @return
  IPA_TEST_RET_CODE_SUCCESS if all the filter rules are successfully installed.
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_filter_dl_socket_filter_init
(
  ipa_test_filter_init_cfg_s   *init_cfg_ptr
);

/*==========================================================================

 FUNCTION: ipa_test_filter_gen_tcp_ack_packet

==========================================================================*/
/*!
  @brief
  Generate ip pkt cfg for TCP ACK and generate the pkt.
 
  @return
  IPA_TEST_RET_CODE_SUCCESS if all the filter rules are successfully installed.
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_filter_gen_tcp_ack_packet
(
  ipa_test_engine_ip_pkt_info_s *pkt_info, 
  uint8 ip_type
);

/*==========================================================================

 FUNCTION: ipa_test_filter_get_tcp_ack_only_flag

==========================================================================*/

/*!
  @brief
  

  @return
  Return  TRUE if it is TCP ACK only test case or else FALSE
  
*/
/*=========================================================================*/
boolean ipa_test_filter_get_tcp_ack_only_flag();

#endif /* IPA_TEST_FILTER_H */

