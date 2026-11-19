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
#include <omm/Property.h>
#include <omm/Property_attributes.h>
#include <omm/Specific_type.h>
#include <omm/Collection_attributes.h>
#include <model/Collection.h>
#include <model/Classifier_search.h>
#include <exception/Exceptions.h>
#include <model/xam.h>
#include <algorithm>
#include <limits>








namespace arolios::model {
    




template <> void Model_element<Class_property_t, omm::Property>::fill_children() {
 
   // no child
}




template <> void Model_element<Class_property_t, omm::Property >::execute () {

    //properties


    //lookup type of property
    std::shared_ptr<omm::Package> search_pk = parent_element_.stored_element()->package();
    
    Classifier_search search ( xml_obj_.type(), search_pk);
    
    auto prop_type = search.execute<omm::Classifier>();


    omm::Property_attributes attrs;
    attrs.type_ = prop_type;

    // "required" gives the default cardinality when no collection is specified
    attrs.min_ = xml_obj_.required() ? 1 : 0;
    attrs.max_ = 1;

    if (xml_obj_.collection().present()) {
        attrs.collection_ = Collection::parse_collection(xml_obj_.collection().get());

        long scalar_min = std::numeric_limits<long>::max();
        long scalar_max = 1;

        for (const auto& range : attrs.collection_->ranges()) {
            scalar_min = std::min(scalar_min, range.min_);

            if (range.max_ == -1) {
                scalar_max = -1;
            } else if (scalar_max != -1) {
                scalar_max = std::max(scalar_max, range.max_);
            }
        }

        if (xml_obj_.required() && scalar_min < 1) {
            scalar_min = 1;
        }

        attrs.min_ = scalar_min;
        attrs.max_ = scalar_max;
    }

    // maxOccurs must be greater or equal to minOccurs

    if (attrs.max_ != -1 && attrs.max_ < attrs.min_) {
      std::string error_msg =
          "Class " + parent_element_.standard_name() +
          ", property: " + xml_obj_.name() +
          " collection range is incompatible with the required flag";
      throw exception::Model_error(error_msg);
    }

    if (xml_obj_.isComposition()) {
        attrs.aggregation_ = omm::Aggregation_kind::composite;
    } else {
        attrs.aggregation_ = omm::Aggregation_kind::none;
    }

    attrs.is_member_end_ = xml_obj_.memberEnd();
    attrs.opposite_name_ = xml_obj_.opposite().present() ? xml_obj_.opposite().get() : "";

    internal_execute ( attrs ) ;
    



}


template class Model_element<Class_property_t, omm::Property> ;
template class Model_composite<Class_property_t, omm::Property, Model_element>;
}

