/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Init_retriever.h>
#include <util/String.h>
#include <common/Db_client.h>
#include <common/Singleton.h>
#include <exception/Exceptions.h>



using namespace arolios;
using namespace arolios::qry;

Init_retriever::Init_retriever ( )
    : db_(common::Singleton<common::Db_client>::instance().object()) {
        

}

Init_retriever::~Init_retriever() {

}

std::string Init_retriever::retrieve_param( const std::string& p_key) const {

    

 

    try {

        // retrieve param
        const std::string key = util::String::standardize(p_key);
        const std::string stmt = "SELECT i_value FROM am_init_param WHERE i_key = $1";

        auto f = db_->execSqlAsyncFuture ( stmt, key );

        auto result = f.get();

        if ( result.size() > 0 ) {
            // param found
            auto row = result[0];
            std::string value =  row["i_value"].as<std::string>();
            return value;
        } else {
            std::cout << "WARNING: init parameter " << p_key << " not found" << std::endl;
            return std::string();
        }

       
    } 
    catch ( const exception::Db_exception &e ) {
        std::cerr << "error: " << e.base().what() << std::endl;

        throw e;
    }
}
 
