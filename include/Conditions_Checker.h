#include <list>
#include <map>
#include "Visitor.h"
#include "Tree.h"
#include "Token.h"

#ifndef CONDITIONS_CHECKER_H
#define CONDITIONS_CHECKER_H

class Conditions_Checker: public Visitor {
    public:
        Conditions_Checker(std::string name): prog(name), current_block(variables.begin()) {}

        void visit(const Compilation_Unit &);
        void visit(const Main &);
        void visit(const Block &);
        void visit(const Var_Declaration &);
        void visit(const Branch &);
        void visit(const Loop &);
        void visit(const Assignment &);
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
        void visit(const Operation_UPLUS &);
    private:
        typedef std::list<std::map<std::string, variable_type>> container_type;

        std::string prog;
        container_type variables;
        typename container_type::iterator current_block;
        token_type current_op;
        std::string field_op;

        void new_block();
        void prev_block();
        variable_type is_declared(std::string) const;
        bool declare(std::string, variable_type);
        bool is_current_boolean() const;
        bool is_current_integer() const;
        bool good_inc_dec() const;
};

#endif // CONDITIONS_CHECKER_H
