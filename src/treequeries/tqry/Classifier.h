/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace arolios {

namespace omm {
    class Classifier;
}

namespace tqry {
    class Property;
    class Member_end;
    class Query_visitor;
    class Query_stmt;

class Classifier {
public:
    /**
     * Constructor
     */
    Classifier();

    /**
     * Destructor
     */
    virtual ~Classifier();
    
    virtual const std::shared_ptr<const omm::Classifier> model_classifier() const = 0 ;
    
    const std::vector<std::shared_ptr<Property>>& properties () const ;
    
    void insert_property (const std::shared_ptr<Property>& p_prop);
    
    const std::vector<std::shared_ptr<Member_end>>& member_ends () const ;
    
    void insert_member_end (const std::shared_ptr<Member_end>& p_me);
    
    const std::string& filter() const ;
    
    void filter (const std::string& p_cond) ;
    
    bool is_root () const ;
    
    void is_root (bool p_flag);
    
    const std::string& table_alias() const ;
    
    void table_alias (const std::string& p_alias) ;
    
    void query (const std::shared_ptr<Query_stmt>& p_qry_stmt) ;
    const std::shared_ptr<Query_stmt>& query () const ;
    
    virtual void accept (Query_visitor& visitor) = 0;
    
private:

    std::vector<std::shared_ptr<Property>> props_vec_;
    std::unordered_map<std::string,std::shared_ptr<Property>> props_map_;    std::vector<std::shared_ptr<Member_end>> member_ends_vec_;
    std::unordered_map<std::string,std::shared_ptr<Member_end>> member_ends_map_;
    std::string filter_;
    bool is_root_;

    std::string table_alias_;
    std::shared_ptr<Query_stmt> query_;
    
    

};
}
}

