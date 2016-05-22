#include <string>

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

enum step_type { INIT, LEX_ANALYSIS, SYNT_ANALYSIS, SEM_ANALYSIS, CODE_GENERATE };

class Error {
    public:
        Error(std::string reason, step_type step = INIT, long line = 0): err_reason(reason), err_step(step), err_line(line) {}

        const std::string &reason() const { return err_reason; }
        step_type step() const { return err_step; }
        long what_line() const { return err_line; }
    private:
        std::string err_reason;
        step_type err_step;
        long err_line;
};

#endif // ERROR_H_INCLUDED
