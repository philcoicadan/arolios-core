/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <json/json.h>
#include <common/Db_client.h>
#include <qry/Return_code.h>
#include <qry/Base_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {




namespace omm {
    class Classifier;
    class Property;
}

namespace mttr {
class Classifier_instance_eliminate : public qry::Base_query {
public:
    /**
     * Constructor
     */
    Classifier_instance_eliminate( const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count = 0);

    /**
     * Destructor
     */
    virtual ~Classifier_instance_eliminate();
 
    virtual void execute () override;
 

protected:
    bool update_for_eliminate ( const std::string& p_table_name, const std::string& p_field_name, const unsigned long p_inst_id , const std::string& p_csf_id_list=std::string() ) ;

    
    const std::shared_ptr<common::App_info> app_info_ptr_;
    std::shared_ptr<const omm::Classifier> csf_ ;

    unsigned long row_id_;
    unsigned int csf_id_;

    std::string inst_id_str_;
    int rfu_count_;

    std::string csf_id_list_ ;
private:
    
    std::pair<bool, bool> eliminate_instance_and_association_in_opposite_rows (const std::shared_ptr<const omm::Property>& p_prop) ;
    bool eliminate_instance() ;
    

};
}
}

