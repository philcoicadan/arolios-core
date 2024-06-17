/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Base_admin_element_read.h>






using namespace arolios;
using namespace arolios::qry;

Base_admin_element_read::Base_admin_element_read( const std::string& p_inst_id, const qry::Util::Read_mode p_usage, const std::shared_ptr<const omm::Language>& p_lang )
: Base_query(p_lang), inst_id_str_(p_inst_id), usage_ (p_usage) {


}

Base_admin_element_read::~Base_admin_element_read() {

}
