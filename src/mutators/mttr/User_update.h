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

namespace arolios {








namespace mttr {
class User_update : public Base_element_update {
public:
    /**
     * Constructor
     */
    User_update( const std::string& p_user_id , const Json::Value& p_data_root, const int p_rfu_count = 0);

    /**
     * Destructor
     */
    virtual ~User_update();
 
    virtual void execute () override;
    

private:

};
}
}

