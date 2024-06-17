/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Trash_list.h>
#include <qry/Classifier_instances_list.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <qry/Util.h>
#include <qry/Instance_filter.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <appcfg/App.h>
#include <exception/Exceptions.h>
#include <qry/Util.h>







using namespace arolios;
using namespace arolios::qry;

Trash_list::Trash_list( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::shared_ptr<const omm::Language>& p_lang) 
: Base_list_query { p_offset, p_limit, p_sort, p_direction, p_lang},
filters_ (p_filters) { 

}

Trash_list::~Trash_list() {

}

void Trash_list::execute ()  {
    
    std::string order_by = compute_order_by () ;
    
    std::string query = "SELECT am_trash.csf_id, am_pk.name, am_p_elem.name FROM am_trash INNER JOIN am_p_elem ON am_trash.csf_id = am_p_elem.id INNER JOIN am_pk ON am_p_elem.package = am_pk.id";
    const std::string app_id_str = app_info_ptr_->app()->id();
    const std::string query2 = (app_id_str == app_info_ptr_->default_app_id()) ? " ": " WHERE am_pk.id IN (SELECT am_app_domain.a_domain FROM am_app_domain INNER JOIN am_app ON am_app.id = am_app_domain.a_app WHERE am_app.id = " + app_id_str + " ) ";
    
    query += query2 + " ORDER BY $1";
    
    
    
    auto f = db_->execSqlAsyncFuture ( query, order_by);
    
    auto result = f.get();
    
    //init result
    return_json_ = Json::Value(Json::objectValue);
    return_json_["elements"] = Json::Value(Json::arrayValue);
    Json::Value& final_cumul_json = return_json_["elements"] ;
    long total = 0;
    Json::Value cumul_json = Json::Value(Json::arrayValue);
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
                std::unique_ptr<qry::Base_classifier_instances_list> q ;
                if ( cls ) {

                    q = std::make_unique<qry::Classifier_instances_list<omm::Class>> ( cls, std::to_string(0) , "ALL", sort, dir, filters_, prop_list , lang_) ;
                } else {
                    q = std::make_unique<qry::Classifier_instances_list<omm::Association>> ( assoc, std::to_string(0), "ALL", sort, dir, filters_, prop_list, lang_ );
                }


                q->execute ();

                if ( q->return_code () == qry::Return_code::OK ) {
                    const Json::Value & json = q->return_json();

                    // concatenate JSON in the result 
                    total += concatenate_json (cumul_json, json, csf);

                } else {
                    std::cout << "WARNING: Trash retrieval for classifier " << csf_id << " failed" << std::endl;
                }
            } else {
                std::string msg ( "Classifier " + std::to_string(csf_id) + " in the trash not found: " );
                exception::Not_found e ( msg );
                throw e;
            }

            // pagination for json results
            //TODO : to be improved because copy of the page of results
            paginate_json (cumul_json, total, final_cumul_json);
            
        }
            
    } else {
        // trash is empty
  
    }
    
     return_json_["total"] = Json::Value(total);
     return_code_ = qry::Return_code::OK; 
    
   
            
}

std::string Trash_list::compute_order_by () {
    std::string order_by;
    std::string default_field = "am_trash.csf_id";
    // Caution: sort is not language-sensitive, so only the internal names for domain and classifier are returned. 
    if ( sort_.empty() || sort_ == "id") {
        order_by = default_field;


    } else if ( sort_ == "classifier" ) {
        order_by = "am_pk.name, am_p_elem.name";

    } else {
        std::cout << "WARNING: Sort for trash list  " + sort_ + " ignored" << std::endl;
        order_by = default_field;

    }
    
    
    order_by += " " + direction_;
    return order_by;
}


int Trash_list::concatenate_json ( Json::Value& p_cumul_json, const Json::Value& p_json, const std::shared_ptr<const omm::Classifier>& p_csf ) {

    int nb_res = 0;
    int rep_count = qry::Util::url_prefix("instance").length()-2;
    std::string trash_rep_str = qry::Util::url_prefix("trash");
    trash_rep_str.erase( trash_rep_str.length()-1, 1);
    trash_rep_str.erase (0,1);
    if ( !p_json.empty() ) {
        const Json::Value& json_rows = p_json["elements"];
        nb_res = p_json["total"].asInt();
        for ( Json::Value::const_iterator it = json_rows.begin() ; it != json_rows.end(); ++it ) {
            const Json::Value& elem = *it;
            Json::Value new_line = Json::Value ( Json::objectValue );

            new_line["classifier"] = Json::Value ( Json::objectValue );
            if (elem.isMember("class")) {
                new_line["classifier"]["url"] =  elem["class"]["url"];
                new_line["classifier"]["_tid_name"] = elem["class"]["_tid_name"];
            }
            else {
                new_line["classifier"]["url"] = qry::Util::url_prefix(p_csf->meta_type()) + p_csf->package()->name() + "." + p_csf->name();
                new_line["classifier"]["_tid_name"] = p_csf->package()->translated_name(lang_) + "." + p_csf->translated_name(lang_);
            }

            new_line["url"] = elem["url"].asString().replace(1,rep_count,trash_rep_str);
            std::string items = qry::Util::extract_json_properties ( wbuilder_, elem );
            new_line["properties"] = items;
            p_cumul_json.append ( new_line );
        }
    }
    return nb_res;
}
           
         
void Trash_list::paginate_json (const Json::Value& p_json, long p_total, Json::Value& p_page_json)  const 
{
    
    
    long start = offset_;
    long end = (limit_ == "ALL") ? p_total : std::min (start + std::stol(limit_), p_total);
    

    Json::ArrayIndex i = 0;
    for (Json::ArrayIndex j = start; j < end ; ++j , ++i) {
        p_page_json [i] = p_json [j];
    }
    
}     
        
