#include "nask_utility.hpp"

namespace nask_utility {
     namespace ModRM {
	  const std::string get_rm_from_reg(const std::string& src_reg) {
	       std::smatch match;
	       if (regex_match(src_reg, match, rm000)) {
		    return "000";
	       } else if (regex_match(src_reg, match, rm001)) {
		    return "001";
	       } else if (regex_match(src_reg, match, rm010)) {
		    return "010";
	       } else if (regex_match(src_reg, match, rm011)) {
		    return "011";
	       }

	       return "test";
	  };

	  uint8_t generate_modrm(enum mods m, const std::string& dst_reg, const std::string& src_reg) {
	       std::string modrm = ModRM::MOD_TO_STR.at(m);
	       modrm += SEGMENT_REGISTERS_SSS_MAP.at(dst_reg);
	       modrm += get_rm_from_reg(src_reg);
	       std::bitset<8> bs(modrm);
	       return bs.to_ulong();
	  };
     };

     bool is_legitimate_numeric(const std::string& s) {
	  return is_hex_notation(s) || is_integer(s);
     }

     bool is_hex_notation(const std::string& s) {
	  return s.compare(0, 2, "0x") == 0
	       && s.size() > 2
	       && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
     }

     bool is_integer(const std::string& s) {
	  if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
	  char * p ;
	  strtol(s.c_str(), &p, 10) ;
	  return (*p == 0) ;
     }

     std::ifstream::pos_type filesize(const char* filename) {
	  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	  return in.tellg();
     }

     std::vector<std::string> split(const std::string &str, char delim) {
	  std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
	  while(std::getline(iss, tmp, delim)) res.push_back(tmp);
	  return res;
     }

     bool ends_with(std::string const &full_string, std::string const &ending) {
	  if (full_string.length() >= ending.length()) {
	       return (0 == full_string.compare(full_string.length() - ending.length(), ending.length(), ending));
	  } else {
	       return false;
	  }
     }

     size_t get_labelpos(std::ifstream& stream, std::string token) {
	  std::string line;
	  for (long line_number = 1; std::getline(stream, line); ++line_number) {
	       if (line.find(token) != std::string::npos) {
		    return line_number;
	       }
	  }
	  return -1;
     }

     bool is_comment_line(TParaCxxTokenTable& token_table, TParaToken& token) {
	  return token_table.IsCommentLimiter(token.AsString());
     }

     bool is_line_terminated(TParaCxxTokenTable& token_table, TParaToken& token) {
	  return token.AsString() == "\n";
     }

     bool is_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  // レジスタ一覧から検索してあれば true
	  auto it = std::find_if(std::begin(REGISTERS), std::end(REGISTERS),
				 [&](const std::string& s)
				 { return token.AsString().find(s) != std::string::npos; });

	  return it != std::end(REGISTERS);
     }

     bool is_segment_register(TParaCxxTokenTable& token_table, TParaToken& token) {
	  // レジスタ一覧から検索してあれば true
	  auto it = std::find_if(std::begin(SEGMENT_REGISTERS), std::end(SEGMENT_REGISTERS),
				 [&](const std::string& s)
				 { return token.AsString().find(s) != std::string::npos; });

	  return it != std::end(SEGMENT_REGISTERS);
     }

     template <class T> void plus_number_from_code(T& num, char c) {
	  switch(c) {
	  case 'A':
	       num += 0;
	       break;
	  case 'C':
	       num += 1;
	       break;
	  case 'D':
	       num += 2;
	       break;
	  case 'B':
	       num += 3;
	       break;
	  }
     }

     // +rb
     uint8_t get_plus_register_code(uint8_t byte, char c) {
	  plus_number_from_code(byte, c);
	  return byte;
     }
     // +rw
     uint16_t get_plus_register_code(uint16_t word, char c) {
	  plus_number_from_code(word, c);
	  return word;
     }
     // +rd
     uint32_t get_plus_register_code(uint32_t dword, char c) {
	  plus_number_from_code(dword, c);
	  return dword;
     }

     void set_nimonic_with_register(const std::string& reg,
				    NIMONIC_INFO* nim_info,
				    TParaTokenizer& tokenizer) {

	  if (reg == "AL" || reg == "BL" || reg == "CL" || reg == "DL") {
	       // prefix = "B0+rb" (AL:+0, CL:+1, DL:+2, BL:+3)
	       nim_info->prefix = get_plus_register_code((uint8_t) 0xb0, reg.at(0));
	       nim_info->reg = reg;
	       nim_info->imm = imm8;
	  } else if (reg == "EAX" || reg == "EBX" || reg == "ECX" || reg == "EDX") {
	       // prefix = "B8+rd" (EAX:+0, EBX:+1, ECX:+2, EDX:+3)
	       nim_info->prefix = get_plus_register_code((uint8_t) 0xb8, reg.at(1));
	       nim_info->reg = reg;
	       nim_info->imm = imm32;
	  } else {
	       // tokenizerを先読みしてみる
	       TParaToken src_token = tokenizer.LookAhead(2);

	       // Reg, Immの場合 => 1011wrrr
	       std::tuple<std::string, std::string> tp = ModRM::REGISTERS_RRR_MAP.at(reg);
	       const std::bitset<8> bs("1011" + std::get<1>(tp) + std::get<0>(tp));
	       nim_info->prefix = bs.to_ulong();

	       if (is_legitimate_numeric(src_token.AsString())) {
		    nim_info->reg = reg;
		    // レジスタの種類を見る
		    std::smatch match;
		    if (regex_match(reg, match, ModRM::regImm08)) {
			 nim_info->imm = imm8;
		    } else {
			 nim_info->imm = imm16;
		    }
	       } else {
		    nim_info->reg = reg;
		    nim_info->imm = offs; // ターゲットはoffset
	       }
	  }

	  return;
     }

     // MOV命令でoffsetが見つかった時に呼び出す
     void Instructions::set_offset_rel_stack(std::string store_label,
					     VECTOR_BINOUT& binout_container,
					     int src_index,
					     int rel_index,
					     OPERAND_KINDS operand) {

	  // 見つかったoffset情報を記録
	  OFFSET_ELEMENT elem;
	  elem.label = store_label;
	  elem.src_index = (src_index == -1) ? binout_container.size() - 1 : src_index;
	  elem.rel_index = (rel_index == -1) ? binout_container.size()     : rel_index;
	  elem.operand = operand;
	  offsets.push_back(elem);
     }

     void Instructions::update_offset_rel_stack(std::string found_label, VECTOR_BINOUT& binout_container) {

	  std::cout << "updating a label for offsets...: " << found_label << std::endl;
	  auto it = std::find_if(std::begin(offsets), std::end(offsets),
				 [&](const OFFSET_ELEMENT& elem)
				 { return elem.label.find(found_label) != std::string::npos; });

	  if (it != std::end(offsets)) {
	       // 見つかったJMP情報を記録
	       std::cout << "found a label from stacked" << std::endl;
	       OFFSET_ELEMENT elem(*it);
	       elem.dst_index = binout_container.size();
	       //offsets.erase(it);
	       // JMP先のアドレスをアップデートする
	       std::cout.setf(std::ios::hex, std::ios::basefield);
	       if (elem.operand == ID_Rel16) {
		    std::cout << "bin[" << std::to_string(elem.rel_index) << "] = " << elem.dst_index << std::endl;
		    std::cout << "bin[" << std::to_string(elem.rel_index + 1) << "] = " << 0x7c << std::endl;
		    binout_container[elem.rel_index] = elem.dst_index;
		    binout_container[elem.rel_index + 1] = 0x7c;
	       } else {
		    std::cout << "bin[" << std::to_string(elem.rel_index) << "] = "
			      << elem.rel_offset() - 1 << std::endl;
		    binout_container[elem.rel_index] = elem.rel_offset() - 1;
		    // std::cout << "bin[" << std::to_string(elem.rel_index) << "] = " << elem.dst_index << std::endl;
		    // binout_container[elem.rel_index] = elem.dst_index;
	       }
	  } else {
	       // 例外を起こしたほうがよさそう
	       std::cout << "not found a label from stacked" << std::endl;
	  }
     }

     // "JMPオペコード"が見つかった時に呼び出す
     void Instructions::set_jmp_stack(std::string store_label, VECTOR_BINOUT& binout_container) {

	  auto it = std::find_if(std::begin(stack), std::end(stack),
				 [&](const JMP_STACK_ELEMENT& elem)
				 { return elem.label.find(store_label) != std::string::npos; });

	  if (it != std::end(stack)) {
	       std::cout << "found a label from stacked" << std::endl;
	       JMP_STACK_ELEMENT elem(*it);
	       elem.src_index = binout_container.size();
	       elem.rel_index = binout_container.size() + 1;

	       // JMPのアドレスをアップデートする
	       std::cout.setf(std::ios::dec, std::ios::basefield);
	       std::cout << "elem.rel_index: " << elem.rel_index << std::endl;
	       std::cout << "elem.dst_index: " << elem.dst_index << std::endl;
	       std::cout.setf(std::ios::hex, std::ios::basefield);
	       std::cout << "bin[" << std::to_string(binout_container.size() + 1) << "] = "
			 << elem.rel_offset() - 1 << std::endl;

	       // とりあえず0xEBのみ実装
	       binout_container.push_back(0xeb);
	       binout_container.push_back(elem.rel_offset() - 1);

	  } else {
	       // 見つかったJMP情報を記録
	       JMP_STACK_ELEMENT elem;
	       elem.label = store_label;
	       elem.src_index = binout_container.size();
	       elem.rel_index = binout_container.size() + 1;
	       stack.push_back(elem);

	       // とりあえず0xEBのみ実装
	       binout_container.push_back(0xeb);
	       binout_container.push_back(0x00);
	  }
     }

     // "ラベル"が見つかった時に呼び出す
     void Instructions::update_jmp_stack(std::string found_label, VECTOR_BINOUT& binout_container) {

	  std::cout << "updating a label for jmp...: " << found_label << std::endl;
	  auto it = std::find_if(std::begin(stack), std::end(stack),
				 [&](const JMP_STACK_ELEMENT& elem)
				 { return elem.label.find(found_label) != std::string::npos; });

	  if (it != std::end(stack)) {
	       // 見つかったJMP情報を記録
	       std::cout << "found a label from stacked" << std::endl;
	       JMP_STACK_ELEMENT elem(*it);
	       elem.dst_index = binout_container.size();
	       stack.erase(it);
	       // JMP先のアドレスをアップデートする
	       std::cout.setf(std::ios::dec, std::ios::basefield);
	       std::cout << "elem.rel_index: " << elem.rel_index << std::endl;
	       std::cout << "elem.dst_index: " << elem.dst_index << std::endl;
	       std::cout.setf(std::ios::hex, std::ios::basefield);
	       std::cout << "bin[" << std::to_string(elem.rel_index) << "] = "
			 << elem.rel_offset() - 1 << std::endl;
	       binout_container[elem.rel_index] = elem.rel_offset() - 1;
	  } else {
	       std::cout << "store a found label into stack" << std::endl;
	       // 見つかったラベル情報を記録
	       JMP_STACK_ELEMENT elem;
	       elem.label = found_label;
	       elem.dst_index = binout_container.size();
	       stack.push_back(elem);
	  }
     }

     // @detail uint16_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的には0x00をバイトサイズで格納する傾向があるので、そうじゃない場合はフラグを設定する
     //
     // @param word             格納するWORDサイズのバイナリ
     // @param binout_container 出力先コンテナ
     // @param zero_as_byte     0x00をバイトサイズで格納する
     //
     void set_word_into_binout(const uint16_t& word,
			       VECTOR_BINOUT& binout_container,
			       bool zero_as_byte) {

	  if (word == 0x0000 && zero_as_byte) {
	       // push_back only 1byte
	       binout_container.push_back(0x00);
	  } else {
	       // http://stackoverflow.com/a/1289360/2565527
	       const uint8_t first_byte  = (word >> 8) & 0xff;
	       const uint8_t second_byte = word & 0xff;
	       // push_back as little_endian
	       binout_container.push_back(second_byte);
	       binout_container.push_back(first_byte);
	  }
     }

     // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的にはDDは0x00を普通に詰めるらしい（仕様ブレブレすぎだろ…）
     void set_dword_into_binout(const uint32_t& dword, VECTOR_BINOUT& binout_container, bool zero_as_byte) {

	  if (dword == 0x00000000 && zero_as_byte) {
	       // push_back only 1byte
	       binout_container.push_back(0x00);
	  } else {
	       uint32_t cp_dword = dword;
	       uint8_t bytes[4];

	       bytes[0] = (cp_dword >> 24) & 0xff;
	       bytes[1] = (cp_dword >> 16) & 0xff;
	       bytes[2] = (cp_dword >> 8)  & 0xff;
	       bytes[3] = cp_dword & 0xff;

	       //printf("[0-3] => 0x%02x 0x%02x 0x%02x 0x%02x", bytes[0], bytes[1], bytes[2], bytes[3]);
	       // push_back as little_endian
	       binout_container.push_back(bytes[3]);
	       binout_container.push_back(bytes[2]);
	       binout_container.push_back(bytes[1]);
	       binout_container.push_back(bytes[0]);
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
     TParaCxxTokenTable Instructions::token_table;
     JMP_STACK Instructions::stack;
     OFFS_STACK Instructions::offsets;
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
	       } else if (is_segment_register(token_table, token)) {
		    //
		    // 8E /r | MOV Sreg,r/m16** | Move r/m16 to segment register
		    //
		    std::cout << "MOV REGISTER: " << token.AsString();
		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(2);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_reg  = src_token.AsString();

		    // MOV Sreg, register の時
		    if (tokenizer.LookAhead(1).Is(",") &&
			!tokenizer.LookAhead(2).IsEmpty() &&
			is_register(token_table, src_token)) {
			 // コンマを飛ばして次へ
			 token = tokenizer.Next();
			 token = tokenizer.Next();
			 std::cout << " <= " << token.AsString();

			 const uint8_t modrm = ModRM::generate_modrm(ModRM::REG, dst_reg, src_reg);
			 binout_container.push_back(0x8e);
			 binout_container.push_back(modrm);
			 // これで終了のはず
			 std::cout << " : NIM: ";
			 std::cout << std::showbase << std::hex << 0x8e;
			 std::cout << std::showbase << std::hex << static_cast<int>(modrm) << std::endl;
			 break;
		    }

	       } else if (is_register(token_table, token) &&
			  tokenizer.LookAhead(1).Is(",")  &&
		          tokenizer.LookAhead(2).Is("[")  &&
		          tokenizer.LookAhead(4).Is("]")) {
		    //
		    // MOV Reg, Mem | 1000101woorrrmmm
		    //---------------------------------
		    // 0x8A /r	MOV r8, r/m8		r/m8をr8に転送します
		    // 0x8B /r	MOV r16, r/m16		r/m16をr16に転送します
		    // 0x8B /r	MOV r32, r/m32		r/m32をr32に転送します
		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(3);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_mem  = "[" + src_token.AsString() + "]";

		    std::cout << dst_reg << " <= " << src_mem << std::endl;

		    // Reg, Immの場合 => 1000101w oorrrmmm
		    std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
		    const std::string tp_src = ModRM::REGISTERS_MMM_MAP.at(src_mem);

		    // 1000101 + w
		    const std::bitset<8> bs_dst("1000101" + std::get<1>(tp_dst));
		    // oo + rrr + mmm
		    const std::bitset<8> bs_src("00" + std::get<0>(tp_dst) + tp_src);
		    binout_container.push_back(bs_dst.to_ulong());
		    binout_container.push_back(bs_src.to_ulong());

		    std::cout << "NIM(W): ";
		    std::cout << std::showbase << std::hex
			      << static_cast<int>(bs_dst.to_ulong());
		    std::cout << ", ";
		    std::cout << std::showbase << std::hex
			      << static_cast<int>(bs_src.to_ulong()) << std::endl;

		    // コンマを飛ばして次へ
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    // これで終了のはず
		    break;

	       } else if (is_register(token_table, token) &&
			  tokenizer.LookAhead(1).Is(",")) {
		    //
		    // MOV Reg, Imm | 1011wrrr
		    //--------------------------------------------------------
		    // 0xB0+rb	MOV r8, imm8	        imm8をr8に転送します
		    // 0xB8+rw	MOV r16, imm16	        imm16をr16に転送します
		    // 0xB8+rd	MOV r32, imm32	        imm32をr32に転送します
		    //
		    NIMONIC_INFO nim_info;
		    set_nimonic_with_register(token.AsString(), &nim_info, tokenizer);

		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(2);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_imm  = src_token.AsString();

		    // コンマを飛ばして次へ
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    std::cout << dst_reg << " <= " << src_imm << ", imm => " << nim_info.imm;

		    const uint16_t nim = nim_info.prefix;
		    if (nim > 0x6600) {
			 const uint8_t first_byte  = nim & 0xff;
			 const uint8_t second_byte = (nim >> 8);
			 binout_container.push_back(second_byte);
			 binout_container.push_back(first_byte);
			 // debug logs
			 std::cout << " NIM(W): ";
			 std::cout << std::showbase << std::hex
				   << static_cast<int>(second_byte);
			 std::cout << ", ";
			 std::cout << std::showbase << std::hex
				   << static_cast<int>(first_byte);
			 std::cout << ", ";
			 std::cout << std::showbase << std::hex
				   << static_cast<int>(token.AsLong()) << std::endl;

		    } else {
			 const uint8_t first_byte  = nim & 0xff;
			 const uint8_t second_byte = (nim >> 8);
			 binout_container.push_back(first_byte);
			 // debug logs
			 std::cout << " NIM:(B) ";
			 std::cout << std::showbase << std::hex
				   << static_cast<int>(first_byte);

			 if (nim_info.imm != offs) {
			      std::cout << ", ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(token.AsLong()) << std::endl;
			 } else {
			      std::cout << ", " << token.AsString() << std::endl;
			 }
		    }

		    // 即値(imm)を設定
		    if (nim_info.imm == imm8) {
			 binout_container.push_back(token.AsLong());
		    } else if (nim_info.imm == imm16) {
			 set_word_into_binout(token.AsLong(), binout_container, false);
		    } else if (nim_info.imm == imm32) {
			 set_dword_into_binout(token.AsLong(), binout_container, false);
		    } else if (nim_info.imm == offs) {
			 std::cout << " offset processing !" << std::endl;
			 set_offset_rel_stack(token.AsString(), binout_container, -1, -1, ID_Rel16);
			 // とりあえずoffsetには0x00を入れておき、見つかった時に更新する
			 binout_container.push_back(0x00);
			 binout_container.push_back(0x7c);
		    } else {
			 std::cerr << "NASK : MOV imm could not set correctly " << std::endl;
			 return 17;
		    }
		    // これで終了のはず
		    break;
	       } else {
		    binout_container.push_back(token.AsLong());
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
			 std::cout << "label: " << store_label << std::endl;
			 set_offset_rel_stack(store_label, binout_container, binout_container.size(), binout_container.size() + 1);
			 // とりあえずoffsetには0x00を入れておき、見つかった時に更新する
			 binout_container.push_back(0x74);
			 binout_container.push_back(0x00);
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
	       } else {
		    const std::string store_label = token.AsString();
		    std::cout << "label stored: " << store_label << std::endl;
		    set_jmp_stack(store_label, binout_container);
		    break;
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

		    if (is_register(token_table, token) &&
			tokenizer.LookAhead(1).Is(",")  &&
			is_register(token_table, tokenizer.LookAhead(2))) {
			 // MOV Reg,Reg
		    } else if (token.Is("[") &&
			       is_register(token_table, tokenizer.LookAhead(1)) &&
			       tokenizer.LookAhead(2).Is("]") &&
			       tokenizer.LookAhead(3).Is(",") &&
			       is_register(token_table, tokenizer.LookAhead(4))) {
			 // MOV Mem,Reg
		    } else if (is_register(token_table, token) &&
			       tokenizer.LookAhead(1).Is(",")  &&
			       tokenizer.LookAhead(2).Is("[")  &&
			       is_register(token_table, tokenizer.LookAhead(3)) &&
			       tokenizer.LookAhead(4).Is("]")) {
			 // MOV Reg,Mem
		    } else if (is_register(token_table, token) &&
			       tokenizer.LookAhead(1).Is(",")  &&
			       is_legitimate_numeric(tokenizer.LookAhead(2).AsString())) {
			 // MOV Acc,Imm
			 // MOV Reg,Imm8
			 // MOV Reg,Imm
			 TParaToken dst_token = token;
			 TParaToken src_token = tokenizer.LookAhead(2);
			 const std::string dst_reg  = dst_token.AsString();
			 const std::string src_imm  = src_token.AsString();

			 // 次へ
			 tokenizer.Next();
			 tokenizer.Next();
			 std::cout << dst_reg << " <= " << src_imm << std::endl;
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

			 if (src_token.AsLong() <= std::numeric_limits<int8_t>::max() &&
			     src_token.AsLong() >= std::numeric_limits<int8_t>::min()) {
			      // MOV Reg,Imm8 に決定 int8_t 内に収まっているので
			      // NIM: 1000001w oo000mmm
			      // 1000001 + w
			      const std::bitset<8> bs_dst("1000001" + std::get<1>(tp_dst));
			      // oo + 000 + mmm
			      // FIXME: mmmではなくrrrかもしれない
			      const std::bitset<8> bs_src("11000" + std::get<0>(tp_dst));

			      // debug logs
			      std::cout << "NIM(W): ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(bs_dst.to_ulong());
			      std::cout << ", ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(bs_src.to_ulong());
			      std::cout << ", ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(src_token.AsLong()) << std::endl;

			      binout_container.push_back(bs_dst.to_ulong());
			      binout_container.push_back(bs_src.to_ulong());
			      binout_container.push_back(src_token.AsLong());
			      break;

			 } else {
			      // MOV Acc,Imm
			      // MOV Reg,Imm
			 }
			 break;

		    } else if (token.Is("[") &&
			       is_register(token_table, tokenizer.LookAhead(1)) &&
			       tokenizer.LookAhead(2).Is("]") &&
			       tokenizer.LookAhead(3).Is(",") &&
			       is_legitimate_numeric(tokenizer.LookAhead(4).AsString())) {
			 // MOV Mem,Imm8
			 // MOV Mem,Imm
		    } else {
			 std::cerr << "NASK : ADD syntax error" << std::endl;
			 return 17;
		    }

		    break;
	       }
	  }
	  return 0;
     }

     // 簡単なCMP命令の実装
     int Instructions::process_token_CMP(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
		    break;
	       } else {

		    if (is_register(token_table, token) &&
			tokenizer.LookAhead(1).Is(",")  &&
			is_register(token_table, tokenizer.LookAhead(2))) {
			 // MOV Reg,Reg
		    } else if (token.Is("[") &&
			       is_register(token_table, tokenizer.LookAhead(1)) &&
			       tokenizer.LookAhead(2).Is("]") &&
			       tokenizer.LookAhead(3).Is(",") &&
			       is_register(token_table, tokenizer.LookAhead(4))) {
			 // MOV Mem,Reg
		    } else if (is_register(token_table, token) &&
			       tokenizer.LookAhead(1).Is(",")  &&
			       tokenizer.LookAhead(2).Is("[")  &&
			       is_register(token_table, tokenizer.LookAhead(3)) &&
			       tokenizer.LookAhead(4).Is("]")) {
			 // MOV Reg,Mem
		    } else if (is_register(token_table, token) &&
			       tokenizer.LookAhead(1).Is(",")  &&
			       is_legitimate_numeric(tokenizer.LookAhead(2).AsString())) {
			 // MOV Acc,Imm
			 // MOV Reg,Imm8
			 // MOV Reg,Imm
			 TParaToken dst_token = token;
			 TParaToken src_token = tokenizer.LookAhead(2);
			 const std::string dst_reg  = dst_token.AsString();
			 const std::string src_imm  = src_token.AsString();

			 // 次へ
			 tokenizer.Next();
			 tokenizer.Next();
			 std::cout << dst_reg << " == " << src_imm << std::endl;
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

			 std::smatch match;
			 if (regex_match(dst_reg, match, ModRM::rm000)) {
			      // AL|AX|EAX なので "MOV Acc,Imm" に決定
			      // MOV Acc,Imm
			      // 0011110 + w
			      const std::bitset<8> bs_dst("0011110" + std::get<1>(tp_dst));

			      // debug logs
			      std::cout << "NIM(B): ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(bs_dst.to_ulong());
			      std::cout << ", ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(src_token.AsLong()) << std::endl;

			      binout_container.push_back(bs_dst.to_ulong());
			      binout_container.push_back(src_token.AsLong());
			      break;

			 } else {
			      // MOV Reg,Imm8
			      // MOV Reg,Imm

			 }
			 break;

		    } else if (token.Is("[") &&
			       is_register(token_table, tokenizer.LookAhead(1)) &&
			       tokenizer.LookAhead(2).Is("]") &&
			       tokenizer.LookAhead(3).Is(",") &&
			       is_legitimate_numeric(tokenizer.LookAhead(4).AsString())) {
			 // MOV Mem,Imm8
			 // MOV Mem,Imm
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
	       } else {
		    // DWを解釈, 0x00の際でもWORDで格納
		    set_word_into_binout(token.AsLong(), binout_container, false);
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
			 std::cerr << "NASK : DD not quoted correctly' " << std::endl;
			 return 17;
		    }
	       } else {
		    // DWを解釈
		    set_dword_into_binout(token.AsLong(), binout_container);
	       }
	  }
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
			 for (uint32_t l = binout_container.size(); l < token.AsLong(); l++) {
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
}
