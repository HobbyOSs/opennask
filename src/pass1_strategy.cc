#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "pass1_strategy.hh"
#include "matchit.h"
#include "demangle.hpp"
#include <jsoncons/json.hpp>
#include "x86.hh"

using namespace std::placeholders;
using namespace matchit;

typedef std::function<void(std::vector<TParaToken>&)> nim_callback;
typedef std::map<std::string, nim_callback> funcs_type;

Pass1Strategy::Pass1Strategy() {
    // spdlog
    if(!spdlog::get("opennask")) {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    sym_table = std::map<std::string, uint32_t>{};
    lit_table = std::map<std::string, uint32_t>{};
    equ_map = std::map<std::string, TParaToken>{};
    loc = 0;
    iset = std::make_unique<x86_64::InstructionSet>(jsoncons::decode_json<x86_64::InstructionSet>(std::string(x86_64::X86_64_JSON)));
}

Pass1Strategy::~Pass1Strategy() {
    sym_table.clear();
    lit_table.clear();
    equ_map.clear();
}

// 以下、抽象クラスの実装(内部で動的に分岐)
void Pass1Strategy::visitProgram(Program *t) {

    if (dynamic_cast<Prog*>(t) != nullptr) {
        this->visitProg(dynamic_cast<Prog*>(t));
    }
}

void Pass1Strategy::visitStatement(Statement *t) {

    if (dynamic_cast<LabelStmt*>(t) != nullptr) {
        this->visitLabelStmt(dynamic_cast<LabelStmt*>(t));
    } else if (dynamic_cast<DeclareStmt*>(t) != nullptr) {
        this->visitDeclareStmt(dynamic_cast<DeclareStmt*>(t));
    } else if (dynamic_cast<ConfigStmt*>(t) != nullptr) {
        this->visitConfigStmt(dynamic_cast<ConfigStmt*>(t));
    } else if (dynamic_cast<MnemonicStmt*>(t) != nullptr) {
        this->visitMnemonicStmt(dynamic_cast<MnemonicStmt*>(t));
    } else if (dynamic_cast<OpcodeStmt*>(t) != nullptr) {
        this->visitOpcodeStmt(dynamic_cast<OpcodeStmt*>(t));
    }
}

void Pass1Strategy::visitMnemonicArgs(MnemonicArgs *t) {

    if (dynamic_cast<MnemoArg*>(t) != nullptr) {
        this->visitMnemoArg(dynamic_cast<MnemoArg*>(t));
    }
}

void Pass1Strategy::visitExp(Exp *t) {

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
    }
}

void Pass1Strategy::visitMemoryAddrExp(MemoryAddrExp *t) {

    if (t->memoryaddr_) t->memoryaddr_->accept(this);
}

void Pass1Strategy::visitFactor(Factor *t) {

    if (dynamic_cast<NumberFactor*>(t) != nullptr) {
        this->visitNumberFactor(dynamic_cast<NumberFactor*>(t));
    } else if (dynamic_cast<HexFactor*>(t) != nullptr) {
        this->visitHexFactor(dynamic_cast<HexFactor*>(t));
    } else if (dynamic_cast<IdentFactor*>(t) != nullptr) {
        this->visitIdentFactor(dynamic_cast<IdentFactor*>(t));
    } else if (dynamic_cast<StringFactor*>(t) != nullptr) {
        this->visitStringFactor(dynamic_cast<StringFactor*>(t));
    }
}

void Pass1Strategy::visitIndexExp(IndexExp *t) {

    if (dynamic_cast<IndexScaleExp*>(t) != nullptr) {
        this->visitIndexScaleExp(dynamic_cast<IndexScaleExp*>(t));
    }
}

void Pass1Strategy::visitConfigType(ConfigType *t) {
    // NOP
}

void Pass1Strategy::visitOpcode(Opcode *t) {
    // NOP
}

void Pass1Strategy::visitDataType(DataType *t) {
    // NOP
}

void Pass1Strategy::visitProg(Prog *prog) {

    if (prog->liststatement_) {
        prog->liststatement_->accept(this);
    }
}

void Pass1Strategy::visitConfigStmt(ConfigStmt *config_stmt) {

    // TODO: 必要な設定を行う
    // [FORMAT "WCOFF"], [FILE "xxxx.c"], [INSTRSET "i386"], [BITS 32]
    if (config_stmt->configtype_) config_stmt->configtype_->accept(this);
    if (config_stmt->factor_) config_stmt->factor_->accept(this);

    TParaToken t = this->ctx.top();
    this->ctx.pop();
    log()->debug("[pass1] visitConfigStmt: args = {}", t.to_string());
}

void Pass1Strategy::visitLabelStmt(LabelStmt *label_stmt) {
    visitLabel(label_stmt->label_);

    TParaToken t = this->ctx.top();
    this->ctx.pop();

    log()->debug("[pass1] visitLabelStmt: args = {}", t.to_string());
}


void Pass1Strategy::visitDeclareStmt(DeclareStmt *declare_stmt) {

    visitIdent(declare_stmt->ident_);
    TParaToken key = this->ctx.top();
    this->ctx.pop();

    if (declare_stmt->exp_) {
        declare_stmt->exp_->accept(this);
    }
    TParaToken value = this->ctx.top();
    this->ctx.pop();

    log()->debug("[pass1] declare {} = {}", key.AsString(), value.AsString());
    equ_map[key.AsString()] = value;
}

void Pass1Strategy::visitMnemonicStmt(MnemonicStmt *mnemonic_stmt){

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

    std::vector<std::string> debug_args;
    std::transform(mnemonic_args.begin(), mnemonic_args.end(),
                   std::back_inserter(debug_args),
                   [](TParaToken x) { return "{ " + x.to_string() + " }"; });

    log()->debug("[pass1] {} mnemonic_args=[{}]", opcode, this->join(debug_args, ","));

    funcs_type funcs {
        // 疑似命令
        std::make_pair("OpcodesALIGNB", std::bind(&Pass1Strategy::processALIGNB, this, _1)),
        std::make_pair("OpcodesDB", std::bind(&Pass1Strategy::processDB, this, _1)),
        std::make_pair("OpcodesDD", std::bind(&Pass1Strategy::processDD, this, _1)),
        std::make_pair("OpcodesDW", std::bind(&Pass1Strategy::processDW, this, _1)),
        std::make_pair("OpcodesORG", std::bind(&Pass1Strategy::processORG, this, _1)),
        std::make_pair("OpcodesRESB", std::bind(&Pass1Strategy::processRESB, this, _1)),
        // x86命令
        // TODO: 疑似命令以外は機械的に判定したいが、パターンがつかめるまではベタで書く
        std::make_pair("OpcodesADD", std::bind(&Pass1Strategy::processADD, this, _1)),
        std::make_pair("OpcodesAND", std::bind(&Pass1Strategy::processAND, this, _1)),
        std::make_pair("OpcodesCALL", std::bind(&Pass1Strategy::processCALL, this, _1)),
        std::make_pair("OpcodesCMP", std::bind(&Pass1Strategy::processCMP, this, _1)),
        std::make_pair("OpcodesIMUL", std::bind(&Pass1Strategy::processIMUL, this, _1)),
        std::make_pair("OpcodesIN", std::bind(&Pass1Strategy::processIN, this, _1)),
        std::make_pair("OpcodesINT", std::bind(&Pass1Strategy::processINT, this, _1)),
        std::make_pair("OpcodesJAE", std::bind(&Pass1Strategy::processJAE, this, _1)),
        std::make_pair("OpcodesJB", std::bind(&Pass1Strategy::processJB, this, _1)),
        std::make_pair("OpcodesJBE", std::bind(&Pass1Strategy::processJBE, this, _1)),
        std::make_pair("OpcodesJC", std::bind(&Pass1Strategy::processJC, this, _1)),
        std::make_pair("OpcodesJE", std::bind(&Pass1Strategy::processJE, this, _1)),
        std::make_pair("OpcodesJMP", std::bind(&Pass1Strategy::processJMP, this, _1)),
        std::make_pair("OpcodesJNC", std::bind(&Pass1Strategy::processJNC, this, _1)),
        std::make_pair("OpcodesJNZ", std::bind(&Pass1Strategy::processJNZ, this, _1)),
        std::make_pair("OpcodesJZ", std::bind(&Pass1Strategy::processJZ, this, _1)),
        std::make_pair("OpcodesLGDT", std::bind(&Pass1Strategy::processLGDT, this, _1)),
        std::make_pair("OpcodesMOV", std::bind(&Pass1Strategy::processMOV, this, _1)),
        std::make_pair("OpcodesOR", std::bind(&Pass1Strategy::processOR, this, _1)),
        std::make_pair("OpcodesOUT", std::bind(&Pass1Strategy::processOUT, this, _1)),
        std::make_pair("OpcodesSHR", std::bind(&Pass1Strategy::processSHR, this, _1)),
        std::make_pair("OpcodesSUB", std::bind(&Pass1Strategy::processSUB, this, _1)),
    };

    funcs_type::iterator func_iter = funcs.find(opcode);

    if (func_iter == funcs.end()) {
        throw std::runtime_error("[pass1] " + opcode + " is not implemented!!!");
    }

    func_iter->second(mnemonic_args);
}

void Pass1Strategy::visitOpcodeStmt(OpcodeStmt *opcode_stmt) {
    if (opcode_stmt->opcode_) {
        opcode_stmt->opcode_->accept(this);
    }

    typedef std::function<void()> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesCLI", std::bind(&Pass1Strategy::processCLI, this)),
        std::make_pair("OpcodesHLT", std::bind(&Pass1Strategy::processHLT, this)),
        std::make_pair("OpcodesNOP", std::bind(&Pass1Strategy::processNOP, this)),
        std::make_pair("OpcodesRET", std::bind(&Pass1Strategy::processRET, this)),
    };

    const std::string opcode = type(*opcode_stmt->opcode_);
    log()->debug("[pass1] {}", opcode);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second();
    } else {
        throw std::runtime_error(opcode + " is not implemented!!!");
    }
}

void Pass1Strategy::processALIGNB(std::vector<TParaToken>& mnemonic_args) {
    uint32_t l = 0;

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttInteger);
    loc += arg.AsInt32();
    log()->debug("[pass1] LOC = {}({:x})", loc, loc);
    return;
}


void Pass1Strategy::processDB(std::vector<TParaToken>& mnemonic_args) {
    uint32_t l = 0;

    for (const auto& e : mnemonic_args) {
        if (e.IsInteger() || e.IsHex()) {
            l += NASK_BYTE;
        } else if (e.IsIdentifier()) {
            std::string s = e.AsString();
            l += s.size();
        }
    }
    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processADD(std::vector<TParaToken>& mnemonic_args) {
    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["ADD"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // --
    // 04      al      imm8
    // 80      r8      imm8
    // 00      r8      r8
    // 02      r8      m8
    // 05      ax      imm16
    // 83      r16     imm8
    // 81      r16     imm16
    // 01      r16     r16
    // 03      r16     m16
    // 05      eax     imm32
    // 83      r32     imm8
    // 81      r32     imm32
    // 01      r32     r32
    // 03      r32     m32
    // 05      rax     imm32
    // 83      r64     imm8
    // 81      r64     imm32
    // 01      r64     r64
    // 03      r64     m64
    // 80      m8      imm8
    // 00      m8      r8
    // 83      m16     imm8
    // 81      m16     imm16
    // 01      m16     r16
    // 83      m32     imm8
    // 81      m32     imm32
    // 01      m32     r32
    // 83      m64     imm8
    // 81      m64     imm32
    // 01      m64     r64
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    auto inst = iset->instructions().at("ADD");

    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg8, "AL", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttMem8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "AX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttMem16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, "EAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, "RAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] ADD, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processAND(std::vector<TParaToken>& mnemonic_args) {
    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["AND"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // --
    // 24      al      imm8
    // 80      r8      imm8
    // 20      r8      r8
    // 22      r8      m8
    // 25      ax      imm16
    // 83      r16     imm8
    // 81      r16     imm16
    // 21      r16     r16
    // 23      r16     m16
    // 25      eax     imm32
    // 83      r32     imm8
    // 81      r32     imm32
    // 21      r32     r32
    // 23      r32     m32
    // 25      rax     imm32
    // 83      r64     imm8
    // 81      r64     imm32
    // 21      r64     r64
    // 23      r64     m64
    // 80      m8      imm8
    // 20      m8      r8
    // 83      m16     imm8
    // 81      m16     imm16
    // 21      m16     r16
    // 83      m32     imm8
    // 81      m32     imm32
    // 21      m32     r32
    // 83      m64     imm8
    // 81      m64     imm32
    // 21      m64     r64
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    auto inst = iset->instructions().at("AND");

    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg8, "AL", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttMem8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "AX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttMem16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, "EAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, "RAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] AND, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processCLI() {
    loc += 1;
    log()->debug("[pass1] LOC = {}({:x})", loc, loc);
    return;
}

void Pass1Strategy::processCALL(std::vector<TParaToken>& mnemonic_args) {
    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["CALL"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // --
    // E8      rel32
    // FF      r64
    // FF      m64
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString()
    );

    auto inst = iset->instructions().at("CALL");

    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return 3; // opcode + iw
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] CALL, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processCMP(std::vector<TParaToken>& mnemonic_args) {
    // cat src/json-x86-64/x86_64.json | \
    // jq -r '.instructions["CMP"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // 3C      al      imm8
    // 80      r8      imm8
    // 38      r8      r8
    // 3A      r8      m8
    // 3D      ax      imm16
    // 83      r16     imm8
    // 81      r16     imm16
    // 39      r16     r16
    // 3B      r16     m16
    // 3D      eax     imm32
    // 83      r32     imm8
    // 81      r32     imm32
    // 39      r32     r32
    // 3B      r32     m32
    // 3D      rax     imm32
    // 83      r64     imm8
    // 81      r64     imm32
    // 39      r64     r64
    // 3B      r64     m64
    // 80      m8      imm8
    // 38      m8      r8
    // 83      m16     imm8
    // 81      m16     imm16
    // 39      m16     r16
    // 83      m32     imm8
    // 81      m32     imm32
    // 39      m32     r32
    // 83      m64     imm8
    // 81      m64     imm32
    // 39      m64     r64
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    auto inst = iset->instructions().at("CMP");

    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg8, "AL", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttMem8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "AX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttMem16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, "EAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, "RAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] CMP, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processDW(std::vector<TParaToken>& mnemonic_args) {
    uint32_t l = 0;

    for (const auto& e : mnemonic_args) {
        if (e.IsInteger() || e.IsHex()) {
            l += NASK_WORD;
        } else if (e.IsIdentifier()) {
            std::string s = e.AsString();
            l += s.size();
        }
    }
    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processDD(std::vector<TParaToken>& mnemonic_args) {
    uint32_t l = 0;

    for (const auto& e : mnemonic_args) {
        if (e.IsInteger() || e.IsHex()) {
            l += NASK_DWORD;
        } else if (e.IsIdentifier()) {
            std::string s = e.AsString();
            l += s.size();
        }
    }
    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processRESB(std::vector<TParaToken>& mnemonic_args) {
    uint32_t l = 0;

    auto arg = mnemonic_args[0];
    const std::string suffix = "-$";

    if (auto range = arg.AsString(); range.find(suffix) != std::string::npos) {
        auto resb_size = range.substr(0, range.length() - suffix.length());
        auto resb_token = TParaToken(resb_size, TParaToken::ttHex);

        loc += resb_token.AsInt32();
        log()->debug("[pass1] LOC = {}({:x})", loc, loc);
        return;
    }

    arg.MustBe(TParaToken::ttInteger);
    loc += arg.AsInt32();
    log()->debug("[pass1] LOC = {}({:x})", loc, loc);
    return;
}

void Pass1Strategy::processHLT() {
    loc += 1;
    log()->debug("[pass1] LOC = {}({:x})", loc, loc);
    return;
}

void Pass1Strategy::processIMUL(std::vector<TParaToken>& mnemonic_args) {
    // cat src/json-x86-64/x86_64.json | \
    // jq -r '.instructions["IMUL"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // ---
    // F6      r8
    // F7      r16
    // F7      r32
    // F7      r64
    // F6      m8
    // F7      m16
    // F7      m32
    // F7      m64
    // AF      r16     r16
    // AF      r16     m16
    // AF      r32     r32
    // AF      r32     m32
    // AF      r64     r64
    // AF      r64     m64
    // 6B      r16     r16     imm8
    // 69      r16     r16     imm16
    // 6B      r16     m16     imm8
    // 69      r16     m16     imm16
    // 6B      r32     r32     imm8
    // 69      r32     r32     imm32
    // 6B      r32     m32     imm8
    // 69      r32     m32     imm32
    // 6B      r64     r64     imm8
    // 69      r64     r64     imm32
    // 6B      r64     m64     imm8
    // 69      r64     m64     imm32

    // オペランドが可変長のためoptionalで宣言
    auto get_optional = [&](std::size_t index) -> std::optional<TParaToken::TIdentiferAttribute> {
        return index < mnemonic_args.size() ? std::make_optional(mnemonic_args[index].AsAttr()) : std::nullopt ;
    };
    auto operands = std::make_tuple(
        get_optional(0),
        get_optional(1),
        get_optional(2)
    );
    auto inst = iset->instructions().at("IMUL");

    uint32_t l = match(operands)(
        // F6      r8
        // F7      r16
        // F7      r32
        // F7      r64
        // F6      m8
        // F7      m16
        // F7      m32
        // F7      m64
        pattern | ds(or_(TParaToken::ttReg8,
                         TParaToken::ttReg16,
                         TParaToken::ttReg32,
                         TParaToken::ttReg64,
                         TParaToken::ttMem8,
                         TParaToken::ttMem16,
                         TParaToken::ttMem32,
                         TParaToken::ttMem64), std::nullopt, std::nullopt) = [&] {
                             return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // AF      r16     r16
        // AF      r16     m16
        // AF      r32     r32
        // AF      r32     m32
        // AF      r64     r64
        // AF      r64     m64
        pattern | ds(or_(TParaToken::ttReg16, TParaToken::ttReg32, TParaToken::ttReg64),
                     or_(TParaToken::ttReg16, TParaToken::ttReg32, TParaToken::ttReg64,
                         TParaToken::ttMem16, TParaToken::ttMem32, TParaToken::ttMem64), std::nullopt) = [&] {
                             return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // TODO: 即値部分はJSONには記載なし
        // 6B      r16     imm8
        // 6B      r32     imm8
        // 69      r16     imm16
        // 69      r32     imm32
        pattern | ds(or_(TParaToken::ttReg16, TParaToken::ttReg32), TParaToken::ttImm, std::nullopt) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 6B      r16     r16     imm8
        // 69      r16     r16     imm16
        // 6B      r16     m16     imm8
        // 69      r16     m16     imm16
        // 6B      r32     r32     imm8
        // 69      r32     r32     imm32
        // 6B      r32     m32     imm8
        // 69      r32     m32     imm32
        // 6B      r64     r64     imm8
        // 69      r64     r64     imm32
        // 6B      r64     m64     imm8
        // 69      r64     m64     imm32
        pattern | ds(or_(TParaToken::ttReg16, TParaToken::ttReg32, TParaToken::ttReg64),
                     or_(TParaToken::ttReg16, TParaToken::ttReg32, TParaToken::ttReg64,
                         TParaToken::ttMem16, TParaToken::ttMem32, TParaToken::ttMem64),
                     or_(TParaToken::ttImm)) = [&] {
                         return inst.get_output_size(bit_mode, {mnemonic_args[0], mnemonic_args[1], mnemonic_args[2]});
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] IMUL, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processIN(std::vector<TParaToken>& mnemonic_args) {
    // json-x86-64/x86_64.json に記述なし
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    uint32_t l = match(operands)(
        pattern | ds(_, or_(std::string("AL"), std::string("AX"), std::string("EAX")), TParaToken::ttImm, _) = [&] {
           return NASK_WORD;
        },
        pattern | ds(_, or_(std::string("AL"), std::string("AX"), std::string("EAX")), _, "DX") = [&] {
            return NASK_BYTE;
        },
        pattern | _ = [&] {
           std::stringstream ss;
           ss << "[pass1] IN, Not implemented or not matched!!! \n"
              << mnemonic_args[0].AsString()
              << ", "
              << mnemonic_args[1].AsString()
              << "\n"
              << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
              << ", "
              << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
              << std::endl;

           throw std::runtime_error(ss.str());
           return 0;
        }
    );

    //loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


void Pass1Strategy::processINT(std::vector<TParaToken>& mnemonic_args) {

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString()
    );

    auto inst = iset->instructions().at("INT");

    uint32_t l = match(operands)(
        pattern | ds(_, "3") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] INT, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJAE(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x73      cb JAE rel8    CF=0 より上か等しい場合ショートジャンプします
        // 0x0F 0x83 cw JAE rel16   CF=0 より上か等しい場合ニアジャンプします
        // 0x0F 0x83 cd JAE rel32   CF=0 より上か等しい場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJB(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x72      cb JB rel8    CF=1 より下の場合ショートジャンプします
        // 0x0F 0x82 cw JB rel16   CF=1 より下の場合ニアジャンプします
        // 0x0F 0x82 cd JB rel32   CF=1 より下の場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJBE(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x76      cb JBE rel8    CF=1 or ZF=1 より下か等しい場合ショートジャンプします
        // 0x0F 0x86 cw JBE rel16   CF=1 or ZF=1 より下か等しい場合ニアジャンプします
        // 0x0F 0x86 cd JBE rel32   CF=1 or ZF=1 より下か等しい場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJC(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x72      cb JC rel8    CF=1 キャリーがある場合ショートジャンプします
        // 0x0F 0x82 cw JC rel16   CF=1 キャリーがある場合ニアジャンプします
        // 0x0F 0x82 cd JC rel32   CF=1 キャリーがある場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJE(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x74      cb JE rel8    ZF=1 等しい場合ショートジャンプします
        // 0x0F 0x84 cw JE rel16   ZF=1 等しい場合ニアジャンプします
        // 0x0F 0x84 cd JE rel32   ZF=1 等しい場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJMP(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    // TODO: 絶対ジャンプについては後ほど実装
    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0xEB cb JMP rel8    次の命令との相対オフセットだけ相対ショートジャンプする
        // 0xE9 cw JMP rel16   次の命令との相対オフセットだけ相対ニアジャンプする
        // 0xE9 cd JMP rel32   次の命令との相対オフセットだけ相対ニアジャンプする
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 2,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 3,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 5
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJNC(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x73      cb JNC rel8    キャリーがない場合ショートジャンプします
        // 0x0F 0x83 cw JNC rel16   キャリーがない場合ニアジャンプします
        // 0x0F 0x83 cd JNC rel32   キャリーがない場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processJNZ(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x75      cb JNZ rel8    ゼロでない場合ショートジャンプします
        // 0x0F 0x85 cw JNZ rel16   ゼロでない場合ニアジャンプします
        // 0x0F 0x85 cd JNZ rel32   ゼロでない場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


void Pass1Strategy::processJZ(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // 相対ジャンプ
        // 0x74      cb JZ rel8    ZF=1 ゼロの場合ショートジャンプします
        // 0x0F 0x84 cw JZ rel16   ZF=1 ゼロの場合ニアジャンプします
        // 0x0F 0x84 cd JZ rel32   ZF=1 ゼロの場合ニアジャンプします
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


void Pass1Strategy::processLGDT(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずm16として処理する, どちらになるかはpass2で判断する
        loc += 3; // opcode + modrm
        loc += NASK_WORD;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 3+NASK_WORD);
        return;
    }

    uint32_t l = match(t.AsInt32())(
        // m16 or m32
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 3 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 3 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processMOV(std::vector<TParaToken>& mnemonic_args) {

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["MOV"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // -- パターンは25個ある
    // TODO: x86 tableに下記1行の記載なし
    // B0+rb   r8      imm8
    //
    // C6      r8      imm8
    // 88      r8      r8
    // 8A      r8      m8
    // C7      r16     imm16
    // 89      r16     r16
    // 8B      r16     m16
    // A1      eax     moffs32
    // C7      r32     imm32
    // 89      r32     r32
    // 8B      r32     m32
    // A1      rax     moffs64
    // C7      r64     imm32
    // B8      r64     imm64
    // 89      r64     r64
    // 8B      r64     m64
    // C6      m8      imm8
    // 88      m8      r8
    // C7      m16     imm16
    // 89      m16     r16
    // C7      m32     imm32
    // 89      m32     r32
    // C7      m64     imm32
    // 89      m64     r64
    // TODO: x86 tableに下記2行の記載なし
    // A2      moffs8  al
    // A3      moffs16 ax
    //
    // A3      moffs32 eax
    // A3      moffs64 rax
    auto inst = iset->instructions().at("MOV");

    uint32_t l = match(operands)(

        // セグメントレジスタ
        pattern | ds(TParaToken::ttSreg, _, _, _) = [&] {
            return 2; // opcode, modrmなので2byte
        },
        pattern | ds(_, _, TParaToken::ttSreg, _) = [&] {
            return 2; // opcode, modrmなので2byte
        },
        // コントロールレジスタ
        pattern | ds(TParaToken::ttCreg, _, TParaToken::ttReg32, _) = [&] {
            return 3;
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttCreg, _) = [&] {
            return 3;
        },
        // A2      moffs8  al
        pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16), _, TParaToken::ttReg8, "AL") = [&] {
            return 3; // opcode + moffs8
        },
        // A3      moffs32 eax
        pattern | ds(_, _, TParaToken::ttReg32, "EAX") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // A3      moffs64 rax
        pattern | ds(_, _, TParaToken::ttReg64, "RAX") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // A1      eax     moffs32
        pattern | ds(TParaToken::ttReg32, "EAX", _, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // A1      rax     moffs64
        pattern | ds(TParaToken::ttReg64, "RAX", _, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },

        // C6      r8      imm8
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 88      r8      r8
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 8A      r8      m8
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), _) = [&] {
            // TODO: m16,m32でもOKなのだがx86 tableの検索に引っかからなくなるのでttMem8に変換する.もっといい方法がないか検討
            auto token = TParaToken(mnemonic_args[1]);
            token.SetAttribute(TParaToken::ttMem8);
            return inst.get_output_size(bit_mode, {mnemonic_args[0], token});
        },
        // C7      r16     imm16
        pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 89      r16     r16
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 8B      r16     m16
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttMem16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // C7      r32     imm32
        pattern | ds(TParaToken::ttReg32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 89      r32     r32
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 8B      r32     m32
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // C7      r64     imm32
        // B8      r64     imm64
        pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 89      r64     r64
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 8B      r64     m64
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // C6      m8      imm8
        pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            // `MOV BYTE [addr_size], X`
            auto addr_size = mnemonic_args[0].GetImmSize();
            auto size = addr_size + inst.get_output_size(bit_mode, mnemonic_args);
            return size;
        },
        // 88      m8      r8
        // 88      m16     r8 (m16の場合下位8ビットが使われる)
        pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16), _, TParaToken::ttReg8, _) = [&] {
            auto token = TParaToken(mnemonic_args[0]);
            token.SetAttribute(TParaToken::ttMem8);
            // `MOV [0x0ff0],CH` だと0x0ff0部分を機械語に足す
            auto size = token.GetImmSize() + inst.get_output_size(bit_mode, {token, mnemonic_args[1]});
            return size;
        },
        // C7      m16     imm16
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            // `MOV WORD [addr_size], X`
            auto addr_size = mnemonic_args[0].GetImmSize();
            auto size = addr_size + inst.get_output_size(bit_mode, mnemonic_args);
            return size;
        },
        // 89      m16     r16
        pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // C7      m32     imm32
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            // `MOV DWORD [addr_size], X`
            // TODO: x86 tableの定義に`prefix`の定義がない
            auto override_prefix_size = (bit_mode != ID_32BIT_MODE) ? 1 : 0;
            auto addr_size = mnemonic_args[0].GetImmSize();
            auto size = override_prefix_size + addr_size + inst.get_output_size(bit_mode, mnemonic_args);
            return size;
        },
        // 89      m32     r32
        pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // C7      m64     imm32
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        // 89      m64     r64
        pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] MOV, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processNOP() {
    loc += 1;
    log()->debug("[pass1] LOC = {}({:x})", loc, loc);
    return;
}

void Pass1Strategy::processORG(std::vector<TParaToken>& mnemonic_args) {
    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttHex);
    loc = arg.AsUInt32();
    log()->debug("[pass1] LOC = {}({:x}) +0", std::to_string(loc), loc);
}

void Pass1Strategy::processOR(std::vector<TParaToken>& mnemonic_args) {
    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["OR"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // --
    // 0C      al      imm8
    // 80      r8      imm8
    // 08      r8      r8
    // 0A      r8      m8
    // 0D      ax      imm16
    // 83      r16     imm8
    // 81      r16     imm16
    // 09      r16     r16
    // 0B      r16     m16
    // 0D      eax     imm32
    // 83      r32     imm8
    // 81      r32     imm32
    // 09      r32     r32
    // 0B      r32     m32
    // 0D      rax     imm32
    // 83      r64     imm8
    // 81      r64     imm32
    // 09      r64     r64
    // 0B      r64     m64
    // 80      m8      imm8
    // 08      m8      r8
    // 83      m16     imm8
    // 81      m16     imm16
    // 09      m16     r16
    // 83      m32     imm8
    // 81      m32     imm32
    // 09      m32     r32
    // 83      m64     imm8
    // 81      m64     imm32
    // 09      m64     r64
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    auto inst = iset->instructions().at("OR");

    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg8, "AL", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttMem8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "AX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttMem16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, "EAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, "RAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] OR, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


void Pass1Strategy::processOUT(std::vector<TParaToken>& mnemonic_args) {
    // cat src/json-x86-64/x86_64.json | \
    // jq -r '.instructions["OUT"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // E6      imm8    al
    // E7      imm8    ax
    // E7      imm8    eax
    // EE      dx      al
    // EF      dx      ax
    // EF      dx      eax
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    auto inst = iset->instructions().at("OUT");

    uint32_t l = match(operands)(
        pattern | ds(or_(TParaToken::ttImm, TParaToken::ttLabel), _, TParaToken::ttReg8, "AL") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(or_(TParaToken::ttImm, TParaToken::ttLabel), _, TParaToken::ttReg16, "AX") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(or_(TParaToken::ttImm, TParaToken::ttLabel), _, TParaToken::ttReg32, "EAX") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "DX", TParaToken::ttReg8, "AL") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "DX", TParaToken::ttReg16, "AX") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "DX", TParaToken::ttReg32, "EAX") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] OUT, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );
    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processRET() {
    loc += 1;
    log()->debug("[pass1] LOC = {}({:x})", loc, loc);
    return;
}

void Pass1Strategy::processSHR(std::vector<TParaToken>& mnemonic_args) {
    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["SHR"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // --
    // D2      r8      cl
    // D3      r16     cl
    // D3      r32     cl
    // D3      r64     cl
    // D2      m8      cl
    // D3      m16     cl
    // D3      m32     cl
    // D3      m64     cl
    // C0      r8      imm8
    // C1      r16     imm8
    // C1      r32     imm8
    // C1      r64     imm8
    // C0      m8      imm8
    // C1      m16     imm8
    // C1      m32     imm8
    // C1      m64     imm8
    // D0      r8
    // D1      r16
    // D1      r32
    // D1      r64
    // D0      m8
    // D1      m16
    // D1      m32
    // D1      m64
    auto operands = std::make_tuple(
        std::make_optional(mnemonic_args[0].AsAttr()),
        std::make_optional(mnemonic_args[0].AsString()),
        (mnemonic_args.size() >= 2) ? std::make_optional(mnemonic_args[1].AsAttr()) : std::nullopt,
        (mnemonic_args.size() >= 2) ? std::make_optional(mnemonic_args[1].AsString()) : std::nullopt
    );

    auto inst = iset->instructions().at("SHR");

    uint32_t l = match(operands)(
        pattern | ds(or_(TParaToken::ttReg8,
                         TParaToken::ttReg16,
                         TParaToken::ttReg32,
                         TParaToken::ttReg64,
                         TParaToken::ttMem8,
                         TParaToken::ttMem16,
                         TParaToken::ttMem32,
                         TParaToken::ttMem64), _, TParaToken::ttReg8, "CL") = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(or_(TParaToken::ttReg8,
                         TParaToken::ttReg16,
                         TParaToken::ttReg32,
                         TParaToken::ttReg64,
                         TParaToken::ttMem8,
                         TParaToken::ttMem16,
                         TParaToken::ttMem32,
                         TParaToken::ttMem64), _, TParaToken::ttImm, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(or_(TParaToken::ttReg8,
                         TParaToken::ttReg16,
                         TParaToken::ttReg32,
                         TParaToken::ttReg64,
                         TParaToken::ttMem8,
                         TParaToken::ttMem16,
                         TParaToken::ttMem32,
                         TParaToken::ttMem64), _, std::nullopt, std::nullopt) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] SHR, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}

void Pass1Strategy::processSUB(std::vector<TParaToken>& mnemonic_args) {
    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["SUB"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // --
    // 2C      al      imm8
    // 80      r8      imm8
    // 28      r8      r8
    // 2A      r8      m8
    // 2D      ax      imm16
    // 83      r16     imm8
    // 81      r16     imm16
    // 29      r16     r16
    // 2B      r16     m16
    // 2D      eax     imm32
    // 83      r32     imm8
    // 81      r32     imm32
    // 29      r32     r32
    // 2B      r32     m32
    // 2D      rax     imm32
    // 83      r64     imm8
    // 81      r64     imm32
    // 29      r64     r64
    // 2B      r64     m64
    // 80      m8      imm8
    // 28      m8      r8
    // 83      m16     imm8
    // 81      m16     imm16
    // 29      m16     r16
    // 83      m32     imm8
    // 81      m32     imm32
    // 29      m32     r32
    // 83      m64     imm8
    // 81      m64     imm32
    // 29      m64     r64
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    auto inst = iset->instructions().at("SUB");

    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg8, "AL", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttMem8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "AX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttMem16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, "EAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, "RAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] SUB, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


//
// Visit Opcode系の処理
//
void Pass1Strategy::visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) {

    for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i) {
        (*i)->accept(this);
    }
}

void Pass1Strategy::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
}

//
// expressionの処理
//
void Pass1Strategy::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitDirect(Direct *direct) {
    if (direct->factor_) direct->factor_->accept(this);

    TParaToken t = this->ctx.top();
    log()->debug("[pass1] visitDirect [{}]", t.AsString());

    match(t)(
        pattern | _ | when(t.IsAsmJitGpbLo()) = [&] {
            t.SetAttribute(TParaToken::ttMem8);
        },
        pattern | _ | when(t.IsAsmJitGpbHi()) = [&] {
            t.SetAttribute(TParaToken::ttMem8);
        },
        pattern | _ | when(t.IsAsmJitGpw()) = [&] {
            t.SetAttribute(TParaToken::ttMem16);
        },
        pattern | _ | when(t.IsAsmJitSReg()) = [&] {
            t.SetAttribute(TParaToken::ttMem16);
        },
        pattern | _ | when(t.IsAsmJitGpd()) = [&] {
            t.SetAttribute(TParaToken::ttMem32);
        },
        pattern | _ | when(t.IsImmediate() && t.GetImmSize() == 1) = [&] {
            t.SetAttribute(TParaToken::ttMem8);
        },
        pattern | _ | when(t.IsImmediate() && t.GetImmSize() == 2) = [&] {
            t.SetAttribute(TParaToken::ttMem16);
        },
        pattern | _ | when(t.IsImmediate() && t.GetImmSize() == 4) = [&] {
            t.SetAttribute(TParaToken::ttMem32);
        }
    );

    this->ctx.pop();
    this->ctx.push(t);
};

void Pass1Strategy::visitBasedOrIndexed(BasedOrIndexed *based_or_indexed) {

    visitIdent(based_or_indexed->ident_);
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();

    visitInteger(based_or_indexed->integer_);
    TParaToken right = this->ctx.top();
    right.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    using namespace asmjit;

    log()->debug("[pass1] visitBasedOrIndexed [{} + {}]",
                 left.AsString(), right.AsString());

    match(left)(
        pattern | _ | when(left.IsAsmJitGpbLo()) = [&] {
            left.SetAttribute(TParaToken::ttMem8);
        },
        pattern | _ | when(left.IsAsmJitGpbHi()) = [&] {
            left.SetAttribute(TParaToken::ttMem8);
        },
        pattern | _ | when(left.IsAsmJitGpw()) = [&] {
            left.SetAttribute(TParaToken::ttMem16);
        },
        pattern | _ | when(left.IsAsmJitSReg()) = [&] {
            left.SetAttribute(TParaToken::ttMem16);
        },
        pattern | _ | when(left.IsAsmJitGpd()) = [&] {
            left.SetAttribute(TParaToken::ttMem32);
        }
    );

    this->ctx.push(left);
};

void Pass1Strategy::visitIndexed(Indexed *p) {};
void Pass1Strategy::visitBasedIndexed(BasedIndexed *p) {};
void Pass1Strategy::visitBasedIndexedDisp(BasedIndexedDisp *p) {};
void Pass1Strategy::visitBasedIndexedDispScale(BasedIndexedDispScale *p) {};
void Pass1Strategy::visitIndexScaleExp(IndexScaleExp *p) {};

void Pass1Strategy::visitDatatypeExp(DatatypeExp *datatype_exp) {
    // DataType "[" Exp "]" ; という間接アドレス表現を読み取る
    // left "[" right "]" ; と変数をおいて、属性をTParaTokenに設定する
    if (datatype_exp->datatype_) {
        datatype_exp->datatype_->accept(this);
    }
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttKeyword);
    this->ctx.pop();

    if (datatype_exp->memoryaddr_) {
        datatype_exp->memoryaddr_->accept(this);
    }
    TParaToken right = this->ctx.top();
    this->ctx.pop();

    match(left.AsString())(
        pattern | "BYTE" = [&]{ right.SetAttribute(TParaToken::ttMem8); },
        pattern | "WORD" = [&]{ right.SetAttribute(TParaToken::ttMem16); },
        pattern | "DWORD" = [&]{ right.SetAttribute(TParaToken::ttMem32); },
        pattern | _ = [&] {
            throw std::runtime_error("[pass1] datatype, Not implemented or not matched!!!");
        }
    );

    this->ctx.push(right);
}

void Pass1Strategy::visitSegmentOffsetExp(SegmentOffsetExp *segment_offset_exp) {
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

template void Pass1Strategy::visitDataTypes<ByteDataType>(ByteDataType *p);
template void Pass1Strategy::visitDataTypes<WordDataType>(WordDataType *p);
template void Pass1Strategy::visitDataTypes<DwordDataType>(DwordDataType *p);

template <class T>
void Pass1Strategy::visitDataTypes(T *t) {

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

    log()->debug("[pass1] datatype {}", literal);
    this->ctx.push(TParaToken(literal, TParaToken::ttKeyword));
}

void Pass1Strategy::visitByteDataType(ByteDataType *p) {
    visitDataTypes(p);
}
void Pass1Strategy::visitWordDataType(WordDataType *p) {
    visitDataTypes(p);
}
void Pass1Strategy::visitDwordDataType(DwordDataType *p) {
    visitDataTypes(p);
}

void Pass1Strategy::visitPlusExp(PlusExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitMinusExp(MinusExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitMulExp(MulExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitDivExp(DivExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitModExp(ModExp *p) {
    visitArithmeticOperations(p);
}

template void Pass1Strategy::visitArithmeticOperations<PlusExp>(PlusExp *p);
template void Pass1Strategy::visitArithmeticOperations<MinusExp>(MinusExp *p);
template void Pass1Strategy::visitArithmeticOperations<MulExp>(MulExp *p);
template void Pass1Strategy::visitArithmeticOperations<DivExp>(DivExp *p);
template void Pass1Strategy::visitArithmeticOperations<ModExp>(ModExp *p);


template <class T>
void Pass1Strategy::visitArithmeticOperations(T *exp) {

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
        log()->debug("[pass1] {} = {} + {}", ans, left.AsInt32(), right.AsInt32());
    } else if constexpr (std::is_same_v<T, MinusExp>) {
        ans = left.AsInt32() - right.AsInt32();
        log()->debug("[pass1] {} = {} - {}", ans, left.AsInt32(), right.AsInt32());
    } else if constexpr (std::is_same_v<T, MulExp>) {
        ans = left.AsInt32() * right.AsInt32();
        log()->debug("[pass1] {} = {} * {}", ans, left.AsInt32(), right.AsInt32());
    } else if constexpr (std::is_same_v<T, DivExp>) {
        ans = left.AsInt32() / right.AsInt32();
        log()->debug("[pass1] {} = {} / {}", ans, left.AsInt32(), right.AsInt32());
    } else if constexpr (std::is_same_v<T, ModExp>) {
        ans = left.AsInt32() % right.AsInt32();
        log()->debug("[pass1] {} = {} % {}", ans, left.AsInt32(), right.AsInt32());
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    TParaToken t = TParaToken(std::to_string(ans), TParaToken::ttInteger);
    this->ctx.push(t);
}

//
// factorの処理
//
void Pass1Strategy::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttInteger);
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttHex);
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->ident_);
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

//
// tokenの処理
//
void Pass1Strategy::visitInteger(Integer x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttInteger);
    this->ctx.push(t);
}

void Pass1Strategy::visitChar(Char x) {
    std::string str{x};
    TParaToken t = TParaToken(str, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Pass1Strategy::visitDouble(Double x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttFloating);
    this->ctx.push(t);
}

void Pass1Strategy::visitString(String x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Pass1Strategy::visitIdent(Ident x) {
    if (equ_map.count(x) > 0) {
        // 変数定義があれば展開する
        log()->debug("[pass1] EQU {} = {}", x, equ_map[x].AsString());
        TParaToken t = TParaToken(equ_map[x].AsString(), equ_map[x].AsType());
        this->ctx.push(t);
        return;
    }
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Pass1Strategy::visitHex(Hex x) {
    TParaToken t = TParaToken(x, TParaToken::ttHex);
    this->ctx.push(t);
}

void Pass1Strategy::visitLabel(Label x) {

    std::string label = x.substr(0, x.find(":", 0));

    // ラベルが存在するので、シンボルテーブルのラベルのレコードに現在のLCを設定
    sym_table[label] = loc;

    // LabelJmp::store_label_dst(label, label_dst_list, binout_container);
    // LabelJmp::update_label_dst_offset(label, label_src_list, dollar_position, binout_container);

    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

template <class T>
int Pass1Strategy::Eval(T *parse_tree) {

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

    return 0;
}

template int Pass1Strategy::Eval<Program>(Program* pt);
template int Pass1Strategy::Eval<ListStatement>(ListStatement* pt);
template int Pass1Strategy::Eval<Statement>(Statement* pt);
template int Pass1Strategy::Eval<ListMnemonicArgs>(ListMnemonicArgs* pt);
template int Pass1Strategy::Eval<MnemonicArgs>(MnemonicArgs* pt);
template int Pass1Strategy::Eval<Exp>(Exp* pt);
template int Pass1Strategy::Eval<MemoryAddr>(MemoryAddr* pt);
template int Pass1Strategy::Eval<IndexExp>(IndexExp* pt);
template int Pass1Strategy::Eval<Factor>(Factor* pt);
template int Pass1Strategy::Eval<ConfigType>(ConfigType* pt);
template int Pass1Strategy::Eval<DataType>(DataType* pt);
template int Pass1Strategy::Eval<Opcode>(Opcode* pt);
