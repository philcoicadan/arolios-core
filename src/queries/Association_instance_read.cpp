/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Association_instance_read.h>
#include <omm/Property.h>
#include <omm/Classifier.h>
#include <omm/Package.h>
#include <omm/Logical_field.h>
#include <util/String.h>
#include <qry/Util.h>
#include <qry/Native_type.h>
#include <exception/Exceptions.h>
#include <appcfg/Class_instance_member_end.h>




using namespace arolios;
using namespace arolios::qry;

Association_instance_read::Association_instance_read( const std::shared_ptr<const omm::Association>& p_csf, const std::string& p_inst_id, const qry::Util::Read_mode p_usage , const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang) :
Instance_read<omm::Association> ( p_csf, p_inst_id, p_usage, p_filters, p_lang), om_ptr_(common::Singleton<common::Object_model_info>::instance().object()){

}

Association_instance_read::~Association_instance_read() {

}


void Association_instance_read::build_ids_json ( const qry::Db_row& p_row, Json::Value& p_root_json ) {
    
   

    auto && id_field = p_row["t0_id"];
    
    // difference with class instance read for avoiding to take class id of the table implementing the association
    p_root_json["id"] = Json::Value (qry::Util::build_instance_id(std::stoul(csf_->id()), id_field.as<unsigned long>()));
    
    p_root_json["association"] = Json::Value(Json::objectValue);
    p_root_json["association"]["url"]= qry::Util::url_prefix("association") + csf_->package()->name() + "." + csf_->name();
    p_root_json["association"]["_tid_name"]= csf_->package()->translated_name(lang_) + "." + csf_->translated_name(lang_);

    if (usage_ ==  qry::Util::Read_mode::read_for_update ) {
        auto && field3 = p_row["t0_upd_count"];
        qry::Native_type native_type = qry::Native_type::_integer;
        p_root_json["upd_count"] = qry::Util::native_to_json ( native_type, field3 ) ; ;
    }
    
}

const std::string Association_instance_read::get_classifier_ids () const {


    return csf_->classifier_id_with_specializations("t0");
}

void Association_instance_read::build_property_json ( const std::string& p_name, const std::shared_ptr<const omm::Property>& p_prop,  const std::shared_ptr<const omm::Association>& p_csf, const qry::Db_row& p_row, const std::shared_ptr<appcfg::Base_classifier_config>& p_config, Json::Value& p_root_json, const int p_recursive, const std::string& p_column_path, const std::string& p_table_alias) {
    
    
    std::string table_prefix = p_table_alias.empty() ? "" : p_table_alias + "_";
    
    if ( p_prop->has_composite_type() ) {
        p_root_json [p_name]= Json::Value(Json::objectValue);
        Json::Value& nested_json = p_root_json [p_name];
        auto && column_path = p_column_path + p_prop->logical_field()->name() + "_";

        for ( const auto & nested_prop : p_prop->nested_properties() ) {
            build_property_json ( nested_prop->name(), nested_prop, p_csf, p_row, p_config, nested_json , p_recursive+1, column_path, p_table_alias );
        }
    } else if (p_prop->has_referencing_type() ) {
        p_root_json [p_name]= Json::Value ( Json::objectValue );
        Json::Value& linked_json = p_root_json [p_name];
        auto member_end = p_config->class_instance_member_end ( p_name );
        auto && alias_map = member_end->property_alias_column_name_map_for_list();
        auto && column_map = member_end->property_column_name_map_for_list() ;
        std::string member_table_alias = member_end->table_alias();
        
        auto && me_id_pair = member_end->management_member_column_name("id");
        auto && me_csf_id_pair = member_end->management_member_column_name("csf_id");
        auto id = p_row[me_id_pair.second];
        auto csf_id = p_row[me_csf_id_pair.second];
        std::string iid = std::to_string(csf_id.as<unsigned int>()) + "." + std::to_string(id.as<unsigned long>());
        linked_json["url"] = Json::Value(qry::Util::url_prefix("instance") + iid);
        
        auto class_ptr = om_ptr_->find_domain_element_by_id<omm::Class>(std::to_string(csf_id.as<unsigned int>()));
        if (class_ptr) {
            linked_json["class"] = Json::Value ( Json::objectValue );
            linked_json["class"]["url"] = qry::Util::url_prefix("class") + class_ptr->package()->name() + "." + class_ptr->name();
            linked_json["class"]["_tid_name"] = class_ptr->package()->translated_name(lang_) + "." + class_ptr->translated_name(lang_);
        } else {
            std::string error_msg = "class identifier for member end of association " + csf_->name() + " not found";
            throw exception::Not_found ( error_msg );
        }

        linked_json["properties"] = Json::Value ( Json::objectValue );
        Json::Value&  jprops_linked = linked_json["properties"];
        for ( auto [ name, prop ] : member_end->property_map_for_list() ) {

            build_linked_property_json ( name, prop, p_row, jprops_linked,  column_map, alias_map);
        }

    } else if (! ( p_prop->has_sub_field()) ) {
        // only selectable properties associated to a column
        auto && column_name = table_prefix + p_column_path+  p_prop->logical_field()->name();
        auto && field = p_row[column_name];
        if ( !field.isNull() ) {
            qry::Native_type native_type;
            if ( p_prop->has_enum_type() ) {
                // enum value is like a char string
                native_type = qry::Native_type::_character_varying;
                                // add translation of the enum value
                auto field_enum_id = p_row[ column_name + "_val_id"];
                if (!field_enum_id.isNull()) {
                    const std::string val_tlt = app_info_ptr_->find_enum_value(std::to_string(field_enum_id.as<unsigned int>()), lang_);
                    p_root_json["_t"+p_name] = Json::Value(val_tlt);
                }
                else {
                    const std::string error_msg = "Enum value identifier of "+ p_name + " not found";
                     
                    throw exception::Failure (error_msg); 
                }
            } else {
                // prop type is a primitive
                native_type = static_cast<qry::Native_type> ( p_prop->logical_field()->native_type() );
            }
            p_root_json [p_name] = qry::Util::native_to_json ( native_type, field ) ;

        }
    }
}
    
