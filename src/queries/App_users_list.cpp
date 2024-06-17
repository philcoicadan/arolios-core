/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/App_users_list.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <util/String.h>
#include <qry/Util.h>
#include <appcfg/App.h>
#include <common/User.h>





using namespace arolios;
using namespace arolios::qry ;

App_users_list::App_users_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang , bool p_only_active
) : Base_admin_element_list ( p_offset, p_limit, p_sort, p_direction,p_lang, p_only_active)

{
}


App_users_list::~App_users_list() { }

std::future<qry::Db_result> App_users_list::internal_execute (const std::string& p_order_by) 
{   
    auto default_app = app_info_ptr_->default_app() ;
    
    auto  info_ua = common::Singleton<common::UAC_info>::instance().object();
    
    const std::string super_user_id =  info_ua->super_user()->id();


        std::string stmt = "SELECT COUNT(*) OVER() AS _total, am_user.u_identifier, am_app.name AS app_name, am_app_user.* FROM am_app_user INNER JOIN am_user ON am_app_user.au_user = am_user.id  INNER JOIN am_app ON am_app_user.au_app = am_app.id WHERE am_app_user.au_user <> $1 AND  am_app_user.au_app <> $2" ;
        
        if (only_active_) {
            stmt += " AND  am_user.u_lc = 'active' ";
        }
        
        stmt += " ORDER BY " + p_order_by + " LIMIT " + limit_+ " OFFSET $3";


        

        auto f = db_->execSqlAsyncFuture ( stmt, util::String::stoui(super_user_id), util::String::stoui(default_app->id()), offset_ );

    return f;

}


void App_users_list::internal_build_json_from_row (Json::Value& p_jrow, const qry::Db_row& p_row)
{
     auto id_str = std::to_string(p_row["id"].as<unsigned int>() );
        p_jrow["url"] = Json::Value (  qry::Util::url_prefix ("appuser") + id_str );
 

        p_jrow["app"] = Json::Value ( Json::objectValue ) ;
        Json::Value& jrow_app = p_jrow["app"];
        jrow_app["name"] = Json::Value ( p_row["app_name"].as<std::string>() );
        jrow_app["url"] = Json::Value (  qry::Util::url_prefix("app") + jrow_app["name"].asString()) ;
        
        
        p_jrow["user"] = Json::Value ( Json::objectValue ) ;
        Json::Value& jrow_user = p_jrow["user"];
        jrow_user["identifier"] = Json::Value ( p_row["u_identifier"].as<std::string>() );
        jrow_user["url"] = Json::Value (  qry::Util::url_prefix("user") + jrow_user["identifier"].asString());

        p_jrow["role"] = Json::Value ( p_row["au_role"].as<std::string>() );


}




std::string App_users_list::compute_order_by () {
    std::string order_by;
    if (sort_.empty() ) {
        order_by =  "am_app_user.id" ;
    } else if (sort_ == "user") {
        order_by = "au_user" ;
    } else if (sort_ == "app") {
        order_by = "au_app";
    } else if ( sort_ == "user.identifier" ) {
        order_by = "u_identifier"  ;
    } else if ( sort_ == "app.name" ) {
        order_by = "app_name" ;
    } else if  (sort_ == "role") {
        order_by = "au_role" ;
    } else {
            
        std::cout << "WARNING: Appusers list, sort ignored"<< std::endl;
         order_by =  "am_app_user.id" ;
    }
    
    order_by += " " + direction_;
    
    return order_by;
}

