/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Logical_field.h"
#include <vector>
#include "Column_constraint.h"

namespace arolios {



namespace omm {


    class Table;
  /**
 * @todo write docs
 */
class Column :  public virtual Logical_field {
public:
    /**
     * Constructor
     */
    Column();

    /**
     * Destructor
     */
    virtual ~Column();
    
   
    virtual const std::vector<std::shared_ptr<Column_constraint>>& constraints () const ;
    
    virtual void insert_constraint (const std::shared_ptr<Column_constraint>& p_ctt) ;
    virtual void erase_constraint ( const Column_constraint::Type ctt_type) ;
  
   
    virtual void accept (Visitor& p_visitor) const override;
    
    virtual std::string type_name() const  override;
    
   
    virtual const std::shared_ptr<Table> table() const;
    

    
    
private :
    
    std::vector<std::shared_ptr<Column_constraint>> constraints_;
 
    
};

}
}

