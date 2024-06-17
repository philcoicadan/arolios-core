
// Generated from Filter.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"


namespace arolios::qryfilter {


class  FilterLexer : public antlr4::Lexer {
public:
  enum {
    OrOper = 1, AndOper = 2, NotOper = 3, OpenPar = 4, ClosePar = 5, Comma = 6, 
    In = 7, Between = 8, Is = 9, Null = 10, NotEq = 11, LtOrEq = 12, GtOrEq = 13, 
    Minus = 14, Eq = 15, Lt = 16, Gt = 17, Dot = 18, Dollar = 19, BoolTrue = 20, 
    BoolFalse = 21, String = 22, Integer = 23, Decimal = 24, Identifier = 25, 
    WS = 26
  };

  explicit FilterLexer(antlr4::CharStream *input);

  ~FilterLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

}  // namespace arolios::qryfilter
