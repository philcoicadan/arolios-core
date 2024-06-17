/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/Class_instance_member_end.h>
#include <exception/Exceptions.h>


using namespace arolios;
using namespace arolios::appcfg;


Class_instance_member_end::Class_instance_member_end(const std::shared_ptr<const omm::Property>& p_prop_member_end)
: prop_member_end_(p_prop_member_end)
{

}

Class_instance_member_end::~Class_instance_member_end() {

}


std::unordered_map<std::string,std::string>& Class_instance_member_end::property_column_name_map_for_list() {
    return property_column_name_map_for_list_;
}

std::unordered_map<std::string,std::string>& Class_instance_member_end::property_alias_column_name_map_for_list() {
    return property_alias_column_name_map_for_list_;
}

std::unordered_map<std::string, std::shared_ptr<const omm::Property>>& Class_instance_member_end::property_map_for_list() {
    return property_map_for_list_;
}

std::unordered_map<std::string, std::shared_ptr<const omm::Property>>& Class_instance_member_end::enum_map_for_list() {
    return enum_map_for_list_;
}

std::string& Class_instance_member_end::list_select_template ()  {
    return list_tplt_;
}

const std::shared_ptr<const omm::Property>& Class_instance_member_end::member_end() const {
    return prop_member_end_;
}

void Class_instance_member_end::table_alias (const std::string& p_name)  {
    table_alias_ = p_name;
}

const std::string Class_instance_member_end::table_alias () const {
    return table_alias_;
}

std::pair<std::string,std::string> Class_instance_member_end::management_member_column_name( const std::string& p_name) const {
    
    static std::unordered_map<std::string,std::string> m  { {"id","id"}, {"csf_id", "csf_id"}, {"trashed_date", "trashed_date"}};
    auto search =  m.find(p_name);
    if (search != m.end()) {
        return std::make_pair<const std::string&,std::string> (p_name, table_alias_ + "_" + search->second);
    } else {
        throw exception::Failure ("management column name for member not found") ;
    }
}

std::string Class_instance_member_end::find_column_from_property_name(
    const std::string &p_name, bool p_with_alias) const {

  std::string result;
  if (p_with_alias) {
    // first search in the alias map
    auto search_alias = property_alias_column_name_map_for_list_.find(p_name);
    if (search_alias != property_alias_column_name_map_for_list_.end()) {
      result = search_alias->second;
    }
  }
  if (result.empty()) {
    // not yet found

    auto search = property_column_name_map_for_list_.find(p_name);
    if (search != property_column_name_map_for_list_.end()) {
      result = search->second;
    } 
  }

  return result;
}
