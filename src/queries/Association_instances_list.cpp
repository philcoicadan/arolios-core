/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Classifier_instances_list.h>
#include <omm/Association.h>
#include <omm/Column.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include "Classifier_instances_list.tpp"






 #include <qry/Util.h>



namespace arolios::qry {


template <> void Classifier_instances_list<omm::Association>::build_main_ids_json(Json::Value &p_jrow, const qry::Db_row &p_row,
                 const std::pair<std::string, std::string> &p_id_pair,
                 const std::pair<std::string, std::string> &p_csf_id_pair)
{

        
        const std::string id_str = qry::Util::build_instance_id (std::stoul(csf_->id()), p_row[p_id_pair.second].as<unsigned long>() ) ;
        p_jrow["url"] = Json::Value ( qry::Util::url_prefix("instance") + id_str);

        
}

template <> void Classifier_instances_list<omm::Association>::build_json_from_row ( Json::Value& p_jrow, const qry::Db_row& p_row, const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_props_map, const std::unordered_map<std::string,std::string>& p_columns_map, const std::unordered_map<std::string,std::string>& p_column_alias_map ) {


    
    for ( const auto& [key, value] : p_props_map ) {
 
        auto key_vec = util::String::split(key,'.');
        
        std::string jkey = key;

        Json::Value& jrow = (key_vec.size() > 1) ?
                    compute_json_key (p_jrow, key_vec, jkey) :
                    p_jrow;
        std::string field_key;
        // for an assoc, prop can have an alias name as column name
        auto && search_alias = p_column_alias_map.find ( key );
        if ( search_alias != p_column_alias_map.end() ) {
            field_key = search_alias->second;
        } else {
            // search in the column_map
            auto && search = p_columns_map.find ( key );
            if ( search != p_columns_map.end() ) {
                field_key = search->second;
            } else {
                throw exception::Failure ( "field not found for property " + key ) ;
            }
        }

        const auto field = p_row[ field_key];


        if ( ! field.isNull() ) {
            qry::Native_type native_type;
            if ( value->has_enum_type() ) {
                // enum value is like a char string
                native_type = qry::Native_type::_character_varying;
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
                native_type = static_cast<qry::Native_type> ( value->logical_field()->native_type() );
            }
            jrow[jkey] =  qry::Util::native_to_json ( native_type, field ) ;
        }
    }
}

template class Classifier_instances_list<omm::Association>;
}