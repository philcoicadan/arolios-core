/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <omm/Classifier.h>
#include <omm/Data_type.h>
#include <omm/Composite_type.h>
#include <omm/Primitive_type.h>
#include <omm/Property.h>
#include <omm/Enum_value.h>
#include <util/Native_type.h>

namespace arolios {




namespace common
{

class Object_model_info 
{
  public:
    
    enum Element_type { domain, classifier, property };
          
    Object_model_info() ;
    
    virtual ~Object_model_info ();
    

    std::shared_ptr<omm::Data_type> find_standard_type_by_name (const std::string& p_type_name, const bool p_only_active=true) const ;

    
    std::shared_ptr<omm::Classifier> find_classifier_by_id (const std::string& p_id, const bool p_only_active=true) const ;
 
    
    std::shared_ptr<omm::Package> find_domain_by_name (const std::string& p_pk_name, const bool p_only_active=true) const;
    std::shared_ptr<omm::Package> find_domain_by_id ( const std::string& p_pk_id , const bool p_only_active=true) const ;
    std::shared_ptr<omm::Property> find_property_by_id ( const std::string& p_prop_id , const bool p_only_active=true) const;
    std::shared_ptr<omm::Enum_value> find_enum_value_by_id ( const std::string& p_enum_val_id , const bool p_only_active=true) const;


    template <typename T1> void insert (const std::shared_ptr<T1>& p_elt) ;
    template <typename T1> void eliminate (const std::shared_ptr<T1>& p_elt) ;
    template <typename T1> void rename (const std::shared_ptr<T1>& p_elt, const std::string& p_new_name) ;

    void insert_classifier (const std::shared_ptr<omm::Classifier>& p_csf) ;
    void insert_property (const std::shared_ptr<omm::Property>& p_prop) ;
    void insert_enum_value (const std::shared_ptr<omm::Enum_value>& p_prop) ;

 
    
    
    const std::shared_ptr<omm::Package>& root_domain () const { return root_pk_; }
   
    
    const std::shared_ptr<omm::Package>& standard_domain () const { return std_pk_; }
    
    void root_domain ( const std::shared_ptr<omm::Package>& p_pk)  { root_pk_ = p_pk; }
    void standard_domain ( const std::shared_ptr<omm::Package>& p_pk) { std_pk_ = p_pk; }
    
    const std::unordered_map<std::string, std::shared_ptr<omm::Package>>& domains() const {return pk_name_map_;}
    
    
    template <typename T> std::shared_ptr<T> find_domain_element_by_id ( const std::string& p_id, const bool p_only_active=true ) const {
        return std::dynamic_pointer_cast<T> ( find_classifier_by_id(p_id, p_only_active ) );
    }

protected:



    std::shared_ptr<omm::Package> root_pk_;
    std::shared_ptr<omm::Package> std_pk_;
    std::unordered_map<std::string, std::shared_ptr<omm::Package>> pk_id_map_;
    std::unordered_map<std::string, std::shared_ptr<omm::Package>> pk_name_map_;
    std::unordered_map<std::string, std::shared_ptr<omm::Classifier>> csf_id_map_;
    std::unordered_map<std::string, std::shared_ptr<omm::Property>> prop_id_map_;
    std::unordered_map<std::string, std::shared_ptr<omm::Enum_value>> enum_val_id_map_;

    


   
  private:

};

}

}

