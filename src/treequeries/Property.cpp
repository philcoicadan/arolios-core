/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Property.h>
#include <tqry/Query_visitor.h>


using namespace arolios;
using namespace arolios::tqry;

Property::Property() {

}

Property::~Property() {

}

void Property::model_property ( const std::shared_ptr<omm::Property>& p_mdl_prop) {
    mdl_prop_ = p_mdl_prop;
}

const std::shared_ptr<omm::Property>& Property::model_property () const  {
    return mdl_prop_;
}
    
const std::shared_ptr<Classifier>& Property::classifier () const {
    return csf_;
}

void Property::classifier ( const std::shared_ptr<Classifier>& p_csf)  {
    csf_ = p_csf;
}
    
const std::shared_ptr<Property>& Property::nested_property () const {
    return nested_;
}
    
void Property::nested_property ( const std::shared_ptr<Property>& p_nested)  {
    nested_ = p_nested;
}

const std::string& Property::id_name () const {
    return id_name_;
}
    
    
void Property::id_name ( const std::string& p_name) {
    id_name_ = p_name;
}
    
    
void Property::accept(Query_visitor& p_visitor)  {
    p_visitor.visit(*this);
}
   
