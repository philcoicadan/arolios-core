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
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {








namespace mttr {
class App_user_delete: public Base_element_delete{
public:
    /**
     * Constructor
     */
    App_user_delete(  const std::string& p_app_user_id );

    /**
     * Destructor
     */
    virtual ~App_user_delete();
 
    virtual void execute () override;
    
       
private:

   
};
}
}

