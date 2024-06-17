/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <tqry/Classifier.h>

namespace arolios {


namespace omm {
    class Property;
}

namespace tqry {
    class Query_visitor;
class Property {
public:
    /**
     * Constructor
     */
    Property();

    /**
     * Destructor
     */
    ~Property();
    
    
    void model_property ( const std::shared_ptr<omm::Property>& p_mdl_prop) ;
    
    const std::shared_ptr<omm::Property>& model_property () const ;
    
    const std::shared_ptr<Classifier>& classifier () const ;
    
    void classifier ( const std::shared_ptr<Classifier>& p_csf) ;
    
    const std::shared_ptr<Property>& nested_property () const ;
    
    void nested_property ( const std::shared_ptr<Property>& p_nested) ;
    
    const std::string& id_name () const ;
    
    void id_name ( const std::string& p_name) ;
    
    virtual void accept (Query_visitor& visitor) ;
 
private:
    
    std::shared_ptr<Classifier> csf_;
    std::shared_ptr<omm::Property> mdl_prop_;
    std::shared_ptr<Property> nested_;
    std::string id_name_;

};

}
}

