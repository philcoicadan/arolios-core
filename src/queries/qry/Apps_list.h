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









namespace qry {


class Apps_list  : public Base_admin_element_list {
public:
    /**
     * Constructor
     */
    Apps_list( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang, bool p_only_active = true) ;

    /**
     * Destructor
     */
    virtual ~Apps_list();

protected:
    
  virtual std::string compute_order_by () override;
    
   

    virtual std::future<qry::Db_result> internal_execute (const std::string& p_order_by)  override;
    
    virtual void internal_build_json_from_row (Json::Value& p_jrow, const qry::Db_row& p_row) override;

    

private:
  
    
};

}
}

