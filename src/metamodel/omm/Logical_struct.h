/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Logical_schema.h"
#include "Logical_object.h"

namespace arolios {


namespace omm {
    class Logical_field;
    class Classifier;
class Logical_struct : public virtual Namespace, public virtual Logical_object {
public:
    /**
     * Constructor
     */
    Logical_struct();

    /**
     * Destructor
     */
    virtual ~Logical_struct();
    
    


    virtual std::shared_ptr<Namespace> own_namespace() const  override;

    virtual void schema (const std::shared_ptr<Logical_schema>& p_schema) ; 

    virtual const std::shared_ptr<Logical_schema>& schema () const;

    virtual std::vector<std::shared_ptr<Classifier>> classifiers () const = 0;
    virtual std::vector<std::shared_ptr<Logical_field>> fields () const = 0;
    virtual bool has_elements (bool p_active_only = true) const override;
    
    virtual std::string full_name() const ;
    
    
    virtual bool is_table() = 0;
    virtual bool is_logical_type() = 0;



    virtual void own_namespace ( const std::shared_ptr<Namespace>& p_ns) override;   

private: 
    std::shared_ptr<omm::Logical_schema> schema_;


};

}
}

