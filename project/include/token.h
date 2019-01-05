#ifndef PROJECT_INCLUDE_TOKEN_H_
#define PROJECT_INCLUDE_TOKEN_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "include/iter2scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Class Declarations
 ******************************************************************************/
class Token {
    // public declarations
 public:
      // Constructor
      Token();
      Token(std::string, TokenType, Token *);
      Token(TokenType, std::string, Token *);

      // Destructor
      ~Token();

      // Accessors
      TokenType terminal();
      std::string lexeme();
      Token * next();

      // Mutators
      void set_terminal_(TokenType);
      void set_lexeme_(std::string);
      void set_next_(Token *);

    // private declarations
 private:
      TokenType terminal_;
      std::string lexeme_;
      Token * next_;
}; /* class Token */
} /* namespace scanner */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_TOKEN_H_
