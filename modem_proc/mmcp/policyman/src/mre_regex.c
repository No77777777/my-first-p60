/**
  @file mre_regex.c

  @brief  Code for regular expressions in policyman
*/

/*
    Copyright (c) 2013-2016 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/mre_regex.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre.h"
#include "mre_regex.h"
#include "mre_util.h"


/*-------- regex_elem_data_t --------*/
/**
@brief  The data type in which data for a particular regex element is stored.
        This is a union because we have to have a consistent data size to
        store in the mre_set_t that is the regex.
*/
typedef union
{
  char       literal;
  mre_set_t *pGroup;
} regex_elem_data_t;


/*-------- pfnRegexElemMatch_t --------*/
/**
@brief  Signature of match functions.
*/
typedef boolean (*regex_match_fn_t)(
  regex_elem_data_t *pData,
  char               c
);


/*-------- regex_elem_t --------*/
/**
@brief  An element of a regex consists of the function to use for matching
        and the data to be used by the match function.
*/
typedef struct
{
  regex_match_fn_t  pfnMatch;
  regex_elem_data_t data;
} regex_elem_t;


/*-----------------------------------------------------------------------------
  Utility functions to read the regex string.
-----------------------------------------------------------------------------*/

/*-------- regex_peek_char --------*/
static char regex_peek_char(
  char const    **ppStr
)
{
  return **ppStr;
}


/*-------- regex_advance_string_pointer --------*/
static void regex_advance_string_pointer(
  char const    **ppStr
)
{
  ++(*ppStr);
}


/*-------- regex_get_char --------*/
static char regex_get_char(
  char const    **ppStr
)
{
  char  c;

  c = regex_peek_char(ppStr);
  regex_advance_string_pointer(ppStr);

  return c;
}


/*-----------------------------------------------------------------------------
  Functions for "any" matches.
-----------------------------------------------------------------------------*/

/*-------- regex_match_any --------*/
static boolean regex_match_any(
  regex_elem_data_t *pData,
  char               c
)
{
  /*  'any' always matches whatever is there...
   */
  return TRUE;
}


/*-------- regex_add_any --------*/
static boolean regex_add_any(
  regex_elem_t   *pElem,
  char const    **ppStr
)
{
  pElem->pfnMatch = regex_match_any;
  return TRUE;
}


/*-----------------------------------------------------------------------------
  Functions for "group" matches.
-----------------------------------------------------------------------------*/

/*-------- regex_match_group --------*/
static boolean regex_match_group(
  regex_elem_data_t *pData,
  char               c
)
{
  return mre_set_contains(pData->pGroup, &c);
}


/*-------- regex_group_add_literal --------*/
static boolean regex_group_add_literal(
  regex_elem_t  *pElem,
  char           c
)
{
  if (NULL == pElem->data.pGroup)
  {
    pElem->data.pGroup = mre_set_new( sizeof(char),
                                      10,
                                      256,
                                      NULL,
                                      NULL );
  }

  mre_set_add(pElem->data.pGroup, &c);

  return TRUE;
}


/*-------- regex_is_possible_range --------*/
static boolean regex_is_possible_range(
  char const    **ppStr
)
{
  return regex_peek_char(ppStr) == '-';
}


/*-------- regex_add_range_to_group --------*/
static void regex_add_range_to_group(
  regex_elem_t *pElem,
  char          start,
  char          end
)
{
  char  c;

  for (c = start ; c <= end ; ++c)
  {
    regex_group_add_literal(pElem, c);
  }
}


/*-------- regex_group_parse_range --------*/
static boolean regex_group_parse_range(
  regex_elem_t   *pElem,
  char            start,
  char const    **ppStr
)
{
  char  end;

  regex_advance_string_pointer(ppStr);  // advance past the '-'

  end= regex_get_char(ppStr);  
  if (']' == end || '\0' == end)
  {
    return FALSE;
  }

  if ('\\' == end)
  {
    end = regex_get_char(ppStr);
  }

  regex_add_range_to_group(pElem, start, end);

  return TRUE;
}


/*-------- regex_parse_group --------*/
static boolean regex_parse_group(
  regex_elem_t   *pElem,
  char const    **ppStr
)
{
  boolean    succeeded = TRUE;
  char       c;

  pElem->pfnMatch = regex_match_group;

  for ( c = regex_get_char(ppStr) ;
        succeeded && c != '\0' && c != ']' ;
        c = regex_get_char(ppStr)
      )
  {
    if ('\\' == c)
    {
      c = regex_get_char(ppStr);
    }

    if (regex_is_possible_range(ppStr))
    {
      succeeded = regex_group_parse_range(pElem, c, ppStr);
    }
    else
    {
      succeeded = regex_group_add_literal(pElem, c);
    }
  }

  return succeeded && c == ']';
}


/*-------- regex_add_digit --------*/
static boolean regex_add_digit(
  regex_elem_t   *pElem
)
{
  pElem->pfnMatch = regex_match_group;
  regex_add_range_to_group(pElem, '0', '9');
  
  return TRUE;
}


/*-----------------------------------------------------------------------------
  Functions for "literal" matches.
-----------------------------------------------------------------------------*/

/*-------- regex_match_literal --------*/
static boolean regex_match_literal(
  regex_elem_data_t *pData,
  char               c
)
{
  return pData->literal == c;
}


/*-------- regex_add_literal --------*/
static boolean regex_add_literal(
  regex_elem_t   *pElem,
  char            c
)
{
  pElem->pfnMatch = regex_match_literal;
  pElem->data.literal = c;

  return TRUE;
}


/*-----------------------------------------------------------------------------
  Functions required for the set.
-----------------------------------------------------------------------------*/

/*-------- regex_elem_compare --------*/
static boolean regex_elem_compare(
  void const *e1,
  void const *e2
)
{
  /*  Always return FALSE, so that mre_set_add will add duplicate elements.
   */
  return FALSE;
}


/*-------- regex_elem_dtor --------*/
static void regex_elem_dtor(
  void  *pe
)
{
  regex_elem_t  *pElem = (regex_elem_t *) pe;

  if (pElem->pfnMatch == regex_match_group)
  {
    MRE_RELEASE_IF(pElem->data.pGroup);
  }
}



/*-----------------------------------------------------------------------------
  Parse a string into a regex.
-----------------------------------------------------------------------------*/

/*-------- mre_regex_create --------*/
mre_regex_t * mre_regex_create(
  char const  *pStr
)
{
  boolean           parseSucceeded;
  regex_elem_t      elem;
  mre_regex_t      *pRegEx;
  char              c;

  pRegEx = mre_set_new( sizeof(regex_elem_t),
                        12,
                        MRE_SET_NO_MAX_SIZE,
                        regex_elem_compare,
                        regex_elem_dtor );

  for (c = regex_get_char(&pStr) ; c != '\0' ; c = regex_get_char(&pStr))
  {
    memset(&elem, 0, sizeof(elem));
    
    switch (c)
    {
      case '.':
        parseSucceeded = regex_add_any(&elem, &pStr);
        break;

      case '[':
        parseSucceeded = regex_parse_group(&elem, &pStr);
        break;

      case '\\':
        c = regex_get_char(&pStr);
        if ('d' == c)
        {
          parseSucceeded = regex_add_digit(&elem);
        }
        else
        {
          parseSucceeded = regex_add_literal(&elem, c);
        }
        break;

      default:
        parseSucceeded = regex_add_literal(&elem, c);
        break;
    }

    if (parseSucceeded == FALSE)
    {
      ref_cnt_obj_release(pRegEx);
      pRegEx = NULL;
      break;
    }

    mre_set_add(pRegEx, &elem);
  }

  return pRegEx;
}



/*-----------------------------------------------------------------------------
  Match a string to the regex.
-----------------------------------------------------------------------------*/


/*-------- regex_do_match --------*/
static boolean regex_do_match(
  void  *e,
  void  *d1
)
{
  regex_elem_t   *pElem   = (regex_elem_t *) e;
  char const    **ppStr   = (char const **) d1;
  char            c       = **ppStr;
  boolean         isMatch = FALSE;

  /*  If we're not at the end of the string do the match.
   */
  if (c != '\0')
  {
    ++(*ppStr);
    isMatch = pElem->pfnMatch(&pElem->data, c);
  }

  return isMatch;
}


/*-------- mre_regex_match --------*/
/**
@brief  Return TRUE or FALSE depending on whether the passed string matches
        the regular expression.

@param

@return

*/
boolean mre_regex_match(
  mre_regex_t *pRegEx,
  char const  *pStr
)
{
  boolean isMatch;

  isMatch = mre_set_iterate(pRegEx, regex_do_match, (void *) &pStr);

  /*  We matched if there was a match and we're left at the end of the string.
   */
  return isMatch && *pStr == '\0';
}

