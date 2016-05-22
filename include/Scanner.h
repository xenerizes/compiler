#include <fstream>
#include <string>
#include <map>
#include <initializer_list>
#include <cctype>
#include <cstdlib>
#include "Error.h"
#include "Token.h"

#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

class Scanner {
    public:
        typedef unsigned long line_type;

        Scanner(std::string filename): file(filename), line(1), max_num(2147483647) { symbol = file.get(); init_tables(); }

        Token get_token();

        line_type get_line() const { return line; }
    private:
        void init_tables();
        enum state_type { START, IDENT, NUMBER, PLUS, MINUS, EQU, AND, OR, DIV, COMMENT, STRING, DELIM };
        typedef std::map<std::string, token_type>::value_type pair_type;
        typedef std::initializer_list<pair_type> init_pair_type;
        std::map<std::string, token_type> table_words, table_delimiters;
        std::fstream file;
        state_type state;
        int symbol;
        std::string current_token;
        line_type line;
        unsigned long max_num;

        Token look_delimiters(std::string);
        Token look_words(std::string);
        Token to_number(std::string);
};

#endif // SCANNER_H_INCLUDED
