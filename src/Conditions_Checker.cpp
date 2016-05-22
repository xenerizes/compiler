#include <exception>
#include "Conditions_Checker.h"
#include "Tree.h"
#include "Error.h"

void Conditions_Checker::visit(const Compilation_Unit &obj)
{
    size_t pos = prog.find_last_of('.');
    for (size_t i = prog.length() - 1; i > pos - 1; --i)
        prog.pop_back();
    pos = prog.find_last_of('/');
    for (size_t i = 0; i < pos + 1; ++i)
        prog.erase(0, 1);
    if (prog.compare(obj.get_name()))
        throw Error("Error: class identifier should be the same as program name", SEM_ANALYSIS);
    Main *tmp = obj.get_main();
    tmp->accept(*this);
}

void Conditions_Checker::visit(const Main &obj) { Block *tmp = obj.get_block(); tmp->accept(*this); }

void Conditions_Checker::visit(const Block &obj)
{
    typename Block::children_type children = obj.get_children();
    new_block();
    for (typename Block::children_type::size_type i = 0; i < children.size(); ++i) {
        children[i]->accept(*this);
    }
    prev_block();
}

void Conditions_Checker::visit(const Var_Declaration &obj)
{
    Var_Declaration::identifiers_type ids = obj.get_identifiers();
    for (typename Var_Declaration::identifiers_type::size_type i = 0; i < ids.size(); ++i) {
        if (!declare(ids[i], obj.get_type())) {
            std::string err = "Error: multiple declaration of variable <";
            err += ids[i];
            err += ">";
            throw Error(err, SEM_ANALYSIS);
        }
    }
}

void Conditions_Checker::visit(const Branch &obj)
{
    obj.get_expression()->accept(*this);
    if (is_current_integer())
        throw Error("Error: expression in branch should be of boolean type", SEM_ANALYSIS);
    new_block();
    obj.get_if_body()->accept(*this);
    prev_block();
    if (obj.has_else_body()) {
        new_block();
        obj.get_else_body()->accept(*this);
        prev_block();
    }
    current_op = TOKEN_NULL;
    field_op = "";
}

void Conditions_Checker::visit(const Loop &obj)
{
    obj.get_expression()->accept(*this);
    if (is_current_integer())
        throw Error("Error: expression in loop should be of boolean type", SEM_ANALYSIS);
    new_block();
    obj.get_body()->accept(*this);
    prev_block();
    current_op = TOKEN_NULL;
    field_op = "";
}

void Conditions_Checker::visit(const Assignment &obj)
{
    variable_type current_var = is_declared(obj.get_identifier());
    if (current_var == _NULL_TYPE)
        throw Error("Error: variable should be declared before assignment", SEM_ANALYSIS);
    obj.get_expression()->accept(*this);
    switch (current_var)
    {
    case _NULL_TYPE:
        break;
    case INT:
        if (is_current_boolean())
            throw Error("Error: boolean expression cannot be assigned to integer variable", SEM_ANALYSIS);
        break;
    case BOOLEAN:
        if (is_current_integer())
            throw Error("Error: integer expression cannot be assigned to boolean variable", SEM_ANALYSIS);
        break;
    }
    current_op = TOKEN_NULL;
    field_op = "";
}

void Conditions_Checker::visit(const Identifier &obj)
{
    if (is_declared(obj.get_name()) == _NULL_TYPE)
        throw Error("Error: variable should be declared before using", SEM_ANALYSIS);
    current_op = TOKEN_ID;
    field_op = obj.get_name();
}

void Conditions_Checker::visit(const Number &obj) { current_op = TOKEN_NUM; }

void Conditions_Checker::visit(const Operation_AND &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_integer())
        throw Error("Error: integer expression cannot be used in boolean operation <&&>", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_integer())
        throw Error("Error: integer expression cannot be used in boolean operation <&&>", SEM_ANALYSIS);
    current_op = TOKEN_AND;
}

void Conditions_Checker::visit(const Operation_DIV &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation </>", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation </>", SEM_ANALYSIS);
    current_op = TOKEN_DIV;
}

void Conditions_Checker::visit(const Operation_EQ &obj)
{
    obj.get_first_argument()->accept(*this);
    bool tmp = is_current_boolean();
    obj.get_second_argument()->accept(*this);
    if (tmp != is_current_boolean())
        throw Error("Error: only expressions of equivalent types can be compared with <==>", SEM_ANALYSIS);
    current_op = TOKEN_EQ;
}

void Conditions_Checker::visit(const Operation_GEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < > >", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < > >", SEM_ANALYSIS);
    current_op = TOKEN_GEQ;
}

void Conditions_Checker::visit(const Operation_GOEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < >= >", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < >= >", SEM_ANALYSIS);
    current_op = TOKEN_GOEQ;
}

void Conditions_Checker::visit(const Operation_LEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < < >", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < < >", SEM_ANALYSIS);
    current_op = TOKEN_LEQ;
}

void Conditions_Checker::visit(const Operation_LOEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < <= >", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in operation < <= >", SEM_ANALYSIS);
    current_op = TOKEN_LOEQ;
}

void Conditions_Checker::visit(const Operation_MINUS &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <->", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <->", SEM_ANALYSIS);
    current_op = TOKEN_MINUS;
}

void Conditions_Checker::visit(const Operation_MOD &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <%>", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <%>", SEM_ANALYSIS);
    current_op = TOKEN_MOD;
}

void Conditions_Checker::visit(const Operation_MUL &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <*>", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <*>", SEM_ANALYSIS);
    current_op = TOKEN_MUL;
}

void Conditions_Checker::visit(const Operation_NEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    bool tmp = is_current_boolean();
    obj.get_second_argument()->accept(*this);
    if (tmp != is_current_boolean())
        throw Error("Error: only expressions of equivalent types can be compared with <!=>", SEM_ANALYSIS);
    current_op = TOKEN_NEQ;
}

void Conditions_Checker::visit(const Operation_OR &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_integer())
        throw Error("Error: integer expression cannot be used in boolean operation <||>", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_integer())
        throw Error("Error: integer expression cannot be used in boolean operation <||>", SEM_ANALYSIS);
    current_op = TOKEN_OR;
}

void Conditions_Checker::visit(const Operation_PLUS &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <+>", SEM_ANALYSIS);
    obj.get_second_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <+>", SEM_ANALYSIS);
    current_op = TOKEN_PLUS;
}

void Conditions_Checker::visit(const Operation_NOT &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_integer() )
        throw Error("Error: integer expression cannot be used in boolean operation <!>", SEM_ANALYSIS);
   current_op = TOKEN_NOT;
}

void Conditions_Checker::visit(const Operation_POSTDEC &obj)
{
    obj.get_first_argument()->accept(*this);
    if (!good_inc_dec())
        throw Error("Error: only integer variable can be used in postfix decrement", SEM_ANALYSIS);
    current_op = TOKEN_DEC;
}

void Conditions_Checker::visit(const Operation_POSTINC &obj)
{
    obj.get_first_argument()->accept(*this);
    if (!good_inc_dec())
        throw Error("Error: only integer variable can be used in postfix increment", SEM_ANALYSIS);
    current_op = TOKEN_INC;
}

void Conditions_Checker::visit(const Operation_PREFDEC &obj)
{
    obj.get_first_argument()->accept(*this);
    if (!good_inc_dec())
        throw Error("Error: only integer variable can be used in prefix decrement", SEM_ANALYSIS);
    current_op = TOKEN_DEC;
}

void Conditions_Checker::visit(const Operation_PREFINC &obj)
{
    obj.get_first_argument()->accept(*this);
    if (!good_inc_dec())
        throw Error("Error: only integer variable can be used in prefix increment", SEM_ANALYSIS);
    current_op = TOKEN_INC;
}

void Conditions_Checker::visit(const Operation_UMINUS &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <->", SEM_ANALYSIS);
    current_op = TOKEN_MINUS;
}

void Conditions_Checker::visit(const Operation_UPLUS &obj)
{
    obj.get_first_argument()->accept(*this);
    if (is_current_boolean())
        throw Error("Error: boolean expression cannot be used in integer operation <+>", SEM_ANALYSIS);
    current_op = TOKEN_PLUS;
}

void Conditions_Checker::new_block()
{
    if (variables.begin() == variables.end()) {
        variables.resize(5);
        current_block = variables.begin();
        return;
    }
    if (++current_block == variables.end()) {
        variables.resize(variables.size() + 1);
        current_block = variables.end();
        --current_block;
    }
}

void Conditions_Checker::prev_block()
{
    if (variables.empty())
        return;
    if (*current_block == variables.front()) {
        variables.clear();
        current_block = variables.begin();
    }
    else {
        --current_block;
        variables.pop_back();
    }
}

variable_type Conditions_Checker::is_declared(std::string str) const
{
    if (variables.empty())
        return _NULL_TYPE;
    for (auto i = current_block; i != variables.begin(); --i) {
        if (i->empty())
            continue;
        if (i->count(str))
            return i->at(str);
    }
    if (variables.begin()->count(str))
        return variables.begin()->at(str);
    return _NULL_TYPE;
}

bool Conditions_Checker::declare(std::string str, variable_type var)
{
    if (current_block->count(str))
        return false;
    current_block->emplace(str, var);
    return true;
}

bool Conditions_Checker::is_current_boolean() const
{
    if ((current_op > TOKEN_MOD) && (current_op < TOKEN_ASSIGN))
        return true;
    if ((current_op == TOKEN_ID) && (is_declared(field_op) == BOOLEAN))
        return true;
    return false;
}

bool Conditions_Checker::is_current_integer() const
{
    if (((current_op > TOKEN_DOT) && (current_op < TOKEN_LOEQ)) || (current_op == TOKEN_NUM))
        return true;
    if ((current_op == TOKEN_ID) && (is_declared(field_op) == INT))
        return true;
    return false;
}

bool Conditions_Checker::good_inc_dec() const
{
    if ((current_op == TOKEN_ID) && (is_declared(field_op) == INT))
        return true;
    if ((current_op == TOKEN_INC) || (current_op == TOKEN_DEC))
        return true;
    return false;
}
