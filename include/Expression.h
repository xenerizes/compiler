#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

class Expression: public Node {
    public:
        virtual ~Expression();
};

class Unary: public Expression {
    public:
        Unary(): argument(nullptr) {}
        Unary(const Unary &other) { if (other.argument != nullptr) argument = other.argument->clone(); }
        ~Unary() { delete argument; }

        void set_argument(Expression *expr) { argument = expr; }
        const Expression *get_argument() const { return *argument; }
    private:
        Expression *argument;
        Unary &operator=(const Unary &);
};

class Binary: public Expression {
    public:
        Binary(): argument(nullptr) {}
        Binary(const Unary &other) { if (other.argument != nullptr) argument = other.argument->clone(); }
        ~Binary() { delete argument; }

        void set_argument(Expression *expr) { argument = expr; }
        const Expression *get_argument() const { return *argument; }
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

#endif // EXPRESSION_H_INCLUDED
