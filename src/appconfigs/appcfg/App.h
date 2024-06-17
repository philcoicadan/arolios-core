/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <omm/Persistent_object.h>

namespace arolios {

namespace omm {
    class Language;
}

namespace appcfg {
    class App_domain;
    class Base_classifier_config;
class App : public omm::Persistent_object {
public:
    /**
     * Constructor
     */
    App();

    /**
     * Destructor
     */
    virtual ~App();
    
    std::string name() const;
    void name (const std::string& p_name) ;
    
    std::unordered_map<std::string,std::shared_ptr<App_domain>>& app_domains() ;    //std::unordered_map<std::string,std::shared_ptr<omm::Language>>& app_languages() ;
    
    std::shared_ptr<Base_classifier_config> find_config_by_name (const std::string& p_pk_name, const std::string& p_csf_name) const ;
    
    std::shared_ptr<App_domain> find_app_domain_by_name (const std::string& p_pk_name) const ;
    
    void update_domain_config (const std::shared_ptr<App_domain>& p_dom_cfg) ;
    
    
private:
    std::string name_;
    std::unordered_map<std::string,std::shared_ptr<App_domain>>  app_domains_;
    

    
    

};
}

}

