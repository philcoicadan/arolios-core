/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element_create.h>
#include <model/Model_recorder.h>
#include <model/Model_implementor.h>
#include <omm/Model_factory.h>
#include <model/Visitor.h>
#include <common/Db_client.h>
#include <exception/Exceptions.h>







using namespace arolios;
using namespace arolios::model ;

template < typename T1, typename T2 > Model_element_create<T1,T2 >::Model_element_create(const T1& p_elem, const std::string& p_std_name, const Base_element & p_parent): Model_element<T1,T2 >(p_elem, p_std_name, std::shared_ptr<T2>(nullptr), p_parent, Element_action::a_create) { 


	}


template < typename T1, typename T2 > Model_element_create<T1,T2 >::~Model_element_create() {
}


template < typename T1, typename T2 > void Model_element_create<T1,T2 >::execute () {

   Model_element<T1,T2>::execute ();

}

template < typename T1, typename T2 > void Model_element_create<T1,T2 >::internal_execute ( const omm::Element_attributes& p_attrs) {


    const std::string desc = this->xml_obj_.description().present() ? this->xml_obj_.description().get() : "";
    
   
    // Model_recorder keeps current transaction
    Model_recorder  mdl_rcd;

 

    try {

	auto pers_elem = mdl_rcd.create<T2 >(this->std_name_, this->parent_element_.stored_element(), desc, p_attrs );
    
	this->stored_ =  omm::Model_factory::instance().make<T2 >(pers_elem, this->std_name_, this->parent_element_.stored_element(), p_attrs, omm::Lc_status::active, desc);

    Model_implementor  impl_visitor (mdl_rcd);
                accept ( impl_visitor );
    
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

template < typename T1, typename T2 > void Model_element_create<T1,T2 >::accept (Visitor& p_visitor) {
    p_visitor.visit(*this);
}


