/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "omm/Composite_type.h"
#include "omm/Primitive_type.h"
#include "omm/Specific_type.h"
#include "omm/Selection_type.h"
#include <model/Classifier_search.h>
#include <omm/Package.h>
#include <omm/Enumeration.h>
#include <omm/Association.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>





using namespace arolios;
using namespace arolios::model;

    

Classifier_search::Classifier_search( const std::string& p_type_full_name, const std::shared_ptr<omm::Package>& p_parent_pk) : type_full_name_(p_type_full_name), search_pk_ (p_parent_pk)  {}

Classifier_search::~Classifier_search() {}

template <typename T> std::shared_ptr<T> Classifier_search::execute () {
    
    std::shared_ptr<T> search_type;
    
    if (type_full_name_.empty()) return search_type;
    
     auto path_vec = util::String::standardize ( util::String::split ( type_full_name_, omm::Named_element::separator() ) );
    

    std::string search_type_name;
    bool domain_specified= false;
    if ( path_vec.size() >1 ) {
        // domain is specified
        auto app_info = common::Singleton<common::App_info>::instance().object();
        search_pk_ = app_info->find_domain_by_name ( path_vec[0] );
        if ( search_pk_== nullptr ) {
            std::string error_msg = "Domain " + path_vec[0] + " for type " + type_full_name_ + " is not in the application scope" ;
            throw exception::Model_error ( error_msg );
        
        }
        domain_specified = true;
        search_type_name = path_vec[1];
    } else {
        search_type_name = path_vec [0];
    }

    //lookup first in search_pk

    std::shared_ptr<T> filtered_type = search_pk_->find_element_by_name<T> ( search_type_name ) ;
    
    search_type = filtered_type;


    if ( !search_type  ) {
        if ( ! domain_specified ) {
            // lookup in the standard types
            auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
            if ( mdl_info->standard_domain()) {
                search_type = std::dynamic_pointer_cast<T> ( mdl_info->find_standard_type_by_name ( search_type_name )) ;
            } // else  nothing to do , standard domain is loading, search_pk must be the standard domain
                
        }
    }
    if ( ! search_type ) {
        std::string error_msg = "Type " + search_type_name + " for property not found" ;
        throw exception::Model_error ( error_msg );
      
    }

    return search_type;
}

template std::shared_ptr<omm::Primitive_type> Classifier_search::execute<omm::Primitive_type>();
template std::shared_ptr<omm::Enumeration> Classifier_search::execute<omm::Enumeration>();
template std::shared_ptr<omm::Composite_type> Classifier_search::execute<omm::Composite_type>();
template std::shared_ptr<omm::Data_type> Classifier_search::execute<omm::Data_type>();
template std::shared_ptr<omm::Class> Classifier_search::execute<omm::Class>();
template std::shared_ptr<omm::Association> Classifier_search::execute<omm::Association>();
template std::shared_ptr<omm::Specific_type> Classifier_search::execute<omm::Specific_type>();
template std::shared_ptr<omm::Selection_type> Classifier_search::execute<omm::Selection_type>();
template std::shared_ptr<omm::Classifier> Classifier_search::execute<omm::Classifier>();
