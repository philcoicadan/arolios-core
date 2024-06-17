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
    class Column;
    class Class;
    class Association;
class Table : public virtual Logical_struct{
public:
    /**
     * Constructor
     */
    Table();

    /**
     * Destructor
     */
    virtual ~Table();
    
    
    virtual std::unordered_map<std::string,std::shared_ptr<Column>>& columns () ;

    virtual std::vector<std::shared_ptr<Classifier>> classifiers () const  override;
    virtual std::vector<std::shared_ptr<Logical_field>> fields () const override;
    
    virtual std::vector<std::shared_ptr<Class>>& classes () ;

    virtual const std::shared_ptr<Association>& association () const ;
    virtual void association (const std::shared_ptr<Association>& p_assoc) ;

    virtual const std::shared_ptr<Classifier>& root_classifier () const ;
    virtual void root_classifier (const std::shared_ptr<Classifier>& p_cls) ;
    

    virtual void accept (Visitor& p_visitor)  const override ;
    
    virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) override;
    virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override;

    virtual std::string type_name () const override;
    
    virtual bool is_table() override { return true;}
    virtual bool is_logical_type() override { return false;}



private: 
    std::unordered_map<std::string,std::shared_ptr<Column>> columns_ ;
    std::vector<std::shared_ptr<Class>> classes_;
    std::shared_ptr<Association> assoc_;
    std::shared_ptr<Classifier> root_cls_;
    

};

}
}

