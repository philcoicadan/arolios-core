
// Generated from Filter.g4 by ANTLR 4.13.0


#include "FilterListener.h"
#include "FilterVisitor.h"

#include "FilterParser.h"


using namespace antlrcpp;
using namespace arolios::qryfilter;

using namespace antlr4;

namespace {

struct FilterParserStaticData final {
  FilterParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FilterParserStaticData(const FilterParserStaticData&) = delete;
  FilterParserStaticData(FilterParserStaticData&&) = delete;
  FilterParserStaticData& operator=(const FilterParserStaticData&) = delete;
  FilterParserStaticData& operator=(FilterParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag filterParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
FilterParserStaticData *filterParserStaticData = nullptr;

void filterParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (filterParserStaticData != nullptr) {
    return;
  }
#else
  assert(filterParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<FilterParserStaticData>(
    std::vector<std::string>{
      "filter", "andcondition", "condition", "simplecondition", "compare", 
      "property", "term", "bindParameter", "value", "numeric", "boolean"
    },
    std::vector<std::string>{
      "", "", "", "", "'('", "')'", "','", "", "", "", "", "", "", "", "'-'", 
      "'='", "", "", "'.'", "'$'"
    },
    std::vector<std::string>{
      "", "OrOper", "AndOper", "NotOper", "OpenPar", "ClosePar", "Comma", 
      "In", "Between", "Is", "Null", "NotEq", "LtOrEq", "GtOrEq", "Minus", 
      "Eq", "Lt", "Gt", "Dot", "Dollar", "BoolTrue", "BoolFalse", "String", 
      "Integer", "Decimal", "Identifier", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,26,115,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,1,0,1,0,1,0,5,0,26,8,0,10,0,12,0,29,9,
  	0,1,1,1,1,1,1,5,1,34,8,1,10,1,12,1,37,9,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,
  	1,2,1,2,3,2,48,8,2,1,3,1,3,1,3,1,3,3,3,54,8,3,1,3,1,3,1,3,1,3,1,3,5,3,
  	61,8,3,10,3,12,3,64,9,3,1,3,1,3,1,3,3,3,69,8,3,1,3,1,3,1,3,1,3,1,3,1,
  	3,1,3,3,3,78,8,3,1,3,3,3,81,8,3,1,4,1,4,1,5,1,5,1,5,3,5,88,8,5,1,6,1,
  	6,3,6,92,8,6,1,7,1,7,1,7,1,8,1,8,1,8,1,8,3,8,101,8,8,1,9,3,9,104,8,9,
  	1,9,1,9,3,9,108,8,9,1,9,3,9,111,8,9,1,10,1,10,1,10,0,0,11,0,2,4,6,8,10,
  	12,14,16,18,20,0,2,2,0,11,13,15,17,1,0,20,21,122,0,22,1,0,0,0,2,30,1,
  	0,0,0,4,47,1,0,0,0,6,80,1,0,0,0,8,82,1,0,0,0,10,84,1,0,0,0,12,91,1,0,
  	0,0,14,93,1,0,0,0,16,100,1,0,0,0,18,110,1,0,0,0,20,112,1,0,0,0,22,27,
  	3,2,1,0,23,24,5,1,0,0,24,26,3,2,1,0,25,23,1,0,0,0,26,29,1,0,0,0,27,25,
  	1,0,0,0,27,28,1,0,0,0,28,1,1,0,0,0,29,27,1,0,0,0,30,35,3,4,2,0,31,32,
  	5,2,0,0,32,34,3,4,2,0,33,31,1,0,0,0,34,37,1,0,0,0,35,33,1,0,0,0,35,36,
  	1,0,0,0,36,3,1,0,0,0,37,35,1,0,0,0,38,39,3,10,5,0,39,40,3,6,3,0,40,48,
  	1,0,0,0,41,42,5,3,0,0,42,48,3,0,0,0,43,44,5,4,0,0,44,45,3,0,0,0,45,46,
  	5,5,0,0,46,48,1,0,0,0,47,38,1,0,0,0,47,41,1,0,0,0,47,43,1,0,0,0,48,5,
  	1,0,0,0,49,50,3,8,4,0,50,51,3,12,6,0,51,81,1,0,0,0,52,54,5,3,0,0,53,52,
  	1,0,0,0,53,54,1,0,0,0,54,55,1,0,0,0,55,56,5,7,0,0,56,57,5,4,0,0,57,62,
  	3,12,6,0,58,59,5,6,0,0,59,61,3,12,6,0,60,58,1,0,0,0,61,64,1,0,0,0,62,
  	60,1,0,0,0,62,63,1,0,0,0,63,65,1,0,0,0,64,62,1,0,0,0,65,66,5,5,0,0,66,
  	81,1,0,0,0,67,69,5,3,0,0,68,67,1,0,0,0,68,69,1,0,0,0,69,70,1,0,0,0,70,
  	71,5,8,0,0,71,72,3,12,6,0,72,73,5,2,0,0,73,74,3,12,6,0,74,81,1,0,0,0,
  	75,77,5,9,0,0,76,78,5,3,0,0,77,76,1,0,0,0,77,78,1,0,0,0,78,79,1,0,0,0,
  	79,81,5,10,0,0,80,49,1,0,0,0,80,53,1,0,0,0,80,68,1,0,0,0,80,75,1,0,0,
  	0,81,7,1,0,0,0,82,83,7,0,0,0,83,9,1,0,0,0,84,87,5,25,0,0,85,86,5,18,0,
  	0,86,88,5,25,0,0,87,85,1,0,0,0,87,88,1,0,0,0,88,11,1,0,0,0,89,92,3,16,
  	8,0,90,92,3,14,7,0,91,89,1,0,0,0,91,90,1,0,0,0,92,13,1,0,0,0,93,94,5,
  	19,0,0,94,95,5,25,0,0,95,15,1,0,0,0,96,101,5,22,0,0,97,101,3,18,9,0,98,
  	101,3,20,10,0,99,101,5,10,0,0,100,96,1,0,0,0,100,97,1,0,0,0,100,98,1,
  	0,0,0,100,99,1,0,0,0,101,17,1,0,0,0,102,104,5,14,0,0,103,102,1,0,0,0,
  	103,104,1,0,0,0,104,105,1,0,0,0,105,111,5,23,0,0,106,108,5,14,0,0,107,
  	106,1,0,0,0,107,108,1,0,0,0,108,109,1,0,0,0,109,111,5,24,0,0,110,103,
  	1,0,0,0,110,107,1,0,0,0,111,19,1,0,0,0,112,113,7,1,0,0,113,21,1,0,0,0,
  	14,27,35,47,53,62,68,77,80,87,91,100,103,107,110
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  filterParserStaticData = staticData.release();
}

}

FilterParser::FilterParser(TokenStream *input) : FilterParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

FilterParser::FilterParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  FilterParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *filterParserStaticData->atn, filterParserStaticData->decisionToDFA, filterParserStaticData->sharedContextCache, options);
}

FilterParser::~FilterParser() {
  delete _interpreter;
}

const atn::ATN& FilterParser::getATN() const {
  return *filterParserStaticData->atn;
}

std::string FilterParser::getGrammarFileName() const {
  return "Filter.g4";
}

const std::vector<std::string>& FilterParser::getRuleNames() const {
  return filterParserStaticData->ruleNames;
}

const dfa::Vocabulary& FilterParser::getVocabulary() const {
  return filterParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FilterParser::getSerializedATN() const {
  return filterParserStaticData->serializedATN;
}


//----------------- FilterContext ------------------------------------------------------------------

FilterParser::FilterContext::FilterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<FilterParser::AndconditionContext *> FilterParser::FilterContext::andcondition() {
  return getRuleContexts<FilterParser::AndconditionContext>();
}

FilterParser::AndconditionContext* FilterParser::FilterContext::andcondition(size_t i) {
  return getRuleContext<FilterParser::AndconditionContext>(i);
}

std::vector<tree::TerminalNode *> FilterParser::FilterContext::OrOper() {
  return getTokens(FilterParser::OrOper);
}

tree::TerminalNode* FilterParser::FilterContext::OrOper(size_t i) {
  return getToken(FilterParser::OrOper, i);
}


size_t FilterParser::FilterContext::getRuleIndex() const {
  return FilterParser::RuleFilter;
}

void FilterParser::FilterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFilter(this);
}

void FilterParser::FilterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFilter(this);
}


std::any FilterParser::FilterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitFilter(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::FilterContext* FilterParser::filter() {
  FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, getState());
  enterRule(_localctx, 0, FilterParser::RuleFilter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(22);
    andcondition();
    setState(27);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(23);
        match(FilterParser::OrOper);
        setState(24);
        andcondition(); 
      }
      setState(29);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AndconditionContext ------------------------------------------------------------------

FilterParser::AndconditionContext::AndconditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<FilterParser::ConditionContext *> FilterParser::AndconditionContext::condition() {
  return getRuleContexts<FilterParser::ConditionContext>();
}

FilterParser::ConditionContext* FilterParser::AndconditionContext::condition(size_t i) {
  return getRuleContext<FilterParser::ConditionContext>(i);
}

std::vector<tree::TerminalNode *> FilterParser::AndconditionContext::AndOper() {
  return getTokens(FilterParser::AndOper);
}

tree::TerminalNode* FilterParser::AndconditionContext::AndOper(size_t i) {
  return getToken(FilterParser::AndOper, i);
}


size_t FilterParser::AndconditionContext::getRuleIndex() const {
  return FilterParser::RuleAndcondition;
}

void FilterParser::AndconditionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAndcondition(this);
}

void FilterParser::AndconditionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAndcondition(this);
}


std::any FilterParser::AndconditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitAndcondition(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::AndconditionContext* FilterParser::andcondition() {
  AndconditionContext *_localctx = _tracker.createInstance<AndconditionContext>(_ctx, getState());
  enterRule(_localctx, 2, FilterParser::RuleAndcondition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(30);
    condition();
    setState(35);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(31);
        match(FilterParser::AndOper);
        setState(32);
        condition(); 
      }
      setState(37);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionContext ------------------------------------------------------------------

FilterParser::ConditionContext::ConditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

FilterParser::PropertyContext* FilterParser::ConditionContext::property() {
  return getRuleContext<FilterParser::PropertyContext>(0);
}

FilterParser::SimpleconditionContext* FilterParser::ConditionContext::simplecondition() {
  return getRuleContext<FilterParser::SimpleconditionContext>(0);
}

tree::TerminalNode* FilterParser::ConditionContext::NotOper() {
  return getToken(FilterParser::NotOper, 0);
}

FilterParser::FilterContext* FilterParser::ConditionContext::filter() {
  return getRuleContext<FilterParser::FilterContext>(0);
}

tree::TerminalNode* FilterParser::ConditionContext::OpenPar() {
  return getToken(FilterParser::OpenPar, 0);
}

tree::TerminalNode* FilterParser::ConditionContext::ClosePar() {
  return getToken(FilterParser::ClosePar, 0);
}


size_t FilterParser::ConditionContext::getRuleIndex() const {
  return FilterParser::RuleCondition;
}

void FilterParser::ConditionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCondition(this);
}

void FilterParser::ConditionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCondition(this);
}


std::any FilterParser::ConditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitCondition(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::ConditionContext* FilterParser::condition() {
  ConditionContext *_localctx = _tracker.createInstance<ConditionContext>(_ctx, getState());
  enterRule(_localctx, 4, FilterParser::RuleCondition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(47);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case FilterParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(38);
        property();
        setState(39);
        simplecondition();
        break;
      }

      case FilterParser::NotOper: {
        enterOuterAlt(_localctx, 2);
        setState(41);
        match(FilterParser::NotOper);
        setState(42);
        filter();
        break;
      }

      case FilterParser::OpenPar: {
        enterOuterAlt(_localctx, 3);
        setState(43);
        match(FilterParser::OpenPar);
        setState(44);
        filter();
        setState(45);
        match(FilterParser::ClosePar);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleconditionContext ------------------------------------------------------------------

FilterParser::SimpleconditionContext::SimpleconditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

FilterParser::CompareContext* FilterParser::SimpleconditionContext::compare() {
  return getRuleContext<FilterParser::CompareContext>(0);
}

std::vector<FilterParser::TermContext *> FilterParser::SimpleconditionContext::term() {
  return getRuleContexts<FilterParser::TermContext>();
}

FilterParser::TermContext* FilterParser::SimpleconditionContext::term(size_t i) {
  return getRuleContext<FilterParser::TermContext>(i);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::In() {
  return getToken(FilterParser::In, 0);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::OpenPar() {
  return getToken(FilterParser::OpenPar, 0);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::ClosePar() {
  return getToken(FilterParser::ClosePar, 0);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::NotOper() {
  return getToken(FilterParser::NotOper, 0);
}

std::vector<tree::TerminalNode *> FilterParser::SimpleconditionContext::Comma() {
  return getTokens(FilterParser::Comma);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::Comma(size_t i) {
  return getToken(FilterParser::Comma, i);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::Between() {
  return getToken(FilterParser::Between, 0);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::AndOper() {
  return getToken(FilterParser::AndOper, 0);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::Is() {
  return getToken(FilterParser::Is, 0);
}

tree::TerminalNode* FilterParser::SimpleconditionContext::Null() {
  return getToken(FilterParser::Null, 0);
}


size_t FilterParser::SimpleconditionContext::getRuleIndex() const {
  return FilterParser::RuleSimplecondition;
}

void FilterParser::SimpleconditionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimplecondition(this);
}

void FilterParser::SimpleconditionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimplecondition(this);
}


std::any FilterParser::SimpleconditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitSimplecondition(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::SimpleconditionContext* FilterParser::simplecondition() {
  SimpleconditionContext *_localctx = _tracker.createInstance<SimpleconditionContext>(_ctx, getState());
  enterRule(_localctx, 6, FilterParser::RuleSimplecondition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(80);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(49);
      compare();
      setState(50);
      term();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(53);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == FilterParser::NotOper) {
        setState(52);
        match(FilterParser::NotOper);
      }
      setState(55);
      match(FilterParser::In);
      setState(56);
      match(FilterParser::OpenPar);
      setState(57);
      term();
      setState(62);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == FilterParser::Comma) {
        setState(58);
        match(FilterParser::Comma);
        setState(59);
        term();
        setState(64);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(65);
      match(FilterParser::ClosePar);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(68);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == FilterParser::NotOper) {
        setState(67);
        match(FilterParser::NotOper);
      }
      setState(70);
      match(FilterParser::Between);
      setState(71);
      term();
      setState(72);
      match(FilterParser::AndOper);
      setState(73);
      term();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(75);
      match(FilterParser::Is);
      setState(77);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == FilterParser::NotOper) {
        setState(76);
        match(FilterParser::NotOper);
      }
      setState(79);
      match(FilterParser::Null);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CompareContext ------------------------------------------------------------------

FilterParser::CompareContext::CompareContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FilterParser::CompareContext::NotEq() {
  return getToken(FilterParser::NotEq, 0);
}

tree::TerminalNode* FilterParser::CompareContext::LtOrEq() {
  return getToken(FilterParser::LtOrEq, 0);
}

tree::TerminalNode* FilterParser::CompareContext::GtOrEq() {
  return getToken(FilterParser::GtOrEq, 0);
}

tree::TerminalNode* FilterParser::CompareContext::Eq() {
  return getToken(FilterParser::Eq, 0);
}

tree::TerminalNode* FilterParser::CompareContext::Lt() {
  return getToken(FilterParser::Lt, 0);
}

tree::TerminalNode* FilterParser::CompareContext::Gt() {
  return getToken(FilterParser::Gt, 0);
}


size_t FilterParser::CompareContext::getRuleIndex() const {
  return FilterParser::RuleCompare;
}

void FilterParser::CompareContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCompare(this);
}

void FilterParser::CompareContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCompare(this);
}


std::any FilterParser::CompareContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitCompare(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::CompareContext* FilterParser::compare() {
  CompareContext *_localctx = _tracker.createInstance<CompareContext>(_ctx, getState());
  enterRule(_localctx, 8, FilterParser::RuleCompare);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(82);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 243712) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyContext ------------------------------------------------------------------

FilterParser::PropertyContext::PropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> FilterParser::PropertyContext::Identifier() {
  return getTokens(FilterParser::Identifier);
}

tree::TerminalNode* FilterParser::PropertyContext::Identifier(size_t i) {
  return getToken(FilterParser::Identifier, i);
}

tree::TerminalNode* FilterParser::PropertyContext::Dot() {
  return getToken(FilterParser::Dot, 0);
}


size_t FilterParser::PropertyContext::getRuleIndex() const {
  return FilterParser::RuleProperty;
}

void FilterParser::PropertyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProperty(this);
}

void FilterParser::PropertyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProperty(this);
}


std::any FilterParser::PropertyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitProperty(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::PropertyContext* FilterParser::property() {
  PropertyContext *_localctx = _tracker.createInstance<PropertyContext>(_ctx, getState());
  enterRule(_localctx, 10, FilterParser::RuleProperty);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(84);
    match(FilterParser::Identifier);
    setState(87);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FilterParser::Dot) {
      setState(85);
      match(FilterParser::Dot);
      setState(86);
      match(FilterParser::Identifier);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TermContext ------------------------------------------------------------------

FilterParser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

FilterParser::ValueContext* FilterParser::TermContext::value() {
  return getRuleContext<FilterParser::ValueContext>(0);
}

FilterParser::BindParameterContext* FilterParser::TermContext::bindParameter() {
  return getRuleContext<FilterParser::BindParameterContext>(0);
}


size_t FilterParser::TermContext::getRuleIndex() const {
  return FilterParser::RuleTerm;
}

void FilterParser::TermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTerm(this);
}

void FilterParser::TermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTerm(this);
}


std::any FilterParser::TermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitTerm(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::TermContext* FilterParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 12, FilterParser::RuleTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(91);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case FilterParser::Null:
      case FilterParser::Minus:
      case FilterParser::BoolTrue:
      case FilterParser::BoolFalse:
      case FilterParser::String:
      case FilterParser::Integer:
      case FilterParser::Decimal: {
        enterOuterAlt(_localctx, 1);
        setState(89);
        value();
        break;
      }

      case FilterParser::Dollar: {
        enterOuterAlt(_localctx, 2);
        setState(90);
        bindParameter();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BindParameterContext ------------------------------------------------------------------

FilterParser::BindParameterContext::BindParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FilterParser::BindParameterContext::Dollar() {
  return getToken(FilterParser::Dollar, 0);
}

tree::TerminalNode* FilterParser::BindParameterContext::Identifier() {
  return getToken(FilterParser::Identifier, 0);
}


size_t FilterParser::BindParameterContext::getRuleIndex() const {
  return FilterParser::RuleBindParameter;
}

void FilterParser::BindParameterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBindParameter(this);
}

void FilterParser::BindParameterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBindParameter(this);
}


std::any FilterParser::BindParameterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitBindParameter(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::BindParameterContext* FilterParser::bindParameter() {
  BindParameterContext *_localctx = _tracker.createInstance<BindParameterContext>(_ctx, getState());
  enterRule(_localctx, 14, FilterParser::RuleBindParameter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    match(FilterParser::Dollar);
    setState(94);
    match(FilterParser::Identifier);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

FilterParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FilterParser::ValueContext::String() {
  return getToken(FilterParser::String, 0);
}

FilterParser::NumericContext* FilterParser::ValueContext::numeric() {
  return getRuleContext<FilterParser::NumericContext>(0);
}

FilterParser::BooleanContext* FilterParser::ValueContext::boolean() {
  return getRuleContext<FilterParser::BooleanContext>(0);
}

tree::TerminalNode* FilterParser::ValueContext::Null() {
  return getToken(FilterParser::Null, 0);
}


size_t FilterParser::ValueContext::getRuleIndex() const {
  return FilterParser::RuleValue;
}

void FilterParser::ValueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValue(this);
}

void FilterParser::ValueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValue(this);
}


std::any FilterParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::ValueContext* FilterParser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 16, FilterParser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(100);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case FilterParser::String: {
        enterOuterAlt(_localctx, 1);
        setState(96);
        match(FilterParser::String);
        break;
      }

      case FilterParser::Minus:
      case FilterParser::Integer:
      case FilterParser::Decimal: {
        enterOuterAlt(_localctx, 2);
        setState(97);
        numeric();
        break;
      }

      case FilterParser::BoolTrue:
      case FilterParser::BoolFalse: {
        enterOuterAlt(_localctx, 3);
        setState(98);
        boolean();
        break;
      }

      case FilterParser::Null: {
        enterOuterAlt(_localctx, 4);
        setState(99);
        match(FilterParser::Null);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericContext ------------------------------------------------------------------

FilterParser::NumericContext::NumericContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FilterParser::NumericContext::Integer() {
  return getToken(FilterParser::Integer, 0);
}

tree::TerminalNode* FilterParser::NumericContext::Minus() {
  return getToken(FilterParser::Minus, 0);
}

tree::TerminalNode* FilterParser::NumericContext::Decimal() {
  return getToken(FilterParser::Decimal, 0);
}


size_t FilterParser::NumericContext::getRuleIndex() const {
  return FilterParser::RuleNumeric;
}

void FilterParser::NumericContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumeric(this);
}

void FilterParser::NumericContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumeric(this);
}


std::any FilterParser::NumericContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitNumeric(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::NumericContext* FilterParser::numeric() {
  NumericContext *_localctx = _tracker.createInstance<NumericContext>(_ctx, getState());
  enterRule(_localctx, 18, FilterParser::RuleNumeric);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(110);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(103);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == FilterParser::Minus) {
        setState(102);
        match(FilterParser::Minus);
      }
      setState(105);
      match(FilterParser::Integer);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(107);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == FilterParser::Minus) {
        setState(106);
        match(FilterParser::Minus);
      }
      setState(109);
      match(FilterParser::Decimal);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanContext ------------------------------------------------------------------

FilterParser::BooleanContext::BooleanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FilterParser::BooleanContext::BoolTrue() {
  return getToken(FilterParser::BoolTrue, 0);
}

tree::TerminalNode* FilterParser::BooleanContext::BoolFalse() {
  return getToken(FilterParser::BoolFalse, 0);
}


size_t FilterParser::BooleanContext::getRuleIndex() const {
  return FilterParser::RuleBoolean;
}

void FilterParser::BooleanContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBoolean(this);
}

void FilterParser::BooleanContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBoolean(this);
}


std::any FilterParser::BooleanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FilterVisitor*>(visitor))
    return parserVisitor->visitBoolean(this);
  else
    return visitor->visitChildren(this);
}

FilterParser::BooleanContext* FilterParser::boolean() {
  BooleanContext *_localctx = _tracker.createInstance<BooleanContext>(_ctx, getState());
  enterRule(_localctx, 20, FilterParser::RuleBoolean);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(112);
    _la = _input->LA(1);
    if (!(_la == FilterParser::BoolTrue

    || _la == FilterParser::BoolFalse)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void FilterParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  filterParserInitialize();
#else
  ::antlr4::internal::call_once(filterParserOnceFlag, filterParserInitialize);
#endif
}
