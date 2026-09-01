/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Classifier_instances_list.h>
#include <omm/Column.h>
#include <util/String.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <common/App_info.h>
#include "Classifier_instances_list.tpp"









namespace arolios::qry {


template <> void Classifier_instances_list<omm::Class>::build_main_ids_json(Json::Value &p_jrow, const qry::Db_row &p_row,
                 const std::pair<std::string, std::string> &p_id_pair,
                 const std::pair<std::string, std::string> &p_csf_id_pair)
{
    build_ids_json ( p_jrow, p_row, p_id_pair, p_csf_id_pair );
}

template <> void Classifier_instances_list<omm::Class>::build_json_from_row ( Json::Value& p_jrow, const qry::Db_row& p_row, const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_props_map, const std::unordered_map<std::string,std::string>& p_fields_map, const std::unordered_map<std::string,std::string>& p_field_aliases_map ) {


    for ( const auto& [key, value] : p_props_map ) {
        // key may be a nested property
        auto key_vec = util::String::split(key,'.');
        
        std::string jkey = key;

        Json::Value& jrow = (key_vec.size() > 1) ?
                    compute_json_key (p_jrow, key_vec, jkey) :
                    p_jrow;

 
        std::string field_key;
        // take alias if exist
        auto && search = p_field_aliases_map.find ( key );
        if ( search != p_field_aliases_map.end() ) {
            field_key = search->second;
        } else {
            search = p_fields_map.find ( key );
            if ( search != p_fields_map.end() ) {
                field_key = search->second;
            } else {
                throw exception::Failure ( "field not found for property " + key ) ;
            }
        }

        const auto field = p_row[ field_key];
        if ( ! field.isNull() ) {
            util::Native_type native_type;
            if ( value->has_referencing_type() ) {
                jrow[jkey] = Json::Value ( Json::objectValue );
                Json::Value& ref_jrow = jrow[jkey];
                const std::string csf_id_field_key  = field_key + "_csf_id";
                const auto csf_id_field = p_row[ csf_id_field_key];
                if ( csf_id_field.isNull()) {
                    throw exception::Failure ( "field not found for classifier of " + key ) ;
                }
                std::string iid = qry::Util::build_instance_id (csf_id_field.as<unsigned int>(), field.as<unsigned long>() );
                

                ref_jrow["url"] = Json::Value ( qry::Util::url_prefix("instance") + iid);

            } else {
                if ( value->has_enum_type() ) {
                    // enum value is like a char string
                    native_type = util::Native_type::character_varying;
                    // add translation of the enum value
                    auto field_enum_id = p_row[ field_key + "_val_id"];
                    if (!field_enum_id.isNull()) {
                        const std::string val_tlt = app_info_ptr_->find_enum_value(std::to_string(field_enum_id.as<unsigned int>()), lang_);
                        jrow["_t"+jkey] = Json::Value(val_tlt);
                    }
                    else {
                        const std::string error_msg = "Enum value identifier of "+ key + " not found";
                         
                        throw exception::Failure (error_msg); 
                    }
                } else {
                    // prop type is a primitive
                    native_type = static_cast<util::Native_type> ( value->logical_field()->native_type() );
                }

                jrow[jkey] =  qry::Util::native_to_json ( native_type, field ) ;
            }
        }
    }
}

template class Classifier_instances_list<omm::Class>;
}