/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/App.h>
#include <appcfg/App_domain.h>
#include <appcfg/Base_classifier_config.h>
#include <omm/Package.h>



using namespace arolios;
using namespace arolios::appcfg;

App::App() {

}

App::~App() {

}

   
std::string App::name() const {
    return name_;
}
    
void App::name (const std::string& p_name) {
    name_ = p_name;
}

std::unordered_map<std::string,std::shared_ptr<App_domain>>& App::app_domains() {
    return app_domains_;
}

    
std::shared_ptr<App_domain> App::find_app_domain_by_name (const std::string& p_pk_name) const {
    std::shared_ptr<App_domain> result;
    auto search = app_domains_.find(p_pk_name);
    if (search != app_domains_.end()) {
        result = search->second;
    }
    return result;
}

std::shared_ptr<Base_classifier_config> App::find_config_by_name ( const std::string& p_pk_name, const std::string& p_csf_name  ) const {
    std::shared_ptr<Base_classifier_config> result ;
    
    auto app_domain =  find_app_domain_by_name(p_pk_name);
    if (app_domain) {
        result = app_domain->find_app_classifier_by_name(p_csf_name);
    }
    return result;
}
        
void App::update_domain_config (const std::shared_ptr<App_domain>& p_dom_cfg) {
    

    
    auto search = app_domains_.insert_or_assign(p_dom_cfg->domain()->name(),p_dom_cfg );
    
    p_dom_cfg->create_select_templates();
    
    
}
    
