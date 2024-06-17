/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Named_element.h"
#include "Logical_object.h"

namespace arolios {


namespace omm {
    class Property;
    class Attribute;
    class Column;
    class Logical_struct;
class Logical_field :  public virtual Named_element, public virtual Logical_object {
public:
    
    enum Type { column_t, attribute_t, sub_field_t};
    /**
     * Constructor
     */
    Logical_field();

    /**
     * Destructor
     */
    virtual ~Logical_field();
    
    virtual std::shared_ptr<Namespace> own_namespace() const override;
    
    virtual const std::shared_ptr<Logical_struct>& logical_struct () const;
    
    virtual void logical_struct ( const std::shared_ptr<Logical_struct>& p_struct ) ;
    
        
    virtual const std::shared_ptr<Property>& property () const;
    
    virtual void property ( const std::shared_ptr<Property>& p_prop ) ;
    
    virtual void native_type (const int p_type ) ;
    
    virtual const int native_type() const;
    

    
    virtual std::string full_name () const;


    virtual void own_namespace ( const std::shared_ptr<Namespace>& p_ns) override;
    
private :
    
    std::shared_ptr<Logical_struct> struct_;
    std::shared_ptr<Property> property_;
    int native_type_;
};

}
}

