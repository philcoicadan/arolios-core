/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Base_element_update.h>
#include <util/String.h>







using namespace arolios;
using namespace arolios::mttr;

Base_element_update::Base_element_update( const std::string& p_app_id , const Json::Value& p_data_root, const int p_rfu_count) :  elem_id_ (p_app_id) ,
data_root_ (p_data_root), rfu_count_ (p_rfu_count), app_info_ptr_ (common::Singleton<common::App_info>::instance().object()) {

}

Base_element_update::~Base_element_update() {

}


int Base_element_update::new_update_counter() const
{
    return new_upd_count_;
}
