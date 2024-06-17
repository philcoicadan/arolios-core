/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Property.h>
#include <omm/Property_attributes.h>
#include <model/Classifier_search.h>
#include <exception/Exceptions.h>
#include <model/xam.h>








namespace arolios::model {
    




template <> void Model_element<Class_property_t, omm::Property>::fill_children() {
 
   // no child
}




template <> void Model_element<Class_property_t, omm::Property >::execute () {

    //properties


    //lookup type of property
    std::shared_ptr<omm::Package> search_pk = parent_element_.stored_element()->package();
    
    Classifier_search search ( xml_obj_.type(), search_pk);
    
    auto prop_type = search.execute<omm::Data_type>();


    omm::Property_attributes attrs;
    attrs.type_ = prop_type;
    attrs.min_ = xml_obj_.required() ? 1 : 0;
    
    internal_execute ( attrs ) ;



}


template class Model_element<Class_property_t, omm::Property> ;

}

