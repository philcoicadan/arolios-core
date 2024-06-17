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
#include <mttr/Base_element_delete.h>

namespace arolios {







namespace omm {
    class Classifier;
    class Property;
}

namespace mttr {
class Classifier_instance_delete : public Base_element_delete {
public:
    /**
     * Constructor
     */
    Classifier_instance_delete( const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count = 0);

    /**
     * Destructor
     */
    virtual ~Classifier_instance_delete();
 
    virtual void execute () override;
    
    
    int new_update_counter () const;

protected:
    bool update_for_trash ( const std::string& p_table_name, const std::string& p_field_name, const unsigned long p_row_id, const std::string& p_csf_list=std::string() ) ;
    bool insert_into_trash ( const unsigned int p_id );

    std::shared_ptr<const omm::Classifier> csf_ ;

    unsigned long row_id_;
    unsigned int csf_id_;

    int rfu_count_;
    int new_upd_count_;
    std::string csf_id_list_ ;
    
private:
    
    bool trash_association_in_instance_row( const std::shared_ptr<const omm::Property>& p_prop);
    std::pair<bool, bool> trash_instance_and_association_in_opposite_rows (const std::shared_ptr<const omm::Property>& p_prop) ;
    bool trash_instance() ;
    

};
}
}

