#ifndef PROJECT_INCLUDE_SCANNER_H_
#define PROJECT_INCLUDE_SCANNER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "include/token.h"
#include "include/regex.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Class Definitions
 * 
 * You will need to define a Token class (remember, a pointer of type Token *
 * is returned by the scan method of the scanner class.) 
 * Also, Classes are typically declared (and sometimes also defined) in
 * their own header files
 * 
 * You will also need to define a Scanner class with a scan and other methods
 ******************************************************************************/

/*******************************************************************************
 * Class Declarations
 ******************************************************************************/
/*!
 * \brief Scanner class definition
 * Holds the array of regular expressions
 * for the FCAL dsl, a token iterator method to
 * scan the next token and a scan method that
 * will scan the entire program to make a linked
 * list of tokens.
 */
class Scanner {
    // public declarations
 public:
  Token * Scan(const char *);
  Token * token_iterator(const char *);

  /*!
  An initialized scanner instance receives all necessary
  regular expressions for the FCAL dsl. These are stored
  within the token_regex array private field.
  */
  Scanner() {
    token_regex[kIntKwd] = make_regex("^int");
    token_regex[kFloatKwd] = make_regex("^float");
    token_regex[kBoolKwd] = make_regex("^boolean");
    token_regex[kTrueKwd] = make_regex("^True");
    token_regex[kFalseKwd] = make_regex("^False");
    token_regex[kStringKwd] = make_regex("^string");
    token_regex[kMatrixKwd] = make_regex("^matrix");
    token_regex[kLetKwd] = make_regex("^let");
    token_regex[kInKwd] = make_regex("^in");
    token_regex[kEndKwd] = make_regex("^end");
    token_regex[kIfKwd] = make_regex("^if");
    token_regex[kThenKwd] = make_regex("^then");
    token_regex[kElseKwd] = make_regex("^else");
    token_regex[kRepeatKwd] = make_regex("^repeat");
    token_regex[kPrintKwd] = make_regex("^print");
    token_regex[kWhileKwd] = make_regex("^while");
    token_regex[kToKwd] = make_regex("^to");

    // Constants
    token_regex[kIntConst] = make_regex("^[0-9]+");
    token_regex[kFloatConst] = make_regex("^[0-9]*\\.?[0-9]*");
    token_regex[kStringConst] = make_regex("^\"(\\.|[^\"])*\"");

    // Names
    token_regex[kVariableName] = make_regex("^[a-zA-Z_][a-zA-Z_0-9]*");

    // Punctuation
    token_regex[kLeftParen] = make_regex("^\\(");
    token_regex[kRightParen] = make_regex("^)");
    token_regex[kLeftCurly] = make_regex("^\\{");
    token_regex[kRightCurly] = make_regex("^}");
    token_regex[kLeftSquare] = make_regex("^\\[");
    token_regex[kRightSquare] = make_regex("^]");
    token_regex[kSemiColon] = make_regex("^;");
    token_regex[kColon] = make_regex("^:");

    // Operators
    token_regex[kAssign] = make_regex("^=");
    token_regex[kPlusSign] = make_regex("^\\+");
    token_regex[kStar] = make_regex("^\\*");
    token_regex[kDash] = make_regex("^-");
    token_regex[kForwardSlash] = make_regex("^/");
    token_regex[kLessThan] = make_regex("^<");
    token_regex[kLessThanEqual] = make_regex("^<=");
    token_regex[kGreaterThan] = make_regex("^>");
    token_regex[kGreaterThanEqual] = make_regex("^>=");
    token_regex[kEqualsEquals] = make_regex("^==");
    token_regex[kNotEquals] = make_regex("^!=");
    token_regex[kAndOp] = make_regex("^&&");
    token_regex[kOrOp] = make_regex("^\\|\\|");
    token_regex[kNotOp] = make_regex("^!");

    // Special terminal types
    token_regex[kEndOfFile] = make_regex("^EOF");
    token_regex[kLexicalError] = make_regex("[^ ]");
  }

  /// Get a regex based on the index
  regex_t * get_regex(int index) {
    return token_regex[index];
  }

 private:
  regex_t * token_regex[kLexicalError+1];
}; /* class Scanner */
}/* namespace scanner */
}/* namespace fcal */

#endif  // PROJECT_INCLUDE_SCANNER_H_
