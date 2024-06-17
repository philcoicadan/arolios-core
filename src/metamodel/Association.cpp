/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Property.h>
#include <omm/Association.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <omm/Table.h>
#include <omm/Class.h>
#include <iostream>
#include <memory>


using namespace arolios;
using namespace arolios::omm;


std::shared_ptr<const std::vector<std::shared_ptr<Element>>> Association::related_elements() const {
	std::shared_ptr<std::vector<std::shared_ptr<Element>>> result = std::make_shared<std::vector<std::shared_ptr<Element>>>();
	if (member_end_[0] ) result->push_back(member_end_[0]->classifier());
	if (member_end_[1] ) result->push_back(member_end_[1]->classifier());
	return result;
}


std::shared_ptr<const std::vector<std::shared_ptr<Property>>> Association::member_ends() const {

	std::shared_ptr<std::vector<std::shared_ptr<Property>>> result = std::make_shared<std::vector<std::shared_ptr<Property>>>();
	if (member_end_[0] ) result->push_back(member_end_[0]);
	if (member_end_[1] ) result->push_back(member_end_[1]);
	return result;
}

std::shared_ptr<Property> Association::find_member_end( const std::string& p_name) const {

    if (member_end_[0]) {
        if (p_name == member_end_[0]->name()) {
            return member_end_[0];
        }
    }
    if (member_end_[1]) {
        if (p_name == member_end_[1]->name()) {
            return member_end_[1];
        }
    } 
    
    return nullptr;
}

void Association::member_end ( const std::shared_ptr<Property>&  p_member_end) {
	if (member_end_[0] == nullptr) { // doublet not yet set
		member_end_[0] = p_member_end;
	}
	else if (member_end_[1] == nullptr) { // the second end to be set
		member_end_[1] = p_member_end;
	}
	else { // set the first end again and reset the second one
		member_end_[0] = p_member_end;
		member_end_[1] = nullptr;
	}
}
void Association::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::shared_ptr<Logical_struct> Association::logical_struct() const {
    return table_;
}

std::shared_ptr<Table> Association::table() const  {
    return table_;
}

std::shared_ptr<Table> Association::managing_table() const {
    if (table_) {
        return table_ ;
    } else  if (managing_table_class_) {
        return managing_table_class_->table();
    } else {
        return nullptr;
    }
}

std::shared_ptr<const Class> Association::managing_table_class ()  const {
    return managing_table_class_;
}

void Association::managing_table_class (const std::shared_ptr<const Class>& p_class) {
    managing_table_class_ = p_class;
}

std::shared_ptr<Column> Association::managing_table_column () const {
    
    std::shared_ptr<Column> result;
    if (managing_table_class_) {
        for (int k =0; k< 2 && !(result); ++k) {
            if (member_end_[k] ) result = member_end_[k]->column();
        }
    }
    return result;     
        

}


void Association::table (const std::shared_ptr<Table>& p_table) {
    table_ = p_table;
}

void Association::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.insert<Association> (p_elem) ;
}

void Association::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Association> (p_elem) ;
}

void Association::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Association> (p_elem, p_new_name) ;

}

std::string Association::read_select_template( const bool p_force) {
    if (p_force) select_tpt_.clear();
    if (select_tpt_.length() == 0 ) {
        select_tpt_ = build_select_template ();
    }
    return select_tpt_;
}

std::string Association::build_select_template () {
    
    std::string stmt;
    
    // all_elements_as_property
    for (auto && prop : all_elements_as_property() ) {
        stmt += prop.second->select_template ("");
    }

        
    stmt.erase ( stmt.length()-1,1 );
    
    auto table = managing_table();
    
    if ( ! (table) ) {

            std::string error_msg = "Association " + name() + " is not mapped correctly to a relational object";
            throw exception::Failure ( error_msg );

    }
    
    stmt += " FROM " + table->full_name() ;
    return stmt;
}

std::shared_ptr<Property> Association::find_element_in_all_properties (const std::string& p_name) const {
    
    
    // No inheritance for associations
    return find_element_by_name<Property> (p_name) ;
}


void Association::make_all_elements_as_property (const bool p_force) {
    if (p_force) {
        all_props_done_ = false;
        all_props_.clear();
    }

    if ( ! all_props_done_ ) {
        

        for ( auto [first, second] : own_properties() ) {
                all_props_.insert(std::pair<std::string, std::shared_ptr<Property>> (first, second ));
        }
        
  
        // add member if not in the own properties
        for ( int k=0; k< 2; ++k ) {
            if ( member_end_[k] ) {
                if ( ! ( this == member_end_[k]->classifier().get() ) ) { // if the member end is not an own property, insert it
                    auto key_name = member_end_[k]->name();
                    // if the name not in the map, suffix with the package and classifier names to get the uniqueness (case )
                    const auto search = all_props_.find ( key_name ) ;
                    if ( search != all_props_.end() ) {
                        key_name = member_end_[k]->package()->name() + "_" + member_end_[k]->classifier()->name() + "_" + member_end_[k]->name();
                    }
                    all_props_.insert ( std::pair<std::string, std::shared_ptr<Property>> ( key_name, member_end_[k] ) );
                }
            }
        }
    

        all_props_done_ = true;
    }

}


std::unordered_map<std::string,std::shared_ptr<Property>> Association::all_id_properties () const {

    std::unordered_map<std::string,std::shared_ptr<Property>> result = Classifier::all_id_properties();
    
    // add member ends if not yet inserted
    for (int k = 0; k < 2; ++k) {
      if (member_end_[k]) {
        auto key_name = member_end_[k]->name();
        // if the member is not a property of the association (most frequent
        // case) , insert it       
        const auto search = result.find(key_name);
        if (search == result.end()) {

          result.insert(std::pair<std::string, std::shared_ptr<Property>>(
              key_name, member_end_[k]));
        }
      }
    }

    
    return result;
}



std::shared_ptr<Property> Association::opposite_member_end ( const std::shared_ptr<const Property>& p_prop ) const {

    std::shared_ptr<Property> result;
    for ( int k=0; k< 2; ++k ) {
        if (member_end_[k] )  {
            if ( ! ( p_prop == member_end_[k] ) ) {
                result = member_end_[k] ;
                break;
            }
        }
    }
    return result;
}

void Association::make_more_specific_ids ( const bool p_force) {
    if (p_force) ids_w_all_spec_.clear();
  if (ids_w_all_spec_.empty()) {
      if (managing_table_class_) {
          ids_w_all_spec_ = managing_table_class_->classifier_id_with_specializations();
      } else {
          // Association with its own table and no inheritance
        ids_w_all_spec_ += "csf_id=" + id();
      }
  }
        
 

}
   

std::shared_ptr<Named_element> Association::find_element ( const std::string& p_name) const {
    
    std::shared_ptr<Named_element> result = Classifier::find_element(p_name);
     
    return result ? result : find_member_end(p_name);
}   


bool Association::has_elements (bool p_active_only) const {
    if (member_end_[0] || member_end_[1]  ) {
        return true;
    }
    else { 
        return Classifier::has_elements(p_active_only);
    }
}

void Association::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    auto prop = std::dynamic_pointer_cast<Property> (p_elem);
    if (prop) {
        if (this == prop->association().get()) {
            member_end(prop);
        }
        else {
            Classifier::insert_element(p_elem);
        }
    }
}

void Association::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    auto prop = std::dynamic_pointer_cast<Property> (p_elem);
    if (prop) {
        if (this == prop->association().get()) {
            if (prop == member_end_[0]) {
                member_end_[0] = nullptr;
            }
            else if (prop == member_end_[1]) {
                member_end_[1] = nullptr;
            }
            else {
                std::cout << "WARNING: Erase association element " << p_elem->name() << " failed" << std::endl;
            }
        }
        else {
            Classifier::erase_element(p_elem);
        }
    }    
}
bool Association::has_required_property (const std::shared_ptr<const Property>& p_prop) const
{
    return ( has_member(p_prop) ||
        (p_prop->is_required() && 
            (find_element_in_all_properties(p_prop->name()) != nullptr)
        )
    );
}

bool Association::has_member (const std::shared_ptr<const Property>& p_prop) const
{
    return (p_prop == member_end_[0] || p_prop == member_end_[1]);
}


const std::string& Association::meta_type() const {
    static std::string mt = "association";
    return mt;
}
    
