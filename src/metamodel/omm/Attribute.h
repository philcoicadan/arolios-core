/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Logical_field.h"

namespace arolios {



namespace omm {
 
class Attribute :  public virtual Logical_field {
public:
    /**
     * Constructor
     */
    Attribute();

    /**
     * Destructor
     */
    virtual ~Attribute();
    
    
    
    virtual void accept (Visitor& p_visitor) const override;
    
    virtual std::string type_name() const override;
    
    
    
private :
    

};

}
}

