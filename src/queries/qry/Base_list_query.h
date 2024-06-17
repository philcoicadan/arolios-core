/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <qry/Base_query.h>

namespace arolios {



namespace util {
    class Instance_filter;
}

namespace qry {


class Base_list_query  : public Base_query {
public:
    /**
     * Constructor
     */
    Base_list_query( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction,  const std::shared_ptr<const omm::Language>& p_lang) ;

    /**
     * Destructor
     */
    virtual ~Base_list_query();



protected:

    virtual std::string compute_order_by () = 0;
    
    const  std::shared_ptr<common::Object_model_info> om_ptr_;
    const std::shared_ptr<common::App_info> app_info_ptr_;
    long offset_ ;
    std::string limit_;
    const std::string sort_;
    const std::string direction_;
    

    
      
    
private:
    

  
    
};

}
}

