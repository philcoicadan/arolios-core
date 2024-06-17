/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Exporter.h>
#include <tqry/Association.h>
#include <tqry/Class.h>
#include <tqry/Member_end.h>
#include <tqry/Property.h>
#include <tqry/Tree_query.h>
#include <tqry/query.h>
#include <exception/Exceptions.h>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <omm/Association.h>
#include <omm/Property.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <FilterLexer.h>
#include <FilterParser.h>
#include <Qry_filter_visitor.h>
#include <util/String.h>
#include <antlr4-runtime.h>



using namespace arolios;
using namespace arolios::tqry;

Exporter::Exporter(const std::string &p_query_file)
    : query_file_(p_query_file) {}

Exporter::~Exporter() {}

void Exporter::param_file(const std::string &p_param_file) {
  param_file_ = p_param_file;
}

void Exporter::execute() {

  query_ = std::make_shared<Tree_query>();

  try {
    std::unique_ptr<XML_query_t> q(query(query_file_));

    for (XML_query_t::parameter_const_iterator p(q->parameter().begin());
         p != q->parameter().end(); ++p) {

      const auto param = util::String::standardize(*p);
      query_->add_parameter(param);
    }

    if (!q->parameter().empty() ) {
        fill_param_values();
    }
    
    auto opt_class = q->class_();

    if (opt_class.present()) {
      auto elem = opt_class.get();


      auto cls = execute_class(elem);
      cls->is_root(true);
      query_->root(cls);

    } else {

      auto opt_assoc = q->association();

      if (opt_assoc.present()) {
        auto elem = opt_assoc.get();


        const std::string error_msg =
            "Association as root classifier is not yet implemented";
         
        throw exception::Not_yet_implemented(error_msg);
      } else {
        const std::string error_msg =
            "Root classifier is missing for exporting data";
         
        throw exception::Model_error(error_msg);
      }
    }

    query_->prepare();

    query_->execute(return_json_);

    return_code_ = qry::Return_code::OK;

  } catch (const xml_schema::parsing &e) {
   
    const std::string error_msg =
        "Error in parsing the query file " + std::string(e.what());
     
    std::cerr << e.diagnostics() << std::endl;
    throw exception::Model_error(error_msg);
  }
}

std::shared_ptr<Class>
Exporter::execute_class(const XML_query_class_t &p_xml_elem) {

  auto mdl_cls = execute_classifier<omm::Class, XML_query_class_t>(p_xml_elem);

  auto cls = std::make_shared<Class>();
  cls->model_class(mdl_cls);

  execute_base_class(p_xml_elem, cls);

  return cls;
}

void Exporter::execute_base_class(const XML_query_base_class_t &p_xml_elem,
                                  const std::shared_ptr<Class> &p_cls) {

  execute_query_base(p_xml_elem, p_cls);

  for (XML_query_base_class_t::memberEnd_const_iterator elem(
           p_xml_elem.memberEnd().begin());
       elem != p_xml_elem.memberEnd().end(); ++elem) {


    execute_member_end(*elem, p_cls);
  }
}

std::shared_ptr<Association>
Exporter::execute_association(const XML_query_association_t &p_xml_elem) {

  auto mdl_assoc =
      execute_classifier<omm::Association, XML_query_association_t>(p_xml_elem);

  auto assoc = std::make_shared<Association>();
  assoc->model_association(mdl_assoc);

  execute_query_base(p_xml_elem, assoc);

  for (XML_query_association_t::memberEnd_const_iterator elem(
           p_xml_elem.memberEnd().begin());
       elem != p_xml_elem.memberEnd().end(); ++elem) {

    execute_member_end(*elem, assoc);
  }

  return assoc;
}

void Exporter::execute_member_end(const XML_query_member_end_t &p_xml_elem,
                                  const std::shared_ptr<Association> &p_assoc) {


  auto me_name = util::String::standardize(p_xml_elem.name());
  auto me_mdl_prop = p_assoc->model_association()->find_member_end(me_name);
  if (!me_mdl_prop) {
    std::string error_msg = "Member end " + me_name + " not found";
     
    throw exception::Not_found(error_msg);
  }
  auto me_prop = std::make_shared<Member_end>();
  me_prop->model_property(me_mdl_prop);
  me_prop->id_name(me_name);
  p_assoc->insert_member_end(me_prop);
  me_prop->association(p_assoc);
  me_prop->classifier(p_assoc);

  auto target_cls = std::make_shared<Class>();
  auto mdl_target =
      std::dynamic_pointer_cast<const omm::Class>(me_mdl_prop->type());
  if (!mdl_target) {
    std::string error_msg =
        "Target class of Member end " + me_name + " not valid";
     
    throw exception::Failure(error_msg);
  }
  target_cls->model_class(mdl_target);
  me_prop->target(target_cls);

  auto class_elem = p_xml_elem.class_();

  execute_base_class(class_elem, target_cls);
}

void Exporter::execute_member_end(const XML_query_member_end_t &p_xml_elem,
                                  const std::shared_ptr<Class> &p_cls) {


  auto me_name = util::String::standardize(p_xml_elem.name());
  auto me_mdl_prop = p_cls->model_class()->find_property_member_end(me_name);
  if (!me_mdl_prop) {
    std::string error_msg = "Member end " + me_name + " not found";
     
    throw exception::Not_found(error_msg);
  }
  auto me_prop = std::make_shared<Member_end>();
  me_prop->model_property(me_mdl_prop);
  me_prop->id_name(me_name);
  p_cls->insert_member_end(me_prop);
  me_prop->classifier(p_cls);

  auto mdl_assoc = me_mdl_prop->association();
  auto assoc = std::make_shared<Association>();
  assoc->model_association(mdl_assoc);
  me_prop->association(assoc);
  assoc->insert_member_end(me_prop);

  auto opt_assoc = p_xml_elem.association();

  if (opt_assoc.present()) {
    auto elem = opt_assoc.get();


    me_prop->optional_association(true);

    execute_query_base(elem, assoc);
  }

  auto target_cls = std::make_shared<Class>();
  auto mdl_target =
      std::dynamic_pointer_cast<const omm::Class>(me_mdl_prop->type());
  if (!mdl_target) {
    std::string error_msg =
        "Target class of Member end " + me_name + " not valid";
     
    throw exception::Failure(error_msg);
  }
  target_cls->model_class(mdl_target);
  me_prop->target(target_cls);

  auto class_elem = p_xml_elem.class_();

  execute_base_class(class_elem, target_cls);
}

void Exporter::execute_query_base(const XML_query_base_t &p_xml_elem,
                                  const std::shared_ptr<Classifier> &p_csf) {

  // property
  for (XML_query_base_t::property_const_iterator elem(
           p_xml_elem.property().begin());
       elem != p_xml_elem.property().end(); ++elem) {
    const std::string prop_name = *elem;
    auto prop_vec = util::String::standardize(
        util::String::split(prop_name, omm::Named_element::separator()));
    execute_property(prop_vec, p_csf);
  }
  // filter
  auto opt_filter = p_xml_elem.filter();

  if (opt_filter.present()) {
    auto elem = opt_filter.get();

    auto sql_filter = transform_filter(elem, p_csf);
    p_csf->filter(sql_filter);
  }
}

template <typename T1, typename T2>
std::shared_ptr<T1> Exporter::execute_classifier(const T2 &p_xml_elem) {


  std::shared_ptr<T1> result;
  auto pk_name = util::String::standardize(p_xml_elem.domain());
  auto csf_name = util::String::standardize(p_xml_elem.name());

  auto app_info = common::Singleton<common::App_info>::instance().object();
  std::shared_ptr<omm::Classifier> csf =
      app_info->find_domain_element_by_name<omm::Classifier>(pk_name, csf_name);
  if (csf) {
    result = std::dynamic_pointer_cast<T1>(csf);
  }
  if (!result) {
    std::string error_msg =
        "Classifier " + pk_name + "." + csf_name + " not found";
     
    throw exception::Not_found(error_msg);
  }
  return result;
}

void Exporter::execute_property(
    const std::vector<std::string> &p_prop_names_vec,
    const std::shared_ptr<Classifier> &p_csf) {

  if (p_prop_names_vec.size() > 0) {

    const auto prop = p_csf->model_classifier()->find_element_in_all_properties(
        p_prop_names_vec[0]);
    if (prop) {
      auto q_prop = std::make_shared<Property>();
      q_prop->model_property(prop);
      q_prop->id_name(p_prop_names_vec[0]);
      p_csf->insert_property(q_prop);
      q_prop->classifier(p_csf);
      if (p_prop_names_vec.size() > 1) {
        const auto attr =
            prop->type()->find_element_by_name<omm::Property>(p_prop_names_vec[1]);
        if (attr) {
          auto q_attr = std::make_shared<Property>();
          q_attr->model_property(attr);
          q_attr->id_name(p_prop_names_vec[1]);
          q_prop->nested_property(q_attr);

        } else {
          std::string error_msg =
              "Property " + p_prop_names_vec[1] + " not found";
           
          throw exception::Not_found(error_msg);
        }
      }
    } else {
      std::string error_msg = "Property " + p_prop_names_vec[0] + " not found";
       
      throw exception::Not_found(error_msg);
    }

  } else {
    std::string error_msg = "Property not valid";
     
    throw exception::Failure(error_msg);
  }
}

std::string
Exporter::transform_filter(const std::string &p_filter,
                           const std::shared_ptr<Classifier> &p_csf) {

  antlr4::ANTLRInputStream input(p_filter);
  qryfilter::FilterLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  qryfilter::FilterParser parser(&tokens);

  qryfilter::FilterParser::FilterContext *tree = parser.filter();


  qryfilter::Qry_filter_visitor visitor(p_csf->model_classifier(), param_values_);

  visitor.visitFilter(tree);

  return visitor.filter_result();
}

void Exporter::fill_param_values() {

  if (!param_file_.empty()) {

    std::ifstream ifs;
    ifs.open(param_file_);
    Json::Value values;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &values, &errs)) {
       
      throw exception::Model_error(errs);
      return;
    }

    for (auto param : query_->parameters()) {
      if (values.isMember(param)) {
        auto value = values[param];
        param_values_.insert({param, value});

      } else {
        std::string error_msg = "Value for parameter " + param + " not found";
         
        throw exception::Not_found(error_msg);
        break;

 
      }
    }
  } else {
    std::string error_msg = "The parameters of the query cannot be set, the parameter file is missing ";
     
    throw exception::Not_found(error_msg);
  }
}
