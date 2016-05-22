#include <vector>

#include "Visitor.h"
#include "Token.h"

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

enum variable_type { _NULL_TYPE, INT, BOOLEAN };

class Node {
    public:
        virtual ~Node() {}

        virtual void accept(Visitor &) const = 0;
        virtual Node *clone() const = 0;
};

class Tree {
    public:
        Tree(Node *node): root(node) {}
        Tree(const Tree &other): root(other.root->clone()) {}
        ~Tree() { delete root; }

        void accept(Visitor &v) const { root->accept(v); }
    private:
        Node *root;
        Tree &operator=(const Tree &);
};

class Expression: public Node {
    public:
        virtual ~Expression() {}

        virtual Expression *get_first_argument() const { return nullptr; }
        virtual Expression *get_second_argument() const { return nullptr; }
        virtual void set_first_argument(Expression *expr) {}
        virtual void set_second_argument(Expression *expr) {}

        Expression *clone() const { return nullptr; }
};

class Identifier: public Expression {
    public:
        Identifier(std::string id): name(id) {}

        std::string get_name() const { return name; }

        Identifier *clone() const { return new Identifier(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        std::string name;
        Identifier &operator=(const Identifier &);
};

class Number: public Expression {
    public:
        Number(std::string str) { num = std::stoul(str); }

        unsigned long get_number() const { return num; }

        Number *clone() const { return new Number(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        unsigned long num;
        Number &operator=(const Number &);
};

class Unary: public Expression {
    public:
        Unary(): argument(nullptr) {}
        Unary(const Unary &other) { if (other.argument != nullptr) argument = other.argument->clone(); }
        ~Unary() { delete argument; }

        void set_first_argument(Expression *expr) { argument = expr; }
        Expression *get_first_argument() const { return argument; }
    private:
        Expression *argument;
        Unary &operator=(const Unary &);
};

class Binary: public Expression {
    public:
        Binary(): arg_first(nullptr), arg_second(nullptr) {}
        Binary(const Binary &other)
        {
            if (other.arg_first != nullptr)
                arg_first = other.arg_first->clone();
            if (other.arg_second != nullptr)
                arg_second = other.arg_second->clone();
        }
        ~Binary() { delete arg_first; delete arg_second; }

        void set_first_argument(Expression *expr) { arg_first = expr; }
        void set_second_argument(Expression *expr) { arg_second = expr; }
        Expression *get_first_argument() const { return arg_first; }
        Expression *get_second_argument() const { return arg_second; }
    private:
        Expression *arg_first, *arg_second;
        Binary &operator=(const Binary &);
};

class Operation_OR: public Binary {
    public:
        Operation_OR *clone() const { return new Operation_OR(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_AND: public Binary {
    public:
        Operation_AND *clone() const { return new Operation_AND(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_EQ: public Binary {
    public:
        Operation_EQ *clone() const { return new Operation_EQ(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_NEQ: public Binary {
    public:
        Operation_NEQ *clone() const { return new Operation_NEQ(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_LOEQ: public Binary {
    public:
        Operation_LOEQ *clone() const { return new Operation_LOEQ(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_GOEQ: public Binary {
    public:
        Operation_GOEQ *clone() const { return new Operation_GOEQ(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_LEQ: public Binary {
    public:
        Operation_LEQ *clone() const { return new Operation_LEQ(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_GEQ: public Binary {
    public:
        Operation_GEQ *clone() const { return new Operation_GEQ(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_PLUS: public Binary {
    public:
        Operation_PLUS *clone() const { return new Operation_PLUS(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_MINUS: public Binary {
    public:
        Operation_MINUS *clone() const { return new Operation_MINUS(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_MUL: public Binary {
    public:
        Operation_MUL *clone() const { return new Operation_MUL(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_DIV: public Binary {
    public:
        Operation_DIV *clone() const { return new Operation_DIV(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_MOD: public Binary {
    public:
        Operation_MOD *clone() const { return new Operation_MOD(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_NOT: public Unary {
    public:
        Operation_NOT *clone() const { return new Operation_NOT(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_UMINUS: public Unary {
    public:
        Operation_UMINUS *clone() const { return new Operation_UMINUS(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_UPLUS: public Unary {
    public:
        Operation_UPLUS *clone() const { return new Operation_UPLUS(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_PREFINC: public Unary {
    public:
        Operation_PREFINC *clone() const { return new Operation_PREFINC(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_PREFDEC: public Unary {
    public:
        Operation_PREFDEC *clone() const { return new Operation_PREFDEC(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_POSTINC: public Unary {
    public:
        Operation_POSTINC *clone() const { return new Operation_POSTINC(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Operation_POSTDEC: public Unary {
    public:
        Operation_POSTDEC *clone() const { return new Operation_POSTDEC(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Statement: public Node {
    public:
        virtual ~Statement() {}

        Statement *clone() const { return nullptr; }
};

class Var_Declaration: public Statement {
    public:
        typedef std::vector<std::string> identifiers_type;

        Var_Declaration(token_type t)
        {
            if (t == TOKEN_INT)
                type = INT;
            if (t == TOKEN_BOOL)
                type = BOOLEAN;
        }

        void set_identifier(std::string id) { identifiers.push_back(id); }
        variable_type get_type() const { return type; }
        std::vector<std::string> get_identifiers() const { return identifiers; }

        Var_Declaration *clone() const { return new Var_Declaration(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        variable_type type;
        identifiers_type identifiers;
};

class Empty: public Statement {
    public:
        Empty() {}

        Empty *clone() const { return new Empty; }
        void accept(Visitor &v) const { v.visit(*this); }
};

class Branch: public Statement {
    public:
        Branch(): expr(nullptr), if_body(nullptr), else_body(nullptr) {}
        Branch(const Branch &other)
        {
            if (other.expr != nullptr)
               expr = other.expr->clone();
            if (other.if_body != nullptr)
               if_body = other.if_body->clone();
            if (other.else_body != nullptr)
               else_body = other.else_body->clone();
        }
        ~Branch() { delete expr; delete if_body; delete else_body; }

        void set_expression(Expression *ex) { expr = ex; }
        void set_if_body(Statement *stat) { if_body = stat; }
        void set_else_body(Statement *stat) { else_body = stat; }
        Expression *get_expression() const { return expr; }
        Statement *get_if_body() const { return if_body; }
        Statement *get_else_body() const { return else_body; }
        bool has_else_body() const { if (else_body != nullptr) return true; return false; }

        Branch *clone() const { return new Branch(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        Expression *expr;
        Statement *if_body, *else_body;
        Branch &operator=(const Branch &);
};

class Loop: public Statement {
    public:
        Loop(): expr(nullptr), body(nullptr) {}
        Loop(const Loop &other)
        {
            if (other.expr != nullptr)
                expr = other.expr->clone();
            if (other.body != nullptr)
                body = other.body->clone();
        }
        ~Loop() { delete expr; delete body; }

        void set_expression(Expression *ex) { expr = ex; }
        void set_body(Statement *stat) { body = stat; }
        Expression *get_expression() const { return expr; }
        Statement *get_body() const { return body; }
        bool has_body() { if (body != nullptr) return true; return false; }

        Loop *clone() const { return new Loop(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        Expression *expr;
        Statement *body;
        Loop &operator=(const Loop &);
};

class Assignment: public Statement {
    public:
        Assignment(std::string name): id(name), expr(nullptr) {}
        Assignment(const Assignment &other) { if (other.expr != nullptr) expr = other.expr->clone(); }
        ~Assignment() { delete expr; }

        void set_expression(Expression *ex) { expr = ex; }
        Expression *get_expression() const { return expr; }
        std::string get_identifier() const { return id; }

        Assignment *clone() { return new Assignment(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        std::string id;
        Expression *expr;
        Assignment &operator=(const Assignment &);
};

class Print: public Statement {
    public:
        Print(std::string s): str(s) { str.erase(0, 1); str.pop_back(); }

        const std::string &get_string() const { return str; }

        Print *clone() const { return new Print(str); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        std::string str;
};

class Block: public Statement {
    public:
        typedef std::vector<Statement *> children_type;

        Block(): statements(children_type()) {}
        Block(const Block &other)
        {
            for (children_type::size_type i = 0; i < other.statements.size(); ++i) {
                statements.push_back(other.statements[i]->clone());
            }
        }
        ~Block()
        {
            for (children_type::size_type i = 0; i < statements.size(); ++i) {
                Statement *tmp = statements[i];
                statements[i] = nullptr;
                delete tmp;
            }
            statements.clear();
        }

        void add_statement(Statement *stat) { statements.push_back(stat); }
        children_type get_children() const { return statements; }

        Block *clone() const { return new Block(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        children_type statements;
        Block &operator=(const Block &);
};

class Main: public Node {
    public:
        Main(): body(nullptr) {}
        Main(const Main &other) { if (other.body != nullptr) body = other.body->clone(); }
        ~Main() { delete body; }

        void set_block(Block *newbody) { body = newbody; }
        Block *get_block() const { return body; }

        Main *clone() const { return new Main(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
     private:
        Block *body;
        Main &operator=(const Main &);
};

class Compilation_Unit: public Node {
    public:
        Compilation_Unit(std::string id): program(nullptr), name(id) {}
        Compilation_Unit(const Compilation_Unit &other): name(other.name) { if (other.program != nullptr) program = other.program->clone(); }
        ~Compilation_Unit() { delete program; }

        void set_program(Main *newmain) { program = newmain; }
        Main *get_main() const { return program; }
        std::string get_name() const { return name; }

        Compilation_Unit *clone() const { return new Compilation_Unit(*this); }
        void accept(Visitor &v) const { v.visit(*this); }
    private:
        Main *program;
        std::string name;
        Compilation_Unit &operator=(const Compilation_Unit &);
};

#endif // TREE_H_INCLUDED
