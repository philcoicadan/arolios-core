/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "omm/Classifier.h"
#include "omm/Property.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <model/Classifier_search.h>
#include <exception/Exceptions.h>

namespace arolios {



namespace model {
    class Util {
        public:
    

    static std::string search_in_standard_domain (const std::shared_ptr<const omm::Classifier>& p_type);

    static std::shared_ptr<const omm::Classifier> find_root_classifier ( const std::shared_ptr<const omm::Classifier>& p_cls) ;

    static std::string make_fts_fragment_from_field( const std::shared_ptr<const omm::Logical_field>& p_field, const std::string& p_path);

    static bool check_inheritance (const std::shared_ptr<omm::Classifier>& p_derived, const std::shared_ptr<omm::Classifier>& p_base) ;

    template <typename T> static std::shared_ptr<T> resolve_base ( const std::string& p_base_name, const std::shared_ptr<omm::Package>& p_pk, const std::shared_ptr<omm::Classifier>& p_stored ) {
        if (p_base_name.empty()) return nullptr;
        Classifier_search search ( p_base_name, p_pk );
        auto stored_base = search.execute<T>();
        if ( p_stored && stored_base ) {
            if ( Util::check_inheritance ( p_stored, std::dynamic_pointer_cast<omm::Classifier>(stored_base) ) ) {
                std::string error_msg = "cyclic inheritance with " + stored_base->name() + " and " + p_stored->name();
                throw exception::Model_error (error_msg);
            }
        }
        return stored_base;
    }

    static void store_identifier ( const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_csf, const std::unordered_map<std::string,std::shared_ptr<omm::Property>>& p_ident_map );

    static std::shared_ptr<omm::Property> find_property_in_inheritance ( const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_csf ) ;

    static void add_identifier ( const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_class, std::unordered_map<std::string,std::shared_ptr<omm::Property>>& ident_map );

    static std::string make_logical_struct_full_name (  const std::shared_ptr<const omm::Classifier>& p_type) ;
    static std::string make_logical_struct_name (  const std::shared_ptr<const omm::Classifier>& p_type) ;
    static std::string make_schema_name (  const std::shared_ptr<const omm::Package>& p_pk) ;
    static std::string make_column_type_full_name (const std::shared_ptr<const omm::Classifier>& p_type) ;
    static std::string make_logical_field_name (const std::shared_ptr<const omm::Property>& p_prop, const bool p_is_root = true) ;

    static std::string make_column_name_for_association (const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const omm::Classifier>& p_class, const std::shared_ptr<const omm::Association>& p_assoc);
    static std::string make_column_name_with_classifier (const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const omm::Classifier>& p_csf);
 
    };
}}

