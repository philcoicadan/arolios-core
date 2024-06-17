/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <stdexcept>
#include <drogon/orm/Exception.h>

namespace arolios {

namespace exception {
class Base_exception {
public:
    /**
     * Destructor
     */
    virtual ~Base_exception() noexcept {};
    
    
    virtual const std::exception &base() const noexcept
    {
        static std::exception except;
        return except;
    }

};

class Failure : public Base_exception, public std::runtime_error
{
    virtual const std::exception &base() const noexcept override
    {
        return *this;
    }

  public:
    explicit Failure(const std::string& p_what);
};

class Not_yet_implemented : public Failure
{


  public:
    explicit Not_yet_implemented(const std::string& p_what);
};

class Model_error : public Base_exception, public std::invalid_argument
{
    virtual const std::exception &base() const noexcept override
    {
        return *this;
    }

  public:
    explicit Model_error(const std::string& p_what);
};

class Data_error : public Base_exception, public std::invalid_argument
{
    virtual const std::exception &base() const noexcept override
    {
        return *this;
    }

  public:
    explicit Data_error(const std::string& p_what);
};

class Not_found : public Base_exception, public std::invalid_argument
{
    virtual const std::exception &base() const noexcept override
    {
        return *this;
    }

  public:
    explicit Not_found(const std::string& p_what);
};

class Sql_error : public  drogon::orm::SqlError
{
    public:
    explicit Sql_error(const std::string& p_what);
}; 

using Db_exception = drogon::orm::DrogonDbException ;

}
}

