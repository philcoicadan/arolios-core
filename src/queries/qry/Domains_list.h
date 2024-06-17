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
    class Package;
    class Language;
}

namespace qry {


class Domains_list  : public Base_list_query {
public:
    /**
     * Constructor
     */
    Domains_list( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction , const std::shared_ptr<const omm::Language>& p_lang
    ) ;


    /**
     * Destructor
     */
    virtual ~Domains_list();

    
    virtual void execute () override;
    
protected:
    
    void internal_execute () ;

    virtual std::string compute_order_by () override;
    
    std::vector<std::shared_ptr<const omm::Package>> domains_;

private:
    
    
    void sort_result () ;
    
};

}
}

