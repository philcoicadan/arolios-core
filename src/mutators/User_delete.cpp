/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/User_delete.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <common/User.h>
#include <util/String.h>






using namespace arolios;
using namespace arolios::mttr;

User_delete::User_delete(const std::string& p_user_ident ) : Base_element_delete(p_user_ident)  {

}

User_delete::~User_delete() {

}

void User_delete::execute () {
    
    
        // the identifier is the external identifier
    elem_id_ = util::String::standardize(elem_id_);
    auto uac_info = common::Singleton<common::UAC_info>::instance().object(); 
    if (elem_id_== uac_info->super_user()->identifier() ) {
        std::string error_msg = "The super user cannot be deleted";
         
        throw exception::Model_error (error_msg);
    }
    
    
     std::string stmt = "DELETE FROM am_user  WHERE  u_identifier = $1 RETURNING id";

    

    

    auto f = db_->execSqlAsyncFuture ( stmt, elem_id_);

    auto result = f.get();

    if ( result.size() == 0 ) {
        // deletion failed, set deprecated
        stmt = "UPDATE am_user SET u_lc = 'deprecated' WHERE u_identifier = $1 RETURNING id";
        
        
        auto f = db_->execSqlAsyncFuture ( stmt, elem_id_);

        auto result = f.get();
        if ( result.size() == 0 ) {
            std::string error_msg = "Error in deleting user";
            throw exception::Sql_error ( error_msg );
        }
    }

    return_code_ = qry::Return_code::OK;
        
}




