/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Association.h>
#include <omm/Association.h>
#include <tqry/Query_visitor.h>



using namespace arolios;
using namespace arolios::tqry;

Association::Association() {

}

Association::~Association() {

}

    
void Association::model_association (const std::shared_ptr<const omm::Association>& p_assoc) {
    mdl_assoc_ = p_assoc;
}
    
const std::shared_ptr<const omm::Association>& Association::model_association () const  {
    return mdl_assoc_;
}
    
const std::shared_ptr<const omm::Classifier> Association::model_classifier () const {
    return mdl_assoc_;
}
       
void Association::insert_member_end (const std::shared_ptr<Member_end>& p_member_end) {
    ends_.push_back(p_member_end);

}
    
const std::vector<std::shared_ptr<Member_end>>& Association::member_ends () const  {

    return ends_;
}

void Association::from_class ( const std::shared_ptr<Class>& p_class) {
    from_class_ = p_class;
}
    
const std::shared_ptr<Class>& Association::from_class () const {
    return from_class_;
}

void Association::accept(Query_visitor& p_visitor)  {
    p_visitor.visit(*this);
}
    
