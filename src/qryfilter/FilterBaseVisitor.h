
// Generated from Filter.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "FilterVisitor.h"


namespace arolios::qryfilter {

/**
 * This class provides an empty implementation of FilterVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  FilterBaseVisitor : public FilterVisitor {
public:

  virtual std::any visitFilter(FilterParser::FilterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndcondition(FilterParser::AndconditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition(FilterParser::ConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimplecondition(FilterParser::SimpleconditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCompare(FilterParser::CompareContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty(FilterParser::PropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTerm(FilterParser::TermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBindParameter(FilterParser::BindParameterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue(FilterParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumeric(FilterParser::NumericContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBoolean(FilterParser::BooleanContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace arolios::qryfilter
