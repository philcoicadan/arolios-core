/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Instance_assoc_instances_list.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <appcfg/Class_instance_member_end.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <util/String.h>



using namespace arolios;
using namespace arolios::qry;

Instance_assoc_instances_list::Instance_assoc_instances_list ( const std::shared_ptr<const omm::Class>& p_csf, const std::string& p_inst_id, const std::shared_ptr<const omm::Property>& p_member_end_property, const std::string& p_prop_id_name, const std::shared_ptr<const omm::Association>& p_assoc, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters, const std::string& p_prop_list, const std::shared_ptr<const omm::Language>& p_lang) 
: Classifier_instances_list<omm::Association> ( p_assoc, p_offset, p_limit, p_sort, p_direction, p_filters, p_prop_list, p_lang ),
    member_end_property_ ( p_member_end_property ), inst_id_str_ ( p_inst_id ), prop_id_name_ ( p_prop_id_name ), cls_ ( p_csf ) {

}

Instance_assoc_instances_list::~Instance_assoc_instances_list() {

}

void Instance_assoc_instances_list::execute () {

    std::shared_ptr<appcfg::App> app_config = app_info_ptr_->app();

    std::shared_ptr<appcfg::Base_classifier_config> class_config = app_config->find_config_by_name ( cls_->package()->name(), cls_->name() );
   
    set_config ();
    std::string&& order_by = compute_order_by ();
    std::string query = config()->list_select_template ();
    std::string && addon_query = class_config->list_select_addon_template ( prop_id_name_ );


    if ( !query.empty() && !addon_query.empty() ) {
        query += addon_query ;
        query += " ORDER BY " + order_by + " LIMIT " + limit_ + " OFFSET";
        query += (enum_val_sort_) ? " $3" : " $2";
        const auto [ csf_id, row_id] = qry::Util::split_instance_id(inst_id_str_);
        
        auto f =  (enum_val_sort_) ?
            db_->execSqlAsyncFuture ( query, row_id, util::String::stoui(lang_->id()), offset_ ) :
            db_->execSqlAsyncFuture ( query, row_id, offset_ );


        auto result = f.get();

        build_return_json ( result);

    } else {
        std::string error_msg = "select template for member end " + prop_id_name_ + " not found";
        throw exception::Not_found ( error_msg );
    }
}


