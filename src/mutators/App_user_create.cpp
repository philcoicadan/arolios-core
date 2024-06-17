/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/App_user_create.h>
#include <mttr/Base_element_create.h>
#include <qry/App_retriever.h>
#include <util/String.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <appcfg/App.h>
#include <util/String.h>
#include <common/User.h>




using namespace arolios;
using namespace arolios::mttr;

App_user_create::App_user_create(const Json::Value& p_data_root) :
Base_element_create(p_data_root){

}

App_user_create::~App_user_create() {

}

void App_user_create::execute() {

  auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
  auto uac_info = common::Singleton<common::UAC_info>::instance().object();


  const std::string user =
      util::String::standardize(data_root_["user"].asString());
  if (user.empty()) {
    std::string error_msg = "Error in the App user creation: user is missing";
     
    throw exception::Model_error(error_msg);
  }

  const std::string user_id = retrieve_user_internal_id(user);

  if (user_id == uac_info->super_user()->id()) {
    std::string error_msg =
        "The super user is already associated to the application";
     
    throw exception::Model_error(error_msg);
  }

  const std::string app =
      util::String::standardize(data_root_["app"].asString());
  if (app.empty()) {
    std::string error_msg = "Error in the App user creation: app is missing";
     
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
            "Error in the App user creation: default app is not valid";
         
        throw exception::Model_error(error_msg);
      }

      std::string au_role = data_root_["role"].asString();
      au_role = uac_info->standardize_role(au_role);
      if (au_role.empty()) {
        std::string error_msg =
            "Error in the App user creation: user role not found";
         
        throw exception::Model_error(error_msg);
      }

      {
        const std::string stmt = "INSERT INTO am_app_user (au_user , au_app, "
                                 "au_role) VALUES ( $1, $2, $3) RETURNING id";

        

        auto f = db_->execSqlAsyncFuture(stmt, util::String::stoui(user_id),
                                         id, au_role);

        auto result = f.get();

        if (result.size() > 0) {
          auto &&row = result[0];
          const std::string id_str =
              std::to_string(row["id"].as<unsigned int>());
          return_json_["url"] =
              Json::Value(qry::Util::url_prefix("appuser") + id_str);

          return_code_ = qry::Return_code::OK;
        } else {
          return_code_ = qry::Return_code::KO;
        }
      }
    } else {
      std::string error_msg =
          "Error in the App user creation: application " + app + " not found";
       
      throw exception::Not_found(error_msg);
    }
  }

  catch (const exception::Db_exception &e) {
    std::cerr << "error: " << e.base().what() << std::endl;
    throw e;
  }
}

std::string App_user_create::retrieve_user_internal_id (const std::string& p_name) const
{
        const std::string name = util::String::standardize(p_name);
        auto f = db_->execSqlAsyncFuture("SELECT id FROM am_user WHERE u_identifier = $1", name);

    try {
        auto result = f.get();

        if (result.size() > 0) {

            return std::to_string(result[0]["id"].as<unsigned int>());
        }
        else {
            std::string msg ("user identifier " + p_name + " not found");
             
            throw exception::Sql_error (msg);

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "ERROR: " << e.base().what() << std::endl;
       throw e;
	}


}
