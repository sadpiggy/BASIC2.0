/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;
//enum commit{LET,REM,"RRINT","INPUT","END","IF","RUN","LIST"};//MAYBE
/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}
void END_Statement::execute(EvalState &state) {
    error("is_ended");
}
void REM_Statement::execute(EvalState &state){}
void GOTO_Statement:: execute(EvalState &state)
{
   // cout<<"debug";
    string line=integerToString(To_line);
    error(line);
}
void LET_Statement::execute(EvalState &state) {
    if(((CompoundExp*)expression)->getRHS()->getType()==IDENTIFIER)
    {
        string var=((CompoundExp*)expression)->getRHS()->toString();
       if(state.isDefined(var)==false)error("VARIABLE NOT DEFINED");
        state.setValue(((CompoundExp*)expression)->getLHS()->toString(),state.getValue(var));
        return;
    }
    //state.setValue(((CompoundExp*)expression)->getLHS()->toString(),stringToInteger(((CompoundExp*)expression)->getRHS()->toString()));
    //state.setValue(((CompoundExp*)expression).)
    state.setValue(((CompoundExp*)expression)->getLHS()->toString(),((CompoundExp*)expression)->getRHS()->eval(state));
}
void PRINT_Statement::execute(EvalState &state) {
  // cout<<"madpee";
    if(expression->getType()==CONSTANT){cout<<expression->eval(state)<<endl;return;}
    if(expression->getType()==IDENTIFIER)
    {
        //cout<<"madpee";
        if(state.isDefined(expression->toString())==false)error("VARIABLE NOT DEFINED");
        cout<<expression->eval(state)<<endl;
        return;
    }
  //  cout<<"madppee";
    cout<<expression->eval(state)<<endl;
}
void INPUT_Statement::execute(EvalState &state)
{
    //cout<<"debug";
    string in_line;int value;string var=expression->toString();
    cout<<" ? ";
    //in_line=getline();
   // in_line=getLine();
    //TokenScanner scanner(in_line);
    TokenScanner scanner;
    string token; TokenType token_type;
    scanner.ignoreWhitespace(); scanner.scanNumbers();
    while(true)
    {
        scanner.setInput(getLine());
        if(scanner.hasMoreTokens()==false){cout<<"INVALID NUMBER"<<endl<<" ? ";continue;}
        token=scanner.nextToken();token_type=scanner.getTokenType(token);
        if(token_type==NUMBER)
        {

            if(scanner.hasMoreTokens()){cout<<"INVALID NUMBER"<<endl<<" ? ";continue;}
            else{
                state.setValue(var,stringToInteger(token));//may bug
                break;
            }
        }
        if(token=="-")
        {
            if(scanner.hasMoreTokens()==false){cout<<"INVALID NUMBER"<<endl<<" ? ";continue;}
            token=scanner.nextToken();token_type=scanner.getTokenType(token);
            if(token_type==NUMBER){
                if(scanner.hasMoreTokens()==false){
                    state.setValue(var,stringToInteger(token));
                    break;
                }
            }
            //cout<<"INVALID NUMBER"<<endl<<"?";continue;
        }
        //cout<<"mid";
        //error("INVALID NUMBER");
        cout<<"INVALID NUMBER"<<endl<<" ? ";

    }
}
void IF_Statement::execute(EvalState &state)
{
    int left=leftex->eval(state);int right=rightex->eval(state);
    if(op=="="){
        if(left!=right)return;
    }
    if(op=="<"){
        if(left>=right)return;
    }
    if(op==">"){
        //cout<<"debug";
        if(left<=right){/*cout<<"debug";*/return;}
    }
    //cout<<"debug";
    GO_to->execute(state);
}
