#ifndef NASK_AST_HH_
#define NASK_AST_HH_

#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

namespace ast {

    class ExpNode {
    public:
        virtual std::string string(){ return "";};
        virtual void evaluate(){};
    };

    class Statement {
    public:
        virtual std::string string(){ return "";};
        virtual void evaluate(){};
    };

    class CmpNode : public ExpNode {
    protected:
        const std::string m_left;
        const std::string m_cmp;
        const std::string m_right;
    public:
        CmpNode(std::string left, std::string cmp, std::string right);
        std::string string();
        void evaluate();
    };

    class CalcNode : public ExpNode {
    protected:
        std::string m_left;
        std::string m_op;
        std::string m_right;
    public:
        CalcNode(std::string left, std::string op, std::string right);
        std::string string();
        void evaluate();
    };

    class ImmediateNode : public ExpNode {
    protected:
        std::string m_imm;
    public:
        ImmediateNode(std::string imm);
        std::string string();
        void evaluate();
    };


    class DeclareStmt : public Statement {
    protected:
        const std::string m_name;
        const ExpNode *m_exp;
    public:
        DeclareStmt();
        DeclareStmt(std::string name, ExpNode exp);
        std::string string();
        void evaluate();
    };

    class ConfigStmt : public Statement {
    protected:
        std::string key;
        std::string value;
    public:
        ConfigStmt();
        ConfigStmt(std::string key, std::string value);
        std::string string();
        void evaluate();
    };

    class LabelStmt : public Statement {
    protected:
        std::string key;
        std::string value;
    public:
        LabelStmt();
        LabelStmt(std::string label);
        std::string string();
        void evaluate();
    };

    class MnemonicStmt : public Statement {
    protected:
        std::string opcode;
        std::list<ExpNode> args;
    public:
        MnemonicStmt();
        MnemonicStmt(std::string opcode);
        MnemonicStmt(std::string opcode, ExpNode arg);
        MnemonicStmt(std::string opcode, std::list<ExpNode> args);
        std::string string();
        void evaluate();
    };

    class Program {
    protected:
        std::list<Statement> stmts;
    public:
        Program();
        Program(std::list<Statement> stmtlist);
        void evaluate();
    };
}

#endif /* NASK_AST_HH_ */
