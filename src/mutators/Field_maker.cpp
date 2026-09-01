/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Field_maker.h>
#include <omm/Enumeration.h>
#include <omm/Primitive_type.h>
#include <util/String.h>
#include <mttr/Instance_field.h>
#include <mttr/Classifier_instance_row.h>
#include <omm/Logical_field.h>
#include <omm/Table.h>
#include <omm/Package.h>
#include <stdexcept>
#include <drogon/orm/Exception.h>
#include <common/Db_client.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <qry/Util.h>
#include <util/Native_type.h>








using namespace arolios;
using namespace arolios::mttr;

Field_maker::Field_maker( mttr::Classifier_instance_row & p_row, const std::string& p_path_name,  const std::shared_ptr<common::Object_model_info>& p_mdl_info) 
: row_(p_row), path_name_(p_path_name) ,  mdl_info_ (p_mdl_info) {
   
}

Field_maker::~Field_maker() {

}

    

void Field_maker::visit (const omm::Class& p_elem) {     
   // it is a reference property
    // native_type is a bigint
    auto && column_name = path_name_.length()>0 ? path_name_+"."+ lf_->name(): lf_->name();
    if ( data_->isObject() ) {

            const Json::Value& jtarget = *data_ ;
            if ( jtarget.isMember ( "id" ) ) {
                std::string target = jtarget["id"].asString();
                std::string iid ;

                if (target[0] == '$') {
                    target.erase(0,1);
                    target = util::String::standardize(target);
                    std::transform ( target.begin(), target.end(), target.begin(),
                            [] ( unsigned char c ) -> unsigned char {return std::tolower ( c ); } );
                    
                    auto search = row_.import_cache().find(target);
                    if ( search != row_.import_cache().end() ) {
                        const Json::Value& jinst = * ( search->second );
                        iid = jinst.asString();
                    } else {
                        const std::string error ( "instance value not found in the imported instances" );
                         
                        throw exception::Not_found ( error ); 
                    }
                } else {
                    iid = target;
                }
                const auto [csf_id, row_id] = qry::Util::split_instance_id(iid);
               
                const std::shared_ptr<const Json::Value> row_id_value = std::make_shared<const Json::Value>(Json::Value::UInt64(row_id));

                
                auto && field = std::make_shared<mttr::Instance_field> ( column_name, util::Native_type::bigint, row_id_value) ;
                
                check_instance_value (field->value_expression(), p_elem);
                
                row_.add_field ( field );
                row_.add_referencing_field(field);
                
                const std::shared_ptr<const Json::Value> csf_id_value = std::make_shared<const Json::Value>(csf_id);
                
                auto && csf_field = std::make_shared<mttr::Instance_field> ( column_name + "_csf_id", util::Native_type::integer, csf_id_value) ;
                
                row_.add_field ( csf_field );
            } else {
                std::string error_msg = "JSON data for property " + prop_v_->id_name() + " bad declared";
                throw exception::Data_error ( error_msg );
            }

        if ( !prop_v_->associated_property_views().empty() && jtarget.isMember("association")) {
            auto && lo = type_->logical_struct();
            const Json::Value& jassoc= jtarget["association"];
            if (jassoc.isObject()) {
              auto assoc_row = std::make_shared<mttr::Classifier_instance_row>(
                  lo, type_, row_.import_cache());
              assoc_row->build_fields(
                  jassoc, prop_v_->associated_property_views(), path_name_);
              row_.add_nested_row(assoc_row);
            }
            else {
                std::string error_msg = "JSON data for properties associated to " + prop_v_->id_name()+ " bad declared";
                 
                throw exception::Data_error( error_msg );
            }
        }
    } else {
            std::string error_msg = "JSON data for property" + prop_v_->id_name()+ " bad declared";
             
            throw exception::Data_error( error_msg );

    }
    
}
void Field_maker::check_instance_value (const std::string& p_inst_id, const omm::Class& p_elem) const {
    
    const auto & table = p_elem.table();
    if (table == nullptr) {
        std::string error_msg = "Class " + p_elem.name() + " not mapped to a relational object";  
        throw exception::Not_found (error_msg) ;
    }
    
    unsigned long  inst_id = 0;
    try {

        inst_id = std::stoul ( p_inst_id ) ;
    } catch ( const std::invalid_argument& e ) {

        throw exception::Data_error ("the instance value is invalid");

    } catch ( const std::out_of_range& e ) {

         throw exception::Data_error ("the instance value is invalid");

    }
    
    const std::string csf_id_list = p_elem.classifier_id_with_specializations();
        
    std::string stmt = "SELECT id FROM " + table->full_name() + " WHERE id=$1 AND trashed_date = 'epoch' AND (" + csf_id_list + " ) ";
    
    
    const std::shared_ptr<common::Db_client> db = common::Singleton<common::Db_client>::instance().object();
    
    auto f = db->execSqlAsyncFuture ( stmt, inst_id);
    
    auto result = f.get();
    
    if (result.size() == 0 ) {
        std::string error_msg = "Instance " + p_inst_id  + " is not an instance of classifier " + p_elem.name() ;
        throw exception::Sql_error(error_msg);
    }
}
    
void Field_maker::visit (const omm::Enumeration& p_elem) {
    // enum value inserted as a string. standardize the value before to insert
    const auto enum_value = util::String::standardize(data_->asString());
    std::shared_ptr<Json::Value> value = std::make_shared<Json::Value>(enum_value);
    auto column_name = path_name_.length()>0 ? path_name_+"."+ lf_->name(): lf_->name();
    auto field = std::make_shared<mttr::Instance_field> (column_name, util::Native_type::character_varying, value);
    row_.add_field (field);

    // additional field for translation
    column_name += "_val_id";
    const auto enum_val_ptr = p_elem.find_element_by_name<omm::Enum_value>(enum_value);
    if (enum_val_ptr) {
        value = std::make_shared<Json::Value>(util::String::stoui(enum_val_ptr->id()));
        field = std::make_shared<mttr::Instance_field> (column_name, util::Native_type::integer, value);
        row_.add_field (field);
    }
    else {
        const std::string error_msg = "Value " + enum_value + " invalid for enumeration " + p_elem.name();
         
        throw exception::Data_error (error_msg);
    }

}

void Field_maker::visit (const omm::Composite_type& p_type) {
     
        //composite type
        auto path_name = path_name_.length()>0 ? path_name_+"."+ lf_->name(): lf_->name();
        auto && lo = type_->logical_struct();
        if (!lo) {
            std::string error_msg = "composite type " + type_->name() + " cannot accept values";
            throw exception::Not_found ( error_msg );
        }
    
        if ( !lo->is_logical_type()) {
            std::string error_msg = "composite type " + type_->name() + " doesn't map correctly to a relational object";
            throw exception::Not_found ( error_msg );
        }
    
        auto nested_row = std::make_shared<mttr::Classifier_instance_row>(lo, type_, row_.import_cache());
        nested_row->build_fields ( *data_ , prop_v_->nested_property_views() , path_name) ;
        row_.add_nested_row (nested_row);
    
        

}

void Field_maker::visit (const omm::Primitive_type& p_type) {
     

    auto native_type = p_type.native_type();
    auto && column_name = path_name_.length()>0 ? path_name_+"."+ lf_->name(): lf_->name();

    auto && field = std::make_shared<mttr::Instance_field> (column_name, native_type, data_);
    row_.add_field (field);
        
}


void Field_maker::data (const std::shared_ptr<const Json::Value>& p_data) {
    data_ = p_data;
}

void Field_maker::type (const std::shared_ptr<const omm::Classifier>& p_type) {
    type_ = p_type;
}


void Field_maker::logical_field (const std::shared_ptr<const omm::Logical_field>& p_lf) {
    lf_ = p_lf;
}

void Field_maker::property_view (const std::shared_ptr<const omm::Property_view>& p_prop_v)  {
    prop_v_ = p_prop_v;
}
