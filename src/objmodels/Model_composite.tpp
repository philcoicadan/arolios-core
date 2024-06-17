/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_composite.h>
#include <model/Base_element.h>
#include <common/Db_client.h>
#include <model/Element_factory.h>
#include <model/Element_action.h>
#include <omm/Namespace.h>
#include <exception/Exceptions.h>




using namespace arolios;
using namespace arolios::model;

template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> Model_composite<T1,T2 ,MOD>::Model_composite( Base_element& p_parent) : parent_ (p_parent) {


}

template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> Model_composite<T1,T2 ,MOD>::~Model_composite() {

}

template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> bool Model_composite<T1,T2 ,MOD>::check_action_compatibility( const T1& p_type) const {
    
    std::string && t_action = util::String::standardize ( p_type.action() );

    
    if ( (this->parent_.action() == Element_action::a_delete ) ||  ( this->parent_.action() == Element_action::a_create && t_action != "create" ) ) {
        return false ;
    } else {
        return true;
    }
}


template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> std::shared_ptr<MOD<T1,T2>> Model_composite<T1,T2 ,MOD>::insert_child (const T1& p_type) {

    std::shared_ptr<MOD<T1,T2>> type_store = nullptr;
    std::string && t_name = util::String::standardize(p_type.name());
    std::string && t_action = util::String::standardize(p_type.action());

    const auto stored_parent = std::dynamic_pointer_cast<omm::Namespace> (this->parent_.stored_element());
    std::shared_ptr<T2> t_stored = ( stored_parent)  ? 
            stored_parent->template find_element_by_name<T2>(t_name) :
            nullptr;

    
    const bool t_defined =  ( t_stored )  ? 
            true :
            (this->parent_.find_child(t_name) != nullptr) ;
        
    if (! check_action_compatibility ( p_type) ) {
        std::string msg ( "The action  " + p_type.action()  + " for the element " + t_name + " is not compatible with the action for its parent." );
        throw exception::Model_error ( msg );

    }

    if ( t_action == "create" ) {
        if ( !t_defined ) {
        
                type_store =Element_factory::instance().make_create<T1,MOD<T1,T2>,T2> ( p_type, t_name,  this->parent_);
                this->parent_.add_child(type_store);
              

        }
        else {
            std::string msg ( "element " + t_name + " already defined, creation is not possible." );
            throw exception::Model_error( msg );
        }
    } 
    else  {
        if (t_stored) {
            const Element_action action = (t_action == "update" ) ? Element_action::a_update : Element_action::a_delete;
            type_store = (t_action == "update" ) ? 
                Element_factory::instance().make_update<T1,MOD<T1,T2>,T2> ( p_type, t_stored, this->parent_) :
                Element_factory::instance().make_delete<T1,MOD<T1,T2>,T2> ( p_type, t_stored, this->parent_);
            this->parent_.add_child(type_store);


        } else {      
            std::string msg ( "element " + t_name + " : not found, update or delete action is not possible." );
            throw exception::Model_error ( msg );
        }
    }

           
    return type_store;

    
}
