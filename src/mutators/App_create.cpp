/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/App_create.h>
#include <mttr/Base_element_create.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <util/String.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <util/String.h>
#include <common/User.h>






using namespace arolios;
using namespace arolios::mttr;

App_create::App_create(const Json::Value& p_data_root) 
: Base_element_create(p_data_root) {

}

App_create::~App_create() {

}

void App_create::execute () {
    
    const std::string name = data_root_["name"].asString() ;
    if (name.empty()) {
        std::string error_msg = "App creation: name is missing";
         
        throw exception::Model_error (error_msg) ;
    }
 
    
    const std::string descr = data_root_["description"].asString() ;
    
    const std::string stdzd_name = util::String::standardize(name);
    std::string fields_stmt = "INSERT INTO am_app (name";
    std::string values_stmt = "VALUES ( '"+ stdzd_name + "'";
    
    if (!descr.empty()) {
       fields_stmt += ", description";
       values_stmt += ", '" + util::String::double_quote(descr) + "'";
    }
    
    fields_stmt += ") ";
    values_stmt += ") ";
    
    const std::string stmt = fields_stmt + values_stmt + " RETURNING id";

    
    
    auto f = db_->execSqlAsyncFuture ( stmt);
    
    auto result = f.get();
    
    if (result.size() > 0 ) {
        auto && row = result[0];
        const std::string id_str = std::to_string(row["id"].as<unsigned int>()) ;

        return_json_["url"] = Json::Value(qry::Util::url_prefix("app")+ stdzd_name);
        
        auto uac_info = common::Singleton<common::UAC_info>::instance().object();
        auto super_u_id = uac_info->super_user()->id();
        
        // associate super_user as admin for the new App 
        auto r2 = db_->execSqlSync ( "INSERT INTO am_app_user (au_user, au_app, au_role) VALUES ($1, $2, $3) RETURNING au_user",util::String::stoui(super_u_id), util::String::stoui(id_str), "admin" );

        if ( r2.size() == 0 ) {
            std::string error_msg = "Error in associating the super user to the new application ";
             
            throw exception::Sql_error ( error_msg );
        }
        
        // associate standard package to the new App_create
        auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
        auto std_domain =  mdl_info->standard_domain();
        
        // associate standard domain to the new App 
        auto r3 = db_->execSqlSync ( "INSERT INTO am_app_domain (a_domain, a_app) VALUES ($1, $2) RETURNING a_domain",util::String::stoui(std_domain->id()), util::String::stoui(id_str) );

        if ( r3.size() == 0 ) {
            std::string error_msg = "Error in associating the standard domain to the new application ";
             
            throw exception::Sql_error ( error_msg );
        }
        
        return_code_ = qry::Return_code::OK;
    } else {
        return_code_ = qry::Return_code::KO;
    }
    
}

