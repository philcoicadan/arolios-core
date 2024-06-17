
// Generated from Filter.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "FilterListener.h"


namespace arolios::qryfilter {

/**
 * This class provides an empty implementation of FilterListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  FilterBaseListener : public FilterListener {
public:

  virtual void enterFilter(FilterParser::FilterContext * /*ctx*/) override { }
  virtual void exitFilter(FilterParser::FilterContext * /*ctx*/) override { }

  virtual void enterAndcondition(FilterParser::AndconditionContext * /*ctx*/) override { }
  virtual void exitAndcondition(FilterParser::AndconditionContext * /*ctx*/) override { }

  virtual void enterCondition(FilterParser::ConditionContext * /*ctx*/) override { }
  virtual void exitCondition(FilterParser::ConditionContext * /*ctx*/) override { }

  virtual void enterSimplecondition(FilterParser::SimpleconditionContext * /*ctx*/) override { }
  virtual void exitSimplecondition(FilterParser::SimpleconditionContext * /*ctx*/) override { }

  virtual void enterCompare(FilterParser::CompareContext * /*ctx*/) override { }
  virtual void exitCompare(FilterParser::CompareContext * /*ctx*/) override { }

  virtual void enterProperty(FilterParser::PropertyContext * /*ctx*/) override { }
  virtual void exitProperty(FilterParser::PropertyContext * /*ctx*/) override { }

  virtual void enterTerm(FilterParser::TermContext * /*ctx*/) override { }
  virtual void exitTerm(FilterParser::TermContext * /*ctx*/) override { }

  virtual void enterBindParameter(FilterParser::BindParameterContext * /*ctx*/) override { }
  virtual void exitBindParameter(FilterParser::BindParameterContext * /*ctx*/) override { }

  virtual void enterValue(FilterParser::ValueContext * /*ctx*/) override { }
  virtual void exitValue(FilterParser::ValueContext * /*ctx*/) override { }

  virtual void enterNumeric(FilterParser::NumericContext * /*ctx*/) override { }
  virtual void exitNumeric(FilterParser::NumericContext * /*ctx*/) override { }

  virtual void enterBoolean(FilterParser::BooleanContext * /*ctx*/) override { }
  virtual void exitBoolean(FilterParser::BooleanContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace arolios::qryfilter
