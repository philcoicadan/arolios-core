/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <omm/Package.h>
#include <omm/Column_constraint.h>
#include <omm/Logical_field.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <common/Db_client.h>

namespace arolios {



namespace common {
    
class Object_model_info;
}

namespace qry {

class Model_retriever {
public:
  
    /**
     * Constructor
     */
    Model_retriever() ;

    /**
     * Destructor
     */
    ~Model_retriever();
    
    void retrieve_init(const std::string& p_root_pk_id, const std::string& p_std_pk_id) const ;
    
    void retrieve_user_domains () const ;

    void retrieve_standard_types () const;
    
    void retrieve_classifiers ( const std::shared_ptr<omm::Package>& p_pk ) const;
    
    void retrieve_properties ( const std::shared_ptr<omm::Package>& p_pk ) const;

    void retrieve_enumeration_values ( const std::shared_ptr<omm::Package>& p_pk ) const;
    
    void retrieve_logical_fields (const std::shared_ptr<omm::Package>& p_pk )  const;
    
    void retrieve_field_constraints (const std::shared_ptr<omm::Package>& p_pk )  const;
 
    void retrieve_identifiers (const std::shared_ptr<omm::Package>& p_pk )  const;
    

    
    omm::Column_constraint::Type convert_column_constraint_type (const std::string& p_type)  const;

    
    
    
private:
     omm::Logical_field::Type convert_logical_field_type (const std::string& p_type)  const;

 
    const std::shared_ptr<common::Db_client> db_;
    const std::shared_ptr<common::Object_model_info> common_obj_;
 };

}
}

