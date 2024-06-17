/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/App_config_composite.h>
#include <appcfg/Base_classifier_config.h>
#include <model/App_element_config.h>
#include <model/Base_element.h>
#include <model/Element_factory.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <model/appconfig.h>
#include <omm/Association.h>
#include <omm/Class.h>
#include <omm/Property.h>
#include <appcfg/App_domain.h>







 namespace arolios::model {

template < typename T1, typename T2, typename T3, template<typename M1, typename M2, typename M3> typename MOD> App_config_composite<T1,T2 ,T3,MOD>::App_config_composite( Base_element& p_parent, const std::shared_ptr<appcfg::App>& p_app) : parent_(p_parent), app_ (p_app) {
	

}

template < typename T1, typename T2, typename T3, template<typename M1, typename M2, typename M3> typename MOD> App_config_composite<T1,T2 , T3,MOD>::~App_config_composite() {
	
}



template < typename T1, typename T2, typename T3, template<typename M1, typename M2, typename  M3> typename MOD> std::shared_ptr<MOD<T1,T2,T3>> App_config_composite<T1,T2 ,T3,MOD>::insert_child (const T1& p_elt, const std::string& p_name) {

    std::shared_ptr<MOD<T1,T2, T3>> elt_config = nullptr;
    std::string && t_name = util::String::standardize(p_name);

    const auto stored_parent = std::dynamic_pointer_cast<omm::Namespace> (parent_.stored_element());
    std::shared_ptr<T2> t_stored = ( stored_parent)  ? 
            stored_parent->template find_element_by_name<T2>(t_name) :
            nullptr;
    
    
        if (t_stored) {

            elt_config = 
                Element_factory::instance().make_element_config<T1,MOD<T1,T2,T3>,T2, T3> ( p_elt, t_stored, parent_, app_) ;
            parent_.add_child(elt_config);


        } else {      
            std::string msg ( "element " + t_name + " : not found, action is not possible." );
            throw exception::Model_error ( msg );
        }


           
    return elt_config;

    
}

template <> std::shared_ptr<App_element_config<std::string, omm::Property, appcfg::Base_classifier_config>> App_config_composite<std::string, omm::Property, appcfg::Base_classifier_config, App_element_config>::insert_child (const std::string& p_elt, const std::string& p_name) {

    std::shared_ptr<App_element_config<std::string, omm::Property, appcfg::Base_classifier_config>> elt_config = nullptr;
    std::string && t_name = util::String::standardize(p_name);

    const auto stored_parent = std::dynamic_pointer_cast<omm::Classifier> (parent_.stored_element());
    std::shared_ptr<omm::Property> t_stored = ( stored_parent)  ? 
            stored_parent->find_element_in_all_properties(t_name) :
            nullptr;
    
    
        if (t_stored) {

            elt_config = 
                Element_factory::instance().make_element_config<std::string,App_element_config<std::string, omm::Property, appcfg::Base_classifier_config>,omm::Property, appcfg::Base_classifier_config> ( p_elt, t_stored, parent_, app_) ;
            parent_.add_child(elt_config);


        } else {      
            std::string msg ( "element " + t_name + " : not found, action is not possible." );
            throw exception::Model_error ( msg );
        }


           
    return elt_config;

    
}
template class App_config_composite<Classifier_c, omm::Association, appcfg::Base_classifier_config, App_element_config> ;
template class App_config_composite<Class_c, omm::Class, appcfg::Base_classifier_config, App_element_config> ;
template class App_config_composite<std::string, omm::Property, appcfg::Base_classifier_config, App_element_config> ;

 }
 


