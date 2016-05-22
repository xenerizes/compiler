#include <string>
#include "Scanner.h"
#include "Parser.h"
#include "Conditions_Checker.h"

#ifndef MODE_H_INCLUDED
#define MODE_H_INCLUDED

class Mode {
    public:
        virtual ~Mode() {}

        virtual void run() = 0;
};

class Full_Analysis: public Mode {
    public:
        Full_Analysis(std::string name): program(name), parser(name) {}

        void run();
    private:
        std::string program;
        Parser parser;
};

class Full_Compiler: public Mode {
    public:
        Full_Compiler(std::string name, std::string o = "out"): program(name), parser(name), out(o) {}

        void run();
    private:
        std::string program;
        Parser parser;
        std::string out;
};

class Lex_Analysis: public Mode {
    public:
        Lex_Analysis(std::string filename): scanner(filename) {}

        void print_token();
        void run();
    private:
        Scanner scanner;
        Token current_token;
};


class Synt_Analysis: public Mode {
    public:
        Synt_Analysis(std::string name): parser(name) {}

        void run() { parser.start(); }
    private:
        Parser parser;
};

#endif // MODE_H_INCLUDED
