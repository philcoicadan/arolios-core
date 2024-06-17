/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <common/User.h>




using namespace arolios;
using namespace arolios::common;

User::User() {

}

User::~User() {

}

const std::string& User::identifier () const {
    return identifier_;
}

User_role User::app_role () const {
    return app_role_;
}
void User::identifier (const std::string& p_name) {
    identifier_ = p_name;
}


void User::app_role (const User_role p_role) {
    app_role_ = p_role;
}

