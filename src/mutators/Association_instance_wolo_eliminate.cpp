/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Association_instance_wolo_eliminate.h>
#include <omm/Property.h>
#include <omm/Column.h>
#include <omm/Class.h>
#include <mttr/Classifier_instance_row.h>
#include <common/Db_client.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>




using namespace arolios;
using namespace arolios::mttr;

Association_instance_wolo_eliminate::Association_instance_wolo_eliminate( const std::shared_ptr<const omm::Association>& p_csf, const std::string& p_inst_id,  const int p_rfu_count)
: Classifier_instance_eliminate (p_csf, p_inst_id, p_rfu_count) {

}

Association_instance_wolo_eliminate::~Association_instance_wolo_eliminate() {

}

void Association_instance_wolo_eliminate::execute()
{
    auto assoc = std::dynamic_pointer_cast<const omm::Association>(csf_);
    int k= 0;
    auto members = assoc->member_ends();
    while (  k < 2 ) {
        if ((*members)[k]->has_sub_field()) {
            break;
        }
        ++k;
    }

    auto opposite_prop = (*members) [1-k];

    
    auto mtc = assoc->managing_table_class();
    
        
    std::shared_ptr<omm::Logical_struct> lo = (mtc) ? mtc->logical_struct() : nullptr;
    if (!lo) {
       std::string error_msg = "classifier " + csf_->name() + " cannot accept values";
        throw exception::Not_found ( error_msg );
    }
    
    if ( !lo->is_table() ) {
        std::string error_msg = "classifier " + csf_->name() + " is not mapped correctly to a relational object";
        throw exception::Not_found ( error_msg );
    }
      
  
        auto && table_name = lo->full_name();
        auto column = opposite_prop->column();
        if (column == nullptr) {
            std::string error_msg = "member end " + opposite_prop->name() + " of association " + csf_->name() + " is not mapped correctly to a relational object";
            throw exception::Not_found ( error_msg );
        }
        
        update_for_eliminate( table_name, column->name(),row_id_, csf_id_list_) ;

        return_code_ = qry::Return_code::OK;


}

