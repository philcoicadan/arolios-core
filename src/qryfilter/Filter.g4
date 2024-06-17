//
//
// Copyright (C) 2024 Philippe Coicadan. All rights reserved.
// License: LGPL-3.0-or-later
//
//

grammar Filter;

//Parser rules

filter: andcondition (OrOper andcondition)*;

andcondition : condition (AndOper condition)* ;

condition: property simplecondition | NotOper filter | OpenPar filter ClosePar ;

simplecondition: compare term | 
                NotOper? In OpenPar term (Comma term)* ClosePar |
                NotOper? Between term AndOper term |
                Is NotOper? Null ;

compare: NotEq | LtOrEq | GtOrEq | Eq | Lt | Gt ;

property: Identifier (Dot Identifier)? ;

term : value | bindParameter ;

bindParameter: Dollar Identifier ;

value: String | numeric | boolean | Null;

numeric: Minus? Integer
        | Minus? Decimal ;

boolean: BoolTrue | BoolFalse ;

// Lexer rules

OrOper: 'OR' | 'or' | 'Or';
AndOper: 'AND' | 'and' | 'And' ;
NotOper: 'NOT'| 'not' | 'Not';
OpenPar: '(';
ClosePar: ')';
Comma: ',';
In: 'IN' | 'in' | 'In' ;
Between: 'BETWEEN' | 'between' | 'Between' ;
Is: 'IS' | 'is' | 'Is';
Null : 'NULL' | 'null' | 'Null';
NotEq: '!=' | '<>';
LtOrEq: '<=' | '&lt;=' ;
GtOrEq: '>=' | '&gt;=' ;
Minus: '-';
Eq: '=';
Lt: '<' | '&lt;' ;
Gt: '>' | '&gt;' ;
Dot: '.';
Dollar: '$';
BoolTrue: 'TRUE' | 'true' | 'True' ;
BoolFalse: 'FALSE' | 'false' | 'False' ;
String: '\'' ( ~('\''|'\\') | ('\\' .) )* '\''
      | '"' ( ~('"'|'\\') | ('\\' .) )* '"';
Integer: [0-9]+;
Decimal: DIGIT+ '.' DIGIT*
    | '.' DIGIT+ ;
Identifier: [a-zA-Z0-9][a-zA-Z0-9_-]*;


fragment DIGIT : [0-9];

WS: [ \r\n\t]+ ->skip ;









