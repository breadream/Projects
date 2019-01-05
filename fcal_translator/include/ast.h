#ifndef PROJECT_INCLUDE_AST_H_
#define PROJECT_INCLUDE_AST_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include <string>
#include "include/scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace ast {

class Node;

class Root;
class Stmts;
class Stmt;
class Expr;
class Decl;
class VarName;


/// Template that uses dynamic cast to check the node type
template <class T, class E>
T* CastCheck(T* type, E* obj, std::string location) {
    if (obj) {
        type = dynamic_cast <T*>(obj);
        if (!type) { throw (std::string("Cast error in " + location));}
    }
    return type;
}

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/*! 
 * The Node class that acts as the 
 * parent class for all non-terminals
 */
class Node {
 public:
  virtual std::string unparse(void) = 0;
  virtual std::string CppCode(void) = 0;
  virtual ~Node(void) {}
};

/*!
 * Program (Root) 
 * production: Program ::= varName '(' ')' '{' Stmts '}'
 * The first node instantiated for an FCAL program AST
 */
class Root : public Node {
 public:
  Root(VarName *varName, Stmts *stmts) :
           varName_(varName), stmts_(stmts) {}
  std::string unparse();
  std::string CppCode();
  ~Root();
 private:
  VarName *varName_;
  Stmts *stmts_;
};

/*!
 * Stmts abstract class
 * Interface for Stmts non-terminal
 * Parent for productions of the form:
 * Stmts ::= ...
 */
class Stmts : public Node {
 public:
  virtual std::string unparse(void) = 0;
  virtual std::string CppCode(void) = 0;
  virtual ~Stmts() {}
};

/*!
 * Concrete class for empty statements
 * production : Stmts ::= <<empty>>
 */
class EmptyStmts : public Stmts {
 public:
  EmptyStmts() {}
  std::string unparse();
  std::string CppCode();
  ~EmptyStmts() {}
};

/*!
 * Concrete class for a statment sequence
 * production : Stmts ::= Stmt Stmts
 */
class StmtsSeq : public Stmts {
 public:
  StmtsSeq(Stmt *stmt, Stmts *stmts) : stmt_(stmt), stmts_(stmts) {}
  std::string unparse();
  std::string CppCode();
  ~StmtsSeq();

 private:
  StmtsSeq() : stmt_(NULL), stmts_(NULL) {}
  StmtsSeq(const StmtsSeq &) {}
  Stmt *stmt_;
  Stmts *stmts_;
};

/*!
 * Stmt abstract class
 * Interface for Stmt non-terminal
 * Parent for productions of the form
 * Stmt ::= ...
 */
class Stmt : public Node {
 public:
  virtual std::string unparse(void) = 0;
  virtual std::string CppCode(void) = 0;
  virtual ~Stmt() {}
};

/*!
 * DeclStmt concrete class
 * production : Stmt ::= Decl
 */ 
class DeclStmt : public Stmt {
 public:
  explicit DeclStmt(Decl *decl) : decl_(decl) {}
  std::string unparse();
  std::string CppCode();
  ~DeclStmt() {}

 private:
  Decl* decl_;
};

/*!
 * Assignment statement concrete class 
 * production: Stmt ::= VarName '=' Expr ';'
 */
class AssignStmt : public Stmt {
 public:
  AssignStmt(VarName *var, Expr *expr) : varName_(var), expr_(expr) {}
  std::string unparse();
  std::string CppCode();
  ~AssignStmt();
 private:
  VarName *varName_;
  Expr *expr_;
};

/*!
 * Matrix assignment statement concrete class
 * production:  VarName '[' Expr ':' Expr ']' '=' Expr ';'
 */
class AssignMatrixStmt : public Stmt {
 public:
  AssignMatrixStmt(VarName *var, Expr *expr1, Expr *expr2, Expr *expr3) :
                   varName_(var), expr1_(expr1), expr2_(expr2), expr3_(expr3) {}
  std::string unparse();
  std::string CppCode();
  ~AssignMatrixStmt();
 private:
  VarName *varName_;
  Expr *expr1_;
  Expr *expr2_;
  Expr *expr3_;
};

/*!
 * Print statement concrete class
 * production:  Stmt ::= 'print' '(' Expr ')' ';'
 */
class PrintStmt : public Stmt {
 public:
  explicit PrintStmt(Expr *expr) : expr_(expr) {}
  std::string CppCode();
  std::string unparse();
  ~PrintStmt();
 private:
  Expr *expr_;
};



/*!
 * if statement concrete class
 * production: Stmt ::= 'if' '(' Expr ')' Stmt
 */
class IfStmt : public Stmt {
 public:
  IfStmt(Expr *expr, Stmt *stmt) : expr_(expr), stmt_(stmt) {}
  std::string CppCode();
  std::string unparse();
  ~IfStmt();
 private:
  Expr *expr_;
  Stmt *stmt_;
};

/*!
 * If else statement concrete class 
 * production: Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
 */
class IfElseStmt : public Stmt {
 public:
  IfElseStmt(Expr *expr, Stmt *stmt1, Stmt *stmt2) : expr_(expr),
  stmt1_(stmt1), stmt2_(stmt2) {}
  std::string unparse();
  std::string CppCode();
  ~IfElseStmt();
 private:
  Expr *expr_;
  Stmt *stmt1_;
  Stmt *stmt2_;
};

/*!
 * Stmt to Stmts concrete class
 * production: Stmt ::= '{' Stmts '}'
 */
class StmtsStmt : public Stmt {
 public:
  explicit StmtsStmt(Stmts *stmts) : stmts_(stmts) {}
  ~StmtsStmt();
  std::string unparse();
  std::string CppCode();
 private:
  Stmts *stmts_;
};


/*!
 * Semi Colon stmt concrete class
 * production: Stmt ::= ';' 
 */
class SemiColonStmt : public Stmt {
 public:
  std::string unparse();
  std::string CppCode();
  ~SemiColonStmt();
};

/*!
 * Repeat statement concrete class
 * production: Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt  
 */
class RepeatStmt : public Stmt {
 public:
  RepeatStmt(VarName *varName, Expr *expr1, Expr *expr2, Stmt *stmt)
  : varName_(varName), expr1_(expr1), expr2_(expr2), stmt_(stmt) {}
  std::string unparse();
  std::string CppCode();
  ~RepeatStmt();
 private:
  VarName *varName_;
  Expr *expr1_;
  Expr *expr2_;
  Stmt *stmt_;
};

/*!
 * While statment concrete class
 * production: Stmt ::= 'while' '(' Expr ')' Stmt
 */
class WhileStmt : public Stmt {
 public:
  WhileStmt(Expr *expr, Stmt *stmt) : expr_(expr), stmt_(stmt) {}
  std::string unparse();
  std::string CppCode();
  ~WhileStmt();
 private:
  Expr *expr_;
  Stmt *stmt_;
};

/*!
 * Abstract class for declaration non-terminal
 * productions of the form Decl ::= ...
 */
class Decl : public Node {
 public:
  virtual std::string unparse(void) = 0;
  virtual std::string CppCode(void) = 0;
  virtual ~Decl() {}
};

/*!
 * Simple declaration concrete class
 * prodution: Decl ::= kwdType varName ';'
 * Where kwdType is part of the set
 * {'int', 'float', 'string', 'boolean'}
 */
class SimpleDecl : public Decl {
 public:
  SimpleDecl(std::string *kwdType, VarName *varName) :
           kwdType_(kwdType), varName_(varName) {}
  std::string unparse();
  std::string CppCode();
  ~SimpleDecl();

 private:
  std::string *kwdType_;
  VarName *varName_;
};

/*!
 * Long Matrix declaration concrete class
 * production: Decl ::= 'Matrix' VarName '[' Expr ':' Expr ']' VarName ':'
 * VarName '=' Expr ';'
 */
class LongMatrixDecl : public Decl {
 public:
  LongMatrixDecl(VarName *var1, Expr *expr1, Expr *expr2,
  VarName *var2, VarName *var3, Expr *expr3) : var1_(var1), expr1_(expr1),
  expr2_(expr2), var2_(var2), var3_(var3), expr3_(expr3) {}
  std::string unparse();
  std::string CppCode();
  ~LongMatrixDecl();
 private:
  VarName *var1_;
  Expr *expr1_;
  Expr *expr2_;
  VarName *var2_;
  VarName *var3_;
  Expr *expr3_;
};

/*!
 * Short matrix declaration concrete class
 * production: Decl ::= 'Matrix' varName '=' Expr ';'
 */
class ShortMatrixDecl : public Decl {
 public:
  ShortMatrixDecl(VarName *var, Expr *expr) : varName_(var), expr_(expr) {}
  std::string unparse();
  std::string CppCode();
  ~ShortMatrixDecl();
 private:
  VarName *varName_;
  Expr *expr_;
};

/*!
 * Abstract class for expression non-terminal
 * productions of the form Expr ::= ...
 */
class Expr : public Node {
 public:
  virtual std::string unparse() = 0;
  virtual std::string CppCode() = 0;
  virtual ~Expr() {}
};

/*!
 * Let Expression concrete class
 * production: Expr ::= 'let' Stmts 'in' Expr 'end'
 */
class LetExpr : public Expr {
 public:
  LetExpr(Stmts *stmts, Expr *expr) : stmts_(stmts), expr_(expr) {}
  std::string unparse();
  std::string CppCode();
  ~LetExpr();
 private:
  Stmts* stmts_;
  Expr* expr_;
};


/*!
 * Binary expression concrete class
 * production: Expr ::= Expr operator Expr
 * Where operator is part of the set
 * {'*', '/', '+', '-', '>', '>=', '<',
 *  '<=', '==', '!=', '&&', '||'}
 */
class BinOpExpr : public Expr {
 public:
  BinOpExpr(Expr *left, std::string *op, Expr *right) : left_(left),
  operator_(op), right_(right) {}
  std::string unparse();
  std::string CppCode();
  ~BinOpExpr();
 private:
  Expr * left_;
  std::string *operator_;
  Expr * right_;
};

/*!
 * Function Expression concrete class
 * production: Expr ::= varName '(' Expr ')'
 */
class FunctionExpr : public Expr {
 public:
  FunctionExpr(VarName *var, Expr *expr) : varName_(var), expr_(expr) {}
  std::string unparse();
  std::string CppCode();
  ~FunctionExpr();
 private:
  VarName *varName_;
  Expr *expr_;
};

/*!
 * Matrix Expression concrete class
 * production: Expr ::= varName '[' Expr ':' Expr ']'
 */
class MatrixExpr : public Expr {
 public:
  MatrixExpr(VarName *var, Expr *expr1, Expr *expr2) :
             varName_(var), expr1_(expr1), expr2_(expr2) {}
  std::string unparse();
  std::string CppCode();
  ~MatrixExpr();
 private:
  VarName *varName_;
  Expr *expr1_;
  Expr *expr2_;
};

/*!
 * If expression concrete class
 * production: Expr ::= 'if' Expr 'then' Expr 'else' Expr
 */
class IfExpr : public Expr {
 public:
  IfExpr(Expr *expr1, Expr *expr2, Expr *expr3) : expr1_(expr1),
  expr2_(expr2), expr3_(expr3) {}
  std::string unparse();
  std::string CppCode();
  ~IfExpr();
 private:
  Expr *expr1_;
  Expr *expr2_;
  Expr *expr3_;
};

/*!
 * Parentheses expression concrete class
 * production: Expr ::= '(' Expr ')'
 */
class ParenExpr : public Expr {
 public:
  explicit ParenExpr(Expr *expr) : expr_(expr) {}
  std::string unparse();
  std::string CppCode();
  ~ParenExpr();
 private:
  Expr *expr_;
};

/*!
 * Class used for variable names
 */
class VarName : public Expr {
 public:
  explicit VarName(std::string lexeme) : lexeme_(lexeme) {}
  std::string unparse();
  std::string CppCode();
  ~VarName() {}
 private:
  VarName() : lexeme_((std::string) "") {}
  VarName(const VarName &) {}
  std::string lexeme_;
};

/*!
 * Constants concrete class
 * production: Expr ::= integerConst | floatConst |  stringConst
 * @param _s is the string in which the const is passed in
 */
class AnyConst : public Expr {
 public:
  explicit AnyConst(std::string *s) : constStr_(s) {}
  std::string unparse();
  std::string CppCode();
  ~AnyConst();
 private:
  std::string *constStr_;
};


/*!
 * Not expression concrete class
 * production: Expr ::= '!' Expr
 */
class NotExpr : public Expr {
 public:
  explicit NotExpr(Expr *expr) : expr_(expr) {}
  std::string unparse();
  std::string CppCode();
  ~NotExpr();
 private:
  Expr *expr_;
};

/*!
 * True keyword concrete class
 * production: Expr ::= trueKwd
 */
class TrueKwdExpr : public Expr {
 public:
  std::string unparse();
  std::string CppCode();
  ~TrueKwdExpr();
};

/*!
 * False keyword concrete class
 * production: Expr ::= falseKwd
 */
class FalseKwdExpr : public Expr {
 public:
  std::string unparse();
  std::string CppCode();
  ~FalseKwdExpr();
};

} /* namespace ast */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_AST_H_
