/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Base_element_create.h>






using namespace arolios;
using namespace arolios::mttr;

Base_element_create::Base_element_create(const Json::Value& p_data_root) :
data_root_ (p_data_root), app_info_ptr_ (common::Singleton<common::App_info>::instance().object()) {

}

Base_element_create::~Base_element_create(){

}

