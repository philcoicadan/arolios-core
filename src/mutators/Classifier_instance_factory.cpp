/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_factory.h>

#include <mttr/Classifier_instance_wlo_update.h>
#include <mttr/Association_instance_wolo_update.h>
#include <mttr/Classifier_instance_wlo_create.h>
#include <mttr/Association_instance_wolo_create.h>
#include <mttr/Association_instance_wolo_delete.h>
#include <mttr/Association_instance_wolo_recover.h>
#include <mttr/Association_instance_wolo_eliminate.h>


#include <omm/Association.h>
#include <memory>





using namespace arolios;
using namespace arolios::mttr;

Classifier_instance_factory::Classifier_instance_factory()
{

}



Classifier_instance_factory::~Classifier_instance_factory()
{

}

Classifier_instance_factory& Classifier_instance_factory::instance() {
    static Classifier_instance_factory inst = Classifier_instance_factory();
    return inst;
}

std::shared_ptr<mttr::Classifier_instance_create> Classifier_instance_factory::make_create(const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache) {

  std::shared_ptr<mttr::Classifier_instance_create> result;
  std::shared_ptr<omm::Table> elem_lo =
      (p_csf == nullptr) ? nullptr : p_csf->table();

  if (elem_lo) {

    result = std::make_shared<Classifier_instance_wlo_create>(p_csf,p_data_root, p_import_cache);
    
  } else {
    std::shared_ptr<const omm::Association> assoc = std::dynamic_pointer_cast<const omm::Association>(p_csf);
    if (assoc) {
      result = std::make_shared<Association_instance_wolo_create>(
          assoc, p_data_root, p_import_cache);
    }
  }

  return result;


}
std::shared_ptr<mttr::Classifier_instance_update> Classifier_instance_factory::make_update(const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root, const std::string& p_inst_id, const int p_rfu_count, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache) {

  std::shared_ptr<mttr::Classifier_instance_update> result;
  std::shared_ptr<omm::Table> elem_lo =
      (p_csf == nullptr) ? nullptr : p_csf->table();

  if (elem_lo) {

    result = std::make_shared<Classifier_instance_wlo_update>(p_csf,p_data_root, p_inst_id, p_rfu_count, p_import_cache);
    
  } else {
    std::shared_ptr<const omm::Association> assoc = std::dynamic_pointer_cast<const omm::Association>(p_csf);
    if (assoc) {
      result = std::make_shared<Association_instance_wolo_update>(
          assoc, p_data_root, p_inst_id, p_rfu_count, p_import_cache);
    }
  }

  return result;

}    
   
std::shared_ptr<mttr::Classifier_instance_delete> Classifier_instance_factory::make_delete(const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count) {

  std::shared_ptr<mttr::Classifier_instance_delete> result;
  std::shared_ptr<omm::Table> elem_lo =
      (p_csf == nullptr) ? nullptr : p_csf->table();

  if (elem_lo) {

    result = std::make_shared<Classifier_instance_delete>(p_csf, p_inst_id, p_rfu_count);
    
  } else {
    std::shared_ptr<const omm::Association> assoc = std::dynamic_pointer_cast<const omm::Association>(p_csf);
    if (assoc) {
      result =  std::make_shared<Association_instance_wolo_delete> (
              assoc, p_inst_id, p_rfu_count);
    }
  }

  return result;
}

std::shared_ptr<mttr::Classifier_instance_recover> Classifier_instance_factory::make_recover(const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count) {

  std::shared_ptr<mttr::Classifier_instance_recover> result;
  std::shared_ptr<omm::Table> elem_lo =
      (p_csf == nullptr) ? nullptr : p_csf->table();

  if (elem_lo) {

    result = std::make_shared<Classifier_instance_recover>(p_csf, p_inst_id, p_rfu_count);
    
  } else {
    std::shared_ptr<const omm::Association> assoc = std::dynamic_pointer_cast<const omm::Association>(p_csf);
    if (assoc) {
      result = std::make_shared<Association_instance_wolo_recover>(
          assoc, p_inst_id, p_rfu_count);
    }
  }

  return result;
}

std::shared_ptr<mttr::Classifier_instance_eliminate> Classifier_instance_factory::make_eliminate(const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count) {

  std::shared_ptr<mttr::Classifier_instance_eliminate> result;
  std::shared_ptr<omm::Table> elem_lo =
      (p_csf == nullptr) ? nullptr : p_csf->table();

  if (elem_lo) {

    result = std::make_shared<Classifier_instance_eliminate>(p_csf, p_inst_id, p_rfu_count);
    
  } else {
    std::shared_ptr<const omm::Association> assoc = std::dynamic_pointer_cast<const omm::Association>(p_csf);
    if (assoc) {
      result = std::make_shared<Association_instance_wolo_eliminate>(
          assoc, p_inst_id, p_rfu_count);
    }
  }

  return result;
}

