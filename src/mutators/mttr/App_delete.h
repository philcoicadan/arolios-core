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

//#include <common/Db_client.h>




namespace mttr {
class App_delete: public Base_element_delete {
public:
    /**
     * Constructor
     */
    App_delete( const std::string& p_app_id );

    /**
     * Destructor
     */
    virtual ~App_delete();
 
    virtual void execute () override;
    
       
private:

   
};
}
}

