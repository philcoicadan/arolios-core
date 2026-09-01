/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <json/json.h>
#include <omm/Lc_status.h>
#include <qry/Db_types.h>
#include <util/Native_type.h>

namespace arolios {


namespace omm {
    class Property;
}

namespace qry {
class Util {
public:
    
    enum Read_mode { read_only, read_for_update };
    
    static Read_mode convert_read_mode (const std::string& p_usage) ;
    
    enum Context { ids, list, create, read, update, all };
    
    static Context convert_context (const std::string& p_context) ;

    static std::string extract_json_properties (const Json::StreamWriterBuilder& p_wbuilder, const Json::Value& p_json);
    
    static omm::Lc_status convert_status (const std::string& p_status);

    static std::string url_prefix (const std::string& p_resource) ;

    static std::string erase_url_prefix (const std::string& p_url);

    static std::pair<unsigned int, unsigned long> split_instance_id (const std::string& p_inst_id_str);
    
    static std::string build_instance_id (const qry::Db_row& p_row) ;
    static std::string build_instance_id (const unsigned int p_csf_id, const unsigned long p_row_id) ;

    static int property_native_type (const std::shared_ptr<const omm::Property>& p_prop);

    static std::string native_data_to_string(const util::Native_type p_type, const std::string& p_data, bool p_double_quote=true) ;
    static std::string json_to_native(const util::Native_type p_type, const Json::Value& p_data, bool p_double_quote=true) ;
    static Json::Value native_to_json(const util::Native_type p_type, const qry::Db_field& p_data) ;

   static util::Native_type get_native_type (const std::string& p_native_name) ;
    static std::string get_native_name (const util::Native_type p_native_type);

private:

}; 
}
}

