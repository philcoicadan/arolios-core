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
#include <omm/Primitive_type.h>
#include <omm/Primitive_attributes.h>
#include <model/xam.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>

namespace arolios::model {


template <> void Model_element<Primitive_t, omm::Primitive_type >::fill_children() {
    
    Model_composite<Constraint_t, omm::Constraint,  Model_element> container (*this);
    for (Primitive_t::constraint_const_iterator i (this->xml_obj_.constraint().begin ());
			i != this->xml_obj_.constraint ().end () ;
			++i)
	{
     		container.insert_child(*i);

	}
}

template <> void Model_element<Primitive_t, omm::Primitive_type >::execute () {


    omm::Primitive_attributes attrs;

    attrs.native_type_ = qry::Util::get_native_type ( xml_obj_.native() );



    this->internal_execute( attrs ) ;

    this->execute_children();

}

template class Model_element<Primitive_t, omm::Primitive_type > ;
template class Model_composite<Primitive_t, omm::Primitive_type,  Model_element>;

}
