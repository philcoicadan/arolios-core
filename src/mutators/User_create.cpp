/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/User_create.h>
#include <mttr/Base_element_create.h>
#include <util/String.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>






using namespace arolios;
using namespace arolios::mttr;

User_create::User_create(const Json::Value& p_data_root) :
Base_element_create(p_data_root){

}

User_create::~User_create() {

}

void User_create::execute () {
    
    std::string identifier = data_root_["identifier"].asString() ;
    if (identifier.empty()) {
        std::string error_msg = "User creation: identifier is missing";
         
        throw exception::Data_error (error_msg) ;
    }
    if (identifier.length() < 3) {
        std::string error_msg = "User creation: identifier is too short ( minimum: 3 characters)";
         
        throw exception::Data_error (error_msg) ;
    }
    identifier = util::String::standardize (identifier);
    
    std::string def_role = data_root_["role"].asString() ;
    auto uac_ptr = common::Singleton<common::UAC_info>::instance().object();
    def_role = uac_ptr->standardize_role(def_role);
    if (def_role.empty()) {
        std::string error_msg = "Error in User creation: role not found";
         
        throw exception::Model_error (error_msg) ;
    }
    
    
    
    const std::string pwd = data_root_["password"].asString() ;
    if (pwd.empty()) {
        std::string error_msg = "User creation: password not found";
         
        throw exception::Data_error (error_msg) ;
    }
    if (pwd.length() < 6) {
        std::string error_msg = "User creation: password is too short ( minimum: 6 characters)";
         
        throw exception::Data_error (error_msg) ;
    }


    return_code_ = qry::Return_code::KO;
    
    try {
        auto r = db_->execSqlSync ( "INSERT INTO am_user (u_identifier, u_pwd, u_role) VALUES ($1, crypt($2, gen_salt('bf')),$3) RETURNING id", identifier, pwd, def_role);
        int nb_r = r.size();
        if ( nb_r >0 ) {
            auto row = r[0];
            unsigned int id = row["id"].as<unsigned int>();
            
          

            const std::string id_str = std::to_string(id) ;

            return_json_["url"] = Json::Value(qry::Util::url_prefix("user")+ identifier);
            return_code_ = qry::Return_code::OK;
        }
        
    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    
    
}


