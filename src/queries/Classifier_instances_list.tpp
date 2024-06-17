/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Classifier_instances_list.h>





using namespace arolios;
using namespace arolios::qry;

template <typename T>
Classifier_instances_list<T>::Classifier_instances_list(
    const std::shared_ptr<const T> &p_csf, const std::string &p_offset,
    const std::string &p_limit, const std::string &p_sort,
    const std::string &p_direction,
    const std::vector<std::shared_ptr<const qry::Instance_filter>> &p_filters,
    const std::string &p_prop_list,
    const std::shared_ptr<const omm::Language> &p_lang)
    : Base_classifier_instances_list{p_offset,
                                     p_limit,   p_sort,      p_direction,
                                     p_filters, p_prop_list, p_lang},
      csf_(p_csf) {}

template <typename T> Classifier_instances_list<T>::~Classifier_instances_list() { }



template <typename T> void Classifier_instances_list<T>::build_json_from_row ( Json::Value& p_jrow, const qry::Db_row& p_row, const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_props_map, const std::unordered_map<std::string,std::string>& p_fields_map, const std::unordered_map<std::string,std::string>& p_field_aliases_map ) 
{
}

template <typename T> void Classifier_instances_list<T>::build_main_ids_json(Json::Value &p_jrow, const qry::Db_row &p_row,
                 const std::pair<std::string, std::string> &p_id_pair,
                 const std::pair<std::string, std::string> &p_csf_id_pair)
{
}

template <typename T> const std::shared_ptr<const omm::Classifier> Classifier_instances_list<T>::classifier() const
{
    return csf_;
}


