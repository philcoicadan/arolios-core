/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Instance_filter.h>
#include <json/json.h>
#include <sstream>
#include <iostream>



using namespace arolios;
using namespace arolios::qry;

Instance_filter::Instance_filter(const std::string& p_item, const std::string& p_operation, const std::string& p_value) :  item_ (p_item) , operation_ (p_operation), value_ (p_value) {

}



Instance_filter::~Instance_filter() { }

const std::string Instance_filter::item() const {
    return item_;
}
const std::string Instance_filter::operation() const {
    return operation_;
}

const std::string Instance_filter::value() const {
    return value_;
}
    
std::vector<std::shared_ptr<const Instance_filter>> Instance_filter::make_fts_filters ( const std::string& p_json_text)  {
        
    std::vector<std::shared_ptr<const Instance_filter>> result;
    if (! p_json_text.empty()) {
        
        std::istringstream iss; 
        iss.str(p_json_text);
        Json::Value root;
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        builder["failIfExtra"] = false;
        JSONCPP_STRING errs;
        if ( !parseFromStream ( builder, iss,&root, &errs ) ) {
             
            throw std::runtime_error ( errs );
        }
        
        for ( auto item : root.getMemberNames() ) {
            if ( item == "_s") {
                std::string fts_string = root[item].asString();
                
                result.push_back(std::make_shared<Instance_filter>(item, std::string("fts"), fts_string));
                
            } else {
                std::cout << "WARNING: FTS Filter " << item << " not valid" << std::endl;
            }
        }
    }
    return result;
}

std::vector<std::shared_ptr<const Instance_filter>> Instance_filter::make_language_filters ( const std::string& p_json_text)  {
        
    std::vector<std::shared_ptr<const Instance_filter>> result;
    if (! p_json_text.empty()) {
        
        std::istringstream iss; 
        iss.str(p_json_text);
        Json::Value root;
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        builder["failIfExtra"] = false;
        JSONCPP_STRING errs;
        if ( !parseFromStream ( builder, iss,&root, &errs ) ) {
             
            throw std::runtime_error ( errs );
        }

        std::string lang = root.get ( "code","" ).asString();

        if ( !lang.empty() ) {
            result.push_back ( std::make_shared<Instance_filter> ( "code", std::string ( "=" ), lang ) );
            

        } else {
            std::cout << "WARNING: Language Filter " << p_json_text << " not valid" << std::endl;
        }

    }
    return result;
}



            
