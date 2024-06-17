/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Class_instance_association_create.h>
#include <omm/Property.h>
#include <omm/Class.h>
#include <omm/Table.h>
#include <mttr/Association_instance_row.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <qry/Util.h>





using namespace arolios;
using namespace arolios::mttr;

Class_instance_association_create::Class_instance_association_create(const std::shared_ptr<const omm::Association>& p_assoc, const Json::Value& p_data_root, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache)
:  Base_element_create (p_data_root), assoc_ (p_assoc),  import_cache_ (p_import_cache) {
upd_row_id_ = 0;

}

Class_instance_association_create::~Class_instance_association_create() {

}

void Class_instance_association_create::execute()
{

    auto upd_csf = assoc_->managing_table_class(); 
    std::shared_ptr<omm::Table> lo = (upd_csf) ? upd_csf->table(): nullptr;
    if (!(lo)) {
        std::string error_msg = "classifier " + assoc_->name() + " is not mapped correctly to a relational object";
         
        throw exception::Not_found ( error_msg );
    }

    if (data_root_.isMember("properties")) {

      std::shared_ptr<appcfg::App> config = app_info_ptr_->app();

      std::shared_ptr<appcfg::Base_classifier_config> csf_config =
          config->find_config_by_name(assoc_->package()->name(),
                                      assoc_->name());

      std::vector<std::shared_ptr<const omm::Property_view>> prop_views;

      csf_config->properties_for_create(prop_views);

      std::shared_ptr<mttr::Association_instance_row> row =
          std::make_shared<mttr::Association_instance_row>(lo, assoc_,
                                                           import_cache_);

      row->build_fields(data_root_["properties"], prop_views);
      upd_row_id_ = row->class_instance_row_id();

      auto ref_vec = row->referencing_fields();

      if (upd_row_id_ == 0 || ref_vec.empty()) {
        std::string error_msg = "classifier " + upd_csf->name() +
                                " does not enable to create association " +
                                assoc_->name();
         
        throw exception::Not_found(error_msg);
      }

      auto &&ref_column = ref_vec[0]->column_full_name();
      auto &&table_name = row->table_name();

      std::string fields_stmt = "UPDATE " + table_name + " SET ( upd_count, ";
      std::string values_stmt = "= ROW ( upd_count+1, ";

      row->prepare_request(fields_stmt, values_stmt);

      fields_stmt.replace(fields_stmt.length() - 2, 2, ") ");
      values_stmt.replace(values_stmt.length() - 2, 2, ") ");

      std::string stmt =
          fields_stmt + values_stmt + " WHERE trashed_date = 'epoch' AND id=" +
          std::to_string(upd_row_id_) + " AND " + ref_column + " IS NULL";

      stmt += " RETURNING upd_count";


      auto f = db_->execSqlAsyncFuture(stmt);

      auto result = f.get();

      if (result.size() > 0) {
        auto &&row = result[0];
        new_upd_count_ = row["upd_count"].as<int>();
        const std::string id_str = qry::Util::build_instance_id(
            std::stoul(assoc_->id()), upd_row_id_);
        return_json_["id"] = Json::Value(id_str);
        return_json_["url"] =
            Json::Value(qry::Util::url_prefix("instance") + id_str);

        return_code_ = qry::Return_code::OK;
      } else {
        return_code_ = qry::Return_code::KO;
      }

    } else {
      std::string error_msg = "Creation without properties for classifier " +
                              assoc_->name() + ", instance not created";
       
      throw exception::Data_error(error_msg);
    }
}
