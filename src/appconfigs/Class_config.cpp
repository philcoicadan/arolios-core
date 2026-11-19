/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/Class_config.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Table.h>
#include <omm/Enumeration.h>
#include <omm/Column.h>
#include <appcfg/Util.h>
#include <appcfg/Class_instance_member_end.h>
#include <exception/Exceptions.h>








using namespace arolios;
using namespace arolios::appcfg;

 Class_config::Class_config(const std::shared_ptr<omm::Class>& p_classifier, const std::shared_ptr<const App_domain>& p_app_domain) 
: Base_classifier_config(p_app_domain) , classifier_ (p_classifier) {
    join_for_list_ = false;
}

 Class_config::~Class_config() {

}
  

 const std::shared_ptr<omm::Classifier> Class_config::classifier () const {
    return classifier_;
}



 void Class_config::create_query_properties () {
    
    auto && all_props = classifier_->all_elements_as_property () ;
    
 
    property_map_.clear();
    enum_map_.clear();
    property_column_name_map_.clear();
    property_alias_column_name_map_.clear();
   for (auto [id_name, prop]: all_props) {
         if ( prop->association () == nullptr  || (! prop->has_sub_field() && ! prop->has_many_targets()) ) { // we keep prop as MemberEnd if mapped on one table field of a class
        
            Util::add_prop_for_select_template ( prop, property_map_, enum_map_, property_column_name_map_, property_alias_column_name_map_, id_name);
            
        }
                    
      
    }
}

 void Class_config::create_query_collections () {
    
       
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> all_cltn_props;

    auto && id_props = classifier_->all_id_properties () ;
    
   
    
    for (auto [ name, prop] : id_props) {
        if (prop->association() && prop->max_occurs() == 1 ) { // prop id => min = 1
            join_for_list_ = true;
        }
            all_cltn_props.insert(std::pair<std::string,std::shared_ptr<const omm::Property>>(name,prop));
    }
    
    
    
    if ( id_props.empty() ) {
        for ( auto [id_name, prop]: classifier_->all_elements_as_property () ) {
            all_cltn_props.insert ( std::pair<std::string,std::shared_ptr<const omm::Property>> ( id_name,prop ) );
        }
    } else {
        auto && extra_cltn_props = collection_props();
        for ( auto [id_name, prop]: extra_cltn_props ) {
            if ( ! classifier_->has_identifier ( prop ) ) {
                if ( prop->association() && prop->max_occurs() == 1 ) { // prop id => min = 1
                    join_for_list_ = true;
                }
                all_cltn_props.insert ( std::pair<std::string,std::shared_ptr<const omm::Property>> ( id_name,prop ) );
            }
        }
    }
 
    property_map_for_list_.clear();
    enum_map_for_list_.clear();
    property_column_name_map_for_list_.clear();
    property_alias_column_name_map_for_list_.clear();
    
    const std::string table_prefix = join_for_list_ ? "t0" : "";

   for (auto [id_name, prop]: all_cltn_props) {
        if ( prop->association () == nullptr  || (! prop->has_sub_field() && ! prop->has_many_targets()) ) { // we keep prop as MemberEnd if mapped on one table field
        
            Util::add_prop_for_select_template ( prop, property_map_for_list_, enum_map_for_list_, property_column_name_map_for_list_, property_alias_column_name_map_for_list_, id_name, table_prefix);
        }
                    
      
    }
    
    cls_inst_me_map_.clear();
    int i =0;
    for ( auto [id_name, prop]: all_cltn_props ) {
        if ( prop->association() && prop->max_occurs() == 1 ) {

            auto cls_inst_me = std::make_shared<Class_instance_member_end> ( prop );
            cls_inst_me_map_.insert ( std::make_pair<const std::string&, const std::shared_ptr<Class_instance_member_end>&> ( prop->name(), cls_inst_me ) );
            auto alias = "t" + std::to_string ( ++i );
            me_table_alias_map_.insert ( std::make_pair<const std::string&, const std::string&> ( prop->name(),alias ) );
            cls_inst_me->table_alias ( alias );

            auto me_type = prop->type();
            auto && me_ids = me_type->all_id_properties();
            cls_inst_me->property_map_for_list().clear();
            cls_inst_me->enum_map_for_list().clear();
            cls_inst_me->property_column_name_map_for_list().clear();
            

            for ( auto [id_name,prop] : me_ids ) {
                if ( ! prop->has_sub_field() && ! ( prop->association() ) ) { // no recursive
   
                    Util::add_prop_for_select_template ( prop, cls_inst_me->property_map_for_list(), cls_inst_me->enum_map_for_list(), cls_inst_me->property_column_name_map_for_list(), cls_inst_me->property_alias_column_name_map_for_list(), id_name, alias );
                }
            }

        }
    }
    
}

    

 void Class_config::all_id_properties (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
   auto && props = classifier_->all_id_properties ();
   for ( auto [id_name, prop] : props ) {
        /*std::string prop_path;
        if ( ! ( prop->classifier() == classifier_ ) ) {
            prop_path = prop->classifier()->name() + ".";
            if ( ! ( prop->classifier()->package()  == classifier->package_() ) ) {
                prop_path = prop->classifier()->package()->name() + "." + prop_path ;
            }
        } */
         p_prop_views.push_back(add_property_view ( prop, id_name, ! ( prop->classifier() == classifier_ )));
        
        
   }
}


    
 void Class_config::properties_for_list (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
    
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> props;

    auto && id_props = classifier_->all_id_properties () ;
    
    for (auto [ name, prop] : id_props) {
            props.insert(std::pair<std::string,std::shared_ptr<const omm::Property>>(name,prop));
    }
    
    

    
    if (id_props.empty() ) {
         for (auto [name, prop]: classifier_->all_elements_as_property () ) {
            props.insert(std::pair<std::string,std::shared_ptr<const omm::Property>>(name,prop));
        }
    } else {
        auto && extra_cltn_props = collection_props();
        for ( auto [name, prop]: extra_cltn_props ) {
            if ( ! classifier_->has_identifier(prop) ) {
                props.insert ( std::pair<std::string,std::shared_ptr<const omm::Property>> ( name,prop ) );
            }
        }
    }
 

    for ( auto [name, prop] : props ) {
        if ( prop->association() == nullptr || (! prop->has_sub_field() && ! prop->has_many_targets())) { // properties as member end mapped on one table field are extracted for list operation. See alignment with select_template
          
            auto && prop_view = add_property_view ( prop, name , ! ( prop->classifier() == classifier_));
             p_prop_views.push_back(prop_view);
             if (prop->has_instance_type()) {
                auto && linked_props= prop->type()->all_id_properties();
                for (auto [ lname, lprop ] : linked_props) {
                    // in the result json, <name>.properties.<lname> => for linked props, id_name = lname
                    auto id_name = lname;

                    auto new_prop_v = add_property_view (lprop, id_name, false);
                    prop_view->insert_linked_property_view(new_prop_v);
                }
            }


        }
    }
    
}


 void Class_config::properties_for_create (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
    auto props_map = classifier_->all_elements_as_property ();
    for ( auto [name, prop] : props_map ) {
        if ( prop->association() == nullptr) {
            if ( prop->computed_value().empty()) { // properties as member end which are not required and have unique reference are not extracted for create operation
           
                 p_prop_views.push_back(add_property_view ( prop, name, ! ( prop->classifier() == classifier_ )));
            }
        } else if (classifier_->has_required_property(prop) && (prop->max_occurs() == 1)) { // assoc instance must be created
            auto && prop_view = add_property_view ( prop, name, ! ( prop->classifier() == classifier_ ));
            p_prop_views.push_back(prop_view);
            const auto assoc = prop->association();
            auto assoc_props_map = assoc->own_properties (); // only own properties which are not member ends AND required can be created
            for ( auto [aname, aprop] : assoc_props_map ) {
                if ( (aprop->association() == nullptr) && assoc->has_required_property(aprop) ) { 
                    std::string id_name = name + "." + aname;
                    auto && new_prop_v = add_property_view ( aprop,  id_name, false);
                    prop_view->insert_associated_property_view(new_prop_v );
                }
            }
        }
        
   }    
}


 void Class_config::properties_for_read (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
    const auto & props_map = classifier_->all_elements_as_property ();
    for ( const auto & [name, prop] : props_map ) {
        
             p_prop_views.push_back(add_property_view ( prop, name, ! ( prop->classifier() == classifier_ )));
            
        
        
   }
}   


 void Class_config::properties_for_update (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
    
   properties_for_create (p_prop_views);
}
 

    
 std::string Class_config::default_sort_column (const bool p_with_prefix) const {
   
    std::string result = (p_with_prefix && join_for_list_) ? "t0_id": "id";
    
    return result;
}

 std::string Class_config::id_column_name (const bool p_with_prefix) const {
    return (p_with_prefix && join_for_list_) ? "t0_id": "id";
}

 std::string Class_config::trashed_date_column_name(const std::string& p_sep, const bool p_with_prefix) const {
    return (p_with_prefix && join_for_list_) ? "t0" + p_sep + "trashed_date": "trashed_date";
}
 const std::string& Class_config::read_select_prefix () const {
    
    static std::string prefix = "SELECT id, upd_count, csf_id,";
    return prefix;
}

 const std::string& Class_config::read_select_suffix () const {
     
    static std::string suffix = " WHERE id=$1";
    return suffix;   
}


 std::string Class_config::default_table_alias() const {
    return std::string("") ;
}



 std::vector<std::string> Class_config::trashed_date_filters ( const qry::Util::Context p_context)  const {

    
    static std::string item = "trashed_date";
    
    std::vector<std::string> result;
    if ( p_context == qry::Util::Context::list && join_for_list_ ) {


        result.push_back( "t0." + item) ;


    } else {
        result.push_back(item);
    }

    return result;
}    

 std::string Class_config::fts_column_prefix (const qry::Util::Context p_context )  const {
   
    static std::string item = "_fts_col_";
    
    std::string result ;
    if ( p_context == qry::Util::Context::list && join_for_list_ ) {
        result = "t0." + item;
    } else {
        result = item;
    }

    return result; 
}




 std::vector<std::string> Class_config::management_columns (const std::string& p_name, const bool select_flag)  const {
    static std::unordered_map<std::string,std::string> m  { {"id","id"}, {"csf_id", "csf_id"}, {"trashed_date", "trashed_date"}};
    auto item = m.find(p_name)->second ;
    
    std::vector<std::string> result;
   
    if (!cls_inst_me_map_.empty()) {
        result.push_back( "t0." + item);
        if (select_flag) {
            result[0] += " AS t0_" + item;
        }
        
        for ( auto cime : cls_inst_me_map_ ) {
            auto me = cime.second->member_end();
            auto tc = me->type();
            auto alias = cime.second->table_alias();
            std::string column = alias + "." + item ;
            if ( select_flag ) {
                column += " AS "+ alias + "_" + item;
            }
            result.push_back ( column ) ;
        }
        
     } else {
         result.push_back(item);
     }
    return result;
}


void Class_config::create_read_select_template ()  {
        
        read_tplt_ = classifier()->read_select_template () ; 
        

} 
    
void Class_config::create_select_all_template ()  {
    
    
    
    auto table = classifier()->table();
    
    auto && csf_id_list = classifier()->classifier_id_with_specializations() ;
    
    create_query_properties();
    
    std::string field_list;

    
    for (auto [key, value] : property_column_name_map_) {
        auto alias = property_alias_column_name_map_.find(key);
        bool has_alias = false;
        if (alias == property_alias_column_name_map_.end() ) {
            field_list += value + ", ";
        } else {
             field_list += value + " AS " + alias->second + ", ";
             has_alias = true;
        }
        auto search_prop = property_map_.find(key);
        if (search_prop == property_map_.end() ) {
            std::string error_msg ("Property " + key + "not found");
             
            throw exception::Not_found (error_msg);
        } else {
            const auto prop = search_prop->second;
            if (prop->association() && prop->max_occurs() == 1 ) {
                if (has_alias) {
                    field_list += value + "_csf_id AS " + alias->second + "_csf_id, ";
                } else {
                    field_list += value + "_csf_id, ";
                }
            }
            // if prop is enum, add field for translation
            const auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(prop->type());
            if (enum_ptr) {
                 if (has_alias) {
                    field_list += value + "_val_id AS " + alias->second + "_val_id, ";
                } else {
                    field_list += value + "_val_id, ";
                }
            }
        }
    }
    if (!field_list.empty()) {
        field_list.erase(field_list.length()-2,2);
    }
    
    
    auto && id = management_column_name ("id");
    auto && csf_id = management_column_name ("csf_id");
    
    all_tplt_ = "SELECT COUNT(*) OVER() AS _total, " + id + ", " + csf_id ;

    
    if (!field_list.empty()) {
        all_tplt_ += ", " + field_list ;
    }

    all_tplt_ += " FROM " + table->full_name() + " WHERE ";
    

    if (!csf_id_list.empty()) {
        all_tplt_ += " (" + csf_id_list + ") ";
    }
    

                           
}   

void Class_config::create_list_select_template ()  {
    
    
    
    create_query_collections();
    const std::string table_prefix = has_join_for_list() ? "t0" : "" ;
    
    auto table = classifier()->table();
    
    auto && csf_id_list = classifier()->classifier_id_with_specializations(table_prefix) ;
    

    
    
    std::string field_list;

    for (auto [key, value] : property_column_name_map_for_list_) {
        auto alias = property_alias_column_name_map_for_list_.find(key);
        bool has_alias = false;
        if (alias == property_alias_column_name_map_for_list_.end() ) {
            field_list += value + ", ";
        } else {
             field_list += value + " AS " + alias->second + ", ";
             has_alias = true;
        }
        auto search_prop = property_map_for_list_.find(key);
        if (search_prop == property_map_for_list_.end() ) {
            std::string error_msg ("Property " + key + "not found");
             
            throw exception::Not_found (error_msg);
        } else {
            const auto prop = search_prop->second;
            if (prop->association() && prop->max_occurs() == 1 ) {
                if (has_alias) {
                    field_list += value + "_csf_id AS " + alias->second + "_csf_id, ";
                } else {
                    field_list += value + "_csf_id, ";
                }
            }
            // if prop is enum, add field for translation
            const auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(prop->type());
            if (enum_ptr) {
                 if (has_alias) {
                    field_list += value + "_val_id AS " + alias->second + "_val_id, ";
                } else {
                    field_list += value + "_val_id, ";
                }
            }
        }
    }
    
    for (auto cime : cls_inst_me_map_) {
        auto && alias_map = cime.second->property_alias_column_name_map_for_list();
        for (auto [key, value] : cime.second->property_column_name_map_for_list() ) {
            auto alias = alias_map.find(key);
            field_list += value + " AS " + alias->second + ", ";
        }
    }
    
    if (!field_list.empty()) {
        field_list.erase(field_list.length()-2,2);
    }
    
    
    auto && id_vec = management_columns ("id");
    auto && csf_id_vec = management_columns ("csf_id");
    
    std::string ids;
    std::string csf_ids;
    
    for (int k=0; k< id_vec.size(); ++k) { 
        ids += id_vec[k] + ", ";
        csf_ids += csf_id_vec[k] + ", ";
    }

    if (! ids.empty()) {
        ids.erase(ids.length()-2,2); 
        csf_ids.erase(csf_ids.length()-2,2); 
    }
    
    std::string joins;
    auto && id = management_column_name ("id");
  
    for ( auto cime : cls_inst_me_map_) {
        auto me = cime.second->member_end();
        auto tc = me->type();
        auto alias = cime.second->table_alias();
        const std::string join_type = me->min_occurs() > 0 ? " INNER" : " LEFT";
        joins += join_type + " JOIN "+ tc->table()->full_name() + " AS " + alias + " ON " ;
        if ( me->column() ) { // t0 contains foreign primary key of tn
            joins +=  alias + "." + id + " = t0." + me->column()->name();
        } else { // tn contains foreign primary key of t0
            // find opposite member_end
            auto assoc = me->association();
            auto ome = assoc->opposite_member_end (me);
            joins += alias + "." + ome->column()->name() + " = t0." + id;
        }
       
       
    }
    
    
    list_tplt_ = "SELECT COUNT(*) OVER() AS _total, " ;

     std::string list_sep = " ";
    if (! ids.empty()) {
        list_tplt_ +=  ids + ", " + csf_ids;
        list_sep = ", ";
    }
    
    if (!field_list.empty()) {
        list_tplt_ += list_sep + field_list ;
    }
    list_sep = " ";

    list_tplt_ += " FROM " + table->full_name() ;
    
    if (!joins.empty()) {
        list_tplt_ +=  " AS t0" + joins ;
    }
    
    list_tplt_  += " WHERE " ;
    

    if (!csf_id_list.empty()) {
        list_tplt_ += " (" + csf_id_list + ") ";
    }
        

                           
}  