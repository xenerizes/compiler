#include <list>
#include <vector>
#include <map>
#include <fstream>
#include "Visitor.h"
#include "Tree.h"

#ifndef CODE_GENERATOR_H_INCLUDED
#define CODE_GENERATOR_H_INCLUDED

class Code_Generator: public Visitor {
    public:
        Code_Generator(std::string o): free_reg(2), labels(0), top_of_stack("") { out.open(o); init_registers(); }
        ~Code_Generator() { out.close(); }

        void visit(const Compilation_Unit &);
        void visit(const Main &);
        void visit(const Block &);
        void visit(const Var_Declaration &);
        void visit(const Branch &);
        void visit(const Loop &);
        void visit(const Assignment &);
        void visit(const Print &);
        void visit(const Identifier &);
        void visit(const Number &);
        void visit(const Operation_AND &);
        void visit(const Operation_DIV &);
        void visit(const Operation_EQ &);
        void visit(const Operation_GEQ &);
        void visit(const Operation_GOEQ &);
        void visit(const Operation_LEQ &);
        void visit(const Operation_LOEQ &);
        void visit(const Operation_MINUS &);
        void visit(const Operation_MOD &);
        void visit(const Operation_MUL &);
        void visit(const Operation_NEQ &);
        void visit(const Operation_NOT &);
        void visit(const Operation_OR &);
        void visit(const Operation_PLUS &);
        void visit(const Operation_POSTDEC &);
        void visit(const Operation_POSTINC &);
        void visit(const Operation_PREFDEC &);
        void visit(const Operation_PREFINC &);
        void visit(const Operation_UMINUS &);
    private:
        typedef std::list<std::map<std::string, std::string>> container_type;
        typedef std::vector<std::string>::size_type registers_iterator_type;

        std::ofstream out;
        std::vector<std::string> strings_to_print;
        std::vector<std::string> registers;
        container_type variables;
        container_type::iterator current_block;
        registers_iterator_type free_reg;
        size_t labels;
        std::string top_of_stack;

        void new_block();
        void prev_block();
        void init_registers();
        void declare(std::string);
        std::string reserve_register();
        std::string find_register(std::string);
        std::string get_label(size_t);
        std::string hex_str(std::string);
};

#endif // CODE_GENERATOR_H_INCLUDED
