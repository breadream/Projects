/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "include/scanner.h"
#include "include/token.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Functions
 ******************************************************************************/
  Token::Token() {
     this->next_ = nullptr;
  } /* Token() */

  Token::~Token() {} /* ~Token() */

  Token::Token(TokenType terminal, std::string lexeme, Token * next) {
    terminal_ = terminal;
    lexeme_ = lexeme;
    next_ = next;
  } /* Token(TokenType, std::string, Token *) */

  Token::Token(std::string lexeme, TokenType terminal, Token * next) {
  lexeme_ = lexeme;
  terminal_ = terminal;
  next_ = next;
  } /* Token(std::string, TokenType, Token) */

  TokenType Token::terminal() {
    /* Return terminal_ value */
    return this->terminal_;
  } /* get_terminal_() */

  std::string Token::lexeme() {
    /* Return lexeme_ value */
    return this->lexeme_;
  } /* get_lexeme_() */

  Token * Token::next() {
    /* Return a pointer to the next token */
    return this->next_;
  } /* get_next_() */

  void Token::set_terminal_(TokenType new_terminal) {
    /* Set the terminal_ to a new value */
    this->terminal_ = new_terminal;
    return;
  } /* set_terminal_() */

  void Token::set_lexeme_(std::string new_lexeme) {
    /* Set the lexeme_ to a new value */
    this->lexeme_ = new_lexeme;
    return;
  } /* set_lexeme() */

  void Token::set_next_(Token * new_next) {
    /* Point next_ to a new value */
    this->next_ = new_next;
    return;
  } /* set_next() */

}  // namespace scanner
}  // namespace fcal
