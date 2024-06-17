/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Users_list.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <qry/Util.h>
#include <common/User.h>






using namespace arolios;
using namespace arolios::qry ;

Users_list::Users_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang , bool p_only_active
) : Base_admin_element_list ( p_offset, p_limit, p_sort, p_direction,p_lang, p_only_active)

{
}


Users_list::~Users_list() { }



std::future<qry::Db_result> Users_list::internal_execute (const std::string& p_order_by) 
{

    
    auto default_app = app_info_ptr_->default_app() ;
    
    auto  info_ua = common::Singleton<common::UAC_info>::instance().object();
    
    const std::string super_user_id =  info_ua->super_user()->id();

    
    
    std::string stmt = "SELECT COUNT(*) OVER() AS _total, * FROM am_user " ;
    if (only_active_) {
        stmt += " WHERE  am_user.u_lc = 'active' ";
    }
    stmt += " ORDER BY " + p_order_by + " LIMIT " + limit_+ " OFFSET $1";    
    
    auto f = db_->execSqlAsyncFuture ( stmt, offset_);
    
    return f;
}

void Users_list::internal_build_json_from_row (Json::Value& p_jrow, const qry::Db_row& p_row)

{



        auto name = p_row["u_identifier"];
        p_jrow["identifier"] = Json::Value ( name.as<std::string>() );

        //auto role = row["au_role"];
        auto role = p_row["u_role"];
        p_jrow["role"] = Json::Value ( role.as<std::string>() );

        auto lc = p_row["u_lc"];
        p_jrow["lc_status"] = Json::Value ( lc.as<std::string>() );

        p_jrow["url"] = Json::Value ( qry::Util::url_prefix ("user") + p_jrow["identifier"].asString());

 
}

std::string Users_list::compute_order_by () {
    
    std::string order_by = (sort_ == "identifier") ? "u_identifier" :
                            (sort_ == "role") ?  "au_role" : 
                            "id";
                           
    
    order_by += " " + direction_;
 
 return order_by;
}
