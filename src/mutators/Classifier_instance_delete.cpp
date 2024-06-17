/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_delete.h>
#include <mttr/Base_element_delete.h>
#include <omm/Association.h>
#include <omm/Table.h>
#include <omm/Column.h>
#include <util/String.h>
#include <appcfg/Base_classifier_config.h>
#include <qry/Class_instance_read.h>
#include <exception/Exceptions.h>







using namespace arolios;
using namespace arolios::mttr ;

Classifier_instance_delete::Classifier_instance_delete( const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count) 
: Base_element_delete(p_inst_id), csf_ (p_csf) {
    rfu_count_ = p_rfu_count;
    
    csf_id_list_ = csf_->classifier_id_with_specializations();

    std::tie (csf_id_, row_id_ ) = qry::Util::split_instance_id(p_inst_id);
}

Classifier_instance_delete::~Classifier_instance_delete() {

}


int Classifier_instance_delete::new_update_counter () const {
    return new_upd_count_;
}

void Classifier_instance_delete::execute () {

    auto lo = csf_->logical_struct();
    if ( !lo ) {
        std::string error_msg = "classifier " + csf_->name() + " cannot accept values";
        throw exception::Not_found ( error_msg );
    }

    if ( !lo->is_table() ) {
        std::string error_msg = "classifier " + csf_->name() + " is not mapped correctly to a relational object";
        throw exception::Not_found ( error_msg );
    }

    

    //update the trash collection
    const bool ret_stat = insert_into_trash ( std::stoul( csf_->id()) );

    if ( ret_stat ) {

        bool has_assoc = false;


        auto cls = std::dynamic_pointer_cast<const omm::Class> ( csf_ );

        if ( cls ) { // instance is a class instance, lookup associations from properties for trash


            const auto & props_map = csf_->all_elements_as_property();
            Json::Value props_json = Json::Value ( Json::objectValue );

            return_code_ = qry::Return_code::OK;

            for ( auto [ name, prop] : props_map ) {
                const auto assoc = prop->association() ;
                if ( assoc ) {
                    if ( !has_assoc ) {
                        // Need to retrieve all fields of the instance for processing its associations after trashing
                                                //add filter to select only active (non-deleted) instance
                        const std::shared_ptr<const qry::Instance_filter> td_filter = std::make_shared<qry::Instance_filter>("trashed_date", "=", "'epoch'") ;
                        const std::vector<std::shared_ptr<const qry::Instance_filter>> filters = { td_filter };
                        // read operation checks instance classifier => not need to do after
                        auto read_inst =  qry::Class_instance_read ( cls, elem_id_, qry::Util::Read_mode::read_for_update ,filters,  app_info_ptr_->default_language());

                        read_inst.execute();
                        if ( read_inst.return_code() == qry::Return_code::OK ) {
                            props_json = read_inst.return_json();
                        } else {
                            const std::string msg = "update for deleting associations linked to inst" + elem_id_ + " of classifier " + csf_->name() +  " failed";
                            std::cout << "WARNING: " << msg << std::endl;
                            break;
                        }
                        has_assoc =true;
                    }
                    bool assoc_deleted = false;
                    if ( prop->max_occurs() == 1 ) { //cardinality 0,1 or 1,1

                        
                        if ( prop->column() ) {
                            // case 1: prop associated to a table column - set trashed_date associated with this column if target of the association not null
                            if (props_json.isMember ("properties")) {
                                Json::Value & properties_json = props_json["properties"];                          
                                if ( properties_json.isMember ( prop->name() )) {
                                    bool ret = trash_association_in_instance_row ( prop ) ;
                                    if ( !ret ) continue;
                                    assoc_deleted = true;
                                }
                            }
                        } else { //  case 2 : opposite member with cardinality = 0,1 or 1,1 and its table owning fields implementing the association,  association must be trashed and, if opposite cardinality   = 1,1 or isComposition,  instance too in trash (recursive processing)

                            std::pair<bool,bool> ret  = trash_instance_and_association_in_opposite_rows ( prop );
                            if ( ! ret.first ) continue;
                            assoc_deleted = ret.second;


                        }

                    } else { // cardinality 0,N or 1,N
                        if ( prop->column() ) { // case 3: opposite cardinality 0,N or 1,N, consider the table of the association
                            const auto table = assoc->managing_table();
                            const std::string table_name = table->full_name();

                            const auto members = assoc->member_ends();
                            std::shared_ptr<omm::Property> opposite;
                            for ( int i=0; i<2; ++i ) {
                                opposite = (*members)[i];
                                if ( opposite != prop ) break;
                            }
                            if ( opposite->column() ) {

                                const std::string column_name = opposite->column()->name();
                                const std::string stmt = "SELECT id , csf_id, upd_count from " + table_name + " WHERE " + column_name + " = " + std::to_string(row_id_) + " AND trashed_date = 'epoch'";
                                auto f = db_->execSqlAsyncFuture ( stmt );

                                auto result = f.get();
                                if ( result.size() > 0 )  {

                                    for ( auto row : result ) {

                                        const std::string instance_id = qry::Util::build_instance_id(row); 
                                        Classifier_instance_delete assoc_del ( assoc, instance_id, rfu_count_);
                                        assoc_del.execute();
                                        
                                        if (assoc_del.return_code() == qry::Return_code::OK) {
                                            assoc_deleted= true;
                                        }

                                    }
                                    
                                } else {
                                    const std::string msg = "No association instance linked to property " + prop->name() + " of classifier " + csf_->name() +  " to be deleted";
                                    std::cout << "WARNING: " << msg<< std::endl;
                                }
                                    
                            } else {
                                const std::string msg = "update " + table_name + " for deleting association linked to property " + prop->name() + " of classifier " + csf_->name() +  " failed";
                                 
                                throw exception::Not_found ( msg );
                            }

                        } else { // case 4: opposite cardinality is 0,1 or 1,1 - idem case 2 with N opposite rows, associations in trash and, if 1,1 or isComposition, instances in trash ( recursive processing)
                            auto [first, second] ( trash_instance_and_association_in_opposite_rows ( prop));
                            assoc_deleted = second;
                        }
                    }

                    if ( assoc_deleted ) {
                        const bool ret_stat = insert_into_trash ( std::stoul(assoc->id()) );
                        if ( !ret_stat ) continue;
                    }
                }
            }
        }

        const bool ret = trash_instance () ;  // update instance for trash
        if ( !ret ) {
            const std::string msg = "update  for deleting instance " + elem_id_ + " of classifier " + csf_->name() +  " failed";
             
            return_code_ = qry::Return_code::KO;
        }


    } else {
        const std::string msg = "update  trash for classifier " + csf_->name() +  " failed";
         
    }
}

bool Classifier_instance_delete::trash_association_in_instance_row( const std::shared_ptr<const omm::Property>& p_prop) {
         
    bool ret_status = false;
    const std::string table_name = csf_->table()->full_name();                            
    const std::string column_name = p_prop->column()->name();
    const std::string fields_stmt = "UPDATE " + table_name + " SET ( " + column_name + "_trashed_date ) ";
    const std::string values_stmt = "= ROW ( now()) ";

    
    std::string stmt = fields_stmt + values_stmt + " WHERE id="+ std::to_string(row_id_) + " RETURNING id";
    //NOT NEED to filter with trashed_date because can be already done if it is a recursive step column_name + "_trashed_date = 'epoch' AND
    
    

    auto f = db_->execSqlAsyncFuture ( stmt );

    auto result = f.get();
    if ( !(result.size() > 0) ) {
        return_code_ = qry::Return_code::KO;
         
        ret_status = false;
    } else {
        ret_status = true;
    }
    return ret_status;
}

std::pair<bool, bool> Classifier_instance_delete::trash_instance_and_association_in_opposite_rows (const std::shared_ptr<const omm::Property>& p_prop) {
    // association implemented in the table of the opposite prop
    
    bool assoc_deleted= false;
    bool ret_status = true;
 
    const auto assoc = p_prop->association();
    const auto table = assoc->managing_table();
    const std::string table_name = table->full_name();

    const auto members = assoc->member_ends();
    std::shared_ptr<omm::Property> opposite;
    for ( int i=0; i<2; ++i ) {
        opposite = (*members)[i];
        if ( opposite != p_prop ) break;
    }
    if ( opposite->column() ) {
        const std::string column_name = opposite->column()->name();
        const std::string stmt = "SELECT id, csf_id, upd_count FROM " + table_name + " WHERE " + column_name + " = " + std::to_string(row_id_) + " AND " + column_name + "_trashed_date = 'epoch'";
        auto f = db_->execSqlAsyncFuture ( stmt );

        auto result = f.get();
        if ( result.size() > 0 ) { // instance(s) referencing the deleted instance through the association exist(s)
            bool recursive = (opposite->min_occurs() > 0 ) || (p_prop->aggregation_kind() == omm::Aggregation_kind::composite) ;
            for ( auto row : result ) {
                
                const unsigned long inst_row_id = row["id"].as<unsigned long>();
                const unsigned int inst_csf_id = row["csf_id"].as<unsigned int>();
                const int upd_count = row["upd_count"].as<int>();
                
                //association in trash
                const std::string field_name = column_name + "_trashed_date";
                
                const bool ret_stat = update_for_trash (table_name, field_name, inst_row_id);
                if (!ret_stat) {
                    ret_status = false;
                    continue;
                } else {
                    assoc_deleted = true;
                }
                
                if (  recursive ) {
                    const std::string inst_id = qry::Util::build_instance_id(row);
                    Classifier_instance_delete opposite_instance ( opposite->classifier(), inst_id, upd_count); // instance in trash
                    opposite_instance.execute ();
                    if (opposite_instance.return_code() == qry::Return_code::KO ) {
                        ret_status = false ;
                        continue;
                    }

                }    
                                                                                                                
            }

        }

    } else {
        const std::string msg = "update " + table_name + " for deleting association linked to property " + p_prop->name() + " of classifier " + csf_->name() +  " failed";
         
        throw exception::Not_found ( msg );
    }
    return  {ret_status, assoc_deleted};
}


bool Classifier_instance_delete::trash_instance() {

    bool ret_status = false;
    auto lo = csf_->logical_struct();
    auto && table_name = lo->full_name();

    const std::string fields_stmt = "UPDATE " + table_name + " SET ( upd_count, trashed_date ) ";
    const std::string values_stmt = "= ROW ( upd_count+1, now()) ";


    std::string stmt = fields_stmt + values_stmt + " WHERE trashed_date = 'epoch' AND id="+ std::to_string(row_id_) + " AND  ( " + csf_id_list_ + " )";

    stmt += " RETURNING upd_count";


    auto f = db_->execSqlAsyncFuture ( stmt );

    auto result = f.get();

    if ( result.size() > 0 ) {
        auto && row = result[0];
        new_upd_count_ = row["upd_count"].as<int>(); ;
        return_code_ = qry::Return_code::OK;
        ret_status = true;
    }

    return ret_status;
}

bool Classifier_instance_delete::update_for_trash (  const std::string& p_table_name, const std::string& p_field_name, const unsigned long p_row_id, const std::string& p_csf_id_list) {

    bool ret_status = false;
    const std::string fields_stmt = "UPDATE " + p_table_name + " SET ( " + p_field_name + " ) ";
    const std::string values_stmt = "= ROW ( now()) ";

     
    
    std::string stmt = fields_stmt + values_stmt + " WHERE " + p_field_name + " = 'epoch' AND id="+ std::to_string(p_row_id);
    if (! p_csf_id_list.empty()) {
        stmt += " AND ( "+ p_csf_id_list + " ) ";
    }
    stmt += " RETURNING id";


    auto f = db_->execSqlAsyncFuture ( stmt );

    auto result = f.get();
    if ( ! ( result.size() > 0 ) ) {
        return_code_ = qry::Return_code::KO;
         
        ret_status = false;
    } else {
        ret_status = true;
    }

    return ret_status;
}

bool Classifier_instance_delete::insert_into_trash ( const unsigned int p_id ) {

    bool ret_status = false;
    const std::string stmt = "INSERT INTO am_trash ( csf_id ) VALUES ( $1 ) ON CONFLICT ( csf_id ) DO UPDATE SET csf_id = $1 RETURNING id" ;
    auto f = db_->execSqlAsyncFuture ( stmt, p_id ) ;
    auto result = f.get();
    if ( ! ( result.size() > 0 ) ) {
        return_code_ = qry::Return_code::KO;
         
        ret_status = false;
    } else {
        ret_status = true;
    }
    return ret_status;
}
