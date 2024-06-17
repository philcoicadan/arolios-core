/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/App_user_delete.h>
#include <mttr/Base_element_delete.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <appcfg/App.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <util/String.h>
#include <common/User.h>





using namespace arolios;
using namespace arolios::mttr;

App_user_delete::App_user_delete(const std::string& p_appuser_id ) : Base_element_delete (p_appuser_id)  {

}

App_user_delete::~App_user_delete() {

}

void App_user_delete::execute () {
    
    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
    auto uac_info = common::Singleton<common::UAC_info>::instance().object();

    std::string stmt  = "SELECT * from am_app_user WHERE id= $1";
    
    

    auto f = db_->execSqlAsyncFuture ( stmt, elem_id_);

    auto result = f.get();

    if ( result.size() > 0 ) {
        
        auto user = result[0]["au_user"].as<unsigned int>();
        auto app = result[0]["au_app"].as<unsigned int>();
        
        if (user == util::String::stoui(uac_info->super_user()->id())) {
             std::string error_msg = "The super user cannot be dissociated from the application";
             
            throw exception::Model_error (error_msg);
        } 
    
        if ( app == util::String::stoui(app_info_ptr_->default_app()->id() )) {
            std::string error_msg = "The default app cannot be dissociated from the user";
             
            throw exception::Model_error ( error_msg );
        }
    

    
        std::string stmt = "DELETE FROM am_app_user  WHERE  id= $1 RETURNING id";

        auto f = db_->execSqlAsyncFuture ( stmt, util::String::stoui(elem_id_));
        auto result = f.get();

        if ( result.size() > 0 ) {

            return_code_ = qry::Return_code::OK;

        }
    } else {
         std::string error_msg = "The appuser " + elem_id_ + " not found" ;
             
            throw exception::Not_found ( error_msg );
    }

}



