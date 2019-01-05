/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "include/parser.h"
#include <assert.h>
#include <stdio.h>
#include "include/ext_token.h"
#include "include/scanner.h"
#include "include/ast.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace parser {

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Parser::~Parser() {
  if (scanner_) delete scanner_;

  scanner::ExtToken *to_delete1;
  curr_token_ = tokens_;
  while (curr_token_) {
    to_delete1 = curr_token_;
    curr_token_ = curr_token_->next();
    delete to_delete1;
  } /* while() */

  scanner::Token *curr_scanner_token = stokens_;
  scanner::Token *to_delete2;
  while (curr_scanner_token) {
    to_delete2 = curr_scanner_token;
    curr_scanner_token = curr_scanner_token->next();
    delete to_delete2;
  } /* while() */
} /* Parser::~Parser() */

/*!
 * Parser::Parse(const char *text) - 
 * Parse the program checking for syntactic correctness
 * and generate the AST
 */
ParseResult Parser::Parse(const char *text) {
  assert(text != nullptr);

  ParseResult pr;
  try {
  scanner_ = new scanner::Scanner();
  stokens_ = scanner_->Scan(text);
  tokens_ = tokens_->ExtendTokenList(this, stokens_);

  assert(tokens_ != nullptr);
  curr_token_ = tokens_;
  pr = ParseProgram();
  }
  catch (std::string errMsg) {
  pr.ok(false);
  pr.errors(errMsg);
  pr.ast(nullptr);
  }
  return pr;
} /* Parser::parse() */

/*!
 * parse method for program.
 * The parser works by instantiating instances
 * of concrete classes as defined by ast.h
 * when the appropriate production is 
 * identified.
 */

// Program
ParseResult Parser::ParseProgram() {
  ParseResult pr;
  // root
  // Program ::= varName '(' ')' '{' Stmts '}'
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  ast::VarName *v = new ast::VarName(name);

  match(scanner::kLeftParen);
  match(scanner::kRightParen);
  match(scanner::kLeftCurly);

  ParseResult pr_stmts = parse_stmts();
  ast::Stmts * stmts = nullptr;
  stmts = ast::CastCheck<>(stmts, pr_stmts.ast(), "ParseProgram");

  match(scanner::kRightCurly);
  match(scanner::kEndOfFile);

  pr.ast(new ast::Root(v, stmts));
  return pr;
} /* Parser::ParseProgram() */

// MatrixDecl
// identical purpose of parse_decl, handles special matrix syntax.
ParseResult Parser::parse_matrix_decl() {
  ParseResult pr;
  ParseResult pr_expr;
  ast::Expr *expr = nullptr;
  match(scanner::kMatrixKwd);
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  ast::VarName *var = new ast::VarName(name);

  // Decl ::= 'matrix' varName '[' Expr ':' Expr ']' varName ':' varName  '='
  // Expr ';'
  if (attempt_match(scanner::kLeftSquare)) {
    ast::Expr * expr2 = nullptr;
    ast::Expr * expr3 = nullptr;
    ParseResult pr_expr2;
    ParseResult pr_expr3;
    ParseResult pr_var2;
    ParseResult pr_var3;

    pr_expr = parse_expr(0);
    expr = ast::CastCheck<> (expr, pr_expr.ast(), "parse_matrix_decl Expr1");
      match(scanner::kColon);

    pr_expr2 = parse_expr(0);
    expr2 = ast::CastCheck<> (expr2, pr_expr2.ast(), "parse_matrix_decl Expr2");
      match(scanner::kRightSquare);

    pr_var2 = parse_variable_name();
    ast::VarName * var2 = new ast::VarName(prev_token_->lexeme());
      match(scanner::kColon);

    pr_var3 = parse_variable_name();
    ast::VarName * var3 = new ast::VarName(prev_token_->lexeme());
      match(scanner::kAssign);

    pr_expr3 = parse_expr(0);
    expr3 = ast::CastCheck<> (expr3, pr_expr3.ast(), "parse_matrix_decl Expr3");
    pr.ast(new ast::LongMatrixDecl(var, expr, expr2, var2, var3, expr3));

  } else if (attempt_match(scanner::kAssign)) {
    // Decl ::= 'matrix' varName '=' Expr ';'
    pr_expr = parse_expr(0);
    expr = ast::CastCheck<>(expr, pr_expr.ast(), "parse_matrix_decl Expr");
    pr.ast(new ast::ShortMatrixDecl(var, expr));
  } else {
    throw((std::string) "Bad Syntax Matrix Decl in in parse_matrix_decl");
  }

  match(scanner::kSemiColon);

  return pr;
}
// standardDecl
// Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
ParseResult Parser::parse_standard_decl() {
  ParseResult pr;
  std::string * kwdType = nullptr;

  if (attempt_match(scanner::kIntKwd)) {  // Type ::= intKwd
    kwdType = new std::string (prev_token_->lexeme());
  } else if (attempt_match(scanner::kFloatKwd)) {  // Type ::= floatKwd
    kwdType = new std::string (prev_token_->lexeme());
  } else if (attempt_match(scanner::kStringKwd)) {  // Type ::= stringKwd
    kwdType = new std::string (prev_token_->lexeme());
  } else if (attempt_match(scanner::kBoolKwd)) {  // Type ::= boolKwd
    kwdType = new std::string (prev_token_->lexeme());
  }

  match(scanner::kVariableName);
  ast::VarName *v = new ast::VarName(prev_token_->lexeme());
  match(scanner::kSemiColon);

  pr.ast(new ast::SimpleDecl(kwdType, v));
  return pr;
}

// Decl
ParseResult Parser::parse_decl() {
  ParseResult pr;
  ParseResult pr_simple;
  ast::Decl * d = nullptr;
  // Decl :: matrix variableName ....
  if (next_is(scanner::kMatrixKwd)) {
    pr = parse_matrix_decl();
  } else {  // Decl ::= Type variableName semiColon
    pr_simple = parse_standard_decl();
    d = ast::CastCheck<> (d, pr_simple.ast(), "parse_decl");
    pr.ast(d);
  }
  return pr;
}

// Stmts
ParseResult Parser::parse_stmts() {
  ParseResult pr;
  if (!next_is(scanner::kRightCurly) && !next_is(scanner::kInKwd)) {
    // Stmts ::= Stmt Stmts
    ParseResult pr_stmt = parse_stmt();
    ast::Stmt *stmt = nullptr;
    stmt = ast::CastCheck<>(stmt, pr_stmt.ast(), "parse_stmts ; 'stmt'");

    ParseResult pr_stmts = parse_stmts();
    ast::Stmts *stmts = nullptr;
    stmts = ast::CastCheck<>(stmts, pr_stmts.ast(), "parse_stmts ; 'stmts'");
    // ast::StmtsSeq *seq = new ast::StmtsSeq(dynamic_cast<ast::Stmt*>
    // (pr_stmt.ast()), dynamic_cast<ast::Stmts*>(pr_stmts.ast()));

    pr.ast(new ast::StmtsSeq(stmt, stmts));
  } else {
    // Stmts ::=
    // nothing to match.k
    // ast::EmptyStmts *eseq = new ast::EmptyStmts();
    pr.ast(new ast::EmptyStmts());
  }
  return pr;
}

// Stmt
ParseResult Parser::parse_stmt() {
  ParseResult pr;
  ParseResult pr_tmp1;
  ParseResult pr_tmp2;
  ParseResult pr_tmp3;
  ast::VarName *var = nullptr;
  ast::Expr * exp1 = nullptr;
  ast::Expr * exp2 = nullptr;
  ast::Expr * exp3 = nullptr;

  // Stmt ::= Decl
  if (next_is(scanner::kIntKwd) || next_is(scanner::kFloatKwd) ||
      next_is(scanner::kMatrixKwd) || next_is(scanner::kStringKwd) ||
      next_is(scanner::kBoolKwd)) {
    ParseResult ds = parse_decl();
    ast::Decl *decl = nullptr;
    decl = ast::CastCheck<>(decl, ds.ast(), "parse_stmt ; 'decl'");

    ast::Stmt * stmt = dynamic_cast<ast::Stmt*> (new ast::DeclStmt(decl));
    if (!stmt) {std::cout << "ALERT\n";}
    pr.ast(stmt);
    // ast::DeclStmt *decls =
    // new ast::DeclStmt(dynamic_cast<ast::Decl*>(ds.ast()));


  } else if (attempt_match(scanner::kLeftCurly)) {
    // Stmt ::= '{' Stmts '}'
    ast::Stmts * stmts = nullptr;
    pr_tmp1 = parse_stmts();
    stmts = ast::CastCheck<>(stmts, pr_tmp1.ast(), "parse_stmt ; 'StmtsStmt'");
    match(scanner::kRightCurly);
    pr.ast(new ast::StmtsStmt(stmts));

  } else if (attempt_match(scanner::kIfKwd)) {
    // Stmt ::= 'if' '(' Expr ')' Stmt
    // Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    ast::Stmt * stmt = nullptr;
    match(scanner::kLeftParen);

    pr_tmp1 = parse_expr(0);
    exp1 = ast::CastCheck<>(exp1, pr_tmp1.ast(), "parse_stmt IfStmt Expr1");
    match(scanner::kRightParen);

    pr_tmp2 = parse_stmt();
    stmt = ast::CastCheck<>(stmt, pr_tmp2.ast(), "parse_stmt IfStmt stmt");
    pr.ast(new ast::IfStmt(exp1, stmt));

    if (attempt_match(scanner::kElseKwd)) {
      ast::Stmt * stmt2 = nullptr;
      pr_tmp3 = parse_stmt();
      stmt2 = ast::CastCheck<>(stmt2, pr_tmp3.ast(), "parse_stmt IfStmt stmt2");
      pr.ast(new ast::IfElseStmt(exp1, stmt, stmt2));
    }
  } else if (attempt_match(scanner::kVariableName)) {
      var = new ast::VarName(prev_token_->lexeme());
    /*
     * Stmt ::= varName '=' Expr ';'  | varName '[' Expr ':' Expr ']'
     * '=' Expr ';'
     */
    if (attempt_match(scanner::kLeftSquare)) {
      pr_tmp1 = parse_expr(0);
      exp1 = ast::CastCheck<>(exp1, pr_tmp1.ast(), "MatrixAssign expr1");

      match(scanner::kColon);

      pr_tmp2 = parse_expr(0);
      exp2 = ast::CastCheck<>(exp2, pr_tmp2.ast(), "MatrixAssign expr2");

      match(scanner::kRightSquare);

      match(scanner::kAssign);

      pr_tmp3 = parse_expr(0);
      exp3 = ast::CastCheck<>(exp3, pr_tmp3.ast(), "MatrixAssign expr3");
      match(scanner::kSemiColon);
      pr.ast(new ast::AssignMatrixStmt(var, exp1, exp2, exp3));

    } else if (attempt_match(scanner::kAssign)) {
        pr_tmp1 = parse_expr(0);
        exp1 = ast::CastCheck(exp1, pr_tmp1.ast(), "Assign");
        match(scanner::kSemiColon);
        pr.ast(new ast::AssignStmt(var, exp1));
      }

  } else if (attempt_match(scanner::kPrintKwd)) {
    // Stmt ::= 'print' '(' Expr ')' ';'
    match(scanner::kLeftParen);
    ast::Expr * expr = nullptr;
    pr_tmp1 = parse_expr(0);
    expr = ast::CastCheck<>(expr, pr_tmp1.ast(), "Print");

    match(scanner::kRightParen);
    match(scanner::kSemiColon);

    pr.ast(new ast::PrintStmt(expr));

  } else if (attempt_match(scanner::kRepeatKwd)) {
    // Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
    ast::Stmt * stmt = nullptr;
    ast::Expr * expr1 = nullptr;
    ast::Expr * expr2 = nullptr;

    match(scanner::kLeftParen);
    match(scanner::kVariableName);
    var = new ast::VarName(prev_token_->lexeme());
    match(scanner::kAssign);

    pr_tmp1 = parse_expr(0);
    expr1 = ast::CastCheck<>(expr1, pr_tmp1.ast(), "parse RepeatStmt expr1");
    match(scanner::kToKwd);

    pr_tmp2 = parse_expr(0);
    expr2 = ast::CastCheck<>(expr2, pr_tmp2.ast(), "parse RepeatStmt expr2");
    match(scanner::kRightParen);

    pr_tmp3 = parse_stmt();
    stmt = ast::CastCheck<>(stmt, pr_tmp3.ast(), "parse Repeatstmt stmt");
    pr.ast(new ast::RepeatStmt(var, expr1, expr2, stmt));

  } else if (attempt_match(scanner::kWhileKwd)) {
    // Stmt ::= 'while' '(' Expr ')' Stmt
    ast::Stmt * stmt = nullptr;
    ast::Expr * expr = nullptr;
    match(scanner::kLeftParen);

    pr_tmp1 = parse_expr(0);
    expr = ast::CastCheck<>(expr, pr_tmp1.ast(), "parse WhileStmt expr");
    match(scanner::kRightParen);

    pr_tmp2 = parse_stmt();
    stmt = ast::CastCheck<>(stmt, pr_tmp2.ast(), "parse WhileStmt stmt");

    pr.ast(new ast::WhileStmt(expr, stmt));

  } else if (attempt_match(scanner::kSemiColon)) {
    // Stmt ::= ';
    pr.ast(new ast::SemiColonStmt());
    // parsed a skip
  } else {
    throw(make_error_msg(curr_token_->terminal()) +
          " while parsing a statement");
  }
  // Stmt ::= variableName assign Expr semiColon
  return pr;
}

// Expr
/*!
 Examine current token, without consuming it, to call its
 associated parse methods.  The ExtToken objects have 'nud' and
 'led' methods that are dispatchers that call the appropriate
 parse methods.
*/
ParseResult Parser::parse_expr(int rbp) {
  ParseResult left = curr_token_->nud();

  while (rbp < curr_token_->lbp()) {
    left = curr_token_->led(left);
  }

  return left;
}

/*
 * parse methods for Expr productions
 * ----------------------------------
 */

// Expr ::= trueKwd
ParseResult Parser::parse_true_kwd() {
  ParseResult pr;
  match(scanner::kTrueKwd);
  pr.ast(new ast::TrueKwdExpr());
  return pr;
}

// Expr ::= trueKwd
ParseResult Parser::parse_false_kwd() {
  ParseResult pr;
  match(scanner::kFalseKwd);
  pr.ast(new ast::FalseKwdExpr());
  return pr;
}

// Expr ::= intConst
ParseResult Parser::parse_int_const() {
  ParseResult pr;
  match(scanner::kIntConst);
  pr.ast(new ast::AnyConst(new std::string(prev_token_->lexeme())));
  return pr;
}

// Expr ::= floatConst
ParseResult Parser::parse_float_const() {
  ParseResult pr;
  match(scanner::kFloatConst);
  pr.ast(new ast::AnyConst(new std::string(prev_token_->lexeme())));
  return pr;
}

// Expr ::= stringConst
ParseResult Parser::parse_string_const() {
  ParseResult pr;
  match(scanner::kStringConst);
  pr.ast(new ast::AnyConst(new std::string(prev_token_->lexeme())));
  return pr;
}

// Expr ::= variableName .....
ParseResult Parser::parse_variable_name() {
  ParseResult pr;
  match(scanner::kVariableName);
  ast::VarName * var = new ast::VarName(prev_token_->lexeme());
  if (attempt_match(scanner::kLeftSquare)) {
    ParseResult pr_expr1 = parse_expr(0);
    ast::Expr *expr1 = nullptr;
    expr1 = ast::CastCheck<>(expr1, pr_expr1.ast(),
                             "parse_variable_name ; expr1");
    match(scanner::kColon);

    ParseResult pr_expr2 = parse_expr(0);
    ast::Expr *expr2 = nullptr;
    expr2 = ast::CastCheck<>(expr2, pr_expr2.ast(),
                             "parse_variable_name ; epxr2");
    match(scanner::kRightSquare);

    pr.ast(new ast::MatrixExpr(var, expr1, expr2));
  }  else if (attempt_match(scanner::kLeftParen)) {
    // Expr ::= varableName '(' Expr ')'
    ParseResult pr_expr = parse_expr(0);
    ast::Expr * expr = nullptr;
    expr = ast::CastCheck<>(expr, pr_expr.ast(), "parse_variable_name ; expr");
    match(scanner::kRightParen);
    pr.ast(new ast::FunctionExpr(var, expr));
  } else {
    // variable
    pr.ast(var);
  }
  return pr;
}

// Expr ::= leftParen Expr rightParen
ParseResult Parser::parse_nested_expr() {
  ParseResult pr;
  match(scanner::kLeftParen);
  ast::Expr * expr = nullptr;
  ParseResult pr_expr = parse_expr(0);
  expr = ast::CastCheck<>(expr, pr_expr.ast(), "parse_nested_expr");
  match(scanner::kRightParen);
  pr.ast(new ast::ParenExpr(expr));
  return pr;
}

// Expr ::= 'if' Expr 'then' Expr 'else' Expr
ParseResult Parser::parse_if_expr() {
  ParseResult pr;

  match(scanner::kIfKwd);

  ParseResult pr_expr1 = parse_expr(0);
  ast::Expr * expr1 = nullptr;
  expr1 = ast::CastCheck<>(expr1, pr_expr1.ast(), "parse_if_expr ; expr1");
  match(scanner::kThenKwd);

  ParseResult pr_expr2 = parse_expr(0);
  ast::Expr * expr2 = nullptr;
  expr2 = ast::CastCheck<>(expr2, pr_expr2.ast(), "parse_if_expr ; expr2");
  match(scanner::kElseKwd);


  ParseResult pr_expr3 = parse_expr(0);
  ast::Expr * expr3 = nullptr;
  expr3 = ast::CastCheck<>(expr3, pr_expr3.ast(), "parse_if_expr ; expr3");

  pr.ast(new ast::IfExpr(expr1, expr2, expr3));

  return pr;
}

// Expr ::= 'let' Stmts 'in' Expr 'end'
ParseResult Parser::parse_let_expr() {
  ParseResult pr;
  match(scanner::kLetKwd);

  ParseResult pr_stmts = parse_stmts();
  ast::Stmts * stmts = nullptr;
  stmts = ast::CastCheck<>(stmts, pr_stmts.ast(), "parse_let_expr ; stmts");
  match(scanner::kInKwd);

  ParseResult pr_expr = parse_expr(0);
  ast::Expr * expr = nullptr;
  expr = ast::CastCheck<>(expr, pr_expr.ast(), "parse_let_expr ; expr");
  match(scanner::kEndKwd);

  pr.ast(new ast::LetExpr(stmts, expr));

  return pr;
}

// Expr ::= '!' Expr
ParseResult Parser::parse_not_expr() {
  ParseResult pr;
  match(scanner::kNotOp);

  ParseResult pr_expr = parse_expr(0);
  ast::Expr * expr = nullptr;
  expr = ast::CastCheck<>(expr, pr_expr.ast(), "parse_not_expr");

  pr.ast(new ast::NotExpr(expr));

  return pr;
}

// Expr ::= Expr plusSign Expr
ParseResult Parser::parse_addition(ParseResult prLeft) {
  // parser has already matched left expression
  ParseResult pr;
  ast::Expr *left = nullptr;
  left = ast::CastCheck<>(left, prLeft.ast(), "parse_addition ; left");
  match(scanner::kPlusSign);

  std::string * op = new std::string(prev_token_->lexeme());

  ParseResult prRight = parse_expr(0);
  // parse_expr(prev_token_->lbp());
  ast::Expr *right = nullptr;
  right = ast::CastCheck<>(right, prRight.ast(), "parse_addition ; right");

  pr.ast(new ast::BinOpExpr(left, op, right));
  return pr;
}

// Expr ::= Expr star Expr
ParseResult Parser::parse_multiplication(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;

  ast::Expr * left = nullptr;
  left = ast::CastCheck<>(left, prLeft.ast(), "parse_multiplication ; left");
  match(scanner::kStar);

  std::string * op = new std::string(prev_token_->lexeme());

  ParseResult prRight = parse_expr(0);
  // parse_expr(prev_token_->lbp());
  ast::Expr * right = nullptr;
  right = ast::CastCheck<>(right, prRight.ast(),
                           "parse_multiplication ; right");
  pr.ast(new ast::BinOpExpr(left, op, right));

  return pr;
}

// Expr ::= Expr dash Expr
ParseResult Parser::parse_subtraction(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;

  ast::Expr * left = nullptr;
  left = ast::CastCheck<>(left, prLeft.ast(), "parse_subtraction ; left");
  match(scanner::kDash);

  std::string * op = new std::string(prev_token_->lexeme());
  ParseResult prRight = parse_expr(0);
  // parse_expr(prev_token_->lbp());
  ast::Expr * right = nullptr;
  right = ast::CastCheck<>(right, prRight.ast(), "parse_subtraction ; right");
  pr.ast(new ast::BinOpExpr(left, op, right));

  return pr;
}

// Expr ::= Expr forwardSlash Expr
ParseResult Parser::parse_division(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr * left = nullptr;
  left = ast::CastCheck<>(left, prLeft.ast(), "parse_division ; left");
  match(scanner::kForwardSlash);

  std::string * op = new std::string(prev_token_->lexeme());

  ParseResult prRight = parse_expr(0);
  ast::Expr * right = nullptr;
  right = ast::CastCheck<>(right, prRight.ast(), "parse_division ; right");
  // parse_expr(prev_token_->lbp());

  pr.ast(new ast::BinOpExpr(left, op, right));
  return pr;
}

// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
*/
ParseResult Parser::parse_relational_expr(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr * left = nullptr;
  left = ast::CastCheck<>(left, prLeft.ast(), "parse_relational_expr ; left");

  next_token();
  // just advance token, since examining it in parse_expr caused
  // this method being called.
  std::string * op = new std::string(prev_token_->lexeme());

  ParseResult prRight = parse_expr(0);
  // parse_expr(prev_token_->lbp());
  ast::Expr * right = nullptr;
  right = ast::CastCheck<>(right, prRight.ast(),
                           "parse_relational_expr ; right");

  pr.ast(new ast::BinOpExpr(left, op, right));
  return pr;
}

// Helper function used by the parser.

void Parser::match(const scanner::TokenType &tt) {
  if (!attempt_match(tt)) {
    throw(make_error_msg_expected(tt));
  }
}

bool Parser::attempt_match(const scanner::TokenType &tt) {
  if (curr_token_->terminal() == tt) {
    next_token();
    return true;
  }
  return false;
}

bool Parser::next_is(const scanner::TokenType &tt) {
  return curr_token_->terminal() == tt;
}

void Parser::next_token() {
  if (curr_token_ == nullptr) {
    throw(std::string(
        "Internal Error: should not call nextToken in unitialized state"));
  } else if (curr_token_->terminal() == scanner::kEndOfFile &&
           curr_token_->next() == nullptr) {
    prev_token_ = curr_token_;
  } else if (curr_token_->terminal() != scanner::kEndOfFile &&
             curr_token_->next() == nullptr) {
    throw(make_error_msg("Error: tokens end with endOfFile"));
  } else {
    prev_token_ = curr_token_;
    curr_token_ = curr_token_->next();
  }
}

std::string Parser::terminal_description(const scanner::TokenType &terminal) {
  scanner::Token *dummy_token = new scanner::Token("", terminal, nullptr);
  scanner::ExtToken *dummy_ext_token = new scanner::ExtToken(this, dummy_token);
  std::string s =
  dummy_ext_token->ExtendToken(this, dummy_token)->description();
  delete dummy_token;
  delete dummy_ext_token;
  return s;
}

std::string Parser::make_error_msg_expected(
    const scanner::TokenType &terminal) {
  std::string s = (std::string) "Expected " + terminal_description(terminal) +
                  " but found " + curr_token_->description();
  return s;
}

std::string Parser::make_error_msg(const scanner::TokenType &terminal) {
  std::string s = "Unexpected symbol " + terminal_description(terminal);
  return s;
}

std::string Parser::make_error_msg(const char *msg) { return msg; }

} /* namespace parser */
} /* namespace fcal */
