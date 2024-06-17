/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <json/json.h>
#include <mttr/Base_element_update.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {







namespace mttr {
class App_update : public Base_element_update {
public:
    /**
     * Constructor
     */
    App_update( const std::string& p_app_id , const Json::Value& p_data_root, const int p_rfu_count = 0);

    /**
     * Destructor
     */
    virtual ~App_update();
 
    virtual void execute () override;
 

    
private:


};
}
}

