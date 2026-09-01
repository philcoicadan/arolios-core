/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
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
#include <util/Native_type.h>
#include <omm/Property.h>
#include <omm/Primitive_type.h>
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


util::Native_type Util::get_native_type (const std::string& p_native_name) {
    static std::unordered_map<std::string,util::Native_type> map = {{ "integer", util::Native_type::integer}, {"bigint", util::Native_type::bigint}, {"smallint", util::Native_type::smallint}, {"boolean", util::Native_type::boolean}, {"numeric", util::Native_type::numeric}, {"real", util::Native_type::real}, { "double precision", util::Native_type::double_precision}, {"character varying", util::Native_type::character_varying}, {"text", util::Native_type::text}, {"date", util::Native_type::date}, {"time", util::Native_type::time}, { "timestamp", util::Native_type::timestamp}, { "time with time zone", util::Native_type::time_with_time_zone}, { "timestamp with time zone", util::Native_type::timestamp_with_time_zone}};

    auto iter = map.find(p_native_name);
    if (iter == map.end()){
        throw exception::Not_found ("native type " + p_native_name + " not found" );

    } else {
        return iter->second;
    }

}


std::string Util::get_native_name (const util::Native_type p_native_type) {
    static std::unordered_map<util::Native_type,std::string> map = {{ util::Native_type::integer, "integer"}, {util::Native_type::bigint, "bigint"}, {util::Native_type::smallint, "smallint"}, {util::Native_type::boolean, "boolean"}, {util::Native_type::numeric, "numeric"}, {util::Native_type::real, "real"}, { util::Native_type::double_precision, "double precision"}, {util::Native_type::character_varying,"character varying"}, {util::Native_type::text, "text"}, {util::Native_type::date, "date"}, {util::Native_type::time, "time"}, { util::Native_type::timestamp, "timestamp"}, { util::Native_type::time_with_time_zone, "time with time zone"}, { util::Native_type::timestamp_with_time_zone, "timestamp with time zone" }};

    return map.at(p_native_type);
}


std::string Util::json_to_native(const util::Native_type p_type, const Json::Value& p_data, bool p_double_quote) {

    switch (static_cast<int>(p_type)) {
        case util::Native_type::character_varying:
        case util::Native_type::text: {
            if (p_double_quote) {
                return "'"+ util::String::double_quote(p_data.asString()) + "'";
            }
            else {
                return "'"+ p_data.asString() + "'";

            }
            break; }
        case util::Native_type::date:
        case util::Native_type::time:
        case util::Native_type::timestamp:
        case util::Native_type::time_with_time_zone:
        case util::Native_type::timestamp_with_time_zone: {
            util::String::check_date_time (p_data.asString());
            return get_native_name(p_type) + " '" + p_data.asString()  + "'";
            break; }
        case util::Native_type::boolean: {
            if (p_data.isBool())  {
                return (p_data.asBool())? "true" : "false";
            }
            else {
               throw exception::Data_error ("boolean value " + p_data.asString() + " contains forbidden characters" );
            }
            break; }
        case util::Native_type::integer:
        case util::Native_type::bigint:
        case util::Native_type::smallint:
        case util::Native_type::real:
        case util::Native_type::double_precision: {
            util::String::check_number (p_data.asString());
            std::stringstream tmp;
            tmp << p_data;
            return tmp.str();
            break; }
        case util::Native_type::numeric: {
            util::String::check_number (p_data.asString());
             return " '" + p_data.asString()  + "'::" + get_native_name(p_type) ;
            break;
        }
        default:
            throw exception::Data_error ("value" + p_data.asString() + " contains forbidden characters" ); 

            break;
    }

}

std::string Util::native_data_to_string ( const util::Native_type p_type, const std::string& p_data , bool p_double_quote) {

    switch ( static_cast<int> ( p_type ) ) {
    case util::Native_type::character_varying:
    case util::Native_type::text: {
        if (p_double_quote) {
            return "'"+ util::String::double_quote ( p_data ) + "'";
        }
        else {
            return "'"+ p_data  + "'";
        }
        break;
    }
    case util::Native_type::date:
    case util::Native_type::time:
    case util::Native_type::timestamp:
    case util::Native_type::time_with_time_zone:
    case util::Native_type::timestamp_with_time_zone: {
        util::String::check_date_time (p_data);
        return get_native_name ( p_type ) + " '" + p_data  + "'";
        break; }
    case util::Native_type::boolean: {
      const std::string bool_val = util::String::standardize(p_data);
      if (bool_val == "true" || bool_val == "false") {
        return bool_val;
      } else {
        throw exception::Data_error("boolean value " + p_data +
                               " contains forbidden characters");
      }
      break;
    }
    case util::Native_type::integer:
    case util::Native_type::bigint:
    case util::Native_type::smallint:
    case util::Native_type::real:
    case util::Native_type::double_precision: {
        util::String::check_number (p_data);
        std::stringstream tmp;
        tmp << p_data;
        return tmp.str();
        break; }
    case util::Native_type::numeric: {
        util::String::check_number (p_data);
            return " '" + p_data  + "'::" + get_native_name(p_type) ;
        break;
    }
    default: 
        throw exception::Data_error ("value" + p_data + " contains forbidden characters" ); 

        break;
    }

}

Json::Value Util::native_to_json(const util::Native_type p_type, const qry::Db_field& p_data) {
    
    switch ( static_cast<int> ( p_type ) ) {
    case util::Native_type::integer:
    case util::Native_type::smallint:
        return  ( p_data.as<int>() );
        break;
    case util::Native_type::bigint:
        return  ( p_data.as<Json::Value::Int64>() );
        break;
    case util::Native_type::real:
        return  ( p_data.as<float>() );
        break;

    case util::Native_type::double_precision:
        return  ( p_data.as<double>() );
        break;
    case util::Native_type::boolean:
        return ( p_data.as<bool>() );
        break;
    case util::Native_type::numeric:
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
        result = static_cast<int>(util::Native_type::bigint);

    } else if (p_prop->has_primitive_type () ) {
        auto prim_type = std::dynamic_pointer_cast<const omm::Primitive_type>(p_prop->type());
        result = static_cast<int>(prim_type->native_type());

    }

    return result;
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
