/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/User_change_password.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <common/User.h>
#include <appcfg/App.h>
#include <common/Singleton.h>
#include <util/String.h>







using namespace arolios;
using namespace arolios::mttr;

User_change_password::User_change_password(const std::shared_ptr<common::User>& p_user , const Json::Value& p_data_root) : user_ (p_user) ,
data_root_ (p_data_root){

}

User_change_password::~User_change_password() {

}

void User_change_password::execute () {



    const std::string current_pwd = data_root_  ["current_password"].asString() ;
    const std::string new_pwd = data_root_  ["new_password"].asString() ;
    return_code_ = qry::Return_code::KO;

    if ( current_pwd.empty() || new_pwd.empty() ) {
        std::string error_msg = "Data for changing password are missing";
         
        throw exception::Not_found ( error_msg );
        
    } else if (current_pwd == new_pwd ) {
        std::string error_msg = "Current and new passwords are identical";
         
        throw exception::Data_error ( error_msg );

    } else if (new_pwd.length() < 6) {
        std::string error_msg = "New password is too short ( minimum: 6 characters)";
         
        throw exception::Data_error (error_msg) ;
    } else {


        const std::string fields_stmt = "UPDATE am_user SET ( upd_count, u_pwd )";
        const std::string values_stmt = "= ROW (upd_count+1, crypt ( $1, gen_salt('bf')) ) ";


        const std::string stmt = fields_stmt + values_stmt + " WHERE id= $2 AND u_pwd = crypt( $3, u_pwd) RETURNING upd_count" ;


        

        auto f = db_->execSqlAsyncFuture ( stmt, new_pwd, util::String::stoui ( user_->id() ) , current_pwd);

        auto result = f.get();

        if ( result.size() > 0 ) {
            auto && row = result[0];
            new_upd_count_ = row["upd_count"].as<int>(); 
            return_code_ = qry::Return_code::OK;
        } else {
            return_code_ = qry::Return_code::KO;
        }
    }
}


int User_change_password::new_update_counter() const
{
    return new_upd_count_;
}
