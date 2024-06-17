/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <qry/Base_classifiers_list.h>

namespace arolios {







namespace omm {
    class Package;
}


namespace qry {


template< typename T> class Classifiers_list  : public Base_classifiers_list {
public:
    /**
     * Constructor
     */
    Classifiers_list( const std::vector<std::shared_ptr<const omm::Package>>& p_pk_vec, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang) ;

    /**
     * Destructor
     */
    virtual ~Classifiers_list();

protected:

    virtual void collect_classifiers (const std::shared_ptr<const omm::Package>& p_pk, std::vector<std::shared_ptr<const omm::Classifier>>& p_csf_vec) override;
    virtual const std::string& classifier_resource () override;

private:
    
   
};

}
}

