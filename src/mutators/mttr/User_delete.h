/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <json/json.h>
#include <mttr/Base_element_delete.h>

namespace arolios {









namespace mttr {
class User_delete: public Base_element_delete {
public:
    /**
     * Constructor
     */
    User_delete( const std::string& p_user_id );

    /**
     * Destructor
     */
    virtual ~User_delete();
 
    virtual void execute () override;
    
       
private:

   
};
}
}

