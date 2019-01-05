#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include "include/read_input.h"
#include "include/scanner.h"
#include "include/regex.h"

using namespace std;
namespace scanner = fcal::scanner;
class ScannerTestSuite : public CxxTest::TestSuite
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    scanner::Scanner *s;
    void test_setup_code () {
      s = new scanner::Scanner();
    }

    void make_token_test (int token_regex_index, const char * good_match, 
			  int good_match_length) {
      regex_t * reg = s->get_regex(token_regex_index);
      int match_length_good = scanner::match_regex(reg, good_match);
      TS_ASSERT(match_length_good == good_match_length);
    }

    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    // You should place tests for these other functions used by
    // scan below.


    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly () {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{");
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */
    void test_terminal_IntKwd() {
      make_token_test(scanner::kIntKwd, "int", 3);
    }
    void test_terminal_FloatKwd() {
      make_token_test(scanner::kFloatKwd, "float", 5);
    }
    void test_terminal_BoolKwd() {
      make_token_test(scanner::kBoolKwd, "boolean", 7);
    } 
    void test_terminal_TrueKwd() {
      make_token_test(scanner::kTrueKwd, "True", 4);
    }
    void test_terminal_FalseKwd() {
      make_token_test(scanner::kFalseKwd, "False", 5);
    }
    void test_terminal_StringKwd() {
      make_token_test(scanner::kStringKwd, "string", 6);
    }
    void test_terminal_MatrixKwd() {
      make_token_test(scanner::kMatrixKwd, "matrix", 6);
    }
    void test_terminal_LetKwd() {
      make_token_test(scanner::kLetKwd, "let", 3);
    }
    void test_terminal_InKwd() {
      make_token_test(scanner::kInKwd, "in", 2);
    }
    void test_terminal_EndKwd() {
      make_token_test(scanner::kEndKwd, "end", 3);
    }
    void test_terminal_IfKwd() {
      make_token_test(scanner::kIfKwd, "if", 2);
    }
    void test_terminal_ThenKwd() {
      make_token_test(scanner::kThenKwd, "then", 4);
    }
    void test_terminal_ElseKwd() {
      make_token_test(scanner::kElseKwd, "else", 4);
    }
    void test_terminal_RepeatKwd() {
      make_token_test(scanner::kRepeatKwd, "repeat", 6);
    }
    void test_terminal_WhileKwd() {
      make_token_test(scanner::kWhileKwd, "while", 5);
    }
    void test_terminal_PrintKwd() {
      make_token_test(scanner::kPrintKwd, "print", 5);
    }
    void test_terminal_ToKwd() {
      make_token_test(scanner::kToKwd, "to", 2);
    }
    void test_terminal_IntConst() {
      make_token_test(scanner::kIntConst, "145", 3);
    }
    void test_terminal_FloatConst() {
      make_token_test(scanner::kFloatConst, "14.358", 6);
    }
    void test_terminal_StringConst() {
      make_token_test(scanner::kStringConst, "\"hiIAmAString\"", 14);
    }
    void test_terminal_VariableName() {
      make_token_test(scanner::kVariableName, "good_variable_name1", 19);
    }
    void test_terminal_LeftParen() {
      make_token_test(scanner::kLeftParen, "((", 1);
    }
    void test_terminal_RightParen() {
      make_token_test(scanner::kRightParen, "))", 1);
    }
    void test_terminal_LeftCurly() {
      make_token_test(scanner::kLeftCurly, "{{", 1);
    }
    void test_terminal_RightCurly() {
      make_token_test(scanner::kRightCurly, "}}", 1);
    }
    void test_terminal_LeftSquare() {
      make_token_test(scanner::kLeftSquare, "[[", 1);
    }
    void test_terminal_RightSquare() {
      make_token_test(scanner::kRightSquare, "]]", 1);
    }
    void test_terminal_SemiColon() {
      make_token_test(scanner::kSemiColon, ";", 1);
    }
    void test_terminal_Colon() {
      make_token_test(scanner::kColon, ":", 1);
    }
    void test_terminal_Assign() {
      make_token_test(scanner::kAssign, "=", 1);
    }
    void test_terminal_PlusSign() {
      make_token_test(scanner::kPlusSign, "+", 1);
    }
    void test_terminal_Star() {
      make_token_test(scanner::kStar, "*", 1);
    }
    void test_terminal_Dash() {
      make_token_test(scanner::kDash, "-", 1);
    }
    void test_terminal_ForwardSlash() {
      make_token_test(scanner::kForwardSlash, "/", 1);
    }
    void test_terminal_LessThan() {
      make_token_test(scanner::kLessThan, "<", 1);
    }
    void test_terminal_LessThanEqual() {
      make_token_test(scanner::kLessThanEqual, "<=", 2);
    }
    void test_terminal_GreaterThan() {
      make_token_test(scanner::kGreaterThan, ">", 1);
    }
    void test_terminal_GreaterThanEqual() {
      make_token_test(scanner::kGreaterThanEqual, ">=", 2);
    }
    void test_terminal_EqualsEquals() {
      make_token_test(scanner::kEqualsEquals, "==", 2);
    }
    void test_terminal_NotEquals() {
      make_token_test(scanner::kNotEquals, "!=", 2);
    }
    void test_terminal_AndOp() {
      make_token_test(scanner::kAndOp, "&&", 2);
    }
    void test_terminal_OrOp() {
      make_token_test(scanner::kOrOp, "||", 2);
    }
    void test_terminal_NotOp() {
      make_token_test(scanner::kNotOp, "!", 1);
    }
    void test_terminal_EndOfFile() {
      make_token_test(scanner::kEndOfFile, "EOF", 3);
    }
    void test_terminal_LexicalError() {
      make_token_test(scanner::kLexicalError, "b", 1);
    }

    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */
    


    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (scanner::Token *tks) {
        scanner::Token *currentToken = tks;
        while (currentToken != NULL) {
          if (currentToken->terminal() == scanner::kLexicalError) {
              printf("problem: %s\n",currentToken->lexeme().c_str());
              fflush(stdout);
                                return false;
            }
            else {
              currentToken = currentToken->next();
            }
        }
        return true;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors(const char* filename) {
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT (text);
        scanner::Token *tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        TS_ASSERT(noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
  bool sameTerminals(scanner::Token *tks, int numTerms,
                     scanner::TokenType *ts) {
        scanner::Token *currentToken = tks;
        int termIndex = 0;
        while(currentToken != NULL && termIndex < numTerms) {
			
            if(currentToken->terminal() != ts[termIndex]) {
                printf("item at index: %i: terminal: %i should be terminal: %i\n",termIndex,currentToken->terminal(), ts[termIndex]);
				fflush(stdout);
                return false;
            }
            else {
                ++ termIndex;
                currentToken = currentToken->next();
            }
        }
        return true;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty() {
        scanner::Token *tks = s->Scan("  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    void test_scan_empty_comment() {
        scanner::Token *tks = s->Scan(" /* a comment */ ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    // When a lexical error occurs, the scanner creates a token with a
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors() {
        scanner::Token *tks = s->Scan("$&1  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "$");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "&");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntConst);
        TS_ASSERT_EQUALS(tks->lexeme(), "1");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }


    // A test for scanning numbers and a variable.

    void test_scan_nums_vars() {
        scanner::Token *tks = s->Scan(" 123 x 12.34 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIntConst, scanner::kVariableName,
                                           scanner::kFloatConst, scanner::kEndOfFile };
        TS_ASSERT( sameTerminals( tks, 4, ts));
    }


    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens() {
        const char *filename = "./samples/bad_syntax_good_tokens.dsl";
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = NULL;
        tks = s->Scan( text);
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kIntConst, scanner::kIntConst, scanner::kIntConst,
          scanner::kStringConst, scanner::kStringConst, scanner::kStringConst,
          scanner::kFloatConst, scanner::kFloatConst, scanner::kFloatConst,
          scanner::kMatrixKwd,
          scanner::kSemiColon, scanner::kColon, scanner::kToKwd,
          scanner::kLeftCurly, scanner::kLeftParen, scanner::kRightCurly, scanner::kRightParen,
          scanner::kPlusSign, scanner::kStar, scanner::kDash, scanner::kForwardSlash,
          scanner::kEqualsEquals, scanner::kLessThanEqual,
          scanner::kGreaterThanEqual, scanner::kNotEquals,
          scanner::kAssign,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kIntKwd, scanner::kFloatKwd, scanner::kStringKwd,
          scanner::kEndOfFile
        };
        int count = 38;
        TS_ASSERT( sameTerminals( tks, count, ts));
    }

    void test_scan_sample_forestLoss() {
        scanFileNoLexicalErrors("./samples/forest_loss_v2.dsl");
    }



};

