/*
 * File: parser.h
 * --------------
 * This file acts as the interface to the parser module.
 */

#ifndef _parser_h
#define _parser_h

#include <string>
#include "exp.h"

#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"
/*
 * Function: parseExp//原来是parser来产生expression
 * Usage: Expression *exp = parseExp(scanner);
 * -------------------------------------------
 * Parses an expression by reading tokens(一个相关的string?) from the scanner, which must
 * be provided by the client.  The scanner should be set to ignore
 * whitespace and to scan numbers.//ignore space
 */
//20 INPUT a   ?
Expression *parseExp(TokenScanner & scanner);
Expression *newparseExp(TokenScanner & scanner);
Expression *newreadE(TokenScanner & scanner, int prec = 0);
int newprecedence(string token);
/*
 * Function: readE
 * Usage: Expression *exp = readE(scanner, prec);
 * ----------------------------------------------
 * Returns the next expression from the scanner involving only operators
 * whose precedence is at least prec.  The prec argument is optional and
 * defaults to 0, which means that the function reads the entire expression.//说什么玩意儿？
 */

Expression *readE(TokenScanner & scanner, int prec = 0);

/*
 * Function: readT
 * Usage: Expression *exp = readT(scanner);
 * ----------------------------------------
 * Returns the next individual term, which is either a constant, an
 * identifier, or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner);

/*
 * Function: precedence
 * Usage: int prec = precedence(token);
 * ------------------------------------
 * Returns the precedence of the specified operator token.  If the token
 * is not an operator, precedence returns 0.
 */

int precedence(std::string token);//所以说优先级是针对operator而言的？
//parse statement
Statement *parseStatement(string line,TokenScanner & scanner);
bool is_four_op(string op);
bool is_saved_words(string word);
#endif
