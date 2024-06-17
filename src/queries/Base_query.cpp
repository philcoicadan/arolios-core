/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Base_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>




using namespace arolios;
using namespace arolios::qry;

Base_query::Base_query( ) : db_ (common::Singleton<common::Db_client>::instance().object())
{
    auto app_ptr = common::Singleton<common::App_info>::instance().object();
    lang_ = app_ptr->default_language();
    return_code_ = qry::Return_code::KO;
    return_json_ = Json::Value(Json::objectValue);
}
Base_query::Base_query( const std::shared_ptr<const omm::Language>& p_lang ) : lang_ (p_lang),
db_ (common::Singleton<common::Db_client>::instance().object())
{
    return_code_ = qry::Return_code::KO;
    return_json_ = Json::Value(Json::objectValue);
    
}

Base_query::~Base_query() {

}

qry::Return_code Base_query::return_code () const {
    return return_code_;
}

const Json::Value& Base_query::return_json () const {
    return return_json_;
}
    

