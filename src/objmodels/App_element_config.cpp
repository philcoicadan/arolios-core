/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/App_element_config.h>
#include <appcfg/Base_classifier_config.h>
#include <model/App_config_composite.h>
#include <model/Model_recorder.h>
#include <model/Element_action.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <omm/Model_factory.h>
#include <common/Db_client.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <omm/Packageable_element.h>
#include <model/appconfig.h>
#include "Model_element.tpp"
#include <appcfg/App_domain.h>
#include <appcfg/Factory.h>









namespace arolios::model {


template < typename T1, typename T2 , typename T3> App_element_config<T1,T2,T3>::App_element_config(const T1& p_elem, const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element, const std::shared_ptr<appcfg::App>& p_app ):Model_element<T1,T2 > (p_elem,p_stored->name(), p_stored, p_parent_element, Element_action::a_update), app_ (p_app) {
	}


template <typename T1, typename T2, typename T3 > App_element_config<T1,T2,T3 >::~App_element_config() {
}


template <typename T1, typename T2 , typename T3> void App_element_config<T1,T2,T3 >::execute () {
    
    Model_element<T1,T2>::execute ();
}




template < typename T1, typename T2 , typename T3>  void App_element_config<T1,T2, T3 >::internal_execute ( const omm::Element_attributes& p_attrs ) {
    
    const App_element_config<Domain_c,omm::Package,appcfg::App_domain>& parent = static_cast<const App_element_config<Domain_c,omm::Package,appcfg::App_domain>&>( this->parent());
    std::shared_ptr<appcfg::App_domain>  parent_app_elt = parent.app_element();
    this->app_elt_ = appcfg::Factory::instance().make<T2,T3,appcfg::App_domain>(this->stored_, parent_app_elt );
    parent_app_elt->insert_classifier(this->stored_->name(), this->app_elt_);

}


template <>  void App_element_config<std::string,omm::Property,appcfg::Base_classifier_config>::internal_execute ( const omm::Element_attributes& p_attrs ) {



    
        const App_element_config<Class_c,omm::Class,appcfg::Base_classifier_config > & parent = static_cast<const App_element_config<Class_c,omm::Class,appcfg::Base_classifier_config>&>( this->parent());
    std::shared_ptr<appcfg::Base_classifier_config>  parent_app_elt = parent.app_element();
    
   
        parent_app_elt-> insert_collection_props( this->stored_->name(), this->stored_ );

    }




template <>  void App_element_config<Domain_c,omm::Package,appcfg::App_domain >::internal_execute ( const omm::Element_attributes& p_attrs ) {

    
    this->app_elt_ = std::make_shared<appcfg::App_domain>(this->stored_, app_);
    
    


}

template <typename T1, typename T2, typename T3 > void App_element_config<T1,T2,T3 >::accept (Visitor& p_visitor) {

}


template <typename T1, typename T2 , typename T3> void App_element_config<T1,T2,T3 >::fill_children() {
 // by default , nothing to do
}


template <> void App_element_config<Domain_c, omm::Package, appcfg::App_domain >::fill_children ()  {

    

	//Classes
    App_config_composite<Class_c, omm::Class, appcfg::Base_classifier_config, App_element_config > cont_class (*this, app_);
	for (Domain_c::class_const_iterator i (this->xml_obj_.class_().begin ());
			i != this->xml_obj_.class_ ().end ();
			++i)
	{
		cont_class.insert_child (*i, (*i).name());
	}

	//Associations
    App_config_composite<Classifier_c, omm::Association, appcfg::Base_classifier_config, App_element_config> cont_assoc (*this, app_);
	for (Domain_c::association_const_iterator i (this->xml_obj_.association().begin ());
			i != this->xml_obj_.association().end ();
			++i)

	{
        
        cont_assoc.insert_child(*i, (*i).name());
    }
    
    
}

template <> void App_element_config<Classifier_c, omm::Association, appcfg::Base_classifier_config >::fill_children() {



    // properties
    App_config_composite<std::string, omm::Property, appcfg::Base_classifier_config, App_element_config> container ( *this, app_ );
    for ( Classifier_c::property_const_iterator i ( this->xml_obj_.property().begin () );
            i != this->xml_obj_.property ().end ();
            ++i ) {

        container.insert_child ( *i, *i );

    }

}




template <> void App_element_config<Class_c, omm::Class,appcfg::Base_classifier_config>::fill_children() {
 
    

    	// properties
    App_config_composite<std::string, omm::Property, appcfg::Base_classifier_config, App_element_config> container (*this, app_);
	for (Class_c::property_const_iterator i (this->xml_obj_.property().begin ());
			i != this->xml_obj_.property ().end ();
			++i)
	{
        
		container.insert_child (*i, *i);

	}

    // member ends
    App_config_composite<std::string, omm::Property, appcfg::Base_classifier_config, App_element_config> cont_me (*this, app_);
	for (Class_c::memberEnd_const_iterator i (this->xml_obj_.memberEnd().begin ());
			i != this->xml_obj_.memberEnd ().end ();
			++i)
	{
        
		cont_me.insert_child (*i, *i);

	}
	
}



template class App_element_config<Domain_c, omm::Package, appcfg::App_domain> ;
template class App_element_config<Classifier_c, omm::Association, appcfg::Base_classifier_config> ;
template class App_element_config<Class_c, omm::Class, appcfg::Base_classifier_config> ;
template class App_element_config<std::string, omm::Property, appcfg::Base_classifier_config> ;


template class Model_element<Domain_c, omm::Package> ;
template class Model_element<Classifier_c, omm::Association> ;
template class Model_element<Class_c, omm::Class> ;
template class Model_element<std::string, omm::Property> ;

}