/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Package.h>
#include <omm/Enumeration.h>
#include <omm/Composite_type.h>
#include <omm/Primitive_type.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Specific_type.h>
#include <omm/Selection_type.h>
#include <model/xam.h>
#include <model/Model_composite.h>
#include <model/Model_element_create.h>
#include <model/Root_loader.h>
#include "Model_composite.tpp"
#include <model/Domain_post_implementor.h>
#include <exception/Exceptions.h>
#include <util/String.h>










namespace arolios::model {
    


template <> void Model_element<Domain_t, omm::Package >::fill_children() {
     
 
     	// enumerations
    Model_composite<Enumeration_t, omm::Enumeration, Model_element > cont_enum (*this);
	for (Domain_t::enumeration_const_iterator i (this->xml_obj_.enumeration().begin ());
			i != this->xml_obj_.enumeration ().end ();
			++i)
	{
        
		cont_enum.insert_child (*i);

	}

	//primitives
    Model_composite<Primitive_t, omm::Primitive_type, Model_element > cont_prim (*this);

	for (Domain_t::primitive_const_iterator i (this->xml_obj_.primitive().begin ());
			i != this->xml_obj_.primitive ().end () ;
			++i)
	{
        
		cont_prim.insert_child(*i);

	}

	// composite types
    Model_composite<Type_t, omm::Composite_type, Model_element > cont_type (*this);

	for (Domain_t::type_const_iterator i (this->xml_obj_.type().begin ());
			i != this->xml_obj_.type ().end () ;
			++i)
	{
        
		cont_type.insert_child(*i);

	}

	//specifics
    Model_composite<Specific_t, omm::Specific_type, Model_element > cont_specific (*this);
	for (Domain_t::specific_const_iterator i (this->xml_obj_.specific().begin ());
			i != this->xml_obj_.specific ().end () ;
			++i)
	{
     		cont_specific.insert_child(*i);

	}

    //selections
    Model_composite<Selection_t, omm::Selection_type, Model_element > cont_selection (*this);
	for (Domain_t::selection_const_iterator i (this->xml_obj_.selection().begin ());
			i != this->xml_obj_.selection ().end () ;
			++i)
	{
        cont_selection.insert_child(*i);

	}

	//Classes
    Model_composite<Class_t, omm::Class, Model_element > cont_class (*this);
	for (Domain_t::class_const_iterator i (this->xml_obj_.class_().begin ());
			i != this->xml_obj_.class_ ().end ();
			++i)
	{
		cont_class.insert_child (*i);
	}

	//Associations
    Model_composite<Association_t, omm::Association, Model_element> cont_assoc (*this);
	for (Domain_t::association_const_iterator i (this->xml_obj_.association().begin ());
			i != this->xml_obj_.association().end ();
			++i)

	{
        
        cont_assoc.insert_child(*i);
    }
    
    
}

template <> void Model_element<Domain_t, omm::Package >::execute () {
    
    omm::Element_attributes attrs;

    internal_execute ( attrs ) ;

    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();

    const auto root_pk =  mdl_info->root_domain();
    
    if (root_pk) {
        if (root_pk->nested_packages().size()== 1) {
            // first nested domain is the standard domain
             mdl_info->standard_domain (this->stored());
        }
    }

    this->execute_children();


    {
        Domain_post_implementor dpi ;        


        try {


            dpi.add_fts_column ( *this );

            dpi.process_id_properties ( *this );

            dpi.process_required_properties ( *this );

        } catch ( const exception::Db_exception &e ) {
            dpi.rollback();
            std::cerr << e.base().what() << std::endl;
            throw e;
        } catch ( const exception::Base_exception &e ) {
            dpi.rollback();
            std::cerr << e.base().what() << std::endl;
            throw e;
        } catch ( std::exception const &e ) {
            dpi.rollback();
            std::cerr << e.what() << std::endl;
            throw e;
        }
    } // dpi destroyed, transaction committed automatically


}
    
    


template class Model_element<Domain_t, omm::Package > ;






}

