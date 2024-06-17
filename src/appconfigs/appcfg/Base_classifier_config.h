/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <omm/Persistent_object.h>
#include <omm/Property_view.h>
#include <qry/Util.h>

namespace arolios {



namespace omm {
    class Classifier;
    class Property;
}

namespace appcfg {
    class App;
    class App_domain;
    class Class_instance_member_end;
class Base_classifier_config {
public:
    /**
     * Constructor
     */
    Base_classifier_config( const std::shared_ptr<const App_domain>& p_app_domain );

    /**
     * Destructor
     */
    virtual ~Base_classifier_config();
    
    virtual const std::shared_ptr<omm::Classifier> classifier () const =0;
    const std::shared_ptr<const App_domain>& app_domain () const ;
    

    const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& collection_props () const;
    const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& property_map () const;
    const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& enum_map () const;
    const std::unordered_map<std::string,std::string>& property_column_name_map () const ;
    const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& property_map_for_list () const;
     const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& enum_map_for_list () const;
   const std::unordered_map<std::string,std::string>& property_column_name_map_for_list () const;
         const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& property_map_for_read () const;
     const std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& enum_map_for_read () const;
    const std::unordered_map<std::string,std::string>& property_column_name_map_for_read () const;
    
    const std::unordered_map<std::string,std::string>& property_alias_column_name_map_for_list()const ;
    const std::unordered_map<std::string,std::string>& property_alias_column_name_map_for_read()const ;
    
    const std::unordered_map<std::string,std::string>& property_alias_column_name_map() const ;
    
    void insert_collection_props (const std::string& p_name, const std::shared_ptr<omm::Property>& p_prop) ;
    
    
    const std::string& select_all_template () const ;
    const std::string& list_select_template () const ;
    
   
    virtual std::string default_sort_column (const bool p_with_prefix=false) const =0;
    virtual std::string id_column_name(const bool p_with_prefix) const =0;
       std::string find_column_from_property_path (const std::string& p_property_path, bool p_with_alias=true) const;

    std::string list_select_addon_template (const std::string& p_prop_name) const ;
   
    std::pair<std::string,std::string> management_property_column_pair(const std::string& p_name) const ;

    
    virtual void create_select_all_template () = 0;
    virtual void create_list_select_template () = 0;
    virtual void create_read_select_template () = 0;
    
    
    void create_instance_associations_list_select_addon_template (const std::shared_ptr<const appcfg::App>& p_app) ;
    
    virtual void all_id_properties (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) = 0;
    
    virtual void properties_for_list (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) =0;
    
    virtual void properties_for_create (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) =0;
    
    virtual void properties_for_read (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) =0;
    
    virtual void properties_for_update (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) =0;
    
    virtual const std::string& read_select_prefix () const = 0;
    virtual const std::string& read_select_suffix () const =0;
    
    virtual const std::string& read_select_template () const ;
    
    virtual std::string default_table_alias() const =0;
    
    virtual std::vector<std::string> trashed_date_filters (const qry::Util::Context p_context )  const =0;
        
    virtual std::string fts_column_prefix (const qry::Util::Context p_context )  const =0;
    
    virtual std::string trashed_date_column_name(const std::string& p_sep, const bool p_with_prefix=false) const =0;

    std::shared_ptr<Class_instance_member_end> class_instance_member_end(const std::string& p_prop_name) const;

    const std::unordered_map<std::string,std::shared_ptr<Class_instance_member_end>>& member_end_map() const ;
    
    virtual std::vector<std::string> management_columns (const std::string& p_name, const bool select_flag = true)  const =0;
    
    virtual std::pair<std::string,std::string> management_column_name_with_prefix (const std::string& p_name, const bool p_with_prefix=false) const ;
    
    virtual bool has_join_for_list () const ;
    
protected:     

    
    std::string management_column_name (const std::string& p_name) const;
    
    std::string all_tplt_;
    std::unordered_map<std::string,std::string> property_column_name_map_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> property_map_; 

    std::unordered_map<std::string,std::shared_ptr<const omm::Property>> cltn_props_;
    std::string list_tplt_;
    std::unordered_map<std::string,std::string> property_column_name_map_for_list_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> property_map_for_list_;    
    
    
    std::string read_tplt_;
    std::unordered_map<std::string,std::string> property_column_name_map_for_read_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> property_map_for_read_;
    
    std::unordered_map<std::string,std::string> property_alias_column_name_map_for_list_;
    std::unordered_map<std::string,std::string> property_alias_column_name_map_for_read_;
    std::unordered_map<std::string,std::string> property_alias_column_name_map_;
    
    std::shared_ptr<omm::Property_view> add_property_view (const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_id_name, const bool p_is_inherited );

    std::unordered_map<std::string,std::string> me_table_alias_map_;


    std::unordered_map<std::string,std::shared_ptr<Class_instance_member_end>> cls_inst_me_map_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> enum_map_for_read_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> enum_map_for_list_;
    std::unordered_map<std::string, std::shared_ptr<const omm::Property>> enum_map_;

    bool join_for_list_;
    
private:
    
    virtual void create_query_collections () =0;
    virtual void create_query_properties () =0;

                                        
    std::shared_ptr<const App_domain> app_domain_;
    
    std::unordered_map<std::string,std::string> me_tplt_map_;

};
}
}

