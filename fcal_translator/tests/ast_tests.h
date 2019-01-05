/*! \file
 * The tests in this file test proper functionality of
 * AST creation. The tests are listed in the order in
 * which they were implemented (i.e test_sample_1 was
 * made to pass before test_sample_2 etc.). This was done
 * by adding implementations of AST classes as needed to
 * pass each test case until the all tests passed and
 * the entire FCAL grammar was encompassed.
 */
#include <cxxtest/TestSuite.h>
#include <iostream>
#include "include/parser.h"
#include "include/read_input.h"

#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;
using namespace fcal;
using namespace parser;
using namespace ast;
using namespace scanner;

class AstTestSuite : public CxxTest::TestSuite
{
public:

    Parser p ;

    void writeFile ( const string text, const string filename) {
        ofstream out(filename.c_str());
        out << (string) text << endl ;
    }

    void unparse_tests ( string file) {
        string path = "./samples/" + file ;

        // 1. Test that the file can be parsed.
        ParseResult pr1 = p.Parse(ReadInputFromFile( path.c_str()));
        TSM_ASSERT(file + " failed to parse.", pr1.ok()) ;

        // 2. Verify that the ast field is not null
        TSM_ASSERT(file + " failed to generate an AST.", pr1.ast() != NULL);

        // 3. Verify that the "unparsing" is non-empty.
        string up1 = pr1.ast()->unparse() ;
        writeFile(up1,(path+"up1").c_str());
        TSM_ASSERT(file + " failed to generate non-empty unparsing.",
                     up1.length() > 0) ;

        // 4. Verify that the un-parsed string can be parsed.
        ParseResult pr2 = p.Parse(up1.c_str()) ;
        TSM_ASSERT(file + " failed to parse the first un-parsing.",
                     pr2.ok()) ;

        // 5. Verify that the ast field is not null after first unparsing
        TSM_ASSERT(file + " first unparsing failed to generate an AST.",
                     pr2.ast() != NULL);

        // 6. Verify that this second unparsing can be parsed.
        string up2 = pr2.ast()->unparse() ;
        writeFile(up2,(path+"up2").c_str());
        ParseResult pr3 = p.Parse(up2.c_str()) ;
        TSM_ASSERT(file + " failed to unparse the second un-parsing.",
                     pr3.ok()) ;

        string up3 = pr3.ast()->unparse() ;
        writeFile(up3,(path+"up3").c_str());
        // 7. Verify that the first and second unparsings are the same.
        TSM_ASSERT_EQUALS(file + " unparse-1 != unparse-2.", up1, up2) ;
        // 8. Verifty that the second and third unparsings are the same.
        TSM_ASSERT_EQUALS(file + " unparse-2 != unparse-3.", up2, up3) ;
    }
    /*! 
     * Tests 1, 2, 3 all passed upon testing April 9th
     * following the implementation of a large portion of
     * the grammar.
     */
    void test_sample_1(void) { unparse_tests("sample_1.dsl"); }
    void test_sample_2(void) { unparse_tests("sample_2.dsl"); }
    void test_sample_3(void) { unparse_tests("sample_3.dsl"); }
    /*!
     * Tests 4 and mysample were passed once the proper
     * matrix productions were implemented. April 9th.
     */
    void test_sample_4(void) { unparse_tests("sample_4.dsl"); }
    /*! 
     * Tests 5 and forest_loss were passed once a segmentation
     * fault was resolved. April 9th.
     */
    void test_sample_5(void) { unparse_tests("sample_5.dsl"); }
    void test_mysample(void) { unparse_tests("mysample.dsl"); }
    void test_forest_loss(void) { unparse_tests("forest_loss_v2.dsl"); }
} ;
