/*===========================================================================

             D O M A I N   N A M E   S E R V I C E S   M O D U L E

DESCRIPTION

 The Domain Name Services mobile API File. Contains basic API functions for
 DNS Lookup.

EXTERNALIZED FUNCTIONS

  dss_dns_init()
    Initializes the DNS subsystem during powerup.

  dss_dns_get_addrinfo()
    Queries the resolver asyncrhonously for a list of addrinfo structs.
    Reference RFC 3493.

  dss_dns_get_nameinfo()
    Queries the resolver asyncrhonously for a list of nameinfo structs.
    Reference RFC 3493.

  dss_dns_read_addrinfo()
    Function to read the the addrinfo structs. The memory for the addrinfo
    structs should be provided by the application.

  dss_dns_read_nameinfo()
    Function to read the nameinfo structs. The memory for the name
    structs should be provided by the application.

  dss_dns_clear_cache()
    Clears the cache of the specified iface.

  dss_dns_clear_cache_entry()
    Clears the specified host name from the cache 
    of the specified iface.

Copyright (c) 2007-2021 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dssvc/src/dssdns.c#4 $
  $DateTime: 2021/05/24 05:50:18 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/17/21   ks      Handling for mismatch request and response type is 
                   enhanced for the new DNS types.
11/28/11   hs      Protect from accessing api_cb from application task
                   while it is freed by PS task.
11/09/11   ea      Protect from performing two queries in parallel with the
                   same session
06/15/11   ea      Remove the critical section from
                   ps_dnsi_api_resolver_event_notify().
07/09/09   mt      Added additional rules to validate_hostname_query to stop
                   numerical queries early that will not succeed.
05/14/09   pp      Moved Local Heap allocations to Modem Heap.
08/29/08   am      Corrected Low WM counts for Low-tier builds.
02/15/08   dm      Fixed a crash that occurs when domain name validation
                   fails
06/18/07   hm      Deleting API CB after notifying error condition;
                   Returning EBADF instead of EFAULT for invalid handles.
06/11/07   hm      Fixed CNAME related bug in ai_construct_results.
05/18/07   hm      Moved deprecated hostent API to another file
03/27/07   hm      Initial development.

===========================================================================*/

/*===========================================================================

                            INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include <stringl/stringl.h>
#include "ps_handle_mgr.h"
#include "ps_mem.h"
#include "ps_dnsi.h"
#include "ps_dnsi_mem_mgr.h"
#include "dss_dns_config.h"
#include "dssdns.h"
#include "dss_dns_priv.h"
#include "ps_dnsi_defs.h"
#include "ps_dnsi_resolver.h"
#include "ps_iface.h"
#include "ps_iface_dns_cache.h"
#include "ds_Utils_DebugMsg.h"

/*===========================================================================

                            LOCAL DATA DECLARATIONS

===========================================================================*/
#define RESULT_CLIENT (PS_HANDLE_MGR_CLIENT_DNS_RESULT)
#define PS_DNSI_MAX_PORTNUM 65535


/*---------------------------------------------------------------------------
  Tuning the number of ps dnsi api callback buffers needed by this module
---------------------------------------------------------------------------*/
#define PS_DNSI_API_CB_BUF_SIZE  ((sizeof(ps_dnsi_api_cb_type) + 3) & ~3)

#ifndef FEATURE_DATA_PS_LOW_MEM_CHIPSET

#define PS_DNSI_API_CB_BUF_NUM        DSS_DNS_MAX_QUERIES
#define PS_DNSI_API_CB_BUF_HIGH_WM    ((PS_DNSI_API_CB_BUF_NUM) - 1)
#define PS_DNSI_API_CB_BUF_LOW_WM     1

#else

#define PS_DNSI_API_CB_BUF_NUM        DSS_DNS_MAX_QUERIES
#define PS_DNSI_API_CB_BUF_HIGH_WM    ((PS_DNSI_API_CB_BUF_NUM) - 1)
#define PS_DNSI_API_CB_BUF_LOW_WM     1
#endif /* FEATURE_DATA_PS_LOW_MEM_CHIPSET */

/*----------------------------------------------------------------------------
  Allocate memory to hold ps_dnsi_api_cb along with ps_mem header
----------------------------------------------------------------------------*/
static int ps_dnsi_api_cb_buf_mem[PS_MEM_GET_TOT_SIZE_OPT
                                  (
                                    PS_DNSI_API_CB_BUF_NUM,
                                    PS_DNSI_API_CB_BUF_SIZE
                                  )];


#ifdef FEATURE_DATA_PS_MEM_DEBUG
/*----------------------------------------------------------------------------
  Array of pointers used to facilitate easy debugging.  The first one points
  to the ps_mem header and the latter ponts to ps_dnsi_api_cb_buf
----------------------------------------------------------------------------*/
static ps_mem_buf_hdr_type * ps_dnsi_api_cb_buf_hdr[PS_DNSI_API_CB_BUF_NUM];
static ps_dnsi_api_cb_type * ps_dnsi_api_cb_buf_ptr[PS_DNSI_API_CB_BUF_NUM];
#endif /* FEATURE_DATA_PS_MEM_DEBUG */

/*---------------------------------------------------------------------------
  Tuning the number of ps dnsi session manager callback buffers needed by this module
---------------------------------------------------------------------------*/
#define PS_DNSI_SESSION_MGR_CB_BUF_SIZE  ((sizeof(ps_dnsi_session_mgr_cb_type) + 3) & ~3)

#ifndef FEATURE_DATA_PS_LOW_MEM_CHIPSET

#define PS_DNSI_SESSION_MGR_CB_BUF_NUM        DSS_DNS_MAX_SESSIONS
#define PS_DNSI_SESSION_MGR_CB_BUF_HIGH_WM    ((PS_DNSI_SESSION_MGR_CB_BUF_NUM) - 1)
#define PS_DNSI_SESSION_MGR_CB_BUF_LOW_WM     1

#else

#define PS_DNSI_SESSION_MGR_CB_BUF_NUM        DSS_DNS_MAX_SESSIONS
#define PS_DNSI_SESSION_MGR_CB_BUF_HIGH_WM    ((PS_DNSI_SESSION_MGR_CB_BUF_NUM) - 1)
#define PS_DNSI_SESSION_MGR_CB_BUF_LOW_WM     1
#endif /* FEATURE_DATA_PS_LOW_MEM_CHIPSET */

/*----------------------------------------------------------------------------
  Allocate memory to hold ps_dnsi_session_mgr_cb along with ps_mem header
----------------------------------------------------------------------------*/
static int ps_dnsi_session_mgr_buf_mem[PS_MEM_GET_TOT_SIZE_OPT
                                       (
                                         PS_DNSI_SESSION_MGR_CB_BUF_NUM,
                                         PS_DNSI_SESSION_MGR_CB_BUF_SIZE
                                       )];

#ifdef FEATURE_DATA_PS_MEM_DEBUG
/*----------------------------------------------------------------------------
  Array of pointers used to facilitate easy debugging.  The first one points
  to the ps_mem header and the latter ponts to ps_dnsi_session_mgr_buf
----------------------------------------------------------------------------*/
static ps_mem_buf_hdr_type         * ps_dnsi_session_mgr_cb_buf_hdr[PS_DNSI_SESSION_MGR_CB_BUF_NUM];
static ps_dnsi_session_mgr_cb_type * ps_dnsi_session_mgr_cb_buf_ptr[PS_DNSI_SESSION_MGR_CB_BUF_NUM];
#endif /* FEATURE_DATA_PS_MEM_DEBUG */

/*---------------------------------------------------------------------------
  Tuning the number of DNS servers(addr) buffers needed by this module
---------------------------------------------------------------------------*/
#define PS_DNSI_SESSION_DNS_SERVERS_BUF_SIZE  ((sizeof(ps_dnsi_addr_node_type) + 3) & ~3)

#ifndef FEATURE_DATA_PS_LOW_MEM_CHIPSET
/*---------------------------------------------------------------------------
  Define number of DNS servers per session. 
---------------------------------------------------------------------------*/
#define PS_DNSI_SESSION_DNS_SERVERS_PER_SESSION 3
/*---------------------------------------------------------------------------
  Calculate the total the number of required buffers based on
  the servers per session and max sessions(PS_DNSI_SESSION_MGR_CB_BUF_NUM).
---------------------------------------------------------------------------*/
#define PS_DNSI_SESSION_DNS_SERVERS_BUF_NUM   \
    (PS_DNSI_SESSION_DNS_SERVERS_PER_SESSION*PS_DNSI_SESSION_MGR_CB_BUF_NUM)

#define PS_DNSI_SESSION_DNS_SERVERS_BUF_HIGH_WM    2
#define PS_DNSI_SESSION_DNS_SERVERS_BUF_LOW_WM     1

#else

/* Define number of DNS servers per session */
#define PS_DNSI_SESSION_DNS_SERVERS_PER_SESSION 3
/*---------------------------------------------------------------------------
Calculate the total the number of required buffers based on
the servers per session and max sessions(PS_DNSI_SESSION_MGR_CB_BUF_NUM).
---------------------------------------------------------------------------*/
#define PS_DNSI_SESSION_DNS_SERVERS_BUF_NUM   \
  (PS_DNSI_SESSION_DNS_SERVERS_PER_SESSION*PS_DNSI_SESSION_MGR_CB_BUF_NUM)

#define PS_DNSI_SESSION_DNS_SERVERS_BUF_HIGH_WM    2
#define PS_DNSI_SESSION_DNS_SERVERS_BUF_LOW_WM     1

#endif /* FEATURE_DATA_PS_LOW_MEM_CHIPSET */

/*----------------------------------------------------------------------------
Allocate memory to hold DNS server address along with ps_mem header
----------------------------------------------------------------------------*/
static int ps_dnsi_session_dns_servers_buf_mem[PS_MEM_GET_TOT_SIZE_OPT
(
 PS_DNSI_SESSION_DNS_SERVERS_BUF_NUM,
 PS_DNSI_SESSION_DNS_SERVERS_BUF_SIZE
 )];

#ifdef FEATURE_DATA_PS_MEM_DEBUG
/*----------------------------------------------------------------------------
Array of pointers used to facilitate easy debugging.  The first one points
to the ps_mem header and the latter points to ps_dnsi_addr_node_type
----------------------------------------------------------------------------*/
static ps_mem_buf_hdr_type    * ps_dnsi_session_dns_servers_buf_hdr[PS_DNSI_SESSION_DNS_SERVERS_BUF_NUM];
static ps_dnsi_addr_node_type * ps_dnsi_session_dns_servers_buf_ptr[PS_DNSI_SESSION_DNS_SERVERS_BUF_NUM];
#endif /* FEATURE_DATA_PS_MEM_DEBUG */

/*---------------------------------------------------------------------------
  Tuning the number of ps dnsi hostent callback buffers needed by this module
---------------------------------------------------------------------------*/
#define PS_DNSI_HOSTENT_CB_BUF_SIZE  ((sizeof(dss_dnsi_hostent_cb_type) + 3) & ~3)

#ifndef FEATURE_DATA_PS_LOW_MEM_CHIPSET

#define PS_DNSI_HOSTENT_CB_BUF_NUM        DSS_DNS_MAX_QUERIES
#define PS_DNSI_HOSTENT_CB_BUF_HIGH_WM    ((PS_DNSI_HOSTENT_CB_BUF_NUM) - 1)
#define PS_DNSI_HOSTENT_CB_BUF_LOW_WM     2

#else

#define PS_DNSI_HOSTENT_CB_BUF_NUM        DSS_DNS_MAX_QUERIES
#define PS_DNSI_HOSTENT_CB_BUF_HIGH_WM    ((PS_DNSI_HOSTENT_CB_BUF_NUM) - 1)
#define PS_DNSI_HOSTENT_CB_BUF_LOW_WM     2
#endif /* FEATURE_DATA_PS_LOW_MEM_CHIPSET */

/*----------------------------------------------------------------------------
  Allocate memory to hold ps_dnsi_hostent_cb along with ps_mem header
----------------------------------------------------------------------------*/
static int ps_dnsi_hostent_buf_mem[PS_MEM_GET_TOT_SIZE_OPT
                                   (
                                     PS_DNSI_HOSTENT_CB_BUF_NUM,
                                     PS_DNSI_HOSTENT_CB_BUF_SIZE
                                   )];

#ifdef FEATURE_DATA_PS_MEM_DEBUG
/*----------------------------------------------------------------------------
  Array of pointers used to facilitate easy debugging.  The first one points
  to the ps_mem header and the latter ponts to ps_dnsi_hostent_buf
----------------------------------------------------------------------------*/
static ps_mem_buf_hdr_type       * ps_dnsi_hostent_cb_buf_hdr[PS_DNSI_HOSTENT_CB_BUF_NUM];
static dss_dnsi_hostent_cb_type  * ps_dnsi_hostent_cb_buf_ptr[PS_DNSI_HOSTENT_CB_BUF_NUM];
#endif /* FEATURE_DATA_PS_MEM_DEBUG */


/*===========================================================================

                         INTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  Forward declarations for internal session management functions
---------------------------------------------------------------------------*/
/*===========================================================================
FUNCTION  DSS_DNS_SESSION_MGR_INIT

DESCRIPTION
  Initializes the session manager module.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void dss_dns_session_mgr_init
(
  void
);

/*===========================================================================
FUNCTION DSS_DNS_SESSION_MGR_IS_HANDLE_VALID

DESCRIPTION
  Checks if the specified session handle is valid or not.

PARAMETERS
  session_handle  - Session manager control block handle

RETURN VALUE
  TRUE if the handle is valid
  FALSE if the handle is not valid.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern ps_dnsi_session_mgr_cb_type* dss_dns_get_session_mgr_from_handle
(
  dss_dns_session_mgr_handle_type     session_handle
);


/*===========================================================================
FUNCTION DSS_DNS_SESSION_MGR_IS_LEGACY_SESSION_TYPE

DESCRIPTION
  Checks if the specified session is legacy session created by legacy API

PARAMETERS
  session_ptr  - Session manager control block pointer

RETURN VALUE
  TRUE if session were created with legacy API
  FALSE if session were created with new ex API

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean dss_dns_mgr_is_legacy_session
(
  ps_dnsi_session_mgr_cb_type*     session_ptr
);


/*===========================================================================
FUNCTION DSS_DNS_SESSION_MGRI_GET_CB_PTR

DESCRIPTION
  Checks if the specified handle is valid and if it is valid gets the
  control block corresponding to the handle.

PARAMETERS
  session_handle  - Session manager control block handle

RETURN VALUE
  Pointer to the control block if the specified handle is valid.
  NULL if the specified handle is not valid.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern ps_dnsi_session_mgr_cb_type  * dss_dns_session_mgri_get_cb_ptr
(
  dss_dns_session_mgr_handle_type     session_handle
);


/*===========================================================================
FUNCTION  PS_DNSI_API_RESOLVER_EVENT_NOTIFY()

DESCRIPTION
  Callback function registered with the resolver.

PARAMETERS

  event         - Type of resolver event.
                  PS_DNSI_RES_NOTIFY_SUCCESS or PS_DNSI_RES_NOTIFY_FAILURE
  res_handle    - Handle to the resolver control block.
  user_data_ptr - Points to the API control block.
  dss_errno         - Error code in both events.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
 void ps_dnsi_api_resolver_event_notify
(
  ps_dnsi_resolver_notify_event_enum_type   event,
  ps_dnsi_resolver_handle_type              res_handle,
  q_type                                  * answer_q,
  void                                    * user_data_ptr,
  int16                                     dss_errno
);

#define DNS_DSS_ASSERT(expression) \
         dns_dss_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dns_dss_assert_wrapper()

DESCRIPTION
  Wrapper function for DNS_HOST_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dns_dss_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DSNET_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* dns_host_assert_wrapper */

/*===========================================================================
FUNCTION DSS_DNSI_ATOI

DESCRIPTION
  Unsigned, radix-10 custom implementation of atoi. If there was an atoi
  in AEE_Stdlib we would not need this.

PARAMETERS
  s - string to convert

RETURN VALUE
  True if conversion is successful.
  Value is set to conversion's value.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean dss_dnsi_atoi
(  
   const char *s,
   uint32 *value
)
{
   char c;
   uint32 val = 0;

   if ((s == NULL) || (value == NULL))
   {
      return FALSE;
   }

   while ( (c = *s++) != '\0')
   {
      if ((c < '0') || (c > '9')) // not a digit
      {
         return FALSE;
      }

      if ((val*10 + c - '0') > PS_DNSI_MAX_PORTNUM) // number is becoming too large
      {
         return FALSE;
      }

      if ((val == 0) && (c == '0')) // do not allow leading 0
      {
         return FALSE;
      }

      val = val*10 + c-'0';
   }

   *value = val;
   return TRUE;
}


/*===========================================================================
FUNCTION DSS_DNSI_GET_CURR_QUERY_TYPE

DESCRIPTION
  Find the highest bit mask set in the current mask. return current query type based on current mask

PARAMETERS
  curr_query_mask - current query mask

RETURN VALUE
  Current query type set in mask

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
ps_dnsi_query_type_enum_type dss_dnsi_get_curr_query_type
(
   dss_dns_api_query_enum_type      curr_query_mask
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    DNS Query processing order 
    1. No other mask will set in case of PTR.
    2. NAPTR->SRV->A->AAAA ( A->AAAA is as legacy API)
  -------------------------------------------------------------------------*/
  if( 0 != (curr_query_mask & DSS_DNS_API_QUERY_PTR) )
  {
    return PS_DNSI_QUERY_TYPE_PTR;
  }
  if( 0 != (curr_query_mask & DSS_DNS_API_QUERY_NAPTR) )
  {
    return PS_DNSI_QUERY_TYPE_NAPTR;
  }
  if( 0 != (curr_query_mask & DSS_DNS_API_QUERY_SRV ))
  {
    return PS_DNSI_QUERY_TYPE_SRV;
  }
  if( 0 != (curr_query_mask & DSS_DNS_API_QUERY_A ))
  {
    return PS_DNSI_QUERY_TYPE_A;
  }
  if( 0 != (curr_query_mask & DSS_DNS_API_QUERY_AAAA ) )
  {
    return PS_DNSI_QUERY_TYPE_AAAA;
  }

  return PS_DNSI_QUERY_TYPE_MIN;
}/* dss_dnsi_get_curr_query_type */

/*===========================================================================
FUNCTION DSS_DNSI_CONVERT_QUERY_INTO_RECORD_TYPE

DESCRIPTION
  Convert query type into record type

PARAMETERS
  record_type- query mask

RETURN VALUE
  query mask

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
dss_dns_record_type_enum_type dss_dnsi_convert_query_into_record_type
(
  ps_dnsi_query_type_enum_type     query_type
)
{

  dss_dns_record_type_enum_type  record_type = DSS_DNS_RECORD_MIN;

  switch( query_type )
  {
    case PS_DNSI_QUERY_TYPE_A:
     record_type = DSS_DNS_RECORD_A;
     break;
    case PS_DNSI_QUERY_TYPE_AAAA:
     record_type = DSS_DNS_RECORD_AAAA;
     break;
    case PS_DNSI_QUERY_TYPE_PTR:
     record_type = DSS_DNS_RECORD_PTR;
     break;
    case PS_DNSI_QUERY_TYPE_CNAME:
     record_type = DSS_DNS_RECORD_CNAME;
     break;
    case PS_DNSI_QUERY_TYPE_SRV:
     record_type = DSS_DNS_RECORD_SRV;
     break;
    case PS_DNSI_QUERY_TYPE_NAPTR:
     record_type = DSS_DNS_RECORD_NAPTR;
     break;
  
    default:
      break;
  }

  return record_type;
}/* dss_dnsi_convert_query_into_record_type */

/*===========================================================================
FUNCTION DSS_DNSI_PARSE_SERVICE_STRING()

DESCRIPTION
  Validate and parse a service string.

  According to RFC 2553 and to POSIX spec of getaddrinfo(), "service" is a 
  string of either a port number or a service name.

  If the input is a numeric string of allowed range (1-65535), it will be 
  parsed, converted, and returned. If the input is NULL, a special value
  PS_DNSI_SERVICE_NOT_SPECIFIED will be returned and the conversion will be
  successful. All other inputs will be rejected.

  Service names are not currently supported.

PARAMETERS
  service_ptr - Pointer to service string.
  service     - Pointer to the conversion result (output).

RETURN VALUE
  TRUE on parse success:
  - service is either a valid port number or PS_DNSI_SERVICE_NOT_SPECIFIED.

  FALSE on parse fail (string does not contain a valid service specifier):
  - service is undefined.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean dss_dnsi_parse_service_string
(
   const char * service_ptr,
   uint16 * service
)
{
   uint32 atoi_result;

   if (NULL == service_ptr) 
   {
      *service = PS_DNSI_SERVICE_NOT_SPECIFIED;
      return TRUE;
   }

   if (dss_dnsi_atoi(service_ptr, &atoi_result))
   {
      if ((atoi_result > 0) && (atoi_result <= PS_DNSI_MAX_PORTNUM))
      {
         *service = (uint16)atoi_result;
         return TRUE;
      }
   }

   /* Service names are not currently supported. If/when they will be, this
   function will also need to receive the hints and check for 
   AI_NUMERICHOST--if set, then by Posix def. of getaddrinfo only numeric 
   service is allowed. */

   return FALSE;
} /* dss_dnsi_parse_service_string */

boolean dss_dnsi_validate_numerical_query
(
 const char      * hostname_ptr,
 boolean         * is_v4_addr
)
{
  struct ps_in_addr  temp_in_addr;
  struct ps_in6_addr temp_in6_addr;
  int16              dss_errno;
  int32              retval = DSS_ERROR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-----------------------------------------------------------------------
    Check if hostname is a IPv4 dotted decimal string.
  -----------------------------------------------------------------------*/
  retval = dss_inet_pton( hostname_ptr,
                          DSS_AF_INET,
                          &temp_in_addr,
                          sizeof(struct ps_in_addr),
                          &dss_errno );
  
  if( DSS_SUCCESS == retval )
  {
    *is_v4_addr       = TRUE;
    return TRUE;
  }
  /*-------------------------------------------------------------------------
    Check if hostname is a IPv6 colon seperated hex string.
  -------------------------------------------------------------------------*/
  retval = dss_inet_pton( hostname_ptr,
                          DSS_AF_INET6,
                          &temp_in6_addr,
                          sizeof(struct ps_in6_addr),
                          &dss_errno );
  if( DSS_SUCCESS == retval )
  {
    *is_v4_addr       = FALSE;
     return TRUE;
  }

  return FALSE;
  
}/*dss_dnsi_validate_numerical_query */

boolean dss_dnsi_validate_alphanumerical_query
(
  const char      * hostname_ptr
)
{
  uint16                      index      = 0;
  unsigned char               curr_char  = 0;
  unsigned char               next_char  = 0;
  int16                       label_len  = 0;
  dss_dns_error_enum_type     err_type      = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Validate that hostname conforms to DNS domain naming guidelines
      1. The first and last character of each label must be either a letter
         or digit
      2. Rest of the characters in a label must be a letter or a digit or a
         hyphen
      3. Length of each label must be <= 63 characters
      4. Length of domain name must be <= 255 characters
  -------------------------------------------------------------------------*/
  do
  {
  
    label_len = 0;
    if( !ps_isalnum(*hostname_ptr) )
    {
      err_type             = DSS_DNS_ERROR_LABEL_INVALID_START_CHAR;
      break;
    }
    
    label_len++;
  
    for( index = 1; (index < ( DSS_DNS_MAX_DOMAIN_NAME_LEN - 1 ))&&( DSS_DNS_ERROR_MIN == err_type); index++ )
    {
      curr_char = hostname_ptr[index];
    
      if( '\0' == curr_char )
      {
        break;
      }
      else if( '.' == curr_char )
      {
        /*---------------------------------------------------------------------
          Reached end of a label. Make sure that length is valid
        ---------------------------------------------------------------------*/
        if( DSS_DNS_MAX_LABEL_LEN <= label_len )
        {          
          err_type             = DSS_DNS_ERROR_LABEL_INVALID_LENGTH;
          break;
        }
        /*---------------------------------------------------------------------
          Reset the label length and start counting for next label
        ---------------------------------------------------------------------*/
        label_len = 0;
        /*---------------------------------------------------------------------
          Make sure that either we reached end of domain name or first character
          of next label is valid
        ---------------------------------------------------------------------*/
        next_char = hostname_ptr[++index];
    
        if( '\0' == next_char )
        {
          break;
        }
        else if( !ps_isalnum( next_char ) )
        {
          err_type             = DSS_DNS_ERROR_LABEL_INVALID_START_CHAR;
          break;
        }
      }
      else if( !ps_isalnum( curr_char ) && '-' != curr_char )
      {
        err_type      = DSS_DNS_ERROR_LABEL_INVALID_CHAR;
        break;
      }
      label_len++;
    }

    if ( DSS_DNS_ERROR_MIN != err_type )
    {
      break;
    }
  
    if(( DSS_DNS_MAX_DOMAIN_NAME_LEN - 1 ) == index )
    {
      /*-----------------------------------------------------------------------
        This means that domain name is of max possible length. Make sure that
          1. Domain name is NULL terminated
          2. Domain name ends with '.'. DNS mandates that a domain name must end
             with '.'. Typically user doesn't do it and hence the resolver
             appends '.' before querying the DNS server. If domian is already of
             max length, resolver can't append '.'
      -----------------------------------------------------------------------*/
      if( '\0' != hostname_ptr[index] )
      {
        err_type      = DSS_DNS_ERROR_DOMAIN_NAME_NOT_TERM_NULL_CHAR;
        break;
      }
      else if( '.' != hostname_ptr[index - 1] )
      {
        err_type      =DSS_DNS_ERROR_DOMAIN_NAME_INVALID_LENGTH;
        break;
      }
    }
    return TRUE;
  }while(0);

  LOG_MSG_INFO1_4("dss_dnsi_validate_alphanumerical_query err_type %d  label_len %d next_char %c index %d", 
                  err_type, label_len, next_char, index);
  
  return FALSE;
}/* dss_dnsi_validate_alphanumerical_query*/

/*===========================================================================
FUNCTION DSS_DNSI_VALIDATE_HOSTNAME_QUERY()

DESCRIPTION
  Validates a domain name

  A domain name is made up of one or more label names seperated by '.'.

  Valid characters allowed in the domain name are a-z, A-Z, 0-9
  and '-'(hyphen). The beginning character of the domain name and of labels
  should be an aplphabet.

  The length of the labels should not exceed DSS_DNS_MAX_LABEL_LEN.
  The length of the entire domain name should not exceed
  DSS_DNS_MAX_DOMAIN_LEN.

  An additional check is performed when hints_ptr is not NULL: when the 
  hostname is a numerical address, the corresponding address family or 
  AF_UNSPEC must be specified in the hints structure. Passing a different
  address family in the hints structure fails the validation, as such a query
  would not succeed.

PARAMETERS
  domain_name_ptr   - Pointer to domain name string.
  hints_ptr         - Pointer to the hints structure.

RETURN VALUE
  TRUE on Success or if the domain name argument is NULL.
  FALSE if the domain name is invalid

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean dss_dnsi_validate_hostname_query
(
  const char      * hostname_ptr,
  const struct dss_dns_addrinfo * hints_ptr
)
{

  boolean            is_v4_addr = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Return true for NULL hostname.
  -------------------------------------------------------------------------*/
  if( NULL == hostname_ptr )
  {
    LOG_MSG_INFO1_0("NULL domain name");
    return TRUE;
  }
  /*-----------------------------------------------------------------------
    Check if hostname is a numerical
  -----------------------------------------------------------------------*/
  if (TRUE == dss_dnsi_validate_numerical_query ( hostname_ptr , &is_v4_addr) )
  {
    if (hints_ptr != NULL)
    {
      if ( (TRUE == is_v4_addr && DSS_AF_INET6 == hints_ptr->ai_family) ||
           (FALSE == is_v4_addr && DSS_AF_INET == hints_ptr->ai_family))
      {
         return FALSE;
      }
    }
    return TRUE;
  }
  
  /* At this point, numerical queries have already returned. If NUMERICHOST
  flag is set, then other types of hostnames are not allowed */
  /*lint -save -e655 Reason for suppressing error 655*/
  if ((hints_ptr != NULL) && (hints_ptr->ai_flags & DSS_DNS_AI_FLAGS_NUMERICHOST))
  /*lint -restore Restore lint error 655*/
  {
     LOG_MSG_INFO1_0("AI_FLAGS_NUMERICHOST specified, but hostname is not numeric.");
     return FALSE;
  }

  if ( FALSE == dss_dnsi_validate_alphanumerical_query( hostname_ptr ) )
  {
    return FALSE;
  }

  return TRUE;
} /* dss_dnsi_validate_hostname_query() */

/*===========================================================================
FUNCTION DSS_DNSI_CONSTRUCT_QUERIES()

DESCRIPTION
  Constructs a hostname or pointer query for the given api type.
  For get_addrinfo API, constructs A and/or AAAA queries.
  For get_nameinfo API, constucts IPv4 or IPv6 PTR query
  depending upon the address family.

  An IPv4 query is represented as a sequence of bytes in reverse order,
  represented as decimals, seperated by dots with suffix in-addr.arpa.
  Ex: a.b.c.d --> d.c.b.a.in-addr.arpa.

  An IPv6 query is represented as a sequence of nibbles in reverse order,
  represented as hex digits, seperated by dots with suffix ip6.arpa.
  Ex: abcd::efgh ->
  h.g.f.e.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.d.c.b.a.ip6.arpa.

PARAMETERS
  api_type         - Type of API requesting the construction of query.
                     Depending upon this we could construct a hostname query
                     or a pointer query.
  addr_family      - Address family.
  flags            - Set of possible flags.
  query_data_ptr   - Pointer to the array containing the query.
  query_struct_ptr - Struct containing number of queries created, query type
                     and query class for each query and a query data array.

RETURN VALUE
  None.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dss_dnsi_construct_queries
(
  ps_dnsi_query_type_enum_type  query_type,
  uint32                        flags,
  const char                  * query_data_ptr,
  ps_dnsi_query_struct_type   * query_struct_ptr
)
{
  struct ps_in_addr  temp_in_addr;
  struct ps_in6_addr temp_in6_addr;
  /* Data structure to convert a nibble to hexadecimal printable character */
  unsigned char     hex_to_char[]={ '0','1','2','3','4','5','6','7',
                                    '8','9','A','B','C','D','E','F' };
  int16             msg_index = 0;
  int16             index = 0;
  uint8             temp_nibble = 0;
  struct ps_sockaddr_in6 temp_sockaddr_in6;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset( query_struct_ptr, 0, sizeof( ps_dnsi_query_struct_type ) );
  /*-----------------------------------------------------------------------
    Set query type and query class
  -----------------------------------------------------------------------*/
  query_struct_ptr->query_class = PS_DNSI_QUERY_CLASS_IN;
  query_struct_ptr->query_type  = query_type;

  /*-------------------------------------------------------------------------
    Fill query data based on Query type
  -------------------------------------------------------------------------*/
  if( query_type ==  PS_DNSI_QUERY_TYPE_NAPTR ||
      query_type ==  PS_DNSI_QUERY_TYPE_SRV   ||
      query_type ==  PS_DNSI_QUERY_TYPE_AAAA ||
      query_type ==  PS_DNSI_QUERY_TYPE_A )
  {
    /*-----------------------------------------------------------------------
      For addrinfo, if query is NULL then depending upon the PASSIVE flag
      we should be able to connect to localhost or bind to PS_INADDR_ANY or
      IN6ADDR_ANY (Both of which are represented by 0)
    -----------------------------------------------------------------------*/
    if( NULL == query_data_ptr )
    {
      if( (uint32)DSS_DNS_AI_FLAGS_PASSIVE & flags )
      {
        *(query_struct_ptr->query_data) = 0;
      }
      else
      {
        (void) strlcpy( (char *) query_struct_ptr->query_data,
                            (char *) "localhost.",
                            DSS_DNS_MAX_DOMAIN_NAME_LEN );
      }
    }
    else
    {
      (void) strlcpy( (char *) query_struct_ptr->query_data,
                          query_data_ptr,
                          DSS_DNS_MAX_DOMAIN_NAME_LEN );
      ps_dnsi_add_trailing_dot_to_hostname(query_struct_ptr->query_data);
    } /* if-else NULL == query_data_ptr */
  } /* if api type is addrinfo */
  else if ( query_type ==  PS_DNSI_QUERY_TYPE_PTR )
  {
    /*-----------------------------------------------------------------------
      For get_nameinfo(PTR) we send a pointer query.
    -----------------------------------------------------------------------*/
    if( NULL != query_data_ptr )
    {
      if( DSS_AF_INET == ((struct ps_sockaddr*)query_data_ptr)->ps_sa_family )
      {
        memscpy(
          &temp_in_addr,
          sizeof(struct ps_in_addr),
          ((struct ps_sockaddr*)query_data_ptr)->ps_sa_data,
          sizeof(struct ps_in_addr) );
        temp_in_addr.ps_s_addr   = ((temp_in_addr.ps_s_addr & 0xff)    << 24) |
                                ((temp_in_addr.ps_s_addr & 0xff00)     << 8)  |
                                ((temp_in_addr.ps_s_addr & 0xff0000)   >> 8)  |
                                ((temp_in_addr.ps_s_addr & 0xff000000U)>> 24);

        if( DSS_ERROR == dss_inet_ntoa( temp_in_addr,
                                        (uint8 *) query_struct_ptr->query_data,
                                        DSS_DNS_MAX_DOMAIN_NAME_LEN ) )
        {
          DNS_DSS_ASSERT(0);
          return;
        }
        /*-------------------------------------------------------------------
          Add a trailing dot '.' and "in-addr.arpa."
        -------------------------------------------------------------------*/
        (void) strlcat( query_struct_ptr->query_data,
                            ".",
                            PS_IN_ADDRSTRLEN + sizeof(DNS_INADDR_ARPA_STR) + 1 );

        (void) strlcat( query_struct_ptr->query_data,
                            DNS_INADDR_ARPA_STR,
                            PS_IN_ADDRSTRLEN + sizeof(DNS_INADDR_ARPA_STR) + 1 );
      }
      else /* if address family is DSS_AF_INET6 */
      {
        memscpy(&temp_sockaddr_in6, sizeof(struct ps_sockaddr_in6),
          query_data_ptr, sizeof(struct ps_sockaddr_in6));
        memscpy( &temp_in6_addr, sizeof(struct ps_in6_addr),
          &(temp_sockaddr_in6.ps_sin6_addr), sizeof(struct ps_in6_addr));
        msg_index = 0;
        for( index = 15; index >= 0; index-- )
        {
          temp_nibble = temp_in6_addr.ps_s6_addr[index] & 0x0F;
          query_struct_ptr->query_data[msg_index] =
            hex_to_char[ temp_nibble ];
          msg_index++;
          query_struct_ptr->query_data[msg_index] = '.';
          msg_index++;

          temp_nibble = (temp_in6_addr.ps_s6_addr[index] & 0xF0) >> 4;
          query_struct_ptr->query_data[msg_index] =
            hex_to_char[ temp_nibble ];
          msg_index++;
          query_struct_ptr->query_data[msg_index] = '.';
          msg_index++;
        }
        (void) strlcpy( (char *) &(query_struct_ptr->query_data[msg_index]),
                                   DNS_IN6ADDR_ARPA_STR,
                                   sizeof(DNS_IN6ADDR_ARPA_STR) );
        msg_index+=sizeof(DNS_IN6ADDR_ARPA_STR);
        query_struct_ptr->query_data[msg_index] = '\0';
      } /* if-else  */
    }/* If NULL != Query data */
    else
    {
      *( query_struct_ptr->query_data) = 0;
    }/* If NULL == Query data */
  } /* if api type is nameinfo */
  
} /* dss_dnsi_construct_queries() */

/*===========================================================================
FUNCTION DSS_DNSI_CONSTRUCT_AI_RESULTS()

DESCRIPTION
  Constructs dss_dns_addrinfo structures from the generic RR Q.

PARAMETERS
  rr_q          - Generic RR Q.
  ai_family     - Address family for the query asked.
  ai_socktype   - Socket type for the socket addresses.
  ai_proto      - Transport protocol
  ai_flags      - Addrinfo options
  dst_buffer    - Buffer where the results need to be stored.
  num_records   - Number of records (max) to be returned.

RETURN VALUE
  None.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void dss_dnsi_construct_ai_results
(
  q_type                    * rr_q,
  uint16                      ai_socktype,
  uint16                      ai_proto,
  dss_dns_ai_flags_enum_type  ai_flags,
  uint16                      ai_port,
  struct dss_dns_addrinfo   * dst_buffer,
  uint16                      num_records
)
{
  uint16                      index            = 0;
  ps_dnsi_generic_rr_type   * rr_node_ptr      = NULL;
  struct ps_sockaddr_in       temp_in_sockaddr;
  struct ps_sockaddr_in6      temp_in6_sockaddr;
  struct ps_in_addr           temp_in_addr;
  struct ps_in6_addr          temp_in6_addr;
  uint16                      ai_family = PS_DNSI_QUERY_TYPE_A;
  int                         len= 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( NULL == dst_buffer || 0 == num_records )
  {
    LOG_MSG_ERROR_2("Invalid destination buffer 0x%p or num records %d",
                    dst_buffer, num_records);
    DNS_DSS_ASSERT(0);
    return;
  }

  if( NULL == rr_q )
  {
    DNS_DSS_ASSERT(0);
    return;
  }

  if( ((uint16)DSS_SOCK_DGRAM != ai_socktype && (uint16)DSS_SOCK_STREAM != ai_socktype) ||
      ((uint16)PS_IPPROTO_TCP != ai_proto && (uint16)PS_IPPROTO_UDP != ai_proto && 
                                                                  0 != ai_proto ) )
  {
    LOG_MSG_ERROR_2("Inv. arguments family %d, socktype %d, proto %d",
                     ai_socktype, ai_proto);
    DNS_DSS_ASSERT(0);
    return;
  }

  /*lint -save -e655 Reason for suppressing error 655*/
  if( 0 != ((int)ai_flags & ~((int)DSS_DNS_AI_FLAGS_PASSIVE     |
                              (int)DSS_DNS_AI_FLAGS_CANONICAL   |
                              (int)DSS_DNS_AI_FLAGS_NUMERICHOST |
                              (int)DSS_DNS_AI_FLAGS_V4MAPPED    |
                              (int)DSS_DNS_AI_FLAGS_ALL         |
                              (int)DSS_DNS_AI_FLAGS_ADDRCONFIG  |
                              (int)DSS_DNS_AI_FLAGS_LOCAL_QUERY)) )
  /*lint -restore Restore lint error 655*/
  {
    LOG_MSG_ERROR_1("Invalid flag parameter 0x%x", ai_flags);
    DNS_DSS_ASSERT(0);
    return;
  }

  rr_node_ptr = q_check(rr_q);
  while( NULL != rr_node_ptr && index < num_records )
  {
    memset(&temp_in_sockaddr, 0, sizeof(struct ps_sockaddr_in));
    memset(&temp_in6_sockaddr, 0, sizeof(struct ps_sockaddr_in6));
    memset( dst_buffer + index, 0, sizeof(struct dss_dns_addrinfo) );

    switch( rr_node_ptr->rr_type )
    {
      case PS_DNSI_QUERY_TYPE_A:
        DNS_DSS_ASSERT( sizeof(uint32) == rr_node_ptr->rdata_len );
        ai_family = DSS_AF_INET;
        memscpy( &temp_in_addr, sizeof(struct ps_in_addr),
          rr_node_ptr->rdata, sizeof(struct ps_in_addr) );
        dst_buffer[index].ai_family = ai_family;
        temp_in_sockaddr.ps_sin_family = DSS_AF_INET;
        temp_in_sockaddr.ps_sin_port   = ps_htons(ai_port);
        temp_in_sockaddr.ps_sin_addr   = temp_in_addr;

        memscpy( &(dst_buffer[index].ai_sockaddr),
          sizeof(struct ps_sockaddr_in), &temp_in_sockaddr,
          sizeof(struct ps_sockaddr_in) );
        break;

      case PS_DNSI_QUERY_TYPE_AAAA:
        DNS_DSS_ASSERT( 2 * sizeof(uint64) == rr_node_ptr->rdata_len );
        ai_family = DSS_AF_INET6;
        memscpy( &temp_in6_addr, sizeof(struct ps_in6_addr),
          rr_node_ptr->rdata, sizeof(struct ps_in6_addr) );
        dst_buffer[index].ai_family = ai_family;
        temp_in6_sockaddr.ps_sin6_family = DSS_AF_INET6;
        temp_in6_sockaddr.ps_sin6_port   = ps_htons(ai_port);
        temp_in6_sockaddr.ps_sin6_addr   = temp_in6_addr;

        memscpy( &(dst_buffer[index].ai_sockaddr),
          sizeof(struct ps_sockaddr_in6),
          &temp_in6_sockaddr,
          sizeof(struct ps_sockaddr_in6) );
        break;

      case PS_DNSI_QUERY_TYPE_CNAME:
        dst_buffer[index].ai_family = ai_family;
        len = MIN( rr_node_ptr->rdata_len, DSS_DNS_MAX_DOMAIN_NAME_LEN );
        
        (void) strlcpy( dst_buffer[index].ai_canonname,
                            (char *) rr_node_ptr->rdata,
                            len );
        break;

      default:
        LOG_MSG_ERROR_1("Unkonwn RR type %d", rr_node_ptr->rr_type);
        break;
    } /* switch */

    if( 0 == *(dst_buffer[index].ai_canonname) )
    {
      (void) strlcpy( dst_buffer[index].ai_canonname,
                          (char *) rr_node_ptr->domain_name,
                          DSS_DNS_MAX_DOMAIN_NAME_LEN );
    }

    dst_buffer[index].ai_socktype = ai_socktype;
    dst_buffer[index].ai_protocol = ai_proto;
    dst_buffer[index].ttl = rr_node_ptr->ttl;
    /*-------------------------------------------------------------------------
      Legacy APIs only return A or AAAA records
    -------------------------------------------------------------------------*/
    if ( rr_node_ptr->rr_type == PS_DNSI_QUERY_TYPE_A ||
         rr_node_ptr->rr_type == PS_DNSI_QUERY_TYPE_AAAA)
    {
      index++;
    }
    rr_node_ptr = q_next( rr_q, &(rr_node_ptr->link) );
  } /* for */

  return;
} /* dss_dnsi_construct_ai_results() */

/*===========================================================================
FUNCTION DSS_DNSI_CONSTRUCT_NI_RESULTS()

DESCRIPTION
  Constructs dss_dns_nameinfo result struct from RR Q.

PARAMETERS
  rr_q    - Generic RR Q.

RETURN VALUE
  None.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void dss_dnsi_construct_ni_results
(
  q_type                   * rr_q,
  struct dss_dns_nameinfo  * dst_buffer,
  uint16                     num_records
)
{
  uint16                      index;
  ps_dnsi_generic_rr_type   * rr_node_ptr;
  int                         len;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( NULL == dst_buffer || 0 == num_records )
  {
    LOG_MSG_ERROR_2("Invalid destination buffer 0x%p or num records %d",
                    dst_buffer, num_records);
    DNS_DSS_ASSERT(0);
    return;
  }

  if( NULL == rr_q )
  {
    DNS_DSS_ASSERT(0);
    return;
  }

  rr_node_ptr = q_check(rr_q);
  for( index = 0; index < num_records && NULL != rr_node_ptr; index++ )
  {
    switch( rr_node_ptr->rr_type )
    {
      case PS_DNSI_QUERY_TYPE_PTR:
        len = MIN( rr_node_ptr->rdata_len,
                   (int16)sizeof(dst_buffer[index].hostname) );
        (void) strlcpy( dst_buffer[index].hostname,
                            (char *) rr_node_ptr->rdata,
                            len );
        dst_buffer[index].ttl = rr_node_ptr->ttl;
        break;

      default:
        LOG_MSG_ERROR_1("Unkonwn RR type %d", rr_node_ptr->rr_type);
        break;
    } /* switch */

    rr_node_ptr = q_next( rr_q, &(rr_node_ptr->link) );
  } /* for */

} /* dss_dnsi_construct_ni_results() */

uint16 dss_dnsi_get_port_from_srv_res
(
  q_type                        * rr_q,
  ps_dnsi_generic_rr_type       * rr_node_ptr
)
{

  ps_dnsi_generic_rr_type         * rr_ptr   = NULL;
  uint16                            port_num = 0;
  ps_dnsi_generic_srv_rdata_type  * srv_rec_ptr  = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Go through all records available. 
    Check if there is any SRV records (Target) which matchs with current A result record domain name.
    return port number.
  -------------------------------------------------------------------------*/
  rr_ptr = q_check(rr_q);
  
  while( NULL != rr_ptr)
  {
    if ( PS_DNSI_QUERY_TYPE_SRV == rr_ptr->rr_type )
    {
        srv_rec_ptr = (ps_dnsi_generic_srv_rdata_type *)rr_ptr->rdata;
        
      if (0 == strncmp( (const char *)srv_rec_ptr->target, (const char *)rr_node_ptr->domain_name, 
                        strlen(rr_node_ptr->domain_name)))
      {
        port_num = ps_htons(srv_rec_ptr->port);
        break;
      }
    }
    rr_ptr = q_next( rr_q, &(rr_ptr->link) );
  }
  return port_num;
}/*dss_dnsi_get_port_from_srv_res*/

dss_dns_api_query_enum_type dss_dnsi_get_query_type_from_naptr_flag
(
  uint8  *flag
)
{

   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        At this time only four flags, "S", "A", "U", and "P", are
        defined.  The "S", "A" and "U" flags denote a terminal lookup.
        This means that this NAPTR record is the last one and that the
        flag determines what the next stage should be.  The "S" flag
        means that the next lookup should be for SRV records. 
        "A" means that the next lookup should be for either
        an A, AAAA, or A6 record.  The "U" flag means that the next step
        is not a DNS lookup but that the output of the Regexp field is an
        URI that adheres to the 'absoluteURI'
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   
  if ( 'S' == flag[0] || 's' == flag[0])
  {
    return DSS_DNS_API_QUERY_SRV;
  }

  if ( 'A' == flag[0] || 'a' == flag[0])
  {
    return DSS_DNS_API_QUERY_A|DSS_DNS_API_QUERY_AAAA;
  }

  if ( 'U' == flag[0] || 'u' == flag[0])
  {
    return DSS_DNS_API_QUERY_MIN;
  }
  /*-------------------------------------------------------------------------
    Special case: The use of NAPTR resource record "" flag pointing to other NAPTR resource records. 
    flag "" is strongly discouraged.Specifically, NAPTR resource flag "" should only be provisioned to 
    point to terminal NAPTR records (Spec 29.303 Section 4.1.2)
  -------------------------------------------------------------------------*/
  if ( 0 == flag[0] || '\0' == flag[0])
  {
    return DSS_DNS_API_QUERY_NAPTR;
  }

  return DSS_DNS_API_QUERY_MAX;
}/* dss_dnsi_get_query_type_from_naptr_flag */

/*===========================================================================
FUNCTION DSS_DNSI_CONSTRUCT_AI_RESULTS()

DESCRIPTION
  Constructs dss_dns_addrinfo structures from the generic RR Q.

PARAMETERS
  rr_q          - Generic RR Q.
  ai_family     - Address family for the query asked.
  ai_socktype   - Socket type for the socket addresses.
  ai_proto      - Transport protocol
  ai_flags      - Addrinfo options
  dst_buffer    - Buffer where the results need to be stored.
  num_records   - Number of records (max) to be returned.

RETURN VALUE
  None.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
int16  dss_dnsi_construct_ai_results_ext
(
  q_type                           * rr_q,
  dss_dns_result_type_enum_type      resp_type_mask, 
  dss_dns_query_result_info        * dst_buffer,
  uint16                             num_records
)
{

  uint16                            index = 0;
  ps_dnsi_generic_rr_type         * rr_node_ptr   = NULL;
  dss_dns_record_type_enum_type     record_type   = DSS_DNS_RECORD_MIN;
  dss_dns_query_result_info       * result_buffer = NULL;
  int                               len= 0;
  struct ps_sockaddr_in             temp_in_sockaddr;
  struct ps_sockaddr_in6            temp_in6_sockaddr;
  struct ps_in_addr                 temp_in_addr;
  struct ps_in6_addr                temp_in6_addr;
  ps_dnsi_generic_srv_rdata_type  * srv_rec_ptr  = NULL;
  ps_dnsi_generic_naptr_rdata_type* naptr_rec_ptr  = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( NULL == dst_buffer || 0 == num_records || NULL == rr_q)
  {
    DNS_DSS_ASSERT(0);
    return 0;
  }

  rr_node_ptr = q_check(rr_q);
  
  while( NULL != rr_node_ptr && index < num_records)
  {
    /*-------------------------------------------------------------------------
      Read the valid matching record (record type match with  response type)
    -------------------------------------------------------------------------*/
   record_type = dss_dnsi_convert_query_into_record_type( rr_node_ptr->rr_type);
   
    if ( 0 == (record_type & resp_type_mask) || NULL == rr_node_ptr->rdata)
    {
      rr_node_ptr = q_next( rr_q, &(rr_node_ptr->link) );
      continue;
    }
    
    result_buffer = &dst_buffer[index];
    result_buffer->record_type = record_type;
    result_buffer->ttl         = rr_node_ptr->ttl;
    
    memscpy( &result_buffer->queried_fqdn.name[0], DSS_DNS_MAX_DOMAIN_NAME_LEN, 
             &rr_node_ptr->domain_name[0], DSS_DNS_MAX_DOMAIN_NAME_LEN );
    
    switch( rr_node_ptr->rr_type )
    {
      case PS_DNSI_QUERY_TYPE_A:
      {
        memset(&temp_in_sockaddr, 0, sizeof(struct ps_sockaddr_in));
        memscpy( &temp_in_addr, sizeof(struct ps_in_addr),
          rr_node_ptr->rdata, sizeof(struct ps_in_addr) );
        
        temp_in_sockaddr.ps_sin_family = DSS_AF_INET;
        //Fetch it from previous srv result (if any) 
        temp_in_sockaddr.ps_sin_port   = dss_dnsi_get_port_from_srv_res(rr_q, rr_node_ptr );
        temp_in_sockaddr.ps_sin_addr   = temp_in_addr;
        
        memscpy( &(result_buffer->result_records.a.ai_sockaddr),
          sizeof(struct ps_sockaddr_in), &temp_in_sockaddr,
          sizeof(struct ps_sockaddr_in) );
      }
      break;
      case PS_DNSI_QUERY_TYPE_AAAA:
      {
        memset(&temp_in6_sockaddr, 0, sizeof(struct ps_sockaddr_in6));

        memscpy( &temp_in6_addr, sizeof(struct ps_in6_addr),
          rr_node_ptr->rdata, sizeof(struct ps_in6_addr) );

        temp_in6_sockaddr.ps_sin6_family = DSS_AF_INET6;
        temp_in6_sockaddr.ps_sin6_port   = dss_dnsi_get_port_from_srv_res(rr_q, rr_node_ptr );
        temp_in6_sockaddr.ps_sin6_addr   = temp_in6_addr;

        memscpy( &(result_buffer->result_records.aaaa.ai_sockaddr),
          sizeof(struct ps_sockaddr_in6),
          &temp_in6_sockaddr,
          sizeof(struct ps_sockaddr_in6) );
      }
      break;

      case PS_DNSI_QUERY_TYPE_CNAME:
      {
        len = MIN( rr_node_ptr->rdata_len, DSS_DNS_MAX_DOMAIN_NAME_LEN );
        (void) strlcpy( &result_buffer->result_records.cname.ai_canonname[0],
                        (char *) rr_node_ptr->rdata,
                        len );
      }
      break;

      case PS_DNSI_QUERY_TYPE_PTR:
      {
        len = MIN( rr_node_ptr->rdata_len, DSS_DNS_MAX_DOMAIN_NAME_LEN );
        (void) strlcpy( &result_buffer->result_records.nameinfo.hostname[0],
                        (char *) rr_node_ptr->rdata,
                        len );
      }
      break;

      case PS_DNSI_QUERY_TYPE_SRV:
      {
        srv_rec_ptr = (ps_dnsi_generic_srv_rdata_type *)rr_node_ptr->rdata;
        result_buffer->result_records.srv.port_number = srv_rec_ptr->port;
        memscpy( &result_buffer->result_records.srv.fqdn.name[0],
                  DSS_DNS_MAX_DOMAIN_NAME_LEN,
                 &srv_rec_ptr->target[0], DSS_DNS_MAX_DOMAIN_NAME_LEN);
      }
      break;

      case PS_DNSI_QUERY_TYPE_NAPTR:
      {
        naptr_rec_ptr = (ps_dnsi_generic_naptr_rdata_type *)rr_node_ptr->rdata;
        result_buffer->result_records.naptr.next_query_type = (dss_dns_naptr_next_query_enum_type)
            dss_dnsi_get_query_type_from_naptr_flag( (uint8 *) &naptr_rec_ptr->flag[0]);

        memscpy( &result_buffer->result_records.naptr.service.name[0],
                  DSS_DNS_MAX_SERV_NAME_LEN,
                  &naptr_rec_ptr->service[0],
                  DSS_DNS_MAX_DOMAIN_NAME_LEN);

        memscpy( &result_buffer->result_records.naptr.resolved.fqdn.name[0],
           DSS_DNS_MAX_SERV_NAME_LEN,
           &naptr_rec_ptr->replacement[0],
           DSS_DNS_MAX_DOMAIN_NAME_LEN);
      }
      break;

      default:
        LOG_MSG_ERROR_1("Unkonwn RR type %d", rr_node_ptr->rr_type);
        break;
    } /* switch */
    
    index++;
    rr_node_ptr = q_next( rr_q, &(rr_node_ptr->link) );
  } /* for */

  return index;
}/* dss_dnsi_construct_ai_results_ext */

/*===========================================================================
FUNCTION PS_DNSI_CREATE_RESULT_CB()

DESCRIPTION
  This function creates and populates result control block.

PARAMETERS
None

RETURN VALUE
  On success, pointer to the newly created result control block
  On error, returns NULL.

DEPENDENCIES
  Caller function should free memory allocated for result control block

SIDE EFFECTS
  None.
===========================================================================*/
ps_dnsi_result_cb_type *  dss_dnsi_create_result_cb
( 
  void
)
{
  ps_dnsi_result_cb_type *    result_cb_ptr = NULL;
  dss_dns_query_handle_type   handle        = PS_HANDLE_MGR_INVALID_HANDLE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  handle = ps_handle_mgr_get_handle( RESULT_CLIENT, 0 );
  
  if( PS_HANDLE_MGR_INVALID_HANDLE == handle )
  {
    LOG_MSG_ERROR_0("Unable to allocate result control block handle %d for API CB");
    return NULL;
  }
    
  /*-------------------------------------------------------------------------
    Allocate a result control block.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(result_cb_ptr, sizeof(ps_dnsi_result_cb_type), ps_dnsi_result_cb_type *);

  if ( NULL != result_cb_ptr )
  {
    /*-------------------------------------------------------------------------
      Initialize result control block
    -------------------------------------------------------------------------*/
    memset( result_cb_ptr, 0x0, sizeof(ps_dnsi_result_cb_type));
   (void) q_link( result_cb_ptr, &(result_cb_ptr->link) );
    result_cb_ptr->handle      =  handle;
    result_cb_ptr->result_mask = DSS_DNS_RESULT_MIN;
    memset(&result_cb_ptr->answer_q, 0x0, sizeof(q_type));
    (void) q_init( &result_cb_ptr->answer_q );
  }

  return result_cb_ptr;
}/* dss_dnsi_create_result_cb */


/*===========================================================================
FUNCTION DSS_DNSI_API_CREATE_INSTANCE()

DESCRIPTION
  This function creates and populates control block for the API layer.

PARAMETERS
  api_type            - Type of query for which the API CB is being created.
  session_handle      - Session identifier.
  query_data_ptr      - Pointer to query data
  addr_family         - Address family.
  additional_info_ptr - API specific additional info
                        ADDRINFO_API: hints structure
                        NAMEINFO_API: flags parameter
  additional_info_len - Length of the additional info

RETURN VALUE
  On success, pointer to the newly created API control block
  On error, returns NULL and sets the dss_errno to the error occurred.

  dss_errno values
  ------------
  DS_ENOMEM - No memory to create the control block.
  DS_EFAULT - Invalid arguments.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static ps_dnsi_api_cb_type  * dss_dnsi_api_create_instance
(
  dss_dns_session_mgr_handle_type     session_handle,
  const char                        * query_data_ptr,
  const dss_dns_query_hints_info          * hints_info_ptr,
  int16                             * dss_errno
)
{
  ps_dnsi_api_cb_type               * api_cb_ptr = NULL;
  ps_dnsi_result_cb_type            * result_cb_ptr = NULL;
  ps_dnsi_query_type_enum_type        query_type;
  boolean                             is_numerical_query = FALSE;
  boolean                             is_v4_addr         = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Allocate a result control block .
  -------------------------------------------------------------------------*/
  result_cb_ptr = dss_dnsi_create_result_cb( );

  if ( NULL == result_cb_ptr )
  {
    *dss_errno = DS_ENOMEM;
     return NULL;
  }
  /*-------------------------------------------------------------------------
    Allocate a Control block for the API layer.
  -------------------------------------------------------------------------*/
  api_cb_ptr  = (ps_dnsi_api_cb_type *)
                ps_mem_get_buf( PS_MEM_DNSI_API_CB_TYPE );
  if( NULL == api_cb_ptr )
  {
    *dss_errno = DS_ENOMEM;
    ps_dnsi_mem_free(result_cb_ptr);
    return NULL;
  } 

  memset( api_cb_ptr, 0x0, sizeof(ps_dnsi_api_cb_type));
  api_cb_ptr->curr_query_mask     = hints_info_ptr->query_mask;
  api_cb_ptr->result_cb_ptr       = result_cb_ptr;
  api_cb_ptr->session_handle      = session_handle;
  api_cb_ptr->state               = PS_DNSI_API_CB_STATE_OPEN;
  api_cb_ptr->query_status        = QUERY_IN_PROGRESS;
  api_cb_ptr->legacy_api_info.is_present = FALSE;
  memset(&api_cb_ptr->pending_query_q, 0x0, sizeof(q_type));
  (void) q_init( &api_cb_ptr->pending_query_q );
  
  /*-------------------------------------------------------------------------
    Memcopy Hints info
  -------------------------------------------------------------------------*/
  memscpy( &api_cb_ptr->hints_info, sizeof(dss_dns_query_hints_info),
    hints_info_ptr, sizeof(dss_dns_query_hints_info));
  /*-------------------------------------------------------------------------
    Special handling: Query type is A|AAAA and query data is numerical string. In such case
    Assign query type to numerical string type
  -------------------------------------------------------------------------*/
  if( ( NULL != query_data_ptr ) && 
      (( DSS_DNS_API_QUERY_A | DSS_DNS_API_QUERY_AAAA ) == api_cb_ptr->curr_query_mask) )
  {
    is_numerical_query = dss_dnsi_validate_numerical_query (query_data_ptr , &is_v4_addr);
    
    if( is_numerical_query )
    {
      if ( TRUE == is_v4_addr)
      {
        api_cb_ptr->curr_query_mask = DSS_DNS_API_QUERY_A;
      }
      else
      {
         api_cb_ptr->curr_query_mask = DSS_DNS_API_QUERY_AAAA;
      }
    }
  }
  
  query_type = dss_dnsi_get_curr_query_type( api_cb_ptr->curr_query_mask );
  
  /*-------------------------------------------------------------------------
    Populate current query.
  -------------------------------------------------------------------------*/
  dss_dnsi_construct_queries( query_type,
                              api_cb_ptr->hints_info.ai_flags,
                              query_data_ptr,
                              &(api_cb_ptr->current_query) );
  return api_cb_ptr;
} /* dss_dnsi_api_create_instance() */

/*===========================================================================
FUNCTION DSS_DNSI_API_DELETE_INSTANCE()

DESCRIPTION
  Deletes the API control block instance specified by the given handle.

PARAMETERS
  api_cb_handle - Handle to the API control block to delete.

RETURN VALUE
  None.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dss_dnsi_api_delete_instance
(
  ps_dnsi_api_cb_type          * api_cb_ptr,
  boolean                        keep_records_cache
)
{
  ps_dnsi_generic_rr_type            * rr_node_ptr = NULL;
  ps_dnsi_pending_query_struct_type  * query_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_2("dss_dnsi_api_delete_instance API PTR %x keep_records_cache %d ",
                  api_cb_ptr, 
                  keep_records_cache);

  if( NULL == api_cb_ptr )
  {
    LOG_MSG_ERROR_0("Delete instance: API CB is NULL");
    return;
  }
  /*-------------------------------------------------------------------------
    Free pending query pointer (if any)
  -------------------------------------------------------------------------*/
  while ( NULL != (query_ptr = q_get( &api_cb_ptr->pending_query_q) ) )
  {
    ps_dnsi_mem_free( (void **) &(query_ptr) );
  }
  
  q_destroy(&api_cb_ptr->pending_query_q);


  if (FALSE == keep_records_cache && NULL != api_cb_ptr->result_cb_ptr)
  {
    while ( NULL != (rr_node_ptr = q_get( &api_cb_ptr->result_cb_ptr->answer_q ) ) )
    {
      ps_dnsi_mem_free( (void **) &(rr_node_ptr->rdata) );
      PS_MEM_FREE(rr_node_ptr );
    }
    q_destroy(&api_cb_ptr->result_cb_ptr->answer_q);
    PS_SYSTEM_HEAP_MEM_FREE(api_cb_ptr->result_cb_ptr);
  }
  else
  { 
    api_cb_ptr->result_cb_ptr = NULL;
  }
  memset(api_cb_ptr, 0, sizeof(ps_dnsi_api_cb_type));

  PS_MEM_FREE(api_cb_ptr );
} /* dss_dnsi_api_delete_instance() */

boolean dss_dnsi_notify_app_legacy
(
  dss_dns_session_mgr_handle_type     session_handle,
  ps_dnsi_api_cb_type               * api_cb_ptr,
  dss_dns_cback_f_type                app_cback_f_ptr,
  void                              * app_user_data_ptr,
  int16                               dss_errno
)
{

  ps_dnsi_generic_rr_type      * rr_node         = NULL;
  uint32                         num_records     = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Legacy APIs return only A, AAA, PTR recrods type to application hence calculate the number
   of records based on A, AAA, PTR recrods type
    In case of failure we can't calculate API type hence storing API type in legacy info
  -------------------------------------------------------------------------*/
  rr_node = q_check(&api_cb_ptr->result_cb_ptr->answer_q); 

  while ( NULL != rr_node )
  {
    if( PS_DNSI_QUERY_TYPE_A ==  rr_node->rr_type ||
        PS_DNSI_QUERY_TYPE_AAAA ==  rr_node->rr_type ||
        PS_DNSI_QUERY_TYPE_PTR ==  rr_node->rr_type )
    {
      num_records++;
    }
    rr_node = q_next(&api_cb_ptr->result_cb_ptr->answer_q, &rr_node->link);
  }

  /*-------------------------------------------------------------------------
    If Any of the earlier queries were successful, return Success to application
  -------------------------------------------------------------------------*/
  if ( 0 != num_records )
  {
    dss_errno = DSS_SUCCESS;
  }

  if ( 0 == num_records )
  {
    api_cb_ptr->query_status = QUERY_FAILED;
  }

  LOG_MSG_INFO1_5("dss_dnsi_notify_app_legacy session handle %d query handle %d "
                  "API type %d, num_records %d total count %d",
                  session_handle, api_cb_ptr->result_cb_ptr->handle, 
                  api_cb_ptr->legacy_api_info.api_type, 
                  num_records,
                  q_cnt( &api_cb_ptr->result_cb_ptr->answer_q));

  app_cback_f_ptr( session_handle,
                   api_cb_ptr->result_cb_ptr->handle,
                   api_cb_ptr->legacy_api_info.api_type,
                   num_records,
                   app_user_data_ptr,
                   dss_errno );

  return TRUE;
}/* dss_dnsi_notify_app_legacy */

boolean dss_dnsi_notify_app_ex
(
  dss_dns_session_mgr_handle_type     session_handle,
  ps_dnsi_api_cb_type               * api_cb_ptr,
  dss_dns_cback_f_type_ex             app_cback_f_ptr_ex,
  void                              * app_user_data_ptr,
  int16                               dss_errno
)
{
  uint32                         num_records     = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-------------------------------------------------------------------------
    1. Get the current count
    2. Store current result mask
    3. Notify Application
  -------------------------------------------------------------------------*/
  num_records =  q_cnt( &api_cb_ptr->result_cb_ptr->answer_q);

  /*-------------------------------------------------------------------------
    If Any of the earlier queries were successful, return Success to application so that application
    can read the earlier successful queries result. Application can rely on result_mask to know about
    successful queires 
  -------------------------------------------------------------------------*/
  if ( 0 != num_records )
  {
    dss_errno = DSS_SUCCESS;
  }

  api_cb_ptr->result_cb_ptr->result_mask = api_cb_ptr->curr_result_mask;
   
  if ( 0 == num_records )
  {
    api_cb_ptr->query_status = QUERY_FAILED;
  }

  LOG_MSG_INFO1_5("dss_dnsi_notify_app_ex session handle %d query handle %d "
                  "Result mask %d, num_records %d dss_errno %d",
                  session_handle, api_cb_ptr->result_cb_ptr->handle, 
                  api_cb_ptr->result_cb_ptr->result_mask, 
                  num_records, dss_errno);

  app_cback_f_ptr_ex( session_handle,
                      api_cb_ptr->result_cb_ptr->handle,
                      api_cb_ptr->result_cb_ptr->result_mask,
                      num_records,
                      app_user_data_ptr,
                      dss_errno );

  return TRUE;
}/* dss_dnsi_notify_app_ex */

boolean dss_dnsi_compare_naptr_order_pref
(
  ps_dnsi_generic_rr_type      * sort_node,
  ps_dnsi_generic_rr_type      * internal_node
)
{
  ps_dnsi_generic_naptr_rdata_type  *sort_naptr_rec_ptr = NULL;
  ps_dnsi_generic_naptr_rdata_type  *int_naptr_rec_ptr  = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  sort_naptr_rec_ptr = (ps_dnsi_generic_naptr_rdata_type *)sort_node->rdata;
  int_naptr_rec_ptr  = (ps_dnsi_generic_naptr_rdata_type *)internal_node->rdata;
  /*-------------------------------------------------------------------------
  Order
        A 16-bit unsigned integer specifying the order in which the NAPTR
        records MUST be processed to ensure the correct ordering of
        rules.  Low numbers are processed before high numbers.
  Preference
        A 16-bit unsigned integer that specifies the order in which NAPTR
        records with equal "order" values SHOULD be processed, low
        numbers being processed before high numbers. 
  -------------------------------------------------------------------------*/
  if ( sort_naptr_rec_ptr->order > int_naptr_rec_ptr->order )
  {
    return TRUE;
  } 
  else if ( sort_naptr_rec_ptr->order == int_naptr_rec_ptr->order &&
            sort_naptr_rec_ptr->preference > int_naptr_rec_ptr->preference)
  {
    return TRUE;
  }
  
  return FALSE;
}/* ps_dnsi_compare_nodes_order_pref */

void dss_dnsi_swap_record_node_fields
(
  ps_dnsi_generic_rr_type      * sort_node,
  ps_dnsi_generic_rr_type      * internal_node
)
{
  uint32                             temp_val;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( PS_DNSI_QUERY_TYPE_NAPTR == sort_node->rr_type )
  {
    ps_dnsi_generic_naptr_rdata_type  *sort_naptr_rec_ptr = 
        (ps_dnsi_generic_naptr_rdata_type *)sort_node->rdata;
    ps_dnsi_generic_naptr_rdata_type  *int_naptr_rec_ptr  = 
        (ps_dnsi_generic_naptr_rdata_type *)internal_node->rdata;
    ps_dnsi_generic_naptr_rdata_type   temp_naptr_rec;
  
    /*-------------------------------------------------------------------------
       Swap the content of NAPTR records
    -------------------------------------------------------------------------*/
    memscpy( &temp_naptr_rec, sizeof(ps_dnsi_generic_naptr_rdata_type), 
              sort_naptr_rec_ptr, sizeof(ps_dnsi_generic_naptr_rdata_type));
    memscpy( sort_naptr_rec_ptr, sizeof(ps_dnsi_generic_naptr_rdata_type), 
             int_naptr_rec_ptr, sizeof(ps_dnsi_generic_naptr_rdata_type));
    memscpy( int_naptr_rec_ptr, sizeof(ps_dnsi_generic_naptr_rdata_type), 
             &temp_naptr_rec, sizeof(ps_dnsi_generic_naptr_rdata_type));
  }
  if ( PS_DNSI_QUERY_TYPE_SRV == sort_node->rr_type )
  {
      ps_dnsi_generic_srv_rdata_type  *sort_srv_rec_ptr = 
          (ps_dnsi_generic_srv_rdata_type *)sort_node->rdata;
      ps_dnsi_generic_srv_rdata_type  *int_srv_rec_ptr  = 
          (ps_dnsi_generic_srv_rdata_type *)internal_node->rdata;
      ps_dnsi_generic_srv_rdata_type   temp_srv_rec;
      
      /*-------------------------------------------------------------------------
         Swap the content of SRV records
      -------------------------------------------------------------------------*/
      memscpy( &temp_srv_rec, sizeof(ps_dnsi_generic_srv_rdata_type), 
                sort_srv_rec_ptr, sizeof(ps_dnsi_generic_srv_rdata_type));
      memscpy( sort_srv_rec_ptr, sizeof(ps_dnsi_generic_srv_rdata_type), 
                int_srv_rec_ptr, sizeof(ps_dnsi_generic_srv_rdata_type));
      memscpy( int_srv_rec_ptr, sizeof(ps_dnsi_generic_srv_rdata_type), 
               &temp_srv_rec, sizeof(ps_dnsi_generic_srv_rdata_type));
  }
  /*-------------------------------------------------------------------------
    Swap ttl value
  -------------------------------------------------------------------------*/
  temp_val           = sort_node->ttl;
  sort_node->ttl     = internal_node->ttl;
  internal_node->ttl = temp_val;

  /*-------------------------------------------------------------------------
    Swap rdata_len
  -------------------------------------------------------------------------*/
  temp_val                 = sort_node->rdata_len;
  sort_node->rdata_len     = internal_node->rdata_len;
  internal_node->rdata_len = (int16)temp_val;

}/* ps_dnsi_swap_naptr_records_fields */

boolean dss_dnsi_process_naptr_result
(
  ps_dnsi_api_cb_type                     * api_cb_ptr,
  q_type                                  * answer_q
)
{
  boolean                            next_query_flag = FALSE;
  ps_dnsi_generic_rr_type          * sort_node       = NULL;
  ps_dnsi_generic_rr_type          * internal_node   = NULL;
  int16                              num_records     = 0;
  dss_dns_api_query_enum_type        remaining_query_mask = DSS_DNS_API_QUERY_MIN;
  dss_dns_api_query_enum_type        next_query_type      = DSS_DNS_API_QUERY_MIN;
  ps_dnsi_generic_naptr_rdata_type  *sort_naptr_rec_ptr   = NULL;
  dss_dns_error_enum_type            err_type             = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    1. Sort result records based on Order and Preference 
    2. Calculate next query type
        1.Next query will be FALSE when there is only NAPTR mask set into Current query mask
        2.if there is multiple mask set into current query mask 
           1. Get next query type from NAPTR result for Best record if it matches with Application 
              preference then issue that query next time
  -------------------------------------------------------------------------*/
  do
  {
  
    if( NULL == api_cb_ptr || NULL == api_cb_ptr->result_cb_ptr)
    {
      err_type        = DSS_DNS_ERROR_API_CB_INVALID;
      break;
    }

    num_records = q_cnt( answer_q );

    if( 0 == num_records)
    {
      break;
    }
    /*-------------------------------------------------------------------------
      Sort the result records.( Swap the content of the sort node and internal node)
    -------------------------------------------------------------------------*/
    sort_node = q_check(answer_q);
    
    while( NULL != sort_node)
    {
      internal_node = q_next(answer_q, &sort_node->link);
        
      while( NULL != internal_node )
      {
        if ( TRUE == dss_dnsi_compare_naptr_order_pref( sort_node, internal_node))
        {
          dss_dnsi_swap_record_node_fields( sort_node, internal_node );
        }
        internal_node = q_next(answer_q, &internal_node->link);
      }
      sort_node = q_next(answer_q, &sort_node->link);
      
    }
    /*-------------------------------------------------------------------------
      Calculate next Query
    -------------------------------------------------------------------------*/
    remaining_query_mask =  api_cb_ptr->curr_query_mask & (~DSS_DNS_RECORD_NAPTR);
    /*-------------------------------------------------------------------------
      Application requested for NAPTR query only
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_RESULT_MIN == remaining_query_mask)
    {
      break;
    }

    /*-------------------------------------------------------------------------
      Get the best record for given service
    -------------------------------------------------------------------------*/
    sort_node = q_check(answer_q);

    while( NULL != sort_node)
    {
      sort_naptr_rec_ptr = (ps_dnsi_generic_naptr_rdata_type *)sort_node->rdata;
      
       if ( 0 == strncmp( (const char *)&api_cb_ptr->hints_info.naptr_result_hints.service.name[0],
                          (const char *)&sort_naptr_rec_ptr->service[0],
                          DSS_DNS_MAX_SERV_NAME_LEN) )
       {
         LOG_MSG_INFO1_0("dss_dnsi_process_naptr_result Service match");
         break;
       }
      sort_node = q_next(answer_q, &sort_node->link);
    }
    
    /*-------------------------------------------------------------------------
      If there is no service match then check if we have to do another NAPTR query (On best/first record)
      where flag is "" and service is "". Otherwise break; we can't do next DNS query. 
    -------------------------------------------------------------------------*/
    if ( NULL == sort_node )
    {
      sort_node = q_check(answer_q);
      
      if (NULL != sort_node)
      {
        sort_naptr_rec_ptr = (ps_dnsi_generic_naptr_rdata_type *)sort_node->rdata;
        next_query_type = dss_dnsi_get_query_type_from_naptr_flag( (uint8 *)&sort_naptr_rec_ptr->flag[0] );

        /*-------------------------------------------------------------------------
          Send another NAPTR query on new FQDN recevied in repalcement field
          rest all field will remain same
      -------------------------------------------------------------------------*/
        if ( DSS_DNS_API_QUERY_NAPTR == next_query_type && 
             0 == strlen((const char *)&sort_naptr_rec_ptr->service[0]))
        {
          (void) strlcpy( ( char *)api_cb_ptr->current_query.query_data, 
                (const char *)&sort_naptr_rec_ptr->replacement[0], 
                PS_DNSI_MAX_DOMAIN_NAME_LEN );
          
          next_query_flag = TRUE;
          break;
        }
      }
      break;
    }
    
    /*-------------------------------------------------------------------------
        We are here only when given service matched in result records
        Any type (A|AAAA|SRV|NAPTR ) of query is possible Hence first fetch next query type 
        from result records and then match with application preference 
      -------------------------------------------------------------------------*/
    sort_naptr_rec_ptr = (ps_dnsi_generic_naptr_rdata_type *)sort_node->rdata;

    next_query_type = dss_dnsi_get_query_type_from_naptr_flag( (uint8 *)&sort_naptr_rec_ptr->flag[0] );

    if ( DSS_DNS_API_QUERY_MAX == next_query_type)
    {
      break;
    }
    /*-------------------------------------------------------------------------
      Send another NAPTR query on new FQDN recevied in repalcement field
      rest all field will remain same
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_API_QUERY_NAPTR == next_query_type)
    {
      (void) strlcpy( ( char *)api_cb_ptr->current_query.query_data, 
            (const char *)&sort_naptr_rec_ptr->replacement[0], 
            PS_DNSI_MAX_DOMAIN_NAME_LEN );
      
      next_query_flag = TRUE;
      break;
    }
    /*-------------------------------------------------------------------------
      Next Query type is SRV, Check the application preference.
      Set Query type and FQDN for next SRV query.
      Reset NAPTR mask from current query mask and set into result mask
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_API_QUERY_SRV == next_query_type )
    {
      if ( 0 != (api_cb_ptr->hints_info.query_mask & DSS_DNS_API_QUERY_SRV) )
      {
          (void) strlcpy( ( char *)api_cb_ptr->current_query.query_data, 
                (const char *)&sort_naptr_rec_ptr->replacement[0], 
                PS_DNSI_MAX_DOMAIN_NAME_LEN );
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_SRV;
          api_cb_ptr->curr_query_mask   = api_cb_ptr->curr_query_mask & (~DSS_DNS_API_QUERY_NAPTR);;
          next_query_flag = TRUE;
      }
      break;
    }
    /*-------------------------------------------------------------------------
      Next Query type is A|AAAA , Check the application preference.
      Set Query type and FQDN for next AAAA or A query.
      Reset NAPTR and SRV query mask from current query mask and set into result mask
    -------------------------------------------------------------------------*/
   if ( (DSS_DNS_API_QUERY_A|DSS_DNS_API_QUERY_AAAA) == next_query_type )
   {
     if ( 0 != (api_cb_ptr->hints_info.query_mask & DSS_DNS_API_QUERY_A) )
     {
       (void) strlcpy( ( char *)api_cb_ptr->current_query.query_data, 
             (const char *)&sort_naptr_rec_ptr->replacement[0], 
             PS_DNSI_MAX_DOMAIN_NAME_LEN );
       api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_A;
       api_cb_ptr->curr_query_mask   = api_cb_ptr->curr_query_mask & (~DSS_DNS_API_QUERY_NAPTR);
       api_cb_ptr->curr_query_mask   = api_cb_ptr->curr_query_mask & (~DSS_DNS_API_QUERY_SRV);
       next_query_flag = TRUE;
       break;
     }
     if ( 0 != (api_cb_ptr->hints_info.query_mask & DSS_DNS_API_QUERY_AAAA) )
     {
        (void) strlcpy( ( char *)api_cb_ptr->current_query.query_data, 
              (const char *)&sort_naptr_rec_ptr->replacement[0], 
              PS_DNSI_MAX_DOMAIN_NAME_LEN );
        api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_AAAA;
        api_cb_ptr->curr_query_mask   = api_cb_ptr->curr_query_mask & (~DSS_DNS_API_QUERY_NAPTR);
        api_cb_ptr->curr_query_mask   = api_cb_ptr->curr_query_mask & (~DSS_DNS_API_QUERY_SRV);
        next_query_flag = TRUE;
        break;
     }
     break;
   }
    
  }while(0);

  LOG_MSG_INFO1_4(" dss_dnsi_process_naptr_result next_query_flag %d err_type 0x%p"
                  " num_records %d remaining_query_mask %d ",
                  next_query_flag , err_type, num_records , remaining_query_mask);

  
  return  next_query_flag;
}/* dss_dnsi_process_naptr_result */

boolean dss_dnsi_compare_srv_priority_weight
(
  ps_dnsi_generic_rr_type      * sort_node,
  ps_dnsi_generic_rr_type      * internal_node
)
{
  ps_dnsi_generic_srv_rdata_type  *sort_srv_rec_ptr = NULL;
  ps_dnsi_generic_srv_rdata_type  *int_srv_rec_ptr  = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  sort_srv_rec_ptr = (ps_dnsi_generic_srv_rdata_type *)sort_node->rdata;
  int_srv_rec_ptr  = (ps_dnsi_generic_srv_rdata_type *)internal_node->rdata;
  /*-------------------------------------------------------------------------
    Priority
      The priority of this target host.  A client MUST attempt to
      contact the target host with the lowest-numbered priority it can
      reach; target hosts with the same priority SHOULD be tried in an
      order defined by the weight field. 
   Weight
        A server selection mechanism.  The weight field specifies a
        relative weight for entries with the same priority. Larger
        weights SHOULD be given a proportionately higher probability of
        being selected.. 
   Note: DS is expecting application to set correct service and protocol hence
   not enforcing "In the absence of a protocol" algorithm for same priority records
  -------------------------------------------------------------------------*/
  if ( sort_srv_rec_ptr->priority > int_srv_rec_ptr->priority)
  {
    return TRUE;
  } 
  else if ( sort_srv_rec_ptr->priority == int_srv_rec_ptr->priority &&
            sort_srv_rec_ptr->weight < int_srv_rec_ptr->weight)
  {
    return TRUE;
  }
  
  return FALSE;
}/*dss_dnsi_compare_srv_priority_weight */

boolean dss_dnsi_process_srv_result
(
  ps_dnsi_api_cb_type                     * api_cb_ptr,
  q_type                                  * answer_q
)
{
  boolean                              next_query_flag = FALSE;
  int16                                num_records     = 0;
  ps_dnsi_generic_rr_type            * sort_node       = NULL;
  ps_dnsi_generic_rr_type            * internal_node   = NULL;
  dss_dns_api_query_enum_type          remaining_query_mask = DSS_DNS_API_QUERY_MIN;
  ps_dnsi_generic_srv_rdata_type     * sort_srv_rec_ptr;
  ps_dnsi_pending_query_struct_type  * pending_query_ptr = NULL;
  dss_dns_error_enum_type              err_type          = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    1. Sort result records based on Order and Preference 
    2. Calculate next query type
  -------------------------------------------------------------------------*/
  do
  {
  
    if( NULL == api_cb_ptr || NULL == api_cb_ptr->result_cb_ptr)
    {
      err_type        = DSS_DNS_ERROR_API_CB_INVALID;
      break;
    }

    num_records = q_cnt( answer_q );

    if( 0 == num_records)
    {
      break;
    }

   /*-------------------------------------------------------------------------
    Sort the result records.( Swap the content of the sort node and internal node)
   -------------------------------------------------------------------------*/
   sort_node = q_check(answer_q);
  
    while( NULL != sort_node)
    {
      internal_node = q_next(answer_q, &sort_node->link);
        
      while( NULL != internal_node )
      {
        if ( TRUE == dss_dnsi_compare_srv_priority_weight( sort_node, internal_node))
        {
          dss_dnsi_swap_record_node_fields( sort_node, internal_node );
        }
        internal_node = q_next(answer_q, &internal_node->link);
      }
      sort_node = q_next(answer_q, &sort_node->link);
      
    }
    /*-------------------------------------------------------------------------
      Calculate next Query
    -------------------------------------------------------------------------*/
    remaining_query_mask =  api_cb_ptr->curr_query_mask & (~DSS_DNS_RECORD_SRV);
    /*-------------------------------------------------------------------------
      Application requested for SRV query only
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_RESULT_MIN == remaining_query_mask)
    {
      break;
    }
    /*-------------------------------------------------------------------------
      Fetch the best record and start A|AAAA query.
      A Target of "." means that the service is decidedly not available at this domain.Hence skip it
    -------------------------------------------------------------------------*/
    sort_node = q_check(answer_q);
    while( NULL != sort_node )
    {
      sort_srv_rec_ptr = (ps_dnsi_generic_srv_rdata_type *)sort_node->rdata;
      
      if( '.' == sort_srv_rec_ptr->target[0] && 1 == strlen((const char *)&sort_srv_rec_ptr->target[0]))
      { //Skip this target
        sort_node = q_next(answer_q, &sort_node->link);
      }
      break;
    }
  
    if (NULL != sort_node)
    {
      sort_srv_rec_ptr = (ps_dnsi_generic_srv_rdata_type *)sort_node->rdata;
      (void) strlcpy( ( char *)api_cb_ptr->current_query.query_data, 
            (const char *)&sort_srv_rec_ptr->target[0], 
            PS_DNSI_MAX_DOMAIN_NAME_LEN );
      api_cb_ptr->curr_query_mask   = api_cb_ptr->curr_query_mask & (~DSS_DNS_API_QUERY_SRV);  

      if ( 0 != (api_cb_ptr->hints_info.query_mask & DSS_DNS_API_QUERY_A) )
      {
        api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_A;
        next_query_flag = TRUE;
      }
      else if ( 0 != (api_cb_ptr->hints_info.query_mask & DSS_DNS_API_QUERY_AAAA) )
      {
        api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_AAAA;
        next_query_flag = TRUE;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
    if ( DSS_DNS_PROCESS_RANK_ALL == api_cb_ptr->hints_info.srv_result_hints.process_priority)
    {
      /*-------------------------------------------------------------------------
        Put remaining records into pending queue
      -------------------------------------------------------------------------*/
      sort_node = q_next(answer_q, &sort_node->link);
      while( NULL != sort_node )
      {
        sort_srv_rec_ptr = (ps_dnsi_generic_srv_rdata_type *)sort_node->rdata;
        
        if( !('.' == sort_srv_rec_ptr->target[0] && 1 == strlen((const char *)&sort_srv_rec_ptr->target[0])))
        { 
          pending_query_ptr =  ps_dnsi_mem_alloc(sizeof(ps_dnsi_pending_query_struct_type));
          if ( NULL == pending_query_ptr)
          {
            break;
          }
          memset(pending_query_ptr,0x0, sizeof(ps_dnsi_pending_query_struct_type));
          pending_query_ptr->curr_query_mask = api_cb_ptr->curr_query_mask;
          pending_query_ptr->query_class     = api_cb_ptr->current_query.query_class;
          (void) strlcpy( ( char *)pending_query_ptr->query_data, 
                (const char *)&sort_srv_rec_ptr->target[0], 
                PS_DNSI_MAX_DOMAIN_NAME_LEN );
          (void) q_link( pending_query_ptr, &(pending_query_ptr->link) );
          q_put(&api_cb_ptr->pending_query_q, &(pending_query_ptr->link));
        }
        sort_node = q_next(answer_q, &sort_node->link);
      }
    }
  }while(0);

  LOG_MSG_INFO1_4(" dss_dnsi_process_srv_result next_query_flag %d err_type 0x%p"
                  " num_records %d remaining_query_mask %d ",
                  next_query_flag , err_type, num_records , remaining_query_mask);
  
  return  next_query_flag;
}/* dss_dnsi_process_srv_result */


boolean dss_dnsi_calculate_next_query
(
  ps_dnsi_resolver_notify_event_enum_type   event,
  q_type                                  * answer_q,
  ps_dnsi_api_cb_type                     * api_cb_ptr
)
{
  boolean                        next_query_flag = FALSE;
  dss_dns_api_query_enum_type    next_query_mask = DSS_DNS_API_QUERY_MIN;
  ps_dnsi_generic_rr_type      * rr_node         = NULL;
  dss_dns_result_type_enum_type  received_result_mask = DSS_DNS_RESULT_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Previous query resolved successfully  .
    1: Calculate received result mask type
    2. Process recevied mask.
     1. Removed result mask from current query mask and add into current query mask
     2. Check if there is any pending current mask
     2.1 Yes
         Update current query mask
         Update query type in API control block for next query
         Send next DNS query
    2.2 No notify to application
  -------------------------------------------------------------------------*/
  if ( PS_DNSI_RES_NOTIFY_SUCCESS == event )
  {
      /*-------------------------------------------------------------------------
        Calculate the recevied mask from answer queue (There can be multiple results possible e.g A or A|CNAME)
      -------------------------------------------------------------------------*/
    rr_node = q_check(answer_q);
    while( NULL != rr_node )
    {
      received_result_mask |= dss_dnsi_convert_query_into_record_type( rr_node->rr_type); 
      rr_node = q_next(answer_q, &rr_node->link);
    }
    /*-------------------------------------------------------------------------
      Process recevied mask
    -------------------------------------------------------------------------*/
    switch ( (uint32)received_result_mask )
    {
      case (uint32)DSS_DNS_RESULT_NAPTR:
      {
        api_cb_ptr->curr_result_mask  |= DSS_DNS_RESULT_NAPTR;
        next_query_flag = dss_dnsi_process_naptr_result( api_cb_ptr, answer_q );
        break;
      }
      case (uint32)DSS_DNS_RESULT_SRV:
      {
        api_cb_ptr->curr_result_mask  |= DSS_DNS_RESULT_SRV;
        next_query_flag = dss_dnsi_process_srv_result( api_cb_ptr, answer_q );
        break;
      }
      case (uint32)DSS_DNS_RESULT_CNAME:
      {
        /*-------------------------------------------------------------------------
            Previous A/AAAA query results CNAME only hence send another same type query with 
            new domain name ('canonical' domain name).  query type and current query mask will
            Remain same.
            Note: FR:24894/27504: UE was not handling Recursive DNS query had an assumption, 
            Network can return only one CNAME record and only in that case DS has to do recursive query.
            In other cases where other records type(A or AAAA) are present in DNS response DS were not doing
            recursive dns query.
            Multiple CNAME records without any A or AAAA is not supported
          -------------------------------------------------------------------------*/
        api_cb_ptr->curr_result_mask         |= received_result_mask;
        rr_node = q_check(answer_q);  
        
        if( NULL != rr_node && 1 == q_cnt(answer_q) )
        {
          (void) strlcpy( api_cb_ptr->current_query.query_data, 
              (const char*)rr_node->rdata, 
              MIN ( rr_node->rdata_len, DSS_DNS_MAX_DOMAIN_NAME_LEN) );
          next_query_flag                       = TRUE;
        }
        
        break;
      }
      case (uint32)DSS_DNS_RESULT_PTR:
      {
        api_cb_ptr->curr_result_mask         |= received_result_mask;
        api_cb_ptr->curr_query_mask           = DSS_DNS_API_QUERY_MIN;
        break;
      }
      case (uint32)DSS_DNS_RESULT_A:
      case (uint32)(DSS_DNS_RESULT_A|DSS_DNS_RESULT_CNAME):
      {
        /*-------------------------------------------------------------------------
           Due to network misbehavior, there is a possibility that current query
           is of type only AAAA and result contains of type A which is not expecting
           to occur.
           Do not process further, If the current query mask is of type only AAAA. 
        -------------------------------------------------------------------------*/

        if ( api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_AAAA ||
             api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_SRV ||
             api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_NAPTR ||
             api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_PTR )
        {
          api_cb_ptr->curr_query_mask = DSS_DNS_API_QUERY_MIN;
          while( NULL != (rr_node = q_get(answer_q) ))
          {
            ps_dnsi_mem_free( (void **) &(rr_node->rdata) );
            PS_MEM_FREE(rr_node );
          }
        
          next_query_flag = FALSE;
          break;
        }
        /*-------------------------------------------------------------------------
          Next expected query is AAAA only, Otherwise it is a error.
        -------------------------------------------------------------------------*/
        next_query_mask =  api_cb_ptr->curr_query_mask & (~DSS_DNS_RESULT_A);
        api_cb_ptr->curr_result_mask         |= received_result_mask;

        if ( DSS_DNS_RESULT_AAAA == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_AAAA;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_SRV == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_SRV;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_NAPTR == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_NAPTR;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_PTR == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_PTR;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_MIN != next_query_mask)
        {
          LOG_MSG_ERROR_2("Invalid A query mask %d, result mask 0x%p",
                             api_cb_ptr->curr_query_mask, received_result_mask );
          DNS_DSS_ASSERT(0);
        }
            
        break;
      }
      case (uint32)DSS_DNS_RESULT_AAAA:
      case (uint32)(DSS_DNS_RESULT_AAAA|DSS_DNS_RESULT_CNAME):
      {
        /*-------------------------------------------------------------------------
           Due to network misbehavior, there is a possibility that current query
           is of type only A and result contains of type AAAA which is not expecting
           to occur.
           Do not process further, If the current query mask is of type only A. 
        -------------------------------------------------------------------------*/

        if ( api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_A ||
             api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_SRV ||
             api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_NAPTR ||
             api_cb_ptr->curr_query_mask ==  DSS_DNS_RESULT_PTR )
        {
          api_cb_ptr->curr_query_mask = DSS_DNS_API_QUERY_MIN;
          while( NULL != (rr_node = q_get(answer_q) ))
          {
            ps_dnsi_mem_free( (void **) &(rr_node->rdata) );
            PS_MEM_FREE(rr_node );
          }
        
          next_query_flag = FALSE;
          break;
        }
      
        /*-------------------------------------------------------------------------
          Next expected query is A only, Otherwise it is a error.
        -------------------------------------------------------------------------*/
        next_query_mask =  api_cb_ptr->curr_query_mask & (~DSS_DNS_RESULT_AAAA);
        api_cb_ptr->curr_result_mask        |= received_result_mask;

        if ( DSS_DNS_RESULT_A == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_A;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_SRV == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_SRV;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_NAPTR == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_NAPTR;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_PTR == next_query_mask)
        {
          api_cb_ptr->curr_query_mask          = next_query_mask;
          api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_PTR;
          next_query_flag                      = TRUE;
        }
        else if ( DSS_DNS_RESULT_MIN != next_query_mask)
        {
          LOG_MSG_ERROR_2("Invalid AAAA query mask %d, result mask 0x%p",
                             api_cb_ptr->curr_query_mask, received_result_mask );
          DNS_DSS_ASSERT(0);
        }
            
        break;
      }
      default:
      {
        
      }
    }
  }
  /*-------------------------------------------------------------------------
    Previous query failed  .
    Send Next AAAA query if previosuly failed query is A. Other combinations are not valid
    For othercases current query will set to MIN
  -------------------------------------------------------------------------*/
  else
  {
    if ( api_cb_ptr->curr_query_mask ==  (DSS_DNS_API_QUERY_AAAA | DSS_DNS_API_QUERY_A) )
    {
      api_cb_ptr->curr_query_mask          = DSS_DNS_API_QUERY_AAAA;
      api_cb_ptr->current_query.query_type = PS_DNSI_QUERY_TYPE_AAAA;
      next_query_flag                      = TRUE;
    }
    else
    {
      api_cb_ptr->curr_query_mask           = DSS_DNS_API_QUERY_MIN;
    }
  }
  /*-------------------------------------------------------------------------
    Copy previous query result (Answer queue)
  -------------------------------------------------------------------------*/
  if ( PS_DNSI_RES_NOTIFY_SUCCESS == event )
  {
    while( NULL != (rr_node = q_get(answer_q) ))
    {
      q_put(&api_cb_ptr->result_cb_ptr->answer_q, &(rr_node->link));
    }
  }

  return next_query_flag;
}/*dss_dnsi_calculate_next_query*/

int16  dss_dnsi_send_next_query
(
  dss_dns_session_mgr_handle_type     session_handle,
  ps_dnsi_api_cb_type               * api_cb_ptr,
  int16                             * dss_errno_ptr
)
{
  int16                                 retval = DSS_ERROR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-------------------------------------------------------------------------
    Get a resolver instance for this query.
  -------------------------------------------------------------------------*/
  api_cb_ptr->res_handle = ps_dnsi_resolver_get_instance( session_handle,
                                                          dss_errno_ptr );
  if( PS_DNSI_RESOLVER_INVALID_HANDLE == api_cb_ptr->res_handle )
  {
    LOG_MSG_ERROR_0("Can't get a resolver for this query; calling the application's callback");
    *dss_errno_ptr       = DSS_ERROR;
    return retval;
  }
  retval = ps_dnsi_resolver_start_query(
    api_cb_ptr->res_handle,
    api_cb_ptr->current_query.query_type,
    api_cb_ptr->current_query.query_class,
    api_cb_ptr->current_query.query_data,
    ps_dnsi_api_resolver_event_notify,
    (void *)api_cb_ptr,
    dss_errno_ptr );
    
  return retval;
}/* dss_dnsi_send_next_query */

int16 dss_dnsi_process_pending_query
(
  dss_dns_session_mgr_handle_type     session_handle,
  ps_dnsi_api_cb_type               * api_cb_ptr,
  int16                             * dss_errno_ptr
)
{
  ps_dnsi_pending_query_struct_type  * pending_query_ptr = NULL;
  int16                                retval = DSS_ERROR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    1. Get the first pending query pointer
    2. Copy into current query pointer
    3. Free pending query pointer
  -------------------------------------------------------------------------*/
  pending_query_ptr =  q_get(&api_cb_ptr->pending_query_q) ;

  if( NULL == pending_query_ptr)
  {
    return retval;
  }
  api_cb_ptr->curr_query_mask           = pending_query_ptr->curr_query_mask;
  api_cb_ptr->current_query.query_type  = dss_dnsi_get_curr_query_type( api_cb_ptr->curr_query_mask );
  api_cb_ptr->current_query.query_class = pending_query_ptr->query_class;
  memscpy( &api_cb_ptr->current_query.query_data[0], DSS_DNS_MAX_DOMAIN_NAME_LEN,
    &pending_query_ptr->query_data[0], DSS_DNS_MAX_DOMAIN_NAME_LEN);

  retval = dss_dnsi_send_next_query(session_handle, api_cb_ptr, dss_errno_ptr);
  
  ps_dnsi_mem_free( (void **) &(pending_query_ptr) );
  
  return retval;
}/* dss_dnsi_process_pending_query */

/*===========================================================================
FUNCTION  PS_DNSI_API_RESOLVER_EVENT_NOTIFY()

DESCRIPTION
  Callback function registered with the resolver.

PARAMETERS

  event         - Type of resolver event.
                  PS_DNSI_RES_NOTIFY_SUCCESS or PS_DNSI_RES_NOTIFY_FAILURE
  res_handle    - Handle to the resolver control block.
  user_data_ptr - Points to the API control block.
  dss_errno         - Error code in both events.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
 void ps_dnsi_api_resolver_event_notify
(
  ps_dnsi_resolver_notify_event_enum_type   event,
  ps_dnsi_resolver_handle_type              res_handle,
  q_type                                  * answer_q,
  void                                    * user_data_ptr,
  int16                                     dss_errno
)
{
  ps_dnsi_api_cb_type               * api_cb_ptr      = NULL;
  dss_dns_session_mgr_handle_type     session_handle  = 0;
  int16                               retval          = DSS_ERROR;
  boolean                             send_next_query = FALSE;
  ps_dnsi_session_mgr_cb_type*        session_ptr     = NULL;
  dss_dns_error_enum_type             err_type        = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_5("ps_dnsi_api_resolver_event_notify Resolver %d event occurred "
                  "for API 0x%p with answer Q 0x%p dss_errno %d event %d",
                  res_handle, user_data_ptr, answer_q, dss_errno, event );
  /*-------------------------------------------------------------------------
    Sanity check.
  -------------------------------------------------------------------------*/
  if( NULL == user_data_ptr ||
    ( PS_DNSI_RES_NOTIFY_SUCCESS != event &&
      PS_DNSI_RES_NOTIFY_FAILURE != event ) )
  {
    DNS_DSS_ASSERT(0);
    return;
  }
  do
  {
    if( !ps_mem_is_valid(user_data_ptr, PS_MEM_DNSI_API_CB_TYPE) )
    {
      err_type        = DSS_DNS_ERROR_API_CB_INVALID;
      break;
    }
    api_cb_ptr = (ps_dnsi_api_cb_type *) user_data_ptr;
    session_handle = api_cb_ptr->session_handle;
    session_ptr = dss_dns_get_session_mgr_from_handle( session_handle );
    
    if( NULL == session_ptr ||
        TRUE == session_ptr->session_deleted ||
        NULL == session_ptr->api_cb_ptr ||
        api_cb_ptr != session_ptr->api_cb_ptr )
    {
      err_type        = DSS_DNS_ERROR_SESSION_CB_INVALID;
      break;
    }
    if (api_cb_ptr->res_handle != res_handle)
    {
      err_type        = DSS_DNS_ERROR_RESOLVER_CB_INVALID;
      break;
    }
    /*---------------------------------------------------------------------
     Step 1: Delete resolver instance, Will create new instance if reuiqred
     Step 2: Calculate next query type based on current query mask and received result mask
     Step 3: Trigger Next DNS query based on next query type
    ---------------------------------------------------------------------*/
  
    /*----------------------------------------------------------------------
      Step 1: Delete resolver instance, Will create new instance if reuiqred
    ----------------------------------------------------------------------*/
    ps_dnsi_resolver_delete_instance( res_handle );
    api_cb_ptr->res_handle = PS_DNSI_RESOLVER_INVALID_HANDLE;
      /*----------------------------------------------------------------------
      Step 2: Calculate next query type based on current query mask and received result mask
    ----------------------------------------------------------------------*/
    send_next_query = dss_dnsi_calculate_next_query( event, answer_q, api_cb_ptr);
    /*-------------------------------------------------------------------------
       3. Send Next DNS query
    -------------------------------------------------------------------------*/
    if( TRUE == send_next_query )
    {
      retval = dss_dnsi_send_next_query(session_handle, api_cb_ptr, &dss_errno);
  
      if ( DSS_ERROR != retval || DS_EWOULDBLOCK != dss_errno )
      {
        if( PS_DNSI_RES_NOTIFY_SUCCESS == event)
        {
          dss_errno       = DSS_SUCCESS;
        }
        send_next_query = FALSE;
      }
    }

    if( FALSE == send_next_query )
    {
      /*-----------------------------------------------------------------------
          Previous recursive query has completed. Check if there is any pending query to execute.
          If it is successfully sent then return else invoke application call back
        -----------------------------------------------------------------------*/
      if ( 0 != q_cnt( &api_cb_ptr->pending_query_q))
      {
        retval = dss_dnsi_process_pending_query( session_handle, api_cb_ptr, &dss_errno );
        if ( DSS_ERROR == retval && DS_EWOULDBLOCK == dss_errno)
        {
          break;
        }
      }
      /*-----------------------------------------------------------------------
       Check the callback information from the session manager to notify
       the application.
     -----------------------------------------------------------------------*/   
      api_cb_ptr->state = PS_DNSI_API_CB_STATE_FREE;
      //Legacy session APIs
      if ( NULL != session_ptr->cback_f_ptr )
      {
        dss_dnsi_notify_app_legacy( session_handle, 
                                   api_cb_ptr, 
                                   session_ptr->cback_f_ptr,
                                   session_ptr->user_data_ptr,
                                   dss_errno);
      }
      //new Session APIs
      else if ( NULL != session_ptr->cback_f_ptr_ext )
      {
        dss_dnsi_notify_app_ex(session_handle, 
                              api_cb_ptr, 
                              session_ptr->cback_f_ptr_ext, 
                              session_ptr->user_data_ptr,
                              dss_errno);
      }
    }
    //Fallthrough to print f3 will all required arguments
  }while(0);

  LOG_MSG_INFO1_6(" ps_dnsi_api_resolver_event_notify  : Exit retval %d "
                  " dss_errno %d api_cb_ptr %d error type %d session_ptr 0x%x api_cb_ptr 0x%x", 
                   retval, dss_errno, api_cb_ptr, err_type, session_ptr, api_cb_ptr);
  return;
} /* ps_dnsi_api_resolver_event_notify() */

/*===========================================================================

                       EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION  DSS_DNS_INIT()

DESCRIPTION
  Initialize the DNS subsystem during powerup.

PARAMETERS
  None.

RETURN VALUE
  None.

DEPENDENCIES
  PS mem and Q subsystems must be initialized.

SIDE EFFECTS
  None.
===========================================================================*/
void dss_dns_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
      Initialize Pool
    -------------------------------------------------------------------------*/
  if(PS_MEM_POOL_INIT_OPT(PS_MEM_DNSI_API_CB_TYPE,
                            ps_dnsi_api_cb_buf_mem,
                            PS_DNSI_API_CB_BUF_SIZE,
                            PS_DNSI_API_CB_BUF_NUM,
                            PS_DNSI_API_CB_BUF_HIGH_WM,
                            PS_DNSI_API_CB_BUF_LOW_WM,
                            NULL,
#ifdef FEATURE_DATA_PS_MEM_DEBUG
                            (int *) ps_dnsi_api_cb_buf_hdr,
                            (int *) ps_dnsi_api_cb_buf_ptr
#else
                            NULL,
                            NULL
#endif /* FEATURE_DATA_PS_MEM_DEBUG */
                           ) == -1)
  {
    LOG_MSG_ERROR_0("Can't init the module");
  }



  if (PS_MEM_POOL_INIT_OPT(PS_MEM_DNSI_SESSION_MGR_CB_TYPE,
                           ps_dnsi_session_mgr_buf_mem,
                           PS_DNSI_SESSION_MGR_CB_BUF_SIZE,
                           PS_DNSI_SESSION_MGR_CB_BUF_NUM,
                           PS_DNSI_SESSION_MGR_CB_BUF_HIGH_WM,
                           PS_DNSI_SESSION_MGR_CB_BUF_LOW_WM,
                           NULL,
#ifdef FEATURE_DATA_PS_MEM_DEBUG
                           (int *) ps_dnsi_session_mgr_cb_buf_hdr,
                           (int *) ps_dnsi_session_mgr_cb_buf_ptr
#else
                           NULL,
                           NULL
#endif /* FEATURE_DATA_PS_MEM_DEBUG */
                          ) == -1)
  {
    LOG_MSG_ERROR_0("Can't init the module");
  }

  if(PS_MEM_POOL_INIT_OPT(PS_MEM_DNSI_SESSION_DNS_SERVERS_TYPE,
                       ps_dnsi_session_dns_servers_buf_mem,
                       PS_DNSI_SESSION_DNS_SERVERS_BUF_SIZE,
                       PS_DNSI_SESSION_DNS_SERVERS_BUF_NUM,
                       PS_DNSI_SESSION_DNS_SERVERS_BUF_HIGH_WM,
                       PS_DNSI_SESSION_DNS_SERVERS_BUF_LOW_WM,
                       NULL,
#ifdef FEATURE_DATA_PS_MEM_DEBUG
                       (int *) ps_dnsi_session_dns_servers_buf_hdr,
                       (int *) ps_dnsi_session_dns_servers_buf_ptr
#else
                       NULL,
                       NULL
#endif /* FEATURE_DATA_PS_MEM_DEBUG */
                      ) == -1)
  {
    LOG_MSG_ERROR_0("Can't init the module");
  }

  if(PS_MEM_POOL_INIT_OPT(PS_MEM_DNSI_HOSTENT_CB_TYPE,
                          ps_dnsi_hostent_buf_mem,
                          PS_DNSI_HOSTENT_CB_BUF_SIZE,
                          PS_DNSI_HOSTENT_CB_BUF_NUM,
                          PS_DNSI_HOSTENT_CB_BUF_HIGH_WM,
                          PS_DNSI_HOSTENT_CB_BUF_LOW_WM,
                          NULL,
#ifdef FEATURE_DATA_PS_MEM_DEBUG
                          (int *) ps_dnsi_hostent_cb_buf_hdr,
                          (int *) ps_dnsi_hostent_cb_buf_ptr
#else
                          NULL,
                          NULL
#endif /* FEATURE_DATA_PS_MEM_DEBUG */
                         ) == -1)
  {
    LOG_MSG_ERROR_0("Can't init the module");
  }
  
  /*-------------------------------------------------------------------------
    Initialize handle manager clients for the DNS API control block,
    session manager control block, resolver control block, and IO manager
    control block.
  -------------------------------------------------------------------------*/
  ps_handle_mgr_init_client( PS_HANDLE_MGR_CLIENT_DNS_RESULT,
                             PS_DNSI_RESULT_CB_BUF_NUM,
                             0,
                             0 );

  ps_handle_mgr_init_client( PS_HANDLE_MGR_CLIENT_DNS_SESSION_MGR,
                             DSS_DNS_MAX_SESSIONS,
                             0,
                             0 );

  ps_handle_mgr_init_client( PS_HANDLE_MGR_CLIENT_DNS_RESOLVER,
                             DSS_DNS_MAX_QUERIES,
                             0,
                             0 );

  ps_handle_mgr_init_client( PS_HANDLE_MGR_CLIENT_DNS_IO_MGR,
                             DSS_DNS_MAX_QUERIES,
                             0,
                             0 );

  /*------------------------------------------------------------------------
    Initialize the Resolver. This takes care of initialzing the IO manager
    the network manager and the cache manager.
  -------------------------------------------------------------------------*/
  dss_dns_session_mgr_init();
  ps_dnsi_resolver_init();

  return;
}/* dss_dns_init() */

/*===========================================================================
FUNCTION  DSS_DNS_GET_ADDRINFO()

DESCRIPTION
  This function starts the resolver to get a list of dss_dns_addrinfo
  structures. This is a blocking function. After the query operations are
  complete, the application would be notified using the callback registered
  with the session manager. The application must call dss_dns_read_addrinfo()
  funtion to read the results.

  Currently service names are not supported. Hence servname argument must
  be NULL or a numeric(decimal) port string.

PARAMETERS
  session_handle - Handle to a valid session control block.
                   (obtained by calling dss_dns_create_session)

  hostname       - Hostname argument
                   Can be a NULL pointer or a IPv4 dotted decimal string
                   or IPv6 colon seperated string or a valid domain name.

  servname       - Currently service names are not supported. Hence this
                   argument must be NULL or a decimal port string.

  hints          - A dss_dns_addrinfo struct pointer which directs the
                   operation of the resolver by providing input values
                   or by limiting the return information.

                   A NULL hints parameter is treated as one with
                   ai_family=DSS_AF_UNSPEC, ai_socktype=0, ai_protocol=0,
                   and ai_flags=0

                   Only the following fields of hints are processed.

                   ai_family
                     DSS_AF_UNSPEC/0 : Caller shall accept any address family
                     DSS_AF_INET     : Caller shall accept only DSS_AF_INET
                     DSS_AF_INET6    : Caller shall accept only DSS_AF_INET6

                   ai_socktype
                     0           : Caller shall accept any socktype
                     DSS_SOCK_STREAM : Caller shall accept DSS_SOCK_STREAM only
                     DSS_SOCK_DGRAM  : Caller shall accept DSS_SOCK_DGRAM only

                   ai_protocol
                     0           : Caller shall accept any protocol
                     PS_IPPROTO_TCP : Caller shall accpet PS_IPPROTO_TCP only
                     PS_IPPROTO_UDP : Caller shall accpet PS_IPPROTO_UDP only

                   ai_flags

                     AI_PASSIVE
                       If hostname argument is NULL, PS_INADDR_ANY or
                       IN6ADDR_ANY will be returned depending upon
                       supported family for the iface and ai_family
                       parameter of the hints message. Ignored if hostname
                       is non-NULL.

                     AI_CANONICAL
                       If the hostname is non-NULL attempts to find the
                       canonical name. Ignored if hostname is NULL.

                     AI_NUMERICHOST
                       If the hostname is non-NULL and contains a valid
                       V4 or V6 numeric address, that will be returned.
                       Otherwise dss_errno will be set to DS_NAMEERR.

                     AI_NUMERICSERV
                       This flag is currently ignored.

                     AI_V4MAPPED
                       If address family is DSS_AF_INET6/DSS_AF_UNSPEC and no
                       IPv6 adresses are found, all IPv4 addresses found
                       will be returned as IPv4-mapped-IPv6 address.
                       Ignored if address family is DSS_AF_INET.

                     AI_ALL
                       All addresses found will be returned IPv6 format.
                       Ignored if AI_V4MAPPED is not set.

                     AI_ADDRCONFIG
                       Addresses configured on local systems are returned.

                   A NULL hints parameter is treated as one with
                   ai_family=DSS_AF_UNSPEC, ai_socktype=0, ai_protocol=0,
                   and ai_flags=0

RETURN VALUE
  On success, returns a query handle identifying the query started.
  On error, returns DSS_DNS_QUERY_INVALID_HANDLE and sets the
  dss_errno to the error code.

  dss_errno values
  ----------------
  DS_EFAULT     - Invalid arguments
  DS_EBADF      - Invalid session handle
  DS_ENOMEM     - Out of memory
  DS_NAMERR     - Malformed query.
  DS_EWOULBLOCK - Operation would block

DEPENDENCIES
  A valid DNS session must be created prior to calling this function.

SIDE EFFECTS
  None.
===========================================================================*/
dss_dns_query_handle_type dss_dns_get_addrinfo
(
  dss_dns_session_mgr_handle_type     session_handle,
  const char                        * hostname_ptr,
  const char                        * service_ptr,
  const struct dss_dns_addrinfo     * hints_ptr,
  int16                             * dss_errno
)
{
  int16                               retval = DSS_ERROR;
  ps_dnsi_api_cb_type               * api_cb_ptr = NULL;
  ps_dnsi_session_mgr_cb_type       * session_mgr_ptr = NULL;
  uint16                              service;
  uint16                              socktype    = 0; 
  uint16                              protocol    = 0; 
  dss_dns_query_hints_info            hints_info;
  dss_dns_query_handle_type           query_handle = DSS_DNS_QUERY_INVALID_HANDLE;
  dss_dns_error_enum_type             err_type     = DSS_DNS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_5("dss_dns_get_addrinfo called with sess %d, hostname 0x%p,"
                   " hints 0x%x dss_errno 0x%x service_ptr 0x%x",
                  session_handle, hostname_ptr, hints_ptr, dss_errno, service_ptr);

  memset(&hints_info, 0x0, sizeof(hints_info));
  
  if( NULL == dss_errno )
  {
    return DSS_DNS_QUERY_INVALID_HANDLE;
  }
  do
  {
    session_mgr_ptr = dss_dns_get_session_mgr_from_handle(session_handle);
    
    if( NULL ==  session_mgr_ptr || FALSE == dss_dns_mgr_is_legacy_session(session_mgr_ptr) )
    {
       err_type        = DSS_DNS_ERROR_SESSION_CB_INVALID;
      *dss_errno = DS_EBADF;
      break;
    }
  
    if( FALSE == dss_dnsi_validate_hostname_query( hostname_ptr, hints_ptr ) )
    {
       err_type        = DSS_DNS_ERROR_INVALID_HOST_NAME;
      *dss_errno = DS_NAMEERR;
      break;
    }
  
    if( FALSE == dss_dnsi_parse_service_string(service_ptr, &service) )
    {
      err_type        = DSS_DNS_ERROR_INVALID_SERVICE_STR;
      *dss_errno = DS_EFAULT;
      break;
    }
  
    if ((NULL == service_ptr) && (NULL == hostname_ptr))
    {
       err_type        = DSS_DNS_ERROR_INVALID_HOST_NAME;
       *dss_errno = DS_EFAULT;
       break;
    }
  
    if( NULL != hints_ptr )
    {
  
      if ( DSS_AF_INET   == hints_ptr->ai_family )
      {
        hints_info.query_mask = DSS_DNS_API_QUERY_A ;
      }
      else if ( DSS_AF_INET6   == hints_ptr->ai_family )
      {
        hints_info.query_mask =  DSS_DNS_API_QUERY_AAAA;
      }
      else if ( DSS_AF_UNSPEC   == hints_ptr->ai_family )
      {
        hints_info.query_mask = DSS_DNS_API_QUERY_A | DSS_DNS_API_QUERY_AAAA;
      }
      else
      {
        *dss_errno = DS_EAFNOSUPPORT;
         err_type        = DSS_DNS_ERROR_INVALID_FAMILY_TYPE;
         break;
      }
  
      if( DSS_SOCK_DGRAM  != hints_ptr->ai_socktype &&
          DSS_SOCK_STREAM != hints_ptr->ai_socktype &&
          0           != hints_ptr->ai_socktype )
      {
        *dss_errno = DS_EOPNOTSUPP;
        err_type        = DSS_DNS_ERROR_INVALID_SOCKET_TYPE;
        break;
      }
      socktype = hints_ptr->ai_socktype;
      if( (int)PS_IPPROTO_TCP != hints_ptr->ai_protocol &&
          (int)PS_IPPROTO_UDP != hints_ptr->ai_protocol &&
          0                   != hints_ptr->ai_protocol )
      {
        *dss_errno = DS_EOPNOTSUPP;
        err_type        = DSS_DNS_ERROR_INVALID_PROTO_TYPE;
        break;
      }
      
      protocol = hints_ptr->ai_protocol;
  
      /*lint -save -e655 Reason for suppressing error 655*/
      if( 0 != ((int)hints_ptr->ai_flags & ~( (int)DSS_DNS_AI_FLAGS_PASSIVE     |
                                              (int)DSS_DNS_AI_FLAGS_CANONICAL   |
                                              (int)DSS_DNS_AI_FLAGS_NUMERICHOST |
                                              (int)DSS_DNS_AI_FLAGS_V4MAPPED    |
                                              (int)DSS_DNS_AI_FLAGS_ALL         |
                                              (int)DSS_DNS_AI_FLAGS_ADDRCONFIG  |
                                              (int)DSS_DNS_AI_FLAGS_LOCAL_QUERY )) )
      /*lint -restore Restore lint error 655*/
      {
        *dss_errno = DS_EOPNOTSUPP;
        err_type        = DSS_DNS_ERROR_INVALID_AAAA_A_MASK;
        break;
      }
      hints_info.ai_flags = hints_ptr->ai_flags;
      
    }
    else
    {
      hints_info.query_mask = DSS_DNS_API_QUERY_A | DSS_DNS_API_QUERY_AAAA;
    }
    /*-------------------------------------------------------------------------
      Create a API control block for the query
    -------------------------------------------------------------------------*/
    api_cb_ptr = dss_dnsi_api_create_instance( session_handle,
                                              hostname_ptr,
                                              &hints_info,
                                              dss_errno );
    if( NULL == api_cb_ptr )
    {
      err_type   = DSS_DNS_ERROR_API_CB_ALLOC_FAIL;
      break;
    }
    /*-------------------------------------------------------------------------
      Legacy API specific data
    -------------------------------------------------------------------------*/
    api_cb_ptr->legacy_api_info.port_num        = service;
    api_cb_ptr->legacy_api_info.sock_type       = socktype;
    api_cb_ptr->legacy_api_info.transport_proto = protocol;
    api_cb_ptr->legacy_api_info.is_present      = TRUE;
    api_cb_ptr->legacy_api_info.api_type       = DSS_DNS_API_ADDRINFO;
    /*-------------------------------------------------------------------------
      Get a resolver instance for this query.
    -------------------------------------------------------------------------*/
    api_cb_ptr->res_handle = ps_dnsi_resolver_get_instance( session_handle,
                                                            dss_errno );
    if( PS_DNSI_RESOLVER_INVALID_HANDLE == api_cb_ptr->res_handle )
    {
      err_type   = DSS_DNS_ERROR_RESOLVER_ALLOC_FAIL;
      *dss_errno = DS_ENOMEM;
      dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
      break;
    }
  
    // Clearing failed query
    if ((NULL != session_mgr_ptr->api_cb_ptr ) &&
                      (QUERY_FAILED == session_mgr_ptr->api_cb_ptr->query_status))
    {
      dss_dnsi_api_delete_instance( session_mgr_ptr->api_cb_ptr, FALSE);
      session_mgr_ptr->api_cb_ptr = NULL;
    }
    if (NULL != session_mgr_ptr->api_cb_ptr)
    {
      err_type   = DSS_DNS_ERROR_API_CB_ASSOC_FAIL;
      *dss_errno = DS_EOPNOTSUPP;
      ps_dnsi_resolver_delete_instance(api_cb_ptr->res_handle);
      dss_dnsi_api_delete_instance(api_cb_ptr, FALSE);
      break;
    }
    session_mgr_ptr->api_cb_ptr = api_cb_ptr;
  
    retval = ps_dnsi_resolver_start_query( api_cb_ptr->res_handle,
                                           api_cb_ptr->current_query.query_type,
                                           api_cb_ptr->current_query.query_class,
                                           api_cb_ptr->current_query.query_data,
                                           ps_dnsi_api_resolver_event_notify,
                                           (void *)api_cb_ptr,
                                           dss_errno );
  
    if( DSS_ERROR != retval || DS_EWOULDBLOCK != *dss_errno )
    {
      err_type   = DSS_DNS_ERROR_START_QUERY_FAIL;
      ps_dnsi_resolver_delete_instance( api_cb_ptr->res_handle );
      dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
      session_mgr_ptr->api_cb_ptr = NULL;
      break;
    }
  
    *dss_errno = DS_EWOULDBLOCK;
    return api_cb_ptr->result_cb_ptr->handle;
  
  }while(0);

  //Error case
   LOG_MSG_ERROR_5("dss_dns_get_nameinfo  failed: retval %d "
                 "dss_errno %d api_cb_ptr %d error type %d session_mgr_ptr 0x%x", 
                  retval, *dss_errno, api_cb_ptr, err_type, session_mgr_ptr);
  
  return query_handle;
} /* dss_dns_get_addrinfo() */

/*===========================================================================
FUNCTION  DSS_DNS_GET_NAMEINFO()

DESCRIPTION
  This function queries for dss_dns_nameinfo records. This function is used
  to convert a socket address to a pair of hostname and service name
  arguments. Currently service name argument is not supported.

PARAMETERS

  session_handle  - Session Identifier
  sa_ptr          - Socket address ptr
  sa_len          - Socket address length
  flags           - Flags
  dss_errno           - Error code

RETURN VALUE
  Returns a handle to the query started on success
  Returns DSS_DNS_QUERY_INVALID_HANDLE on error.

  dss_errno values
  ------------
  DS_EFAULT     - Invalid arguments
  DS_EBADF      - Invalid handle.
  DS_ENOMEM     - Out of memory
  DS_NAMERR     - Malformed query.
  DS_EWOULBLOCK - Operation would block

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
dss_dns_query_handle_type dss_dns_get_nameinfo
(
  dss_dns_session_mgr_handle_type     session_handle,
  const struct ps_sockaddr          * sa_ptr,
  uint16                              sa_len,
  uint32                              flags,
  int16                             * dss_errno
)
{
  ps_dnsi_api_cb_type           * api_cb_ptr = NULL;
  int16                           retval = DSS_ERROR;
  ps_dnsi_session_mgr_cb_type   * session_mgr_ptr = NULL;
  dss_dns_query_hints_info        hints_info;
  dss_dns_query_handle_type       query_handle = DSS_DNS_QUERY_INVALID_HANDLE;
  dss_dns_error_enum_type         err_type     = DSS_DNS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_5("dss_dns_get_nameinfo called with sess %d, sockaddr 0x%p,"
                  " flags 0x%x dss_errno %x sa_len %d",
                  session_handle, sa_ptr, flags, dss_errno, sa_len);

  if( NULL == dss_errno )
  {
    return query_handle;
  }
  do
  {
    session_mgr_ptr = dss_dns_get_session_mgr_from_handle(session_handle);
    
    if( NULL ==  session_mgr_ptr || FALSE == dss_dns_mgr_is_legacy_session(session_mgr_ptr) )
    {
      err_type        = DSS_DNS_ERROR_SESSION_CB_INVALID;
      *dss_errno = DS_EBADF;
      break;
    }
  
    if( 0 != flags )
    {
      err_type   = DSS_DNS_ERROR_INVALID_PTR_MASK;
      *dss_errno = DS_EOPNOTSUPP;
      break;
    }
  
    if( NULL == sa_ptr || 0 == sa_len )
    {
      err_type   = DSS_DNS_ERROR_INVALID_NAME_INFO_LEN;
      *dss_errno = DS_EFAULT;
      break;
    }
  
    if( !( (DSS_AF_INET == sa_ptr->ps_sa_family && 
            sizeof(struct ps_sockaddr_in) == sa_len) ||
           (DSS_AF_INET6 == sa_ptr->ps_sa_family && 
            sizeof(struct ps_sockaddr_in6) == sa_len) ) )
    {
      err_type   = DSS_DNS_ERROR_VALIDATION_FAIL;
      *dss_errno = DS_EAFNOSUPPORT;
       break;
    }
  
    memset(&hints_info, 0x0, sizeof(hints_info));
  
    hints_info.query_mask = DSS_DNS_API_QUERY_PTR;
    /*-------------------------------------------------------------------------
      Create a API control block for the query
    -------------------------------------------------------------------------*/
    api_cb_ptr = dss_dnsi_api_create_instance( session_handle,
                                              (char *) sa_ptr,
                                              &hints_info,
                                              dss_errno );
    if( NULL == api_cb_ptr )
    {
      err_type   = DSS_DNS_ERROR_API_CB_ALLOC_FAIL;
      break;
    }
    /*-------------------------------------------------------------------------
      Update API type
    -------------------------------------------------------------------------*/
    api_cb_ptr->legacy_api_info.api_type       = DSS_DNS_API_NAMEINFO;
      
    /*-------------------------------------------------------------------------
      Get a resolver instance for this query.
    -------------------------------------------------------------------------*/
    api_cb_ptr->res_handle = ps_dnsi_resolver_get_instance( session_handle,
                                                            dss_errno );
    if( PS_DNSI_RESOLVER_INVALID_HANDLE == api_cb_ptr->res_handle )
    {
      err_type   = DSS_DNS_ERROR_RESOLVER_ALLOC_FAIL;
      *dss_errno = DS_ENOMEM;
      dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
      break;
    }
  
    // Clearing failed query
    if ((NULL != session_mgr_ptr->api_cb_ptr ) &&
                      (QUERY_FAILED == session_mgr_ptr->api_cb_ptr->query_status))
    {
      dss_dnsi_api_delete_instance( session_mgr_ptr->api_cb_ptr, FALSE );
      session_mgr_ptr->api_cb_ptr = NULL;
    }
    if (NULL != session_mgr_ptr->api_cb_ptr)
    {
      err_type   = DSS_DNS_ERROR_API_CB_ASSOC_FAIL;
      *dss_errno = DS_EOPNOTSUPP;
      ps_dnsi_resolver_delete_instance(api_cb_ptr->res_handle);
      dss_dnsi_api_delete_instance(api_cb_ptr, FALSE);
      break;
    }
    session_mgr_ptr->api_cb_ptr = api_cb_ptr;
  
    retval = ps_dnsi_resolver_start_query( api_cb_ptr->res_handle,
                                           api_cb_ptr->current_query.query_type,
                                           api_cb_ptr->current_query.query_class,
                                           api_cb_ptr->current_query.query_data,
                                           ps_dnsi_api_resolver_event_notify,
                                           (void *)api_cb_ptr,
                                           dss_errno );
    if( DSS_ERROR != retval || DS_EWOULDBLOCK != *dss_errno )
    {
      err_type   = DSS_DNS_ERROR_START_QUERY_FAIL;
      ps_dnsi_resolver_delete_instance( api_cb_ptr->res_handle );
      dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
      session_mgr_ptr->api_cb_ptr = NULL;
      break;
    }
  
    *dss_errno = DS_EWOULDBLOCK;
    return api_cb_ptr->result_cb_ptr->handle;
  
  }while(0);

  //Error case
   LOG_MSG_ERROR_5("dss_dns_get_nameinfo  failed: retval %d "
                   "dss_errno %d api_cb_ptr %d error type %d session_mgr_ptr 0x%x", 
                    retval, *dss_errno, api_cb_ptr, err_type, session_mgr_ptr);
    
  return query_handle;
} /* dss_dns_get_nameinfo() */

boolean dss_dnsi_validate_query_api_params
(
  dss_dns_query_info_type                 query_info,
  const dss_dns_query_hints_info              * hints_ptr
)
{
  boolean                      is_numerical_query = FALSE;
  boolean                      is_v4_addr         = FALSE;
  dss_dns_error_enum_type      err_type           = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-------------------------------------------------------------------------
    Valid/Allowed/Supported 
  -------------------------------------------------------------------------*/
  do
  {
    if ( NULL == hints_ptr ||
         DSS_DNS_API_QUERY_MIN == hints_ptr->query_mask)
    {
      err_type = DSS_DNS_ERROR_HINT_PTR_NULL;
      break;
    }
    /*-------------------------------------------------------------------------
        .PTR type query is not allowed with other query type. .PTR type is Name info query whereas
        other are Address info query
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_API_QUERY_PTR == ( hints_ptr->query_mask & DSS_DNS_API_QUERY_PTR ) &&
        DSS_DNS_API_QUERY_PTR  != hints_ptr->query_mask )
    {
      err_type = DSS_DNS_ERROR_INVALID_PTR_MASK;
      break;
    }
    /*-------------------------------------------------------------------------
        .NAPTR query type is set with other query type. Only higest rank result recursive processing allowed
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_API_QUERY_NAPTR == (hints_ptr->query_mask & DSS_DNS_API_QUERY_NAPTR) &&
         ( DSS_DNS_API_QUERY_NAPTR != hints_ptr->query_mask))
    {
      if ( DSS_DNS_PROCESS_RANK_HIGHEST != hints_ptr->naptr_result_hints.process_order)
      {
        err_type = DSS_DNS_ERROR_INVALID_NAPTR_RANK;
        break;
      }
    }
    /*-------------------------------------------------------------------------
        .A/AAAA flags
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_API_QUERY_A == (hints_ptr->query_mask & DSS_DNS_API_QUERY_A) ||
        DSS_DNS_API_QUERY_AAAA == (hints_ptr->query_mask & DSS_DNS_API_QUERY_AAAA))
    {
      if( 0 != ((int)hints_ptr->ai_flags & ~( (int)DSS_DNS_AI_FLAGS_PASSIVE     |
                                             (int)DSS_DNS_AI_FLAGS_CANONICAL   |
                                             (int)DSS_DNS_AI_FLAGS_NUMERICHOST |
                                             (int)DSS_DNS_AI_FLAGS_V4MAPPED    |
                                             (int)DSS_DNS_AI_FLAGS_ALL         |
                                             (int)DSS_DNS_AI_FLAGS_ADDRCONFIG  |
                                             (int)DSS_DNS_AI_FLAGS_LOCAL_QUERY )) )
      {
        err_type = DSS_DNS_ERROR_INVALID_AAAA_A_MASK;
        break;
      }
    }
    /*-------------------------------------------------------------------------
        .Validate Name info 
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_API_QUERY_PTR == hints_ptr->query_mask )
    {
      if( NULL == query_info.query.nameinfo.sa_ptr || 0 == query_info.query.nameinfo.sa_len )
      {
        err_type = DSS_DNS_ERROR_INVALID_NAME_INFO;
        break;
      }
      if( !( (DSS_AF_INET == query_info.query.nameinfo.sa_ptr->ps_sa_family && 
              sizeof(struct ps_sockaddr_in) == query_info.query.nameinfo.sa_len) ||
             (DSS_AF_INET6 == query_info.query.nameinfo.sa_ptr->ps_sa_family && 
              sizeof(struct ps_sockaddr_in6) == query_info.query.nameinfo.sa_len) ) )
      {
        err_type = DSS_DNS_ERROR_INVALID_NAME_INFO_LEN;
        break;
      }
    }
    else
    {
      /*-------------------------------------------------------------------------
            .Validate Address  info 
            We are not allowing numerical query for SRV and NAPTR as we dont know the family.
            Numerical query can be  allowed for A or AAAA query
      -------------------------------------------------------------------------*/
      if ( NULL !=  query_info.query.addrinfo.hostname_ptr )
      {
        is_numerical_query =  dss_dnsi_validate_numerical_query ( 
            query_info.query.addrinfo.hostname_ptr , &is_v4_addr);

        if ( TRUE == is_numerical_query && 
           ( 0 != (hints_ptr->query_mask & (DSS_DNS_API_QUERY_SRV|DSS_DNS_API_QUERY_NAPTR))))
        {
          err_type = DSS_DNS_ERROR_INVALID_NUM_QUERY_NOT_SUPPORTED;
          break;
        }
        if ( TRUE == is_numerical_query && TRUE == is_v4_addr && 
             DSS_DNS_API_QUERY_AAAA == hints_ptr->query_mask)
        {
          err_type = DSS_DNS_ERROR_INVALID_NUM_QUERY_AAAA;
          break;
        }
        
        if ( TRUE == is_numerical_query && FALSE == is_v4_addr && 
             DSS_DNS_API_QUERY_A == hints_ptr->query_mask)
        {
          err_type = DSS_DNS_ERROR_INVALID_NUM_QUERY_A;
          break;
        }
        if ( FALSE == dss_dnsi_validate_alphanumerical_query( query_info.query.addrinfo.hostname_ptr ) )
        {
          err_type = DSS_DNS_ERROR_INVALID_HOST_NAME;
          break;
        }
      }

    }
    
    LOG_MSG_INFO1_1("dss_dnsi_validate_query_api_params validated query mask %d ",
                     hints_ptr->query_mask);
   return TRUE;
   
  }while(0);

  LOG_MSG_ERROR_2("dss_dnsi_validate_query_api_params  validated fail: query mask %d err_type %d", 
                   (NULL != hints_ptr) ? hints_ptr->query_mask : 0xFF,
                   err_type);
  return FALSE;
}/* dss_dnsi_validate_query_api_params*/

/*===========================================================================
FUNCTION  DSS_DNS_RESOLVE_DNS_QUERY()

DESCRIPTION
  This function starts the resolver to get a list of dss_dns_query_result_info
  structures. This is a blocking function. After the query operations are
  complete, the application would be notified using the callback registered
  with the session manager. The application must call dss_dns_read_addrinfo_ex()
  funtion to read the results. This API can be used for NAPTR, SRV, A and AAAA DNS
  queries 

PARAMETERS
  session_handle - Handle to a valid session control block.
                   (obtained by calling dss_dns_create_session)

  query_info - Query data, which contaon hostname or sa_ptr (Socket Address info)
  
        hostname       - Hostname argument
                       Can be a NULL pointer or a IPv4 dotted decimal string
                       or IPv6 colon seperated string or a valid domain name.
      
        sa_ptr          - Socket address ptr

  hints          - A dss_dns_hints_info struct pointer which directs the
                      operation of the resolver by providing input values
                      or by limiting the return information.
 dss_errno       - Error code in case of error.
 
RETURN VALUE
  On success, returns a query handle identifying the query started.
  On error, returns DSS_DNS_QUERY_INVALID_HANDLE and sets the
  dss_errno to the error code.

  dss_errno values
  ----------------
  DS_EFAULT     - Invalid arguments
  DS_EBADF      - Invalid session handle
  DS_ENOMEM     - Out of memory
  DS_NAMERR     - Malformed query
  DS_EWOULBLOCK - Operation would block

DEPENDENCIES
  A valid DNS session must be created prior to calling this function by .dss_dns_create_session_ex

SIDE EFFECTS
  None.

===========================================================================*/
dss_dns_query_handle_type dss_dns_resolve_query
(
  dss_dns_session_mgr_handle_type         session_handle,
  dss_dns_query_info_type                 query_info,
  const  dss_dns_query_hints_info        * hints_ptr,
  int16                                  * dss_errno
)
{
  int16                                 retval          = DSS_ERROR;
  dss_dns_query_handle_type             query_handle    = DSS_DNS_QUERY_INVALID_HANDLE;
  ps_dnsi_session_mgr_cb_type         * session_mgr_ptr = NULL;
  ps_dnsi_api_cb_type                 * api_cb_ptr      = NULL;
  dss_dns_error_enum_type               err_type        = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO1_3("dss_dns_resolve_query session handle %d, hint ptr 0x%p, dss_errno ptr 0x%x",
                    session_handle, hints_ptr, dss_errno);
  /*-------------------------------------------------------------------------
    Validate input params
    Create a API control block for the query
    Get a resolver instance for this query.
    Start DNS query
  -------------------------------------------------------------------------*/
  do
  {
    /*-------------------------------------------------------------------------
        . Validate dss error number ptr
    -------------------------------------------------------------------------*/
    if ( NULL == dss_errno )
    {
      return query_handle;
    }
    /*-------------------------------------------------------------------------
        . Validate  session id and query params
    -------------------------------------------------------------------------*/
    if ( (NULL == ( session_mgr_ptr = dss_dns_get_session_mgr_from_handle(session_handle)) )||
          (TRUE == dss_dns_mgr_is_legacy_session(session_mgr_ptr)) ||
           FALSE ==  dss_dnsi_validate_query_api_params( query_info, hints_ptr))
    {
      *dss_errno = DS_EFAULT;
       err_type = DSS_DNS_ERROR_VALIDATION_FAIL;
       break;
    }
    /*-------------------------------------------------------------------------
      Create a API control block for the query
    -------------------------------------------------------------------------*/
    if ( DSS_DNS_API_QUERY_PTR == hints_ptr->query_mask )
    {
      api_cb_ptr = dss_dnsi_api_create_instance( session_handle,
                                              (char *)query_info.query.nameinfo.sa_ptr,
                                              hints_ptr,
                                              dss_errno );
    }
    else
    {
      api_cb_ptr = dss_dnsi_api_create_instance( session_handle,
                                                (char *)query_info.query.addrinfo.hostname_ptr,
                                                hints_ptr,
                                                dss_errno );
    }
    if( NULL == api_cb_ptr )
    {
      err_type = DSS_DNS_ERROR_API_CB_ALLOC_FAIL;
      break;
    }
    /*-------------------------------------------------------------------------
      Legacy API specific data
  -------------------------------------------------------------------------*/
    api_cb_ptr->legacy_api_info.is_present      = FALSE;
    /*-------------------------------------------------------------------------
      Get a resolver instance for this query.
    -------------------------------------------------------------------------*/
    api_cb_ptr->res_handle = ps_dnsi_resolver_get_instance( session_handle,
                                                            dss_errno );
    if( PS_DNSI_RESOLVER_INVALID_HANDLE == api_cb_ptr->res_handle )
    {
      *dss_errno = DS_ENOMEM;
      err_type = DSS_DNS_ERROR_RESOLVER_ALLOC_FAIL;
      dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
      break;
    }
    /*-------------------------------------------------------------------------
      Check if previous API control block is still associated with  session control block.
      Allowed only in failure case else return error
    -------------------------------------------------------------------------*/
    if ((NULL != session_mgr_ptr->api_cb_ptr ) &&
                      (QUERY_FAILED == session_mgr_ptr->api_cb_ptr->query_status))
    {
      dss_dnsi_api_delete_instance( session_mgr_ptr->api_cb_ptr, FALSE);
      session_mgr_ptr->api_cb_ptr = NULL;
    }
    
    if (NULL != session_mgr_ptr->api_cb_ptr)
    {
      *dss_errno = DS_EOPNOTSUPP;
      err_type = DSS_DNS_ERROR_API_CB_ASSOC_FAIL;
      ps_dnsi_resolver_delete_instance(api_cb_ptr->res_handle);
      dss_dnsi_api_delete_instance(api_cb_ptr, FALSE);
      break;
    }
    /*-------------------------------------------------------------------------
      Associated new call API control block with session control block
    -------------------------------------------------------------------------*/
    session_mgr_ptr->api_cb_ptr = api_cb_ptr;
    /*-------------------------------------------------------------------------
      Start DNS query
    -------------------------------------------------------------------------*/
    retval = ps_dnsi_resolver_start_query( api_cb_ptr->res_handle,
                                           api_cb_ptr->current_query.query_type,
                                           api_cb_ptr->current_query.query_class,
                                           api_cb_ptr->current_query.query_data,
                                           ps_dnsi_api_resolver_event_notify,
                                           (void *)api_cb_ptr,
                                           dss_errno );
    
    if( DSS_ERROR != retval || DS_EWOULDBLOCK != *dss_errno )
    {
      ps_dnsi_resolver_delete_instance( api_cb_ptr->res_handle );
      dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
      err_type = DSS_DNS_ERROR_START_QUERY_FAIL;
      session_mgr_ptr->api_cb_ptr = NULL;
      break;
    }
    *dss_errno = DS_EWOULDBLOCK;
    query_handle =  api_cb_ptr->result_cb_ptr->handle;

    LOG_MSG_INFO1_3("dss_dns_resolve_query API call success %d, query_handle 0x%p, api_cb_ptr 0x%x",
                      session_handle, query_handle, api_cb_ptr);
    return query_handle;
  }while(0);

  //Error case
  LOG_MSG_ERROR_4("dss_dns_resolve_dns_query  failed: retval %d "
                  "dss_errno %d api_cb_ptr %d error type %d", 
                   retval, *dss_errno, api_cb_ptr, err_type);
      
  return query_handle;
}/* dss_dns_resolve_dns_query */


/*===========================================================================
FUNCTION  DSS_DNS_READ_ADDRINFO()

DESCRIPTION
  This function reads the dss_dns_addrinfo records which were looked up
  for earlier using dss_dns_get_addrinfo.

PARAMETERS
 session_handle -  Session Identifier
 query_handle   -  Query identifier
 results_buf    -  Results buffer
 num_records    -  Number of records
 dss_errno      -  Error code

RETURN VALUE
  DSS_SUCCESS on success and reads the results into the results buffer
  DSS_ERROR on error.

  dss_errno values
  ----------------
  DS_EBADF       - Invalid session or query handle
  DS_EWOULDBLOCK - Operation not complete yet.


DEPENDENCIES
  This function must be called after the application has been signalled
  using callback function after dss_dns_get_addrinfo.

SIDE EFFECTS
  The API control block instance, as well as the results Q are freed.
===========================================================================*/
int16 dss_dns_read_addrinfo
(
  dss_dns_session_mgr_handle_type     session_handle,
  dss_dns_query_handle_type           query_handle,
  dss_dns_addrinfo                  * results_buf,
  uint16                              num_records,
  int16                             * dss_errno
)
{
  ps_dnsi_api_cb_type              * api_cb_ptr      = NULL;
  ps_dnsi_session_mgr_cb_type      * session_mgr_ptr = NULL;
    dss_dns_error_enum_type          err_type        = DSS_DNS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_5("dss_dns_read_addrinfo called with sess %d, handle %d,"
                  " num recs %d dss_errno 0x%x results_buf 0x%x",
                  session_handle, query_handle, num_records, dss_errno, results_buf);

  if( NULL == dss_errno )
  {
    return DSS_ERROR;
  }
  
  do
  {
    session_mgr_ptr = dss_dns_get_session_mgr_from_handle( session_handle );
    if( NULL ==  session_mgr_ptr || FALSE == dss_dns_mgr_is_legacy_session(session_mgr_ptr))
    {
      err_type        = DSS_DNS_ERROR_SESSION_CB_INVALID;
      *dss_errno = DS_EBADF;
      break;
    }
    api_cb_ptr = session_mgr_ptr->api_cb_ptr;
    
    if( NULL == api_cb_ptr )
    {
      err_type        = DSS_DNS_ERROR_API_CB_INVALID;
      *dss_errno = DS_EBADF;
      break;
    }
  
    if( PS_DNSI_API_CB_STATE_FREE != api_cb_ptr->state || 
        NULL == api_cb_ptr->result_cb_ptr )
    {
       err_type        = DSS_DNS_ERROR_RESULT_CB_INVALID;
      *dss_errno = DS_EWOULDBLOCK;
       break;
    }
    if ( NULL != results_buf && 0 != num_records)
    {
      dss_dnsi_construct_ai_results( &api_cb_ptr->result_cb_ptr->answer_q,
                                     api_cb_ptr->legacy_api_info.sock_type,
                                     api_cb_ptr->legacy_api_info.transport_proto,
                                     (dss_dns_ai_flags_enum_type)api_cb_ptr->hints_info.ai_flags, 
                                     api_cb_ptr->legacy_api_info.port_num,
                                     results_buf,
                                     num_records );
    }
  
    ps_dnsi_resolver_delete_instance( api_cb_ptr->res_handle );
    dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
  
    if (NULL != session_mgr_ptr)
    {
      session_mgr_ptr->api_cb_ptr = NULL;
    }
  
    return DSS_SUCCESS;
  
  }while(0);

  LOG_MSG_ERROR_4("dss_dns_read_nameinfo err_type %d  dss_errno %d "
                "session_mgr_ptr 0x%x api_cb_ptr 0x%x", 
                 err_type, *dss_errno, session_mgr_ptr, api_cb_ptr);
  
  return DSS_ERROR;
} /* dss_dns_read_addrinfo() */

/*===========================================================================
FUNCTION  DSS_DNS_READ_NAMEINFO()

DESCRIPTION
  This function reads the dss_dns_nameinfo records which were looked up
  for earlier using dss_dns_get_nameinfo.

PARAMETERS
 session_handle -  Session Identifier
 query_handle   -  Query identifier
 results_buf    -  Results buffer
 num_records    -  Number of records
 dss_errno      -  Error code

RETURN VALUE
  DSS_SUCCESS on success and reads the results into the results buffer
  DSS_ERROR on error.

  dss_errno values
  ----------------
  DS_EBADF       - Invalid session or query handle
  DS_EWOULDBLOCK - Operation not complete yet.

DEPENDENCIES
  This function must be called after the application has been signalled
  using callback function after dss_dns_get_nameinfo.

SIDE EFFECTS
  The API control block instance, as well as the results Q are freed.
===========================================================================*/
int16 dss_dns_read_nameinfo
(
  dss_dns_session_mgr_handle_type     session_handle,
  dss_dns_query_handle_type           query_handle,
  dss_dns_nameinfo                  * results_buf,
  uint16                              num_records,
  int16                             * dss_errno
)
{
  ps_dnsi_api_cb_type              * api_cb_ptr      = NULL;
  ps_dnsi_session_mgr_cb_type      * session_mgr_ptr = NULL;
  dss_dns_error_enum_type            err_type        = DSS_DNS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_5("dss_dns_read_nameinfo called with sess %d, handle %d, "
                  "num recs %d dss_errno 0x%x results_buf 0%x",
                  session_handle, query_handle, num_records, dss_errno, results_buf);

  if( NULL == dss_errno )
  {
    return DSS_ERROR;
  }
  do
  {
    session_mgr_ptr = dss_dns_get_session_mgr_from_handle( session_handle );
    if( NULL ==  session_mgr_ptr || FALSE == dss_dns_mgr_is_legacy_session(session_mgr_ptr) )
    {
      *dss_errno = DS_EBADF;
      err_type        = DSS_DNS_ERROR_SESSION_CB_INVALID;
      break;
    }
    api_cb_ptr = session_mgr_ptr->api_cb_ptr;
    
    if( NULL == api_cb_ptr )
    {
      err_type        = DSS_DNS_ERROR_API_CB_INVALID;
      *dss_errno = DS_EBADF;
      break;
    }
  
    if( PS_DNSI_API_CB_STATE_FREE != api_cb_ptr->state || 
        NULL == api_cb_ptr->result_cb_ptr )
    {
      err_type        = DSS_DNS_ERROR_RESULT_CB_INVALID;
      *dss_errno = DS_EWOULDBLOCK;
      break;
    }

    if ( NULL != results_buf && 0 != num_records)
    {
      dss_dnsi_construct_ni_results( &api_cb_ptr->result_cb_ptr->answer_q,
                                     results_buf,
                                     num_records );
    }
  
    ps_dnsi_resolver_delete_instance( api_cb_ptr->res_handle);
    dss_dnsi_api_delete_instance( api_cb_ptr, FALSE );
  
    if (NULL != session_mgr_ptr)
    {
      session_mgr_ptr->api_cb_ptr = NULL;
    }
    return DSS_SUCCESS;
  }while(0);

  LOG_MSG_ERROR_4("dss_dns_read_nameinfo err_type %d  dss_errno %d "
                  "session_mgr_ptr 0x%x api_cb_ptr 0x%x", 
                   err_type, *dss_errno, session_mgr_ptr, api_cb_ptr);
    
  return DSS_ERROR;
} /* dss_dns_read_nameinfo() */

/*===========================================================================
FUNCTION  DSS_DNS_READ_QUERY_RESULT()

DESCRIPTION
  This function reads the dss_dns_query_result_info records which were looked up
  earlier using dss_dns_resolve_dns_query. it returns the records based
  on query mask set by the applicaiton. It retains the cache only when 
  application requests for it (keep_result_cache should set to TRUE) , In such case 
  application has to free the resource via dss_dns_free_query_results API.
  
PARAMETERS
 session_handle -  Session Identifier
 query_handle   -  Query identifier
 result_mask - Read record type mask 
 results_buf    -  Results buffer
 num_records    -  Number of records
 keep_result_cache - Retain(true) the cache otherwise free the cache after read 
 dss_errno          -  Error code

RETURN VALUE
  Number of records read and set into result buffer after successful read
  -1 on error.

  dss_errno values
  ----------------
  DS_EBADF       - Invalid session or query handle
  DS_EWOULDBLOCK - Operation not complete yet.

DEPENDENCIES
  This function must be called after the application has been signalled
  using callback function after dss_dns_resolve_dns_query.

SIDE EFFECTS
  The API control block instance, as well as the results Q are freed when keep_result_cache is FALSE
===========================================================================*/

int16 dss_dns_read_query_result
(
  dss_dns_session_mgr_handle_type     session_handle,
  dss_dns_query_handle_type           query_handle,
  dss_dns_result_type_enum_type       resp_type_mask, 
  dss_dns_query_result_info         * results_buf,
  uint16                              num_records,
  boolean                             keep_records_cache,
  int16                             * dss_errno
)
{
  int16                              num_rec         = 0;
  ps_dnsi_api_cb_type              * api_cb_ptr      = NULL;
  ps_dnsi_session_mgr_cb_type      * session_mgr_ptr = NULL;
  ps_dnsi_result_cb_type           * result_cb_ptr   = NULL;
  boolean                            result_found    = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO1_6("dss_dns_read_query_result called with sess %d, query handle %d,"
                  "num recs %d result read mask %d keep record %d dss_errno_ptr %p ",
                   session_handle, query_handle, num_records, resp_type_mask,
                   keep_records_cache, dss_errno );
  /*-------------------------------------------------------------------------
    1. There is two possible cache can present
       1 API control block (Active);
       2.In Session control block
    2. Read the result based on the mask provided by Application
    3. Free the result if cache flag is set to FALSE, otherwise moved into session cache for later read
       3.1 Application should free the cache later
  -------------------------------------------------------------------------*/
  if( NULL == dss_errno  || 0 == num_records || NULL == results_buf || 0 == resp_type_mask)
  {
    return DSS_ERROR;
  }
  session_mgr_ptr = dss_dns_get_session_mgr_from_handle( session_handle );
  if( (NULL ==  session_mgr_ptr) || (TRUE == dss_dns_mgr_is_legacy_session(session_mgr_ptr)))
  {
    LOG_MSG_ERROR_1("Session handle %d is invalid ", session_handle);
    *dss_errno = DS_EBADF;
    return DSS_ERROR;
  }
  api_cb_ptr = session_mgr_ptr->api_cb_ptr;

  /*-------------------------------------------------------------------------
     DS will look into old DNS restults stored in session cache. when query handle doesnt match
     with current query handle
    Note: Session cache doesn't run any TTL timer. It only keep record cache
  -------------------------------------------------------------------------*/
  if( NULL == api_cb_ptr || 
      NULL == api_cb_ptr->result_cb_ptr ||
      query_handle != api_cb_ptr->result_cb_ptr->handle )
  {
    /*------------------------------------------------------------------------
      Find matching result
    -------------------------------------------------------------------------*/
    result_cb_ptr = q_check(&session_mgr_ptr->passive_results_q );

    while ( NULL != result_cb_ptr )
    {
      if ( query_handle == result_cb_ptr->handle)
      {
        result_found = TRUE;
        break;
      }
      result_cb_ptr = q_next( &session_mgr_ptr->passive_results_q , &(result_cb_ptr->link) );
      
    }
    if ( (TRUE == result_found) &&
         ( 0 != ( result_cb_ptr->result_mask & resp_type_mask) ) )
    {
      num_rec = dss_dnsi_construct_ai_results_ext( &result_cb_ptr->answer_q,
                                          resp_type_mask,
                                          results_buf,
                                          num_records );
    }
  }
  else
  {
    num_rec = dss_dnsi_construct_ai_results_ext( &api_cb_ptr->result_cb_ptr->answer_q,
                                        resp_type_mask,
                                        results_buf,
                                        num_records );
    
    /*-------------------------------------------------------------------------
    Moved the result in session control block for later read
    -------------------------------------------------------------------------*/
    if ( TRUE == keep_records_cache )
    {
      q_put(&session_mgr_ptr->passive_results_q, &(api_cb_ptr->result_cb_ptr->link));
      api_cb_ptr->result_cb_ptr = NULL;
    }
    /*-------------------------------------------------------------------------
    Free resolver and API control block
    -------------------------------------------------------------------------*/
    ps_dnsi_resolver_delete_instance( api_cb_ptr->res_handle);
    dss_dnsi_api_delete_instance( api_cb_ptr, keep_records_cache );
    session_mgr_ptr->api_cb_ptr = NULL;
    
  }

  LOG_MSG_INFO2_1 ("dss_dns_read_query_result num records %d", num_rec);

  return num_rec;
}/* dss_dns_read_query_result */

/*===========================================================================
FUNCTION  DSS_DNS_CLEAR_CACHE()

DESCRIPTION
  This function clears the cache of the specified iface asynchronously.

PARAMETERS
 iface_id           -  Interface ID
 dss_errno          -  Error code

RETURN VALUE
  DSS_SUCCESS on success.
  DSS_ERROR on error.

  dss_errno values
  ----------------
  DS_EFAULT        - Invalid arguments.

DEPENDENCIES
  None.
SIDE EFFECTS
  None.
===========================================================================*/
int16 dss_dns_clear_cache
(
  dss_iface_id_type                   iface_id,
  int16                             * dss_errno
)
{
  ps_iface_type             * iface_ptr = NULL;
  dss_dns_error_enum_type     err_type      = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_2("dss_dns_clear_cache called with iface_id %d 0x%x dss_errno ", iface_id, dss_errno);
  
  if( NULL == dss_errno )
  {
    return DSS_ERROR;
  }
  do
  {
    /*-------------------------------------------------------------------------
      Determine the specific ps_iface_ptr based on the interface name and
      instance passed in.
    -------------------------------------------------------------------------*/
    if ( DSS_IFACE_INVALID_ID == iface_id )
    {
      err_type      = DSS_DNS_ERROR_INVALID_IFACE_ID;
      *dss_errno = DS_EFAULT;
      break;
    }
    // TODO: use a DSS function that will handle this transformation
    iface_ptr = ps_iface_get_handle( (ps_iface_id_type) ((iface_id & 0xFF000000) | 0x00FFFF00) );
    if (iface_ptr == NULL)
    {
      err_type      = DSS_DNS_ERROR_INVALID_IFACE_PTR;
      *dss_errno = DS_EFAULT;
      break;
    }
    return ps_iface_dns_cache_flush( iface_ptr, dss_errno );
  }while(0);
  
  LOG_MSG_ERROR_2("dss_dns_clear_cache_entry  err_type %d  dss_errno %d", err_type, *dss_errno);
  return DSS_ERROR;
}

/*===========================================================================
FUNCTION  DSS_DNS_CLEAR_CACHE_ENTRY()

DESCRIPTION
  This function clears the specified host name from the cache 
  of the specified iface asynchronously.

PARAMETERS
 iface_id           -  Interface ID
 hostname_ptr       -  Host name
 dss_errno          -  Error code

RETURN VALUE
  DSS_SUCCESS on success.
  DSS_ERROR on error.

  dss_errno values
  ----------------
  DS_NAMEERR - Invalid host name.
  DS_EFAULT  - Invalid arguments.
  DS_ENOMEM  - Out of memory.

DEPENDENCIES
  None.
SIDE EFFECTS
  None.
===========================================================================*/
int16 dss_dns_clear_cache_entry
(
  dss_iface_id_type                   iface_id,
  const char                        * hostname_ptr,
  int16                             * dss_errno
)
{
  ps_iface_type             * iface_ptr     = NULL;
  dss_dns_error_enum_type     err_type      = DSS_DNS_ERROR_MIN;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO2_2("dss_dns_clear_cache_entry called with iface_id %d dss_errno 0x%x",
                  iface_id, dss_errno);
  
  if( NULL == dss_errno )
  {
    return DSS_ERROR;
  }
  
  /*-------------------------------------------------------------------------
    Determine the specific ps_iface_ptr based on the interface name and
    instance passed in.
  -------------------------------------------------------------------------*/
  do
  {
    if ( DSS_IFACE_INVALID_ID == iface_id )
    {
      err_type      = DSS_DNS_ERROR_INVALID_IFACE_ID;
      *dss_errno = DS_EFAULT;
      break;
    }
    // TODO: use a DSS function that will handle this transformation
    iface_ptr = ps_iface_get_handle( (ps_iface_id_type) ((iface_id & 0xFF000000) | 0x00FFFF00) );
    if (iface_ptr == NULL)
    {
       err_type      = DSS_DNS_ERROR_INVALID_IFACE_PTR;
      *dss_errno = DS_EFAULT;
      break;
    }
    if( (NULL == hostname_ptr) ||
        (FALSE == dss_dnsi_validate_hostname_query( hostname_ptr, NULL ) ) 
      )
    {
      err_type      = DSS_DNS_ERROR_INVALID_HOST_NAME;
      *dss_errno = DS_NAMEERR;
      break;
    }
  
    return ps_iface_dns_cache_flush_entry( iface_ptr, hostname_ptr, dss_errno );
  }while(0);

  LOG_MSG_ERROR_2("dss_dns_clear_cache_entry err_type %d  dss_errno %d", err_type, *dss_errno);
   
  return DSS_ERROR;
}/* dss_dns_clear_cache_entry */

/*===========================================================================
FUNCTION  DSS_DNS_CLEAR_CACHE()

DESCRIPTION
  This function clears the cache of the specified query handle and session handle asynchronously.

PARAMETERS
 session_handle -  Session Identifier
 query_handle   -  Query identifier

RETURN VALUE
  DSS_SUCCESS on success.
  DSS_ERROR on error.

  dss_errno values
  ----------------
  DS_EFAULT        - Invalid arguments.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
int16 dss_dns_free_query_results
(
  dss_dns_session_mgr_handle_type     session_handle,
  dss_dns_query_handle_type           query_handle
)
{
  ps_dnsi_session_mgr_cb_type      * session_mgr_ptr = NULL;
  ps_dnsi_result_cb_type           * result_cb_ptr   = NULL;
  boolean                            result_found    = FALSE;
  ps_dnsi_generic_rr_type          * rr_node_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  session_mgr_ptr = dss_dns_get_session_mgr_from_handle( session_handle );
  
  if( NULL ==  session_mgr_ptr)
  {
    LOG_MSG_ERROR_2("Session handle %d is invalid query handle %d",
                     session_handle, query_handle);
    return DSS_ERROR;
  }
  /*------------------------------------------------------------------------
    Find matching result
  -------------------------------------------------------------------------*/
  result_cb_ptr = q_check(&session_mgr_ptr->passive_results_q );
  while ( NULL != result_cb_ptr )
  {
    if ( query_handle == result_cb_ptr->handle)
    {
      result_found = TRUE;
      break;
    }
    result_cb_ptr = q_next( &session_mgr_ptr->passive_results_q , &(result_cb_ptr->link) );
  }
  /*------------------------------------------------------------------------
    Remove  node from the list
    Free internal generic result records
    Destory answer queue
    Free result control blocke
  -------------------------------------------------------------------------*/
  if ( TRUE == result_found )
  {
    q_delete( &session_mgr_ptr->passive_results_q, &(result_cb_ptr->link) );

    while ( NULL != (rr_node_ptr = q_get( &result_cb_ptr->answer_q ) ) )
    {
      ps_dnsi_mem_free( (void **) &(rr_node_ptr->rdata) );
      PS_MEM_FREE(rr_node_ptr );
    }
    q_destroy(&result_cb_ptr->answer_q);
    ps_dnsi_mem_free(result_cb_ptr);
  }

  LOG_MSG_INFO1_3("dss_dns_free_query_results session id %d query_handle %d is found %d ",
                  session_handle, query_handle, result_found);
  return DSS_SUCCESS;
}/* dss_dns_free_query_results */

