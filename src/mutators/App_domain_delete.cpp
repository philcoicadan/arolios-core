/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/App_domain_delete.h>
#include <mttr/Base_element_delete.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <appcfg/App.h>
#include <util/String.h>





using namespace arolios;
using namespace arolios::mttr;

App_domain_delete::App_domain_delete(const std::string& p_appdomain_id ) :  Base_element_delete(p_appdomain_id)  {

}

App_domain_delete::~App_domain_delete() {

}

void App_domain_delete::execute () {
    
    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();

    
    std::string stmt  = "SELECT * from am_app_domain WHERE id= $1";
    
    

    auto f = db_->execSqlAsyncFuture ( stmt, util::String::stoui(elem_id_));

    auto result = f.get();

    if ( result.size() > 0 ) {
        
        auto domain = result[0]["a_domain"].as<unsigned int>();
        auto app = result[0]["a_app"].as<unsigned int>();
        
        if (domain == util::String::stoui( mdl_info->standard_domain()->id())) {
             std::string error_msg = "The standard domain cannot be dissociated";
             
            throw exception::Model_error (error_msg);
        } 
    
        if ( app == util::String::stoui(app_info_ptr_->default_app()->id() )) {
            std::string error_msg = "The default app cannot be dissociated";
             
            throw exception::Model_error ( error_msg );
        }
    
        std::string stmt = "DELETE FROM am_app_domain  WHERE  id= $1 RETURNING id";

        auto f = db_->execSqlAsyncFuture ( stmt, util::String::stoui(elem_id_ ));

        auto result = f.get();

        if ( result.size() > 0 ) {

            return_code_ = qry::Return_code::OK;

        }
    } else {
         std::string error_msg = "The appdomain " + elem_id_ + " not found" ;
             
            throw exception::Not_found ( error_msg );
    }

}



