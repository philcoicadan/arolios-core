/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Base_classifier_instances_list.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Table.h>
#include <omm/Property.h>
#include <omm/Language.h>
#include <util/String.h>
#include <qry/Instance_filter.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <qry/Util.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <appcfg/Class_instance_member_end.h>
#include <json/value.h>








using namespace arolios;
using namespace arolios::qry;

Base_classifier_instances_list::Base_classifier_instances_list ( const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::string& p_prop_list , const std::shared_ptr<const omm::Language>& p_lang) 
: Base_list_query {  p_offset, p_limit, p_sort, p_direction, p_lang},
  filters_ (p_filters), prop_list_ (p_prop_list) , enum_val_sort_ (false) { 

}


Base_classifier_instances_list::~Base_classifier_instances_list() { }

void Base_classifier_instances_list::execute ()  {
 
    set_config ();

    std::string&& order_by = compute_order_by();

    std::string query = (prop_list_ == "ALL" ) ? config()->select_all_template() :  config()->list_select_template () ;
    
    const qry::Util::Context ctx= (prop_list_ == "ALL") ? qry::Util::Context::all : qry::Util::Context::list;
    for (auto filter : filters_) {
        if (filter->operation() == "fts") {
            auto fts_col = config()->fts_column_prefix(ctx);
            const std::string value = util::String::double_quote(filter->value());
            query += " AND " + fts_col + lang_->id() + " @@ websearch_to_tsquery('" + value + "') ";
        } else if ( filter->item() == "trashed_date") {
            
            auto trashed_date_filters = config()->trashed_date_filters(ctx);
            for (auto t_d_filter: trashed_date_filters) {
                query += " AND " + t_d_filter + " " + filter->operation() + " " + filter->value() + " ";
            }
        } else {
            std::cout << "WARNING: Filter " << filter->item() << " not applied in the query" << std::endl;
        }
    }
   query +=  " ORDER BY " + order_by + " LIMIT " + limit_ + " OFFSET $1";
    
    
    
    
     auto f = (enum_val_sort_) ?
        db_->execSqlAsyncFuture ( query, offset_, util::String::stoui(lang_->id())) :
        db_->execSqlAsyncFuture ( query, offset_);
    
    auto result = f.get();
    
    build_return_json ( result);
            
}

std::string Base_classifier_instances_list::compute_order_by () {
    std::string order_by;
    if ( sort_.empty() ) {
        order_by = config()->default_sort_column (prop_list_!="ALL");
    } else if (sort_ == "id") {
        order_by = config()->id_column_name(prop_list_!="ALL") ;
    } else if (sort_ == "trashed_date") {
        order_by = config()->trashed_date_column_name(".", prop_list_!="ALL");
    } else {
        const auto sort_vec = util::String::split(sort_,'.');
        std::string sort_extract;
        for (auto i = 0; i< sort_vec.size() ; ++i) {
            if (sort_vec[i] == "properties") continue;
            sort_extract += sort_vec[i] + ".";
        }
        if (!sort_extract.empty()) {
            sort_extract.erase(sort_extract.length()-1, 1);
        }
        

        order_by = config()->find_column_from_property_path ( sort_extract ) ;
        if ( order_by.empty() ) {
            std::cout << "WARNING: " <<"Property " + sort_ + "ignored for sorting" << std::endl;
            order_by = config()->default_sort_column (prop_list_!="ALL");

        } else if (config()->enum_map_for_list().find(sort_extract) != config()->enum_map_for_list().end()) {
                    
            order_by = config()->find_column_from_property_path ( sort_extract, false ) ; // search again because no alias for COALESCE
            order_by = "COALESCE(am_translate_enum(" + order_by + "_val_id,$2), " + order_by +"::text)";
            enum_val_sort_ = true;
        }

    }
    
    order_by += " " + direction_;
    return order_by;
}


void Base_classifier_instances_list::build_return_json (const qry::Db_result& p_result) {
    
    const auto & props_map = (prop_list_ == "ALL" ) ? config()->property_map (): config()->property_map_for_list ();

    const auto & fields_map = (prop_list_ == "ALL" ) ? config()->property_column_name_map() : config()->property_column_name_map_for_list();
    
    const auto & field_aliases_map = (prop_list_ == "ALL" ) ? config()->property_alias_column_name_map() : config()->property_alias_column_name_map_for_list();
    
    
    build_rows_for_return_json (p_result, props_map, fields_map, field_aliases_map);
    
    return_code_ = qry::Return_code::OK;
        
}

void Base_classifier_instances_list::build_rows_for_return_json ( const qry::Db_result& p_result, 
        const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_props_map,
        const std::unordered_map<std::string,std::string>& p_fields_map,
        const std::unordered_map<std::string,std::string>& p_field_aliases_map ) {

    int total = 0;
    if ( p_result.size() > 0 ) {
        auto first = p_result[0] ;
        total = first["_total"].as<int>();
    }

    return_json_ ["total"] = total;

    auto  id_pair = config()->management_column_name_with_prefix ( "id", prop_list_ != "ALL" );
    auto csf_id_pair= config()->management_column_name_with_prefix ( "csf_id", prop_list_ != "ALL" );
 
    return_json_["elements"] = Json::Value ( Json::arrayValue );

    Json::Value& jrows = return_json_["elements"];

    jrows.resize ( p_result.size() );

    int i= 0;
    for ( auto row : p_result ) {
        jrows[i] =  Json::Value ( Json::objectValue ) ;
        Json::Value& jrow = jrows[i++];

        build_main_ids_json ( jrow, row, id_pair, csf_id_pair );
        

        jrow["properties"] = Json::Value(Json::objectValue);
        Json::Value & jprops = jrow["properties"];

        build_json_from_row( jprops, row,  p_props_map, p_fields_map, p_field_aliases_map ) ;

        if ( prop_list_ != "ALL"  && config()->has_join_for_list() ) { // extract id properties of each member end

            for ( auto cime: config()->member_end_map() ) {
                auto && me_id_pair = cime.second->management_member_column_name ( "id" );
                auto && me_csf_id_pair = cime.second->management_member_column_name ( "csf_id" );
                jprops[cime.second->member_end()->name()] = Json::Value ( Json::objectValue );
                Json::Value& nested_jrow = jprops[cime.second->member_end()->name()];


                build_ids_json ( nested_jrow, row,  me_id_pair, me_csf_id_pair );
                
                nested_jrow["properties"] = Json::Value(Json::objectValue);
                Json::Value & nested_jprops = nested_jrow["properties"];
                build_json_from_row ( nested_jprops, row, cime.second->property_map_for_list(),  cime.second->property_column_name_map_for_list(), cime.second->property_alias_column_name_map_for_list());
            }
        } 
    }
    //
}

void Base_classifier_instances_list::build_ids_json ( Json::Value& p_jrow, const qry::Db_row& p_row,  const std::pair<std::string,std::string>& p_id_pair, const std::pair<std::string,std::string>& p_csf_id_pair ) {

    auto row_id = p_row[p_id_pair.second];

    if ( !row_id.isNull() ) {

        auto csf_id = p_row[p_csf_id_pair.second];
        const std::string iid = qry::Util::build_instance_id ( csf_id.as<unsigned int>(),row_id.as<unsigned long>() );

        p_jrow[p_id_pair.first] = Json::Value ( iid );

        p_jrow["url"] = Json::Value ( qry::Util::url_prefix("instance") + iid);

 
        // optional
        auto class_ptr = om_ptr_->find_domain_element_by_id<omm::Class>(
             std::to_string(csf_id.as<unsigned int>()));
        if (class_ptr) {
            p_jrow["class"] = Json::Value (Json::objectValue);
            p_jrow["class"]["url"] = qry::Util::url_prefix("class") + class_ptr->package()->name() + "." + class_ptr->name();
            p_jrow["class"]["_tid_name"] = class_ptr->package()->translated_name(lang_) + "." + class_ptr->translated_name(lang_);
        } else {
          std::string error_msg =
              "class identifier " +
              std::to_string(csf_id.as<unsigned int>()) + " not found";
          throw exception::Not_found(error_msg);
        }
    }
}

void Base_classifier_instances_list::set_config () {
std::shared_ptr<appcfg::App> appcfg = app_info_ptr_->app();
    csf_config_ = appcfg->find_config_by_name ( classifier()->package()->name(), classifier()->name() );
}

const std::shared_ptr<const appcfg::Base_classifier_config>& Base_classifier_instances_list::config() const {
    return csf_config_;

}

Json::Value &Base_classifier_instances_list::compute_json_key(
    Json::Value &p_jrow, std::vector<std::string> &p_vec, std::string &p_jkey) const {
  if (!p_jrow.isMember(p_vec[0])) {
    p_jrow[p_vec[0]] = Json::Value(Json::objectValue);
  }

  if (p_vec.size() > 2) {
    Json::Value &jrow = p_jrow[p_vec[0]];
    p_vec.erase(p_vec.begin());
    return compute_json_key(jrow, p_vec, p_jkey);
  } else {
    p_jkey = p_vec[1];
    return p_jrow[p_vec[0]];
  }
}

