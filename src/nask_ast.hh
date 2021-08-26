#ifndef NASK_AST_HH_
#define NASK_AST_HH_

#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

namespace ast {

    // 先行class宣言
    class Program;
    class MnemonicStmt;
    class LabelStmt;
    class ConfigStmt;
    class DeclareStmt;
    class ImmediateNode;
    class CalcNode;
    class CmpNode;


    class Visitor {
        // 文や式の要素をを訪れる訪問者を表す抽象クラス
    public:
        virtual ~Visitor(){}
        virtual void visit(Program *p) = 0;
        virtual void visit(MnemonicStmt *p) = 0;
        virtual void visit(LabelStmt *p) = 0;
        virtual void visit(ConfigStmt *p) = 0;
        virtual void visit(DeclareStmt *p) = 0;
        virtual void visit(ImmediateNode *p) = 0;
        virtual void visit(CalcNode *p) = 0;
        virtual void visit(CmpNode *p) = 0;
    };

    class Visitable {
        // Visitorクラスのインスタンスを受け入れるデータ構造を表すインターフェース
    public:
        virtual ~Visitable() {}
        virtual void accept(Visitor *v) = 0;
    };

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

    class CmpNode : public ExpNode, public Visitable {
    protected:
        const std::string m_left;
        const std::string m_cmp;
        const std::string m_right;
    public:
        CmpNode(std::string left, std::string cmp, std::string right);
        std::string string();
        void evaluate();
        virtual CmpNode *clone() const = 0;
    };

    class CalcNode : public ExpNode, public Visitable {
    protected:
        std::string m_left;
        std::string m_op;
        std::string m_right;
    public:
        CalcNode(std::string left, std::string op, std::string right);
        std::string string();
        void evaluate();
        virtual CalcNode *clone() const = 0;
    };

    class ImmediateNode : public ExpNode, public Visitable {
    protected:
        std::string m_imm;
    public:
        ImmediateNode(std::string imm);
        std::string string();
        void evaluate();
        virtual ImmediateNode *clone() const = 0;
    };


    class DeclareStmt : public Statement, public Visitable {
    protected:
        const std::string m_name;
        const ExpNode *m_exp;
    public:
        DeclareStmt();
        DeclareStmt(std::string name, ExpNode exp);
        std::string string();
        void evaluate();
        virtual DeclareStmt *clone() const = 0;
    };

    class ConfigStmt : public Statement, public Visitable {
    protected:
        std::string key;
        std::string value;
    public:
        ConfigStmt();
        ConfigStmt(std::string key, std::string value);
        std::string string();
        void evaluate();
        virtual ConfigStmt *clone() const = 0;
    };

    class LabelStmt : public Statement, public Visitable {
    protected:
        std::string key;
        std::string value;
    public:
        LabelStmt();
        LabelStmt(std::string label);
        std::string string();
        void evaluate();
        virtual LabelStmt *clone() const = 0;
    };

    class MnemonicStmt : public Statement, public Visitable {
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
        virtual MnemonicStmt *clone() const = 0;
    };

    class Program: public Visitable {
    protected:
        std::list<Statement> stmts;
    public:
        Program();
        Program(std::list<Statement> stmtlist);
        void evaluate();
        Program *clone() const = 0;
    };


}

#endif /* NASK_AST_HH_ */
