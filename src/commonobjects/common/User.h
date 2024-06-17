/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <omm/Persistent_object.h>
#include <common/User_role.h>

namespace arolios {

namespace common {
class User : public virtual omm::Persistent_object {
public:
    /**
     * Constructor
     */
    User();

    /**
     * Destructor
     */
    ~User();
    
    const std::string& identifier () const ;

    User_role app_role () const ;

    
    void identifier (const std::string& p_name) ;

    void app_role (const User_role p_role);
    

    
private:
    
    std::string identifier_;

    User_role app_role_;

};

}
}

