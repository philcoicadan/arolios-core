/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <memory>
#include <unordered_map>

namespace arolios {

namespace omm {
    class Property;
}

namespace appcfg {
class Class_instance_member_end {
public:
    /**
     * Constructor
     */
    Class_instance_member_end(const std::shared_ptr<const omm::Property>& p_prop_member_end);

    /**
     * Destructor
     */
    ~Class_instance_member_end();
    
    std::unordered_map<std::string,std::string>& property_column_name_map_for_list();
    std::unordered_map<std::string,std::string>& property_alias_column_name_map_for_list();
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>>& property_map_for_list();
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>>& enum_map_for_list();
    
    std::string& list_select_template () ;
    
    
    const std::shared_ptr<const omm::Property>& member_end() const;
    
    void table_alias (const std::string& p_name) ;
    const std::string table_alias () const ;
    
    std::pair<std::string,std::string> management_member_column_name( const std::string& p_name) const ;

    std::string find_column_from_property_name (const std::string& p_path_name, bool p_with_alias=true) const ;

private:

    std::shared_ptr<const omm::Property> prop_member_end_;
    std::unordered_map<std::string,std::string> property_column_name_map_for_list_;
    std::unordered_map<std::string,std::string> property_alias_column_name_map_for_list_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> property_map_for_list_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> enum_map_for_list_;
    std::string table_alias_;
    std::string list_tplt_;

};
}
}

