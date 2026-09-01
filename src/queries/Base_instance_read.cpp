/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Base_instance_read.h>
#include <omm/Logical_field.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <util/Native_type.h>
#include <qry/Util.h>






using namespace arolios;
using namespace arolios::qry;

Base_instance_read::Base_instance_read(const std::string& p_inst_id, const qry::Util::Read_mode p_usage, const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang )
: Base_query(p_lang), inst_id_str_(p_inst_id), app_info_ptr_ (common::Singleton<common:: App_info>::instance().object()), usage_ (p_usage) , filters_ (p_filters){


}

Base_instance_read::~Base_instance_read() {

}

void Base_instance_read::build_linked_property_json (const std::string& p_key, const std::shared_ptr<const omm::Property>& p_prop,   const qry::Db_row& p_row, Json::Value& p_jrow, const std::unordered_map<std::string,std::string>& p_fields_map, const std::unordered_map<std::string,std::string>& p_field_aliases_map)
{
      const auto key_vec = util::String::split(p_key,'.');
        
        std::string jkey = p_key;
        const auto root_key = key_vec[0];
        // size = 1 or 2
        if (key_vec.size() > 1) {
            if (!p_jrow.isMember(root_key)) {
                p_jrow[root_key] = Json::Value (Json::objectValue);
            }
            jkey = key_vec[1];
        }
        Json::Value& jrow = (key_vec.size() > 1) ? p_jrow[root_key] : p_jrow;

        std::string field_key;
        // take alias if exist
        auto && search = p_field_aliases_map.find ( p_key );
        if ( search != p_field_aliases_map.end() ) {
            field_key = search->second;
        } else {
            search = p_fields_map.find ( p_key );
            if ( search != p_fields_map.end() ) {
                field_key = search->second;
            } else {
                throw exception::Failure ( "field not found for property " + p_key ) ;
            }
        }

        const auto field = p_row[ field_key];
        if ( ! field.isNull() ) {
            util::Native_type native_type;
            if ( p_prop->has_referencing_type() ) {
                jrow[jkey] = Json::Value ( Json::objectValue );
                Json::Value& ref_jrow = jrow[jkey];
                const std::string csf_id_field_key  = field_key + "_csf_id";
                const auto csf_id_field = p_row[ csf_id_field_key];
                if ( csf_id_field.isNull()) {
                    throw exception::Failure ( "field not found for classifier of " + p_key ) ;
                }
                std::string iid = qry::Util::build_instance_id (csf_id_field.as<unsigned int>(), field.as<unsigned long>() );
                
                ref_jrow["id"] = Json::Value ( iid);
                ref_jrow["url"] = Json::Value ( qry::Util::url_prefix("instance") + iid);

            } else {
                if ( p_prop->has_enum_type() ) {
                    // enum value is like a char string
                    native_type = util::Native_type::character_varying;
                    // add translation of the enum value
                    auto field_enum_id = p_row[ field_key + "_val_id"];
                    if (!field_enum_id.isNull()) {
                        const std::string val_tlt = app_info_ptr_->find_enum_value(std::to_string(field_enum_id.as<unsigned int>()), lang_);
                        jrow["_t"+jkey] = Json::Value(val_tlt);
                    }
                    else {
                        const std::string error_msg = "Enum value identifier of "+ p_key + " not found";
                         
                        throw exception::Failure (error_msg); 
                    }
                } else {
                    // prop type is a primitive
                    native_type = static_cast<util::Native_type> ( p_prop->logical_field()->native_type() );
                }

                jrow[jkey] =  qry::Util::native_to_json ( native_type, field ) ;
            }
        }
    }

