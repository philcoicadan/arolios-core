/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Base_list_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>






using namespace arolios;
using namespace arolios::qry;

Base_list_query::Base_list_query ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang  ) 
: Base_query(p_lang),
om_ptr_(common::Singleton<common::Object_model_info>::instance().object()), app_info_ptr_( common::Singleton<common::App_info>::instance().object()) , sort_(p_sort) , direction_(p_direction)  { 

    offset_ =  (p_offset.empty() ) ? 0 : std::stol(p_offset); 
    limit_ = ( p_limit.empty()) ? "ALL" : p_limit ;
 
}


Base_list_query::~Base_list_query() { }


