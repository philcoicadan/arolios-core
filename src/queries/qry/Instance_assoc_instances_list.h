/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Classifier_instances_list.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {



namespace omm {
    class Class;
    class Property;
}

namespace qry {
class Instance_assoc_instances_list :  public Classifier_instances_list<omm::Association> {
public:
    /**
     * Constructor
     */
    Instance_assoc_instances_list( const std::shared_ptr<const omm::Class>& p_csf, const std::string& p_inst_id, const std::shared_ptr<const omm::Property>& p_member_end_property, const std::string& p_prop_id_name, const std::shared_ptr<const omm::Association>& p_assoc, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::string& p_prop_list, const std::shared_ptr<const omm::Language>& p_lang
    );

    /**
     * Destructor
     */
    virtual ~Instance_assoc_instances_list();
    
    virtual void execute () override;
    

private: 
    
    std::shared_ptr<const omm::Property> member_end_property_    ;
    
    std::string inst_id_str_;
    
    std::string prop_id_name_;
    
    std::shared_ptr<const omm::Class> cls_;

    



};

}
}

