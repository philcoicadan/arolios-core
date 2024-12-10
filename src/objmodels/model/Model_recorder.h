/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <common/Db_client.h>
#include <qry/Db_types.h>
#include <omm/Persistent_object.h>
#include <omm/Package.h>
#include <appcfg/App.h>
#include <model/Base_element.h>
#include <omm/Element_attributes.h>

namespace arolios {





namespace omm {
    class Class;
    class Property;
}

namespace model {



class Model_recorder {
public:

    Model_recorder ( );
    virtual ~Model_recorder();
    
    void rollback () ;
    
    template <typename T1> std::shared_ptr<omm::Persistent_object> create( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const std::string& p_desc, const omm::Element_attributes& p_attrs) const;
    
    template <typename T1> void update (const std::shared_ptr<T1>& p_elt,  const std::string& p_name, const std::string& p_desc,  const omm::Element_attributes& p_attrs) const;
    template <typename T1> void deprecate (const std::shared_ptr<T1>& p_elt) const;
    template <typename T1> bool eliminate (const std::shared_ptr<T1>& p_elt) const;
    
    void update_translation (const std::shared_ptr<omm::Persistent_object>& p_elt, const std::string& p_lang_id, const std::string& p_translation) const;
    
    std::shared_ptr<omm::Persistent_object> store_class ( const std::string& name, const std::string& elem_type, const unsigned int pk_id, const unsigned int p_base_id, const bool p_is_abstract, const std::string& p_desc ) const;
    std::shared_ptr<omm::Persistent_object> store_class ( const std::string& name, const std::string& elem_type, const unsigned int pk_id, const bool p_is_abstract, const std::string& p_desc) const;
    std::shared_ptr<omm::Persistent_object> store_classifier ( const std::string& name, const std::string& elem_type, const unsigned int pk_id, const std::string& p_desc ) const;
    std::shared_ptr<omm::Persistent_object> store_enumeration_value ( const std::string& p_token, const unsigned int enum_id ) const;
    std::shared_ptr<omm::Persistent_object> store_property ( const std::string& p_name, const unsigned int classifier_id, const unsigned int type_id,
                          const int p_min_occurs, const int p_max_occurs, const std::string& p_computed, const std::string& p_default, /*const bool p_part_of_id,*/ const std::string& p_desc ) const;
    std::shared_ptr<omm::Persistent_object> store_member_end ( const std::string& p_name, const unsigned int pk_elem_target_id, 
                            const unsigned int pk_elem_source_id, const unsigned int pk_assoc_id,
                            const int p_min_occurs, const int p_max_occurs, const bool p_is_source_class_property, const bool p_is_composition, /*bool p_part_of_id,*/ const std::string& p_desc ) const;
    
    void store_app_domain ( const std::shared_ptr<const omm::Package>& p_pk, const std::shared_ptr<const appcfg::App>& p_app) const;

    void update_classifier (const std::shared_ptr<omm::Classifier>& p_elt,  const std::string& p_name, const std::string& p_desc) const;
    void deprecate_classifier (const std::shared_ptr<omm::Classifier>& p_elt) const;
    bool eliminate_classifier (const std::shared_ptr<omm::Classifier>& p_elt) const;
    
    void update_identifier ( const std::shared_ptr<omm::Classifier>& p_csf, const std::unordered_map<std::string,std::shared_ptr<omm::Property>>& p_ident_map) const ;
    void reset_identifier ( const std::shared_ptr<omm::Classifier>& p_csf) const ;
    
    
    void update_config_collection_props ( const std::shared_ptr<appcfg::App_domain>& p_app_dom );

    friend class Model_implementor;
    
protected:
    
    const std::shared_ptr<qry::Db_transaction>& transaction() const {
        return transPtr_;
    }
    
private:


    std::shared_ptr<qry::Db_transaction> transPtr_;
    
    void store_optional_description ( const std::string& p_desc, const std::shared_ptr<const omm::Persistent_object>& p_elem) const;
    
    void clear_config_collection_props (const std::shared_ptr<omm::Package>& p_domain) ;


};
}


}

