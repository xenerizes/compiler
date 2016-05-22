#include "Compiler.h"
#include "Error.h"
#include <string>

#ifndef _COMPILER_
#define _COMPILER_

Compiler analyze(int argc, char *argv[])
{
    if (argc < 2) throw Error("You should select a file to start compilation");
    std::string name = argv[1];
    const char *behavior_error = "Undetermined behavior: conflicting options";
    if (argc == 2)
        return Compiler(new Full_Compiler(name));
    std::string option = argv[2];
    if (!option.compare("-o")) {
        if (argc > 4)
            throw Error(behavior_error);
        if (argc == 3)
            throw Error("You should select a file for this option");
        return Compiler(new Full_Compiler(name, argv[3]));
    }
    if (!option.compare("-t")) {
        if (argc > 3) throw Error(behavior_error);
        return Compiler(new Lex_Analysis(name));
    }
    if (!option.compare("-p")) {
        if (argc > 3) throw Error(behavior_error);
        return Compiler(new Synt_Analysis(name));
    }
    if (!option.compare("-c")) {
        if (argc > 3) throw Error(behavior_error);
        return Compiler(new Full_Analysis(name));
    }
    throw Error("Incorrect options");
}

#endif // _COMPILER
