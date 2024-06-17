/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <tqry/Property.h>
#include <tqry/Association.h>

namespace arolios {


namespace tqry {
    class Class;
    class Query_stmt;
class Member_end :  public virtual Property {
public:
    /**
     * Constructor
     */
    Member_end();

    /**
     * Destructor
     */
    virtual ~Member_end();
    
    void target (const std::shared_ptr<Class>& p_class) ;
    const std::shared_ptr<Class>& target () const ;
    
    void query (const std::shared_ptr<Query_stmt>& p_qry_stmt) ;
    const std::shared_ptr<Query_stmt>& query () const ;
    
    void association (const std::shared_ptr<Association>& p_assoc) ;
    const std::shared_ptr<Association>& association () const ; 
    
    void optional_association (bool p_flag) ;
    bool optional_association() const ;
    
    virtual void accept (Query_visitor& visitor) override;
      
private:
    
    std::shared_ptr<Association> assoc_;
    std::shared_ptr<Class> target_;
    std::shared_ptr<Query_stmt> query_;
    bool opt_assoc_;

};
}
}

