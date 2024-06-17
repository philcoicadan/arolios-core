/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <qry/Base_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {






namespace mttr {
class Base_element_delete: public qry::Base_query {
public:
    /**
     * Constructor
     */
    Base_element_delete( const std::string& p_app_id );

    /**
     * Destructor
     */
    virtual ~Base_element_delete();

    
       
protected:
    const std::shared_ptr<common::App_info> app_info_ptr_;

    std::string elem_id_;

   
};
}
}

