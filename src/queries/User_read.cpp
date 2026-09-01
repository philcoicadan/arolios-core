/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/User_read.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <util/Native_type.h>
#include <qry/Util.h>







using namespace arolios;
using namespace arolios::qry;

User_read::User_read( const std::string&  p_inst_id, const qry::Util::Read_mode p_usage ,  const std::shared_ptr<const omm::Language>& p_lang) :Base_admin_element_read ( p_inst_id, p_usage, p_lang)
{

}

User_read::~User_read() {

}
 
void User_read::execute () {
    


    inst_id_str_ = util::String::standardize(inst_id_str_);
         
    auto f = db_->execSqlAsyncFuture("SELECT * FROM am_user WHERE u_identifier = $1", inst_id_str_);

    try {
        auto result = f.get();

        if (result.size() > 0) {
            auto row = result[0];
            return_json_["identifier"] = Json::Value ( row["u_identifier"].as<std::string>()) ;
            return_json_["role"] = Json::Value (row["u_role"].as<std::string>() );
            return_json_["lc_status"] = Json::Value (row["u_lc"].as<std::string>() );

            
            if (usage_ ==  qry::Util::Read_mode::read_for_update ) {
                auto && field3 = row["upd_count"];
                util::Native_type native_type = util::Native_type::integer;
                return_json_["upd_count"] = qry::Util::native_to_json ( native_type, field3 ) ; ;
            }
            return_code_ = qry::Return_code::OK;

        }        
        
        else {
            std::string msg ("user identifier " + inst_id_str_ + " not found");
             
            throw exception::Not_found (msg);

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}


    
      


   

}



