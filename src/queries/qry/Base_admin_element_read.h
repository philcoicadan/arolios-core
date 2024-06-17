/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Util.h>
#include <qry/Base_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {


namespace omm {
    class Property;
    class Classifier;
}

namespace qry {

class Base_admin_element_read : public Base_query {
public:
    /**
     * Constructor
     */
    Base_admin_element_read( const std::string& p_inst_id, const qry::Util::Read_mode p_usage , const std::shared_ptr<const omm::Language>& p_lang);

    /**
     * Destructor
     */
    virtual ~Base_admin_element_read();
    
    
protected:
   std::string inst_id_str_;
    const qry::Util::Read_mode usage_ ;


};
}
}

