/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>

namespace arolios {

namespace omm {
    class Classifier;
}

namespace appcfg {

    class App_domain;
    class Base_classifier_config;

class Factory
{
public:
    

    static Factory& instance();

    std::shared_ptr<Base_classifier_config> make_classifier_config (const std::shared_ptr<omm::Classifier>& p_csf, const std::shared_ptr<App_domain>& p_domain_cfg);
    template <typename T1, typename T2, typename T3> std::shared_ptr<T2> make (const std::shared_ptr<T1>& p_elem, const std::shared_ptr<T3>& p_parent);

private:
	
	Factory();
	~Factory();
};
}}

