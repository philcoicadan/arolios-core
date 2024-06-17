/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "Model_element_create.tpp"
#include <omm/Class.h>
#include <omm/Package.h>
#include <omm/Association.h>
#include <omm/Data_type.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <omm/Enum_value.h>

namespace arolios::model {


template class Model_element_create<Domain_t, omm::Package > ;
template class Model_element_create<Class_t, omm::Class > ;
template class Model_element_create<Association_t, omm::Association > ;
template class Model_element_create<Type_t, omm::Data_type > ;
template class Model_element_create<Enumeration_t, omm::Enumeration > ;
template class Model_element_create<Type_property_t, omm::Property > ;
template class Model_element_create<Class_property_t, omm::Property > ;
template class Model_element_create<Member_end_t, omm::Property > ;
template class Model_element_create<Element_t, omm::Enum_value > ;

}

