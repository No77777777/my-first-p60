#ifndef PS_IN_H
#define PS_IN_H
/*===========================================================================

                   M O D E L    H E A D E R    F I L E

DESCRIPTION


Copyright (c) 2003-2010 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/* IPv6 address structure */

/** @addtogroup datatypes 
  @{
*/
/** Identifies protocols and contains the extension header types for IPv6. 
  These protocol numbers are use in dss_socket() in the dssocket.h file.
*/
typedef enum
{
  PS_IPV6_BASE_HDR        = 4,                /**< IPv6 base header. */
  PS_IPPROTO_HOP_BY_HOP_OPT_HDR = 0,          /**< Hop-by-hop option header. */
  PS_IPPROTO_ICMP         = 1,                /**< ICMP protocol. */
  PS_IPPROTO_IGMP         = 2,                /**< IGMP protocol. */
  PS_IPPROTO_IP           = PS_IPV6_BASE_HDR, /**< IPv4 protocol. */
  PS_IPPROTO_TCP          = 6,                /**< TCP protocol. */
  PS_IPPROTO_UDP          = 17,               /**< UDP protocol. */
  PS_IPPROTO_IPV6         = 41,               /**< IPv6 protocol. */
  PS_IPPROTO_ROUTING_HDR  = 43,               /**< Routing header. */
  PS_IPPROTO_FRAG_HDR     = 44,               /**< Fragmentation header. */
  PS_IPPROTO_GRE          = 47,               /**< GRE protocol. */
  PS_IPPROTO_ESP          = 50,               /**< ESP protocol. */
  PS_IPPROTO_AH           = 51,               /**< Authentication header. */
  PS_IPPROTO_ICMP6        = 58,               /**< ICMPv6. */
  PS_NO_NEXT_HDR          = 59,               /**< No next header for IPv6. */
  PS_IPPROTO_DEST_OPT_HDR = 60,               /**< Destination options header. */
  PS_IPPROTO_MOBILITY_HDR = 135,              /**< Mobility header. */
  PS_IPPROTO_TCP_UDP      = 253               /**< Unspecified protocol. */
} ps_ip_protocol_enum_type;

struct ps_in6_addr
{
  union
  {
    uint8   u6_addr8[16];
    uint16  u6_addr16[8];
    uint32  u6_addr32[4];
    uint64  u6_addr64[2];
  } in6_u;

#define ps_s6_addr    in6_u.u6_addr8
#define ps_s6_addr16  in6_u.u6_addr16
#define ps_s6_addr32  in6_u.u6_addr32
#define ps_s6_addr64  in6_u.u6_addr64
};

/*---------------------------------------------------------------------------
TYPEDEF IP_ADDR_ENUM_TYPE
---------------------------------------------------------------------------*/
/**
  Defines all address types.

  @note1hang The values are chosen for easy debugging.
 */
typedef enum
{
  IP_ANY_ADDR     = 0,                        
    /**< Address type can be IPv4 or IPv6. */
  IPV4_ADDR       = 4,                        
    /**< IPv4 address type. */
  IPV6_ADDR       = 6,                        
    /**< IPv6 address type. */
  IP_ADDR_INVALID           = 255,            
    /**< Invalid address type. */
  IFACE_ANY_ADDR_FAMILY     = IP_ANY_ADDR,    
    /**< Address family can be IPv4 or IPv6. */
  IFACE_IPV4_ADDR_FAMILY    = IPV4_ADDR,      
    /**< IPv4 address family. */
  IFACE_IPV6_ADDR_FAMILY    = IPV6_ADDR,      
    /**< IPv6 address family. */
  IFACE_UNSPEC_ADDR_FAMILY  = 8,              
    /**< Unspecified address family. */
  IFACE_INVALID_ADDR_FAMILY = IP_ADDR_INVALID 
    /**< Invalid address family. */
} ip_addr_enum_type;


/**
  @brief Internet-family specific host Internet address.
*/
struct ps_in_addr  /* structure defined for historic reasons.*/
{
  uint32 ps_s_addr; /**< Socket address.*/
};


/** @addtogroup datatypes
  @{
*/
/*---------------------------------------------------------------------------
TYPEDEF PS_IP_ADDR_TYPE
---------------------------------------------------------------------------*/
/**
  @brief Defines the IP addresses. For use with defines names; recommended.
 */
typedef struct
{
  ip_addr_enum_type type;   /**< Address Type.*/
  /** IP Address. */
  union
  {
    struct ps_in_addr  v4;  /**< IPv4 address. */
    struct ps_in6_addr v6;  /**< IPv6 address. */
  } addr;

} ps_ip_addr_type;

/**
  @brief Defines the IP addresses. For use with standard names.
*/
typedef struct ip_address
{
  ip_addr_enum_type type;     /**< Address type. */
  /** IP address structure. */
  union
  {
    uint32 v4;                /**< IPv4 address. */
    uint64 v6[2];             /**< IPv6 address. */
  } addr;

} ip_addr_type;

/**
  Indicates the IP version type.
*/

typedef enum
{
  IP_V4 = 4,  /**< IPv4 version. */
  IP_V6 = 6   /**< IPv6 version. */
} ip_version_enum_type;





#endif

