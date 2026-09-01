/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Model_retriever.h>
#include <json/json.h>
#include <iostream>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Enumeration.h>
#include <omm/Data_type.h>
#include <omm/Composite_type.h>
#include <omm/Package.h>
#include <omm/Model_factory.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <util/String.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <util/String.h>




using namespace arolios;
using namespace arolios::qry;

Model_retriever::Model_retriever ()
    : db_ ( common::Singleton<common::Db_client>::instance().object() ), common_obj_(common::Singleton<common::Object_model_info>::instance().object()) {


}

Model_retriever::~Model_retriever() {

}

void Model_retriever::retrieve_init( const std::string& p_root_pk_id, const std::string& p_std_pk_id) const {


 

    //lookup root domain
    auto f = db_->execSqlAsyncFuture ( "SELECT * FROM am_pk  WHERE id = $1", util::String::stoui(p_root_pk_id));

    try {
        auto result = f.get();

        if ( result.size() > 0 ) {
            // root domain found
            auto row = result[0];
            
            const std::string name = row["name"].as<std::string>() ;
            const std::string desc = (row["description"].isNull()) ? "" : row["description"].as<std::string>() ;

            const auto root_pk = omm::Model_factory::instance().make_package ( name, nullptr, omm::Lc_status::active, desc );

            root_pk->persistent_attributes(p_root_pk_id, "am_pk" ) ;
            common_obj_->insert<omm::Package> ( root_pk );
            common_obj_->root_domain( root_pk);

            // lookup standard domain
            f = db_->execSqlAsyncFuture ( "SELECT am_pk.*, am_schema.* FROM am_pk  INNER JOIN am_schema ON am_pk.id = am_schema.l_package WHERE am_pk.id = $1", util::String::stoui(p_std_pk_id) );
            result = f.get();

            if ( result.size() > 0 ) {
                // standard domain found
                auto row = result[0];
                const std::string name = row["name"].as<std::string>() ;
                const std::string desc = (row["description"].isNull()) ? "" : row["description"].as<std::string>() ;
                const auto std_pk  = omm::Model_factory::instance().make_package ( name, root_pk, omm::Lc_status::active, desc  );

                std_pk->persistent_attributes(p_std_pk_id, "am_pk" ) ;
                
                std::shared_ptr<omm::Logical_schema> sch = omm::Model_factory::instance().make_logical_schema ( std_pk, row["l_name"].as<std::string>());
                
                common_obj_->insert<omm::Package> ( std_pk );
                common_obj_->standard_domain ( std_pk);
            } else {
                std::cout << "WARNING: standard domain not found" << std::endl;
            }
        } else {
            std::cout << "WARNING: root domain not found" << std::endl;
        }
    }

    catch ( const exception::Db_exception &e ) {
        std::cerr << "error: " << e.base().what() << std::endl;

        throw e;
    }




}


void Model_retriever::retrieve_user_domains() const {
    
    
 
    const std::string std_pk_name = common_obj_->standard_domain()->name();
    
    auto f = db_->execSqlAsyncFuture("SELECT am_pk.*, am_schema.* FROM am_pk  INNER JOIN am_schema ON am_pk.id = am_schema.l_package ORDER BY am_pk.id ");

    try {
        auto result = f.get();

        if (result.size() > 0) {
            for (auto row : result) {
                std::string pk_name = row["name"].as<std::string>() ;
                if (pk_name == std_pk_name) continue; // standard domain already retrieved
                
                omm::Lc_status lc = qry::Util::convert_status (row["lc"].as<std::string>());

                
                    
                std::shared_ptr<omm::Package> nesting_pk = nullptr;
                if (!row["nesting"].isNull()) {
                    unsigned int nesting_id = row["nesting"].as<unsigned int>();
                    nesting_pk = common_obj_->find_domain_by_id(std::to_string(nesting_id));
                   
                }
                const std::string desc = (row["description"].isNull()) ? "" : row["description"].as<std::string>() ;
                
                std::shared_ptr<omm::Package> pk = omm::Model_factory::instance().make_package ( pk_name, nesting_pk, lc, desc);

                unsigned int id = row["id"].as<unsigned int>();
                pk->persistent_attributes ( std::to_string(id), "am_pk" ) ;
                std::shared_ptr<omm::Logical_schema> sch = omm::Model_factory::instance().make_logical_schema ( pk, row["l_name"].as<std::string>());
             

                common_obj_->insert<omm::Package> ( pk );
            }
        }
        else {
            std::string msg ("no user domains found");
            std::cout << "WARNING: " << msg << std::endl;

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}

}



void Model_retriever::retrieve_classifiers ( const std::shared_ptr<omm::Package>& p_pk )  const{
    
    
         
    auto f = db_->execSqlAsyncFuture("SELECT am_p_elem.*, am_primitive_type.* , am_csf_struct.* FROM am_p_elem FULL OUTER JOIN am_primitive_type ON am_p_elem.id = am_primitive_type.primitive LEFT JOIN am_csf_struct ON am_p_elem.id = am_csf_struct.l_elem WHERE am_p_elem.package=$1 ORDER BY am_p_elem.id", util::String::stoui(p_pk->id()));

    try {
        auto result = f.get();

        if (result.size() >0 ) {
             std::shared_ptr<omm::Classifier> new_classifier = nullptr;
            
            for (auto row : result) {
                const std::string elem_type = row["elem_type"].as<std::string>();
                const omm::Lc_status lc = qry::Util::convert_status (row["lc"].as<std::string>());
                const std::string desc = (row["description"].isNull()) ? "" : row["description"].as<std::string>() ;

                if (elem_type == "enum") {
                    auto && new_enum = omm::Model_factory::instance().make_enumeration(row["name"].as<std::string>(), p_pk, lc, desc);
                    new_classifier = std::static_pointer_cast<omm::Classifier>(new_enum);
                    auto && new_logical_enum = omm::Model_factory::instance().make_logical_enum (new_enum, row["l_name"].as<std::string>());
                }
                else if (elem_type == "type") {
                    auto && new_type = omm::Model_factory::instance().make_composite_type(row["name"].as<std::string>(), 
                    p_pk, lc, desc);
                    new_classifier = std::static_pointer_cast<omm::Classifier>(new_type);
                    auto && new_logical_type = omm::Model_factory::instance().make_logical_type (new_type, row["l_name"].as<std::string>());
                    // properties retrieved later
                } 
                else if (elem_type == "primitive") {
                     auto field = row["native"];
                    if (! field.isNull()) { //primitive type
                        std::string native_name = row["native"].as<std::string>();
                        auto native_type = Util::get_native_type(native_name);
                        new_classifier = omm::Model_factory::instance().make_primitive_type ( row["name"].as<std::string>(), p_pk , native_type, lc, desc);
                    } else {
                        std::string msg ( "primitive type " + row["name"].as<std::string>() + " in domain " + p_pk->name() + " not associated to a database native type " );
                        exception::Sql_error e ( msg );
                        throw e;
                    }
                }
                else if (elem_type == "class") {
                    auto && new_class = omm::Model_factory::instance().make_class ( row["name"].as<std::string>(), p_pk, row["is_abstract"].as<bool>(), lc, desc );
                    new_classifier = std::static_pointer_cast<omm::Classifier> ( new_class );
                    auto && map = p_pk->schema()->logical_structs();
                    auto search = map.find(row["l_name"].as<std::string>());
                    if (search != map.end()) {
                        auto && table = std::dynamic_pointer_cast<omm::Table>(search->second);
                        new_class->table(table);
                        table->classes().push_back(new_class);
                    } else {
                        auto && new_table = omm::Model_factory::instance().make_table (new_class, row["l_name"].as<std::string>());
                    }

                    // properties retrieved later
                }
                else if (elem_type == "association") {
                    auto && new_assoc = omm::Model_factory::instance().make_association ( row["name"].as<std::string>(), p_pk , lc, desc);
                    new_classifier = std::static_pointer_cast<omm::Classifier> ( new_assoc ); 
                    if ( row["l_type"].as<std::string>() != "null" ) {
                        auto && new_table = omm::Model_factory::instance().make_table (new_assoc, row["l_name"].as<std::string>());
                    } 
                        
                    // properties retrieved later
                }
                else {
                    std::string msg ( "classifier not found: " + elem_type );
                    exception::Sql_error e ( msg );
                    throw e;
                }
    
                unsigned int id = row["id"].as<unsigned int>();
                new_classifier->persistent_attributes ( std::to_string(id), "am_p_elem" ) ;
                if ( ! row["base"].isNull() ) {
                    unsigned int base_id = row["base"].as<unsigned int>();
                    auto && base = common_obj_->find_domain_element_by_id<omm::Class>(std::to_string( base_id) );
                    std::shared_ptr<omm::Generalization> && general = omm::Model_factory::instance().make_generalization ( base, new_classifier);
            
                }
                
 
                
                                
                common_obj_->insert_classifier(new_classifier);
                                
               
            }
        }
        else {
            std::cout << "WARNING: classifiers not found in domain: " << p_pk->name()<< std::endl;
        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}

}


void Model_retriever::retrieve_enumeration_values ( const std::shared_ptr<omm::Package>& p_pk ) const {
    

         
    auto f = db_->execSqlAsyncFuture("SELECT am_enum_val.* FROM am_enum_val INNER JOIN am_p_elem ON am_enum_val.e_elem= am_p_elem.id WHERE am_p_elem.package=$1 ORDER BY am_enum_val.id", util::String::stoui(p_pk->id()));
    
    try {
        auto result = f.get();

        if (result.size() >0 ) {
            for (auto row : result) {
                const unsigned int new_id = row["id"].as<unsigned int>();
                const std::string val = row["e_value"].as<std::string>();
                const unsigned int enum_id = row["e_elem"].as<unsigned int>();
                const omm::Lc_status lc = qry::Util::convert_status (row["lc"].as<std::string>());
                auto && enum_ptr = common_obj_->find_domain_element_by_id<omm::Enumeration>( std::to_string(enum_id) );
                
                auto new_value = omm::Model_factory::instance().make_enum_value(val, enum_ptr, lc);
                new_value->persistent_attributes ( std::to_string(new_id), "am_enum_val" ) ;
                common_obj_->insert_enum_value ( new_value );


            }
        }
        else {
            std::cout << "WARNING: enumeration values not found in domain: " << p_pk->name()<< std::endl;
        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;

       throw e;
	}
    
}

void Model_retriever::retrieve_properties (const std::shared_ptr<omm::Package>& p_pk )  const{
    
    

    
   auto f = db_->execSqlAsyncFuture("SELECT am_property.*, am_member_end.*, am_p_elem.elem_type FROM am_property LEFT JOIN am_member_end ON am_property.id= am_member_end.property INNER JOIN am_p_elem ON am_property.owning_t = am_p_elem.id WHERE am_p_elem.package=$1 ORDER BY am_property.id" , util::String::stoui(p_pk->id()));
    
    try {
        auto result = f.get();

        for ( auto row : result ) {
            std::shared_ptr<omm::Property> new_prop = nullptr;

            const std::string name = row["name"].as<std::string>();
            const omm::Lc_status lc = qry::Util::convert_status ( row["lc"].as<std::string>() );
            const std::string desc = (row["description"].isNull()) ? "" : row["description"].as<std::string>() ;

            const unsigned int parent_id = row["owning_t"].as<unsigned int>();
            auto && parent = common_obj_->find_domain_element_by_id<omm::Classifier>( std::to_string(parent_id) );

            const unsigned int type_id = row["p_type"].as<unsigned int>();
            auto && type  = common_obj_->find_domain_element_by_id<omm::Classifier> ( std::to_string(type_id) );

            const int min_occurs = row["min_occurs"].as<int>();
            const int max_occurs = row["max_occurs"].as<int>();
            const bool is_member_end = row["is_member_end"].as<bool>();
            if ( !is_member_end ) {
                //simple property
                std::string computed ( "" );
                if ( ! row["computed"].isNull() ) {
                    computed = row["computed"].as<std::string>();
                }
                std::string default_val ( "" );
                if ( ! row["default_v"].isNull() ) {
                    default_val = row["default_v"].as<std::string>();
                }
                new_prop = omm::Model_factory::instance().make_property ( name, parent, type, min_occurs, max_occurs, computed, default_val, /* part_of_id,*/ lc, desc );
            } else {
                unsigned int assoc_id = row["assoc"].as<unsigned int>();
                auto && assoc = common_obj_->find_domain_element_by_id<omm::Association> ( std::to_string(assoc_id) );


                unsigned int source_id = row["source"].as<unsigned int>();
                auto && source = common_obj_->find_domain_element_by_id<omm::Class> ( std::to_string(source_id) );
                std::shared_ptr<omm::Class> target = std::dynamic_pointer_cast<omm::Class> ( type );

                bool is_src_property = row["c_property"].as<bool>();
                bool is_composition = row["is_compo"].as<bool>();

                new_prop = omm::Model_factory::instance().make_member_end ( name,assoc,target, source, min_occurs, max_occurs, is_src_property, is_composition, /*part_of_id, */lc, desc );

            }

            unsigned int new_id = row["id"].as<unsigned int>();
            new_prop->persistent_attributes ( std::to_string(new_id), "am_property" ) ;

            common_obj_->insert_property ( new_prop );

            
        }
        
        
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;
       throw e;
	}
	
} 

void Model_retriever::retrieve_logical_fields (const std::shared_ptr<omm::Package>& p_pk )  const{
    
    

    // retrieve column and attribute fields at first because query fields can reference columns
   auto f = db_->execSqlAsyncFuture("SELECT am_prop_field.*, am_property.id FROM am_prop_field INNER JOIN am_property ON am_property.id = am_prop_field.l_property INNER JOIN am_p_elem ON am_property.owning_t = am_p_elem.id WHERE am_p_elem.package=$1 AND am_prop_field.l_type != 'sub_field' ORDER BY am_property.id" , util::String::stoui(p_pk->id()));
    
    try {
        auto result = f.get();

        for ( auto row : result ) {

            unsigned int prop_id = row["l_property"].as<unsigned int>();
           
            auto prop = common_obj_->find_property_by_id(std::to_string(prop_id));
            
            omm::Logical_field::Type type = convert_logical_field_type(row["l_type"].as<std::string>());
            int native_type = qry::Util::property_native_type(prop);
            auto table_class = common_obj_->find_domain_element_by_id<omm::Classifier>(std::to_string(row["l_parent"].as<unsigned int>()));
            switch (type) {
                case omm::Logical_field::Type::column_t: {
                    auto lf = omm::Model_factory::instance().make_column (prop, row["l_name"].as<std::string>(), table_class->table(), native_type);
                    break; }
                case omm::Logical_field::Type::attribute_t: {
                    auto lf = omm::Model_factory::instance().make_attribute (prop, row["l_name"].as<std::string>(), native_type);
                    break; }
                
                default:
                    break;
            }
        
         
                     
        
        }
    }

    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;
       throw e;
	}
	
	
    // retrieve query fields at last
   auto f2 = db_->execSqlAsyncFuture("SELECT am_prop_field.*, am_property.id FROM am_prop_field INNER JOIN am_property ON am_property.id = am_prop_field.l_property INNER JOIN am_p_elem ON am_property.owning_t = am_p_elem.id WHERE am_p_elem.package=$1 AND am_prop_field.l_type = 'sub_field' ORDER BY am_property.id" , util::String::stoui(p_pk->id()));
    
    try {
        auto result = f2.get();

        for ( auto row : result ) {

            unsigned int prop_id = row["l_property"].as<unsigned int>();
           
            auto prop = common_obj_->find_property_by_id(std::to_string(prop_id));

            auto csf = common_obj_->find_domain_element_by_id<omm::Classifier>(std::to_string(row["l_parent"].as<unsigned int>()));
            
            auto table_class = csf ?  std::dynamic_pointer_cast<omm::Class>(csf) : nullptr;
            
            if (!(table_class)) {
                std::string msg ( "property " + prop->name() + " does not refer to a class"  );
                exception::Not_found e ( msg );
                throw e;
            }
	
            auto qry = omm::Model_factory::instance().make_property_by_sub_field (prop,  table_class->table(), row["l_name"].as<std::string>());
            
            //update association having this property as member end for setting the class of the managing table
            auto assoc = prop->association ();
            if (assoc) {
                
                assoc->managing_table_class (table_class);
            } else {
                std::string msg ( "property " + prop->name() + " is not a member end"  );
                exception::Failure e ( msg );
                throw e;
            }
                 

        }
    }
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;
       throw e;
	}
	   
	
	
} 

void Model_retriever::retrieve_field_constraints (const std::shared_ptr<omm::Package>& p_pk )  const{
    
    

    
   auto f = db_->execSqlAsyncFuture("SELECT am_column_constraint.* FROM am_column_constraint INNER JOIN am_p_elem ON am_column_constraint.l_depends_on = am_p_elem.id WHERE am_p_elem.package=$1 ORDER BY am_column_constraint.l_column" , util::String::stoui(p_pk->id()));
    
    try {
        auto result = f.get();

        std::shared_ptr<omm::Property> new_prop = nullptr;

        for ( auto row : result ) {

            unsigned int cls_id = row["l_depends_on"].as<unsigned int>();
            std::shared_ptr<omm::Classifier> && cls = std::static_pointer_cast<omm::Classifier>(common_obj_->find_domain_element_by_id<omm::Class> ( std::to_string(cls_id) ));
            if (!cls ) {
                cls = std::static_pointer_cast<omm::Classifier>(common_obj_->find_domain_element_by_id<omm::Association> ( std::to_string( cls_id) ));
            }

            if (!cls) {
                 std::string msg ( "classifier " + std::to_string (cls_id) + " not found in domain: " + p_pk->name() );
                exception::Not_found e ( msg );
                throw e;
            }
            unsigned int prop_id = row["l_column"].as<unsigned int>();
            auto && prop  = common_obj_->find_property_by_id ( std::to_string(prop_id) );
            
            if (!prop) {
                 std::string msg ( "property " + std::to_string ( prop_id) + " not found in domain: " + p_pk->name() );
                exception::Not_found e ( msg );
                throw e;
            }
            
            omm::Column_constraint::Type fct = convert_column_constraint_type (row["l_constr_type"].as<std::string>());
                        
            auto new_column_constr = omm::Model_factory::instance().make_column_constraint (prop->column(),fct);

        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;
       throw e;
	}
	
} 


void Model_retriever::retrieve_identifiers (const std::shared_ptr<omm::Package>& p_pk )  const{
    
    

    
   auto f = db_->execSqlAsyncFuture("SELECT am_csf_ident.* FROM am_csf_ident INNER JOIN am_p_elem ON am_csf_ident.i_csf = am_p_elem.id WHERE am_p_elem.package=$1 ORDER BY am_csf_ident.i_csf" , util::String::stoui(p_pk->id()));
    
    try {
        auto result = f.get();

        std::shared_ptr<omm::Property> new_prop = nullptr;

        for ( auto row : result ) {

            unsigned int csf_id = row["i_csf"].as<unsigned int>();
            std::shared_ptr<omm::Classifier> csf = common_obj_->find_domain_element_by_id<omm::Classifier> ( std::to_string(csf_id) );


            if (!csf) {
                 std::string msg ( "classifier " + std::to_string (csf_id) + " not found in domain: " + p_pk->name() );
                exception::Not_found e ( msg );
                throw e;
            }
            
            unsigned int prop_id = row["i_prop"].as<unsigned int>();
            auto && prop  = common_obj_->find_property_by_id ( std::to_string(prop_id) );
            
            if (!prop) {
                 std::string msg ( "property " + std::to_string ( prop_id) + " not found in domain: " + p_pk->name() );
                exception::Not_found e ( msg );
                throw e;
            }
            
            csf->insert_identifier (prop);
            
            
        }
    }
    
    catch (const exception::Db_exception &e) {
       std::cerr << "error: " << e.base().what() << std::endl;
       throw e;
	}
	
} 



omm::Logical_field::Type Model_retriever::convert_logical_field_type (const std::string& p_type)  const{
    

    static std::unordered_map<std::string,omm::Logical_field::Type> enum_map {{"column", omm::Logical_field::Type::column_t}, {"attribute", omm::Logical_field::Type::attribute_t}, {"sub_field", omm::Logical_field::Type::sub_field_t}};
    
    return enum_map.find(p_type)->second;
}



omm::Column_constraint::Type Model_retriever::convert_column_constraint_type (const std::string& p_type) const {

     static std::unordered_map<std::string,omm::Column_constraint::Type> f_ctt_t_map {{"not_null", omm::Column_constraint::Type::not_null}, { "unique", omm::Column_constraint::Type::unique}} ;
     
     return f_ctt_t_map.find(p_type)->second;
}


