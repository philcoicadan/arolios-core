/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_wlo_update.h>
#include <omm/Classifier.h>
#include <util/String.h>
#include <mttr/Classifier_instance_row.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>






using namespace arolios;
using namespace arolios::mttr ;

Classifier_instance_wlo_update::Classifier_instance_wlo_update( const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root, const std::string& p_inst_id, const int p_rfu_count, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache) 
: Classifier_instance_update (p_data_root, p_inst_id, p_rfu_count, p_import_cache), csf_ (p_csf) {

    std::tie(csf_id_,row_id_)= qry::Util::split_instance_id(p_inst_id);
    
}

Classifier_instance_wlo_update::~Classifier_instance_wlo_update() {

}

void Classifier_instance_wlo_update::execute () {
    
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

      csf_config->properties_for_update(prop_views);

      std::shared_ptr<mttr::Classifier_instance_row> row =
          std::make_shared<mttr::Classifier_instance_row>(lo, csf_,
                                                          import_cache_);

      row->build_fields(data_root_["properties"], prop_views);

      auto &&table_name = row->table_name();

      std::string fields_stmt = "UPDATE " + table_name + " SET ( upd_count, ";
      std::string values_stmt = "= ROW ( upd_count+1, ";

      row->prepare_request(fields_stmt, values_stmt);

      fields_stmt.replace(fields_stmt.length() - 2, 2, ") ");
      values_stmt.replace(values_stmt.length() - 2, 2, ") ");

      // auto mtc = csf_->managing_table_class();

      const std::string csf_id_list =
          csf_->classifier_id_with_specializations();

      std::string stmt =
          fields_stmt + values_stmt +
          " WHERE trashed_date = 'epoch' AND id=" + std::to_string(row_id_) +
          " AND  ( " + csf_id_list + " )";

      if (rfu_count_ > 0) {
        stmt += " AND upd_count=" + std::to_string(rfu_count_);
      }
      stmt += " RETURNING upd_count";


      auto f = db_->execSqlAsyncFuture(stmt);

      auto result = f.get();

      if (result.size() > 0) {
        auto &&row = result[0];
        new_upd_count_ = row["upd_count"].as<int>();
        ;
        return_code_ = qry::Return_code::OK;
      } else {
        return_code_ = qry::Return_code::KO;
      }

    } else {
      std::string error_msg = "Update without properties for classifier " +
                              csf_->name() + ", instance not updated";
       
      throw exception::Data_error(error_msg);
    }
}
