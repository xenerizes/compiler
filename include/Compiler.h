#include "Mode.h"
#include "Error.h"

#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

class Compiler {
    public:
        Compiler(Mode *mode): compiler_mode(mode) {}
        Compiler(Compiler &&other)
        {
            compiler_mode = other.compiler_mode;
            other.compiler_mode = nullptr;
        }
        ~Compiler() { delete compiler_mode; }

        void run() const { compiler_mode->run(); }
    private:
        Mode *compiler_mode;
};

Compiler analyze(int argc, char *argv[]);

#endif // COMPILER_H_INCLUDED
