#include <set>
#include "Parser.h"

#ifndef _PARSER_
#define _PARSER_

Tree Parser::start()
{
    next_token();
    return Tree(_compilation_unit());
}

Expression *Parser::_primary()
{
    if (current_token.get_type() == TOKEN_NUM) {
        Number *tmp = new Number(current_token.get_value());
        try {
            next_token();
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    else if (current_token.get_type() == TOKEN_ID) {
        Identifier *tmp = new Identifier(current_token.get_value());
        try {
            next_token();
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    else if (current_token.get_type() != TOKEN_LPAREN)
        throw Error("Syntax error: an expression in parentheses, number or identifier expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    Expression *tmp = _expression();
    try {
        if (current_token.get_type() != TOKEN_RPAREN)
            throw Error("Syntax error: delim <)> expected", SYNT_ANALYSIS, scanner.get_line());
            next_token();
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Expression *Parser::_operation_POST(Expression *argument)
{
    if (current_token.get_type() == TOKEN_INC) {
        next_token();
        Operation_POSTINC *tmp = new Operation_POSTINC;
        tmp->set_first_argument(argument);
        Expression *arg;
        try {
            arg = _operation_POST(tmp);
            if (arg != nullptr)
                return arg;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    } else if (current_token.get_type() == TOKEN_DEC) {
        next_token();
        Operation_POSTDEC *tmp = new Operation_POSTDEC;
        tmp->set_first_argument(argument);
        Expression *arg;
        try {
            arg = _operation_POST(tmp);
            if (arg != nullptr)
                return arg;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    return nullptr;
}

Expression *Parser::_operation_PREF()
{
    if (current_token.get_type() == TOKEN_PLUS)
        return new Operation_UPLUS;
    if (current_token.get_type() == TOKEN_MINUS)
        return new Operation_UMINUS;
    if (current_token.get_type() == TOKEN_INC)
        return new Operation_PREFINC;
    if (current_token.get_type() == TOKEN_DEC)
        return new Operation_PREFDEC;
    return nullptr;
}

Expression *Parser::_operation_MD(Expression *first_arg)
{
    if (first_arg == nullptr)
        throw Error("Syntax error: empty argument (operation_MD)", SYNT_ANALYSIS, scanner.get_line());
    if (current_token.get_type() == TOKEN_MUL) {
        next_token();
        Operation_MUL *tmp = new Operation_MUL;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_MD();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_MD(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    } else if (current_token.get_type() == TOKEN_DIV) {
        next_token();
        Operation_DIV *tmp = new Operation_DIV;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_MD();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_MD(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    } else if (current_token.get_type() == TOKEN_MOD) {
        next_token();
        Operation_MOD *tmp = new Operation_MOD;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_MD();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_MD(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    return nullptr;
}

Expression *Parser::_operation_CMP(Expression *first_arg)
{
    if (first_arg == nullptr)
        throw Error("Syntax error: empty argument (operation_CMP)", SYNT_ANALYSIS, scanner.get_line());
    if (current_token.get_type() == TOKEN_LOEQ) {
        next_token();
        Operation_LOEQ *tmp = new Operation_LOEQ;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_CMP();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_CMP(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    } else if (current_token.get_type() == TOKEN_GOEQ) {
        next_token();
        Operation_GOEQ *tmp = new Operation_GOEQ;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_CMP();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_CMP(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    } else if (current_token.get_type() == TOKEN_LEQ) {
        next_token();
        Operation_LEQ *tmp = new Operation_LEQ;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_CMP();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_CMP(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    } else if (current_token.get_type() == TOKEN_GEQ) {
        next_token();
        Operation_GEQ *tmp = new Operation_GEQ;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_CMP();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_CMP(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    return nullptr;
}

Expression *Parser::_operation_AS(Expression *first_arg)
{
    if (first_arg == nullptr)
        throw Error("Syntax error: empty argument (operation_AS)", SYNT_ANALYSIS, scanner.get_line());
    if (current_token.get_type() == TOKEN_PLUS) {
        next_token();
        Operation_PLUS *tmp = new Operation_PLUS;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_AS();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_AS(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    } else if (current_token.get_type() == TOKEN_MINUS) {
        next_token();
        Operation_MINUS *tmp = new Operation_MINUS;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_AS();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_AS(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    return nullptr;
}

Expression *Parser::_operation_EQ(Expression *first_arg)
{
    if (first_arg == nullptr)
        throw Error("Syntax error: empty argument (operation_EQ)", SYNT_ANALYSIS, scanner.get_line());
    if (current_token.get_type() == TOKEN_EQ) {
        next_token();
        Operation_EQ *tmp = new Operation_EQ;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_EQ();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_EQ(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    else if (current_token.get_type() == TOKEN_NEQ) {
        next_token();
        Operation_NEQ *tmp = new Operation_NEQ;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_EQ();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_EQ(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    return nullptr;
}

Expression *Parser::_operation_AND(Expression *first_arg)
{
    if (first_arg == nullptr)
        throw Error("Syntax error: empty argument (operation <&&>)", SYNT_ANALYSIS, scanner.get_line());
    if (current_token.get_type() == TOKEN_AND) {
        next_token();
        Operation_AND *tmp = new Operation_AND;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_AND();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_AND(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    return nullptr;
}

Expression *Parser::_operation_OR(Expression *first_arg)
{
    if (first_arg == nullptr)
        throw Error("Syntax error: empty argument (operation <||>)", SYNT_ANALYSIS, scanner.get_line());
    if (current_token.get_type() == TOKEN_OR) {
        next_token();
        Operation_OR *tmp = new Operation_OR;
        tmp->set_first_argument(first_arg);
        Expression *second_arg;
        try {
            second_arg = _expression_OR();
            tmp->set_second_argument(second_arg);
            Expression *ptr;
            ptr = _operation_OR(tmp);
            if (ptr != nullptr)
                return ptr;
        } catch (Error &err) { delete tmp; throw; }
        return tmp;
    }
    return nullptr;
}

Expression *Parser::_expression_PREF()
{
    Expression *arg = _primary();
    Expression *post;
    try {
        post = _operation_POST(arg);
        if (post != nullptr)
            return post;
    } catch (Error &err) { delete arg; throw; }
    return arg;
}

Expression *Parser::_expression_NOT()
{
    Expression *ptr, *arg, *tmp = nullptr;
    std::set<token_type> tokens = { TOKEN_PLUS, TOKEN_MINUS, TOKEN_INC, TOKEN_DEC };
    try {
        if (tokens.count(current_token.get_type()) != 0) {
            tmp = _operation_PREF();
            ptr = tmp;
            next_token();
            while (tokens.count(current_token.get_type()) != 0) {
                ptr->set_first_argument(_operation_PREF());
                ptr = ptr->get_first_argument();
                next_token();
            }
        }
        arg = _expression_PREF();
        if (tmp != nullptr) {
            ptr->set_first_argument(arg);
            return tmp;
        }
    } catch (Error &err) { delete tmp; throw; }
    return arg;
}

Expression *Parser::_expression_MD()
{
    Expression *ptr, *arg, *tmp = nullptr;
    try {
        if (current_token.get_type() == TOKEN_NOT) {
            tmp = new Operation_NOT;
            ptr = tmp;
            next_token();
            while (current_token.get_type() == TOKEN_NOT) {
                ptr->set_first_argument(new Operation_NOT);
                ptr = ptr->get_first_argument();
                next_token();
            }
        }
        arg = _expression_NOT();
        if (tmp != nullptr) {
            ptr->set_first_argument(arg);
            return tmp;
        }
    } catch (Error &err) { delete tmp; throw; }
    return arg;
}

Expression *Parser::_expression_AS()
{
    Expression *first_arg = _expression_MD();
    Expression *tmp;
    try {
        tmp = _operation_MD(first_arg);
        if (tmp != nullptr)
            return tmp;
    } catch (Error &err) { delete first_arg; throw; }
    return first_arg;
}

Expression *Parser::_expression_CMP()
{
    Expression *first_arg = _expression_AS();
    Expression *tmp;
    try {
        tmp = _operation_AS(first_arg);
        if (tmp != nullptr)
            return tmp;
    } catch (Error &err) { delete first_arg; throw; }
    return first_arg;
}

Expression *Parser::_expression_EQ()
{
    Expression *first_arg = _expression_CMP();
    Expression *tmp;
    try {
        tmp = _operation_CMP(first_arg);
        if (tmp != nullptr)
            return tmp;
    } catch (Error &err) { delete first_arg; throw; }
    return first_arg;
}

Expression *Parser::_expression_AND()
{
    Expression *first_arg = _expression_EQ();
    Expression *tmp;
    try {
        tmp = _operation_EQ(first_arg);
        if (tmp != nullptr)
            return tmp;
    } catch (Error &err) { delete first_arg; throw; }
    return first_arg;
}

Expression *Parser::_expression_OR()
{
    Expression *first_arg = _expression_AND();
    Expression *tmp;
    try {
        tmp = _operation_AND(first_arg);
        if (tmp != nullptr)
            return tmp;
    } catch (Error &err) { delete first_arg; throw; }
    return first_arg;
}

Expression *Parser::_expression()
{
    Expression *first_arg = _expression_OR();
    Expression *tmp;
    try {
        tmp = _operation_OR(first_arg);
        if (tmp != nullptr)
            return tmp;
    } catch (Error &err) { delete first_arg; throw; }
    return first_arg;
}

Branch *Parser::_branch()
{
    next_token();
    if (current_token.get_type() != TOKEN_LPAREN)
        throw Error("Syntax error: delim <(> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    Branch *tmp = new Branch;
    try {
        tmp->set_expression(_expression());
        if (current_token.get_type() != TOKEN_RPAREN)
            throw Error("Syntax error: delim <)> expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
        tmp->set_if_body(_statement());
        if (current_token.get_type() == TOKEN_ELSE) {
            next_token();
            tmp->set_else_body(_statement());
        }
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Loop *Parser::_while()
{
    next_token();
    if (current_token.get_type() != TOKEN_LPAREN)
        throw Error("Syntax error: delim <(> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    Loop *tmp = new Loop;
    try {
        tmp->set_expression(_expression());
        if (current_token.get_type() != TOKEN_RPAREN)
            throw Error("Syntax error: delim <)> expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
        tmp->set_body(_statement());
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Assignment *Parser::_assignment()
{
    Assignment *tmp = new Assignment(current_token.get_value());
    try {
        next_token();
        if (current_token.get_type() != TOKEN_ASSIGN)
            throw Error("Syntax error: identifier expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
        tmp->set_expression(_expression());
        if (current_token.get_type() != TOKEN_SEMICOL)
            throw Error("Syntax error: delim <;> expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Print *Parser::_print_statement()
{
    next_token();
    if (current_token.get_type() != TOKEN_DOT)
        throw Error("Syntax error: delim <.> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_OUT)
        throw Error("Syntax error: keyword <out> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_DOT)
        throw Error("Syntax error: delim <.> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_PRINTL)
        throw Error("Syntax error: keyword <println> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_LPAREN)
        throw Error("Syntax error: delim <(> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_STR)
        throw Error("Syntax error: string expected", SYNT_ANALYSIS, scanner.get_line());
    Print *tmp = new Print(current_token.get_value());
    try {
        next_token();
        if (current_token.get_type() != TOKEN_RPAREN)
            throw Error("Syntax error: delim <)> expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
        if (current_token.get_type() != TOKEN_SEMICOL)
            throw Error("Syntax error: delim <;> expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Statement *Parser::_statement()
{
    token_type tmp = current_token.get_type();
    switch (tmp) {
        case TOKEN_SEMICOL:
            next_token();
            return new Empty;
            break;
        case TOKEN_IF:
            return _branch();
            break;
        case TOKEN_WHILE:
            return _while();
            break;
        case TOKEN_ID:
            return _assignment();
            break;
        case TOKEN_SYS:
            return _print_statement();
            break;
        case TOKEN_LBLOCK:
            return _block();
            break;
        /*case TOKEN_INT:
            return _variable_declaration();
            break;
        case TOKEN_BOOL:
            return _variable_declaration();
            break;*/
        default:
            return nullptr;
    }
}

Var_Declaration *Parser::_variable_declaration()
{
    Var_Declaration *tmp = new Var_Declaration(current_token.get_type());
    try {
        next_token();
        if (current_token.get_type() != TOKEN_ID)
            throw Error ("Syntax error: identifier expected", SYNT_ANALYSIS, scanner.get_line());
        tmp->set_identifier(current_token.get_value());
        next_token();
        while (current_token.get_type() == TOKEN_COMMA) {
            next_token();
            if (current_token.get_type() != TOKEN_ID)
                throw Error ("Syntax error: identifier expected", SYNT_ANALYSIS, scanner.get_line());
            tmp->set_identifier(current_token.get_value());
            next_token();
        }
        if (current_token.get_type() != TOKEN_SEMICOL)
            throw Error ("Syntax error: delim <;> expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Statement *Parser::_block_statement()
{
    if ((current_token.get_type() == TOKEN_INT) || (current_token.get_type() == TOKEN_BOOL))
        return _variable_declaration();
    else
        return _statement();
}

Block *Parser::_block()
{
    if (current_token.get_type() != TOKEN_LBLOCK)
        throw Error ("Syntax error: delim <{> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    Block *tmp = new Block;
    try {
        Statement *stat = _block_statement();
        while (stat != nullptr) {
            tmp->add_statement(stat);
            stat = _block_statement();
        }
        if (current_token.get_type() != TOKEN_RBLOCK)
            throw Error ("Syntax error: missing delim <}> at the end of the block", SYNT_ANALYSIS, scanner.get_line());
        next_token();
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Main *Parser::_main_declaration()
{
    if (current_token.get_type() != TOKEN_PUBL)
        throw Error("Syntax error: keyword <public> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_STATIC)
        throw Error("Syntax error: keyword <static> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_VOID)
        throw Error("Syntax error: keyword <void> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_MAIN)
        throw Error("Syntax error: function name <main> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_LPAREN)
        throw Error("Syntax error: delim <(> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_RPAREN)
        throw Error("Syntax error: delim <)> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    Main *tmp = new Main;
    try {
        tmp->set_block(_block());
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

Node *Parser::_compilation_unit()
{
    if (current_token.get_type() != TOKEN_PUBL)
        throw Error("Syntax error: keyword <public> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_CLASS)
        throw Error("Syntax error: keyword <class> expected", SYNT_ANALYSIS, scanner.get_line());
    next_token();
    if (current_token.get_type() != TOKEN_ID)
        throw Error("Syntax error: class identifier expected", SYNT_ANALYSIS, scanner.get_line());
    Compilation_Unit *tmp = new Compilation_Unit(current_token.get_value());
    try {
        next_token();
        if (current_token.get_type() != TOKEN_LBLOCK)
            throw Error("Syntax error: delim <{> expected", SYNT_ANALYSIS, scanner.get_line());
        next_token();
        tmp->set_program(_main_declaration());
        if (current_token.get_type() != TOKEN_RBLOCK)
            throw Error("Syntax error: missing delim <}> at the end of the block", SYNT_ANALYSIS, scanner.get_line());
    } catch (Error &err) { delete tmp; throw; }
    return tmp;
}

#endif // _PARSER_
