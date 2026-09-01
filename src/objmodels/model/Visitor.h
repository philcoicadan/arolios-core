/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Model_element_create.h>
#include <model/Model_element_delete.h>
#include <model/Model_element_update.h>
#include <model/xam.h>

namespace arolios {


namespace omm {
    class Enumeration;
    class Enum_value;
    class Data_type;
    class Composite_type;
    class Primitive_type;
    class Class;
    class Association;
    class Property;
    class Package;
    
}

namespace model {
    
    class Domain_create;
    class Domain_update;
    class Domain_delete;
    class Enumeration_create;
    class Enumeration_update;
    class Enumeration_delete;
    class Type_create;
    class Type_update;
    class Type_delete;
    class Type_store;
    class Class_store;
    class Association_store;
    class Translatable;
    class Root_loader;

class Visitor {
public:


    /**
     * Destructor
     */
    virtual ~Visitor() ;
    

    virtual void visit (const Model_element_create<Enumeration_t, omm::Enumeration >& p_obj)  = 0 ;
    virtual void visit (const Model_element_delete<Enumeration_t, omm::Enumeration >& p_obj)  = 0 ;
    virtual void visit (const Model_element_update<Enumeration_t, omm::Enumeration >& p_obj)  = 0 ;
    virtual void visit (const Model_element_create<Element_t, omm::Enum_value >& p_obj)  = 0 ;
    virtual void visit (const Model_element_delete<Element_t, omm::Enum_value >& p_obj)  = 0 ;
    virtual void visit (const Model_element_update<Element_t, omm::Enum_value >& p_obj)  = 0 ; 
    virtual void visit (const Model_element_create<Type_t, omm::Composite_type >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Type_t, omm::Composite_type >& p_obj)  = 0;
    virtual void visit (const Model_element_update<Type_t, omm::Composite_type >& p_obj)  = 0;
    virtual void visit (const Model_element_create<Primitive_t, omm::Primitive_type >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Primitive_t, omm::Primitive_type >& p_obj)  = 0;
    virtual void visit (const Model_element_update<Primitive_t, omm::Primitive_type >& p_obj)  = 0;
    virtual void visit (const Model_element_create<Class_t, omm::Class >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Class_t, omm::Class >& p_obj)  = 0;
    virtual void visit (const Model_element_update<Class_t, omm::Class >& p_obj)  = 0;
    virtual void visit (const Model_element_create<Association_t, omm::Association >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Association_t, omm::Association >& p_obj)  = 0;
    virtual void visit (const Model_element_update<Association_t, omm::Association >& p_obj)  = 0;
    virtual void visit (const Model_element_create<Type_property_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Type_property_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_update<Type_property_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_create<Class_property_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Class_property_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_update<Class_property_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_create<Member_end_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Member_end_t, omm::Property >& p_obj)  = 0;
    virtual void visit (const Model_element_update<Member_end_t, omm::Property >& p_obj)  = 0;    
    virtual void visit (const Model_element_create<Domain_t, omm::Package >& p_obj)  = 0;
    virtual void visit (const Model_element_delete<Domain_t, omm::Package >& p_obj)  = 0 ;
    virtual void visit (const Model_element_update<Domain_t, omm::Package >& p_obj)  = 0;
  
    virtual void visit (const Root_loader& p_obj) = 0 ;
protected:
    
    /**
     * Constructor
     */
    Visitor() ;
};

}
}

