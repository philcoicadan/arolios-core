/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Specific_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <omm/Constraint.h>
#include <omm/Composite_type.h>
#include <omm/Enumeration.h>
#include <omm/Primitive_type.h>
#include <omm/Class.h>

using namespace arolios;
using namespace arolios::omm;



void Specific_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

bool Specific_type::has_properties () const {
     return false;
}

bool  Specific_type::has_generalizations () const {
    return false;
}

bool Specific_type::has_id () const {
    // TODO
    throw exception::Not_yet_implemented("Specific_type::has_id");
    return false;
}

std::shared_ptr<Logical_struct> Specific_type::logical_struct() const {

    return (referenced_classifier_ != nullptr) ? referenced_classifier_->logical_struct() : nullptr;

}

void Specific_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem){
    p_pk.insert<Specific_type> (p_elem) ;
}


void Specific_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Specific_type> (p_elem) ;
}

void Specific_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Specific_type> (p_elem, p_new_name) ;

}

// void Specific_type::insert_element (const std::shared_ptr<Named_element>& p_elem) {
//      auto && elem = std::dynamic_pointer_cast<Constraint>(p_elem);
//     if (elem) {
//         insert_constraint(elem);
//     }

// }

// void Specific_type::erase_element (const std::shared_ptr<Named_element>& p_elem) {
//      auto && elem = std::dynamic_pointer_cast<Constraint>(p_elem);
//     if (elem) {
//         erase_constraint(elem);
//     }
// }
        
// bool Specific_type::has_elements (bool p_active_only) const {
//     if ( !p_active_only) {
//         return constraints().empty();
//     } else {
//         bool result = false;
//         for (auto  it = constraints().begin(); it != constraints().end() && result == false ; ++it ) {
//             if (it->second->lc_status() != Lc_status::active) continue;
//             result = true;
//         }
//         return result;
//     }
// }

       
    

const std::string& Specific_type::meta_type() const {
    static std::string mt = "specific";
    return mt;
}

// void Specific_type::rename_element (const std::shared_ptr<omm::Packageable_element>& p_elem, const std::string& p_new_name) {
//        if (referenced_type_ ) referenced_type_->rename_element (p_elem, p_new_name);
// }

void Specific_type::referenced_classifier(const std::shared_ptr<const Classifier>& p_type) {
    referenced_classifier_ = p_type;
}

const std::shared_ptr<const Classifier>& Specific_type::referenced_classifier() const {
    return referenced_classifier_;
}

void Specific_type::collection(const std::shared_ptr<Collection_attributes>& p_collection) {
    collection_ = p_collection;
}

const std::shared_ptr<Collection_attributes>& Specific_type::collection() const {
    return collection_;
}

std::shared_ptr<Table> Specific_type::table() const {
    // NYI
    return nullptr;
}

std::shared_ptr<Table> Specific_type::managing_table() const {
    //NYI
    return nullptr;
}
    
std::string Specific_type::read_select_template(const bool p_force ) {
    // NYI
    return std::string();
}

void Specific_type::make_more_specific_ids ( const bool p_force )  {
   // NYI
}

template <typename T> bool Specific_type::reference_is_a( const std::shared_ptr<const Classifier>& p_cls) {

    auto specific = std::dynamic_pointer_cast<const omm::Specific_type> ( p_cls);
    if (specific) {
        auto ref_type = specific->referenced_classifier();
        if ( ref_type) {
            if (std::dynamic_pointer_cast<const T> ( ref_type ).operator bool() ) {
                return true;
            } else {
                // recursive test
                return Specific_type::reference_is_a<T> (ref_type);
            }
        } else {
            return false;
        }
    } else {
        return false;
    }

}

template bool Specific_type::reference_is_a<omm::Composite_type> (const std::shared_ptr<const omm::Classifier>& p_cls);
template bool Specific_type::reference_is_a<omm::Enumeration> (const std::shared_ptr<const omm::Classifier>& p_cls);
template bool Specific_type::reference_is_a<omm::Primitive_type> (const std::shared_ptr<const omm::Classifier>& p_cls);
template bool Specific_type::reference_is_a<omm::Class> (const std::shared_ptr<const omm::Classifier>& p_cls);
