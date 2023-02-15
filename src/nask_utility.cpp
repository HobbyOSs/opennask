#include "nask_utility.hpp"
#include "string_util.hpp"
#include "mod_rm.hpp"
#include "tinyexpr.h"
#include "bracket_impl.hpp"

namespace nask_utility {

    size_t get_labelpos(std::ifstream& stream, std::string token) {
        std::string line;
        for (long line_number = 1; std::getline(stream, line); ++line_number) {
            if (line.find(token) != std::string::npos) {
                return line_number;
            }
        }
        return -1;
    }

    bool is_comment_line(TParaCxxTokenTable* token_table, TParaToken& token) {
        return token_table->IsCommentLimiter(token.AsString());
    }

    bool is_line_terminated(TParaCxxTokenTable* token_table, TParaToken& token) {
        return token.AsString() == "\n";
    }

    template<size_t N>
    bool is_registers_with_args(const TParaToken& token, const std::array<std::string, N>& regs) {
        // レジスタ一覧から検索してあれば true
        auto it = std::find_if(std::begin(regs), std::end(regs),
                               [&](const std::string& s)
                               { return equals_ignore_case(token.AsString(), s); });
        return it != std::end(regs);
    }

    template<size_t N>
    bool is_registers_with_args(const std::string& token, const std::array<std::string, N>& regs) {
        // レジスタ一覧から検索してあれば true
        auto it = std::find_if(std::begin(regs), std::end(regs),
                               [&](const std::string& s)
                               { return equals_ignore_case(token, s); });
        return it != std::end(regs);
    }

    bool is_common_register(TParaCxxTokenTable* token_table, const TParaToken& token) {
        return is_registers_with_args(token, REGISTERS);
    }

    bool is_common_register(TParaCxxTokenTable* token_table, const std::string token) {
        return is_registers_with_args(token, REGISTERS);
    }

    bool is_segment_register(TParaCxxTokenTable* token_table, const TParaToken& token) {
        return is_registers_with_args(token, SEGMENT_REGISTERS);
    }

    bool is_control_register(TParaCxxTokenTable* token_table, const TParaToken& token) {
        return is_registers_with_args(token, CONTROL_REGISTERS);
    }

    bool is_register(TParaCxxTokenTable* token_table, const TParaToken& token) {
        const bool r = is_common_register(token_table, token) || is_segment_register(token_table, token);
        log()->debug("{} is_register? -> {}", token.AsString(), r);
        return r;
    }

    bool is_datatype(TParaCxxTokenTable* token_table, const TParaToken& token) {
        // データ型一覧から検索してあれば true
        auto it = std::find_if(std::begin(DATA_TYPES), std::end(DATA_TYPES),
                               [&](const std::string& s)
                               { return token.AsString().find(s) != std::string::npos; });

        return it != std::end(DATA_TYPES);
    }

    uint8_t plus_number_from_code(uint8_t byte, const std::string& reg) {
        log()->debug("Processing +rb, +rw, +rd, +ro ... {}", reg);
        std::smatch match;

        if (regex_match(reg, match, ModRM::rm000)) {
            return byte + 0;
        } else if (regex_match(reg, match, ModRM::rm001)) {
            return byte + 1;
        } else if (regex_match(reg, match, ModRM::rm010)) {
            return byte + 2;
        } else if (regex_match(reg, match, ModRM::rm011)) {
            return byte + 3;
        } else if (regex_match(reg, match, ModRM::rm100)) {
            return byte + 4;
        } else if (regex_match(reg, match, ModRM::rm101)) {
            return byte + 5;
        } else if (regex_match(reg, match, ModRM::rm110)) {
            return byte + 6;
        } else if (regex_match(reg, match, ModRM::rm111)) {
            return byte + 7;
        } else {
            std::cerr << "Unknown register is the argument for +rb, +rw, +rd, +ro" << std::endl;
            return 0;
        }
    }

    std::string expr_math_op(const std::string& subject) {

        const std::regex reg_numeric("([^0-9]*)([-\\*/+0-9]*)([^0-9]*)");

        try {
            std::smatch match;
            if (std::regex_search(subject, match, reg_numeric) && match.size() > 1) {
                int error;
                const int process = te_interp(match[2].str().c_str(), &error);
                const std::string cat = match[1].str() + match[2].str() + match[3].str();
                const std::string empty = "";

                std::string result = replace(subject, cat, empty);
                return match[1].str() + std::to_string(process) + match[3].str() + result;
            }
        } catch (std::regex_error& e) {
            log()->info(e.what());
        }
        return "";
    }

    // Instructionクラスの定数を初期化
    Instructions::Instructions() {
        // デフォルトのトークンテーブル
        this->token_table = new TParaCxxTokenTable();
        token_table->AddCommentLimiter(";", "\n");
        token_table->AddCommentLimiter("#", "\n");
        this->support_cpus = meta::flip_map(SUPPORT_CPUS);

        // 基本的なオペレーター登録
        for (std::string op : PRE_PROCESS_OPERATORS) {
            token_table->AddOperator(op);
        }
    }

    Instructions::~Instructions() {
        label_dst_stack.clear();
        label_src_stack.clear();
        equ_map.clear();
        gl_symbol_list.clear();
        ex_symbol_list.clear();
        symbol_offsets.clear();
        support_cpus.clear();
        delete token_table;
    }

    void Instructions::set_nimonic_with_register(const std::string& reg,
                                                 NIMONIC_INFO* nim_info,
                                                 TParaTokenizer& tokenizer) {

        if (reg == "AL" || reg == "BL" || reg == "CL" || reg == "DL") {
            // prefix = "B0+rb" (AL:+0, CL:+1, DL:+2, BL:+3)
            log()->debug("reg == AL|BL|CL|DL");
            nim_info->prefix = plus_number_from_code(0xb0, reg);
            nim_info->reg = reg;
            nim_info->imm = imm8;
        } else if (reg == "EAX" || reg == "EBX" || reg == "ECX" || reg == "EDX") {
            // prefix = "B8+rd" (EAX:+0, EBX:+1, ECX:+2, EDX:+3)
            log()->debug("reg == EAX|EBX|ECX|EDX");
            nim_info->prefix = plus_number_from_code(0xb8, reg);
            nim_info->reg = reg;
            nim_info->imm = imm32;
        } else {
            // tokenizerを先読みしてみる
            TParaToken src_token = tokenizer.LookAhead(2);

            // 対象のsrcの部分がEQUとして保存されていれば取得
            const std::string numeric_test = get_equ_label_or_asis(src_token.AsString());

            if (is_legitimate_numeric(numeric_test)) {
                nim_info->reg = reg;
                // レジスタの種類を見る
                std::smatch match;
                if (regex_match(reg, match, ModRM::regImm08)) {
                    const uint8_t modrm = ModRM::get_opecode_from_reg(0xb0, reg);
                    log()->debug("register is for imm8: {}, opecode 0x{:02x}", reg, modrm);
                    nim_info->prefix = modrm;
                    nim_info->imm = imm8;
                } else if (regex_match(reg, match, ModRM::regImm16)) {
                    const uint8_t modrm = ModRM::get_opecode_from_reg(0xb8, reg);
                    log()->debug("register is for imm16: {}, opecode 0x{:02x}", reg, modrm);
                    nim_info->prefix = modrm;
                    nim_info->imm = imm16;
                } else {
                    const uint8_t modrm = ModRM::get_opecode_from_reg(0xb8, reg);
                    log()->debug("register is for imm32: {}, opecode 0x{:02x}", reg, modrm);
                    nim_info->prefix = modrm;
                    nim_info->imm = imm32;
                }
            } else {
                log()->debug("MOV this is label: {}", src_token.AsString());
                nim_info->reg = reg;
                nim_info->imm = offs; // ターゲットはoffset
            }
        }
        return;
    }

    // segment-override prefix
    // see: http://wiki.osdev.org/X86-64_Instruction_Encoding
    void Instructions::store_segment_override_prefix(const std::string& src_reg, VECTOR_BINOUT& binout_container) {
        if (src_reg == "CS") {
            log()->debug("CS: Segment override prefix: 0x2e");
            binout_container.push_back(0x2e);
        } else if (src_reg == "SS") {
            log()->debug("SS: Segment override prefix: 0x36");
            binout_container.push_back(0x36);
        } else if (src_reg == "DS") {
            log()->debug("DS: Segment override prefix: 0x3e");
            binout_container.push_back(0x3e);
        } else if (src_reg == "ES") {
            log()->debug("ES: Segment override prefix: 0x26");
            binout_container.push_back(0x26);
        } else if (src_reg == "FS") {
            log()->debug("FS: Segment override prefix: 0x64");
            binout_container.push_back(0x64);
        } else if (src_reg == "GS") {
            log()->debug("GS: Segment override prefix: 0x65");
            binout_container.push_back(0x65);
        } else {
            std::cerr << "Invalid segment register: " << src_reg << std::endl;
        }
    }

    // 忌々しい 0x66を処理する
    void Instructions::store_register_size_prefix(const std::string& src_reg, VECTOR_BINOUT& binout_container) {

        std::smatch match;
        log()->debug("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

        if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
            log()->debug("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
            binout_container.push_back(0x66);
            return;
        } else if (regex_match(src_reg, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
            log()->debug("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
            binout_container.push_back(0x66);
            return;
        } else {
            log()->debug("Register-size prefix is absent");
            return;
        }
    }

    // label: (label_dstと呼ぶ)
    // 1) label_dstの位置を記録する → label_dst_stack
    void Instructions::store_label_dst(std::string label_dst, VECTOR_BINOUT& binout_container) {
        log()->debug("stored label: {} bin[{}]", label_dst, binout_container.size());
        LABEL_DST_ELEMENT elem;
        elem.label = label_dst;
        elem.src_index = binout_container.size();
        label_dst_stack.push_back(elem);
    }

    // label: (label_dstと呼ぶ)
    // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了
    void Instructions::update_label_dst_offset(std::string label_dst, VECTOR_BINOUT& binout_container) {

        for (auto it = std::begin(label_src_stack); it != std::end(label_src_stack); ++it) {
            // C++って::selectみたいなことできんのかい
            LABEL_SRC_ELEMENT elem(*it);
            //log()->debug("update target: {}, stored info {}", label_dst, elem.label);
            if (elem.label != label_dst)
                continue;

            // 処理の開始
            elem.dst_index = binout_container.size();
            if (elem.abs) {
                // ORGからの絶対値でオフセットを設定する
                switch (elem.offset_size) {
                case imm32:
                    log()->debug("imm32: update_label_dst_offset bin[{}, {}, {}, {}] = 0x{:02x}",
                                 elem.rel_index,
                                 elem.rel_index + 1,
                                 elem.rel_index + 2,
                                 elem.rel_index + 3,
                                 dollar_position + elem.dst_index);

                    set_dword_into_binout(dollar_position + elem.dst_index,
                                          binout_container, false, elem.rel_index);
                    break;

                case imm16:
                    log()->debug("imm16: update_label_dst_offset bin[{}, {}] = 0x{:02x}",
                                 elem.rel_index,
                                 elem.rel_index + 1,
                                 dollar_position + elem.dst_index);

                    set_word_into_binout(dollar_position + elem.dst_index,
                                         binout_container, false, elem.rel_index);
                    break;

                case imm8:
                default:
                    log()->debug("imm8: update_label_dst_offset bin[{}] = {}, bin[{}] = {}",
                                 elem.rel_index - 1, elem.dst_index, elem.rel_index, 0x7c);
                    binout_container[elem.rel_index - 1] = elem.dst_index;
                    binout_container[elem.rel_index] = 0x7c;
                    break;
                }
            } else {
                // ラベルからの相対値でオフセットを設定する
                switch (elem.offset_size) {
                case imm16:
                    log()->debug("imm16: update_label_dst_offset bin[{}, {}] = 0x{:02x}",
                                 std::to_string(elem.rel_index),
                                 std::to_string(elem.rel_index + 1),
                                 elem.rel_offset() - 1);

                    set_word_into_binout(elem.rel_offset() - 1,
                                         binout_container,
                                         false,
                                         elem.rel_index);
                    break;

                case imm32:
                    log()->debug("imm32: update_label_dst_offset bin[{}, {}, {}, {}] = 0x{:02x}",
                                 std::to_string(elem.rel_index),
                                 std::to_string(elem.rel_index + 1),
                                 std::to_string(elem.rel_index + 2),
                                 std::to_string(elem.rel_index + 3),
                                 elem.rel_offset());

                    set_dword_into_binout(elem.rel_offset(),
                                          binout_container,
                                          false,
                                          elem.rel_index);
                    break;

                case imm8:
                default:
                    log()->debug("imm8: update_label_dst_offset bin[{}] = 0x{:02x}",
                                 std::to_string(elem.rel_index + 1),
                                 elem.rel_offset());
                    binout_container[elem.rel_index] = elem.rel_offset();
                    break;
                }
            }
        }
    }

    const long Instructions::get_label_src_offset(std::string label_src) {

        auto it = std::find_if(std::begin(label_dst_stack), std::end(label_dst_stack),
                               [&](const LABEL_DST_ELEMENT& elem)
                               { return elem.label == label_src; });

        if (it != std::end(label_dst_stack)) {
            LABEL_DST_ELEMENT elem(*it);
            log()->debug("matched label: {} with {}", elem.label, label_src);
            const long abs_size = elem.src_index + dollar_position;
            return abs_size;
        }

        return 0;
    }

    // OPECODE label (label_srcと呼ぶ)
    // 1) 同名のlabel_dstが保存されていれば、オフセット値を計算して終了
    //    処理対象があれば true, 処理対象がなければ false
    bool Instructions::update_label_src_offset(std::string label_src,
                                               VECTOR_BINOUT& binout_container,
                                               uint8_t nim) {

        auto it = std::find_if(std::begin(label_dst_stack), std::end(label_dst_stack),
                               [&](const LABEL_DST_ELEMENT& elem)
                               { return elem.label == label_src; });

        if (it != std::end(label_dst_stack)) {
            LABEL_DST_ELEMENT elem(*it);
            elem.dst_index = binout_container.size();
            elem.rel_index = binout_container.size() + 1;
            log()->debug("update_label_src_offset bin[{}] = {}",
                         std::to_string(elem.rel_index),
                         elem.rel_offset());

            binout_container.push_back(nim);
            binout_container.push_back(elem.rel_offset());
            return true;
        }

        return false;
    }

    // OPECODE label (label_srcと呼ぶ)
    // 1) 同名のlabel_dstが保存されていれば、オフセット値を計算して終了
    //    処理対象があれば true, 処理対象がなければ false
    bool Instructions::update_label_src_offset(std::string label_src,
                                               VECTOR_BINOUT& binout_container) {

        auto it = std::find_if(std::begin(label_dst_stack), std::end(label_dst_stack),
                               [&](const LABEL_DST_ELEMENT& elem)
                               { return elem.label == label_src; });

        if (it != std::end(label_dst_stack)) {
            return true;
        }

        return false;
    }

    std::string Instructions::get_equ_label_or_asis(std::string key) {
        if( this->equ_map.find(key) != this->equ_map.end() ) {
            log()->debug("label: {} replaced as {}", key, equ_map[key]);
            return this->equ_map[key];
        } else {
            return key;
        }
    }

    std::vector<std::string> Instructions::get_equ_keyset() {
        std::vector<std::string> keyset;

        for (auto it = this->equ_map.begin(); it != this->equ_map.end(); ++it) {
            keyset.push_back(it->first);
        }

        // 長い文字列を先頭にする
        std::sort(keyset.begin(), keyset.end(),
                  [] (std::string l, std::string r) -> bool {
                      return l.length() > r.length();
                  });

        return keyset;
    }

    std::string Instructions::try_replace_equ(const std::string& line) {
        std::string replaced(line);
        for (auto const &key : this->get_equ_keyset()) {
            if (nask_utility::contains(line, key)) {
                // C++ syntax is poor... :<
                const std::string dec = nask_utility::is_hex_notation(this->equ_map[key]) ?
                    std::to_string(std::stoul(this->equ_map[key], nullptr, 16)) : this->equ_map[key];

                //logger->info("asm contains {} as {}", key, dec);
                replaced = nask_utility::replace(replaced, key, dec);
                //logger->info("{}: {} // *** EQU replaced ***", line_number, replaced);
            }
        }
        return replaced;
    }

    bool Instructions::dst_is_stored(std::string label_src) {
        auto it = std::find_if(std::begin(label_dst_stack), std::end(label_dst_stack),
                               [&](const LABEL_DST_ELEMENT& elem)
                               { return elem.label.find(label_src) != std::string::npos; });
        return it != std::end(label_dst_stack);
    }

    // OPECODE label (label_srcと呼ぶ)
    // 2) 同名のlabel_dstが保存されていなければ、label_srcの位置を保存する → label_src_stack
    void Instructions::store_label_src(std::string label_src, VECTOR_BINOUT& binout_container, bool abs, size_t offset_size) {
        LABEL_SRC_ELEMENT elem;
        elem.abs   = abs;
        elem.offset_size = offset_size;
        elem.label = label_src;
        elem.src_index = binout_container.size();
        elem.rel_index = binout_container.size() + 1;
        label_src_stack.push_back(elem);
    }

    // @detail uint16_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
    // nask的には0x00をバイトサイズで格納する傾向があるので、そうじゃない場合はフラグを設定する
    //
    // @param word             格納するWORDサイズのバイナリ
    // @param binout_container 出力先コンテナ
    // @param zero_as_byte     0x00をバイトサイズで格納する
    // @param start_index      格納するコンテナの開始index
    //
    void set_word_into_binout(const uint16_t& word,
                              VECTOR_BINOUT& binout_container,
                              bool zero_as_byte,
                              size_t start_index) {

        if (word == 0x0000 && zero_as_byte) {
            // push_back only 1byte
            binout_container.push_back(0x00);
            log()->debug("(B): 0x00");
        } else {
            // http://stackoverflow.com/a/1289360/2565527
            const uint8_t first_byte  = (word >> 8) & 0xff;
            const uint8_t second_byte = word & 0xff;
            // push_back as little_endian
            if (start_index == 0) {
                binout_container.push_back(second_byte);
                binout_container.push_back(first_byte);
            } else {
                binout_container.at(start_index + 0) = second_byte;
                binout_container.at(start_index + 1) = first_byte;
            }
            log()->debug("(W): 0x{:02x}, 0x{:02x}", second_byte, first_byte);
        }
    }

    // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
    // nask的にはDDは0x00を普通に詰めるらしい（仕様ブレブレすぎだろ…）
    void set_dword_into_binout(const uint32_t& dword,
                               VECTOR_BINOUT& binout_container,
                               bool zero_as_byte,
                               size_t start_index) {

        if (dword == 0x00000000 && zero_as_byte) {
            // push_back only 1byte
            binout_container.push_back(0x00);
        } else {
            uint32_t cp_dword = dword;
            uint8_t bytes[4] = {};

            bytes[0] = (cp_dword >> 24) & 0xff;
            bytes[1] = (cp_dword >> 16) & 0xff;
            bytes[2] = (cp_dword >> 8)  & 0xff;
            bytes[3] = cp_dword & 0xff;

            if (start_index == 0) {
                binout_container.push_back(bytes[3]);
                binout_container.push_back(bytes[2]);
                binout_container.push_back(bytes[1]);
                binout_container.push_back(bytes[0]);
            } else {
                binout_container.at(start_index + 0) = bytes[3];
                binout_container.at(start_index + 1) = bytes[2];
                binout_container.at(start_index + 2) = bytes[1];
                binout_container.at(start_index + 3) = bytes[0];
            }
            log()->debug("(DW): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}",
                         bytes[3], bytes[2], bytes[1], bytes[0]);
        }
    }

    void set_hexstring_into_binout(const std::string& in,
                                   VECTOR_BINOUT& binout_container,
                                   bool reverse) {

        const size_t len = in.length();
        std::vector<uint8_t> c;

        for(size_t i = 0; i < len; i += 2) {
            std::istringstream strm(in.substr(i, 2));
            uint8_t x = std::stoi(strm.str(), nullptr, 16);
            log()->debug("(BIN): 0x{:02x} ({})", x, strm.str());
            c.push_back(x);
        }

        if (reverse) {
            std::reverse(c.begin(), c.end());
        }

        for( auto it = c.begin(); it != c.end(); ++it ) {
            binout_container.push_back(*it);
        }
    }

    // MOVの命令
    // http://www5c.biglobe.ne.jp/~ecb/assembler/2_1.html
    // MOV DEST, SRC
    //     動作：DEST←SRC
    //     DEST：レジスタ、メモリー
    //     SRC ：レジスタ、メモリー、即値（ただしメモリー、メモリーの組み合わせは除く）
    //
    // より網羅的な表: http://softwaretechnique.jp/OS_Development/Tips/IA32_Instructions/MOV.html
    //
    LABEL_DST_STACK Instructions::label_dst_stack;
    LABEL_SRC_STACK Instructions::label_src_stack;
    std::map<std::string, std::string> Instructions::equ_map;
    std::map<std::string, size_t> Instructions::symbol_offsets;
    std::string Instructions::data_type;
    std::map<uint32_t, std::string> Instructions::support_cpus;

    // These parameters are only one instance each other
    uint32_t Instructions::dollar_position = 0;
    uint32_t Instructions::support = SUP_8086;
    int Instructions::OPENNASK_MODES = ID_16BIT_MODE;

    int Instructions::process_token_MOV(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // From: chapter MOV - Move 3-530
        // ------------------------------
        // Intel® 64 and IA-32 Architectures
        // Software Developer’s Manual
        // Volume 2 (2A, 2B & 2C):
        // Instruction Set Reference, A-Z
        //
        //         0x88 /r	MOV r/m8, r8	        r8をr/m8に転送します
        // REX   + 0x88 /r	MOV r/m8, r8	        r8をr/m8に転送します
        //         0x89 /r	MOV r/m16, r16		r16をr/m16に転送します
        //         0x89 /r	MOV r/m32, r32		r32をr/m32に転送します
        // REX.W + 0x89 /r	MOV r/m64, r64	        r64をr/m64に転送します
        //         0x8A /r	MOV r8, r/m8	        r/m8をr8に転送します
        // REX   + 0x8A /r	MOV r8, r/m8	        r/m8をr8に転送します
        //         0x8B /r	MOV r16, r/m16		r/m16をr16に転送します
        //         0x8B /r	MOV r32, r/m32		r/m32をr32に転送します
        // REX.W + 0x8B /r	MOV r64, r/m64	        r/m64をr64に転送します
        //         0x8C /r	MOV r/m16, Sreg	        セグメントレジスタをr/m16に転送します
        // REX.W + 0x8C /r	MOV r/m16, Sreg	        セグメントレジスタをr/m16に転送します
        //         0x8E /r	MOV Sreg, r/m16         r/m16をセグメントレジスタに転送します
        // REX.W + 0x8E /r	MOV Sreg, r/m64         r/m64をセグメントレジスタに転送します
        //         0xA0	MOV AL, moffs8   	（セグメント：オフセット）のバイトをALに転送します
        // REX.W + 0xA0	MOV AL, moffs8  	（セグメント：オフセット）のバイトをALに転送します
        //         0xA1	MOV AX, moffs16 	（セグメント：オフセット）のワードをAXに転送します
        //         0xA1	MOV EAX, moffs32	（セグメント：オフセット）のダブルワードをEAXに転送します
        // REX.W + 0xA1	MOV RAX, moffs64	（セグメント：オフセット）のダブルワードをRAXに転送します
        //         0xA2	MOV moffs8, AL          ALを（セグメント：オフセット）に転送します
        // REX.W + 0xA2	MOV moffs8, AL	        ALを（セグメント：オフセット）に転送します
        //         0xA3	MOV moffs16※, AX	AXを（セグメント：オフセット）に転送します
        //         0xA3	MOV moffs32※, EAX	EAXを（セグメント：オフセット）に転送します
        // REX.W + 0xA3	MOV moffs64※, RAX	RAXを（セグメント：オフセット）に転送します
        //         0xB0+rb	MOV r8, imm8	        imm8をr8に転送します
        // REX   + 0xB0+rb	MOV r8, imm8	        imm8をr8に転送します
        //         0xB8+rw	MOV r16, imm16		imm16をr16に転送します
        //         0xB8+rd	MOV r32, imm32		imm32をr32に転送します
        // REX.W + 0xB8+rd	MOV r64, imm64	        imm64をr64に転送します
        //         0xC6 /0	MOV r/m8, imm8		imm8をr/m8に転送します
        // REX.W + 0xC6 /0	MOV r/m8, imm8		imm8をr/m8に転送します
        //         0xC7 /0	MOV r/m16, imm16	imm16をr/m16に転送します
        //         0xC7 /0	MOV r/m32, imm32	imm32をr/m32に転送します
        // REX.W + 0xC7 /0	MOV r/m64, imm64	imm64をr/m64に転送します
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {

            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                continue;
            } else if (is_datatype(token_table, token)) {
                log()->debug("declared datatype: {}", token.AsString());
                this->data_type = token.AsString();
                continue;
            } else if (is_control_register(token_table, token)) {
                // 0x0F 0x22 /r
                TParaToken dst_token = token;
                TParaToken src_token = tokenizer.LookAhead(2);
                const std::string dst_reg  = dst_token.AsString();
                const std::string src_reg  = src_token.AsString();
                log()->debug("Reg {} <= {}", dst_reg, src_reg);

                // CRn,Reg32
                // 00001111 00100000 11sssrrr
                // 0x0F	0x22
                if (tokenizer.LookAhead(1).Is(",") &&
                    !tokenizer.LookAhead(2).IsEmpty() &&
                    is_control_register(token_table, dst_token)) {
                    // コンマを飛ばして次へ
                    token = tokenizer.Next();
                    token = tokenizer.Next();
                    log()->debug("{} <= {}", dst_reg, src_reg);

                    std::tuple<std::string, std::string> tp = ModRM::REGISTERS_RRR_MAP.at(src_reg);
                    const std::bitset<8> nim("11000" + std::get<0>(tp));
                    binout_container.push_back(0x0f);
                    binout_container.push_back(0x22);
                    binout_container.push_back(nim.to_ulong());
                    // これで終了のはず
                    log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                 0x0f, 0x22, nim.to_ulong());
                    break;
                }

            } else if (is_segment_register(token_table, token) &&
                       tokenizer.LookAhead(1).Is(",")) {
                //
                // 8E /r | MOV Sreg,r/m16** | Move r/m16 to segment register
                //
                TParaToken dst_token = token;
                TParaToken src_token = tokenizer.LookAhead(2);
                const std::string dst_reg  = dst_token.AsString();
                const std::string src_reg  = src_token.AsString();
                log()->debug("Sreg {} <= {}", dst_reg, src_reg);

                // MOV Sreg, register の時
                if (tokenizer.LookAhead(1).Is(",") && is_common_register(token_table, src_token)) {
                    // コンマを飛ばして次へ
                    token = tokenizer.Next();
                    token = tokenizer.Next();
                    const uint8_t modrm = ModRM::generate_modrm(0x8e, ModRM::REG, src_reg, dst_reg);
                    binout_container.push_back(0x8e);
                    binout_container.push_back(modrm);
                    // これで終了のはず
                    log()->debug("NIM: 0x{:02x}, 0x{:02x}", 0x8e, modrm);
                    break;
                }

            } else if (token.Is("[") && tokenizer.LookAhead(2).Is("]") &&
                       tokenizer.LookAhead(3).Is(",") &&
                       is_legitimate_numeric(tokenizer.LookAhead(4).AsString())) {
                //
                // MOV Mem, Imm | 1100011w oo000mmm
                // --------------------------------
                // imm8の場合w=0, imm16,32の場合w=1
                //
                // 0xC6 /0	MOV r/m8,  imm8	  imm8をr/m8に転送します
                // 0xC7 /0	MOV r/m16, imm16  imm16をr/m16に転送します
                // 0xC7 /0	MOV r/m32, imm32  imm32をr/m32に転送します
                //
                std::string w = "";

                if (this->data_type == "BYTE") {
                    w = "0";
                } else if (this->data_type == "WORD" || this->data_type == "DWORD") {
                    w = "1";
                } else {
                    std::cerr << "NASK : MOV syntax error, imm size is not supported now" << std::endl;
                    return 17;
                }

                TParaToken dst_token = tokenizer.LookAhead(1);
                TParaToken src_token = tokenizer.LookAhead(4);
                const std::string dst_mem  = "[" + get_equ_label_or_asis(dst_token.AsString()) + "]";
                const std::string src_imm  = src_token.AsString();

                const std::string dst_addr = get_equ_label_or_asis(dst_token.AsString());
                const uint32_t dst_addr_imm = std::stol(dst_addr, nullptr, 16);

                log()->debug("{}({}) <= {}", dst_mem, dst_addr_imm, src_imm);

                // Mem, Immの場合 => 1100011w oo 000 mmm
                // 1100011 + w
                if (this->data_type == "DWORD") { // 66 NOP
                    binout_container.push_back(0x66);
                }
                const std::bitset<8> bs_src("1100011" + w);
                binout_container.push_back(bs_src.to_ulong());

                // oo + rrr + mmm
                // 00 + 000 + 110
                // この場合 mod=00, reg=000, r/m=101で確定となる
                const std::bitset<8> bs_dst("00000110");
                binout_container.push_back(bs_dst.to_ulong());

                // 転送先は常にWORDサイズ
                set_word_into_binout(dst_addr_imm, binout_container, false);
                if (this->data_type == "BYTE") {
                    binout_container.push_back(src_token.AsLong());
                } else if (this->data_type == "WORD") {
                    set_word_into_binout(src_token.AsLong(), binout_container, false);
                } else if (this->data_type == "DWORD") {
                    set_dword_into_binout(src_token.AsLong(), binout_container, false);
                } else {
                    std::cerr << "NASK : MOV syntax error, imm size is not supported now" << std::endl;
                    return 17;
                }

                log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}",
                             bs_src.to_ulong(),
                             bs_dst.to_ulong(),
                             dst_addr_imm,
                             src_token.AsLong());

                // コンマを飛ばして次へ
                token = tokenizer.Next();
                token = tokenizer.Next();
                token = tokenizer.Next();
                token = tokenizer.Next();
                // これで終了のはず
                break;

            } else if (token.Is("[") && tokenizer.LookAhead(2).Is("]") &&
                       !is_common_register(token_table, get_equ_label_or_asis(tokenizer.LookAhead(1).AsString())) &&
                       tokenizer.LookAhead(3).Is(",") &&
                       ModRM::is_accumulator(tokenizer.LookAhead(4).AsString())) {
                //
                // MOV Mem	   , Reg  | 1000100w oorrrmmm
                //
                // MOV moffs8* , AL	  | 0xA2
                // MOV moffs16*, AX	  | 0xA3
                // MOV moffs32*, EAX  | 0xA3
                //
                // http://x86.renejeschke.de/html/file_module_x86_id_176.html
                // --------------------------------
                TParaToken dst_token = tokenizer.LookAhead(1);
                TParaToken src_token = tokenizer.LookAhead(4);
                const std::string dst_mem  = "[" + get_equ_label_or_asis(dst_token.AsString()) + "]";
                const std::string dst_addr = get_equ_label_or_asis(dst_token.AsString());
                const std::string src_reg  = src_token.AsString();

                log()->debug("{} <= {}, CPU {}", dst_mem, src_reg, this->support_cpus[this->support]);

                if (src_reg == "AL" || src_reg == "AX" || (src_reg == "EAX" && this->support <= SUP_i386)) {
                    store_register_size_prefix(src_reg, binout_container);
                    log()->debug("MOV moffs* , AL or AX");
                    const uint8_t bs_src = (src_reg == "AL") ? 0xa2 : 0xa3;
                    binout_container.push_back(bs_src);
                    const uint16_t dst_addr_imm = std::stol(dst_addr, nullptr, 16);
                    set_word_into_binout(dst_addr_imm, binout_container, false);
                } else {
                    log()->debug("MOV Mem,Reg");
                    // Mem, Regの場合 => 1000100w oo rrr mmm
                    // 1000100 + w
                    std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(src_reg);
                    const std::bitset<8> bs_src("1000100" + std::get<1>(tp_dst));

                    // FIXME: ここの部分、ModR/Mの原則にしたがってなさそう
                    const uint8_t modrm = ModRM::generate_modrm(bs_src.to_ulong(), ModRM::REG_REG, dst_mem, src_reg);
                    std::smatch match;
                    log()->debug("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");
                    if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
                        log()->debug("32bit reg using & 16bit-mode: Override prefix: 0x67");
                        binout_container.push_back(0x67);
                    }
                    if (regex_match(dst_mem, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
                        log()->debug("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
                        binout_container.push_back(0x66);
                    } else if (regex_match(dst_mem, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
                        log()->debug("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
                        binout_container.push_back(0x66);
                    } else {
                        log()->debug("Register-size prefix is absent");
                    }

                    // Opcode && MOD-REG-R/M
                    binout_container.push_back(bs_src.to_ulong());
                    binout_container.push_back(modrm);
                    log()->debug("NIM(W): 0x{:02x}, 0x{:02x}", bs_src.to_ulong(), modrm);

                    if (!is_common_register(token_table, dst_addr)) {
                        const std::string dst_addr = get_equ_label_or_asis(dst_token.AsString());
                        const uint16_t dst_addr_imm = std::stol(dst_addr, nullptr, 16);
                        set_word_into_binout(dst_addr_imm, binout_container, false);
                    }
                }

                // これで終了のはず
                break;

            } else if (
                (token.Is("[") && tokenizer.LookAhead(2).Is("]") &&
                 tokenizer.LookAhead(3).Is(",") &&
                 is_common_register(token_table, tokenizer.LookAhead(4))) ||
                // MOV [ESP],AX
                (token.Is("[") && tokenizer.LookAhead(4).Is("]") &&
                 tokenizer.LookAhead(5).Is(",") &&
                 is_common_register(token_table, tokenizer.LookAhead(6))) ||
                // MOV [ESP+6],AX
                (is_common_register(token_table, token) &&
                 tokenizer.LookAhead(1).Is(",") &&
                 is_common_register(token_table, tokenizer.LookAhead(2).AsString()))
                // MOV EAX, ESP
		    ) {

                // MOV r/m8, r8	  | 0x88 /r
                // MOV r/m16, r16	  | 0x89 /r
                // MOV r/m32, r32	  | 0x89 /r
                bool exists_disp = tokenizer.LookAhead(4).Is("]") ? true : false;

                TParaToken dst_token = tokenizer.LookAhead(1);
                TParaToken src_token = exists_disp ? tokenizer.LookAhead(6)
                    : tokenizer.LookAhead(4);

                std::string mem = "";
                std::string disp = "";
                if (exists_disp) {
                    mem = "[";
                    mem += tokenizer.LookAhead(1).AsString();
                    mem += tokenizer.LookAhead(2).AsString();
                    mem += tokenizer.LookAhead(3).AsString();
                    mem += "]";
                    disp = tokenizer.LookAhead(3).AsString();
                }
                log()->debug("exists disp ? => {}", exists_disp);

                std::string src_reg = src_token.AsString();
                std::string dst_reg = exists_disp ? tokenizer.LookAhead(1).AsString()
                    : get_equ_label_or_asis(dst_token.AsString());

                std::string dst_mem  = exists_disp ? mem
                    : "[" + get_equ_label_or_asis(dst_token.AsString()) + "]";

                const ModRM::mods mod_kind
                    = (src_token.IsEmpty()) ? ModRM::REG : exists_disp ? ModRM::REG_DISP8 : ModRM::REG_REG;

                if (src_token.IsEmpty()) {
                    // 上書き
                    src_reg = tokenizer.LookAhead(2).AsString();
                    dst_reg = token.AsString();
                    dst_mem = dst_reg;
                    log()->debug("{} <= {}", dst_reg, src_reg);
                } else {
                    log()->debug("{} <= {}", dst_mem, src_reg);
                }

                std::smatch match;
                log()->debug("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

                if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
                    log()->debug("32bit reg using & 16bit-mode: Override prefix: 0x67");
                    binout_container.push_back(0x67);
                }
                if (regex_match(dst_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
                    log()->debug("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                } else if (regex_match(dst_reg, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
                    log()->debug("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                } else if (regex_match(src_reg, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
                    // MOV [ESP+4], AX
                    log()->debug("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                } else {
                    log()->debug("Register-size prefix is absent");
                }

                if (regex_match(src_reg, match, ModRM::regImm08) && !exists_disp) { // +dispの場合だいたい32bitのレジスタ
                    // MOV r/m8, r8       | 0x88 /r
                    const uint8_t modrm = exists_disp ? ModRM::generate_modrm(0x88, mod_kind, dst_mem, src_reg)
                        : ModRM::generate_modrm(0x88, mod_kind, dst_mem, src_reg);
                    log()->debug("NIM(B): 0x88, 0x{:02x}, {}", modrm, tokenizer.LookAhead(2).AsString());
                    binout_container.push_back(0x88);
                    binout_container.push_back(modrm);
                    if (is_hex_notation(dst_token.AsString())) {
                        const std::string from = "0x";
                        const std::string to = "";
                        const std::string hex = replace(dst_token.AsString(), from, to);
                        set_hexstring_into_binout(hex, binout_container);
                    }
                } else {
                    // MOV r/m16, r16	  | 0x89 /r
                    // MOV r/m32, r32	  | 0x89 /r
                    const uint8_t modrm = exists_disp ? ModRM::generate_modrm(0x89, mod_kind, dst_mem, src_reg)
                        : ModRM::generate_modrm(0x88, mod_kind, dst_mem, src_reg);

                    log()->debug("NIM(B): 0x89, 0x{:02x}", modrm, tokenizer.LookAhead(2).AsString());
                    binout_container.push_back(0x89);
                    binout_container.push_back(modrm);
                }

                if (disp != "" && ModRM::get_rm_from_reg(dst_reg) == ModRM::SIB) {
                    const uint8_t sib = ModRM::generate_sib(exists_disp ? dst_mem : dst_reg, src_reg);
                    log()->debug("SIB: 0x{:02x}", sib);
                    binout_container.push_back(sib);
                }
                if (tokenizer.LookAhead(2).Is("+")) {
                    const uint8_t disp = tokenizer.LookAhead(3).AsLong();
                    log()->debug("Disp: 0x{:02x}", disp);
                    binout_container.push_back(disp);
                }

                break;

            } else if (is_common_register(token_table, token) &&
                       tokenizer.LookAhead(1).Is(",")  &&
                       // MOV XX, [YY]
                       (tokenizer.LookAhead(2).Is("[") && tokenizer.LookAhead(4).Is("]")) ||
                       // MOV XX, [YY+00]
                       (tokenizer.LookAhead(2).Is("[") && tokenizer.LookAhead(6).Is("]")) ||
                       // MOV XX, [YY:ZZ+00]
                       (tokenizer.LookAhead(2).Is("[") && tokenizer.LookAhead(8).Is("]")) ||
                       // MOV XX, BYTE [YY]
                       (is_datatype(token_table, tokenizer.LookAhead(2)) &&
                        tokenizer.LookAhead(3).Is("[") &&tokenizer.LookAhead(5).Is("]"))
                      ) {
                //
                // MOV Reg, Mem | 1000101woorrrmmm
                //---------------------------------
                // 0x8A /r	MOV r8, r/m8		r/m8をr8に転送します
                // 0x8B /r	MOV r16, r/m16		r/m16をr16に転送します
                // 0x8B /r	MOV r32, r/m32		r/m32をr32に転送します
                TParaToken dst_token = token;
                TParaToken src_token = is_datatype(token_table, tokenizer.LookAhead(2)) ?
                    tokenizer.LookAhead(4) : tokenizer.LookAhead(3);
                const std::string dst_reg  = dst_token.AsString();
                const std::string src_reg  = get_equ_label_or_asis(src_token.AsString());
                // mod=00: [レジスター + レジスター]
                // mod=01: [レジスター + disp8]

                bool exists_disp = (tokenizer.LookAhead(6).Is("]")||tokenizer.LookAhead(8).Is("]")) ? true : false;
                log()->debug("exists disp ? => {}", exists_disp);

                bool equ_specified = get_equ_label_or_asis(src_token.AsString()) != src_token.AsString();
                bool indexes_and_pointers = tokenizer.LookAhead(8).Is("]"); // [YY:ZZ+00]
                log()->debug("indexes_and_pointers ? => {}", indexes_and_pointers);

                const std::string disp = (equ_specified || !exists_disp) ? ""
                    : indexes_and_pointers ? tokenizer.LookAhead(6).AsString() + tokenizer.LookAhead(7).AsString()
                    : tokenizer.LookAhead(4).AsString() + tokenizer.LookAhead(5).AsString();

                const std::string src_mem  = !exists_disp ? "[" + src_reg + "]"
                    : indexes_and_pointers ? "[" + tokenizer.LookAhead(5).AsString() + disp + "]"
                    : "[" + src_reg + disp + "]";

                log()->debug("{} <= {}", dst_reg, src_mem);

                std::smatch match;
                log()->debug("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

                // 0x67
                if (regex_match(dst_reg, match, ModRM::regImm32) &&
                    this->OPENNASK_MODES == ID_16BIT_MODE && !indexes_and_pointers) {
                    log()->debug("32bit reg using & 16bit-mode: Override prefix: 0x67");
                    binout_container.push_back(0x67);
                }
                // 0x66
                if (regex_match(dst_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE
                    && indexes_and_pointers) {
                    log()->debug("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                } else if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
                    log()->debug("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                } else if (!indexes_and_pointers && exists_disp && this->OPENNASK_MODES == ID_16BIT_MODE) {
                    log()->debug("32bit operand using & 16bit-mode: Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                } else if (regex_match(dst_reg, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
                    // MOV AX,[ESP+4]
                    log()->debug("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                } else {
                    log()->debug("Register-size prefix is absent");
                }
                // segment-override prefix
                if (contains(src_mem, ":")) {
                    const std::string seg = src_mem.substr(1, 2);
                    log()->debug("it's necessary to store segment-override prefix: {} for {}", src_mem, seg);
                    store_segment_override_prefix(seg, binout_container);
                } else if (tokenizer.LookAhead(4).Is(":")) {
                    const std::string seg = tokenizer.LookAhead(3).AsString();
                    log()->debug("it's necessary to store segment-override prefix: {} for {}", src_mem, seg);
                    store_segment_override_prefix(seg, binout_container);
                }

                // 実際のオペコードなどを格納する
                const uint8_t op = regex_match(dst_reg, match, ModRM::regImm08) ? 0x8a : 0x8b;
                const bool mod_is = (disp != "" && disp.substr(1) == "0x00") ? false : exists_disp || indexes_and_pointers;
                const ModRM::mods mod_kind = mod_is ? ModRM::REG_DISP8 : ModRM::REG_REG;

                if (contains(src_mem, ":")) {
                    const uint8_t modrm = ModRM::generate_modrm(op,
                                                                ModRM::REG_REG,
                                                                "[" + src_mem.substr(4, 3) + "]",
                                                                dst_reg);
                    log()->debug("Opecode: 0x{:02x}, 0x{:02x}", op, modrm);
                    binout_container.push_back(op);
                    binout_container.push_back(modrm);
                } else {
                    const uint8_t modrm = ModRM::generate_modrm(op, mod_kind, src_mem, dst_reg);
                    log()->debug("Opecode: 0x{:02x}, 0x{:02x}", op, modrm);
                    binout_container.push_back(op);
                    binout_container.push_back(modrm);
                }


                if (disp != "" && ModRM::get_rm_from_reg(src_reg) == ModRM::SIB) {
                    const uint8_t sib = ModRM::generate_sib(exists_disp ? src_mem : src_reg, src_reg);
                    log()->debug("SIB: 0x{:02x}", sib);
                    binout_container.push_back(sib);
                } else if (is_hex_notation(src_reg)) {
                    // set memory
                    const std::string hex_imm = src_reg.substr(2);
                    set_hexstring_into_binout(hex_imm, binout_container);
                }

                if (exists_disp) {
                    const uint8_t disp =
                        indexes_and_pointers ? tokenizer.LookAhead(7).AsLong() : tokenizer.LookAhead(5).AsLong();
                    if (disp != 0x00) {
                        log()->debug("Disp: 0x{:02x}", disp);
                        binout_container.push_back(disp);
                    }
                }

                // これで終了のはず
                break;

            } else if (is_register(token_table, token) &&
                       tokenizer.LookAhead(1).Is(",")  &&
                       is_segment_register(token_table, tokenizer.LookAhead(2))) {

                // MOV Reg16,Seg
                //--------------------------------------------------------
                // 0x8C /r	MOV r/m16, Sreg		セグメントレジスタをr/m16に転送します
                TParaToken dst_token = token;
                TParaToken src_token = tokenizer.LookAhead(2);
                const std::string dst_reg  = dst_token.AsString();
                const std::string src_reg  = src_token.AsString();

                log()->debug("{} <= {}", dst_reg, src_reg);
                store_register_size_prefix(src_reg, binout_container);
                const uint8_t modrm = ModRM::generate_modrm(0x8c, ModRM::REG, dst_reg, src_reg);
                binout_container.push_back(0x8c);
                binout_container.push_back(modrm);
                log()->debug("NIM(B): 0x8c, 0x{:02x}", modrm);

                // これで終了のはず
                break;

            } else if (is_register(token_table, token) &&
                       tokenizer.LookAhead(1).Is(",")  &&
                       is_control_register(token_table, tokenizer.LookAhead(2))) {

                // MOV Reg32, CRn
                //	   00001111 00100010 11sssrrr
                //--------------------------------------------------------
                // 0x0F 0x20 /r | CRnをr32に転送します
                // ここでのsssは常に000にしかならない気がする
                TParaToken dst_token = token;
                TParaToken src_token = tokenizer.LookAhead(2);
                const std::string dst_reg  = dst_token.AsString();
                const std::string src_reg  = src_token.AsString();

                log()->debug("{} <= {}", dst_reg, src_reg);

                // Reg32, CRnの場合 => 11sssrrr
                std::tuple<std::string, std::string> tp = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

                // oo + sss + mmm
                const std::bitset<8> nim("11000" + std::get<0>(tp));
                binout_container.push_back(0x0f);
                binout_container.push_back(0x20);
                binout_container.push_back(nim.to_ulong());

                log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
                             0x0f,
                             0x20,
                             nim.to_ulong());

                // これで終了のはず
                break;

            } else if (is_register(token_table, token) &&
                       tokenizer.LookAhead(1).Is(",")  &&
                       is_register(token_table, tokenizer.LookAhead(2))) {

                // MOV Reg, Reg
                //--------------------------------------------------------
                // 0x8A /r	MOV r8, r/m8		r/m8をr8に転送します
                // 0x8B /r	MOV r16, r/m16		r/m16をr16に転送します
                // 0x8B /r	MOV r32, r/m32		r/m32をr32に転送します
                TParaToken dst_token = token;
                TParaToken src_token = tokenizer.LookAhead(2);
                const std::string dst_reg  = dst_token.AsString();
                const std::string src_reg  = src_token.AsString();

                log()->debug("{} <= {}", dst_reg, src_reg);

                std::smatch match;

                if (regex_match(dst_reg, match, ModRM::regImm08)) {
                    const uint8_t modrm = ModRM::generate_modrm(0x8a, ModRM::REG_REG, src_reg, dst_reg);
                    binout_container.push_back(0x8a);
                    binout_container.push_back(modrm);
                    log()->debug("NIM:(B) 0x{:02x}, 0x{:02x}", 0x8a, modrm);
                } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                    const uint8_t modrm = ModRM::generate_modrm(0x8b, ModRM::REG_REG, src_reg, dst_reg);
                    binout_container.push_back(0x8b);
                    binout_container.push_back(modrm);
                    log()->debug("NIM:(B) 0x{:02x}, 0x{:02x}", 0x8b, modrm);
                } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                    const uint8_t modrm = ModRM::generate_modrm(0x8b, ModRM::REG_REG, src_reg, dst_reg);
                    binout_container.push_back(0x8b);
                    binout_container.push_back(modrm);
                    log()->debug("NIM:(B) 0x{:02x}, 0x{:02x}", 0x8b, modrm);
                }
                // これで終了のはず
                break;

            } else if (is_register(token_table, token) &&
                       tokenizer.LookAhead(1).Is(",")  &&
                       !is_legitimate_numeric(get_equ_label_or_asis(tokenizer.LookAhead(2).AsString())) &&
                       !is_legitimate_numeric(tokenizer.LookAhead(2).AsString())) {
                //
                // MOV Reg, Imm | 1011wrrr
                // Immがオフセットで示されている場合
                //--------------------------------------------------------
                // 0xB0+rb	MOV r8, imm8		imm8をr8に転送します
                // 0xB8+rw	MOV r16, imm16		imm16をr16に転送します
                // 0xB8+rd	MOV r32, imm32		imm32をr32に転送します
                //
                TParaToken dst_token = token;
                TParaToken src_token = tokenizer.LookAhead(2);
                const std::string dst_reg  = dst_token.AsString();
                std::string src_imm  = get_equ_label_or_asis(src_token.AsString());
                std::smatch match;

                // 直接リテラルを指定された場合
                if (starts_with(src_imm, "'") && ends_with(src_imm, "'")) {
                    src_imm = src_imm.substr(1, src_imm.size() - 2);
                    const uint8_t base = regex_match(dst_reg, match, ModRM::regImm08) ? 0xb0 : 0xb8;
                    const uint8_t op = plus_number_from_code(base, dst_reg);
                    log()->debug("NIM:(B) 0x{:02x}", op);
                    binout_container.push_back(op);

                    if (regex_match(dst_reg, match, ModRM::regImm08)) {
                        binout_container.push_back(src_imm.at(0));
                    } else {
                        std::cerr << "NASK: MOV can't set literal with single quote" << std::endl;
                        return 17;
                    }
                } else {
                    if (regex_match(dst_reg, match, ModRM::regImm08)) {
                        const uint8_t o = ModRM::get_opecode_from_reg(0xb0, dst_reg);
                        log()->debug("NIM:(B) 0x{:02x}, 0x{:02x}", o, 0x00);
                        binout_container.push_back(o);
                        store_label_src(src_imm, binout_container, false, imm8);
                        binout_container.push_back(0x00);

                    } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                        const uint8_t o = ModRM::get_opecode_from_reg(0xb8, dst_reg);
                        log()->debug("NIM:(W) 0x{:02x}, 0x{:02x}, 0x{:02x}", o, 0x00, 0x00);
                        store_label_src(src_imm, binout_container, true, imm16);
                        binout_container.push_back(o);
                        binout_container.push_back(0x00);
                        binout_container.push_back(0x00);

                    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                        const uint8_t o = ModRM::get_opecode_from_reg(0xb8, dst_reg);
                        store_register_size_prefix(dst_reg, binout_container);
                        log()->debug("NIM:(DW) 0x{:02x}, 0x{:02x}...", o, 0x00);
                        store_label_src(src_imm, binout_container, true, imm32);
                        binout_container.push_back(o);
                        binout_container.push_back(0x00);
                        binout_container.push_back(0x00);
                        binout_container.push_back(0x00);
                        binout_container.push_back(0x00);
                    }
                }
                log()->debug("{} <= {}, represented with label", dst_reg, src_imm);

                break;

            } else if (is_register(token_table, token) &&
                       tokenizer.LookAhead(1).Is(",")) {
                //
                // MOV Reg, Imm | 1011wrrr
                //--------------------------------------------------------
                // 0xB0+rb	MOV r8,  imm8		imm8をr8に転送します
                // 0xB8+rw	MOV r16, imm16		imm16をr16に転送します
                // 0xB8+rd	MOV r32, imm32		imm32をr32に転送します
                //
                TParaToken dst_token = token;
                TParaToken src_token = tokenizer.LookAhead(2);
                const std::string dst_reg     = dst_token.AsString();
                const std::string src_imm_str = get_equ_label_or_asis(src_token.AsString());

                const long src_imm = (src_imm_str != src_token.AsString() && is_hex_notation(src_imm_str)) ?
                    std::strtol(src_imm_str.c_str(), NULL, 16) : src_token.AsLong();

                log()->debug("{} <= {}, with imm", dst_reg, src_imm);

                std::smatch match;
                store_register_size_prefix(dst_reg, binout_container);

                if (regex_match(dst_reg, match, ModRM::regImm08)) {
                    // 0xb0+rb
                    const uint8_t op = plus_number_from_code(0xb0, dst_reg);
                    log()->debug("(B): 0x{:02x}, 0x{:02x}", op, src_imm);
                    binout_container.push_back(op);
                    binout_container.push_back(src_imm);

                } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                    // 0xb8+rw
                    const uint8_t op = plus_number_from_code(0xb8, dst_reg);
                    log()->debug("(W): 0x{:02x}, 0x{:02x}", op, src_imm);
                    binout_container.push_back(op);
                    set_word_into_binout(src_imm, binout_container, false);

                } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                    // 0xb8+rd
                    const uint8_t op = plus_number_from_code(0xb8, dst_reg);
                    log()->debug("(D): 0x{:02x}, 0x{:02x}", op, src_imm);
                    binout_container.push_back(op);
                    set_dword_into_binout(src_imm, binout_container);
                } else {
                    update_label_src_offset(src_token.AsString(), binout_container, 0x00);
                    store_label_src(src_token.AsString(), binout_container, true);
                    // とりあえずoffsetには0x00を入れておき、見つかった時に更新する
                    binout_container.push_back(0x00);
                    binout_container.push_back(0x7c);
                }

                // これで終了のはず
                break;
            } else {
                log()->debug("not matching");
                binout_container.push_back(token.AsLong());
            }
        }

        return 0;
    }

    // 簡単なNOP命令の実装
    int Instructions::process_token_NOP(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0x90 を格納
        binout_container.push_back(0x90);
        return 0;
    }

    // JAE命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JAE(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x73, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x73);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x73);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // JBE命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JBE(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x76, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x76);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x76);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // JB命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JB(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x72, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x72);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x72);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // JC命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JC(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x72, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x72);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x72);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // JE命令の実装
    int Instructions::process_token_JE(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                const std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x74, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x74);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x74);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // 簡単なJMP命令の実装
    int Instructions::process_token_JMP(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {


        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                continue;
            } else if (token.Is("FAR") && tokenizer.LookAhead(1).Is("[")) {
                // JMP m16:16   | 0xFF /5
                // JMP m16:32   | 0xFF /5
                const std::string reg    = tokenizer.LookAhead(2).AsString();
                const std::string op     = tokenizer.LookAhead(3).AsString();
                const std::string disp_s = tokenizer.LookAhead(4).AsString();
                const uint8_t disp       = tokenizer.LookAhead(4).AsLong();
                const std::string mem    = "[" + reg + op + disp_s + "]";
                log()->debug("far jmp [{}{}{}]", reg, op, disp);
                const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_DISP8, mem, ModRM::SLASH_5);

                log()->debug("NIM(W): 0xff, 0x{:02x}, 0x{:02x}", modrm, disp);
                binout_container.push_back(0xff);
                binout_container.push_back(modrm);

                if (disp_s != "" && ModRM::get_rm_from_reg(reg) == ModRM::SIB) {
                    const uint8_t sib = ModRM::generate_sib(mem, reg);
                    log()->debug("SIB: 0x{:02x}", sib);
                    binout_container.push_back(sib);
                }
                binout_container.push_back(disp);
                break;

            } else if (tokenizer.LookAhead(1).Is(":") || tokenizer.LookAhead(2).Is(":")) {
                // JMP ptr16:16 | 0xEA cd
                // JMP ptr16:32 | 0xEA cp
                long ptr16 = 0x00;
                long addr  = 0x00;
                if (tokenizer.LookAhead(1).Is(":")) {
                    ptr16 = token.AsLong();
                    addr  = tokenizer.LookAhead(2).AsLong();
                } else if (tokenizer.LookAhead(2).Is(":")) {
                    log()->debug("Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                    ptr16 = tokenizer.LookAhead(1).AsLong();
                    addr  = tokenizer.LookAhead(3).AsLong();
                }

                log()->debug("NIM(W): 0xea, 0x{:02x}, 0x{:02x}", addr, ptr16);
                binout_container.push_back(0xea);
                set_dword_into_binout(addr, binout_container);
                set_word_into_binout(ptr16, binout_container);
                break;

            } else if (is_hex_notation(token.AsString())) {
                // ラベルではなく即値でジャンプ先を指定された場合
                // JMP rel8  | 0xEB cb
                // JMP rel16 | 0xE9 cw
                // JMP rel32 | 0xE9 cd

                // offset = dst - src - current_pos
                if (is_between_bytesize(token.AsLong()) && imm8 == ModRM::get_imm_size(token.AsString())) {
                    // 0xEB
                    const long jmp_offset = (token.AsLong() - dollar_position - binout_container.size()) - 2;
                    log()->debug("JMP: {}", token.AsString());
                    log()->debug("0xeb with Byte {}", jmp_offset);
                    binout_container.push_back(0xeb);
                    binout_container.push_back(jmp_offset);
                } else {
                    // 0xE9
                    log()->debug("JMP: ", token.AsString());
                    const long jmp_offset = (token.AsLong() - dollar_position - binout_container.size()) - 3;
                    if (ModRM::get_imm_size(token.AsString()) == imm16) {
                        log()->debug("0xe9 with Word {}", jmp_offset);
                        binout_container.push_back(0xe9);
                        set_word_into_binout(jmp_offset, binout_container);
                    } else if (ModRM::get_imm_size(token.AsString()) == imm32) {
                        log()->debug("0xe9 with Dword {}", jmp_offset);
                        binout_container.push_back(0xe9);
                        set_dword_into_binout(jmp_offset, binout_container);
                    } else {
                        std::cerr << "NASK : JMP syntax error, imm size is wierd" << std::endl;
                        return 17;
                    }
                }
                break;

            } else {
                const std::string store_label = token.AsString();
                log()->debug("label stored: ", store_label);
                log()->debug("0xeb, 0x00");

                if (dst_is_stored(store_label)) {
                    update_label_src_offset(store_label, binout_container, 0xeb);
                } else {
                    store_label_src(store_label, binout_container);
                    binout_container.push_back(0xeb);
                    binout_container.push_back(0x00);
                    log()->debug("bin[{}] = 0xeb, bin[{}] = 0x00",
                                 binout_container.size() - 1,
                                 binout_container.size());
                }
                break;
            }
        }
        return 0;
    }

    // JNC命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JNC(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x73, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x73);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x73);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // JNE命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JNE(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x75, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x75);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x75);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // JZ命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JNZ(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x75, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x75);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x75);
                        binout_container.push_back(0x00);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // JZ命令の実装(JMP命令全般でまとめて良いかもしれない)
    int Instructions::process_token_JZ(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                std::string store_label = token.AsString();
                if (store_label.empty()) {
                    continue;
                } else {
                    log()->debug("label stored: ", store_label);
                    log()->debug("0x74, 0x00");

                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x74);
                    } else {
                        store_label_src(store_label, binout_container);
                        binout_container.push_back(0x74);
                        binout_container.push_back(0x00);
                        log()->debug("bin[{}] = 0x74, bin[{}] = 0x00",
                                     binout_container.size() - 1,
                                     binout_container.size());
                    }
                    break;
                }
            }
        }
        return 0;
    }

    // LGDT命令の実装
    int Instructions::process_token_LGDT(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is("[") && tokenizer.LookAhead(2).Is("]")) {
                //----------------------------------------------------//
                // 0x0F 01 /2  | LGDT m16& 32    mをGDTRにロードします//
                //----------------------------------------------------//
                // [mod] 00 :
                // [reg] 010: /2
                // [r/m] 110: 固定値？
                log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}", 0x0f, 0x01, 0x16);
                binout_container.push_back(0x0f);
                binout_container.push_back(0x01);

                const std::string store_label = tokenizer.LookAhead(1).AsString();

                if (is_hex_notation(store_label)) {
                    // ラベルではなく即値でジャンプ先を指定された場合
                    log()->debug("** Not implemented **");
                } else {
                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x16);
                    } else {
                        store_label_src(store_label, binout_container, true, imm16);
                        binout_container.push_back(0x16);
                        binout_container.push_back(0x00);
                        binout_container.push_back(0x00);
                    }
                    break;
                }

            } else if (token.Is("[") && tokenizer.LookAhead(4).Is("]")) {
                //----------------------------------------------------//
                // 0x0F 01 /2  | LGDT m16& 32    mをGDTRにロードします//
                //----------------------------------------------------//
                // [mod] 00 :
                // [reg] 010: /2
                // [r/m] 110: 固定値？
                const std::string reg    = tokenizer.LookAhead(1).AsString();
                const std::string op     = tokenizer.LookAhead(2).AsString();
                const std::string disp_s = tokenizer.LookAhead(3).AsString();
                const uint8_t disp       = tokenizer.LookAhead(3).AsLong();
                const std::string mem    = "[" + reg + op + disp_s + "]";
                log()->debug("LGDT [{}{}{}]", reg, op, disp);
                const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_DISP8, mem, ModRM::SLASH_2);

                log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}", 0x0f, 0x01, modrm, disp);
                binout_container.push_back(0x0f);
                binout_container.push_back(0x01);
                binout_container.push_back(modrm);

                if (disp_s != "" && ModRM::get_rm_from_reg(reg) == ModRM::SIB) {
                    const uint8_t sib = ModRM::generate_sib(mem, reg);
                    log()->debug("SIB: 0x{:02x}", sib);
                    binout_container.push_back(sib);
                }

                binout_container.push_back(disp);
                break;

            } else {
                std::cerr << "NASK : LGDT syntax error " << token.AsString() << std::endl;
                return 17;
            }
        }
        return 0;
    }

    // LIDT命令の実装
    int Instructions::process_token_LIDT(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is("[") && tokenizer.LookAhead(2).Is("]")) {
                //----------------------------------------------------//
                // 0x0F 01 /2  | LGDT m16& 32    mをGDTRにロードします//
                //----------------------------------------------------//
                // [mod] 00 :
                // [reg] 010: /2
                // [r/m] 110: 固定値？
                log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}", 0x0f, 0x01, 0x16);
                binout_container.push_back(0x0f);
                binout_container.push_back(0x01);

                const std::string store_label = tokenizer.LookAhead(1).AsString();

                if (is_hex_notation(store_label)) {
                    // ラベルではなく即値でジャンプ先を指定された場合
                    log()->debug("** Not implemented **");
                } else {
                    if (dst_is_stored(store_label)) {
                        update_label_src_offset(store_label, binout_container, 0x16);
                    } else {
                        store_label_src(store_label, binout_container, true, imm16);
                        binout_container.push_back(0x16);
                        binout_container.push_back(0x00);
                        binout_container.push_back(0x00);
                    }
                    break;
                }

            } else if (token.Is("[") && tokenizer.LookAhead(4).Is("]")) {
                //----------------------------------------------------//
                // 0x0F 01 /2  | LIDT m16& 32    mをGDTRにロードします//
                //----------------------------------------------------//
                // [mod] 00 :
                // [reg] 010: /3
                // [r/m] 110: 固定値？
                const std::string reg    = tokenizer.LookAhead(1).AsString();
                const std::string op     = tokenizer.LookAhead(2).AsString();
                const std::string disp_s = tokenizer.LookAhead(3).AsString();
                const uint8_t disp       = tokenizer.LookAhead(3).AsLong();
                const std::string mem    = "[" + reg + op + disp_s + "]";
                log()->debug("LIDT [{}{}{}]", reg, op, disp);
                const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_DISP8, mem, ModRM::SLASH_3);

                log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}", 0x0f, 0x01, modrm, disp);
                binout_container.push_back(0x0f);
                binout_container.push_back(0x01);
                binout_container.push_back(modrm);

                if (disp_s != "" && ModRM::get_rm_from_reg(reg) == ModRM::SIB) {
                    const uint8_t sib = ModRM::generate_sib(mem, reg);
                    log()->debug("SIB: 0x{:02x}", sib);
                    binout_container.push_back(sib);
                }

                binout_container.push_back(disp);
                break;

            } else {
                std::cerr << "NASK : LIDT syntax error " << token.AsString() << std::endl;
                return 17;
            }
        }
        return 0;
    }

    // LTR命令の実装
    int Instructions::process_token_LTR(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is("[") && tokenizer.LookAhead(4).Is("]")) {
                //-------------------------------------------------------------//
                // 0x0F 0x00 /3 | LTR r/m16	r/m16をタスクレジスタにロードします//
                //-------------------------------------------------------------//
                // [mod] 01 : disp8
                // [reg] 010: /3
                // [r/m] xxx:
                const std::string reg    = tokenizer.LookAhead(1).AsString();
                const std::string op     = tokenizer.LookAhead(2).AsString();
                const std::string disp_s = tokenizer.LookAhead(3).AsString();
                const uint8_t disp       = tokenizer.LookAhead(3).AsLong();
                const std::string mem    = "[" + reg + op + disp_s + "]";
                log()->debug("LTR [{}{}{}]", reg, op, disp);
                const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_DISP8, mem, ModRM::SLASH_3);

                log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}", 0x0f, 0x00, modrm, disp);
                binout_container.push_back(0x0f);
                binout_container.push_back(0x00);
                binout_container.push_back(modrm);

                if (disp_s != "" && ModRM::get_rm_from_reg(reg) == ModRM::SIB) {
                    const uint8_t sib = ModRM::generate_sib(mem, reg);
                    log()->debug("SIB: 0x{:02x}", sib);
                    binout_container.push_back(sib);
                }

                binout_container.push_back(disp);
                break;

            } else {
                std::cerr << "NASK : LTR syntax error " << token.AsString() << std::endl;
                return 17;
            }
        }
        return 0;
    }

    // 簡単なADD命令の実装
    int Instructions::process_token_ADD(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {

                if (is_common_register(token_table, token) &&
                    tokenizer.LookAhead(1).Is(",")  &&
                    is_common_register(token_table, tokenizer.LookAhead(2))) {
                    // ADD Reg,Reg
                    //
                    // 0x00 /r     ADD r/m8, r8        r8をr/m8に加算する
                    // 0x01 /r     ADD r/m16, r16      r16をr/m16に加算する
                    // 0x01 /r     ADD r/m32, r32      r32をr/m32に加算する
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg  = dst_token.AsString();
                    const std::string src_reg  = src_token.AsString();
                    log()->debug("{} + {}", dst_reg, src_reg);

                    std::smatch match;
                    if (regex_match(src_reg, match, ModRM::regImm08)) {
                        const uint8_t op  = 0x00;
                        const uint8_t nim = ModRM::generate_modrm(ModRM::REG_REG, dst_reg);
                        // debug logs
                        log()->debug("NIM(W): 0x{:02x}, 0x{:02x}", op, nim);
                        binout_container.push_back(op);
                        binout_container.push_back(nim);
                    } else {
                        const uint8_t pre = 0x66;
                        const uint8_t op  = 0x01;
                        const uint8_t nim = ModRM::generate_modrm(0x01, ModRM::REG, src_reg, dst_reg);
                        if (regex_match(src_reg, match, ModRM::regImm32)) {
                            // see: http://wiki.osdev.org/X86-64_Instruction_Encoding#Legacy_Prefixes
                            log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}", pre, op, nim);
                            binout_container.push_back(pre);
                            binout_container.push_back(op);
                            binout_container.push_back(nim);
                        } else {
                            // debug logs
                            log()->debug("NIM(W): 0x{:02x}, 0x{:02x}", op, nim);
                            binout_container.push_back(op);
                            binout_container.push_back(nim);
                        }
                    }

                    // 次へ
                    break;

                } else if (token.Is("[") &&
                           is_common_register(token_table, tokenizer.LookAhead(1)) &&
                           tokenizer.LookAhead(2).Is("]") &&
                           tokenizer.LookAhead(3).Is(",") &&
                           is_common_register(token_table, tokenizer.LookAhead(4))) {
                    // ADD Mem,Reg
                } else if (is_common_register(token_table, token) &&
                           tokenizer.LookAhead(1).Is(",")  &&
                           tokenizer.LookAhead(2).Is("[")  &&
                           is_common_register(token_table, tokenizer.LookAhead(3)) &&
                           tokenizer.LookAhead(4).Is("]")) {
                    // ADD Reg,Mem
                } else if (is_common_register(token_table, token) &&
                           tokenizer.LookAhead(1).Is(",")  &&
                           is_legitimate_numeric(tokenizer.LookAhead(2).AsString())) {
                    // ADD Acc,Imm
                    // ADD Reg,Imm8
                    // ADD Reg,Imm
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg  = dst_token.AsString();
                    const std::string src_imm  = src_token.AsString();

                    // 次へ
                    tokenizer.Next();
                    tokenizer.Next();
                    log()->debug("{} + {}", dst_reg, src_imm);
                    std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
                    // Imm8 or Imm16
                    const size_t imm_size = ModRM::get_imm_size(src_token.AsString());
                    log()->debug("imm size: {}", imm_size);

                    if (ModRM::is_accumulator(dst_reg)) {
                        // ADD Acc,Imm
                        // NIM: 0000010w
                        const std::bitset<8> bs_dst("0000010" + std::get<1>(tp_dst));
                        // debug logs
                        log()->debug("NIM(W): 0x{:02x}, 0x{:02x}", bs_dst.to_ulong(), src_token.AsLong());
                        binout_container.push_back(bs_dst.to_ulong());

                        if (dst_reg == "AL") {
                            log()->debug("imm8: {}", src_imm);
                            binout_container.push_back(src_token.AsLong());
                        } else if (dst_reg == "AX") {
                            log()->debug("imm16: {}", src_imm);
                            set_word_into_binout(src_token.AsLong(), binout_container);
                        } else {
                            log()->debug("imm32: {}", src_imm);
                            set_dword_into_binout(src_token.AsLong(), binout_container);
                        }
                        break;

                    } else {

                        // ADD Reg8, Imm
                        // -------------
                        // 0x80 /0 ib  ADD r/m8, imm8
                        // 0x81 /0 ib  ADD r/m8, imm8

                        // ADD Reg16, Imm
                        // -------------
                        // 0x83 /0 ib  ADD r/m16, imm8
                        // 0x81 /0 iw  ADD r/m16, imm16

                        // ADD Reg32, Imm
                        // -------------
                        // 0x83 /0 ib  ADD r/m32, imm8
                        // 0x81 /0 id  ADD r/m32, imm32
                        std::smatch match;

                        if (regex_match(dst_reg, match, ModRM::regImm08)) {
                            // ADD Reg8, Imm
                            // -------------
                            // 0x80 /0 ib  ADD r/m8, imm8
                            // 0x81 /0 ib  ADD r/m8, imm8
                            log()->debug("r/m8: ", dst_reg);
                            const uint8_t modrm =
                                ModRM::generate_modrm(ModRM::REG, dst_reg, ModRM::SLASH_0);

                            log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                         0x80,
                                         modrm,
                                         src_token.AsLong());

                            binout_container.push_back(0x80);
                            binout_container.push_back(modrm);
                            binout_container.push_back(src_token.AsLong());

                        } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                            const uint8_t op = is_imm8(src_token.AsString()) ? 0x83 : 0x81;
                            // ADD Reg16, Imm
                            // -------------
                            // 0x83 /0 ib  ADD r/m16, imm8
                            // 0x81 /0 iw  ADD r/m16, imm16
                            log()->debug("r/m16: ", dst_reg);
                            const uint8_t modrm =
                                ModRM::generate_modrm(ModRM::REG, dst_reg, ModRM::SLASH_0);

                            log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                         op, modrm, src_token.AsLong());

                            binout_container.push_back(op);
                            binout_container.push_back(modrm);
                            if (is_imm8(src_token.AsString())) {
                                binout_container.push_back(src_token.AsLong());
                            } else {
                                set_word_into_binout(src_token.AsLong(), binout_container);
                            }

                        } else {
                            const uint8_t op = is_imm8(src_token.AsString()) ? 0x83 : 0x81;
                            // ADD Reg32, Imm
                            // -------------
                            // 0x83 /0 ib  ADD r/m32, imm8
                            // 0x81 /0 id  ADD r/m32, imm32
                            log()->debug("r/m32: ", dst_reg);
                            const uint8_t modrm =
                                ModRM::generate_modrm(ModRM::REG, dst_reg, ModRM::SLASH_0);

                            store_register_size_prefix(dst_reg, binout_container);
                            log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                         op, modrm, src_token.AsLong());

                            binout_container.push_back(op);
                            binout_container.push_back(modrm);
                            if (is_imm8(src_token.AsString())) {
                                binout_container.push_back(src_token.AsLong());
                            } else {
                                set_dword_into_binout(src_token.AsLong(), binout_container);
                            }
                        }
                        break;
                    }
                    break;

                } else if (token.Is("[") &&
                           is_common_register(token_table, tokenizer.LookAhead(1)) &&
                           tokenizer.LookAhead(2).Is("]") &&
                           tokenizer.LookAhead(3).Is(",") &&
                           is_legitimate_numeric(tokenizer.LookAhead(4).AsString())) {
                    // ADD Mem,Imm8
                    // ADD Mem,Imm
                } else {
                    std::cerr << "NASK : ADD syntax error " << token.AsString() << std::endl;
                    return 17;
                }

                break;
            }
        }
        return 0;
    }

    int Instructions::process_token_ALIGNB(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        TParaToken token = tokenizer.Next();
        if (!token.IsEmpty() || token.AsLong() % 2 == 0 ) {
            log()->debug("ALIGNB {}", token.AsString());
            const size_t unit = static_cast<int>(token.AsLong());
            const size_t nearest_size = binout_container.size() / unit + 1;
            const size_t times = nearest_size * unit - binout_container.size();

            log()->debug("ALIGNB stores 0x00 {} times", times);
            for (size_t l = 0; l < times; l++) {
                binout_container.push_back(0x00);
            }
        } else {
            std::cerr << "NASK : ALIGNB syntax error " << token.AsString() << std::endl;
            return 17;
        }
        return 0;
    }

    // 簡単なAND命令の実装
    int Instructions::process_token_AND(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (ModRM::is_accumulator(token.AsString()) && tokenizer.LookAhead(1).Is(",")) {
                    // 0x24 ib AND AL, imm8  ALとimm8とのANDをとる
                    // 0x25 iw AND AX, imm16 AXとimm16とのANDをとる
                    // 0x25 id AND EAX,imm32 EAXとimm32とのANDをとる
                    if (token.Is("AL")) {
                        log()->debug("0x24 {}", tokenizer.LookAhead(2).AsString());
                        binout_container.push_back(0x24);
                        binout_container.push_back(tokenizer.LookAhead(2).AsLong());
                    } else if (token.Is("AX")) {
                        log()->debug("0x25 {}", tokenizer.LookAhead(2).AsString());
                        binout_container.push_back(0x25);
                        set_word_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    } else { // EAX
                        log()->debug("0x25 {}", tokenizer.LookAhead(2).AsString());
                        store_register_size_prefix(token.AsString(), binout_container);
                        binout_container.push_back(0x25);
                        set_dword_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    }
                }
                break;
            }
        }
        return 0;
    }

    // 簡単なCALL命令の実装
    int Instructions::process_token_CALL(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                continue;
            } else if (tokenizer.LookAhead(1).Is(":")) {
                // 0x9A cd	CALL ptr16:16	絶対ファーコール、オペランドでアドレスを指定
                // 0x9A cp	CALL ptr16:32	絶対ファーコール、オペランドでアドレスを指定
                long ptr16 = 0x00;
                long addr  = 0x00;
                if (tokenizer.LookAhead(1).Is(":")) {
                    ptr16 = token.AsLong();
                    addr  = tokenizer.LookAhead(2).AsLong();
                } else if (tokenizer.LookAhead(2).Is(":")) {
                    log()->debug("Register-size prefix: 0x66");
                    binout_container.push_back(0x66);
                    ptr16 = tokenizer.LookAhead(1).AsLong();
                    addr  = tokenizer.LookAhead(3).AsLong();
                }

                log()->debug("NIM(W): 0x9a, 0x{:02x}, 0x{:02x}", addr, ptr16);
                binout_container.push_back(0x9a);
                set_dword_into_binout(addr, binout_container);
                set_word_into_binout(ptr16, binout_container);
                break;

            } else if (token.Is("FAR") && tokenizer.LookAhead(1).Is("[")) {
                // CALL m16:16 | 0xFF /3
                // CALL m16:32 | 0xFF /3
                const std::string reg    = tokenizer.LookAhead(2).AsString();
                const std::string op     = tokenizer.LookAhead(3).AsString();
                const std::string disp_s = tokenizer.LookAhead(4).AsString();
                const uint8_t disp       = tokenizer.LookAhead(4).AsLong();
                const std::string mem    = "[" + reg + op + disp_s + "]";
                log()->debug("far call [{}{}{}]", reg, op, disp);
                const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_DISP8, mem, ModRM::SLASH_3);

                log()->debug("NIM(W): 0xff, 0x{:02x}, 0x{:02x}", modrm, disp);
                binout_container.push_back(0xff);
                binout_container.push_back(modrm);

                if (disp_s != "" && ModRM::get_rm_from_reg(reg) == ModRM::SIB) {
                    const uint8_t sib = ModRM::generate_sib(mem, reg);
                    log()->debug("SIB: 0x{:02x}", sib);
                    binout_container.push_back(sib);
                }
                binout_container.push_back(disp);
                break;

            } else if (starts_with(token.AsString(), "_")) {
                const std::string store_label = token.AsString();
                log()->debug("label stored: ", store_label);
                log()->debug("0xe8, 0x00000000");

                // Count offset
                this->symbol_offsets[store_label] = this->symbol_offsets["_"] + 1;
                // EXTERN symbol
                binout_container.push_back(0xe8);
                binout_container.push_back(0x00);
                binout_container.push_back(0x00);
                binout_container.push_back(0x00);
                binout_container.push_back(0x00);
                break;
            } else {
                const std::string store_label = token.AsString();
                log()->debug("label stored: ", store_label);
                log()->debug("0xe8, 0x0000");

                if (dst_is_stored(store_label)) {
                    update_label_src_offset(store_label, binout_container, 0xe8);
                } else {
                    store_label_src(store_label, binout_container, false, imm16);
                    binout_container.push_back(0xe8);
                    binout_container.push_back(0x00);
                    binout_container.push_back(0x00);
                }
                break;
            }
        }
        return 0;
    }

    // 簡単なCLI命令の実装
    int Instructions::process_token_CLI(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xFA を格納
        binout_container.push_back(0xfa);
        return 0;
    }

    // 簡単なCMP命令の実装
    int Instructions::process_token_CMP(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {

                if (is_common_register(token_table, token) &&
                    tokenizer.LookAhead(1).Is(",")  &&
                    is_common_register(token_table, tokenizer.LookAhead(2))) {
                    // CMP Reg,Reg
                } else if (token.Is("[") &&
                           is_common_register(token_table, tokenizer.LookAhead(1)) &&
                           tokenizer.LookAhead(2).Is("]") &&
                           tokenizer.LookAhead(3).Is(",") &&
                           is_common_register(token_table, tokenizer.LookAhead(4))) {
                    // CMP Mem,Reg
                } else if (is_common_register(token_table, token) &&
                           tokenizer.LookAhead(1).Is(",")  &&
                           tokenizer.LookAhead(2).Is("[")  &&
                           is_common_register(token_table, tokenizer.LookAhead(3))) {
                    // CMP Reg,Mem
                    // 0x3A /r | CMP r8,  r/m8  r/m8をr8と比較します
                    // 0x3B /r | CMP r16, r/m16 r/m16をr16と比較します
                    // 0x3B /r | CMP r32, r/m32 r/m32をr32と比較します

                    // ex1) CMP EAX,[ESP+20]
                    // ex2) CMP EDI,[EBX]
                    // ex3) CMP ESI,[EBX]
                    bool exists_disp = tokenizer.LookAhead(6).Is("]") ? true : false;
                    TParaToken dst_token = token;
                    const std::string dst_reg = dst_token.AsString();

                    std::string src_mem = "";
                    std::string disp = "";
                    if (exists_disp) {
                        src_mem = "[";
                        src_mem += tokenizer.LookAhead(3).AsString();
                        src_mem += tokenizer.LookAhead(4).AsString();
                        src_mem += tokenizer.LookAhead(5).AsString();
                        src_mem += "]";
                        disp = tokenizer.LookAhead(5).AsString();
                    } else {
                        src_mem = "[";
                        src_mem += tokenizer.LookAhead(3).AsString();
                        src_mem += "]";
                    }

                    log()->debug("exists disp ? => {}", exists_disp);

                    std::smatch match;
                    const ModRM::mods mod_kind = exists_disp ? ModRM::REG_DISP8 : ModRM::REG_REG;

                    if (regex_match(dst_reg, match, ModRM::regImm08)) {
                        const uint8_t op = 0x3a;
                        const uint8_t modrm = ModRM::generate_modrm(op, mod_kind, src_mem, dst_reg);
                        log()->debug("NIM(B): 0x3a, 0x{:02x}", modrm);
                        binout_container.push_back(op);
                        binout_container.push_back(modrm);
                    } else {
                        const uint8_t op = 0x3b;
                        const uint8_t modrm = ModRM::generate_modrm(op, mod_kind, src_mem, dst_reg);
                        log()->debug("NIM(B): 0x3b, 0x{:02x}", modrm);
                        binout_container.push_back(op);
                        binout_container.push_back(modrm);
                    }
                    if (disp != "" && ModRM::get_rm_from_reg(src_mem) == ModRM::SIB) {
                        const uint8_t sib = ModRM::generate_sib(exists_disp ? src_mem : dst_reg, src_mem);
                        log()->debug("SIB: 0x{:02x}", sib);
                        binout_container.push_back(sib);
                    }
                    if (tokenizer.LookAhead(4).Is("+")) {
                        const uint8_t disp = tokenizer.LookAhead(5).AsLong();
                        log()->debug("Disp: 0x{:02x}", disp);
                        binout_container.push_back(disp);
                    }

                    // 終わり
                    break;

                } else if (is_common_register(token_table, token) &&
                           tokenizer.LookAhead(1).Is(",")  &&
                           is_legitimate_numeric(get_equ_label_or_asis(tokenizer.LookAhead(2).AsString()))) {
                    // CMP Acc,Imm
                    // CMP Reg,Imm8
                    // CMP Reg,Imm
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg      = dst_token.AsString();
                    const std::string src_imm_str  = get_equ_label_or_asis(src_token.AsString());
                    const long src_imm = (src_imm_str != src_token.AsString()) ?
                        std::stol(src_imm_str) : src_token.AsLong();

                    // 次へ
                    tokenizer.Next();
                    tokenizer.Next();
                    log()->debug("{} == {}", dst_reg, src_imm);
                    std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

                    std::smatch match;
                    if (regex_match(dst_reg, match, ModRM::rm000)) {
                        // AL|AX|EAX なので "CMP Acc,Imm" に決定
                        // CMP Acc,Imm
                        // 0011110 + w
                        const std::bitset<8> bs_dst("0011110" + std::get<1>(tp_dst));

                        // debug logs
                        log()->debug("NIM(B): 0x{:02x}, 0x{:02x}", bs_dst.to_ulong(), src_imm);
                        binout_container.push_back(bs_dst.to_ulong());

                        if (is_imm8(src_imm_str)) {
                            binout_container.push_back(src_imm);
                        } else {
                            set_word_into_binout(src_imm, binout_container);
                        }

                    } else {
                        // 8086 Opecodeの表のほうが間違えてる

                        // CMP Reg8, Imm
                        // ------------
                        // 0x80 /7 ib | CMP r/m8, imm8
                        //
                        // CMP Reg16, Imm
                        // ------------
                        // 0x81 /7 iw | CMP r/m16, imm16
                        // 0x83 /7 ib | CMP r/m16, imm8
                        //
                        // CMP Reg32, Imm
                        // ------------
                        // 0x80 /7 id | CMP r/m32, imm32
                        // 0x83 /7 ib | CMP r/m32, imm8
                        //
                        if (is_between_bytesize(src_imm) &&
                            regex_match(dst_reg, match, ModRM::regImm08)) {
                            // CMP Reg8, Imm
                            // ------------
                            // 0x80 /7 ib | CMP r/m8, imm8
                            log()->debug("r/m8: ", dst_reg);
                            const std::bitset<8> bs_dst2("11111" + std::get<0>(tp_dst));

                            // debug logs
                            log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                         0x80,
                                         bs_dst2.to_ulong(),
                                         src_imm);

                            binout_container.push_back(0x80);
                            binout_container.push_back(bs_dst2.to_ulong());
                            binout_container.push_back(src_imm);

                        } else if (regex_match(dst_reg, match, ModRM::regImm16)) {

                            const uint8_t op = is_imm8(src_imm_str) ? 0x83 : 0x81;
                            // CMP Reg16, Imm
                            // ------------
                            // 0x83 /7 ib | CMP r/m16, imm8
                            // 0x81 /7 iw | CMP r/m16, imm16
                            log()->debug("r/m16: ", dst_reg);
                            const std::bitset<8> bs_dst2("11111" + std::get<0>(tp_dst));

                            // debug logs
                            log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                         op, bs_dst2.to_ulong(), src_imm);

                            binout_container.push_back(op);
                            binout_container.push_back(bs_dst2.to_ulong());
                            if (op == 0x83) {
                                binout_container.push_back(src_imm);
                            } else {
                                set_word_into_binout(src_imm, binout_container);
                            }
                        } else {
                            // CMP Reg32, Imm
                            // ------------
                            // 0x80 /7 id | CMP r/m32, imm32
                            // 0x83 /7 ib | CMP r/m32, imm8
                            // 未実装
                        }
                    }
                    break;

                } else if (token.Is("[") &&
                           is_common_register(token_table, tokenizer.LookAhead(1)) &&
                           tokenizer.LookAhead(2).Is("]") &&
                           tokenizer.LookAhead(3).Is(",") &&
                           is_legitimate_numeric(tokenizer.LookAhead(4).AsString())) {
                    // CMP Mem,Imm8
                    // CMP Mem,Imm

                } else if (is_datatype(token_table, token) &&
                           tokenizer.LookAhead(1).Is("[")) {
                    // CMP BYTE Mem,Imm
                    //
                    // 0x80 /7 ib	CMP r/m8,  imm8	 imm8をr/m8と比較します
                    // 0x83 /7 ib	CMP r/m16, imm8	 imm8をr/m16と比較します
                    // 0x83 /7 ib	CMP r/m32, imm8	 imm8をr/m32と比較します
                    // 0x81 /7 iw	CMP r/m16, imm16 imm16をr/m16と比較します
                    // 0x80 /7 id	CMP r/m32, imm32 imm32をr/m32と比較します
                    //
                    const std::string seg_reg = tokenizer.LookAhead(2).AsString();
                    const std::string dst_reg = tokenizer.LookAhead(4).AsString();
                    const std::string op_disp = tokenizer.LookAhead(5).AsString();
                    const std::string vl_disp = tokenizer.LookAhead(6).AsString();
                    const std::string src_imm = tokenizer.LookAhead(9).AsString();

                    const uint8_t disp = tokenizer.LookAhead(6).AsLong();
                    const uint8_t imm  = tokenizer.LookAhead(9).AsLong();

                    log()->debug("[{}{}{}{}{}],{}",
                                 seg_reg,
                                 tokenizer.LookAhead(3).AsString(),
                                 dst_reg,
                                 op_disp,
                                 vl_disp,
                                 imm);

                    std::smatch match;
                    store_segment_override_prefix(seg_reg, binout_container);
                    if (regex_match(dst_reg, match, ModRM::regImm08) || token.Is("BYTE")) {
                        const uint8_t op = 0x80;
                        const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_DISP8,
                                                                    "[" + dst_reg + op_disp + vl_disp + "]",
                                                                    ModRM::SLASH_7);
                        log()->debug("NIM(B): 0x80, 0x{:02x}, 0x{:02x}, 0x{:02x}", modrm, disp, imm);
                        binout_container.push_back(op);
                        binout_container.push_back(modrm);
                        binout_container.push_back(disp);
                        binout_container.push_back(imm);
                    } else {
                        const uint8_t op = 0x83;
                        const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_DISP8,
                                                                    "[" + dst_reg + op_disp + vl_disp + "]",
                                                                    ModRM::SLASH_7);
                        log()->debug("NIM(B): 0x83, 0x{:02x}, 0x{:02x}, 0x{:02x}", modrm, disp, imm);
                        binout_container.push_back(op);
                        binout_container.push_back(modrm);
                        binout_container.push_back(disp);
                        binout_container.push_back(imm);
                    }
                    break;
                } else {
                    std::cerr << "NASK : CMP syntax error" << std::endl;
                    return 17;
                }

                break;
            }
        }
        return 0;
    }

    // 簡単なDB命令の実装
    int Instructions::process_token_DB(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                continue;
            } else if (token.IsQuote()) {
                std::string str = token.AsString();
                if (str.front() == '"' && str.back() == '"') {
                    str.erase( 0, 1 );
                    str.erase( str.size() - 1 );
                    for ( uint8_t b : str ) {
                        binout_container.push_back(b);
                    }
                } else {
                    std::cerr << "NASK : DB not quoted correctly' " << std::endl;
                    return 17;
                }
            } else {
                binout_container.push_back(token.AsLong());
            }
        }
        return 0;
    }

    // 簡単なDW命令の実装
    //
    // NASM produces WORDs anyhow ;-)
    //
    // dw 'a' is equivalent to dw 0x61 and stores the WORD 0x0061 (big-endian) as 61 00 (little-endian).
    // dw 'ab' (little-endian) is equivalent to dw 0x6261 (big-endian) and stores 61 62 (little-endian).
    // dw 'abc' (one word, one byte) is equivalent to dw 0x6261, 0x63 and stores two WORDS (little-endian): 61 62 63 00.
    // dw 'abcd' (two words) stores two WORDs: 61 62 63 64.
    //
    // msg2 dw "hello world",10,13,0 transfers the string into 6 words and
    // the numbers to 3 words and stores it:
    //   68 65 6C 6C 6F 20 77 6F 72 6C 64 00 0A 00 0D 00.
    // In your example, msg won't be printed until its end.
    //
    int Instructions::process_token_DW(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                continue;
            } else if (token.IsQuote()) {
                std::string str = token.AsString();
                if (str.front() == '"' && str.back() == '"') {
                    str.erase( 0, 1 );
                    str.erase( str.size() - 1 );
                    str.push_back(0x00); // 終端文字

                    if (str.size() % 2 == 1) {
                        // WORD単位なので、1BYTE少なければ足しておく
                        str.push_back(0x00);
                    }
                    for ( uint8_t b : str ) {
                        binout_container.push_back(b);
                    }
                } else {
                    std::cerr << "NASK : DW not quoted correctly' " << std::endl;
                    return 17;
                }
            } else if ( !is_legitimate_numeric(token.AsString()) && !token.IsEmpty() ) {
                // DW
                const std::string store_label = token.AsString();
                const long abs_size = get_label_src_offset(store_label);
                log()->debug("DW found rel: {}", store_label);
                log()->debug("DW absolute size: {}", abs_size);
                set_word_into_binout(abs_size, binout_container);
                break;
            } else {
                // DWを解釈, 0x00の際でもWORDで格納
                set_word_into_binout(token.AsLong(), binout_container, false);
                continue;
            }
        }
        return 0;
    }

    int Instructions::process_token_EQU(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (tokenizer.LookAhead(1).Is("EQU")) {
                    if (token.IsEmpty() || tokenizer.LookAhead(2).IsEmpty()) {
                        std::cerr << "NASK : EQU syntax is not correct" << std::endl;
                        return 17;
                    } else {
                        const std::string key = token.AsString();
                        const std::string val = tokenizer.LookAhead(2).AsString();
                        log()->debug("{} is keeped as {} because of EQU", key, val);
                        this->equ_map[key] = val;
                        return 0;
                    }
                } else {
                    std::cerr << "NASK : EQU syntax is not correct" << std::endl;
                    return 17;
                }
            }
        }
        return 0;
    }

    int Instructions::process_token_EXTERN(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is("EXTERN")) {
                continue;
            } else {
                if (!token.IsEmpty()) {
                    const std::string head_symbol = trim(token.AsString());
                    log()->debug("Add new symbol: {}", head_symbol);
                    gl_symbol_list.push_back(head_symbol);
                    ex_symbol_list.push_back(head_symbol);
                    symbol_offsets[head_symbol] = 0;

                    for ( size_t i = 2; ; i+=2) {
                        if ( !tokenizer.LookAhead(i).IsEmpty() && tokenizer.LookAhead(i-1).Is(",")) {
                            const std::string tail = trim(tokenizer.LookAhead(i).AsString());
                            log()->debug("Add new symbol: {}", tail);
                            gl_symbol_list.push_back(tail);
                            ex_symbol_list.push_back(tail);
                            symbol_offsets[tail] = 0;
                        } else {
                            break;
                        }
                    }
                }
                break;
            }
        }
        return 0;
    }

    int Instructions::process_token_GLOBAL(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is("GLOBAL")) {
                continue;
            } else {
                if (!token.IsEmpty()) {
                    const std::string head_symbol = trim(token.AsString());
                    log()->debug("Add new symbol: {}", head_symbol);
                    gl_symbol_list.push_back(head_symbol);
                    symbol_offsets[head_symbol] = 0;

                    for ( size_t i = 2; ; i+=2) {
                        if ( !tokenizer.LookAhead(i).IsEmpty() && tokenizer.LookAhead(i-1).Is(",")) {
                            const std::string tail = trim(tokenizer.LookAhead(i).AsString());
                            log()->debug("Add new symbol: {}", tail);
                            gl_symbol_list.push_back(tail);
                            symbol_offsets[tail] = 0;
                        } else {
                            break;
                        }
                    }
                }
                break;
            }
        }
        return 0;
    }

    // 簡単なHLT命令の実装
    int Instructions::process_token_HLT(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xF4 を格納
        binout_container.push_back(0xf4);
        return 0;
    }

    // IMUL命令の実装
    int Instructions::process_token_IMUL(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (is_register(token_table, token) && tokenizer.LookAhead(1).Is(",")) {
                    // 0x6B /r ib | IMUL r16, imm8  | r16を符号拡張即値imm8で符号付き乗算し、結果をr16に格納します
                    // 0x69 /r iw | IMUL r16, imm16 | r16を即値imm16で符号付き乗算し、結果をr16に格納します
                    // 0x6B /r ib | IMUL r32, imm8  | r32を符号拡張即値imm8で符号付き乗算し、結果をr32に格納します
                    // 0x69 /r iw | IMUL r32, imm32 | r32を即値imm32で符号付き乗算し、結果をr16に格納します
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg  = dst_token.AsString();
                    const std::string src_imm  = src_token.AsString();
                    std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
                    const uint8_t nim = is_imm8(src_imm) ? 0x6b : 0x69;
                    const std::bitset<8> bs_dst("11" + std::get<0>(tp_dst) + std::get<0>(tp_dst));

                    std::smatch match;
                    if (regex_match(dst_reg, match, ModRM::regImm16)) {
                        // IMUL r16, imm8  = RegWord,RegWord,Imm8 | 01101011oorrrmmm
                        // IMUL r16, imm16 = RegWord,RegWord,Imm  | 01101001oorrrmmm

                        // [mod] 10 : A 16-bit signed displacement follows the opcode
                        // [reg] rrr: from dest register
                        // [r/m] rrr: from dest register
                        log()->debug("NIM(B): 0x{:02x}, 0x{:02x}, 0x{:02x}", nim, bs_dst.to_ulong(), src_token.AsLong());
                        binout_container.push_back(nim);
                        binout_container.push_back(bs_dst.to_ulong());
                        if (is_imm8(src_imm)) {
                            binout_container.push_back(src_token.AsLong());
                        } else {
                            set_word_into_binout(src_token.AsLong(), binout_container);
                        }
                        break;
                    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                        // IMUL r32, imm8  = RegWord,RegWord,Imm8 | 01101011oorrrmmm
                        // IMUL r32, imm32 = RegWord,RegWord,Imm  | 01101001oorrrmmm

                        // [mod] 10 : A 16-bit signed displacement follows the opcode
                        // [reg] rrr: from dest register
                        // [r/m] rrr: from dest register
                        log()->debug("NIM(W): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}", nim, bs_dst.to_ulong(), src_token.AsLong());
                        binout_container.push_back(0x66);
                        binout_container.push_back(nim);
                        binout_container.push_back(bs_dst.to_ulong());
                        if (is_imm8(src_imm)) {
                            binout_container.push_back(src_token.AsLong());
                        } else {
                            set_dword_into_binout(src_token.AsLong(), binout_container);
                        }
                        break;
                    } else {
                        std::cerr << "NASK : IMUL specified strange register" << std::endl;
                        return 17;
                    }

                } else {
                    std::cerr << "NASK : IMUL syntax is strange" << std::endl;
                    return 17;
                }
                break;
            }
        }
        return 0;
    }

    // 簡単なIN命令の実装
    int Instructions::process_token_IN(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xE4 ib IN AL,  imm8 I/Oポートアドレスimm8からALにバイトを入力します
        // 0xE5 ib IN AX,  imm8 I/Oポートアドレスimm8からAXにワードを入力します
        // 0xE5 ib IN EAX, imm8 I/Oポートアドレスimm8からEAXにダブルワードを入力します
        // 0xEC    IN AL,  DX	  DXで指定するI/OポートアドレスからALにバイトを入力します
        // 0xED    IN AX,  DX	  DXで指定するI/OポートアドレスからAXにワードを入力します
        // 0xED    IN EAX, DX	  DXで指定するI/OポートアドレスからEAXにダブルワードを入力します
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (ModRM::is_accumulator(token.AsString()) && tokenizer.LookAhead(1).Is(",")) {

                    const std::string src_reg = token.AsString();
                    store_register_size_prefix(src_reg, binout_container);
                    if (tokenizer.LookAhead(2).Is("DX")) {
                        const uint8_t nim = token.Is("AL") ? 0xec : 0xed;
                        log()->debug("NIM(B): 0x{:02x}", nim);
                        binout_container.push_back(nim);
                    } else {
                        const uint8_t imm = tokenizer.LookAhead(2).AsLong();
                        const uint8_t nim = token.Is("AL") ? 0xe4 : 0xe5;
                        log()->debug("NIM(B): 0x{:02x}, 0x{:02x}", nim, imm);
                        binout_container.push_back(nim);
                        binout_container.push_back(imm);
                    }
                } else {
                    std::cerr << "NASK : IN instruction param should be accumlator" << std::endl;
                    return 17;
                }
                break;
            }
        }
        return 0;
    }

    // 簡単なINT命令の実装
    int Instructions::process_token_INT(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xCC    INC 3       割り込み3（デバッガへのトラップ）
        // 0xCD ib INC imm8    割り込みベクタ番号のバイトサイズの即値による指定
        // 0xCE    INCO        割り込み4（OFが1である場合）
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (is_legitimate_numeric(token.AsString())) {
                    binout_container.push_back(0xcd);
                    binout_container.push_back(token.AsLong());
                } else {
                    std::cerr << "NASK : INT param is not correct" << std::endl;
                    return 17;
                }
                break;
            }
        }
        return 0;
    }

    // テキトーなIRET命令の実装
    int Instructions::process_token_IRET(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xCF を格納
        binout_container.push_back(0xcf);
        return 0;
    }

    // 簡単なDD命令の実装
    int Instructions::process_token_DD(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                continue;
            } else if (token.IsQuote()) {
                std::string str = token.AsString();
                if (str.front() == '"' && str.back() == '"') {
                    str.erase( 0, 1 );
                    str.erase( str.size() - 1 );
                    str.push_back(0x00); // 終端文字

                    if (str.size() % 2 == 1) {
                        // DWORD単位なので、1BYTE少なければ足しておく
                        str.push_back(0x00);
                    }
                    for ( uint8_t b : str ) {
                        binout_container.push_back(b);
                    }
                } else {
                    std::cerr << "NASK : DD not quoted correctly" << std::endl;
                    return 17;
                }

            } else if ( !is_legitimate_numeric(token.AsString()) && !token.IsEmpty() ) {
                // DD
                const std::string store_label = token.AsString();
                const long abs_size = get_label_src_offset(store_label);
                log()->debug("DD found rel: {}", store_label);
                log()->debug("DD absolute size: {}", abs_size);
                // DDだけどWORDサイズで格納？？
                set_word_into_binout(abs_size, binout_container);
                break;
            } else {
                // DDを解釈
                set_dword_into_binout(token.AsLong(), binout_container);
                continue;
            }
        }
        return 0;
    }

    // 簡単なOR命令の実装
    int Instructions::process_token_OR(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (tokenizer.LookAhead(1).Is(",") &&
                    is_hex_notation(tokenizer.LookAhead(2).AsString())) {

                    std::smatch match;
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg  = dst_token.AsString();
                    const std::string src_imm  = src_token.AsString();

                    // [mod] 11, [reg] 001: /1, [r/m] xxx
                    std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
                    const std::bitset<8> bs_dst("11001" + std::get<0>(tp_dst));

                    if (regex_match(dst_reg, match, ModRM::regImm08)) {
                        // 0x80 /1 ib | OR r/m8, imm8
                        log()->debug("NIM(B): 0x66, 0x80, 0x{:02x}, 0x{:02x}",
                                     bs_dst.to_ulong(), src_token.AsLong());

                        binout_container.push_back(0x66);
                        binout_container.push_back(0x80);
                        binout_container.push_back(bs_dst.to_ulong());
                        binout_container.push_back(src_token.AsLong());

                    } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                        // 0x81 /1 iw | OR r/m16, imm16
                        // 0x83 /1 ib | OR r/m16, imm8
                        const uint8_t nim = is_between_bytesize(src_token.AsLong()) ? 0x83 : 0x81;
                        log()->debug("NIM(B/W): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                     nim,
                                     bs_dst.to_ulong(),
                                     src_token.AsLong());

                        binout_container.push_back(0x66);
                        binout_container.push_back(nim);
                        binout_container.push_back(bs_dst.to_ulong());
                        if (is_between_bytesize(src_token.AsLong())) {
                            binout_container.push_back(src_token.AsLong());
                        } else {
                            set_word_into_binout(src_token.AsLong(), binout_container);
                        }

                    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                        // 0x81 /1 id | OR r/m32, imm32
                        // 0x83 /1 ib | OR r/m32, imm8
                        const uint8_t nim = is_between_bytesize(src_token.AsLong()) ? 0x83 : 0x81;
                        log()->debug("NIM(B/W): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}",
                                     nim,
                                     bs_dst.to_ulong(),
                                     src_token.AsLong());

                        binout_container.push_back(0x66);
                        binout_container.push_back(nim);
                        binout_container.push_back(bs_dst.to_ulong());
                        if (is_between_bytesize(src_token.AsLong())) {
                            binout_container.push_back(src_token.AsLong());
                        } else {
                            set_word_into_binout(src_token.AsLong(), binout_container);
                        }
                    }

                    break;

                } else if (ModRM::is_accumulator(token.AsString()) && tokenizer.LookAhead(1).Is(",")) {
                    // 0x0C ib | OR AL, imm8  ALとimm8とのORをとる
                    // 0x0D iw | OR AX, imm16 AXとimm16とのORをとる
                    // 0x0D id | OR EAX,imm32 EAXとimm32とのORをとる
                    if (token.Is("AL")) {
                        log()->debug("0x66 0x0c {}", tokenizer.LookAhead(2).AsString());
                        binout_container.push_back(0x66);
                        binout_container.push_back(0x0c);
                        binout_container.push_back(tokenizer.LookAhead(2).AsLong());
                    } else if (token.Is("AX")) {
                        log()->debug("0x66 0x0d {}", tokenizer.LookAhead(2).AsString());
                        binout_container.push_back(0x66);
                        binout_container.push_back(0x0d);
                        set_word_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    } else { // EAX
                        log()->debug("0x66 0x0d {}", tokenizer.LookAhead(2).AsString());
                        binout_container.push_back(0x66);
                        binout_container.push_back(0x0d);
                        set_dword_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    }
                    // ここで終わりのはず
                    break;
                }
                break;
            }
        }
        return 0;
    }

    // 簡単なORG命令の実装
    int Instructions::process_token_ORG(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                std::cerr << "NASK : ORG contains unused ',' " << std::endl;
                return 17;
            } else {
                log()->debug(token.AsString());

                if (is_legitimate_numeric(token.AsString())) {
                    dollar_position = token.AsLong();
                    log()->debug("ORG = ", dollar_position);
                    break;
                } else {
                    std::cerr << "NASK : ORG specified incorrect value" << std::endl;
                    return 17;
                }
            }
        }
        return 0;
    }

    // 簡単なOUT命令の実装
    int Instructions::process_token_OUT(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {

                if (is_legitimate_numeric(token.AsString()) &&
                    tokenizer.LookAhead(1).Is(",") &&
                    ModRM::is_accumulator(tokenizer.LookAhead(2).AsString())) {
                    // 0xE6 ib | OUT imm8, AL
                    // 0xE7 ib | OUT imm8, AX
                    // 0xE7 ib | OUT imm8, EAX
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_imm  = dst_token.AsString();
                    const std::string src_reg  = src_token.AsString();
                    log()->debug("{} <= {}", dst_imm, src_reg);

                    store_register_size_prefix(src_reg, binout_container);
                    const uint8_t opecode = (src_reg == "AL") ? 0xe6 : 0xe7;
                    binout_container.push_back(opecode);
                    binout_container.push_back(dst_token.AsLong());

                    log()->debug("NIM(W): 0x{:02x}, 0x{:02x}", opecode, dst_token.AsLong());
                    break;

                } else if (token.Is("DX") && tokenizer.LookAhead(1).Is(",") &&
                           ModRM::is_accumulator(tokenizer.LookAhead(2).AsString())) {
                    // 0xEE | OUT DX, AL
                    // 0xEF | OUT DX, AX
                    // 0xEF | OUT DX, EAX
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg  = dst_token.AsString();
                    const std::string src_reg  = src_token.AsString();
                    log()->debug("{} <= {}", dst_reg, src_reg);

                    store_register_size_prefix(src_reg, binout_container);
                    const uint8_t opecode = (src_reg == "AL") ? 0xee : 0xef;
                    binout_container.push_back(opecode);
                    log()->debug("NIM(B): 0x{:02x}", opecode);
                    break;

                } else {
                    std::cerr << "NASK : OUT specified incorrect value" << std::endl;
                    return 17;
                }
            }
        }
        return 0;
    }

    // POP命令の実装
    int Instructions::process_token_POP(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {

                if (is_segment_register(token_table, token)) {
                    // 0x1F      | POP DS
                    // 0x07      | POP ES
                    // 0x17      | POP SS
                    // 0x0F 0xA1 | POP FS
                    // 0x0F 0xA9 | POP GS
                    // ないよ！  | POP CS
                    TParaToken dst_token = token;
                    const std::string dst_reg  = dst_token.AsString();
                    log()->debug("POP from {}", dst_reg);

                    uint8_t opecode_b = 0x00;
                    uint16_t opecode_w = 0x0000;

                    if (dst_reg == "DS") {
                        opecode_b = 0x1f;
                    } else if (dst_reg == "ES") {
                        opecode_b = 0x07;
                    } else if (dst_reg == "SS") {
                        opecode_b = 0x17;
                    } else if (dst_reg == "FS") {
                        opecode_w = 0xa10f;
                    } else if (dst_reg == "GS") {
                        opecode_w = 0xa90f;
                    } else {
                        std::cerr << "NASK : POP specified incorrect register" << std::endl;
                        return 17;
                    }

                    if (opecode_b != 0x00) {
                        binout_container.push_back(opecode_b);
                        log()->debug("NIM(B): 0x{:02x}", opecode_b);
                    } else {
                        set_word_into_binout(opecode_w, binout_container);
                        log()->debug("NIM(W): 0x{:02x}", opecode_w);
                    }
                    break;

                } else if (is_register(token_table, token)) {
                    std::smatch match;
                    TParaToken dst_token = token;
                    const std::string dst_reg  = dst_token.AsString();

                    if (regex_match(dst_reg, match, ModRM::regImm16)) {

                        // 0x58+rw | POP r16 <-- AX, BX, CX, DX
                        log()->debug("POP from {}", dst_reg);
                        const uint8_t opecode = plus_number_from_code(0x58, dst_reg);
                        log()->debug("NIM(B): 0x{:02x}", opecode);
                        binout_container.push_back(opecode);

                    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {

                        // 0x58+rd | POP r32 <-- EAX, EBX, ECX, EDX
                        log()->debug("POP from {}", dst_reg);
                        const uint8_t opecode = plus_number_from_code(0x58, dst_reg);
                        log()->debug("NIM(B): 0x{:02x}", opecode);
                        binout_container.push_back(opecode);
                    } else {
                        std::cerr << "NASK : POP specified incorrect register" << std::endl;
                        return 17;
                    }
                    break;

                } else {
                    std::cerr << "NASK : POP specified incorrect value" << std::endl;
                    return 17;
                }
            }
        }
        return 0;
    }

    // テキトーなPOPA命令の実装
    int Instructions::process_token_POPA(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0x61 を格納
        binout_container.push_back(0x61);
        return 0;
    }

    // テキトーなPOPF命令の実装
    int Instructions::process_token_POPF(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0x9D を格納
        binout_container.push_back(0x9d);
        return 0;
    }

    // PUSH命令の実装
    int Instructions::process_token_PUSH(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {

                if (token.Is("[") || tokenizer.LookAhead(1).Is("[")) {
                    // 0xFF /6   | PUSH r/m16
                    // 0xFF /6   | PUSH r/m32
                    const size_t reg_index   = token.Is("[") ? 0 : 1;
                    const std::string reg    = tokenizer.LookAhead(reg_index+1).AsString();
                    const std::string mem    = "[" + reg + "]";
                    log()->debug("PUSH [{}] as {}", reg, token.AsString());

                    uint8_t modrm = 0x00;
                    if (is_datatype(token_table, token)) {
                        modrm = ModRM::generate_modrm_imm(ModRM::REG_REG, token.AsString(), ModRM::SLASH_6);
                    } else {
                        modrm = ModRM::generate_modrm(ModRM::REG_REG, mem, ModRM::SLASH_6);
                    }

                    log()->debug("NIM(B): 0xff, 0x{:02x}", modrm);
                    binout_container.push_back(0xff);
                    binout_container.push_back(modrm);

                    if (is_hex_notation(reg) && is_datatype(token_table, token)) {
                        const long addr = tokenizer.LookAhead(reg_index+1).AsLong();
                        if (token.Is("BYTE")) {
                            binout_container.push_back(addr);
                        } else if (token.Is("WORD")) {
                            set_word_into_binout(addr, binout_container);
                        } else if (token.Is("DWORD")) {
                            set_dword_into_binout(addr, binout_container);
                        } else {
                            std::cerr << "NASK : PUSH specified incorrect memory address" << std::endl;
                            return 17;
                        }
                    }
                    break;

                } else if (is_legitimate_numeric(token.AsString())) {
                    // 0x6A      | PUSH imm8
                    // 0x68      | PUSH imm16
                    // 0x68      | PUSH imm32
                    const uint8_t op = is_imm8(token.AsString()) ? 0x6a : 0x68;
                    log()->debug("NIM(B): 0x{:02x}", op);
                    binout_container.push_back(op);
                    binout_container.push_back(token.AsLong());
                    break;

                } else if (is_segment_register(token_table, token)) {
                    // 0x0E      | PUSH CS
                    // 0x16      | PUSH SS
                    // 0x1E      | PUSH DS
                    // 0x06      | PUSH ES
                    // 0x0F 0xA0 | PUSH FS
                    // 0x0F 0xA8 | PUSH GS
                    TParaToken dst_token = token;
                    const std::string dst_reg  = dst_token.AsString();
                    log()->debug("PUSH from {}", dst_reg);

                    uint8_t opecode_b = 0x00;
                    uint16_t opecode_w = 0x0000;

                    if (dst_reg == "DS") {
                        opecode_b = 0x1e;
                    } else if (dst_reg == "ES") {
                        opecode_b = 0x06;
                    } else if (dst_reg == "SS") {
                        opecode_b = 0x16;
                    } else if (dst_reg == "CS") {
                        opecode_b = 0x0e;
                    } else if (dst_reg == "FS") {
                        opecode_w = 0xa00f;
                    } else if (dst_reg == "GS") {
                        opecode_w = 0xa80f;
                    } else {
                        std::cerr << "NASK : PUSH specified incorrect register" << std::endl;
                        return 17;
                    }

                    if (opecode_b != 0x00) {
                        binout_container.push_back(opecode_b);
                        log()->debug("NIM(B): 0x{:02x}", opecode_b);
                    } else {
                        set_word_into_binout(opecode_w, binout_container);
                        log()->debug("NIM(W): 0x{:02x}", opecode_w);
                    }
                    break;

                } else if (is_register(token_table, token)) {

                    std::smatch match;
                    TParaToken dst_token = token;
                    const std::string dst_reg  = dst_token.AsString();

                    if (regex_match(dst_reg, match, ModRM::regImm16)) {
                        // 0x50+rw | PUSH r16 <-- AX, BX, CX, DX
                        log()->debug("PUSH from {}", dst_reg);
                        const uint8_t opecode = plus_number_from_code(0x50, dst_reg);
                        log()->debug("NIM(B): 0x{:02x}", opecode);
                        binout_container.push_back(opecode);

                    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                        // 0x50+rd | PUSH r32 <-- EAX, EBX, ECX, EDX
                        log()->debug("PUSH from {}", dst_reg);
                        const uint8_t opecode = plus_number_from_code(0x50, dst_reg);
                        log()->debug("NIM(B): 0x{:02x}", opecode);
                        binout_container.push_back(opecode);

                    } else {
                        std::cerr << "NASK : POP specified incorrect register" << std::endl;
                        return 17;
                    }
                    break;

                } else {
                    std::cerr << "NASK : PUSH specified incorrect value" << std::endl;
                    return 17;
                }
            }
        }
        return 0;
    }

    // テキトーなPUSHA/PUSHAD命令の実装
    int Instructions::process_token_PUSHA(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0x60 を格納
        binout_container.push_back(0x60);
        return 0;
    }

    // テキトーなPUSHF命令の実装
    int Instructions::process_token_PUSHF(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0x9C を格納
        binout_container.push_back(0x9c);
        return 0;
    }

    // テキトーなRET命令の実装
    int Instructions::process_token_RET(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xC3 を格納
        binout_container.push_back(0xc3);
        return 0;
    }

    int Instructions::process_token_RETF(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xCB を格納
        binout_container.push_back(0xcb);
        return 0;
    }

    // 簡単なRESB命令の実装
    int Instructions::process_token_RESB(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else if (token.Is(",")) {
                continue;
            } else {
                if (tokenizer.LookAhead(1).AsString() == "-" && tokenizer.LookAhead(2).AsString() == "$") {
                    // ハイフンを処理する、どこまで埋めるか取得する
                    for (uint32_t l = binout_container.size(); l < token.AsLong() - dollar_position; l++) {
                        binout_container.push_back(0x00);
                    }

                    // ハイフン処理は確定なのでtokenを進めておく
                    token = tokenizer.Next();
                    token = tokenizer.Next();
                } else {
                    for (uint32_t l = 0; l < token.AsLong(); l++) {
                        binout_container.push_back(0x00);
                    }
                }
            }
        }
        return 0;
    }

    // SHR命令の実装
    int Instructions::process_token_SHR(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {

                if (is_common_register(token_table, token)) {
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg  = dst_token.AsString();
                    const std::string src  = src_token.AsString();

                    // 次へ
                    tokenizer.Next();
                    tokenizer.Next();
                    log()->debug("{} / 2 {} times", dst_reg, src);

                    std::smatch match;
                    if (regex_match(dst_reg, match, ModRM::regImm08)) {
                        // 0xD0 /5     SHR r/m8        r/m8を2で1回符号なし除算します※
                        // 0xD2 /5     SHR r/m8, CL    r/m8を2でCL回符号なし除算します※
                        // 0xC0 /5 ib  SHR r/m8, imm8  r/m8を2でimm8回符号なし除算します※
                        const uint8_t op  = src_token.IsEmpty() ? 0xd0 : ( src == "CL" ? 0xd2 : 0xc0 );
                        const uint8_t nim = ModRM::generate_modrm(ModRM::REG, dst_reg, ModRM::SLASH_5);

                        // debug logs
                        log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, {}", op, nim, src_token.AsString());

                        binout_container.push_back(op);
                        binout_container.push_back(nim);
                        if (op == 0xc0) {
                            binout_container.push_back(src_token.AsLong());
                        }

                    } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                        // 0xD1 /5     SHR r/m16       r/m16を2で1回符号なし除算します※
                        // 0xD3 /5     SHR r/m16, CL   r/m16を2でCL回符号なし除算します※
                        // 0xC1 /5 ib  SHR r/m16, imm8 r/m16を2でimm8回符号なし除算します※
                        const uint8_t op  = src_token.IsEmpty() ? 0xd1 : ( src == "CL" ? 0xd3 : 0xc1 );
                        const uint8_t nim = ModRM::generate_modrm(ModRM::REG, dst_reg, ModRM::SLASH_5);

                        // debug logs
                        log()->debug("NIM(W): 0x{:02x}, 0x{:02x}, {}", op, nim, src_token.AsString());

                        binout_container.push_back(op);
                        binout_container.push_back(nim);
                        if (op == 0xc1) {
                            binout_container.push_back(src_token.AsLong());
                        }

                    } else {
                        // 0xD1 /5     SHR r/m32       r/m32を2で1回符号なし除算します※
                        // 0xD3 /5     SHR r/m32, CL   r/m32を2でCL回符号なし除算します※
                        // 0xC1 /5 ib  SHR r/m32, imm8 r/m32を2でimm8回符号なし除算します※
                        const uint8_t op  = src_token.IsEmpty() ? 0xd1 : ( src == "CL" ? 0xd3 : 0xc1 );
                        const uint8_t nim = ModRM::generate_modrm(ModRM::REG, dst_reg, ModRM::SLASH_5);

                        // debug logs
                        log()->debug("NIM(W): 0x66, 0x{:02x}, 0x{:02x}, {}", op, nim, src_token.AsString());

                        binout_container.push_back(0x66);
                        binout_container.push_back(op);
                        binout_container.push_back(nim);
                        if (op == 0xc1) {
                            binout_container.push_back(src_token.AsLong());
                        }
                    }
                    break;

                } else {
                    // error
                    std::cerr << "NASK : SHR syntax error" << std::endl;
                    return 17;
                }

            }
        }
        return 0;
    }

    // テキトーなSTI命令の実装
    int Instructions::process_token_STI(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
        // 0xFB を格納
        binout_container.push_back(0xfb);
        return 0;
    }

    // 簡単なSUB命令の実装
    int Instructions::process_token_SUB(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (ModRM::is_accumulator(token.AsString()) && tokenizer.LookAhead(1).Is(",")) {
                    // 0x2C ib | SUB AL, imm8   | ALからimm8を引きます
                    // 0x2D iw | SUB AX, imm16  | AXからimm16を引きます
                    // 0x2D id | SUB EAX, imm32 | EAXからimm32を引きます
                    const uint8_t nim = token.Is("AL") ? 0x2c : 0x2d;
                    const uint8_t imm = tokenizer.LookAhead(2).AsLong();
                    log()->debug("NIM(B): 0x{:02x}, 0x{:02x}", nim, imm);
                    binout_container.push_back(nim);
                    if (token.Is("AL")) {
                        binout_container.push_back(imm);
                    } else if (token.Is("AX")) {
                        set_word_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    } else if (token.Is("EAX")) {
                        set_dword_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    }
                    break; // end

                } else if (is_legitimate_numeric(tokenizer.LookAhead(2).AsString())) {
                    // 0x80 /5 ib | SUB r/m8, imm8   | r/m8からimm8を引きます
                    // 0x81 /5 iw | SUB r/m16, imm16 | r/m16からimm16を引きます
                    // 0x81 /5 id | SUB r/m32, imm32 | r/m32からimm32を引きます
                    // 0x83 /5 ib | SUB r/m16, imm8  | r/m16から符号拡張したimm8を引きます
                    // 0x83 /5 ib | SUB r/m32, imm8  | r/m32から符号拡張したimm8を引きます
                    std::smatch match;
                    TParaToken dst_token = token;
                    TParaToken src_token = tokenizer.LookAhead(2);
                    const std::string dst_reg = dst_token.AsString();
                    const std::string src_imm = src_token.AsString();
                    const std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

                    // [mod] 11 :
                    // [reg] 101: /5
                    // [r/m] xxx:
                    const std::bitset<8> bs_dst("11101" + std::get<0>(tp_dst));
                    const uint8_t nim2 = bs_dst.to_ulong();

                    if (regex_match(dst_reg, match, ModRM::regImm08)) {
                        // 0x80 /5 ib | SUB r/m8, imm8   | r/m8からimm8を引きます
                        log()->debug("NIM(B): 0x80, 0x{:02x}, 0x{:02x}", nim2, tokenizer.LookAhead(2).AsLong());
                        binout_container.push_back(0x80);
                        binout_container.push_back(nim2);
                        binout_container.push_back(tokenizer.LookAhead(2).AsLong());
                    } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                        // 0x81 /5 iw | SUB r/m16, imm16 | r/m16からimm16を引きます
                        // 0x83 /5 ib | SUB r/m16, imm8  | r/m16から符号拡張したimm8を引きます

                        // FIXME: nask side process is strange,
                        // ex1) imm is "512/4", => 0x83
                        // ex2) imm is "1",     => 0x81
                        const uint8_t nim1 = is_imm8(src_imm) ? 0x83 : 0x81;
                        log()->debug("NIM(B): 0x{:02x}, 0x{:02x}, 0x{:02x}", nim1, nim2, tokenizer.LookAhead(2).AsLong());
                        binout_container.push_back(nim1);
                        binout_container.push_back(nim2);
                        if (nim1 == 0x83) {
                            binout_container.push_back(tokenizer.LookAhead(2).AsLong());
                        } else {
                            set_word_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                        }
                    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                        // 0x81 /5 id | SUB r/m32, imm32 | r/m32からimm32を引きます
                        // 0x83 /5 ib | SUB r/m32, imm8  | r/m32から符号拡張したimm8を引きます

                        // FIXME: nask side process is strange,
                        // ex1) imm is "512/4", => 0x83
                        // ex2) imm is "1",     => 0x81
                        const uint8_t nim1 = is_imm8(src_imm) ? 0x83 : 0x81;
                        log()->debug("NIM(B): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}", nim1, nim2, tokenizer.LookAhead(2).AsLong());
                        binout_container.push_back(0x66);
                        binout_container.push_back(nim1);
                        binout_container.push_back(nim2);
                        if (nim1 == 0x83) {
                            binout_container.push_back(tokenizer.LookAhead(2).AsLong());
                        } else {
                            set_dword_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                        }
                    } else {
                        // memory
                        std::cerr << "NASK : SUB syntax error, imm from memory is not supported now" << std::endl;
                        return 17;
                    }
                    break; // end

                } else {
                    // not implemented
                    std::cerr << "NASK : SUB syntax error, not supported now" << std::endl;
                    return 17;
                }
                break;
            }
        }
        return 0;
    }


    // XOR命令の実装
    int Instructions::process_token_XOR(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

        for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
            if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
                break;
            } else {
                if (ModRM::is_accumulator(token.AsString()) && tokenizer.LookAhead(1).Is(",")) {
                    // 0x34 ib | XOR AL,  imm8  | ALとimm8の排他的論理和
                    // 0x35 iw | XOR AX,  imm16 | AXとimm16の排他的論理和
                    // 0x35 id | XOR EAX, imm32 | EAXとimm32の排他的論理和
                    const uint8_t nim = token.Is("AL") ? 0x34 : 0x35;
                    const uint8_t imm = tokenizer.LookAhead(2).AsLong();
                    log()->debug("NIM(B): 0x{:02x}, 0x{:02x}", nim, imm);
                    binout_container.push_back(nim);
                    if (token.Is("AL")) {
                        binout_container.push_back(imm);
                    } else if (token.Is("AX")) {
                        set_word_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    } else if (token.Is("EAX")) {
                        set_dword_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
                    }
                    break; // end

                } else if (is_legitimate_numeric(tokenizer.LookAhead(2).AsString()) ||
                           // XOR EBX, 0xffffffff
                           (is_datatype(token_table, token) && tokenizer.LookAhead(1).Is("["))
                           // XOR DWORD [EBX],0xffffffff
                          ) {

                    // 0x80 /6 ib | XOR r/m8,  imm8  | r/m8とimm8との排他的論理和をとります
                    // 0x83 /6 ib | XOR r/m16, imm8  | r/m16と符号拡張したimm8との排他的論理和をとります
                    // 0x83 /6 ib | XOR r/m32, imm8  | r/m32と符号拡張したimm8との排他的論理和をとります

                    // 以下は実装しない
                    // 0x81 /6 iw | XOR r/m16, imm16 | r/m16とimm16との排他的論理和をとります
                    // 0x81 /6 id | XOR r/m32, imm32 | r/m32とimm32との排他的論理和をとります
                    const bool using_data_type = is_datatype(token_table, token);
                    const bool dst_is_mem = (token.Is("[") || tokenizer.LookAhead(1).Is("["));

                    std::smatch match;

                    TParaToken dst_token;
                    TParaToken src_token;

                    log()->debug("using_data_type: {}, dst_is_mem: {}", using_data_type, dst_is_mem);
                    if (using_data_type && dst_is_mem) {
                        // XOR DWORD [EBX],0xffffffff
                        dst_token = tokenizer.LookAhead(2);
                        src_token = tokenizer.LookAhead(5);
                    } else if (using_data_type && !dst_is_mem) {
                        // XOR DWORD EBX,0xffffffff
                        dst_token = tokenizer.LookAhead(1);
                        src_token = tokenizer.LookAhead(3);
                    } else if (!using_data_type && dst_is_mem) {
                        // XOR [EBX],0xffffffff
                        dst_token = tokenizer.LookAhead(2);
                        src_token = tokenizer.LookAhead(5);
                    } else {
                        // XOR EBX,0xffffffff
                        dst_token = token;
                        src_token = tokenizer.LookAhead(2);
                    }

                    const std::string dst_reg = dst_token.AsString();
                    const std::string src_imm = src_token.AsString();
                    log()->debug("dst_reg: {}, src_imm: {}", dst_reg, src_imm);

                    if (regex_match(dst_reg, match, ModRM::regImm08)) {
                        // 0x80 /6 ib | XOR r/m8,  imm8  | r/m8とimm8との排他的論理和をとります
                        const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, dst_reg, ModRM::SLASH_6);
                        log()->debug("NIM(B): 0x80, 0x{:02x}, 0x{:02x}", modrm, src_token.AsLong());
                        binout_container.push_back(0x80);
                        binout_container.push_back(modrm);
                        binout_container.push_back(src_token.AsLong());
                    } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
                        // 0x83 /6 ib | XOR r/m16, imm8  | r/m16と符号拡張したimm8との排他的論理和をとります
                        const uint8_t op    = 0x83;
                        const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, dst_reg, ModRM::SLASH_6);
                        log()->debug("NIM(B): 0x{:02x}, 0x{:02x}, 0x{:02x}", op, modrm, src_token.AsLong());
                        binout_container.push_back(op);
                        binout_container.push_back(modrm);
                        binout_container.push_back(src_token.AsLong());
                    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
                        // 0x83 /6 ib | XOR r/m32, imm8  | r/m32と符号拡張したimm8との排他的論理和をとります
                        const uint8_t op    = 0x83;
                        const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, dst_reg, ModRM::SLASH_6);
                        log()->debug("NIM(B): 0x{:02x}, 0x{:02x}, 0x{:02x}", op, modrm, src_token.AsLong());
                        binout_container.push_back(op);
                        binout_container.push_back(modrm);
                        binout_container.push_back(src_token.AsLong());
                    } else {
                        // memory
                        std::cerr << "NASK : XOR syntax error" << std::endl;
                        return 17;
                    }
                    break; // end

                } else {
                    // not implemented
                    std::cerr << "NASK : XOR syntax error, not supported now" << std::endl;
                    return 17;
                }
                break;
            }
        }
        return 0;
    }
}
