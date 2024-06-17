/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element_translation.h>
#include <model/Model_tlt_composite.h>
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
#include <model/xtlt.h>
#include "Model_element.tpp"








namespace arolios::model {


template < typename T1, typename T2 > Model_element_translation<T1,T2 >::Model_element_translation(const T1& p_elem, const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element, const std::shared_ptr<const omm::Language>& p_lang ):Model_element<T1,T2 > (p_elem,p_stored->name(), p_stored, p_parent_element, Element_action::a_update) , language_ (p_lang){
	}


template <typename T1, typename T2 > Model_element_translation<T1,T2 >::~Model_element_translation() {
}


template <typename T1, typename T2 > void Model_element_translation<T1,T2 >::execute () {
    
    Model_element<T1,T2>::execute ();
}

template < typename T1, typename T2 >  void Model_element_translation<T1,T2 >::internal_execute ( const omm::Element_attributes& p_attrs ) {


    translation_ = this->xml_obj_.translation() ;

    if ( !translation_.empty() ) {

        {
            Model_recorder  mdl_rcd ;


            try {


                mdl_rcd.update_translation ( this->stored_, this->language_->id(), this->translation_ );

                // }
            } catch ( const exception::Db_exception &e ) {
                mdl_rcd.rollback();
                 
                throw e;
            } catch ( const exception::Base_exception &e ) {
                mdl_rcd.rollback();
                 
                throw e;
            } catch ( std::exception const &e ) {
                mdl_rcd.rollback();
                 
                throw e;
            }
        } // mdl_rcd destroyed, transaction committed automatically


       this->stored_->create_or_update_translation ( language_, translation_ );

    }
}

template <typename T1, typename T2 > void Model_element_translation<T1,T2 >::accept (Visitor& p_visitor) {

}

template <typename T1, typename T2 > void Model_element_translation<T1,T2 >::fill_children() {
 // by default , nothing to do
}


template <> void Model_element_translation<Domain_tlt, omm::Package >::fill_children ()  {

    

    	
    Model_tlt_composite<Enumeration_tlt, omm::Enumeration, Model_element_translation > cont_enum (*this, language_);
	for (Domain_tlt::enumeration_const_iterator i (this->xml_obj_.enumeration().begin ());
			i != this->xml_obj_.enumeration ().end ();
			++i)
	{
        
		cont_enum.insert_child (*i);

	}

	//types
    Model_tlt_composite<Type_tlt, omm::Data_type, Model_element_translation > cont_type (*this, language_);

	for (Domain_tlt::type_const_iterator i (this->xml_obj_.type().begin ());
			i != this->xml_obj_.type ().end () ;
			++i)
	{
        
		cont_type.insert_child(*i);

	}

	//Classes
    Model_tlt_composite<Type_tlt, omm::Class, Model_element_translation > cont_class (*this, language_);
	for (Domain_tlt::class_const_iterator i (this->xml_obj_.class_().begin ());
			i != this->xml_obj_.class_ ().end ();
			++i)
	{
		cont_class.insert_child (*i);
	}

	//Associations
    Model_tlt_composite<Association_tlt, omm::Association, Model_element_translation> cont_assoc (*this, language_);
	for (Domain_tlt::association_const_iterator i (this->xml_obj_.association().begin ());
			i != this->xml_obj_.association().end ();
			++i)

	{
        
        cont_assoc.insert_child(*i);
    }
    
    
}

template <> void Model_element_translation<Association_tlt, omm::Association >::fill_children() {



    // properties
    Model_tlt_composite<Element_tlt, omm::Property, Model_element_translation> container ( *this , language_);
    for ( Association_tlt::property_const_iterator i ( this->xml_obj_.property().begin () );
            i != this->xml_obj_.property ().end ();
            ++i ) {

        container.insert_child ( *i );

    }

    int count = xml_obj_.memberEnd().size();


        // create or update action , check the consistency of the members ( 2 members are required)
        if ( count != 2 ) {
            throw exception::Model_error ( std::to_string(count) + "-ary association  is not authorized" );

        }


        const Element_tlt* members[2] = {&xml_obj_.memberEnd() [0], &xml_obj_.memberEnd() [1]};


        Model_tlt_composite<Element_tlt, omm::Property, Model_element_translation> cont_me ( *this, language_ );


            cont_me.insert_child ( * members[0] );
            cont_me.insert_child ( * members[1] );

 

         
}


template <> void Model_element_translation<Type_tlt, omm::Class >::fill_children() {
 
    

    	// properties
    Model_tlt_composite<Element_tlt, omm::Property, Model_element_translation> container (*this, language_);
	for (Type_tlt::property_const_iterator i (this->xml_obj_.property().begin ());
			i != this->xml_obj_.property ().end ();
			++i)
	{
        
		container.insert_child (*i);

	}

	
}

template <> void Model_element_translation<Type_tlt, omm::Data_type >::fill_children() {
 
    

    	// properties
    Model_tlt_composite<Element_tlt, omm::Property, Model_element_translation> container (*this, language_);
	for (Type_tlt::property_const_iterator i (this->xml_obj_.property().begin ());
			i != this->xml_obj_.property ().end ();
			++i)
	{
        
		container.insert_child (*i);

	}

	
}


template <> void Model_element_translation<Enumeration_tlt, omm::Enumeration >::fill_children() {
 
    Model_tlt_composite<Element_tlt, omm::Enum_value,  Model_element_translation> container (*this, language_);
	for (Enumeration_tlt::value_const_iterator j (this->xml_obj_.value().begin ());
			j!=this->xml_obj_.value().end ();
			++j)
	{
        container.insert_child(*j);
    }
        
}



template class Model_element_translation<Domain_tlt, omm::Package> ;
template class Model_element_translation<Association_tlt, omm::Association> ;
template class Model_element_translation<Type_tlt, omm::Class> ;
template class Model_element_translation<Type_tlt, omm::Data_type> ;
template class Model_element_translation<Enumeration_tlt, omm::Enumeration> ;
template class Model_element_translation<Element_tlt, omm::Property> ;
template class Model_element_translation<Element_tlt, omm::Enum_value> ;


template class Model_element<Domain_tlt, omm::Package> ;
template class Model_element<Association_tlt, omm::Association> ;
template class Model_element<Type_tlt, omm::Class> ;
template class Model_element<Type_tlt, omm::Data_type> ;
template class Model_element<Enumeration_tlt, omm::Enumeration> ;
template class Model_element<Element_tlt, omm::Property> ;
template class Model_element<Element_tlt, omm::Enum_value> ;
}
