/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>

namespace arolios {


namespace omm {
    
class Column;

class Column_constraint {
public:
    enum Type { not_null, unique };
    /**
     * Constructor
     */
    Column_constraint(const std::shared_ptr<Column>& column, Type p_type);

    /**
     * Destructor
     */
    virtual ~Column_constraint();
    

    virtual const std::shared_ptr<Column>& column() const;
    virtual Type type()const ;
    
    
private:
    std::shared_ptr<Column> column_;
    Type type_;
};

}

}

