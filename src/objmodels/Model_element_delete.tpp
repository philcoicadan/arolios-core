/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element_delete.h>
#include <model/Model_recorder.h>
#include <common/Db_client.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <model/Model_implementor.h>
#include <omm/Packageable_element.h>
#include <exception/Exceptions.h>







using namespace arolios;
using namespace arolios::model;

    
template < typename T1, typename T2 > Model_element_delete<T1,T2 >::Model_element_delete(const T1& p_elem, const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element ):Model_element<T1,T2 > (p_elem,p_stored->name(), p_stored, p_parent_element, Element_action::a_delete)  {


	}


template < typename T1, typename T2 > Model_element_delete<T1,T2 >::~Model_element_delete() {

}


template < typename T1, typename T2 > void Model_element_delete<T1,T2 >::execute () {
    
    omm::Element_attributes attrs;
    
    this->internal_execute(attrs); 
}

template < typename T1, typename T2 >  void Model_element_delete<T1,T2 >::internal_execute ( const omm::Element_attributes& p_attrs) {    
    
    if (this->xml_obj_.description().present() ||
        this->has_children()  ) {
        std::string error_msg = "Element declarations not possible for the delete action of the element" + this->stored_->name();
        throw exception::Model_error ( error_msg );
    }
    
    if (this->stored_->lc_status() == omm::Lc_status::active) {


        {
            Model_recorder  mdl_rcd ;


            try {
                // update element and its contents in db
                mdl_rcd.deprecate<T2> ( this->stored_ );

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

        this->stored_->recursive_lc_status (omm::Lc_status::deprecated);
    }
    
    if ( this->stored_->lc_status() == omm::Lc_status::deprecated ) {
        
        {
            Model_recorder  mdl_rcd ;


            try {
                // update package in db
                if ( mdl_rcd.eliminate<T2> ( this->stored_ ) ) {

                    Model_implementor  impl_visitor (mdl_rcd);
                    accept ( impl_visitor );
                }
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

            // update maps in the common objects
            auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
             mdl_info->eliminate<T2> ( this->stored_ );

            auto app_info = common::Singleton<common::App_info>::instance().object();
            app_info->eliminate<T2> ( this->stored_ );

            //update status
            this->stored_->recursive_lc_status ( omm::Lc_status::deleted );
            
            //erase the element
            const std::shared_ptr<omm::Packageable_element>& elem = this->stored_;
            std::shared_ptr<omm::Namespace> elem_owner= elem->own_namespace();
            elem_owner->erase_element(elem);
            elem->own_namespace(nullptr);
 
    
    }
}



template < typename T1, typename T2 > void Model_element_delete<T1,T2 >::accept (Visitor& p_visitor) {
    p_visitor.visit(*this);
}

template < typename T1, typename T2 >  bool Model_element_delete<T1,T2>::has_children() const {
    return false; // by default

}

