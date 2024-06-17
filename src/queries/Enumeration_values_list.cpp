/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Enumeration_values_list.h>
#include <omm/Enumeration.h>
#include <omm/Enum_value.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <exception/Exceptions.h>
#include <string>
#include <vector>
#include <util/String.h>











using namespace arolios;
using namespace arolios::qry;

Enumeration_values_list::Enumeration_values_list ( const std::shared_ptr<const omm::Enumeration>& p_enum, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::shared_ptr<const omm::Language>& p_lang
) : Base_list_query ( p_offset, p_limit, p_sort, p_direction, p_lang) , enum_ (p_enum){ 
   
}   


Enumeration_values_list::~Enumeration_values_list() { }

void Enumeration_values_list::execute ()  {
 
  
    
    std::vector<std::shared_ptr<const omm::Enum_value>> values_vec;
    for  (auto [ key, value] : enum_->enum_values()) { // copy before sort
        values_vec.push_back(value);
    }; 
    

        
    
    sort_result (values_vec) ;
    
    
    
    return_json_ ["total"] = static_cast<int>(values_vec.size());
    return_json_ ["elements"] = Json::Value (Json::arrayValue);
    
    Json::Value& jrows = return_json_["elements"];
    if (limit_ == "ALL" ) {
        jrows.resize(values_vec.size()-offset_);
    } else {
        jrows.resize(std::min<int>(std::stol (limit_), (values_vec.size()-offset_)));
    }
    
    int i= 0;
    for (auto  j = values_vec.begin() + offset_; j< values_vec.end() && i<jrows.size(); ++j) {
        jrows[i] =  Json::Value ( Json::objectValue ) ;
        Json::Value& jrow = jrows[i++];
        
        jrow["name"] = (*j)->name();
        jrow["_tname"] = (*j)->translated_name(lang_);
       
        
    }
    
    return_code_ = qry::Return_code::OK;
            
}

std::string Enumeration_values_list::compute_order_by() 
{
  
    
    int order_by = 0; // default = id
    if (sort_ == "name") {

        order_by = 1;
    } else {
            std::cout << "WARNING: Attribute " <<  sort_  << " ignored." << std::endl;
 
    }
   
 
    order_by += (direction_ == "ASC") ? 0 : 10;

    return std::to_string(order_by);
}

void Enumeration_values_list::sort_result (std::vector<std::shared_ptr<const omm::Enum_value>>& p_values)
{
    
   auto order_by = compute_order_by();
    
    switch (std::stoi(order_by)) {
        case 0:
            std::stable_sort(p_values.begin(), p_values.end(), 
                          [this] (const std::shared_ptr<const omm::Enum_value>& p1, const std::shared_ptr<const omm::Enum_value>& p2 ) { return util::String::stoui(p1->id()) < util::String::stoui(p2->id()) ; });
         break;   
        case 1:
         std::stable_sort(p_values.begin(), p_values.end(), 
                          [this] (const std::shared_ptr<const omm::Enum_value>& p1, const std::shared_ptr<const omm::Enum_value>& p2 ) { return p1->translated_name(lang_) < p2->translated_name(lang_) ; });
         break;   
                              
        
        
        case 10:
            std::stable_sort(p_values.begin(), p_values.end(), 
                          [this] (const std::shared_ptr<const omm::Enum_value>& p1, const std::shared_ptr<const omm::Enum_value>& p2 ) { return util::String::stoui(p1->id())  > util::String::stoui(p2->id())  ; });
         break;  
         
        case 11:
         std::stable_sort(p_values.begin(), p_values.end(), 
                          [this] (const std::shared_ptr<const omm::Enum_value>& p1, const std::shared_ptr<const omm::Enum_value>& p2 ) { return p1->translated_name(lang_)  > p2->translated_name(lang_)  ; });
         break;  
         
       
         
        default:
            throw exception::Failure ( "Internal error, sort not possible" );
            break;
        }

    }

