/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <qry/Base_admin_element_list.h>

namespace arolios {







namespace omm {
    class Package;
    class Language;
}


namespace qry {


class App_domains_list  : public Base_admin_element_list {
public:
    /**
     * Constructor
     */
    App_domains_list( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang,  bool p_only_active = true
    ) ;


    /**
     * Destructor
     */
    virtual ~App_domains_list();

    

private:
    
   
      virtual std::string compute_order_by () override;
    
   bool order_by_lang_ ;

    virtual std::future<qry::Db_result> internal_execute (const std::string& p_order_by)  override;
    
    virtual void internal_build_json_from_row (Json::Value& p_jrow, const qry::Db_row& p_row) override;

 ;
    
};

}
}

