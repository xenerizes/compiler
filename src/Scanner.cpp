#include "Scanner.h"

#ifndef _SCANNER_
#define _SCANNER_

Token Scanner::get_token()
{
    state = START;
    do {
        switch (state)
        {
        case START:
            current_token = "";
            current_token += symbol;
            if (isspace(symbol)) {
                if (symbol == '\n')
                    ++line;
                current_token = "";
                symbol = file.get();
            }
            else if (isalpha(symbol)) { symbol = file.get(); state = IDENT; }
            else if (isdigit(symbol)) { symbol = file.get(); state = NUMBER; }
            else if (symbol == '-') { symbol = file.get(); state = MINUS; }
            else if (symbol == '+') { symbol = file.get(); state = PLUS; }
            else if (symbol == '&') { symbol = file.get(); state = AND; }
            else if (symbol == '|') { symbol = file.get(); state = OR; }
            else if (symbol == '/') { symbol = file.get(); state = DIV; }
            else if (symbol == '"') { symbol = file.get(); state = STRING; }
            else if ((symbol == '>') || (symbol == '<') || (symbol == '!') || (symbol == '=')) { symbol = file.get(); state = EQU; }
            else { symbol = file.get(); state = DELIM; }
            break;
        case IDENT:
            if (isalpha(symbol) || (isdigit(symbol)) || (symbol == '_')) {
                current_token += symbol;
                symbol = file.get();
            }
            else return look_words(current_token);
            break;
        case NUMBER:
            if (isdigit(symbol)) {
                current_token += symbol;
                symbol = file.get();
            }
            else return to_number(current_token);
            break;
        case PLUS:
            if (symbol == '+') {
                current_token += symbol;
                symbol = file.get();
                return Token(TOKEN_INC, current_token);
            }
            else return Token(TOKEN_PLUS, current_token);
            break;
        case MINUS:
            if (symbol == '-') {
                current_token += symbol;
                symbol = file.get();
                return Token(TOKEN_DEC, current_token);
            }
            else return Token(TOKEN_MINUS, current_token);
            break;
        case EQU:
            if (symbol == '=') {
                current_token += symbol;
                symbol = file.get();
                return look_delimiters(current_token);
            }
            else return look_delimiters(current_token);
        case AND:
            if (symbol == '&') {
                current_token += symbol;
                symbol = file.get();
                return Token(TOKEN_AND, current_token);
            }
            else throw Error("Error in token: < & > - did you mean < && >?", LEX_ANALYSIS, line);
            break;
        case OR:
            if (symbol == '|') {
                current_token += symbol;
                symbol = file.get();
                return Token(TOKEN_OR, current_token);
            }
            else throw Error("Error in token: < | > - did you mean < || >?", LEX_ANALYSIS, line);
            break;
        case DIV:
            if (symbol == '/') {
                current_token += symbol;
                symbol = file.get();
                state = COMMENT;
            }
            else return Token(TOKEN_DIV, current_token);
            break;
        case COMMENT:
           /* if (symbol == '/') {
                current_token += symbol;
                symbol = file.get();
              //  if (symbol == '/') throw Error("Error in token: no nested comments enabled", LEX_ANALYSIS, line);
                current_token += symbol;
            }
            else*/ if ((symbol == '\n') || (symbol == EOF))
                return Token(TOKEN_COMMENT, current_token);
            else {
                current_token += symbol;
                symbol = file.get();
            }
            break;
        case STRING:
            if (symbol == '"') {
                current_token += symbol;
                symbol = file.get();
                return Token(TOKEN_STR, current_token);
            }
            else if ((symbol == '\n') || (symbol == EOF))
                throw Error("Error in token: missing terminating caracter in string", LEX_ANALYSIS, line);
            else {
                current_token += symbol;
                symbol = file.get();
            }
            break;
        case DELIM:
            return look_delimiters(current_token);
            break;
        }
    } while (symbol != EOF);
    return Token(TOKEN_FIN);
}

Token Scanner::look_delimiters(std::string token)
{
    if (table_delimiters.count(token))
        return Token(table_delimiters[token], token);
    else throw Error("Error in token: invalid combination or character", LEX_ANALYSIS, line);
}

Token Scanner::look_words(std::string token)
{
    if (table_words.count(token))
        return Token(table_words[token], token);
    else return Token(TOKEN_ID, token);
}

Token Scanner::to_number(std::string token)
{
    try {
        unsigned long number = std::stoul(token);
        if (number > max_num)
            throw Error("Error in token: number out of range", LEX_ANALYSIS, line);
        return Token(TOKEN_NUM, std::to_string(number));
    } catch (std::invalid_argument) { throw Error("Error in token: invalid number", LEX_ANALYSIS, line); }
    catch (std::out_of_range) { throw Error("Error in token: number out range", LEX_ANALYSIS, line); }
}

void Scanner::init_tables()
{
    init_pair_type keywords {pair_type("public", TOKEN_PUBL),
                pair_type("class", TOKEN_CLASS),
                pair_type("static", TOKEN_STATIC),
                pair_type("void", TOKEN_VOID),
                pair_type("main", TOKEN_MAIN),
                pair_type("if", TOKEN_IF),
                pair_type("else", TOKEN_ELSE),
                pair_type("while", TOKEN_WHILE),
                pair_type("System", TOKEN_SYS),
                pair_type("out", TOKEN_OUT),
                pair_type("println", TOKEN_PRINTL),
                pair_type("int", TOKEN_INT),
                pair_type("boolean", TOKEN_BOOL) };
    init_pair_type delimiters { pair_type("{", TOKEN_LBLOCK),
                pair_type("}", TOKEN_RBLOCK),
                pair_type("(", TOKEN_LPAREN),
                pair_type(")", TOKEN_RPAREN),
                pair_type(";", TOKEN_SEMICOL),
                pair_type(",", TOKEN_COMMA),
                pair_type(".", TOKEN_DOT),
                pair_type("++", TOKEN_INC),
                pair_type("--", TOKEN_DEC),
                pair_type("+", TOKEN_PLUS),
                pair_type("-", TOKEN_MINUS),
                pair_type("!", TOKEN_NOT),
                pair_type("*", TOKEN_MUL),
                pair_type("/", TOKEN_DIV),
                pair_type("%", TOKEN_MOD),
                pair_type("<=", TOKEN_LOEQ),
                pair_type(">=", TOKEN_GOEQ),
                pair_type("<", TOKEN_LEQ),
                pair_type(">", TOKEN_GEQ),
                pair_type("==", TOKEN_EQ),
                pair_type("!=", TOKEN_NEQ),
                pair_type("&&", TOKEN_AND),
                pair_type("||", TOKEN_OR),
                pair_type("=", TOKEN_ASSIGN) };
    table_words.insert(keywords);
    table_delimiters.insert(delimiters);
}

#endif // _SCANNER_
