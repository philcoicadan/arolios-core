
// Generated from Filter.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"
#include "FilterParser.h"


namespace arolios::qryfilter {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by FilterParser.
 */
class  FilterVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by FilterParser.
   */
    virtual std::any visitFilter(FilterParser::FilterContext *context) = 0;

    virtual std::any visitAndcondition(FilterParser::AndconditionContext *context) = 0;

    virtual std::any visitCondition(FilterParser::ConditionContext *context) = 0;

    virtual std::any visitSimplecondition(FilterParser::SimpleconditionContext *context) = 0;

    virtual std::any visitCompare(FilterParser::CompareContext *context) = 0;

    virtual std::any visitProperty(FilterParser::PropertyContext *context) = 0;

    virtual std::any visitTerm(FilterParser::TermContext *context) = 0;

    virtual std::any visitBindParameter(FilterParser::BindParameterContext *context) = 0;

    virtual std::any visitValue(FilterParser::ValueContext *context) = 0;

    virtual std::any visitNumeric(FilterParser::NumericContext *context) = 0;

    virtual std::any visitBoolean(FilterParser::BooleanContext *context) = 0;


};

}  // namespace arolios::qryfilter
