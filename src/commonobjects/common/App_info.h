/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <omm/Language.h>
#include <omm/Package.h>
#include <string>
#include <memory>
#include <json/json.h>

namespace arolios {




namespace appcfg {
    class App;
    class App_domain;
    class Base_classifier_config;
}

namespace common
{

class App_info 
{
  public:
      
    App_info() ;
    
    virtual ~App_info () ;

       
 	const std::shared_ptr<appcfg::App>& app () const ;
	void app (const std::shared_ptr<appcfg::App> & p_app) ;
   
    template <typename T1> void insert (const std::shared_ptr<T1>& p_elt) ;
    template <typename T1> void eliminate (const std::shared_ptr<T1>& p_elt) ;
    template <typename T1> void rename (const std::shared_ptr<T1>& p_elt, const std::string& p_new_name) ;


    std::shared_ptr<omm::Package> find_domain_by_name ( const std::string& p_pk_name, const bool p_only_active=true ) const ;
    std::shared_ptr<omm::Package> find_domain_by_id ( const std::string& p_pk_id, const bool p_only_active=true ) const ;
    
    std::shared_ptr<omm::Association> find_association_by_name (const std::string& p_pk_name, const std::string& p_csf_name, const bool p_only_active=true );
    
    std::shared_ptr<appcfg::App_domain> create_domain_config (const std::shared_ptr<omm::Package>& p_pk) ;
    void erase_domain_config (const std::shared_ptr<omm::Package>& p_pk);
    
    std::vector<std::shared_ptr<const omm::Package>> list_user_domains (const bool p_only_active=true ) const;
        
    template <typename T1> std::shared_ptr<T1> find_domain_element_by_name (const std::string& p_pk_name, const std::string& p_csf_name, const bool p_only_active=true) const ;
    template <typename T1> std::shared_ptr<T1> find_domain_element_by_full_name (const std::string& p_elem, const bool p_only_active=true) const ;
    
    
    std::shared_ptr<omm::Language> get_language_by_code (const std::string& p_lang) const ;
    std::shared_ptr<omm::Language> find_language_by_code (const std::string& p_lang) const ;
    std::shared_ptr<omm::Language> find_language_by_id (const std::string& p_id) const ;
    const std::shared_ptr<omm::Language>& default_language () const;

    std::string find_enum_value(const std::string& p_enum_value_id, const std::shared_ptr<const omm::Language>& p_lang, bool p_only_active=true) const ;
    void set_default_language ( const std::string& p_lang, const std::string& p_def_lang) ;
    void default_language ( const std::shared_ptr<omm::Language>& p_lang) ;

    void insert_language ( const std::shared_ptr<omm::Language>& p_lang) ;
    
    const std::unordered_map<std::string, std::shared_ptr<omm::Language>>& languages() const ;
    
    std::vector<std::shared_ptr<const omm::Language>> list_user_languages (const bool p_only_active=true ) const;
        
    const std::shared_ptr<appcfg::App>& default_app () const ;
    const std::string default_app_id () const ;
    
    const std::shared_ptr<omm::Package> root_domain () const ;

    void init_root () ;
    
protected:

    std::shared_ptr<appcfg::App> app_;
    std::unordered_map<std::string, std::shared_ptr<omm::Package>> pk_id_map_;
    std::shared_ptr<omm::Package> non_persistent_root_pk_ ;

    std::unordered_map<std::string, std::shared_ptr<omm::Language>> lang_code_map_;
    std::unordered_map<std::string, std::shared_ptr<omm::Language>> lang_id_map_;
    std::shared_ptr<omm::Language> default_language_;
    std::shared_ptr<appcfg::App> default_app_;
    std::string default_app_id_;

private:
    template <typename T> std::shared_ptr<appcfg::Base_classifier_config> create_classifier_config ( const std::shared_ptr<T>& p_csf, const std::shared_ptr<appcfg::App_domain>& p_domain_config  ) ;
    
    std::shared_ptr<omm::Packageable_element> find_domain_element  (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active=true) const ;
    

    
	


};

}
}

