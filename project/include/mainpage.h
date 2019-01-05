#ifndef PROJECT_INCLUDE_MAINPAGE_H_
#define PROJECT_INCLUDE_MAINPAGE_H_
/*! \mainpage Index Page

\section intro_sec Introduction

Welcome to the implementation of the FCAL to C++ translator!
This project translates code written in FCAL, a domain specific language, 
and translates it to compilable and executable C++ code. The task is performed
by first scanning a file containing FCAL code, confirming that all characters
present are legally within the FCAL dsl. Regular expression matching is 
primarily used to achieve this. Second, a parser has been implemented to
check for proper syntax within the FCAL program. As the parser is checking the code,
it creates an abstract syntax tree to represent the structure of the FCAL
program. The tree is created by utilizing a class hierarchy that acts as a 
representation of the FCAL grammar. Abstract classes have been created for
each non-terminal within the language. Concrete subclasses were 
subsequently made to represent the set of productions in the grammar. 
Finally, the code is checked for semantic correctness before being 
translated to the equivalent C++ code.

\section install_sec Installation

Due to the large quantity of files required to create this software,
a Makefile is used for convenience of creating the necessary object files.
The stage of the software currently includes the implementation for
a scanner and parser/ast creation as described in the introduction.
Making the object files for these components is done by first retrieving
all files of type .cc and .h, in addition to the Makefile, then by executing
the following command:

make make_objects

These object files can then be used by applications as needed. This will
be updated with fully functioning software that performs the full 
language translation once all necessary components are implemented.

*/

#endif  // PROJECT_INCLUDE_MAINPAGE_H_
