/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Package.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Data_type.h>
#include <omm/Specific_type.h>
#include <omm/Enumeration.h>
#include <omm/Primitive_type.h>
#include <omm/Selection_type.h>
#include <omm/Composite_type.h>
#include <memory>
#include <omm/Visitor.h>
#include "Package.tpp"




using namespace arolios;
using namespace arolios::omm;

const std::unordered_map<std::string,std::shared_ptr<Package>>& Package::nested_packages() const{

	return nested_packages_;
}

const std::unordered_map<std::string,std::shared_ptr<Enumeration>>& Package::enumerations () const {
	return enums_;
}
const std::unordered_map<std::string,std::shared_ptr<Primitive_type>>& Package::primitive_types () const {
	return primitive_types_;
}
const std::unordered_map<std::string,std::shared_ptr<Selection_type>>& Package::selection_types () const {
	return selection_types_;
}
const std::unordered_map<std::string,std::shared_ptr<Composite_type>>& Package::composite_types () const {
	return composite_types_;
}
const std::unordered_map<std::string,std::shared_ptr<Specific_type>>& Package::specific_types () const {
	return specific_types_;
}
const std::unordered_map<std::string,std::shared_ptr<Class>>& Package::classes () const {
	return classes_;
}
const std::unordered_map<std::string,std::shared_ptr<Association>>& Package::associations () const {
	return assocs_;
}


void Package::schema (const std::shared_ptr<Logical_schema>& p_schema) {
    schema_ = p_schema ;
}

const std::shared_ptr<Logical_schema>& Package::schema () const {
    return schema_  ;
}

void Package::nesting (const std::shared_ptr<Package>& p_nesting) {
    nesting_ = p_nesting;
}

std::shared_ptr<Package> Package::package() const {
    return nesting_;
}

const std::unordered_map<std::string,std::shared_ptr<Packageable_element>>& Package::package_elements () const {
    return pk_elems_;
}

std::shared_ptr<Namespace> Package::own_namespace() const  {
	return package();
}

void Package::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

void Package::insert_element (const std::shared_ptr<Named_element>& p_elem) {

    auto && pk_elem = std::dynamic_pointer_cast<Packageable_element>(p_elem);
    if (pk_elem) {
        pk_elem->insert_in_package (*this, pk_elem);
    } else {
        throw exception::Failure("bad type for element" + p_elem->name());
    }

}

void Package::erase_element (const std::shared_ptr<Named_element>& p_elem) {

    auto && pk_elem = std::dynamic_pointer_cast<Packageable_element>(p_elem);
    if (pk_elem) {
        pk_elem->erase_from_package (*this, pk_elem);
    } else {
        throw exception::Failure("bad type for element" + p_elem->name());
    }

}
void Package::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.insert<Package> (p_elem) ;
}

void Package::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Package> (p_elem) ;
}

void Package::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Package> (p_elem, p_new_name) ;

}

    
bool Package::has_elements (bool p_active_only) const {
    
    auto elts = package_elements();
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
            

void Package::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
    if (!p_parent) {
        nesting_ = nullptr;
    }
    else {
        const auto & parent = std::dynamic_pointer_cast<omm::Package> (p_parent);
        if (parent) {
            nesting_ = parent;
        } else {
            throw exception::Failure ("bad type for parent object");
        }
        
    }
}


std::shared_ptr<Named_element> Package::find_element ( const std::string& p_name) const {
    auto search = pk_elems_.find(p_name);
    if ( search != pk_elems_.end()) {
        return search->second;
    } else {
        return nullptr;
    }
}

void Package::rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) {

    p_elem->rename_in_package(*this, p_elem,  p_new_name);

}
        
        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Class>>& Package::element_type_map() {
            return classes_;
        }
        
        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Association>>& Package::element_type_map() {
            return assocs_;
        }
        
        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Enumeration>>& Package::element_type_map() {
            return enums_;
        }
        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Primitive_type>>& Package::element_type_map() {
            return primitive_types_;
        }
        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Selection_type>>& Package::element_type_map() {
            return selection_types_;
        }
        
        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Composite_type>>& Package::element_type_map() {
            return composite_types_;
        }

        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Specific_type>>& Package::element_type_map() {
            return specific_types_;
        }
        template <>  std::unordered_map<std::string,std::shared_ptr<omm::Package>>& Package::element_type_map() {
            return nested_packages_;
        }
        
        template <>  const std::unordered_map<std::string,std::shared_ptr<omm::Class>>& Package::const_element_type_map() const {
            return classes_;
        }
        
        template <>  const std::unordered_map<std::string,std::shared_ptr<omm::Association>>& Package::const_element_type_map() const  {
            return assocs_;
        }
        
template <>  const std::unordered_map<std::string,std::shared_ptr<omm::Enumeration>>& Package::const_element_type_map() const {
    return enums_;
}
template <>  const std::unordered_map<std::string,std::shared_ptr<omm::Primitive_type>>& Package::const_element_type_map() const {
    return primitive_types_;
}
template <>  const std::unordered_map<std::string,std::shared_ptr<omm::Selection_type>>& Package::const_element_type_map() const {
    return selection_types_;
}
        
        template <>  const std::unordered_map<std::string,std::shared_ptr<omm::Composite_type>>& Package::const_element_type_map() const {
            return composite_types_;
        }
        
        template <>  const std::unordered_map<std::string,std::shared_ptr<omm::Package>>& Package::const_element_type_map() const {
            return nested_packages_;
        }

        template  void Package::insert<omm::Class> (const std::shared_ptr<Packageable_element>& p_elem );
        template  void Package::insert<omm::Association> (const std::shared_ptr<Packageable_element>& p_elem );
        template  void Package::insert<omm::Composite_type> (const std::shared_ptr<Packageable_element>& p_elem );
        template  void Package::insert<omm::Specific_type> (const std::shared_ptr<Packageable_element>& p_elem );
        template  void Package::insert<omm::Enumeration> (const std::shared_ptr<Packageable_element>& p_elem );
        template  void Package::insert<omm::Primitive_type> (const std::shared_ptr<Packageable_element>& p_elem );
        template  void Package::insert<omm::Selection_type> (const std::shared_ptr<Packageable_element>& p_elem );
        template  void Package::insert<omm::Package> (const std::shared_ptr<Packageable_element>& p_elem );

        template  void Package::rename<omm::Class> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;
        template  void Package::rename<omm::Association> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;
        template  void Package::rename<omm::Composite_type> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;
        template  void Package::rename<omm::Specific_type> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;
        template  void Package::rename<omm::Enumeration> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;
        template  void Package::rename<omm::Primitive_type> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;
        template  void Package::rename<omm::Selection_type> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;
        template  void Package::rename<omm::Package> (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;

        template  void Package::erase<omm::Class> (const std::shared_ptr<Packageable_element>& p_elem) ;
        template  void Package::erase<omm::Association> (const std::shared_ptr<Packageable_element>& p_elem) ;
        template  void Package::erase<omm::Composite_type> (const std::shared_ptr<Packageable_element>& p_elem) ;
        template  void Package::erase<omm::Specific_type> (const std::shared_ptr<Packageable_element>& p_elem) ;
        template  void Package::erase<omm::Enumeration> (const std::shared_ptr<Packageable_element>& p_elem) ;
        template  void Package::erase<omm::Primitive_type> (const std::shared_ptr<Packageable_element>& p_elem) ;
        template  void Package::erase<omm::Selection_type> (const std::shared_ptr<Packageable_element>& p_elem) ;
        template  void Package::erase<omm::Package> (const std::shared_ptr<Packageable_element>& p_elem) ;
        

