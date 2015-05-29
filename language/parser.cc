#include "parser.hpp"

using namespace peglib;
using namespace std;

static auto g_grammar = R"(
    PROGRAM             <-  _ STATEMENTS

    STATEMENTS          <-  EXPRESSION*

    EXPRESSION          <-  ASSIGNMENT / PRIMARY
    ASSIGNMENT          <-  IDENTIFIER '=' _ EXPRESSION
    WHILE               <- 'while' _ EXPRESSION BLOCK
    IF                  <- 'if' _ EXPRESSION BLOCK ('else' _ 'if' _ EXPRESSION BLOCK)* ('else' _ BLOCK)?
    FUNCTION            <-  'fn' _ PARAMETERS BLOCK
    PARAMETERS          <-  '(' _ IDENTIFIER* ')' _ 
    FUNCTION_CALL       <-  IDENTIFIER ARGUMENTS
    ARGUMENTS           <-  '(' _ EXPRESSION* ')' _ 

    PRIMARY             <-  CONDITION (CONDITION_OPERATOR CONDITION)?
    CONDITION           <-  TERM (TERM_OPERATOR TERM)*
    TERM                <-  FACTOR (FACTOR_OPERATOR FACTOR)*
    FACTOR              <-  WHILE / IF / FUNCTION / FUNCTION_CALL / NUMBER / BOOLEAN / STRING / IDENTIFIER / '(' _ EXPRESSION ')' _

    BLOCK               <-  '{' _ STATEMENTS '}' _

    CONDITION_OPERATOR  <-  < ('==' / '!=' / '<=' / '<' / '>=' / '>') > _
    TERM_OPERATOR       <-  < [-+] > _
    FACTOR_OPERATOR     <-  < [*/%] > _
    IDENTIFIER          <-  < [a-zA-Z_]+ > _

    NUMBER              <-  < [0-9]+ > _
    BOOLEAN             <-  < ('true' / 'false') > _
    STRING              <-  ['] < (!['] .)* > ['] _

    ~_                  <-  (Space / EndOfLine / Comment)*
    Space               <-  ' ' / '\t'
    EndOfLine           <-  '\r\n' / '\n' / '\r'
    EndOfFile           <-  !.
    Comment             <-  '/*' (!'*/' .)* '*/' /  ('#' / '//') (!(EndOfLine / EndOfFile) .)* (EndOfLine / EndOfFile)
)";

static auto g_parser = peg(g_grammar)
    .ast_node_optimizable("STATEMENTS", Statements)
    .ast_node("WHILE", While)
    .ast_node("ASSIGNMENT", Assignment)
    .ast_node("IF", If)
    .ast_node("FUNCTION", Function)
    .ast_node("PARAMETERS")
    .ast_node("FUNCTION_CALL", FunctionCall)
    .ast_node("ARGUMENTS")
    .ast_node_optimizable("PRIMARY", Condition)
    .ast_node_optimizable("CONDITION", BinExpresion)
    .ast_node_optimizable("TERM", BinExpresion)
    .ast_token("CONDITION_OPERATOR")
    .ast_token("TERM_OPERATOR")
    .ast_token("FACTOR_OPERATOR")
    .ast_token("NUMBER", Number)
    .ast_token("BOOLEAN", Boolean)
    .ast_token("STRING")
    .ast_token("IDENTIFIER", Identifier)
    .ast_end()
    .set_logger([&](size_t ln, size_t col, const string& msg) {
        cerr << ln << ":" << col << ": " << msg << endl;
    });

const peglib::peg& get_parser()
{
    return g_parser;
}

// vim: et ts=4 sw=4 cin cino={1s ff=unix
