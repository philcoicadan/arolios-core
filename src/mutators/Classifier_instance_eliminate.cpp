/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Classifier_instance_eliminate.h>
#include <omm/Association.h>
#include <omm/Table.h>
#include <omm/Column.h>
#include <util/String.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <qry/Class_instance_read.h>
#include <exception/Exceptions.h>





using namespace arolios;
using namespace arolios::mttr ;

Classifier_instance_eliminate::Classifier_instance_eliminate( const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count) 
: app_info_ptr_(common::Singleton<common:: App_info>::instance().object()), csf_ (p_csf), inst_id_str_(p_inst_id) {
    
    rfu_count_ = p_rfu_count;
        
    csf_id_list_ = csf_->classifier_id_with_specializations();
    
    std::tie (csf_id_, row_id_) = qry::Util::split_instance_id(p_inst_id);

}

Classifier_instance_eliminate::~Classifier_instance_eliminate() {

}


void Classifier_instance_eliminate::execute () {

    auto lo = csf_->logical_struct();
    if ( !lo ) {
        std::string error_msg = "classifier " + csf_->name() + " cannot accept values";
        throw exception::Not_found ( error_msg );
    }

    if ( !lo->is_table() ) {
        std::string error_msg = "classifier " + csf_->name() + " is not mapped correctly to a relational object";
        throw exception::Not_found ( error_msg );
    }

 
        bool has_assoc = false;


        auto cls = std::dynamic_pointer_cast<const omm::Class> ( csf_ );

        if ( cls ) { // instance is a class instance, lookup associations from properties for eliminate


            const auto & props_map = csf_->all_elements_as_property();
            Json::Value props_json = Json::Value ( Json::objectValue );

            return_code_ = qry::Return_code::OK;


            for ( auto [ name, prop] : props_map ) {
                const auto assoc = prop->association() ;
                if ( assoc ) {
                    if ( !has_assoc ) {
                        // Need to retrieve all fields of the instance for processing its associations 
                                                //add filter to select only deleted instance
                        const std::shared_ptr<const qry::Instance_filter> td_filter = std::make_shared<qry::Instance_filter>("trashed_date", "<>", "'epoch'") ;
                        const std::vector<std::shared_ptr<const qry::Instance_filter>> filters = { td_filter };
                        auto read_inst =  qry::Class_instance_read ( cls, inst_id_str_, qry::Util::Read_mode::read_for_update , filters, app_info_ptr_->default_language());

                        read_inst.execute();
                        if ( read_inst.return_code() == qry::Return_code::OK ) {
                            props_json = read_inst.return_json();
                        } else {
                            const std::string msg = "update for purging associations linked to inst" + inst_id_str_ + " of classifier " + csf_->name() +  " failed";
                            std::cout << "WARNING: " << msg<< std::endl;
                            break;
                        }
                        has_assoc =true;
                    }
                    bool assoc_eliminated = false;
                    if ( prop->max_occurs() == 1 ) { //cardinality 0,1 or 1,1

                        
                        if ( prop->column() ) {
                            // case 1: prop associated to a table column - the association will be eliminated with the class instance , nothing to do

                           
                        } else { //  case 2 : opposite member with cardinality = 0,1 or 1,1 and its table owning fields implementing the association,  association must be eliminated and, if opposite cardinality   = 1,1 or isComposition,  instance too in trash (recursive processing) to be eliminated

                            std::pair<bool,bool> ret  = eliminate_instance_and_association_in_opposite_rows ( prop );
                            if ( ! ret.first ) continue;
                            assoc_eliminated = ret.second;


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

                                const std::string stmt = "SELECT id , csf_id, upd_count FROM " + table_name + " WHERE " + column_name + " = " + std::to_string(row_id_) + " AND trashed_date <> 'epoch'";
                                auto f = db_->execSqlAsyncFuture ( stmt );

                                auto result = f.get();
                                if ( result.size() > 0 )  {

                                    for ( auto row : result ) {

                                        const unsigned long inst_row_id = row["id"].as<unsigned long>();
                                        const std::string inst_id = qry::Util::build_instance_id(row);
                                        
                                        Classifier_instance_eliminate assoc_eliminate (assoc, inst_id, rfu_count_);
                                        assoc_eliminate.execute();
                                        
                                        if (assoc_eliminate.return_code() == qry::Return_code::OK) {
                                            assoc_eliminated= true;
                                        }

                                    }
                                }
                            } else {
                                const std::string msg = "update " + table_name + " for purging association linked to property " + prop->name() + " of classifier " + csf_->name() +  " failed";
                                 
                                throw exception::Not_found ( msg );
                            }

                        } else { // case 4: opposite cardinality is 0,1 or 1,1 - idem case 2 with N opposite rows, associations in trash and, if 1,1 or isComposition, instances in trash ( recursive processing)
                            auto [first, second] ( eliminate_instance_and_association_in_opposite_rows ( prop));
                            assoc_eliminated = second;
                        }
                    }


                }
            }
        }

        const bool ret = eliminate_instance () ;  // update instance for trash
        if ( !ret ) {
            const std::string msg = "update  for purging instance " + inst_id_str_ + " of classifier " + csf_->name() +  " failed";
             
            return_code_ = qry::Return_code::KO;
        }
    
        

}



std::pair<bool, bool> Classifier_instance_eliminate::eliminate_instance_and_association_in_opposite_rows (const std::shared_ptr<const omm::Property>& p_prop) {
    
    bool assoc_eliminated= false;
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
        
        //const std::string stmt = "SELECT id, upd_count from " + table_name + " WHERE " + column_name + " = " + inst_id_ + " AND " + column_name + "_trashed_date <> 'epoch'";
        const std::string stmt = "SELECT id, csf_id, upd_count from " + table_name + " WHERE " + column_name + " = " + std::to_string(row_id_) + " AND " + column_name + "_trashed_date <> 'epoch'";
        auto f = db_->execSqlAsyncFuture ( stmt );

        auto result = f.get();
        if ( result.size() > 0 ) { // instance(s) referencing the deleted instance through the association exist(s)
            bool recursive = (opposite->min_occurs() > 0 ) || (p_prop->aggregation_kind() == omm::Aggregation_kind::composite) ;
            for ( auto row : result ) {
                
                const int upd_count = row["upd_count"].as<int>();
                  if (  recursive ) {
                    const std::string inst_id = qry::Util::build_instance_id(row);

                    Classifier_instance_eliminate opposite_instance ( opposite->classifier(), inst_id, upd_count); // instance in trash
                    opposite_instance.execute ();
                    if (opposite_instance.return_code() == qry::Return_code::KO ) {
                        ret_status = false ;
                        continue;
                    }
                    
                } else {
                    //association in trash has to be eliminated
                    const unsigned long inst_row_id = row["id"].as<unsigned long>();

                    const bool ret_stat = update_for_eliminate (table_name, column_name, inst_row_id);
                    if (!ret_stat) {
                        ret_status = false;
                        continue;
                    } else {
                        assoc_eliminated = true;
                    }
                
                }
                  
                                                                                                                
            }

        }

    } else {
        const std::string msg = "update " + table_name + " for eliminating association linked to property " + p_prop->name() + " of classifier " + csf_->name() +  " failed";
         
        throw exception::Not_found ( msg );
    }
    return  {ret_status, assoc_eliminated};
}


bool Classifier_instance_eliminate::eliminate_instance() {

    bool ret_status = false;
    auto lo = csf_->logical_struct();
    auto && table_name = lo->full_name();

    
    std::string stmt = "DELETE FROM " + table_name  + " WHERE trashed_date <> 'epoch' AND id=$1 AND  ( " + csf_id_list_ + " ) RETURNING id";

    auto f = db_->execSqlAsyncFuture ( stmt, row_id_);

    auto result = f.get();

    if ( result.size() > 0 ) {

        return_code_ = qry::Return_code::OK;
        ret_status = true;
    }

    return ret_status;
}

bool Classifier_instance_eliminate::update_for_eliminate (  const std::string& p_table_name, const std::string& p_field_name, const unsigned long p_row_id, const std::string& p_csf_id_list) {

    bool ret_status = false;
    const std::string date_column = p_field_name + "_trashed_date";
    const std::string csf_id_column = p_field_name + "_csf_id";

    const std::string fields_stmt = "UPDATE " + p_table_name + " SET ( " + p_field_name + ", " + csf_id_column + ", " + date_column + ") ";
    const std::string values_stmt = "= ROW ( NULL, NULL, 'epoch') ";

    std::string stmt = fields_stmt + values_stmt + " WHERE " + date_column + " <> 'epoch' AND id="+ std::to_string(p_row_id);
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

