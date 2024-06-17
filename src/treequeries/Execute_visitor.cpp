/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Execute_visitor.h>
#include <tqry/Class.h>
#include <tqry/Association.h>
#include <tqry/Query_stmt.h>
#include <tqry/Member_end.h>
#include <json/value.h>
#include <exception/Exceptions.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Property.h>
#include <util/String.h>
#include <qry/Util.h>
#include <omm/Column.h>





using namespace arolios;
using namespace arolios::tqry;

Execute_visitor::Execute_visitor(Tree_query& p_query, Json::Value& p_result): query_(p_query), jresult_(p_result),  current_jobj_ (nullptr), current_row_ (nullptr),
om_ptr_ (common::Singleton<common::Object_model_info>::instance().object ()),
db_ (common::Singleton<common::Db_client>::instance().object())
{

}

Execute_visitor::~Execute_visitor() {

}

void Execute_visitor::visit (Association& p_elem) {
    
    
    if (p_elem.is_root()) {

       std::string error_msg = "Tree query with an association as root is not yet implemented" ;
        
       throw exception::Not_yet_implemented(error_msg);
       
    } else {

        // loop on the rows in progress, process one row
        const std::string prefix = p_elem.table_alias() + "_";

        auto row_id = ( *current_row_ ) [prefix + id_pair().second];
        if ( ! row_id.isNull() ) {
           
            // }

            auto csf_id = ( *current_row_ ) [prefix + csf_id_pair().second];

            
            const std::string id_str = qry::Util::build_instance_id( csf_id.as<unsigned int>() , row_id.as<unsigned long>() );
            
             ( *current_jobj_ ) ["instance_url"] = Json::Value ( qry::Util::url_prefix("instance") + id_str);


            // search the actual
            auto assoc_ptr = om_ptr_->find_domain_element_by_id<omm::Association> ( std::to_string ( csf_id.as<unsigned int>() ) );
            if ( assoc_ptr ) {
                ( *current_jobj_ ) ["url"]= qry::Util::url_prefix("association") + assoc_ptr->package()->name() + "." + assoc_ptr->name();
            }

            if (!p_elem.properties().empty()) {
                ( *current_jobj_ ) ["properties"] = Json::Value (Json::objectValue);
                Json::Value* jelem = current_jobj_ ;
                current_jobj_ = & (( *current_jobj_ ) ["properties"]);
                // process the properties
                for ( auto prop : p_elem.properties() ) {

                    prop->accept ( *this ) ;
                }
                current_jobj_ = jelem;
            }
                
                
        }

    }
    
}

void Execute_visitor::visit ( Class& p_elem ) {



    if ( p_elem.is_root() ) {

        // qry_stmt
        auto qry_stmt = p_elem.query();

        // limit of results = 1000
        qry_stmt->add_stmt_complement(" LIMIT 1000 ");

        qry_stmt->build_sql_statement ();
        qry_stmts_.push ( qry_stmt );

        auto f = db_->execSqlAsyncFuture ( qry_stmt->sql_statement() );

        auto result = f.get();

        int total = 0;
        if ( result.size() > 0 ) {
            auto first = result[0] ;
            total = first["_total"].as<int>();
        }
        jresult_ ["total"] = total;

        jresult_["elements"] = Json::Value ( Json::arrayValue );

        Json::Value& jobjs = jresult_["elements"];
        

        jobjs.resize ( result.size() );

        std::string prefix ="t0_";
        int i= 0;

        // loop on the rows
        for ( int i =0; i<result.size(); ++i) {
            const auto & row = result[i];
            jobjs[i] =  Json::Value ( Json::objectValue ) ;
            Json::Value& jobj = jobjs[i];

            auto csf_id = row[prefix + csf_id_pair().second];
            const std::string csf_id_str = std::to_string ( csf_id.as<unsigned int>());
            
            auto row_id = row[prefix + id_pair().second];
            const std::string  id_str = qry::Util::build_instance_id (csf_id.as<unsigned int>(),row_id.as<unsigned long>() );
            jobj["url"] = Json::Value ( qry::Util::url_prefix("instance") + id_str);

            
            qry_stmts_.top()->add_result_id (row_id.as<unsigned long>(), csf_id.as<unsigned int>()) ;

            auto class_ptr = om_ptr_->find_domain_element_by_id<omm::Class> ( std::to_string ( csf_id.as<unsigned int>() ) );
            if ( class_ptr ) {

                jobj ["class"] = Json::Value(Json::objectValue);
                jobj ["class"]["url"]= qry::Util::url_prefix("class") + class_ptr->package()->name() + "." + class_ptr->name();
            }
            // set the current pointers to the result row and json object
            current_jobj_ = &jobj;
            current_row_ = &row ;
            
            

             if (!p_elem.properties().empty() || !p_elem.member_ends().empty()) {
                ( *current_jobj_ ) ["properties"] = Json::Value (Json::objectValue);
                Json::Value* jelem = current_jobj_ ;
                current_jobj_ = & (( *current_jobj_ ) ["properties"]);
                
                // keep the pointer to the current JSON object with the row_id
                qry_stmts_.top()->add_row_id_json_obj (row_id.as<unsigned long>(),current_jobj_ ) ;
                
                for ( auto prop : p_elem.properties() ) {

                    prop->accept ( *this ) ;
                }

                for ( auto end : p_elem.member_ends() ) {
                    end->accept ( *this ) ;
                }
                current_jobj_ = jelem;
             }

        }
        
        // reset current pointers

        current_jobj_ = nullptr;
        current_row_ = nullptr;
        
        if ( result.size() > 0 ) { // continue with subqueries  only if there is a result
            
            // process member_ends with sub_query
            for ( auto end : p_elem.member_ends() ) {
                end->accept ( *this ) ;
                current_jobj_ = nullptr;
                current_row_ = nullptr;
            }
        }

    } else {
        // loop on the rows in progress, process one row
        const std::string prefix = p_elem.table_alias() + "_";

        const auto row_id = ( *current_row_ ) [prefix + id_pair().second];
        if ( ! row_id.isNull() ) {
            
            // }

            const auto csf_id = ( *current_row_ ) [prefix + csf_id_pair().second];

            
            const std::string id_str = qry::Util::build_instance_id (csf_id.as<unsigned int>(), row_id.as<unsigned long>());
            
            ( *current_jobj_ ) ["url"] = Json::Value ( qry::Util::url_prefix("instance") + id_str);

            // search the actual class
            auto class_ptr = om_ptr_->find_domain_element_by_id<omm::Class> ( std::to_string ( csf_id.as<unsigned int>() ) );
            if ( class_ptr ) {
                ( *current_jobj_ ) ["class"] = Json::Value(Json::objectValue);
                ( *current_jobj_ ) ["class"]["url"]= qry::Util::url_prefix("class") + class_ptr->package()->name() + "." + class_ptr->name();

                
            }


            if ( !p_elem.properties().empty() || !p_elem.member_ends().empty() ) {
                ( *current_jobj_ ) ["properties"] = Json::Value ( Json::objectValue );
                Json::Value* jelem = current_jobj_ ;
                current_jobj_ = & ( ( *current_jobj_ ) ["properties"] );

                for ( auto prop : p_elem.properties() ) {

                    prop->accept ( *this ) ;
                }

                for ( auto end : p_elem.member_ends() ) {
                    end->accept ( *this ) ;
                }
                current_jobj_ = jelem;
            }
        }
    }

}

            
void Execute_visitor::visit ( Member_end& p_elem ) {

    auto qry_stmt = p_elem.query();
    if (qry_stmt ) {
        if ( ! current_row_ ) {
            // process sub query
            std::string from_alias_id = qry_stmt->from_alias() + ".id";
            std::string from_alias_id_alias = qry_stmt->from_alias() + "_id";
            std::string condition = from_alias_id + " IN ( ";

            const auto & result_ids = qry_stmts_.top()->result_ids();
            // result_ids must not be empty 
            if (result_ids.size() == 0 ) {
                std::string error_msg = "Exporter: result of query is empty" ;
                 
                throw exception::Failure (error_msg);
            }
            for ( int i=0; i < result_ids.size(); ++i) {
                const auto id_pair = result_ids.at(i);
                condition += std::to_string(id_pair.first) + ", ";
            }
            
            condition.replace (condition.length()-2, 2, ") ");
            
            qry_stmt->add_and_condition (condition);
            
            

            qry_stmt->add_stmt_complement ( " ORDER BY " + from_alias_id_alias + " ASC ") ;
            // limit of results = 1000
            qry_stmt->add_stmt_complement(" LIMIT 1000 ");
            qry_stmt->build_sql_statement ();

            const std::string& query = qry_stmt->sql_statement();
            
            auto f = db_->execSqlAsyncFuture ( query);

            auto result = f.get();


            // keep ids map  and array index before stacking new query
            const auto & row_id_json_obj_map = qry_stmts_.top()->row_id_json_obj_map() ; 
            
            const std::string& key = p_elem.id_name ();
                  
           
            qry_stmts_.push ( qry_stmt );
            unsigned long previous_row_id = -1;
            Json::Value* parent_jobj_ptr = nullptr;
            int count = 0;
             for (const auto&  row : result) {
                 auto row_id =  row [from_alias_id_alias].as<unsigned long>();
                 if (row_id != previous_row_id) {
                     count = 0;
                     if ( auto search = row_id_json_obj_map.find ( row_id ) ; search != row_id_json_obj_map.end() ) {
                        parent_jobj_ptr = search->second;
                    }
                    if ( parent_jobj_ptr == nullptr ) {
                        std::string error_msg = "Exporter: index in the parent query not found" ;
                         
                        throw exception::Failure ( error_msg );
                    }
                 } else {
                     ++count;
                 }
                 previous_row_id = row_id;
                 
                Json::Value& jobj = (*parent_jobj_ptr)[key];
                jobj.resize ( count+1 );  
                jobj[count] = Json::Value ( Json::objectValue );
                current_jobj_ = & jobj[count];
                current_row_ = &row;
                
                if ( p_elem.optional_association() ) {
                    // process the association which the member end belongs to
                    // row kept, jobj as new object
                    ( *current_jobj_ ) ["association"] = Json::Value ( Json::objectValue );
                    // save json obj of the member_end
                    Json::Value* member_end_jobj = current_jobj_;
                    current_jobj_ = & ( ( *current_jobj_ ) ["association"] );

                    p_elem.association()->accept ( *this );

                    // recover json obj of the member end
                    current_jobj_  = member_end_jobj;

                }

                // process the type of the member end
                p_elem.target()->accept ( *this );


            }
            
        } else {
            // member end is an arrayValue
            const std::string& key = p_elem.id_name ();
            (*current_jobj_)[key] = Json::Value ( Json::arrayValue );
            
        }
    } else {
        if ( current_row_ ) {
            // the member end is in a join, row  kept , jobj as new object
            const std::string& key = p_elem.id_name ();
            ( *current_jobj_ ) [key] = Json::Value ( Json::objectValue );
            Json::Value * csf_jobj = current_jobj_;
            current_jobj_ = & ( ( *current_jobj_ ) [key] );


            if ( p_elem.optional_association() ) {
                // process the association which the member end belongs to
                // row kept, jobj as new object
                ( *current_jobj_ ) ["association"] = Json::Value ( Json::objectValue );
                // save json obj of the member_end
                Json::Value* member_end_jobj = current_jobj_;
                current_jobj_ = & ( ( *current_jobj_ ) ["association"] );

                p_elem.association()->accept ( *this );

                // recover json obj of the member end
                current_jobj_  = member_end_jobj;

            }

            // process the type of the member end
            p_elem.target()->accept ( *this );
            current_jobj_ = csf_jobj;
        } else {
            // nothing to do
        }
    }
    
}


void Execute_visitor::visit ( Property& p_elem ) {


    std::string alias = p_elem.classifier()->table_alias();
    auto terminal_prop = p_elem.model_property();
    auto key = p_elem.id_name ();
    auto column =terminal_prop->column() ;
    if ( column ) {
        alias += column->name();
        const auto nested = p_elem.nested_property();
        if ( nested ) {
            const auto nested_column = nested->model_property()->logical_field();
            alias += nested_column->name();
            terminal_prop = nested->model_property();
            
        }


        const auto field = (*current_row_)[ alias];
        if ( ! field.isNull() ) {
            qry::Native_type native_type;
            Json::Value* prop_jobj = current_jobj_;
            if (nested) {
                if (! current_jobj_->isMember(key)) {
                    (*current_jobj_)[key] = Json::Value ( Json::objectValue );
                }
                prop_jobj = & (*current_jobj_)[key] ;
                key = nested->id_name();
            }
                
            if ( terminal_prop->has_referencing_type() ) {
                // to be declared by association ends
                std::string error_msg = "Property "+  terminal_prop->name() + " is not valid in this context" ;
                 
                throw exception::Failure ( error_msg ) ;

            } else {
                if ( terminal_prop->has_enum_type() ) {
                    // enum value is like a char string
                    native_type = qry::Native_type::_character_varying;
                } else {
                    // prop type is a primitive
                    native_type = static_cast<qry::Native_type> ( terminal_prop->logical_field()->native_type() );
                }

                (*prop_jobj)[key] =  qry::Util::native_to_json ( native_type, field ) ;
            }
        }
    }
}


const std::pair<std::string,std::string>& Execute_visitor::id_pair () const {
    static std::pair<std::string,std::string> pair =  {"id","id"};
    return pair;
}

const std::pair<std::string,std::string>& Execute_visitor::csf_id_pair () const {
    static std::pair<std::string,std::string> pair = {"csf_id", "csf_id"};
     return pair;
}

