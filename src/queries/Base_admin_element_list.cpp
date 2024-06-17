/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Base_admin_element_list.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <appcfg/App.h>







using namespace arolios;
using namespace arolios::qry;

Base_admin_element_list::Base_admin_element_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang , bool p_only_active
) : Base_list_query ( p_offset, p_limit, p_sort, p_direction,p_lang),  only_active_ (p_only_active)

{
}


Base_admin_element_list::~Base_admin_element_list() { }

void Base_admin_element_list::execute ()  {
    
 
   
    
    const std::string order_by = compute_order_by();

    auto f = internal_execute (order_by);

  
    auto result = f.get();
    
     build_json (result) ;
    

}

 
void Base_admin_element_list::build_json ( const qry::Db_result& p_result ) {

    int total = 0;
    if ( p_result.size() > 0 ) {
        auto first = p_result[0] ;
        total = first["_total"].as<int>();
    }

    return_json_ ["total"] = total;
    return_json_["elements"] = Json::Value ( Json::arrayValue );

    Json::Value& jrows = return_json_["elements"];

    jrows.resize ( p_result.size() );
    
    int i = 0;
    for ( auto row : p_result ) {
        jrows[i] =  Json::Value ( Json::objectValue ) ;
        Json::Value& jrow = jrows[i++];

        internal_build_json_from_row ( jrow, row);
        
    }

    return_code_ = qry::Return_code::OK;

}

    
            


