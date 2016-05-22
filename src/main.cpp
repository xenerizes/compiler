#include <iostream>
#include "Compiler.h"
#include "Error.h"

int main(int argc, char *argv[])
{
    try {
        Compiler my_compiler = analyze(argc, argv);
        my_compiler.run();
    } catch (Error &err) {
        if ((err.step() == LEX_ANALYSIS) || (err.step() == SYNT_ANALYSIS))
            std::cerr << "[" << err.what_line() << ": ] ";
        std::cerr << err.reason() << '\n';
        return 1;
    }
    return 0;
}
