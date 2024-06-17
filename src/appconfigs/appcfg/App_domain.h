/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <omm/Persistent_object.h>

namespace arolios {

namespace omm {
    class Package;
    class Classifier;
}


namespace appcfg {
    class Base_classifier_config;
    class App;
class App_domain {
public:
    /**
     * Constructor
     */
    App_domain(const std::shared_ptr<omm::Package>& p_domain, const std::shared_ptr<App>& p_app);

    /**
     * Destructor
     */
    ~App_domain();
    
    const std::shared_ptr<omm::Package>& domain () const;
    const std::shared_ptr<App>& app () const;
    
    const std::unordered_map<std::string,std::shared_ptr<Base_classifier_config>>& app_classifiers () const;
    
    void insert_classifier (const std::string& p_name, const std::shared_ptr<Base_classifier_config>& p_config_csf);
    
    std::shared_ptr<Base_classifier_config> find_app_classifier_by_name (const std::string& p_csf_name) const ;
    
    void create_select_templates () ;  
    

    
private:
    template <typename T> void create_classifier_select_template ( const std::shared_ptr<T>& p_csf ) ;
    
        
    std::shared_ptr<App> app_;
    std::shared_ptr<omm::Package> domain_;
    std::unordered_map<std::string,std::shared_ptr<Base_classifier_config>> app_classifiers_;
    

};
}
}

