/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/App_domains_list.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <exception/Exceptions.h>
#include <appcfg/App.h>
#include <util/String.h>
#include <qry/Util.h>
#include <json/value.h>
#include <qry/Db_types.h>






using namespace arolios;
using namespace arolios::qry ;

 App_domains_list::App_domains_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang , bool p_only_active
) : Base_admin_element_list ( p_offset, p_limit, p_sort, p_direction,p_lang, p_only_active), order_by_lang_(false)

{
}


 App_domains_list::~App_domains_list() { }




std::future<qry::Db_result> App_domains_list::internal_execute ( const std::string& p_order_by)  {
    
 
    
    auto default_app_id = app_info_ptr_->default_app()->id();
    auto std_domain = om_ptr_->standard_domain()->id();
    
    std::string stmt = "SELECT COUNT(*) OVER() AS _total, am_app_domain.*, am_app.name AS app_name, am_pk.name AS domain_name FROM am_app_domain INNER JOIN am_app ON am_app.id = am_app_domain.a_app INNER JOIN am_pk ON am_pk.id = am_app_domain.a_domain  LEFT JOIN am_pk_tlt ON am_pk_tlt.t_elem = am_pk.id WHERE am_app_domain.a_app <> $1 AND am_app_domain.a_domain <> $2 ORDER BY " + p_order_by + " LIMIT " + limit_+ " OFFSET $3";
    
         

    
    std::future<qry::Db_result>  f = (order_by_lang_) ?
        db_->execSqlAsyncFuture ( stmt, util::String::stoui(default_app_id), util::String::stoui(std_domain),offset_, util::String::stoui(lang_->id())) :
        db_->execSqlAsyncFuture ( stmt, util::String::stoui(default_app_id), util::String::stoui(std_domain),offset_);
    return f;
}
    
    
void App_domains_list::internal_build_json_from_row ( 
    Json::Value& p_jrow,
    const qry::Db_row& p_row ) {


    
 
        auto id_str = std::to_string(p_row["id"].as<unsigned int>() );
        p_jrow["url"] = Json::Value (  qry::Util::url_prefix ("appdomain") + id_str );
        
        p_jrow["app"] = Json::Value ( Json::objectValue ) ;
        Json::Value& jrow_app = p_jrow["app"];
        jrow_app["name"] = Json::Value ( p_row["app_name"].as<std::string>() );
        jrow_app["url"] = Json::Value (  qry::Util::url_prefix ("app") + jrow_app["name"].asString() );
        
        
        p_jrow["domain"] = Json::Value ( Json::objectValue ) ;
        Json::Value& jrow_domain = p_jrow["domain"];
        jrow_domain["name"] = Json::Value ( p_row["domain_name"].as<std::string>() );
        
        auto domain_id = p_row["a_domain"].as<unsigned int>() ;
        auto domain = om_ptr_->find_domain_by_id (std::to_string(domain_id)) ;
        if (domain) {
            jrow_domain["_tname"] = domain->translated_name(lang_);
        } else {
            std::string error_msg = "Domain " + std::to_string ( domain_id ) + " not found" ;
             
            throw exception::Not_found (error_msg) ;
        }
        jrow_domain["url"] = Json::Value ( qry::Util::url_prefix ("domain") + jrow_domain["name"].asString());


}

std::string App_domains_list::compute_order_by () {
  
    std::string order_by;
    if (sort_.empty()  ) {
        order_by =  "am_app_domain.id" ;
    } else if (sort_ == "domain") {
        order_by = "a_domain" ;
    } else if (sort_ == "app") {
        order_by = "a_app";
    } else if ( sort_ == "domain.name" ) {
        order_by = "COALESCE(am_translate_domain(a_domain,$4), domain_name::text)";
        order_by_lang_ = true;    
    } else if ( sort_ == "app.name" ) {
        order_by = "app_name" ;
    } else {
        std::cout << "WARNING: Appdomains list, sort ignored"<< std::endl;
         order_by =  "am_app_domain.id" ;
    }
    
    order_by += " " + direction_;
    
    return order_by;
}
    
    
    


