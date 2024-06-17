/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Property_view.h>
#include <omm/Class.h>


using namespace arolios;
using namespace arolios::omm;

Property_view::Property_view(const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_id_name) :
 prop_ (p_prop) , id_name_ (p_id_name), metatype_ (Metatype::_primitive) , is_inherited_ (false) {
    

}

Property_view::~Property_view() {
	
}

const std::string&  Property_view::id_name() const {
    return id_name_;
} 

const std::shared_ptr<const omm::Property>& Property_view::property () const {
    return prop_;
}

void Property_view::metatype (const Property_view::Metatype p_metatype) {
    metatype_ = p_metatype;
}

const Property_view::Metatype Property_view::metatype() const
{
    return metatype_;
}

const std::string& Property_view::metatype_to_string() const
{
 
    static const std::string tab [4] = { "_primitive", "_enumeration", "_composite_type", "_class" };
    return tab[ static_cast<int>(metatype_) ] ;
}

void Property_view::is_inherited (const bool p_is_inherited)  {
    is_inherited_ = p_is_inherited;
}

const bool Property_view::is_inherited() const {
    return is_inherited_;
}

const std::vector<std::shared_ptr<const omm::Property_view>>& Property_view::nested_property_views () const {
    return nested_prop_views_;
}
    
const std::vector<std::shared_ptr<const omm::Property_view>>& Property_view::linked_property_views () const {
    return linked_prop_views_;
}

const std::vector<std::shared_ptr<const omm::Property_view>>& Property_view::associated_property_views () const {
    return associated_prop_views_;
}
    
void Property_view::insert_linked_property_view (const std::shared_ptr<const Property_view>& p_prop_v) {
    linked_prop_views_.push_back(p_prop_v);
}

void Property_view::insert_nested_property_view (const std::shared_ptr<const Property_view>& p_prop_v) {
        nested_prop_views_.push_back(p_prop_v);
}

void Property_view::insert_associated_property_view( const std::shared_ptr<const Property_view>& p_prop_v) {
     associated_prop_views_.push_back(p_prop_v);
   
}

