/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <common/Db_client.h>
#include <memory>
#include <string>
#include <json/json.h>
#include <qry/Base_list_query.h>

namespace arolios {




namespace appcfg {
    class Base_classifier_config;
}

namespace omm {
    class Classifier;
}



namespace qry {
       class Instance_filter;


class Trash_list  : public Base_list_query {
 
public:
    /**
     * Constructor
     */
    Trash_list( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang);


    /**
     * Destructor
     */
    virtual ~Trash_list();

    
    virtual void execute () override;
    

protected:
   
    
protected:
    
    virtual std::string compute_order_by () override;

private:
    
    int concatenate_json (Json::Value& p_cumul_json, const Json::Value& p_json, const std::shared_ptr<const omm::Classifier>& p_csf);
    
    void paginate_json (const Json::Value& p_cumul_json, long p_total, Json::Value& p_page_json) const ;

 
    std::vector<std::shared_ptr<const qry::Instance_filter>> filters_;
    const Json::StreamWriterBuilder wbuilder_;
};

}
}

