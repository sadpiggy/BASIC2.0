/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */
bool check_three(string token){if(token=="LET"||token=="INPUT"||token=="PRINT")return true;return false;}
void processLine(string line, Program & program, EvalState & state);
//20 INPUT n1
//25 LET n1=n2
/* Main program */

int main() 
{
   EvalState state;//一个存变量值的map
   Program program;//一个存操作行的map
   //cout << "Stub implementation of BASIC" << endl;//坑死了
   while (true)
   {
      try
      {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex)
      {
         //cerr << "Error: " << ex.getMessage() << endl;
         cout<<ex.getMessage()<<endl;
      }
      catch(...){cout<<"SYNNAX ERROR"<<endl;}
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);//
   string token; TokenType tokentype;
   if(!scanner.hasMoreTokens())return;
   token=scanner.nextToken();
   tokentype=scanner.getTokenType(token);
    if(tokentype == NUMBER)//常规版
    {
        int  linenumber = stringToInteger(token);//得到line number
        if(!scanner.hasMoreTokens())
        {
            program.removeSourceLine(linenumber);//执行删除
            return;
        }
        try
        {
            Statement *stmt = parseStatement(line,scanner);
            program.addSourceLine(linenumber,line);
            program.setParsedStatement(linenumber,stmt);//执行时才会报错
            return;
        } catch (...) {
            error("SYNTAX ERROR");
        }
    }
    //LET PRINT END INPUT REM
    if(tokentype==WORD)
    {
        if(check_three(token))
        {
           scanner.saveToken(token);
            try {
                Statement *stmt=parseStatement(line,scanner);
                stmt->execute(state);
            }
            catch (ErrorException &error)
            {
                if(error.getMessage()=="DIVIDE BY ZERO"){throw  ErrorException (error.getMessage());}
                if(error.getMessage()=="INVALID NUMBER"){throw    ErrorException (error.getMessage());}
                if(error.getMessage()=="VARIABLE NOT DEFINED"){throw   ErrorException (error.getMessage());}
                if(error.getMessage()=="LINE NUMBER ERROR"){throw  ErrorException (error.getMessage());}
                throw  ErrorException ("SYNTAX ERROR");
            }
           return;
        }
        if(token=="RUN")
        {
            if(scanner.hasMoreTokens()==true){
               error("SYNTAX ERROR");
            }
            try 
            {
                program.RunProgram(state);
                return;
            }
            catch (ErrorException &error)
            {
               // ErrorException newerror;
             if(error.getMessage()=="DIVIDE BY ZERO"){throw  ErrorException (error.getMessage());}
             if(error.getMessage()=="INVALID NUMBER"){throw    ErrorException (error.getMessage());}
             if(error.getMessage()=="VARIABLE NOT DEFINED"){throw   ErrorException (error.getMessage());}
             if(error.getMessage()=="LINE NUMBER ERROR"){throw  ErrorException (error.getMessage());}
              throw  ErrorException ("SYNTAX ERROR");
            }
            catch(...){cout<<"WKY 你的程序出问题了";}
        }
        if(token=="LIST")
        {
            if(scanner.hasMoreTokens()==true){
                error("SYNTAX ERROR");
            }
            program.Showlist();
            return ;
        }
        if(token=="CLEAR")
        {
            if(scanner.hasMoreTokens()==true){
                error("SYNTAX ERROR");
            }
            program.clear();
            state.clear();//这里曾经出了一个BUG
            return;
        }
        if(token=="QUIT"){
            if(scanner.hasMoreTokens()==true){
                error("SYNTAX ERROR");
            }
            exit(0);
        }
        if(token=="HELP")
        {
            if(scanner.hasMoreTokens()==true){
                error("SYNTAX ERROR");
            }
            cout<<"Yet another basic interpreter";return;
        }
        //cout<<"SYNTAX ERROR"<<endl;return;
        throw ErrorException("SYNTAX ERROR");
    }
    if(tokentype!=NUMBER&&tokentype!=WORD){error("SYNTAX ERROR");};
}
//map<int,bool>line_is_delete;想多了，DELETE之后还是可以继续使用
