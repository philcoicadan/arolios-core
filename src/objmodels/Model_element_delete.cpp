/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "Model_element_delete.tpp"
#include <omm/Class.h>
#include <omm/Package.h>
#include <omm/Association.h>
#include <omm/Data_type.h>
#include <omm/Composite_type.h>
#include <omm/Primitive_type.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <omm/Enum_value.h>






namespace arolios::model {

template <> bool Model_element_delete<Domain_t, omm::Package >::has_children() const {
    return ( !this->xml_obj_.class_().empty() ||
        !this->xml_obj_.association().empty() ||
        !this->xml_obj_.type().empty() ||
        !this->xml_obj_.primitive().empty() ||
        !this->xml_obj_.enumeration().empty() ) ? true : false; 

}

template <> bool Model_element_delete<Class_t, omm::Class >::has_children() const {
    return ( !this->xml_obj_.property().empty() || 
        this->xml_obj_.base().present()
    ) ? true : false; 

}

    
template <> bool Model_element_delete<Association_t, omm::Association >::has_children() const {
    return ( !this->xml_obj_.property().empty() || 
        !this->xml_obj_.memberEnd().empty()
    ) ? true : false; 

}

template <> bool Model_element_delete<Enumeration_t, omm::Enumeration >::has_children()  const {
        return ( !this->xml_obj_.value().empty()  ) ? true : false; 

}

    
template <> bool Model_element_delete<Type_t, omm::Composite_type >::has_children()  const {
        return ( !this->xml_obj_.property().empty()  ) ? true : false; 

}
  
    
template class Model_element_delete<Domain_t,omm::Package > ;        
template class Model_element_delete<Class_t,omm::Class > ;
template class Model_element_delete<Association_t,omm::Association > ;
template class Model_element_delete<Type_t,omm::Composite_type > ;
template class Model_element_delete<Primitive_t,omm::Primitive_type > ;
template class Model_element_delete<Enumeration_t,omm::Enumeration > ;

template class Model_element_delete<Class_property_t,omm::Property > ;
template class Model_element_delete<Type_property_t,omm::Property > ;
template class Model_element_delete<Member_end_t,omm::Property > ;

template class Model_element_delete<Element_t, omm::Enum_value> ;



}
