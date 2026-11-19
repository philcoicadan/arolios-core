/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <model/Model_composite.h>
#include "Model_composite.tpp"
#include <omm/Constraint.h>
#include <omm/Constraint_attributes.h>
#include <omm/Package.h>
#include <model/xam.h>







namespace arolios::model {
    



template <> void Model_element<Constraint_t, omm::Constraint>::fill_children() {
 
   // no child
}

template <> void Model_element<Constraint_t, omm::Constraint >::execute () {


    omm::Constraint_attributes attrs;

    attrs.expression_ = xml_obj_.expression();

    internal_execute ( attrs ) ;

    this->execute_children();

}

template class Model_element<Constraint_t, omm::Constraint> ;

}

