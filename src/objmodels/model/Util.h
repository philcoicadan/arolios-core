/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <memory>
#include <unordered_map>

namespace arolios {



namespace omm {
    class Classifier;
    class Class;
    class Package;
    class Property;
    class Logical_field;
    class Association;
}

namespace model {
    class Util {
        public:
    

    static std::string search_in_standard_domain (const std::shared_ptr<const omm::Classifier>& p_type);

    static std::shared_ptr<const omm::Classifier> find_root_classifier ( const std::shared_ptr<const omm::Classifier>& p_cls) ;

    static std::string make_fts_fragment_from_field( const std::shared_ptr<const omm::Logical_field>& p_field, const std::string& p_path);

    static bool check_inheritance (const std::shared_ptr<omm::Class>& p_derived, const std::shared_ptr<omm::Class>& p_base) ;

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

