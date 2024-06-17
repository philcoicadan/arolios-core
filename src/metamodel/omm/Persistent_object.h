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
    
class Persistent_object {
public:
    /**
     * Constructor
     */
    Persistent_object();

    /**
     * Destructor
     */
    virtual ~Persistent_object();
    
    virtual void id (const std::string& p_id) ;
	virtual const std::string& id () const ;
	virtual void id_scope(const std::string& p_scope ) ;
	virtual const std::string& id_scope () const ;
	
	virtual void persistent_attributes (const std::string& p_id, const std::string& p_scope)  ;


private:

	std::string id_;
	std::string scope_;

};
}
}

