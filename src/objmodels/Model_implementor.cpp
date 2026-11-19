/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "model/xam.h"
#include "omm/Primitive_type.h"
#include "omm/Selection_type.h"
#include <omm/Composite_type.h>
#include <omm/Enumeration.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Column.h>
#include <omm/Column_constraint.h>
#include <omm/Property.h>
#include <omm/Specific_type.h>
#include <omm/Logical_schema.h>
#include <omm/Table.h>
#include <omm/Generalization.h>
#include <omm/Attribute.h>
#include <omm/Property_attributes.h>
#include <model/Model_element_create.h>
#include <model/Model_element_update.h>
#include <model/Model_element_delete.h>
#include <model/Model_implementor.h>
#include <model/Model_recorder.h>
#include <common/Db_client.h>
#include <omm/Model_factory.h>
#include <model/Classifier_search.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <util/String.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <model/Root_loader.h>
#include <model/Util.h>
#include <qry/Util.h>








using namespace arolios;
using namespace arolios::model;

Model_implementor::Model_implementor(const Model_recorder& p_mdl_rcd) : mdl_rcd_ (p_mdl_rcd) {

}

Model_implementor::~Model_implementor() {

}
void Model_implementor::visit (const Root_loader& p_obj) {}

void Model_implementor::visit (const Model_element_create<Domain_t, omm::Package >& p_obj) {
    
//void Model_implementor::visit(const Domain_create& p_store) {
    
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    // create schema
    auto stored = p_obj.stored();
	const std::string schema_name = Util::make_schema_name( stored);
    
    try {
        std::string stmt = "CREATE SCHEMA " + schema_name ;
        

         auto r = transPtr->execSqlSync ( stmt );
        
        std::shared_ptr<omm::Logical_schema> sch = omm::Model_factory::instance().make_logical_schema (stored,schema_name);
        r = transPtr->execSqlSync ("INSERT INTO am_schema (l_package, l_name) VALUES ($1,$2) RETURNING l_package",util::String::stoui(stored->id()), schema_name);
        if ( r.size() == 0 ) {
            std::string error_msg = "Error in implementing domain " + stored->name();
            throw exception::Sql_error ( error_msg );
        }
        
      
        
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }


}
void Model_implementor::visit(const Model_element_update<Domain_t,omm::Package>& p_store) {
    
    // No impact, the schema keeps the  same name

   
}

void Model_implementor::visit(const Model_element_delete<Domain_t,omm::Package>& p_store) {
    
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

    const auto stored = p_store.stored();

    
    try {
        std::string stmt = "DROP SCHEMA " + stored->schema()->name() ;
        

         auto r = transPtr->execSqlSync ( stmt );
        
        
            
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }


}



void Model_implementor::visit (const Model_element_create<Class_t, omm::Class >& p_store) {
    


    try {
        std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

        std::shared_ptr<omm::Class> class_ptr = p_store.stored();
        bool is_root = ! class_ptr->has_generalizations();
        std::string qualified_name;
        std::shared_ptr<omm::Table> table;
        
        if ( is_root ) {
            // no base class => create new table

            qualified_name = Util::make_logical_struct_full_name ( class_ptr );

            std::string stmt = "CREATE TABLE " + qualified_name + + " ( id bigserial PRIMARY KEY, ci_uuid uuid UNIQUE, upd_count integer DEFAULT 1, trashed_date timestamp with time zone DEFAULT 'epoch', csf_id integer REFERENCES am_p_elem ON DELETE RESTRICT NOT NULL)" ;
            

            auto r = transPtr->execSqlSync ( stmt );

        } else {
            // to extend the table associated to the base class with these own properties (w/o association members)
            // simple inheritance, iterator must return only one class
            for ( auto i = class_ptr->generalizations().begin(); i != class_ptr->generalizations().end(); ++i ) {
                table = i->second->general()->table();
                qualified_name =  table->full_name();
            }
        }
       
        if (is_root) {
            const std::string table_name = Util::make_logical_struct_name (class_ptr) ;
            table = omm::Model_factory::instance().make_table (class_ptr, table_name);
        } else {
            class_ptr->table(table);
            table->classes().push_back(class_ptr);
        }
        execute_insert_for_classifier (class_ptr);

    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
        
}

void Model_implementor::visit(const Model_element_update<Class_t,omm::Class >& p_store) {
    
    //  no impact, the table keeps the same name

       
            
            
}

void Model_implementor::visit(const Model_element_delete<Class_t,omm::Class >& p_store) {
    

        std::shared_ptr<omm::Class> class_ptr = p_store.stored();
        bool is_root = ! class_ptr->has_generalizations();

        
        if ( is_root ) {
            // no base class => create new table

            delete_classifier (class_ptr, "TABLE") ;
        }
            
            
}

void Model_implementor::visit (const Model_element_create<Enumeration_t,omm::Enumeration >& p_store) {
 // create enum type
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    auto enum_stored = p_store.stored();

    // creation of type composed with a type enum and the possible translations (one for each language)
	std::string qualified_name = Util::make_logical_struct_full_name (enum_stored);
	std::string stmt = "CREATE TYPE " + qualified_name + " AS ENUM () ";
    

	try {
        
        auto r = transPtr->execSqlSync ( stmt );
        
        const std::string enum_name = Util::make_logical_struct_name (enum_stored);
        auto logical_enum = omm::Model_factory::instance().make_logical_enum (enum_stored, enum_name );
        execute_insert_for_classifier (enum_stored);

    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}   

void Model_implementor::visit(const Model_element_update<Enumeration_t,omm::Enumeration >& p_store) {
     //  no impact, the type keeps the same name


}


void Model_implementor::visit(const Model_element_delete<Enumeration_t,omm::Enumeration >& p_store) {
    
        
    const auto stored = p_store.stored();

    // enumeration implemented in a type ( ... as enum ...)
    
    delete_classifier (stored, "TYPE");

}





void Model_implementor::visit (const Model_element_create<Type_t,omm::Composite_type >& p_store) {


     std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    
    auto type_stored = p_store.stored();

        std::string qualified_name = Util::make_logical_struct_full_name (type_stored);
        // create composite type empty
        std::string stmt = "CREATE TYPE " + qualified_name + " AS ()" ;
               

        try {
            
            auto r = transPtr->execSqlSync ( stmt );

                        const std::string type_name = Util::make_logical_struct_name (type_stored);
            auto logical_type = omm::Model_factory::instance().make_logical_type (type_stored, type_name);
            execute_insert_for_classifier (type_stored);

 
        }
        catch ( const exception::Db_exception &e ) {
             
            throw e;
        }

	

}


void Model_implementor::visit(const Model_element_update<Type_t,omm::Composite_type >& p_store) {
         //  no impact, the type keeps the same name

}

void Model_implementor::visit(const Model_element_delete<Type_t,omm::Composite_type >& p_store) {
    
        
    const auto stored = p_store.stored();
    
    delete_classifier (stored, "TYPE");
} 

void Model_implementor::visit (const Model_element_create<Primitive_t,omm::Primitive_type >& p_store) {
    // primitive type : native type, no RDB object
}

void Model_implementor::visit(const Model_element_update<Primitive_t,omm::Primitive_type >& p_store) {
    //  no impact, the type keeps the same name
}

void Model_implementor::visit(const Model_element_delete<Primitive_t,omm::Primitive_type >& p_store) {
    
    // nothing to do for a primitive    

} 






void Model_implementor::visit ( const Model_element_create<Association_t, omm::Association >& p_store ) {

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

    const std::shared_ptr<omm::Association> stored = p_store.stored();
    const std::shared_ptr<omm::Association> base = stored->first_base_association();

    // own declared member ends
    const auto& xml_ends = p_store.xml_object().memberEnd();

    // inherited member ends (from the base association, if any)
    std::vector<std::shared_ptr<omm::Property>> inherited;
    if ( base ) {
        for ( auto && me : *base->member_ends() ) {
            inherited.push_back ( me );
        }
    }
    const std::size_t effective_count = xml_ends.size() + inherited.size();

    // an abstract association without a full pair of member ends has no physical mapping
    // (a concrete association with less than 2 effective member ends is rejected earlier)
    if ( stored->is_abstract() && effective_count < 2 ) {
        return;
    }

    // when the base association already owns a physical mapping, the derived association shares it
    if ( base ) {
        if ( base->managing_table_class() ) {
            stored->managing_table_class ( base->managing_table_class() );
            return;
        }
        if ( base->table() ) {
            stored->table ( base->table() );
            return;
        }
    }

    // otherwise the mapping is decided from the effective member ends
    struct End_desc {
        bool many;
        bool required;
        std::shared_ptr<const omm::Class> target;
    };
    std::vector<End_desc> ends;
    for ( std::size_t k = 0; k < xml_ends.size(); ++k ) {
        const Member_end_t& me = xml_ends[k];
        Classifier_search search ( me.class_(), stored->package() );
        auto target = search.execute<omm::Class>();
        ends.push_back ( End_desc { me.maxOccurs() == "many", me.minOccurs() == "one", target } );
    }
    for ( auto && me : inherited ) {
        auto target = std::dynamic_pointer_cast<const omm::Class>( me->type() );
        ends.push_back ( End_desc { me->max_occurs() > 1, me->min_occurs() > 0, target } );
    }

    if ( ends.size() != 2 ) {
        std::string error_msg = "Association " + stored->name() + " does not have 2 effective member ends";
        throw exception::Model_error ( error_msg );
    }

    try {

        bool no_impl = true;

        if ( ends[0].many && ! ends[1].many ) {
            // one-to-many
            // assoc implemented in the target of the member[0]
            stored->managing_table_class ( ends[0].target );

        } else if ( ! ends[0].many && ends[1].many ) {
            // many-to-one
            // assoc implemented in the target of the member[1]
            stored->managing_table_class ( ends[1].target );

        } else if ( ! ends[0].many && ! ends[1].many ) {
            // one-to-one
            if ( ends[1].required && ! ends[0].required ) {
                // case #1: 1,1 <-> 0,1, assoc implemented in the target of the member[0]
                stored->managing_table_class ( ends[0].target );

            } else if ( ends[0].required && ! ends[1].required ) {
                // case #2: 0,1 <-> 1,1, assoc implemented in the target of the member[1]
                stored->managing_table_class ( ends[1].target );

            } else if ( ! ends[0].required && ! ends[1].required ) {
                // case #3: 0,1 <-> 0,1, assoc implemented in the target of the member[1]
                stored->managing_table_class ( ends[1].target );

            } else {
                // case #4: 1,1 <-> 1,1, cannot be implemented. Decision to implement the case #2
                stored->managing_table_class ( ends[1].target );
            }
        } else {
            // many-to-many
            // => add a complementary table to keep the relationship bw the classes

            std::string table_full_name = Util::make_logical_struct_full_name ( stored );

            std::string stmt = "CREATE TABLE " + table_full_name + + " ( id bigserial PRIMARY KEY, ai_uuid uuid UNIQUE, upd_count integer DEFAULT 1, trashed_date timestamp with time zone DEFAULT 'epoch', csf_id integer REFERENCES am_p_elem ON DELETE RESTRICT NOT NULL )" ;

            
            auto r = transPtr->execSqlSync ( stmt );

            const std::string table_name = Util::make_logical_struct_name ( stored );
            auto && table = omm::Model_factory::instance().make_table ( stored, table_name );

            execute_insert_for_classifier ( stored );
            no_impl = false;
        }

        if ( no_impl ) {
            execute_insert_for_classifier_with_no_logical ( stored );
        }

        

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}

void Model_implementor::visit(const Model_element_update<Association_t,omm::Association >& p_store) {

  // no impact the table keeps the same name  
   
}

void Model_implementor::visit(const Model_element_delete<Association_t,omm::Association >& p_store) {
    
        
    const auto stored = p_store.stored();
    if ( ! stored->has_generalizations() && stored->logical_struct() ) {
        // only a root association owns its logical structure (table)
        delete_classifier (stored, "TABLE");
    }
} 


void Model_implementor::visit (const Model_element_create<Value_t,omm::Enum_value >& p_store) {
    
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

	const std::string enum_name = p_store.stored()->enumeration()->logical_struct()->full_name();
    
    try {
        const std::string value = "'" + util::String::double_quote ( p_store.stored()->name() ) + "'";
        std::string stmt = "ALTER TYPE " + enum_name + " ADD VALUE IF NOT EXISTS " + value ;
        

        auto r = transPtr->execSqlSync ( stmt );
               
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }


}

void Model_implementor::visit (const Model_element_update<Value_t,omm::Enum_value >& p_store) {
    
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

	const std::string enum_name = p_store.stored()->enumeration()->logical_struct()->full_name();

    if ( !p_store.new_name().empty() ) {
        try {
            const std::string old_value = "'" + util::String::double_quote ( p_store.stored()->name() ) + "'";
            const std::string new_value = "'" + util::String::double_quote ( p_store.new_name() ) + "'";
            std::string stmt = "ALTER TYPE " + enum_name + " RENAME VALUE " + old_value + " TO " + new_value;
            

            auto r = transPtr->execSqlSync ( stmt );

        } catch ( const exception::Db_exception &e ) {
             
            throw e;
        }

    }
}

void Model_implementor::visit (const Model_element_delete<Value_t,omm::Enum_value >& p_store) {
    
 // nothing to do


}

void Model_implementor::visit (const Model_element_create<Type_property_t,omm::Property >& p_store) {
    
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

	const std::string type_name = p_store.stored()->classifier()->logical_struct()->full_name();
    const std::string attr_name = Util::make_logical_field_name (p_store.stored());
    std::string stmt = "ALTER TYPE " + type_name + " ADD ATTRIBUTE " ;
    prepare_declaration_for_column(stmt, p_store.stored(),attr_name );
    


    stmt.erase(stmt.length()-1,1);
    stmt += " CASCADE";

        auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(p_store.stored()->type());
    if (enum_ptr) {
        // property is an enum, add column for value id, useful for translations
        post_prepare_declaration_for_enum_attribute ( stmt, p_store.stored(), attr_name);
    }
    
    try {
        

        auto r = transPtr->execSqlSync ( stmt );
               
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    
    int native_type = qry::Util::property_native_type(p_store.stored());
    auto && att = omm::Model_factory::instance().make_attribute (p_store.stored(), attr_name, native_type);
                
    execute_insert_for_property (p_store.stored(), p_store.stored()->classifier());

}

void Model_implementor::visit (const Model_element_update<Type_property_t,omm::Property >& p_store) {
    // no impact , the field keeps the same name
    
}

void Model_implementor::visit (const Model_element_delete<Type_property_t,omm::Property >& p_store) {
    
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

	const std::string type_name = p_store.stored()->classifier()->logical_struct()->full_name();
    const std::string attr_name = Util::make_logical_field_name (p_store.stored());
    const std::string stmt = "ALTER TYPE " + type_name + " DROP ATTRIBUTE " + attr_name + " RESTRICT";
    
    
    try {
        

        auto r = transPtr->execSqlSync ( stmt );
               
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }


}

void Model_implementor::visit (const Model_element_create<Class_property_t,omm::Property >& p_store) {
    
    if (std::dynamic_pointer_cast<const omm::Specific_type>(p_store.stored()->type())) {
        throw exception::Model_error("property " + p_store.stored()->name() + " : specific type is not supported by the SQL implementation");
    }

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    std::shared_ptr<const omm::Classifier> impl_csf = p_store.stored()->classifier();
    auto table = impl_csf->table();
    if ( !table) {
        // property of assoc w/o table
        const auto assoc = std::dynamic_pointer_cast<omm::Association>(p_store.stored()->classifier());
        table = assoc->managing_table();
        impl_csf = assoc->managing_table_class();
    }         // else  property of a class or assoc with its own table
    
	const std::string table_name = table->full_name();
    bool is_root = !impl_csf->has_generalizations();

    bool classifier_has_own_table  = impl_csf == p_store.stored()->classifier();
   
    const std::string col_name = (classifier_has_own_table) ?
    Util::make_logical_field_name (p_store.stored(), is_root) :
    Util::make_column_name_with_classifier (p_store.stored(), impl_csf);
    std::string stmt = "ALTER TABLE " + table_name + " ADD COLUMN " ;
    prepare_declaration_for_column(stmt, p_store.stored(),col_name , (is_root && (p_store.stored()->min_occurs() == 1) && classifier_has_own_table)); // Not null only if classifier is root, classifier has own table and min=1
   
    
    auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(p_store.stored()->type());
    if (enum_ptr) {
        // property is an enum, add column for value id, useful for translations
        post_prepare_declaration_for_enum_column ( stmt, p_store.stored(), col_name);
    }

    stmt.erase(stmt.length()-1,1);

    try {
        

        auto r = transPtr->execSqlSync ( stmt );

        int native_type = qry::Util::property_native_type ( p_store.stored() );
        auto && column = omm::Model_factory::instance().make_column ( p_store.stored(), col_name, table , native_type);

        execute_insert_for_property ( p_store.stored(), impl_csf );

        if ( (!is_root || !classifier_has_own_table) && ( p_store.stored()->min_occurs() == 1 ) ) {
            auto && cctt =  omm::Model_factory::instance().make_column_constraint ( column, omm::Column_constraint::Type::not_null );
            execute_insert_column_constraint ( cctt );
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}

void Model_implementor::visit ( const Model_element_update<Class_property_t,omm::Property >& p_store ) {

    if ( p_store.stored()->column() ) {
        std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

        const std::string table_name = p_store.stored()->column()->table()->full_name();
        const std::string col_name = p_store.stored()->column()->name();

        // no impact the column keeps the same name

       

        omm::Property_attributes attrs;

        attrs.min_ = p_store.xml_object().required() ? 1 : 0;

       

        if ( attrs.min_ !=  p_store.stored()->min_occurs() ) {
            // not null constraint must be updated
            auto  column = p_store.stored()->column();

            if ( attrs.min_ == 1 ) {
                // add column in constraint
                auto && cctt =  omm::Model_factory::instance().make_column_constraint ( column, omm::Column_constraint::Type::not_null );
                execute_insert_column_constraint ( cctt );
            } else {
                // column constraint must be removed
                column->erase_constraint ( omm::Column_constraint::Type::not_null );
                execute_delete_column_constraint ( column, omm::Column_constraint::Type::not_null );
            }
        }
    }
}

void Model_implementor::visit (const Model_element_delete<Class_property_t,omm::Property >& p_store) {
    
    if ( p_store.stored()->column() ) {
        std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

        const std::string table_name = p_store.stored()->column()->table()->full_name();
        bool is_root = ! p_store.stored()->classifier()->has_generalizations();
        const std::string col_name = p_store.stored()->column()->name();
        const std::string stmt = "ALTER TABLE " + table_name + " DROP COLUMN " + col_name + " RESTRICT";


        try {
            

            auto r = transPtr->execSqlSync ( stmt );

        } catch ( const exception::Db_exception &e ) {
             
            throw e;
        }

    }
}

void Model_implementor::create_member_end_column ( const std::shared_ptr<omm::Association>& p_assoc, const std::shared_ptr<omm::Property>& p_prop ) const {

    auto managing = p_assoc->managing_table();
    if ( ! managing ) {
        // no physical mapping: abstract association with less than 2 effective member ends
        return;
    }

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    const std::string table_name = managing->full_name();

    std::string stmt = "ALTER TABLE " + table_name + " ADD COLUMN " ;
    try {
        if ( p_assoc->table() ) {
            const std::string column_name = Util::make_logical_field_name ( p_prop );
            // assoc is root, cascade delete if referenced obj deleted, not null, many-many => not unique
            prepare_declaration_for_reference_column ( stmt, p_prop, column_name, table_name, true, true, true, false, p_assoc );

            // add csf_id for the reference
            stmt += " ADD COLUMN " + column_name + "_csf_id integer";

            auto r = transPtr->execSqlSync ( stmt );

            util::Native_type ref_type = util::Native_type::bigint;
            auto column = omm::Model_factory::instance().make_column ( p_prop, column_name, p_assoc->table(), static_cast<int> ( ref_type ) );

            auto && cctt =  omm::Model_factory::instance().make_column_constraint ( column, omm::Column_constraint::Type::unique );

            execute_insert_for_property ( p_prop, p_assoc ) ;

        } else {
            // association implemented in the table of one of its member ends
            auto impl_class = p_assoc->managing_table_class();

            std::shared_ptr<omm::Property> opposite = p_assoc->opposite_member_end ( p_prop );

            if ( p_prop->type()->id() == impl_class->id() && opposite) {
                // target of the member end is the implementing class: the member end is implemented by a query
                auto && qry_field = omm::Model_factory::instance().make_property_by_sub_field ( p_prop, opposite );

                execute_insert_for_property_by_sub_field ( p_prop );

            } else {
                bool is_root = !impl_class->has_generalizations();
                const std::string column_name = Util::make_column_name_for_association ( p_prop, impl_class, p_assoc );

                // the reference is  a column, column type is bigint
                util::Native_type ref_type = util::Native_type::bigint;
                auto && column = omm::Model_factory::instance().make_column ( p_prop, column_name, p_assoc->managing_table(), static_cast<int> ( ref_type ) );

                const bool opposite_composition = opposite ? ( opposite->aggregation_kind() == omm::Aggregation_kind::composite ) : false;
                const bool opposite_one = opposite ? ( opposite->max_occurs() == 1 ) : false;
                prepare_declaration_for_reference_column ( stmt, p_prop, column_name, table_name, is_root, opposite_composition, ( p_prop->min_occurs() > 0 ), opposite_one, impl_class );

                execute_insert_for_property ( p_prop, impl_class );

                stmt += " ADD COLUMN "+ column_name + "_trashed_date timestamp with time zone DEFAULT 'epoch' , ADD COLUMN " + column_name + "_csf_id integer";

                auto r = transPtr->execSqlSync ( stmt );

            }

        }


    } catch ( const exception::Db_exception &e ) {

        throw e;
    }


}

void Model_implementor::visit ( const Model_element_create<Member_end_t,omm::Property >& p_store ) {

    const auto & assoc = p_store.stored()->association();

    // own member end
    create_member_end_column ( assoc, p_store.stored() );

    // materialize the inherited member ends which are not yet mapped to a column
    for ( auto && me : *assoc->member_ends() ) {
        if ( me && me->association().get() != assoc.get() && ! me->column() ) {
            create_member_end_column ( assoc, me );
        }
    }
}

void Model_implementor::visit ( const Model_element_update<Member_end_t,omm::Property >& p_store ) {

    const auto &  column = p_store.stored()->column();
    if ( column ) {

        std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

        const std::string table_name = column->table()->full_name();
        const std::string col_name = column->name();

        // no impact , the column keeps the same name

       

        omm::Property_attributes attrs;

        attrs.is_scp_ = true;
        attrs.aggregation_ = p_store.xml_object().isComposition() == true ? omm::Aggregation_kind::composite : omm::Aggregation_kind::none;

      


        if ( attrs.aggregation_ != p_store.stored()->aggregation_kind() ) {
            // aggregation_kind has changed, constraint ON DELETE CASCADE must be changed. Firstly, drop reference constraint
            const std::string stmt = "ALTER TABLE " + table_name + " DROP CONSTRAINT IF EXISTS " + p_store.stored()->id() + "_ref_ctt " ;
            
            transPtr->execSqlSync (stmt ) ;

            std::string ref_table_name = Util::make_logical_struct_full_name ( p_store.stored()->type() );

            if ( attrs.aggregation_ == omm::Aggregation_kind::composite ) {
                // add on delete cascade
                const std::string stmt = "ALTER TABLE " + table_name + " ADD CONSTRAINT " + p_store.stored()->id() + "_ref_ctt FOREIGN KEY ( " + col_name + " ) REFERENCES " + ref_table_name + " ON DELETE CASCADE "; 
                
                transPtr->execSqlSync (stmt  );
            } else {
                // simple reference
                const std::string stmt = "ALTER TABLE " + table_name + " ADD CONSTRAINT " + p_store.stored()->id() + "_ref_ctt FOREIGN KEY ( " + col_name + " ) REFERENCES " + ref_table_name;
                
                transPtr->execSqlSync ( stmt);
            }
        }
    }

}

void Model_implementor::visit ( const Model_element_delete<Member_end_t,omm::Property >& p_store ) {

        if ( p_store.stored()->column() ) {
            std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

            const std::string table_name = p_store.stored()->classifier()->logical_struct()->full_name();
            const std::string col_name = p_store.stored()->column()->name();
            // the following recomputation is at risk , get the colmun name from the member_end
            const std::string stmt = "ALTER TABLE " + table_name + " DROP COLUMN " + col_name + " RESTRICT, DROP COLUMN " + col_name + "_csf_id, DROP COLUMN IF EXISTS " + col_name + "_trashed_date";


            try {
                
                auto r = transPtr->execSqlSync ( stmt );

            } catch ( const exception::Db_exception &e ) {
                 
                throw e;
            }

        }
    }



void Model_implementor::delete_classifier (const std::shared_ptr<omm::Classifier>& p_elem, const std::string& p_elem_type) const {

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    
    try {
        std::string stmt = "DROP " + p_elem_type + " " + p_elem->logical_struct()->full_name() ;
        

         auto r = transPtr->execSqlSync ( stmt );
            
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    
    


}

	
void Model_implementor::execute_insert_for_classifier ( const std::shared_ptr<const omm::Classifier>& p_elem )  const {

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    auto parent = p_elem->package();
    auto && qname = p_elem->logical_struct()->name();
    std::string logical_type = p_elem->logical_struct()->type_name();
    
    
    auto r = transPtr->execSqlSync ( "INSERT INTO am_csf_struct (l_elem, l_type, l_name, l_parent) VALUES ($1,$2,$3,$4) RETURNING l_elem",util::String::stoui(p_elem->id()), logical_type, qname, util::String::stoui(parent->id()) );
    if ( r.size() == 0 ) {
        std::string error_msg = "Error in implementing classifier " + p_elem->name();
        throw exception::Sql_error ( error_msg );
    }
}

void Model_implementor::execute_insert_for_classifier_with_no_logical ( const std::shared_ptr<const omm::Classifier>& p_elem )  const {

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    
    std::string logical_type = "null";
    
    auto r = transPtr->execSqlSync ( "INSERT INTO am_csf_struct (l_elem, l_type) VALUES ($1,$2) RETURNING l_elem",util::String::stoui(p_elem->id()), logical_type );
    if ( r.size() == 0 ) {
        std::string error_msg = "Error in implementing classifier " + p_elem->name();
        throw exception::Sql_error ( error_msg );
    }
}


void Model_implementor::prepare_declaration_for_column (std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_column_name, bool p_required)  const {
	std::string type_name = Util::make_column_type_full_name (p_prop->type());
	p_stmt += p_column_name + " " + type_name;
    if (p_required) {
        p_stmt += " CONSTRAINT _" + p_prop->id() + "_not_null_ctt NOT NULL";
    }
    p_stmt += ",";
}


void Model_implementor::post_prepare_declaration_for_enum_column (std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_column_name)  const {
	p_stmt +="ADD COLUMN " + p_column_name + "_val_id integer REFERENCES am_enum_val ON DELETE RESTRICT " ;

    p_stmt += ",";
}

void Model_implementor::post_prepare_declaration_for_enum_attribute ( std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_attr_name)  const {
   p_stmt +=", ADD ATTRIBUTE " + p_attr_name + "_val_id integer CASCADE " ;

}
void Model_implementor::execute_insert_for_property ( const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const omm::Classifier>& p_struct_csf )  const {

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    // the implementation parent classifier may not be the property classifier
    
    auto && qname = p_prop->logical_field()->name();
    std::string logical_type = p_prop->logical_field()->type_name() ;

    auto r = transPtr->execSqlSync ( "INSERT INTO am_prop_field (l_property, l_type, l_name, l_parent) VALUES ($1,$2,$3,$4) RETURNING l_property",util::String::stoui(p_prop->id()) , logical_type, qname, util::String::stoui(p_struct_csf->id()) );
    if ( r.size() == 0 ) {
        std::string error_msg = "Error in implementing property " + p_prop->name();
        throw exception::Sql_error ( error_msg );
    }
}

void Model_implementor::execute_insert_for_property_by_sub_field ( const std::shared_ptr<const omm::Property>& p_prop
)  const {

    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
   
    if (p_prop->has_sub_field() ) {
        auto && qname = p_prop->property_by_sub_field()->filtering_column()->name();
        std::string logical_type =  p_prop->property_by_sub_field()->type_name() ;
        auto filter_csf = p_prop->type();

        auto r = transPtr->execSqlSync ( "INSERT INTO am_prop_field (l_property, l_type, l_name, l_parent) VALUES ($1,$2,$3,$4) RETURNING l_property",util::String::stoui(p_prop->id()) , logical_type, qname, util::String::stoui(filter_csf->id()) );
        if ( r.size() == 0 ) {
            std::string error_msg = "Error in implementing property " + p_prop->name();
            throw exception::Sql_error ( error_msg );
        }
    } else {
        std::string error_msg = "Error in implementing property " + p_prop->name() + ". it is not a property by query";
        throw exception::Failure ( error_msg );
    }   
}



void Model_implementor::prepare_declaration_for_reference_column (std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_column_name, const std::string& p_table_name, const bool is_root, const bool p_delete_cascade, const bool p_not_null, const bool p_unique, const std::shared_ptr<const omm::Classifier>& p_class) const  {

    auto root_cls = Util::find_root_classifier(p_prop->type());
    
    std::string RDB_type_name = Util::make_column_type_full_name (root_cls);

	p_stmt += p_column_name + " bigint CONSTRAINT _" + p_prop->id() + "_ref_ctt REFERENCES " + RDB_type_name ;
    
    if ( p_delete_cascade ) {
        p_stmt += " ON DELETE CASCADE ";
    }
    
    if (p_unique) {
        p_stmt += " CONSTRAINT _" + p_prop->id() + "_unique_ctt UNIQUE ";
    }

    if ( p_not_null ) {
        if ( is_root ) {
            p_stmt += " CONSTRAINT _" + p_prop->id() + "_not_null_ctt NOT NULL ";
        } else {
            auto && cctt =  omm::Model_factory::instance().make_column_constraint (p_prop->column(), omm::Column_constraint::Type::not_null);
            
        }
    }
     
     p_stmt += ",";
}

void Model_implementor::add_complementary_table (const std::shared_ptr<omm::Association>& p_assoc)  const {
   
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();
    

    std::string table_full_name = Util::make_logical_struct_full_name (  p_assoc );

    std::string stmt = "CREATE TABLE " + table_full_name + + " ( id bigserial PRIMARY KEY, ai_uuid uuid UNIQUE, upd_count integer DEFAULT 1, trashed_date timestamp with time zone DEFAULT 'epoch', csf_id integer REFERENCES am_p_elem ON DELETE RESTRICT NOT NULL , " ;
    
    std::shared_ptr<const std::vector<std::shared_ptr<omm::Property>>> members_vec = p_assoc->member_ends();

    std::string column_names[2];
    for (std::size_t k=0; k<members_vec->size() && k<2; ++k ) {
        // no inheritance column name is direct
        column_names [k] = Util::make_logical_field_name ( (*members_vec)[k]  );
        // assoc is root, cascade delete if referenced obj deleted, not null, many-many => not unique
        prepare_declaration_for_reference_column( stmt, (*members_vec)[k], column_names[k], table_full_name, true, true, true, false, p_assoc );
        
        //add_column_constraint( p_assoc, (*i), omm::Column_constraint::Type::unique);
    }
    

    stmt.replace ( stmt.length()-1,1, ")" );
     
    auto r = transPtr->execSqlSync ( stmt );


    const std::string table_name = Util::make_logical_struct_name ( p_assoc );
    auto && table = omm::Model_factory::instance().make_table ( p_assoc, table_name );

    execute_insert_for_classifier ( p_assoc );
    
    std::shared_ptr<omm::Column> columns[2] ;
    for ( std::size_t k=0; k<members_vec->size() && k<2; ++k ) {

      
            //  column type is bigint
            util::Native_type ref_type = util::Native_type::bigint;
            columns[k] = omm::Model_factory::instance().make_column ( ( *members_vec ) [k], column_names[k], table, static_cast<int>(ref_type) );

    
        auto && cctt =  omm::Model_factory::instance().make_column_constraint ( columns[k], omm::Column_constraint::Type::unique );

    }
    
    for ( std::size_t k=0; k<members_vec->size() && k<2; ++k ) {

        

            execute_insert_for_property ( ( *members_vec ) [k], p_assoc ) ;


    }
    

    
}
void Model_implementor::execute_insert_column_constraint (const std::shared_ptr<const omm::Column_constraint>& p_cctt)  const {
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

    std::string constraint_type = (p_cctt->type() == omm::Column_constraint::Type::not_null ? "not_null" : "unique");

    auto r = transPtr->execSqlSync ( "INSERT INTO am_column_constraint (l_depends_on, l_column, l_constr_type) VALUES ($1,$2,$3) RETURNING l_column",util::String::stoui(p_cctt->column()->property()->classifier()->id()) , util::String::stoui(p_cctt->column()->property()->id()), constraint_type );
    if ( r.size() == 0 ) {
        std::string error_msg = "Error in implementing column constraint " + p_cctt->column()->name();
        throw exception::Sql_error ( error_msg );
    }

}

 
void Model_implementor::execute_delete_column_constraint (const std::shared_ptr<const omm::Column>& p_column, const omm::Column_constraint::Type p_type)  const {
    std::shared_ptr<qry::Db_transaction> transPtr = mdl_rcd_.transaction();

    std::string constraint_type = (p_type == omm::Column_constraint::Type::not_null ? "not_null" : "unique");

    auto r = transPtr->execSqlSync ( "DELETE am_column_constraint WHERE l_column = $1 AND l_constr_type = $2  RETURNING l_column", util::String::stoui(p_column->property()->id()), constraint_type );
    if ( r.size() == 0 ) {
        std::string error_msg = "Error in erasing column constraint " + p_column->name();
        throw exception::Sql_error ( error_msg );
    }

}


void Model_implementor::visit (const Model_element_create<Selection_t,omm::Selection_type >& p_store) {
 
// NYI

}   

void Model_implementor::visit(const Model_element_update<Selection_t,omm::Selection_type >& p_store) {
     
// NYI

}


void Model_implementor::visit(const Model_element_delete<Selection_t,omm::Selection_type >& p_store) {
    
// NYI


}


void Model_implementor::visit (const Model_element_create<Specific_t,omm::Specific_type >& p_store) {
 
// NYI

}   

void Model_implementor::visit(const Model_element_update<Specific_t,omm::Specific_type>& p_store) {
     
// NYI

}


void Model_implementor::visit(const Model_element_delete<Specific_t,omm::Specific_type >& p_store) {
    
// NYI


}

void Model_implementor::visit (const Model_element_create<Constraint_t,omm::Constraint >& p_store) {
 
// NYI

}   

void Model_implementor::visit(const Model_element_update<Constraint_t,omm::Constraint>& p_store) {
     
// NYI

}


void Model_implementor::visit(const Model_element_delete<Constraint_t,omm::Constraint >& p_store) {
    
// NYI


}