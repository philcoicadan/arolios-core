/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_wlo_create.h>
#include <util/String.h>
#include <mttr/Classifier_instance_row.h>
#include <omm/Property_view.h>
#include <omm/Logical_struct.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>
#include <qry/Util.h>





using namespace arolios;
using namespace arolios::mttr;

Classifier_instance_wlo_create::Classifier_instance_wlo_create( const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root,const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache)
:  Classifier_instance_create (p_data_root, p_import_cache) , csf_ (p_csf) {


}

Classifier_instance_wlo_create::~Classifier_instance_wlo_create() {

}

void Classifier_instance_wlo_create::execute()
{
    if (csf_->is_abstract()) {
       std::string error_msg = "classifier " + csf_->name() + " is abstract, the instance cannot be created";
         
        throw exception::Data_error ( error_msg );
    }
    
    auto lo = csf_->logical_struct();
    if (!lo) {
       std::string error_msg = "classifier " + csf_->name() + " cannot accept values";
         
        throw exception::Not_found ( error_msg );
    }
    
    if ( !lo->is_table() ) {
        std::string error_msg = "classifier " + csf_->name() + " is not mapped correctly to a relational object";
         

        throw exception::Not_found ( error_msg );
    }

    if (data_root_.isMember("properties")) {

      std::shared_ptr<appcfg::App> config = app_info_ptr_->app();

      std::shared_ptr<appcfg::Base_classifier_config> csf_config =
          config->find_config_by_name(csf_->package()->name(), csf_->name());

      std::vector<std::shared_ptr<const omm::Property_view>> prop_views;

      csf_config->properties_for_create(prop_views);

      std::shared_ptr<mttr::Classifier_instance_row> row =
          std::make_shared<mttr::Classifier_instance_row>(lo, csf_,
                                                          import_cache_);

      row->build_fields(data_root_["properties"], prop_views);

      auto &&table_name = row->table_name();

      std::string fields_stmt = "INSERT INTO " + table_name + " (csf_id, ";
      std::string values_stmt = "VALUES (" + csf_->id() + ", ";

      row->prepare_request(fields_stmt, values_stmt);

      fields_stmt.replace(fields_stmt.length() - 2, 2, ") ");
      values_stmt.replace(values_stmt.length() - 2, 2, ") ");

      std::string stmt = fields_stmt + values_stmt + " RETURNING id, csf_id";

 
      auto f = db_->execSqlAsyncFuture(stmt);

      auto result = f.get();

      if (result.size() > 0) {
        auto &&row = result[0];
        const std::string id_str = qry::Util::build_instance_id(row);
        //return_json_["id"] = Json::Value(id_str);
        return_json_["url"] =
            Json::Value(qry::Util::url_prefix("instance") + id_str);

        return_code_ = qry::Return_code::OK;
      } else {
        return_code_ = qry::Return_code::KO;
      }

    } else {
      std::string error_msg = "Creation without properties for classifier " +
                              csf_->name() + ", instance not created";
       
      throw exception::Data_error(error_msg);
    }
}


