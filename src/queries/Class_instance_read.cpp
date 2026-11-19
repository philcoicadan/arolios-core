/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Class_instance_read.h>
#include <appcfg/Base_classifier_config.h>
#include <omm/Property.h>
#include <omm/Classifier.h>
#include <omm/Association.h>
#include <omm/Package.h>
#include <omm/Logical_field.h>
#include <util/String.h>
#include <qry/Util.h>
#include <util/Native_type.h>
#include <exception/Exceptions.h>
#include <json/value.h>


using namespace arolios;
using namespace arolios::qry;

Class_instance_read::Class_instance_read(const std::shared_ptr<const omm::Class>& p_csf, const std::string& p_inst_id, const qry::Util::Read_mode p_usage , const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang) :
Instance_read<omm::Class> ( p_csf, p_inst_id, p_usage, p_filters, p_lang) {


}

Class_instance_read::~Class_instance_read() {

}


void Class_instance_read::build_ids_json ( const qry::Db_row& p_row, Json::Value& p_root_json ) {
    

    p_root_json["id"] = Json::Value (qry::Util::build_instance_id(p_row));

    p_root_json["class"] = Json::Value(Json::objectValue);
    p_root_json["class"]["url"]= qry::Util::url_prefix("class") + csf_->package()->name() + "." + csf_->name();
    p_root_json["class"]["_tid_name"]= csf_->package()->translated_name(lang_) + "." + csf_->translated_name(lang_);

   
    if (usage_ ==  qry::Util::Read_mode::read_for_update ) {
        auto && field3 = p_row["upd_count"];
        util::Native_type native_type = util::Native_type::integer;
        p_root_json["upd_count"] = qry::Util::native_to_json ( native_type, field3 ) ; ;
    }
    
}

const std::string Class_instance_read::get_classifier_ids () const {
    

    return  csf_->classifier_id_with_specializations(); 

}

void Class_instance_read::build_property_json ( const std::string& p_name, const std::shared_ptr<const omm::Property>& p_prop,  const std::shared_ptr<const omm::Class>& p_csf, const qry::Db_row& p_row, const std::shared_ptr<appcfg::Base_classifier_config>& p_config, Json::Value& p_root_json, const int p_recursive, const std::string& p_column_path, const std::string& p_table_alias) {
   
    if ( p_prop->has_composite_type() ) {
       p_root_json [p_name]= Json::Value(Json::objectValue);
        Json::Value& nested_json = p_root_json [p_name];
        auto && column_path = p_column_path + p_prop->logical_field()->name() + "_";

        for ( const auto & nested_prop : p_prop->nested_properties() ) {
            build_property_json ( nested_prop->name(), nested_prop, p_csf, p_row, p_config, nested_json , p_recursive+1, column_path ,p_table_alias );
        }
    } else if ( p_prop->has_instance_type() ) {
        if ( p_prop->has_sub_field() || p_prop->max_occurs() != 1) {
            // no column but necessary for navigation in the associations
            p_root_json [p_name]= Json::Value(Json::objectValue);
            p_root_json [p_name]["association"] = Json::Value(Json::objectValue);
            Json::Value& assoc_json = p_root_json [p_name]["association"];
            
            assoc_json["id_name"]= p_prop->association()->package()->name() + "." + p_prop->association()->name();
            assoc_json["_tid_name"]= p_prop->association()->package()->translated_name(lang_) + "." + p_prop->association()->translated_name(lang_);
        } else {
            auto && column_name = p_column_path+ p_prop->logical_field()->name();
            auto && field = p_row[column_name];
            if ( !field.isNull() ) {
                p_root_json [p_name]= Json::Value(Json::objectValue);
                p_root_json [p_name]["association"] = Json::Value(Json::objectValue);

                Json::Value& assoc_json = p_root_json [p_name]["association"];
                assoc_json["url"]= qry::Util::url_prefix("association") + p_prop->association()->package()->name() + "." + p_prop->association()->name();
                assoc_json["_tid_name"]= p_prop->association()->package()->translated_name(lang_) + "." + p_prop->association()->translated_name(lang_);

               
                auto && row_id_field = p_row["id"];
               const std::string iid = qry::Util::build_instance_id(std::stoul(p_prop->association()->id() ),row_id_field.as<unsigned long>());
                assoc_json["instance_url"] = Json::Value (qry::Util::url_prefix("instance") + iid);

                
                if ((p_prop->min_occurs() == 1) && (p_prop->max_occurs() == 1)) {
                    assoc_json["properties"] = Json::Value (Json::objectValue);
                    Json::Value& assoc_prop_json = assoc_json["properties"];
                   for ( auto  [a_name, a_prop] : p_prop->association()->own_properties()) {
                       if (a_prop->association() == nullptr) {
                           build_property_json(a_name, a_prop, p_csf, p_row, p_config, assoc_prop_json, p_recursive, p_column_path, p_table_alias);
                       }
                   }
                }
                
                Json::Value&  target_json = p_root_json [p_name];
                target_json["class"] = Json::Value(Json::objectValue);
                target_json["class"]["url"] = qry::Util::url_prefix("class") + p_prop->type()->package()->name() + p_prop->type()->name(); 
                target_json["class"] ["_tid_name"] = p_prop->type()->package()->translated_name(lang_) + "." + p_prop->type()->translated_name(lang_);
               
                const std::string csf_column_name = column_name + "_csf_id";
                auto && csf_id_field = p_row[csf_column_name];
                if (csf_id_field.isNull()) {
                    throw exception::Failure ( "field not found for classifier of " + p_prop->name() ) ;
                }
                std::string target_iid = qry::Util::build_instance_id ( csf_id_field.as<unsigned int>() , field.as<unsigned long>() );
                    
                target_json["id"] = Json::Value ( target_iid);
                target_json["url"] = Json::Value (qry::Util::url_prefix("instance") + target_iid);

               

            }
        }
    } else if (! ( p_prop->has_sub_field()) ) {
        // only selectable properties associated to a column
        auto && column_name = p_column_path+ p_prop->logical_field()->name();
        auto && field = p_row[column_name];
        if ( !field.isNull() ) {
            util::Native_type native_type;
            if ( p_prop->has_enum_type() ) {
                // enum value is like a char string
                native_type = util::Native_type::character_varying;
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
                native_type = static_cast<util::Native_type> ( p_prop->logical_field()->native_type() );
            }
               p_root_json [p_name] = qry::Util::native_to_json ( native_type, field ) ;


        }
    }
}
    
