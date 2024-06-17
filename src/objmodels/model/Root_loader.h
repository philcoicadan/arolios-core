/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <model/Base_element.h>

namespace arolios {

namespace omm {
    class Package;
}

namespace model {
    
class Root_loader  : public Base_element {

public:
    Root_loader ( const std::shared_ptr<omm::Packageable_element>& p_elem) ;
    
    virtual ~Root_loader() ;
    
    virtual void execute () override ;
    virtual const std::shared_ptr<omm::Packageable_element> stored_element () const override ;
    virtual void accept(Visitor& p_visitor) override ;
    
    
    
protected:
    virtual void fill_children () override;
    virtual void internal_execute (const omm::Element_attributes& p_attrs) override ;
    
private:
    const std::shared_ptr<omm::Packageable_element> root_elem_;

};
}

}

