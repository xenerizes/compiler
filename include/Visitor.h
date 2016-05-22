#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED
class Tree;
class Compilation_Unit;
class Main;
class Statement;
class Block;
class Var_Declaration;
class Empty;
class Branch;
class Loop;
class Assignment;
class Print;
class Identifier;
class Number;
class Operation_AND;
class Operation_DIV;
class Operation_EQ;
class Operation_GEQ;
class Operation_GOEQ;
class Operation_LEQ;
class Operation_LOEQ;
class Operation_MINUS;
class Operation_MOD;
class Operation_MUL;
class Operation_NEQ;
class Operation_NOT;
class Operation_OR;
class Operation_PLUS;
class Operation_POSTDEC;
class Operation_POSTINC;
class Operation_PREFDEC;
class Operation_PREFINC;
class Operation_UMINUS;
class Operation_UMINUS;
class Operation_UPLUS;

class Visitor {
    public:
        Visitor() {}

        virtual void visit(const Tree &) {}
        virtual void visit(const Compilation_Unit &) {}
        virtual void visit(const Main &) {}
        virtual void visit(const Statement &) {}
        virtual void visit(const Block &) {}
        virtual void visit(const Var_Declaration &) {}
        virtual void visit(const Empty &) {}
        virtual void visit(const Branch &) {}
        virtual void visit(const Loop &) {}
        virtual void visit(const Assignment &) {}
        virtual void visit(const Print &) {}
        virtual void visit(const Identifier &) {}
        virtual void visit(const Number &) {}
        virtual void visit(const Operation_AND &) {}
        virtual void visit(const Operation_DIV &) {}
        virtual void visit(const Operation_EQ &) {}
        virtual void visit(const Operation_GEQ &) {}
        virtual void visit(const Operation_GOEQ &) {}
        virtual void visit(const Operation_LEQ &) {}
        virtual void visit(const Operation_LOEQ &) {}
        virtual void visit(const Operation_MINUS &) {}
        virtual void visit(const Operation_MOD &) {}
        virtual void visit(const Operation_MUL &) {}
        virtual void visit(const Operation_NEQ &) {}
        virtual void visit(const Operation_NOT &) {}
        virtual void visit(const Operation_OR &) {}
        virtual void visit(const Operation_PLUS &) {}
        virtual void visit(const Operation_POSTDEC &) {}
        virtual void visit(const Operation_POSTINC &) {}
        virtual void visit(const Operation_PREFDEC &) {}
        virtual void visit(const Operation_PREFINC &) {}
        virtual void visit(const Operation_UMINUS &) {}
        virtual void visit(const Operation_UPLUS &) {}
};

#endif // VISITOR_H_INCLUDED
