/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <qry/Base_query.h>

namespace arolios {


namespace common {
    class App_info;
}

namespace mttr {
class Data_import: public qry::Base_query {
public:
    /**
     * Constructor
     */
    Data_import( const std::string& p_filename);

    /**
     * Destructor
     */
    virtual ~Data_import();
    
    virtual void execute () override;
    
private:
    
    void execute_statement (const Json::Value& p_stmt) ;
    void register_instance (const Json::Value& p_j_value, const std::string& p_inst_name) ;
    
    const std::shared_ptr<common::App_info> app_info_ptr_;
    const std::string filename_;
    std::unordered_map<std::string, std::shared_ptr<const Json::Value>> cache_;
    

};

}
}

