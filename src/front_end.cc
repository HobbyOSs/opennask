#include <fstream>
#include <typeinfo>
#include <type_traits>
#include "matchit.h"
#include "front_end.hh"
#include "driver.hh"
#include "parser.hh"
#include "demangle.hpp"
#include "mod_rm.hpp"


using namespace std::placeholders;
using namespace matchit;


FrontEnd::FrontEnd(bool trace_scanning, bool trace_parsing) {

    // spdlog
    if(!spdlog::get("opennask")) {
        auto logger = spdlog::stdout_logger_mt("opennask", "console");
    }

    // lexer, parser
    this->trace_scanning = trace_scanning;
    this->trace_parsing = trace_parsing;

    // nask
    this->dollar_position = 0;
}

FrontEnd::~FrontEnd() {
    // メモリの開放
    equ_map.clear();
    label_dst_list.clear();
    label_dst_list.shrink_to_fit();
    label_src_list.clear();
    label_src_list.shrink_to_fit();
};

std::map<std::string, TParaToken> FrontEnd::equ_map = std::map<std::string, TParaToken>{};
LabelDstList FrontEnd::label_dst_list = LabelDstList{};
LabelSrcList FrontEnd::label_src_list = LabelSrcList{};

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
    } else if (dynamic_cast<OpcodeStmt*>(t) != nullptr) {
        this->visitOpcodeStmt(dynamic_cast<OpcodeStmt*>(t));
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
    } else if (dynamic_cast<IndirectAddrExp*>(t) != nullptr) {
        this->visitIndirectAddrExp(dynamic_cast<IndirectAddrExp*>(t));
    } else if (dynamic_cast<DatatypeExp*>(t) != nullptr) {
        this->visitDatatypeExp(dynamic_cast<DatatypeExp*>(t));
    } else if (dynamic_cast<RangeExp*>(t) != nullptr) {
        this->visitRangeExp(dynamic_cast<RangeExp*>(t));
    } else if (dynamic_cast<ImmExp*>(t) != nullptr) {
        this->visitImmExp(dynamic_cast<ImmExp*>(t));
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
    log()->debug("visitLabelStmt: args = {}", t.to_string());
}


void FrontEnd::visitDeclareStmt(DeclareStmt *declare_stmt) {

    log()->debug("visitDeclareStmt start");
    visitIdent(declare_stmt->ident_);
    TParaToken key = this->ctx.top();
    this->ctx.pop();

    if (declare_stmt->exp_) {
        declare_stmt->exp_->accept(this);
    }
    TParaToken value = this->ctx.top();
    this->ctx.pop();

    log()->debug("declare {} = {}", key.AsString(), value.AsString());
    equ_map[key.AsString()] = value;

    log()->debug("visitDeclareStmt end");
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

    //std::string debug_str = this->join(mnemonic_args->to_string(), ",");
    std::vector<std::string> debug_args;
    std::transform(mnemonic_args.begin(), mnemonic_args.end(),
                   std::back_inserter(debug_args),
                   [](TParaToken x) { return "{ " + x.to_string() + " }"; });

    log()->debug("mnemonic_args=[{}]", this->join(debug_args, ","));

    typedef std::function<void(std::vector<TParaToken>&)> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesADD", std::bind(&FrontEnd::processADD, this, _1)),
        std::make_pair("OpcodesCMP", std::bind(&FrontEnd::processCMP, this, _1)),
        std::make_pair("OpcodesDB", std::bind(&FrontEnd::processDB, this, _1)),
        std::make_pair("OpcodesDW", std::bind(&FrontEnd::processDW, this, _1)),
        std::make_pair("OpcodesDD", std::bind(&FrontEnd::processDD, this, _1)),
        std::make_pair("OpcodesINT", std::bind(&FrontEnd::processINT, this, _1)),
        std::make_pair("OpcodesJAE", std::bind(&FrontEnd::processJAE, this, _1)),
        std::make_pair("OpcodesJB", std::bind(&FrontEnd::processJB, this, _1)),
        std::make_pair("OpcodesJBE", std::bind(&FrontEnd::processJBE, this, _1)),
        std::make_pair("OpcodesJC", std::bind(&FrontEnd::processJC, this, _1)),
        std::make_pair("OpcodesJE", std::bind(&FrontEnd::processJE, this, _1)),
        std::make_pair("OpcodesJMP", std::bind(&FrontEnd::processJMP, this, _1)),
        std::make_pair("OpcodesJNC", std::bind(&FrontEnd::processJNC, this, _1)),
        std::make_pair("OpcodesMOV", std::bind(&FrontEnd::processMOV, this, _1)),
        std::make_pair("OpcodesORG", std::bind(&FrontEnd::processORG, this, _1)),
        std::make_pair("OpcodesRESB", std::bind(&FrontEnd::processRESB, this, _1)),
    };

    const std::string opcode = type(*mnemonic_stmt->opcode_);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second(mnemonic_args);
    } else {
        throw std::runtime_error(opcode + " is not implemented!!!");
    }
}

void FrontEnd::visitOpcodeStmt(OpcodeStmt *opcode_stmt) {
    if (opcode_stmt->opcode_) {
        opcode_stmt->opcode_->accept(this);
    }

    typedef std::function<void()> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesHLT", std::bind(&FrontEnd::processHLT, this)),
    };

    const std::string opcode = type(*opcode_stmt->opcode_);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second();
    } else {
        throw std::runtime_error(opcode + " is not implemented!!!");
    }
}

void FrontEnd::processDB(std::vector<TParaToken>& mnemonic_args) {
    for (const auto& e : mnemonic_args) {
        log()->debug("{}", e.to_string());

        if (e.IsInteger() || e.IsHex()) {
            this->binout_container.push_back(e.AsInt());
        } else if (e.IsIdentifier()) {
            std::string s = e.AsString();
            std::copy(s.begin(), s.end(), std::back_inserter(binout_container));
        }
    }
}

void FrontEnd::processADD(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsString(),
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsAttr()
    );
    std::string dst = mnemonic_args[0].AsString();

    std::vector<uint8_t> machine_codes = match(operands)(
        // 0x04 ib		ADD AL, imm8		imm8をALに加算する
        // 0x05 iw		ADD AX, imm16		imm16をAXに加算する
        // 0x05 id		ADD EAX, imm32		imm32をEAXに加算する
        pattern | ds("AL", TParaToken::ttReg8 , TParaToken::ttImm) = [&] {

            const uint8_t base = 0x04;
            std::vector<uint8_t> b = {base};
            auto imm = mnemonic_args[1].AsUInt8t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds("AX", TParaToken::ttReg16, TParaToken::ttImm) = [&] {

            const uint8_t base = 0x05;
            std::vector<uint8_t> b = {base};
            auto imm = mnemonic_args[1].AsUInt16t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds("EAX", TParaToken::ttReg32, TParaToken::ttImm) = [&] {

            const uint8_t base = 0x05;
            std::vector<uint8_t> b = {base};
            auto imm = mnemonic_args[1].AsUInt32t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },

        // 0x80 /0 ib	ADD r/m8, imm8		imm8をr/m8に加算する
        pattern | ds(_, TParaToken::ttReg8 , TParaToken::ttImm) = [&] {

            const uint8_t base = 0x80;
            const uint8_t modrm = ModRM::generate_modrm(ModRM::REG, dst, ModRM::SLASH_0);
            std::vector<uint8_t> b = {base, modrm};
            auto imm = mnemonic_args[1].AsUInt8t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        // 0x81 /0 iw	ADD r/m16, imm16	imm16をr/m16に加算する
        // 0x83 /0 ib	ADD r/m16, imm8		符号拡張imm8をr/m16に加算する
        pattern | ds(_, TParaToken::ttReg16, TParaToken::ttImm) = [&] {

            uint8_t base = 0x81;
            uint8_t modrm = ModRM::generate_modrm(ModRM::REG, dst, ModRM::SLASH_0);

            if (mnemonic_args[1].AsLong() <= 255) {
                base = 0x83;
                std::vector<uint8_t> b = {base, modrm};
                auto imm = mnemonic_args[1].AsUInt8t();
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
                return b;
            } else {
                base = 0x81;
                std::vector<uint8_t> b = {base, modrm};
                auto imm = mnemonic_args[1].AsUInt16t();
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
                return b;
            }
        },
        // 0x81 /0 id	ADD r/m32, imm32	imm32をr/m32に加算する
        // 0x83 /0 ib	ADD r/m32, imm8		符号拡張imm8をr/m32に加算する
        pattern | ds(_, TParaToken::ttReg32, TParaToken::ttImm) = [&] {

            uint8_t base = 0x00;
            uint8_t modrm = ModRM::generate_modrm(ModRM::REG, dst, ModRM::SLASH_0);

            if (mnemonic_args[1].AsLong() <= 255) {
                base = 0x83;
                std::vector<uint8_t> b = {base, modrm};
                auto imm = mnemonic_args[1].AsUInt8t();
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
                return b;
            } else {
                base = 0x81;
                std::vector<uint8_t> b = {base, modrm};
                auto imm = mnemonic_args[1].AsUInt32t();
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
                return b;
            }
        },


        // 0x00 /r		ADD r/m8, r8		r8をr/m8に加算する
        // 0x01 /r		ADD r/m16, r16		r16をr/m16に加算する
        // 0x01 /r		ADD r/m32, r32		r32をr/m32に加算する
        // 0x02 /r		ADD r8, r/m8		r/m8をr8に加算する
        // 0x03 /r		ADD r16, r/m16		r/m16をr16に加算する
        // 0x03 /r		ADD r32, r/m32		r/m32をr32に加算する

        pattern | _ = [&] {
            throw std::runtime_error("ADD, Not implemented or not matched!!!");
            return std::vector<uint8_t>();
        }
    );

    // 結果を投入
    binout_container.insert(binout_container.end(), std::begin(machine_codes), std::end(machine_codes));
    return;
}

void FrontEnd::processCMP(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsString(),
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsAttr()
    );
    std::string dst = mnemonic_args[0].AsString();
    log()->debug("processCMP dst={}", dst);

    std::vector<uint8_t> machine_codes = match(operands)(
        // 0x3C ib		CMP AL, imm8		imm8をALと比較します
        // 0x3D iw		CMP AX, imm16		imm16をAXと比較します
        // 0x3D id		CMP EAX, imm32		imm32をEAXと比較します
        pattern | ds("AL", TParaToken::ttReg8, TParaToken::ttImm) = [&] {

            const uint8_t base = 0x3c;
            std::vector<uint8_t> b = {base};
            auto imm = mnemonic_args[1].AsUInt8t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds("AX", TParaToken::ttReg16, TParaToken::ttImm) = [&] {

            const uint8_t base = 0x3d;
            std::vector<uint8_t> b = {base};
            auto imm = mnemonic_args[1].AsUInt16t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds("EAX", TParaToken::ttReg32, TParaToken::ttImm) = [&] {

            const uint8_t base = 0x3d;
            std::vector<uint8_t> b = {base};
            auto imm = mnemonic_args[1].AsUInt32t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },

        // 0x80 /7 ib	CMP r/m8, imm8		imm8をr/m8と比較します
        // 0x81 /7 iw	CMP r/m16, imm16	imm16をr/m16と比較します <-- ?
        // 0x80 /7 id	CMP r/m32, imm32	imm32をr/m32と比較します <-- ?
        // 0x83 /7 ib	CMP r/m16, imm8		imm8をr/m16と比較します
        // 0x83 /7 ib	CMP r/m32, imm8		imm8をr/m32と比較します
        pattern | ds(_, TParaToken::ttReg8, TParaToken::ttImm) = [&] {

            const std::string src = mnemonic_args[0].AsString();
            const uint8_t base = 0x80;
            const uint8_t modrm = ModRM::generate_modrm(ModRM::REG, src, ModRM::SLASH_7);
            std::vector<uint8_t> b = {base, modrm};
            auto imm = mnemonic_args[1].AsUInt8t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds(_, TParaToken::ttReg16, TParaToken::ttImm) = [&] {

            const std::string src = mnemonic_args[0].AsString();
            const uint8_t base = 0x83;
            const uint8_t modrm = ModRM::generate_modrm(ModRM::REG, src, ModRM::SLASH_7);
            std::vector<uint8_t> b = {base, modrm};
            auto imm = mnemonic_args[1].AsUInt8t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds(_, TParaToken::ttReg32, TParaToken::ttImm) = [&] {

            const std::string src = mnemonic_args[0].AsString();
            const uint8_t base = 0x83;
            const uint8_t modrm = ModRM::generate_modrm(ModRM::REG, src, ModRM::SLASH_7);
            std::vector<uint8_t> b = {base, modrm};
            auto imm = mnemonic_args[1].AsUInt8t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },

        // 0x38 /r		CMP r/m8, r8		r8をr/m8と比較します
        // 0x39 /r		CMP r/m16, r16		r16をr/m16と比較します
        // 0x39 /r		CMP r/m32, r32		r32をr/m32と比較します
        // 0x3A /r		CMP r8, r/m8		r/m8をr8と比較します
        // 0x3B /r		CMP r16, r/m16		r/m16をr16と比較します
        // 0x3B /r		CMP r32, r/m32		r/m32をr32と比較します

        pattern | _ = [&] {
            throw std::runtime_error("CMP, Not implemented or not matched!!!");
            return std::vector<uint8_t>();
        }
    );

    // 結果を投入
    binout_container.insert(binout_container.end(), std::begin(machine_codes), std::end(machine_codes));
    return;
}

void FrontEnd::processDW(std::vector<TParaToken>& mnemonic_args) {
    // uint16_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
    for (const auto& e : mnemonic_args) {
        log()->debug("{}", e.to_string());

        if (e.IsInteger() || e.IsHex()) {
            uint16_t word = e.AsInt();
            std::vector<uint8_t> bytes = {
                static_cast<uint8_t>( (word >> 8) & 0xff ),
                static_cast<uint8_t>( word & 0xff ),
            };
            // リトルエンディアンなので逆順コピー
            std::reverse_copy(bytes.begin(), bytes.end(), std::back_inserter(binout_container));

        } else if (e.IsIdentifier()) {
            throw std::runtime_error("not implemented");
        }
    }
}

void FrontEnd::processDD(std::vector<TParaToken>& mnemonic_args) {
    // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
    for (const auto& e : mnemonic_args) {
        log()->debug("{}", e.to_string());

        if (e.IsInteger() || e.IsHex()) {
            uint32_t dword = e.AsLong();
            std::vector<uint8_t> bytes = {
                static_cast<uint8_t>( (dword >> 24) & 0xff ),
                static_cast<uint8_t>( (dword >> 16) & 0xff ),
                static_cast<uint8_t>( (dword >> 8)  & 0xff ),
                static_cast<uint8_t>( dword & 0xff ),
            };
            // リトルエンディアンなので逆順コピー
            std::reverse_copy(bytes.begin(), bytes.end(), std::back_inserter(binout_container));

        } else if (e.IsIdentifier()) {
            throw std::runtime_error("not implemented");
        }
    }
}

void FrontEnd::processRESB(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    const std::string suffix = "-$";

    if (auto range = arg.AsString(); range.find(suffix) != std::string::npos) {
        log()->debug("type: {}, value: {}", type(arg), arg.to_string());
        auto resb_size = range.substr(0, range.length() - suffix.length());
        auto resb_token = TParaToken(resb_size, TParaToken::ttHex);

        std::vector<uint8_t> resb(resb_token.AsLong() - dollar_position - binout_container.size(), 0);
        log()->debug("padding upto: {}(={}), current: {}",
                     resb_token.AsString(), resb_token.AsLong(), binout_container.size());
        binout_container.insert(binout_container.end(), std::begin(resb), std::end(resb));
        return;
    }

    arg.MustBe(TParaToken::ttInteger);
    log()->debug("type: {}, value: {}", type(arg), arg.AsLong());

    std::vector<uint8_t> resb(arg.AsLong(), 0);
    binout_container.insert(binout_container.end(), std::begin(resb), std::end(resb));
}

void FrontEnd::processHLT() {
    binout_container.push_back(0xf4);
}

void FrontEnd::processINT(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttHex);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    binout_container.push_back(0xcd);
    binout_container.push_back(arg.AsInt());
}

void FrontEnd::processJAE(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();

    if (LabelJmp::dst_is_stored(label, label_dst_list)) {
        LabelJmp::update_label_src_offset(label, label_dst_list, 0x73, binout_container);
    } else {
        LabelJmp::store_label_src(label, label_src_list, binout_container);
        binout_container.push_back(0x73);
        binout_container.push_back(0x00);
        log()->debug("bin[{}] = 0x73, bin[{}] = 0x00", binout_container.size() - 1, binout_container.size());
    }
}

void FrontEnd::processJB(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();

    if (LabelJmp::dst_is_stored(label, label_dst_list)) {
        LabelJmp::update_label_src_offset(label, label_dst_list, 0x72, binout_container);
    } else {
        LabelJmp::store_label_src(label, label_src_list, binout_container);
        binout_container.push_back(0x72);
        binout_container.push_back(0x00);
        log()->debug("bin[{}] = 0x72, bin[{}] = 0x00", binout_container.size() - 1, binout_container.size());
    }
}

void FrontEnd::processJBE(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();

    if (LabelJmp::dst_is_stored(label, label_dst_list)) {
        LabelJmp::update_label_src_offset(label, label_dst_list, 0x76, binout_container);
    } else {
        LabelJmp::store_label_src(label, label_src_list, binout_container);
        binout_container.push_back(0x76);
        binout_container.push_back(0x00);
        log()->debug("bin[{}] = 0x76, bin[{}] = 0x00", binout_container.size() - 1, binout_container.size());
    }
}

void FrontEnd::processJC(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();

    if (LabelJmp::dst_is_stored(label, label_dst_list)) {
        LabelJmp::update_label_src_offset(label, label_dst_list, 0x72, binout_container);
    } else {
        LabelJmp::store_label_src(label, label_src_list, binout_container);
        binout_container.push_back(0x72);
        binout_container.push_back(0x00);
        log()->debug("bin[{}] = 0x72, bin[{}] = 0x00", binout_container.size() - 1, binout_container.size());
    }
}

void FrontEnd::processJE(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();

    if (LabelJmp::dst_is_stored(label, label_dst_list)) {
        LabelJmp::update_label_src_offset(label, label_dst_list, 0x74, binout_container);
    } else {
        LabelJmp::store_label_src(label, label_src_list, binout_container);
        binout_container.push_back(0x74);
        binout_container.push_back(0x00);
        log()->debug("bin[{}] = 0x74, bin[{}] = 0x00", binout_container.size() - 1, binout_container.size());
    }
}

void FrontEnd::processJMP(std::vector<TParaToken>& mnemonic_args) {

    // 0xEB cb	JMP rel8	次の命令との相対オフセットだけ相対ショートジャンプする
    // 0xE9 cw	JMP rel16	次の命令との相対オフセットだけ相対ニアジャンプする
    // 0xE9 cd	JMP rel32	次の命令との相対オフセットだけ相対ニアジャンプする
    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].GetImmSize()
    );
    auto arg = mnemonic_args[0];

    std::vector<uint8_t> machine_codes = match(operands)(
        // 即値処理
        pattern | ds(TParaToken::ttImm, 1) = [&] {
            std::vector<uint8_t> b = {0xeb};
            const auto byte = (arg.AsLong() - dollar_position - binout_container.size()) + 1;
            auto jmp_offset = IntAsByte(byte);
            std::copy(jmp_offset.begin(), jmp_offset.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds(TParaToken::ttImm, 2) = [&] {
            std::vector<uint8_t> b = {0xe9};
            const auto word = (arg.AsLong() - dollar_position - binout_container.size()) + 1;
            auto jmp_offset = IntAsWord(word);
            std::copy(jmp_offset.begin(), jmp_offset.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds(TParaToken::ttImm, 4) = [&] {
            std::vector<uint8_t> b = {0xe9};
            const auto dword = (arg.AsLong() - dollar_position - binout_container.size()) + 1;
            auto jmp_offset = LongAsDword(dword);
            std::copy(jmp_offset.begin(), jmp_offset.end(), std::back_inserter(b));
            return b;
        },
        // ラベル処理
        pattern | ds(TParaToken::ttLabel, _) = [&] {
            log()->debug("type: {}, value: {}", type(arg), arg.AsString());
            std::string label = arg.AsString();

            if (LabelJmp::dst_is_stored(label, label_dst_list)) {
                LabelJmp::update_label_src_offset(label, label_dst_list, 0xeb, binout_container);
            } else {
                LabelJmp::store_label_src(label, label_src_list, binout_container);
                binout_container.push_back(0xeb);
                binout_container.push_back(0x00);
                log()->debug("bin[{}] = 0xeb, bin[{}] = 0x00", binout_container.size() - 1, binout_container.size());
            }

            return std::vector<uint8_t>();
        },
        pattern | _ = [&] {
            throw std::runtime_error("JMP, Not implemented or not matched!!!");
            return std::vector<uint8_t>();
        }
    );

    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(machine_codes),
                            std::end(machine_codes));
    return;
}

void FrontEnd::processJNC(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();

    if (LabelJmp::dst_is_stored(label, label_dst_list)) {
        LabelJmp::update_label_src_offset(label, label_dst_list, 0x73, binout_container);
    } else {
        LabelJmp::store_label_src(label, label_src_list, binout_container);
        binout_container.push_back(0x73);
        binout_container.push_back(0x00);
        log()->debug("bin[{}] = 0x73, bin[{}] = 0x00", binout_container.size() - 1, binout_container.size());
    }
}

void FrontEnd::processMOV(std::vector<TParaToken>& mnemonic_args) {

    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsAttr()
    );

    std::vector<uint8_t> machine_codes = match(operands)(
        //         0x88 /r	MOV r/m8   , r8
        // REX   + 0x88 /r	MOV r/m8   , r8
        pattern | ds(TParaToken::ttMem , TParaToken::ttReg8) = [&] {
            const std::string dst_mem = "[" + mnemonic_args[0].AsString() + "]";
            const std::string src_reg = mnemonic_args[1].AsString();

            const uint8_t base = 0x88;
            const uint8_t modrm = ModRM::generate_modrm(base, ModRM::REG_REG, dst_mem, src_reg);
            std::vector<uint8_t> b = {base, modrm};
            auto imm = mnemonic_args[0].AsUInt16t(); // TODO: int16で返しているが実際は可変なのでちゃんと処理する
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },

        //         0x89 /r	MOV r/m16  , r16
        //         0x89 /r	MOV r/m32  , r32
        // REX.W + 0x89 /r	MOV r/m64  , r64

        //         0x8A /r	MOV r8     , r/m8
        // REX   + 0x8A /r	MOV r8     , r/m8
        //         0x8B /r	MOV r16    , r/m16
        //         0x8B /r	MOV r32    , r/m32
        // REX.W + 0x8B /r	MOV r64    , r/m64
        pattern | ds(TParaToken::ttReg8 , TParaToken::ttMem) = [&] {
            const std::string src_mem = "[" + mnemonic_args[1].AsString() + "]";
            const std::string dst_reg = mnemonic_args[0].AsString();

            const uint8_t base = 0x8a;
            const uint8_t modrm = ModRM::generate_modrm(0x8e, ModRM::REG_REG, src_mem, dst_reg);
            std::vector<uint8_t> b = {base, modrm};
            return b;
        },
        pattern | ds(TParaToken::ttReg16, TParaToken::ttMem) = [&] {
            const std::string src_mem = "[" + mnemonic_args[1].AsString() + "]";
            const std::string dst_reg = mnemonic_args[0].AsString();

            const uint8_t base = 0x8b;
            const uint8_t modrm = ModRM::generate_modrm(0x8e, ModRM::REG_REG, src_mem, dst_reg);
            std::vector<uint8_t> b = {base, modrm};
            return b;
        },
        pattern | ds(TParaToken::ttReg32, TParaToken::ttMem) = [&] {
            const std::string src_mem = "[" + mnemonic_args[1].AsString() + "]";
            const std::string dst_reg = mnemonic_args[0].AsString();

            const uint8_t base = 0x8b;
            const uint8_t modrm = ModRM::generate_modrm(0x8e, ModRM::REG_REG, src_mem, dst_reg);
            std::vector<uint8_t> b = {base, modrm};
            return b;
        },

        //         0x8C /r	MOV r/m16  , Sreg
        // REX.W + 0x8C /r	MOV r/m16  , Sreg
        pattern | ds(TParaToken::ttReg16 , TParaToken::ttSegReg) = [&] {
            const std::string src = mnemonic_args[0].AsString();
            const std::string dst = mnemonic_args[1].AsString();

            const uint8_t base = 0x8c;
            const uint8_t modrm = ModRM::generate_modrm(0x8c, ModRM::REG, dst, src);
            std::vector<uint8_t> b = {base, modrm};
            return b;
        },

        //         0x8E /r	MOV Sreg   , r/m16
        // REX.W + 0x8E /r	MOV Sreg   , r/m64
        pattern | ds(TParaToken::ttSegReg , _) = [&] {
            const std::string src = mnemonic_args[0].AsString();
            const std::string dst = mnemonic_args[1].AsString();

            const uint8_t base = 0x8e;
            const uint8_t modrm = ModRM::generate_modrm(0x8e, ModRM::REG, dst, src);
            std::vector<uint8_t> b = {base, modrm};
            return b;
        },

        //         0xA0	    MOV AL     , moffs8
        // REX.W + 0xA0	    MOV AL     , moffs8
        //         0xA1	    MOV AX     , moffs16
        //         0xA1	    MOV EAX    , moffs32
        // REX.W + 0xA1	    MOV RAX    , moffs64
        //         0xA2	    MOV moffs8 , AL
        // REX.W + 0xA2	    MOV moffs8 , AL
        //         0xA3	    MOV moffs16, AX
        //		   0xA3		MOV moffs32, EAX
        // REX.W + 0xA3		MOV moffs64, RAX



        //         0xB0+rb	MOV r8     , imm8
        // REX   + 0xB0+rb	MOV r8     , imm8
        //         0xB8+rw	MOV r16    , imm16
        //         0xB8+rd	MOV r32    , imm32
        pattern | ds(TParaToken::ttReg8 , TParaToken::ttImm) = [&] {
            const std::string src = mnemonic_args[0].AsString();
            const std::string dst = mnemonic_args[1].AsString();

            const uint8_t base = 0xb0;
            const uint8_t opcode = ModRM::get_opecode_from_reg(base, src);
            std::vector<uint8_t> b = {opcode};

            if (std::get<1>(operands) == TParaToken::ttLabel) {
                // immがラベルだった場合は後でオフセットを計算する
                if (LabelJmp::dst_is_stored(dst, label_dst_list)) {
                    LabelJmp::update_label_src_offset(dst, label_dst_list, opcode, binout_container);
                } else {
                    LabelJmp::store_label_src(dst, label_src_list, binout_container);
                }
                auto imm = std::array<uint8_t, 1>{0x00};
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            } else {
                auto imm = mnemonic_args[1].AsUInt8t();
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            }
            return b;
        },
        pattern | ds(TParaToken::ttReg16, or_(TParaToken::ttImm, TParaToken::ttLabel)) = [&] {
            const std::string src = mnemonic_args[0].AsString();
            const std::string dst = mnemonic_args[1].AsString();
            const uint8_t base = 0xb8;
            const uint8_t opcode = ModRM::get_opecode_from_reg(base, src);
            std::vector<uint8_t> b = {opcode};

            if (std::get<1>(operands) == TParaToken::ttLabel) {
                // immがラベルだった場合は後でオフセットを計算する
                if (LabelJmp::dst_is_stored(dst, label_dst_list)) {
                    LabelJmp::update_label_src_offset(dst, label_dst_list, opcode, binout_container);
                } else {
                    LabelJmp::store_label_src(dst, label_src_list, binout_container, true, imm16);
                }
                auto imm = std::array<uint8_t, 2>{0x00, 0x00};
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            } else {
                auto imm = mnemonic_args[1].AsUInt16t();
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            }

            return b;
        },
        pattern | ds(TParaToken::ttReg32, TParaToken::ttImm) = [&] {
            const std::string src = mnemonic_args[0].AsString();
            const std::string dst = mnemonic_args[1].AsString();

            const uint8_t base = 0xb8;
            const uint8_t opcode = ModRM::get_opecode_from_reg(base, src);
            std::vector<uint8_t> b = {opcode};

            if (std::get<1>(operands) == TParaToken::ttLabel) {
                // immがラベルだった場合は後でオフセットを計算する
                if (LabelJmp::dst_is_stored(dst, label_dst_list)) {
                    LabelJmp::update_label_src_offset(dst, label_dst_list, opcode, binout_container);
                } else {
                    LabelJmp::store_label_src(dst, label_src_list, binout_container, true, imm32);
                }
                auto imm = std::array<uint8_t, 4>{0x00, 0x00};
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            } else {
                auto imm = mnemonic_args[1].AsUInt32t();
                std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            }
            return b;
        },

        // REX.W + 0xB8+rd	MOV r64    , imm64
        //         0xC6 /0	MOV r/m8   , imm8
        // REX.W + 0xC6 /0	MOV r/m8   , imm8
        //         0xC7 /0	MOV r/m16  , imm16
        //         0xC7 /0	MOV r/m32  , imm32
        // REX.W + 0xC7 /0	MOV r/m64  , imm64
        pattern | ds(TParaToken::ttMem8, TParaToken::ttImm) = [&] {
            std::string dst = "[" + mnemonic_args[0].AsString() + "]";
            const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, dst, ModRM::SLASH_0);

            std::vector<uint8_t> b = {0xc6, modrm};
            auto addr = mnemonic_args[0].AsUInt16t();
            std::copy(addr.begin(), addr.end(), std::back_inserter(b));
            auto imm = mnemonic_args[1].AsUInt8t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds(TParaToken::ttMem16, TParaToken::ttImm) = [&] {
            std::string dst = "[" + mnemonic_args[0].AsString() + "]";
            const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, dst, ModRM::SLASH_0);

            std::vector<uint8_t> b = {0xc7, modrm};
            auto addr = mnemonic_args[0].AsUInt16t();
            std::copy(addr.begin(), addr.end(), std::back_inserter(b));
            auto imm = mnemonic_args[1].AsUInt16t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | ds(TParaToken::ttMem32, TParaToken::ttImm) = [&] {
            std::string dst = "[" + mnemonic_args[0].AsString() + "]";
            const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, dst, ModRM::SLASH_0);

            // Override prefixes(0x66)
            // TODO: 16bit命令モードで動作中のみ0x66を付与するようにする
            std::vector<uint8_t> b = {0x66, 0xc7, modrm};
            auto addr = mnemonic_args[0].AsUInt16t();
            std::copy(addr.begin(), addr.end(), std::back_inserter(b));
            auto imm = mnemonic_args[1].AsUInt32t();
            std::copy(imm.begin(), imm.end(), std::back_inserter(b));
            return b;
        },
        pattern | _ = [&] {
            throw std::runtime_error("MOV, Not implemented or not matched!!!");
            return std::vector<uint8_t>();
        }
    );

    // 結果を投入
    binout_container.insert(binout_container.end(), std::begin(machine_codes), std::end(machine_codes));
    return;
}

void FrontEnd::processORG(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttHex);
    log()->debug("type: {}, value: {}", type(arg), arg.AsLong());
    dollar_position = arg.AsLong();
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

void FrontEnd::visitIndirectAddrExp(IndirectAddrExp *indirect_addr_exp) {
    if (indirect_addr_exp->exp_) {
        indirect_addr_exp->exp_->accept(this);
    }
    // [SI] のような間接アドレス表現を読み取る
    TParaToken t = this->ctx.top();
    t.SetAttribute(TParaToken::ttMem);
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitDatatypeExp(DatatypeExp *datatype_exp) {

    // DataType "[" Exp "]" ; という間接アドレス表現を読み取る
    // left "[" right "]" ; と変数をおいて、属性をTParaTokenに設定する
    if (datatype_exp->datatype_) {
        datatype_exp->datatype_->accept(this);
    }
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttKeyword);
    this->ctx.pop();

    if (datatype_exp->exp_) {
        datatype_exp->exp_->accept(this);
    }
    TParaToken right = this->ctx.top();
    this->ctx.pop();

    match(left.AsString())(
        pattern | "BYTE" = [&]{ right.SetAttribute(TParaToken::ttMem8); },
        pattern | "WORD" = [&]{ right.SetAttribute(TParaToken::ttMem16); },
        pattern | "DWORD" = [&]{ right.SetAttribute(TParaToken::ttMem32); },
        pattern | _ = [&] {
            throw std::runtime_error("datatype, Not implemented or not matched!!!");
        }
    );

    this->ctx.push(right);
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

    log()->debug("datatype {}", literal);
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
    left.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    if (exp->exp_2) {
        exp->exp_2->accept(this);
    }
    TParaToken right = this->ctx.top();
    right.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    long ans = 0;
    if constexpr (std::is_same_v<T, PlusExp>) {
        ans = left.AsLong() + right.AsLong();
    } else if constexpr (std::is_same_v<T, MinusExp>) {
        ans = left.AsLong() - right.AsLong();
    } else if constexpr (std::is_same_v<T, MulExp>) {
        ans = left.AsLong() * right.AsLong();
    } else if constexpr (std::is_same_v<T, DivExp>) {
        ans = left.AsLong() / right.AsLong();
    } else if constexpr (std::is_same_v<T, ModExp>) {
        ans = left.AsLong() % right.AsLong();
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
    visitIdent(ident_factor->ident_);
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
        log()->debug("EQU {} = {}", x, equ_map[x].AsString());
        TParaToken t = TParaToken(equ_map[x].AsString(), equ_map[x].AsType());
        this->ctx.push(t);
        return;
    }
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitHex(Hex x) {
    TParaToken t = TParaToken(x, TParaToken::ttHex);
    this->ctx.push(t);
}

void FrontEnd::visitLabel(Label x) {

    std::string label = x.substr(0, x.find(":", 0));
    log()->debug("label='{}' binout_container[{}]",
                 label, std::to_string(this->binout_container.size()));

    // label: (label_dstと呼ぶ)
    // 1) label_dstの位置を記録する → label_dst_list
    // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了
    LabelJmp::store_label_dst(label, label_dst_list, binout_container);
    LabelJmp::update_label_dst_offset(label, label_src_list, dollar_position, binout_container);

    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

const std::string FrontEnd::join(std::vector<std::string>& array, const std::string& sep) {

    std::stringstream ss;
    for(size_t i = 0; i < array.size(); ++i) {
        if(i != 0) {
            ss << sep;
        }
        ss << array[i];
    }
    return ss.str();
}

const std::array<uint8_t, 1> FrontEnd::IntAsByte(const int v) {
    return std::array<uint8_t, 1>{static_cast<uint8_t>(v)};
}

const std::array<uint8_t, 2> FrontEnd::IntAsWord(const int word) {
    return std::array<uint8_t, 2>{
        static_cast<uint8_t>( word & 0xff ),
        static_cast<uint8_t>( (word >> 8) & 0xff ),
    };
}

const std::array<uint8_t, 4> FrontEnd::LongAsDword(const long dword) {
    return std::array<uint8_t, 4>{
        static_cast<uint8_t>( dword & 0xff ),
        static_cast<uint8_t>( (dword >> 8)  & 0xff ),
        static_cast<uint8_t>( (dword >> 16) & 0xff ),
        static_cast<uint8_t>( (dword >> 24) & 0xff ),
    };
}

template <class T>
std::shared_ptr<T> FrontEnd::Parse(std::istream &input) {

    std::shared_ptr<T> parse_tree = nullptr;
    auto driver = std::make_unique<NaskDriver>();

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
template std::shared_ptr<Factor> FrontEnd::Parse<Factor>(std::istream &input);
template std::shared_ptr<ConfigType> FrontEnd::Parse<ConfigType>(std::istream &input);
template std::shared_ptr<DataType> FrontEnd::Parse<DataType>(std::istream &input);
template std::shared_ptr<Opcode> FrontEnd::Parse<Opcode>(std::istream &input);


template <class T>
int FrontEnd::Eval(T *parse_tree, const char* assembly_dst) {

    std::ofstream binout(assembly_dst, std::ios::trunc | std::ios::binary);
    if ( binout.bad() || binout.fail() ) {
        std::cerr << "NASK : can't open " << assembly_dst << std::endl;
        return 17;
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

    // output binary
    binout.write(
        reinterpret_cast<char*>(binout_container.data()), binout_container.size()
    );
    binout.close();

    return 0;
}

template int FrontEnd::Eval<Program>(Program* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ListStatement>(ListStatement* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Statement>(Statement* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ListMnemonicArgs>(ListMnemonicArgs* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<MnemonicArgs>(MnemonicArgs* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Exp>(Exp* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Factor>(Factor* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ConfigType>(ConfigType* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<DataType>(DataType* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Opcode>(Opcode* parse_tree, const char* assembly_dst);