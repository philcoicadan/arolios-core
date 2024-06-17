/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Base_instance_read.h>
#include <qry/Util.h>

namespace arolios {



namespace qry {

template < class T1> class Instance_read : public Base_instance_read {
public:
    /**
     * Constructor
     */
    Instance_read( const std::shared_ptr<const T1>& p_csf, const std::string& p_inst_id, const qry::Util::Read_mode p_usage , const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang);

    /**
     * Destructor
     */
    virtual ~Instance_read();
    
    void execute () override;
    
protected:
    
    std::shared_ptr<const T1> csf_ ;
    
private:

    void build_return_json (const qry::Db_row& p_row, const std::shared_ptr<appcfg::Base_classifier_config>& p_config);

    virtual void build_property_json ( const std::string& p_id_name, const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const T1>& p_csf, const qry::Db_row& p_row, const std::shared_ptr<appcfg::Base_classifier_config>& p_config, Json::Value& p_root_json, const int p_recursive, const std::string& p_column_path, const std::string& p_table_prefix) = 0;
    



};
}
}

