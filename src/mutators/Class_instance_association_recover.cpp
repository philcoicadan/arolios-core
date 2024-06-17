/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Class_instance_association_recover.h>
#include <omm/Property.h>
#include <omm/Column.h>
#include <omm/Table.h>
#include <omm/Class.h>
#include <mttr/Classifier_instance_row.h>
#include <common/Db_client.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>





using namespace arolios;
using namespace arolios::mttr;

Class_instance_association_recover::Class_instance_association_recover( const std::shared_ptr<const omm::Association>& p_csf, const std::string& p_inst_id,  const int p_rfu_count)
: Classifier_instance_recover ( p_csf, p_inst_id, p_rfu_count) {

}

Class_instance_association_recover::~Class_instance_association_recover() {

}

void Class_instance_association_recover::execute() {
  auto assoc = std::dynamic_pointer_cast<const omm::Association>(csf_);
  int k = 0;
  auto members = assoc->member_ends();
  while (k < 2) {
    if ((*members)[k]->has_sub_field()) {
      break;
    }
    ++k;
  }

  auto opposite_prop = (*members)[1 - k];


  auto mtc = assoc->managing_table_class();


  std::shared_ptr<omm::Logical_struct> lo =
      (mtc) ? mtc->logical_struct() : nullptr;
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

  
  auto &&table_name = lo->full_name();
  auto column = opposite_prop->column();
  if (column == nullptr) {
    std::string error_msg = "member end " + opposite_prop->name() +
                            " of association " + csf_->name() +
                            " is not mapped correctly to a relational object";
    throw exception::Not_found(error_msg);
  }

  // check associated instances not in trash
  // check with table referenced by the column of the opposite prop
  auto target_table = opposite_prop->type()->table();
  if (target_table == nullptr) {
    const std::string msg = "Property " + opposite_prop->name() +
                            " of the classifier " + csf_->name() +
                            " is invalid. Recovering is not possible.";
     
    throw exception::Failure(msg);
  }
  std::string stmt1 = "SELECT " + target_table->full_name() + ".id FROM " +
                      target_table->full_name();
  
  if (table_name == target_table->full_name()) {
    stmt1 += " WHERE " + target_table->full_name() +
                      ".id = " + table_name + "." + column->name() + " AND ";
  }
  else {
    stmt1 += " INNER JOIN " + table_name +
                      " ON " + target_table->full_name() +
                      ".id = " + table_name + "." + column->name() + " WHERE ";
  }

  stmt1 +=  table_name + ".id=$1 AND " + target_table->full_name() +
                      ".trashed_date <> 'epoch' ";
  

  auto f1 = db_->execSqlAsyncFuture(stmt1, row_id_);

  auto result1 = f1.get();

  if (result1.size() > 0) {
    return_code_ = qry::Return_code::KO;
    std::cout << "WARNING: Class instance associated through " << inst_id_str_ << " is in the trash. Association recovering is not possible"<< std::endl;

  } else {
    // check class instance hosting the association instance in the same
    // table
    std::string stmt2 = "SELECT id FROM " + table_name +
                        "  WHERE id=$1 AND trashed_date <> 'epoch' ";
    
    auto f2 = db_->execSqlAsyncFuture(stmt2, row_id_);

    auto result2 = f2.get();
    if (result2.size() > 0) {
      return_code_ = qry::Return_code::KO;
      std::cout << "WARNING: Class instance associated through " << inst_id_str_ << " is in the trash. Association recovering is not possible"<< std::endl;
    } else {

      std::string column_name = column->name() + "_trashed_date";

      update_for_recover(table_name, column_name, row_id_, csf_id_list_);

      return_code_ = qry::Return_code::OK;
    }

  }
}
