/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_create.h>






using namespace arolios;
using namespace arolios::mttr;

Classifier_instance_create::Classifier_instance_create( const Json::Value& p_data_root,const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache)
:  Base_element_create (p_data_root) , import_cache_( p_import_cache) {


}

Classifier_instance_create::~Classifier_instance_create() {

}


