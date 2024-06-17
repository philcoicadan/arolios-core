/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <vector>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Logical_field.h>
#include <qry/Native_type.h>
#include <qry/Db_types.h>
#include <json/json.h>

namespace arolios {


namespace common {
    class App_info;
    class Object_model_info;
}

namespace util
{
class String
{
public:

    static std::string standardize ( const std::string& p_name ) ;
    static std::vector<std::string> standardize ( std::vector<std::string> && p_name_vec ) ;
    static std::vector<std::string> split ( const std::string& p_string, const char p_sep, const int p_min_nb=0) ;
    static std::string double_quote( const std::string& p_str);
    
 

    
    static void check_date_time (const std::string& p_datetime);
    static void check_number (const std::string& p_number);

    static unsigned int stoui (const std::string& p_str) ;
    



};
}
}

