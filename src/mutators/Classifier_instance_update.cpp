/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_update.h>
#include <omm/Classifier.h>
#include <util/String.h>
#include <mttr/Classifier_instance_row.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>






using namespace arolios;
using namespace arolios::mttr ;

Classifier_instance_update::Classifier_instance_update( const Json::Value& p_data_root, const std::string& p_inst_id, const int p_rfu_count, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache) 
: Base_element_update (p_inst_id, p_data_root, p_rfu_count), import_cache_(p_import_cache) {

   
}

Classifier_instance_update::~Classifier_instance_update() {

}
