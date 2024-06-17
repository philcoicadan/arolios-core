/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <common/Db_client.h>
#include <qry/Db_types.h>

namespace arolios {


namespace omm {
    class Package;
    class Language;
    class Named_element;
}


namespace common {
    
class App_info;
class Object_model_info;

}

namespace appcfg {
    class App;
}

namespace qry {
class App_retriever {
public:
    /**
     * Constructor
     */
    App_retriever();
    /**
     * Destructor
     */
    ~App_retriever();
    
    std::shared_ptr<appcfg::App> retrieve_app_by_name( const std::string& p_name) const ;
    
    void retrieve_app_config (const std::shared_ptr<const appcfg::App>& p_app_config, const std::shared_ptr<const omm::Package>& p_pk) const ;

    void retrieve_languages() const ;
    
    void retrieve_translations ( const std::shared_ptr<omm::Package>& p_pk) const ;
    
    const std::shared_ptr<appcfg::App> retrieve_app( const std::string& p_app_id) const;
    const std::shared_ptr<omm::Language> retrieve_language( const std::string& p_lang_id) const;
    
private:
    
    void retrieve_classifiers_translations ( const std::shared_ptr<const omm::Package>& p_pk) const ;
    void retrieve_properties_translations ( const std::shared_ptr<const omm::Package>& p_pk) const ;
    void retrieve_enum_values_translations ( const std::shared_ptr<const omm::Package>& p_pk) const ;
    
    void map_element_translation (const qry::Db_row&, const std::shared_ptr<omm::Named_element>& p_elem) const;
    
    std::shared_ptr<appcfg::App> retrieve_app_data ( const qry::Db_result& p_result) const;

    std::shared_ptr<appcfg::App> init_default_app ( const qry::Db_result& p_result ) const ;


    const std::shared_ptr<common::Db_client> db_;
    std::shared_ptr<common::App_info> app_info_;
    const std::shared_ptr<common::Object_model_info> mdl_info_;    

};

}
}

