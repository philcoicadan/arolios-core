/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Base_admin_element_read.h>
#include <qry/Util.h>

namespace arolios {



namespace qry {

class App_read : public Base_admin_element_read {
public:
    /**
     * Constructor
     */
    App_read( const std::string& p_inst_id, const qry::Util::Read_mode p_usage, const std::shared_ptr<const omm::Language>& p_lang);

    /**
     * Destructor
     */
    virtual ~App_read();
    
    void execute () override;
    
protected:
    
    
    
private:

};
}
}

