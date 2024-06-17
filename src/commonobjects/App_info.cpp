/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <qry/App_retriever.h>
#include <qry/Model_retriever.h>
#include <qry/Init_retriever.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <omm/Classifier.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Enumeration.h>
#include <omm/Data_type.h>
#include <omm/Language.h>
#include <omm/Enum_value.h>
#include <omm/Package.h>
#include <appcfg/App.h>
#include <appcfg/App_domain.h>
#include <appcfg/Base_classifier_config.h>
#include <appcfg/Factory.h>
#include <omm/Model_factory.h>







using namespace arolios;
using namespace arolios::common;

App_info::App_info() {
   


}

App_info::~App_info () {
    
}

template <typename T> void App_info::insert (const std::shared_ptr<T>& p_elt)  {
    
// nothing to do
}

template <typename T> void App_info::rename (const std::shared_ptr<T>& p_elt, const std::string& p_new_name) {

// nothing to do

}

template <typename T> void App_info::eliminate (const std::shared_ptr<T>& p_elt) {

    // nothing to do

}

template void App_info::insert<omm::Class> (const std::shared_ptr<omm::Class>& p_elt) ;
template void App_info::insert<omm::Data_type> (const std::shared_ptr<omm::Data_type>& p_elt) ;
template void App_info::insert<omm::Enumeration> (const std::shared_ptr<omm::Enumeration>& p_elt) ;
template void App_info::insert<omm::Association> (const std::shared_ptr<omm::Association>& p_elt) ;
template void App_info::insert<omm::Property> (const std::shared_ptr<omm::Property>& p_elt) ;
template void App_info::insert<omm::Enum_value> (const std::shared_ptr<omm::Enum_value>& p_elt) ;

template void App_info::rename<omm::Class> (const std::shared_ptr<omm::Class>& p_elt, const std::string& p_new_name) ;
template void App_info::rename<omm::Data_type> (const std::shared_ptr<omm::Data_type>& p_elt, const std::string& p_new_name) ;
template void App_info::rename<omm::Enumeration> (const std::shared_ptr<omm::Enumeration>& p_elt, const std::string& p_new_name) ;
template void App_info::rename<omm::Association> (const std::shared_ptr<omm::Association>& p_elt, const std::string& p_new_name) ;
template void App_info::rename<omm::Property> (const std::shared_ptr<omm::Property>& p_elt, const std::string& p_new_name) ;
template void App_info::rename<omm::Enum_value> (const std::shared_ptr<omm::Enum_value>& p_elt, const std::string& p_new_name) ;


template void App_info::eliminate<omm::Class> (const std::shared_ptr<omm::Class>& p_elt) ;
template void App_info::eliminate<omm::Data_type> (const std::shared_ptr<omm::Data_type>& p_elt) ;
template void App_info::eliminate<omm::Enumeration> (const std::shared_ptr<omm::Enumeration>& p_elt) ;
template void App_info::eliminate<omm::Association> (const std::shared_ptr<omm::Association>& p_elt) ;
template void App_info::eliminate<omm::Property> (const std::shared_ptr<omm::Property>& p_elt) ;
template void App_info::eliminate<omm::Enum_value> (const std::shared_ptr<omm::Enum_value>& p_elt) ;


template <> void App_info::insert (const std::shared_ptr<omm::Package>& p_pk)  {
    
   pk_id_map_.insert(std::make_pair<const std::string&,const std::shared_ptr<omm::Package>&> ( p_pk->id(), p_pk ));
   
   non_persistent_root_pk_->insert_element (p_pk);

    
}

template <> void App_info::rename (const std::shared_ptr<omm::Package>& p_domain, const std::string& p_new_name) {
  if (p_new_name != p_domain->name()) {
    // replace in the map only if it exists
    const auto pk = find_domain_by_name(p_domain->name());
    if (pk) {
      non_persistent_root_pk_->rename<omm::Package>(p_domain, p_new_name);
    }
  }
}

template <> void App_info::eliminate (const std::shared_ptr<omm::Package>& p_domain) {

     non_persistent_root_pk_->erase_element (p_domain);
     pk_id_map_.erase (p_domain->name());
}
  

std::vector<std::shared_ptr<const omm::Package>> App_info::list_user_domains ( const bool p_only_active) const {
    
    std::vector<std::shared_ptr<const omm::Package>> result;
    const auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
    const std::shared_ptr<const omm::Package> std_dom =   mdl_info->standard_domain();
    for ( auto [first,second] : pk_id_map_) {
        if ( second != std_dom  && (!p_only_active || (second->lc_status() == omm::Lc_status::active))) {
            result.push_back(second);
        }
    }
    return result;
}


std::shared_ptr<omm::Package> App_info::find_domain_by_name ( const std::string& p_pk_name, const bool p_only_active) const {
    

    auto pk =  non_persistent_root_pk_->find_element_by_name<omm::Package> (p_pk_name);
    if (pk == nullptr) {
        return nullptr;
    }
       return (!p_only_active || pk->lc_status() == omm::Lc_status::active)  ? pk : nullptr;
}

std::shared_ptr<omm::Package> App_info::find_domain_by_id ( const std::string& p_pk_id, const bool p_only_active) const {
    auto search = pk_id_map_.find ( p_pk_id ) ;
    if ( search == pk_id_map_.end() ) {
        return nullptr;
    }
    return (!p_only_active || (search->second->lc_status() == omm::Lc_status::active) ) ? search->second : nullptr;

}


std::shared_ptr<omm::Packageable_element> App_info::find_domain_element  (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active) const {
 

    auto pk = non_persistent_root_pk_->find_element_by_name<omm::Package> (p_pk_name);
    if (pk == nullptr ) { 
        return nullptr;
    }
    if ( p_only_active && pk->lc_status() != omm::Lc_status::active) {
        return nullptr;
    }
    
    const auto & map = pk->package_elements();
    auto && search2 = map.find ( p_elem_name );
    if ( search2 == map.end() ) {
        return nullptr;
    }

    return (!p_only_active || (search2->second->lc_status() == omm::Lc_status::active) ) ? search2->second : nullptr;
}

template<typename T1>  std::shared_ptr<T1> App_info::find_domain_element_by_name (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active) const {
    
    return std::dynamic_pointer_cast<T1>(find_domain_element(p_pk_name, p_elem_name, p_only_active));
 
}

template std::shared_ptr<omm::Classifier> App_info::find_domain_element_by_name<omm::Classifier> (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Class> App_info::find_domain_element_by_name<omm::Class> (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Association> App_info::find_domain_element_by_name<omm::Association> (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Enumeration> App_info::find_domain_element_by_name<omm::Enumeration> (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Data_type> App_info::find_domain_element_by_name<omm::Data_type> (const std::string& p_pk_name, const std::string& p_elem_name, const bool p_only_active) const ;


template <typename T1> std::shared_ptr<T1> App_info::find_domain_element_by_full_name (const std::string& p_elem, const bool p_only_active) const {
    const auto vec = util::String::standardize ( util::String::split (p_elem, '.'));
    if (vec.size() != 2) {
        const std::string error_msg = "Domain element name is invalid";
         
        throw exception::Model_error( error_msg);
    }
    return find_domain_element_by_name<T1>(vec[0], vec[1], p_only_active);
}

template std::shared_ptr<omm::Classifier> App_info::find_domain_element_by_full_name<omm::Classifier> (const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Class> App_info::find_domain_element_by_full_name<omm::Class> (const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Association> App_info::find_domain_element_by_full_name<omm::Association> (const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Enumeration> App_info::find_domain_element_by_full_name<omm::Enumeration> (const std::string& p_elem_name, const bool p_only_active) const ;

template std::shared_ptr<omm::Data_type> App_info::find_domain_element_by_full_name<omm::Data_type> (const std::string& p_elem_name, const bool p_only_active) const ;




const std::shared_ptr<appcfg::App>& App_info::app () const {
    return app_; 
    
}

void App_info::app (const std::shared_ptr<appcfg::App> & p_app) {
    app_ = p_app;
}
   

std::shared_ptr<appcfg::App_domain> App_info::create_domain_config (const std::shared_ptr<omm::Package>& p_pk) {
    
    
    
    std::shared_ptr<appcfg::App_domain> app_dom;
    if ( app_ ) {
        auto search = app_->app_domains().find ( p_pk->name() );
        if ( search == app_->app_domains().end() ) { // not found => create
            app_dom = std::make_shared<appcfg::App_domain> ( p_pk, app_ );
            app_->app_domains().insert ( std::make_pair<const std::string&, std::shared_ptr<appcfg::App_domain>&> ( p_pk->name(), app_dom ) );
        } else {
            app_dom = search->second;
        }
        for (auto csf : p_pk->classes()) {
            //create_classifier_config<omm::Class,appcfg::Classifier_config> (csf.second, app_dom);
            create_classifier_config<omm::Class> (csf.second, app_dom);
        }
    
        for (auto csf : p_pk->associations()) {
            //create_classifier_config<omm::Association,appcfg::Classifier_config> (csf.second, app_dom);
            create_classifier_config<omm::Association> (csf.second, app_dom);
        }
     
    } else {
        std::cout << "WARNING: application not yet specified" << std::endl;
    }
    return app_dom;
}

void App_info::erase_domain_config (const std::shared_ptr<omm::Package>& p_pk) {
     if ( app_ ) app_->app_domains().erase (p_pk->name()) ;
}


template <typename T> std::shared_ptr<appcfg::Base_classifier_config> App_info::create_classifier_config ( const std::shared_ptr<T>& p_csf, const std::shared_ptr<appcfg::App_domain>& p_domain_config  ) {
    
    std::shared_ptr<appcfg::Base_classifier_config> app_csf;
    auto search = p_domain_config->app_classifiers().find ( p_csf->name() );
    if ( search == p_domain_config->app_classifiers().end() ) { // not found => create
        app_csf = appcfg::Factory::instance().make_classifier_config ( p_csf , p_domain_config);

        p_domain_config->insert_classifier (p_csf->name(), app_csf);
    } else {
        app_csf = search->second;
    }
    return app_csf;
}


std::shared_ptr<omm::Language> App_info::get_language_by_code (const std::string& p_lang) const {

    if ( p_lang.empty() ) {
        return default_language();
    } else {
        auto lang = find_language_by_code ( p_lang );
        if ( lang == nullptr ) {
            std::cout << "WARNING: Language " << p_lang << " not found, the default language is applied" << std::endl;
            return default_language();
        } else {
            return lang;
        }
    }
}

std::shared_ptr<omm::Language> App_info::find_language_by_code (const std::string& p_lang) const {
    std::shared_ptr<omm::Language> result ;

    auto lang = util::String::standardize ( p_lang );
    auto search = lang_code_map_.find ( lang ) ;
    if ( search != lang_code_map_.end() ) {
        result = search->second;
    }
    return result;
}

std::shared_ptr<omm::Language> App_info::find_language_by_id (const std::string& p_id) const {
    std::shared_ptr<omm::Language> result ;

    auto search = lang_id_map_.find ( p_id ) ;
    if ( search != lang_id_map_.end() ) {
        result = search->second;
    }
    return result;
}

const std::shared_ptr<omm::Language>& App_info::default_language () const {
    return default_language_;
}

void App_info::default_language (const std::shared_ptr<omm::Language>& p_lang) {
    default_language_ = p_lang;
}

void App_info::set_default_language ( const std::string& p_lang, const std::string& p_default_lang) {

    auto search = lang_code_map_.find(p_lang) ;
           
    if (search == lang_code_map_.end()) {
        std::cout << "WARNING: Language " << p_lang << " not found, the default language is applied" << std::endl;
        search = lang_code_map_.find(p_default_lang);
        if (search == lang_code_map_.end()) {
            throw exception::Not_found("bad retrieval of languages") ;
        } else {
            default_language( search->second);
        }
    } else {
        default_language( search->second);
    }
        
}



void App_info::insert_language ( const std::shared_ptr<omm::Language>& p_lang) {
    lang_code_map_.insert(std::make_pair<const std::string&,const std::shared_ptr<omm::Language>&> ( p_lang->code(), p_lang ));
    lang_id_map_.insert(std::make_pair<const std::string&,const std::shared_ptr<omm::Language>&> ( p_lang->id(), p_lang ));
}

const std::unordered_map<std::string, std::shared_ptr<omm::Language>>& App_info::languages() const {
    return lang_code_map_;
}

    
std::vector<std::shared_ptr<const omm::Language>> App_info::list_user_languages ( const bool p_only_active) const {
    
    std::vector<std::shared_ptr<const omm::Language>> result;

    for ( auto [first,second] : lang_id_map_) {
        if ( !p_only_active || (second->lc_status() == omm::Lc_status::active)) {
            result.push_back(second);
        }
    }
    return result;
}    
    
    
const std::shared_ptr<appcfg::App>& App_info::default_app () const {
    return default_app_;
}

const std::string App_info::default_app_id () const {
    return default_app_id_;
}

const std::shared_ptr<omm::Package> App_info::root_domain () const {
    return non_persistent_root_pk_;
}


std::string App_info::find_enum_value(const std::string& p_enum_value_id, const std::shared_ptr<const omm::Language>& p_lang, bool p_only_active) const {
        
        std::string result ;
        auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();

        const auto enum_value =  mdl_info->find_enum_value_by_id (p_enum_value_id, p_only_active);

    if (enum_value) {
        result = enum_value->translated_name(p_lang);
    }

    return result;
}

void App_info::init_root ()
{
    // create root package for gathering the app domains
    non_persistent_root_pk_ = omm::Model_factory::instance().make_package("app_root");
  
}