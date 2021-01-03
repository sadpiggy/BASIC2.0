/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;
//readE优先值阅读
//readT读下一个
/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner & scanner) {//这我感觉有问题
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {
      //error("parseExp: Found extra token: " + scanner.nextToken());//是会break吗
      error("SYNTAX ERROR");
   }
   return exp;
}

Expression *newparseExp(TokenScanner & scanner) {
    Expression *exp = newreadE(scanner);
    if (scanner.hasMoreTokens())
    {
        string token=scanner.nextToken();
        TokenType tokenType=scanner.getTokenType(token);
        if(token=="THEN")
        {
            return exp;//注意，THEN token已经被吃了
        }
        if(tokenType!=OPERATOR)error("SYNTAX ERROR");
        if(token=="<"||token==">"||token=="=")
        {
            string op=token;
            Expression *right=newparseExp(scanner);
            return new CompoundExp(op,exp,right);
        }
        if(token=="+"||token=="-"||token=="*"||token=="/")
        {
            string op;
            while(true)
            {
                exp=new CompoundExp(token,exp,newreadE(scanner));
                if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");
                string token=scanner.nextToken();
                if(token=="<"||token==">"||token=="="){op=token;break;}
            }
            Expression *right=newparseExp(scanner);
            return new CompoundExp(op,exp,right);
        }
    }
  error("SYNTAX ERROR");//我在写什么鬼？
}
/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity//模棱两可 in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing(平凡的？) one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;//这里是不是写错了？应该是<吧？不然怎么计算5+5+5+5呢？
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);//op l right
   }
   scanner.saveToken(token);
   return exp;
}
Expression *newreadE(TokenScanner & scanner, int prec) {
    Expression *exp = readT(scanner);
    string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = newprecedence(token);
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec);
        exp = new CompoundExp(token, exp, rhs);//op l right
    }
    scanner.saveToken(token);
    return exp;
}
/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
    if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");//此处被我自己改过!!!
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stringToInteger(token));//
   if (token != "(") error("Illegal term in expression");
   Expression *exp = readE(scanner);//oeprator 为什么没有考虑？
   if (scanner.nextToken() != ")") {
     // error("Unbalanced parentheses in expression");
       error("SYNTAX ERROR");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

Statement *parseStatement(string line,TokenScanner & scanner)
{
    string token=scanner.nextToken();
    TokenType tokenType=scanner.getTokenType(token);
    Expression* expression= nullptr;
    scanner.ignoreWhitespace();scanner.scanNumbers();//引用前已经初始化过了，现在应该没必要再初始化了。但是，如果我还是这样做了。会有报应马？
    if(tokenType!=WORD)error("SYNTAX ERROR");
    if(is_saved_words(token)==false)error("SYNTAX ERROR");
    if(token=="REM"){
        return  new REM_Statement;
    }
    if(token=="END"){
        if(scanner.hasMoreTokens())error("SYNTAX ERROR");
        return new END_Statement;
    }
    if(token=="LET"){
        if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");
        Expression* expression= parseExp(scanner);
        if(expression->getType()!=COMPOUND)error("SYNTAX ERROR");
        if(((CompoundExp* )expression)->getOp()!="=")error("SYNTAX ERROR");
        if(((CompoundExp* )expression)->getLHS()->getType()!=IDENTIFIER)error("SYNTAX ERROR");
       if(is_saved_words(((CompoundExp* )expression)->getLHS()->toString()))error("SYNTAX ERROR");
        if(((CompoundExp* )expression)->getRHS()->getType()==CONSTANT)return new LET_Statement(expression);
        if(((CompoundExp* )expression)->getRHS()->getType()==IDENTIFIER)
        {
            if(is_saved_words(((CompoundExp* )expression)->getRHS()->toString())==true)error("SYNTAX ERROR");
            return new LET_Statement(expression);
        }
        if(((CompoundExp* )expression)->getRHS()->getType()==COMPOUND)
        {
            return new LET_Statement(expression);
        }
    }
    if(token=="PRINT"){
        if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");
        Expression* expression=parseExp(scanner);
        if(expression->getType()==CONSTANT)return new PRINT_Statement(expression);
        if(expression->getType()==IDENTIFIER)
        {
            if(is_saved_words(expression->toString())==true)error("SYNTAX ERROR");
            //if(is_saved_words(((CompoundExp* )expression)->getRHS()->toString())==true)error("SYNTAX ERROR");
            return new PRINT_Statement(expression);
        }
        if(is_four_op(((CompoundExp* )expression)->getOp()))return new PRINT_Statement(expression);
        error("SYNTAX ERROR");
    }
    if(token=="INPUT")
    {
        if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");
        Expression* expression=parseExp(scanner);
        if(expression->getType()!=IDENTIFIER)error("SYNTAX ERROR");
        if(is_saved_words(expression->toString())==true)error("SYNTAX ERROR");
        return new INPUT_Statement(expression);
    }
    if(token=="GOTO"){
        if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");
        string token=scanner.nextToken();
        TokenType tokenType=scanner.getTokenType(token);
        if(tokenType!=NUMBER)error("SYNTAX ERROR");
        if(scanner.hasMoreTokens()==true)error("SYNTAX ERROR");
        return new GOTO_Statement(stringToInteger(token));
    }
    if(token=="IF")
    {
        if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");
        /*Expression* expression=parseExp(scanner);
        if(expression->getType()!=COMPOUND)error("SYNTAX ERROR");
        string op=((CompoundExp*)expression)->getOp();
        if(is_four_op(op)==false)error("SYNTAX ERROR");*/
        //if(((CompoundExp*)expression)->getOp())
        Expression *expression=newparseExp(scanner);
        if(scanner.hasMoreTokens()==false)error("SYNTAX ERROR");
        string token=scanner.nextToken();
        TokenType tokenType=scanner.getTokenType(token);
        if(tokenType!=NUMBER)error("SYNTAX ERROR");
        auto left=((CompoundExp*)expression)->getLHS();
        auto right=((CompoundExp*)expression)->getRHS();
        auto op=((CompoundExp*)expression)->getOp();
       // return new GOTO_Statement(stringToInteger(token));
        return new IF_Statement(left,op,right,new GOTO_Statement(stringToInteger(token)));
    }
    //cout<<"SYNTAX ERROR"<<endl;
    error("SYNTAX ERROR");
    //if(token==""){}
}

int precedence(string token) {
   if (token == "=") return 1;
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
    //if (token == "<"||token == ">") return 1;//我又改了，注意出问题
   return 0;
}
int newprecedence(string token) {
    if (token == "=") return -1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    if (token == "<"||token == ">") return -1;
    return 0;
}
bool is_saved_words(string word){
    if(word=="LIST"||word=="RUN"||word=="END"||word=="REM"||word=="LET"||word=="PRINT"||word=="INPUT"||word=="IF"||word=="GOTO"||word=="HELP"||word=="THEN")return true;
    return false;
}
bool is_four_op(string op){
    if(op=="+"||op=="-"||op=="*"||op=="/")return true;
    return false;
}