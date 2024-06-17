
// Generated from Filter.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "FilterParser.h"


namespace arolios::qryfilter {

/**
 * This interface defines an abstract listener for a parse tree produced by FilterParser.
 */
class  FilterListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterFilter(FilterParser::FilterContext *ctx) = 0;
  virtual void exitFilter(FilterParser::FilterContext *ctx) = 0;

  virtual void enterAndcondition(FilterParser::AndconditionContext *ctx) = 0;
  virtual void exitAndcondition(FilterParser::AndconditionContext *ctx) = 0;

  virtual void enterCondition(FilterParser::ConditionContext *ctx) = 0;
  virtual void exitCondition(FilterParser::ConditionContext *ctx) = 0;

  virtual void enterSimplecondition(FilterParser::SimpleconditionContext *ctx) = 0;
  virtual void exitSimplecondition(FilterParser::SimpleconditionContext *ctx) = 0;

  virtual void enterCompare(FilterParser::CompareContext *ctx) = 0;
  virtual void exitCompare(FilterParser::CompareContext *ctx) = 0;

  virtual void enterProperty(FilterParser::PropertyContext *ctx) = 0;
  virtual void exitProperty(FilterParser::PropertyContext *ctx) = 0;

  virtual void enterTerm(FilterParser::TermContext *ctx) = 0;
  virtual void exitTerm(FilterParser::TermContext *ctx) = 0;

  virtual void enterBindParameter(FilterParser::BindParameterContext *ctx) = 0;
  virtual void exitBindParameter(FilterParser::BindParameterContext *ctx) = 0;

  virtual void enterValue(FilterParser::ValueContext *ctx) = 0;
  virtual void exitValue(FilterParser::ValueContext *ctx) = 0;

  virtual void enterNumeric(FilterParser::NumericContext *ctx) = 0;
  virtual void exitNumeric(FilterParser::NumericContext *ctx) = 0;

  virtual void enterBoolean(FilterParser::BooleanContext *ctx) = 0;
  virtual void exitBoolean(FilterParser::BooleanContext *ctx) = 0;


};

}  // namespace arolios::qryfilter
