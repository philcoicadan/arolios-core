/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Selection_type.h>
#include <omm/Selection_attributes.h>
#include <model/xam.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <model/Classifier_search.h>
#include <omm/Primitive_type.h>
#include <omm/Enumeration.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Specific_type.h>
#include <omm/Composite_type.h>
#include <model/Model_composite.h>
#include "Model_composite.tpp"

namespace arolios::model {


template <> void Model_element<Selection_t, omm::Selection_type >::fill_children() {

}

template <> void Model_element<Selection_t, omm::Selection_type >::execute () {

 
    omm::Selection_attributes attrs;

    //lookup the classifiers
    std::shared_ptr<omm::Package> search_pk = parent_element_.stored_element()->package();

    for (Selection_t::option_const_iterator i(
             this->xml_obj_.option().begin());
         i != this->xml_obj_.option().end(); ++i) {


        Classifier_search search ( *i, search_pk);
    
        auto option_cls = search.execute<omm::Classifier>();
        if (option_cls) {
            attrs.options_.push_back(option_cls);
            continue;
        }

        // auto option_prim = search.execute<omm::Primitive_type>();
        // if (option_prim) {
        //     attrs.options_.push_back(option_prim);
        //     continue;
        // }

        // auto option_enum = search.execute<omm::Enumeration>();
        // if (option_enum) {
        //     attrs.options_.push_back(option_enum);
        //     continue;
        // }

        // auto option_type = search.execute<omm::Composite_type>();
        // if (option_type) {
        //     attrs.options_.push_back(option_type);
        //     continue;
        // }

        // auto option_specific = search.execute<omm::Specific_type>();
        // if (option_specific) {
        //     attrs.options_.push_back(option_specific);
        //     continue;
        // }

        // auto option_class = search.execute<omm::Class>();
        // if (option_class) {
        //     attrs.options_.push_back(option_class);
        //     continue;
        // }

        // auto option_assoc = search.execute<omm::Association>();
        // if (option_assoc) {
        //     attrs.options_.push_back(option_assoc);
        //     continue;
        // }

    
        std::string error_msg =
            "Option " + *i + "of Selection "  + xml_obj_.name() +
            " does not have a valid type or is not found";
        throw exception::Model_error(error_msg);
    }

         
    this->internal_execute( attrs ) ;

    this->execute_children();

}

template class Model_element<Selection_t, omm::Selection_type > ;

template class Model_composite<Selection_t, omm::Selection_type, Model_element>;
}
