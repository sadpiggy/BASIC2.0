/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.//?what
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
//#include<string>
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 *                           stmt声明
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.//不是很懂
 */

   virtual void execute(EvalState & state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for//解释一个statement 然后执行？
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */
class END_Statement: public Statement{
public:
    END_Statement()=default;
    virtual ~END_Statement()=default;
    void execute(EvalState &state);
};
class REM_Statement: public Statement{
public:
    REM_Statement()=default;
    virtual ~REM_Statement()=default;
    void execute(EvalState &state);
};
class GOTO_Statement: public Statement{
public:
    explicit GOTO_Statement(int to_line):To_line(to_line){}
    virtual ~GOTO_Statement()=default;
    void execute(EvalState &state);

private:
    int To_line;
};
class LET_Statement: public Statement{
public:
   explicit LET_Statement(Expression * exp){expression=exp;}
    virtual ~LET_Statement()=default;
    void execute(EvalState &state);

private:
    Expression* expression;
};
class PRINT_Statement: public Statement{
public:
    explicit PRINT_Statement(Expression * exp){expression=exp;}
    virtual ~PRINT_Statement()=default;
    void execute(EvalState &state);

private:
    Expression* expression;
};
class INPUT_Statement: public Statement{
public:
    explicit INPUT_Statement(Expression * exp){expression=exp;}
    virtual ~INPUT_Statement()=default;
    void execute(EvalState &state);

private:
    Expression* expression;
};
class IF_Statement:public Statement{
public:
    explicit IF_Statement(Expression* left,string& opmid,Expression* right,Statement* GOto){
        leftex=left;rightex=right;op=opmid;GO_to=GOto;
    }
    virtual ~IF_Statement()=default;
    void execute(EvalState & state);

private:
    Expression *leftex,*rightex;
    string op;
    Statement *GO_to;
};
#endif
