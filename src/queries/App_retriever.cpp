/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/App_retriever.h>
#include <memory>
#include <string>
#include <appcfg/App.h>
#include <appcfg/App_domain.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <qry/Util.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <appcfg/App_domain.h>
#include <appcfg/Base_classifier_config.h>
#include <omm/Model_factory.h>
#include <qry/Util.h>
#include <omm/Language.h>
#include <util/String.h>
#include <appcfg/Factory.h>




using namespace arolios;
using namespace arolios::qry;

App_retriever::App_retriever()
: db_ ( common::Singleton<common::Db_client>::instance().object() ), app_info_(common::Singleton<common::App_info>::instance().object() ),  mdl_info_(common::Singleton<common::Object_model_info>::instance().object() ) {

}

App_retriever::~App_retriever() {

}

std::shared_ptr<appcfg::App> App_retriever::retrieve_app_by_name( const std::string& p_name)  const {

    
        
    
    
    std::shared_ptr<appcfg::App> app = std::make_shared<appcfg::App> ();

    const std::string name = util::String::standardize(p_name);
    auto f = db_->execSqlAsyncFuture("SELECT * FROM am_app WHERE name = $1", name);

    try {
        auto result = f.get();

        if (result.size() > 0) {
            
            return  retrieve_app_data (result);
        }        
        
        else {
            std::string msg ("app " + p_name + " not found");
             
            throw exception::Sql_error (msg);

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}


}
 
std::shared_ptr<appcfg::App> App_retriever::retrieve_app_data ( const qry::Db_result& p_result ) const {
    const auto app = std::make_shared<appcfg::App> ();
    auto row = p_result[0];


    unsigned int id = row["id"].as<unsigned int>();
    app->persistent_attributes ( std::to_string ( id ), "am_app" ) ;

    const std::string name = row["name"].as<std::string>() ;
    const std::string desc = ( row["description"].isNull() ) ? "" : row["description"].as<std::string>() ;
    app->name ( name );
    app_info_->app ( app );

    // retrieve app domains
    auto f2 = db_->execSqlAsyncFuture ( "SELECT am_app_domain.* FROM am_app_domain INNER JOIN am_app ON am_app.id = am_app_domain.a_app WHERE am_app.id = $1", util::String::stoui ( app->id() ) );

    auto result2 = f2.get();

    for ( auto row2: result2 ) {
        unsigned int domain_id = row2["a_domain"].as<unsigned int>();
        auto domain =  mdl_info_->find_domain_by_id ( std::to_string ( domain_id ) );
        if ( domain ) {
            auto app_domain = std::make_shared<appcfg::App_domain> ( domain, app ) ;
            app->app_domains().insert ( std::make_pair<const std::string&, std::shared_ptr<appcfg::App_domain>&> ( domain->name(), app_domain ) );
        } else {
            std::string msg = "domain id " + std::to_string ( domain_id ) + " is not valid for app " + app->name() ;
            throw exception::Not_found ( msg ) ;
            break;
        }
    }
    return app;
}

std::shared_ptr<appcfg::App> App_retriever::init_default_app ( const qry::Db_result& p_result ) const {
    const auto app = std::make_shared<appcfg::App> ();
    auto row = p_result[0];


    unsigned int id = row["id"].as<unsigned int>();
    app->persistent_attributes ( std::to_string ( id ), "am_app" ) ;

    const std::string name = row["name"].as<std::string>() ;
    const std::string desc = ( row["description"].isNull() ) ? "" : row["description"].as<std::string>() ;
    app->name ( name );
    app_info_->app ( app );

    const auto root_pk =  mdl_info_->root_domain();
    const auto std_pk =  mdl_info_->standard_domain();
    if ( root_pk && std_pk )  {
        for ( auto [name, domain] :  mdl_info_->domains() ) {

            if ( domain->id() != root_pk->id() && domain->id() != std_pk->id() ) {

                auto app_domain = std::make_shared<appcfg::App_domain> ( domain, app ) ;
                app->app_domains().insert ( std::make_pair<const std::string&, std::shared_ptr<appcfg::App_domain>&> ( domain->name(), app_domain ) );
            }
        }

    }
    return app;
}


void App_retriever::retrieve_app_config (const std::shared_ptr<const appcfg::App>& p_app_config, const std::shared_ptr<const omm::Package>& p_pk) const  {
    
     
     
     auto domain_config = p_app_config->find_app_domain_by_name (p_pk->name());
     
    // retrieve app domains
    
     auto f = db_->execSqlAsyncFuture ( "SELECT am_app_prop.* FROM am_app_prop INNER JOIN am_p_elem  ON am_p_elem.id = am_app_prop.a_csf INNER JOIN  am_pk ON am_pk.id = am_p_elem.package WHERE am_pk.id = $1 AND am_app_prop.a_app = $2 ORDER BY am_app_prop.a_prop", util::String::stoui(p_pk->id()), util::String::stoui(p_app_config->id()) );

    auto result = f.get();

    for ( auto row: result ) {
        unsigned int csf_id = row["a_csf"].as<unsigned int>();
        auto csf =  mdl_info_->find_domain_element_by_id<omm::Classifier>(std::to_string(csf_id));
        if (csf) {
            auto csf_config = domain_config->find_app_classifier_by_name (csf->name());
            if (!(csf_config) ) {
                csf_config = appcfg::Factory::instance().make_classifier_config(csf, domain_config); 
                domain_config->insert_classifier(csf->name(), csf_config);
            }
            unsigned int prop_id = row["a_prop"].as<unsigned int>();
            auto prop =  mdl_info_->find_property_by_id(std::to_string(prop_id));
            if (prop) {
                csf_config->insert_collection_props(prop->name(), prop);
            } else {
                std::string msg = "property id " + std::to_string(prop_id) + " not found";
                throw exception::Not_found(msg);
                break;
            }
        } else {
            std::string msg = "classifier id " + std::to_string(csf_id) + " not found";
            throw exception::Not_found(msg);
            break;
        }
    }
}


void App_retriever::retrieve_languages() const {
    
    
 
    auto f = db_->execSqlAsyncFuture("SELECT * FROM am_language WHERE lc = 'active' ORDER BY id ");

    try {
        auto result = f.get();

        if (result.size() > 0) {
            for (auto row : result) {
                std::string lang_code = row["code"].as<std::string>() ;
                std::string lang_name = row["name"].as<std::string>();
                std::string lang_fts_cfg = row["fts_config"].as<std::string>();
                omm::Lc_status lc = qry::Util::convert_status (row["lc"].as<std::string>());

                
                                 
                const std::string desc = (row["description"].isNull()) ? "" : row["description"].as<std::string>() ;
                
                std::shared_ptr<omm::Language> lang = omm::Model_factory::instance().make_language ( lang_code, lang_name, lang_fts_cfg, lc, desc);
                 

                unsigned int id = row["id"].as<unsigned int>();
                lang->persistent_attributes ( std::to_string(id), "am_language" ) ;

                app_info_->insert_language ( lang );
            }
        }
        else {
            std::string msg ("no languages found");
             
            throw exception::Sql_error (msg);

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}

}


void App_retriever:: retrieve_translations ( const std::shared_ptr<omm::Package>& p_pk) const  {
    
    
    
 
    // domain translations
    
    auto f = db_->execSqlAsyncFuture("SELECT * FROM am_pk_tlt WHERE  t_elem = $1 ORDER BY id ", util::String::stoui(p_pk->id()));
    
    
    try {
        auto result = f.get();

        if (result.size() > 0) {
            for (auto row : result) {
                map_element_translation (row, p_pk);
            }
        }
        else {
            // no translation

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}
	
	retrieve_classifiers_translations (p_pk);
    
    retrieve_properties_translations (p_pk);
    
    retrieve_enum_values_translations (p_pk);

}
    
void App_retriever::map_element_translation (const qry::Db_row& p_row, const std::shared_ptr<omm::Named_element>& p_elem) const {
    
    unsigned int lang_id = p_row["t_lang"].as<unsigned int>() ;
    std::string tlt_name = p_row["t_name"].as<std::string>();


    std::shared_ptr<const omm::Language> lang = app_info_->find_language_by_id ( std::to_string ( lang_id ) );

    if ( lang ) {
        p_elem->create_or_update_translation ( lang, tlt_name ) ;
    } else {
        
    }
}

void App_retriever:: retrieve_classifiers_translations ( const std::shared_ptr<const omm::Package>& p_pk) const  {
    
    
    
 
   
    // classifier translations
    
    auto f = db_->execSqlAsyncFuture("SELECT * FROM am_p_elem_tlt  INNER JOIN am_p_elem ON am_p_elem_tlt.t_elem = am_p_elem.id WHERE am_p_elem.package=$1 ORDER BY am_p_elem_tlt.id ", util::String::stoui(p_pk->id()));
    
    
    try {
        auto result = f.get();

        if (result.size() > 0) {
            for (auto row : result) {
                unsigned int elem_id = row["t_elem"].as<unsigned int>() ;
                std::shared_ptr<omm::Classifier> csf =  mdl_info_->find_classifier_by_id ( std::to_string ( elem_id ) );
                
                if (csf) {
                    map_element_translation (row, csf);
                }
            }
        }
        else {
            // no translation

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}
}

void App_retriever:: retrieve_properties_translations ( const std::shared_ptr<const omm::Package>& p_pk) const  {
    
    
    
 
   
    // properties translations
    
    auto f = db_->execSqlAsyncFuture("SELECT * FROM am_property_tlt  INNER JOIN am_property ON am_property_tlt.t_elem = am_property.id  INNER JOIN am_p_elem ON am_property.owning_t = am_p_elem.id WHERE am_p_elem.package=$1 ORDER BY am_property_tlt.id ", util::String::stoui(p_pk->id()));
    
    
    try {
        auto result = f.get();

        if (result.size() > 0) {
            for (auto row : result) {
                unsigned int elem_id = row["t_elem"].as<unsigned int>() ;
                std::shared_ptr<omm::Property> prop =  mdl_info_->find_property_by_id ( std::to_string ( elem_id ) );
                
                if (prop) {
                    map_element_translation (row, prop);
                }
            }
        }
        else {
            // no translation

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}
}

void App_retriever:: retrieve_enum_values_translations ( const std::shared_ptr<const omm::Package>& p_pk) const  {
    
    
    
 
   
    // enum_values translations
    
    auto f = db_->execSqlAsyncFuture("SELECT am_enum_val_tlt.* , am_enum_val.e_elem, am_enum_val.e_value  FROM am_enum_val_tlt  INNER JOIN am_enum_val ON am_enum_val_tlt.t_elem = am_enum_val.id  INNER JOIN am_p_elem ON am_enum_val.e_elem = am_p_elem.id WHERE am_p_elem.package=$1 ORDER BY am_enum_val_tlt.id ", util::String::stoui(p_pk->id()));
    
    
    try {
        auto result = f.get();

        if (result.size() > 0) {
            for (auto row : result) {
                unsigned int enum_id = row["e_elem"].as<unsigned int>() ;
                const std::string elem_name = row["e_value"].as<std::string>() ;
                std::shared_ptr<omm::Enumeration> enum_ptr =  mdl_info_->find_domain_element_by_id<omm::Enumeration> ( std::to_string ( enum_id ) );
                
                if (enum_ptr) {
                    std::shared_ptr<omm::Named_element> enum_val = enum_ptr->find_element ( elem_name);
                    if (enum_val) {
                        map_element_translation (row, enum_val);
                    }
                }
            }
        }
        else {
            // no translation

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}
}

const std::shared_ptr<appcfg::App> App_retriever::retrieve_app( const std::string& p_app_id) const {

    
   
    //lookup default app
    auto f = db_->execSqlAsyncFuture ( "SELECT * FROM am_app  WHERE id = $1", util::String::stoui(p_app_id));

    
    
    try {
        auto result = f.get();

        if ( result.size() > 0 ) {
            
            if ( p_app_id == app_info_-> default_app_id()) {
                return init_default_app (result) ;
            } else {
            
                return retrieve_app_data (result);
            }
            
           

        } else {
            std::string error_msg = "application " + p_app_id + " not found";
             
            throw exception::Failure( error_msg ) ;
        }
    }

    catch ( const exception::Db_exception &e ) {
        std::cerr << "error: " << e.base().what() << std::endl;

        throw e;
    }
}
    
const std::shared_ptr<omm::Language> App_retriever::retrieve_language( const std::string& p_lang_id) const {

    
   
    //lookup default app
    auto f = db_->execSqlAsyncFuture ( "SELECT * FROM am_language  WHERE id = $1", util::String::stoui(p_lang_id));

    try {
        auto result = f.get();

        if ( result.size() > 0 ) {
                // found
                auto row = result[0];

                std::string lang_code = row["code"].as<std::string>() ;
                std::string lang_name = row["name"].as<std::string>();
                std::string lang_fts_cfg = row["fts_config"].as<std::string>();
                omm::Lc_status lc = qry::Util::convert_status ( row["lc"].as<std::string>() );

                

                const std::string desc = ( row["description"].isNull() ) ? "" : row["description"].as<std::string>() ;

                const std::shared_ptr<omm::Language> lang = omm::Model_factory::instance().make_language ( lang_code, lang_name, lang_fts_cfg, lc, desc );


                lang->persistent_attributes ( p_lang_id , "am_language" ) ;


                return lang;

        } else {
            std::string error_msg = "default language not found" ;
             
            throw exception::Failure( error_msg ) ;
        }
    }

    catch ( const exception::Db_exception &e ) {
        std::cerr << "error: " << e.base().what() << std::endl;

        throw e;
    }

    


}
