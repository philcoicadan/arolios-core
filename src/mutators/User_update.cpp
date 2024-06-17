/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/User_update.h>
#include <mttr/Base_element_update.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <common/User.h>








using namespace arolios;
using namespace arolios::mttr;

User_update::User_update( const std::string& p_user_identifier , const Json::Value& p_data_root, const int p_rfu_count) 
: Base_element_update( p_user_identifier, p_data_root, p_rfu_count) {

}

User_update::~User_update() {

}

void User_update::execute () {

    // the identifier is the external identifier
    elem_id_ = util::String::standardize(elem_id_);
    auto uac_info = common::Singleton<common::UAC_info>::instance().object(); 
    if (elem_id_== uac_info->super_user()->identifier() ) {
        std::string error_msg = "The super user cannot be modified";
         
        throw exception::Model_error (error_msg);
    }
    
    std::string fields_stmt = "UPDATE am_user SET ( upd_count";
    std::string values_stmt = "= ROW (upd_count+1";

    
    const std::string identifier = data_root_["identifier"].asString() ;
    
    if ( !identifier.empty() ) {
        if (identifier.length() < 3) {
            std::string error_msg = "User update: identifier is too short ( minimum: 3 characters)";
             
            throw exception::Data_error (error_msg) ;
        }
        fields_stmt += ", u_identifier ";
        values_stmt += ", '" +util::String::standardize ( identifier ) + "'";
        

    }
    
    const std::string pwd = data_root_["password"].asString() ;
    if ( !pwd.empty() ) {
        
        if (pwd.length() < 6) {
            std::string error_msg = "User update: password is too short ( minimum: 6 characters)";
             
            throw exception::Data_error (error_msg) ;
        }



        fields_stmt += ", u_pwd ";
        values_stmt += ", crypt( '" + pwd + "', gen_salt('bf'))";

    }

    std::string def_role = data_root_["role"].asString() ;
    if (!def_role.empty()) {
        auto uac_ptr = common::Singleton<common::UAC_info>::instance().object();
        def_role = uac_ptr->standardize_role(def_role);
        if (def_role.empty()) {
            std::string error_msg = "Error in User creation: role not found";
             
            throw exception::Model_error (error_msg) ;
        }
        fields_stmt += ", u_role ";
        values_stmt += ", '" + def_role + "'";
        
    }

    if ( !identifier.empty()  || !def_role.empty() || !pwd.empty()) {


        fields_stmt += " ) ";
        values_stmt += " ) ";

        std::string stmt = fields_stmt + values_stmt + " WHERE u_identifier='" + elem_id_+ "' ";

        if ( rfu_count_ > 0 ) {
            stmt += " AND upd_count=" + std::to_string ( rfu_count_ );
        }
        stmt += " RETURNING upd_count";

        

        auto f = db_->execSqlAsyncFuture ( stmt );

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

