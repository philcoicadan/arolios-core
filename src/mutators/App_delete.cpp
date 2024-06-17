/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/App_delete.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <appcfg/App.h>




using namespace arolios;
using namespace arolios::mttr;

App_delete::App_delete(const std::string& p_app_name ) : Base_element_delete (p_app_name)  {

}

App_delete::~App_delete() {

}

void App_delete::execute () {
    
    // the name is the external identifier
    elem_id_ = util::String::standardize(elem_id_);
    if (elem_id_== app_info_ptr_->default_app()->name() ) {
        std::string error_msg = "The default app cannot be deleted";
         
        throw exception::Model_error (error_msg);
    }
    
    
     std::string stmt = "DELETE FROM am_app  WHERE  name= $1 RETURNING id";

    

    auto f = db_->execSqlAsyncFuture ( stmt, elem_id_);

    auto result = f.get();

    if ( result.size() > 0 ) {

        return_code_ = qry::Return_code::OK;
        
    }

}



