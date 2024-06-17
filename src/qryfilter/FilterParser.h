
// Generated from Filter.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"


namespace arolios::qryfilter {


class  FilterParser : public antlr4::Parser {
public:
  enum {
    OrOper = 1, AndOper = 2, NotOper = 3, OpenPar = 4, ClosePar = 5, Comma = 6, 
    In = 7, Between = 8, Is = 9, Null = 10, NotEq = 11, LtOrEq = 12, GtOrEq = 13, 
    Minus = 14, Eq = 15, Lt = 16, Gt = 17, Dot = 18, Dollar = 19, BoolTrue = 20, 
    BoolFalse = 21, String = 22, Integer = 23, Decimal = 24, Identifier = 25, 
    WS = 26
  };

  enum {
    RuleFilter = 0, RuleAndcondition = 1, RuleCondition = 2, RuleSimplecondition = 3, 
    RuleCompare = 4, RuleProperty = 5, RuleTerm = 6, RuleBindParameter = 7, 
    RuleValue = 8, RuleNumeric = 9, RuleBoolean = 10
  };

  explicit FilterParser(antlr4::TokenStream *input);

  FilterParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~FilterParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class FilterContext;
  class AndconditionContext;
  class ConditionContext;
  class SimpleconditionContext;
  class CompareContext;
  class PropertyContext;
  class TermContext;
  class BindParameterContext;
  class ValueContext;
  class NumericContext;
  class BooleanContext; 

  class  FilterContext : public antlr4::ParserRuleContext {
  public:
    FilterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AndconditionContext *> andcondition();
    AndconditionContext* andcondition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> OrOper();
    antlr4::tree::TerminalNode* OrOper(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FilterContext* filter();

  class  AndconditionContext : public antlr4::ParserRuleContext {
  public:
    AndconditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ConditionContext *> condition();
    ConditionContext* condition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AndOper();
    antlr4::tree::TerminalNode* AndOper(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AndconditionContext* andcondition();

  class  ConditionContext : public antlr4::ParserRuleContext {
  public:
    ConditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyContext *property();
    SimpleconditionContext *simplecondition();
    antlr4::tree::TerminalNode *NotOper();
    FilterContext *filter();
    antlr4::tree::TerminalNode *OpenPar();
    antlr4::tree::TerminalNode *ClosePar();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionContext* condition();

  class  SimpleconditionContext : public antlr4::ParserRuleContext {
  public:
    SimpleconditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CompareContext *compare();
    std::vector<TermContext *> term();
    TermContext* term(size_t i);
    antlr4::tree::TerminalNode *In();
    antlr4::tree::TerminalNode *OpenPar();
    antlr4::tree::TerminalNode *ClosePar();
    antlr4::tree::TerminalNode *NotOper();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);
    antlr4::tree::TerminalNode *Between();
    antlr4::tree::TerminalNode *AndOper();
    antlr4::tree::TerminalNode *Is();
    antlr4::tree::TerminalNode *Null();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleconditionContext* simplecondition();

  class  CompareContext : public antlr4::ParserRuleContext {
  public:
    CompareContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NotEq();
    antlr4::tree::TerminalNode *LtOrEq();
    antlr4::tree::TerminalNode *GtOrEq();
    antlr4::tree::TerminalNode *Eq();
    antlr4::tree::TerminalNode *Lt();
    antlr4::tree::TerminalNode *Gt();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompareContext* compare();

  class  PropertyContext : public antlr4::ParserRuleContext {
  public:
    PropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);
    antlr4::tree::TerminalNode *Dot();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyContext* property();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueContext *value();
    BindParameterContext *bindParameter();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TermContext* term();

  class  BindParameterContext : public antlr4::ParserRuleContext {
  public:
    BindParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Dollar();
    antlr4::tree::TerminalNode *Identifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindParameterContext* bindParameter();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *String();
    NumericContext *numeric();
    BooleanContext *boolean();
    antlr4::tree::TerminalNode *Null();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();

  class  NumericContext : public antlr4::ParserRuleContext {
  public:
    NumericContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Integer();
    antlr4::tree::TerminalNode *Minus();
    antlr4::tree::TerminalNode *Decimal();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericContext* numeric();

  class  BooleanContext : public antlr4::ParserRuleContext {
  public:
    BooleanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BoolTrue();
    antlr4::tree::TerminalNode *BoolFalse();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanContext* boolean();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

}  // namespace arolios::qryfilter
