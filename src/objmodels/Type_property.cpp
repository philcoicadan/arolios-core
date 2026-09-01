/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Property.h>
#include <omm/Primitive_type.h>
#include <omm/Property_attributes.h>
#include <model/Classifier_search.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <exception/Exceptions.h>
#include <model/xam.h>








namespace arolios::model {
    



template <> void Model_element<Type_property_t, omm::Property>::fill_children() {
 
   // no child
}



template <> void Model_element<Type_property_t, omm::Property >::execute () {

    //properties
    

    //lookup type of property
    std::shared_ptr<omm::Package> search_pk = parent_element_.stored_element()->package();
    
    Classifier_search search ( xml_obj_.type(), search_pk);
    
    auto prop_type = search.execute<omm::Data_type>();
    auto prim_type = std::dynamic_pointer_cast<omm::Primitive_type>(prop_type);
    auto enum_type = std::dynamic_pointer_cast<omm::Enumeration>(prop_type);

    if (!enum_type && !prim_type) {

            std::string error_msg = "Property type " + xml_obj_.type() + " cannot be nested" ;
            throw exception::Model_error ( error_msg );
    }
     

    omm::Property_attributes attrs;
    attrs.type_ = prop_type;

    internal_execute ( attrs ) ;

   


}


template class Model_element<Type_property_t, omm::Property> ;

}

