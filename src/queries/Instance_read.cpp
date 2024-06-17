/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Instance_read.h>
#include <omm/Property.h>
#include <omm/Association.h>
#include <omm/Class.h>
#include <util/String.h>
#include <qry/Util.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>








using namespace arolios;
using namespace arolios::qry;

template <class T1> Instance_read<T1>::Instance_read(const std::shared_ptr<const T1>& p_csf, const std::string&  p_inst_id, const qry::Util::Read_mode p_usage , const std::vector<std::shared_ptr<const qry::Instance_filter>>& p_filters,  const std::shared_ptr<const omm::Language>& p_lang) :Base_instance_read ( p_inst_id, p_usage, p_filters, p_lang),
 csf_ (p_csf){

}

template <class T1> Instance_read<T1>::~Instance_read() {

}
 
template <class T1> void Instance_read<T1>::execute () {
    

    std::shared_ptr<appcfg::App> config = app_info_ptr_->app();

    std::shared_ptr<appcfg::Base_classifier_config> csf_config = config->find_config_by_name ( csf_->package()->name(), csf_->name() );
    
    
    const std::string& prefix = csf_config->read_select_prefix () ; //"SELECT id, upd_count, csf_id,";
    const std::string& suffix = csf_config->read_select_suffix () ; //" WHERE id=$1";
    
    std::string && stmt = prefix + csf_config->read_select_template() + suffix  ;

    const std::string && csf_id_list = get_classifier_ids() ;   
    
    stmt += " AND ( " + csf_id_list + " ) ";
    
    for (auto filter : filters_) {
        if ( filter->item() == "trashed_date") {
            auto trashed_date_filters = csf_config->trashed_date_filters(qry::Util::Context::read);
            for (auto t_d_filter: trashed_date_filters) {
                stmt += " AND " + t_d_filter + " " + filter->operation() + " " + filter->value() + " ";
            }
        } else {
            std::cout << "WARNING: Filter " << filter->item() << " not applied in the query"<< std::endl ;
        }
    }
    
    
    const auto [csf_id, row_id] = qry::Util::split_instance_id(inst_id_str_);
    auto f = db_->execSqlAsyncFuture ( stmt, row_id);
    
    auto result = f.get();
    
    if (result.size() > 0 ) {
        auto && row = result[0];

            build_return_json (row, csf_config);
            return_code_ = qry::Return_code::OK;

    } else {
        std::cout << "WARNING: Instance " << inst_id_str_ << " is not an instance of classifier " << csf_->name() << std::endl ;
        return_code_ = qry::Return_code::KO;
    }
}


template <class T1> void Instance_read<T1>::build_return_json (const qry::Db_row& p_row, const std::shared_ptr<appcfg::Base_classifier_config>& p_config) {


    build_ids_json (p_row, return_json_);

    return_json_["properties"]= Json::Value(Json::objectValue);
    Json::Value& jprops =  return_json_["properties"];
    
    
   const auto & all_props = csf_->all_elements_as_property () ;

    
    for (const auto & [name,prop] : all_props) {
             build_property_json (name, prop, csf_, p_row, p_config,  jprops, 0, std::string(""), p_config->default_table_alias());
    
    }

}
namespace arolios::qry {

template class Instance_read<omm::Class> ;

template class Instance_read<omm::Association> ;

}

