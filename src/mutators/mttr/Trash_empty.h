/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <qry/Base_query.h>
#include <omm/Classifier.h>
#include <omm/Association.h>
#include <omm/Table.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <qry/Instance_filter.h>

namespace arolios {




namespace common {
    class App_info;
}

namespace mttr {
class Trash_empty : public qry::Base_query {
public:
    /**
     * Constructor
     */
    Trash_empty();

    /**
     * Destructor
     */
    virtual ~Trash_empty();
    
    void execute ();
    
private:
    
void eliminate_instances ( const Json::Value& p_json, const std::shared_ptr<const omm::Classifier>& p_csf, const std::shared_ptr<const omm::Association>& p_assoc, const std::shared_ptr<omm::Table>& p_elem_lo ) const ;
    
    const std::shared_ptr<common::App_info> app_info_ptr_;
    const  std::shared_ptr<common::Object_model_info> om_ptr_;
    std::vector<std::shared_ptr<const qry::Instance_filter>> filters_;

};

}
}

