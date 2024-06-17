/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_loader.h>
#include <model/Model_recorder.h>
#include <model/Model_implementor.h>
#include <model/xam.h>
#include <iostream>
#include <util/String.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <model/Model_element_delete.h>
#include <model/Model_implementor.h>
#include <model/Root_loader.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Data_type.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <appcfg/App_domain.h>
#include <exception/Exceptions.h>







using namespace arolios;
using namespace arolios::model;

Model_loader::Model_loader( ) :
    db_ (common::Singleton<common::Db_client>::instance().object())
{
}

Model_loader::~Model_loader() {

}

void Model_loader::execute ( const std::string& filename) {
    
  
    try {
    std::unique_ptr<Model_t> m (model(filename));

   
        

    
    auto model_info = common::Singleton<common::Object_model_info>::instance().object();
    
    for ( Model_t::domain_const_iterator p ( m->domain().begin () );
            p != m->domain ().end ();
            ++p ) {
    
    
        std::string d_name = (*p).name() ;
        
            d_name = util::String::standardize(d_name);
   
        
    
        std::string d_action = util::String::standardize((*p).action());
    
        auto dom = model_info->find_domain_by_name(d_name, false);
      
        bool d_exists = dom.operator bool();
        

    
        if (d_action == "create") {
            if (!d_exists) {
                create_domain ( (*p), d_name, model_info) ;
            }
            else {
                std::string msg ( "domain " + d_name + " already exists, creation is not possible." );
                throw exception::Model_error  ( msg );
                
            }
        }
        else  if (d_exists) {


            
            auto app_info = common::Singleton<common::App_info>::instance().object();
            if (!app_info->find_domain_by_name(d_name, false)) {
                std::string msg ( "domain " + d_name + " is not in the application scope, action is not possible." );
                throw exception::Model_error ( msg );
                
            }
            if ( d_action == "update") {
                if ( dom->lc_status() != omm::Lc_status::active ) {
                    std::string msg ( "domain " + d_name + " is not active, update is not possible." );
                    throw exception::Model_error ( msg );
                  
                }
                update_domain ( (*p), dom,model_info) ;
            } else {
                // d_action == "delete"
                delete_domain ( (*p), dom, model_info) ;
            }
        }
        else {
            const std::string msg ( "domain " + d_name + " does not exist, action is not possible." );
            throw exception::Model_error ( msg );
        }
    }
    }

    catch ( const xml_schema::parsing&  e) {
        const std::string error_msg =  "Error in parsing the model file " + std::string(e.what()) ; 
         
        std::cerr << e.diagnostics() << std::endl;
        throw exception::Model_error (error_msg);
    }
}

void Model_loader::update_domain ( const Domain_t& p_domain, const std::shared_ptr<omm::Package>& p_pk, const std::shared_ptr<common::Object_model_info>& p_info) {

      Root_loader rl (p_info->root_domain());
        
    Model_element_update<Domain_t,omm::Package > d_update (p_domain, p_pk, rl);
    
    d_update.execute ();
    
    update_domain_in_app_model( d_update.stored());


}

void Model_loader::delete_domain ( const Domain_t& p_domain, const std::shared_ptr<omm::Package>& p_pk, const std::shared_ptr<common::Object_model_info>& p_info) {
    
       Root_loader rl (p_info->root_domain());
        
    Model_element_delete<Domain_t,omm::Package > d_delete (p_domain, p_pk, rl);
    
 
    d_delete.execute ();
    
}



void Model_loader::create_domain ( const Domain_t& p_domain, const std::string& p_name, const std::shared_ptr<common::Object_model_info>& p_info) {
    
    auto root_pk = p_info->root_domain();
            
    Root_loader rl (root_pk);
        
    
    Model_element_create<Domain_t,omm::Package > d_store (p_domain, p_name, rl);
    



            d_store.execute ();
            
            
            associate_domain_app (d_store.stored());
            
   
       
       

    
    insert_domain_in_app_model( d_store.stored());
    

    
} 

template<> void Model_loader::insert_classifiers_in_app_model ( const std::unordered_map<std::string,std::shared_ptr<omm::Enumeration>>& p_map) const 
            {
            auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
            for ( auto elem : p_map ) {
                auto cls = elem.second ;
                 mdl_info->insert_classifier( cls );
                for ( auto value : cls->enum_values () ) {
                     mdl_info->insert_enum_value ( value.second );
                }
            }
        } 

 

template<typename T> void Model_loader::insert_classifiers_in_app_model ( const std::unordered_map<std::string,std::shared_ptr<T>>& p_map) const 
            {
            auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
            for ( auto elem : p_map ) {
                auto cls = elem.second ;
                 mdl_info->insert_classifier( cls );
                for ( auto prop : cls->own_properties () ) {
                     mdl_info->insert_property ( prop.second );
                }
            }
        } 

        
void Model_loader::insert_domain_in_app_model ( const std::shared_ptr<omm::Package>& p_dom ) const {
    
    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();

    

        // insert in the model object list
         mdl_info->insert<omm::Package> ( p_dom ) ;

        // insert in the app packages list
        auto app_info = common::Singleton<common::App_info>::instance().object();
        app_info->insert<omm::Package> ( p_dom ) ;

        //compute all properties and specializations for all classifiers ( class + association)
        for ( auto cls: p_dom->classes() ) {
            cls.second->make_all_elements_as_property(true);
            cls.second->make_more_specific_ids(true);
        }
        for ( auto assoc: p_dom->associations() ) {
            assoc.second->make_all_elements_as_property(true);
            assoc.second->make_more_specific_ids(true);
        }

        if (p_dom !=  mdl_info->standard_domain()) {
            auto domain_cfg = app_info->create_domain_config (p_dom);
            if (domain_cfg) {
                domain_cfg->create_select_templates ();
            } else {
                 
            }
        }
        // insert the classifiers, their properties in the object model list
        insert_classifiers_in_app_model<omm::Enumeration> (p_dom->enumerations());
        
        insert_classifiers_in_app_model<omm::Data_type> (p_dom->data_types());
        
        insert_classifiers_in_app_model<omm::Class> (p_dom->classes());
        
        insert_classifiers_in_app_model<omm::Association> (p_dom->associations());
        
    
}

void Model_loader::update_domain_in_app_model ( const std::shared_ptr<omm::Package>& p_dom ) const {
    
    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
        auto app_info = common::Singleton<common::App_info>::instance().object();

   
        //compute all properties and specializations for all classifiers ( class + association)
    
        for ( auto cls: p_dom->classes() ) {
            cls.second->reset_all_elements_as_property();
            cls.second->reset_more_general_classifiers();
        }
        for ( auto assoc: p_dom->associations() ) {
            assoc.second->reset_all_elements_as_property();
            assoc.second->reset_more_general_classifiers();
        }
        
        for ( auto cls: p_dom->classes() ) {
            cls.second->make_all_elements_as_property(true);
            cls.second->make_more_specific_ids(true);
        }
        for ( auto assoc: p_dom->associations() ) {
            assoc.second->make_all_elements_as_property(true);
            assoc.second->make_more_specific_ids(true);
        }

    if (  p_dom !=  mdl_info->standard_domain() ) {
        app_info->erase_domain_config ( p_dom ) ; // erase to recompute new configs

        auto domain_cfg = app_info->create_domain_config ( p_dom );
        if ( domain_cfg ) {
            domain_cfg->create_select_templates ();
        } else {
             
        }
    }
        // insert the new classifiers and the new properties in the object model list
        insert_classifiers_in_app_model<omm::Enumeration> (p_dom->enumerations());
        
        insert_classifiers_in_app_model<omm::Data_type> (p_dom->data_types());
        
        insert_classifiers_in_app_model<omm::Class> (p_dom->classes());
        
        insert_classifiers_in_app_model<omm::Association> (p_dom->associations());
        

   
    }
    


void Model_loader::associate_domain_app ( const std::shared_ptr<omm::Package>& p_dom ) const {

    auto app_info = common::Singleton<common::App_info>::instance().object();

    auto current_app = app_info->app();

    if ( current_app->id() != app_info->default_app_id() ) {


        Model_recorder  mdl_rcd ;


        try {
            mdl_rcd.store_app_domain ( p_dom, current_app );
        } catch ( const exception::Db_exception &e ) {
            mdl_rcd.rollback();
             
            throw e;
        } catch ( const exception::Base_exception &e ) {
            mdl_rcd.rollback();
             
            throw e;
        } catch ( std::exception const &e ) {
            mdl_rcd.rollback();
             
            throw e;
        }

    } // mdl_rcd destroyed, transaction committed automatically
}
