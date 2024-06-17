/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <appcfg/Base_classifier_config.h>
#include <omm/Property_view.h>
#include <qry/Util.h>

namespace arolios {



namespace omm {
    class Class;
    class Property;
}

namespace appcfg {
    class App;
    class App_domain;
    class Class_instance_member_end;
 class Class_config : public Base_classifier_config {
public:
    /**
     * Constructor
     */
    Class_config(const std::shared_ptr<omm::Class>& p_classifier, const std::shared_ptr<const App_domain>& p_app_domain );

    /**
     * Destructor
     */
    virtual ~Class_config();
    
    virtual const std::shared_ptr<omm::Classifier> classifier () const override;
   
    virtual std::string default_sort_column (const bool p_with_prefix=false) const override;
    virtual std::string id_column_name(const bool p_with_prefix) const override;
    
    virtual void create_select_all_template () override;
    virtual void create_list_select_template () override;
    virtual void create_read_select_template () override;
     
    virtual void all_id_properties (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) override;
    
    virtual void properties_for_list (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) override;
    
    virtual void properties_for_create (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) override;
    
    virtual void properties_for_read (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) override ;
    
    virtual void properties_for_update (std::vector<std::shared_ptr<const omm::Property_view>>& prop_views) override;
    
    virtual const std::string& read_select_prefix () const override;
    virtual const std::string& read_select_suffix () const override;
    
    virtual std::string default_table_alias() const override;
    
    virtual std::vector<std::string> trashed_date_filters (const qry::Util::Context p_context )  const override;
        
    virtual std::string fts_column_prefix (const qry::Util::Context p_context )  const override;
    
    virtual std::string trashed_date_column_name(const std::string& p_sep, const bool p_with_prefix=false) const  override;

    virtual std::vector<std::string> management_columns (const std::string& p_name, const bool select_flag = true)  const override;
       
protected: 
    
      
    
private:
    
    virtual void create_query_collections ()  override;
    virtual void create_query_properties ()  override;

    std::shared_ptr<omm::Class> classifier_;
    
};
}
}

