/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Classifiers_list.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Enumeration.h>
#include <common/Singleton.h>
#include <common/App_info.h>








using namespace arolios;
using namespace arolios::qry;

template <typename T> Classifiers_list<T>::Classifiers_list ( const std::vector<std::shared_ptr<const omm::Package>>& p_pk_vec, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction , const std::shared_ptr<const omm::Language>& p_lang
) : Base_classifiers_list ( p_pk_vec, p_offset, p_limit, p_sort, p_direction, p_lang) { 

}


template <typename T> Classifiers_list<T>::~Classifiers_list() { }



template <typename T> void Classifiers_list<T>::collect_classifiers (const std::shared_ptr<const omm::Package>& p_pk, std::vector<std::shared_ptr<const omm::Classifier>>& p_csf_vec) {

}

template <> void Classifiers_list<omm::Class>::collect_classifiers (const std::shared_ptr<const omm::Package>& p_pk, std::vector<std::shared_ptr<const omm::Classifier>>& p_csf_vec) {

    const auto csf_map = p_pk->classes();
    int i = p_csf_vec.size();
        p_csf_vec.resize(p_csf_vec.size() + csf_map.size());
        for (auto [first, second]: csf_map) {
         p_csf_vec[i++]= second;
        }
}

template <> void Classifiers_list<omm::Association>::collect_classifiers (const std::shared_ptr<const omm::Package>& p_pk, std::vector<std::shared_ptr<const omm::Classifier>>& p_csf_vec) {

    const auto csf_map = p_pk->associations();
    int i = p_csf_vec.size();
        p_csf_vec.resize(p_csf_vec.size() + csf_map.size());
        for (auto [first, second]: csf_map) {
         p_csf_vec[i++]= second;
        }
}

template <> void Classifiers_list<omm::Enumeration>::collect_classifiers (const std::shared_ptr<const omm::Package>& p_pk, std::vector<std::shared_ptr<const omm::Classifier>>& p_csf_vec) {

    const auto csf_map = p_pk->enumerations();
    int i = p_csf_vec.size();
        p_csf_vec.resize(p_csf_vec.size() + csf_map.size());
        for (auto [first, second]: csf_map) {
         p_csf_vec[i++]= second;
        }
}

template <typename T> const std::string& Classifiers_list<T>::classifier_resource () 
{
    static std::string empty;
    return empty;

}

template <> const std::string& Classifiers_list<omm::Class>::classifier_resource () 
{
    static std::string res = "class";
    return res;

}

template <> const std::string& Classifiers_list<omm::Association>::classifier_resource () 
{
    static std::string res = "association";
    return res;

}

template <> const std::string& Classifiers_list<omm::Enumeration>::classifier_resource () 
{
    static std::string res = "enumeration";
    return res;

}

namespace arolios::qry {

template class Classifiers_list<omm::Class>;
template class Classifiers_list<omm::Association>;
template class Classifiers_list<omm::Enumeration>;

}