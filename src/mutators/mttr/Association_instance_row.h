/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <mttr/Classifier_instance_row.h>

namespace arolios {



namespace mttr {

class Association_instance_row : public Classifier_instance_row {
public:
    /**
     * Constructor
     */
    Association_instance_row(const std::shared_ptr<const omm::Logical_struct>& p_lo, const std::shared_ptr<const omm::Classifier>& p_prop_csf, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache);

    /**
     * Destructor
     */
    virtual ~Association_instance_row();
    
    virtual void build_field ( Field_maker& p_f_maker, const std::shared_ptr<const Json::Value>& p_data, const std::shared_ptr<const omm::Property_view>& prop_view, const std::string& p_path_name = std::string()) override;
    
    const unsigned long class_instance_row_id() const;
    
    const std::string& class_instance_column () const;

private:
    
    unsigned long class_instance_row_id_;
    std::string class_instance_column_;

};
}
}

