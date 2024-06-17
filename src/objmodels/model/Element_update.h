/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>

namespace arolios {




namespace model {

    
class Element_update {
public:
	Element_update();
	virtual ~Element_update();

    const std::string& new_name() const;
    


protected:
        
    std::string new_name_;

private:


} ;
}
}

