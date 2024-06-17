/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Logical_struct.h"
#include <unordered_set>

namespace arolios {


namespace omm {
    class Enumeration;

class Logical_enum : public virtual Logical_struct{
public:
    /**
     * Constructor
     */
    Logical_enum();

    /**
     * Destructor
     */
    virtual ~Logical_enum();
    
    
    virtual std::unordered_set<std::string>& values () ;

    void accept (Visitor& p_visitor)  const override ;

    virtual void enumeration (const std::shared_ptr<Enumeration>& p_enum) ; 

    virtual const std::shared_ptr<Enumeration>& enumeration () const;

    virtual std::vector<std::shared_ptr<Classifier>> classifiers () const  override;
    virtual std::vector<std::shared_ptr<Logical_field>> fields () const override;
    virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override;

    virtual std::string type_name() const override;
    
    virtual bool is_table() override { return false;}
    virtual bool is_logical_type() override { return false;}



private: 
    std::unordered_set<std::string> values_ ;
    std::shared_ptr<omm::Enumeration> enum_;

};

}
}

