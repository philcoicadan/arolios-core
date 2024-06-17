/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/App_config_loader.h>
#include <appcfg/App_domain.h>
#include <model/appconfig.h>
#include <model/Root_loader.h>
#include <model/App_element_config.h>
#include <model/Model_recorder.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <appcfg/App.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <qry/Db_types.h>



using namespace arolios;
using namespace arolios::model;

App_config_loader::App_config_loader() : 
db_ (common::Singleton<common::Db_client>::instance().object())
{
    
    app_info_ =  common::Singleton<common::App_info>::instance().object();

 

}

App_config_loader::~App_config_loader() {

}

void App_config_loader::execute ( const std::string& p_app_name, const std::string& filename ) {

    


    try {
        std::unique_ptr<App_config_c> m ( appConfig ( filename ) );

        std::shared_ptr<appcfg::App> app;
        std::shared_ptr<appcfg::App> default_app = app_info_->default_app();
        std::shared_ptr<appcfg::App> current_app = app_info_->app();

        const std::string app_name = util::String::standardize(p_app_name);

        if ( app_name == current_app->name() ) {
            // app_id is the current app, configuration is possible
            app = current_app;

        } else if ( current_app->id() == default_app->id() ) {
            // default app is the current app, any app can be configured
            app = retrieve_app ( app_name ) ;

        } else {

            std::string error_msg = "The app identifier is not the current one and the current app is not the defaut one, the configuration cannot be loaded";
             
            throw exception::Model_error ( error_msg );
        }



         
        
        // browse the tree
        
           for ( App_config_c::domain_const_iterator p ( m->domain().begin () );
                   p != m->domain ().end ();
                   ++p ) {



               std::string d_name = (*p).name() ;
              
                d_name = util::String::standardize(d_name);
               

                // check domain is associated to the app  and active
               auto dom_cfg = app->find_app_domain_by_name(d_name);

               bool d_exists = dom_cfg.operator bool();


               if ( !d_exists ) {

                   std::string msg ( "domain " + d_name + " not found in the application, configuration loading is not possible." );
                   exception::Model_error e ( msg );
                   throw e;


               } else  {
                    auto dom= dom_cfg->domain();
                   if ( dom->lc_status() != omm::Lc_status::active ) {
                       std::string msg ( "domain " + d_name + " in not active, action is not possible." );
                       exception::Model_error e ( msg );
                       throw e;
                   } else {
                       load_domain_config ( ( *p ), dom, app_info_ );
                   }
               }

           }
           

    } catch ( const xml_schema::parsing&  e ) {
        const std::string error_msg = "Error in parsing the model file " + std::string ( e.what() ) ;
         
        std::cerr << e.diagnostics() << std::endl;
        throw exception::Model_error ( error_msg );
    }
}

void App_config_loader::load_domain_config ( const Domain_c& p_domain_config, const std::shared_ptr<omm::Package>& p_domain, std::shared_ptr<common::App_info>& p_app_info) {
    
    Root_loader rl (p_app_info->root_domain());
    
    App_element_config<Domain_c,omm::Package,appcfg::App_domain> d_config (p_domain_config, p_domain, rl, p_app_info->app() );
    
    d_config.execute();
    
    std::shared_ptr<appcfg::App_domain> new_dom_config = d_config.app_element();
    
    //update database;
    Model_recorder rcd;
    try {
        
        rcd.update_config_collection_props ( new_dom_config );

        // }
    } catch ( const exception::Db_exception &e ) {
        rcd.rollback();
         
        throw e;
    } catch ( const exception::Base_exception &e ) {
        rcd.rollback();
         
        throw e;
    } catch ( std::exception const &e ) {
        rcd.rollback();
         
        throw e;
    }

    // transaction committed
    
    //update in memory
    p_app_info->app()->update_domain_config (new_dom_config);
    
    
}

std::shared_ptr<appcfg::App> App_config_loader::retrieve_app( const std::string& p_app_name)  const {   
 
    
    
    std::shared_ptr<appcfg::App> app = std::make_shared<appcfg::App> ();

    const std::string app_name = util::String::standardize(p_app_name)  ;  
    auto f = db_->execSqlAsyncFuture("SELECT * FROM am_app WHERE name = $1", app_name);

    try {
        auto result = f.get();

        if (result.size() > 0) {
            auto row = result[0];

            const unsigned int id= row["id"].as<unsigned int>();
            app->persistent_attributes ( std::to_string(id), "am_app" ) ;
            app->name ( app_name );
        }
        
        else {
            std::string msg ("application name" + app_name + " not found");
             
            throw exception::Sql_error (msg);

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}

    
    return app;

}
