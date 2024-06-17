/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Return_code.h>
#include <qry/Util.h>
#include <common/Db_client.h>
#include <qry/Base_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <qry/Db_types.h>
#include <appcfg/Base_classifier_config.h>
#include <qry/Instance_filter.h>

namespace arolios {


namespace omm {
    class Property;
    class Classifier;
}

namespace qry {

class Base_instance_read : public Base_query {
public:
    /**
     * Constructor
     */
    Base_instance_read( const std::string& p_inst_id, const qry::Util::Read_mode p_usage , const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang);

    /**
     * Destructor
     */
    virtual ~Base_instance_read();
    
    
protected:

virtual void build_linked_property_json (const std::string& p_key, const std::shared_ptr<const omm::Property>& p_prop,   const qry::Db_row& p_row, Json::Value& p_jrow, const std::unordered_map<std::string,std::string>& p_fields_map, const std::unordered_map<std::string,std::string>& p_field_aliases_map);
    
    virtual void build_ids_json ( const qry::Db_row& p_row, Json::Value& p_root_json ) = 0;
    virtual const std::string get_classifier_ids () const = 0;
    const std::string inst_id_str_;
    const qry::Util::Read_mode usage_ ;

 
    const std::shared_ptr<common::App_info> app_info_ptr_;
    const std::vector<std::shared_ptr<const qry::Instance_filter>> filters_;


};
}
}

