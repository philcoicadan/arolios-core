/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/Base_classifier_config.h>
#include <omm/Property.h>
#include <omm/Logical_field.h>
#include <omm/Classifier.h>
#include <omm/Association.h>
#include <omm/Table.h>
#include <omm/Column.h>
#include <appcfg/App_domain.h>
#include <appcfg/App.h>
#include <appcfg/Util.h>
#include <appcfg/Class_instance_member_end.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <memory>





using namespace arolios;
using namespace arolios::appcfg;

Base_classifier_config::Base_classifier_config( const std::shared_ptr<const App_domain>& p_app_domain) 
: app_domain_(p_app_domain)  {

}

Base_classifier_config::~Base_classifier_config() {

}
  

const std::shared_ptr<const App_domain>& Base_classifier_config::app_domain () const {
    return app_domain_;
}

const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& Base_classifier_config::collection_props () const {
    return cltn_props_;
}

const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& Base_classifier_config::property_map () const {
    return property_map_;
}

const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& Base_classifier_config::enum_map () const {
    return enum_map_;
}

const std::unordered_map<std::string,std::string>& Base_classifier_config::property_column_name_map () const{
    return property_column_name_map_;
}

const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& Base_classifier_config::property_map_for_list () const {
    return property_map_for_list_;
}
const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& Base_classifier_config::enum_map_for_list () const {
    return enum_map_for_list_;
}

const std::unordered_map<std::string,std::string>& Base_classifier_config::property_column_name_map_for_list () const{
    return property_column_name_map_for_list_;
}

const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& Base_classifier_config::property_map_for_read () const{
    return property_map_for_read_;
}
const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& Base_classifier_config::enum_map_for_read () const{
    return enum_map_for_read_;
}
const std::unordered_map<std::string,std::string>& Base_classifier_config::property_column_name_map_for_read () const {
    return property_column_name_map_for_read_;
}

   

void Base_classifier_config::create_instance_associations_list_select_addon_template (const std::shared_ptr<const appcfg::App>& p_app) {
    
    

    
    for (auto [id_name, prop]: classifier()->all_elements_as_property() ) {
        auto assoc = prop->association();
        if (assoc ) { 
            
            std::shared_ptr<appcfg::Base_classifier_config> assoc_cfg = p_app->find_config_by_name(assoc->package()->name(), assoc->name());
            if (assoc_cfg) {


                    auto id = management_column_name("id");
                    auto alias_table = assoc_cfg->default_table_alias();

                    std::string where = " AND ";
                    if (prop->column() ) {
                        // one join is necessary
                        
                        auto mtc = assoc->managing_table_class();
                        if (mtc) {
                            //  binary 1,N or 1,1 association
                            where +=  alias_table + "." + id + " = $1 "; 
                        } else {
                            // binary N-M association 
                            auto ome = assoc->opposite_member_end (prop);
                            where += alias_table + "." + ome->column()->name() + " = $1 ";
                        }
                    } else {
                        // prop by query, binary 1,N or 1,1 association
                        auto ome = assoc->opposite_member_end (prop);
                        where += alias_table + "." + ome->column()->name() + " = $1 ";
                    }
                     me_tplt_map_.insert(std::make_pair<const std::string&, const std::string&>(prop->name(), where) );

            } else {
                
                throw exception::Failure ("no association configuration for select addon template of class " + classifier()->name());
            }
        }
    }
}
    
 
const std::string& Base_classifier_config::list_select_template() const {
    return list_tplt_;
}

const std::string& Base_classifier_config::select_all_template() const {
    return all_tplt_;
}


std::string Base_classifier_config::find_column_from_property_path (const std::string& p_path_name, bool p_with_alias) const {
    
    std::string result;
    if (p_with_alias) {
      // first search in the alias map
      auto search_alias =
          property_alias_column_name_map_for_list_.find(p_path_name);
      if (search_alias != property_alias_column_name_map_for_list_.end()) {
        result = search_alias->second;
      }
    }
    if (result.empty()) {
      // not yet found
      auto search = property_column_name_map_for_list_.find(p_path_name);
      if (search != property_column_name_map_for_list_.end()) {
        result = search->second;
      } else { // search in member ends
        std::vector<std::string> path_vec =
            util::String::split(p_path_name, '.');
        if (path_vec.size() > 1) {
          auto search_me = cls_inst_me_map_.find(path_vec[0]);
          if (search_me != cls_inst_me_map_.end()) {
            result =
                search_me->second->find_column_from_property_name(path_vec[1], p_with_alias);
          }
        }
      }
    }

    return result;
}


std::string Base_classifier_config::management_column_name (const std::string& p_name) const {
    static std::unordered_map<std::string,std::string> m  { {"id","id"}, {"csf_id", "csf_id"}, {"trashed_date", "trashed_date"}};

    return m.find(p_name)->second ;
}


std::pair<std::string,std::string> Base_classifier_config::management_property_column_pair(const std::string& p_name) const {
    static std::unordered_map<std::string,std::string> m  { {"id","id"}, {"csf_id", "csf_id"}, {"trashed_date", "trashed_date"}};
    return *m.find(p_name);
}


std::shared_ptr<omm::Property_view> Base_classifier_config::add_property_view (const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_id_name, const bool p_is_inherited)
{
    

        auto prop_view = std::make_shared<omm::Property_view> ( p_prop, p_id_name );
        prop_view->is_inherited (p_is_inherited);
        if ( p_prop->has_primitive_type() ) {
            prop_view->metatype(omm::Property_view::Metatype::_primitive);
        } else if (p_prop->has_instance_type()) {
                prop_view->metatype(omm::Property_view::Metatype::_class);
        } else if  (p_prop->has_enum_type()) {
            prop_view->metatype(omm::Property_view::Metatype::_enumeration);
        } else if  (p_prop->has_composite_type() ) {
            prop_view->metatype(omm::Property_view::Metatype::_composite_type);
            for ( auto nested_prop : p_prop->nested_properties() ) {
                auto id_name = p_id_name + "." + nested_prop->name();
                auto new_prop = add_property_view( nested_prop, id_name, p_is_inherited);
                prop_view->insert_nested_property_view(new_prop);
            }
        } else {


            throw exception::Failure ("internal error for property metatype");
        }
        
        return prop_view;
   
}


const std::string& Base_classifier_config::read_select_template () const  {
    return read_tplt_;

}


std::string Base_classifier_config::list_select_addon_template (const std::string& p_prop_name) const {
     std::string result;
    auto search = me_tplt_map_.find (p_prop_name) ;
    if (search != me_tplt_map_.end() ) {
        result = search->second;
    }
    return result;
}    




void Base_classifier_config::insert_collection_props (const std::string& p_name, const std::shared_ptr<omm::Property>& p_prop) {
    cltn_props_.insert(std::make_pair<const std::string&, const std::shared_ptr<omm::Property>&>(p_name, p_prop));
}

    
const std::unordered_map<std::string,std::string>& Base_classifier_config::property_alias_column_name_map_for_list() const {
    return property_alias_column_name_map_for_list_;
}

const std::unordered_map<std::string,std::string>& Base_classifier_config::property_alias_column_name_map_for_read() const {
    return property_alias_column_name_map_for_read_;
}

const std::unordered_map<std::string,std::string>& Base_classifier_config::property_alias_column_name_map() const {
    return property_alias_column_name_map_;
}


std::shared_ptr<Class_instance_member_end> Base_classifier_config::class_instance_member_end(const std::string& p_prop_name) const {
    
    std::shared_ptr<Class_instance_member_end> result;
    auto search = cls_inst_me_map_.find (p_prop_name) ;
    if (search != cls_inst_me_map_.end() ) {
        result = search->second;
    }
    return result;
}    

const std::unordered_map<std::string,std::shared_ptr<Class_instance_member_end>>& Base_classifier_config::member_end_map() const { 
    return cls_inst_me_map_;
}



std::pair<std::string,std::string> Base_classifier_config::management_column_name_with_prefix (const std::string& p_name, const bool p_with_prefix) const {
    static std::unordered_map<std::string,std::string> with_prefix_  { {"id","t0_id"}, {"csf_id", "t0_csf_id"}, {"trashed_date", "t0_trashed_date"}};
    if (p_with_prefix && has_join_for_list() ) {
        return *with_prefix_.find(p_name);
    } else {
        return management_property_column_pair (p_name);
    }
}

bool Base_classifier_config::has_join_for_list () const {
    return join_for_list_;
}
