/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Member_end.h>
#include <tqry/Query_visitor.h>


using namespace arolios;
using namespace arolios::tqry;

Member_end::Member_end() : opt_assoc_(false) {

}

Member_end::~Member_end() {

}

void Member_end::target (const std::shared_ptr<Class>& p_class) {
    target_ = p_class;
}

const std::shared_ptr<Class>& Member_end::target () const {
    return target_;
}

void Member_end::query (const std::shared_ptr<Query_stmt>& p_query) {
    query_ = p_query;
}
    
const std::shared_ptr<Query_stmt>& Member_end::query () const {
    return query_;
}
    
void Member_end::association (const std::shared_ptr<Association>& p_assoc) {
    assoc_ = p_assoc;
}

const std::shared_ptr<Association>& Member_end::association () const {
    return assoc_;
}
void Member_end::accept(Query_visitor& p_visitor)  {
    p_visitor.visit(*this);
}
   
void Member_end::optional_association (bool p_flag) {
    opt_assoc_ = p_flag;
}

bool Member_end::optional_association() const {
    return opt_assoc_;
}

