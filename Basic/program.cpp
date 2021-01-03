/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */
/*
 * This class stores the lines in a BASIC program.  Each line
 * in the program is stored in order according to its line number.
 * Moreover, each line in the program is associated with two
 * components:
 *
 * 1. The source line, which is the complete line (including the
 *    line number) that was entered by the user.
 *
 * 2. The parsed representation of that statement, which is a
 *    pointer to a Statement.
 */
#include <string>
#include "program.h"
#include "statement.h"
//#include""
using namespace std;
Program::Program() =default;
   // Replace this stub with your own code


Program::~Program() {
   // Replace this stub with your own code
    (*this).clear();
}

void Program::clear() {
   // Replace this stub with your own code
   if(first_line==-1)return;
   for(auto obj=Lines.begin();obj!=Lines.end();obj++){
       if(obj->second.stmt!= nullptr){
           delete obj->second.stmt;
       }
   }
   Lines.clear();
   first_line=-1;
}

void Program::addSourceLine(int lineNumber, string line) {
   // Replace this stub with your own code
   if(havecontained(lineNumber)==true){
       auto obj=Lines.find(lineNumber);
       delete obj->second.stmt;obj->second.stmt= nullptr;
       obj->second.sourse_line=line;
       first_line=getFirstLineNumber();
       return;
   }
   Lines.insert(make_pair(lineNumber,Line(line)));
   first_line=getFirstLineNumber();
}

void Program::removeSourceLine(int lineNumber) {
   // Replace this stub with your own code
   if(first_line==-1)return;
   auto obj=Lines.find(lineNumber);
   if(obj==Lines.end())return;
   delete obj->second.stmt;obj->second.stmt= nullptr;
   Lines.erase(lineNumber);
   first_line=getFirstLineNumber();
}

string Program::getSourceLine(int lineNumber) {
  if(havecontained(lineNumber)==false) return "";    // Replace this stub with your own code
  return Lines.find(lineNumber)->second.sourse_line;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   // Replace this stub with your own code
   if(havecontained(lineNumber)==false)error("[Warning] SYNTAX ERROR in setParsedStatement");
   auto obj=Lines.find(lineNumber);
   delete obj->second.stmt;obj->second.stmt=stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    if(havecontained(lineNumber)==false)return  NULL;
   // return NULL;  // Replace this stub with your own code//??//
    return Lines.find(lineNumber)->second.stmt;
}

/*int Program::getFirstLineNumber() {
   return 0;     // Replace this stub with your own code
}*/

int Program::getNextLineNumber(int lineNumber) {
   if(first_line==-1)return 0;
   auto obj=Lines.upper_bound(lineNumber);
   if(obj==Lines.end())return 0;     // Replace this stub with your own code
    return obj->first;
}
void Program::Showlist(){
    if(first_line==-1)return;
    for(auto obj=Lines.begin();obj!=Lines.end();obj++){
        cout<<obj->second.sourse_line<<endl;
    }
}
void Program::RunProgram(EvalState &evalstate){//TODO//uncompleted
    if(first_line==-1)return;
    for(auto obj=Lines.begin();obj!=Lines.end();obj++)
    {
        Statement* statement=obj->second.stmt;
        try {
            //if(is_ended==true)break;
            statement->execute(evalstate);
        }
        catch (ErrorException &errorex)
        {
            string message = errorex.getMessage();
            if (errorex.getMessage() == "is_ended") { return; }
            TokenScanner scanner;
            if (scanner.getTokenType(errorex.getMessage()) == NUMBER)
            {
                if (this->havecontained(stringToInteger(errorex.getMessage())) == true)
                {
                    obj = this->Lines.find(stringToInteger(errorex.getMessage()));
                    obj--;
                    continue;
                } else error("LINE NUMBER ERROR");
            }
            else error(message);
        }
    }
}