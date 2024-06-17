/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <omm/Namespace.h>
#include <unordered_set>
#include <json/json.h>

namespace arolios {

namespace omm {
    class Visitor;
}
namespace tqry {
    
    class Classifier;
    class Parameter;
    class Query_stmt;
class Tree_query : public omm::Named_element {
public:
    /**
     * Constructor
     */
    Tree_query();

    /**
     * Destructor
     */
    virtual ~Tree_query();
    
    void root (const std::shared_ptr<Classifier>& p_csf) ; 
    
    const std::shared_ptr<Classifier>& root () const ;
    
    virtual std::shared_ptr<omm::Namespace> own_namespace() const override;
    virtual  void own_namespace(const std::shared_ptr<omm::Namespace>& p_ns) override;
    
     
    virtual void add_parameter (const std::string& p_param) ;
    virtual bool has_parameters () const ;
    virtual const std::unordered_set<std::string>& parameters () const ; 
    
    virtual void accept (omm::Visitor& p_visitor) const override;
    
   
    
    void prepare () ;
    void execute(Json::Value& p_result) ;
    
private: 
    
    std::shared_ptr<Classifier> root_;
    std::unordered_set<std::string> params_;
    

};
}
}

