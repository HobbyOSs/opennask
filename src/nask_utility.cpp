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

     bool is_comment_line(TParaCxxTokenTable& token_table, TParaToken& token) {
	  return token_table.IsCommentLimiter(token.AsString());
     }

     bool is_line_terminated(TParaCxxTokenTable& token_table, TParaToken& token) {
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

     bool is_common_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  return is_registers_with_args(token, REGISTERS);
     }

     bool is_common_register(TParaCxxTokenTable& token_table, const std::string token) {
	  return is_registers_with_args(token, REGISTERS);
     }

     bool is_segment_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  return is_registers_with_args(token, SEGMENT_REGISTERS);
     }

     bool is_control_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  return is_registers_with_args(token, CONTROL_REGISTERS);
     }

     bool is_register(TParaCxxTokenTable& token_table, const TParaToken& token) {
	  const bool r = is_common_register(token_table, token) || is_segment_register(token_table, token);
	  log()->info("{} is_register? -> {}", token.AsString(), r);
	  return r;
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

     // Instructionクラスの定数を初期化
     Instructions::Instructions() {
	  // デフォルトのトークンテーブル
	  TParaCxxTokenTable token_table;
	  token_table.AddCommentLimiter(";", "\n");
	  token_table.AddCommentLimiter("#", "\n");
	  this->token_table = token_table;
	  this->support_cpus = meta::flip_map(SUPPORT_CPUS);

	  // 基本的なオペレーター登録
	  for (std::string op : PRE_PROCESS_OPERATORS) {
	       this->token_table.AddOperator(op);
	  }
     }

     void Instructions::set_nimonic_with_register(const std::string& reg,
						  NIMONIC_INFO* nim_info,
						  TParaTokenizer& tokenizer) {

	  if (reg == "AL" || reg == "BL" || reg == "CL" || reg == "DL") {
	       // prefix = "B0+rb" (AL:+0, CL:+1, DL:+2, BL:+3)
	       log()->info("reg == AL|BL|CL|DL");
	       nim_info->prefix = get_plus_register_code((uint8_t) 0xb0, reg.at(0));
	       nim_info->reg = reg;
	       nim_info->imm = imm8;
	  } else if (reg == "EAX" || reg == "EBX" || reg == "ECX" || reg == "EDX") {
	       // prefix = "B8+rd" (EAX:+0, EBX:+1, ECX:+2, EDX:+3)
	       log()->info("reg == EAX|EBX|ECX|EDX");
	       nim_info->prefix = get_plus_register_code((uint8_t) 0xb8, reg.at(1));
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
			 log()->info("register is for imm8: {}, opecode 0x{:02x}", reg, modrm);
			 nim_info->prefix = modrm;
			 nim_info->imm = imm8;
		    } else if (regex_match(reg, match, ModRM::regImm16)) {
			 const uint8_t modrm = ModRM::get_opecode_from_reg(0xb8, reg);
			 log()->info("register is for imm16: {}, opecode 0x{:02x}", reg, modrm);
			 nim_info->prefix = modrm;
			 nim_info->imm = imm16;
		    } else {
			 const uint8_t modrm = ModRM::get_opecode_from_reg(0xb8, reg);
			 log()->info("register is for imm32: {}, opecode 0x{:02x}", reg, modrm);
			 nim_info->prefix = modrm;
			 nim_info->imm = imm32;
		    }
	       } else {
		    log()->info("MOV this is label: {}", src_token.AsString());
		    nim_info->reg = reg;
		    nim_info->imm = offs; // ターゲットはoffset
	       }
	  }
	  return;
     }

     // label: (label_dstと呼ぶ)
     // 1) label_dstの位置を記録する → label_dst_stack
     void Instructions::store_label_dst(std::string label_dst, VECTOR_BINOUT& binout_container) {
	  log()->info("stored label: {} bin[{}]", label_dst, binout_container.size());
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
	       //log()->info("update target: {}, stored info {}", label_dst, elem.label);
	       if (elem.label != label_dst)
		    continue;

	       // 処理の開始
	       elem.dst_index = binout_container.size();
	       if (elem.abs) {
		    // ORGからの絶対値でオフセットを設定する
		    switch (elem.offset_size) {
		    case imm32:
			 log()->info("imm32: update_label_dst_offset bin[{}, {}, {}, {}] = 0x{:02x}",
				     elem.rel_index,
				     elem.rel_index + 1,
				     elem.rel_index + 2,
				     elem.rel_index + 3,
				     dollar_position + elem.dst_index);

			 set_dword_into_binout(dollar_position + elem.dst_index,
					       binout_container, false, elem.rel_index);
			 break;

		    case imm16:
			 log()->info("imm16: update_label_dst_offset bin[{}, {}] = 0x{:02x}",
				     elem.rel_index,
				     elem.rel_index + 1,
				     dollar_position + elem.dst_index);

			 set_word_into_binout(dollar_position + elem.dst_index,
					      binout_container, false, elem.rel_index);
			 break;

		    case imm8:
		    default:
			 log()->info("imm8: update_label_dst_offset bin[{}] = {}, bin[{}] = {}",
				     elem.rel_index - 1, elem.dst_index, elem.rel_index, 0x7c);
			 binout_container[elem.rel_index - 1] = elem.dst_index;
			 binout_container[elem.rel_index] = 0x7c;
			 break;
		    }
	       } else {
		    // ラベルからの相対値でオフセットを設定する
		    switch (elem.offset_size) {
		    case imm16:
			 log()->info("imm16: update_label_dst_offset bin[{}, {}] = 0x{:02x}",
				     std::to_string(elem.rel_index),
				     std::to_string(elem.rel_index + 1),
				     elem.rel_offset() - 1);

			 set_word_into_binout(elem.rel_offset() - 1,
					      binout_container,
					      false,
					      elem.rel_index);
			 break;

		    case imm32:
			 log()->info("imm32: update_label_dst_offset bin[{}, {}, {}, {}] = 0x{:02x}",
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
			 log()->info("imm8: update_label_dst_offset bin[{}] = 0x{:02x}",
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
	       log()->info("matched label: {} with {}", elem.label, label_src);
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
	       log()->info("update_label_src_offset bin[{}] = {}",
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
	       log()->info("label: {} replaced as {}", key, equ_map[key]);
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
	       log()->info("(B): 0x00");
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
	       log()->info("(W): 0x{:02x}, 0x{:02x}", second_byte, first_byte);
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
	       uint8_t bytes[4];

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
	       log()->info("(DW): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}",
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
		   log()->info("(BIN): 0x{:02x} ({})", x, strm.str());
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
     TParaCxxTokenTable Instructions::token_table;
     LABEL_DST_STACK Instructions::label_dst_stack;
     LABEL_SRC_STACK Instructions::label_src_stack;
     std::map<std::string, std::string> Instructions::equ_map;
     std::vector<std::string> Instructions::symbol_list;
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
		    log()->info("declared datatype: {}", token.AsString());
		    this->data_type = token.AsString();
		    continue;
	       } else if (is_control_register(token_table, token)) {
		    // 0x0F 0x22 /r
		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(2);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_reg  = src_token.AsString();
		    log()->info("Reg {} <= {}", dst_reg, src_reg);

		    // CRn,Reg32
		    // 00001111 00100000 11sssrrr
		    // 0x0F	0x22
		    if (tokenizer.LookAhead(1).Is(",") &&
			!tokenizer.LookAhead(2).IsEmpty() &&
			is_control_register(token_table, dst_token)) {
			 // コンマを飛ばして次へ
			 token = tokenizer.Next();
			 token = tokenizer.Next();
			 log()->info("{} <= {}", dst_reg, src_reg);

			 std::tuple<std::string, std::string> tp = ModRM::REGISTERS_RRR_MAP.at(src_reg);
			 const std::bitset<8> nim("11000" + std::get<0>(tp));
			 binout_container.push_back(0x0f);
			 binout_container.push_back(0x22);
			 binout_container.push_back(nim.to_ulong());
			 // これで終了のはず
			 log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
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
		    log()->info("Sreg {} <= {}", dst_reg, src_reg);

		    // MOV Sreg, register の時
		    if (tokenizer.LookAhead(1).Is(",") && is_common_register(token_table, src_token)) {
			 // コンマを飛ばして次へ
			 token = tokenizer.Next();
			 token = tokenizer.Next();
			 const uint8_t modrm = ModRM::generate_modrm(0x8e, ModRM::REG, src_reg, dst_reg);
			 binout_container.push_back(0x8e);
			 binout_container.push_back(modrm);
			 // これで終了のはず
			 log()->info("NIM: 0x{:02x}, 0x{:02x}", 0x8e, modrm);
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

		    log()->info("{}({}) <= {}", dst_mem, dst_addr_imm, src_imm);

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

		    log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}",
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

		    log()->info("{} <= {}", dst_mem, src_reg);

		    if (src_reg == "AL" || src_reg == "AX") {
			 log()->info("MOV moffs* , AL or AX");
			 const uint8_t bs_src = (src_reg == "AL") ? 0xa2 : 0xa3;
			 binout_container.push_back(bs_src);
			 const uint16_t dst_addr_imm = std::stol(dst_addr, nullptr, 16);
			 set_word_into_binout(dst_addr_imm, binout_container, false);
		    } else {
			 log()->info("MOV Mem,Reg");
			 // Mem, Regの場合 => 1000100w oo rrr mmm
			 // 1000100 + w
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(src_reg);
			 const std::bitset<8> bs_src("1000100" + std::get<1>(tp_dst));

			 // FIXME: ここの部分、ModR/Mの原則にしたがってなさそう
			 const uint8_t modrm = ModRM::generate_modrm(bs_src.to_ulong(), ModRM::REG_REG, dst_mem, src_reg);
			 std::smatch match;
			 log()->info("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

			 if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
			      log()->info("32bit reg using & 16bit-mode: Override prefix: 0x67");
			      binout_container.push_back(0x67);
			 }
			 if (regex_match(dst_mem, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
			      log()->info("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
			      binout_container.push_back(0x66);
			 } else if (regex_match(dst_mem, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
			      log()->info("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
			      binout_container.push_back(0x66);
			 } else {
			      log()->info("Register-size prefix is absent");
			 }

			 // Opcode && MOD-REG-R/M
			 binout_container.push_back(bs_src.to_ulong());
			 binout_container.push_back(modrm);
			 log()->info("NIM(W): 0x{:02x}, 0x{:02x}", bs_src.to_ulong(), modrm);

			 if (!is_common_register(token_table, dst_addr)) {
			      const std::string dst_addr = get_equ_label_or_asis(dst_token.AsString());
			      const uint16_t dst_addr_imm = std::stol(dst_addr, nullptr, 16);
			      set_word_into_binout(dst_addr_imm, binout_container, false);
			 }
		    }

		    // これで終了のはず
		    break;

	       } else if (token.Is("[") && tokenizer.LookAhead(2).Is("]") &&
			  tokenizer.LookAhead(3).Is(",") &&
			  is_common_register(token_table, tokenizer.LookAhead(4))) {
		    // MOV r/m8, r8	  | 0x88 /r
		    // MOV r/m16, r16	  | 0x89 /r
		    // MOV r/m32, r32	  | 0x89 /r
		    TParaToken dst_token = tokenizer.LookAhead(1);
		    TParaToken src_token = tokenizer.LookAhead(4);

		    const std::string dst_reg  = get_equ_label_or_asis(dst_token.AsString());
		    const std::string dst_mem  = "[" + get_equ_label_or_asis(dst_token.AsString()) + "]";
		    const std::string src_reg  = src_token.AsString();

		    log()->info("{} <= {}", dst_mem, src_reg);

		    std::smatch match;
		    log()->info("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

		    if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
			 log()->info("32bit reg using & 16bit-mode: Override prefix: 0x67");
			 binout_container.push_back(0x67);
		    }
		    if (regex_match(dst_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
			 log()->info("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
			 binout_container.push_back(0x66);
		    } else if (regex_match(dst_reg, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
			 log()->info("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
			 binout_container.push_back(0x66);
		    } else {
			 log()->info("Register-size prefix is absent");
		    }

		    if (regex_match(src_reg, match, ModRM::regImm08)) {
			 // MOV r/m8, r8       | 0x88 /r
			 const uint8_t modrm = ModRM::generate_modrm(0x88, ModRM::REG_REG, dst_mem, src_reg);
			 log()->info("NIM(B): 0x88, 0x{:02x}, {}", modrm, tokenizer.LookAhead(2).AsString());
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
			 const uint8_t modrm = ModRM::generate_modrm(0x89, ModRM::REG_REG, dst_mem, src_reg);
			 log()->info("NIM(B): 0x89, 0x{:02x}", modrm, tokenizer.LookAhead(2).AsString());
			 binout_container.push_back(0x89);
			 binout_container.push_back(modrm);
		    }

		    break;

	       } else if (is_common_register(token_table, token) &&
			  tokenizer.LookAhead(1).Is(",")  &&
			  // MOV XX, [YY]
			  (tokenizer.LookAhead(2).Is("[") && tokenizer.LookAhead(4).Is("]")) ||
			  // MOV XX, [YY+00]
			  (tokenizer.LookAhead(2).Is("[") && tokenizer.LookAhead(6).Is("]")) ||
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

		    bool exists_disp = tokenizer.LookAhead(6).Is("]") ? true : false;
		    log()->info("exists disp ? => {}", exists_disp);

		    bool equ_specified = get_equ_label_or_asis(src_token.AsString()) != src_token.AsString();

		    const std::string disp = (equ_specified || !exists_disp) ?
			 "" : tokenizer.LookAhead(4).AsString() + tokenizer.LookAhead(5).AsString();

		    const std::string src_mem  = exists_disp ? "[" + src_reg + disp + "]" : "[" + src_reg + "]";

		    log()->info("{} <= {}", dst_reg, src_mem);

		    std::smatch match;
		    log()->info("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

		    if (regex_match(dst_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
			 log()->info("32bit reg using & 16bit-mode: Override prefix: 0x67");
			 binout_container.push_back(0x67);
		    }
		    if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
			 log()->info("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
			 binout_container.push_back(0x66);
		    } else if (exists_disp && this->OPENNASK_MODES == ID_16BIT_MODE) {
			 log()->info("32bit operand using & 16bit-mode: Register-size prefix: 0x66");
			 binout_container.push_back(0x66);
		    } else if (regex_match(src_reg, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
			 log()->info("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
			 binout_container.push_back(0x66);
		    } else {
			 log()->info("Register-size prefix is absent");
		    }

		    if (regex_match(dst_reg, match, ModRM::regImm08)) {
			 // r8
			 const uint8_t op = 0x8a;
			 const uint8_t modrm = exists_disp ? ModRM::generate_modrm(0x8a, ModRM::REG_DISP8, src_mem, dst_reg)
			      : ModRM::generate_modrm(0x8a, ModRM::REG_REG, src_mem, dst_reg);
			 log()->info("Opecode: 0x{:02x}, 0x{:02x}", op, modrm);
		    	 binout_container.push_back(op);
		    	 binout_container.push_back(modrm);
		    } else {
			 // r16 or r32
			 const uint8_t op = 0x8b;
			 const uint8_t modrm = exists_disp ? ModRM::generate_modrm(0x8b, ModRM::REG_DISP8, src_mem, dst_reg)
			      : ModRM::generate_modrm(0x8b, ModRM::REG_REG, src_mem, dst_reg);
			 log()->info("Opecode: 0x{:02x}, 0x{:02x}", op, modrm);
		    	 binout_container.push_back(op);
		    	 binout_container.push_back(modrm);
		    }

		    if (disp != "" && ModRM::get_rm_from_reg(src_reg) == ModRM::SIB) {
			 const uint8_t sib = ModRM::generate_sib(exists_disp ? src_mem : src_reg, src_reg);
			 log()->info("SIB: 0x{:02x}", sib);
		    	 binout_container.push_back(sib);
		    } else if (is_hex_notation(src_reg)) {
			 // set memory
			 const std::string hex_imm = src_reg.substr(2);
			 set_hexstring_into_binout(hex_imm, binout_container);
		    }

		    if (tokenizer.LookAhead(4).Is("+")) {
			 const uint8_t disp = tokenizer.LookAhead(5).AsLong();
			 log()->info("Disp: 0x{:02x}", disp);
		    	 binout_container.push_back(disp);
		    }

		    // これで終了のはず
		    break;

	       } else if (is_register(token_table, token) &&
			  tokenizer.LookAhead(1).Is(",")  &&
			  is_segment_register(token_table, tokenizer.LookAhead(2))) {

		    // MOV Reg16,Seg
		    // 10001100 oosssmmm
		    //--------------------------------------------------------
		    // 0x8C /r	MOV r/m16, Sreg		セグメントレジスタをr/m16に転送します
		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(2);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_reg  = src_token.AsString();

		    log()->info("{} <= {}", dst_reg, src_reg);

		    // Reg16, Sregの場合 => oosssmmm
		    const std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
		    const std::string tp_src = ModRM::get_MMMSSS_from_reg(src_reg);

		    // 10001100
		    const std::bitset<8> bs_dst("10001100");
		    // oo + sss + mmm
		    const std::bitset<8> bs_src("11" + std::get<0>(tp_dst) + tp_src);
		    binout_container.push_back(bs_dst.to_ulong());
		    binout_container.push_back(bs_src.to_ulong());

		    log()->info("NIM(W): {}, {}",
				static_cast<int>(bs_dst.to_ulong()),
				static_cast<int>(bs_src.to_ulong()));

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

		    log()->info("{} <= {}", dst_reg, src_reg);

		    // Reg32, CRnの場合 => 11sssrrr
		    std::tuple<std::string, std::string> tp = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

		    // oo + sss + mmm
		    const std::bitset<8> nim("11000" + std::get<0>(tp));
		    binout_container.push_back(0x0f);
		    binout_container.push_back(0x20);
		    binout_container.push_back(nim.to_ulong());

		    log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
				0x0f,
				0x20,
				static_cast<int>(nim.to_ulong()));

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

		    log()->info("{} <= {}", dst_reg, src_reg);

		    std::smatch match;

		    if (regex_match(dst_reg, match, ModRM::regImm08)) {
			 const uint8_t modrm = ModRM::generate_modrm(0x8a, ModRM::REG_REG, src_reg, dst_reg);
			 binout_container.push_back(0x8a);
			 binout_container.push_back(modrm);
			 log()->info("NIM:(B) 0x{:02x}, 0x{:02x}", 0x8a, modrm);
		    } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
			 const uint8_t modrm = ModRM::generate_modrm(0x8b, ModRM::REG_REG, src_reg, dst_reg);
			 binout_container.push_back(0x8b);
			 binout_container.push_back(modrm);
			 log()->info("NIM:(B) 0x{:02x}, 0x{:02x}", 0x8b, modrm);
		    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
			 const uint8_t modrm = ModRM::generate_modrm(0x8b, ModRM::REG_REG, src_reg, dst_reg);
			 binout_container.push_back(0x8b);
			 binout_container.push_back(modrm);
			 log()->info("NIM:(B) 0x{:02x}, 0x{:02x}", 0x8b, modrm);
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
		    const std::string src_imm  = get_equ_label_or_asis(src_token.AsString());
		    log()->info("{} <= {}, represented with label", dst_reg, src_imm);

		    std::smatch match;

		    if (regex_match(dst_reg, match, ModRM::regImm08)) {
			 const uint8_t o = ModRM::get_opecode_from_reg(0xb0, dst_reg);
			 log()->info("NIM:(B) 0x{:02x}, 0x{:02x}", o, 0x00);
			 binout_container.push_back(o);
			 store_label_src(src_imm, binout_container, false, imm8);
			 binout_container.push_back(0x00);

		    } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
			 const uint8_t o = ModRM::get_opecode_from_reg(0xb8, dst_reg);
			 log()->info("NIM:(W) 0x{:02x}, 0x{:02x}, 0x{:02x}", o, 0x00, 0x00);
			 store_label_src(src_imm, binout_container, true, imm16);
			 binout_container.push_back(o);
			 binout_container.push_back(0x00);
			 binout_container.push_back(0x00);

		    } else if (regex_match(dst_reg, match, ModRM::regImm32)) {
			 const uint8_t o = ModRM::get_opecode_from_reg(0xb8, dst_reg);
			 log()->info("NIM:(DW) 0x{:02x}, 0x{:02x}, 0x{:02x}...", 0x66, o, 0x00);
			 binout_container.push_back(0x66);
			 store_label_src(src_imm, binout_container, true, imm32);
			 binout_container.push_back(o);
			 binout_container.push_back(0x00);
			 binout_container.push_back(0x00);
			 binout_container.push_back(0x00);
			 binout_container.push_back(0x00);
		    }
		    break;

	       } else if (is_register(token_table, token) &&
			  tokenizer.LookAhead(1).Is(",")) {
		    //
		    // MOV Reg, Imm | 1011wrrr
		    //--------------------------------------------------------
		    // 0xB0+rb	MOV r8, imm8		imm8をr8に転送します
		    // 0xB8+rw	MOV r16, imm16		imm16をr16に転送します
		    // 0xB8+rd	MOV r32, imm32		imm32をr32に転送します
		    //
		    TParaToken dst_token = token;
		    TParaToken src_token = tokenizer.LookAhead(2);
		    const std::string dst_reg  = dst_token.AsString();
		    const std::string src_imm  = get_equ_label_or_asis(src_token.AsString());
		    log()->info("{} <= {}, with imm", dst_reg, src_imm);

		    NIMONIC_INFO nim_info;
		    set_nimonic_with_register(token.AsString(), &nim_info, tokenizer);

		    // コンマを飛ばして次へ
		    token = tokenizer.Next();
		    token = tokenizer.Next();

		    const uint16_t nim = nim_info.prefix;
		    if (nim_info.imm != offs && nim_info.imm != imm32 ) {
			 if (nim > 0x6600) {
			      const uint8_t first_byte	= nim & 0xff;
			      const uint8_t second_byte = (nim >> 8);
			      binout_container.push_back(second_byte);
			      binout_container.push_back(first_byte);
			      // debug logs
			      log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
					  second_byte,
					  first_byte,
					  token.AsLong());

			 } else {
			      const uint8_t first_byte	= nim & 0xff;
			      const uint8_t second_byte = (nim >> 8);
			      binout_container.push_back(first_byte);
			      // debug logs
			      log()->info("NIM:(B) 0x{:02x}, 0x{:02x}",
					  static_cast<int>(first_byte),
					  (nim_info.imm != offs) ?
					  static_cast<int>(token.AsLong()) : token.AsLong());
			 }
		    }

		    std::smatch match;
		    log()->info("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

		    if (this->OPENNASK_MODES == ID_16BIT_MODE) {
			 log()->info("32bit operand using & 16bit-mode: Register-size prefix: 0x66");
			 binout_container.push_back(0x66);
		    } else {
			 log()->info("Register-size prefix is absent");
		    }

		    // 即値(imm)を設定
		    if (nim_info.imm == imm8) {
			 log()->info("Imm8");
			 binout_container.push_back(token.AsLong());

		    } else if (nim_info.imm == imm16) {
			 log()->info("Imm16");
			 set_word_into_binout(token.AsLong(), binout_container, false);

		    } else if (nim_info.imm == imm32) {
			 binout_container.push_back(nim);
			 const long real_src_imm = is_hex_notation(src_imm) ?
			      std::stoul(src_imm, nullptr, 16) : src_token.AsLong();

			 log()->info("NIM:(B) 0x{:02x}, 0x{:02x}", nim, real_src_imm);
			 set_dword_into_binout(real_src_imm, binout_container, false);

		    } else if (nim_info.imm == offs) {
			 log()->info("Offset processing !");
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
		    log()->info("not matching");
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
			 log()->info("label stored: ", store_label);
			 log()->info("0x73, 0x00");

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
			 log()->info("label stored: ", store_label);
			 log()->info("0x76, 0x00");

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
			 log()->info("label stored: ", store_label);
			 log()->info("0x72, 0x00");

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
			 log()->info("label stored: ", store_label);
			 log()->info("0x72, 0x00");

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
			 log()->info("label stored: ", store_label);
			 log()->info("0x74, 0x00");

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
	       } else if (tokenizer.LookAhead(2).AsString() == ":") {
		    // JMP ptr16:16 | 0xEA cd
		    // JMP ptr16:32 | 0xEA cp
		    std::string data_type = token.AsString();
		    const long ptr16 = tokenizer.LookAhead(1).AsLong();
		    const long addr  = tokenizer.LookAhead(3).AsLong();
		    log()->info("NIM(W): 0x66, 0xea, 0x{:02x}, 0x{:02x}", addr, ptr16);
		    binout_container.push_back(0x66);
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
		    if (is_between_bytesize(token.AsLong()) && imm8 == get_imm_size(token.AsString())) {
			 // 0xEB
			 const long jmp_offset = (token.AsLong() - dollar_position - binout_container.size()) - 2;
			 log()->info("JMP: {}", token.AsString());
			 log()->info("0xeb with Byte {}", jmp_offset);
			 binout_container.push_back(0xeb);
			 binout_container.push_back(jmp_offset);
		    } else {
			 // 0xE9
			 log()->info("JMP: ", token.AsString());
			 const long jmp_offset = (token.AsLong() - dollar_position - binout_container.size()) - 3;
			 if (get_imm_size(token.AsString()) == imm16) {
			      log()->info("0xe9 with Word {}", jmp_offset);
			      binout_container.push_back(0xe9);
			      set_word_into_binout(jmp_offset, binout_container);
			 } else if (get_imm_size(token.AsString()) == imm32) {
			      log()->info("0xe9 with Dword {}", jmp_offset);
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
		    log()->info("label stored: ", store_label);
		    log()->info("0xeb, 0x00");

		    if (dst_is_stored(store_label)) {
			 update_label_src_offset(store_label, binout_container, 0xeb);
		    } else {
			 store_label_src(store_label, binout_container);
			 binout_container.push_back(0xeb);
			 binout_container.push_back(0x00);
			 log()->info("bin[{}] = 0xeb, bin[{}] = 0x00",
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
			 log()->info("label stored: ", store_label);
			 log()->info("0x73, 0x00");

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
			 log()->info("label stored: ", store_label);
			 log()->info("0x75, 0x00");

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
			 log()->info("label stored: ", store_label);
			 log()->info("0x74, 0x00");

			 if (dst_is_stored(store_label)) {
			      update_label_src_offset(store_label, binout_container, 0x74);
			 } else {
			      store_label_src(store_label, binout_container);
			      binout_container.push_back(0x74);
			      binout_container.push_back(0x00);
			      log()->info("bin[{}] = 0x74, bin[{}] = 0x00",
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
		    log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}", 0x0f, 0x01, 0x16);
		    binout_container.push_back(0x0f);
		    binout_container.push_back(0x01);

		    const std::string store_label = tokenizer.LookAhead(1).AsString();

		    if (is_hex_notation(store_label)) {
			 // ラベルではなく即値でジャンプ先を指定された場合
			 log()->info("** Not implemented **");
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

	       } else {
		    std::cerr << "NASK : LGDT syntax error " << token.AsString() << std::endl;
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
			 log()->info("{} + {}", dst_reg, src_reg);

			 std::smatch match;
			 if (regex_match(src_reg, match, ModRM::regImm08)) {
			      const uint8_t op  = 0x00;
			      const uint8_t nim = ModRM::generate_modrm(ModRM::REG_REG, dst_reg);
			      // debug logs
			      log()->info("NIM(W): 0x{:02x}, 0x{:02x}", op, nim);
			      binout_container.push_back(op);
			      binout_container.push_back(nim);
			 } else {
			      const uint8_t pre = 0x66;
			      const uint8_t op  = 0x01;
			      const uint8_t nim = ModRM::generate_modrm(0x01, ModRM::REG, src_reg, dst_reg);
			      if (regex_match(src_reg, match, ModRM::regImm32)) {
				   // see: http://wiki.osdev.org/X86-64_Instruction_Encoding#Legacy_Prefixes
				   log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}", pre, op, nim);
				   binout_container.push_back(pre);
				   binout_container.push_back(op);
				   binout_container.push_back(nim);
			      } else {
				   // debug logs
				   log()->info("NIM(W): 0x{:02x}, 0x{:02x}", op, nim);
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
			 log()->info("{} + {}", dst_reg, src_imm);
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);
			 // Imm8 or Imm16
			 const size_t imm_size = get_imm_size(src_token.AsString());
			 log()->info("imm size: ", imm_size);

			 if (ModRM::is_accumulator(dst_reg)) {
			      // ADD Acc,Imm
			      // NIM: 0000010w
			      const std::bitset<8> bs_dst("0000010" + std::get<1>(tp_dst));
			      // debug logs
			      log()->info("NIM(W): 0x{:02x}, 0x{:02x}", bs_dst.to_ulong(), src_token.AsLong());
			      binout_container.push_back(bs_dst.to_ulong());

			      if (imm_size == imm8) {
				   log()->info("imm8 ! => ", src_token.AsLong());
				   binout_container.push_back(src_token.AsLong());
			      } else {
				   log()->info("imm16 ! => ", src_token.AsLong());
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
				   log()->info("r/m8: ", dst_reg);
				   //const std::bitset<8> bs_dst1("1000001" + std::get<1>(tp_dst));
				   const std::bitset<8> bs_dst2("11000" + std::get<0>(tp_dst));

				   // debug logs
				   log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
					       0x80,
					       bs_dst2.to_ulong(),
					       src_token.AsLong());

				   binout_container.push_back(0x80);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_token.AsLong());

			      } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
				   const uint8_t op = is_imm8(src_token.AsString()) ? 0x83 : 0x81;
				   // ADD Reg16, Imm
				   // -------------
				   // 0x83 /0 ib  ADD r/m16, imm8
				   // 0x81 /0 iw  ADD r/m16, imm16
				   log()->info("r/m16: ", dst_reg);
				   const std::bitset<8> bs_dst2("11000" + std::get<0>(tp_dst));

				   // debug logs
				   log()->info("NIM(W): 0x{:02x}, 0x{:02x}, 0x{:02x}",
					       op, bs_dst2.to_ulong(), src_token.AsLong());

				   binout_container.push_back(op);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_token.AsLong());

			      } else {
				   const uint8_t op = is_imm8(src_token.AsString()) ? 0x83 : 0x81;
				   // ADD Reg32, Imm
				   // -------------
				   // 0x83 /0 ib  ADD r/m32, imm8
				   // 0x81 /0 id  ADD r/m32, imm32
				   log()->info("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");
				   log()->info("r/m32: ", dst_reg);
				   const std::bitset<8> bs_dst2("11000" + std::get<0>(tp_dst));

				   // debug logs
				   log()->info("NIM(W): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}",
					       op, bs_dst2.to_ulong(), src_token.AsLong());

				   binout_container.push_back(0x66);
				   binout_container.push_back(op);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_token.AsLong());

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
	       log()->info("ALIGNB {}", token.AsString());
	       const size_t unit = static_cast<int>(token.AsLong());
	       const size_t nearest_size = binout_container.size() / unit + 1;
	       const size_t times = nearest_size * unit - binout_container.size();

	       log()->info("ALIGNB stores 0x00 {} times", times);
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
			      log()->info("0x24 {}", tokenizer.LookAhead(2).AsString());
			      binout_container.push_back(0x24);
			      binout_container.push_back(tokenizer.LookAhead(2).AsLong());
			 } else if (token.Is("AX")) {
			      log()->info("0x25 {}", tokenizer.LookAhead(2).AsString());
			      binout_container.push_back(0x25);
			      set_word_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
			 } else { // EAX
			      log()->info("0x66 0x25 {}", tokenizer.LookAhead(2).AsString());
			      binout_container.push_back(0x66);
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
	       } else if (is_hex_notation(token.AsString())) {
		    // ラベルではなく即値でジャンプ先を指定された場合
		    log()->info("** Not implemented **");
	       } else {
		    const std::string store_label = token.AsString();
		    log()->info("label stored: ", store_label);
		    log()->info("0xe8, 0x0000");

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
			 log()->info("{} == {}", dst_reg, src_imm);
			 std::tuple<std::string, std::string> tp_dst = ModRM::REGISTERS_RRR_MAP.at(dst_reg);

			 std::smatch match;
			 if (regex_match(dst_reg, match, ModRM::rm000)) {
			      // AL|AX|EAX なので "CMP Acc,Imm" に決定
			      // CMP Acc,Imm
			      // 0011110 + w
			      const std::bitset<8> bs_dst("0011110" + std::get<1>(tp_dst));

			      // debug logs
			      log()->info("NIM(B): {}, {}",
					  static_cast<int>(bs_dst.to_ulong()),
					  static_cast<int>(src_imm));

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
				   log()->info("r/m8: ", dst_reg);
				   //const std::bitset<8> bs_dst1("1000001" + std::get<1>(tp_dst));
				   const std::bitset<8> bs_dst2("11111" + std::get<0>(tp_dst));

				   // debug logs
				   log()->info("NIM(W): {}, {}, {}",
					       static_cast<int>(0x80),
					       static_cast<int>(bs_dst2.to_ulong()),
					       static_cast<int>(src_imm));

				   binout_container.push_back(0x80);
				   binout_container.push_back(bs_dst2.to_ulong());
				   binout_container.push_back(src_imm);

			      } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
				   const uint8_t op = is_between_bytesize(src_imm) ? 0x83 : 0x81;
				   // CMP Reg16, Imm
				   // ------------
				   // 0x83 /7 ib | CMP r/m16, imm8
				   // 0x81 /7 iw | CMP r/m16, imm16
				   log()->info("r/m16: ", dst_reg);
				   //const std::bitset<8> bs_dst1("1000000" + std::get<1>(tp_dst));
				   const std::bitset<8> bs_dst2("11111" + std::get<0>(tp_dst));

				   // debug logs
				   log()->info("NIM(W): {}, {}, {}",
					       static_cast<int>(op),
					       static_cast<int>(bs_dst2.to_ulong()),
					       static_cast<int>(src_imm));

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
	       } else if ( !is_legitimate_numeric(token.AsString()) && !token.IsEmpty() ) {
		    // DW
		    const std::string store_label = token.AsString();
		    const long abs_size = get_label_src_offset(store_label);
		    log()->info("DW found rel: {}", store_label);
		    log()->info("DW absolute size: {}", abs_size);
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
			      log()->info("{} is keeped as {} because of EQU", key, val);
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

     int Instructions::process_token_GLOBAL(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
		    break;
	       } else if (token.Is("GLOBAL")) {
		    continue;
	       } else {
		    if (!token.IsEmpty()) {
			 log()->info("Add new symbol: {}", token.AsString());
			 symbol_list.push_back(token.AsString());

			 for ( size_t i = 2; ; i+=2) {
			      if ( !tokenizer.LookAhead(i).IsEmpty() && tokenizer.LookAhead(i-1).Is(",")) {
				   log()->info("Add new symbol: {}", tokenizer.LookAhead(i).AsString());
				   symbol_list.push_back(tokenizer.LookAhead(i).AsString());
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
			      log()->info("NIM(B): 0x{:02x}, 0x{:02x}, 0x{:02x}", nim, bs_dst.to_ulong(), src_token.AsLong());
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
			      log()->info("NIM(W): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}", nim, bs_dst.to_ulong(), src_token.AsLong());
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

			 std::smatch match;
			 log()->info("CPU Mode: {}", this->OPENNASK_MODES == ID_16BIT_MODE ? "16bit" : "32bit");

			 const std::string src_reg = token.AsString();

			 if (regex_match(src_reg, match, ModRM::regImm32) && this->OPENNASK_MODES == ID_16BIT_MODE) {
			      log()->info("32bit reg using & 16bit-mode: Register-size prefix: 0x66");
			      binout_container.push_back(0x66);
			 } else if (regex_match(src_reg, match, ModRM::regImm16) && this->OPENNASK_MODES == ID_32BIT_MODE) {
			      log()->info("16bit reg using & 32bit-mode: Register-size prefix: 0x66");
			      binout_container.push_back(0x66);
			 } else {
			      log()->info("Register-size prefix is absent");
			 }

			 if (tokenizer.LookAhead(2).Is("DX")) {
			      const uint8_t nim = token.Is("AL") ? 0xec : 0xed;
			      log()->info("NIM(B): 0x{:02x}", nim);
			      binout_container.push_back(nim);
			 } else {
			      const uint8_t imm = tokenizer.LookAhead(2).AsLong();
			      const uint8_t nim = token.Is("AL") ? 0xe4 : 0xe5;
			      log()->info("NIM(B): 0x{:02x}, 0x{:02x}", nim, imm);
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
		    log()->info("DD found rel: {}", store_label);
		    log()->info("DD absolute size: {}", abs_size);
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
			      log()->info("NIM(B): 0x66, 0x80, 0x{:02x}, 0x{:02x}",
					  bs_dst.to_ulong(), src_token.AsLong());

			      binout_container.push_back(0x66);
			      binout_container.push_back(0x80);
			      binout_container.push_back(bs_dst.to_ulong());
			      binout_container.push_back(src_token.AsLong());

			 } else if (regex_match(dst_reg, match, ModRM::regImm16)) {
			      // 0x81 /1 iw | OR r/m16, imm16
			      // 0x83 /1 ib | OR r/m16, imm8
			      const uint8_t nim = is_between_bytesize(src_token.AsLong()) ? 0x83 : 0x81;
			      log()->info("NIM(B/W): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}",
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
			      log()->info("NIM(B/W): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}",
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
			      log()->info("0x66 0x0c {}", tokenizer.LookAhead(2).AsString());
			      binout_container.push_back(0x66);
			      binout_container.push_back(0x0c);
			      binout_container.push_back(tokenizer.LookAhead(2).AsLong());
			 } else if (token.Is("AX")) {
			      log()->info("0x66 0x0d {}", tokenizer.LookAhead(2).AsString());
			      binout_container.push_back(0x66);
			      binout_container.push_back(0x0d);
			      set_word_into_binout(tokenizer.LookAhead(2).AsLong(), binout_container);
			 } else { // EAX
			      log()->info("0x66 0x0d {}", tokenizer.LookAhead(2).AsString());
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
		    log()->info(token.AsString());

		    if (is_legitimate_numeric(token.AsString())) {
			 dollar_position = token.AsLong();
			 log()->info("ORG = ", dollar_position);
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
			 log()->info("{} <= {}", dst_imm, src_reg);

			 const uint8_t opecode = (src_reg == "AL") ? 0xe6 : 0xe7;
			 binout_container.push_back(opecode);
			 binout_container.push_back(dst_token.AsLong());

			 log()->info("NIM(W): 0x{:02x}, 0x{:02x}", opecode, dst_token.AsLong());
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
			 log()->info("{} <= {}", dst_reg, src_reg);

			 const uint8_t opecode = (src_reg == "AL") ? 0xee : 0xef;
			 binout_container.push_back(opecode);
			 log()->info("NIM(B): 0x{:02x}", opecode);
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

		    if (is_register(token_table, token)) {
			 std::smatch match;
			 TParaToken dst_token = token;
			 const std::string dst_reg  = dst_token.AsString();

			 if (regex_match(dst_reg, match, ModRM::regImm16)) {

			      // 0x58+rw | POP r16 <-- AX, BX, CX, DX
			      log()->info("POP from {}", dst_reg);
			      const uint8_t opecode = get_plus_register_code((uint8_t) 0x58, dst_reg.at(0));
			      log()->info("NIM(B): 0x{:02x}", opecode);
			      binout_container.push_back(opecode);

			 } else if (regex_match(dst_reg, match, ModRM::regImm32)) {

			      // 0x58+rd | POP r32 <-- EAX, EBX, ECX, EDX
			      log()->info("POP from {}", dst_reg);
			      const uint8_t opecode = get_plus_register_code((uint8_t) 0x58, dst_reg.at(1));
			      log()->info("NIM(B): 0x{:02x}", opecode);
			      binout_container.push_back(opecode);

			 } else {
			      std::cerr << "NASK : POP specified incorrect register" << std::endl;
			      return 17;
			 }
			 break;

		    } else if (is_segment_register(token_table, token)) {
			 // 0x1F      | POP DS
			 // 0x07      | POP ES
			 // 0x17      | POP SS
			 // 0x0F 0xA1 | POP FS
			 // 0x0F 0xA9 | POP GS
			 // ないよ！  | POP CS
			 TParaToken dst_token = token;
			 const std::string dst_reg  = dst_token.AsString();
			 log()->info("POP from {}", dst_reg);

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
			      log()->info("NIM(B): 0x{:02x}", opecode_b);
			 } else {
			      set_word_into_binout(opecode_w, binout_container);
			      log()->info("NIM(W): 0x{:02x}", opecode_w);
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

     // テキトーなRET命令の実装
     int Instructions::process_token_RET(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {
	  // 0xC3 を格納
	  binout_container.push_back(0xc3);
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
			 log()->info("{} / 2 {} times", dst_reg, src);

			 std::smatch match;
			 if (regex_match(dst_reg, match, ModRM::regImm08)) {
			      // 0xD0 /5     SHR r/m8        r/m8を2で1回符号なし除算します※
			      // 0xD2 /5     SHR r/m8, CL    r/m8を2でCL回符号なし除算します※
			      // 0xC0 /5 ib  SHR r/m8, imm8  r/m8を2でimm8回符号なし除算します※
			      const uint8_t op  = src_token.IsEmpty() ? 0xd0 : ( src == "CL" ? 0xd2 : 0xc0 );
			      const uint8_t nim = ModRM::generate_modrm(ModRM::REG, dst_reg, ModRM::SLASH_5);

			      // debug logs
			      log()->info("NIM(W): 0x{:02x}, 0x{:02x}, {}", op, nim, src_token.AsString());

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
			      log()->info("NIM(W): 0x{:02x}, 0x{:02x}, {}", op, nim, src_token.AsString());

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
			      log()->info("NIM(W): 0x66, 0x{:02x}, 0x{:02x}, {}", op, nim, src_token.AsString());

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
			 log()->info("NIM(B): 0x{:02x}, 0x{:02x}", nim, imm);
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
			      log()->info("NIM(B): 0x80, 0x{:02x}, 0x{:02x}", nim2, tokenizer.LookAhead(2).AsLong());
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
			      log()->info("NIM(B): 0x{:02x}, 0x{:02x}, 0x{:02x}", nim1, nim2, tokenizer.LookAhead(2).AsLong());
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
			      log()->info("NIM(B): 0x66, 0x{:02x}, 0x{:02x}, 0x{:02x}", nim1, nim2, tokenizer.LookAhead(2).AsLong());
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
}
