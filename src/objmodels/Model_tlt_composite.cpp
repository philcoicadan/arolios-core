/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_tlt_composite.h>
#include <model/Base_element.h>
#include <common/Db_client.h>
#include <model/Element_factory.h>
#include <model/Element_action.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <omm/Association.h>
#include <omm/Class.h>
#include <omm/Data_type.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <omm/Enum_value.h>
#include <model/xtlt.h>







using namespace arolios;
using namespace arolios::model;

template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> Model_tlt_composite<T1,T2 ,MOD>::Model_tlt_composite( Base_element& p_parent, const std::shared_ptr<const omm::Language>& p_lang) : parent_(p_parent), language_ (p_lang) {

}

template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> Model_tlt_composite<T1,T2 ,MOD>::~Model_tlt_composite() {
}


template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> std::shared_ptr<MOD<T1,T2>> Model_tlt_composite<T1,T2 ,MOD>::insert_child (const T1& p_type) {

    std::shared_ptr<MOD<T1,T2>> type_store = nullptr;
    std::string && t_name = util::String::standardize(p_type.name());

    const auto stored_parent = std::dynamic_pointer_cast<omm::Namespace> (parent_.stored_element());
    std::shared_ptr<T2> t_stored = ( stored_parent)  ? 
            stored_parent->template find_element_by_name<T2>(t_name) :
            nullptr;
    
    
        if (t_stored) {

            type_store = 
                Element_factory::instance().make_translation<T1,MOD<T1,T2>,T2> ( p_type, t_stored, parent_, language_) ;
            parent_.add_child(type_store);


        } else {      
            std::string msg ( "element " + t_name + " : not found, action is not possible." );
            throw exception::Model_error ( msg );
        }


           
    return type_store;

    
}
namespace arolios::model {
    template class Model_tlt_composite<Association_tlt, omm::Association, Model_element_translation> ;
    template class Model_tlt_composite<Type_tlt, omm::Class, Model_element_translation> ;
    template class Model_tlt_composite<Type_tlt, omm::Data_type, Model_element_translation> ;
    template class Model_tlt_composite<Enumeration_tlt, omm::Enumeration, Model_element_translation> ;
    template class Model_tlt_composite<Element_tlt, omm::Property, Model_element_translation> ;
    template class Model_tlt_composite<Element_tlt, omm::Enum_value, Model_element_translation> ;

    template class Model_tlt_composite<Association_tlt, omm::Association, Model_element> ;
    template class Model_tlt_composite<Type_tlt, omm::Class, Model_element> ;
    template class Model_tlt_composite<Type_tlt, omm::Data_type, Model_element> ;
    template class Model_tlt_composite<Enumeration_tlt, omm::Enumeration, Model_element> ;
    template class Model_tlt_composite<Element_tlt, omm::Property, Model_element> ;
    template class Model_tlt_composite<Element_tlt, omm::Enum_value, Model_element> ;
}