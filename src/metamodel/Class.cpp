/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Class.h>
#include <omm/Package.h>
#include <omm/Classifier.h>
#include <omm/Table.h>
#include <omm/Property.h>
#include <omm/Logical_field.h>
#include <omm/Generalization.h>
#include <omm/Association.h>
#include <iostream>
#include <omm/Visitor.h>





using namespace arolios;
using namespace arolios::omm;

Class::Class (const bool p_is_abstract) : Classifier (p_is_abstract) {}


void Class::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}
       
std::shared_ptr<Logical_struct> Class::logical_struct() const {
    return table_;
}

std::shared_ptr<Table> Class::table() const  {
    return table_;
}

std::shared_ptr<Table> Class::managing_table() const {
    return table_;
}
        
void Class::table (const std::shared_ptr<Table>& p_table) {
    table_ = p_table;
}

void Class::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.insert<Class> (p_elem) ;
}


void Class::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Class> (p_elem) ;
}

void Class::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Class> (p_elem, p_new_name) ;

}

std::string Class::read_select_template( const bool p_force) {
    if (p_force) select_tpt_.clear();
    if (select_tpt_.length() == 0 ) {
        select_tpt_ = build_select_template ();
    }
    return select_tpt_;
}

std::string Class::build_select_template () {
    
    std::string stmt;
    
    for (auto && [name, prop] : all_elements_as_property() ) {
        if (prop->association() == nullptr) {
             stmt += prop->select_template ("");
        } else if (prop->association() && (prop->max_occurs() == 1) && !prop->has_sub_field() ) { // class instance is defined with an association because the cardinality max is 1 and has its own field
            stmt += prop->select_template ("");
            for (auto && [ aname, aprop] : prop->association()->own_properties()) {
                if (aprop->association() == nullptr) {
                    stmt += aprop->select_template("");
                }
            }
        }
    }   
        
    stmt.erase ( stmt.length()-1,1 );
    
    stmt += " FROM " + table_->full_name() ;
    return stmt;
}



std::shared_ptr<Property> Class::find_property_member_end( const std::string& p_prop_key_name) const {
    
std::shared_ptr<Property> result;

    if ( ! all_props_done_ ) {
       // all_props_ = all_elements_as_property();
        std::string error_msg = "Computation of all properties not done for class " + name();
        std::cerr << error_msg << std::endl;
        throw exception::Failure(error_msg);
    }
    
    auto && search = all_props_.find(p_prop_key_name);
    if (search != all_props_.end()) {
        if (search->second->association()) { //property is a member end
            result = search->second;
        }
    }
    return result;
}

void Class::make_more_specific_ids ( const bool p_force) {
    if (p_force) ids_w_all_spec_.clear();
    if (ids_w_all_spec_.empty()) {
        ids_w_all_spec_ += "csf_id=" + id();
        
        if ( ! (specializations().empty()) ) {
            for ( auto desc : specializations() ) {
               // more_specific_ids_.insert(desc.second->specific()->id());
                ids_w_all_spec_ += " OR ";
                desc.second->specific()->make_more_specific_ids(p_force);
                //const auto & desc_desc = desc.second->specific()->more_specific_ids();
                //more_specific_ids_.insert(desc_desc.begin(), desc_desc.end());
                ids_w_all_spec_ += desc.second->specific()->classifier_id_with_specializations();
            }
        }
    }

}

std::shared_ptr<omm::Class> Class::first_base_class () const {
    
    const auto & general = generalizations();
    return  general.empty() ?
        std::shared_ptr<omm::Class> (nullptr) : 
        std::dynamic_pointer_cast<omm::Class>( general.begin()->second->general() );
}



const std::string& Class::meta_type() const {
    static std::string mt = "class";
    return mt;
}

std::shared_ptr<Named_element> Class::find_element ( const std::string& p_name) const {
 

   // the same name is possible in a more general class
   auto search = own_properties().find(p_name) ;
   return  (search== own_properties().end()) ? nullptr : search->second;
}
    
