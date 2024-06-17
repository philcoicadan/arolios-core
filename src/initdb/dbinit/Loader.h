/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Return_code.h>
#include <common/Db_client.h>
#include <json/value.h>
#include <memory>
#include <string>

namespace arolios {
namespace common {
    class User;
}

namespace model {
    class Model_recorder;
}

namespace dbinit {
class Loader {
public:
    /**
     * Constructor
     */
    Loader(const std::string& p_json_file, const std::string& p_xml_file);

    /**
     * Destructor
     */
    ~Loader();
    
    void execute () ;

    qry::Return_code return_code () const ;


private:

    const std::shared_ptr<common::Db_client> db_;

    qry::Return_code return_code_;

    
    void create_root (const std::string& p_name) const;
    void create_default_app (const std::string& p_name) const;
    const std::shared_ptr<common::User> create_super_user (const std::string& p_name) const;
    void create_language ( const std::string& p_code, const std::string& p_name, const std::string& p_fts_config, const std::string& p_descr) const ;
    void map_primitive_types (const Json::Value& p_root) const;
    void associate_primitive_native_types () const;
    void set_default_language ( const std::string& p_code) const ;
    void insert_param ( const std::string& p_key, const std::string& p_value) const ;
    void add_multi_languages (const std::string&  p_type_name, const std::string& p_prop_type_name, const model::Model_recorder& p_mdl_rcd) const ;
    void process_multilingual_types ( const Json::Value& p_jroot ) const;
    
    const std::string json_file_;
    const std::string xml_file_;
    
};
}
}

