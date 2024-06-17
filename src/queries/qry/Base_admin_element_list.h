/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <qry/Base_list_query.h>
#include <qry/Db_types.h>

namespace arolios {







namespace qry {


class Base_admin_element_list  : public Base_list_query {
public:
    /**
     * Constructor
     */
    Base_admin_element_list(const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang, bool p_only_active=true) ;

    /**
     * Destructor
     */
    virtual ~Base_admin_element_list();

   
    virtual void execute () override;
    
protected:
    
    
    void build_json (const qry::Db_result& p_result) ;

    virtual std::future<qry::Db_result> internal_execute (const std::string& p_order_by) = 0;
    
    virtual void internal_build_json_from_row (Json::Value& p_jrow, const qry::Db_row& p_row) = 0;

    bool only_active_;

private:
    
   
    
};

}
}

