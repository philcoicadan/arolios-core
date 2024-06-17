/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_row.h>
#include <omm/Logical_field.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <util/String.h>
#include <drogon/orm/Exception.h>
#include <mttr/Field_maker.h>






using namespace arolios;
using namespace arolios::mttr;

Classifier_instance_row::Classifier_instance_row(const std::shared_ptr<const omm::Logical_struct>& p_lo, const std::shared_ptr<const omm::Classifier>& p_prop_csf, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache) : lo_ (p_lo), prop_csf_(p_prop_csf) , inst_id_str_(std::to_string(0)), import_cache_(p_import_cache) {

}

Classifier_instance_row::~Classifier_instance_row() {

}

void Classifier_instance_row::add_field (const std::shared_ptr<const Instance_field>& p_field) {
    all_fields_.push_back(p_field);
}

void Classifier_instance_row::add_referencing_field (const std::shared_ptr<const Instance_field>& p_field) {
    referencing_fields_.push_back(p_field);
}

void Classifier_instance_row::add_nested_row (const std::shared_ptr<const Classifier_instance_row>& p_row) {
    nested_rows_.push_back(p_row);
}

std::string Classifier_instance_row::table_name() const {
    //only applicable if lo = table
    return lo_->full_name();
}


const std::string& Classifier_instance_row::instance_id() const {
    return inst_id_str_;
}

const std::vector<std::shared_ptr<const Instance_field>>& Classifier_instance_row::referencing_fields() const {
    return referencing_fields_;
}

const std::shared_ptr<const omm::Classifier>& Classifier_instance_row::classifier() const {
    return prop_csf_;
}

const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& Classifier_instance_row::import_cache () const {
    return import_cache_;
}

    
void Classifier_instance_row::build_fields ( const Json::Value& p_data, const std::vector<std::shared_ptr<const omm::Property_view>>& prop_views, const std::string& p_path_name ) {

    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();


    std::unordered_map<std::string, std::shared_ptr<const omm::Property_view>>  prop_map;
    for ( auto prop_v : prop_views ) {
        //fix issue with type composite 
        prop_map.insert ( std::pair<std::string,std::shared_ptr<const omm::Property_view>> ( prop_v->property()->name(), prop_v ) );
    }

    mttr::Field_maker f_maker ( *this,  p_path_name,  mdl_info ) ;
    
    if ( p_data ) {

        for ( auto item : p_data.getMemberNames() ) {
            // return tokens and standardize them
            if ( ! ( item[0] == '_' ) ) { // ignore the fields beginning with an underscore

                std::shared_ptr<const omm::Property_view> prop_v;
                

                auto && item_stddd = util::String::standardize ( item );
                const auto search = prop_map.find ( item_stddd );
                if ( search != prop_map.end() ) {
                    prop_v = search->second;
                    auto data = std::make_shared<Json::Value> ( p_data [item] );

                    build_field ( f_maker, data, prop_v, p_path_name );


                } else {
                    std::string warn_msg = "JSON member " + item + " doesn't map to a property of the classifier " + prop_csf_->name();
                    std::cout << "WARNING: " << warn_msg<< std::endl ;
                }
            }
        }

    }
}

void Classifier_instance_row::build_field (  Field_maker& p_f_maker, const std::shared_ptr<const Json::Value>& p_data, const std::shared_ptr<const omm::Property_view>& p_prop_view, const std::string& p_path_name ) {


    const auto & type = p_prop_view->property()->type();
    const auto & lf = p_prop_view->property()->logical_field();
    if ( lf ) {
        p_f_maker.data ( p_data );
        p_f_maker.logical_field ( lf ) ;
        p_f_maker.type ( type ) ;
        p_f_maker.property_view ( p_prop_view );
        type->accept ( p_f_maker ) ;

        // the value of this property is a target instance of an association  (cardinality O,N or 0,1) implemented in the table of tha target class. This value is the instance to be updated.


    } else {
        std::string warn_msg = "for the classifier " + prop_csf_->name() + " property " + p_prop_view->id_name()+ " cannot accept a value ";
        std::cout << "WARNING: " << warn_msg << std::endl;
    }
}

void Classifier_instance_row::prepare_request ( std::string& p_fields, std::string& p_values) const {
    
    for ( auto && field : all_fields_ ) {
        auto && name = field->column_full_name ();
        auto && val_expr = field->value_expression();
        p_fields += name + ", ";
        p_values += val_expr + ", ";
    }
    //recursive call for composites types
    for (auto && nested_row : nested_rows_ ) {
        nested_row->prepare_request( p_fields, p_values) ;
    }
    
    
}
