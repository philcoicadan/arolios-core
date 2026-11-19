/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <model/Model_composite.h>
#include "Model_composite.tpp"
#include <omm/Enumeration.h>
#include <omm/Enum_value.h>
#include <omm/Package.h>
#include <model/xam.h>







namespace arolios::model {
    



template <> void Model_element<Value_t, omm::Enum_value>::fill_children() {
 
   // no child
}



template class Model_element<Value_t, omm::Enum_value> ;
template class Model_composite<Value_t, omm::Enum_value, Model_element>;


}

