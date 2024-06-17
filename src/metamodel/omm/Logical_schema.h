/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Namespace.h"
#include "Logical_object.h"
#include <unordered_map>

namespace arolios {


namespace omm {
    
class Logical_struct;
class Package;

class Logical_schema : virtual public Namespace, public virtual Logical_object {
public:
    /**
     * Constructor
     */
    Logical_schema();

    /**
     * Destructor
     */
    virtual ~Logical_schema();

    virtual std::shared_ptr<Namespace> own_namespace() const override ;
    
    virtual std::unordered_map<std::string,std::shared_ptr<Logical_struct>>& logical_structs ();
    
    virtual void accept (Visitor& p_visitor) const override;
    
    virtual std::string type_name () const override;
    
    virtual void package (const std::shared_ptr<Package>& p_pk); 
    virtual const std::shared_ptr<Package> & package () const ; 
    
    virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override;
    
    virtual bool has_elements (bool p_active_only = true) const override;


    virtual void own_namespace ( const std::shared_ptr<Namespace>& p_ns) override;
        
private:
    std::unordered_map<std::string,std::shared_ptr<Logical_struct>> structs_;
    std::shared_ptr<Package> pk_;
    
};

}
}

