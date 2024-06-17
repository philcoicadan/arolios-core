/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "FilterBaseVisitor.h"
#include <json/json.h>


namespace arolios {
namespace omm {
    class Classifier;
    class Logical_field;
}
namespace qryfilter {
class Qry_filter_visitor : public FilterBaseVisitor {
public:
    /**
     * Constructor
     */
    Qry_filter_visitor(const std::shared_ptr<const omm::Classifier>& p_csf, const std::unordered_map<std::string,Json::Value>& p_params);
    
    virtual ~Qry_filter_visitor ();
    
    virtual std::any visitFilter(FilterParser::FilterContext *ctx) override ;

  virtual std::any visitAndcondition(FilterParser::AndconditionContext *ctx) override ;

  virtual std::any visitCondition(FilterParser::ConditionContext *ctx) override ;

  virtual std::any visitSimplecondition(FilterParser::SimpleconditionContext *ctx) override ;

  virtual std::any visitCompare(FilterParser::CompareContext *ctx) override ;

  virtual std::any visitProperty(FilterParser::PropertyContext *ctx) override ;


  virtual std::any visitBindParameter(FilterParser::BindParameterContext *ctx) override ;

  virtual std::any visitValue(FilterParser::ValueContext *ctx) override ;


  
  virtual const std::string & filter_result () const; 

private: 
    void property_to_field (const std::vector<std::string>& p_prop_names_vec) ;
    
    const std::shared_ptr<const omm::Classifier> classifier_;
    const std::unordered_map<std::string,Json::Value>& param_values_;
    std::string filter_;
    std::shared_ptr<const omm::Logical_field> last_field_;
    

};

}
}