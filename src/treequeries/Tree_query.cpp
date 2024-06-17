/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Tree_query.h>
#include <tqry/Classifier.h>
#include <exception/Exceptions.h>
#include <omm/Visitor.h>
#include <tqry/Prepare_visitor.h>
#include <tqry/Execute_visitor.h>




using namespace arolios;
using namespace arolios::tqry ;
Tree_query::Tree_query() {

}

Tree_query::~Tree_query() {

}

void Tree_query::root (const std::shared_ptr<Classifier>& p_csf) {
    root_ = p_csf;
}

const std::shared_ptr<Classifier>& Tree_query::root () const {
    return root_;
}

std::shared_ptr<omm::Namespace> Tree_query::own_namespace() const {
    return nullptr; // query is a root object
}

void Tree_query::own_namespace(const std::shared_ptr<omm::Namespace>& p_ns) {
    std::cout << "WARNING: setting namespace: nothing to do, query is a root object"<< std::endl;
}



void Tree_query::add_parameter (const std::string& p_elem) {

    params_.insert ( p_elem ) ;
    
}

  
        
bool Tree_query::has_parameters () const {
    return !params_.empty();
}

const std::unordered_set<std::string>& Tree_query::parameters () const {
    
    return params_;
} 

void Tree_query::prepare () {
    
    Prepare_visitor prepa (*this);
    if (root_) {
       root_->accept(prepa) ;
    }

}

void Tree_query::execute (Json::Value& p_result) {
    Execute_visitor exec (*this, p_result);
    if (root_) {
        root_->accept(exec);
    }
}
 
void Tree_query::accept (omm::Visitor& p_visitor) const {
    std::cout << "WARNING: no visitor accepted"<< std::endl;
} 

