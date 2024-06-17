/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <memory>

namespace arolios {



namespace omm {
    class Package;
    class Classifier;
}

namespace model {

class Classifier_search {
public:
	Classifier_search( const std::string& p_type_full_name, const std::shared_ptr<omm::Package>& p_parent_pk);
	virtual ~Classifier_search();
	template <typename T> std::shared_ptr<T> execute ();

    
protected:

private:
    const std::string type_full_name_;
    std::shared_ptr<omm::Package> search_pk_;

    
};

}

}

