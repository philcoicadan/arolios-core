/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <common/User_role.h>
#include <memory>

namespace arolios {



namespace common {

  class User;
    
class UAC_info  {
public:
    
    
    /**
     * Constructor
     */
    UAC_info();
    


     /**
     * Destructor
     */
    virtual ~UAC_info();   

       
    virtual bool check_role (const common::User_role p_user_role, const common::User_role p_required_role) const ;
    
    virtual std::shared_ptr<common::User> super_user () const;
    
    virtual std::string standardize_role (const std::string& p_user_role) const ;
    
    virtual common::User_role role_cast (const std::string& p_role_str) const ;
    virtual const std::string& string_cast ( const common::User_role p_role) const; 

    virtual std::shared_ptr<common::User> retrieve (const std::string& p_identifier, const std::string& p_pwd) const;

protected:



  std::shared_ptr<common::User> super_user_;
    
private:

      

    
};

}
}

