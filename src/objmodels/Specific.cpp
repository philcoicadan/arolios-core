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
#include <omm/Specific_type.h>
#include <omm/Collection_attributes.h>
#include <omm/Specific_attributes.h>
#include <omm/Model_factory.h>
#include <omm/Package.h>
#include <model/Classifier_search.h>
#include <model/Collection.h>
#include <model/xam.h>
#include <exception/Exceptions.h>
#include <string>

namespace arolios::model {

template <> void Model_element<Specific_t, omm::Specific_type >::fill_children() {
    
    Model_composite<Constraint_t, omm::Constraint,  Model_element> container (*this);
    for (Specific_t::constraint_const_iterator i (this->xml_obj_.constraint().begin ());
			i != this->xml_obj_.constraint ().end () ;
			++i)
	{
     		container.insert_child(*i);

	}
}

template <> void Model_element<Specific_t, omm::Specific_type >::execute () {


    omm::Specific_attributes attrs;

    
    //lookup referenced type
    std::shared_ptr<omm::Package> search_pk = parent_element_.stored_element()->package();
    
    Classifier_search search ( xml_obj_.type(), search_pk);
    
    auto ref_type = search.execute<omm::Classifier>();

    attrs.type_ = ref_type;

    if (xml_obj_.collection().present()) {
        attrs.collection_ = Collection::parse_collection(xml_obj_.collection().get());
   }

    internal_execute ( attrs ) ;

    this->execute_children();

}
// template <> void Model_element_create<Specific_t, omm::Specific_type>::internal_execute (const omm::Element_attributes&) {

//     if (!this->xml_obj_.constraint().empty()) {
//         throw exception::Model_error("constraint is not supported for a specific");
//     }

//     auto parent = std::dynamic_pointer_cast<omm::Package>(this->parent_element_.stored_element());
//     if (!parent) {
//         throw exception::Model_error("specific " + this->std_name_ + " : parent package not found");
//     }

//     Classifier_search search (this->xml_obj_.type(), parent);
//     auto referenced_type = search.execute<omm::Data_type>();

//     std::shared_ptr<omm::Collection_attributes> collection;
//     if (this->xml_obj_.collection().present()) {
//         collection = parse_collection(this->xml_obj_.collection().get());
//     }

//     const std::string desc = this->xml_obj_.description().present() ? this->xml_obj_.description().get() : "";

//     this->stored_ = omm::Model_factory::instance().make_specific_type (
//         this->std_name_, parent, referenced_type, collection, omm::Lc_status::active, desc);
// }


template class Model_element<Specific_t, omm::Specific_type>;
template class Model_composite<Specific_t, omm::Specific_type,  Model_element>;
}
