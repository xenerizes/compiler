#include "Scanner.h"
#include "Tree.h"

#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

class Parser {
    public:
        Parser(std::string filename): scanner(filename) {}
        ~Parser() {}

        Tree start();
    private:
        Scanner scanner;
        Token current_token;

        Parser(const Parser &);
        Parser &operator=(const Parser &);

        void next_token()
        {
            current_token = scanner.get_token();
            while (current_token.get_type() == TOKEN_COMMENT)
                current_token = scanner.get_token();
        }

        Node *_compilation_unit();
        Main *_main_declaration();
        Block *_block();
        Statement *_block_statement();
        Var_Declaration *_variable_declaration();
        Statement *_statement();
        Branch *_branch();
        Loop *_while();
        Assignment *_assignment();
        Print *_print_statement();
        Expression *_expression();
        Expression *_expression_OR();
        Expression *_expression_AND();
        Expression *_expression_EQ();
        Expression *_expression_CMP();
        Expression *_expression_AS();
        Expression *_expression_MD();
        Expression *_expression_NOT();
        Expression *_expression_PREF();
        Expression *_operation_OR(Expression *);
        Expression *_operation_AND(Expression *);
        Expression *_operation_EQ(Expression *);
        Expression *_operation_CMP(Expression *);
        Expression *_operation_AS(Expression *);
        Expression *_operation_MD(Expression *);
        Expression *_operation_PREF();
        Expression *_operation_POST(Expression *);
        Expression *_primary();
};

#endif // PARSER_H_INCLUDED
