/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <common/Db_client.h>
#include <dbinit/Loader.h>
#include <model/Model_recorder.h>
#include <model/Model_loader.h>
#include <model/Root_loader.h>
#include <model/Model_implementor.h>
#include <model/Model_element_create.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/UAC_info.h>
#include <qry/Model_retriever.h>
#include <omm/Model_factory.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <qry/Return_code.h>
#include <common/User.h>
#include <appcfg/App.h>
#include <model/xam.h>
#include <model/Root_loader.h>
#include <util/String.h>
#include <qry/Util.h>




using namespace arolios;
using namespace arolios::dbinit;

Loader::Loader(const std::string& p_json_file, const std::string& p_xml_file) 
: json_file_ (p_json_file) , xml_file_ (p_xml_file), db_ (common::Singleton<common::Db_client>::instance().object()) {
    //name_sep_ = '.'; // default separator
    
}

Loader::~Loader() {

return_code_ = qry::Return_code::KO;
}

void Loader::execute ()  {
    
        auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object () ;
        auto app_info = common::Singleton<common::App_info>::instance().object() ;
        


        if (  mdl_info->standard_domain() ) {
            throw  exception::Failure ( "Database already initialized" );
        }


        Json::Value jroot;
        std::ifstream ifs;
        ifs.open ( json_file_ );

        Json::CharReaderBuilder builder;
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if ( !parseFromStream ( builder, ifs,&jroot, &errs ) ) {
             
            throw exception::Model_error ( errs );
            return;
        }

        std::string root_pk_name = jroot.get("root_domain", "").asString();
        if (!root_pk_name.empty()) {
            root_pk_name =util::String::standardize(root_pk_name);
        }
        else {
            const std::string error_msg ( "root domain: parameter not found" );
             
            throw exception::Not_found ( error_msg );
            return;
        }
        create_root ( root_pk_name );
        
        std::string default_app_name = jroot.get("default_app", "").asString();
        if (!default_app_name.empty()) {
            default_app_name =util::String::standardize(default_app_name);
        }
        else {
            const std::string error_msg ( "default application: parameter not found" );
            std::cerr  << error_msg << std::endl;
            throw exception::Not_found ( error_msg );
            return;
        }
        create_default_app ( default_app_name) ;
        
        std::string super_user_name = jroot.get("super_user", "admin").asString();
        if (super_user_name != "admin") {
            super_user_name =util::String::standardize(super_user_name);
        }
        const auto su = create_super_user ( super_user_name) ;
        
        
        const Json::Value& languages = jroot["languages"];
        if (languages.isNull() || !languages.isArray()) {
            create_language ("fr","Français", "french", "Français");
        } else {
            for ( auto language: languages) {
                if (language.isObject()) {
                    auto code = language.get("code","").asString();
                    const auto name = language.get("name",code).asString();
                    const auto description = language.get("description","").asString();
                    const auto fts_config = language.get("fts_config",name).asString();
                    if (code.empty()) {
                        std::cout << "WARNING: Language badly defined" << std::endl;
                    } else {
                        code = util::String::standardize(code);
                        create_language (code, name, fts_config, description);
                    }
                }
            }
        }
        
        std::string default_language = jroot.get("default_language", "fr").asString();
        if ( default_language != "fr") {
            default_language = util::String::standardize(default_language);
        }
        set_default_language ( default_language) ;
                    
        
        const Json::Value& prim_map = jroot["primitives_map"];
        if (prim_map.isNull() || !prim_map.isObject()) {
            throw exception::Model_error("primitives map is missing") ;
        }
        
        map_primitive_types (prim_map);

        
        //load standard domain
        model::Model_loader std_l  ;
        std_l.execute (xml_file_);
               
        associate_primitive_native_types ();
        
        process_multilingual_types ( jroot );
        
        
        
        insert_param ("root_domain",  mdl_info->root_domain()->id());
        insert_param ("standard_domain",  mdl_info->standard_domain()->id());
        insert_param ("default_app", app_info->app()->id());
        insert_param ("default_language", app_info->default_language()->id());
        insert_param ("super_user", su->id());
    
        return_code_ = qry::Return_code::OK;
        

        
}

void Loader::create_root (const std::string& p_name) const {
    

    

    std::shared_ptr<omm::Package> root_pk = nullptr;
    // transaction for root domain
    {
        model::Model_recorder  mdl_rcd ;

        try {

            // Model_recorder keeps current transaction

            const std::string desc = "root_domain";

            // create root
            model::Root_loader rl ( std::shared_ptr<omm::Package> ( nullptr ) );
            omm::Element_attributes attrs;
            auto pers_root = mdl_rcd.create<omm::Package > ( p_name, rl.stored_element(), desc, attrs );
            // domain not chained with its parent

            root_pk  = omm::Model_factory::instance().make_package ( p_name, nullptr, omm::Lc_status::active, desc );
            root_pk->persistent_attributes ( pers_root->id(), pers_root->id_scope() );

            // transaction committing
        } catch ( const exception::Db_exception &e ) {
            mdl_rcd.rollback();
            std::cerr << e.base().what() << std::endl;
            throw e;
        } catch ( const exception::Base_exception &e ) {
            mdl_rcd.rollback();
            std::cerr << e.base().what() << std::endl;
            throw e;
        } catch ( std::exception const &e ) {
            mdl_rcd.rollback();
            std::cerr << e.what() << std::endl;
            throw e;
        }

    } // mdl_rcd destroyed, transaction committed automatically 
    
    // update Object_model
    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object () ;
     mdl_info->root_domain ( root_pk ) ;
     mdl_info->insert<omm::Package> ( root_pk );

    
}

void Loader::map_primitive_types (const Json::Value& root ) const {


    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object () ;

    for ( auto item : root.getMemberNames() ) {
        auto native_name = root.get ( item,"null" ).asString();
        if ( native_name == "null" ) {
            std::string error_msg = "Native type for primitive type " + item + " not found";
            throw exception::Not_found ( error_msg );
            break;
        } else if ( ! qry::Util::has_native_type_name ( native_name ) ) {
            std::string error_msg = "Native type " + native_name + " not supported";
            throw exception::Model_error ( error_msg );
            break;
        } else {
            
             mdl_info->map_native_primitive_type ( item, native_name );
        }
    }
}
        

     

void Loader::associate_primitive_native_types () const {

    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object () ;

    auto transPtr = db_->newTransaction();

    try {
        auto std_domain = mdl_info->standard_domain();
        for ( auto [ type_name, std_type] :  std_domain->data_types() ) {

                auto search =  mdl_info->primitive_native_types_map().find ( std_type->name() );
                if ( search !=  mdl_info->primitive_native_types_map().end() ) {

                    auto native_name = qry::Util::get_native_name(search->second);

                    auto r = transPtr->execSqlSync ( "INSERT INTO am_primitive_type (primitive, native) VALUES ($1,$2) RETURNING primitive",util::String::stoui ( std_type->id() ),native_name );
                    if ( r.size() == 0 ) {
                        std::string error_msg = "Error while associating the primitive type " + std_type->name() + " to the native type " + native_name;
                        throw exception::Sql_error ( error_msg );
                        break;
                    }
                } else {

                    // Type in standard_domain is not a primitive
                    std::cout << "WARNING: " << std_type->name() << " is not a primitive type" << std::endl;

                }
           
        }
    } catch ( const exception::Db_exception &e ) {
        transPtr->rollback();
        std::cerr << e.base().what() << std::endl;
        throw e;
    } catch ( const exception::Base_exception &e ) {
        transPtr->rollback();
        std::cerr << e.base().what() << std::endl;
        throw e;
    } catch ( std::exception const &e ) {
        transPtr->rollback();
        std::cerr << e.what() << std::endl;
        throw e;
    }

     
}

void Loader::create_default_app (const std::string& p_name) const {
    
    auto app_info = common::Singleton<common::App_info>::instance().object() ;
 ;


    try {
        auto r = db_->execSqlSync ( "INSERT INTO am_app (name) VALUES ($1) RETURNING id",p_name );
        int nb_r = r.size();
        if ( nb_r >0 ) {
            auto row = r[0];
            unsigned int id = row["id"].as<unsigned int>();
            auto app = std::make_shared<appcfg::App> ();
            app->name ( p_name );
            app->persistent_attributes ( std::to_string(id), "am_app" );
            // set the current config with this new app
            app_info->app(app);
        } else {
            std::string error_msg = "Error in recording application " + p_name;
            throw exception::Sql_error ( error_msg );
        }
    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }


    
}
const std::shared_ptr<common::User> Loader::create_super_user (const std::string& p_name) const {
    
    auto app_info = common::Singleton<common::App_info>::instance().object();
    auto uac_info = common::Singleton<common::UAC_info>::instance().object() ;
    
    // password input, declaration of specific function
    const std::function<void (bool)> echo ([] (bool on=true) ->
        void {
            struct termios settings;
            tcgetattr (STDIN_FILENO, & settings);
            settings.c_lflag = on
                ? (settings.c_lflag | ECHO)
                : (settings.c_lflag & ~(ECHO));
            tcsetattr (STDIN_FILENO, TCSANOW, &settings);
        }
    );
    // default password is the super user name +_pwd
    const std::string default_password = p_name + "_pwd";
    std::cout << "Super user password (min: 6 characters, default: <super user name>_pwd) :" ;
    std::string password;
    echo (false);
    std::getline(std::cin, password);
    echo (true);
    std::cout << std::endl;
    
    if (password.empty() ) {
        password = default_password;
    }
    if (password.length() < 6) {
        std::string error_msg = "User creation: password is too short ( minimum: 6 characters)";
         
        throw exception::Data_error (error_msg) ;
    }
    //

    const auto result = std::make_shared<common::User>();
    result->identifier(p_name);
    result->app_role (common::User_role::admin);
    const std::string role_str = uac_info->string_cast(common::User_role::admin);
    
    try {
        auto r = db_->execSqlSync ( "INSERT INTO am_user (u_identifier, u_pwd, u_role) VALUES ($1, crypt($2, gen_salt('bf')), $3) RETURNING id",result->identifier(), password, role_str );
        int nb_r = r.size();
        if ( nb_r >0 ) {
            auto row = r[0];
            unsigned int id = row["id"].as<unsigned int>();
            result->persistent_attributes (std::to_string(id), "am_user");
            
        } else {
            std::string error_msg = "Error in recording the super user " + p_name;
            throw exception::Sql_error ( error_msg );
        }
        
    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    return result;

    
}

void Loader::create_language ( const std::string& p_code, const std::string& p_name, const std::string& p_fts_config, const std::string& p_description) const {
    
    auto app_info = common::Singleton<common::App_info>::instance().object() ;
    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object () ;


    try {
        auto r = db_->execSqlSync ( "INSERT INTO am_language (code, name, fts_config) VALUES ($1,$2,$3) RETURNING id",p_code, p_name, p_fts_config);
        int nb_r = r.size();
        if ( nb_r >0 ) {
            auto row = r[0];
            unsigned int id = row["id"].as<unsigned int>();
            if ( p_description.length()>0 ) {
                auto r2 = db_->execSqlSync ("UPDATE am_language SET description = $1 WHERE id = $2 RETURNING id",p_description, id);
                if ( r2.size() == 0 ) {
                    std::string error_msg = "Error in recording language " + p_description;
                    throw exception::Sql_error ( error_msg );
                }
            }
            auto language = omm::Model_factory::instance().make_language(p_code, p_name, p_fts_config, omm::Lc_status::active, p_description);
            language->persistent_attributes ( std::to_string(id), "am_language" );
            
            app_info->insert_language(language);
            
        } else {
            std::string error_msg = "Error in recording language " + p_code;
            throw exception::Sql_error ( error_msg );
        }
    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }


    
}

void Loader::set_default_language ( const std::string& p_code) const {
    
    auto app_info = common::Singleton<common::App_info>::instance().object() ;

    auto search = app_info->find_language_by_code(p_code) ;
    if (search) {
        app_info->default_language (search) ;
    } else {
        std::string error_msg = "Default language " + p_code + " not found";
        throw exception::Not_found(error_msg) ;
    }
   
}

void Loader::insert_param (const std::string& p_key, const std::string& p_value) const {
    

    try {
        auto r1 = db_->execSqlSync ( "INSERT INTO am_init_param (i_key, i_value) VALUES ($1, $2) RETURNING i_key",p_key, p_value);
        if ( r1.size() == 0 ) {
            std::string error_msg = "Error in recording init parameter, key: " + p_key + ", value: " + p_value ;
            throw exception::Sql_error ( error_msg );
        }
        
    }
    
     catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}

void Loader::process_multilingual_types ( const Json::Value& p_jroot ) const {

    model::Model_recorder mdl_rcd ;


    try {

        const Json::Value& mltypes = p_jroot["multilingual"];
        if ( !mltypes.isNull() && mltypes.isArray() ) {

            for ( auto mltype: mltypes ) {
                if ( mltype.isObject() ) {
                    const auto name = util::String::standardize ( mltype.get ( "type_name","" ).asString() );
                    const auto type = util::String::standardize ( mltype.get ( "property_type","" ).asString() );
                    if ( name.empty() || type.empty() ) {
                        std::cout << "WARNING: multilingual type badly declared" << std::endl;
                    } else {
                        add_multi_languages ( name, type, mdl_rcd );
                    }
                } else {
                    std::cout << "WARNING: multilingual type badly declared" << std::endl;
                }
            }

        } else {
            std::cout << "WARNING: multilingual types not initialized"<< std::endl;
        }

    }// transaction committing
    catch ( const exception::Db_exception &e ) {
        mdl_rcd.rollback();
        std::cerr << e.base().what() << std::endl;
        throw e;
    } catch ( const exception::Base_exception &e ) {
        mdl_rcd.rollback();
        std::cerr << e.base().what() << std::endl;
        throw e;
    } catch ( std::exception const &e ) {
        mdl_rcd.rollback();
        std::cerr << e.what() << std::endl;
        throw e;
    }
}      // mdl_rcd destroyed, transaction committed automatically



void Loader::add_multi_languages (const std::string&  p_type_name, const std::string& p_prop_type_name, const model::Model_recorder& p_mdl_rcd) const {
    
    auto app_info = common::Singleton<common::App_info>::instance().object() ;

    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object () ;

        
    // the type and the property type must belong to the standard_domain
    const std::shared_ptr<omm::Data_type> type = app_info->find_domain_element_by_name<omm::Data_type>(  mdl_info->standard_domain()->name(), p_type_name) ;
    
    // prop type must be a primitive
    const std::shared_ptr<omm::Data_type> prop_type =  mdl_info->find_primitive_type_by_name( p_prop_type_name) ;

    
    if (type && prop_type) {
        for ( auto [code, lang] : app_info->languages() ) {
            auto pers_prop = p_mdl_rcd.store_property ( code,  util::String::stoui(type->id()), util::String::stoui(prop_type->id()), 0, 1, std::string(), std::string(), std::string() );
            
            auto prop = omm::Model_factory::instance().make_property (code, type, prop_type) ;
            prop->persistent_attributes ( pers_prop->id(), pers_prop->id_scope() ) ;
        }

        model::Root_loader type_store (type);
        for (auto prop: type->own_properties()) {
            const std::string& name = prop.first;
            model::Type_property_t xml_prop (name, p_prop_type_name) ;
            model::Model_element_create<model::Type_property_t,omm::Property> type_prop_store (xml_prop, name, type_store) ;
            type_prop_store.stored(prop.second);
            model::Model_implementor impl(p_mdl_rcd);
            type_prop_store.accept(impl);
        }
            
        
    } else {
        std::cout << "WARNING: Multilingual type " << p_type_name << "or property type " << p_prop_type_name << " not found" << std::endl;
    }
}

qry::Return_code Loader::return_code () const {
    return return_code_;
}