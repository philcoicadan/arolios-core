/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Persistent_object.h"
#include "Named_element.h"
#include "Lc_status.h"

namespace arolios {

namespace omm {

class Language : public virtual Named_element, public virtual Persistent_object {
public:
    /**
     * Constructor
     */
    Language( const std::string& p_code);

    /**
     * Destructor
     */
    virtual ~Language();
    
    virtual const std::string& code() const;
    
    virtual const std::string& fts_config() const;
    virtual void fts_config ( const std::string& p_fts_config) ;
    
    virtual Lc_status lc_status() const ;

    virtual void lc_status (const Lc_status p_status);
    
    virtual void accept (Visitor& p_visitor) const override;
    
    virtual std::shared_ptr<Namespace> own_namespace() const override; 
    

    virtual void own_namespace ( const std::shared_ptr<Namespace>& p_ns) override;

private:
    
    std::string code_;
    Lc_status lc_status_;
    std::string fts_config_;


     

};
}
}

