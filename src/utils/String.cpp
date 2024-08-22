/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <util/String.h>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <exception/Exceptions.h>







using namespace arolios;
using namespace arolios::util;




std::string String::standardize( const std::string& p_name) {

    if (p_name.length() > 30 ) { //TODO limit to be configured
        throw exception::Model_error ("name " + p_name + " too long" ); 
    }
    std::regex regexp("[a-zA-Z0-9][a-zA-Z0-9_-]*"); 
        if (!std::regex_match (p_name,regexp) ) {
        if (p_name.empty() ) {
            return p_name;
        } else {
            throw exception::Model_error ("name " + p_name + " contains forbidden characters" );
        }
    }

    std::string std_name = p_name;
    std::transform (std_name.begin(), std_name.end(), std_name.begin(), ::tolower);
    return std_name;
}

std::vector<std::string> String::standardize( std::vector<std::string> && p_name_vec) {

    for (auto i = 0; i < p_name_vec.size() ; i++) {
        auto && name = standardize(p_name_vec[i]);
        p_name_vec[i] = name;
    }
    return std::move ( p_name_vec );
}






std::vector<std::string> String::split ( const std::string& p_string, const char p_sep, const int p_min_nb) {

    std::vector<std::string> result;
    const char * str_ptr = &p_string[0];
    int count = 0;

    do {
        const char* begin = str_ptr;

        while (*str_ptr != p_sep && *str_ptr) { str_ptr++; }

        if (begin != str_ptr) { result.push_back(std::string(begin, str_ptr)); count++;}

    } while (0 != *str_ptr++);

    if (count >= p_min_nb) {
        return result;
    } else {
        const std::string error_msg = p_string + "has a bad format. At least " + std::to_string(p_min_nb) + " tokens are expected" ;
         
        throw exception::Data_error(error_msg);
    }
}




void String::check_date_time (const std::string& p_datetime) {
    if (! p_datetime.empty() ) {
        std::regex regexp("[a-zA-Z0-9_,: \\+\\-\\/\\.]+");
        if (!std::regex_match (p_datetime,regexp) ) {
            throw exception::Data_error ("date and/or time " + p_datetime + " contains forbidden characters" ); 
        }
    }
}

void String::check_number (const std::string& p_number) {
    if (! p_number.empty() ) {
        std::regex regexp("[-+]?[0-9]+(\\.[0-9]+)?([eE][-+]?[0-9]+)?");
        if (!std::regex_match(p_number,regexp) ) {
            throw exception::Data_error ("number " + p_number + " contains forbidden characters" ); 
        }
    }
}


std::string String::double_quote( const std::string& p_str) {

    std::string tmp = p_str;
    for (std::string::size_type pos{};
         tmp.npos != (pos = tmp.find("'", pos, 1));
         pos += 2) {
        tmp.replace(pos, 1, "''", 2);
    }

    return tmp;
}




unsigned int String::stoui (const std::string& p_str)  {
    
    try {
        unsigned long ul = std::stoul ( p_str );
        if ( ul > std::numeric_limits<unsigned int>::max() ) {
            throw exception::Data_error ( p_str + " is not a number");
        }
        return static_cast<unsigned int> ( ul );
        
    } catch ( const std::invalid_argument& e ) {
         
        throw exception::Data_error ( p_str + " is not a number");

    } catch ( const std::out_of_range& e ) {
         
        throw exception::Data_error ( p_str + " is not a number");
    }
}


