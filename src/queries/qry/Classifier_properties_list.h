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
#include <omm/Property_view.h>

namespace arolios {









namespace omm {
 
    class Property;
}

namespace appcfg {
    class Base_classifier_config;
}

namespace qry {


class Classifier_properties_list  : public Base_list_query {
public:
    /**
     * Constructor
     */
    Classifier_properties_list( const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::string& p_context, const std::shared_ptr<const omm::Language>& p_lang
    )  ;


    /**
     * Destructor
     */
    virtual ~Classifier_properties_list();

    
    virtual void execute () override;
    

protected:
     virtual std::string compute_order_by () override;
   
private:
    
    
    std::shared_ptr<const omm::Classifier> csf_;
    qry::Util::Context context_;
    
    void build_property_json (Json::Value& p_jrow, const std::shared_ptr<const omm::Property_view>& p_prop_v,/* const bool p_nesting_part_of_id,*/ const bool p_nesting_is_required, const std::vector<std::shared_ptr<const omm::Property_view>>& p_nesting_prop_view_vec = std::vector<std::shared_ptr<const omm::Property_view>>()) ;

    
    void sort_result (std::vector<std::shared_ptr<const omm::Property_view>>& p_props) ;
    
  
    
};

}
}

