#Note, you may have to update CXX_DIR and
#CXXTEXT to point the right locations and executables
#to get it to work - look at your Lab 3 Makefile
CXX_DIR = ./cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -I. -std=c++11

# Program files.
read_input.o:	src/read_input.cc include/read_input.h
	g++ $(FLAGS) -c src/read_input.cc 

regex.o:	src/regex.cc include/regex.h
	g++ $(FLAGS) -c src/regex.cc 

# Below is a possible way to make scanner
# Yours may vary depending on your implementation
scanner.o:	src/scanner.cc include/scanner.h include/regex.h include/token.h
	g++ $(FLAGS) -c src/scanner.cc 

token.o:	src/token.cc include/token.h include/scanner.h
	g++ $(FLAGS) -c src/token.cc


# Testing files and targets.  
# run-tests should work once 
# and run regex_tests once you put the files
# we gave you in the right places
# and update the CXX_DIR and CXXTEST variables so they
# reference the correct directory locations
# Add scanner_tests to the dependency list and uncomment when
# you are ready to start testing units with scanner_tests.
run-tests:	regex_tests scanner_tests parser_tests ast_tests codegeneration_tests
	./regex_tests
	./scanner_tests
	./parser_tests
	./ast_tests
	./codegeneration_tests

#This should work once you put the files
#we gave you in the right places
regex_tests:	regex_tests.cc regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o regex_tests regex.o regex_tests.cc

regex_tests.cc:	include/regex.h tests/regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cc tests/regex_tests.h

# Below is a possible way to make scanner_tests and scanner_tests.cc
# Yours may vary depending on your design and implementation 
scanner_tests:	scanner_tests.cc scanner.o token.o regex.o read_input.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o scanner_tests \
		regex.o scanner.o read_input.o token.o scanner_tests.cc

scanner_tests.cc:	scanner.o tests/scanner_tests.h include/read_input.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cc tests/scanner_tests.h

clean:
	rm -Rf *.o \
		regex_tests regex_tests.cc \
		scanner_tests scanner_tests.cc \
		parser_tests parser_tests.cc \
                ast_tests ast_tests.cc \
		codegeneration_tests codegeneration_tests.cc
clean_dsl:
	rm samples/*.dslup? 
			 

# for future use 
ast.o: include/ast.h src/ast.cc
	g++ $(FLAGS) -c src/ast.cc
parser.o : src/parser.cc include/parser.h include/ext_token.h include/parse_result.h include/scanner.h include/ast.h
	g++ $(FLAGS) -c src/parser.cc
ext_token.o : src/ext_token.cc include/ext_token.h include/parser.h include/scanner.h include/token.h
	g++ $(FLAGS) -c src/ext_token.cc
Matrix.o: include/Matrix.h src/Matrix.cc
	g++ $(FLAGS) -c src/Matrix.cc

parser_tests.cc: parser.o tests/parser_tests.h include/ext_token.h include/parse_result.h include/parser.h include/read_input.h include/scanner.h
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cc tests/parser_tests.h
parser_tests : parser_tests.cc parser.o read_input.o ext_token.o scanner.o token.o regex.o ast.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o parser_tests ext_token.o read_input.o parser.o scanner.o token.o regex.o ast.o parser_tests.cc

ast_tests.cc: ast.o include/parser.h include/read_input.h
	$(CXXTEST) $(CXXFLAGS) -o ast_tests.cc tests/ast_tests.h
ast_tests: ast_tests.cc ast.o parser.o read_input.o ext_token.o scanner.o token.o regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o ast_tests ast.o parser.o read_input.o ext_token.o scanner.o token.o regex.o ast_tests.cc

codegeneration_tests.cc: Matrix.o include/parser.h include/read_input.h
	$(CXXTEST) $(CXXFLAGS) -o codegeneration_tests.cc tests/codegeneration_tests.h
codegeneration_tests: codegeneration_tests.cc ast.o parser.o read_input.o ext_token.o scanner.o token.o regex.o Matrix.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o codegeneration_tests Matrix.o ast.o parser.o read_input.o ext_token.o scanner.o token.o regex.o codegeneration_tests.cc

make_objects: read_input.o regex.o scanner.o token.o ast.o parser.o ext_token.o Matrix.o
