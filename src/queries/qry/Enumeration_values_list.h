/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <json/json.h>
#include <qry/Base_list_query.h>

namespace arolios {










namespace omm {
    class Enumeration;
    class Enum_value;

}

namespace appcfg {
    class Base_classifier_config;
}

namespace qry {


class Enumeration_values_list  : public Base_list_query {
public:
    /**
     * Constructor
     */
    Enumeration_values_list( const std::shared_ptr<const omm::Enumeration>& p_enum, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang 
    )  ;


    /**
     * Destructor
     */
    virtual ~Enumeration_values_list();

    
    virtual void execute () override;
    

protected:

  
 virtual std::string compute_order_by () override;
    
    
private:
    
    
    std::shared_ptr<const omm::Enumeration> enum_;

    
    void sort_result (std::vector<std::shared_ptr<const omm::Enum_value>>& p_values) ;
    
    
    
};

}
}

