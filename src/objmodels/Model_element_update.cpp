/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "Model_element_update.tpp"
#include "model/xam.h"
#include <omm/Class.h>
#include <omm/Package.h>
#include <omm/Association.h>
#include <omm/Data_type.h>
#include <omm/Composite_type.h>
#include <omm/Primitive_type.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <omm/Enum_value.h>
#include <omm/Selection_type.h>
#include <omm/Specific_type.h>


namespace arolios::model {
    
template class Model_element_update<Domain_t, omm::Package > ;
template class Model_element_update<Class_t, omm::Class > ;
template class Model_element_update<Association_t, omm::Association > ;
template class Model_element_update<Type_t, omm::Composite_type > ;
template class Model_element_update<Primitive_t, omm::Primitive_type > ;
template class Model_element_update<Enumeration_t, omm::Enumeration > ;
template class Model_element_update<Type_property_t, omm::Property > ;
template class Model_element_update<Class_property_t, omm::Property > ;
template class Model_element_update<Member_end_t, omm::Property > ;
template class Model_element_update<Value_t, omm::Enum_value > ;
template class Model_element_update<Selection_t, omm::Selection_type > ;
template class Model_element_update<Specific_t, omm::Specific_type > ;
template class Model_element_update<Constraint_t, omm::Constraint > ;
}
