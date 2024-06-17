/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <tqry/Query_visitor.h>
#include <string>
#include <memory>
#include <stack>

namespace arolios {

namespace omm {
    class Table;
}

namespace tqry {
    class Tree_query;
    class Query_stmt;
    class Classifier;
class Prepare_visitor :  public Query_visitor {
public:
    /**
     * Constructor
     */
    Prepare_visitor(Tree_query& p_query);

    /**
     * Destructor
     */
    virtual ~Prepare_visitor();
    
    virtual void visit (Association& p_elem ) override;
    virtual void visit (Class& p_elem ) override;
    virtual void visit (Member_end& p_elem ) override;
    virtual void visit (Property& p_elem ) override;


    
private:
    
    void add_filter (const std::string& p_filter, const Classifier& p_elem) ;
    
    Tree_query& query_;
    
    std::stack<std::shared_ptr<Query_stmt>> qry_stmts_;

    
    
    
    
};
}
}

