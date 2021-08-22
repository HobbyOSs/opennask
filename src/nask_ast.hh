#ifndef NASK_AST_HH_
#define NASK_AST_HH_

#include <iostream>
#include <string>
#include <list>

// FIXME: どこまで抽象クラスにしていいかわからないので作りながら修正

namespace ast {

    class ExpNode {
    public:
        virtual void print(){};
        virtual void evaluate(){};
    };

    class CmpNode : public ExpNode {
    public:
        CmpNode(std::string left, std::string cmp, std::string right) {};
        void print(){};
        void evaluate(){};
    };

    class CalcNode : public ExpNode {
    public:
        CalcNode(std::string left, std::string op, std::string right) {};
        void print(){};
        void evaluate(){};
    };

    class ImmediateNode : public ExpNode {
    public:
        ImmediateNode(std::string imm) {};
        void print(){};
        void evaluate(){};
    };

    class Statement {
    public:
        void print(){}
        void evaluate(){};
    };

    class DeclareStmt : public Statement {
    protected:
        std::string id;
        ExpNode *exp;
    public:
        DeclareStmt() {
        };
        DeclareStmt(std::string name, ExpNode exp) {
        };
        void print(){};
        void evaluate(){};
    };

    class ConfigStmt : public Statement {
    protected:
        std::string key;
        std::string value;
    public:
        ConfigStmt(){};
        ConfigStmt(std::string key, std::string value) {};
        void print(){};
        void evaluate(){};
    };

    class LabelStmt : public Statement {
    protected:
        std::string key;
        std::string value;
    public:
        LabelStmt(){};
        LabelStmt(std::string label) {};
        void print(){};
        void evaluate(){};
    };

    class MnemonicStmt : public Statement {
    protected:
        std::string opcode;
        std::list<ExpNode> args;
    public:
        MnemonicStmt(){};
        MnemonicStmt(std::string opcode) {};
        MnemonicStmt(std::string opcode, ExpNode arg) {};
        MnemonicStmt(std::string opcode, std::list<ExpNode> args) {};
        void print(){};
        void evaluate(){};
    };

    class Program {
    protected:
        std::list<Statement> stmts;
    public:
        Program(){};
        Program(std::list<Statement> stmtlist){
        };
        void evaluate();
    };
}

#endif /* NASK_AST_HH_ */
