#include <iostream>
#include "include/regex.h"
#include "include/scanner.h"
#include "include/token.h"

namespace fcal {
namespace scanner {
// Create the compiled regular expressions for future use
regex_t * white_space = make_regex("^[\n\t\r ]+");
regex_t * block_comment = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
regex_t * line_comment = make_regex("^//[^\n]*\n");

int consume_whitespace_and_comments(regex_t *white_space,
regex_t *block_comment, regex_t *line_comment, const char *text);

Token * Scanner::Scan(const char *text) {
    int num_matched_chars;

    /*!
     * Scanned objects will be represented as
     * having a head, body and tail to represent
     * subsequent scanning of tokens in an 
     * intuitive manner.
     */
    Token * head = nullptr;
    Token * body = nullptr;
    Token * tail = nullptr;

    /*!
     * Get rid of white spaces and comments
     */
    num_matched_chars =
    consume_whitespace_and_comments(white_space,
  block_comment, line_comment, text);
    text += num_matched_chars;

    while (text[0] != '\0') {
        /*!
         *  Match the next token and make it the tail
         */
        tail = token_iterator(text);
        if (head != nullptr) {
            /*!
             * Move the body pointer along to the tail
             * in preparation for the next token to 
             * be scanned.
             */
            body->set_next_(tail);
            body = tail;
        } else {
            /*!
             * Initial case for the first token scanned.
             * Head, body and tail all point to the same
             * token.
             */
            head = tail;
            body = tail;
        }
        text += body->lexeme().length();
        num_matched_chars =
  consume_whitespace_and_comments(white_space,
  block_comment, line_comment, text);
        text += num_matched_chars;
    }
    /*!
     * Add an end of file token at the end.
     */
    tail = new Token(kEndOfFile, "kEndOfFile", nullptr);

    if (head == nullptr) {
        return tail;
    }
    body->set_next_(tail);
    return head;
} /* scan(const char *) */

/*!
 * An iterator method to find
 * the next token.
 */
Token * Scanner::token_iterator(const char *text) {
    int terminal_type;
    int max_matched = int();  // zero-initialized
    std::string lexeme;
    Token * next_token;

    for (int i = kIntKwd; i < kLexicalError+1; i++) {
        /*!
         * Perform checks to identify which regular expression
         * the current token should be matched to. Done by
         * comparing lengths of attempting matches for each
         * possible regular expression.
         */ 
        if (max_matched < match_regex(get_regex(i), text)) {
            terminal_type = i;
            max_matched = match_regex(get_regex(i), text);
  // Usage : string& assign (const char* s, size_t n);
  // Copies the first n characters from the array of characters pointed by s.
            lexeme.assign(text, match_regex(get_regex(i), text));
        }
    }
    // static_cast -> int to enum type casting
    next_token = new Token(static_cast<TokenType>(terminal_type),
 lexeme, nullptr);
    return next_token;
}

int consume_whitespace_and_comments(regex_t *white_space,
 regex_t *block_comment, regex_t *line_comment, const char *text) {
  int num_matched_chars = 0;
  int total_num_matched_chars = 0;
  int still_consuming_white_space;

  do {
    still_consuming_white_space = 0;  // exit loop if not reset by a match

    // Try to match white space
    num_matched_chars = match_regex(white_space, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }

    // Try to match block comments
    num_matched_chars = match_regex(block_comment, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }

    // Try to match line comment
    num_matched_chars = match_regex(line_comment, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }
  } while (still_consuming_white_space);

  return total_num_matched_chars;
} /* consume_whitespace_and_comments() */

}  // namespace scanner
}  // namespace fcal
