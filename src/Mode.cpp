#include <iostream>
#include "Mode.h"
#include "Conditions_Checker.h"
#include "Code_Generator.h"

void Full_Compiler::run()
{
    Tree syntax_tree = parser.start();
    Conditions_Checker checker(program);
    syntax_tree.accept(checker);
    Code_Generator generator(out);
    syntax_tree.accept(generator);
}

void Full_Analysis::run()
{
    Tree syntax_tree = parser.start();
    Conditions_Checker visitor(program);
    syntax_tree.accept(visitor);
}

void Lex_Analysis::print_token()
{
    Token::value_type val = current_token.get_value();
    Scanner::line_type line = scanner.get_line();
    token_type type = current_token.get_type();
    std::cout << val << '\t' << line << '\t';
    if ((type > TOKEN_NULL) && (type < TOKEN_NUM))
        std::cout << "KEYWORD\n";
    else if (type == TOKEN_NUM)
        std::cout << "NUMBER\n";
    else if (type == TOKEN_ID)
        std::cout << "ID\n";
    else if (type == TOKEN_COMMENT)
        std::cout << "COMMENT\n";
    else if (type == TOKEN_STR)
        std::cout << "STRING\n";
}

void Lex_Analysis::run()
{
    current_token = scanner.get_token();
    while (current_token.get_type() != TOKEN_FIN) {
        print_token();
        current_token = scanner.get_token();
    }
}
