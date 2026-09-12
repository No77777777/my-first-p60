/*==============================================================================

                              ds_dynamic_ss_config.h

GENERAL DESCRIPTION

Copyright (c) 2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
==============================================================================*/

/*==============================================================================
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/04/17    ml     Created file/Initial version.
==============================================================================*/
#ifndef DS_DYNAMIC_SS_CONFIG_H
#define DS_DYNAMIC_SS_CONFIG_H

#include "comdef.h"

#include "ds_ASString.h"
#include "ds_ASVector.h"
#include "ds_ASMap.h"
#include "ds_ASString.h"
#include "ds_ASIOStream.h"
#include "ds_XMLElement.h"
#include "ps_sys_event.h"
#include "ps_sys_ioctl.h"


class ds_abss_iin_info
{
public:
  ds_abss_iin_info(const ASString& iin_str, uint32 iin_weight);
  ds_abss_iin_info(const Appsrv::XML::XMLElement* elem, bool& flag);

private:
  void parse_iin(const Appsrv::XML::XMLElement* elem, bool& flag);
  void parse_weight(const Appsrv::XML::XMLElement* elem, bool& flag);

public:
  ASString iin;
  uint32 weight;
};



class ds_abss_app_info
{
public:
  explicit ds_abss_app_info(const ASString& app_id_str);
  ds_abss_app_info(const Appsrv::XML::XMLElement* elem, bool& flag);
  ~ds_abss_app_info();

  void clear();
  bool is_empty() const;

private:
  void parse_app_id(const Appsrv::XML::XMLElement* elem, bool& flag);
  void parse_priority(const Appsrv::XML::XMLElement* elem, bool& flag);
  void parse_iin_list(const Appsrv::XML::XMLElement* elem, bool& flag);

public:
  ASString app_id;
  uint16   priority;
  ASVector<ds_abss_iin_info*> iin_info_list;
};



class ds_abss_data_mgr
{
public:
  ds_abss_data_mgr()
  { }

  ~ds_abss_data_mgr();

  void clear();
  bool is_empty() const;

  bool parse_xml(const char* filepath);
  void parse_app_list(const Appsrv::XML::XMLElement* elem, bool& flag);

  ds_abss_app_info* get_app_priority_info(const ASString& app_id);

  ps_sys_event_config_app_pref_result_enum_type update_app_config(ps_sys_ioctl_3gpp_config_app_pref_type* app_pref_info);
  bool remove_app_config(ps_sys_3gpp_app_id_info_type* app_id);
  bool remove_all_app_config();
  bool get_app_id_list(ps_sys_ioctl_3gpp_get_app_list* app_id_list);

private:
  friend Appsrv::Utils::ASOStream& operator<<(Appsrv::Utils::ASOStream& os, const ds_abss_data_mgr* pinfo);
  friend Appsrv::Utils::ASOStream& operator<<(Appsrv::Utils::ASOStream& os, const ds_abss_data_mgr& pinfo);

  // ASString ota_url;
  // Pair<app_id, pinfo>
  Appsrv::Utils::ASMap<ASString, ds_abss_app_info*> app_info_list;
};


Appsrv::Utils::ASOStream& operator<<(Appsrv::Utils::ASOStream& os, const ds_abss_data_mgr* pinfo);
Appsrv::Utils::ASOStream& operator<<(Appsrv::Utils::ASOStream& os, const ds_abss_data_mgr& pinfo);

Appsrv::Utils::ASOStream& operator<<(Appsrv::Utils::ASOStream& os, const ds_abss_app_info* app_info);
Appsrv::Utils::ASOStream& operator<<(Appsrv::Utils::ASOStream& os, const ds_abss_iin_info* iin_info);


#endif /* DS_DYNAMIC_SS_CONFIG_H */
