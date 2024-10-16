/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Data_import.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <fstream>
#include <mttr/Classifier_instance_factory.h>
#include <mttr/Classifier_instance_create.h>
#include <json/value.h>
#include <mttr/Classifier_instance_update.h>
#include <util/String.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <qry/Return_code.h>




using namespace arolios;
using namespace arolios::mttr;

Data_import::Data_import( const std::string& p_filename)
: app_info_ptr_ (common::Singleton<common:: App_info>::instance().object()), filename_ (p_filename) {

}

Data_import::~Data_import() {

}


void Data_import::execute () {
    
    
    
    
   

        
        std::ifstream ifs;
        ifs.open ( filename_ );
        if ( ifs.is_open() ) {
            Json::Value root;

            Json::CharReaderBuilder builder;
            builder["collectComments"] = true;
            JSONCPP_STRING errs;
            if ( !parseFromStream ( builder, ifs,&root, &errs ) ) {
                 
                throw exception::Data_error ( errs );
            }
    
            if (root.isMember("import")) {
                const Json::Value& jimport = root["import"];
                if ( jimport.isArray()) {
                    return_code_ = qry::Return_code::OK; // OK by default
                    for ( int i=0; i < jimport.size(); ++i) {
                        
                        try {
                            execute_statement (jimport[i]) ;
                            
                        } catch ( const std::exception& e) {
                             
                            return_code_ = qry::Return_code::KO;
                        }
                    }
                } else {
                    return_code_ = qry::Return_code::KO;
                    const std::string error ("The JSON import file does not contain a sequence of statements");
                    
                    throw exception::Data_error (error);
                }
            } else {
                return_code_ = qry::Return_code::KO;
                const std::string error ("The JSON import file does not contain the import key");
                 
                throw exception::Data_error (error);
            }
            
        } else {
            return_code_ = qry::Return_code::KO;
            const std::string error ("Import file not found");
             
            throw exception::Not_found (error);
        }

    
}

void Data_import::execute_statement(const Json::Value &p_stmt) {

  auto jcsf = p_stmt["classifier"];
  std::string csf_full_name;
  Json::Value jcls;
  Json::Value jassoc;
  if (jcsf.isNull()) {
    jcls = p_stmt["class"];
    if (jcls.isNull()) {
      jassoc = p_stmt["association"];
      csf_full_name = jassoc.asString();
    } else {
      csf_full_name = jcls.asString();
    }
  } else {
    csf_full_name = jcsf.asString();
  }

  if (jcsf.isNull() && jcls.isNull() && jassoc.isNull()) {
    const std::string error(
        "classifier key not found in the import statement: ");

    throw exception::Data_error(error);
  }

  auto action = p_stmt.get("action", "create").asString();

  std::transform(
      action.begin(), action.end(), action.begin(),
      [](unsigned char c) -> unsigned char { return std::tolower(c); });
  if (action != "create" && action != "update") {
    const std::string error("action value unknown in the import statement: ");

    throw exception::Data_error(error);
  }

  std::string inst_str;
  const auto jinstance = p_stmt["instance"];
  if (jinstance.isNull() && action == "update") {
    const std::string error("instance key not found in the import statement: ");

    throw exception::Data_error(error);
  } else if (!jinstance.isNull()) {
    if (action ==
        "update") { // instance can be a string composed of 2 integers or a name
                    // prefixed by a $ and contained in the import cache

      inst_str = jinstance.asString();
      if (inst_str[0] != '$') {

        // check the format of inst_str
        const auto [csf_id, row_id] = qry::Util::split_instance_id(inst_str);

      } else {
        // instance is not an unsigned long . Check if string starts with $ and
        // present in the import cache

        inst_str.erase(0, 1);
        inst_str = util::String::standardize(inst_str);
        std::transform(
            inst_str.begin(), inst_str.end(), inst_str.begin(),
            [](unsigned char c) -> unsigned char { return std::tolower(c); });

        auto search = cache_.find(inst_str);
        if (search != cache_.end()) {
          const Json::Value &jinst = *(search->second);
          inst_str = jinst.asString();
        } else {
          const std::string error(
              "instance value not found in the import statement: ");

          throw exception::Data_error(error);
        }
      }
    } else { // action = create, instance must be a name
      inst_str = jinstance.asString();
      if (inst_str[0] == '$') {
        inst_str.erase(0, 1);
        inst_str = util::String::standardize(inst_str);
        std::transform(
            inst_str.begin(), inst_str.end(), inst_str.begin(),
            [](unsigned char c) -> unsigned char { return std::tolower(c); });
      } else {
        const std::string error(
            "instance value is not valid in the import statement: ");

        throw exception::Data_error(error);
      }
    }
  }

  std::shared_ptr<omm::Classifier> csf =
      app_info_ptr_->find_domain_element_by_full_name<omm::Classifier>(
          csf_full_name);

  if (!csf->is_abstract()) {
    if (action == "create") {
      auto ic = mttr::Classifier_instance_factory::instance().make_create(
          csf, p_stmt, cache_);
      if (ic) {
        ic->execute();

        if (ic->return_code() == qry::Return_code::OK) {

          register_instance(ic->return_json(), inst_str);

        } else {
          const std::string error("Classifier instance creation failed ");

          throw exception::Data_error(error);
        }
      } else {
        const std::string error(
            "Classifier not mapped to a relational object ");

        throw exception::Failure(error);
      }
    } else { // update
      auto iu = mttr::Classifier_instance_factory::instance().make_update(
          csf, p_stmt, inst_str, 0, cache_);
      if (iu) {
        iu->execute();

        if (iu->return_code() != qry::Return_code::OK) {

          const std::string error("Classifier instance updating failed ");

          throw exception::Data_error(error);
        }
      } else {
        const std::string error(
            "Classifier not mapped to a relational object ");

        throw exception::Failure(error);
      }
    }

  } else {
    const std::string error("Classifier is abstract, action not applicable ");

    throw exception::Data_error(error);
  }
}


void Data_import::register_instance (const Json::Value& p_j_value, const std::string& p_inst_name) {
    
    if (p_inst_name.empty()) {
        std::cout << "WARNING: Instance not registered in the import cache" << std::endl;
    }
    else {

        auto search = cache_.find ( p_inst_name );
        if ( search != cache_.end() ) {
            std::cout << "WARNING: Instance value already used, the content will be replaced" << std::endl;
        }
        auto id_j_value = std::make_shared<const Json::Value>(qry::Util::erase_url_prefix(p_j_value["url"].asString()));
        cache_.insert_or_assign ( p_inst_name, id_j_value );
    } 
 }
                            
