/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <unordered_map>
#include <string>
#include <memory>

namespace arolios {
namespace omm {
    class Property;
}

namespace appcfg {
class Util {
public:
    /**
     * Constructor
     */
    Util();

    /**
     * Destructor
     */
    ~Util();
    
    static  void add_prop_for_select_template ( const std::shared_ptr<const omm::Property>& p_prop,
            std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_prop_map,
            std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_enum_map,
            std::unordered_map<std::string,std::string>& p_prop_column_name_map,
            std::unordered_map<std::string,std::string>& p_prop_alias_column_name_map,
            const std::string& p_prop_name=std::string(), const std::string& p_table_prefix=std::string() );

    static void add_nested_prop_for_select_template ( const std::shared_ptr<const omm::Property>& p_prop,
            std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_prop_map,
            std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_enum_map,
            std::unordered_map<std::string,std::string>& p_prop_column_name_map,
            std::unordered_map<std::string,std::string>& p_prop_alias_column_name_map,
            const std::string& p_prop_name=std::string(), const std::string& p_col_path=std::string(), const std::string& p_alias_col_path=std::string(), const std::string& p_table_prefix=std::string() );

};


}
}

