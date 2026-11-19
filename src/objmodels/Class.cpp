/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Package.h>
#include <omm/Class.h>
#include <omm/Property.h>
#include <omm/Classifier_attributes.h>
#include <model/xam.h>
#include <model/Model_composite.h>
#include <model/Classifier_search.h>
#include "Model_composite.tpp"
#include <exception/Exceptions.h>
#include <util/String.h>
#include <model/Util.h>
#include <exception/Exceptions.h>











namespace arolios::model {

template <> void Model_element<Class_t, omm::Class >::fill_children() {
     

     	// properties
    Model_composite<Class_property_t, omm::Property, Model_element> container (*this);
	for (Class_t::property_const_iterator i (this->xml_obj_.property().begin ());
			i != this->xml_obj_.property ().end ();
			++i)
	{
        
		container.insert_child (*i);

	}

    Model_composite<Constraint_t, omm::Constraint,  Model_element> cont_ctt (*this);
    for (Class_t::constraint_const_iterator i (this->xml_obj_.constraint().begin ());
			i != this->xml_obj_.constraint ().end () ;
			++i)
	{
     		cont_ctt.insert_child(*i);

	}

}

template <> void Model_element<Class_t, omm::Class >::execute () {
    
    //check base 
    std::string base = (xml_obj_.base().present() ) ? xml_obj_.base().get() : "";
    auto && pk = std::dynamic_pointer_cast<omm::Package>(parent_element_.stored_element());
    auto stored_base = Util::resolve_base<omm::Class> ( base, pk, (this->stored_ ? std::dynamic_pointer_cast<omm::Classifier>(this->stored_) : nullptr) ) ;

    omm::Classifier_attributes c_attrs;
    c_attrs.is_abstract_ = xml_obj_.isAbstract();
    c_attrs.base_= std::dynamic_pointer_cast<omm::Classifier>(stored_base);
    
    internal_execute (c_attrs) ;

	this->execute_children();
    
    // identifier
    
    if (xml_obj_.identifier().present()) {
         auto ident = xml_obj_.identifier().get();
         
         std::unordered_map<std::string,std::shared_ptr<omm::Property>> ident_map;
         auto pers_cls = this->stored_;

        for (Class_identifier_t::property_const_iterator i (ident.property().begin ());
            i != ident.property ().end ();
            ++i)
        {
    
            Util::add_identifier (*i, pers_cls, ident_map);
        }
        
        for (Class_identifier_t::memberEnd_const_iterator i (ident.memberEnd().begin ());
            i != ident.memberEnd ().end ();
            ++i)
        {
        
            Util::add_identifier (*i, pers_cls, ident_map);

        }

        Util::store_identifier ( "Class " + xml_obj_.name(), pers_cls, ident_map );
    }
}


template class Model_element<Class_t, omm::Class > ;
template class Model_composite<Class_t, omm::Class,  Model_element>;






}

