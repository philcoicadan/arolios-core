/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <exception/Exceptions.h>
#include <iostream>
#include <drogon/orm/Exception.h>


using namespace arolios;
using namespace arolios::exception;

Failure::Failure(const std::string & p_what) : std::runtime_error(p_what)
{
    std::cerr << "ERROR: " << p_what  << std::endl;
}

Input_error::Input_error(const std::string & p_what) : std::invalid_argument(p_what)
{
    std::cerr << "ERROR: " << p_what << std::endl;
}

Model_error::Model_error(const std::string & p_what) : Input_error(p_what)
{

}

Data_error::Data_error(const std::string & p_what) : Input_error(p_what)
{

}

Not_found::Not_found (const std::string & p_what) : Input_error(p_what)
{

}

Not_yet_implemented::Not_yet_implemented(const std::string & p_what) : Failure(p_what) {
}

Sql_error::Sql_error (const std::string & p_what) : drogon::orm::SqlError(p_what) {
}
