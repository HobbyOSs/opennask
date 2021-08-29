#include "nask_ast.hh"
#include "spdlog/spdlog.h"

namespace ast {

    // spdlog
    auto logger = spdlog::stdout_logger_mt("nask_parse");

    static const std::shared_ptr<spdlog::logger> log() {
        return spdlog::get("nask_parse");
    }



    Program::Program() {};
    Program::Program(std::list<Statement> stmtlist) {
        log()->debug("Program Parse stmtlist len:{}", stmtlist.size());
    };
    void Program::evaluate() {
        log()->debug("Program Eval IN:");
        log()->debug("Program Eval OUT:");
    };



    MnemonicStmt::MnemonicStmt() {};
    MnemonicStmt::MnemonicStmt(std::string opcode) {
        log()->debug("{} Parse opcode:", this->string());
    };
    MnemonicStmt::MnemonicStmt(std::string opcode, ExpNode arg) {
        log()->debug("{} Parse opcode:", this->string());
    };
    MnemonicStmt::MnemonicStmt(std::string opcode, std::list<ExpNode> args) {
        log()->debug("{} Parse opcode:", this->string());
    };
    std::string MnemonicStmt::string() {
        return demangle(typeid(this).name());
    };
    void MnemonicStmt::evaluate() {
        log()->debug("MnemonicStmt Eval IN:");
        log()->debug("MnemonicStmt Eval OUT:");
    };



    LabelStmt::LabelStmt() {};
    LabelStmt::LabelStmt(std::string label) {
        log()->debug("{} Parse opcode:", this->string());
    };
    std::string LabelStmt::string() {
        return demangle(typeid(this).name());
    };
    void LabelStmt::evaluate() {
        log()->debug("LabelStmt Eval IN:");
        log()->debug("LabelStmt Eval OUT:");
    };



    ConfigStmt::ConfigStmt() {};
    ConfigStmt::ConfigStmt(std::string key, std::string value) {
        log()->debug("{} Parse opcode:", this->string());
    };
    std::string ConfigStmt::string() {
        return demangle(typeid(this).name());
    };
    void ConfigStmt::evaluate() {
        log()->debug("ConfigStmt Eval IN:");
        log()->debug("ConfigStmt Eval OUT:");
    };



    DeclareStmt::DeclareStmt() {};
    DeclareStmt::DeclareStmt(std::string name, ExpNode exp):
        m_nane(name), m_exp(exp) {
        log()->debug("Parse {}", this->string());
    };
    std::string DeclareStmt::string() {
        std::stringstream buf;
        buf << "DeclareStmt{"
            << "name: " << this->m_name
            << "exp: "  << this->m_exp->string()
            << "}"
            << std::endl;
        return buf.str();
    };
    void DeclareStmt::evaluate() {
        log()->debug("DeclareStmt Eval IN:");
        log()->debug("DeclareStmt Eval OUT:");
    };



    ImmediateNode::ImmediateNode(std::string imm): m_imm(imm) {
        log()->debug("Parse {}", this->string());
    };
    std::string ImmediateNode::string() {
        std::stringstream buf;
        buf << "CalcNode{"
            << "imm: "  << this->m_imm
            << "}"
            << std::endl;
        return buf.str();
    };
    void ImmediateNode::evaluate() {
        log()->debug("ImmediateNode Eval IN:");
        log()->debug("ImmediateNode Eval OUT:");
    };



    CalcNode::CalcNode(std::string left, std::string op, std::string right):
        m_left(left), m_op(op), m_right(right) {
        log()->debug("Parse {}", this->string());
    };
    std::string CalcNode::string() {
        std::stringstream buf;
        buf << "CalcNode{"
            << "left: "  << this->m_left
            << "op: "    << this->m_op
            << "right: " << this->m_right
            << "}"
            << std::endl;
        return buf.str();
    };
    void CalcNode::evaluate() {
        log()->debug("CalcNode Eval IN:");
        log()->debug("CalcNode Eval OUT:");
    };



    CmpNode::CmpNode(std::string left, std::string cmp, std::string right):
        m_left(left), m_cmp(cmp), m_right(right) {
        log()->debug("Parse {}", this->string());
    };
    std::string CmpNode::string() {
        std::stringstream buf;
        buf << "CmpNode{"
            << "left: "  << this->m_left
            << "cmp: "   << this->m_cmp
            << "right: " << this->m_right
            << "}"
            << std::endl;
        return buf.str();
    };
    void CmpNode::evaluate() {
        log()->debug("CmpNode Eval IN:");
        log()->debug("CmpNode Eval OUT:");
    };
}
