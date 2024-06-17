/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <common/Db_client.h>

namespace arolios {






namespace qry {

class Init_retriever {
public:

    /**
     * Constructor
     */
    Init_retriever();

    /**
     * Destructor
     */
    ~Init_retriever();

    std::string retrieve_param ( const std::string& p_key) const; ;



private:

        const std::shared_ptr<common::Db_client> db_;
};

}
}

