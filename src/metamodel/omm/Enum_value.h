/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <memory>
#include "Packageable_element.h"

namespace arolios {

namespace omm {
    class Namespace;
    class Package;
    class Enumeration;

class Enum_value : public virtual Packageable_element {
public:
    /**
     * Constructor
     */
    Enum_value();

    /**
     * Destructor
     */
    virtual ~Enum_value();
    
    virtual std::shared_ptr<Namespace> own_namespace() const override;
        
    virtual std::shared_ptr<Package> package() const override ;

    virtual void accept (Visitor& p_visitor) const override;
    
    virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
    virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
    virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

    
    virtual const std::shared_ptr<Enumeration>& enumeration () const;
    virtual void enumeration (const std::shared_ptr<Enumeration>&  p_enum) ;

    virtual void own_namespace ( const std::shared_ptr<Namespace>& p_ns) override;

    virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

    
private:
    
    std::shared_ptr<Enumeration> enum_;

};
}
}

