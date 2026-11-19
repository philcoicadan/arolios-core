/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Enumeration.h>
#include <omm/Package.h>
#include <omm/Enum_value.h>
#include <model/Model_composite.h>
#include "Model_composite.tpp"
#include <model/Root_loader.h>
#include <model/xam.h>
#include <exception/Exceptions.h>








namespace arolios::model {
    


template <> void Model_element<Enumeration_t, omm::Enumeration >::fill_children() {
  

    Model_composite<Value_t, omm::Enum_value,  Model_element> container (*this);
	for (Enumeration_t::value_const_iterator j (this->xml_obj_.value().begin ());
			j!=this->xml_obj_.value().end ();
			++j)
	{
        container.insert_child(*j);
    }
        
	
 
}



template class Model_element<Enumeration_t, omm::Enumeration > ;

template class Model_composite<Enumeration_t, omm::Enumeration, Model_element >;

}

