/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Base_element_delete.h>
#include <util/String.h>







using namespace arolios;
using namespace arolios::mttr;

Base_element_delete::Base_element_delete( const std::string& p_app_id ) : app_info_ptr_ (common::Singleton<common::App_info>::instance().object()), elem_id_ (p_app_id)  {

}

Base_element_delete::~Base_element_delete() {

}




