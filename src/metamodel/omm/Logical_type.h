/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Logical_struct.h"

namespace arolios {


namespace omm {
    class Attribute;
    class Data_type;
    class Table;
class Logical_type : public virtual Logical_struct{
public:
    /**
     * Constructor
     */
    Logical_type();

    /**
     * Destructor
     */
    virtual ~Logical_type();
    
    
    virtual const std::unordered_map<std::string,std::shared_ptr<Attribute>>& attributes () const ;


    virtual void accept (Visitor& p_visitor)  const override ;

    virtual void data_type (const std::shared_ptr<Data_type>& p_type) ; 

    virtual const std::shared_ptr<Data_type>& data_type () const;

    virtual std::vector<std::shared_ptr<Classifier>> classifiers () const  override;
    virtual std::vector<std::shared_ptr<Logical_field>> fields () const override;

    virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override;

    virtual std::string type_name () const override;
    
    virtual bool is_table() override { return false;}
    virtual bool is_logical_type() override { return true;}
    
    virtual std::shared_ptr<Attribute> find_attribute (const std::string& p_name) const ;
    virtual void add_attribute ( const std::shared_ptr<Attribute>& p_attr) ;




private: 
    std::unordered_map<std::string,std::shared_ptr<Attribute>> attributes_ ;
    std::shared_ptr<Data_type> data_type_;

};

}
}

