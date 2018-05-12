# Compiler-Front-End
Lexical Analyzer and Parser

The project requires you to build a basic compiler front-end. This means that your compiler should be able to
read an input source le, perform lexical analysis, parsing, semantic analysis and nally generate code in
some machine-independent intermediate form (such as three address code). The compiler should be built
using Flex and Bison.
The source language for the compiler is a restricted subset of the C programming language. In addition
to the basic features (loops, conditionals etc.) we have already seen in class, it should have
1. Scopes
2. Data types including strings
3. Arrays
4. Type checking.
5. Support for functions.
6. A record or structure type such as the C struct.
The language should be expressive enough that you can write a simple program such as bubble sort.
## Lexical Analyzer
As a first step in building the compiler you are to write a lexical analyzer for your proposed language using
Flex.
Figure out all the tokens your programming language allows and write them out in a Lex le. Submit the
Lex le alongwith three example source les. The example source les should contain examples of programs
in your intended language. Make sure that the examples cover all features of your language. One of the
programs should be an implementation of bubble sort in your proposed language with a separate function
for the sorting routine.
## Parser
Step two of the project will be to create a parser using either top-down or bottom-up methods. Bottom-up
parsers can be built using Bison. You should also have a working handling of data types and scopes.
