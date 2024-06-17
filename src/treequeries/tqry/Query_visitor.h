/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once

namespace arolios {

namespace tqry {
    class Association;
    class Class;
    class Member_end;
    class Property;
    class Tree_query;
class Query_visitor {
public:
    /**
     * Constructor
     */
    Query_visitor();

    /**
     * Destructor
     */
    virtual ~Query_visitor();
    
    virtual void visit (Association& p_elem) = 0;
    virtual void visit (Class& p_elem) = 0;
    virtual void visit (Member_end& p_elem) = 0;
    virtual void visit (Property& p_elem) = 0;

};
}
}

