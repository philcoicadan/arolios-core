/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Util.h>
#include <json/writer.h>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <omm/Property.h>
#include <common/Singleton.h>
#include <common/Object_model_info.h>


using namespace arolios;
using namespace arolios::qry;

Util::Read_mode Util::convert_read_mode (const std::string& p_usage) {
    if (p_usage == "ro") {
        return Util::Read_mode::read_only;
    } else {
        return Util::Read_mode::read_for_update;
    }
}


Util::Context Util::convert_context (const std::string& p_context) {
    static std::unordered_map<std::string, Util::Context> map { 
        {"IDS", Util::Context::ids },
        {"LIST", Util::Context::list },
        {"CREATE", Util::Context::create},
        {"READ", Util::Context::read},
        {"UPDATE", Util::Context::update},
        {"ALL", Util::Context::all}

    } ;
    
    auto search = map.find(p_context);
    if (search != map.end()) {
        return search->second;
    } else {
        return Util::Context::ids;
    }
}

std::string Util::extract_json_properties ( const Json::StreamWriterBuilder& p_wbuilder, const Json::Value& p_json ) {
    std::string result;


    const Json::Value& jprops = ( p_json.isMember("properties")) ?
                                    p_json["properties"] :
                                    p_json;
    
    auto members = jprops.getMemberNames ();
    std::string item;
    std::ostringstream oss;
    std::unique_ptr<Json::StreamWriter> writer (p_wbuilder.newStreamWriter());

    for ( auto member : members ) {

        if ( jprops[member].isObject() ) {

                oss <<  extract_json_properties ( p_wbuilder, jprops[member] ) ;
        } else if (jprops[member].isString() ){ // avoid the quotes
                oss << jprops[member].asString();
        } else {           
                writer->write(jprops[member], &oss);

        }
        oss << ",";
        
    }
    result = oss.str();
    if (!result.empty() ) {
        result.erase(result.length()-1,1);
    }
    
    return result;
}

omm::Lc_status Util::convert_status (const std::string& p_status) {
    
     static std::unordered_map<std::string,omm::Lc_status> lc_status_map {{"active", omm::Lc_status::active}, { "deprecated", omm::Lc_status::deprecated}, { "deleted",  omm::Lc_status::deleted}} ;
     
     return lc_status_map.find(p_status)->second;
}

std::string Util::url_prefix (const std::string& p_str)  {
    
    
    static std::unordered_map<std::string, const std::string> map {
        {"app", "/apps/"},
        {"association", "/associations/"},
        {"class", "/classes/"},
        {"domain", "/domains/"},
        {"enumeration", "/enumerations/"},
        {"instance", "/instances/"},
        {"language", "/languages/"},
        {"trash", "/trash/"},
        {"user", "/users/"} ,
        {"appdomain", "/appdomains/"},
        {"appuser", "/appusers/"}

    };

    auto search = map.find(p_str);
    if (search == map.end()) {
        std::string error_msg = "Resource " + p_str + " not found";
         
        throw exception::Not_found (error_msg);
    }

    return search->second;
    

}

std::string Util::erase_url_prefix (const std::string& p_url) {

    int pos = p_url.rfind("/");
    if (pos != std::string::npos) {
        return p_url.substr(pos+1);
    }
    else {
        const std::string error_msg = "URL " + p_url + " is invalid";
         
        throw exception::Data_error (error_msg);
    }
}


Native_type Util::get_native_type (const std::string& p_native_name) {
    return Internal::instance().native_type(p_native_name);
}

bool Util::has_native_type_name (const std::string& p_native_name) {
    return Internal::instance().has_native_type(p_native_name);
}

std::string Util::get_native_name (const Native_type p_native_type) {
    static std::unordered_map<Native_type,std::string> map = {{ Native_type::_integer, "integer"}, {Native_type::_bigint, "bigint"}, {Native_type::_smallint, "smallint"}, {Native_type::_boolean, "boolean"}, {Native_type::_numeric, "numeric"}, {Native_type::_real, "real"}, { Native_type::_double_precision, "double precision"}, {Native_type::_character_varying,"character varying"}, {Native_type::_text, "text"}, {Native_type::_date, "date"}, {Native_type::_time, "time"}, { Native_type::_timestamp, "timestamp"}, { Native_type::_time_with_time_zone, "time with time zone"}, { Native_type::_timestamp_with_time_zone, "timestamp with time zone" }};

    return map.at(p_native_type);
}


std::string Util::json_to_native(const Native_type p_type, const Json::Value& p_data, bool p_double_quote) {

    switch (static_cast<int>(p_type)) {
        case Native_type::_character_varying:
        case Native_type::_text: {
            if (p_double_quote) {
                return "'"+ util::String::double_quote(p_data.asString()) + "'";
            }
            else {
                return "'"+ p_data.asString() + "'";

            }
            break; }
        case Native_type::_date:
        case Native_type::_time:
        case Native_type::_timestamp:
        case Native_type::_time_with_time_zone:
        case Native_type::_timestamp_with_time_zone: {
            util::String::check_date_time (p_data.asString());
            return get_native_name(p_type) + " '" + p_data.asString()  + "'";
            break; }
        case Native_type::_boolean: {
            if (p_data.isBool())  {
                return (p_data.asBool())? "true" : "false";
            }
            else {
               throw exception::Data_error ("boolean value " + p_data.asString() + " contains forbidden characters" );
            }
            break; }
        case Native_type::_integer:
        case Native_type::_bigint:
        case Native_type::_smallint:
        case Native_type::_real:
        case Native_type::_double_precision: {
            util::String::check_number (p_data.asString());
            std::stringstream tmp;
            tmp << p_data;
            return tmp.str();
            break; }
        case Native_type::_numeric: {
            util::String::check_number (p_data.asString());
             return " '" + p_data.asString()  + "'::" + get_native_name(p_type) ;
            break;
        }
        default:
            throw exception::Data_error ("value" + p_data.asString() + " contains forbidden characters" ); 

            break;
    }

}

std::string Util::string_to_native ( const Native_type p_type, const std::string& p_data , bool p_double_quote) {

    switch ( static_cast<int> ( p_type ) ) {
    case Native_type::_character_varying:
    case Native_type::_text: {
        if (p_double_quote) {
            return "'"+ util::String::double_quote ( p_data ) + "'";
        }
        else {
            return "'"+ p_data  + "'";
        }
        break;
    }
    case Native_type::_date:
    case Native_type::_time:
    case Native_type::_timestamp:
    case Native_type::_time_with_time_zone:
    case Native_type::_timestamp_with_time_zone: {
        util::String::check_date_time (p_data);
        return get_native_name ( p_type ) + " '" + p_data  + "'";
        break; }
    case Native_type::_boolean: {
      const std::string bool_val = util::String::standardize(p_data);
      if (bool_val == "true" || bool_val == "false") {
        return bool_val;
      } else {
        throw exception::Data_error("boolean value " + p_data +
                               " contains forbidden characters");
      }
      break;
    }
    case Native_type::_integer:
    case Native_type::_bigint:
    case Native_type::_smallint:
    case Native_type::_real:
    case Native_type::_double_precision: {
        util::String::check_number (p_data);
        std::stringstream tmp;
        tmp << p_data;
        return tmp.str();
        break; }
    case Native_type::_numeric: {
        util::String::check_number (p_data);
            return " '" + p_data  + "'::" + get_native_name(p_type) ;
        break;
    }
    default: 
        throw exception::Data_error ("value" + p_data + " contains forbidden characters" ); 

        break;
    }

}

Json::Value Util::native_to_json(const Native_type p_type, const qry::Db_field& p_data) {
    
    switch ( static_cast<int> ( p_type ) ) {
    case Native_type::_integer:
    case Native_type::_smallint:
        return  ( p_data.as<int>() );
        break;
    case Native_type::_bigint:
        return  ( p_data.as<Json::Value::Int64>() );
        break;
    case Native_type::_real:
        return  ( p_data.as<float>() );
        break;

    case Native_type::_double_precision:
        return  ( p_data.as<double>() );
        break;
    case Native_type::_boolean:
        return ( p_data.as<bool>() );
        break;
    case Native_type::_numeric:
        return ( p_data.as<std::string>() );
        break;
    default:
        return ( p_data.as<std::string>() );
        break;
    }
}

int Util::property_native_type(const std::shared_ptr<const omm::Property>& p_prop) {

    int result = -1;
    if (p_prop->has_referencing_type() ) {
        result = static_cast<int>(Native_type::_bigint);
    } else {
        const auto & map = common::Singleton<common::Object_model_info>::instance().object()->primitive_native_types_map();
        auto && search = map.find(p_prop->type()->name());
        if (search != map.end()) {
            result =  static_cast<int>(search->second);
        }
    }

    return result;
}


Util::Internal::Internal()
{


    native_name_type_map_ = {{"integer", Native_type::_integer}, {"bigint", Native_type::_bigint}, {"smallint", Native_type::_smallint}, {"boolean", Native_type::_boolean}, {"numeric", Native_type::_numeric}, {"real", Native_type::_real}, {"double precision", Native_type::_double_precision}, {"character varying", Native_type::_character_varying}, {"text", Native_type::_text}, {"date", Native_type::_date}, {"time", Native_type::_time}, {"timestamp", Native_type::_timestamp}, {"time with time zone", Native_type::_time_with_time_zone}, {"timestamp with time zone" , Native_type::_timestamp_with_time_zone}};
}

Util::Internal::~Internal()
{
}




Native_type Util::Internal::native_type (const std::string& p_type_name) const
{
    return native_name_type_map_.at(p_type_name);
}

bool Util::Internal::has_native_type (const std::string& p_type_name) const
{
 return (native_name_type_map_.find(p_type_name) != native_name_type_map_.end());
}

Util::Internal& Util::Internal::instance()
{
    static Internal inst = Internal ();
    return inst;
}



std::pair<unsigned int, unsigned long> Util::split_instance_id (const std::string& p_inst_id_str) {

    const auto id_vec = util::String::split(p_inst_id_str,'.', 2);
    try {
        unsigned long row_id = std::stoul ( id_vec[1] ) ;
        unsigned int csf_id = std::stoul ( id_vec[0] );
        return { csf_id, row_id } ;

    } catch ( const std::invalid_argument& e ) {
        std::string error_msg = "Instance id " + p_inst_id_str + " bad declared";
         
        throw exception::Not_found ( error_msg );

    } catch ( const std::out_of_range& e ) {
        std::string error_msg = "Instance id " + p_inst_id_str + " bad declared";
         
        throw exception::Not_found ( error_msg );
    }
}

std::string Util::build_instance_id (const qry::Db_row& p_row) {

       return build_instance_id (p_row["csf_id"].as<unsigned int>(),p_row["id"].as<unsigned long>()) ;
}

std::string Util::build_instance_id (const unsigned int p_csf_id, const unsigned long p_row_id) {

    return std::to_string(p_csf_id) + "." + std::to_string(p_row_id) ;
}
