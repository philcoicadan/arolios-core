/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Return_code.h>
#include <qry/Util.h>
#include <json/json.h>
#include <memory>
#include <common/Db_client.h>

namespace arolios {


namespace omm {
    class Language;
}

namespace qry {

class Base_query {
public:
    /**
     * Constructor
     */
    Base_query () ;
    Base_query( const std::shared_ptr<const omm::Language>& p_lang );

    /**
     * Destructor
     */
    virtual ~Base_query();
    
     
    qry::Return_code return_code () const;
    const Json::Value& return_json () const;
    
    virtual void execute () = 0;

    
protected:

    std::shared_ptr<const omm::Language> lang_;

    Json::Value return_json_;
    qry::Return_code return_code_;
    const std::shared_ptr<common::Db_client> db_;


};
}
}

