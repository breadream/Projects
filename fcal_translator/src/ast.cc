#include "include/ast.h"

namespace fcal {
namespace ast {

/// Unparse the root (program)
std::string Root::unparse() {
  return varName_->unparse() + " () {\n" + stmts_->unparse() + "\n}\n";
}

/// Translate the root (program) to C++ code
std::string Root::CppCode() {
  return "#include <iostream>\n#include <stdio.h>\n"
  "#include <string>\n#include <math.h>\n"
  "#include \"include/Matrix.h\"\n\nusing namespace std ;\n\nint "
  + varName_->CppCode() + " () {\n" + stmts_->CppCode() + "\n}\n";
}

/// Root Destructor
Root::~Root() {
  if (varName_) {delete varName_;}
  if (stmts_) {delete stmts_;}
}

// StmtsSeq destructor
StmtsSeq::~StmtsSeq() {
  if (stmt_) {delete stmt_;}
  if (stmts_) {delete stmts_;}
}

/// Unparse a sequence of statements
std::string StmtsSeq::unparse() {
  return stmt_->unparse() + stmts_->unparse();
}

/// Translate a seqence of statements to C++ code
std::string StmtsSeq::CppCode() {
  return stmt_->CppCode() + stmts_->CppCode();
}

/// Unparse an empty statement (->empty string)
std::string EmptyStmts::unparse() {
  return "";
}

/// Translate an empty statement to C++ code (->empty string)
std::string EmptyStmts::CppCode() {
  return "";
}

/// Unparse a declaration
std::string DeclStmt::unparse() { return decl_->unparse(); }

/// Translate a declaration to C++ code
std::string DeclStmt::CppCode() { return decl_->CppCode(); }

/// Assignment statement destructor
AssignStmt::~AssignStmt() {
  if (varName_) { delete varName_; }
  if (expr_)    { delete expr_; }
}

/// Unparse an assignment statement
std::string AssignStmt::unparse() {
  return varName_->unparse() + " = " + expr_->unparse() + ";\n";
}

/// Translate an assignment statement to C++ code
std::string AssignStmt::CppCode() {
  return varName_->CppCode() + " = " + expr_->CppCode() + ";\n";
}

/// Matrix statement assignment destructor
AssignMatrixStmt::~AssignMatrixStmt() {
  if (varName_) {delete varName_;}
  if (expr1_) {delete expr1_;}
  if (expr2_) {delete expr2_;}
  if (expr3_) {delete expr3_;}
}

/// Unparse a matrix assignment statement
std::string AssignMatrixStmt::unparse() {
  return varName_->unparse() + " [" + expr1_->unparse() + " : " +
         expr2_->unparse() + "] = " + expr3_->unparse() + ";\n";
}

/// Unparse a matrix assignment statement to C++ code
std::string AssignMatrixStmt::CppCode() {
  // Stmt ::= varName '[' Expr ':' Expr ']' '=' Expr ';
  return "*(" + varName_->CppCode() + ".access(" + expr1_->CppCode() +
         ", " + expr2_->CppCode() + ")) = " + expr3_->CppCode() + ";\n";
}

/// Destructor for PrintStmt
PrintStmt::~PrintStmt() {
  if (expr_) {delete expr_;}
}

/// Unparse a print statement
std::string PrintStmt::unparse() {
  return "print(" + expr_->unparse() + ");\n";
}

/// Translate a print statement to C++ code
std::string PrintStmt::CppCode() {
  return "cout << " + expr_->CppCode() + ";\n";
}

/// Destructor for IfStmt
IfStmt::~IfStmt() {
  if (expr_) {delete expr_;}
  if (stmt_) {delete stmt_;}
}

/// Unparse an if statement
std::string IfStmt::unparse() {
  return "if (" + expr_->unparse() + ") " + stmt_->unparse();
}

/// Translate an if statement to C++ code
std::string IfStmt::CppCode() {
  return "if (" + expr_->CppCode() + ")\n" + stmt_->CppCode();
}

/// Destructor for an if else statement
IfElseStmt::~IfElseStmt() {
  if (expr_) {delete expr_;}
  if (stmt1_) {delete stmt1_;}
  if (stmt2_) {delete stmt2_;}
}

/// Unparse an if else statement
std::string IfElseStmt::unparse() {
  return "if (" + expr_->unparse() + ") " +
         stmt1_->unparse() + " else " + stmt2_->unparse();
}

/// Translate an if else statement to C++ code
std::string IfElseStmt::CppCode() {
  return "if (" + expr_->CppCode() + ")\n" +stmt1_->unparse() +
         "\nelse\n" + stmt2_->unparse();
}

/// Destructor for StmtsStmt
StmtsStmt::~StmtsStmt() {
  if (stmts_) {delete stmts_;}
}

/// Unparse a Statement to statement
std::string StmtsStmt::unparse() { return "{\n" + stmts_->unparse() + "}\n";}

/// Translate a statement to statement to C++ code
std::string StmtsStmt::CppCode() { return  "{\n" + stmts_->CppCode() + "}\n";}

/// Destructor for a semicolon statement
SemiColonStmt::~SemiColonStmt() {}

/// Unparse a semicolon statement
std::string SemiColonStmt::unparse() { return ";\n";}

/// Translate a semicolon to C++ code
std::string SemiColonStmt::CppCode() { return ";\n";}

/// Destructor for RepeatStmt
RepeatStmt::~RepeatStmt() {
  if (varName_) {delete varName_;}
  if (expr1_) {delete expr1_;}
  if (expr2_) {delete expr2_;}
  if (stmt_) {delete stmt_;}
}

/// Unparse a repeat statement
std::string RepeatStmt::unparse() {
    return "repeat (" + varName_->unparse() + " = " +
           expr1_->unparse() + " to " + expr2_->unparse() + ") " +
           stmt_->unparse();
}

/// repeat ( init; condition; increment ) { statement(s); }
std::string RepeatStmt::CppCode() {
  return "for (" + varName_->CppCode() + " = " +
         expr1_->CppCode() + "; " + varName_->CppCode() +
         " <= " + expr2_->CppCode() + "; " + varName_->CppCode() +
         "++) " + stmt_->CppCode() + "\n";
}

/// destructor for a while statement
WhileStmt::~WhileStmt() {
  if (expr_) {delete expr_;}
  if (stmt_) {delete stmt_;}
}

/// Unparse a while statement
std::string WhileStmt::unparse() {
  return "while (" + expr_->unparse() + ") " + stmt_->unparse();
}

/// Translate a while statement to C++ code
std::string WhileStmt::CppCode() {
  return "while (" + expr_->CppCode() + ")\n" + stmt_->CppCode();
}

/// SimpleDecl destructor
SimpleDecl::~SimpleDecl() {
  if (kwdType_) {delete kwdType_;}
  if (varName_) {delete varName_;}
}

/// Unparse a simple declaration
std::string SimpleDecl::unparse() {
  return *kwdType_ + " " + varName_->unparse() + ";\n";
}

/// Translate a simple declaration to C++ code
std::string SimpleDecl::CppCode() {
  std::string* str = nullptr;
  std::string Int("int");
  std::string Float("float");
  std::string String("string");
  std::string Bool("boolean");

  if (kwdType_->compare(Int) == 0) {
    str = new std::string("int");
  } else if (kwdType_->compare(Float) == 0) {
    str = new std::string("float");
  } else if (kwdType_->compare(String) == 0) {
    str = new std::string("string");
  } else if (kwdType_->compare(Bool) == 0) {
    str = new std::string("bool");
  } else {
    throw "no match on SimpleDecl";
  }

  return *str + " " + varName_->CppCode() + ";\n";
}

/// Destructor for LongMatrixDecl
LongMatrixDecl::~LongMatrixDecl() {
  if (var1_) {delete var1_;}
  if (expr1_) {delete expr1_;}
  if (expr2_) {delete expr2_;}
  if (var2_) {delete var2_;}
  if (var3_) {delete var3_;}
  if (expr3_) {delete expr3_;}
}

/// Unparse a long matrix declaration
std::string LongMatrixDecl::unparse() {
    return "matrix " + var1_->unparse() + " [" + expr1_->unparse() +
           ": " + expr2_->unparse() + "] " + var2_->unparse() + ": " +
           var3_->unparse() + " = " + expr3_->unparse() + ";\n";
}


// Decl ::= 'matrix' varName '[' Expr ':' Expr ']'
//          varName ':' varName '=' Expr ';'
/// Translate a long matrix declaration to C++ code
std::string LongMatrixDecl::CppCode() {
  std::string matrixDecl("matrix " + var1_->CppCode() +
  "(" + expr1_->CppCode() + ", " + expr2_->CppCode() + ");\n");

  std::string nestedFor("for (int " + var3_->CppCode() + " = 0; " +
  var3_->CppCode() + " < " + expr2_->CppCode() + "; " +
  var3_->CppCode() + "++ ) {\n*(" + var1_->CppCode() +
  ".access(" + var2_->CppCode() + ", " + var3_->CppCode() + ")) = " +
  expr3_->CppCode() + ";} ");

  std::string forStmt("for (int " + var2_->CppCode() +
  " = 0; " + var2_->CppCode() + " < " + expr1_->CppCode() +
  "; " + var2_->CppCode() + "++ ) {\n" + nestedFor + "}\n");
  return matrixDecl + forStmt;
}

/// Destructor for ShortMatrixDecl
ShortMatrixDecl::~ShortMatrixDecl() {
  if (varName_) {delete varName_;}
  if (expr_) {delete expr_;}
}

/// Unparse a short matrix declarattion
std::string ShortMatrixDecl::unparse() {
  return "matrix " + varName_->unparse() + " = " + expr_->unparse() + ";\n";
}

/// Translate a short matrix declaration to C++ code
std::string ShortMatrixDecl::CppCode() {
  return "matrix " + varName_->CppCode() + " ( " + expr_->CppCode() + " ) ;\n";
}

// Expr
/// LetExpr destructor
LetExpr::~LetExpr() {
  if (stmts_) {delete stmts_;}
  if (expr_) {delete expr_;}
}

/// Unparse a let expression
std::string LetExpr::unparse() {
  return "let " + stmts_->unparse() + " in " + expr_->unparse() + " end";
}

/// Translate a let expression to C++ code
std::string LetExpr::CppCode() {
  return "({ " + stmts_->CppCode() + "(" + expr_->CppCode() + "); });\n";
}

/// Destructor for BinOpExpr
BinOpExpr::~BinOpExpr() {
  if (left_) {delete left_;}
  if (operator_) {delete operator_;}
  if (right_) {delete right_;}
}

/// Unparse a binary operation expression
std::string BinOpExpr::unparse() {
  return left_->unparse() + " " + *operator_ + " " + right_->unparse();
}

/// Translate a binary operation expression to C++ code
std::string BinOpExpr::CppCode() {
  return left_->CppCode() + " " + *operator_ + " " + right_->CppCode();
}

/// Destructor for a function expression
FunctionExpr::~FunctionExpr() {
  if (varName_) {delete varName_;}
  if (expr_) {delete expr_;}
}

/// Unparse a function expression
std::string FunctionExpr::unparse() {
  return varName_->unparse() + "(" + expr_->unparse() + ")";
}

/// Translate  function expression to C++ code
std::string FunctionExpr::CppCode() {
  std::string var_local(varName_->CppCode());
  std::string expr_local(expr_->CppCode());
  if (var_local.compare("matrix_read") == 0
  && expr_local.compare("data") != 0) {
    var_local = "matrix::matrix_read";
    return var_local + "(" + expr_->CppCode() + ")";
  } else if (expr_local.compare("data") == 0) {
    return expr_local + "." + var_local + "()";
  } else {
    return var_local + "(" + expr_->CppCode() + ")";
  }
}

/// Destructor for MatrixExpr
MatrixExpr::~MatrixExpr() {
  if (varName_) {delete varName_;}
  if (expr1_) {delete expr1_;}
  if (expr2_) {delete expr2_;}
}

/// Unparse a matrix expression
std::string MatrixExpr::unparse() {
  return varName_->unparse() + " [" + expr1_->unparse() + ": " +
         expr2_->unparse() + "]";
}

/// Translate a matrix expression to C++ code
std::string MatrixExpr::CppCode() {
  // Expr ::= varName '[' Expr ':' Expr ']'
  return "*(" + varName_->CppCode() + ".access(" + expr1_->CppCode() + ", " +
         expr2_->CppCode() + "))";
}

/// Destructor for IfExpr
IfExpr::~IfExpr() {
  if (expr1_) {delete expr1_;}
  if (expr2_) {delete expr2_;}
  if (expr3_) {delete expr3_;}
}

/// Unparse and if expression
std::string IfExpr::unparse() {
  return "if " + expr1_->unparse() + " then " + expr2_->unparse() + " else "
         + expr3_->unparse();
}

/// Translate an if expression to C++ code
std::string IfExpr::CppCode() {
  return expr1_->CppCode() + " ? " + expr2_->CppCode() + " : " +
         expr3_->CppCode();
}

/// Destructor for ParenExpr
ParenExpr::~ParenExpr() {
  if (expr_) {delete expr_;}
}

/// Unparse a ParenExpr
std::string ParenExpr::unparse() { return "(" + expr_->unparse() + ")";}

/// Translate a ParenExpr to C++ code
std::string ParenExpr::CppCode() { return "(" + expr_->CppCode() + ")";}

/// Unparse a VarName (return the lexeme)
std::string VarName::unparse() { return lexeme_; }

/// Translate a VarName to C++ code (return the lexeme)
std::string VarName::CppCode() { return lexeme_; }

/// Destructor for AnyConst
AnyConst::~AnyConst() {
  if (constStr_) {delete constStr_;}
}

/// Unparse AnyConst (return the constant)
std::string AnyConst::unparse() { return *constStr_; }

/// Translate AnyConst to C++ code (return the constant)
std::string AnyConst::CppCode() { return *constStr_;}

/// Destructor for a not expression
NotExpr::~NotExpr() {
  if (expr_) {delete expr_;}
}

/// Unparse a not expression
std::string NotExpr::unparse () { return "!" + expr_->unparse(); }

/// Translate a not expression to C++ code
std::string NotExpr::CppCode() { return "!" + expr_->CppCode(); }

/// Destructor for TrueKwdExpr
TrueKwdExpr::~TrueKwdExpr() {}

/// Unparse a TrueKwdExpre (return "True")
std::string TrueKwdExpr::unparse() { return "True";}

/// Translate a TrueKwdExpr to C++ code (return "true")
std::string TrueKwdExpr::CppCode() { return "true";}

/// Destructor for FalseKwdExpr
FalseKwdExpr::~FalseKwdExpr() {}

/// Unparse a FalseKwdExpre (return "False")
std::string FalseKwdExpr::unparse() { return "False";}

/// Translate a FalseKwdExpr to C++ code (return "false")
std::string FalseKwdExpr::CppCode() { return "false";}

} /* namespace ast */
} /* namespace fcal */
