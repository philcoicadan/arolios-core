/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>

namespace arolios {

namespace omm {
class Logical_object {
public:
    /**
     * @todo write docs
     */
    Logical_object ();

    /**
     * @todo write docs
     */
    virtual ~Logical_object ();
    
    virtual std::string type_name() const = 0;
    

};

}
}

