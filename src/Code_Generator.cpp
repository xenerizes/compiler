#include <sstream>
#include "Code_Generator.h"

void Code_Generator::visit(const Compilation_Unit &obj)
{
    out << '\t' << ".text" << '\n';
    out << '\t' << "calla main" << '\n';
    out << '\t' << "halt" << '\n';
    Main *tmp = obj.get_main();
    tmp->accept(*this);
    out << '\t' << ".rodata" << '\n';
    for (typename std::vector<std::string>::size_type i = 0; i < strings_to_print.size(); ++i) {
        out << "str" << i + 1 << ':';
        out << '\t' << ".b " << hex_str(strings_to_print[i]) << '\n';
    }
}

void Code_Generator::visit(const Main &obj)
{
    out << '\t' << ".globl main" << '\n';
    out << "main:" << '\n';
    Block *tmp = obj.get_block();
    tmp->accept(*this);
    out << '\t' << "ret" << '\n';
}

void Code_Generator::visit(const Block &obj)
{
    typename Block::children_type children = obj.get_children();
    new_block();
    for (typename Block::children_type::size_type i = 0; i < children.size(); ++i) {
        children[i]->accept(*this);
    }
    prev_block();
}

void Code_Generator::visit(const Var_Declaration &obj)
{
    Var_Declaration::identifiers_type ids = obj.get_identifiers();
    for (typename Var_Declaration::identifiers_type::size_type i = 0; i < ids.size(); ++i) {
        declare(ids[i]);
    }
}

void Code_Generator::visit(const Branch &obj)
{
    obj.get_expression()->accept(*this);
    size_t label1 = labels++;
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "tsti " << registers[free_reg] << '\n';
    out << '\t' << "jz " << get_label(label1) << '\n';
    obj.get_if_body()->accept(*this);
    if (obj.has_else_body()) {
        size_t label2 = labels++;
        out << '\t' << "jmpa " << get_label(label2) << '\n';
        out << get_label(label1) << ':' << '\n';
        obj.get_else_body()->accept(*this);
        out << get_label(label2) << ':' << '\n';
    } else
        out << get_label(label1) << ':' << '\n';
}

void Code_Generator::visit(const Loop &obj)
{
    size_t label1 = labels++;
    size_t label2 = labels++;
    out << get_label(label1) << ':' << '\n';
    obj.get_expression()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "tsti " << registers[free_reg] << '\n';
    out << '\t' << "jz " << get_label(label2) << '\n';
    top_of_stack = "";
    obj.get_body()->accept(*this);
    out << '\t' << "jmpa " << get_label(label1) << '\n';
    out << get_label(label2) << ':' << '\n';
}

void Code_Generator::visit(const Assignment &obj)
{
    obj.get_expression()->accept(*this);
    std::string reg = find_register(obj.get_identifier());
    out << '\t' << "popi " << reg << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Print &obj)
{
    strings_to_print.push_back(obj.get_string());
    out << '\t' << "ea " << registers[free_reg] << ", str" << strings_to_print.size() << '\n';
    out << '\t' << "sys2 0x03, " << registers[free_reg] << ", " << registers[free_reg + 1] << '\n';
}

void Code_Generator::visit(const Identifier &obj)
{
    out << '\t' << "pushi " << find_register(obj.get_name()) << '\n';
    top_of_stack = find_register(obj.get_name());
}

void Code_Generator::visit(const Number &obj)
{
    out << '\t' << "lpi " << registers[free_reg] << ", " << obj.get_number() << '\n';
    out << '\t' << "pushi " << registers[free_reg] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_DIV &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "divi " << registers[free_reg + 2] << ", " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_EQ &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "lpi " << registers[free_reg + 2] << ", 0" << '\n';
    out << '\t' << "cmpi " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "jnz " << get_label(labels) << '\n';
    out << '\t' << "noti " << registers[free_reg + 2] << ", " << registers[free_reg + 2] << '\n';
    out << get_label(labels++) << ':'  << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_GEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "lpi " << registers[free_reg + 2] << ", 0" << '\n';
    out << '\t' << "cmpi " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "jle " << get_label(labels) << '\n';
    out << '\t' << "noti " << registers[free_reg + 2] << ", " << registers[free_reg + 2] << '\n';
    out << get_label(labels++) << ':' << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_GOEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "lpi " << registers[free_reg + 2] << ", 0" << '\n';
    out << '\t' << "cmpi " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "jl " << get_label(labels) << '\n';
    out << '\t' << "noti " << registers[free_reg + 2] << ", " << registers[free_reg + 2] << '\n';
    out << get_label(labels++) << ':' << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_LEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "lpi " << registers[free_reg + 2] << ", 0" << '\n';
    out << '\t' << "cmpi " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "jge " << get_label(labels) << '\n';
    out << '\t' << "noti " << registers[free_reg + 2] << ", " << registers[free_reg + 2] << '\n';
    out << get_label(labels++) << ':'  << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_LOEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "lpi " << registers[free_reg + 2] << ", 0" << '\n';
    out << '\t' << "cmpi " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "jg " << get_label(labels) << '\n';
    out << '\t' << "noti " << registers[free_reg + 2] << ", " << registers[free_reg + 2] << '\n';
    out << get_label(labels++) << ':' << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_MINUS &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "subi " << registers[free_reg + 2] << ", " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "erro " << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_MOD &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "modi " << registers[free_reg + 2] << ", " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_MUL &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "muli " << registers[free_reg + 2] << ", " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "erro " << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_NEQ &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "lpi " << registers[free_reg + 2] << ", 0" << '\n';
    out << '\t' << "cmpi " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "jz " << get_label(labels) << '\n';
    out << '\t' << "noti " << registers[free_reg + 2] << ", " << registers[free_reg + 2] << '\n';
    out << get_label(labels++) << ':' << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_NOT &obj)
{
    obj.get_first_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "noti " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "pushi " << registers[free_reg + 1] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_AND &obj)
{
    obj.get_first_argument()->accept(*this);
    size_t label = labels++;
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "tsti " << registers[free_reg] << '\n';
    out << '\t' << "jz " << get_label(label) << '\n';
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << get_label(label) << ':' << '\n';
    out << '\t' << "pushi " << registers[free_reg] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_OR &obj)
{
    obj.get_first_argument()->accept(*this);
    size_t label = labels++;
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "tsti " << registers[free_reg] << '\n';
    out << '\t' << "jnz " << get_label(labels) << '\n';
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << get_label(label) << ':' << '\n';
    out << '\t' << "pushi " << registers[free_reg] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_PLUS &obj)
{
    obj.get_first_argument()->accept(*this);
    obj.get_second_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "popi " << registers[free_reg + 1] << '\n';
    out << '\t' << "addi " << registers[free_reg + 2] << ", " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "erro " << '\n';
    out << '\t' << "pushi " << registers[free_reg + 2] << '\n';
    top_of_stack = "";
}

void Code_Generator::visit(const Operation_POSTDEC &obj)
{
    obj.get_first_argument()->accept(*this);
    out << '\t' << "deci " << top_of_stack << ", " << top_of_stack << '\n';
    out << '\t' << "erro " << '\n';
}

void Code_Generator::visit(const Operation_POSTINC &obj)
{
    obj.get_first_argument()->accept(*this);
    out << '\t' << "inci " << top_of_stack << ", " << top_of_stack << '\n';
    out << '\t' << "erro " << '\n';
}

void Code_Generator::visit(const Operation_PREFDEC &obj)
{
    obj.get_first_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "deci " << top_of_stack << ", " << top_of_stack << '\n';
    out << '\t' << "erro " << '\n';
    out << '\t' << "pushi " << top_of_stack << '\n';
}

void Code_Generator::visit(const Operation_PREFINC &obj)
{
    obj.get_first_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "inci " << top_of_stack << ", " << top_of_stack << '\n';
    out << '\t' << "erro " << '\n';
    out << '\t' << "pushi " << top_of_stack << '\n';
}

void Code_Generator::visit(const Operation_UMINUS &obj)
{
    obj.get_first_argument()->accept(*this);
    out << '\t' << "popi " << registers[free_reg] << '\n';
    out << '\t' << "negi " << registers[free_reg + 1] << ", " << registers[free_reg] << '\n';
    out << '\t' << "pushi " << registers[free_reg + 1] << '\n';
    top_of_stack = "";
}

void Code_Generator::init_registers()
{
    for (size_t i = 2; i < 252; ++i) {
        std::string tmp = "%";
        tmp.append(std::to_string(i));
        registers.push_back(tmp);
    }
}

void Code_Generator::new_block()
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

void Code_Generator::prev_block()
{
    if (variables.empty())
        return;
    if (*current_block == variables.front()) {
        free_reg -= current_block->size();
        free_reg++;
        variables.clear();
        current_block = variables.begin();
    }
    else {
        free_reg -= current_block->size();
        free_reg++;
        --current_block;
        variables.pop_back();
    }

}

void Code_Generator::declare(std::string str)
{
    std::string tmp = reserve_register();
    current_block->emplace(str, tmp);
}

std::string Code_Generator::reserve_register()
{
    return registers[free_reg++];
}

std::string Code_Generator::find_register(std::string str)
{
    for (auto i = variables.rbegin(); i != variables.rend(); ++i) {
        if (i->count(str))
            return i->at(str);
    }
    return "";
}

std::string Code_Generator::get_label(size_t num)
{
    std::string tmp = "L";
    tmp.append(std::to_string(num));
    return tmp;
}

std::string Code_Generator::hex_str(std::string str)
{
    std::stringstream tmp;
    tmp.flags(std::ios::hex | std::ios::showbase);
    for (std::string::size_type i = 0; i != str.size(); ++i)
        tmp << (int)str[i] << ", ";
    tmp << "0x0a, 0";
    return tmp.str();
}
