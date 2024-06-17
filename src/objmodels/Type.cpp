/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Data_type.h>
#include <omm/Package.h>
#include <omm/Property.h>
#include <model/Model_composite.h>
#include "Model_composite.tpp"
#include <model/xam.h>







namespace arolios::model {
    


template <> void Model_element<Type_t, omm::Data_type >::fill_children() {
 
    Model_composite<Type_property_t, omm::Property, Model_element> container (*this);
	for (Type_t::property_const_iterator j (this->xml_obj_.property().begin ());
			j!=this->xml_obj_.property().end ();
			++j)
	{
        container.insert_child(*j);
    }
        
	
}





template class Model_element<Type_t, omm::Data_type > ;

template class Model_composite<Type_property_t, omm::Property, Model_element>;

}

