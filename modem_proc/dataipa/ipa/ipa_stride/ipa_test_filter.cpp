/*=========================================================================*/
/*!
  @file
  IPA_TEST_FILTER.cpp

  @brief
  This file provides all the filter utilities required by the random test engine.

  @details
  Services provided in this file include management of various tasks.

  Copyright (c) 2013-14 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/ipa_test_filter.cpp#2 $
$DateTime: 2020/11/21 09:40:24 $
$Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/17/20    mk      CR 2820227: IPA Stride test Compilation Fix
02/17/15    mi     API change for frag rule addition.
07/15/14    mi     IPA 2.1 Test Regression fix.   
04/25/14    rp     Changes to support ON-target testing on full stack CRM
03/26/14    mi     Regression test changes.
03/06/14    mi     Added fragmenation test changes.
01/17/14    SaCh   CR#600855: Expecting SIO_stream_id instead of uid value in 
                   ipa_ipfltr_alloc_rule_handle API for QOS rule.
12/13/13    SaCh   Removed priority argument from alloc_rule API call.
12/05/13    mi     TCP Ack related fixes.
11/19/13    pgm    TCP Ack prioritization changes.
10/25/13    pgm    Random Filter Framework changes
10/25/13    rp     Adding supoort for multiple engine_run within a test
08/26/13    mi     Created module

===========================================================================*/

/*==========================================================================

                            INCLUDE FILES FOR MODULE

===========================================================================*/
#include "IPA_test_engine.h"
#include "IPA_test.h"
#include "ipa_test_filter.h"

#pragma optimize("", off)
/*! brief : Top level filter structure*/
typedef struct
{
  ipa_test_filter_rule_cfg_s         dl_sckt_rule_cfg[IPA_TEST_FILTER_MAX_RULES];
  ipa_test_filter_rule_cfg_s         ul_qos_rule_cfg[IPA_IPFLTR_MAX_NUM_QOS_RULES];
  ipa_test_filter_rule_cfg_s         ul_static_rule_cfg[IPA_TEST_MAX_UL_STATIC_FILTERS];
  ipa_ipfltr_rule_handle_t           dl_socket_hdl_arr[IPA_TEST_FILTER_MAX_RULES];
  ipa_ipfltr_rule_handle_t           ul_static_hdl_arr[IPA_TEST_MAX_UL_STATIC_FILTERS];
  ipa_ipfltr_rule_handle_t           ul_qos_hdl_arr[50];
  ipa_test_engine_fltr_cat_type_e    fltr_rule_cat_type; 
  ipa_test_engine_fltr_type_e        fltr_rule_type; /*dir_test_type*/
  ipa_test_engine_test_type_e        test_type;
  uint32                             v4_src_subnet_mask;
  uint16                             num_dl_sckt_fltr;
  uint16                             num_ul_qos_fltr;
  uint16                             num_tot_filters;
  uint16                             num_ul_static_fltr;
  uint16                             ihl_value;
  uint8                              total_pkts; 
  uint8                              curr_pkts; 
  uint8                              frag_filter_result;
  uint8                              bearer_cnt;
  boolean                            random_fltr_test;
  boolean                            is_tcp_ack_only;
  boolean                            is_frag_case;
  boolean                            is_tcp_ack_qos_rule; 
  boolean                            filter_tcp_ack_gen;
  boolean                            is_clat_enabled;
}ipa_test_filter_s;

/*==========================================================================
                             Internal Variables
===========================================================================*/
ipa_test_filter_s ipa_test_filter;

/*==========================================================================

                                 FUNCTIONS

==========================================================================*/

/*========================================================================== 
                   Filter matching packet generation
============================================================================*/
/*==========================================================================

  FUNCTION:  ipa_test_filter_match_32

============================================================================*/
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
  uint32   word_to_match,
  /*! Where the word is to be written to */
  uint8    *rule_dest_ptr
)
{
  *rule_dest_ptr++ = (uint8)((word_to_match >> 24) & 0xFF);
  *rule_dest_ptr++ = (uint8)((word_to_match >> 16) & 0xFF);
  *rule_dest_ptr++ = (uint8)((word_to_match >> 8) & 0xFF);
  *rule_dest_ptr = (uint8)((word_to_match) & 0xFF);
}

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
  uint16  half_word,
  /*! Where the word is to be written to */
  uint8   *rule_dest_ptr
)
{
  *rule_dest_ptr++ = (uint8)((half_word >> 8) & 0xFF);
  *rule_dest_ptr = (uint8)((half_word) & 0xFF);
}

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
  uint8   byte_to_match,
  /*! Where the word is to be written to */
  uint8   *rule_dest_ptr
)
{
  *rule_dest_ptr = (byte_to_match) & 0xFF;
}

/*==============================================================================

  FUNCTION:  ipa_test_ipfilter_match_meq_32

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
  ipa_test_filter_mask_eq_32_s  *eqn,
  /*! Where the rule it to be written to */
  uint8                         *match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean                       match,
  /*! IHL value*/
  uint32                        offset
)
{
  uint32 value_to_write = rand();
  value_to_write &= ~eqn->mask;
  if ( match )
  {
    value_to_write |= (eqn->mask & eqn->value );
  }
  else
  {
    value_to_write |= (eqn->mask & ~eqn->value );
  }
  ipa_test_filter_match_32(value_to_write, match_dest + eqn->offset + offset);
}

/*==============================================================================

  FUNCTION:  ipa_test_ipfilter_match_eq_32

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
  const ipa_test_filter_eq_32_s  *eqn,
  /*! Where the rule it to be written to */
  uint8                         *match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean                        match,
  /*! Rule offset */
  uint32                         offset
)
{
  if ( match )
  {
    ipa_test_filter_match_32(eqn->value, match_dest + eqn->offset + offset);
  }
  else
  {
    ipa_test_filter_match_32(~(eqn->value), match_dest + eqn->offset + offset );
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_ipfilter_match_meq_128

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
  ipa_test_filter_mask_eq_128_s  *eqn,
  /*! Where the rule it to be written to */
  uint8                          *match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean                        match
)
{
  uint32 i;
  uint8 byte_to_write;

  match_dest += eqn->offset;

  for ( i = 0; i < 16; i++ )
  {
    byte_to_write = (uint8) rand();
    byte_to_write &= ~eqn->mask[i];

    if ( match )
    {
      byte_to_write |= (eqn->mask[i] & eqn->value[i] );
    }
    else
    {
      /* Flip the bits of the value, so it will not match */
      byte_to_write |= (eqn->mask[i] & (~(eqn->value[i])) );
    }

   ipa_test_filter_match_8(byte_to_write, match_dest + i);
  }
}

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
  ipa_test_filter_eq_16_s   *eqn,
  /*! Where the rule it to be written to */
  uint8                     *match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean                   match,
  /*! Rule offset */
  uint32                    offset
)
{
  if ( match )
  {
    ipa_test_filter_match_16(eqn->value, match_dest + eqn->offset + offset);
  }
  else
  {
    ipa_test_filter_match_16(~eqn->value,match_dest + eqn->offset + offset);
  }
}

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
  ipa_test_filter_range_eq_16_s    *eqn,
  /*! Where the rule it to be written to */
  uint8                            *match_dest,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean                          match,
  /*! Rule offset */
  uint32                           offset
)
{
  uint16 value_to_write;

  if ( match )
  {
    value_to_write = eqn->range_low +
        (rand() % (eqn->range_high - eqn->range_low));
    if (value_to_write == eqn->range_low)
    {
      value_to_write = eqn->range_low + 1;
    }
    IPA_TF_ASSERT( value_to_write <= eqn->range_high );
    IPA_TF_ASSERT( value_to_write > eqn->range_low);
  }
  else
  {
    value_to_write = eqn->range_high + 1;
  }
  /*! Check if its a negative offset IHL equation to be matched */
  if((eqn->offset & 0x80) == 0)
  {
  ipa_test_filter_match_16 ( value_to_write, match_dest + eqn->offset + offset);
}
  else
  {
    ipa_test_filter_match_16(value_to_write, match_dest + (eqn->offset & 0x7F));
  }
 
}

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
  uint8      *match_dest,
  /*!protocol value*/
  uint8      protocol,
  /*! The ip type of the packet*/
  uint8      ip_version_type,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean    match
)
{
  uint8 prot_offset;
  if (ip_version_type == 4)
  {
    prot_offset = IPA_TEST_FILTER_V4_PROT_OFFSET;
  }
  else
  {
    prot_offset = IPA_TEST_FILTER_V6_PROT_OFFSET;
  }
  if ( match )
  {
    ipa_test_filter_match_8(protocol, match_dest + prot_offset);
  }
  else
  {
    //ipa_test_filter_match_8(~protocol, match_dest + IPA_TEST_FILTER_PROT_OFFSET);
  }
}
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
  uint8   *match_dest,
  /*!Traffic class value*/
  uint8   tos,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match
)
{
  if ( match )
  {
    ipa_test_filter_match_8(tos, match_dest + IPA_TEST_FILTER_TOS_OFFSET);
  }
  else
  {
    ipa_test_filter_match_8(~tos, match_dest + IPA_TEST_FILTER_TOS_OFFSET);
  }
}
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
  uint8    *match_dest,
  /*!Traffic class value*/
  uint8    traffic_class,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean  match
)
{
  if ( match )
  {
     match_dest[0] = (match_dest[0] | 0x0F) & (((traffic_class & 0xF0) >> 4)| 0xF0);
     match_dest[1] = (match_dest[1] | 0xF0) & (((traffic_class & 0x0F) << 4)| 0x0F);
  }
  else
  {
     match_dest[0] = (match_dest[0] | 0x0F) & (~((traffic_class & 0xF0) >> 4)| 0xF0);
     match_dest[1] = (match_dest[1] | 0xF0) & (~((traffic_class & 0x0F << 4))| 0x0F);
  }
}
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
  uint8   *match_dest,
  /*!flow label value*/
  uint32  flow_label,
  /*! TRUE if the data is supposed to match this rule, false otherwise */
  boolean match
)
{
  if (match)
  {
    match_dest[1] =  (match_dest[1] | 0x0F) & (uint8)((flow_label << 16)| 0xF0 );
    match_dest[2] =  (uint8)((flow_label & 0x0000FF00) << 8 );
    match_dest[3] =  (uint8)((flow_label & 0x000000FF));
  }
  else
  {
    match_dest[1]  = ~((match_dest[1] | 0x0F) & (uint8)((flow_label << 16)| 0xF0 ));
    match_dest[2]  =  ~(uint8)((flow_label & 0x0000FF00) << 8 );
    match_dest[3]  =  ~(uint8)((flow_label & 0x000000FF));
  }
}
/*==========================================================================

 FUNCTION: ipa_test_filter_init

==========================================================================*/
/*!
  @brief
  
  @return
  IPA_TEST_RET_CODE_SUCCESS
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_filter_init
(
  ipa_test_filter_init_cfg_s   *init_cfg_ptr
)
{
  /*! Initialize the TCP ACK only to False*/
  ipa_test_filter.is_tcp_ack_only =  FALSE;
  /*! Initialize UL QOS rules*/
  if ( ipa_test_filter.is_tcp_ack_qos_rule )
  {
     /*Install non-conflicting QOS rules for TCP ACK*/
  }
  else
  {
     if ( ipa_test_filter.num_ul_qos_fltr > 0 )
     {
       IPA_TF_ASSERT(ipa_test_filter_ul_qos_filter_init(init_cfg_ptr)
                      == IPA_TEST_RET_CODE_SUCCESS);
     }
  }
 /*! Install all DL socket filters with static configurations */
  if( ipa_test_filter.num_dl_sckt_fltr > 0 )
  {
     IPA_TF_ASSERT(ipa_test_filter_dl_socket_filter_init(init_cfg_ptr) 
                   == IPA_TEST_RET_CODE_SUCCESS);
  }
  /*!Return success on filter init completion*/
  return IPA_TEST_RET_CODE_SUCCESS;
}

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
void ipa_test_filter_run_init()
{
  ipa_test_filter.curr_pkts = 0;
}

/*==========================================================================

 FUNCTION: ipa_test_static_filter_init

==========================================================================*/
/*!
  @brief
  
  @return
  IPA_TEST_RET_CODE_SUCCESS
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_static_filter_init
(
  ipa_test_filter_init_cfg_s   *init_cfg_ptr
)
{
   /*! Update Flr Init Cfg in Filter global structure*/
  ipa_test_filter.test_type = init_cfg_ptr->test_type;
  ipa_test_filter.total_pkts = init_cfg_ptr->total_pkts;
  ipa_test_filter.curr_pkts = 0;
  ipa_test_filter.filter_tcp_ack_gen = init_cfg_ptr->filter_pkt_gen;
  ipa_test_filter.random_fltr_test   = FALSE; 
  ipa_test_filter.num_dl_sckt_fltr = init_cfg_ptr->num_dl_sckt_fltr;
  ipa_test_filter.num_ul_qos_fltr = init_cfg_ptr->num_ul_qos_fltr;
  ipa_test_filter.fltr_rule_cat_type = init_cfg_ptr->fltr_rule_cat_type;
  ipa_test_filter.fltr_rule_type = init_cfg_ptr->fltr_rule_type;
  ipa_test_filter.is_tcp_ack_qos_rule = init_cfg_ptr->is_tcp_ack_qos_rule;
  ipa_test_filter.is_clat_enabled = init_cfg_ptr->is_clat_enabled;
  ipa_test_filter.v4_src_subnet_mask = init_cfg_ptr->v4_src_subnet_mask;
  /*! Initialize the TCP ACK filters */
  IPA_TF_ASSERT(ipa_ipfltr_enable_tcp_ack_prioritization() 
                != IPA_ERR_IPFLTR_COMMIT_FAIL);
  /*! Initialize all the UL static rules*/
  IPA_TF_ASSERT(ipa_test_ul_static_filter_init() 
                == IPA_TEST_RET_CODE_SUCCESS);
  /*!Return success on filter init completion*/
  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==========================================================================

 FUNCTION: ipa_test_filter_process_ip_pkt

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
)
{
  uint8                  rand_filter = IPA_TEST_MAX_RAND_FILTER_TYPE;
  uint8                  qos_rule_match_index = IPA_TEST_MAX_RAND_FILTER_TYPE;
  uint8                  ip_version_type;
  
  do
  {
    /* ipa_test_filter rule_cat_type/rule_type holds the test config
     from user.(Random or Directed test).
     *  pkt_gen_cfg fltr rule_cat_type/rule_type holds the rule types
      for the current pkt, filled either from
     *  directed test config or generated randomly.
     *  Fltr_rule_type is decided within the specific fltr_cat_type processing.
     */

    pkt_gen_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_MAX;
    pkt_gen_cfg.fltr_rule_type     = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
    ip_version_type                = 
      IPA_TEST_GET_IP_VERSION(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr);
    ipa_test_filter.curr_pkts++;
    /*! Get the IHL value for IPV6 filters from the test engine 
        This includes the IP header length and the extension header length.*/
    ipa_test_filter.ihl_value = pkt_gen_cfg.ip_pkt_info_ptr->ip_hdr_len +
                                pkt_gen_cfg.ip_pkt_info_ptr->extn_hdr_len;

    /* Generate Random Filter rule category for the current packet if it is not a directed test
     * UL Filter Rule cat - UL_STATIC_LOCAL, UL_QOS, UL_STATIC_AND_QOS
     */
    if (ipa_test_filter.fltr_rule_cat_type == IPA_TEST_ENGINE_FLTR_RULE_MAX)
    {
      if (ipa_test_filter.test_type == IPA_TEST_ENGINE_TEST_TYPE_UL)
      {
        if (ipa_test_filter.is_tcp_ack_qos_rule)
        {
          pkt_gen_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_QOS_TCP_ACK;
        }
        else
        {
          pkt_gen_cfg.fltr_rule_cat_type = 
              (ipa_test_engine_fltr_cat_type_e)ipa_test_generate_random_num(
                                           IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_AND_QOS,
                                           IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL);
        }
      }
      else if (ipa_test_filter.test_type == IPA_TEST_ENGINE_TEST_TYPE_DL)
      {
         pkt_gen_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_DL_SOCKET;
      }
    } /*fltr_rule_cat_type == IPA_TEST_ENGINE_FLTR_RULE_MAX*/
    
    else
    {
      /*Directed test*/
      pkt_gen_cfg.fltr_rule_cat_type = ipa_test_filter.fltr_rule_cat_type;
    }

    /*****************!@brief UL Static Filter Processing************************/

    if (pkt_gen_cfg.fltr_rule_cat_type == IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL)
    {

      /*Generate Random Filter for the current packet if it is not a directed test*/
      if(ipa_test_filter.fltr_rule_type == IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX)
      { 

        switch (ip_version_type)
        {
          case 4 :
          {
            pkt_gen_cfg.fltr_rule_type = 
              (ipa_test_engine_fltr_type_e)ipa_test_generate_random_num \
                (IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V4, 
                IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4);
            break;
          }
          case 6 :
          {
            pkt_gen_cfg.fltr_rule_type = 
              (ipa_test_engine_fltr_type_e)ipa_test_generate_random_num \
                (IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6, 
                IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6);
            break;
          }
          default:
            break;
          
        }
        ipa_test_filter.random_fltr_test = TRUE;      
       }
       else
       {
         ipa_test_filter.random_fltr_test = FALSE;      
         pkt_gen_cfg.fltr_rule_type = ipa_test_filter.fltr_rule_type;
       }

       /***********Setup the filter to be matched based on the above config*******/
       switch (pkt_gen_cfg.fltr_rule_type)
       {
         case IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4:
         {
           rand_filter = IPV4_TCP_ACK;
           ipa_test_filter.filter_tcp_ack_gen = TRUE; 
           /*To generate TCP ACK 
           only for the last pkt in directed test*/
           pkt_gen_cfg.ip_pkt_info_ptr->is_prioritized = FALSE;
          /**************************************************************
           Re Generate TCP ACK pkt only for the last packet out of 4 pkts
           For first 3 pkts - 
           If prot == ICMP -->generate valid ICMP_REQUEST pkts
           and prioritize it. If prot == TCP or UDP -->Do not
           generate new filter pkts.
           ***************************************************************/
          /*Below Logic is only for TCP ACK Directed test*/
          if (!ipa_test_filter.random_fltr_test)
          {
            if (ipa_test_filter.curr_pkts != ipa_test_filter.total_pkts)
            {
              /*Generate valid DHCPv4 filter pkt and Prioritize it*/
              if(IPA_TEST_UDP_PROT == IPA_TEST_GET_IP_PROTOCOL \
                      (pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr))
              {
                rand_filter = IPV4_DHCP;
              }
              /*This is needed if first 3 pkts are some valid data pkts 
              which doesnt need to match any filter*/
              ipa_test_filter.filter_tcp_ack_gen = FALSE;
            }        
          }
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V4:
        {
          rand_filter = IPV4_DHCP;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V4:
        {
          rand_filter = IPV4_FRAG;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V6:
        {
          rand_filter = IPV6_FRAG;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6:
        {
          rand_filter = IPV6_TCP_ACK;
          ipa_test_filter.filter_tcp_ack_gen = TRUE;
          pkt_gen_cfg.ip_pkt_info_ptr->is_prioritized = FALSE;
          /*Re Generate TCP ACK pkt only for the last packet out of 4 pkts*/
          if (ipa_test_filter.curr_pkts != ipa_test_filter.total_pkts)
          {
            /*Generate valid ICMPv6 or DHCPv6 filter pkt and Prioritize it*/
            if(IPA_TEST_ICMP_PROT == IPA_TEST_GET_IP_NXT_HEADER\
               (pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr))
            {
              rand_filter = IPV6_ICMP_REQUEST;
            }
            else if( IPA_TEST_UDP_PROT == IPA_TEST_GET_IP_NXT_HEADER\
                    (pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr) )
            {
              rand_filter = IPV6_DHCP;
            }
            ipa_test_filter.filter_tcp_ack_gen = FALSE;
          }
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V6:
        {
          rand_filter = IPV6_DHCP;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REQ_V6:
        {
          rand_filter = IPV6_ICMP_REQUEST;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REP_V6:
        {
          rand_filter = IPV6_ICMP_REPLY;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6:
        {
          rand_filter = IPV6_ICMP_NDP;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_ANY_ICMP_V4:
        {
          rand_filter = IPV4_CLAT_ANY_ICMP;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_UDP_CHKSUM_ZERO_V4:
        {
          rand_filter = IPV4_CLAT_UDP_CHKSUM_ZERO;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_TTL_V4:
        {
          rand_filter = IPV4_CLAT_TTL;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_DNS_PROXY_V4:
        {
          rand_filter = IPV4_CLAT_DNS_PROXY;
          break;
        }
        case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_FRAG_V4:
        {
          rand_filter = IPV4_CLAT_FRAG_V4;
          break;
        }
        default:
          break;
      }
      /*Check For below only if this is a TCP_ACK Directed test*/
      if ( !ipa_test_filter.random_fltr_test )
      {
        /*No need to generate filter pkt for valid data pkts for TCP_ACK V4 test*/
        if ( (IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4 == \
              ipa_test_filter.fltr_rule_type) && 
            (ipa_test_filter.filter_tcp_ack_gen == FALSE) 
             && 
            (IPV4_DHCP != rand_filter) )
          break;

        /*No need to generate filter pkt for valid data pkts for TCP_ACK V6 test*/
        if (IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6 ==
             ipa_test_filter.fltr_rule_type && 
            (ipa_test_filter.filter_tcp_ack_gen == FALSE) && 
            (IPV6_ICMP_REQUEST != rand_filter && IPV6_DHCP
              != rand_filter))
          break;
      }
      /*! Generate packet to match the corresponding static filter*/
      pkt_gen_cfg.ip_pkt_info_ptr->filter_result = 
        ipa_test_filter.ul_static_rule_cfg[rand_filter].rule_result;
      ipa_test_filter_ul_static_gen_ip_pkt(pkt_gen_cfg, ip_version_type, rand_filter);
    } /*IPA_TEST_FILTER_RULE_UL_STATIC_LOCAL*/

    /*! *************UL QoS Filter Processing*****************************/

    else if ( pkt_gen_cfg.fltr_rule_cat_type == IPA_TEST_ENGINE_FLTR_RULE_UL_QOS )
    {
      /*If Directed test, get index into the qos rule cfg table for the test*/
      if( ipa_test_filter.fltr_rule_type != IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX )
      {
        qos_rule_match_index = ipa_test_filter_get_qos_rule_cfg_index(ipa_test_filter.fltr_rule_type);
      }
      ipa_test_filter_ul_qos_gen_ip_pkt(pkt_gen_cfg, ip_version_type, qos_rule_match_index);
    }/*IPA_TEST_FILTER_RULE_UL_QOS*/
    /*! *************UL Static + QoS Filter Processing*********************/
    else if ( pkt_gen_cfg.fltr_rule_cat_type == IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_AND_QOS )
    {
       /*Generate Random UL static filter based on ip version*/
       if ( IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX == ipa_test_filter.fltr_rule_type )
       {
         switch ( ip_version_type )
         {
           case 4 :
           {
             pkt_gen_cfg.fltr_rule_type = 
              (ipa_test_engine_fltr_type_e)ipa_test_generate_random_num
              (IPA_TEST_ENGINE_FLTR_TYPE_UL_STATIC_MAX_V4, 
               IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4);
             break;
           }
           case 6 :
           {
             pkt_gen_cfg.fltr_rule_type = 
               (ipa_test_engine_fltr_type_e)ipa_test_generate_random_num
               (IPA_TEST_ENGINE_FLTR_TYPE_UL_STATIC_MAX, 
                IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6);
             break;
           }
         }
       }
       /*If no UL static filter only chosen generate random QoS filter*/
       if ( (IPA_TEST_ENGINE_FLTR_TYPE_UL_STATIC_MAX_V4 == pkt_gen_cfg.fltr_rule_type) ||
           (IPA_TEST_ENGINE_FLTR_TYPE_UL_STATIC_MAX == pkt_gen_cfg.fltr_rule_type) )
       {
         /*Generate Random QoS filters*/
         switch ( ip_version_type )
         {
           case 4 :
           {
             pkt_gen_cfg.fltr_rule_type = 
               (ipa_test_engine_fltr_type_e) ipa_test_generate_random_num 
               (IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE_MAX_V4, 
               IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE1_V4);
             break;
           }
           case 6 :
           { 
             pkt_gen_cfg.fltr_rule_type = 
               (ipa_test_engine_fltr_type_e)ipa_test_generate_random_num
               (IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX, IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE9_V6);
             break;
           }
         }
         /*If no QOS only filter chosen, generate static + qos rule pkt*/
         /*temp dbg*/
         pkt_gen_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE_MAX_V4;
         if ( IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE_MAX_V4 == pkt_gen_cfg.fltr_rule_type  ||
              IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX == pkt_gen_cfg.fltr_rule_type )
         {
           /*Generate static + qos pkt*/
           switch (ip_version_type)
           {
             case 4 :
             { /*Qos pkt*/
               ipa_test_filter_ul_qos_gen_ip_pkt(pkt_gen_cfg,ip_version_type,IPV4_QOS_RULE1); /*TOS*/
               /*Static filter pkt*/
               pkt_gen_cfg.fltr_rule_type = 
              (ipa_test_engine_fltr_type_e)ipa_test_generate_random_num
              (IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V4, IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V4);
               rand_filter = ipa_test_filter_get_ul_static_rand_filter(pkt_gen_cfg.fltr_rule_type);
               pkt_gen_cfg.ip_pkt_info_ptr->filter_result = 
                 ipa_test_filter.ul_static_rule_cfg[rand_filter].rule_result;       
               ipa_test_filter_ul_static_gen_ip_pkt(pkt_gen_cfg, ip_version_type, rand_filter);              
               break;
             }
             case 6 :
             {  /*Qos pkt*/
               ipa_test_filter_ul_qos_gen_ip_pkt\
                 (pkt_gen_cfg,ip_version_type,IPV6_QOS_RULE15); /*Traffic class eqn*/
               /*Static filter pkt*/
               pkt_gen_cfg.fltr_rule_type = 
                 (ipa_test_engine_fltr_type_e)ipa_test_generate_random_num
                 (IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6, IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V6);
               rand_filter = 
                 ipa_test_filter_get_ul_static_rand_filter(pkt_gen_cfg.fltr_rule_type);
               pkt_gen_cfg.ip_pkt_info_ptr->filter_result = 
                 ipa_test_filter.ul_static_rule_cfg[rand_filter].rule_result;       
               ipa_test_filter_ul_static_gen_ip_pkt(pkt_gen_cfg, ip_version_type, rand_filter);
               break;
             }
           }
         }
         else
         {  /*Random QoS filter*/
            qos_rule_match_index = 
              ipa_test_filter_get_qos_rule_cfg_index(ipa_test_filter.fltr_rule_type);
            ipa_test_filter_ul_qos_gen_ip_pkt \
              (pkt_gen_cfg, ip_version_type, qos_rule_match_index);
         }
       }
       else
       {
         /* Generate packet to match the corresponding static filter*/
         rand_filter =
            ipa_test_filter_get_ul_static_rand_filter(pkt_gen_cfg.fltr_rule_type);
         pkt_gen_cfg.ip_pkt_info_ptr->filter_result =
            ipa_test_filter.ul_static_rule_cfg[rand_filter].rule_result;
         if ( (rand_filter == IPV4_TCP_ACK) ||(rand_filter == IPV6_TCP_ACK) ) 
         {
           ipa_test_filter.is_tcp_ack_only = TRUE;
         }
         ipa_test_filter_ul_static_gen_ip_pkt(pkt_gen_cfg, ip_version_type, rand_filter);
       }
    }/*IPA_TEST_FILTER_RULE_UL_STATIC_AND_QOS*/
    /*! *************TCP ACK + QoS Filter Processing*********************/
    else if ( pkt_gen_cfg.fltr_rule_cat_type == 
              IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_QOS_TCP_ACK )
    {
      ipa_test_filter.filter_tcp_ack_gen = TRUE; 
      /*To generate TCP ACK only for the last pkt in directed test*/
      pkt_gen_cfg.ip_pkt_info_ptr->is_prioritized = FALSE;
      switch ( ip_version_type )
      {
        case 4 :
        { 
          pkt_gen_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4;
          rand_filter = IPV4_TCP_ACK;
          if ( ipa_test_filter.curr_pkts != ipa_test_filter.total_pkts )
          {
            if( IPA_TEST_UDP_PROT ==
                      IPA_TEST_GET_IP_PROTOCOL(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr))
            {
              rand_filter = IPV4_DHCP;
            }
            /*This is needed if first 3 pkts are some valid data pkts which doesnt need to match any filter*/
            ipa_test_filter.filter_tcp_ack_gen = FALSE;
          }
          break;
        }
        case 6:
        {
          pkt_gen_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6;
          rand_filter = IPV6_TCP_ACK;
          /*Re Generate TCP ACK pkt only for the last packet out of 4 pkts*/
          if ( ipa_test_filter.curr_pkts != ipa_test_filter.total_pkts )
          {
            /*Generate valid ICMPv6 or DHCPv6 filter pkt and Prioritize it*/
            if ( IPA_TEST_ICMP_PROT ==
                  IPA_TEST_GET_IP_NXT_HEADER(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr) )
            {
              rand_filter = IPV6_ICMP_REQUEST;
            }
            else if ( IPA_TEST_UDP_PROT ==
                       IPA_TEST_GET_IP_NXT_HEADER(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr) )
            {
              rand_filter = IPV6_DHCP;
            }
            ipa_test_filter.filter_tcp_ack_gen = FALSE;
          }
          break;
        }
        default:
          break;
      }
      if ( IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4 == pkt_gen_cfg.fltr_rule_type && 
           (ipa_test_filter.filter_tcp_ack_gen == FALSE) && 
           (IPV4_DHCP != rand_filter) )
        break;

      /*No need to generate filter pkt for valid data pkts for TCP_ACK V6 test*/
      if (IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6 == pkt_gen_cfg.fltr_rule_type &&
           (ipa_test_filter.filter_tcp_ack_gen == FALSE) && 
           (IPV6_ICMP_REQUEST != rand_filter && IPV6_DHCP != rand_filter))
        break;
       /***! Generate TCP ACK QOS packet***/
      /*1.Generate packet based on rand_filter*/
      pkt_gen_cfg.ip_pkt_info_ptr->filter_result =
         ipa_test_filter.ul_static_rule_cfg[rand_filter].rule_result;
      ipa_test_filter_ul_static_gen_ip_pkt(pkt_gen_cfg, ip_version_type, rand_filter);
      /*2.Generate TCP ACK QOS packet if TCP ACK pkt was generated previously*/
      if (IPV4_TCP_ACK == rand_filter)
      {
         ipa_test_filter_ul_qos_gen_ip_pkt(pkt_gen_cfg,4,IPV4_QOS_RULE1); /*TOS*/
      }
   }/*IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_QOS_TCP_ACK*/
  }while(0);
}

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
)
{
  uint8 rand_filter = IPA_TEST_MAX_RAND_FILTER_TYPE;

  switch (fltr_rule_type)
  {
    case IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4:
    {
      rand_filter = IPV4_TCP_ACK;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V4:
    {
      rand_filter = IPV4_DHCP;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V4:
    {
      rand_filter = IPV4_FRAG;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V6:
    {
      rand_filter = IPV6_FRAG;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6:
    { rand_filter = IPV6_TCP_ACK;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V6:
    {
      rand_filter = IPV6_DHCP;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REQ_V6:
    {
      rand_filter = IPV6_ICMP_REQUEST;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REP_V6:
    {
      rand_filter = IPV6_ICMP_REPLY;      
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6:
    {
      rand_filter = IPV6_ICMP_NDP;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_ANY_ICMP_V4:
    {
      rand_filter = IPV4_CLAT_ANY_ICMP;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_UDP_CHKSUM_ZERO_V4:
    {
      rand_filter = IPV4_CLAT_UDP_CHKSUM_ZERO;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_TTL_V4:
    {
      rand_filter = IPV4_CLAT_TTL;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_DNS_PROXY_V4:
    {
      rand_filter = IPV4_CLAT_DNS_PROXY;
      break;
    }
    case IPA_TEST_ENGINE_FLTR_TYPE_CLAT_FRAG_V4:
    {
      rand_filter = IPV4_CLAT_FRAG_V4;
      break;
    }
    default:
      break;
  }
  return rand_filter;
}/*ipa_test_filter_get_ul_static_rand_filter()*/

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
)
{
  uint8 *pkt_ptr = pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr;
  ipa_test_ret_code_e    ret_code;
  uint8 j;
  uint16 ihl_value;

  if (pkt_ip_type == 4) 
  {
    ihl_value = 20;
  }
  else
  {
    ihl_value = ipa_test_filter.ihl_value;
  }
  if(rand_filter == IPV6_TCP_ACK)
  {
    /*Free current packet and generate TCP ACK pkt.
    Set is_prioritized flag to priority enqueue.*/
    if (pkt_ptr != NULL)
    {
        IPA_FREE(pkt_ptr);
        ret_code = ipa_test_filter_gen_tcp_ack_packet
        (pkt_gen_cfg.ip_pkt_info_ptr, IPV6_TCP_ACK);
        IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
        pkt_gen_cfg.ip_pkt_info_ptr->is_prioritized = TRUE;
    }
    return;
  }

  if (rand_filter == IPV4_TCP_ACK) 
  {/*Free current packet and generate TCP ACK pkt. 
          Set is_prioritized flag to priority enqueue.*/
    if (pkt_ptr != NULL)
    {
      IPA_FREE(pkt_ptr);
      ret_code = ipa_test_filter_gen_tcp_ack_packet\
                 (pkt_gen_cfg.ip_pkt_info_ptr, IPV4_TCP_ACK);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
      pkt_gen_cfg.ip_pkt_info_ptr->is_prioritized = TRUE;
    }
    return;
  }
  /***********! Generate packet to match the corresponding static filter*******************/

  /*! Only if the packet is IPV4 check for TOS and Protocol equations */
  if ( pkt_ip_type == 4 )
  {
    /*! Vary the packet to match or not match the TOS equation present in the rule*/
    if( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.tos_eq == 1 )
    {
      ipa_test_filter_match_tos_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                     ipa_test_filter.ul_static_rule_cfg[rand_filter].tos,
                                     1);
    
    }
  }

  if ( pkt_ip_type == 6 )
  {
    if( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.tc_eq == 1 )
    {
      ipa_test_filter_match_tc_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                    ipa_test_filter.ul_static_rule_cfg[rand_filter].traffic_class,
                                    1);
    }
    if( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.fl_eq == 1 )
    {
      ipa_test_filter_match_fl_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                    ipa_test_filter.ul_static_rule_cfg[rand_filter].flow_label,
                                    1);
    }
  }

  if ( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.protocol_eq  == 1 )
  {
    /*! Vary the packet to match or not match the Protocol equation present in the rule*/
    ipa_test_filter_match_protocol_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                      ipa_test_filter.ul_static_rule_cfg[rand_filter].protocol,
                                      pkt_ip_type,
                                      1);
  }
  if ( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.offset_meq_32  == 1 ) 
  {
    for ( j = 0; j < ipa_test_filter.ul_static_rule_cfg[rand_filter].eq_count.num_offset_meq_32 ; j++ )
    {
      ipa_test_filter_match_meq_32(&ipa_test_filter.ul_static_rule_cfg[rand_filter].meq_32[j],
                                     pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                     1,
                                     0);
    }
  }

  if ( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.offset_meq_128  == 1 ) 
  {
    for ( j = 0; j < ipa_test_filter.ul_static_rule_cfg[rand_filter].eq_count.num_offset_meq_128 ; j++ )
    {
      ipa_test_filter_match_meq_128(&ipa_test_filter.ul_static_rule_cfg[rand_filter].meq_128[j],
                                      pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                      1);
    }
  }

  if ( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.ihl_offset_eq_16  == 1 )
  {
    ipa_test_filter_match_eq_16(&ipa_test_filter.ul_static_rule_cfg[rand_filter].ihl_offset_eq_16,
                                  pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                  1,
                                  ihl_value);
  }

  if ( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.ihl_offset_range_16  == 1 )
  {
    for ( j = 0 ; j < ipa_test_filter.ul_static_rule_cfg[rand_filter].eq_count.num_ihl_offset_range_16 ; j++)
    {
      ipa_test_filter_match_range_16(&ipa_test_filter.ul_static_rule_cfg[rand_filter].ihl_range_eq_16[j],
                                       pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                       1,
                                       ihl_value);
    }
  }

  if ( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.ihl_offset_eq_32  == 1 ) 
  {
    ipa_test_filter_match_eq32(&ipa_test_filter.ul_static_rule_cfg[rand_filter].ihl_eq_32,
                                 pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                 1,
                                 ihl_value);
  }

  if ( ipa_test_filter.ul_static_rule_cfg[rand_filter].bitmap.bits.ihl_offset_meq_32  == 1 ) 
  {
    for ( j = 0; j < ipa_test_filter.ul_static_rule_cfg[rand_filter].eq_count.num_ihl_offset_meq_32; j++ )
    {
      ipa_test_filter_match_meq_32(&ipa_test_filter.ul_static_rule_cfg[rand_filter].ihl_meq_32[j],
                                     pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                     1,
                                     ihl_value);
    }
  }
  if (rand_filter == IPV6_ICMP_NDP)
  {
    ipa_test_filter_match_8(0x00, &pkt_ptr[ihl_value + 1]);
    if (pkt_ptr[ihl_value] == (uint8)ipa_ntohs
        (ipa_test_filter.ul_static_rule_cfg[rand_filter].ihl_range_eq_16[0].range_low))
    {
      pkt_ptr[ihl_value] = pkt_ptr[ihl_value] + 1;
    }
  }  
}

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
)
{
  ipa_ipfltr_ip_type_e filter_ip_type = IPA_IPFLTR_NUM_IP_TYPES;
  uint8 should_match = FALSE; 
  uint8 rule_match = 0xFF;
  uint8 i,j;
  uint8 pkt_complt_flag = 0;
  uint8 ihl_value;
  
  if (pkt_ip_type == 4) 
  {
    ihl_value = 20;
    filter_ip_type = IPA_IPFLTR_IPV4;
  }
  else
  {
     filter_ip_type = IPA_IPFLTR_IPV6;
     ihl_value = ipa_test_filter.ihl_value;
  }
  /*Generate Random QOS Rule if it is not a directed test*/
  if (rule_match_index == IPA_TEST_MAX_RAND_FILTER_TYPE)
  {
     if (pkt_ip_type == 4)
     { 
       rule_match = ipa_test_generate_random_num
                        (IPA_TEST_FILTER_MAX_QOS_V4_RULES , 0);
       
     }
     else
     {
         rule_match = ipa_test_generate_random_num
                      (IPA_TEST_FILTER_MAX_QOS_V6_RULES , 8);
     }
  }
  else
  {
     rule_match = rule_match_index;
  }
  for ( i = 0; i <= ipa_test_filter.num_ul_qos_fltr ; i++ )
  { 
    while(( i == rule_match )
             && ( i != ipa_test_filter.num_ul_qos_fltr ))
    {
        i++;
    }
     /*! When all the active rules except the rule to be matched are used up 
         match the packet to the rule to be matched.*/
    if ( i == (ipa_test_filter.num_ul_qos_fltr) ) 
    {
      i = rule_match;
      /*! The packet needs to match this particular rule*/
      should_match = TRUE;
      /*! Return the filter result of this matched rule*/
      pkt_gen_cfg.ip_pkt_info_ptr->filter_result = 
        ipa_test_filter.ul_qos_rule_cfg[rule_match].rule_result;
      /*! Packet has been completely generated flag*/
      pkt_complt_flag = 1;
    }
     
    /*! Only if the packet is IPV4 check for TOS and Protocol equations */
    if ( pkt_ip_type == 4 )
    {
      /*! Vary the packet to match or not match the TOS equation present in the rule*/
      if( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.tos_eq == 1 )
      {
        ipa_test_filter_match_tos_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                     ipa_test_filter.ul_qos_rule_cfg[i].tos,
                                     should_match);
     
      }

      if ( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.protocol_eq  == 1 )
      {
        /*! Vary the packet to match or not match the Protocol equation present in the rule*/
        ipa_test_filter_match_protocol_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                          ipa_test_filter.ul_qos_rule_cfg[i].protocol,
                                          pkt_ip_type,
                                          should_match);
       }
    }

    if ( pkt_ip_type == 6 )
    {

	  ipa_test_filter_match_tc_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                     ipa_test_filter.ul_qos_rule_cfg[i].tos,
                                     FALSE);

	  
      if( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.tc_eq == 1 )
      {
        ipa_test_filter_match_tc_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                    ipa_test_filter.ul_qos_rule_cfg[i].traffic_class,
                                    should_match);
      }
      if( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.fl_eq == 1 )
      {
        ipa_test_filter_match_fl_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                    ipa_test_filter.ul_qos_rule_cfg[i].flow_label,
                                    should_match);
      }
    }

    if ( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.offset_meq_32  == 1 ) 
    {
      for ( j = 0; j < ipa_test_filter.ul_qos_rule_cfg[i].eq_count.num_offset_meq_32 ; j++ )
      {
        ipa_test_filter_match_meq_32(&ipa_test_filter.ul_qos_rule_cfg[i].meq_32[j],
                                     pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                     should_match,
                                     0);
      }
    }

    if ( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.offset_meq_128  == 1 ) 
    {
      for ( j = 0; j < ipa_test_filter.ul_qos_rule_cfg[i].eq_count.num_offset_meq_128 ; j++ )
      {
        ipa_test_filter_match_meq_128(&ipa_test_filter.ul_qos_rule_cfg[i].meq_128[j],
                                      pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                      should_match);
      }
    }

    if ( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.ihl_offset_eq_16  == 1 )
    {
      ipa_test_filter_match_eq_16(&ipa_test_filter.ul_qos_rule_cfg[i].ihl_offset_eq_16,
                                  pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                  should_match,
                                  ihl_value);
    }

    if ( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.ihl_offset_range_16  == 1 )
    {
      for ( j = 0 ; j < ipa_test_filter.ul_qos_rule_cfg[i].eq_count.num_ihl_offset_range_16 ; j++)
      {
        ipa_test_filter_match_range_16(&ipa_test_filter.ul_qos_rule_cfg[i].ihl_range_eq_16[j],
                                       pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                       should_match,
                                       ihl_value);
      }
    }

    if ( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.ihl_offset_eq_32  == 1 ) 
    {
      ipa_test_filter_match_eq32(&ipa_test_filter.ul_qos_rule_cfg[i].ihl_eq_32,
                                 pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                 should_match,
                                 ihl_value);
        
    }

    if ( ipa_test_filter.ul_qos_rule_cfg[i].bitmap.bits.ihl_offset_meq_32  == 1 ) 
    {
      for ( j = 0; j < ipa_test_filter.ul_qos_rule_cfg[i].eq_count.num_ihl_offset_meq_32; j++ )
      {
        ipa_test_filter_match_meq_32(&ipa_test_filter.ul_qos_rule_cfg[i].ihl_meq_32[j],
                                     pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
                                     should_match,
                                     ihl_value);
      }
    }

    if (pkt_complt_flag == 1) 
    {
      break;
    }
  }   
}/*ipa_test_filter_ul_qos_gen_ip_pkt()*/

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
)
{
   switch ( fltr_rule_type )
   {
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE1_V4:
     {
       return IPV4_QOS_RULE1;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE2_V4:
     {
       return IPV4_QOS_RULE2;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE3_V4:
     {
       return IPV4_QOS_RULE3;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE4_V4:
     {
       return IPV4_QOS_RULE4;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE5_V4:
     {
       return IPV4_QOS_RULE5;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE6_V4:
     {
       return IPV4_QOS_RULE6;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE7_V4:
     {
       return IPV4_QOS_RULE7;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE8_V4:
     {
       return IPV4_QOS_RULE8;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE9_V6:
     {
       return IPV6_QOS_RULE9;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE10_V6:
     {
       return IPV6_QOS_RULE10;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE11_V6:
     {
       return IPV6_QOS_RULE11;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE12_V6:
     {
       return IPV6_QOS_RULE12;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE13_V6:
     {
       return IPV6_QOS_RULE13;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE14_V6:
     {
       return IPV6_QOS_RULE14;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE15_V6:
     {
       return IPV6_QOS_RULE15;
       break;
     }
     case IPA_TEST_ENGINE_FLTR_TYPE_QOS_RULE16_V6:
     {
       return IPV6_QOS_RULE16;
       break;
     }
   default:
     IPA_TF_ASSERT(0);
   }
}

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
)
{

  ipa_ipfltr_ip_type_e   filter_ip_type;
  uint8                  pkt_ip_type, rule_match_index = 0xFF;

  /*! Get the IHL value for IPV6 filters from the test engine 
  This includes the IP header length and the extension header length.*/
  ipa_test_filter.ihl_value = pkt_gen_cfg.ip_pkt_info_ptr->ip_hdr_len +
                              pkt_gen_cfg.ip_pkt_info_ptr->extn_hdr_len;

  if ( ipa_test_filter.is_frag_case == TRUE )
  {
    pkt_gen_cfg.ip_pkt_info_ptr->filter_result = ipa_test_filter.frag_filter_result;
    return;
  }

  pkt_ip_type = IPA_TEST_GET_IP_VERSION(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr); 
  if ( pkt_ip_type == 4 )
  { 
    rule_match_index = ipa_test_generate_random_num(
      IPA_TEST_FILTER_MAX_DL_V4_RULES,
      0);
    filter_ip_type = IPA_IPFLTR_IPV4;
    if ( ipa_test_engine_get_if_frag_case() == TRUE )
    {
      rule_match_index = 0;
    }
  }
  else
  {
    rule_match_index = ipa_test_generate_random_num(
      IPA_TEST_FILTER_MAX_DL_V6_RULES, 
      IPA_TEST_FILTER_MAX_DL_V4_RULES + 1);
    filter_ip_type = IPA_IPFLTR_IPV6;
  }
  /* Generate the ip packet according to the rule to be matched*/
  ipa_test_filter_dl_gen_ip_pkt(pkt_gen_cfg, filter_ip_type, rule_match_index);
  if ( ipa_test_engine_get_if_frag_case() == TRUE )
  {
    ipa_test_filter.is_frag_case = TRUE;
  }
}

/*==========================================================================

 FUNCTION: ipa_test_filter_dl_gen_ip_pkt

==========================================================================*/
/*!
  @brief
  Configures IP packet config stucture used to generate IP packet
  according to the filter it needs to be matched.

  @return
  None
*/
/*=========================================================================*/
void ipa_test_filter_dl_gen_ip_pkt
(
  ipa_test_filter_pkt_gen_cfg_s   pkt_gen_cfg,
  ipa_ipfltr_ip_type_e            filter_ip_type,
  uint8                           rule_match_index
)
{
  uint8        pkt_complt_flag = 0, pkt_ip_type = 0;
  uint8        should_match = FALSE, i, j;

  /* Since the IPV6 packets might have an extension header the IHL value to be 
  used depends on the packet configuration i.e whether extension hdr length should be 
  added or not. */
  if (filter_ip_type == IPA_IPFLTR_IPV4) 
  {
    ipa_test_filter.ihl_value  = 20;
  }
  else
  {
    ipa_test_filter.ihl_value = pkt_gen_cfg.ip_pkt_info_ptr->ip_hdr_len +
                                pkt_gen_cfg.ip_pkt_info_ptr->extn_hdr_len;
  }

  for ( i = 0; i <= ipa_test_filter.num_dl_sckt_fltr ; i++ )
  { 
    while ((ipa_test_filter.dl_sckt_rule_cfg[i].\
            ip_type != filter_ip_type || (i == rule_match_index)) &&
           (i != ipa_test_filter.num_dl_sckt_fltr))
    {
      i++;
    }
    /*When all the active rules except the rule to be matched are used up 
        match the packet to the rule to be matched.*/
    if ( i == ipa_test_filter.num_dl_sckt_fltr ) 
    {
      i = rule_match_index;
      /*! The packet needs to match this particular rule*/
      should_match = TRUE;
      /*! Return the filter result of this matched rule*/
      pkt_gen_cfg.ip_pkt_info_ptr->filter_result = 
      ipa_test_filter.dl_sckt_rule_cfg[rule_match_index].rule_result;
      ipa_test_filter.frag_filter_result = 
        ipa_test_filter.dl_sckt_rule_cfg[rule_match_index].rule_result;
      /*! Packet has been completely generated flag*/
      pkt_complt_flag = 1;
    }
    /*! Only if the packet is IPV4 check for TOS and Protocol equations */
    if ( filter_ip_type == IPA_IPFLTR_IPV4 )
    {
      pkt_ip_type = 4;
      /*! Vary the packet to match or not match the TOS equation 
          present in the rule*/
      if(ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.tos_eq == 1)
      {
        ipa_test_filter_match_tos_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
           ipa_test_filter.dl_sckt_rule_cfg[i].tos,
           should_match);
      }
    }
    
    if ( filter_ip_type == IPA_IPFLTR_IPV6 )
    {
      pkt_ip_type = 6;
      if(ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.tc_eq == 1)
      {
        ipa_test_filter_match_tc_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
           ipa_test_filter.dl_sckt_rule_cfg[i].traffic_class,
           should_match);
      }

      if( ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.fl_eq == 1 )
      {
        ipa_test_filter_match_fl_eq(pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
           ipa_test_filter.dl_sckt_rule_cfg[i].flow_label,
           should_match);
      }
    }

    if ( ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.protocol_eq == 1 )
    {
        /*! Vary the packet to match or not match the Protocol equation 
            present in the rule*/
        ipa_test_filter_match_protocol_eq
          (pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
           ipa_test_filter.dl_sckt_rule_cfg[i].protocol,
           pkt_ip_type,
           should_match);
     }

    if (ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.offset_meq_32 == 1) 
    {
      for (j = 0;
            j < ipa_test_filter.dl_sckt_rule_cfg[i].eq_count.num_offset_meq_32;
            j++ )
      {
        ipa_test_filter_match_meq_32
          (&ipa_test_filter.dl_sckt_rule_cfg[i].meq_32[j],
           pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
           should_match,
           0);
      }
    }

    if (ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.offset_meq_128 == 1) 
    {
      for (j = 0;
            j < ipa_test_filter.dl_sckt_rule_cfg[i].eq_count.num_offset_meq_128;
            j++ )
      {
        ipa_test_filter_match_meq_128
          (&ipa_test_filter.dl_sckt_rule_cfg[i].meq_128[j],
           pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
           should_match);
      }
    }

    if (ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.ihl_offset_eq_16 == 1)
    {
      ipa_test_filter_match_eq_16
        (&ipa_test_filter.dl_sckt_rule_cfg[i].ihl_offset_eq_16,
         pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
         should_match,
         ipa_test_filter.ihl_value);
    }

    if (ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.ihl_offset_range_16 == 1)
    {
      for (j = 0;
            j < ipa_test_filter.dl_sckt_rule_cfg[i].eq_count.num_ihl_offset_range_16;
            j++ )
      {
        ipa_test_filter_match_range_16
          (&ipa_test_filter.dl_sckt_rule_cfg[i].ihl_range_eq_16[j],
           pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
           should_match,
           ipa_test_filter.ihl_value);
      }
    }

    if (ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.ihl_offset_eq_32 == 1) 
    {
      ipa_test_filter_match_eq32
        (&ipa_test_filter.dl_sckt_rule_cfg[i].ihl_eq_32,
         pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
         should_match,
         ipa_test_filter.ihl_value);
        
    }

    if (ipa_test_filter.dl_sckt_rule_cfg[i].bitmap.bits.ihl_offset_meq_32 == 1) 
    {
      for (j = 0;
            j < ipa_test_filter.dl_sckt_rule_cfg[i].eq_count.num_ihl_offset_meq_32;
            j++ )
      {
          ipa_test_filter_match_meq_32
            (&ipa_test_filter.dl_sckt_rule_cfg[i].ihl_meq_32[j],
             pkt_gen_cfg.ip_pkt_info_ptr->pkt_buf_ptr,
             should_match,
             ipa_test_filter.ihl_value);
      }
    }

    if (pkt_complt_flag == 1) 
    {
      break;
    }
  }
}

/*==========================================================================

 FUNCTION: ipa_test_filter_get_tcp_ack_only_flag

==========================================================================*/
/*!
  @brief
  @return
  Return  TRUE if it is TCP ACK only test case or else FALSE
  
*/
/*=========================================================================*/
boolean ipa_test_filter_get_tcp_ack_only_flag()
{
  return ipa_test_filter.is_tcp_ack_only;
}

/*==========================================================================

 FUNCTION: ipa_test_filter_gen_tcp_ack_packet

==========================================================================*/
/*!
  @brief
  Generates a TCP ACK packet.

  @return
  IPA_TEST_RET_CODE_SUCCESS if all the filter rules are successfully destroyed.
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_filter_gen_tcp_ack_packet
( 
  ipa_test_engine_ip_pkt_info_s *pkt_info,
  uint8                         ip_type
)
{
  ipa_test_engine_ip_pkt_cfg_s   tcp_ack_pkt_cfg;

  switch (ip_type)
  {
    case IPV4_TCP_ACK:
    {
      tcp_ack_pkt_cfg.ip_hdr_len = IPA_TEST_IPV4_HDR_LEN;
      pkt_info->ip_hdr_len = IPA_TEST_IPV4_HDR_LEN;
      tcp_ack_pkt_cfg.version = IPA_IP_TYPE_IPV4;
      break;
    }
    case IPV6_TCP_ACK:
    {
      tcp_ack_pkt_cfg.ip_hdr_len = IPA_TEST_IPV6_HDR_LEN;
      pkt_info->ip_hdr_len = IPA_TEST_IPV6_HDR_LEN;
      tcp_ack_pkt_cfg.version = IPA_IP_TYPE_IPV6;
      break;
    }
  }
  tcp_ack_pkt_cfg.num_dsm_items = 1;
  tcp_ack_pkt_cfg.payload_len = 0;
  tcp_ack_pkt_cfg.num_pkts = 1;
  tcp_ack_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  tcp_ack_pkt_cfg.is_random_data = FALSE;

  return(ipa_test_generate_tcp_ack_packet(&tcp_ack_pkt_cfg, pkt_info));
}

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
ipa_test_ret_code_e ipa_test_filter_destroy()
{
  uint8 i = 0;
  /*!Destroy UL QOS filters*/
  if (ipa_test_filter.num_ul_qos_fltr > 0)
  {
    IPA_TF_ASSERT(ipa_ipfltri_remove_rules(ipa_test_filter.num_ul_qos_fltr,
                        ipa_test_filter.ul_qos_hdl_arr) == IPA_SUCCESS);
  }
  for ( i = 0; i < ipa_test_filter.num_ul_qos_fltr; i++ )
  {
    IPA_MEMSET(ipa_test_filter.ul_qos_hdl_arr[i],
               0,sizeof(ipa_ipfltr_rule_handle_t));
  }
  /*!Destroy DL Socket filters*/
  if( ipa_test_filter.num_dl_sckt_fltr > 0 )
  {
    IPA_TF_ASSERT(ipa_ipfltri_remove_rules(\
      ipa_test_filter.num_dl_sckt_fltr,
      ipa_test_filter.dl_socket_hdl_arr) == IPA_SUCCESS);
  }
  for (i = 0; i < ipa_test_filter.num_dl_sckt_fltr; i++)
  {
    IPA_MEMSET(ipa_test_filter.dl_socket_hdl_arr[i], \
               0, sizeof(ipa_ipfltr_rule_handle_t));
  }
  /*! Remove all UL static filters*/
  IPA_TF_ASSERT(ipa_ipfltri_remove_rules(ipa_test_filter.num_ul_static_fltr, 
                           ipa_test_filter.ul_static_hdl_arr) == IPA_SUCCESS);
  for ( i = 0;i < ipa_test_filter.num_ul_static_fltr ; i++ )
  {
    IPA_MEMSET(ipa_test_filter.ul_static_hdl_arr[i], \
               0,sizeof(ipa_ipfltr_rule_handle_t));
  }
  /*! Destroy the TCP ACK filter*/
  while (ipa_ipfltr.fltr.ul.pending.pending_activation_flag == TRUE) {
    ipa_sleep(2);
  }
  
  /*! Reset the frag flag*/
  ipa_test_filter.is_frag_case =  FALSE;

  return IPA_TEST_RET_CODE_SUCCESS;
}

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
ipa_test_ret_code_e ipa_test_ul_static_filter_init()
{
  /*! *********************TCP ACK V4/V6****************************************/
  ipa_test_filter.ul_static_rule_cfg[IPV4_TCP_ACK].rule_result = IPV4_TCP_ACK_FLTR_RES;
  ipa_test_filter.ul_static_rule_cfg[IPV6_TCP_ACK].rule_result = IPV6_TCP_ACK_FLTR_RES;

   /*! **********************DHCP V4 ***************************************************/ 
  /*!Setting the static rule structure for DHCP V4*/
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].rule_type = IPA_IPFLTR_RULE_UL_STATIC_LOCAL;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ihl_value = 20;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].eq_count.num_ihl_offset_meq_32 = 1;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].eq_count.num_ihl_offset_range_16 = 0 ;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].bitmap.value = 0x208;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].protocol = IPA_TEST_UDP_PROT;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ihl_meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ihl_meq_32[0].offset = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ihl_meq_32[0].value = 0x43;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].usr_data = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].rule_result = IPV4_DHCP_FLTR_RES;
 
  IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV4_DHCP] = 
                 ipa_ipfltri_alloc_rule_handle( \
                   ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].rule_type,
                   ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ip_type,
                   TRUE,
                   ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].rule_result,
                   ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_protocol_eq( \
    ipa_test_filter.ul_static_hdl_arr[IPV4_DHCP],
    ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].protocol) == IPA_IPFLTR_SUCCESS);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32( \
    ipa_test_filter.ul_static_hdl_arr[IPV4_DHCP],
    ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ihl_meq_32[0].offset,
    ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ihl_meq_32[0].mask,
    ipa_test_filter.ul_static_rule_cfg[IPV4_DHCP].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS);
  /*! Commit first 3 rules out of 7*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(1,&ipa_test_filter.ul_static_hdl_arr[IPV4_DHCP])
                != IPA_ERR_IPFLTR_COMMIT_FAIL);
  
  /*! ICMP Echo Request V6*/
  /*!Setting the static rule structure for ICMP Echo request V6*/
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].rule_type = IPA_IPFLTR_RULE_UL_STATIC_LOCAL;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ihl_value = 40;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].eq_count.num_ihl_offset_meq_32 = 1;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].eq_count.num_ihl_offset_range_16 = 0 ;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].protocol = 0x3A;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].bitmap.value = 0x0208;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ihl_meq_32[0].mask = 0xFFFF0000;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ihl_meq_32[0].offset = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ihl_meq_32[0].value = 0x80000000;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].usr_data = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].rule_result = IPV6_ICMP_REQ_FLTR_RES;
  IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_REQUEST] = 
                 ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].rule_type,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ip_type,
                  TRUE,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].rule_result,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].usr_data))!= NULL);
  IPA_TF_ASSERT(
    ipa_ipfltri_add_protocol_eq(ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_REQUEST],
                              ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].protocol)
	                      == IPA_IPFLTR_SUCCESS); 
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32 \
                (ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_REQUEST],
                 ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ihl_meq_32[0].offset,
                 ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ihl_meq_32[0].mask,
                 ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REQUEST].ihl_meq_32[0].value) 
                == IPA_IPFLTR_SUCCESS);
  /*! ICMP Echo Reply V6*/
  /*!Setting the static rule structure for ICMP Echo Reply V6*/
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].rule_type = IPA_IPFLTR_RULE_UL_STATIC_LOCAL;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ihl_value = 40;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].eq_count.num_ihl_offset_meq_32 = 1;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].eq_count.num_ihl_offset_range_16 = 0 ;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].bitmap.value = 0x0208;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].protocol = 0x3A;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ihl_meq_32[0].mask = 0xFFFF0000;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ihl_meq_32[0].offset = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ihl_meq_32[0].value = 0x81000000;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].usr_data = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].rule_result = IPV6_ICMP_REPLY_FLTR_RES;

  IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_REPLY] =
                 ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].rule_type,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ip_type,
                  TRUE,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].rule_result,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].usr_data))!= NULL);
  IPA_TF_ASSERT(
    ipa_ipfltri_add_protocol_eq(ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_REPLY],
                              ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].protocol)
	                      == IPA_IPFLTR_SUCCESS);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32 \
                (ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_REPLY],
                 ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ihl_meq_32[0].offset,
                 ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ihl_meq_32[0].mask,
                 ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_REPLY].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS);
  /*! ICMP6 NDP */
  /*!Setting the static rule structure for ICMP6 NDP*/
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].rule_type = IPA_IPFLTR_RULE_UL_STATIC_LOCAL;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].ihl_value = 40;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].eq_count.num_ihl_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].eq_count.num_ihl_offset_range_16 = 1 ;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].bitmap.value = 0x48;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].protocol = 0x3A;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].usr_data = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].rule_result = IPV6_ICMP_NDP_FLTR_RES;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].ihl_range_eq_16[0].offset = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].ihl_range_eq_16[0].range_low = 0x8500;
  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].ihl_range_eq_16[0].range_high = 0x8900;
  IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_NDP] =
                 ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].rule_type,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].ip_type,
                  TRUE,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].rule_result,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].usr_data))!= NULL);

  IPA_TF_ASSERT(
    ipa_ipfltri_add_protocol_eq(ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_NDP],
                               ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].protocol)
			       == IPA_IPFLTR_SUCCESS);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16 \
                (ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_NDP],
                 ipa_test_filter.ul_static_rule_cfg[IPV6_ICMP_NDP].ihl_range_eq_16[0].offset,
                 0x8500,
                 0x8900)
                == IPA_IPFLTR_SUCCESS); 
  /*! DHCP V6*/
  /*!Setting the static rule structure for DHCP V6*/
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].rule_type = IPA_IPFLTR_RULE_UL_STATIC_LOCAL;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ihl_value = 40;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].eq_count.num_ihl_offset_meq_32 = 1;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].eq_count.num_ihl_offset_range_16 = 0 ;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].bitmap.value = 0x0200;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].protocol = 0x11;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ihl_meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ihl_meq_32[0].offset = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ihl_meq_32[0].value = 0x00000223;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].usr_data = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].rule_result = IPV6_DHCP_FLTR_RES;
  IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV6_DHCP] = 
                 ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].rule_type,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ip_type,
                  TRUE,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].rule_result,
                  ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].usr_data))!= NULL);
 IPA_TF_ASSERT(
    ipa_ipfltri_add_protocol_eq(ipa_test_filter.ul_static_hdl_arr[IPV6_DHCP],
                               ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].protocol)
			       == IPA_IPFLTR_SUCCESS);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32 \
                (ipa_test_filter.ul_static_hdl_arr[IPV6_DHCP],
                 ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ihl_meq_32[0].offset,
                 ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ihl_meq_32[0].mask,
                 ipa_test_filter.ul_static_rule_cfg[IPV6_DHCP].ihl_meq_32[0].value) 
                == IPA_IPFLTR_SUCCESS);
  /*! Commit remaining 4 rules out of 7*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(4,&ipa_test_filter.ul_static_hdl_arr[IPV6_ICMP_REQUEST])
                != IPA_ERR_IPFLTR_COMMIT_FAIL);
  /*! FRAG filter*/
  /*!Setting the static rule structure for FRAG filter*/
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].rule_type = IPA_IPFLTR_RULE_UL_STATIC_FRAG;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].ihl_value = 20;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].eq_count.num_ihl_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].eq_count.num_ihl_offset_range_16 = 0 ;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].usr_data = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].rule_result = IPV4_FRAG_FLTR_RES;
  IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV4_FRAG] =
  ipa_ipfltri_alloc_rule_handle(ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].rule_type,
                               ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].ip_type,
                               TRUE,
                               ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].rule_result,
                               ipa_test_filter.ul_static_rule_cfg[IPV4_FRAG].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_frag_eq(ipa_test_filter.ul_static_hdl_arr[IPV4_FRAG])
                   == IPA_IPFLTR_SUCCESS); 
  /*! Commit Frag UL static rules*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(1,
                         &ipa_test_filter.ul_static_hdl_arr[IPV4_FRAG])
                         != IPA_ERR_IPFLTR_COMMIT_FAIL);

  /*!Setting the static rule structure for FRAG filter*/
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].rule_type = IPA_IPFLTR_RULE_UL_STATIC_FRAG;
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].eq_count.num_ihl_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].eq_count.num_ihl_offset_range_16 = 0 ;
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].usr_data = 0;
  ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].rule_result = IPV6_FRAG_FLTR_RES;
  IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV6_FRAG] =
  ipa_ipfltri_alloc_rule_handle(ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].rule_type,
                                ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].ip_type,
                                TRUE,
                                ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].rule_result,
                                ipa_test_filter.ul_static_rule_cfg[IPV6_FRAG].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_frag_eq(ipa_test_filter.ul_static_hdl_arr[IPV6_FRAG])
                   == IPA_IPFLTR_SUCCESS); 
  /*! Commit Frag UL static rules*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(1,
                         &ipa_test_filter.ul_static_hdl_arr[IPV6_FRAG])
                         != IPA_ERR_IPFLTR_COMMIT_FAIL);

  /*! Update the number of static filters installed */
  ipa_test_filter.num_ul_static_fltr = 7;

  if (ipa_test_filter.is_clat_enabled == TRUE)
  {
    /*!Setting the static rule structure for FRAG filter*/
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ip_type = IPA_IPFLTR_IPV4;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].rule_type = IPA_IPFLTR_RULE_UL_CLAT_STATIC_LOCAL;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ihl_value = 20;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].eq_count.num_ihl_offset_meq_32 = 1;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].eq_count.num_ihl_offset_range_16 = 0 ;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].eq_count.num_offset_meq_128 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].eq_count.num_offset_meq_32 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ihl_meq_32[0].mask = ipa_ntohl(0xFFFF0000);
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ihl_meq_32[0].offset = 0x8C;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ihl_meq_32[0].value = ipa_ntohl(ipa_test_filter.v4_src_subnet_mask);
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].protocol = 0x01;
      /*! Bitmask to specify that protocol_eq, ihl_offset_meq_32 present */
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].bitmap.value = 0x0008;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].usr_data = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].rule_result = IPV4_CLAT_ANY_ICMP_RES;
    IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_ANY_ICMP] =
    ipa_ipfltri_alloc_rule_handle(ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].rule_type,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ip_type,
                                  TRUE,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].rule_result,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].usr_data))!= NULL);

    IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
      ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_ANY_ICMP],
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ihl_meq_32[0].offset,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ihl_meq_32[0].mask,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS);

    IPA_TF_ASSERT(ipa_ipfltri_add_protocol_eq(
      ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_ANY_ICMP],
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_ANY_ICMP].protocol)
        == IPA_IPFLTR_SUCCESS);
   
    /*!Setting the static rule structure for IPV4_CLAT_UDP_CHKSUM_ZERO filter*/
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ip_type = IPA_IPFLTR_IPV4;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].rule_type = IPA_IPFLTR_RULE_UL_CLAT_STATIC_LOCAL;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_value = 20;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].eq_count.num_ihl_offset_meq_32 = 1;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].eq_count.num_ihl_offset_range_16 = 0 ;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].eq_count.num_offset_meq_128 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].eq_count.num_offset_meq_32 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_meq_32[0].mask = ipa_ntohl(0xFFFF0000);
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_meq_32[0].offset = 0x8C;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_meq_32[0].value = ipa_ntohl(ipa_test_filter.v4_src_subnet_mask);
    /*! Bitmask to specify that ihl_offset_eq_32, ihl_offset_meq_32 present */
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].bitmap.value = 0x0100;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_eq_32.offset = 6;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_eq_32.value = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].usr_data = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].protocol = 0x11;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].rule_result = IPV4_CLAT_UDP_CHKSUM_ZERO_RES;
    IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_UDP_CHKSUM_ZERO] =
    ipa_ipfltri_alloc_rule_handle(ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].rule_type,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ip_type,
                                  TRUE,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].rule_result,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].usr_data))!= NULL);

    IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
      ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_UDP_CHKSUM_ZERO],
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_meq_32[0].offset,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_meq_32[0].mask,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS); 

   IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_32(
     ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_UDP_CHKSUM_ZERO],
     ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_eq_32.offset,
     ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].ihl_eq_32.value)
       == IPA_IPFLTR_SUCCESS); 

   IPA_TF_ASSERT(ipa_ipfltri_add_protocol_eq(
      ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_UDP_CHKSUM_ZERO],
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_UDP_CHKSUM_ZERO].protocol)
        == IPA_IPFLTR_SUCCESS);

    /*!Setting the static rule structure for IPV4_CLAT_TTL filter*/
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].ip_type = IPA_IPFLTR_IPV4;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].rule_type = IPA_IPFLTR_RULE_UL_STATIC_TTL;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].ihl_value = 20;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].eq_count.num_ihl_offset_meq_32 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].eq_count.num_ihl_offset_range_16 = 0 ;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].eq_count.num_offset_meq_128 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].eq_count.num_offset_meq_32 = 1;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].meq_32[0].offset = 8;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].meq_32[0].mask = 0x000000FF;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].meq_32[0].value = 0x01;
    /*! Bitmask to specify that meq_32, ihl_offset_meq_32 present */
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].bitmap.value = 0x0010;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].rule_result = IPV4_CLAT_TTL_RES;
    IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_TTL] =
    ipa_ipfltri_alloc_rule_handle(ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].rule_type,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].ip_type,
                                  TRUE,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].rule_result,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].usr_data))!= NULL);

    IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(
    ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_TTL],
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].meq_32[0].offset,
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].meq_32[0].mask,
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_TTL].meq_32[0].value) 
                == IPA_IPFLTR_SUCCESS); 

    /*!Setting the static rule structure for DNS_PROXY filter*/
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ip_type = IPA_IPFLTR_IPV4;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].rule_type = IPA_IPFLTR_RULE_UL_CLAT_STATIC_LOCAL;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_value = 20;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].eq_count.num_ihl_offset_meq_32 = 1;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].eq_count.num_ihl_offset_range_16 = 1 ;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].eq_count.num_offset_meq_128 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].eq_count.num_offset_meq_32 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_meq_32[0].mask = ipa_ntohl(0xFFFF0000);
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_meq_32[0].offset = 0x8C;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_meq_32[0].value = ipa_ntohl(ipa_test_filter.v4_src_subnet_mask);
    /*! Bitmask to specify that ihl_offset_eq_32, ihl_offset_meq_32 present */
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].bitmap.value = 0x0100;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_eq_32.offset = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_eq_32.value = 0x443;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].rule_result = IPV4_CLAT_DNS_PROXY_RES;
    IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_DNS_PROXY] =
    ipa_ipfltri_alloc_rule_handle(ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].rule_type,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ip_type,
                                  TRUE,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].rule_result,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].usr_data))!= NULL);

    IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
      ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_DNS_PROXY],
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_meq_32[0].offset,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_meq_32[0].mask,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS); 

    IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_32(
      ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_DNS_PROXY],
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_eq_32.offset,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_DNS_PROXY].ihl_eq_32.value)
        == IPA_IPFLTR_SUCCESS); 

    /*!Setting the static rule structure for FRAG filter*/
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ip_type = IPA_IPFLTR_IPV4;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].rule_type = IPA_IPFLTR_RULE_UL_CLAT_STATIC_FRAG;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ihl_value = 20;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].eq_count.num_ihl_offset_meq_32 = 1;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].eq_count.num_ihl_offset_range_16 = 0 ;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].eq_count.num_offset_meq_128 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].eq_count.num_offset_meq_32 = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ihl_meq_32[0].mask = ipa_ntohl(0xFFFF0000);
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ihl_meq_32[0].offset = 0x8C;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ihl_meq_32[0].value = ipa_ntohl(ipa_test_filter.v4_src_subnet_mask);
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].usr_data = 0;
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].rule_result = IPV4_CLAT_FRAG_V4_RES;
    /*! Bitmask to specify that ihl_offset_meq_32 present */
    ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].bitmap.value = 0x0000;
    IPA_TF_ASSERT((ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_FRAG_V4] =
    ipa_ipfltri_alloc_rule_handle(ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].rule_type,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ip_type,
                                  TRUE,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].rule_result,
                                  ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].usr_data))!= NULL);

    IPA_TF_ASSERT(ipa_ipfltri_add_frag_eq(ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_FRAG_V4])
                     == IPA_IPFLTR_SUCCESS); 

    IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
      ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_FRAG_V4],
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ihl_meq_32[0].offset,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ihl_meq_32[0].mask,
      ipa_test_filter.ul_static_rule_cfg[IPV4_CLAT_FRAG_V4].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS); 

    /*! Commit CLAT UL static rules*/
    IPA_TF_ASSERT(ipa_ipfltri_commit_rules(5,
                           &ipa_test_filter.ul_static_hdl_arr[IPV4_CLAT_ANY_ICMP])
                           != IPA_ERR_IPFLTR_COMMIT_FAIL);

    /*! Update the number of static filters installed with the additional CLAT static filters*/
    ipa_test_filter.num_ul_static_fltr += 5;

  }
  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==========================================================================

 FUNCTION: ipa_test_filter_ul_qos_filter_init

==========================================================================*/
/*!
  @brief
  Install all UL QOS filters.
 
  @return
  IPA_TEST_RET_CODE_SUCCESS if all the filter rules are successfully installed.
*/
/*=========================================================================*/
ipa_test_ret_code_e ipa_test_filter_ul_qos_filter_init
(
  ipa_test_filter_init_cfg_s   *init_cfg_ptr
)
{
  uint8 i;
  uint32 usr_data;

  usr_data = (uint32)ipa_test_engine_get_sio_stream_id(init_cfg_ptr->uid_array[0]);

  /*! Apply all static configurations for DL socket filters*/
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].rule_type = IPA_IPFLTR_RULE_UL_QOS; 
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].tos = 0x0A;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].rule_result = 0x01;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].bitmap.value = 0x0001;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].ihl_value = 20;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE1]
                    = ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].rule_type,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].ip_type,
                  FALSE,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].rule_result,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].usr_data))!= NULL);
 
  IPA_TF_ASSERT(ipa_ipfltri_add_tos_eq
                (ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE1],
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE1].tos) 
                == IPA_IPFLTR_SUCCESS);
  
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].rule_result = 0x02;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].bitmap.value = 0x0008;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].protocol = 0x02;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].ihl_value = 20;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE2]
                 = ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].rule_type,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].ip_type,
                  FALSE,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].rule_result,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_protocol_eq
                (ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE2],
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE2].protocol)
                == IPA_IPFLTR_SUCCESS );
  
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].rule_result = 0x03;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].bitmap.value = 0x0010;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].ihl_value = 20;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[0].offset = 22 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[0].value = 0x0A000000; 
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[1].offset = 30 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[1].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[1].value = 0x0B000000; 
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].eq_count.num_offset_meq_32 = 2;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].eq_count.num_ihl_offset_meq_32 = 0;
 
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE3] =
                  ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].rule_type,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].ip_type,
                  FALSE,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].rule_result,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32
                (ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE3],
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[0].offset,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[0].mask,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[0].value) 
                == IPA_IPFLTR_SUCCESS); 

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32
                (ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE3],
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[1].offset,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[1].mask,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE3].meq_32[1].value) 
                == IPA_IPFLTR_SUCCESS); 
  
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].rule_result = 0x04;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].bitmap.value = 0x0020;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[0].offset = 37 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[1].offset = 60 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].eq_count.num_offset_meq_128 = 2;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].eq_count.num_ihl_offset_meq_32 = 0;

  for (i = 0; i < 16  ; i++)
  { 
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[0].mask[i] = 0xFF;
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[0].value[i] = i;
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[1].mask[i] = 0xFF;
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[1].value[i] = i*3;
  }

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE4]
                 = ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].rule_type,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].ip_type,
                  FALSE,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].rule_result,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_128
                (ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE4],
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[0].offset,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[0].mask,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[0].value)
                == IPA_IPFLTR_SUCCESS);
   
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_128
                (ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE4],
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[1].offset,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[1].mask,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE4].meq_128[1].value)
                == IPA_IPFLTR_SUCCESS); 

  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].rule_result = 0x05;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].bitmap.value = 0x0040;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_value = 20;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[0].offset = 60 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[0].range_low = 0x0000;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[0].range_high = 0x4567;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[1].offset = 65 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[1].range_low = 0x0000;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[1].range_high = 0x6789;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].eq_count.num_ihl_offset_range_16 = 2;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].eq_count.num_ihl_offset_meq_32 = 0;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE5]
                 = ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].rule_type,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ip_type,
                  FALSE,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].rule_result,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16(
    ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE5],
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[0].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[0].range_low,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[0].range_high)
                == IPA_IPFLTR_SUCCESS); 

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16(
    ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE5],
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[1].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[1].range_low,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE5].ihl_range_eq_16[1].range_high)
                == IPA_IPFLTR_SUCCESS); 

  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].rule_result = 0x06;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].bitmap.value = 0x0080;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].ihl_value = 20;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].ihl_offset_eq_16.offset = 70 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].ihl_offset_eq_16.value = 0xFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].eq_count.num_ihl_offset_meq_32 = 0;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE6]
                 = ipa_ipfltri_alloc_rule_handle
                 (ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].rule_type,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].ip_type,
                  FALSE,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].rule_result,
                  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_16
                (ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE6],
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].ihl_offset_eq_16.offset,
                 ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE6].ihl_offset_eq_16.value) 
                == IPA_IPFLTR_SUCCESS);

  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].rule_result = 0x07;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].bitmap.value = 0x0100;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].ihl_value = 20;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].ihl_eq_32.offset = 75 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].ihl_eq_32.value = 0xAAAA;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].eq_count.num_ihl_offset_range_16 = 1;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].eq_count.num_ihl_offset_meq_32 = 0;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE7]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].rule_type,
                   ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].ip_type,
                   FALSE,
                   ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].rule_result,
                   ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_32(
    ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE7],
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].ihl_eq_32.offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE7].ihl_eq_32.value)
                == IPA_IPFLTR_SUCCESS); 

  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].rule_result = 0x08;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].bitmap.value = 0x0200;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_value = 20;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[0].offset = 80 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[0].value = 0x0A;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[1].offset = 85 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[1].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[1].value = 0x0A;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].eq_count.num_ihl_offset_meq_32 = 2;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE8]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].rule_type,
                      ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ip_type,
                      FALSE,
                      ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].rule_result,
                      ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
    ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE8],
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[0].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[0].mask,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS); 

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
    ipa_test_filter.ul_qos_hdl_arr[IPV4_QOS_RULE8],
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[1].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[1].mask,
    ipa_test_filter.ul_qos_rule_cfg[IPV4_QOS_RULE8].ihl_meq_32[1].value)
                == IPA_IPFLTR_SUCCESS); 

  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].rule_result = 0x0B;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].bitmap.value = 0x0010;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].ihl_value = 40;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].meq_32[0].offset = 22 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].meq_32[0].value = 0x0C000000; 
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].eq_count.num_offset_meq_32 = 1;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].eq_count.num_ihl_offset_meq_32 = 0;
 
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE9] =
                  ipa_ipfltri_alloc_rule_handle(
                    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].rule_type,
                    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].ip_type,
                    FALSE,
                    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].rule_result,
                    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE9],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].meq_32[0].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].meq_32[0].mask,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE9].meq_32[0].value) 
                == IPA_IPFLTR_SUCCESS); 
  
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].rule_result = 0x0C;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].bitmap.value = 0x0020;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].ihl_value = 40;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].meq_128[0].offset = 30  ;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].eq_count.num_offset_meq_128 = 1;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].eq_count.num_ihl_offset_meq_32 = 0;

  for (i = 0; i < 16  ; i++)
  { 
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].meq_128[0].mask[i] = 0xFF;
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].meq_128[0].value[i] = i*5;
  }

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE10]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].rule_type,
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].ip_type,
                      FALSE,
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].rule_result,
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_128(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE10],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].meq_128[0].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].meq_128[0].mask,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE10].meq_128[0].value)
                == IPA_IPFLTR_SUCCESS); 

  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].rule_result = 0x0F;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].bitmap.value = 0x0002;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].traffic_class = 0x0B;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE11]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].rule_type,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].ip_type,
                   FALSE,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].rule_result,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_tc_eq(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE11],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE11].traffic_class)
                == IPA_IPFLTR_SUCCESS );

  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].rule_result = 0x10;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].bitmap.value = 0x0004;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].flow_label = 0x50;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE12]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].rule_type,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].ip_type,
                   FALSE,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].rule_result,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_flow_eq(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE12],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE12].flow_label)
                == IPA_IPFLTR_SUCCESS );

  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].rule_result = 0x09;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].bitmap.value = 0x0100;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].ihl_value = 40;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].ihl_eq_32.offset = 30;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].ihl_eq_32.value = 0xAAAA;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].eq_count.num_ihl_offset_meq_32 = 0;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE13]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].rule_type,
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].ip_type,
                      FALSE,
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].rule_result,
                      ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_32(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE13],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].ihl_eq_32.offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE13].ihl_eq_32.value)
                == IPA_IPFLTR_SUCCESS); 
 
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].rule_result = 0x0A;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].bitmap.value = 0x0200;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ihl_value = 40;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ihl_meq_32[0].offset = 40;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ihl_meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ihl_meq_32[0].value = 0x0A;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].eq_count.num_ihl_offset_meq_32 = 1;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE14]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].rule_type,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ip_type,
                   FALSE,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].rule_result,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE14],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ihl_meq_32[0].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ihl_meq_32[0].mask,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE14].ihl_meq_32[0].value)
                == IPA_IPFLTR_SUCCESS);

  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].rule_result = 0x0D;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].bitmap.value = 0x0040;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ihl_value = 40;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ihl_range_eq_16[0].offset = 15;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ihl_range_eq_16[0].range_low = 0x0000;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ihl_range_eq_16[0].range_high = 0x1234;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].eq_count.num_ihl_offset_range_16 = 1;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].eq_count.num_ihl_offset_meq_32 = 0;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE15]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].rule_type,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ip_type,
                   FALSE,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].rule_result,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE15],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ihl_range_eq_16[0].offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ihl_range_eq_16[0].range_low,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE15].ihl_range_eq_16[0].range_high)
                    == IPA_IPFLTR_SUCCESS); 

  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].rule_type = IPA_IPFLTR_RULE_UL_QOS;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].rule_result = 0x0E;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].usr_data = usr_data;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].bitmap.value = 0x0080;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].ihl_value = 40;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].ihl_offset_eq_16.offset = 20 ;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].ihl_offset_eq_16.value = 0xFFFF;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].eq_count.num_ihl_offset_meq_32 = 0;

  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE16]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].rule_type,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].ip_type,
                   FALSE,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].rule_result,
                   ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].usr_data))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_16(
    ipa_test_filter.ul_qos_hdl_arr[IPV6_QOS_RULE16],
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].ihl_offset_eq_16.offset,
    ipa_test_filter.ul_qos_rule_cfg[IPV6_QOS_RULE16].ihl_offset_eq_16.value) 
                == IPA_IPFLTR_SUCCESS);

 /*! Commit all the UL QOS rules*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(ipa_test_filter.num_ul_qos_fltr,
                                         ipa_test_filter.ul_qos_hdl_arr)
                != IPA_ERR_IPFLTR_COMMIT_FAIL);

  return IPA_TEST_RET_CODE_SUCCESS;
}

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
)
{
  uint8  i;
  uint32 usr_data;

  usr_data = (uint32)ipa_test_engine_get_sio_stream_id(init_cfg_ptr->uid_array[0]);
  /*! Apply all static configurations for DL socket filters*/
  /*! DL Socket Rule 0*/
  ipa_test_filter.dl_sckt_rule_cfg[0].rule_type = IPA_IPFLTR_RULE_DL_SOCKET; 
  ipa_test_filter.dl_sckt_rule_cfg[0].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[0].tos = 0xFF;
  ipa_test_filter.dl_sckt_rule_cfg[0].rule_result = 0x01;
  ipa_test_filter.dl_sckt_rule_cfg[0].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[0].bitmap.value = 0x0001;
  ipa_test_filter.dl_sckt_rule_cfg[0].ihl_value = 20;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[0]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.dl_sckt_rule_cfg[0].rule_type,
                   ipa_test_filter.dl_sckt_rule_cfg[0].ip_type,
                   FALSE,
                   ipa_test_filter.dl_sckt_rule_cfg[0].rule_result,
                   ipa_test_filter.dl_sckt_rule_cfg[0].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_tos_eq(
    ipa_test_filter.dl_socket_hdl_arr[0],
    ipa_test_filter.dl_sckt_rule_cfg[0].tos)
                == IPA_IPFLTR_SUCCESS);
  /*! DL Socket Rule 1 */
  ipa_test_filter.dl_sckt_rule_cfg[1].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[1].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[1].rule_result = 0x02;
  ipa_test_filter.dl_sckt_rule_cfg[1].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[1].bitmap.value = 0x0008;
  ipa_test_filter.dl_sckt_rule_cfg[1].protocol = 0xFF;
  ipa_test_filter.dl_sckt_rule_cfg[1].ihl_value = 20;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[1]
                 = ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.dl_sckt_rule_cfg[1].rule_type,
                   ipa_test_filter.dl_sckt_rule_cfg[1].ip_type,
                   FALSE,
                   ipa_test_filter.dl_sckt_rule_cfg[1].rule_result,
                   ipa_test_filter.dl_sckt_rule_cfg[1].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_protocol_eq(
    ipa_test_filter.dl_socket_hdl_arr[1],
    ipa_test_filter.dl_sckt_rule_cfg[1].protocol)
                == IPA_IPFLTR_SUCCESS );
  /*! DL Socket Rule 2*/ 
  ipa_test_filter.dl_sckt_rule_cfg[2].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[2].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[2].rule_result = 0x03;
  ipa_test_filter.dl_sckt_rule_cfg[2].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[2].bitmap.value = 0x0010;
  ipa_test_filter.dl_sckt_rule_cfg[2].ihl_value = 20;
  ipa_test_filter.dl_sckt_rule_cfg[2].meq_32[0].offset = 22 ;
  ipa_test_filter.dl_sckt_rule_cfg[2].meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[2].meq_32[0].value = 0x0A000000; 
  ipa_test_filter.dl_sckt_rule_cfg[2].eq_count.num_offset_meq_32 = 1;
  ipa_test_filter.dl_sckt_rule_cfg[2].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[2].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[2].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[2] =
                  ipa_ipfltri_alloc_rule_handle(
                    ipa_test_filter.dl_sckt_rule_cfg[2].rule_type,
                    ipa_test_filter.dl_sckt_rule_cfg[2].ip_type,
                    FALSE,
                    ipa_test_filter.dl_sckt_rule_cfg[2].rule_result,
                    ipa_test_filter.dl_sckt_rule_cfg[2].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(
    ipa_test_filter.dl_socket_hdl_arr[2],
    ipa_test_filter.dl_sckt_rule_cfg[2].meq_32[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[2].meq_32[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[2].meq_32[0].value) 
                == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 3 */
  ipa_test_filter.dl_sckt_rule_cfg[3].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[3].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[3].rule_result = 0x04;
  ipa_test_filter.dl_sckt_rule_cfg[3].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[3].bitmap.value = 0x0020;
  ipa_test_filter.dl_sckt_rule_cfg[3].meq_128[0].offset = 30  ;
  ipa_test_filter.dl_sckt_rule_cfg[3].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[3].eq_count.num_offset_meq_128 = 1;
  ipa_test_filter.dl_sckt_rule_cfg[3].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[3].eq_count.num_ihl_offset_meq_32 = 0;
  for (i = 0; i < 16  ; i++)
  { 
    ipa_test_filter.dl_sckt_rule_cfg[3].meq_128[0].mask[i] = 0xFF;
    ipa_test_filter.dl_sckt_rule_cfg[3].meq_128[0].value[i] = i;
  }
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[3]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[3].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[3].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[3].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[3].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_128(
    ipa_test_filter.dl_socket_hdl_arr[3],
    ipa_test_filter.dl_sckt_rule_cfg[3].meq_128[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[3].meq_128[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[3].meq_128[0].value)
                == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 4 */
  ipa_test_filter.dl_sckt_rule_cfg[4].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[4].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[4].rule_result = 0x05;
  ipa_test_filter.dl_sckt_rule_cfg[4].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[4].bitmap.value = 0x0040;
  ipa_test_filter.dl_sckt_rule_cfg[4].ihl_value = 20;
  ipa_test_filter.dl_sckt_rule_cfg[4].ihl_range_eq_16[0].offset = 30 ;
  ipa_test_filter.dl_sckt_rule_cfg[4].ihl_range_eq_16[0].range_low = 0x0000;
  ipa_test_filter.dl_sckt_rule_cfg[4].ihl_range_eq_16[0].range_high = 0xFFFE;
  ipa_test_filter.dl_sckt_rule_cfg[4].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[4].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[4].eq_count.num_ihl_offset_range_16 = 1;
  ipa_test_filter.dl_sckt_rule_cfg[4].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[4]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[4].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[4].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[4].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[4].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16(
    ipa_test_filter.dl_socket_hdl_arr[4],
    ipa_test_filter.dl_sckt_rule_cfg[4].ihl_range_eq_16[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[4].ihl_range_eq_16[0].range_low,
    ipa_test_filter.dl_sckt_rule_cfg[4].ihl_range_eq_16[0].range_high)
                    == IPA_IPFLTR_SUCCESS); 
 
  /*! Commit 5 DL socket rules*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(5, ipa_test_filter.dl_socket_hdl_arr)
                 != IPA_ERR_IPFLTR_COMMIT_FAIL);
  /*! DL Socket Rule 5*/
  ipa_test_filter.dl_sckt_rule_cfg[5].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[5].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[5].rule_result = 0x06;
  ipa_test_filter.dl_sckt_rule_cfg[5].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[5].bitmap.value = 0x0080;
  ipa_test_filter.dl_sckt_rule_cfg[5].ihl_value = 20;
  ipa_test_filter.dl_sckt_rule_cfg[5].ihl_offset_eq_16.offset = 35 ;
  ipa_test_filter.dl_sckt_rule_cfg[5].ihl_offset_eq_16.value = 0xFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[5].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[5].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[5].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[5].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[5]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[5].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[5].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[5].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[5].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_16(
    ipa_test_filter.dl_socket_hdl_arr[5],
    ipa_test_filter.dl_sckt_rule_cfg[5].ihl_offset_eq_16.offset,
    ipa_test_filter.dl_sckt_rule_cfg[5].ihl_offset_eq_16.value) 
                == IPA_IPFLTR_SUCCESS);
  /*! DL Socket Rule 6 */
  ipa_test_filter.dl_sckt_rule_cfg[6].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[6].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[6].rule_result = 0x07;
  ipa_test_filter.dl_sckt_rule_cfg[6].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[6].bitmap.value = 0x0100;
  ipa_test_filter.dl_sckt_rule_cfg[6].ihl_value = 20;
  ipa_test_filter.dl_sckt_rule_cfg[6].ihl_eq_32.offset = 40 ;
  ipa_test_filter.dl_sckt_rule_cfg[6].ihl_eq_32.value = 0xAAAA;
  ipa_test_filter.dl_sckt_rule_cfg[6].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.dl_sckt_rule_cfg[6].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[6].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[6].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[6]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[6].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[6].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[6].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[6].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_32(
    ipa_test_filter.dl_socket_hdl_arr[6],
    ipa_test_filter.dl_sckt_rule_cfg[6].ihl_eq_32.offset,
    ipa_test_filter.dl_sckt_rule_cfg[6].ihl_eq_32.value)
                == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 7 */
  ipa_test_filter.dl_sckt_rule_cfg[7].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[7].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[7].rule_result = 0x08;
  ipa_test_filter.dl_sckt_rule_cfg[7].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[7].bitmap.value = 0x0200;
  ipa_test_filter.dl_sckt_rule_cfg[7].ihl_value = 20;
  ipa_test_filter.dl_sckt_rule_cfg[7].ihl_meq_32[0].offset = 45 ;
  ipa_test_filter.dl_sckt_rule_cfg[7].ihl_meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[7].ihl_meq_32[0].value = 0x0A;
  ipa_test_filter.dl_sckt_rule_cfg[7].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.dl_sckt_rule_cfg[7].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[7].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[7].eq_count.num_ihl_offset_meq_32 = 1;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[7]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[7].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[7].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[7].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[7].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
     ipa_test_filter.dl_socket_hdl_arr[7],
    ipa_test_filter.dl_sckt_rule_cfg[7].ihl_meq_32[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[7].ihl_meq_32[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[7].ihl_meq_32[0].value)
                  == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 8 */
  ipa_test_filter.dl_sckt_rule_cfg[8].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[8].ip_type = IPA_IPFLTR_IPV4;
  ipa_test_filter.dl_sckt_rule_cfg[8].rule_result = 0x14;
  ipa_test_filter.dl_sckt_rule_cfg[8].ihl_value = 20;
  ipa_test_filter.dl_sckt_rule_cfg[8].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[8].bitmap.value = 0x0020;
  ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[0].offset = 70;
  ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[1].offset = 90;
  ipa_test_filter.dl_sckt_rule_cfg[8].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[8].eq_count.num_offset_meq_128 = 2;
  ipa_test_filter.dl_sckt_rule_cfg[8].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[8].eq_count.num_ihl_offset_meq_32 = 0;
  for (i = 0; i < 16  ; i++)
  { 
    ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[0].mask[i] = 0xFF;
    ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[0].value[i] = i;
    ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[1].mask[i] = 0xFF;
    ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[1].value[i] = i;
  }
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[8]
                     = ipa_ipfltri_alloc_rule_handle(
                       ipa_test_filter.dl_sckt_rule_cfg[8].rule_type,
                       ipa_test_filter.dl_sckt_rule_cfg[8].ip_type,
                       FALSE,
                       ipa_test_filter.dl_sckt_rule_cfg[8].rule_result,
                       ipa_test_filter.dl_sckt_rule_cfg[8].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_128(
     ipa_test_filter.dl_socket_hdl_arr[8],
     ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[0].offset,
     ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[0].mask,
     ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[0].value)
                  == IPA_IPFLTR_SUCCESS); 
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_128(
     ipa_test_filter.dl_socket_hdl_arr[8],
     ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[1].offset,
     ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[1].mask,
     ipa_test_filter.dl_sckt_rule_cfg[8].meq_128[1].value)
                  == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 9 */
  ipa_test_filter.dl_sckt_rule_cfg[9].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[9].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[9].rule_result = 0x0A;
  ipa_test_filter.dl_sckt_rule_cfg[9].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[9].bitmap.value = 0x0020;
  ipa_test_filter.dl_sckt_rule_cfg[9].meq_128[0].offset = 30;
  ipa_test_filter.dl_sckt_rule_cfg[9].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[9].eq_count.num_offset_meq_128 = 1;
  ipa_test_filter.dl_sckt_rule_cfg[9].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[9].eq_count.num_ihl_offset_meq_32 = 0;
  for (i = 0; i < 16  ; i++)
  { 
    ipa_test_filter.dl_sckt_rule_cfg[9].meq_128[0].mask[i] = 0xFF;
    ipa_test_filter.dl_sckt_rule_cfg[9].meq_128[0].value[i] = i;
  }
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[9]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[9].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[9].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[9].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[9].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_128(
    ipa_test_filter.dl_socket_hdl_arr[9],
    ipa_test_filter.dl_sckt_rule_cfg[9].meq_128[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[9].meq_128[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[9].meq_128[0].value)
                 == IPA_IPFLTR_SUCCESS); 

  /*! Commit 5 DL socket rules*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(5, &ipa_test_filter.dl_socket_hdl_arr[5])
                 != IPA_ERR_IPFLTR_COMMIT_FAIL);
  /*! DL Socket Rule 10*/
  ipa_test_filter.dl_sckt_rule_cfg[10].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[10].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[10].rule_result = 0x0F;
  ipa_test_filter.dl_sckt_rule_cfg[10].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[10].bitmap.value = 0x0002;
  ipa_test_filter.dl_sckt_rule_cfg[10].traffic_class = 0x0A;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[10]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[10].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[10].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[10].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[10].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_tc_eq(
    ipa_test_filter.dl_socket_hdl_arr[10],
    ipa_test_filter.dl_sckt_rule_cfg[10].traffic_class)
                == IPA_IPFLTR_SUCCESS );
  /*! DL Socket Rule 11*/
  ipa_test_filter.dl_sckt_rule_cfg[11].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[11].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[11].rule_result = 0x10;
  ipa_test_filter.dl_sckt_rule_cfg[11].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[11].bitmap.value = 0x0004;
  ipa_test_filter.dl_sckt_rule_cfg[11].flow_label = 0x50;
  ipa_test_filter.dl_sckt_rule_cfg[11].ihl_value = ipa_test_filter.ihl_value;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[11]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[11].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[11].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[11].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[11].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_flow_eq(
    ipa_test_filter.dl_socket_hdl_arr[11],
    ipa_test_filter.dl_sckt_rule_cfg[11].flow_label)
                 == IPA_IPFLTR_SUCCESS );
  /*! DL Socket Rule 12*/
  ipa_test_filter.dl_sckt_rule_cfg[12].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[12].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[12].rule_result = 0x13;
  ipa_test_filter.dl_sckt_rule_cfg[12].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[12].bitmap.value = 0x0010;
  ipa_test_filter.dl_sckt_rule_cfg[12].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[0].offset = 90 ;
  ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[0].value = 0xAAAAAAAA; 
  ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[1].offset = 95 ;
  ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[1].mask = 0xFFFFFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[1].value = 0xBBBBBBBB; 
  ipa_test_filter.dl_sckt_rule_cfg[12].eq_count.num_offset_meq_32 = 2;
  ipa_test_filter.dl_sckt_rule_cfg[12].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[12].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[12].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[12] =
                 ipa_ipfltri_alloc_rule_handle(
                   ipa_test_filter.dl_sckt_rule_cfg[12].rule_type,
                   ipa_test_filter.dl_sckt_rule_cfg[12].ip_type,
                   FALSE,
                   ipa_test_filter.dl_sckt_rule_cfg[12].rule_result,
                   ipa_test_filter.dl_sckt_rule_cfg[12].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(
    ipa_test_filter.dl_socket_hdl_arr[12],
    ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[0].value) 
                == IPA_IPFLTR_SUCCESS); 
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(
    ipa_test_filter.dl_socket_hdl_arr[12],
    ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[1].offset,
    ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[1].mask,
    ipa_test_filter.dl_sckt_rule_cfg[12].meq_32[1].value)
                == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 13*/

  /*! Hop Limit = 1 socket filter*/
  ipa_test_filter.dl_sckt_rule_cfg[13].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[13].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[13].rule_result = 0x09;
  ipa_test_filter.dl_sckt_rule_cfg[13].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[13].bitmap.value = 0x0010;
  ipa_test_filter.dl_sckt_rule_cfg[13].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[13].meq_32[0].offset = 7;
  ipa_test_filter.dl_sckt_rule_cfg[13].meq_32[0].mask = 0x000000FF;
  ipa_test_filter.dl_sckt_rule_cfg[13].meq_32[0].value = 0x00000001; 
  ipa_test_filter.dl_sckt_rule_cfg[13].eq_count.num_offset_meq_32 = 1;
  ipa_test_filter.dl_sckt_rule_cfg[13].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[13].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[13].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[13] =
                  ipa_ipfltri_alloc_rule_handle(
                    ipa_test_filter.dl_sckt_rule_cfg[13].rule_type,
                    ipa_test_filter.dl_sckt_rule_cfg[13].ip_type,
                    FALSE,
                    ipa_test_filter.dl_sckt_rule_cfg[13].rule_result,
                    ipa_test_filter.dl_sckt_rule_cfg[13].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(
    ipa_test_filter.dl_socket_hdl_arr[13],
    ipa_test_filter.dl_sckt_rule_cfg[13].meq_32[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[13].meq_32[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[13].meq_32[0].value) == IPA_IPFLTR_SUCCESS); 

  /*! DL Socket Rule 14 */
  ipa_test_filter.dl_sckt_rule_cfg[14].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[14].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[14].rule_result = 0x0E;
  ipa_test_filter.dl_sckt_rule_cfg[14].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[14].bitmap.value = 0x0200;
  ipa_test_filter.dl_sckt_rule_cfg[14].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[14].ihl_meq_32[0].offset = 25 ;
  ipa_test_filter.dl_sckt_rule_cfg[14].ihl_meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[14].ihl_meq_32[0].value = 0x0A;
  ipa_test_filter.dl_sckt_rule_cfg[14].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.dl_sckt_rule_cfg[14].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[14].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[14].eq_count.num_ihl_offset_meq_32 = 1;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[14]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[14].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[14].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[14].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[14].usr_data))!= NULL);
   IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
    ipa_test_filter.dl_socket_hdl_arr[14],
    ipa_test_filter.dl_sckt_rule_cfg[14].ihl_meq_32[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[14].ihl_meq_32[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[14].ihl_meq_32[0].value)
                      == IPA_IPFLTR_SUCCESS); 
  /*! Commit 5 DL socket rules*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(5, &ipa_test_filter.dl_socket_hdl_arr[10])
                 != IPA_ERR_IPFLTR_COMMIT_FAIL);
  /*! DL Socket Rule 15 */
  /*! DNS Proxy socket filter*/
  ipa_test_filter.dl_sckt_rule_cfg[15].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[15].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[15].rule_result = 0x0D;
  ipa_test_filter.dl_sckt_rule_cfg[15].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[15].bitmap.value = 0x0100;
  ipa_test_filter.dl_sckt_rule_cfg[15].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[15].ihl_eq_32.offset = 20 ;
  ipa_test_filter.dl_sckt_rule_cfg[15].ihl_eq_32.value = 0x443;
  ipa_test_filter.dl_sckt_rule_cfg[15].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.dl_sckt_rule_cfg[15].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[15].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[15].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[15]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[15].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[15].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[15].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[15].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_32(
    ipa_test_filter.dl_socket_hdl_arr[15],
    ipa_test_filter.dl_sckt_rule_cfg[15].ihl_eq_32.offset,
    ipa_test_filter.dl_sckt_rule_cfg[15].ihl_eq_32.value)
                == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 16 */
  ipa_test_filter.dl_sckt_rule_cfg[16].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[16].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[16].rule_result = 0x11;
  ipa_test_filter.dl_sckt_rule_cfg[16].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[16].bitmap.value = 0x0200;
  ipa_test_filter.dl_sckt_rule_cfg[16].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[0].offset = 30 ;
  ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[0].mask = 0xFFFFFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[0].value = 0x0B;
  ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[1].offset = 35 ;
  ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[1].mask = 0xFFFFFFFF;
  ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[1].value = 0x0C;
  ipa_test_filter.dl_sckt_rule_cfg[16].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.dl_sckt_rule_cfg[16].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[16].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[16].eq_count.num_ihl_offset_meq_32 = 2;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[16]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[16].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[16].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[16].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[16].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
    ipa_test_filter.dl_socket_hdl_arr[16],
    ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[0].mask,
    ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[0].value)
                      == IPA_IPFLTR_SUCCESS); 
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_meq_32(
    ipa_test_filter.dl_socket_hdl_arr[16],
    ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[1].offset,
    ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[1].mask,
    ipa_test_filter.dl_sckt_rule_cfg[16].ihl_meq_32[1].value)
                      == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 17 */
  ipa_test_filter.dl_sckt_rule_cfg[17].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[17].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[17].rule_result = 0x12;
  ipa_test_filter.dl_sckt_rule_cfg[17].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[17].bitmap.value = 0x0040;
  ipa_test_filter.dl_sckt_rule_cfg[17].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[0].offset = 40 ;
  ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[0].range_low = 0x0000;
  ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[0].range_high = 0xFFFE;
  ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[1].offset = 45;
  ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[1].range_low = 0x0000;
  ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[1].range_high = 0xFFFE;
  ipa_test_filter.dl_sckt_rule_cfg[17].eq_count.num_offset_meq_32 = 0; 
  ipa_test_filter.dl_sckt_rule_cfg[17].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[17].eq_count.num_ihl_offset_range_16 = 2;
  ipa_test_filter.dl_sckt_rule_cfg[17].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[17]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[17].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[17].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[17].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[17].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16(
    ipa_test_filter.dl_socket_hdl_arr[17],
    ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[0].range_low,
    ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[0].range_high)
                    == IPA_IPFLTR_SUCCESS); 
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16(
    ipa_test_filter.dl_socket_hdl_arr[17],
    ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[1].offset,
    ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[1].range_low,
    ipa_test_filter.dl_sckt_rule_cfg[17].ihl_range_eq_16[1].range_high)
                    == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 18*/
  ipa_test_filter.dl_sckt_rule_cfg[18].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[18].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[18].rule_result = 0x0B;
  ipa_test_filter.dl_sckt_rule_cfg[18].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[18].bitmap.value = 0x0040;
  ipa_test_filter.dl_sckt_rule_cfg[18].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[18].ihl_range_eq_16[0].offset = 10 ;
  ipa_test_filter.dl_sckt_rule_cfg[18].ihl_range_eq_16[0].range_low = 0x0000;
  ipa_test_filter.dl_sckt_rule_cfg[18].ihl_range_eq_16[0].range_high = 0xFFFE;
  ipa_test_filter.dl_sckt_rule_cfg[18].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[18].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[18].eq_count.num_ihl_offset_range_16 = 1;
  ipa_test_filter.dl_sckt_rule_cfg[18].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[18]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[18].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[18].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[18].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[18].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_range_eq_16(
    ipa_test_filter.dl_socket_hdl_arr[18],
    ipa_test_filter.dl_sckt_rule_cfg[18].ihl_range_eq_16[0].offset,
    ipa_test_filter.dl_sckt_rule_cfg[18].ihl_range_eq_16[0].range_low,
    ipa_test_filter.dl_sckt_rule_cfg[18].ihl_range_eq_16[0].range_high)
                    == IPA_IPFLTR_SUCCESS); 
  /*! DL Socket Rule 19 */
  ipa_test_filter.dl_sckt_rule_cfg[19].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[19].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[19].rule_result = 0x0C;
  ipa_test_filter.dl_sckt_rule_cfg[19].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[19].bitmap.value = 0x0080;
  ipa_test_filter.dl_sckt_rule_cfg[19].ihl_value = 40;
  ipa_test_filter.dl_sckt_rule_cfg[19].ihl_offset_eq_16.offset = 14 ;
  ipa_test_filter.dl_sckt_rule_cfg[19].ihl_offset_eq_16.value = 0x1234;
  ipa_test_filter.dl_sckt_rule_cfg[19].eq_count.num_offset_meq_32 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[19].eq_count.num_offset_meq_128 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[19].eq_count.num_ihl_offset_range_16 = 0;
  ipa_test_filter.dl_sckt_rule_cfg[19].eq_count.num_ihl_offset_meq_32 = 0;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[19]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[19].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[19].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[19].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[19].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_ihl_offset_eq_16(
    ipa_test_filter.dl_socket_hdl_arr[19],
    ipa_test_filter.dl_sckt_rule_cfg[19].ihl_offset_eq_16.offset,
    ipa_test_filter.dl_sckt_rule_cfg[19].ihl_offset_eq_16.value) 
                     == IPA_IPFLTR_SUCCESS);

  #if 0
  /*!Checking for duplicate rule installation*/
  ipa_test_filter.dl_sckt_rule_cfg[20].rule_type = IPA_IPFLTR_RULE_DL_SOCKET;
  ipa_test_filter.dl_sckt_rule_cfg[20].ip_type = IPA_IPFLTR_IPV6;
  ipa_test_filter.dl_sckt_rule_cfg[20].rule_result = 0x15;
  ipa_test_filter.dl_sckt_rule_cfg[20].usr_data = usr_data;
  ipa_test_filter.dl_sckt_rule_cfg[20].bitmap.value = 0x0002;
  ipa_test_filter.dl_sckt_rule_cfg[20].traffic_class = 0x0A;
  /*! Rule memory to be allocated to add the filter rule*/
  IPA_TF_ASSERT((ipa_test_filter.dl_socket_hdl_arr[20]
                    = ipa_ipfltri_alloc_rule_handle(
                      ipa_test_filter.dl_sckt_rule_cfg[20].rule_type,
                      ipa_test_filter.dl_sckt_rule_cfg[20].ip_type,
                      FALSE,
                      ipa_test_filter.dl_sckt_rule_cfg[20].rule_result,
                      ipa_test_filter.dl_sckt_rule_cfg[20].usr_data))!= NULL);
  IPA_TF_ASSERT(ipa_ipfltri_add_tc_eq(
    ipa_test_filter.dl_socket_hdl_arr[20],
    ipa_test_filter.dl_sckt_rule_cfg[20].traffic_class)
                == IPA_IPFLTR_SUCCESS );
  #endif
  /*! Commit 6 DL socket rules*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(5, &ipa_test_filter.dl_socket_hdl_arr[15])
                 != IPA_ERR_IPFLTR_COMMIT_FAIL);

  return IPA_TEST_RET_CODE_SUCCESS;
}
#pragma optimize("", on)
