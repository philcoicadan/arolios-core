/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Instance_read.h>
#include <omm/Class.h>
#include <appcfg/Base_classifier_config.h>

namespace arolios {



namespace qry {

class Class_instance_read : public Instance_read<omm::Class> {
public:
    /**
     * Constructor
     */
    Class_instance_read(const std::shared_ptr<const omm::Class>& p_csf, const std::string& p_inst_id, const qry::Util::Read_mode p_usage , const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang);

    /**
     * Destructor
     */
    ~Class_instance_read();

    
protected:
    
    virtual void build_ids_json ( const qry::Db_row& p_row, Json::Value& p_root_json ) override;
    virtual const std::string get_classifier_ids () const override;

private:
    virtual void build_property_json ( const std::string& p_id_name, const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const omm::Class>& p_csf, const qry::Db_row& p_row, const std::shared_ptr<appcfg::Base_classifier_config>& p_config, Json::Value& p_root_json, const int p_recursive, const std::string& p_column_path, const std::string& p_table_prefix) override ;
    
    
};
}
}

