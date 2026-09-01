/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Primitive_type.h>
#include <omm/Primitive_attributes.h>
#include <model/xam.h>
#include <qry/Util.h>

namespace arolios::model {


template <> void Model_element<Primitive_t, omm::Primitive_type >::fill_children() {

}

template <> void Model_element<Primitive_t, omm::Primitive_type >::execute () {

    omm::Primitive_attributes attrs;

    attrs.native_type_ = qry::Util::get_native_type ( xml_obj_.native() );

    this->internal_execute( attrs ) ;

    this->execute_children();

}

template class Model_element<Primitive_t, omm::Primitive_type > ;

}
