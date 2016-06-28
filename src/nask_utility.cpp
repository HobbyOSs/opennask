#include "nask_utility.hpp"

namespace nask_utility {
     namespace ModRM {
	  const std::string get_MMMSSS_from_reg(const std::string& reg) {
	       if (REGISTERS_MMM_MAP.count(reg) != 0) {
		    return REGISTERS_MMM_MAP.at(reg);
	       } else if (SEGMENT_REGISTERS_SSS_MAP.count(reg) != 0) {
		    return SEGMENT_REGISTERS_SSS_MAP.at(reg);
	       } else {
		    return "000";
	       }
	  }

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

	       return "";
	  };

	  uint8_t generate_modrm(enum mods m, const std::string& dst_reg, const std::string& src_reg) {
	       std::string modrm = ModRM::MOD_TO_STR.at(m);
	       modrm += SEGMENT_REGISTERS_SSS_MAP.at(dst_reg);
	       modrm += get_rm_from_reg(src_reg);
	       std::bitset<8> bs(modrm);
	       return bs.to_ulong();
	  };

	  bool is_accumulator(const std::string& reg) {
	       std::smatch match;
	       return regex_match(reg, match, rm000);
	  }
     };

     bool is_legitimate_numeric(const std::string& s) {
	  return is_hex_notation(s) || is_integer(s);
     }

     bool is_between_bytesize(const long l) {
	  return (l <= std::numeric_limits<int8_t>::max() &&
		  l >= std::numeric_limits<int8_t>::min());
     }

     bool is_integer(const std::string& s) {
	  if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
	  char * p ;
	  strtol(s.c_str(), &p, 10) ;
	  return (*p == 0) ;
     }

     bool is_hex_notation(const std::string& s) {
	  return s.compare(0, 2, "0x") == 0
	       && s.size() > 2
	       && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
     }

     size_t get_imm_size(const std::string& hex_string) {
	  if (is_hex_notation(hex_string)) {
	       const size_t s = hex_string.substr(2).size() / 2;
	       std::cout << "check imm size: "
			 << hex_string
			 << " => "
			 << s
			 << std::endl;
	       return s;
	  } else {
	       return 0;
	  }
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

     bool is_common_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  // レジスタ一覧から検索してあれば true
	  auto it = std::find_if(std::begin(REGISTERS), std::end(REGISTERS),
				 [&](const std::string& s)
				 { return token.AsString().find(s) != std::string::npos; });

	  return it != std::end(REGISTERS);
     }

     bool is_segment_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  // レジスタ一覧から検索してあれば true
	  auto it = std::find_if(std::begin(SEGMENT_REGISTERS), std::end(SEGMENT_REGISTERS),
				 [&](const std::string& s)
				 { return token.AsString().find(s) != std::string::npos; });

	  return it != std::end(SEGMENT_REGISTERS);
     }

     bool is_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  return is_common_register(token_table, token) || is_segment_register(token_table, token);
     }

     bool is_datatype(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  // データ型一覧から検索してあれば true
	  auto it = std::find_if(std::begin(DATA_TYPES), std::end(DATA_TYPES),
				 [&](const std::string& s)
				 { return token.AsString().find(s) != std::string::npos; });

	  return it != std::end(DATA_TYPES);
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

     // label: (label_dstと呼ぶ)
     // 1) label_dstの位置を記録する → label_dst_stack
     void Instructions::store_label_dst(std::string label_dst, VECTOR_BINOUT& binout_container) {

	  std::cout << "stored label: "
		    << label_dst
		    << " bin["
		    << std::to_string(binout_container.size())
		    << "]"
		    << std::endl;

     	  LABEL_DST_ELEMENT elem;
	  elem.label = label_dst;
	  elem.src_index = binout_container.size();
	  label_dst_stack.push_back(elem);
     }

     // label: (label_dstと呼ぶ)
     // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了
     void Instructions::update_label_dst_offset(std::string label_dst, VECTOR_BINOUT& binout_container) {
	  auto it = std::find_if(std::begin(label_src_stack), std::end(label_src_stack),
				 [&](const LABEL_SRC_ELEMENT& elem)
				 { return elem.label.find(label_dst) != std::string::npos; });

      	  if (it != std::end(label_src_stack)) {
	       LABEL_SRC_ELEMENT elem(*it);
	       elem.dst_index = binout_container.size();

	       std::cout.setf(std::ios::hex, std::ios::basefield);
	       if (elem.abs) {
		    std::cout << "update_label_dst_offset bin["
			      << std::to_string(elem.rel_index - 1)
			      << "] = "
			      << elem.dst_index << std::endl;

		    std::cout << "update_label_dst_offset bin["
			      << std::to_string(elem.rel_index)
			      << "] = "
			      << 0x7c << std::endl;

		    binout_container[elem.rel_index - 1] = elem.dst_index;
		    binout_container[elem.rel_index] = 0x7c;
	       } else {
		    std::cout << "update_label_dst_offset bin["
			      << std::to_string(elem.rel_index)
			      << "] = "
			      << elem.rel_offset() << std::endl;
		    binout_container[elem.rel_index] = elem.rel_offset();
	       }
	  }
     }

     // OPECODE label (label_srcと呼ぶ)
     // 1) 同名のlabel_dstが保存されていれば、オフセット値を計算して終了
     //    処理対象があれば true, 処理対象がなければ false
     bool Instructions::update_label_src_offset(std::string label_src,
						VECTOR_BINOUT& binout_container,
						uint8_t nim) {
	  auto it = std::find_if(std::begin(label_dst_stack), std::end(label_dst_stack),
				 [&](const LABEL_DST_ELEMENT& elem)
				 { return elem.label.find(label_src) != std::string::npos; });

      	  if (it != std::end(label_dst_stack)) {
	       LABEL_DST_ELEMENT elem(*it);
	       elem.dst_index = binout_container.size();
	       elem.rel_index = binout_container.size() + 1;
	       std::cout.setf(std::ios::hex, std::ios::basefield);
	       std::cout << "update_label_src_offset bin["
			 << std::to_string(elem.rel_index)
			 << "] = "
			 << elem.rel_offset() << std::endl;

	       binout_container.push_back(nim);
	       binout_container.push_back(elem.rel_offset());
	       return true;
	  }

	  return false;
     }

     std::string Instructions::get_equ_label_or_asis(std::string key) {
	  if( this->equ_map.find(key) != this->equ_map.end() ) {
	       std::cout << "label: "
			 << key
			 << " replaced "
			 << equ_map[key]
			 << std::endl;
	       return this->equ_map[key];
	  } else {
	       return key;
	  }
     }

     bool Instructions::dst_is_stored(std::string label_src, VECTOR_BINOUT& binout_container) {
	  auto it = std::find_if(std::begin(label_dst_stack), std::end(label_dst_stack),
				 [&](const LABEL_DST_ELEMENT& elem)
				 { return elem.label.find(label_src) != std::string::npos; });
	  return it != std::end(label_dst_stack);
     }

     // OPECODE label (label_srcと呼ぶ)
     // 2) 同名のlabel_dstが保存されていなければ、label_srcの位置を保存する → label_src_stack
     void Instructions::store_label_src(std::string label_src, VECTOR_BINOUT& binout_container, bool abs) {
     	  LABEL_SRC_ELEMENT elem;
	  elem.abs   = abs;
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
     //
     void set_word_into_binout(const uint16_t& word,
			       VECTOR_BINOUT& binout_container,
			       bool zero_as_byte) {

	  if (word == 0x0000 && zero_as_byte) {
	       // push_back only 1byte
	       binout_container.push_back(0x00);

	       std::cout << "(B): ";
	       std::cout << std::showbase << std::hex
			 << static_cast<int>(0x00)
			 << std::endl;
	  } else {
	       // http://stackoverflow.com/a/1289360/2565527
	       const uint8_t first_byte  = (word >> 8) & 0xff;
	       const uint8_t second_byte = word & 0xff;
	       // push_back as little_endian
	       binout_container.push_back(second_byte);
	       binout_container.push_back(first_byte);

	       std::cout << "(W): ";
	       std::cout << std::showbase << std::hex
			 << static_cast<int>(second_byte);
	       std::cout << ", ";
	       std::cout << std::showbase << std::hex
			 << static_cast<int>(first_byte)
			 << std::endl;

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
     LABEL_DST_STACK Instructions::label_dst_stack;
     LABEL_SRC_STACK Instructions::label_src_stack;
     std::map<std::string, std::string> Instructions::equ_map;
     std::string Instructions::data_type;
     uint32_t Instructions::dollar_position = 0;
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
		    std::cout << "declared datatype: " << token.AsString() << std::endl;
		    data_type = token.AsString();
		    continue;
	       } else if (is_segment_register(token_table, token)) {
		    //
		    // 8E /r | MOV Sreg,r/m16** | Move r/m16 to segment register
		    //
		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(2);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_reg  = src_token.AsString();

		    // MOV Sreg, register の時
		    if (tokenizer.LookAhead(1).Is(",") &&
			!tokenizer.LookAhead(2).IsEmpty() &&
			is_common_register(token_table, src_token)) {
			 // コンマを飛ばして次へ
			 token = tokenizer.Next();
			 token = tokenizer.Next();
			 std::cout << " <= " << token.AsString();

			 const uint8_t modrm = ModRM::generate_modrm(ModRM::REG, dst_reg, src_reg);
			 binout_container.push_back(0x8e);
			 binout_container.push_back(modrm);
			 // これで終了のはず
			 std::cout << " : NIM: ";
			 std::cout << std::showbase
				   << std::hex
				   << 0x8e
				   << ", ";
			 std::cout << std::showbase
				   << std::hex
				   << static_cast<int>(modrm)
				   << std::endl;
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

		    std::cout << dst_mem << "(" << dst_addr_imm << ")" << " <= " << src_imm << std::endl;

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

		    std::cout << "NIM(W): "
			      << std::showbase << std::hex
			      << static_cast<int>(bs_src.to_ulong())
			      << ", "
			      << static_cast<int>(bs_dst.to_ulong())
			      << ", "
			      << static_cast<int>(dst_addr_imm)
			      << ", "
			      << static_cast<int>(src_token.AsLong()) << std::endl;

		    // コンマを飛ばして次へ
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    // これで終了のはず
		    break;

	       } else if (token.Is("[") && tokenizer.LookAhead(2).Is("]") &&
			  tokenizer.LookAhead(3).Is(",") &&
		          is_common_register(token_table, tokenizer.LookAhead(4))) {
		    //
		    // MOV Mem     , Reg  | 1000100w oorrrmmm
		    // MOV moffs8* , AL   | 0xA2
		    // MOV moffs16*, AX   | 0xA3
		    // MOV moffs32*, EAX  | 0xA3
		    //
		    // http://x86.renejeschke.de/html/file_module_x86_id_176.html
		    // --------------------------------
		    TParaToken dst_token = tokenizer.LookAhead(1);
		    TParaToken src_token = tokenizer.LookAhead(4);
		    const std::string dst_mem  = "[" + get_equ_label_or_asis(dst_token.AsString()) + "]";
		    const std::string src_reg  = src_token.AsString();

		    std::cout << dst_mem << " <= " << src_reg << std::endl;

		    if (ModRM::is_accumulator(src_reg)) {
			 std::cout << "MOV moffs* , AL or AX or EAX" << std::endl;
			 const uint8_t bs_src = (src_reg == "AL") ? 0xa2 : 0xa3;
			 binout_container.push_back(bs_src);
		    } else {
			 std::cout << "MOV Mem,Reg" << std::endl;
			 // Mem, Regの場合 => 1000100w oo rrr mmm
			 // 1000100 + w
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(src_reg);
			 const std::bitset<8> bs_src("1000100" + std::get<1>(tp_dst));
			 binout_container.push_back(bs_src.to_ulong());
			 // oo + rrr + mmm
			 // 00 + rrr + 110
			 // この場合 mod=00, r/m=101で確定となる
			 // r/m=101...[disp16], [disp32]
			 const std::bitset<8> bs_dst("00" + std::get<0>(tp_dst) + "110");
			 binout_container.push_back(bs_dst.to_ulong());
		    }

		    // 転送先は常にWORD
		    const std::string dst_addr = get_equ_label_or_asis(dst_token.AsString());
		    const uint16_t dst_addr_imm = std::stol(dst_addr, nullptr, 16);
		    set_word_into_binout(dst_addr_imm, binout_container, false);

		    // コンマを飛ばして次へ
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    token = tokenizer.Next();
		    // これで終了のはず
		    break;

	       } else if (is_common_register(token_table, token) &&
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
			  tokenizer.LookAhead(1).Is(",")  &&
			  is_segment_register(token_table, tokenizer.LookAhead(2))) {

		    // MOV Reg16,Seg
		    // 10001100 oosssmmm
		    //--------------------------------------------------------
		    // 0x8C /r	MOV r/m16, Sreg	        セグメントレジスタをr/m16に転送します
		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(2);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_reg  = src_token.AsString();

		    std::cout << dst_reg << " <= " << src_reg << std::endl;

		    // Reg16, Sregの場合 => oosssmmm
		    const std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
		    const std::string tp_src = ModRM::get_MMMSSS_from_reg(src_reg);

		    // 10001100
		    const std::bitset<8> bs_dst("10001100");
		    // oo + sss + mmm
		    const std::bitset<8> bs_src("11" + std::get<0>(tp_dst) + tp_src);
		    binout_container.push_back(bs_dst.to_ulong());
		    binout_container.push_back(bs_src.to_ulong());

		    std::cout << "NIM(W): ";
		    std::cout << std::showbase << std::hex
			      << static_cast<int>(bs_dst.to_ulong());
		    std::cout << ", ";
		    std::cout << std::showbase << std::hex
			      << static_cast<int>(bs_src.to_ulong()) << std::endl;

		    // これで終了のはず
		    break;

	       } else if (is_common_register(token_table, token) &&
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

			 update_label_src_offset(token.AsString(), binout_container, 0x00);
			 store_label_src(token.AsString(), binout_container, true);

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
			 std::cout << "label stored: " << store_label << std::endl;
			 std::cout << "0x73, 0x00" << std::endl;

			 if (dst_is_stored(store_label, binout_container)) {
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
			 std::cout << "label stored: " << store_label << std::endl;
			 std::cout << "0x76, 0x00" << std::endl;

			 if (dst_is_stored(store_label, binout_container)) {
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
			 std::cout << "label stored: " << store_label << std::endl;
			 std::cout << "0x72, 0x00" << std::endl;

			 if (dst_is_stored(store_label, binout_container)) {
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
			 std::cout << "label stored: " << store_label << std::endl;
			 std::cout << "0x72, 0x00" << std::endl;

			 if (dst_is_stored(store_label, binout_container)) {
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
			 std::cout << "label stored: " << store_label << std::endl;
			 std::cout << "0x74, 0x00" << std::endl;

			 if (dst_is_stored(store_label, binout_container)) {
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
	       } else if (is_hex_notation(token.AsString())) {
		    // ラベルではなく即値でジャンプ先を指定された場合
		    // JMP rel8  | 0xEB cb
		    // JMP rel16 | 0xE9 cw
		    // JMP rel32 | 0xE9 cd

		    // offset = dst - src - current_pos
		    if (is_between_bytesize(token.AsLong()) && imm8 == get_imm_size(token.AsString())) {
			 // 0xEB
			 const long jmp_offset = (token.AsLong() - dollar_position - binout_container.size()) - 2;
			 std::cout << "JMP: " << token.AsString() << std::endl;
			 std::cout << "0xeb with Byte "
				   << std::showbase << std::hex
				   << jmp_offset << std::endl;
			 binout_container.push_back(0xeb);
			 binout_container.push_back(jmp_offset);
		    } else {
			 // 0xE9
			 std::cout << "JMP: " << token.AsString() << std::endl;
			 const long jmp_offset = (token.AsLong() - dollar_position - binout_container.size()) - 3;
			 if (get_imm_size(token.AsString()) == imm16) {
			      std::cout << "0xe9 with Word "
					<< std::showbase << std::hex
					<< jmp_offset << std::endl;
			      binout_container.push_back(0xe9);
			      set_word_into_binout(jmp_offset, binout_container);
			 } else if (get_imm_size(token.AsString()) == imm32) {
			      std::cout << "0xe9 with Dword "
					<< std::showbase << std::hex
					<< jmp_offset <<std::endl;
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
		    std::cout << "label stored: " << store_label << std::endl;
		    std::cout << "0xeb, 0x00" << std::endl;

		    if (dst_is_stored(store_label, binout_container)) {
			 update_label_src_offset(store_label, binout_container, 0xeb);
		    } else {
			 store_label_src(store_label, binout_container);
			 binout_container.push_back(0xeb);
			 binout_container.push_back(0x00);
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
			 std::cout << "label stored: " << store_label << std::endl;
			 std::cout << "0x73, 0x00" << std::endl;

			 if (dst_is_stored(store_label, binout_container)) {
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
			 std::cout << dst_reg << " <= " << src_imm << std::endl;
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
			 // Imm8 or Imm16
			 const size_t imm_size = get_imm_size(src_token.AsString());
			 std::cout << "imm size: " << imm_size << std::endl;

			 if (ModRM::is_accumulator(dst_reg)) {
			      // ADD Acc,Imm
			      // NIM: 0000010w
			      const std::bitset<8> bs_dst("0000010" + std::get<1>(tp_dst));
			      // debug logs
			      std::cout << "NIM(W): ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(bs_dst.to_ulong());
			      std::cout << ", ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(src_token.AsLong()) << std::endl;

			      binout_container.push_back(bs_dst.to_ulong());

			      if (imm_size == imm8) {
				   std::cout << "imm8 ! => " << src_token.AsLong() << std::endl;
				   binout_container.push_back(src_token.AsLong());
			      } else {
				   std::cout << "imm16 ! => " << src_token.AsLong() << std::endl;
				   set_word_into_binout(src_token.AsLong(), binout_container, false);
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
				   std::cout << "r/m8: " << dst_reg << std::endl;
				   //const std::bitset<8> bs_dst1("1000001" + std::get<1>(tp_dst));
				   const std::bitset<8> bs_dst2("11000" + std::get<0>(tp_dst));

				   // debug logs
				   std::cout << "NIM(W): ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(0x80);
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(bs_dst2.to_ulong());
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(src_token.AsLong()) << std::endl;

				   binout_container.push_back(0x80);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_token.AsLong());

			      } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
				   const uint8_t op = is_between_bytesize(src_token.AsLong()) ? 0x83 : 0x81;
				   // ADD Reg16, Imm
				   // -------------
				   // 0x83 /0 ib  ADD r/m16, imm8
				   // 0x81 /0 iw  ADD r/m16, imm16
				   std::cout << "r/m16: " << dst_reg << std::endl;
				   //const std::bitset<8> bs_dst1("1000000" + std::get<1>(tp_dst));
				   const std::bitset<8> bs_dst2("11000" + std::get<0>(tp_dst));

				   // debug logs
				   std::cout << "NIM(W): ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(op);
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(bs_dst2.to_ulong());
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(src_token.AsLong()) << std::endl;

				   binout_container.push_back(op);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_token.AsLong());

			      } else {
				   // ADD Reg32, Imm
				   // -------------
				   // 0x83 /0 ib  ADD r/m32, imm8
				   // 0x81 /0 id  ADD r/m32, imm32

			      }

			      // ADD Reg,Imm8 に決定 int8_t 内に収まっているので
			      // NIM: 1000001w oo000mmm
			      // 1000001 + w
			      // const std::bitset<8> bs_dst("1000001" + std::get<1>(tp_dst));
			      // oo + 000 + mmm
			      // FIXME: mmmではなくrrrかもしれない
			      // const std::bitset<8> bs_src("11000" + std::get<0>(tp_dst));

			      // debug logs
			      //std::cout << "NIM(W): ";
			      //std::cout << std::showbase << std::hex
			      // 		<< static_cast<int>(bs_dst.to_ulong());
			      //std::cout << ", ";
			      //std::cout << std::showbase << std::hex
			      // 		<< static_cast<int>(bs_src.to_ulong());
			      //std::cout << ", ";
			      //std::cout << std::showbase << std::hex
			      // 		<< static_cast<int>(src_token.AsLong()) << std::endl;
			      //
			      //binout_container.push_back(bs_dst.to_ulong());
			      //binout_container.push_back(bs_src.to_ulong());
			      //binout_container.push_back(src_token.AsLong());
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
			 std::cerr << "NASK : ADD syntax error" << std::endl;
			 return 17;
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
			       is_common_register(token_table, tokenizer.LookAhead(3)) &&
			       tokenizer.LookAhead(4).Is("]")) {
			 // CMP Reg,Mem
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
			 std::cout << dst_reg << " == " << src_imm << std::endl;
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

			 std::smatch match;
			 if (regex_match(dst_reg, match, ModRM::rm000)) {
			      // AL|AX|EAX なので "CMP Acc,Imm" に決定
			      // CMP Acc,Imm
			      // 0011110 + w
			      const std::bitset<8> bs_dst("0011110" + std::get<1>(tp_dst));

			      // debug logs
			      std::cout << "NIM(B): ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(bs_dst.to_ulong());
			      std::cout << ", ";
			      std::cout << std::showbase << std::hex
					<< static_cast<int>(src_imm) << std::endl;

			      binout_container.push_back(bs_dst.to_ulong());
			      binout_container.push_back(src_imm);

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
				   std::cout << "r/m8: " << dst_reg << std::endl;
				   //const std::bitset<8> bs_dst1("1000001" + std::get<1>(tp_dst));
				   const std::bitset<8> bs_dst2("11111" + std::get<0>(tp_dst));

				   // debug logs
				   std::cout << "NIM(W): ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(0x80);
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(bs_dst2.to_ulong());
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(src_imm) << std::endl;

				   binout_container.push_back(0x80);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_imm);

			      } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
				   const uint8_t op = is_between_bytesize(src_imm) ? 0x83 : 0x81;
				   // CMP Reg16, Imm
				   // ------------
				   // 0x83 /7 ib | CMP r/m16, imm8
				   // 0x81 /7 iw | CMP r/m16, imm16
				   std::cout << "r/m16: " << dst_reg << std::endl;
				   //const std::bitset<8> bs_dst1("1000000" + std::get<1>(tp_dst));
				   const std::bitset<8> bs_dst2("11111" + std::get<0>(tp_dst));

				   // debug logs
				   std::cout << "NIM(W): ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(op);
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(bs_dst2.to_ulong());
				   std::cout << ", ";
				   std::cout << std::showbase << std::hex
					     << static_cast<int>(src_imm) << std::endl;

				   binout_container.push_back(op);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_imm);
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
			      std::cout << key
					<< " is keeped as "
					<< val
					<< " because of EQU"
					<< std::endl;
			      this->equ_map[key] = val;
			      return 0;
			 }
		    } else {
			 std::cerr << "NASK : EQU syntax is not correct" << std::endl;
			 return 17;
		    }
		    std::cout << "!!!" << token.AsString() << "!!!" << std::endl;
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
			 std::cerr << "NASK : DD not quoted correctly" << std::endl;
			 return 17;
		    }
	       } else {
		    // DWを解釈
		    set_dword_into_binout(token.AsLong(), binout_container);
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
		    std::cout << token.AsString() << std::endl;

		    if (is_legitimate_numeric(token.AsString())) {
			 dollar_position = token.AsLong();
			 std::cout.setf(std::ios::hex, std::ios::basefield);
			 std::cout << "ORG = " << dollar_position << std::endl;
			 break;
		    } else {
			 std::cerr << "NASK : ORG specified incorrect value" << std::endl;
			 return 17;
		    }
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
}
