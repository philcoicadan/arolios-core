/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "Model_element_create.tpp"
#include "omm/Selection_type.h"
#include "omm/Specific_type.h"
#include <omm/Class.h>
#include <omm/Package.h>
#include <omm/Association.h>
#include <omm/Composite_type.h>
#include <omm/Primitive_type.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <omm/Enum_value.h>
#include <omm/Selection_type.h>
#include <omm/Specific_type.h>

namespace arolios::model {


template class Model_element_create<Domain_t, omm::Package > ;
template class Model_element_create<Class_t, omm::Class > ;
template class Model_element_create<Association_t, omm::Association > ;
template class Model_element_create<Type_t, omm::Composite_type > ;
template class Model_element_create<Primitive_t, omm::Primitive_type > ;
template class Model_element_create<Enumeration_t, omm::Enumeration > ;
template class Model_element_create<Type_property_t, omm::Property > ;
template class Model_element_create<Class_property_t, omm::Property > ;
template class Model_element_create<Member_end_t, omm::Property > ;
template class Model_element_create<Value_t, omm::Enum_value > ;
template class Model_element_create<Selection_t, omm::Selection_type > ;
template class Model_element_create<Specific_t, omm::Specific_type > ;
template class Model_element_create<Constraint_t, omm::Constraint > ;

}

