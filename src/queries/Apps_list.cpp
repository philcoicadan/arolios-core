/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/App.h>
#include <qry/Apps_list.h>
#include <qry/Util.h>





using namespace arolios;
using namespace arolios::qry ;

Apps_list::Apps_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang , bool p_only_active
) : Base_admin_element_list ( p_offset, p_limit, p_sort, p_direction,p_lang, p_only_active)

{
}


Apps_list::~Apps_list() { }

std::future<qry::Db_result> Apps_list::internal_execute (const std::string& p_order_by) 
{
    
    auto default_app = app_info_ptr_->default_app() ;
    
    std::string stmt = "SELECT COUNT(*) OVER() AS _total, * FROM am_app WHERE id <> " + default_app->id() + " ORDER BY " + p_order_by + " LIMIT " + limit_+ " OFFSET $1";    
    
    
    
    
    auto f = db_->execSqlAsyncFuture ( stmt, offset_);

    return f;
}

void Apps_list::internal_build_json_from_row (Json::Value& p_jrow, const qry::Db_row& p_row)
{



        auto name = p_row["name"];
        p_jrow["name"] = Json::Value ( name.as<std::string>() );

        auto descr = p_row["description"];
        if ( !descr.isNull() ) {
            p_jrow["description"] = Json::Value ( descr.as<std::string>() );
        }
        auto id = p_row["id"].as<unsigned int>() ;
        p_jrow["url"] = Json::Value (  qry::Util::url_prefix("app") + p_jrow["name"].asString()) ;
        

}

std::string Apps_list::compute_order_by () {
    std::string order_by = (sort_.empty() || sort_ != "name" || sort_ != "description") ? "id" : sort_;
    
    order_by += " " + direction_;
    
    return order_by;
}

