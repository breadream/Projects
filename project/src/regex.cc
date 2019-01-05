/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <cstddef>
#include "include/regex.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Functions
 ******************************************************************************/
regex_t* make_regex(const char* pattern) {
  regex_t *re = new regex_t;
  int rc;

  /*
   * "Compile" the regular expression.  This sets up the regex to do the
   * matching specified by the regular expression given as a character string.
   */
  rc = regcomp(re, pattern, REG_EXTENDED);

  if (rc!= 0) {
    // printf ("Error in compiling regular expression.\n");
    std::size_t length = regerror(rc, re, nullptr, 0);
    char *buffer = new char[length];
    (void) regerror(rc, re, buffer, length);
    delete re;
    return nullptr;
  } else {
    return re;
  }
} /* make_regex() */

int match_regex(regex_t *re, const char *text) {
  int status;
  regmatch_t matches[kRegexNSub];

  /*
   * Execute the regular expression match against the text. If it matches, the
   * beginning and ending of the matched text are stored in the first element
   * of the matches array.
   */
  status = regexec(re, text, static_cast<size_t>(kRegexNSub), matches, 0);

  if (status == REG_NOMATCH) {
    return 0;
  } else {
    return matches[0].rm_eo;
  }
} /* match_regex() */

} /* namespace scanner */
} /* namespace fcal */
