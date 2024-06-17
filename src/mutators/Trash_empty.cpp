/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Trash_empty.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <mttr/Class_instance_association_eliminate.h>
#include <mttr/Classifier_instance_eliminate.h>
#include <appcfg/App.h>
#include <qry/Classifier_instances_list.h>
#include <qry/Base_list_query.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <exception/Exceptions.h>




using namespace arolios;
using namespace arolios::mttr;

Trash_empty::Trash_empty()
: app_info_ptr_ (common::Singleton<common:: App_info>::instance().object()), om_ptr_ (common::Singleton<common::Object_model_info>::instance().object()) {

}

Trash_empty::~Trash_empty() {

}


void Trash_empty::execute () {
    
    
    
    
   std::string query = "SELECT am_trash.csf_id, am_p_elem.elem_type FROM am_trash INNER JOIN am_p_elem ON am_trash.csf_id = am_p_elem.id";
    const std::string app_id_str = app_info_ptr_->app()->id();
    const std::string query2 = (app_id_str == app_info_ptr_->default_app_id()) ? " ": " INNER JOIN am_pk ON am_p_elem.package = am_pk.id WHERE am_pk.id IN (SELECT am_app_domain.a_domain FROM am_app_domain INNER JOIN am_app ON am_app.id = am_app_domain.a_app WHERE am_app.id = " + app_id_str + " ) ";
    
    query += query2 + " ORDER BY am_p_elem.elem_type ASC, am_trash.csf_id DESC";
    
    auto f = db_->execSqlAsyncFuture ( query);
    
    auto result = f.get();
    
    if (result.size()>0 ) {
        
                // add filter
        const std::shared_ptr<const qry::Instance_filter> td_filter = std::make_shared<qry::Instance_filter>("trashed_date", "<>", "'epoch'") ;
        filters_.push_back(td_filter);
        
            // set prop_list to LIST Mode 
        std::string prop_list = "LIST";
        
            // set sort & direction to trashed_date & descending:
        std::string sort = "trashed_date"; // it is not a property but a table field . See the sort method in Classifier/Association instances list 
        std::string dir = "DESC";
        

        
        for ( auto row : result ) {

            unsigned int csf_id = row["csf_id"].as<unsigned int>();

            std::shared_ptr<omm::Classifier> csf;

            std::shared_ptr<omm::Class> cls = om_ptr_->find_domain_element_by_id<omm::Class> ( std::to_string(csf_id) ); // Class ?

            std::shared_ptr<omm::Association> assoc;
            if ( cls )  {
                csf = cls;
            } else {
                assoc = om_ptr_->find_domain_element_by_id<omm::Association> ( std::to_string(csf_id)); // Association ?
                if ( assoc ) {
                    csf = assoc;
            
                }

            }

            if ( csf ) {
                const std::shared_ptr<omm::Table> elem_lo = csf->table ();
                std::unique_ptr<qry::Base_list_query> q ;
                const std::string offset;
                const std::string limit;
                const auto lang = app_info_ptr_->default_language();
                if ( cls ) {
                    q = std::make_unique<qry::Classifier_instances_list<omm::Class>>( cls, offset , limit, sort, dir, filters_, prop_list , lang_) ;
                } else {
                    q = std::make_unique<qry::Classifier_instances_list<omm::Association>> ( assoc, offset, limit, sort, dir, filters_, prop_list, lang_ );
                }


                q->execute ();

                if ( q->return_code () == qry::Return_code::OK ) {
                    const Json::Value & json = q->return_json();

                    // eliminate instances 
                    eliminate_instances ( json, csf, assoc, elem_lo);

                } else {
                    std::cout << "WARNING: Trash retrieval for classifier " << csf_id << " failed" << std::endl;
                }
            } else {
                std::string msg ( "Classifier " + std::to_string(csf_id) + " in the trash not found: " );
                exception::Not_found e ( msg );
                throw e;
            }
            
        }
            
    } else {
        // trash is empty

    }
    
     return_code_ = qry::Return_code::OK; 
                
}

void Trash_empty::eliminate_instances ( const Json::Value& p_json, const std::shared_ptr<const omm::Classifier>& p_csf, const std::shared_ptr<const omm::Association>& p_assoc, const std::shared_ptr<omm::Table>& p_elem_lo ) const {

    if ( !p_json.empty() ) {
        const Json::Value& json_rows = p_json["elements"];
        for ( Json::Value::const_iterator it = json_rows.begin() ; it != json_rows.end(); ++it ) {
            const Json::Value& elem = *it;
            const std::string iid_str = elem["id"].asString();
            if ( p_elem_lo ) {
                // additional table for association (binary association N-M)

                mttr::Classifier_instance_eliminate ip ( p_csf, iid_str, 0 );
                ip.execute ();

                if ( ip.return_code () != qry::Return_code::OK ) {
                    std::cout << "WARNING: Instance deletion failed for classifier " << p_csf->id() << " instance " << iid_str<< std::endl;
                }

            } else if ( p_assoc ) { // no table (binary association 1-N or 1-1), one class instance must be updated
                mttr::Class_instance_association_eliminate iap ( p_assoc, iid_str, 0 );
                iap.execute ();

                if ( iap.return_code () != qry::Return_code::OK ) {
                    std::cout << "WARNING: Instance deletion failed for classifier " << p_assoc->id() << " instance " << iid_str<< std::endl;

                }

            }
        }
    }
}
