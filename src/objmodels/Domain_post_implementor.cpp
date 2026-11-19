/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Domain_post_implementor.h>
#include <model/Model_recorder.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <omm/Logical_schema.h>
#include <omm/Table.h>
#include <omm/Logical_type.h>
#include <omm/Language.h>
#include <omm/Column.h>
#include <omm/Package.h>
#include <omm/Column_constraint.h>
#include <omm/Association.h>
#include <omm/Generalization.h>
#include <omm/Attribute.h>
#include <util/String.h>
#include <exception/Exceptions.h>
#include <model/Util.h>



using namespace arolios;
using namespace arolios::model;

Domain_post_implementor::Domain_post_implementor() {

}

Domain_post_implementor::~Domain_post_implementor() {

}

void Domain_post_implementor::add_fts_column( const Model_element<Domain_t,omm::Package>& p_store) const {
    
    std::shared_ptr<qry::Db_transaction> transPtr = transaction();

    auto app_info = common::Singleton<common::App_info>::instance().object();
    
    const std::string fragment_sep = "|| ' ' || ";
    
    const auto & schema = p_store.stored()->schema();
    
    
    // iterate on all tables
    for ( auto && [ ls_name, lstruct] : schema->logical_structs() ) {
        if (lstruct->is_table() ) {
            auto table = std::dynamic_pointer_cast<omm::Table>(lstruct);
            
            // iterate on all languages
            for ( auto [ code, lang ] : app_info->languages() ) {
                std::string fts_config = lang->fts_config();

                std::string fts_fragments_str;
                //iterate on all columns
                for ( auto [ col_name, column] : table->columns() ) {
                    prepare_declaration_for_fts_column ( column, code, fts_fragments_str, fragment_sep ) ;
                }
    
                if ( !fts_fragments_str.empty() ) {
                    fts_fragments_str.erase ( fts_fragments_str.length()-fragment_sep.length(), fragment_sep.length() );
                    //Drop existing constraint if need - Assuming there is no concurrent access
                     std::string stmt = "ALTER TABLE " + table->full_name() + " DROP CONSTRAINT IF EXISTS _fts_col_" + lang->id() + "_ctt ";
                      auto r0 = transPtr->execSqlSync ( stmt );
                      
                    // add column if not exists with new constraint
                      stmt = "ALTER TABLE " + table->full_name() + " ADD COLUMN IF NOT EXISTS _fts_col_" + lang->id() + "  tsvector CONSTRAINT _fts_col_" + lang->id() + "_ctt GENERATED ALWAYS AS (to_tsvector('" + fts_config + "'," + fts_fragments_str + ")) STORED" ;

                    
                    auto r1 = transPtr->execSqlSync ( stmt );
                    
                    // Drop th existing index if need - Assuming there is no concurrent access
                    stmt = "DROP INDEX IF EXISTS " + table->schema()->name() + "._" + table->root_classifier()->id() + "_fts_col_" + lang->id() + "_idx" ;
                    auto r11 = transPtr->execSqlSync ( stmt );

                    stmt = "CREATE INDEX _" + table->root_classifier()->id() + "_fts_col_" + lang->id() + "_idx ON " + table->full_name() + " USING GIN(_fts_col_" + lang->id() + " )" ;

                    
                    auto r2 = transPtr->execSqlSync ( stmt );
                }
            }
        }
    }
}
                

    

void Domain_post_implementor::process_required_properties (const Model_element<Domain_t,omm::Package>& p_store) const {
    
    std::shared_ptr<qry::Db_transaction> transPtr = transaction();

    auto model_info = common::Singleton<common::Object_model_info>::instance().object();
    // iterate on all concrete classes 
    for ( auto && child : p_store.get_children() ) {
        auto stored = child->stored_element();
        auto cls_ptr = std::dynamic_pointer_cast<omm::Class> ( stored );
        if ( cls_ptr ) {
            // class

            // only concrete classes and require a table constraint
            if ( ! cls_ptr->is_abstract() ) {

                std::vector<std::shared_ptr<omm::Column_constraint>> cctts;
                // get the list of required properties in the classifier and its super-classifiers except the root one
                fill_column_constraints_with_inheritance ( cls_ptr, cctts, omm::Column_constraint::Type::not_null, false ) ;
                if ( cctts.size() >0 ) {
                    auto lo = cls_ptr->table();
                    auto table_name = lo->full_name ();

                    auto csf_id = cls_ptr->id();

                    

                    //Drop existing check expression if need - Assuming there is no concurrent access
                     std::string stmt = "ALTER TABLE " + table_name + " DROP CONSTRAINT IF EXISTS _" + csf_id + "_not_null_ctt ";
                      auto r = transPtr->execSqlSync ( stmt );
                      
                    stmt = "ALTER TABLE " + table_name + " ADD CONSTRAINT _" + csf_id + "_not_null_ctt CHECK ( csf_id != " + csf_id  + " OR (";
                    for ( auto&& cctt : cctts ) {
                        // get the column name
                        stmt += " " + cctt->column()->name() + " IS NOT NULL AND";
                    }
                    stmt.replace ( stmt.length()-3,3, ") )" );
                    
                    r = transPtr->execSqlSync ( stmt );
                }
            }
        }
        auto assoc_ptr = std::dynamic_pointer_cast<omm::Association>(stored);
        if (assoc_ptr) {
          // association

          // only one-one and one-many association can have a table constraint
          if (assoc_ptr->table() == nullptr) { // not a many-many association

            std::vector<std::shared_ptr<omm::Column_constraint>> cctts;
            // get the list of required properties in the classifier
            fill_column_constraints_for_not_many_many_association(assoc_ptr,
                                                                  cctts);
            if (cctts.size() > 0) {

              // get the referencing column
              auto members = assoc_ptr->member_ends();
              std::shared_ptr<omm::Column> ref_column = nullptr;
              for (std::size_t k = 0; k < members->size(); ++k) {
                ref_column = (*members)[k]->column();
                if (ref_column)
                  break;
              }

              if (ref_column) {
                auto lo = assoc_ptr->managing_table();
                auto table_name = lo->full_name();
                auto csf_id = assoc_ptr->id();

                // Drop existing check expression if need - Assuming there is no
                // concurrent access
                std::string stmt = "ALTER TABLE " + table_name +
                                   " DROP CONSTRAINT IF EXISTS _" + csf_id +
                                   "_not_null_ctt ";
                auto r = transPtr->execSqlSync(stmt);

                stmt = "ALTER TABLE " + table_name + " ADD CONSTRAINT _" +
                       csf_id + "_not_null_ctt CHECK (" + ref_column->name() +
                       " IS NULL OR (";
                for (auto &&cctt : cctts) {
                  // get the column name
                  stmt += " " + cctt->column()->name() + " IS NOT NULL AND";
                }
                stmt.replace(stmt.length() - 3, 3, ") )");

                r = transPtr->execSqlSync(stmt);
              } else {
                std::cout << "WARNING: Association " << assoc_ptr->name() << " is not mapped to a table column" << std::endl;

              }
            }
          }
        }
    }
}
void Domain_post_implementor::process_id_properties ( const Model_element<Domain_t,omm::Package>& p_store ) const {

    std::shared_ptr<qry::Db_transaction> transPtr = transaction();

    auto model_info = common::Singleton<common::Object_model_info>::instance().object();

    // iterate on all concrete classes
    for ( auto && child : p_store.get_children() ) {
        auto stored = child->stored_element();
        auto csf_ptr = std::dynamic_pointer_cast<const omm::Classifier> ( stored );
        if ( csf_ptr ) {
            // classifier

            // only concrete classes require a table constraint
            if ( ! csf_ptr->is_abstract() ) {

                std::vector<std::shared_ptr<omm::Column_constraint>> cctts;
                // get the list of id properties in the classifier and its super-classifiers
                
                fill_column_constraints_for_uniqueness (csf_ptr, cctts);
                
                //assoc N-M , add constraint for unicity if memberEnd owned by source class.
                fill_column_constraints_for_many_to_many_association( csf_ptr, cctts);

                if ( cctts.size() >0 ) {
                    auto lo =  csf_ptr->table();
                    auto table_name = lo->full_name ();
                    auto schema_name = lo->schema()->name();

                    auto csf_id = csf_ptr->id();

                    

                    // Drop th existing index if need - Assuming there is no concurrent access
                    std::string stmt = "DROP INDEX IF EXISTS "  + schema_name + "._" + csf_id + "_unique_idx";
                    auto r = transPtr->execSqlSync ( stmt );
                    
                    stmt = "CREATE UNIQUE INDEX _" + csf_id + "_unique_idx ON " + table_name + " (";
                    for ( auto&& cctt : cctts ) {
                        // get the column name
                        stmt += " " + cctt->column()->name() + ",";
                    }
                    // add column trashed_date for avoiding conflict of user-defined ids after deletion
                    stmt += "trashed_date )";
                    stmt += " WHERE  csf_id = " +  csf_id  ;

                    
                    r = transPtr->execSqlSync ( stmt );
                }
            }
        }
    }

  
}

void Domain_post_implementor::fill_column_constraints_for_uniqueness ( const std::shared_ptr<const omm::Classifier>& p_csf, std::vector<std::shared_ptr<omm::Column_constraint>>& p_columns)  const {

 
        // keep columns in the table of the classifier
        auto && csf_table = p_csf->table();
        for ( auto [id, prop] : p_csf->identifier() ) {
            auto && column = prop->column();
            if ( column ) {
                if ( column->table() == csf_table ) {
                    auto ctt = std::make_shared<omm::Column_constraint>(column, omm::Column_constraint::Type::unique);
                    p_columns.push_back ( ctt );
                        
                }
            }
        }
}

void Domain_post_implementor::fill_column_constraints_with_inheritance ( const std::shared_ptr<const omm::Classifier>& p_csf, std::vector<std::shared_ptr<omm::Column_constraint>>& p_columns, omm::Column_constraint::Type p_type, bool p_with_root )  const {

    bool is_root = true;


    for ( auto [first, second] :  p_csf->generalizations() ) {
        auto cls =  second->general();
        //the super class has some field constraints
        fill_column_constraints_with_inheritance( cls, p_columns, p_type, p_with_root );
        is_root = false;
    }

    if ( ! is_root || p_with_root ) {
        // keep columns in the table of the classifier
        auto && csf_table = p_csf->table();
        for ( auto && prop : p_csf->own_properties() ) {
            auto && column = prop.second->column();
            if ( column ) {
                if ( column->table() == csf_table ) {
                    for ( auto & ctt : column->constraints() ) {
                        if ( ctt->type() == p_type ) {
                            p_columns.push_back ( ctt );
                        }
                    }
                }
            }
        }

     

    }

}

void Domain_post_implementor::fill_column_constraints_for_not_many_many_association ( const std::shared_ptr<const omm::Classifier>& p_csf, std::vector<std::shared_ptr<omm::Column_constraint>>& p_columns)  const {

    const auto & assoc = std::dynamic_pointer_cast<const omm::Association>(p_csf) ;
    if (assoc) {


        auto && assoc_table = assoc->managing_table();
        for ( auto && prop : assoc->own_properties() ) {
            auto && column = prop.second->column();
            if ( column ) {
                if ( column->table() == assoc_table ) {
                    for ( auto & ctt : column->constraints() ) {
                        if ( ctt->type() == omm::Column_constraint::Type::not_null  ) {
                            p_columns.push_back ( ctt );
                        }
                    }
                }
            }
        }

     

    }

}
void Domain_post_implementor::fill_column_constraints_for_many_to_many_association(  const std::shared_ptr<const omm::Classifier>& p_csf, std::vector<std::shared_ptr<omm::Column_constraint>>& p_columns) const {
   
    const auto & assoc = std::dynamic_pointer_cast<const omm::Association>(p_csf) ;
    if (assoc) {
        
        auto members = assoc->member_ends();
        if (members->size() == 2 && (*members)[0]->has_many_targets() && (*members)[1]->has_many_targets() ) {

            for ( std::size_t k=0; k<members->size(); ++k ) {
                if  ((*members)[k]->is_end_property() ) {

                    auto && column = ( *members ) [k]->column();
                    for ( auto & ctt : column->constraints() ) {
                        if ( ctt->type() == omm::Column_constraint::Type::unique ) {
                            p_columns.push_back ( ctt );
                        }
                    }
                }
            }
        }
    }
}


void Domain_post_implementor::prepare_declaration_for_fts_column ( const std::shared_ptr<const omm::Logical_field>& p_field, const std::string& p_code, std::string& p_fts_fragments_str, const std::string& p_fragment_sep,  const std::string& p_path ) const {
    
    auto  mdl_info = common::Singleton<common::Object_model_info>::instance().object();
    bool multilingual_composite = false;
    auto prop = p_field->property();
    if (prop) {
        if (prop->type()->package() ==  mdl_info->standard_domain()) { 
            multilingual_composite = true;
        }
        
        if ( prop->has_composite_type() ) {
            std::string col_path = "(";
            if (p_path.empty()) {
                 col_path += p_field->name() + ")" ;
            } else {
                col_path +=  p_path + "." + p_field->name() + ")";
            }
            const auto lt = std::dynamic_pointer_cast<omm::Logical_type>( prop->type()->logical_struct());
            if (!lt) {
                    throw exception::Not_found ("logical type not found");
            }
            if (multilingual_composite) {
                const auto nested_prop =  prop->find_nested_property(p_code);
                if (nested_prop) {
                  const auto nested_field =
                      nested_prop->logical_field()->name();

                  const auto nested = lt->find_attribute(nested_field);
                  if (nested) {
                    prepare_declaration_for_fts_column(
                        nested, p_code, p_fts_fragments_str, p_fragment_sep,
                        col_path);
                  } else {
                    throw exception::Not_found("language field not found");
                  }
                } else {
                  throw exception::Not_found("language field not found");
                }
            } else {
                for ( const auto [name, attr ]: lt->attributes() ) {
                    prepare_declaration_for_fts_column ( attr, p_code, p_fts_fragments_str, p_fragment_sep, col_path );
                }
            }

        } else if ( prop->has_primitive_type ()){
        
                
                auto && fragment_str = Util::make_fts_fragment_from_field ( p_field, p_path );
                if (!fragment_str.empty()) {
                    p_fts_fragments_str += fragment_str + p_fragment_sep ;
                }
        }
    }
}
