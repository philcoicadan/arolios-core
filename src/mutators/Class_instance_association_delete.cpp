/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Class_instance_association_delete.h>
#include <omm/Property.h>
#include <omm/Column.h>
#include <mttr/Classifier_instance_row.h>
#include <omm/Class.h>
#include <exception/Exceptions.h>



using namespace arolios;
using namespace arolios::mttr;

Class_instance_association_delete::Class_instance_association_delete( const std::shared_ptr<const omm::Association>& p_csf, const std::string& p_inst_id,  const int p_rfu_count)
: Classifier_instance_delete (p_csf, p_inst_id, p_rfu_count) {

}

Class_instance_association_delete::~Class_instance_association_delete() {

}

void Class_instance_association_delete::execute()
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
        
 
    const std::string csf_id = csf_->id();
    const bool ret_stat = insert_into_trash (std::stoul(csf_id));

    if ( ret_stat ) {

        auto && table_name = lo->full_name();
        auto column = opposite_prop->column();
        if (column == nullptr) {
            std::string error_msg = "member end " + opposite_prop->name() + " of association " + csf_->name() + " is not mapped correctly to a relational object";
            throw exception::Not_found ( error_msg );
        }
        std::string column_name = column->name() + "_trashed_date";
        
        update_for_trash( table_name, column_name,row_id_, csf_id_list_) ;

        return_code_ = qry::Return_code::OK;

    } else {
        return_code_ = qry::Return_code::KO;
    }

}

