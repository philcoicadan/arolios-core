/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <tqry/Query_visitor.h>
#include <qry/Base_query.h>
#include <string>
#include <memory>
#include <stack>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <qry/Db_types.h>

namespace arolios {



namespace omm {
    class Table;
}

namespace tqry {
    class Tree_query;
    class Query_stmt;
    class Classifier;
class Execute_visitor :  public Query_visitor {
public:
    /**
     * Constructor
     */
    Execute_visitor(Tree_query& p_query, Json::Value& p_result);

    /**
     * Destructor
     */
    virtual ~Execute_visitor();
    
    virtual void visit (Association& p_elem ) override;
    virtual void visit (Class& p_elem ) override;
    virtual void visit (Member_end& p_elem ) override;
    virtual void visit (Property& p_elem ) override;

private:
    
    Tree_query& query_;
    Json::Value & jresult_ ;
    
    const  std::shared_ptr<common::Object_model_info> om_ptr_;
    const std::shared_ptr<common::Db_client> db_;
    
    Json::Value* current_jobj_;
    const qry::Db_row* current_row_;
    
    std::stack<std::shared_ptr<Query_stmt>> qry_stmts_;
    
    const std::pair<std::string,std::string>& id_pair () const ;
    const std::pair<std::string,std::string>& csf_id_pair () const ;


    
};
}
}

