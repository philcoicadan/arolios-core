/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <appcfg/Base_classifier_config.h>
#include <qry/Base_classifier_instances_list.h>
#include <qry/Base_list_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {



namespace omm {
class Property;
} 

namespace util {
class Instance_filter;
}

namespace qry {
template <typename T> class Classifier_instances_list : public Base_classifier_instances_list {
public:
  /**
   * Constructor
   */
  Classifier_instances_list(
      const std::shared_ptr<const T> &p_csf,
      const std::string &p_offset, const std::string &p_limit,
      const std::string &p_sort, const std::string &p_direction,
      const std::vector<std::shared_ptr<const qry::Instance_filter>>
          &p_filters,
      const std::string &p_prop_list,
      const std::shared_ptr<const omm::Language> &p_lang);

  /**
   * Destructor
   */
  virtual ~Classifier_instances_list();

  
protected:
//  virtual std::string compute_order_by () override;

    const std::shared_ptr<const T> csf_;
    virtual const std::shared_ptr<const omm::Classifier> classifier () const override;


private:


  virtual void build_json_from_row(
      Json::Value &p_jrow, const qry::Db_row &p_row,
      const std::unordered_map<
          std::string, std::shared_ptr<const omm::Property>> &p_props_map,
      const std::unordered_map<std::string, std::string> &p_prop_column_map,
      const std::unordered_map<std::string, std::string> &p_column_alias_map) override;

  virtual void
  build_main_ids_json(Json::Value &p_jrow, const qry::Db_row &p_row,
                 const std::pair<std::string, std::string> &p_id_pair,
                 const std::pair<std::string, std::string> &p_csf_id_pair) override;

 
};

} // namespace qry
}

