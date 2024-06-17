/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/Association_config.h>
#include <appcfg/Util.h>
#include <omm/Association.h>
#include <omm/Property.h>
#include <omm/Enumeration.h>
#include <omm/Table.h>
#include <omm/Column.h>
#include <appcfg/Class_instance_member_end.h>
#include <exception/Exceptions.h>
#include <iostream>




using namespace arolios;
using namespace arolios::appcfg ;

 Association_config::Association_config(const std::shared_ptr<omm::Association>& p_assoc, const std::shared_ptr<const App_domain>& p_app_domain) 
: Base_classifier_config (p_app_domain) , classifier_ (p_assoc){

    join_for_list_ = true;
}

Association_config::~Association_config () {}


 void Association_config::properties_for_read (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
    const auto & props_map = classifier_->all_elements_as_property ();
    for ( const auto & [name, prop] : props_map ) {

            auto && prop_view = add_property_view ( prop, name, false);
            p_prop_views.push_back(prop_view);
            if (prop->has_referencing_type()) {
                auto && linked_props= prop->type()->all_id_properties();
                for (auto [ lname, lprop ] : linked_props) {
                    // in the result json, <name>.properties.<lname> => for linked props, id_name = lname
                    auto id_name = lname;
                    auto && new_prop_v = add_property_view (lprop, id_name, false);
                    prop_view->insert_linked_property_view(new_prop_v);
                }
            }
        }
        
   //}
}   


 void Association_config::create_query_properties ()  {

    
    auto && all_props = classifier_->all_elements_as_property () ;
    
    
  
    property_map_.clear();
    property_column_name_map_.clear();
    property_alias_column_name_map_.clear();
    
  for (auto [id_name,prop]: all_props) {
      if ( ! prop->has_sub_field() && ! ( prop->association() ) ) { // props as memberEnds are removed, including the id ones. MemberEnd are managed after
           
           
            Util::add_prop_for_select_template ( prop, property_map_, enum_map_, property_column_name_map_, property_alias_column_name_map_, id_name);
        }


    }

    // add in the select template each member end with a field 
    auto members = classifier_->member_ends();
    for ( auto me: *members ) {
        if ( ! me->has_sub_field()) {
         std::unordered_map<std::string,std::string> dummy;
            Util::add_prop_for_select_template ( me, property_map_, enum_map_, property_column_name_map_, dummy, me->name());
        }
    }
        

}

 void Association_config::create_query_collections ()  {

    
    if (cls_inst_me_map_.size()  >0) return;
    
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> all_cltn_props;

    auto && id_props = classifier_->all_id_properties () ;
    
    for (auto [ name, prop] : id_props) {
            all_cltn_props.insert(std::pair<std::string,std::shared_ptr<const omm::Property>>(name,prop));
    }
    
    if (id_props.empty() ) {
         for (auto [id_name, prop]: classifier_->all_elements_as_property () ) {
            all_cltn_props.insert(std::pair<std::string,std::shared_ptr<const omm::Property>>(id_name,prop));
        }
    } else {
        auto && extra_cltn_props = collection_props();
        for ( auto [id_name, prop]: extra_cltn_props ) {
            if ( ! classifier_->has_identifier(prop) ) {
                all_cltn_props.insert ( std::pair<std::string,std::shared_ptr<const omm::Property>> ( id_name,prop ) );
            }
        }
    }
 


    property_map_for_list_.clear();
    enum_map_for_list_.clear();
    property_column_name_map_for_list_.clear();
    property_alias_column_name_map_for_list_.clear();
            std::string table_prefix = "t0"; // assoc alias = t0

  for (auto [id_name,prop]: all_cltn_props) {
      if ( ! prop->has_sub_field() && ! ( prop->association() ) ) { // props as memberEnds are removed, including the id ones. MemberEnd are managed after

            Util::add_prop_for_select_template ( prop, property_map_for_list_, enum_map_for_list_, property_column_name_map_for_list_, property_alias_column_name_map_for_list_, id_name, table_prefix );
        }


    }

    // create query collection for each member end
    auto members = classifier_->member_ends();
    cls_inst_me_map_.clear();
    int i =0;
    for ( auto me: *members ) {
        auto cls_inst_me = std::make_shared<Class_instance_member_end> ( me );
        cls_inst_me_map_.insert ( std::make_pair<const std::string&, const std::shared_ptr<Class_instance_member_end>&> ( me->name(), cls_inst_me ) );
        auto alias = "t" + std::to_string ( ++i );
        me_table_alias_map_.insert ( std::make_pair<const std::string&, const std::string&> ( me->name(),alias ) );
        cls_inst_me->table_alias(alias);
        
        auto me_type = me->type();
        auto && me_ids = me_type->all_id_properties();
        cls_inst_me->property_map_for_list().clear();
        cls_inst_me->property_column_name_map_for_list().clear();

        for ( auto [id_name,prop] : me_ids ) {
            if ( ! prop->has_sub_field() && ! ( prop->association() ) ) {

                Util::add_prop_for_select_template ( prop, cls_inst_me->property_map_for_list(), cls_inst_me->enum_map_for_list(), cls_inst_me->property_column_name_map_for_list(), cls_inst_me->property_alias_column_name_map_for_list(), id_name , alias );
            }
        }

    }
    

}
    
 std::string Association_config::default_sort_column (const bool p_with_prefix) const {

    std::string result = (p_with_prefix) ? "t0_id": "id";
    
    return result;
}

 std::string Association_config::id_column_name (const bool p_with_prefix) const {
    return (p_with_prefix) ? "t0_id": "id";
}

 std::string Association_config::trashed_date_column_name(const std::string& p_sep, const bool p_with_prefix) const {
    return (p_with_prefix) ? "t0" + p_sep + "trashed_date": "trashed_date";
}


 std::vector<std::string> Association_config::management_columns (const std::string& p_name, const bool select_flag)  const {
    static std::unordered_map<std::string,std::string> m  { {"id","id"}, {"csf_id", "csf_id"}, {"trashed_date", "trashed_date"}};
    auto item = m.find(p_name)->second ;
    
    std::vector<std::string> result;
    
     auto members = classifier_->member_ends();
     result.resize (members->size() + 1);
     result [0] = "t0." + item;
     if (select_flag) {
         result[0] += " AS t0_" + item;
     }
     int i=0;
     for (auto me: *members) {

        auto && alias = me_table_alias_map_.find(me->name())->second;
        std::string column = alias + "." + item ;
        if (select_flag) {
            column += " AS "+ alias + "_" + item;
        }
        result[++i] = column ;

    }
    return result;
}

 void Association_config::create_list_select_template ()  {
    
    

    create_query_collections();
       
    auto table = classifier_->managing_table();

    const std::string csf_id_list = classifier_->classifier_id_with_specializations("t0");
  
    std::string field_list;

    for (auto [key, value] : property_column_name_map_for_list_) {
        auto alias = property_alias_column_name_map_for_list_.find(key);
        field_list += value + " AS " + alias->second + ", ";
       
        const auto search_prop = property_map_for_list_.find(key);
        if (search_prop == property_map_for_list_.end() ) {
            std::string error_msg ("Property " + key + "not found");
             
            throw exception::Not_found (error_msg);
        } else {
            const auto prop = search_prop->second;
            if (prop->association() ) {
                field_list += value + "_csf_id AS " + alias->second + "_csf_id, ";
            }
                        // if prop is enum, add field for translation
            const auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(prop->type());
            if (enum_ptr) {
                    field_list += value + "_val_id AS " + alias->second + "_val_id, ";
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
    
        // binary N-M association or n_ary association => 2 or more joins
    auto members = classifier_->member_ends();
    int i= 1;
    for ( auto me : *members ) {
        auto tc = me->type();
        auto && alias = me_table_alias_map_.find ( me->name() )->second;
        joins += " INNER JOIN "+ tc->table()->full_name() + " AS " + alias + " ON " + alias + "." + id + " = t0." ;
        if ( me->column() ) {
            joins += me->column()->name();
        } else {
            joins += id;
        }
    }

  //  }
    
    list_tplt_ = "SELECT COUNT(*) OVER() AS _total, ";
    std::string list_sep = " ";
    if (! ids.empty()) {
        list_tplt_ +=  ids + ", " + csf_ids;
        list_sep = ", ";
    }
    
    if (!field_list.empty()) {
        list_tplt_ += list_sep + field_list ;
    }
    list_sep = " ";

    list_tplt_ += " FROM " + table->full_name() + " AS t0" + joins + " WHERE ";
    

    if (!csf_id_list.empty()) {
        list_tplt_ += " (" + csf_id_list + ") ";
    }
    
                        
}

 void Association_config::create_select_all_template ()  {
    
    
    
    auto table = classifier_->managing_table();
    auto mtc = classifier_->managing_table_class();
    
    const std::string csf_id_list = classifier_->classifier_id_with_specializations();
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

        const auto search_prop = property_map_.find(key);
        if (search_prop == property_map_.end() ) {
            std::string error_msg ("Property " + key + "not found");
             
            throw exception::Not_found (error_msg);
        } else {
            const auto prop = search_prop->second;
            if (prop->association() ) {
                if (has_alias) {
                    field_list += value + "_csf_id AS " + alias->second + "_csf_id, ";
                } else {
                    field_list += value + "_csf_id, ";
                }
            }
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

    all_tplt_ += " FROM " + table->full_name() + " WHERE " ;
    
    if (!csf_id_list.empty()) {
        all_tplt_ += " (" + csf_id_list + ") ";
    }
    
    if (mtc) { // class table managing the assoc: restrict rows with member end not null
        auto mb_ends = classifier_->member_ends();
        for ( auto mb_end : *mb_ends) {
            if (! mb_end->has_sub_field()) {
                auto search = property_column_name_map_.find(mb_end->name());
                if ( search != property_column_name_map_.end()) {
                    all_tplt_ += " AND " + search->second + " IS NOT NULL ";
                } else {
                    std::cout << "WARNING: Property " << mb_end->name() << " is not mapped to a table column" << std::endl;
                }
            }
        }
    }
    

                           
}   


 void Association_config::create_read_select_template ()  {
    
    

    
       
    auto table = classifier_->managing_table();


    
    create_query_collections(); // normally already done if create_list_select_template called before.
    
    std::vector<std::shared_ptr<const omm::Property_view>> props_vec ; 
    properties_for_read( props_vec); // all properties ( owned, inherited, member ends);
    
    
    for (auto prop_v: props_vec) {
      if ( ! prop_v->property()->association()  ) { // props as memberEnds are removed, including the id ones. MemberEnd are managed after
            std::string table_prefix = "t0"; // assoc alias = t0
            Util::add_prop_for_select_template ( prop_v->property(), property_map_for_read_, enum_map_for_read_, property_column_name_map_for_read_, property_alias_column_name_map_for_read_, prop_v->id_name(), table_prefix );
        }
    }
    
    std::string field_list;

 
    for (auto [key, value] : property_column_name_map_for_read_) {

        auto alias = property_alias_column_name_map_for_read_.find(key);
        field_list += value + " AS " + alias->second + ", ";

        const auto search_prop = property_map_for_read_.find(key);
        if (search_prop == property_map_for_read_.end() ) {
            std::string error_msg ("Property " + key + "not found");
             
            throw exception::Not_found (error_msg);
        } else {
            const auto prop = search_prop->second;
            const auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(prop->type());
            if (enum_ptr) {
                    field_list += value + "_val_id AS " + alias->second + "_val_id, ";
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

        // binary N-M association or n_ary association => 2 or more joins
    auto members = classifier_->member_ends();
    int i= 1;
    for ( auto me : *members ) {
        auto tc = me->type();
        auto && alias = me_table_alias_map_.find ( me->name() )->second;
        joins += " INNER JOIN "+ tc->table()->full_name() + " AS " + alias + " ON " + alias + "." + id + " = t0." ;
        if ( me->column() ) {
            joins += me->column()->name();
        } else {
            joins += id;
        }
    }

  //  }
    
    read_tplt_ = "";
    std::string list_sep = " ";
    if (! ids.empty()) {
        read_tplt_ +=  ids + ", " + csf_ids;
        list_sep = ", ";
    } 
   
        
    if (!field_list.empty()) {
        read_tplt_ += list_sep + field_list ;
    }
    list_sep = " ";

    read_tplt_ += " FROM " + table->full_name() + " AS t0" + joins ;
    
   
                        
}


 void Association_config::all_id_properties (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
   auto && props = classifier_->all_id_properties ();
   for ( auto [id_name, prop] : props ) {

        auto && new_prop_v = add_property_view ( prop, id_name, false);
        p_prop_views.push_back(new_prop_v);
        
        
   }
}
    
 void Association_config::properties_for_list (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
        
        
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

            auto && prop_view = add_property_view ( prop, name , false);
            p_prop_views.push_back(prop_view);
            if (prop->has_referencing_type()) {
                auto && linked_props= prop->type()->all_id_properties();
                for (auto [ lname, lprop ] : linked_props) {
                    
                    // in the result json, <name>.properties.<lname> => for linked props, id_name = lname
                    auto id_name = lname;

                    auto new_prop_v = add_property_view (lprop, id_name, false);
                    prop_view->insert_linked_property_view(new_prop_v);
                }
            }

 //       }
    }
}


 void Association_config::properties_for_create (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
    auto props_map = classifier_->all_elements_as_property ();
    for ( auto [name, prop] : props_map ) {
        if ( prop->computed_value().empty()) { 
        
            p_prop_views.push_back(add_property_view ( prop, name, false ));
        }
        
   }    
}




 void Association_config::properties_for_update (std::vector<std::shared_ptr<const omm::Property_view>>& p_prop_views) {
    auto props_map = classifier_->own_properties (); // only own properties which are not member ends can be updated
    for ( auto [name, prop] : props_map ) {
        if ( (prop->association() == nullptr) && prop->computed_value().empty() ) { // properties as memberEnd cannot be updated through the association (delete it and create a new one) and properties computed are excluded (TODO : NYI). Matched with select_template
  
             p_prop_views.push_back(add_property_view ( prop,  name, false));
        }
        
   } 
}



 const std::string& Association_config::read_select_prefix () const {
    
    static std::string prefix = "SELECT t0.upd_count AS t0_upd_count, ";
    return prefix;
}

 const std::string& Association_config::read_select_suffix () const {
     
    static std::string suffix = " WHERE t0.id=$1";
    return suffix;   
}


 std::string Association_config::default_table_alias() const {
    return std::string("t0") ;
}


 std::vector<std::string> Association_config::trashed_date_filters (const qry::Util::Context p_context)  const {

    
    std::string item = "trashed_date";

    if (classifier_->table() == nullptr) {
        // assoc implemented into the table of a class, search the column for the link (foreign key)
        int k= 0;
        auto members = classifier_->member_ends();
        while (  k < 2 ) {
            if ((*members)[k]->has_sub_field()) {
                break;
            }
            ++k;
        }
        auto opposite_prop = (*members) [1-k];

        auto column = opposite_prop->column();
        if (column == nullptr) {
            std::string error_msg = "member end " + opposite_prop->name() + " of association " + classifier_->name() + " is not mapped correctly to a relational object";
            throw exception::Not_found ( error_msg );
        }
        item = column->name() + "_" + item;

    }

 
    std::vector<std::string> result;
    if (p_context != qry::Util::Context::all) {
        
        result.push_back( "t0." + item) ;

    } else {
        result.push_back ( item );
    }
    return result;
}

 std::string Association_config::fts_column_prefix (const qry::Util::Context p_context )  const {
   
    static std::string item = "_fts_col_";
    
    std::string result ;
    if ( p_context != qry::Util::Context::all ) {
        result = "t0." + item;
    } else {
        result = item;
    }

    return result; 
}

 const std::shared_ptr<omm::Classifier> Association_config::classifier () const {
    return classifier_;
}