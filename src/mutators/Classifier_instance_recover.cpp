/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_recover.h>
#include <omm/Association.h>
#include <omm/Column.h>
#include <omm/Property.h>
#include <omm/Table.h>
#include <qry/Util.h>
#include <util/String.h>
#include <memory>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <qry/Class_instance_read.h>
#include <exception/Exceptions.h>
#include <qry/Return_code.h>





using namespace arolios;
using namespace arolios::mttr;

Classifier_instance_recover::Classifier_instance_recover(
    const std::shared_ptr<const omm::Classifier> &p_csf,
    const std::string &p_inst_id, const int p_rfu_count)
    : app_info_ptr_(common::Singleton<common:: App_info>::instance().object()), csf_(p_csf), inst_id_str_(p_inst_id) {

  rfu_count_ = p_rfu_count;

  csf_id_list_ = csf_->classifier_id_with_specializations();
  std::tie(csf_id_, row_id_) = qry::Util::split_instance_id(p_inst_id);
}

Classifier_instance_recover::~Classifier_instance_recover() {}

int Classifier_instance_recover::new_update_counter() const {
  return new_upd_count_;
}

void Classifier_instance_recover::execute() {

  // TODO improvement: only ONE transaction is needed for avoiding concurrent
  // access (multi read for checking then update)

  auto lo = csf_->logical_struct();
  if (!lo) {
    std::string error_msg =
        "classifier " + csf_->name() + " cannot accept values";
    throw exception::Not_found(error_msg);
  }

  if (!lo->is_table()) {
    std::string error_msg = "classifier " + csf_->name() +
                            " is not mapped correctly to a relational object";
    throw exception::Not_found(error_msg);
  }

  bool has_assoc = false;

  const auto om_ptr =
      common::Singleton<common::Object_model_info>::instance().object();
  auto cls = std::dynamic_pointer_cast<const omm::Class>(csf_);

  if (cls) { // instance is a class instance

    const auto &props_map = csf_->all_elements_as_property();
    Json::Value props_json = Json::Value(Json::objectValue);

    return_code_ = qry::Return_code::OK;

    for (auto [name, prop] : props_map) {
      const auto assoc = prop->association();
      if (assoc) {
        if (!has_assoc) {
          // Need to retrieve all fields of the instance for processing its
          // associations
          // add filter to select only deleted instance
          const std::shared_ptr<const qry::Instance_filter> td_filter =
              std::make_shared<qry::Instance_filter>("trashed_date", "<>",
                                                      "'epoch'");
          const std::vector<std::shared_ptr<const qry::Instance_filter>>
              filters = {td_filter};
          auto read_inst = qry::Class_instance_read(
              cls, inst_id_str_,
              qry::Util::Read_mode::read_for_update, filters,
              app_info_ptr_->default_language());

          read_inst.execute();
          if (read_inst.return_code() == qry::Return_code::OK) {
            props_json = read_inst.return_json();
          } else {
            const std::string msg =
                "update for restoring associations linked to inst" +
                inst_id_str_ + " of classifier " + csf_->name() + " failed";
            std::cout << "WARNING: " << msg<< std::endl;
            break;
          }
          has_assoc = true;
        }
        if (prop->max_occurs() == 1) { // cardinality 0,1 or 1,1

          if (prop->min_occurs() ==
              1) { // cardinality = 1,1. target does not have to be in trash
            if (prop->column()) {
              // prop associated to a table column
              bool ret = check_target_instance_in_trash(prop, props_json);
              if (ret) {
                return_code_ = qry::Return_code::KO;
                break;
              }

            } else {
              const std::string msg =
                  "Property " + prop->name() + " of classifier " +
                  csf_->name() +
                  " is invalid. Recovering of the instance is not possible";
               
              throw exception::Failure(msg);
            }
          } else { // cardinality is 0,1 . If opposite is a composition, the
                   // target instance does not have to be in the trash

            const auto members = assoc->member_ends();
            std::shared_ptr<omm::Property> opposite;
            for (int i = 0; i < 2; ++i) {
              opposite = (*members)[i];
              if (opposite != prop)
                break;
            }

            if (opposite->aggregation_kind() ==
                omm::Aggregation_kind::composite) {
              if (prop->column()) {
                // prop associated to a table column
                bool ret = check_target_instance_in_trash(prop, props_json);
                if (ret) {
                  return_code_ = qry::Return_code::KO;
                  break;
                }

              } else {
                const std::string msg =
                    "Property " + prop->name() + " of classifier " +
                    csf_->name() +
                    " is invalid. Recovering of the instance is not possible";
                 
                throw exception::Failure(msg);
              }
            }
          }


        } else { // cardinality 0,N or 1,N
                 // nothing to recover: 1,N managed as 0,N
        }
      }
    }
  } else {
    // classifier is an association, the associated instances doesn't have to be
    // in the trash
    auto assoc = std::dynamic_pointer_cast<const omm::Association>(csf_);
    if (assoc) {
      return_code_ = (check_associated_instances_in_trash(assoc))
                         ? qry::Return_code::KO
                         : qry::Return_code::OK;

    } else {
      const std::string msg = "Bad type for the classifier " + csf_->name() +
                              ". Recovering is not possible.";
       
      throw exception::Failure(msg);
    }
  }

  if (return_code_ == qry::Return_code::OK) {

    const bool ret = recover_instance(); // update instance for trash
    if (!ret) {
      const std::string msg = "update  for restoring instance " + inst_id_str_ +
                              " of classifier " + csf_->name() + " failed";
      std::cout << "WARNING: " << msg<< std::endl;
      return_code_ = qry::Return_code::KO;
    }
  }
}



bool Classifier_instance_recover::recover_instance() {

  bool ret_status = false;
  auto lo = csf_->logical_struct();
  auto &&table_name = lo->full_name();

  const std::string fields_stmt =
      "UPDATE " + table_name + " SET ( upd_count, trashed_date ) ";
  const std::string values_stmt = "= ROW ( upd_count+1, 'epoch') ";

  std::string stmt =
      fields_stmt + values_stmt +
      " WHERE trashed_date <> 'epoch' AND id=" + std::to_string(row_id_) +
      " AND  ( " + csf_id_list_ + " )";


  stmt += " RETURNING upd_count";


  auto f = db_->execSqlAsyncFuture(stmt);

  auto result = f.get();

  if (result.size() > 0) {
    auto &&row = result[0];
    new_upd_count_ = row["upd_count"].as<int>();
    ;
    return_code_ = qry::Return_code::OK;
    ret_status = true;
  }

  return ret_status;
}

bool Classifier_instance_recover::update_for_recover(
    const std::string &p_table_name, const std::string &p_field_name,
    const unsigned long p_row_id, const std::string &p_csf_id_list) {

  bool ret_status = false;
  const std::string fields_stmt =
      "UPDATE " + p_table_name + " SET ( " + p_field_name + " ) ";
  const std::string values_stmt = "= ROW ( 'epoch') ";

  std::string stmt = fields_stmt + values_stmt + " WHERE " + p_field_name +
                     " <> 'epoch' AND id=" + std::to_string(p_row_id);
  if (!p_csf_id_list.empty()) {
    stmt += " AND ( " + p_csf_id_list + " ) ";
  }
  stmt += " RETURNING id";


  auto f = db_->execSqlAsyncFuture(stmt);

  auto result = f.get();
  if (!(result.size() > 0)) {
    return_code_ = qry::Return_code::KO;
    std::cout << "WARNING: update " << p_table_name << "." << p_field_name << " failed"<< std::endl;
    ret_status = false;
  } else {
    ret_status = true;
  }

  return ret_status;
}

bool Classifier_instance_recover::check_associated_instances_in_trash(
    const std::shared_ptr<const omm::Association> &p_assoc) const {
  auto ends = p_assoc->member_ends();
  auto table = p_assoc->table();
  if (table == nullptr) {
    const std::string msg = "Data of the classifier " + csf_->name() +
                            " are invalid. Recovering is not possible.";
     
    throw exception::Failure(msg);
  }
  std::shared_ptr<omm::Table> target_tables[2];
  std::shared_ptr<omm::Column> columns[2];
  for (int k = 0; k < 2; ++k) {
    auto prop = (*ends)[k];
    columns[k] = prop->column();
    target_tables[k] = prop->type()->table();
    if (columns[k] == nullptr || target_tables[k] == nullptr) {
      const std::string msg = "Property " + prop->name() +
                              " of the classifier " + csf_->name() +
                              " is invalid. Recovering is not possible.";
       
      throw exception::Failure(msg);
    }
  }
  bool associated_in_trash = false;
  for (int k = 0; k < 2 && !associated_in_trash; ++k) {
    std::string stmt =
        "SELECT " + target_tables[k]->full_name() + ".id FROM " + target_tables[k]->full_name() + " INNER JOIN " +
        table->full_name() + " ON " + target_tables[k]->full_name() +
        ".id = " + table->full_name() + "." + columns[k]->name() +
        " WHERE " + table->full_name() + ".id=$1 AND " +
        target_tables[k]->full_name() + ".trashed_date <> 'epoch' ";
      
    

    auto f = db_->execSqlAsyncFuture(stmt, row_id_);

    auto result = f.get();

    if (result.size() > 0) {
      associated_in_trash = true;
      std::cout << "WARNING: Class instance associated through " << inst_id_str_ << " is in the trash. Association recovering is not possible"<< std::endl;

    }
  }
  return associated_in_trash;
}

bool Classifier_instance_recover::check_target_instance_in_trash(
    const std::shared_ptr<const omm::Property> &p_prop,
    const Json::Value &p_props_json) const {

  bool target_in_trash = false;

  const auto table = p_prop->type()->table();
  if (table == nullptr) {
    const std::string msg = "Data of the classifier " + p_prop->type()->name() +
                            " are invalid. Recovering is not possible.";
     
    throw exception::Failure(msg);
  }

  if (p_props_json.isMember("properties")) {
    const Json::Value &properties_json = p_props_json["properties"];
    if (properties_json.isMember(p_prop->name())) {

      const std::string target_iid = qry::Util::erase_url_prefix(
          properties_json[p_prop->name()]["url"].asString());

      auto [csf_id, row_id] = qry::Util::split_instance_id(target_iid);

      std::string stmt = "SELECT id FROM " + table->full_name() +
                         "  WHERE id=$1 AND trashed_date <> 'epoch' ";

      
      auto f = db_->execSqlAsyncFuture(stmt, row_id);

      auto result = f.get();
      if (result.size() > 0) {
        target_in_trash = true;
        std::cout << "WARNING: Associated class instance " << target_iid
                 << " is in the trash. Recovering is not possible"<< std::endl;
      }
    }
  }
  return target_in_trash;
}