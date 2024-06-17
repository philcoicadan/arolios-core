/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Query_stmt.h>
#include <tqry/Classifier.h>
#include <tqry/Class.h>
#include <tqry/Association.h>
#include <tqry/Property.h>
#include <omm/Classifier.h>
#include <omm/Association.h>
#include <omm/Class.h>
#include <omm/Property.h>
#include <omm/Table.h>
#include <omm/Column.h>
#include <exception/Exceptions.h>
#include <common/Db_client.h>



using namespace arolios;
using namespace arolios::tqry;

Query_stmt::Query_stmt(int p_count)  : tb_alias_count_(p_count) {
    join_types_.push(Join_type::inner);
}

Query_stmt::~Query_stmt() {

}

void Query_stmt::add_from (const Classifier& p_csf, bool p_add_id) {


    from_alias_ = p_csf.table_alias();
    from_str_ = p_csf.model_classifier()->managing_table()->full_name() + " AS " + from_alias_;
    if (p_add_id) {
        columns_.push_back ( from_alias_ + ".id AS " + from_alias_ + "_id" );
        columns_.push_back ( from_alias_ + ".csf_id AS " + from_alias_ + "_csf_id" );
    }
    
    
}

const std::string& Query_stmt::from_alias () const {
    return from_alias_;
}



void Query_stmt::add_column ( const Property& p_elem ) {

    std::string alias = p_elem.classifier()->table_alias();
    if ( alias.empty() ) {
        std::string error_msg = "Table alias for classifier "  + p_elem.classifier()->model_classifier()->name() + " not found" ;
         
        throw exception::Not_found ( error_msg );
    }

    std::string col_name = alias + ".";

    const auto column = p_elem.model_property()->column();
    if ( column ) {
        col_name += column->name();
        alias += column->name();
        const auto nested = p_elem.nested_property();
        if ( nested ) {
            const auto nested_column = nested->model_property()->logical_field();
            col_name.insert ( 0,1, '(' );
            col_name += ")." + nested_column->name();
            alias += nested_column->name();
        }
        columns_.push_back ( col_name + " AS " + alias ) ;

    } else {
        std::cout << "WARNING: Property " + p_elem.model_property()->name() + " is not mapped to a relational object, its value is unavailable" << std::endl;
    }


}
    
void Query_stmt::add_join (const std::string& p_stmt)  {
    
    joins_.push_back( p_stmt) ;


}

void Query_stmt::add_and_condition ( const std::string& p_stmt) {
    and_conditions_.push_back (p_stmt);
}

void Query_stmt::add_stmt_complement (const std::string& p_stmt) {
    complements_.push_back (p_stmt);
}

void Query_stmt::add_result_id (const unsigned long p_row_id, const unsigned int p_csf_id)  {
    result_ids_.push_back ({ p_row_id, p_csf_id}) ;
}

const std::vector<std::pair<unsigned long, unsigned int>>& Query_stmt::result_ids () const {
    return result_ids_;
}

const std::unordered_map<unsigned long, Json::Value*>& Query_stmt::row_id_json_obj_map () const {
    return row_id_json_obj_map_;
}

 void Query_stmt::add_row_id_json_obj ( const unsigned long p_row_id, Json::Value* p_jobj) {
         row_id_json_obj_map_.insert ( { p_row_id, p_jobj} );

 }


const std::string Query_stmt::new_alias (bool add_id) {
    const auto alias = "t" + std::to_string (++tb_alias_count_);
    if (add_id) {
        columns_.push_back ( alias + ".id AS " + alias + "_id") ;
        columns_.push_back ( alias + ".csf_id AS " + alias + "_csf_id") ;
    }
    
    return alias; 
}

int Query_stmt::alias_count () const {
    return tb_alias_count_;
}

                
std::stack<Query_stmt::Join_type>& Query_stmt::join_types() {
    return join_types_;
}

const std::string& Query_stmt::join_type_to_string ( Query_stmt::Join_type p_join_type) {
    static std::unordered_map<Query_stmt::Join_type, std::string>  map = { 
    { Query_stmt::Join_type::inner, "INNER JOIN"},
    { Query_stmt::Join_type::left, "LEFT JOIN" }} ;
    
    return map.at(p_join_type);
}

void Query_stmt::build_sql_statement ()  {
    
    
    sql_stmt_ = "SELECT COUNT(*) OVER() AS _total, " ;

    for (auto column : columns_) {
        sql_stmt_ += column + "," ;
    }
    
    if (!columns_.empty())  {
        sql_stmt_.erase(sql_stmt_.length()-1,1);
    }
    
    sql_stmt_ += " FROM " + from_str_ ;
    
    for (auto join : joins_) {
        sql_stmt_ += " " + join ;
    }
    
    if (! and_conditions_.empty()) {
        sql_stmt_ += " WHERE ";
        for (auto cond : and_conditions_) {
            sql_stmt_ += cond + " AND "  ;
        }
        sql_stmt_.erase(sql_stmt_.length()-4,4);
        
    }
    
    for (auto cpmt : complements_) {
        sql_stmt_ += " " + cpmt ;
    }
    
    
    
}

const std::string& Query_stmt::sql_statement () const {
    return sql_stmt_;
}




void Query_stmt::print (std::ostream& p_os) const {
    
    p_os << "SELECT " ;

    for (auto column : columns_) {
        p_os << column << " " ;
    }
    
    p_os << " FROM " << from_str_ << std::endl;
    
    for (auto join : joins_) {
        p_os << join << std::endl;
    }
    
    p_os <<"WHERE ";
    for (auto cond : and_conditions_) {
        p_os << cond << " ";
    }
    
    for (auto cpmt : complements_) {
        p_os << cpmt << std::endl;
    }
    

    p_os << std::endl;
}


