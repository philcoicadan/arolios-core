/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <appcfg/Base_classifier_config.h>
#include <qry/Base_list_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <qry/Db_types.h>

namespace arolios {




namespace omm {
class Property;
} 



namespace qry {
    class Instance_filter;
class Base_classifier_instances_list : public Base_list_query {
public:
  /**
   * Constructor
   */
  Base_classifier_instances_list(
      const std::string &p_offset, const std::string &p_limit,
      const std::string &p_sort, const std::string &p_direction,
      const std::vector<std::shared_ptr<const qry::Instance_filter>>
          &p_filters,
      const std::string &p_prop_list,
      const std::shared_ptr<const omm::Language> &p_lang);

  /**
   * Destructor
   */
  virtual ~Base_classifier_instances_list();

  virtual void execute() override;

  virtual void
  build_return_json(const qry::Db_result &p_result) ;

  virtual void build_rows_for_return_json(
      const qry::Db_result &p_result,
      const std::unordered_map<
          std::string, std::shared_ptr<const omm::Property>> &p_props_map,
      const std::unordered_map<std::string, std::string> &p_prop_column_map,
      const std::unordered_map<std::string, std::string>
          &p_prop_column_alias_map) ;
protected:





    virtual const std::shared_ptr<const appcfg::Base_classifier_config>& config() const ;
    virtual const std::shared_ptr<const omm::Classifier> classifier () const = 0;

      virtual void
  build_ids_json(Json::Value &p_jrow, const qry::Db_row &p_row,
                 const std::pair<std::string, std::string> &p_id_pair,
                 const std::pair<std::string, std::string> &p_csf_id_pair);

    
    virtual void set_config () ;
     virtual std::string compute_order_by () override;

     virtual Json::Value & compute_json_key(
    Json::Value &p_jrow, std::vector<std::string> &p_vec, std::string &p_jkey) const;
     bool enum_val_sort_;


private:
  virtual void build_json_from_row(
      Json::Value &p_jrow, const qry::Db_row &p_row,
      const std::unordered_map<
          std::string, std::shared_ptr<const omm::Property>> &p_props_map,
      const std::unordered_map<std::string, std::string> &p_prop_column_map,
      const std::unordered_map<std::string, std::string> &p_column_alias_map) = 0;






const std::vector<std::shared_ptr<const qry::Instance_filter>> filters_;
const std::string prop_list_;


    virtual void
  build_main_ids_json(Json::Value &p_jrow, const qry::Db_row &p_row,
                 const std::pair<std::string, std::string> &p_id_pair,
                 const std::pair<std::string, std::string> &p_csf_id_pair) = 0;

    std::shared_ptr<const appcfg::Base_classifier_config> csf_config_;
};

} // namespace qry
}

