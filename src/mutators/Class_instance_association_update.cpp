/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Class_instance_association_update.h>
#include <mttr/Base_element_update.h>
#include <omm/Property.h>
#include <omm/Logical_field.h>
#include <mttr/Classifier_instance_row.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>
#include <util/String.h>



using namespace arolios;
using namespace arolios::mttr;

Class_instance_association_update::Class_instance_association_update( const std::shared_ptr<const omm::Association>& p_assoc, const Json::Value& p_data_root, const std::string& p_inst_id, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache, const int p_rfu_count)
: Base_element_update(p_inst_id, p_data_root, p_rfu_count), assoc_ (p_assoc), import_cache_(p_import_cache){



}

Class_instance_association_update::~Class_instance_association_update() {

}

void Class_instance_association_update::execute()
{
 
    
    auto mtc = assoc_->managing_table_class();
    
    const std::string csf_id_list = assoc_->classifier_id_with_specializations();
    const auto [ csf_id, row_id] = qry::Util::split_instance_id(elem_id_);
        
    std::shared_ptr<omm::Logical_struct> lo = (mtc) ? mtc->logical_struct() : nullptr;
    if (!lo) {
       std::string error_msg = "classifier " + assoc_->name() + " cannot accept values";
        throw exception::Not_found ( error_msg );
    }
    
    if ( !lo->is_table() ) {
        std::string error_msg = "classifier " + assoc_->name() + " is not mapped correctly to a relational object";
        throw exception::Not_found ( error_msg );
    }

    if (data_root_.isMember("properties")) {

      std::shared_ptr<appcfg::App> config = app_info_ptr_->app();

      std::shared_ptr<appcfg::Base_classifier_config> csf_config =
          config->find_config_by_name(assoc_->package()->name(),
                                      assoc_->name());

      std::vector<std::shared_ptr<const omm::Property_view>> prop_views;

      csf_config->properties_for_update(prop_views);

      std::shared_ptr<mttr::Classifier_instance_row> row =
          std::make_shared<mttr::Classifier_instance_row>(lo, assoc_,
                                                          import_cache_);

      row->build_fields(data_root_["properties"], prop_views);

      auto &&table_name = row->table_name();

      std::string fields_stmt = "UPDATE " + table_name + " SET ( upd_count, ";
      std::string values_stmt = "= ROW ( upd_count+1, ";

      row->prepare_request(fields_stmt, values_stmt);

      fields_stmt.replace(fields_stmt.length() - 2, 2, ") ");
      values_stmt.replace(values_stmt.length() - 2, 2, ") ");

      std::string stmt =
          fields_stmt + values_stmt +
          " WHERE trashed_date = 'epoch' AND id=" + std::to_string(row_id) +
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
                              assoc_->name() + ", instance not updated";
       
      throw exception::Data_error(error_msg);
    }
}