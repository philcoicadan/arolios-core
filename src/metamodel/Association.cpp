/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Property.h>
#include <omm/Association.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <omm/Table.h>
#include <omm/Class.h>
#include <omm/Generalization.h>
#include <iostream>
#include <memory>
#include <unordered_set>


using namespace arolios;
using namespace arolios::omm;


bool Association::has_id () const {
    return table_.operator bool();
}

std::vector<std::shared_ptr<Property>> Association::effective_member_ends () const {

    std::vector<std::shared_ptr<Property>> result;
    std::unordered_set<std::string> seen;

    for (int k = 0; k < 2; ++k) {
        if (member_end_[k]) {
            if (seen.insert(member_end_[k]->name()).second) {
                result.push_back(member_end_[k]);
            }
        }
    }

    // add inherited member ends, the nearest generalization first
    for (auto [first, second] : generalizations()) {
        auto base = std::dynamic_pointer_cast<Association>(second->general());
        if (base) {
            for (auto && me : base->effective_member_ends()) {
                if (me && seen.insert(me->name()).second) {
                    result.push_back(me);
                }
            }
        }
    }

    return result;
}


std::shared_ptr<Association> Association::first_base_association () const {

    const auto & general = generalizations();
    return  general.empty() ?
        std::shared_ptr<Association> (nullptr) :
        std::dynamic_pointer_cast<Association>( general.begin()->second->general() );
}


std::shared_ptr<const std::vector<std::shared_ptr<Element>>> Association::related_elements() const {
	std::shared_ptr<std::vector<std::shared_ptr<Element>>> result = std::make_shared<std::vector<std::shared_ptr<Element>>>();
	for (auto && me : effective_member_ends() ) {
        if (me) result->push_back(me->classifier());
    }
	return result;
}


std::shared_ptr<const std::vector<std::shared_ptr<Property>>> Association::member_ends() const {

	std::shared_ptr<std::vector<std::shared_ptr<Property>>> result = std::make_shared<std::vector<std::shared_ptr<Property>>>();
	for (auto && me : effective_member_ends() ) {
        if (me) result->push_back(me);
    }
	return result;
}

std::shared_ptr<Property> Association::find_member_end( const std::string& p_name) const {

    for (auto && me : effective_member_ends() ) {
        if (me && p_name == me->name()) {
            return me;
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

std::shared_ptr<const Classifier> Association::managing_table_class ()  const {
    return managing_table_class_;
}

void Association::managing_table_class (const std::shared_ptr<const Classifier>& p_class) {
    managing_table_class_ = p_class;
}

std::shared_ptr<Column> Association::managing_table_column () const {
    
    std::shared_ptr<Column> result;
    if (managing_table_class_) {
        for (auto && me : effective_member_ends() ) {
            result = me->column();
            if (result) break;
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

    return Classifier::find_element_in_all_properties (p_name) ;
}


void Association::make_all_elements_as_property (const bool p_force) {
    if (p_force) {
        all_props_done_ = false;
        all_props_.clear();
    }

    if ( ! all_props_done_ ) {
        
        // own and inherited properties
        Classifier::collect_all_elements_as_property (all_props_);
  
        // add member ends which are not own properties of this association
        for ( auto && me : effective_member_ends() ) {
            if ( me ) {
                if ( ! ( this == me->classifier().get() ) ) { // if the member end is not an own property, insert it
                    auto key_name = me->name();
                    // if the name already exists, suffix with the package and classifier names to get the uniqueness
                    const auto search = all_props_.find ( key_name ) ;
                    if ( search != all_props_.end() ) {
                        key_name = me->package()->name() + "_" + me->classifier()->name() + "_" + me->name();
                    }
                    all_props_.insert ( std::pair<std::string, std::shared_ptr<Property>> ( key_name, me ) );
                }
            }
        }
    

        all_props_done_ = true;
    }

}


std::unordered_map<std::string,std::shared_ptr<Property>> Association::all_id_properties () const {

    std::unordered_map<std::string,std::shared_ptr<Property>> result = Classifier::all_id_properties();
    
    // add member ends if not yet inserted
    for ( auto && me : effective_member_ends() ) {
      if (me) {
        auto key_name = me->name();
        // if the member is not a property of the association (most frequent
        // case) , insert it       
        const auto search = result.find(key_name);
        if (search == result.end()) {

          result.insert(std::pair<std::string, std::shared_ptr<Property>>(
              key_name, me));
        }
      }
    }

    
    return result;
}



std::shared_ptr<Property> Association::opposite_member_end ( const std::shared_ptr<const Property>& p_prop ) const {

    std::shared_ptr<Property> result;
    for ( auto && me : effective_member_ends() ) {
        if (me)  {
            if ( ! ( p_prop == me ) ) {
                result = me ;
                break;
            }
        }
    }
    return result;
}

void Association::make_more_specific_ids ( const bool p_force) {
    if (p_force) ids_w_all_spec_.clear();
  if (ids_w_all_spec_.empty()) {
      auto base = first_base_association();
      if (base) {
          // a derived association shares the ids of its base association
          base->make_more_specific_ids(p_force);
          ids_w_all_spec_ = base->classifier_id_with_specializations();
      } else if (managing_table_class_) {
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
    if ( ! effective_member_ends().empty() ) {
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
    for (auto && me : effective_member_ends() ) {
        if (p_prop == me) {
            return true;
        }
    }
    return false;
}


const std::string& Association::meta_type() const {
    static std::string mt = "association";
    return mt;
}
    
