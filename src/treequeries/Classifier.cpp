/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Classifier.h>
#include <tqry/Property.h>
#include <tqry/Member_end.h>
#include <omm/Property.h>


using namespace arolios;
using namespace arolios::tqry;

Classifier::Classifier(): is_root_ (false) , filter_(std::string()), table_alias_(std::string()){
    

}

Classifier::~Classifier() {

}

const std::string& Classifier::filter() const  {
    return filter_;
}
    
void Classifier::filter (const std::string& p_filter) {
    filter_ = p_filter;
}

const std::string& Classifier::table_alias() const  {
    return table_alias_;
}
    
void Classifier::table_alias (const std::string& p_alias) {
    table_alias_ = p_alias;
}

const std::vector<std::shared_ptr<Property>>& Classifier::properties () const {
    return props_vec_;
}

    
void Classifier::insert_property (const std::shared_ptr<Property>& p_prop) {
    
    props_map_.insert ( {p_prop->model_property()->name(), p_prop} );
    props_vec_.push_back(p_prop);
}

const std::vector<std::shared_ptr<Member_end>>& Classifier::member_ends () const {
    return member_ends_vec_ ;
}
    
void Classifier::insert_member_end (const std::shared_ptr<Member_end>& p_me) {
    member_ends_map_.insert ( {p_me->model_property()->name(), p_me} );
    member_ends_vec_.push_back(p_me);
}

void Classifier::is_root ( bool p_flag) {
    is_root_ = p_flag;
}

bool Classifier::is_root () const {
    return is_root_;
}

void Classifier::query (const std::shared_ptr<Query_stmt>& p_query) {
    query_ = p_query;
}
    
const std::shared_ptr<Query_stmt>& Classifier::query () const {
    return query_;
}
