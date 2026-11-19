/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Property.h>
#include <omm/Column.h>
#include <omm/Attribute.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Composite_type.h>
#include <omm/Enumeration.h>
#include <omm/Primitive_type.h>
#include <omm/Specific_type.h>
#include <omm/Logical_field.h>
#include <omm/Visitor.h>
#include <memory>
#include <exception/Exceptions.h>



using namespace arolios;
using namespace arolios::omm;

Property::Property () { classifier_ = nullptr; type_ = nullptr; assoc_ = nullptr; aggregation_ = none;  /*part_of_id_ = false;*/ min_ = 1; max_ = 1; 
  
  
    
}


std::shared_ptr<Namespace> Property::own_namespace() const
{
	return classifier();
};

std::shared_ptr<Package> Property::package() const {
    return (classifier_) ? classifier_->package() : nullptr;
}

void Property::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    
    throw exception::Failure ("a property cannot be inserted directly in a package");
 
}

void Property::erase_from_package(Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    
// Nothing to do
}

void Property::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
   // Nothing to do 

}

void Property::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

const std::shared_ptr<Column> Property::column () const {
    
   return  std::dynamic_pointer_cast<omm::Column>(field_) ;
}

const std::shared_ptr<Attribute > Property::attribute() const {
   return  std::dynamic_pointer_cast<omm::Attribute>(field_) ;
}

std::string Property::select_template ( const std::string& p_col_path , const std::string& p_col_alias) {
    std::string result;
    if ( has_composite_type() ) {
        const std::string col_path = "(" + p_col_path+ logical_field()->name() + ").";
        const std::string col_alias = p_col_alias + logical_field()->name() + "_";
        for ( const auto & nested_prop : nested_properties() ) {
            auto && column_name = nested_prop->select_template ( col_path, col_alias );
            if (column_name.length() > 0) {
                result += column_name ;
            }
        }

    } else if (! has_sub_field() ) {
        auto && lf = logical_field();
        if ( lf ) {
            result = p_col_path+ lf->name();
            if (!p_col_alias.empty()) {
                result += " AS " + p_col_alias + lf->name() ;
            } 
            result += ","; 
            if ( assoc_ && max_ == 1 ) {
                result += p_col_path+ lf->name() + "_csf_id";
                if (!p_col_alias.empty()) {
                    result += " AS " + p_col_alias + lf->name() + "_csf_id" ;
                }
                result += ","; 
            }
            const auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(type());
            if (enum_ptr) {
                result += p_col_path+ lf->name() + "_val_id";
                if (!p_col_alias.empty()) {
                    result += " AS " + p_col_alias + lf->name() + "_val_id" ;
                }
                result += ",";
            }
            
        }
    }
    return result;
}

const std::vector<std::shared_ptr<omm::Property>>& Property::nested_properties () const {
 
    return nested_properties_;

}

std::shared_ptr<omm::Property> Property::find_nested_property (const std::string& p_name) const 
{
    std::shared_ptr<omm::Property> result;
    if (has_composite_type()) {
        result =  std::dynamic_pointer_cast<omm::Property>(type_->find_element(p_name))  ; 
    }
    return result;
}

void Property::type(const std::shared_ptr<const Classifier>& p_type) { 
    type_ = p_type;
    nested_properties_.clear();

    if (has_composite_type()) {
        for ( auto && [first, second] : type_->own_properties() ) {
                nested_properties_.push_back ( second );
        }
    }
        
}


bool Property::has_composite_type() const {
    return  (std::dynamic_pointer_cast<const omm::Composite_type> ( type_ ).operator bool() ||
      Specific_type::reference_is_a<omm::Composite_type>(type_) ); 
}

bool Property::has_enum_type() const {
    return (std::dynamic_pointer_cast<const omm::Enumeration> ( type_ ).operator bool() ||
     Specific_type::reference_is_a<omm::Enumeration>(type_) );
}

bool Property::has_primitive_type() const {
    return ( std::dynamic_pointer_cast<const omm::Primitive_type> ( type_ ).operator bool() ||
     Specific_type::reference_is_a<omm::Primitive_type>(type_) );
}

bool Property::has_instance_type () const {
    
    if (type_) {
        return type_->has_id();
    } else {
        return false;
    }
}


bool Property::has_sub_field () const {
    return sub_field_.operator bool() ;
}

bool Property::is_required () const {
    return (min_ > 0 ) ? true: false ;
}

void Property::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
    if (!p_parent) {
        classifier(nullptr);
    }
    else {
        const auto & parent = std::dynamic_pointer_cast<omm::Classifier> (p_parent);
        if (parent) {
            classifier(parent);
        } else {
            throw exception::Failure ("bad type for parent object");
        }
        
    }
}

bool Property::is_end_property () const {

    return (classifier_ == assoc_ )? true: false ;

}

bool Property::has_many_targets () const {

	return  (max_ == 1 ) ? false : true ;

}

void Property::rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) {
// nothing to do, no element
}

