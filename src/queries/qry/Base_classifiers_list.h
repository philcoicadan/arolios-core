/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <qry/Base_list_query.h>

namespace arolios {







namespace omm {
    class Package;
}


namespace qry {


class Base_classifiers_list  : public Base_list_query {
public:
    /**
     * Constructor
     */
    Base_classifiers_list( const std::vector<std::shared_ptr<const omm::Package>>& p_pk_vec, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang) ;

    /**
     * Destructor
     */
    virtual ~Base_classifiers_list();

   
    virtual void execute () override;
    
protected:
    virtual std::string compute_order_by () override;

    virtual void collect_classifiers (const std::shared_ptr<const omm::Package>& p_pk, std::vector<std::shared_ptr<const omm::Classifier>>& p_csf_vec) = 0;

    virtual const std::string& classifier_resource () = 0;
private:

    const std::vector<std::shared_ptr<const omm::Package>> pk_vec_;
   
    virtual void sort_result (std::vector<std::shared_ptr<const omm::Classifier>>& p_csf) ;

   
    
};

}
}

