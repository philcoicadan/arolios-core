/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/Factory.h>
#include <appcfg/App_domain.h>
#include <appcfg/Base_classifier_config.h>
#include <appcfg/Class_config.h>
#include <appcfg/Association_config.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Package.h>
#include <memory>




using namespace arolios;
using namespace arolios::appcfg;

Factory::Factory()
{

}



Factory::~Factory()
{

}

Factory& Factory::instance() {
    static Factory inst = Factory();
    return inst;
}


template <typename T1, typename T2, typename T3> std::shared_ptr<T2> Factory::make(const std::shared_ptr<T1>& p_elem, const std::shared_ptr<T3>& p_parent) 
{
    
    auto result = std::make_shared<T2>(p_elem, p_parent);
    
    return result;
}

template <> std::shared_ptr<Base_classifier_config> Factory::make(const std::shared_ptr<omm::Class>& p_elem, const std::shared_ptr<App_domain>& p_parent) 
{
    
    auto result = std::make_shared<Class_config>(p_elem, p_parent);
    
    return result;
}

template <> std::shared_ptr<Base_classifier_config> Factory::make(const std::shared_ptr<omm::Association>& p_elem, const std::shared_ptr<App_domain>& p_parent) 
{
    
    auto result = std::make_shared<Association_config>(p_elem, p_parent);
    
    return result;
}

std::shared_ptr<Base_classifier_config> Factory::make_classifier_config(const std::shared_ptr<omm::Classifier>& p_elem, const std::shared_ptr<App_domain>& p_parent) {

    std::shared_ptr<Base_classifier_config> result;

    auto cls = std::dynamic_pointer_cast<omm::Class>(p_elem);
    if (cls) {
        result = make<omm::Class, Base_classifier_config, App_domain> (cls, p_parent);
    } 
    else {
        auto assoc = std::dynamic_pointer_cast<omm::Association>(p_elem);
        if (assoc) {
            result = make<omm::Association, Base_classifier_config, App_domain> (assoc, p_parent);
        }
    }
    
	return result;

}

namespace arolios::appcfg {
template   std::shared_ptr<App_domain> Factory::make<omm::Package,App_domain, App>(const std::shared_ptr<omm::Package>& p_elem, const std::shared_ptr<App>& p_parent) ;
template   std::shared_ptr<Base_classifier_config> Factory::make<omm::Class,Base_classifier_config,App_domain>(const std::shared_ptr<omm::Class>& p_elem, const std::shared_ptr<App_domain>& p_parent) ;
template   std::shared_ptr<Base_classifier_config> Factory::make<omm::Association,Base_classifier_config,App_domain>(const std::shared_ptr<omm::Association>& p_elem, const std::shared_ptr<App_domain>& p_parent) ;
}
