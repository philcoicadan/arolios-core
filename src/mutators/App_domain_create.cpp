/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/App_domain_create.h>
#include <mttr/Base_element_create.h>
#include <util/String.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <appcfg/App.h>
#include <util/String.h>





using namespace arolios;
using namespace arolios::mttr;

App_domain_create::App_domain_create(const Json::Value& p_data_root) :
Base_element_create(p_data_root){

}

App_domain_create::~App_domain_create() {

}

void App_domain_create::execute() {

  auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();


  const std::string domain =
      util::String::standardize(data_root_["domain"].asString());
  if (domain.empty()) {
    std::string error_msg =
        "Error in the App domain creation: domain is missing";
     
    throw exception::Model_error(error_msg);
  }

  if (domain ==  mdl_info->standard_domain()->name()) {
    std::string error_msg =
        "Error in the App domain creation: standard domain is not valid";
     
    throw exception::Model_error(error_msg);
  }

  auto domain_ptr =  mdl_info->find_domain_by_name(domain);

  if (domain_ptr == nullptr) {
    std::string error_msg =
        "Error in the App domain creation: domain " + domain + " not found";
     
    throw exception::Not_found(error_msg);
  }

  const std::string app =
      util::String::standardize(data_root_["app"].asString());
  if (app.empty()) {
    std::string error_msg = "Error in the App domain creation: app is missing";
     
    throw exception::Model_error(error_msg);
  }

  auto f = db_->execSqlAsyncFuture("SELECT * FROM am_app WHERE name = $1", app);

  try {
    auto result = f.get();

    if (result.size() > 0) {
      auto row = result[0];
      auto id = row["id"].as<unsigned int>();

      if (id == util::String::stoui(app_info_ptr_->default_app()->id())) {
        std::string error_msg =
            "Error in the App domain creation: default app is not valid";
         
        throw exception::Model_error(error_msg);
      }
      {
        const std::string stmt = "INSERT INTO am_app_domain (a_app, a_domain) "
                                 "VALUES ( $1, $2) RETURNING id";

        

        auto f = db_->execSqlAsyncFuture(
            stmt, id, util::String::stoui(domain_ptr->id()));

        auto result = f.get();

        if (result.size() > 0) {
          auto &&row = result[0];
          const std::string id_str =
              std::to_string(row["id"].as<unsigned int>());
          //return_json_["id"] = Json::Value(id_str);
          return_json_["url"] =
              Json::Value(qry::Util::url_prefix("appdomain") + id_str);

          return_code_ = qry::Return_code::OK;
        } else {
          return_code_ = qry::Return_code::KO;
        }
      }
    } else {
      std::string error_msg =
          "Error in the App domain creation: application " + app + " not found";
       
      throw exception::Not_found(error_msg);
    }
  }

  catch (const exception::Db_exception &e) {
    std::cerr << "error: " << e.base().what() << std::endl;
    throw e;
  }
}
