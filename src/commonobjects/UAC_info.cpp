/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <common/UAC_info.h>
#include <common/Db_client.h>
#include <util/String.h>
#include <common/User.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <appcfg/App.h>
#include <qry/Init_retriever.h>
#include <util/String.h>
#include <exception/Exceptions.h>




using namespace arolios;
using namespace arolios::common ;

UAC_info::UAC_info() {

}

UAC_info::~UAC_info() {

}



std::shared_ptr<common::User> UAC_info::super_user() const {
    return super_user_;
}



    


bool UAC_info::check_role (const common::User_role p_user_role, const common::User_role p_required_role)  const {
    
    bool result = false;
    switch (p_user_role) {
        case common::User_role::admin: // admin has all rights
            result = true;
            break;
        case common::User_role::author:
            switch (p_required_role) {
                case common::User_role::reader:
                case common::User_role::author:
                    result= true;
                    break;
                default:
                    break;
            }
            break;
        case common::User_role::reader:
            switch (p_required_role) {
                case common::User_role::reader:
                    result = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    
    
    return result;
} 


std::string UAC_info::standardize_role (const std::string& p_user_role) const  {
    
    const std::string std_role = util::String::standardize(p_user_role);
    
    return string_cast( role_cast (std_role));
  
}
    
common::User_role UAC_info::role_cast (const std::string& p_role_str) const {
    static std::unordered_map<std::string,common::User_role> m { {"no", common::User_role::no} , {"reader", common::User_role::reader}, 
    {"author", common::User_role::author}, {"admin", common::User_role::admin} };
    
    auto search = m.find(p_role_str);
    if (search == m.end()) {
        const std::string error_msg ="user role " + p_role_str + " not found";
         
        throw exception::Not_found (error_msg);
    }
    return search->second;
}

const std::string& UAC_info::string_cast ( const common::User_role p_role) const {
    
    static std::string tab [4] = { "no", "reader", "author", "admin" };
    return tab[ static_cast<int>(p_role) ] ;
}        


std::shared_ptr<common::User> UAC_info::retrieve (const std::string& p_identifier, const std::string& p_pwd) const {

    std::shared_ptr<common::User> user;
    auto db = common::Singleton<common::Db_client>::instance().object();
    const auto identifier = util::String::standardize(p_identifier);
    const auto app_info = common::Singleton<common::App_info>::instance().object();
    const auto app = app_info->app();
    
    std::string select = "SELECT am_user.* ";
    std::string from = "FROM am_user " ;
    std::string where = "WHERE am_user.u_identifier = $1 AND am_user.u_pwd = crypt($2, u_pwd) ";
    
    if (app->id() != app_info->default_app_id())  {
        select += ", am_app_user.au_role ";
        from +=  "INNER JOIN am_app_user ON am_user.id = am_app_user.au_user  ";
        where += "AND  am_app_user.au_app = " + app->id();
    }
    
    auto f = db->execSqlAsyncFuture( select + from + where, identifier, p_pwd);

    

    try {
        auto result = f.get();

        if ( result.size() > 0 ) {
            auto row = result[0];

            common::User_role ur = (app->id() != app_info->default_app_id()) ?
                                    role_cast ( row["au_role"].as<std::string>() ) :
                                    role_cast ( row["u_role"].as<std::string>() ) ;
;

            if ( ur != common::User_role::no ) {

                user = std::make_shared<common::User>();
                user->identifier ( identifier );

                unsigned int id = row["id"].as<unsigned int>();
                user->persistent_attributes ( std::to_string ( id ), "am_user" ) ;

                user->app_role ( ur );
            }
        }
            
        return user;
    }
    catch ( const exception::Db_exception &e ) {
        std::cerr << "error: " << e.base().what() << std::endl;

        throw e;
    }
}