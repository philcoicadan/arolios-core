/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/App_update.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <appcfg/App.h>




using namespace arolios;
using namespace arolios::mttr;

App_update::App_update(const std::string& p_app_name , const Json::Value& p_data_root, const int p_rfu_count) 
: Base_element_update(p_app_name, p_data_root, p_rfu_count) {

}

App_update::~App_update() {

}

void App_update::execute () {
    
        // the name is the external identifier
    elem_id_ = util::String::standardize(elem_id_);
    if (elem_id_== app_info_ptr_->default_app()->name() ) {
        std::string error_msg = "The default app cannot be modified";
         
        throw exception::Model_error (error_msg);
    }

    
    const std::string name = data_root_["name"].asString() ;
    const std::string descr = data_root_["description"].asString() ;
    
    std::string fields_stmt = "UPDATE am_app SET ( upd_count";
    std::string values_stmt = "= ROW (upd_count+1";
    
    if (!name.empty()) {
       fields_stmt += ", name";
       values_stmt += ", '" + util::String::standardize(name) + "'";
    }
    
    if (!descr.empty()) {
       fields_stmt += ", description";
       values_stmt += ", '" + util::String::double_quote(descr) + "'";
    }
    
    fields_stmt += " ) ";
    values_stmt += " ) ";
    
    std::string stmt = fields_stmt + values_stmt + " WHERE name='" + elem_id_+ "' ";

    if (rfu_count_ > 0) {
        stmt += " AND upd_count=" + std::to_string(rfu_count_);
    }
    stmt += " RETURNING upd_count";
    
    
    
    auto f = db_->execSqlAsyncFuture ( stmt);
    
    auto result = f.get();
    
    if (result.size() > 0 ) {
        auto && row = result[0];
        new_upd_count_ = row["upd_count"].as<int>(); ;
        return_code_ = qry::Return_code::OK;
    } else {
        return_code_ = qry::Return_code::KO;
    }
}

