/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/App_domain.h>
#include <omm/Package.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <appcfg/Base_classifier_config.h>
#include <appcfg/App.h>
#include <appcfg/Factory.h>


using namespace arolios;
using namespace arolios::appcfg;

App_domain::App_domain(const std::shared_ptr<omm::Package>& p_domain, const std::shared_ptr<App>& p_app)
: app_(p_app), domain_(p_domain) {

}

App_domain::~App_domain() {

}

const std::unordered_map<std::string,std::shared_ptr<Base_classifier_config>>& App_domain::app_classifiers () const{
    return app_classifiers_;
}

const std::shared_ptr<omm::Package>& App_domain::domain () const {
    return domain_;
}
const std::shared_ptr<App>& App_domain::app () const {
    return app_;
}

std::shared_ptr<Base_classifier_config> App_domain::find_app_classifier_by_name (const std::string& p_csf_name) const {
    std::shared_ptr<Base_classifier_config> result;
    auto search = app_classifiers_.find(p_csf_name);
    if (search != app_classifiers_.end()) {
        result = search->second;
    }
    return result;
}

void App_domain::create_select_templates()  {
    
    for (auto csf : domain_->classes()) {
        create_classifier_select_template<omm::Class> (csf.second);
        
    }
    
     for (auto csf : domain_->associations()) {
        create_classifier_select_template<omm::Association> (csf.second);
    }
    
    for (auto csf: domain_->classes()) {
        auto search = app_classifiers_.find(csf.first);
        if ( search != app_classifiers_.end()) {
            search->second->create_instance_associations_list_select_addon_template(app_);
        }
    }
}


template <typename T> void App_domain::create_classifier_select_template ( const std::shared_ptr<T>& p_csf ) {
    std::shared_ptr<Base_classifier_config> app_csf;
    auto search = app_classifiers_.find ( p_csf->name() );
    if ( search == app_classifiers_.end() ) { // not found => create
        auto && this_shared_ptr = app_->find_app_domain_by_name (domain_->name());
        app_csf = appcfg::Factory::instance().make<T,Base_classifier_config,App_domain> ( p_csf , this_shared_ptr);
        app_classifiers_.insert (std::make_pair<const std::string&, std::shared_ptr<Base_classifier_config>&>(p_csf->name(), app_csf));
    } else {
        app_csf = search->second;
    }
    app_csf->create_select_all_template(); 
    app_csf->create_list_select_template(); 
    app_csf->create_read_select_template();
} 

    

void App_domain::insert_classifier (const std::string& p_name, const std::shared_ptr<Base_classifier_config>& p_config_csf) {
           app_classifiers_.insert (std::make_pair<const std::string&, const std::shared_ptr<Base_classifier_config>&>(p_name, p_config_csf));
}  


