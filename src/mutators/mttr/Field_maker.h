/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <omm/Visitor.h>
#include <mttr/Classifier_instance_row.h>
#include <memory>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <json/json.h>

namespace arolios {


namespace mttr {
class Field_maker : public omm::Visitor {
public:
    /**
     * Constructor
     */
    Field_maker( Classifier_instance_row& p_row, const std::string& p_path_name,  const std::shared_ptr<common::Object_model_info>& p_mdl_info);

    /**
     * Destructor
     */
    virtual ~Field_maker();
    
    

		virtual void visit (const omm::Class& p_elem) override;
		virtual void visit (const omm::Enumeration& p_elem)  override;
		virtual void visit (const omm::Primitive_type& p_elem)  override;
		virtual void visit (const omm::Composite_type& p_elem)  override;
        
        void data (const std::shared_ptr<const Json::Value>& p_data);
        void type (const std::shared_ptr<const omm::Classifier>& p_type);

        void logical_field (const std::shared_ptr<const omm::Logical_field>& p_lf);
        void property_view (const std::shared_ptr<const omm::Property_view>& p_prop_v) ;

    
private:

    void check_instance_value (const std::string& p_value, const omm::Class& p_elem) const ;
    
    mttr::Classifier_instance_row& row_;
    std::shared_ptr<const omm::Logical_field> lf_;
    std::shared_ptr<const omm::Classifier> type_;
    std::shared_ptr<const omm::Property_view> prop_v_;
    std::string path_name_;
    std::shared_ptr<const Json::Value> data_;
    const std::shared_ptr<common::Object_model_info>  mdl_info_;
    
    
        

};

}
}

