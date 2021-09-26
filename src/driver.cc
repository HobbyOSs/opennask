#include <fstream>
#include <typeinfo>
#include <type_traits>
#include "matchit.h"
#include "driver.hh"
#include "parser.hh"
#include "demangle.hpp"
#include "mod_rm.hpp"


using namespace std::placeholders;

Driver::Driver(bool trace_scanning, bool trace_parsing) {

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

// 以下、抽象クラスの実装(内部で動的に分岐)
void Driver::visitProgram(Program *t) {

    if (dynamic_cast<Prog*>(t) != nullptr) {
        this->visitProg(dynamic_cast<Prog*>(t));
    }
}

void Driver::visitStatement(Statement *t) {

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

void Driver::visitMnemonicArgs(MnemonicArgs *t) {

    if (dynamic_cast<MnemoArg*>(t) != nullptr) {
        this->visitMnemoArg(dynamic_cast<MnemoArg*>(t));
    }
}

void Driver::visitExp(Exp *t) {

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

void Driver::visitFactor(Factor *t) {

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

void Driver::visitConfigType(ConfigType *t) {
}

void Driver::visitDataType(DataType *t) {
}

void Driver::visitOpcode(Opcode *t) {
}

// テストのため各種の型をエントリーポイントとしてパースできるようにしている
template int Driver::Parse<Program>(FILE* in, char const* dst);
template int Driver::Parse<Program>(const char* in, char const* dst);
template int Driver::Parse<ListStatement>(FILE* in, char const* dst);
template int Driver::Parse<ListStatement>(const char* in, char const* dst);
template int Driver::Parse<Statement>(FILE* in, char const* dst);
template int Driver::Parse<Statement>(const char* in, char const* dst);
template int Driver::Parse<ListMnemonicArgs>(FILE* in, char const* dst);
template int Driver::Parse<ListMnemonicArgs>(const char* in, char const* dst);
template int Driver::Parse<MnemonicArgs>(FILE* in, char const* dst);
template int Driver::Parse<MnemonicArgs>(const char* in, char const* dst);
template int Driver::Parse<Exp>(FILE* in, char const* dst);
template int Driver::Parse<Exp>(const char* in, char const* dst);
template int Driver::Parse<Factor>(FILE* in, char const* dst);
template int Driver::Parse<Factor>(const char* in, char const* dst);
template int Driver::Parse<ConfigType>(FILE* in, char const* dst);
template int Driver::Parse<ConfigType>(const char* in, char const* dst);
template int Driver::Parse<DataType>(FILE* in, char const* dst);
template int Driver::Parse<DataType>(const char* in, char const* dst);
template int Driver::Parse<Opcode>(FILE* in, char const* dst);
template int Driver::Parse<Opcode>(const char* in, char const* dst);


template <class T, class IN>
int Driver::Parse(IN input, const char* assembly_dst) {

    T* parse_tree = NULL;

    try {
        if constexpr (std::is_same_v<T, Program>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pProgram(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psProgram(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, ListStatement>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pListStatement(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psListStatement(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, Statement>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pStatement(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psStatement(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pListMnemonicArgs(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psListMnemonicArgs(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, MnemonicArgs>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pMnemonicArgs(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psMnemonicArgs(input);
            }
        }
        else if constexpr (std::is_same_v<T, Exp>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pExp(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psExp(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, Factor>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pFactor(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psFactor(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, ConfigType>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pConfigType(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psConfigType(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, DataType>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pDataType(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psDataType(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, Opcode>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pOpcode(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psOpcode(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
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
            std::unique_ptr<ShowAbsyn> s(new ShowAbsyn());
            printf("%s\n\n", s->show(parse_tree));
        }
        if (trace_parsing) {
            printf("[Linearized Tree]\n");
            std::unique_ptr<PrintAbsyn> p(new PrintAbsyn());
            printf("%s\n\n", p->print(parse_tree));
        }

        this->Eval<T>(parse_tree, assembly_dst);
        log()->debug("parse_tree is: {}", type(parse_tree));

        this->Delete<T>(parse_tree);

        return 0;
    }
    return 1;
}


template <class T>
void Driver::Delete(T *pt) {

    if (auto prog = dynamic_cast<Prog*>(pt); prog != nullptr) {

        log()->trace("success cast {}", type(prog));
        for (auto stmt : *prog->liststatement_) {
            log()->trace("iterate under prog {}", type(stmt));
            this->Delete<Statement>(stmt);
        }
        delete(prog->liststatement_); //deleteできない
        delete(prog);

    } else if (auto stmt = dynamic_cast<Statement*>(pt); stmt != nullptr) {

        log()->trace("success cast {}", type(stmt));

        if (auto t = dynamic_cast<LabelStmt*>(stmt); t != nullptr) {
            log()->trace("success cast {}", type(t));
            log()->trace("delete {}", type(t->label_));
            delete(t);

        } else if (auto t = dynamic_cast<DeclareStmt*>(stmt); t != nullptr) {
            // NOP
        } else if (auto t = dynamic_cast<ConfigStmt*>(stmt); t != nullptr) {
            // NOP
        } else if (auto t = dynamic_cast<MnemonicStmt*>(stmt); t != nullptr) {
            log()->trace("success cast {}", type(t));

            delete(t->opcode_);
            log()->trace("delete {}", type(t->opcode_));
            for (auto arg : *t->listmnemonicargs_) {
                log()->trace("iterate under mnemonic_stmt {}", type(arg));
                this->Delete<MnemonicArgs>(arg);
                delete(arg);
            }
            log()->trace("delete {}", type(t->listmnemonicargs_));
            delete(t->listmnemonicargs_);
            log()->trace("delete {}", type(t));
            delete(t);

        } else if (auto t = dynamic_cast<OpcodeStmt*>(stmt); t != nullptr) {
            log()->trace("success cast {}", type(t));
            delete(t->opcode_);
            log()->trace("delete {}", type(t->opcode_));
        }

    } else if (auto args = dynamic_cast<MnemonicArgs*>(pt); args != nullptr) {

        if (auto arg = dynamic_cast<MnemoArg*>(args); arg != nullptr) {
            log()->trace("success cast {}", type(arg));

            this->Delete<Exp>(arg->exp_);

            log()->trace("delete {}", type(arg->exp_));
            delete(arg->exp_);
        }
    } else if (auto exp = dynamic_cast<Exp*>(pt); exp != nullptr) {

        log()->trace("success cast {}", type(exp));

        if (auto plus_exp = dynamic_cast<PlusExp*>(exp); plus_exp != nullptr) {
        } else if (dynamic_cast<MinusExp*>(exp) != nullptr) {
        } else if (dynamic_cast<MulExp*>(exp) != nullptr) {
        } else if (dynamic_cast<DivExp*>(exp) != nullptr) {
        } else if (dynamic_cast<ModExp*>(exp) != nullptr) {
        } else if (dynamic_cast<IndirectAddrExp*>(exp) != nullptr) {
        } else if (dynamic_cast<DatatypeExp*>(exp) != nullptr) {
        } else if (dynamic_cast<RangeExp*>(exp) != nullptr) {
        } else if (auto imm_exp = dynamic_cast<ImmExp*>(exp); imm_exp != nullptr) {
            log()->trace("success cast {}", type(imm_exp));

            this->Delete<Factor>(imm_exp->factor_);

            log()->trace("delete {}", type(imm_exp->factor_));
            delete(imm_exp->factor_);
        }
    } else if (auto factor = dynamic_cast<Factor*>(pt); factor != nullptr) {

        log()->trace("success cast {}", type(factor));

        if (auto f = dynamic_cast<NumberFactor*>(factor); f != nullptr) {
            log()->trace("success cast {}", type(f));
        } else if (auto f = dynamic_cast<HexFactor*>(factor); f != nullptr) {
            log()->trace("success cast {}", type(f));
        } else if (auto f = dynamic_cast<IdentFactor*>(factor); f != nullptr) {
            log()->trace("success cast {}", type(f));
        } else if (auto f = dynamic_cast<StringFactor*>(factor); f != nullptr) {
            log()->trace("success cast {}", type(f));
        }
    }

    return;
}

template <class T>
int Driver::Eval(T *parse_tree, const char* assembly_dst) {

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


void Driver::visitProg(Prog *prog) {

    if (prog->liststatement_) {
        prog->liststatement_->accept(this);
    }
}

void Driver::visitLabelStmt(LabelStmt *label_stmt) {
    visitLabel(label_stmt->label_);

    TParaToken t = this->ctx.top();
    this->ctx.pop();
    log()->debug("visitLabelStmt: args = {}", t.to_string());
}


void Driver::visitDeclareStmt(DeclareStmt *declare_stmt) {

    log()->debug("visitDeclareStmt start");
    visitIdent(declare_stmt->ident_);
    if (declare_stmt->exp_) {
        declare_stmt->exp_->accept(this);
    }
    log()->debug("visitDeclareStmt end");
}

void Driver::visitMnemonicStmt(MnemonicStmt *mnemonic_stmt){

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

    std::string debug_str = this->join(mnemonic_args, ",");
    log()->debug("mnemonic_args={}", debug_str);

    typedef std::function<void(std::vector<TParaToken>&)> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesDB", std::bind(&Driver::processDB, this, _1)),
        std::make_pair("OpcodesDW", std::bind(&Driver::processDW, this, _1)),
        std::make_pair("OpcodesDD", std::bind(&Driver::processDD, this, _1)),
        std::make_pair("OpcodesJMP", std::bind(&Driver::processJMP, this, _1)),
        std::make_pair("OpcodesMOV", std::bind(&Driver::processMOV, this, _1)),
        std::make_pair("OpcodesORG", std::bind(&Driver::processORG, this, _1)),
        std::make_pair("OpcodesRESB", std::bind(&Driver::processRESB, this, _1)),
    };

    const std::string opcode = type(*mnemonic_stmt->opcode_);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second(mnemonic_args);
    } else {
        std::cout << "not implemented..." << std::endl;
    }
}

void Driver::processDB(std::vector<TParaToken>& mnemonic_args) {
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

void Driver::processDW(std::vector<TParaToken>& mnemonic_args) {
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

void Driver::processDD(std::vector<TParaToken>& mnemonic_args) {
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

void Driver::processRESB(std::vector<TParaToken>& mnemonic_args) {

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

void Driver::processJMP(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("type: {}, value: {}", type(arg), arg.AsString());
    binout_container.push_back(0xeb);
    binout_container.push_back(0x00);

    std::string label = arg.AsString().substr(0, arg.AsString().find(":", 0));
    auto label_calc = LabelCalc{label: label, src_index: binout_container.size()};
    log()->debug("JMP label: {}", label);
    label_calc_stack.push(label_calc);
}

void Driver::processMOV(std::vector<TParaToken>& mnemonic_args) {

    //std::string debug_str = this->join(mnemonic_args, ",");
    //log()->debug("processMOV: args = [{}]", debug_str);

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(mnemonic_args[0].AsAttr(), mnemonic_args[1].AsAttr());

    std::vector<uint8_t> machine_codes = match(operands)(
        //         0x88 /r	MOV r/m8   , r8
        // REX   + 0x88 /r	MOV r/m8   , r8
        //         0x89 /r	MOV r/m16  , r16
        //         0x89 /r	MOV r/m32  , r32
        // REX.W + 0x89 /r	MOV r/m64  , r64
        //         0x8A /r	MOV r8     , r/m8
        // REX   + 0x8A /r	MOV r8     , r/m8
        //         0x8B /r	MOV r16    , r/m16
        //         0x8B /r	MOV r32    , r/m32
        // REX.W + 0x8B /r	MOV r64    , r/m64
        //         0x8C /r	MOV r/m16  , Sreg
        // REX.W + 0x8C /r	MOV r/m16  , Sreg
        //         0x8E /r	MOV Sreg   , r/m16
        // REX.W + 0x8E /r	MOV Sreg   , r/m64
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
            const uint8_t base = 0xb0;
            const uint8_t opcode = ModRM::get_opecode_from_reg(base, mnemonic_args[0].AsString());
            std::vector<uint8_t> b = {opcode, static_cast<uint8_t>(mnemonic_args[1].AsInt())};
            return b;
        },
		pattern | ds(TParaToken::ttReg16, TParaToken::ttImm) = [&] {
            const uint8_t base = 0xb8;
            const uint8_t opcode = ModRM::get_opecode_from_reg(base, mnemonic_args[0].AsString());
            std::vector<uint8_t> b = {opcode, static_cast<uint8_t>(mnemonic_args[1].AsInt())};
            return b;
        },
		pattern | ds(TParaToken::ttReg32, TParaToken::ttImm) = [&] {
            const uint8_t base = 0xb8;
            const uint8_t opcode = ModRM::get_opecode_from_reg(base, mnemonic_args[0].AsString());
            std::vector<uint8_t> b = {opcode, static_cast<uint8_t>(mnemonic_args[1].AsInt())};
            return b;
        },

        // REX.W + 0xB8+rd	MOV r64    , imm64
        //         0xC6 /0	MOV r/m8   , imm8
        // REX.W + 0xC6 /0	MOV r/m8   , imm8
        //         0xC7 /0	MOV r/m16  , imm16
        //         0xC7 /0	MOV r/m32  , imm32
        // REX.W + 0xC7 /0	MOV r/m64  , imm64
        pattern | _ = [&] { log()->debug("Not implemented or not matched..."); return std::vector<uint8_t>(); }
    );

    // 結果を投入
    binout_container.insert(binout_container.end(), std::begin(machine_codes), std::end(machine_codes));
    return;
}

void Driver::processORG(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttHex);
    log()->debug("type: {}, value: {}", type(arg), arg.AsLong());
    dollar_position = arg.AsLong();
}

//
// Visit Opcode系の処理
//
void Driver::visitOpcodesDB(OpcodesDB *opcodes_db) {}
void Driver::visitOpcodesDW(OpcodesDW *opcodes_db) {}
void Driver::visitOpcodesDD(OpcodesDD *opcodes_dd) {}
void Driver::visitOpcodesRESB(OpcodesRESB *opcodes_resb) {}
void Driver::visitOpcodesJMP(OpcodesJMP *opcodes_jmp) {}
void Driver::visitOpcodesMOV(OpcodesMOV *opcodes_mov) {}
void Driver::visitOpcodesORG(OpcodesORG *opcodes_org) {}


void Driver::visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) {

    for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i) {
        (*i)->accept(this);
    }
}

void Driver::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
    TParaToken t = this->ctx.top();
    log()->debug("visitMnemoArg: {}", t.to_string());
    this->ctx.pop();
    this->ctx.push(t);
}

//
// expressionの処理
//
void Driver::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitPlusExp(PlusExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitMinusExp(MinusExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitMulExp(MulExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitDivExp(DivExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitModExp(ModExp *p) {
    visitArithmeticOperations(p);
}

template void Driver::visitArithmeticOperations<PlusExp>(PlusExp *p);
template void Driver::visitArithmeticOperations<MinusExp>(MinusExp *p);
template void Driver::visitArithmeticOperations<MulExp>(MulExp *p);
template void Driver::visitArithmeticOperations<DivExp>(DivExp *p);
template void Driver::visitArithmeticOperations<ModExp>(ModExp *p);


template <class T>
void Driver::visitArithmeticOperations(T *exp) {

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
void Driver::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttInteger);
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttHex);
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->ident_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

//
// tokenの処理
//
void Driver::visitInteger(Integer x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttInteger);
    this->ctx.push(t);
}

void Driver::visitChar(Char x) {
    std::string str{x};
    TParaToken t = TParaToken(str, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Driver::visitDouble(Double x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttFloating);
    this->ctx.push(t);
}

void Driver::visitString(String x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Driver::visitIdent(Ident x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Driver::visitHex(Hex x) {
    TParaToken t = TParaToken(x, TParaToken::ttHex);
    this->ctx.push(t);
}

void Driver::visitLabel(Label x) {

    std::string label = x.substr(0, x.find(":", 0));
    log()->debug("label='{}' binout_container[{}]",
                 label, std::to_string(this->binout_container.size()));

    if (label_calc_stack.size() > 0) {
        LabelCalc l = label_calc_stack.top();
        if (l.label == label) {
            l.dst_index = this->binout_container.size();
            const size_t offset = l.get_offset();
            log()->debug("offset: {} := {} - {}", offset, l.dst_index, l.src_index);
            binout_container[l.src_index - 1] = offset;
            label_calc_stack.pop();
        }
    } else {
        log()->debug("unmatch!!! in={}, stack size={}", label, label_calc_stack.size());
    }

    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

const std::string Driver::join(std::vector<TParaToken>& array, const std::string& sep) {

    std::stringstream ss;
    for(size_t i = 0; i < array.size(); ++i) {
        if(i != 0) {
            ss << sep;
        }

        if (array[i].IsInteger() || array[i].IsHex()){
            ss << "0x"
               << std::setfill('0')
               << std::setw(2)
               << std::hex
               << array[i].AsLong();

        } else if (array[i].IsIdentifier()) {
            ss << "'" << array[i].AsString() << "'";
        }
    }
    return ss.str();
}
