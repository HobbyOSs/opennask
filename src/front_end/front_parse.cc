#include <fstream>
#include <typeinfo>
#include <type_traits>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "matchit.h"
#include "front_end.hh"
#include "driver.hh"
#include "parser.hh"
#include "demangle.hpp"
#include "mod_rm.hpp"


using namespace std::placeholders;


void FrontEnd::define_funcs() {
    this->funcs = {
        // 引数なしのオペコード
        std::make_pair("OpcodesAAA", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdAaa>, this)),
        std::make_pair("OpcodesAAS", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdAas>, this)),
        std::make_pair("OpcodesCBW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCbw>, this)),
        std::make_pair("OpcodesCDQ", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCdq>, this)),
        std::make_pair("OpcodesCLC", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdClc>, this)),
        std::make_pair("OpcodesCLD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCld>, this)),
        std::make_pair("OpcodesCLI", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCli>, this)),
        std::make_pair("OpcodesCLTS", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdClts>, this)),
        std::make_pair("OpcodesCMC", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCmc>, this)),
        //std::make_pair("OpcodesCMPSB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCmpsb>, this)),
        //std::make_pair("OpcodesCMPSD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCmpsd>, this)),
        //std::make_pair("OpcodesCMPSW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCmpsw>, this)),
        std::make_pair("OpcodesCWD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCwd>, this)),
        std::make_pair("OpcodesCWDE", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdCwde>, this)),
        std::make_pair("OpcodesDAA", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdDaa>, this)),
        std::make_pair("OpcodesDAS", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdDas>, this)),
        std::make_pair("OpcodesF2XM1", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdF2xm1>, this)),
        std::make_pair("OpcodesFABS", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFabs>, this)),
        std::make_pair("OpcodesFCHS", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFchs>, this)),
        std::make_pair("OpcodesFCLEX", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFclex>, this)),
        std::make_pair("OpcodesFCOMPP", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFcompp>, this)),
        std::make_pair("OpcodesFCOS", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFcos>, this)),
        std::make_pair("OpcodesFDECSTP", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFdecstp>, this)),
        //std::make_pair("OpcodesFDISI", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFdisi>, this)),
        //std::make_pair("OpcodesFENI", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFeni>, this)),
        std::make_pair("OpcodesFINCSTP", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFincstp>, this)),
        std::make_pair("OpcodesFINIT", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFinit>, this)),
        std::make_pair("OpcodesFLD1", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFld1>, this)),
        std::make_pair("OpcodesFLDL2E", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFldl2e>, this)),
        std::make_pair("OpcodesFLDL2T", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFldl2t>, this)),
        std::make_pair("OpcodesFLDLG2", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFldlg2>, this)),
        std::make_pair("OpcodesFLDLN2", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFldln2>, this)),
        std::make_pair("OpcodesFLDPI", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFldpi>, this)),
        std::make_pair("OpcodesFLDZ", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFldz>, this)),
        std::make_pair("OpcodesFNCLEX", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFnclex>, this)),
        //std::make_pair("OpcodesFNDISI", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFndisi>, this)),
        //std::make_pair("OpcodesFNENI", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFneni>, this)),
        std::make_pair("OpcodesFNINIT", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFninit>, this)),
        std::make_pair("OpcodesFNOP", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFnop>, this)),
        std::make_pair("OpcodesFPATAN", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFpatan>, this)),
        std::make_pair("OpcodesFPTAN", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFptan>, this)),
        std::make_pair("OpcodesFPREM", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFprem>, this)),
        std::make_pair("OpcodesFPREM1", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFprem1>, this)),
        std::make_pair("OpcodesFRNDINT", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFrndint>, this)),
        std::make_pair("OpcodesFSCALE", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFscale>, this)),
        //std::make_pair("OpcodesFSETPM", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFsetpm>, this)),
        std::make_pair("OpcodesFSIN", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFsin>, this)),
        std::make_pair("OpcodesFSINCOS", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFsincos>, this)),
        std::make_pair("OpcodesFSQRT", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFsqrt>, this)),
        std::make_pair("OpcodesFTST", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFtst>, this)),
        std::make_pair("OpcodesFUCOMPP", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFucompp>, this)),
        std::make_pair("OpcodesFXAM", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFxam>, this)),
        std::make_pair("OpcodesFXTRACT", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFxtract>, this)),
        std::make_pair("OpcodesFYL2X", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFyl2x>, this)),
        std::make_pair("OpcodesFYL2XP1", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdFyl2xp1>, this)),
        std::make_pair("OpcodesHLT", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdHlt>, this)),
        //std::make_pair("OpcodesINSB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdInsb>, this)),
        //std::make_pair("OpcodesINSD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdInsd>, this)),
        //std::make_pair("OpcodesINSW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdInsw>, this)),
        std::make_pair("OpcodesINT3", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdInt3>, this)),
        std::make_pair("OpcodesINTO", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdInto>, this)),
        std::make_pair("OpcodesINVD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdInvd>, this)),
        std::make_pair("OpcodesIRET", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdIret>, this)),
        std::make_pair("OpcodesIRETD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdIretd>, this)),
        //std::make_pair("OpcodesIRETW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdIretw>, this)),
        std::make_pair("OpcodesLAHF", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdLahf>, this)),
        std::make_pair("OpcodesLEAVE", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdLeave>, this)),
        //std::make_pair("OpcodesLODSB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdLodsb>, this)),
        //std::make_pair("OpcodesLODSD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdLodsd>, this)),
        //std::make_pair("OpcodesLODSW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdLodsw>, this)),
        //std::make_pair("OpcodesMOVSB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdMovsb>, this)),
        std::make_pair("OpcodesMOVSD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdMovsd>, this)),
        //std::make_pair("OpcodesMOVSW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdMovsw>, this)),
        std::make_pair("OpcodesNOP", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdNop>, this)),
        //std::make_pair("OpcodesOUTSB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdOutsb>, this)),
        //std::make_pair("OpcodesOUTSD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdOutsd>, this)),
        //std::make_pair("OpcodesOUTSW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdOutsw>, this)),
        std::make_pair("OpcodesPOPA", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPopa>, this)),
        std::make_pair("OpcodesPOPAD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPopad>, this)),
        //std::make_pair("OpcodesPOPAW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPopaw>, this)),
        std::make_pair("OpcodesPOPF", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPopf>, this)),
        std::make_pair("OpcodesPOPFD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPopfd>, this)),
        //std::make_pair("OpcodesPOPFW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPopfw>, this)),
        std::make_pair("OpcodesPUSHA", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPusha>, this)),
        std::make_pair("OpcodesPUSHAD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPushad>, this)),
        //std::make_pair("OpcodesPUSHAW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPushaw>, this)),
        std::make_pair("OpcodesPUSHF", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPushf>, this)),
        std::make_pair("OpcodesPUSHFD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPushfd>, this)),
        //std::make_pair("OpcodesPUSHFW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdPushfw>, this)),
        std::make_pair("OpcodesSAHF", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdSahf>, this)),
        //std::make_pair("OpcodesSCASB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdScasb>, this)),
        //std::make_pair("OpcodesSCASD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdScasd>, this)),
        //std::make_pair("OpcodesSCASW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdScasw>, this)),
        std::make_pair("OpcodesSTC", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdStc>, this)),
        std::make_pair("OpcodesSTD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdStd>, this)),
        std::make_pair("OpcodesSTI", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdSti>, this)),
        //std::make_pair("OpcodesSTOSB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdStosb>, this)),
        //std::make_pair("OpcodesSTOSD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdStosd>, this)),
        //std::make_pair("OpcodesSTOSW", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdStosw>, this)),
        //std::make_pair("OpcodesWAIT", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdWait>, this)),
        std::make_pair("OpcodesWBINVD", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdWbinvd>, this)),
        std::make_pair("OpcodesXLATB", std::bind(&FrontEnd::processEmit<asmjit::x86::Inst::kIdXlatb>, this)),

        // JCC系オペコード
        std::make_pair("OpcodesJA", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJa>, this, _1)),
        std::make_pair("OpcodesJAE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJae>, this, _1)),
        std::make_pair("OpcodesJB", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJb>, this, _1)),
        std::make_pair("OpcodesJBE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJbe>, this, _1)),
        std::make_pair("OpcodesJC", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJc>, this, _1)),
        std::make_pair("OpcodesJE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJe>, this, _1)),
        std::make_pair("OpcodesJG", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJg>, this, _1)),
        std::make_pair("OpcodesJGE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJge>, this, _1)),
        std::make_pair("OpcodesJL", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJl>, this, _1)),
        std::make_pair("OpcodesJLE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJle>, this, _1)),
        std::make_pair("OpcodesJNA", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJna>, this, _1)),
        std::make_pair("OpcodesJNAE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnae>, this, _1)),
        std::make_pair("OpcodesJNB", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnb>, this, _1)),
        std::make_pair("OpcodesJNBE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnbe>, this, _1)),
        std::make_pair("OpcodesJNC", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnc>, this, _1)),
        std::make_pair("OpcodesJNE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJne>, this, _1)),
        std::make_pair("OpcodesJNG", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJng>, this, _1)),
        std::make_pair("OpcodesJNGE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnge>, this, _1)),
        std::make_pair("OpcodesJNL", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnl>, this, _1)),
        std::make_pair("OpcodesJNLE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnle>, this, _1)),
        std::make_pair("OpcodesJNO", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJno>, this, _1)),
        std::make_pair("OpcodesJNP", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnp>, this, _1)),
        std::make_pair("OpcodesJNS", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJns>, this, _1)),
        std::make_pair("OpcodesJNZ", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnz>, this, _1)),
        std::make_pair("OpcodesJO", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJo>, this, _1)),
        std::make_pair("OpcodesJP", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJp>, this, _1)),
        std::make_pair("OpcodesJPE", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJpe>, this, _1)),
        std::make_pair("OpcodesJPO", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJpo>, this, _1)),
        std::make_pair("OpcodesJS", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJs>, this, _1)),
        std::make_pair("OpcodesJZ", std::bind(&FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJz>, this, _1)),

        std::make_pair("OpcodesADC", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdAdc, 0x15>, this, _1)),
        std::make_pair("OpcodesADD", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdAdd, 0x05>, this, _1)),
        std::make_pair("OpcodesALIGNB", std::bind(&FrontEnd::processALIGNB, this, _1)),
        std::make_pair("OpcodesAND", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdAnd, 0x25>, this, _1)),
        std::make_pair("OpcodesCALL", std::bind(&FrontEnd::processCALL, this, _1)),
        std::make_pair("OpcodesCMP", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdCmp, 0x3d>, this, _1)),
        std::make_pair("OpcodesDB", std::bind(&FrontEnd::processDB, this, _1)),
        std::make_pair("OpcodesDW", std::bind(&FrontEnd::processDW, this, _1)),
        std::make_pair("OpcodesDD", std::bind(&FrontEnd::processDD, this, _1)),
        std::make_pair("OpcodesIMUL", std::bind(&FrontEnd::processIMUL, this, _1)),
        std::make_pair("OpcodesIN", std::bind(&FrontEnd::processIN, this, _1)),
        std::make_pair("OpcodesINT", std::bind(&FrontEnd::processINT, this, _1)),
        std::make_pair("OpcodesJMP", std::bind(&FrontEnd::processJMP, this, _1)),
        std::make_pair("OpcodesLIDT", std::bind(&FrontEnd::processLIDT, this, _1)),
        std::make_pair("OpcodesLGDT", std::bind(&FrontEnd::processLGDT, this, _1)),
        std::make_pair("OpcodesMOV", std::bind(&FrontEnd::processMOV, this, _1)),
        std::make_pair("OpcodesOR", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdOr, 0x0d>, this, _1)),
        std::make_pair("OpcodesORG", std::bind(&FrontEnd::processORG, this, _1)),
        std::make_pair("OpcodesOUT", std::bind(&FrontEnd::processOUT, this, _1)),
        std::make_pair("OpcodesPOP", std::bind(&FrontEnd::processPOP, this, _1)),
        std::make_pair("OpcodesPUSH", std::bind(&FrontEnd::processPUSH, this, _1)),
        std::make_pair("OpcodesRET", std::bind(&FrontEnd::processRET, this, _1)),
        std::make_pair("OpcodesRESB", std::bind(&FrontEnd::processRESB, this, _1)),
        std::make_pair("OpcodesSBB", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdSbb, 0x1d>, this, _1)),
        std::make_pair("OpcodesSHR", std::bind(&FrontEnd::processSHR, this, _1)),
        std::make_pair("OpcodesSUB", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdSub, 0x2d>, this, _1)),
        std::make_pair("OpcodesXOR", std::bind(&FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdXor, 0x35>, this, _1))
    };
};


FrontEnd::FrontEnd(bool trace_scanning, bool trace_parsing) {

    // spdlog
    if(!spdlog::get("opennask")) {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // lexer, parser
    this->trace_scanning = trace_scanning;
    this->trace_parsing = trace_parsing;

    // nask
    dollar_position = 0;
    equ_map = std::map<std::string, TParaToken>{};

    // asmjit
    using namespace asmjit;
    Environment env;
    env.setArch(Arch::kX86);
    code_.init(env);
    a_ = std::make_unique<x86::Assembler>(&code_);

    // coff, elf
    o_writer_ = nullptr;

    define_funcs();
}

FrontEnd::~FrontEnd() {
    // メモリの開放
    equ_map.clear();
};

// 以下、抽象クラスの実装(内部で動的に分岐)
void FrontEnd::visitProgram(Program *t) {

    if (dynamic_cast<Prog*>(t) != nullptr) {
        this->visitProg(dynamic_cast<Prog*>(t));
    }
}

void FrontEnd::visitStatement(Statement *t) {

    if (dynamic_cast<LabelStmt*>(t) != nullptr) {
        this->visitLabelStmt(dynamic_cast<LabelStmt*>(t));
    } else if (dynamic_cast<DeclareStmt*>(t) != nullptr) {
        this->visitDeclareStmt(dynamic_cast<DeclareStmt*>(t));
    } else if (dynamic_cast<ConfigStmt*>(t) != nullptr) {
        this->visitConfigStmt(dynamic_cast<ConfigStmt*>(t));
    } else if (dynamic_cast<MnemonicStmt*>(t) != nullptr) {
        this->visitMnemonicStmt(dynamic_cast<MnemonicStmt*>(t));
    } else if (dynamic_cast<ExportSymStmt*>(t) != nullptr) {
        this->visitExportSymStmt(dynamic_cast<ExportSymStmt*>(t));
    } else if (dynamic_cast<ExternSymStmt*>(t) != nullptr) {
        this->visitExternSymStmt(dynamic_cast<ExternSymStmt*>(t));
    }
}

void FrontEnd::visitMnemonicArgs(MnemonicArgs *t) {

    if (dynamic_cast<MnemoArg*>(t) != nullptr) {
        this->visitMnemoArg(dynamic_cast<MnemoArg*>(t));
    }
}

void FrontEnd::visitExp(Exp *t) {

    if (dynamic_cast<PlusExp*>(t) != nullptr) {
        this->visitPlusExp(dynamic_cast<PlusExp*>(t));
    } else if (dynamic_cast<MinusExp*>(t) != nullptr) {
        this->visitMinusExp(dynamic_cast<MinusExp*>(t));
    } else if (dynamic_cast<MulExp*>(t) != nullptr) {
        this->visitMulExp(dynamic_cast<MulExp*>(t));
    } else if (dynamic_cast<DivExp*>(t) != nullptr) {
        this->visitDivExp(dynamic_cast<DivExp*>(t));
    } else if (dynamic_cast<ModExp*>(t) != nullptr) {
        this->visitModExp(dynamic_cast<ModExp*>(t));
    } else if (dynamic_cast<DatatypeExp*>(t) != nullptr) {
        this->visitDatatypeExp(dynamic_cast<DatatypeExp*>(t));
    } else if (dynamic_cast<SegmentOffsetExp*>(t) != nullptr) {
        this->visitSegmentOffsetExp(dynamic_cast<SegmentOffsetExp*>(t));
    } else if (dynamic_cast<ImmExp*>(t) != nullptr) {
        this->visitImmExp(dynamic_cast<ImmExp*>(t));
    } else if (dynamic_cast<MemoryAddrExp*>(t) != nullptr) {
        this->visitMemoryAddrExp(dynamic_cast<MemoryAddrExp*>(t));
    } else if (dynamic_cast<JmpMemoryAddrExp*>(t) != nullptr) {
        this->visitJmpMemoryAddrExp(dynamic_cast<JmpMemoryAddrExp*>(t));
    }
}

void FrontEnd::visitMemoryAddrExp(MemoryAddrExp *t) {
    // 派生クラスでの読み取りは front_memory_addr.cc に記述
    if (t->memoryaddr_) t->memoryaddr_->accept(this);
}

void FrontEnd::visitJmpMemoryAddrExp(JmpMemoryAddrExp *jmp_memory_addr_exp) {
    // 派生クラスでの読み取りは front_memory_addr.cc に記述
    if (jmp_memory_addr_exp->jumpdir_) jmp_memory_addr_exp->jumpdir_->accept(this);
    if (jmp_memory_addr_exp->memoryaddr_) jmp_memory_addr_exp->memoryaddr_->accept(this);

    using namespace matchit;
    auto jump_type = match(type(*jmp_memory_addr_exp->jumpdir_))(
        pattern | "ShortJumpDir" = "SHORT",
        pattern | "NearJumpDir" = "NEAR",
        pattern | "FarJumpDir" = "FAR"
    );

    log()->debug("[pass2] jump directive = {}", jump_type);
    this->ctx.push(TParaToken(jump_type, TParaToken::ttKeyword));
}


void FrontEnd::visitIndexExp(IndexExp *t) {

    if (dynamic_cast<IndexScaleExp*>(t) != nullptr) {
        this->visitIndexScaleExp(dynamic_cast<IndexScaleExp*>(t));
    }
}

void FrontEnd::visitFactor(Factor *t) {

    if (dynamic_cast<NumberFactor*>(t) != nullptr) {
        this->visitNumberFactor(dynamic_cast<NumberFactor*>(t));
    } else if (dynamic_cast<HexFactor*>(t) != nullptr) {
        this->visitHexFactor(dynamic_cast<HexFactor*>(t));
    } else if (dynamic_cast<IdentFactor*>(t) != nullptr) {
        this->visitIdentFactor(dynamic_cast<IdentFactor*>(t));
    } else if (dynamic_cast<StringFactor*>(t) != nullptr) {
        this->visitStringFactor(dynamic_cast<StringFactor*>(t));
    } else if (dynamic_cast<CharFactor*>(t) != nullptr) {
        this->visitCharFactor(dynamic_cast<CharFactor*>(t));
    }
}

void FrontEnd::visitJumpDir(JumpDir *t) {

    if (dynamic_cast<ShortJumpDir*>(t) != nullptr) {
        this->visitShortJumpDir(dynamic_cast<ShortJumpDir*>(t));
    } else if (dynamic_cast<NearJumpDir*>(t) != nullptr) {
        this->visitNearJumpDir(dynamic_cast<NearJumpDir*>(t));
    } else if (dynamic_cast<FarJumpDir*>(t) != nullptr) {
        this->visitFarJumpDir(dynamic_cast<FarJumpDir*>(t));
    }
}

void FrontEnd::visitConfigType(ConfigType *t) {
    // NOP
}

void FrontEnd::visitOpcode(Opcode *t) {
    // NOP
}

void FrontEnd::visitDataType(DataType *t) {
    // NOP
}

void FrontEnd::visitProg(Prog *prog) {

    if (prog->liststatement_) {
        prog->liststatement_->accept(this);
    }
}

void FrontEnd::visitLabelStmt(LabelStmt *label_stmt) {
    visitLabel(label_stmt->label_);

    TParaToken t = this->ctx.top();
    this->ctx.pop();
    log()->debug("[pass2] visitLabelStmt: args = {}", t.to_string());
}

void FrontEnd::visitDeclareStmt(DeclareStmt *declare_stmt) {

    visitIdent(declare_stmt->id_);
    TParaToken key = this->ctx.top();
    this->ctx.pop();

    if (declare_stmt->exp_) {
        declare_stmt->exp_->accept(this);
    }
    TParaToken value = this->ctx.top();
    this->ctx.pop();

    log()->debug("[pass2] declare {} = {}", key.AsString(), value.AsString());
    equ_map[key.AsString()] = value;
}

void FrontEnd::visitMnemonicStmt(MnemonicStmt *mnemonic_stmt){

    if (mnemonic_stmt->opcode_) {
        mnemonic_stmt->opcode_->accept(this);
    }
    if (mnemonic_stmt->listmnemonicargs_) {
        mnemonic_stmt->listmnemonicargs_->accept(this);
    }

    std::vector<TParaToken> mnemonic_args;
    size_t size = this->ctx.size();

    for (int i = 0; i < size; i++ ) {
        // stackなので上から順に取得している
        TParaToken t = this->ctx.top();
        mnemonic_args.insert(mnemonic_args.begin(), t);
        this->ctx.pop();
    }

    const std::string opcode = type(*mnemonic_stmt->opcode_);

    //std::string debug_str = this->join(mnemonic_args->to_string(), ",");
    std::vector<std::string> debug_args;
    std::transform(mnemonic_args.begin(), mnemonic_args.end(),
                   std::back_inserter(debug_args),
                   [](TParaToken x) { return "{ " + x.to_string() + " }"; });

    log()->debug("[pass2] opcode={} mnemonic_args=[{}]", opcode, this->join(debug_args, ","));

    FuncsType::iterator it = funcs.find(opcode);
    if (it == funcs.end()) {
        throw std::runtime_error("[pass2] " + opcode + " is not implemented");
    }

    try {
        it->second(mnemonic_args);
    } catch (const std::bad_alloc& e) {
        log()->error("[pass2] std::bad_alloc例外がスローされました: {}", e.what());
    }
}

//
// Visit Opcode系の処理
//
void FrontEnd::visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) {

    for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i) {
        (*i)->accept(this);
    }
}

void FrontEnd::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
}

//
// expressionの処理
//
void FrontEnd::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitSegmentOffsetExp(SegmentOffsetExp *segment_offset_exp) {
    if (segment_offset_exp->datatype_) {
        segment_offset_exp->datatype_->accept(this);
    }
    if (segment_offset_exp->exp_1) {
        segment_offset_exp->exp_1->accept(this);
    }
    if (segment_offset_exp->exp_2) {
        segment_offset_exp->exp_2->accept(this);
    }
    TParaToken offset = this->ctx.top();
    this->ctx.pop();
    TParaToken segment = this->ctx.top();
    this->ctx.pop();
    TParaToken data_type = this->ctx.top();
    this->ctx.pop();

    auto mem = asmjit::x86::Mem();
    mem.setOffset(offset.AsInt32());
    offset.SetMem(mem, segment.AsInt32());

    using namespace matchit;
    match(data_type.AsString())(
        pattern | "BYTE"  = [&]{ offset.SetAttribute(TParaToken::ttMem8); },
        pattern | "WORD"  = [&]{ offset.SetAttribute(TParaToken::ttMem16); },
        pattern | "DWORD" = [&]{ offset.SetAttribute(TParaToken::ttMem32); },
        pattern | _ = [&] {
            throw std::runtime_error("[pass2] segment:offset, data type is invalid");
        }
    );

    this->ctx.push(offset);
}

template void FrontEnd::visitDataTypes<ByteDataType>(ByteDataType *p);
template void FrontEnd::visitDataTypes<WordDataType>(WordDataType *p);
template void FrontEnd::visitDataTypes<DwordDataType>(DwordDataType *p);

template <class T>
void FrontEnd::visitDataTypes(T *t) {

    std::string literal;
    if constexpr (std::is_same_v<T, ByteDataType>) {
        literal = "BYTE";
    } else if constexpr (std::is_same_v<T, WordDataType>) {
        literal = "WORD";
    } else if constexpr (std::is_same_v<T, DwordDataType>) {
        literal = "DWORD";
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    log()->debug("[pass2] datatype {}", literal);
    this->ctx.push(TParaToken(literal, TParaToken::ttKeyword));
}

void FrontEnd::visitByteDataType(ByteDataType *p) {
    visitDataTypes(p);
}
void FrontEnd::visitWordDataType(WordDataType *p) {
    visitDataTypes(p);
}
void FrontEnd::visitDwordDataType(DwordDataType *p) {
    visitDataTypes(p);
}

void FrontEnd::visitPlusExp(PlusExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitMinusExp(MinusExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitMulExp(MulExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitDivExp(DivExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitModExp(ModExp *p) {
    visitArithmeticOperations(p);
}

template void FrontEnd::visitArithmeticOperations<PlusExp>(PlusExp *p);
template void FrontEnd::visitArithmeticOperations<MinusExp>(MinusExp *p);
template void FrontEnd::visitArithmeticOperations<MulExp>(MulExp *p);
template void FrontEnd::visitArithmeticOperations<DivExp>(DivExp *p);
template void FrontEnd::visitArithmeticOperations<ModExp>(ModExp *p);


template <class T>
void FrontEnd::visitArithmeticOperations(T *exp) {

    if (exp->exp_1) {
        exp->exp_1->accept(this);
    }
    TParaToken left = this->ctx.top();
    //left.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    if (exp->exp_2) {
        exp->exp_2->accept(this);
    }
    TParaToken right = this->ctx.top();
    //right.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    long ans = 0;
    if constexpr (std::is_same_v<T, PlusExp>) {
        ans = left.AsInt32() + right.AsInt32();
    } else if constexpr (std::is_same_v<T, MinusExp>) {
        ans = left.AsInt32() - right.AsInt32();
    } else if constexpr (std::is_same_v<T, MulExp>) {
        ans = left.AsInt32() * right.AsInt32();
    } else if constexpr (std::is_same_v<T, DivExp>) {
        ans = left.AsInt32() / right.AsInt32();
    } else if constexpr (std::is_same_v<T, ModExp>) {
        ans = left.AsInt32() % right.AsInt32();
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    TParaToken t = TParaToken(std::to_string(ans), TParaToken::ttInteger);
    this->ctx.push(t);
}

//
// factorの処理
//
void FrontEnd::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttInteger);
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttHex);
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->id_);
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitCharFactor(CharFactor *char_factor) {
    visitNaskChar(char_factor->naskchar_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

//
// tokenの処理
//
void FrontEnd::visitInteger(Integer x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttInteger);
    this->ctx.push(t);
}

void FrontEnd::visitChar(Char x) {
    std::string str{x};
    TParaToken t = TParaToken(str, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitDouble(Double x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttFloating);
    this->ctx.push(t);
}

void FrontEnd::visitString(String x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitIdent(Ident x) {
    if (equ_map.count(x) > 0) {
        // 変数定義があれば展開する
        log()->debug("[pass2] EQU {} = {}", x, equ_map[x].AsString());
        TParaToken t = TParaToken(equ_map[x].AsString(), equ_map[x].AsType());
        this->ctx.push(t);
        return;
    }
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitNaskChar(NaskChar x) {
    x.erase(x.begin());
    x.pop_back();
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitHex(Hex x) {
    TParaToken t = TParaToken(x, TParaToken::ttHex);
    this->ctx.push(t);
}

void FrontEnd::visitId(Id x) {
    if (equ_map.count(x) > 0) {
        // 変数定義があれば展開する
        log()->debug("[pass2] EQU {} = {}", x, equ_map[x].AsString());
        TParaToken t = TParaToken(equ_map[x].AsString(), equ_map[x].AsType());
        this->ctx.push(t);
        return;
    }
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

template <class T>
std::shared_ptr<T> FrontEnd::Parse(std::istream &input) {

    std::shared_ptr<T> parse_tree = nullptr;
    auto driver = std::make_unique<NaskDriver>();
    // TODO: 開発者向けのデバッグログオプションを作る
    //driver->trace_scanning = this->trace_scanning;
    //driver->trace_parsing = this->trace_parsing;

    try {
        if constexpr (std::is_same_v<T, Program>) {
            parse_tree = driver->pProgram(input);
        }
        else if constexpr (std::is_same_v<T, ListStatement>) {
            parse_tree = driver->pListStatement(input);
        }
        else if constexpr (std::is_same_v<T, Statement>) {
            parse_tree = driver->pStatement(input);
        }
        else if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
            parse_tree = driver->pListMnemonicArgs(input);
        }
        else if constexpr (std::is_same_v<T, MnemonicArgs>) {
            parse_tree = driver->pMnemonicArgs(input);
        }
        else if constexpr (std::is_same_v<T, Exp>) {
            parse_tree = driver->pExp(input);
        }
        else if constexpr (std::is_same_v<T, MemoryAddr>) {
            parse_tree = driver->pMemoryAddr(input);
        }
        else if constexpr (std::is_same_v<T, IndexExp>) {
            parse_tree = driver->pIndexExp(input);
        }
        else if constexpr (std::is_same_v<T, Factor>) {
            parse_tree = driver->pFactor(input);
        }
        else if constexpr (std::is_same_v<T, ConfigType>) {
            parse_tree = driver->pConfigType(input);
        }
        else if constexpr (std::is_same_v<T, DataType>) {
            parse_tree = driver->pDataType(input);
        }
        else if constexpr (std::is_same_v<T, Opcode>) {
            parse_tree = driver->pOpcode(input);
        }
        else {
            static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
        }

    } catch( parse_error &e) {
        std::cerr << "Parse error on line " << e.getLine() << "\n";
    }

    if (parse_tree) {
        printf("\nParse Successful!\n");
        if (trace_scanning) {
            printf("\n[Abstract Syntax]\n");
            auto s = std::make_unique<ShowAbsyn>(ShowAbsyn());
            printf("%s\n\n", s->show(parse_tree.get()));
        }
        if (trace_parsing) {
            printf("[Linearized Tree]\n");
            auto p = std::make_unique<PrintAbsyn>(PrintAbsyn());
            printf("%s\n\n", p->print(parse_tree.get()));
        }
    }
    return parse_tree;
}

template std::shared_ptr<Program> FrontEnd::Parse<Program>(std::istream &input);
template std::shared_ptr<ListStatement> FrontEnd::Parse<ListStatement>(std::istream &input);
template std::shared_ptr<Statement> FrontEnd::Parse<Statement>(std::istream &input);
template std::shared_ptr<ListMnemonicArgs> FrontEnd::Parse<ListMnemonicArgs>(std::istream &input);
template std::shared_ptr<MnemonicArgs> FrontEnd::Parse<MnemonicArgs>(std::istream &input);
template std::shared_ptr<Exp> FrontEnd::Parse<Exp>(std::istream &input);
template std::shared_ptr<MemoryAddr> FrontEnd::Parse<MemoryAddr>(std::istream &input);
template std::shared_ptr<IndexExp> FrontEnd::Parse<IndexExp>(std::istream &input);
template std::shared_ptr<Factor> FrontEnd::Parse<Factor>(std::istream &input);
template std::shared_ptr<ConfigType> FrontEnd::Parse<ConfigType>(std::istream &input);
template std::shared_ptr<DataType> FrontEnd::Parse<DataType>(std::istream &input);
template std::shared_ptr<Opcode> FrontEnd::Parse<Opcode>(std::istream &input);


template <class T>
int FrontEnd::Eval(T *parse_tree, const char* assembly_dst) {

    std::ofstream img_out(assembly_dst, std::ios::trunc | std::ios::binary);
    if ( img_out.bad() || img_out.fail() ) {
        std::cerr << "NASK : can't open " << assembly_dst << std::endl;
        return 17;
    }

    // ここでシンボルテーブル等をpass1からgetする
    auto pass1 = std::make_unique<Pass1Strategy>();
    pass1->Eval(parse_tree);
    sym_table = std::move(pass1->sym_table);

    for (auto entry : sym_table) {
        log()->debug("[pass2] symbol_table [{}] = {}({:x})", entry.first, entry.second, entry.second);
    }

    // Eval開始
    if constexpr (std::is_same_v<T, Program>) {
        this->visitProgram(parse_tree);
    } else if constexpr (std::is_same_v<T, ListStatement>) {
        this->visitListStatement(parse_tree);
    } else if constexpr (std::is_same_v<T, Statement>) {
        this->visitStatement(parse_tree);
    } else if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
        this->visitListMnemonicArgs(parse_tree);
    } else if constexpr (std::is_same_v<T, MnemonicArgs>) {
        this->visitMnemonicArgs(parse_tree);
    } else if constexpr (std::is_same_v<T, Exp>) {
        this->visitExp(parse_tree);
    } else if constexpr (std::is_same_v<T, MemoryAddr>) {
        this->visitMemoryAddr(parse_tree);
    } else if constexpr (std::is_same_v<T, IndexExp>) {
        this->visitIndexExp(parse_tree);
    } else if constexpr (std::is_same_v<T, Factor>) {
        this->visitFactor(parse_tree);
    } else if constexpr (std::is_same_v<T, ConfigType>) {
        this->visitConfigType(parse_tree);
    } else if constexpr (std::is_same_v<T, DataType>) {
        this->visitDataType(parse_tree);
    } else if constexpr (std::is_same_v<T, Opcode>) {
        this->visitOpcode(parse_tree);
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    using namespace asmjit;
    CodeBuffer& buf = code_.textSection()->buffer();

    if (o_writer_ != nullptr) {
        // オブジェクトファイル書き出しモード
        o_writer_->write_coff(code_, *a_, sym_table);
    }
    binout_container.assign(buf.data(), buf.data() + buf.size());

    img_out.write(reinterpret_cast<char*>(binout_container.data()), binout_container.size());
    img_out.close();

    return 0;
}

template int FrontEnd::Eval<Program>(Program* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ListStatement>(ListStatement* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Statement>(Statement* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ListMnemonicArgs>(ListMnemonicArgs* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<MnemonicArgs>(MnemonicArgs* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Exp>(Exp* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<MemoryAddr>(MemoryAddr* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<IndexExp>(IndexExp* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Factor>(Factor* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ConfigType>(ConfigType* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<DataType>(DataType* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Opcode>(Opcode* parse_tree, const char* assembly_dst);
