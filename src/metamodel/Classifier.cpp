/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Property.h>
#include <omm/Generalization.h>
#include <omm/Constraint.h>
#include <omm/Classifier.h>
#include <omm/Package.h>
#include <omm/Table.h>
#include <regex>



using namespace arolios;
using namespace arolios::omm;

Classifier::Classifier ( const bool p_is_abstract) {
    is_abstract_ = p_is_abstract;
    all_props_done_ = false;
    more_general_classifiers_done_ = false;
}

bool Classifier::has_properties () const {
		return !properties_.empty();
}

bool  Classifier::has_generalizations () const {
	return !generalizations_.empty();
}

bool  Classifier::has_specializations () const {
	return !specializations_.empty();
}

const std::unordered_map<std::string,std::shared_ptr<Property>>& Classifier::all_elements_as_property () const {
    return all_props_;
}

void Classifier::reset_all_elements_as_property () {
    all_props_done_ = false;
    all_props_.clear();
}

void Classifier::make_all_elements_as_property(const bool p_force) {
    if (p_force) {
        all_props_done_ = false;
        all_props_.clear();
    }
    if (!all_props_done_) {

        collect_all_elements_as_property (all_props_);




        all_props_done_ = true;
    }
  
}

void Classifier::collect_all_elements_as_property (std::unordered_map<std::string, std::shared_ptr<Property>>& p_map)
{
        for ( auto [first, second] : properties_ ) {
            auto key_name = first;
            // if the name already exists in the inheritance tree, suffix with the package and classifier names to get the uniqueness 
            const auto search = p_map.find(key_name) ;
            if (search != p_map.end()) {
                key_name = second->package()->name() + "_" + second->classifier()->name() + "_" + second->name();
            }
            p_map.insert (std::pair<std::string, std::shared_ptr<Property>>(key_name, second ));
            
        }

        for ( auto [first, second] : generalizations_ ) {
            second->general()->collect_all_elements_as_property(p_map);
            
        }
}

void Classifier::reset_more_general_classifiers() {
    more_general_classifiers_done_ = false;
    more_general_classifiers_.clear();
}

const std::unordered_set<std::shared_ptr<Classifier>>& Classifier::more_general_classifiers() {

    if ( ! more_general_classifiers_done_  ) {

        for ( auto [first, second] : generalizations_ ) {

            const auto & set = second->general()->more_general_classifiers();
            more_general_classifiers_.insert ( set.begin(), set.end() );
        }

        for ( auto [first, second] : generalizations_ ) {
            more_general_classifiers_.insert ( second->general() );
        }
        more_general_classifiers_done_ =true;
    }
    return more_general_classifiers_;
}



void Classifier::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    auto && elem = std::dynamic_pointer_cast<Property>(p_elem);
    if (elem) {
        properties_.insert(std::make_pair<const std::string&,const std::shared_ptr<Property>&>(elem->name(),elem));
    }
}

void Classifier::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    
    auto search = properties_.find(p_elem->name());
    if ( search != properties_.end() ) {
        properties_.erase (search->first) ;
    } else {
        throw exception::Failure("element " + p_elem->name() + " not found" );
    }
}

void Classifier::rename_element (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) {
    auto && elem = std::dynamic_pointer_cast<Property>(p_elem);
    if (elem) {
        properties_.insert(std::make_pair<const std::string&,const std::shared_ptr<Property>&>(p_new_name,elem));
        properties_.erase (elem->name());
    }
}


std::shared_ptr<Property> Classifier::find_element_in_all_properties (const std::string& p_name) const {
    
    std::shared_ptr<omm::Property> result ;
 

    auto && search = all_props_.find(p_name);
    if (search != all_props_.end()) {
        result  = search->second;
    }

    return result;
}


const std::string  Classifier::classifier_id_with_specializations (const std::string& p_table_prefix) const {
    if (p_table_prefix.empty() ) {
        return ids_w_all_spec_;
    } else {
        const std::string fmt = p_table_prefix + ".csf";
        const std::regex expr ("csf");
        return std::regex_replace(ids_w_all_spec_, expr, fmt);
    }
}



std::unordered_map<std::string,std::shared_ptr<Property>> Classifier::all_id_properties () const {

    std::unordered_map<std::string,std::shared_ptr<Property>> result;
    
    const auto & all_props = all_elements_as_property();


    for ( auto [first, second] : all_props ) {
        if ( second->association () == nullptr  || second->max_occurs() == 1) { // we keep id as MemberEnd => association 1,N or 1,1 , join to do with the referenced table to select its own ids (without recursion)
            if (has_identifier(second) ) {
                result.insert (std::pair<std::string,std::shared_ptr<Property>>(first, second ));
            }
        }
    }
    
    return result;
}

void Classifier::insert_own_property( const std::shared_ptr<Property>& p_prop) {
    properties_.insert(std::pair<const std::string,const std::shared_ptr<Property>>(p_prop->name(), p_prop));
}

void Classifier::insert_generalization (const std::string& p_name, const std::shared_ptr<Generalization>& p_general) {
    generalizations_.insert(std::pair<const std::string,const std::shared_ptr<Generalization>>(p_name, p_general));
}

bool Classifier::has_elements (bool p_active_only) const {
    
    auto elts = own_properties();
    if (! p_active_only) {
        return !elts.empty();
    } else {
        bool result = false;
        for (auto  it = elts.begin(); it != elts.end() && result == false ; ++it ) {
            if (it->second->lc_status() != Lc_status::active) continue;
            result = true;
        }
        return result;
    }
}

void Classifier::recursive_lc_status(const omm::Lc_status p_status)
{
    auto elts = own_properties();

    for ( auto  it = elts.begin(); it != elts.end()  ; ++it ) {
        it->second->lc_status ( p_status );

    }

    lc_status ( p_status );
}

void Classifier::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
    if (!p_parent) {
        package (nullptr);
    }
    else {
        const auto & pk = std::dynamic_pointer_cast<omm::Package> (p_parent);
        if (pk) {
            package(pk);
        } else {
            throw exception::Failure ("bad type for parent object");
        }
        
    }
}


std::shared_ptr<Named_element> Classifier::find_element ( const std::string& p_name) const {
   auto search = properties_.find(p_name) ;
   return  (search== properties_.end()) ? nullptr : search->second;
}

std::shared_ptr<Namespace> Classifier::own_namespace() const  {
	//std::shared_ptr<Namespace> result = std::static_pointer_cast<Namespace> (package());
	return package();
}

const std::unordered_map<std::string,std::shared_ptr<Property>>& Classifier::identifier () const {
    return ident_map_;
}

void Classifier::identifier (const std::unordered_map<std::string,std::shared_ptr<Property>>& p_ident_names_map) {
    ident_map_.clear();
    for (auto [name, prop] : p_ident_names_map) {
        ident_map_.insert ({ prop->id(),prop} );
    }
}

bool Classifier::has_identifier (const std::shared_ptr<const Property>& p_prop) const  {
    const auto search = ident_map_.find(p_prop->id()) ;
    return (search != ident_map_.end() ) ? true: false;
}

void Classifier::insert_identifier (const std::shared_ptr<Property>& p_prop)  {
    ident_map_.insert ({ p_prop->id(),p_prop} );
}

    
void Classifier::erase_identifier (const std::shared_ptr<Property>& p_prop) {
    auto search = ident_map_.find(p_prop->id());
    if ( search != ident_map_.end() ) {
        ident_map_.erase (search->first) ;
    } else {
        throw exception::Failure("identifier " + p_prop->name() + " not found" );
    } 
}

bool Classifier::has_required_property (const std::shared_ptr<const Property>& p_prop) const
{

    return ( // identifier prop can be null else creation can be blocked
        (p_prop->is_required() && 
            (find_element_in_all_properties(p_prop->name()) != nullptr)
        )
    );
}

bool Classifier::is_kind_of (const std::shared_ptr<Classifier>& p_csf)  {

    if (id() == p_csf->id()) {
        return true;
    }
    else {
        const auto more_general = more_general_classifiers ();
        return (more_general.find(p_csf) != more_general.end());
    } 
}
