/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Class.h>
#include <tqry/Association.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <tqry/Query_visitor.h>



using namespace arolios;
using namespace arolios::tqry;

Class::Class() {

}

Class::~Class() {

}

    
void Class::model_class (const std::shared_ptr<const omm::Class>& p_class) {
    mdl_class_ = p_class;
}
    
const std::shared_ptr<const omm::Class>& Class::model_class () const {
    return mdl_class_;
}

const std::shared_ptr<const omm::Classifier> Class::model_classifier () const {
     return mdl_class_;
}

void Class::accept(Query_visitor& p_visitor)  {
    p_visitor.visit(*this);
}
   
void Class::insert_association (const std::shared_ptr<Association>& p_assoc) {
    assoc_map_.insert( { p_assoc->model_association()->id(), p_assoc});
    assoc_vec_.push_back( p_assoc);
}

const std::vector<std::shared_ptr<Association>>& Class::associations() const {
    return assoc_vec_;
}
