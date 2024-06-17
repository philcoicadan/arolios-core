/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Logical_object.h"
#include "Element.h"
#include <memory>

namespace arolios {


namespace omm {
    class Property;
    class Column;
    class Visitor;
class Property_by_sub_field : public virtual Element , public virtual Logical_object{
public:
    /**
     * Constructor
     */
    Property_by_sub_field();

    /**
     * Destructor
     */
    virtual ~Property_by_sub_field ( );
    
       
    virtual void accept (Visitor& p_visitor) const override;
    
   virtual std::string type_name() const override;
    
    virtual const std::shared_ptr<Column>& filtering_column() const;
    
    virtual void filtering_column ( const std::shared_ptr<Column>& p_column) ;
    
    virtual const std::shared_ptr<Property>& property () const;
    
    virtual void property ( const std::shared_ptr<Property>& p_prop ) ;
    
    
private:
    

    std::shared_ptr<omm::Column> fk_column_;
    std::shared_ptr<Property> property_;
};

}
}

