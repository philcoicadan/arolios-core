/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_tlt_loader.h>
#include <model/xtlt.h>
#include <iostream>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <model/Model_element_translation.h>
#include <model/Root_loader.h>




using namespace arolios;
using namespace arolios::model;

Model_tlt_loader::Model_tlt_loader( ) :
    db_ (common::Singleton<common::Db_client>::instance().object())
{
}

Model_tlt_loader::~Model_tlt_loader() {

}

void Model_tlt_loader::execute ( const std::string& filename) {
    
   
    try {
    std::unique_ptr<Model_tlt> m (model_translation(filename));

    std::string lang_code = m->language();
    

    
    auto model_info = common::Singleton<common::Object_model_info>::instance().object();
    auto app_info = common::Singleton<common::App_info>::instance().object();
    
    auto language = app_info->find_language_by_code (lang_code);
    if (language == nullptr) {
        
        std::string msg ( "language " + lang_code + " not found, translation loading is not possible." );
            exception::Model_error e ( msg );
            throw e;
    }
    
    for ( Model_tlt::domain_const_iterator p ( m->domain().begin () );
            p != m->domain ().end ();
            ++p ) {
    
    
        std::string d_name = (*p).name() ;
        if (d_name[0]== '_' ) {
            auto tmp_name = d_name.erase (0,1); 
            tmp_name = util::String::standardize (tmp_name);
            d_name = tmp_name.insert(0,1,'_');
        }
        else {
            d_name = util::String::standardize(d_name);
        }
    
    
        auto dom = model_info->find_domain_by_name(d_name);
    
        bool d_exists = dom.operator bool();


        if ( !d_exists ) {

            std::string msg ( "domain " + d_name + " not found, translation loading is not possible." );
            exception::Model_error e ( msg );
            throw e;


        } else  {

            if ( dom->lc_status() != omm::Lc_status::active ) {
                std::string msg ( "domain " + d_name + " in not active, action is not possible." );
                exception::Model_error e ( msg );
                throw e;
            } else {
                load_tlt_domain ( ( *p ), dom, model_info , language);
            }
        }
    
    }
    }
    catch ( const xml_schema::parsing&  e) {
        const std::string error_msg = "Error in parsing the model file " + std::string( e.what()) ;
         
        std::cerr << e.diagnostics() << std::endl;
        throw exception::Model_error (error_msg);
    }
}

void Model_tlt_loader::load_tlt_domain ( const Domain_tlt& p_domain_tlt, const std::shared_ptr<omm::Package>& p_domain, const std::shared_ptr<common::Object_model_info>& p_info, const std::shared_ptr<const omm::Language>& p_lang) {
    
    Root_loader rl (p_info->root_domain());
    
    Model_element_translation<Domain_tlt,omm::Package> d_store (p_domain_tlt, p_domain, rl, p_lang);
    
    
    d_store.execute ();
 
        
            
        


} 
