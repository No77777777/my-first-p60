/*
#============================================================================
#  Name:
#    servreg_utils.c 
#
#  Description:
#     Utility APIs for Service Registry feature
#
# Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/

#include "stringl/stringl.h"
#include "servreg_internal.h"
#include "servreg_utils.h"
#include "servreg_localdb.h"
#include "tms_utils_msg.h"

SERVREG_NAME servreg_utils_return_string_between_delimiter(SERVREG_NAME str, uint32_t delim_start, uint32_t delim_end, SERVREG_NAME delim);
SERVREG_NAME servreg_utils_get_string(SERVREG_NAME str, uint32_t val);
SERVREG_RESULT servreg_get_relationship(SERVREG_NAME service_name, servreg_relationship_type *relation);
SERVREG_RESULT servreg_calculate_relationship_helper_root(SERVREG_NAME service_name, servreg_relationship_type *relation);
SERVREG_RESULT servreg_calculate_relationship_helper_user(SERVREG_NAME service_name, servreg_relationship_type *relation);
SERVREG_RESULT servreg_calculate_relationship(SERVREG_NAME service_name, servreg_relationship_type *relation);
SERVREG_RESULT servreg_name_check(SERVREG_NAME domain, SERVREG_NAME service);
SERVREG_NAME servreg_create_rcesn_name(SERVREG_NAME name);
SERVREG_NAME servreg_create_sync_name(SERVREG_NAME name, SERVREG_SERVICE_STATE state);
SERVREG_NAME servreg_create_ssr_name(SERVREG_NAME domain, SERVREG_SERVICE_STATE state);
SERVREG_NAME servreg_concat(SERVREG_NAME domain, SERVREG_NAME service);
int servreg_nmelen(SERVREG_NAME const name);
int servreg_nmecmp(SERVREG_NAME const name_1, SERVREG_NAME const name_2, int len);
servreg_hash_t servreg_nmehash(SERVREG_NAME const name, uint32_t size);
SERVREG_RESULT servreg_is_valid_state_change(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE new_state, SERVREG_BOOL *is_valid);

#ifdef SERVREG_TRACE_ENABLE
/* Declare the log structure */
static volatile srt_type servreg_trace;
static volatile srt_type servreg_time_trace;
#endif

/** =====================================================================
 * Function:
 *     servreg_utils_return_string_between_delimiter
 *
 * Description:
 *     It will return substring between delimiter for a given string.
 *
 * Parameters:
 *     str : String, from which , substring will be extracted
 *     delim_start : position of first delimiter
 *     delim_end : position of second delimiter
 *     delim : delimiter, which is used to divide string
 *
 * Returns:
 *     SERVREG_NULL : error case OR not able to extract substring
 *                    otherwise required substring
 *
 * Note : Caller should free the memory which will be allocated in this API
 * =====================================================================  */
SERVREG_NAME servreg_utils_return_string_between_delimiter(SERVREG_NAME str, uint32_t delim_start, uint32_t delim_end, SERVREG_NAME delim)
{
  SERVREG_NAME substring = SERVREG_NULL,  /* result will be stored in it */
               token = SERVREG_NULL,      /* temp variable to save tokens return by strtok_r*/
               rest = SERVREG_NULL,       /* temp variable to save str since strtok_r modifies passed string so we cannot pass str*/
               saveptr = SERVREG_NULL,    /* strtok_r requirement*/
               rest_f = SERVREG_NULL;     /* to free memory allocated to rest,
                                             rest will be made NULL after first iteration of strtok_r(as per strtok_r requirement)*/

  uint32_t count = 0, len = 0;

  if(SERVREG_NULL == str || SERVREG_NULL == delim || delim_start >= delim_end)
    return SERVREG_NULL;

  rest = (SERVREG_NAME)calloc(sizeof(char), SERVREG_NAME_MAX_LENGTH);
  if(SERVREG_NULL == rest)
    return SERVREG_NULL;

  rest_f = rest;
  strlcpy(rest,str,SERVREG_NAME_MAX_LENGTH);

  /* caller should free this memory */
  substring = (SERVREG_NAME)calloc(sizeof(char), SERVREG_NAME_MAX_LENGTH);
  if(SERVREG_NULL == substring)
  {
    free(rest);
    return SERVREG_NULL;
  }

  for ( count = 0 ; count < delim_end ; count++)
  {
     token = strtok_r(rest, delim, &saveptr);

     rest = SERVREG_NULL;

     if (token == SERVREG_NULL)
        break;

     if (count < delim_start)
        continue;

     strlcat(substring,token,SERVREG_NAME_MAX_LENGTH);
     len = strlcat(substring,"/",SERVREG_NAME_MAX_LENGTH);
  }

  if (len == 0 || count != delim_end)
  {
    free(rest_f);
    free(substring);
    return SERVREG_NULL;
  }

  /* to remove last '/' character*/
  substring[len-1] = '\0';

  free(rest_f);
  return substring;

}

/** =====================================================================
 * Function:
 *     servreg_utils_get_string
 *
 * Description:
 *     It will return substring based on val passed.
 *
 * Parameters:
 *     str : String, from which , substring will be extracted
 *     val : combination of servreg_string_type enum values ,
 *           Contiguous bits should be populated
 *
 * Returns:
 *     SERVREG_NULL : error case OR not able to extract substring
 *                    otherwise required substring
 *
 * Note : Caller should free the memory which will be allocated in
 *        servreg_utils_return_string_between_delimiter() API
 * =====================================================================  */
SERVREG_NAME servreg_utils_get_string(SERVREG_NAME str, uint32_t val)
{
  uint32_t delim_start = 0, delim_end = 0, val_t = 0 , flag = 0;
  val_t = val;

  while(val_t)
  {
     if(0 == (val_t & 0x1))
     {
       if(flag)
         break;
       delim_start++;
     }
     else
       flag = 1;

     val_t = val_t >> 1;
     delim_end++;
  }

  if (0 != val_t)
    return SERVREG_NULL;

  return servreg_utils_return_string_between_delimiter(str,delim_start,delim_end,SERVREG_NAME_DELIMITER);
}

/** =====================================================================
 * Function:
 *     servreg_get_relationship
 *
 * Description:
 *     It will check if relationship already exist or not,
 *     if exist then will return it in relation.
 *
 * Parameters:
 *     service_name : Service for which we need to calculate its relationship
 *                    with Local Process
 *     relation  :    Calculated Relationship will be saved in it.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_get_relationship(SERVREG_NAME service_name, servreg_relationship_type *relation)
{
  SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
  servreg_mon_node_p sr_mon_node = SERVREG_NULL;

  sr_mon_handle = servreg_get_sr_mon_handle(service_name);

  if(SERVREG_NULL != sr_mon_handle)
  {
     sr_mon_node = (servreg_mon_node_p)sr_mon_handle;
     if(RELATIONSHIP_NOT_ESTABLISHED != sr_mon_node->relationship)
     {
        *relation = sr_mon_node->relationship;
        return SERVREG_SUCCESS;
     }
  }

  return SERVREG_FAILURE;

}

/** =====================================================================
 * Function:
 *     servreg_calculate_relationship_helper_root
 *
 * Description:
 *      Helper API for servreg_calculate_relationship if we are in ROOT PD
 *
 * Parameters:
 *     service_name : Service for which we need to calculate its relationship
 *                    with Local Process
 *     relation  :    Calculated Relationship will be saved in it.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_calculate_relationship_helper_root(SERVREG_NAME service_name, servreg_relationship_type *relation)
{
  SERVREG_RESULT ret = SERVREG_FAILURE;
  SERVREG_NAME service_domain = SERVREG_NULL;
  SERVREG_NAME service_provider = SERVREG_NULL;
  SERVREG_NAME service_process = SERVREG_NULL;
  SERVREG_NAME service_soc = servreg_utils_get_string(service_name,SOC);

  if(SERVREG_NULL == service_soc)
     goto result;

  if(0 != strncmp(service_soc,servreg_get_local_info(LOCAL_SOC),SERVREG_NAME_MAX_LENGTH))
  {
     *relation = WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERENT_PARENT;
     ret = SERVREG_SUCCESS;
     goto result;
  }

  service_process = servreg_utils_get_string(service_name,SOC+DOMAIN+SUBDOMAIN);

  if(SERVREG_NULL == service_process)
     goto result;

  if(0 == strncmp(service_process,servreg_get_local_info(LOCAL_PROCESS),SERVREG_NAME_MAX_LENGTH))
  {
     *relation = WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS;
     ret = SERVREG_SUCCESS;
     goto result;
  }

  service_domain = servreg_utils_get_string(service_name,DOMAIN);

  if(SERVREG_NULL == service_domain)
     goto result;

  if(0 != strncmp(service_domain,servreg_get_local_info(LOCAL_DOMAIN),SERVREG_NAME_MAX_LENGTH))
  {
     *relation = WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERENT_PARENT;
     ret = SERVREG_SUCCESS;
  }
  else
  {
     service_provider = servreg_utils_get_string(service_name,PROVIDER);
     if(SERVREG_NULL == service_provider)
       *relation = WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS; //special case for "soc/domain/subdomain" only services like "msm/adsp/audio_pd"
     else
       *relation = WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_CHILD_PROCESS;
     ret = SERVREG_SUCCESS;
  }

result:
  if(SERVREG_NULL != service_soc)
     free(service_soc);
  if(SERVREG_NULL != service_process)
     free(service_process);
  if(SERVREG_NULL != service_domain)
     free(service_domain);
  if(SERVREG_NULL != service_provider)
     free(service_provider);

  return ret;
}

/** =====================================================================
 * Function:
 *     servreg_calculate_relationship_helper_user
 *
 * Description:
 *     Helper API for servreg_calculate_relationship if we are in USER PD
 *
 * Parameters:
 *     service_name : Service for which we need to calculate its relationship
 *                    with Local Process
 *     relation  :    Calculated Relationship will be saved in it.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_calculate_relationship_helper_user(SERVREG_NAME service_name, servreg_relationship_type *relation)
{
  SERVREG_RESULT ret = SERVREG_FAILURE;
  SERVREG_NAME service_domain = SERVREG_NULL;
  SERVREG_NAME service_subdomain = SERVREG_NULL;
  SERVREG_NAME service_process = SERVREG_NULL;
  SERVREG_NAME service_soc = servreg_utils_get_string(service_name,SOC);

  if(SERVREG_NULL == service_soc)
     goto result;

  if(0 != strncmp(service_soc,servreg_get_local_info(LOCAL_SOC),SERVREG_NAME_MAX_LENGTH))
  {
     *relation = WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERNT_PARENT;
     ret = SERVREG_SUCCESS;
     goto result;
  }

  service_process = servreg_utils_get_string(service_name,SOC+DOMAIN+SUBDOMAIN);

  if(SERVREG_NULL == service_process)
     goto result;

  if(0 == strncmp(service_process,servreg_get_local_info(LOCAL_PROCESS),SERVREG_NAME_MAX_LENGTH))
  {
     *relation = WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS;
     ret = SERVREG_SUCCESS;
     goto result;
  }

  service_domain = servreg_utils_get_string(service_name,DOMAIN);

  if(SERVREG_NULL == service_domain)
     goto result;

  if(0 != strncmp(service_domain,servreg_get_local_info(LOCAL_DOMAIN),SERVREG_NAME_MAX_LENGTH))
  {
     *relation = WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERNT_PARENT;
     ret = SERVREG_SUCCESS;
     goto result;
  }

  service_subdomain = servreg_utils_get_string(service_name,SUBDOMAIN);

  if(SERVREG_NULL == service_subdomain)
     goto result;

  if(0 == strncmp(service_subdomain,SERVREG_ROOT_PD_NAME,SERVREG_NAME_MAX_LENGTH))
     *relation = WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_OUR_PARENT;
  else
     *relation = WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_A_SIBLING_PROCESS;
  ret = SERVREG_SUCCESS;

result:
  if(SERVREG_NULL != service_soc)
     free(service_soc);
  if(SERVREG_NULL != service_process)
     free(service_process);
  if(SERVREG_NULL != service_domain)
     free(service_domain);
  if(SERVREG_NULL != service_subdomain)
     free(service_subdomain);

  return ret;
}

/** =====================================================================
 * Function:
 *     servreg_calculate_relationship
 *
 * Description:
 *     It will return relationship between service_name and local process.
 *     Please refer servreg_relationship_type enum to understand relationship types.
 *     Process indicates "soc/domain/subdomain". We will extract "Process"
 *     from servreg_name and calculate its relationship with Local "Process".
 *     Local indicates "where currently code is executing".
 *     Example : if we are executing this code in Audio PD then local process
 *               will be "msm/adsp/audio_pd" and if we are executing in modem 
 *               Root PD then local process will be "msm/modem/root_pd".
 *
 * Parameters:
 *     service_name : Service for which we need to calculate its relationship
 *                    with Local Process
 *     relation  :    Calculated Relationship will be saved in it.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_calculate_relationship(SERVREG_NAME service_name, servreg_relationship_type *relation)
{
   if(SERVREG_NULL == relation)
      return SERVREG_FAILURE;

   *relation = RELATIONSHIP_NOT_ESTABLISHED;

   if(SERVREG_NULL == service_name)
      return SERVREG_FAILURE;

   /* Search in existing list first*/
   if(SERVREG_SUCCESS == servreg_get_relationship(service_name,relation))
      return SERVREG_SUCCESS;

   if(0 == strncmp(SERVREG_ROOT_PD_NAME, servreg_get_local_info(LOCAL_SUBDOMAIN),SERVREG_NAME_MAX_LENGTH))
   {
      /* We are in Root PD*/
      return servreg_calculate_relationship_helper_root(service_name,relation);
   }
   else
   {
      /* We are in USER PD*/
      return servreg_calculate_relationship_helper_user(service_name,relation);
   }
}

/** =====================================================================
 * Function:
 *     servreg_name_check
 *
 * Description:
 *     This function checks if the given name is a valid service name or not.
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     SERVREG_SUCCESS       : If name is valid
 *     SERVREG_INVALID_PARAM : If name is invalid
 * =====================================================================  */
SERVREG_RESULT servreg_name_check(SERVREG_NAME domain, SERVREG_NAME service)
{
   SERVREG_NAME str_domain = domain, str_service = service;
   SERVREG_RESULT ret = SERVREG_INVALID_PARAM;
   uint32_t d_count = 0, s_count = 0;

   if(SERVREG_NULL != str_domain)
   {
      while(*str_domain != '\0')
      {
         if(strncmp(str_domain, "/", 1) == 0)
         {
            d_count = d_count + 1;
         }
         str_domain ++;
      }

      if(SERVREG_NULL != str_service)
      {
         while(*str_service != '\0')
         {
            if(strncmp(str_service, "/", 1) == 0)
            {
               s_count = s_count + 1;
            }
            str_service++;
         }

         if(d_count == 2 && s_count == 1)
            ret = SERVREG_SUCCESS;
      }
      else
      {
         /* domain = "soc/domain/subdomain/provider/service" or just domain = "soc/domain/subdomain" */
         if(d_count == 2 || d_count == 4)
           ret = SERVREG_SUCCESS;
      }
   }

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_create_rcesn_name
 *
 * Description:
 *     This function takes in the service name and returns the rcesn sync name
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     rcecb sync name : "sr:soc/domain/subdomain/provider/service" or just "sr:soc/domain/subdomain" 
 * =====================================================================  */
SERVREG_NAME servreg_create_rcesn_name(SERVREG_NAME name)
{
   SERVREG_NAME rcesn_name = SERVREG_NULL;
   uint32_t len = 0;

   if(SERVREG_NULL != name)
   {
      len = SERVREG_RCESN_NAME_PREFIX_LEN + servreg_nmelen(name) + 1;
      rcesn_name = (SERVREG_NAME)calloc(sizeof(char), len);

      if(SERVREG_NULL != rcesn_name)
      {
         strlcpy(rcesn_name, SERVREG_RCESN_NAME_PREFIX, len);
         strlcat(rcesn_name, name, len);
      }
   }

   return rcesn_name;
}

/** =====================================================================
 * Function:
 *     servreg_create_sync_name
 *
 * Description:
 *     This function takes in the service name and returns the rcecb sync name
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     rcecb sync name : "sr_xx:soc/domain/subdomain/provider/service" or just "sr_xx:soc/domain/subdomain" 
 *                        sr_xx : sr_up or sr_down supported for now
 * =====================================================================  */
SERVREG_NAME servreg_create_sync_name(SERVREG_NAME name, SERVREG_SERVICE_STATE state)
{
   SERVREG_NAME sync_name = SERVREG_NULL;
   uint32_t len = 0;

   if(SERVREG_NULL != name)
   {
      len = SERVREG_SYNC_NAME_PREFIX_LEN + servreg_nmelen(name) + 1;
      sync_name = (SERVREG_NAME)calloc(sizeof(char), len);

      if(SERVREG_NULL != sync_name)
      {
         if(SERVREG_SERVICE_STATE_UP == state)
         {
            strlcpy(sync_name, SERVREG_SYNC_NAME_UP_PREFIX, len);
            strlcat(sync_name, name, len);
         }
         else if(SERVREG_SERVICE_STATE_DOWN == state)
         {
            strlcpy(sync_name, SERVREG_SYNC_NAME_DOWN_PREFIX, len);
            strlcat(sync_name, name, len);
         }
         else
         {
            free(sync_name);
            sync_name = SERVREG_NULL;
         }
      }
   }

   return sync_name;
}

/** =====================================================================
 * Function:
 *     servreg_create_ssr_name
 *
 * Description:
 *     This function takes in the service name and returns the before shutdown ssr message name
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *     state : state of the service. UP or DOWN
 *
 * Returns:
 *     ssr name : "ssr:####:before_shutdown" or "ssr:####:after_powerup" #### will be the name extracted from the input param domain
 * =====================================================================  */
SERVREG_NAME servreg_create_ssr_name(SERVREG_NAME domain, SERVREG_SERVICE_STATE state)
{
   SERVREG_NAME sub_name = SERVREG_NULL, ssr_name = SERVREG_NULL, sub_name1 = SERVREG_NULL, sub_name2 = SERVREG_NULL;
   uint32_t sub_name_len = 0, len = 0;

   /* Using strstr function. As name can be domain+service or just domain */
   sub_name = strstr(domain , "/");
   sub_name1 = strstr(domain , "/");

   if(NULL != sub_name)
   {
      sub_name++;
      sub_name1++;

      while(*sub_name != '/')
      {
         sub_name_len++;
         sub_name++;
      }

      /* First create a null terminated string that has only the domain info from the soc/domain/subdomain string */
      sub_name2 = (SERVREG_NAME)calloc(sizeof(char), (sub_name_len + 1));

      if(SERVREG_NULL != sub_name2)
      {
         strlcpy(sub_name2, sub_name1, sub_name_len + 1);

         if(SERVREG_SERVICE_STATE_DOWN == state)
         {
            len = SERVREG_SSR_NAME_PREFIX_LEN + sub_name_len + SERVREG_SSR_NAME_DOWN_SUFFIX_LEN + 1;
            ssr_name = (SERVREG_NAME)calloc(sizeof(char), len);
            if(SERVREG_NULL != ssr_name)
            {
               strlcpy(ssr_name, SERVREG_SSR_NAME_PREFIX, len);
               strlcat(ssr_name, sub_name2, len);
               strlcat(ssr_name, SERVREG_SSR_NAME_DOWN_SUFFIX, len);

               free(sub_name2);
            }
         }
         else if(SERVREG_SERVICE_STATE_UP == state)
         {
            len = SERVREG_SSR_NAME_PREFIX_LEN + sub_name_len + SERVREG_SSR_NAME_UP_SUFFIX_LEN + 1;
            ssr_name = (SERVREG_NAME)calloc(sizeof(char), len);
            if(SERVREG_NULL != ssr_name)
            {
               strlcpy(ssr_name, SERVREG_SSR_NAME_PREFIX, len);
               strlcat(ssr_name, sub_name2, len);
               strlcat(ssr_name, SERVREG_SSR_NAME_UP_SUFFIX, len);

               free(sub_name2);
            }
         }
      }
   }

   return ssr_name;

}

/** =====================================================================
 * Function:
 *     servreg_concat
 *
 * Description:
 *     This functions concatenates "soc/domain/subdomain" and "provider/service"
 *     string
 *
 * Parameters:
 *     domain : "soc/domain/subdomain" info
 *     service : "provider/service" info
 *
 * Returns:
 *     concatenated name : "soc/domain/subdomain/provider/service"
 * =====================================================================  */
SERVREG_NAME servreg_concat(SERVREG_NAME domain, SERVREG_NAME service)
{
   SERVREG_NAME servreg_name = SERVREG_NULL;
   uint32_t len = 0;

   if(SERVREG_NULL != domain)
   {
      if(SERVREG_NULL != service)
      {
         len = servreg_nmelen(domain) + 1 + servreg_nmelen(service) + 1;
         servreg_name = (SERVREG_NAME)calloc(sizeof(char), len);
         if(SERVREG_NULL != servreg_name)
         {
            strlcpy(servreg_name, domain, len);
            strlcat(servreg_name, "/", len);
            strlcat(servreg_name, service, len);
         }
      }
      else
      {
         len = servreg_nmelen(domain) + 1;
         servreg_name = (SERVREG_NAME)calloc(sizeof(char), len);
         if(SERVREG_NULL != servreg_name)
         {
            strlcpy(servreg_name, domain, len);
         }
      }
   }

   return servreg_name;
}

/** =====================================================================
 * Function:
 *     servreg_nmelen
 *
 * Description:
 *     Function to find out the string length
 *
 * Parameters:
 *     name : string
 *
 * Returns:
 *     int : string length
 * =====================================================================  */
int servreg_nmelen(SERVREG_NAME const name)
{
   SERVREG_NAME s;
   for (s = name; '\0' != *s; ++s)
   /* NULL */;
   return s - name;
}

/** =====================================================================
 * Function:
 *     servreg_nmecmp
 *
 * Description:
 *     Compares two strings
 *
 * Parameters:
 *     name_1 : string 1
 *     name_2 : string 2
 *     len    : length of the string
 *
 * Returns:
 *     -1, 0 or 1
 * =====================================================================  */
int servreg_nmecmp(SERVREG_NAME const name_1, SERVREG_NAME const name_2, int len)
{
   SERVREG_NAME s1 = name_1, s2 = name_2;

   if (0 == len)
   {
      return 0;
   }

   while (len-- > 0 && *s1 == *s2)
   {
      if (0 == len || '\0' == *s1)
      {
         return 0;
      }
      s1++, s2++;
   }
   /* lexical analysis and return result (-1, 0, 1) */
   return(*s1 < *s2) ? -1 : (*s1 > *s2);
}

/** =====================================================================
 * Function:
 *     servreg_nmehash
 *
 * Description:
 *     Function calculated the hash value given a name
 *
 * Parameters:
 *     name : name given to calculate the hash value
 *
 * Returns:
 *     hash : hash value calculated for the given name
 * =====================================================================  */
servreg_hash_t servreg_nmehash(SERVREG_NAME const name, uint32_t size)
{
   SERVREG_NAME s = name;
   servreg_hash_t hash = 0;

   while ('\0' != *s)
   {
      servreg_hash_t temp;

      hash = (hash << 4) + (servreg_hash_t)(*s);
      if (0 != (temp = hash & 0xf0000000))
      {
         hash ^= (temp >> 24);
      }
      hash &= (~temp);
      s++;
   }
   return hash % size;
}

#ifdef SERVREG_TRACE_ENABLE
void srt_event(srt_event_type event, void** data)
{
   srt_data_type *trace = (srt_data_type *)&servreg_trace.events[servreg_trace.index];
   trace->event = event;

   //currently data is binary (0 or 2) if we need more variety add a param count to the macro
   if (data != NULL)
   {
      trace->data0 = data[0];
      trace->data1 = data[1];
   }
   else
   {
      trace->data0 = 0;
      trace->data1 = 0;
   }

  /* Go to the next index */
  servreg_trace.index++;
  if (servreg_trace.index >= SRT_LENGTH)
  {
    servreg_trace.index = 0;
  }

}

void srt_time_event(srt_event_type event, void** data)
{
   srt_data_type *trace = (srt_data_type *)&servreg_time_trace.events[servreg_time_trace.index];
   trace->event = event;

   //currently data is binary (0 or 2) if we need more variety add a param count to the macro
   if (data != NULL)
   {
      trace->data0 = data[0];
      trace->data1 = data[1];
   }
   else
   {
      trace->data0 = 0;
      trace->data1 = 0;
   }

  /* Go to the next index */
  servreg_time_trace.index++;
  if (servreg_time_trace.index >= SRT_LENGTH)
  {
    servreg_time_trace.index = 0;
  }

}

/*
void *srt_q6_return_address()
{
  int return_addr;
  __asm__ ("{r1=memw(fp+#4); r2=framekey}\n\t"
           "%0=xor(r1, r2)\n"
  :"=r"(return_addr): : "r1", "r2"
  );
  return (void *)return_addr;
}
*/

#endif //SERVREG_TRACE_ENABLE

/** =====================================================================
 * Function:
 *     servreg_is_valid_state_change
 *
 * Description:
 *     This functions check if state change is valid or not
 *
 * Parameters:
 *     Incoming : sr_mon_handle
 *     Incoming : new state
 *     Outgoing : is_valid (Return TRUE if state change is valid else FALSE)
 *
 * Return:
 *     if parameters passed to API are valid then SERVREG_SUCCESS else SERVREG_FAILURE
 *     Outgoing parameter: is_valid (Return TRUE if state change is valid else FALSE)
 * =====================================================================  */
SERVREG_RESULT servreg_is_valid_state_change(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE new_state, SERVREG_BOOL *is_valid)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_SERVICE_STATE curr_state = SERVREG_SERVICE_STATE_UNINIT;
   SERVREG_RESULT ret = SERVREG_SUCCESS;

   if(SERVREG_NULL == is_valid)
      return SERVREG_FAILURE;

   *is_valid = SERVREG_FALSE;

   sr_mon_node = (servreg_mon_node_p)sr_mon_handle;

   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
      return SERVREG_FAILURE;

   curr_state = (SERVREG_SERVICE_STATE)rcesn_getstatecurr_handle(sr_mon_node->rcesn_handle);

   switch(curr_state)
   {
      case SERVREG_SERVICE_STATE_UNINIT:
      {
         if(new_state == SERVREG_SERVICE_STATE_UP  ||  new_state == SERVREG_SERVICE_STATE_DOWN  ||  new_state == SERVREG_SERVICE_STATE_EARLY_DOWN)
         {
            *is_valid = SERVREG_TRUE;
         }
      }
      break;
      case SERVREG_SERVICE_STATE_UP :
      {
         if(new_state == SERVREG_SERVICE_STATE_DOWN ||  new_state == SERVREG_SERVICE_STATE_EARLY_DOWN)
         {
            *is_valid = SERVREG_TRUE;
         }
      }
      break;
      case SERVREG_SERVICE_STATE_DOWN :
      {
         if(new_state == SERVREG_SERVICE_STATE_UP)
         {
            *is_valid = SERVREG_TRUE;
         }
      }
      break;
      case SERVREG_SERVICE_STATE_EARLY_DOWN :
      {
         if(new_state == SERVREG_SERVICE_STATE_DOWN)
         {
            *is_valid = SERVREG_TRUE;
         }
      }
      break;
      default:
      {
         ret = SERVREG_FAILURE;
      }
   }

   if(SERVREG_FALSE == *is_valid)
      SERVREG_TRACE_2(INVALID_STATE_TRANS, sr_mon_node, new_state);

   return ret;
}