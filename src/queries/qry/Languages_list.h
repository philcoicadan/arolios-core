/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <qry/Base_list_query.h>

namespace arolios {






namespace omm {
    class Language;

}

namespace qry {

    class Instance_filter;


class Languages_list  : public Base_list_query {
public:
    /**
     * Constructor
     */
    Languages_list( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction , std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang
    ) ;


    /**
     * Destructor
     */
    virtual ~Languages_list();

    
    virtual void execute () override;
    
protected: 
  virtual std::string compute_order_by () override;

private:
    
   
    void sort_result (std::vector<std::shared_ptr<const omm::Language>>& p_languages);
    
    const std::vector<std::shared_ptr<const qry::Instance_filter>> filters_;
    
};

}
}

