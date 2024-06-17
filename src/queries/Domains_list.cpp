/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Domains_list.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <omm/Package.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <qry/Util.h>
#include <string>





using namespace arolios;
using namespace arolios::qry;

Domains_list::Domains_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang 
) : Base_list_query ( p_offset, p_limit, p_sort, p_direction,p_lang) {
}


Domains_list::~Domains_list() { }

void Domains_list::execute ()  {
 
  
    
    domains_ = app_info_ptr_->list_user_domains();
    
    internal_execute ();
    
}

void Domains_list::internal_execute() {
    
    sort_result();
    
    return_json_ ["total"] = static_cast<int>(domains_.size());
    return_json_ ["elements"] = Json::Value (Json::arrayValue);
    
    
    Json::Value& jrows = return_json_["elements"];
    if (limit_ == "ALL" ) {
        jrows.resize(domains_.size()-offset_);
    } else {
        jrows.resize(std::min<int>(std::stol (limit_), (domains_.size()-offset_)));
    }
    
    int i= 0;
    for (auto  j = domains_.begin() + offset_; j< domains_.end() && i<jrows.size(); ++j) {
        jrows[i] =  Json::Value ( Json::objectValue ) ;
        Json::Value& jrow = jrows[i++];
        
        jrow["name"] = (*j)->name();
        jrow["description"] = (*j)->description();
        jrow["_tname"] = (*j)->translated_name(lang_) ;
        jrow["url"] = Json::Value ( qry::Util::url_prefix ("domain") + (*j)->name());

        
    }
    
    return_code_ = qry::Return_code::OK;
            
}

std::string Domains_list::compute_order_by ()
{
    int order_by = 1; // default = name
   
    if (sort_.empty() ) {
        order_by = 0; //id
    } else if (sort_ == "description") {
        order_by = 2; //description
    } else if (sort_ != "name"  ) {
        std::cout << "WARNING: Sort with property " << sort_ << " ignored" << std::endl;
        order_by = 0; //id
    }
    
    order_by += (direction_ == "ASC") ? 0 : 10;

    return std::to_string(order_by);
}

void Domains_list::sort_result () {
    
    auto order_by = compute_order_by();
    
    switch (std::stoi(order_by)) {
        case 0:
         std::stable_sort(domains_.begin(), domains_.end(), 
                          [] (const std::shared_ptr<const omm::Package>& p1, const std::shared_ptr<const omm::Package>& p2 ) { return util::String::stoui(p1->id()) < util::String::stoui(p2->id()) ; });
         break;
        case 1:
         std::stable_sort(domains_.begin(), domains_.end(), 
                          [this] (const std::shared_ptr<const omm::Package>& p1, const std::shared_ptr<const omm::Package>& p2 ) { return p1->translated_name(lang_) < p2->translated_name(lang_) ; });
         break;   
                              
        case 2:
         std::stable_sort(domains_.begin(), domains_.end(), 
                          [] (const std::shared_ptr<const omm::Package>& p1, const std::shared_ptr<const omm::Package>& p2 ) { return p1->description() < p2->description() ; });
         break;
 
        case 10:
         std::stable_sort(domains_.begin(), domains_.end(), 
                          [] (const std::shared_ptr<const omm::Package>& p1, const std::shared_ptr<const omm::Package>& p2 ) { return util::String::stoui(p1->id())  > util::String::stoui(p2->id()) ; });
         break;
        case 11:
         std::stable_sort(domains_.begin(), domains_.end(), 
                          [this] (const std::shared_ptr<const omm::Package>& p1, const std::shared_ptr<const omm::Package>& p2 ) { return p1->translated_name(lang_) > p2->translated_name(lang_) ; });
         break;   
                              
        case 12:
         std::stable_sort(domains_.begin(), domains_.end(), 
                          [] (const std::shared_ptr<const omm::Package>& p1, const std::shared_ptr<const omm::Package>& p2 ) { return p1->description() > p2->description() ; });
         break;
        default:
            throw exception::Failure ( "Internal error, sort not possible" );
            break;
        }

    }
