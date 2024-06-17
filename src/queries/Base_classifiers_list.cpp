/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Base_classifiers_list.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <qry/Util.h>






using namespace arolios;
using namespace arolios::qry;

Base_classifiers_list::Base_classifiers_list ( const std::vector<std::shared_ptr<const omm::Package>>& p_pk_vec, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction , const std::shared_ptr<const omm::Language>& p_lang
) : Base_list_query ( p_offset, p_limit, p_sort, p_direction, p_lang) , pk_vec_ (p_pk_vec){ 
 
}


Base_classifiers_list::~Base_classifiers_list() { }

void Base_classifiers_list::execute ()  {
 
 
    std::vector<std::shared_ptr<const omm::Classifier>> csf_vec ;
    
    for (const auto pk : pk_vec_) {
        collect_classifiers (pk, csf_vec);
    }
    sort_result (csf_vec) ;
    
    
    
    return_json_ ["total"] = static_cast<int>(csf_vec.size());
    return_json_ ["elements"] = Json::Value (Json::arrayValue);

    
    Json::Value& jrows = return_json_["elements"];
    if (limit_ == "ALL" ) {
        jrows.resize(csf_vec.size()-offset_);
    } else {
        jrows.resize(std::min<int>(std::stol (limit_), (csf_vec.size()-offset_)));
    }
    
    int i= 0;
    for (auto  j = csf_vec.begin() + offset_; j< csf_vec.end() && i<jrows.size(); ++j) {
        jrows[i] =  Json::Value ( Json::objectValue ) ;
        Json::Value& jrow = jrows[i++];
        

        jrow["name"] = (*j)->name();
        jrow["_tname"] = (*j)->translated_name(lang_);
        jrow["description"] = (*j)->description();
        jrow["isAbstract"] = (*j)->is_abstract();

        jrow["_tid_name"] = (*j)->package()->translated_name(lang_) + "." + (*j)->translated_name(lang_);
        jrow["url"] = Json::Value (qry::Util::url_prefix(classifier_resource()) + (*j)->package()->name() + "." + (*j)->name());
        
    }
    
    return_code_ = qry::Return_code::OK;

            
}

std::string Base_classifiers_list::compute_order_by()
{
   int order_by = 1; // default = name
   
    if (sort_.empty()) {
        order_by = 0; //id
    } else if (sort_ == "description") {
        order_by = 2; //description
    } else if (sort_ == "isabstract") { // all HTTP inputs are transformed in lowercase
        order_by = 3; //isAbstract

    } else if (sort_ != "name" ) {
        std::cout << "WARNING: Sort with property " << sort_ << " ignored" << std::endl;
        order_by = 0; //id
    }
    
    order_by += (direction_ == "ASC") ? 0 : 10;

    return std::to_string(order_by);
}


void Base_classifiers_list::sort_result (std::vector<std::shared_ptr<const omm::Classifier>>& p_csf_vec) {
    
 auto order_by = compute_order_by();
    
    switch (std::stoi(order_by)) {
        case 0:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return util::String::stoui(p1->id()) < util::String::stoui(p2->id()) ; });
         break;
        case 1:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [this] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return p1->translated_name(lang_) < p2->translated_name(lang_) ; });
         break;   
                              
        case 2:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return p1->description() < p2->description() ; });
         break;
        
        case 3:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return static_cast<int>(p1->is_abstract()) < static_cast<int>(p2->is_abstract()) ; });
         break;
 
                  
        case 10:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return util::String::stoui(p1->id())  > util::String::stoui(p2->id()) ; });
         break;
        case 11:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [this] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return p1->translated_name(lang_) > p2->translated_name(lang_) ; });
         break;   
                              
        case 12:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return p1->description() > p2->description() ; });
         break;
        
        case 13:
         std::stable_sort(p_csf_vec.begin(), p_csf_vec.end(), 
                          [] (const std::shared_ptr<const omm::Classifier>& p1, const std::shared_ptr<const omm::Classifier>& p2 ) { return static_cast<int>(p1->is_abstract()) > static_cast<int>(p2->is_abstract()) ; });
         break;
         

        default:
            throw exception::Failure ( "Internal error, sort not possible" );
            break;
        }

    }

