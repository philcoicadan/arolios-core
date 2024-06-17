/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element_update.h>
#include <model/Model_recorder.h>
#include <model/Model_implementor.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <omm/Model_factory.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>









namespace arolios::model {


template < typename T1, typename T2 > Model_element_update<T1,T2 >::Model_element_update(const T1& p_elem, const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element ):Model_element<T1,T2 > (p_elem,p_stored->name(), p_stored, p_parent_element, Element_action::a_update) {
	}


template <typename T1, typename T2 > Model_element_update<T1,T2 >::~Model_element_update() {
}


template <typename T1, typename T2 > void Model_element_update<T1,T2 >::execute () {
    
    Model_element<T1,T2>::execute ();
}

template < typename T1, typename T2 >  void Model_element_update<T1,T2 >::internal_execute ( const omm::Element_attributes& p_attrs ) {


    const std::string new_desc = this->xml_obj_.description().present() ? this->xml_obj_.description().get() : "";

    new_name_ = this->xml_obj_.rename().present() ? this->xml_obj_.rename().get() : "";
    new_name_ = util::String::standardize ( new_name_ );


    {
        Model_recorder  mdl_rcd ;



        try {
            // update package in db


            mdl_rcd.update<T2> ( this->stored_, new_name_, new_desc, p_attrs );


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

    if ( !new_name_.empty() ) {
        // update maps of the common objects
        auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
         mdl_info->rename<T2> ( this->stored_, new_name_ );

        auto app_info = common::Singleton<common::App_info>::instance().object();
        app_info->rename<T2> ( this->stored_, new_name_ ); // if necessary

    }


    //generic update in memory
    omm::Model_factory::instance().update<T2> ( this->stored_, new_name_, new_desc, p_attrs );
}

template <typename T1, typename T2 > void Model_element_update<T1,T2 >::accept (Visitor& p_visitor) {
    p_visitor.visit(*this);
}




}
