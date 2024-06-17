/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <json/json.h>
#include <qry/Return_code.h>
#include <qry/Base_query.h>
#include <common/User.h>

namespace arolios {








namespace mttr {
class User_change_password : public qry::Base_query {
public:
    /**
     * Constructor
     */
    User_change_password( const std::shared_ptr<common::User>& p_user, const Json::Value& p_data_root);

    /**
     * Destructor
     */
    virtual ~User_change_password();
 
    virtual void execute () override;
    
    
    int new_update_counter () const;

    
private:

    const Json::Value& data_root_;
    const std::shared_ptr<common::User>& user_;

    int new_upd_count_;

};
}
}

