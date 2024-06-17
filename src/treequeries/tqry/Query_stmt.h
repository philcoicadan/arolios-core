/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <json/json.h>

namespace arolios {

namespace omm {
    class Classifier;
}

namespace tqry {
    class Classifier;
    class Property;
    class Association;
    class Class;
class Query_stmt  {
public:
    
    enum Join_type { inner, left } ;
    /**
     * Constructor
     */
    Query_stmt(int p_count=-1);

    /**
     * Destructor
     */
    virtual ~Query_stmt();
    
    void add_from (const Classifier& p_csf, bool add_id = false) ;
    void add_join (const std::string& p_stmt);
    void add_and_condition ( const std::string& p_stmt);
    void add_column (const Property& p_elem) ;
    void add_result_id (const unsigned long p_row_id, const unsigned int p_csf_id) ;
    void add_row_id_json_obj (const unsigned long p_row_id, Json::Value* p_jobj) ;
    void add_stmt_complement (const std::string& p_stmt);
       const std::vector<std::pair<unsigned long, unsigned int>>& result_ids () const ;
    std::stack<Join_type>& join_types() ;
    static const std::string& join_type_to_string (Join_type p_join_type) ;
    
    const std::string new_alias (bool add_id_column = true) ;
    int alias_count () const;

    
    void build_sql_statement ();
    
    const std::string& sql_statement() const ;

    
    const std::string& from_alias () const;
    
    const std::unordered_map<unsigned long,Json::Value*>& row_id_json_obj_map () const;
 
     friend std::ostream& operator<< (std::ostream& p_os, Query_stmt& p_qry_stmt) {
             p_qry_stmt.print (p_os);
            return p_os;
     }
     
private:
    
    std::string from_alias_;
    std::string from_str_;
    std::vector<std::string> columns_;
    std::vector<std::string> joins_;
    std::vector<std::string> and_conditions_;
    std::stack<Join_type> join_types_;
    int tb_alias_count_;

    void print (std::ostream& p_os) const ;
    
    std::string sql_stmt_;
    
    std::vector<std::pair<unsigned long, unsigned int>> result_ids_;
    std::unordered_map<unsigned long,Json::Value*> row_id_json_obj_map_;
    std::vector<std::string> complements_;

};

}
}

