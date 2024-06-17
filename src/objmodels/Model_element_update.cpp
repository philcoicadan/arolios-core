/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "Model_element_update.tpp"
#include <omm/Class.h>
#include <omm/Package.h>
#include <omm/Association.h>
#include <omm/Data_type.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <omm/Enum_value.h>



namespace arolios::model {
    
template class Model_element_update<Domain_t, omm::Package > ;
template class Model_element_update<Class_t, omm::Class > ;
template class Model_element_update<Association_t, omm::Association > ;
template class Model_element_update<Type_t, omm::Data_type > ;
template class Model_element_update<Enumeration_t, omm::Enumeration > ;
template class Model_element_update<Type_property_t, omm::Property > ;
template class Model_element_update<Class_property_t, omm::Property > ;
template class Model_element_update<Member_end_t, omm::Property > ;
template class Model_element_update<Element_t, omm::Enum_value > ;
}
