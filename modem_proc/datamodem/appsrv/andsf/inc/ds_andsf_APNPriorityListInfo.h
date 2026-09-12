/*==============================================================================

                        ds_andsf_priority_list_manager.h

GENERAL DESCRIPTION
  ANDSF Priority List Manager

  Copyright (c) 2014 by Qualcomm Technologies Incorporated. All Rights Reserved.
==============================================================================*/

/*==============================================================================
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/26/15    ml     Created file/Initial version.
==============================================================================*/
#ifndef DS_ANDSF_APN_PRIORITY_LIST_INFO_H
#define DS_ANDSF_APN_PRIORITY_LIST_INFO_H

#include "datamodem_variation.h"
#ifdef FEATURE_DATA_ANDSF_RULE_MGR

#include "ds_andsf_ext_i.h"
#include "ds_ASVector.h"
#include "ds_ASString.h"

namespace Appsrv
{
namespace ANDSF
{


/* ==========================================================================
 * Class APNPriorityListInfo
 *
 * DESCRIPTION
 *  This class manages priority list information for a specific APN. APN name
 *  must be specified on class creation.
 * ========================================================================== */
class APNPriorityListInfo
{
public:
  /* ========================================================================
   * APNPriorityListInfo Constructor
   *
   * DESCRIPTION
   *  APNPriorityListInfo constructor.
   *
   * PARAMETERS
   *  [In] name         - APN name
   *  [In] api_override - True if ANDSF policy specifies API override, else false
   *  [In] plist        - Default priority list to use
   *
   * RETURN VALUE
   *  None
   *
   * DEPENDENCIES
   *  None
   * ======================================================================== */
  APNPriorityListInfo(const ASString& name);
  APNPriorityListInfo(const APNPriorityListInfo& plist);
  APNPriorityListInfo(
                      const ASString&                                   name,
                      bool                                              api_override,
                      bool                                              data_roaming_option,
                      const ASVector<ds_andsf_access_tech_info_s_type>& basic_plist
                      );

  /* ========================================================================
   * APNPriorityListInfo operator<
   *
   * DESCRIPTION
   *  Operator overload used for sorting list of APNPriorityListInfo.
   *  Sorting criteria is based on APN name.
   *
   * PARAMETERS
   *  [In] rhs - The other APNPriorityListInfo object to compare
   *
   * RETURN VALUE
   *  True if 'this' object's APN name < rhs APN name, else false
   *
   * DEPENDENCIES
   *  None
   * ======================================================================== */
  bool operator<(const APNPriorityListInfo& rhs) const;

  /* ========================================================================
   * APNPriorityListInfo check_apn_name
   *
   * DESCRIPTION
   *  Checks if the input string matches with the APN name.
   *
   * PARAMETERS
   *  [In] str - The string to compare
   *
   * RETURN VALUE
   *  True if APN name matches the given string, else false
   *
   * DEPENDENCIES
   *  None
   * ======================================================================== */
  bool check_apn_name(const ASString& str) const;

  /* ========================================================================
   * APNPriorityListInfo get_apn_plist
   *
   * DESCRIPTION
   *  Fills the priority information of the APN to the given output object
   *
   * PARAMETERS
   *  [Out] apn_plist - The output object to fill
   *
   * RETURN VALUE
   *  None
   *
   * DEPENDENCIES
   *  None
   * ======================================================================== */
  void get_apn_plist(ds_andsf_apn_priority_info_s_type* apn_plist) const;

  /* ========================================================================
   * APNPriorityListInfo update_tech_plist
   *
   * DESCRIPTION
   *  Updates the current priority list with the input priority list.
   *  If the input object's APN name does not match, update will fail.
   *
   * PARAMETERS
   *  [In] new_plist - The output object to fill
   *
   * RETURN VALUE
   *  None
   *
   * DEPENDENCIES
   *  None
   * ======================================================================== */
  void update_tech_plist(const APNPriorityListInfo& new_plist);

  void create_active_tech_plist(
                                const bool roaming_status,
                                const bool data_roaming_setting
                                );


private:
  // Hide default constructor
  APNPriorityListInfo()
  { }


  const ASString                             apn_name;
  bool                                       is_api_override;
  bool                                       data_roaming_option;
  ASVector<ds_andsf_access_tech_info_s_type> basic_tech_plist;
  ASVector<ds_andsf_access_tech_info_s_type> tech_plist;

  friend void compare_apn_plist(const APNPriorityListInfo& lhs, const APNPriorityListInfo& rhs, uint32 &change_mask);


  /*Debug*/
public:
  void print();
};


/*===========================================================================
FUNCTION      COMPARE_APN_PLIST

DESCRIPTION   This function is called to compare the old and new apn_plist per apn. 
                              
PARAMETERS 
  lhs[IN]             - left  apn_plist 
  rhs[IN]             - right apn_plist
  change_mask[IN&OUT] - mark which fields inside apn_plist has been changed.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void compare_apn_plist(const APNPriorityListInfo& lhs, const APNPriorityListInfo& rhs, uint32 &change_mask);

} /* namespace ANDSF */
} /* namespace Appsrv */

#endif /* FEATURE_DATA_ANDSF_RULE_MGR */
#endif /* DS_ANDSF_APN_PRIORITY_LIST_INFO_H */
