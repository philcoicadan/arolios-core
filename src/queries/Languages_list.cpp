/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Languages_list.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <omm/Language.h>
#include <exception/Exceptions.h>
#include <qry/Instance_filter.h>
#include <util/String.h>
#include <string>




using namespace arolios;
using namespace arolios::qry;

Languages_list::Languages_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang 
) : Base_list_query ( p_offset, p_limit, p_sort, p_direction,p_lang) , filters_ (p_filters) {

}


Languages_list::~Languages_list() { }

void Languages_list::execute ()  {
 
     
    
    auto && languages = filters_.empty() ? app_info_ptr_->list_user_languages(): 
         [this] () ->  std::vector<std::shared_ptr<const omm::Language>> {
            std::vector<std::shared_ptr<const omm::Language>> tmp_vec;
            for (auto filter : this->filters_) {
                if (filter->item() != "code" || filter->operation() != "=" ) {
                    std::cout << "WARNING: Filter on " << filter->item() << " ignored " << std::endl;
                    continue;
                }
                auto lang = this->app_info_ptr_->find_language_by_code(filter->value());
                if (lang) {
                    tmp_vec.push_back(lang);
                }
            }
            return tmp_vec;
         } ()   ;
    
    sort_result(languages);
    
    return_json_ ["total"] = static_cast<int>(languages.size());
    return_json_ ["elements"] = Json::Value (Json::arrayValue);

    
    Json::Value& jrows = return_json_["elements"];
    if (limit_ == "ALL" ) {
        jrows.resize(languages.size()-offset_);
    } else {
        jrows.resize(std::min<int>(std::stol (limit_), (languages.size()-offset_)));
    }
    
    int i= 0;
    for (auto  j = languages.begin() + offset_; j< languages.end() && i<jrows.size(); ++j) {
        jrows[i] =  Json::Value ( Json::objectValue ) ;
        Json::Value& jrow = jrows[i++];
        

        jrow["code"] = (*j)->code();

        jrow["name"] = (*j)->name() ;
        
    }
    
    return_code_ = qry::Return_code::OK;
            
}

std::string Languages_list::compute_order_by()
{
    int order_by = 1; // default = name
   
    if (sort_ == "id") {
        order_by = 0; // id
    } else if (sort_ == "code") {
        order_by = 2; //code
    } else if (sort_ != "name" && ! sort_.empty() ) {
        throw exception::Not_found ( "Property " + sort_ + " not found. Sort not possible" );
    }
    
    order_by += (direction_ == "ASC") ? 0 : 10;

    return std::to_string(order_by);
}

void Languages_list::sort_result (std::vector<std::shared_ptr<const omm::Language>>& p_languages)
{
    
    auto order_by = compute_order_by();
    
    switch (std::stoi(order_by)) {
        case 0:
         std::stable_sort(p_languages.begin(), p_languages.end(), 
                          [] (const std::shared_ptr<const omm::Language>& p1, const std::shared_ptr<const omm::Language>& p2 ) { return util::String::stoui(p1->id()) < util::String::stoui(p2->id()) ; });
         break;
        case 1:
         std::stable_sort(p_languages.begin(), p_languages.end(), 
                          [this] (const std::shared_ptr<const omm::Language>& p1, const std::shared_ptr<const omm::Language>& p2 ) { return p1->name() < p2->name() ; });
         break;   
                              
        case 2:
         std::stable_sort(p_languages.begin(), p_languages.end(), 
                          [] (const std::shared_ptr<const omm::Language>& p1, const std::shared_ptr<const omm::Language>& p2 ) { return p1->code() < p2->code() ; });
         break;
 
        case 10:
         std::stable_sort(p_languages.begin(), p_languages.end(), 
                          [] (const std::shared_ptr<const omm::Language>& p1, const std::shared_ptr<const omm::Language>& p2 ) { return util::String::stoui(p1->id())  > util::String::stoui(p2->id()) ; });
         break;
        case 11:
         std::stable_sort(p_languages.begin(), p_languages.end(), 
                          [this] (const std::shared_ptr<const omm::Language>& p1, const std::shared_ptr<const omm::Language>& p2 ) { return p1->name() > p2->name() ; });
         break;   
                              
        case 12:
         std::stable_sort(p_languages.begin(), p_languages.end(), 
                          [] (const std::shared_ptr<const omm::Language>& p1, const std::shared_ptr<const omm::Language>& p2 ) { return p1->code() > p2->code() ; });
         break;
        default:
            throw exception::Failure ( "Internal error, sort not possible" );
            break;
        }

    }
