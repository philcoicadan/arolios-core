/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <mttr/Instance_field.h>
#include <omm/Logical_struct.h>
#include <omm/Property_view.h>
#include <memory>

namespace arolios {

namespace mttr {
    class Field_maker;
class Classifier_instance_row {
public:
    /**
     * Constructor
     */
    Classifier_instance_row(const std::shared_ptr<const omm::Logical_struct>& p_lo, const std::shared_ptr<const omm::Classifier>& p_prop_csf, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache);

    /**
     * Destructor
     */
    virtual ~Classifier_instance_row();
    
    void add_field (const std::shared_ptr<const Instance_field>& p_field);
    void add_referencing_field (const std::shared_ptr<const Instance_field>& p_field);
    void add_nested_row (const std::shared_ptr<const Classifier_instance_row>& p_row);
    

    std::string table_name() const;

    const std::string& instance_id() const;
    
    void build_fields (const Json::Value& p_data, const std::vector<std::shared_ptr<const omm::Property_view>>& prop_views, const std::string& p_path_name = std::string()) ;
    virtual void build_field (  Field_maker& p_f_maker, const std::shared_ptr<const Json::Value>& p_data, const std::shared_ptr<const omm::Property_view>& prop_view, const std::string& p_path_name = std::string());

    
    void prepare_request ( std::string& p_fields, std::string& p_values) const ;
    
    const std::vector<std::shared_ptr<const Instance_field>>& referencing_fields() const;
    
    const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& import_cache () const;
    
protected:
    
    const std::shared_ptr<const omm::Classifier>& classifier() const;


private:
    
    std::vector<std::shared_ptr<const Instance_field>> all_fields_ ;
    std::vector<std::shared_ptr<const Instance_field>> referencing_fields_ ;
    std::vector<std::shared_ptr<const Classifier_instance_row>> nested_rows_  ;
    std::shared_ptr<const omm::Logical_struct> lo_;
    std::shared_ptr<const omm::Classifier> prop_csf_;
    std::string inst_id_str_;
    const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& import_cache_;

};

}
}

